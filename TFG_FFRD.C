//===================================================================
//Written by Zhang
//10.8, 1998
//This function is for the calculation of the real freedom of this system
//===================================================================

#include "tfalgo.h"
#include "tfcmdmc.h"
#ifdef __ONSPEC
#include "Mtmfun.h"
#include "Mtdllfun.h"
#endif

//Since in MCC, all the effective MV should be undel the IRV control mode, so it should not
//use the freedom.
ACRET	GetFinalFreedom( LPCOBJPRIV lpCobjPrivAll_, LPCOBJPRIVMV lpCobjPrivMV_, LPCOBJPRIVCV lpCobjPrivCV_ )
{
	int	_j;
	GetFreedom( lpCobjPrivAll_, lpCobjPrivMV_, lpCobjPrivCV_ );
	for( _j = 0; _j < lpCobjPrivAll_->m; _j++ )
	{
		if( PRIVMVGET( lpCobjPrivMV_, _j, 0 )->MControl_type == 1 )
		{
			lpCobjPrivAll_->Freecount++;
		}
	}
	return ACRET_TRUE;
}