#include "tfalgo.h"
#include "tfcmdmc.h"

void CVCorrect( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVCV lpCobj_PrivCV_ )
{
	int		_i;
	WORD 	_flag_recount;

	for( _i = 0; _i < lpCobj_All_->p; _i++ )
	{
//	if control_type of CV is 5, then it can be imagined
//	that the CV Setpoint is not allowed, the control scheme
//  should be corrected;
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 4 )
			if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CCT_history == 1 ||
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CCT_history == 2 )
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDirty = 1;

		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 2 ||
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 3 )
		{
			if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHOver == 0 &&
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLOver == 0 )
			{
				if( //PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CCT_history == 1 ||
						PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CCT_history == 2 )
				{
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDelayCount++;
//it would not be runned maybe??????
                    if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDelayCount >= lpCobj_All_->MaxDelay )
//						&&
//							PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSDirty == 0 )
					{
						PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDirty = 1;
						PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDelayCount = 0;
					}
					else
						PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDirty = 0;
				}
				if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDelayCount == 0 ||
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CNoIll == 1 ||
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CNoLP == 1 )
						PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type = 3;
				else
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type = 2;
//note: here should not be executed!
			}
//it should return to 2
			else
			{
/*				if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHOver == 1  )
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSetpoint =
						PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_hsetpoint;
				else
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSetpoint =
						PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_lsetpoint;*/
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDelayCount = 0;
				if( lpCobj_All_->Flag_ReSP == 1 &&
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CCT_history == 2 )
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDirty = 0;
				else
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDirty = 1;
			}
		}
//is it right? --~
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 1 )
		{
			if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CCT_history != 1 )
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDirty = 1;
			else
				if( lpCobj_All_->Flag_ReSP != 1 )
					if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHOver == 1 ||
						PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLOver == 1 )
						PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDirty = 1;
		}
	}
	_flag_recount = 0;
	for( _i = 0; (_i < lpCobj_All_->p) && ( _flag_recount == 0 ); _i++ )
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDirty == 1 )
			_flag_recount = 1;

	lpCobj_All_->Flag_MCVRecount = _flag_recount;
	return ;
}
