#include "tfalgo.h"
#include "tfcmdmc.h"
int 		Isrecount( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpCobj_PrivMV_,
						LPCOBJPRIVCV lpCobj_PrivCV_, WORD recount_ )
{
	int 	_i, _j, _kk;
	WORD	_flag_QBroken, _flag_PBroken, _flag_RBroken, _flag_Exceed;
	double 	_temp;
	//double	_tempCond;
	LPMATRIX _lpmu, _lpmdelu, _lpmDeluM;

	_lpmDeluM = COBJ_2_MATRIX( lpCobj_All_->LPDeluM );
	_lpmu = MatrixAlloc(lpCobj_All_->m, 1);
	_lpmdelu = MatrixAlloc(lpCobj_All_->m,1);

//	for( _i = 0; _i < lpCobj_All_->p; _i++ )
//		for( _j = 0; _j < lpCobj_All_->N; _j++ )
//			_mypredN[_i][_j] = MGET( _lpmypredN, (_i*lpCobj_All_->N+_j), 0 );

	for( _j = 0; _j < lpCobj_All_->m; _j++ )
	{
		MGET( _lpmu, _j, 0 ) = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSample;
		MGET( _lpmdelu, _j, 0 ) = MGET( _lpmDeluM, _j*lpCobj_All_->M, 0 );
		MGET( _lpmu, _j, 0 ) += MGET( _lpmdelu, _j, 0 );
	}

	_flag_RBroken = 0;
	_flag_QBroken = 0;
	_flag_PBroken = 0;
	for( _i = 0; _i < lpCobj_All_->p; _i++ )
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CQBroken == 1 )
			_flag_QBroken = 1;

	for( _j = 0; _j < lpCobj_All_->m; _j++ )
	{
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MRBroken == 1 )
			_flag_RBroken = 1;
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MPBroken == 1 )
			_flag_PBroken = 1;
	}

	if( _flag_RBroken == 0 )
		for( _j = 0; _j < lpCobj_All_->m; _j++ )
		{
			if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type != 4 && 
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type != 5 )
			{
				_flag_Exceed = 0;
//Here only the move of MV is considered
				_temp = MGET( _lpmDeluM, _j*lpCobj_All_->M+_kk, 0 );
				for( _kk = 0; (_kk < 1)&&( _flag_Exceed == 0); _kk++ )
				{
					_temp = MGET( _lpmDeluM, _j*lpCobj_All_->M+_kk, 0 );
					if( _temp < PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDelmin ||
						_temp > PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDelmax)
					{
						if( _flag_Exceed == 0 )
						{
							PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MRDirty = 1;
							PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MCT_history =
								PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type;
							PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type = 8;
							PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSatuation = 1;
							if( _temp < PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDelmin )
								PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLOverCount++;
							else
								PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHOverCount++;
						}
						_flag_Exceed = 1;
						if( _temp < PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDelmin )
							_temp = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDelmin;
						else
							_temp = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDelmax;

						MGET( _lpmDeluM, _j*lpCobj_All_->M+_kk, 0 ) = _temp;
					}
				}
			}
		}
	else
	{
		//LogErr( "delu Broken" );
		MessageBox(NULL, "delu Broken", "", MB_OK);
		_flag_RBroken = 0;
	}

	for( _j = 0; _j < lpCobj_All_->m; _j++ )
	{
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type != 4 &&
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type != 5 )
//		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 1 )
		{
			_flag_Exceed = 0;
			for( _kk = 0; (_kk < 1 )&&( _flag_Exceed == 0); _kk++ )
			{
				_temp = MGET( _lpmDeluM, _j*lpCobj_All_->M+_kk, 0 );
				_temp += PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSample;
				if( _temp < PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMin ||
					_temp > PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMax )
				{
					_flag_Exceed = 1;
					if( _temp < PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMin )
					{
						 MGET( _lpmDeluM, _j*lpCobj_All_->M+_kk, 0 )=
							PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMin -
												PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSample;
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLOver = 1;
					}
					else
					{
						MGET( _lpmDeluM, _j*lpCobj_All_->M+_kk, 0 )=
							PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMax -
												PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSample;
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHOver = 1;
					}
					if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 1 )
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MPDirty = 1;
					else
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MPDirty = 0;

					if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MRDirty == 0 )
					{
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MCT_history =
							PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type;
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type = 8;
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSatuation = 1;
					}
				}
			}
		}
	}
	if( _flag_QBroken == 0 )
	{
		if( recount_ < repeatcount )
			lpCobj_All_->Freecount = IsmypredOver( lpCobj_All_,
									lpCobj_PrivMV_, lpCobj_PrivCV_ );
	}
	else
	{
		MessageBeep( -1 );
        LogError( "CVBroken", "tfis_re.c" );
//		MessageBox(NULL, "CVBroken", "", MB_OK);
		_flag_QBroken = 0;
	}

	if( lpCobj_All_->Flag_MControl == 1 )
		MVPrioritySort( lpCobj_All_, lpCobj_PrivMV_ );
/*
	if( _flag_PBroken == 0 )
	{
		for( _j = 0; _j < lpCobj_All_->m; _j++ )
		{
			if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MPDirty == 0 )
			{
				if( lpCobj_All_->Flag_MControl == 1 )
					_i = ( int )MGET( ( COBJ_2_MATRIX( lpCobj_All_->LPMVSort ) ), (lpCobj_All_->m-_j-1), 0);
				else
					_i = _j;

				if( PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MControl_type == 2 ||
					PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MControl_type == 3 )
				{
					_flag_Exceed = 0;
					for( _kk = 0; (_kk < lpCobj_All_->M)&&( _flag_Exceed == 0); _kk++ )
					{
						_temp = MGET( _lpmDeluM, _j*lpCobj_All_->M+_kk, 0 );
						_temp += PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSample;
						if( _temp > PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MIrvFault_high
							|| _temp < PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MIrvFault_low )
						{
		//here maybe dirty flag should be transfered to next count_, freedom?
		//note: is matrix R should be corrected?
							_flag_Exceed = 1;
							PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MCT_history = PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MControl_type;
							if( PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MControl_type == 2 )
							{
								PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MPDirty = 1;
								if( MGET( _lpmu, _i, 0 ) > PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MIrvHigh )
									PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MIrv = PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MFault_hirv;
								else
									PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MIrv = PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MFault_lirv;
							}
							if( PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MControl_type == 3 )
							{
								if( lpCobj_All_->Flag_MControl	== 1 )
								{
									if( lpCobj_All_->Freecount > 0 )
									{
										PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MControl_type = 2;
										_tempCond = GetConditionNumber( lpCobj_All_, 
												lpCobj_PrivMV_, lpCobj_PrivCV_ );
										if( _tempCond < lpCobj_All_->Big_Cond )
										{
											PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MPDirty = 1;
											PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MControl_type = 2;
											if( MGET( _lpmu, _i, 0 ) > PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MIrvHigh )
												PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MIrv = PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MFault_hirv;
											else
												PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MIrv = PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MFault_lirv;
											lpCobj_All_->Freecount--;
										}
										else
											PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MControl_type = 3;
									}
								}
								else
								{
									PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MControl_type = 2;
									_tempCond = GetConditionNumber( lpCobj_All_,
											lpCobj_PrivMV_, lpCobj_PrivCV_ );
									if( _tempCond < lpCobj_All_->Big_Cond )
									{
										PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MPDirty = 1;
										PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MControl_type = 2;
										if( MGET( _lpmu, _i, 0 ) > PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MIrvHigh )
											PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MIrv = PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MFault_hirv;
										else
											PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MIrv = PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MFault_lirv;
									}
									else
										PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MControl_type = 3;
								}
							}
						}
					}
				}
			}
		}
	}
	else
	{
		//LogErr( "IRVBroken" );
		MessageBox(NULL, "IRVBroken", "", MB_OK);
		_flag_PBroken = 0;
	}
*/
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
	{
		_temp = MGET( _lpmu, _j, 0 );
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type ==1 )
		{
			if( _temp < PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMin ||
				_temp > PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMax )
			{
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MPDirty = 1;
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MCT_history =
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type;
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type = 8;
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSatuation = 1;
			}
		}
	}

	if( lpCobj_All_->Flag_ReSP == 1 )
	{
		for( _i = 0; _i < lpCobj_All_->p; _i++ )
			if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDirty == 1 )
			{
				if( (PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 1 &&
						PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CCT_history == 1) ||
					(PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 2 &&
						PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CCT_history == 2) )
						PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDirty = 0;

			}
		for( _j = 0; _j < lpCobj_All_->m; _j++ )
		{
			if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MRDirty == 1 )
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MRDirty = 0;
			if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MPDirty == 1 )
			{
				if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type ==
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MCT_history )
					if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MCT_history == 1 ||
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MCT_history == 2 )
                        PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MRDirty = 0;
			}
		}
	}
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
	{
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MPBroken = 0;
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MRBroken = 0;
	}
	for( _i = 0; _i < lpCobj_All_->p; _i++ )
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CQBroken = 0;
	MATRIXFREE( _lpmu );
	MATRIXFREE( _lpmdelu );
//lpCobj_All_->Freecount has been changed;
	return lpCobj_All_->Freecount;
}
