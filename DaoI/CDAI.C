//===================================================================
//  CDBI.C      Core Database interface
//Zheng zhikai  96-10-18
//===================================================================

#include "prehead.h"
#include "coreall.h"

#include <dos.h>
#include <time.h>
#include <sqlext.h>
#include <math.h>

#include "codbc.h"
 
#include "CDaoi.h"

//...................................................................
//Entries:
//
//	lpszDBName_		a full file name which to be created, not include
//					file extension
//...................................................................
BOOL CreateDatabase( LPCSTR szDBName_ )
{
	return CreateDatabase_ODBC( szDBName_ );
}

//...................................................................
//Entries:
//
//	lpszDBName_		a full Database file name
//					Note: not include file extension 
//...................................................................
BOOL InitialDBI( LPCSTR lpszDBName_ )
{
	return InitialDBI_ODBC( lpszDBName_ );
}

//...................................................................
//...................................................................
BOOL FreeDBI()
{
	return FreeDBI_ODBC();
}

//...................................................................
//...................................................................
void GetDBIError()
{
	GetDBIError_ODBC();
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
BOOL LogCore( int iMode_ )
{
	return LogCore_ODBC( iMode_ );
}

//...................................................................
//...................................................................
_DLLEXP LPCACHEHEAD _DLLFUNC QueryHisData( LPCACHEHEAD lpch_,
										   WORD table_,
										   WORD wrd_,
										   CORETIME begin_time_,
										   CORETIME end_time_ )
{
	return QueryHisData_ODBC( lpch_, table_, wrd_, begin_time_, end_time_ );
}

//...................................................................
//...................................................................
_DLLEXP LPCACHEHEAD _DLLFUNC AllocateHisCache( int max_item_ )
{
	LPCOBJ		_lpCobj;
	LPCACHEHEAD _lpch = NULL;

	_lpCobj = CobjAlloc( IDCOBJ_BINARY, sizeof(CACHEHEAD) + max_item_ * sizeof( CACHECELL ) );

	_lpch = ( LPCACHEHEAD )GET_COBJ_DATA( _lpCobj );
	if( !_lpch )
		return NULL;

	_lpch->max_cc = max_item_;
	_lpch->num_cc = 0;
	_lpch->pack = FALSE;
	_lpch->begin_time = 0;
	_lpch->end_time = 0;
	_lpch->table = 0;
	_lpch->wrd = 0;

	return _lpch;
}

