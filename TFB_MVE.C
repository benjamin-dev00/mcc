#include "tfalgo.h"
#include "tfcmdmc.h"
void MVBeginErr( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpCobj_PrivMV_ )
{
	int		_j;

	if( lpCobj_All_->Flag_MControl == 0 )
	{
		for( _j = 0; _j < lpCobj_All_->m; _j++ )
		{
			if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSDirty == 1 )
			{
				if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type != 5 &&
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type != 4 &&
							PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type != 7  )
				{
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type = 1;
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv =
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSteadOptim;
				}
			}
		}
	}
//if sqp has considered all the MV, then NO ELSE here!????
/*		else
		{
			if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 2 )
				if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData >
					  PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvFault_high
					||	PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData <
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvFault_low )
				{
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type = 2;
					if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData >
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvFault_high )
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_hirv;
					else
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_lirv;
				}
				else
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type = 3;

		}
	}*/

	if( lpCobj_All_->Flag_MControl == 0 )
	{
		for( _j = 0; _j < lpCobj_All_->m; _j++ )
		{
			if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 2 &&
					lpCobj_All_->Flag_Alarm != 1 )
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type = 3;
		}		
	}
	return  ;
}
