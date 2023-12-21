#include "tfalgo.h"
#include "tfcmdmc.h"

void MVCorrect( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpCobj_PrivMV_ )
{
	int 	_j;
	WORD	_flag_recount;

	for( _j = 0; _j < lpCobj_All_->m; _j++ )
	{
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MPDirty = 0;

		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 4 ||
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 5 ||
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 7 )
			if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MCT_history != 4 &&
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MCT_history != 5 &&
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MCT_history != 7 &&
                                                        PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSetSatuation == 0 )
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MPDirty =1;
//here if Control_type == 0, same as Control_type == 1;

		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 3 )
			if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MCT_history != 3 &&
                                PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSetSatuation == 0     )
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MPDirty =1;

		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 1 )
			if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MCT_history != 1 )
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MPDirty = 1;
			else
			{
				if( lpCobj_All_->Flag_ReSP != 2 )
					if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData > PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMax ||
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData < PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMin )
					{
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MPDirty = 1;
						if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData > PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMax )
							PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHOverCount++;
						else
							PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLOverCount++;
					}
                if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSetSatuation != 0 )
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MPDirty = 1;
			}
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 2 &&
				lpCobj_All_->Flag_ReSP != 2 )
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MPDirty = 1;
	}
	_flag_recount = 0;
	for( _j = 0; (_j < lpCobj_All_->m) && ( _flag_recount == 0 ); _j++ )
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MPDirty != 0 ||
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MRDirty != 0 )
			_flag_recount = 1;

// If R or P needn't to be rebuilt, then reset the dirty flag;
	if( _flag_recount == 0 )
		for( _j = 0; _j < lpCobj_All_->m; _j++ )
		{
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MPDirty = 0;
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MRDirty = 0;
		}
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSatuation = 0;

	if( lpCobj_All_->Flag_MCVRecount == 0 )
		lpCobj_All_->Flag_MCVRecount = _flag_recount;
	return ;
}
