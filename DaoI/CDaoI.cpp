#include "stdafx.h"
#include <afx.h>

#include <stdio.h>
#include <math.h>
#include <Winbase.h>

#include "CDaoI.h"

#define LOOP_COUNTS 100

//////////////////////////////////////////////////////
//	DAO History Data Implication	// KEVIN 10/26/1998
//////////////////////////////////////////////////////

const CString _HisRealTable = "hisreal";
const CString _HisTintTable = "histint";
const CString _HisBoolTable = "hisbool";

#define LOGFIRST    0x0001
#define LOGLAST     0x0002

#define TEMPLATE_DATABASE	"template.mdb"

CDaoI::CDaoI(void)
{

}

CDaoI::CDaoI(LPCSTR lpszDBName_)
{
	m_DatabaseName = lpszDBName_;
	Initialize(lpszDBName_);
}

CDaoI::~CDaoI()
{
	if(m_pDB)
	{
		if(m_pDB->IsOpen())
			m_pDB->Close();
		delete m_pDB;
		return ;
	}
	delete m_pRS;
}

BOOL CDaoI::CreateDatabase(LPCSTR lpszDBName)
{
	TCHAR	_szFilePath[MAXPATH];
	TCHAR	_szDBPath[MAXPATH];
	TCHAR	_szModuleName[MAXPATH];
	LPTSTR	_pszFileName;
	LPTSTR	_pszDBName;

    GetModuleFileName( NULL, _szModuleName, sizeof(_szModuleName) );
	GetFullPathName( _szModuleName, sizeof(_szFilePath), _szFilePath, &_pszFileName );
	GetFullPathName( _szModuleName, sizeof(_szDBPath), _szDBPath, &_pszDBName );
	
	strcpy( _pszFileName, TEMPLATE_DATABASE );
	strcpy( _pszDBName, lpszDBName);
	strcat( _pszDBName, ".mdb" );
	
	//m_DatabaseName =_szDBPath;

	return CopyFile( _szFilePath, _szDBPath, FALSE );
}

BOOL CDaoI::Initialize(LPCSTR lpszDBName)
{
	TCHAR	_szDBName[MAXPATH]; 

	strcpy( _szDBName, lpszDBName);
	strcat( _szDBName, ".mdb" );

	m_DatabaseName = _szDBName;

	CDaoDatabase * pTempDB=new CDaoDatabase;

	try
	{
		pTempDB->Open( m_DatabaseName );
	}
	catch(CDaoException *e)
	{
		delete pTempDB;
		e->Delete();
		return FALSE;
	}

	m_pDB = pTempDB;
	m_pRS = new CDaoRecordset(m_pDB);
	return TRUE;
}

////////////////////////////////////////////////
// NOTE: Please read CDaoI.h file ...
////////////////////////////////////////////////
BOOL CDaoI::InitialDBI_DAO( void )
{
	CDaoDatabase * _pTempDB = new CDaoDatabase;
	try
	{
		_pTempDB->Open
			(
			"CoreHisData",
			TRUE,
			FALSE,
			";PWD=Admin"//Modify when the Database has built
			);
	}
	catch(CDaoException * e)
	{
		//Deal with Exception
		delete _pTempDB;
		e->GetErrorInfo( e->m_nAfxDaoError);
		delete e;
		return FALSE;
	}
	if(m_pDB)
	{
		if(m_pDB->IsOpen())m_pDB->Close();
		delete m_pDB;
	}
	m_pDB = _pTempDB;
	return TRUE;
}

BOOL CDaoI::FreeDBI_DAO(void)
{
	if(m_pDB != 0) 
		delete m_pDB;
	return TRUE;
}

////////////////////////////////////////////////
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
BOOL CDaoI::LogCore_DAO( int iMode_ )
{
	BOOL				bResult;
	CString				strSQL;
	WORD                _i;
	LPRTU               _lpRTU;
//	BOOL				_IsNullValue;

	CORETIME			_tt;

//	_IsNullValue=FALSE;
	_tt = CoreTimeNow();
	if( iMode_ == 0 || iMode_ == -1 ) 
	{
		CoreSetData( CGD_LASLOGTIME, _tt );
	}
	else
	{
		//the two times log interval must be great than 1 second
		if( CoreGetData( CGD_LASLOGTIME ) == (DWORD)_tt )
			return TRUE;

		//update the log time
		CoreSetData( CGD_LASLOGTIME, _tt );
	}

	RTInfo * _gRTInfo = GetRealTableInformation();

	//log real table
	for( _i=1; _i<_gRTInfo->wAdding; _i++ )
	{
		_lpRTU = _gRTInfo->lpBase + _i;
		if(  (_lpRTU->Status & RTUBIT_VALID)
		   &&(_lpRTU->Status & RTUBIT_LOGHIS) )
		{
			if( iMode_ == 0 )   //initial state of the core
			{
				//CDaoRecordset
				strSQL.Format("INSERT INTO hisreal VALUES(%d, %ld, NULL)",
						 _i,
						 _tt - 1 );

				bResult = TRUE;
				try
				{
					m_pDB->Execute(strSQL, dbConsistent );   
				}
				catch(CDaoException * e)
				{
					e->GetErrorInfo( e->m_nAfxDaoError);
					delete e;
					bResult = FALSE;
					return bResult;
				}
	
			}
			//_r_delta = _lpRTU->LogBand;
			if( (iMode_ != 1) || ( fabs(_lpRTU->Value - _lpRTU->LastLog) > _lpRTU->LogBand ) )
			{
				strSQL.Format("INSERT INTO hisreal VALUES(%d, %ld, %f)",
						 _i,
						 _tt,
						 _lpRTU->Value );

				bResult = TRUE;
				try
				{
					m_pDB->Execute ( strSQL , dbConsistent );
				}
				catch(CDaoException * e)
				{
					e->GetErrorInfo( e->m_nAfxDaoError);
					delete e;
					bResult = FALSE;
					return bResult ;
				}
			}
			//end state of the core
			if( iMode_ == -1 )
			{
				strSQL.Format("INSERT INTO hisreal VALUES(%d, %ld, NULL)",
						 _i,
						 _tt + 1 );

				bResult = TRUE;
				try
				{
					m_pDB->Execute( strSQL , dbConsistent );
				}
				catch(CDaoException * e)
				{
					e->GetErrorInfo( e->m_nAfxDaoError);
					delete e;
					bResult = FALSE;
					return bResult;
				}
			}
		}
	}
/*************
	//log tint table
	LPTTU	_lpTTU;
	TTInfo * _gTTInfo = GetTintTableInformation();

	//log real table
	for( _i=1; _i<_gTTInfo->wAdding; _i++ )
	{
		_lpTTU = _gRTInfo->lpBase + _i;
		if(  (_lpTTU->Status & RTUBIT_VALID)
		   &&(_lpTTU->Status & RTUBIT_LOGHIS) )
		{
			if( iMode_ == 0 )   //initial state of the core
			{
				strSQL.Format("INSERT INTO hisreal VALUES(%d, %ld, NULL)",
						 _i,
						 _tt - 1 );

				bResult = TRUE;
				try
				{
					m_pDB->Execute(strSQL, dbConsistent );   
				}
				catch(CDaoException * e)
				{
					e->GetErrorInfo( e->m_nAfxDaoError);
					delete e;
					bResult = FALSE;
					return bResult;
				}
	
			}
			//_r_delta = _lpTTU->LogBand;
			if( (iMode_ != 1) || ( fabs(_lpTTU->Value - _lpTTU->LastLog) > _lpTTU->LogBand ) )
			{
				strSQL.Format("INSERT INTO hisreal VALUES(%d, %ld, %f)",
						 _i,
						 _tt,
						 _lpTTU->Value );

				bResult = TRUE;
				try
				{
					m_pDB->Execute ( strSQL , dbConsistent );
				}
				catch(CDaoException * e)
				{
					e->GetErrorInfo( e->m_nAfxDaoError);
					delete e;
					bResult = FALSE;
					return bResult ;
				}
			}
			//end state of the core
			if( iMode_ == -1 )
			{
				strSQL.Format("INSERT INTO hisreal VALUES(%d, %ld, NULL)",
						 _i,
						 _tt + 1 );

				bResult = TRUE;
				try
				{
					m_pDB->Execute( strSQL , dbConsistent );
				}
				catch(CDaoException * e)
				{
					e->GetErrorInfo( e->m_nAfxDaoError);
					delete e;
					bResult = FALSE;
					return bResult;
				}
			}
		}
	}
	//log bool table
	LPBTU _lpBTU;
	for( _i=1; _i<pgBTInfo->wAdding; _i++ )
	{
		_lpBTU = pgBTInfo->lpBase + _i;
		if(  (_lpBTU->Status & RTU_VALID)
		   &&(_lpBTU->Status & RTU_LOGHIS) )
		{
			if( iMode_ == 0 )   //initial state of the core
			{
				strSQL.Format("INSERT INTO hisbool VALUES(%d, %ld, NULL)",
						 _i,
						 _tt - 1 );

				bResult = TRUE;
				try
				{
					m_pDB->Execute(strSQL, dbConsistent );   
				}
				catch(CDaoException * e)
				{
					e->GetErrorInfo( e->m_nAfxDaoError);
					delete e;
					bResult = FALSE;
				}
			}

			if(  ( iMode_ != 1 )
			   ||( _lpBTU->Value != _lpBTU->LastLog) )
			{
				strSQL.Format("INSERT INTO hisbool VALUES(%d, %ld, %s)",
						 _i,
						 _tt,
						 (_lpBTU->Value==TRUE)?"TRUE":"FALSE");

				bResult = TRUE;
				try
				{
					m_pDB->Execute ( strSQL , dbConsistent );
				}
				catch(CDaoException * e)
				{
					e->GetErrorInfo( e->m_nAfxDaoError);
					delete e;
					bResult = FALSE;
				}
			}
			//end state of the core
			if( iMode_ == -1 )
			{
				strSQL.Format("INSERT INTO hisbool VALUES(%d, %ld, NULL)",
						 _i,
						 _tt + 1 );

				bResult = TRUE;
				try
				{
					m_pDB->Execute( strSQL , dbConsistent );
				}
				catch(CDaoException * e)
				{
					e->GetErrorInfo( e->m_nAfxDaoError);
					delete e;
					bResult = FALSE;
				}
			}
		}
	}
	****/

	return TRUE;
}

//...................................................................
//...................................................................
LPCACHEHEAD CDaoI::QueryHisData_DAO( LPCACHEHEAD lpch_,
								WORD table_,
								WORD wrd_,
								CORETIME begin_time_,
								CORETIME end_time_ )
{
	switch(table_)
	{
	case TBX_RT:
		return QueryDataReal( lpch_, wrd_, begin_time_, end_time_ );
/*******
	case TBX_TT:
		return QueryDataTint( lpch_, wrd_, begin_time_, end_time_ );
	case TBX_BT:
		return QueryDataBool( lpch_, wrd_, begin_time_, end_time_ );
********/
	}
}

long _near CDaoI::QueryResultRows( LPCSTR lpszTableName_,
									WORD wrd_,
									CORETIME begin_time_,
									CORETIME end_time_ )
{
	long _relValue = -1;
	VARIANT			_varV;
	COleVariant		_oleVar;

	CString strSQL;
	
	strSQL.Format(
		"SELECT count(*) FROM %s WHERE ( id = %d ) AND ( dt BETWEEN %d AND %d )", 
		lpszTableName_,
		wrd_,
		begin_time_,
		end_time_ );
	try
	{
		m_pRS->Open(dbOpenDynaset , strSQL );

		if(m_pRS->IsEOF())
		{
			m_pRS->Close();
			return _relValue;
		}
		_oleVar = m_pRS->GetFieldValue(0);
		_varV = _oleVar.Detach();
		m_pRS->Close(); //Should it be shut down ?
		_relValue = _varV.lVal;
	}
	catch(CDaoException * e)
	{
		e->GetErrorInfo( e->m_nAfxDaoError);
		delete e;
		return _relValue;
	}
	return _relValue;
}

LPCACHEHEAD _near CDaoI::QueryDataReal( LPCACHEHEAD lpch_,
										WORD wrd_,
										CORETIME begin_time_,
										CORETIME end_time_ )
{

	SDWORD          _nr0, _nr1, _nr;
	CString			strSQL;
	BOOL            _pack = FALSE;
	LPCACHEHEAD     _lpch = lpch_;
	SQL_RESULT_REAL _setBefore;
	SQL_RESULT_REAL _setAfter;

	if( !_lpch )
		return _lpch;
	
	if( _lpch->num_cc >= _lpch->max_cc )
		return _lpch;

	CORE_ASSERT( _lpch->max_cc == CACHE_MAX_ITEMS );
	_nr0 = _QueryDataBeforeReal( &_setBefore, wrd_, begin_time_ );
	_nr1 = _QueryDataAfterReal( &_setAfter, wrd_, end_time_ );

	_nr = QueryResultRows( _HisRealTable, wrd_, begin_time_, end_time_ );
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

	_FetachDataReal(  _lpch , wrd_ , begin_time_ , end_time_ );

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
			_lpcc->time =_setAfter.dt;
			_lpcc->val = _setAfter.val;
			_lpch->num_cc ++;
		}
	}
	return  _lpch;
}

////////////////////////////////////////////////////////
//	_QueryDataBeforeReal
////////////////////////////////////////////////////////
int CDaoI::_QueryDataBeforeReal(SQL_RESULT_REAL FAR * _lpset,
								WRD wrd_,
								CORETIME begin_time_ )
{
	int         _result = 0;
	CString		strSQL;
	VARIANT		_varV;
	COleVariant	_oleVar;

	strSQL.Format( "SELECT dt, val FROM %s WHERE dt=(Select MAX(dt) from %s where id=%d and dt<=%d)",
		_HisRealTable,
		_HisRealTable,
		wrd_,
		begin_time_ 
		);
	try
	{
		//CDaoRecordset
		m_pRS->Open(dbOpenDynaset , strSQL);
		
		if (m_pRS->IsEOF()) 
		{
			m_pRS->Close();
			_lpset->cbval = SQL_NULL_DATA;
			return _result;
		}
		
		_oleVar = m_pRS->GetFieldValue(0);//Get dt
		_varV = _oleVar.Detach();
		_lpset->dt = _varV.lVal;

		_oleVar = m_pRS->GetFieldValue(1);//Get val
		if (_oleVar.vt == VT_NULL)
		{
			m_pRS->Close();
			_lpset->cbval = SQL_NULL_DATA;
			return _result;
		}
		_varV = _oleVar.Detach();
		_lpset->val = _varV.fltVal ;
		m_pRS->Close(); //Should it be shut down ?
		_result = 1;
	}
	catch(CDaoException * e)
	{
		e->GetErrorInfo( e->m_nAfxDaoError);
		delete e;
		return _result;
	}
	return _result;
}

////////////////////////////////////////////////////////
//	_QueryDataAfterReal
////////////////////////////////////////////////////////
int CDaoI::_QueryDataAfterReal( SQL_RESULT_REAL FAR * _lpset,
								WRD wrd_,
								CORETIME end_time_ )
{
	int         _result = 0;
	CString		strSQL;
	VARIANT		_varV;
	COleVariant	_oleVar;

	//step
	strSQL.Format( "SELECT dt, val FROM %s WHERE dt="\
		"(SELECT min(dt) FROM %s WHERE id=%d AND dt>=%d) ",
		 _HisRealTable,
		 _HisRealTable,
		 wrd_,
		 end_time_ );
	try
	{
		
		m_pRS->Open(dbOpenDynaset , strSQL);

		if(m_pRS->IsEOF())
		{
			m_pRS->Close();
			_lpset->cbval = SQL_NULL_DATA;
			return _result;
		}
		
		_oleVar = m_pRS->GetFieldValue(0);
		_varV = _oleVar.Detach();
		_lpset->dt = _varV.lVal;

		_oleVar = m_pRS->GetFieldValue(1);
		if(_oleVar.vt==VT_NULL)
		{
			m_pRS->Close();
			_lpset->cbval = SQL_NULL_DATA;
			return _result;
		}
		_varV = _oleVar.Detach();
		_lpset->val = _varV.fltVal ;
		m_pRS->Close(); //Should it be shut down ?
		_result = 1;
	}
	catch(CDaoException * e)
	{
		e->GetErrorInfo( e->m_nAfxDaoError);
		delete e;
		return _result;
	}
	return _result;
}

////////////////////////////////////////////////////////
//	_FetachDataReal
////////////////////////////////////////////////////////
void CDaoI::_FetachDataReal(	LPCACHEHEAD lpch_ ,
								WORD wrd_,
								CORETIME begin_time_,
								CORETIME end_time_ )
{
	LPCACHECELL     _lpcc;
	CString			strSQL;
	VARIANT			_varV;
	COleVariant		_oleVar;
	long			_dt;
	float			_val;


	
	if( lpch_->pack )
	{
		_FetachDataPackReal( lpch_ , wrd_ , begin_time_ , end_time_ );
		return;
	}

	strSQL.Format(
		"SELECT dt , val FROM %s WHERE ( id = %d ) AND "
		"( dt BETWEEN %ld AND %ld )", 
		_HisRealTable,
		wrd_,
		begin_time_,
		end_time_ );
	
	_lpcc = &(lpch_->cc) + (int)lpch_->num_cc;

	try
	{
		m_pRS->Open(dbOpenDynaset , strSQL);
	}
	catch(CDaoException * e)
	{
		e->GetErrorInfo( e->m_nAfxDaoError);
		delete e;
		return ;
	}

	while( ! m_pRS->IsEOF()  )
	{
		if( lpch_->num_cc >= lpch_->max_cc )
			break;  //cache is full

		_oleVar = m_pRS->GetFieldValue(0);
		_varV = _oleVar.Detach();
		_dt = _varV.lVal;

		_oleVar = m_pRS->GetFieldValue(1);
		_varV = _oleVar.Detach();		//COleVariant
		_val = _varV.fltVal ;

		if( VT_NULL == _varV.vt)
		{   //this is a mark to identify NULL data
			_dt *= -1;
			_val = 0;
		}
		_lpcc->time = _dt;
		_lpcc->val = _val;

		_lpcc ++, lpch_->num_cc++;
		m_pRS->MoveNext( );//CDaoRecordset		
	}
	m_pRS->Close();

	return;
}


#define CC_MIN      0
#define CC_MAX      1
#define CC_FIRST    2
#define CC_LAST     3

////////////////////////////////////////////////////////
//	_FetachDataPackReal
////////////////////////////////////////////////////////
void CDaoI::_FetachDataPackReal(	LPCACHEHEAD lpch_ ,
							WORD wrd_,
							CORETIME begin_time_,
							CORETIME end_time_ )
{
	long			_dt;
	float			_val;
	long            _pack_iv;
	long            _t0, _t1;
	int             _pack_option;
	CACHECELL       _cc[4];
	CString			strSQL;
	VARIANT			_varV;
	COleVariant		_oleVar;

	//initial values
	_pack_iv = ( lpch_->end_time - lpch_->begin_time ) * 2 / lpch_->max_cc;
	_t0 = 0;
	_pack_option = 0;
	memset( _cc, 0, sizeof(_cc) );

	strSQL.Format(
		"SELECT dt , val FROM %s WHERE ( id = %d ) AND "\
		"( dt BETWEEN %ld AND %ld )", 
		_HisRealTable,
		wrd_,
		begin_time_,
		end_time_ );
	
	try
	{
		m_pRS->Open(dbOpenDynaset , strSQL);
		
	}
	catch(CDaoException * e)
	{
		e->GetErrorInfo( e->m_nAfxDaoError);
		delete e;
		return ;
	}
	
	while( ! m_pRS->IsEOF() )
	{


		_oleVar = m_pRS->GetFieldValue(0);
		_varV = _oleVar.Detach();
		_dt = _varV.lVal;

		_oleVar = m_pRS->GetFieldValue(1);
		_varV = _oleVar.Detach();		//COleVariant
		_val = _varV.fltVal ;

		if( VT_NULL == _varV.vt )	//if this is a NULL data
		{
			//first commit the packed data
			if( !_CommitDataReal( lpch_, _cc, _pack_option ) )
				break;  //cache is full

			//don't pack the NULL data and commit it to the cache immediately,
			_cc[CC_MIN].time = -1 * _dt;
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
			_t0 = _dt;
			_t1 = _t0 + _pack_iv;
			_PackDataReal( _cc, _dt , _val);
		}
		else
		{
			if( _dt > _t1 )
			{
				if( (_dt - _t1 ) > 4 * _pack_iv )
					_pack_option |= LOGLAST;	//log last data in this pack period

				if( !_CommitDataReal( lpch_, _cc, _pack_option ) )
					break;  //cache is full

				_t0 = 0;    //reset _t0 to begin a new pack period
				_t1 = _t0 + _pack_iv;

				//if log last at the last pack period, the log first at
				//this pack period;
				if( _pack_option & LOGLAST )
					_pack_option = LOGFIRST;    //note: here is =, not |=

				_PackDataReal( _cc, _dt , _val);
			}
			else
				_PackDataReal( _cc, _dt , _val);
		}
	}//end while
	m_pRS->Close();
	return;
}

////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////
BOOL CDaoI::_CommitDataReal( LPCACHEHEAD lpch_, LPCACHECELL lp_, int option_ )
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

/////////////////////////////////////////////////////////////
//	_PackDataReal
/////////////////////////////////////////////////////////////
void CDaoI::_PackDataReal( LPCACHECELL lpcc_, long _dt , float _val )
{
	if( lpcc_[CC_FIRST].time == 0 )
	{
		lpcc_[CC_FIRST].time = _dt;
		lpcc_[CC_FIRST].val = _val;
	}

	lpcc_[CC_LAST].time = _dt;
	lpcc_[CC_LAST].val = _val;

	if( lpcc_[CC_MIN].time == 0 )
	{
		lpcc_[CC_MIN].time = _dt;
		lpcc_[CC_MIN].val = _val;
	}
	else
	{
		if( lpcc_[CC_MIN].val > _val )
		{
			lpcc_[CC_MIN].time = _dt;
			lpcc_[CC_MIN].val = _val;
		}
	}

	if( lpcc_[CC_MAX].time == 0 )
	{
		lpcc_[CC_MAX].time = _dt;
		lpcc_[CC_MAX].val = _val;
	}
	else
	{
		if( lpcc_[CC_MAX].val < _val )
		{
			lpcc_[CC_MAX].time = _dt;
			lpcc_[CC_MAX].val = _val;
		}
	}
	return;
}
/************
BOOL CDaoI::LogCore_TEST( void)
{
	CString strSQL;
	strSQL.Format("INSERT INTO hisreal VALUES(100, 100, NULL)");
	try
	{
		m_pDB->Execute(strSQL, dbConsistent );   
	}
	catch(CDaoException * e)
	{
		e->GetErrorInfo( e->m_nAfxDaoError);
		delete e;
		return FALSE;
	}
	return TRUE;
}
*******/
///////////////////////////////////////////////////////
static class CDaoI sDaoi;
///////////////////////////////////////////////////////
DLLEXP BOOL CreateDatabase_DAO( LPCSTR szDBName_ )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	return CDaoI::CreateDatabase (szDBName_);
}

DLLEXP BOOL InitialDBI_DAO( LPCSTR lpszDBName_ )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	return sDaoi.Initialize (lpszDBName_);
}

DLLEXP BOOL FreeDBI_DAO(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	return sDaoi.FreeDBI_DAO(); 
}

DLLEXP BOOL LogCore_DAO( int iMode_ )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	return sDaoi.LogCore_DAO (iMode_);
}

DLLEXP LPCACHEHEAD QueryHisData_DAO( _tagCACHE_HEAD* lpch_, WORD table_, WORD wrd_, long begin_time_, long end_time_ )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	return sDaoi.QueryHisData_DAO( lpch_, table_, wrd_, begin_time_, end_time_ );
}
