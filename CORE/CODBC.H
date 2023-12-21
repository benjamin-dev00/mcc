//===================================================================
//CODBC.H
//	head file for CODBC.C
//Zheng zhikai	98-10-28
//===================================================================

#ifndef __CODBC_H
#define __CODBC_H

BOOL CreateDatabase_ODBC( LPCSTR szName_ );
BOOL InitialDBI_ODBC( LPCSTR lpszDBName_ );
BOOL FreeDBI_ODBC(void);
BOOL LogCore_ODBC( int iMode_ );
void GetDBIError_ODBC(void);

LPCACHEHEAD QueryHisData_ODBC(LPCACHEHEAD lpch_,
							 WORD table_,
							 WORD wrd_,
							 CORETIME begin_time_,
							 CORETIME end_time_ );
#endif	//__CODBC_H

