#include "tfalgo.h"
#include "tfcmdmc.h"
///???????????? CT_history
void MVBeginPri( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpCobj_PrivMV_ )
{
	int		_j, _kk, _l;
	WORD	_Dirtycount, _found_irv, freecount_;

//	when there is LP program
	freecount_ = (WORD)lpCobj_All_->Freecount;
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MPDirty = 0;
	if( freecount_ > 0 && lpCobj_All_->Flag_LP == 1 )
	{
		_Dirtycount = 0;
		for( _j = 0; _j < lpCobj_All_->m; _j++ )
			if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSDirty == 1 )
				_Dirtycount++;
		if( freecount_ >= _Dirtycount )
		{
			for( _j = 0; _j < lpCobj_All_->m; _j++ )
				if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSDirty == 1 )
				{
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv =
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSteadOptim;
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type = 1;
					freecount_--;
				}
		}
		else
		{
			MVPrioritySort( lpCobj_All_, lpCobj_PrivMV_ );
			for( _kk = 0; _kk < freecount_; _kk++ )
			{
				_found_irv = 0;
				for( _j = 0; (_j < lpCobj_All_->m) && (_found_irv == 0 ); _j++ )
				{
					_l = (int)MGET( COBJ_2_MATRIX( lpCobj_All_->LPMVSort ), (lpCobj_All_->m-_j-1), 0 );
					if( PRIVMVGET( lpCobj_PrivMV_, _l, 0 )->MSDirty == 1 )
					{
						PRIVMVGET( lpCobj_PrivMV_, _l, 0 )->MControl_type = 1;
						PRIVMVGET( lpCobj_PrivMV_, _l, 0 )->MIrv =
							PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSteadOptim;
						_found_irv = 1;
					}
				}
			}
			freecount_ = 0;
		}
	}
//if no LP, Then
	if( freecount_ > 0 )
	{
		for( _l = 0; _l < freecount_; _l++ )
		{
			_found_irv = 0;
			MVPrioritySort( lpCobj_All_, lpCobj_PrivMV_ );
			for( _kk = 0; (_kk < lpCobj_All_->m) && (_found_irv == 0); _kk++ )
			{
					_j = (int) MGET( COBJ_2_MATRIX( lpCobj_All_->LPMVSort ), (lpCobj_All_->m-_kk-1), 0 );
					if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 1 )
					{
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type = 1;
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MPDirty = 1;
						_found_irv = 1;
					}
					if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 2 )
						if( ( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData >
							  PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvHigh )
							||	(PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData <
								PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvLow ) )
						{
							PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type = 2;
							PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MPDirty = 1;
							_found_irv = 1;
							if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData >
								PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvHigh )
								PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv =
									PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvHigh;
							else
								PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv =
									PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvLow;
						}
						else
						{
							PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type = 3;
                           	PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MPDirty = 1;
						}
			}
		}
		freecount_ -= _l;
		for( _j = 0; _j < lpCobj_All_->m; _j++ )
			if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MPDirty == 0 )
         			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type = 4;
	}

	for( _j = 0; _j < lpCobj_All_->m; _j++ )
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MPDirty != 1 )
			if( lpCobj_All_->Flag_LP == 1 )
			{
				MessageBox( NULL, "error", "B_MVP", MB_OK );
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type = 4;
			}
//till now all the control_type of MV is determined;
	return  ;
}
