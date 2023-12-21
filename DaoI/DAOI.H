#ifndef _DAOI_COMPILED_
#define _DAOI_COMPILED_

#include "cdbi.h"

BOOL CreateDatabase_DAO( LPCSTR szDBName_ );
BOOL InitialDBI_DAO( LPCSTR lpszDBName_ );
BOOL FreeDBI_DAO(void);
BOOL LogCore_DAO( int iMode_ );
LPCACHEHEAD QueryHisData_DAO( LPCACHEHEAD lpch_, WORD table_, WORD wrd_, long begin_time_, long end_time_ );

#endif	//_DAOI_COMPILED_