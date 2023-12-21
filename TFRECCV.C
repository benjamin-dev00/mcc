//===================================================================
//
//Zhang 11.11
//===================================================================

//	Recover CV,

#include "tfalgo.h"
#include "tfcmdmc.h"

// the control_type should be freed to be bad CV...
void RecoverCV( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVCV lpCobj_PrivCV_ )
{
	int		_i;

	if( lpCobj_All_->Flag_CControl == 0 )
	{
		for( _i = 0; _i < lpCobj_All_->p; _i++ )
		{
			if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CNoLP == 1 )
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type = 1;
			lpCobj_All_->Freecount--;
/*			if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CNoLP == 2 )
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type = 2;
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CNoLP = 0;*/
		}
	}
	return ;
}
