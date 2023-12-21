//note: is priority should be considered???????????

#include "tfalgo.h"
#include "tfcmdmc.h"
#ifdef debug
#include "tfdebug.h"
#endif
int 		IsmypredOver( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpCobj_PrivMV_,
							LPCOBJPRIVCV lpCobj_PrivCV_ )
{
	LPMATRIX	_lpmA, _lpmdelypredP, _lpmypredPNew, _lpmypredP, _lpmypredN;
	int			_i, _j, _addCV, _k, _Real_i;
	double		_temp, _tempCond;
	WORD		_flag_recount = 0;
#ifdef debug
	double		_mdeluM[mm][MM], _mypredP[pp][PP];
#endif

	_lpmypredN = MAKE_MATRIX( lpCobj_All_->LPPredN );
	_lpmypredP = MatrixAlloc(lpCobj_All_->p*lpCobj_All_->P, 1);
	_lpmdelypredP = MatrixAlloc( (lpCobj_All_->p*lpCobj_All_->P), 1 );
	_lpmypredPNew = MatrixAlloc( lpCobj_All_->p*lpCobj_All_->P, 1 );
	for(_i = 0; _i < lpCobj_All_->p*lpCobj_All_->P; _i++)
	{
		_Real_i = _i / lpCobj_All_->P;
		MGET(_lpmypredP, _i, 0) = MGET(_lpmypredN,
				(_i/lpCobj_All_->P*lpCobj_All_->N+_i-
					_i/lpCobj_All_->P*lpCobj_All_->P + 
						PRIVCVGET( lpCobj_PrivCV_, _Real_i, 0 )->CN ), 0);
	}
	_lpmA = MAKE_MATRIX( lpCobj_All_->LPA );
#ifdef debug
	for( _i = 0; _i < lpCobj_All_->p; _i++ )
		for( _j = 0; _j < lpCobj_All_->P; _j++ )
			_mypredP[_i][_j] = MGET( _lpmypredP, _i*lpCobj_All_->P+_j, 0 );
#endif
	mmultiple( _lpmdelypredP, _lpmA, ( COBJ_2_MATRIX( lpCobj_All_->LPDeluM ) ));
	madd( _lpmypredPNew, _lpmypredP, _lpmdelypredP );

#ifdef debug
	for( _i = 0; _i < lpCobj_All_->p; _i++ )
		for( _j = 0; _j < lpCobj_All_->P; _j++ )
			_mypredP[_i][_j] = MGET( _lpmypredPNew, _i*lpCobj_All_->P+_j, 0 );

	for( _i = 0; _i < lpCobj_All_->m; _i++ )
		for( _j = 0; _j < MM; _j++ )
			_mdeluM[_i][_j] = MGET( ( COBJ_2_MATRIX( lpCobj_All_->LPDeluM ) ), _i*MM+_j, 0 );

	for( _i = 0; _i < lpCobj_All_->p; _i++ )
		for( _j = 0; _j < lpCobj_All_->P; _j++ )
			_mypredP[_i][_j] = MGET( _lpmdelypredP, _i*lpCobj_All_->P+_j, 0 );
#endif
	_flag_recount = 0;
	if( lpCobj_All_->Flag_CControl == 1 )
		CVPrioritySort( lpCobj_All_, lpCobj_PrivCV_ );

	for( _i = 0; _i < lpCobj_All_->p; _i++ )
	{
		if( lpCobj_All_->Flag_CControl == 1 )
			_j = (int)MGET( ( COBJ_2_MATRIX( lpCobj_All_->LPCVSort ) ), ( lpCobj_All_->p - _i - 1 ), 0 );
		else
			_j = _i;

		_flag_recount = 0;
		if( PRIVCVGET( lpCobj_PrivCV_, _j, 0 )->CControl_type != 4 )
			for( _k = 0; _k < lpCobj_All_->P; _k++)
				if( _flag_recount == 0 )
				{
					_temp = MGET( _lpmypredPNew, (_j*lpCobj_All_->P+_k), 0 );
	
					if( ( PRIVCVGET( lpCobj_PrivCV_, _j, 0 )->CControl_type == 2 ||
						PRIVCVGET( lpCobj_PrivCV_, _j, 0 )->CControl_type == 3 ) && 
							PRIVCVGET( lpCobj_PrivCV_, _j, 0 )->CNoIll == 0 )
					{
						if( PRIVCVGET( lpCobj_PrivCV_, _j, 0 )->CNoLP == 1 &&
								lpCobj_All_->Flag_CControl == 1 )
								continue;
						PRIVCVGET( lpCobj_PrivCV_, _j, 0 )->CCT_history =
							PRIVCVGET( lpCobj_PrivCV_, _j, 0 )->CControl_type;
						if( _temp < PRIVCVGET( lpCobj_PrivCV_, _j, 0 )->CFault_low ||
							_temp > PRIVCVGET( lpCobj_PrivCV_, _j, 0 )->CFault_high )
						{
							_addCV = 0;
							if( PRIVCVGET( lpCobj_PrivCV_, _j, 0 )->CControl_type == 3 && 
									PRIVCVGET( lpCobj_PrivCV_, _j, 0 )->CNoIll == 0 )
							{
								if( lpCobj_All_->Flag_CControl == 1 )
								{
									if( lpCobj_All_->Freecount > 0 && PRIVCVGET( lpCobj_PrivCV_, _j, 0 )->CNoLP == 0 )
									{
										PRIVCVGET( lpCobj_PrivCV_, _j, 0 )->CControl_type = 2;
										_tempCond = GetConditionNumber( lpCobj_All_, 
												lpCobj_PrivMV_, lpCobj_PrivCV_ );
										if( _tempCond < lpCobj_All_->Big_Cond )
										{
											_addCV = 1;
											lpCobj_All_->Freecount--;
										}
										else
											PRIVCVGET( lpCobj_PrivCV_, _j, 0 )->CControl_type = 3;
									}
								}
								else
								{
									PRIVCVGET( lpCobj_PrivCV_, _j, 0 )->CControl_type = 2;
									_tempCond = GetConditionNumber( lpCobj_All_, 
											lpCobj_PrivMV_, lpCobj_PrivCV_ );
									if( _tempCond < lpCobj_All_->Big_Cond )
										_addCV = 1;
									else
										PRIVCVGET( lpCobj_PrivCV_, _j, 0 )->CControl_type = 3;
								}
							}
							else
								_addCV = 1;

							if( _addCV == 1 )
							{
								_flag_recount = 1;
								PRIVCVGET( lpCobj_PrivCV_, _j, 0 )->CDirty = 1;
								if( _temp < PRIVCVGET( lpCobj_PrivCV_, _j, 0 )->CFault_low )
									PRIVCVGET( lpCobj_PrivCV_, _j, 0 )->CLOverCount++;
								else
									PRIVCVGET( lpCobj_PrivCV_, _j, 0 )->CHOverCount++;
							}
						}
					}
					else
					{
						if( _temp < PRIVCVGET( lpCobj_PrivCV_, _j, 0 )->CFault_min ||
							_temp > PRIVCVGET( lpCobj_PrivCV_, _j, 0 )->CFault_max )
						{
							if( PRIVCVGET( lpCobj_PrivCV_, _j, 0 )->CControl_type == 1 && !lpCobj_All_->Flag_Weight )
							{
								_flag_recount = 1;
								PRIVCVGET( lpCobj_PrivCV_, _j, 0 )->CDirty = 1;
								PRIVCVGET( lpCobj_PrivCV_, _j, 0 )->CCT_history = 1;
								if( _temp < PRIVCVGET( lpCobj_PrivCV_, _j, 0 )->CFault_min )
									PRIVCVGET( lpCobj_PrivCV_, _j, 0 )->CLOverCount++;
								else
									PRIVCVGET( lpCobj_PrivCV_, _j, 0 )->CHOverCount++;
							}
						}
					}
				}
	}

	MATRIXFREE( _lpmypredPNew );
	MATRIXFREE( _lpmdelypredP );
	MATRIXFREE( _lpmypredP );


	return	lpCobj_All_->Freecount;
}

