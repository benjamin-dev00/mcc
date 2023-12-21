//===================================================================
//  CODBC.C      Core Database interface implememted via ODBC on
//				 Microsoft Access Driver 
//
//Zheng zhikai  98-10-28
//===================================================================

#include "prehead.h"
#include "coreall.h"

#include "codbc.h"

#include <dos.h>
#include <time.h>
#include <sqlext.h>
#include <odbcinst.h>
#include <math.h>

/*********************************************************************
History database structure:
	Table:
		hisreal
			id:		indexed, integer,      index i.e. WRD, of RTUs
			dt:		indexed, long integer, logging date and time 
			val:	         short float,  logging value 

		histint
			id:		indexed, integer,      index i.e. WRD, of TTUs
			dt:		indexed, long integer, logging date and time 
			val:	         long integer, logging value 

		hisbool
			id:		indexed, integer,      index i.e. WRD, of TTUs
			dt:		indexed, long integer, logging date and time 
			val:	         boolean,      logging value 

*********************************************************************/
extern RTInfo       gRTInfo;
extern TTInfo       gTTInfo;
extern BTInfo       gBTInfo;
extern HINSTANCE	ghInst;

static BOOL CreateDbAndAddDsn(LPCSTR lpszDBName_,LPCSTR lpszDSN_);

struct _tagODBC{
	HENV    henv;
	HDBC    hdbc;
	HSTMT   hstmt;
}   gDbi;

const char	gszDataSource[] = "CoreHisData";

const char  gszHisReal[] = "hisreal";
const char  gszHisTint[] = "histint";
const char  gszHisBool[] = "hisbool";


extern CoreInfo  gCoreInfo;

#define CHECK_ERROR             \
if( _ret != SQL_SUCCESS )       \
{                               \
	GetDBIError();              \
	FreeDBI();                  \
	return FALSE;               \
}

//...................................................................
//Entries:
//
//	lpszDBName_		a full file name which to be created, note: not
//					include file extension
//...................................................................
BOOL CreateDatabase_ODBC( LPCSTR lpszDBName_ )
{
	char	_szDBName[MAXPATH];
	TCHAR	_szTemplate[MAXPATH];
	TCHAR	_szModuleName[MAXPATH];
	LPTSTR	_pszFileName;

	//We don't create a database directly, 
	//
	//sprintf( _szAttributesCreateMDB, "CREATE_DB=%s General\0", _szDBName);

	strcpy( _szDBName, lpszDBName_ );
	strcat( _szDBName, ".mdb" );

	//Create a new database by copying it from the database template

    GetModuleFileName( ghInst, _szModuleName, sizeof(_szModuleName) );
	GetFullPathName( _szModuleName, sizeof(_szTemplate), _szTemplate, &_pszFileName );
	strcpy( _pszFileName, "Template.mdb" );

	return CopyFile( _szTemplate, _szDBName, FALSE );
}

//...................................................................
//Entries:
//
//	lpszDBName_		a full file name which to be created, note: not
//					include file extension
//
//  lpszDSN_		a DSN name string
//...................................................................
BOOL ConnectDatabase_ODBC( LPCSTR lpszDBName_, LPCSTR lpszDSN_ )
{
	char	_szDBName[MAXPATH];
	char	*_pointer;
	int		_ch='!';	//just used for char substitute 
	
	char _szAttributes[80+MAXPATH];
	char _szDriveName[]="Microsoft Access Driver (*.mdb)";

	strcpy( _szDBName, lpszDBName_ );
	strcat( _szDBName, ".mdb" );

	sprintf( _szAttributes, 
			 "DSN=%s!DriverId=25!DBQ=%s!", 
			 lpszDSN_,
			 _szDBName );
	
	_pointer= strchr( _szAttributes, _ch );
	
	while(_pointer!=NULL)
	{
		*_pointer='\0';
		_pointer=_pointer+1;
		_pointer= strchr(_pointer, _ch );
	}

	if( SQLConfigDataSource(NULL, ODBC_ADD_DSN, _szDriveName, _szAttributes) == FALSE )
		return FALSE;
	else
		return TRUE;
}

//...................................................................
//Entries:
//	lpszDBName_		a full file name which to be created
//...................................................................
BOOL InitialDBI_ODBC( LPCSTR lpszDBName_ )
{
	RETCODE     _ret;

	//initialize the struct
	gDbi.henv   = SQL_NULL_HENV;
	gDbi.hdbc   = SQL_NULL_HDBC;
	gDbi.hstmt  = SQL_NULL_HSTMT;

	if( !ConnectDatabase_ODBC( lpszDBName_, gszDataSource ) )
		return FALSE;

	_ret = SQLAllocEnv( &(gDbi.henv) );
	CHECK_ERROR

	_ret = SQLAllocConnect( gDbi.henv, &(gDbi.hdbc) );
	CHECK_ERROR

	_ret = SQLConnect(  gDbi.hdbc,
						(UCHAR FAR*)gszDataSource,
						SQL_NTS,
						(UCHAR FAR*)"Admin",
						SQL_NTS,
						(UCHAR FAR*)"",
						SQL_NTS);
	CHECK_ERROR

	_ret = SQLAllocStmt( gDbi.hdbc, &(gDbi.hstmt) );
	CHECK_ERROR

	return TRUE;
}

//...................................................................
//...................................................................
BOOL FreeDBI_ODBC()
{
	if( gDbi.hstmt != SQL_NULL_HSTMT )
	{
		SQLFreeStmt( gDbi.hstmt, SQL_DROP );
		gDbi.hstmt = SQL_NULL_HSTMT;
	}

	if( gDbi.hdbc != SQL_NULL_HDBC )
	{
		SQLDisconnect( gDbi.hdbc );
		SQLFreeConnect( gDbi.hdbc );
		gDbi.hdbc = SQL_NULL_HSTMT;
	}

	if( gDbi.henv != SQL_NULL_HENV )
	{
		SQLFreeEnv( gDbi.henv );
		gDbi.henv = SQL_NULL_HENV;
	}

	return TRUE;
}

//...................................................................
//...................................................................
void GetDBIError_ODBC()
{
	UCHAR   _szSqlState[6];
	UCHAR   _szErrorMsg[SQL_MAX_MESSAGE_LENGTH];
	SDWORD  _dwNativeError;
	SWORD   _wErrorMsg;

	SQLError(   gDbi.henv,
				gDbi.hdbc,
				gDbi.hstmt,
				_szSqlState,
				&_dwNativeError,
				_szErrorMsg,
				SQL_MAX_MESSAGE_LENGTH - 1,
				&_wErrorMsg);

	LOGERR0( _szErrorMsg );

	return ;
}

//...................................................................
//Description:
//Entries:
//  iMode_      == 0    log initial state of core,
//                      use this value when core initiate;
//              == 1    log run-time state of core,
//                      use this value when core working;
//              == -1   log stop state of core,
//                      use this value when core stop running;
//Note:
//  The format of DATE/TIME transfered to database is the DATE/TIME format
//  of Microsoft Access Ver2.0.
//  The inner value of DATE/TIME is a double value which is the number
//  of days from 1960-01-01, for example, 1970-01-01 00:00:00 is 25569.0;
//...................................................................
BOOL LogCore_ODBC( int iMode_ )
{
	RETCODE             _ret;
	char                _db_buffer[128];    //for database
	WORD                _i;
	LPRTU               _lpRTU;
	REAL                _delta;

	CORETIME			_tt;

	_tt = CoreTimeNow();
	if( iMode_ == 0 )
		gCoreInfo.LastLogTime = _tt;
	else if( iMode_ == -1 )
		gCoreInfo.LastLogTime = _tt;
	else
	{
		//the two times log interval must be great than 1 second
		if( gCoreInfo.LastLogTime == _tt )
			return TRUE;

		//update the log time
		gCoreInfo.LastLogTime = _tt;
	}

	//log real table
	for( _i=1; _i<gRTInfo.wAdding; _i++ )
	{
		_lpRTU = gRTInfo.lpBase + _i;
		if(  (_lpRTU->Status & RTUBIT_VALID)
		   &&(_lpRTU->Status & RTUBIT_LOGHIS) )
		{
			if( iMode_ == 0 )   //initial state of the core
			{
				sprintf( _db_buffer,
						 "INSERT INTO hisreal VALUES(%d, %ld, NULL)",
						 _i,
						 _tt - 1 );

				_ret = SQLExecDirect( gDbi.hstmt,
									  (UCHAR FAR*)_db_buffer,
									  SQL_NTS );
				CHECK_ERROR
			}

			_delta = _lpRTU->LogBand;

			if(  ( iMode_ != 1 )
			   ||( fabs(_lpRTU->Value - _lpRTU->LastLog) > _delta ) )
			{
				sprintf( _db_buffer,
						 "INSERT INTO hisreal VALUES(%d, %ld, %e)",
						 _i,
						 _tt,
						 _lpRTU->Value );

				_ret = SQLExecDirect( gDbi.hstmt,
									  (UCHAR FAR*)_db_buffer,
									  SQL_NTS );

				_lpRTU->LastLog = _lpRTU->Value;
				CHECK_ERROR
			}

			//end state of the core
			if( iMode_ == -1 )
			{
				sprintf( _db_buffer,
						 "INSERT INTO hisreal VALUES(%d, %ld, NULL)",
						 _i,
						 _tt + 1 );

				_ret = SQLExecDirect( gDbi.hstmt,
									  (UCHAR FAR*)_db_buffer,
									  SQL_NTS );
				CHECK_ERROR
			}
		}
	}

	//log tint table
	//log bool table

	return TRUE;
}

//...................................................................
//Description:  Temp. function
//  Test history logging
//
//...................................................................
_DLLEXP BOOL _DLLFUNC TestLogging( WORD wLoopTimes_ )
{
	char                _dt_buffer[32];     //for date-time
	char                _db_buffer[128];    //for database
	WORD                _i;

/****
#ifdef _WIN32
	SYSTEMTIME  _st;
	GetLocalTime(&_st);
	sprintf(_dt_buffer,
			"\"%4d-%2d-%2d %2d:%2d:%2d\"",
			_st.wYear,
			_st.wMonth,
			_st.wDay,
			_st.wHour,
			_st.wMinute,
			_st.wSecond );

#else
	struct dosdate_t    _date;
	struct dostime_t    _time;

	_dos_gettime( &_time );
	_dos_getdate( &_date );
	sprintf(_dt_buffer,
			"\"%4d-%2d-%2d %2d:%2d:%2d\"",
			_date.year,
			_date.month,
			_date.day,
			_time.hour,
			_time.minute,
			_time.second );
#endif
****/

	for( _i=1; _i<wLoopTimes_; _i++ )
	{
		sprintf( _db_buffer,
			 "INSERT INTO hisreal VALUES(100, 100, NULL)" );

		SQLExecDirect( gDbi.hstmt,
					   (UCHAR FAR*)_db_buffer,
					   SQL_NTS );
	}

	return FALSE;
}

static LPCACHEHEAD _near QueryDataReal( LPCACHEHEAD lpch_,
										WORD wrd_,
										CORETIME begin_time_,
										CORETIME end_time_ );

typedef struct _tag_SQL_RESULT_REAL{
	long                dt;
	SDWORD              cbdt;
	float               val;
	SDWORD              cbval;
}SQL_RESULT_REAL;

typedef struct _tag_SQL_RESULT_TINT{
	long                dt;
	SDWORD              cbdt;
	long                val;
	SDWORD              cbval;
}SQL_RESULT_TINT;

typedef struct _tag_SQL_RESULT_BOOL{
	long                dt;
	SDWORD              cbdt;
	int                 val;
	SDWORD              cbval;
}SQL_RESULT_BOOL;

static long _near QueryResultRows( LPCSTR lpszTableName_,
									WORD wrd_,
									CORETIME begin_time_,
									CORETIME end_time_ );
static int _near _QueryDataAfterReal(  SQL_RESULT_REAL FAR* lpset_,
									   WRD wrd_,
									   CORETIME end_time_ );
static int _near _QueryDataBeforeReal( SQL_RESULT_REAL FAR* lpset_,
									   WRD wrd_,
									   CORETIME begin_time_ );
void _PackDataReal( LPCACHECELL lpcc_, SQL_RESULT_REAL FAR* lprset_ );
BOOL _CommitDataReal( LPCACHEHEAD lpch_, LPCACHECELL lp_, int option_ );
void _FetachDataReal( HSTMT hstmt_, LPCACHEHEAD lpch_ );
void _FetachDataPackReal( HSTMT hstmt_, LPCACHEHEAD lpch_ );

//...................................................................
//...................................................................
LPCACHEHEAD QueryHisData_ODBC( LPCACHEHEAD lpch_,
								WORD table_,
								WORD wrd_,
								CORETIME begin_time_,
								CORETIME end_time_ )
{
	if( table_ == TBX_RT )
		return QueryDataReal( lpch_, wrd_, begin_time_, end_time_ );
	/*
	if( table_ == TBX_TT )
		return QueryDataReal( wrd_, begin_time_, end_time_, max_items );
	if( table_ == TBX_BT )
		return QueryDataReal( wrd_, begin_time_, end_time_, max_items );
	*/

	lpch_->num_cc = 0;
	lpch_->pack = FALSE;
	lpch_->begin_time = begin_time_;
	lpch_->end_time = end_time_;
	lpch_->table = table_;
	lpch_->wrd = wrd_;

	return lpch_;
}

//...................................................................
//...................................................................
static long _near QueryResultRows( LPCSTR lpszTableName_,
									WORD wrd_,
									CORETIME begin_time_,
									CORETIME end_time_ )
{
	HSTMT           _hstmt = SQL_NULL_HSTMT;
	char            _szBuffer[256];
	long            _count = 0;
	SDWORD          _cb;
	RETCODE         _ret;

	_ret = SQLAllocStmt( gDbi.hdbc, &_hstmt );
	if( _ret != SQL_SUCCESS )
		goto _RETURN;

	sprintf( _szBuffer,
			 "SELECT count(*) FROM %s WHERE ( id = %d ) AND ( dt BETWEEN %ld AND %ld )",
			 lpszTableName_,
			 wrd_,
			 begin_time_,
			 end_time_ );

	SQLBindCol( _hstmt, 1, SQL_C_LONG, &_count, sizeof(_count), &_cb );

	_ret = SQLExecDirect( _hstmt,
						  (UCHAR FAR*)_szBuffer,
						  SQL_NTS );
	if( _ret != SQL_SUCCESS )
		goto _RETURN;

	_ret = SQLFetch( _hstmt );
	if( _ret != SQL_SUCCESS )
		goto _RETURN;

_RETURN:
	if( _hstmt != SQL_NULL_HSTMT )
		SQLFreeStmt( _hstmt, SQL_DROP );

	return _count;
}

//...................................................................
//...................................................................
static LPCACHEHEAD _near QueryDataReal( LPCACHEHEAD lpch_,
										WORD wrd_,
										CORETIME begin_time_,
										CORETIME end_time_ )
{
	HSTMT           _hstmt = SQL_NULL_HSTMT;
	char            _szBuffer[256];
	RETCODE         _ret;
	SDWORD          _nr0, _nr1, _nr;
	BOOL            _pack = FALSE;
	LPCACHEHEAD     _lpch = lpch_;
	SQL_RESULT_REAL _setBefore;
	SQL_RESULT_REAL _setAfter;

	if( !_lpch )
		return _lpch;

	CORE_ASSERT( _lpch->max_cc == CACHE_MAX_ITEMS );

	_ret = SQLAllocStmt( gDbi.hdbc, &_hstmt );
	if( _ret != SQL_SUCCESS )
		goto _RETURN;

	SQLSetStmtOption( _hstmt, SQL_BIND_TYPE, sizeof( SQL_RESULT_REAL ) );
	SQLSetStmtOption( _hstmt, SQL_ROWSET_SIZE, 1 );

	sprintf( _szBuffer,
			 "SELECT dt, val FROM %s WHERE ( id = %d ) AND ( dt BETWEEN %ld AND %ld ) ORDER BY dt ",
			 gszHisReal,
			 wrd_,
			 begin_time_,
			 end_time_ );

	_ret = SQLExecDirect( _hstmt,
						  (UCHAR FAR*)_szBuffer,
						  SQL_NTS );
	if( _ret != SQL_SUCCESS )
		goto _RETURN;

	_nr0 = _QueryDataBeforeReal( &_setBefore, wrd_, begin_time_ );
	_nr1 = _QueryDataAfterReal( &_setAfter, wrd_, end_time_ );

	_nr = QueryResultRows( gszHisReal, wrd_, begin_time_, end_time_ );
	_nr += _nr0 + _nr1;
	if( _nr <= _lpch->max_cc )
		_pack = FALSE;
	else
		_pack = TRUE;

	_lpch->num_cc = 0;
	_lpch->pack = _pack;
	_lpch->begin_time = begin_time_;
	_lpch->end_time = end_time_;
	_lpch->table = TBX_RT;
	_lpch->wrd = wrd_;

	//commit the before-data to cache
	if( _setBefore.cbval == SQL_NULL_DATA )
	{   //this is a mark to identify NULL data
		_setBefore.dt = -1 * _setBefore.dt;
		_setBefore.val = 0;
	}

	if( _lpch->num_cc < _lpch->max_cc )
	{
		if( _nr0 )
		{
			LPCACHECELL     _lpcc = &(_lpch->cc) + (int)lpch_->num_cc;

			_lpcc->time = _setBefore.dt;
			_lpcc->val = _setBefore.val;
			_lpch->num_cc ++;
		}
	}

	_FetachDataReal( _hstmt, _lpch );

	//commit the after-data to cache
	if( _setAfter.cbval == SQL_NULL_DATA )
	{   //this is a mark to identify NULL data
		_setAfter.dt *= -1;
		_setAfter.val = 0;
	}

	if( _lpch->num_cc < _lpch->max_cc )
	{
		if( _nr1 )
		{
			LPCACHECELL     _lpcc = &(_lpch->cc) + (int)lpch_->num_cc;
			_lpcc->time = _setAfter.dt;
			_lpcc->val = _setAfter.val;
			_lpch->num_cc ++;
		}
	}

_RETURN:
	SQLFreeStmt( _hstmt, SQL_DROP );

	return  _lpch;
}

static int _near _QueryDataBeforeReal( SQL_RESULT_REAL FAR* lpset_,
									   WRD wrd_,
									   CORETIME begin_time_ )
{
	RETCODE     _ret;
	int         _result = 0;
	HSTMT       _hstmt = SQL_NULL_HSTMT;
	char        _szBuffer[256];

	_ret = SQLAllocStmt( gDbi.hdbc, &_hstmt );
	if( _ret != SQL_SUCCESS )
		goto _RETURN;

	SQLSetStmtOption( _hstmt, SQL_BIND_TYPE, sizeof( SQL_RESULT_REAL ) );
	SQLSetStmtOption( _hstmt, SQL_ROWSET_SIZE, 1 );

	//step
	sprintf( _szBuffer,
			 "SELECT dt, val FROM %s "
			 "WHERE dt= "
			 "(SELECT MAX(dt) FROM %s "
			 "WHERE ( id = %d ) AND (dt < %ld) ) ",
			 gszHisReal,
			 gszHisReal,
			 wrd_,
			 begin_time_ );

	_ret = SQLExecDirect( _hstmt,
						  (UCHAR FAR*)_szBuffer,
						  SQL_NTS );
	if( _ret != SQL_SUCCESS )
		goto _RETURN;

	SQLBindCol( _hstmt, 1, SQL_C_LONG, &(lpset_->dt), sizeof(lpset_->dt), &(lpset_->cbdt) );
	SQLBindCol( _hstmt, 2, SQL_C_FLOAT, &(lpset_->val), sizeof(lpset_->val), &(lpset_->cbval) );

	_ret = SQLFetch( _hstmt );
	if( _ret != SQL_SUCCESS )
		goto _RETURN;   //no data or other errors occurred
	else
		_result = 1;

_RETURN:
	if( _hstmt != SQL_NULL_HSTMT )
		SQLFreeStmt( _hstmt, SQL_DROP );
	return _result;
}

static int _near _QueryDataAfterReal(  SQL_RESULT_REAL FAR* lpset_,
									   WRD wrd_,
									   CORETIME end_time_ )
{
	RETCODE     _ret;
	int         _result = 0;
	HSTMT       _hstmt = SQL_NULL_HSTMT;
	char        _szBuffer[256];

	_ret = SQLAllocStmt( gDbi.hdbc, &_hstmt );
	if( _ret != SQL_SUCCESS )
		goto _RETURN;

	SQLSetStmtOption( _hstmt, SQL_BIND_TYPE, sizeof( SQL_RESULT_REAL ) );
	SQLSetStmtOption( _hstmt, SQL_ROWSET_SIZE, 1 );

	sprintf( _szBuffer,
			 "SELECT dt, val FROM %s "
			 "WHERE dt= "
			 "(SELECT MAX(dt) FROM %s "
			 "WHERE ( id = %d ) AND (dt > %ld) ) ",
			 gszHisReal,
			 gszHisReal,
			 wrd_,
			 end_time_ );

	_ret = SQLExecDirect( _hstmt,
						  (UCHAR FAR*)_szBuffer,
						  SQL_NTS );
	if( _ret != SQL_SUCCESS )
		goto _RETURN;

	SQLBindCol( _hstmt, 1, SQL_C_LONG, &(lpset_->dt), sizeof(lpset_->dt), &(lpset_->cbdt) );
	SQLBindCol( _hstmt, 2, SQL_C_FLOAT, &(lpset_->val), sizeof(lpset_->val), &(lpset_->cbval) );

	_ret = SQLFetch( _hstmt );
	if( _ret != SQL_SUCCESS )
		goto _RETURN;   //no data or other errors occurred
	else
		_result = 1;

_RETURN:
	if( _hstmt != SQL_NULL_HSTMT )
		SQLFreeStmt( _hstmt, SQL_DROP );
	return _result;
}

void _FetachDataReal( HSTMT hstmt_, LPCACHEHEAD lpch_ )
{
	RETCODE         _ret;
	SQL_RESULT_REAL _rset;
	LPCACHECELL     _lpcc;

	if( lpch_->pack )
	{
		_FetachDataPackReal( hstmt_, lpch_ );
		return;
	}

	SQLBindCol( hstmt_, 1, SQL_C_LONG, &(_rset.dt), sizeof(_rset.dt), &(_rset.cbdt) );
	SQLBindCol( hstmt_, 2, SQL_C_FLOAT, &(_rset.val), sizeof(_rset.val), &(_rset.cbval) );

	_lpcc = &(lpch_->cc) + (int)lpch_->num_cc;

	while( 1 )
	{
		if( lpch_->num_cc >= lpch_->max_cc )
			break;  //cache is full

		_ret = SQLFetch( hstmt_ );
		if( _ret != SQL_SUCCESS )
		{
			//no data or other errors occurred
			break;
		}

		if( _rset.cbval == SQL_NULL_DATA )
		{   //this is a mark to identify NULL data
			_rset.dt *= -1;
			_rset.val = 0;
		}

		_lpcc->time = _rset.dt;
		_lpcc->val = _rset.val;
		_lpcc ++, lpch_->num_cc++;
	}
	return;
}


//for _pack_option
#define LOGFIRST    0x0001
#define LOGLAST     0x0002

#define CC_MIN      0
#define CC_MAX      1
#define CC_FIRST    2
#define CC_LAST     3
void _FetachDataPackReal( HSTMT hstmt_, LPCACHEHEAD lpch_ )
{
	RETCODE         _ret;
	SQL_RESULT_REAL _rset;
	long            _pack_iv;
	long            _t0, _t1;
	int             _pack_option;
	CACHECELL       _cc[4];

	//initial values
	_pack_iv = ( lpch_->end_time - lpch_->begin_time ) * 2 / lpch_->max_cc;
	_t0 = 0;
	_pack_option = 0;
	memset( _cc, 0, sizeof(_cc) );

	SQLBindCol( hstmt_, 1, SQL_C_LONG, &(_rset.dt), sizeof(_rset.dt), &(_rset.cbdt) );
	SQLBindCol( hstmt_, 2, SQL_C_FLOAT, &(_rset.val), sizeof(_rset.val), &(_rset.cbval) );

	while( 1 )
	{
		_ret = SQLFetch( hstmt_ );
		if( _ret != SQL_SUCCESS )
		{
			//no data or other errors occurred

			//commit the packed data to the cache
			_CommitDataReal( lpch_, _cc, _pack_option );
			break;
		}

		if( _rset.cbval == SQL_NULL_DATA )	//if this is a NULL data
		{
			//first commit the packed data
			if( !_CommitDataReal( lpch_, _cc, _pack_option ) )
				break;  //cache is full

			//don't pack the NULL data and commit it to the cache immediately,
			_cc[CC_MIN].time = -1 * _rset.dt;
			_cc[CC_MIN].val = 0;
			if( !_CommitDataReal( lpch_, _cc, 0 ) )
				break;  //cache is full

			//force to a new pack period
			_t0 = 0;
			_pack_option = 0;
			continue;
		}

		if( _t0 == 0 )  //a new pack period
		{
			_t0 = _rset.dt;
			_t1 = _t0 + _pack_iv;
			_PackDataReal( _cc, &_rset );
		}
		else
		{
			if( _rset.dt > _t1 )
			{
				if( (_rset.dt - _t1 ) > 4 * _pack_iv )
					_pack_option |= LOGLAST;	//log last data in this pack period

				if( !_CommitDataReal( lpch_, _cc, _pack_option ) )
					break;  //cache is full

				_t0 = 0;    //reset _t0 to begin a new pack period
				_t1 = _t0 + _pack_iv;

				//if log last at the last pack period, the log first at
				//this pack period;
				if( _pack_option & LOGLAST )
					_pack_option = LOGFIRST;    //note: here is =, not |=

				_PackDataReal( _cc, &_rset );
			}
			else
				_PackDataReal( _cc, &_rset );
		}
	}//end while

	return;
}

//...................................................................
//Descrption:
//  this function copy data from buffer _cc to lpch_;
//Entry:
//
//  LPCACHEHEAD lpch_
//          points to the head of the cache
//  LPCACHECELL lpcc_
//          points to an array contains four elements
//  int option_
//          besides to log min and max, whether log first data
//          and last data in a pack period;
//          the valid value is the combination of following:
//              LOGFIRST
//              LOGLAST
//
//Return:
//  returns FALSE if the cache is full; otherwise, TRUE returned
//...................................................................
BOOL _CommitDataReal( LPCACHEHEAD lpch_, LPCACHECELL lp_, int option_ )
{
	int             _cc0, _cc1;
	LPCACHECELL     _lpcc;

	if( lpch_->num_cc >= lpch_->max_cc )
		goto _RETURN_FALSE;

	_lpcc = &(lpch_->cc) + (int)lpch_->num_cc;

	if( option_ & LOGFIRST )
	{
		if(  ( lp_[CC_FIRST].time )
		   &&( lp_[CC_FIRST].time != lp_[CC_MIN].time )
		   &&( lp_[CC_FIRST].time != lp_[CC_MAX].time ) )
		{
			_lpcc->time = lp_[CC_FIRST].time;
			_lpcc->val = lp_[CC_FIRST].val;
			_lpcc ++, lpch_->num_cc ++;
			if( lpch_->num_cc >= lpch_->max_cc )
				goto _RETURN_FALSE;
		}
	}

	if( lp_[CC_MIN].time < lp_[CC_MAX].time )
		_cc0 = CC_MIN, _cc1 = CC_MAX;
	else
		_cc0 = CC_MAX, _cc1 = CC_MIN;

	if( lp_[CC_MIN].time == lp_[CC_MAX].time )
	{
		//avoid logging two duplicate data
		lp_[_cc0].time = 0;
	}

	if( lp_[_cc0].time )
	{
		_lpcc->time = lp_[_cc0].time;
		_lpcc->val = lp_[_cc0].val;
		_lpcc ++, lpch_->num_cc ++;
		if( lpch_->num_cc >= lpch_->max_cc )
			goto _RETURN_FALSE;
	}

	if( lp_[_cc1].time )
	{
		_lpcc->time = lp_[_cc1].time;
		_lpcc->val = lp_[_cc1].val;
		_lpcc ++, lpch_->num_cc ++;
		if( lpch_->num_cc >= lpch_->max_cc )
			goto _RETURN_FALSE;
	}

	if( option_ & LOGLAST )
	{
		if(  ( lp_[CC_LAST].time )
		   &&( lp_[CC_LAST].time != lp_[CC_MIN].time )
		   &&( lp_[CC_LAST].time != lp_[CC_MAX].time ) )
		{
			_lpcc->time = lp_[CC_LAST].time;
			_lpcc->val = lp_[CC_LAST].val;
			_lpcc ++, lpch_->num_cc ++;
			if( lpch_->num_cc >= lpch_->max_cc )
				goto _RETURN_FALSE;
		}
	}

	memset( lp_, 0, sizeof(CACHECELL) * 4 );
	return TRUE;    

_RETURN_FALSE:
	memset( lp_, 0, sizeof(CACHECELL) * 4 );
	return FALSE;   //cache is full

}

void _PackDataReal( LPCACHECELL lpcc_, SQL_RESULT_REAL FAR* lprset_ )
{
	if( lpcc_[CC_FIRST].time == 0 )
	{
		lpcc_[CC_FIRST].time = lprset_->dt;
		lpcc_[CC_FIRST].val = lprset_->val;
	}

	lpcc_[CC_LAST].time = lprset_->dt;
	lpcc_[CC_LAST].val = lprset_->val;

	if( lpcc_[CC_MIN].time == 0 )
	{
		lpcc_[CC_MIN].time = lprset_->dt;
		lpcc_[CC_MIN].val = lprset_->val;
	}
	else
	{
		if( lpcc_[CC_MIN].val > lprset_->val )
		{
			lpcc_[CC_MIN].time = lprset_->dt;
			lpcc_[CC_MIN].val = lprset_->val;
		}
	}

	if( lpcc_[CC_MAX].time == 0 )
	{
		lpcc_[CC_MAX].time = lprset_->dt;
		lpcc_[CC_MAX].val = lprset_->val;
	}
	else
	{
		if( lpcc_[CC_MAX].val < lprset_->val )
		{
			lpcc_[CC_MAX].time = lprset_->dt;
			lpcc_[CC_MAX].val = lprset_->val;
		}
	}

	return;
}
