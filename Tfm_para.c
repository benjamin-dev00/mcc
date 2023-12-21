#include "tfalgo.h"
#include "tfcmdmc.h"
#ifdef	__ONSPEC
#include "mtmfun.h"
#include "mtdllfun.h" 
#endif
#ifdef __debug
#include "tfdebug.h"
#endif

ACRET MAKE_PARAM( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpCobj_PrivMV_,
						LPCOBJPRIVCV lpCobj_PrivCV_ )
{
	LPMATRIX    _lpmA, _lpmP, _lpmQ, _lpmR;
	LPMATRIX	_lpmRaqa, _lpmRadd, _lpmAb, _lpmmul2, _lpmmul3;
	LPDIMMATRIX _lpmSR;
	int			_i, _j, _kk, _l, _Rm = 0, _addcol, _Real_i, _Real_j;
	double		_temp;
	LPMATRIX	_lpmRA, _lpmRAt, _lpmRT, _lpmRTt, _lpmRTP, _lpmRAtQ, _lpmRR, _lpmRD;
	LPMATRIX	_lpmRP, _lpmDrow, _lpmRmul2, _lpmRmul3;
	LPMATRIX	_lpTempMVCT, _lpTempCVCT;
#ifdef __debug
	double		_arrayq[pp][PP], _arrayP[mm][MM], _arrayr[mm][MM];
#endif
//	LPVARCV		lpPrivCV_[lpCobj_All_->p];
//	LPVARMV		lpPrivMV_[lpCobj_All_->m];

	if( lpCobj_All_->Flag_ReSP == 2 )
	{
		_lpTempMVCT = MatrixAlloc( lpCobj_All_->m, 1 );
		_lpTempCVCT = MatrixAlloc( lpCobj_All_->p, 1 );
		for( _i = 0; _i < lpCobj_All_->p; _i++ )
			MGET( _lpTempCVCT, _i, 0 ) = (double)
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CCT_history;
		for( _j = 0; _j < lpCobj_All_->m; _j++ )
			MGET( _lpTempMVCT, _j, 0 ) = (double)
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MCT_history;
	}
	_lpmP 		= MAKE_MATRIX( lpCobj_All_->LPS );
	_lpmQ 		= MAKE_MATRIX( lpCobj_All_->LPQ );
	_lpmR 		= MAKE_MATRIX( lpCobj_All_->LPR );

	_lpmAb = MAKE_MATRIX(lpCobj_All_->LPAb);
	_lpmA = MAKE_MATRIX(lpCobj_All_->LPA);
	_lpmSR = MAKE_DIMMATRIX(lpCobj_All_->LPTempSR);

	for( _j = 0; _j < lpCobj_All_->m; _j++ )
	{
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MPDirty == 1 )
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MPDirtyCount++;
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MRDirty == 1 )
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MRDirtyCount++;
	}

	for( _i = 0; _i < lpCobj_All_->p; _i++ )
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDirty == 1 )
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CQDirtyCount++;
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MPDirty == 1 ||
				lpCobj_All_->Count == 1 )
		{
/*			if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 4 ||
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 5 ||
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 7 )
				for ( _i = 0; _i < lpCobj_All_->p; _i++)
					for ( _kk = 0; _kk < lpCobj_All_->N; _kk++)
						DIMMGET( _lpmSR, _i, _j, _kk ) = 0;
*/
			if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 4 ||
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MCT_history == 4 ||
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 5 ||
							PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MCT_history == 5 ||
								PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 7 ||
									PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MCT_history == 7 ||
										PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 8 ||
											PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MCT_history == 8 ||
												lpCobj_All_->Count == 1 )
			{
				for (_i = 0; _i < ( (lpCobj_All_->p)*(lpCobj_All_->P) ); _i++)
				{
					_Real_i = _i / lpCobj_All_->P;
					for (_l = _j*lpCobj_All_->M; _l < ((_j+1)*lpCobj_All_->M); _l++)
					{
						_kk = (_i - _i/lpCobj_All_->P*lpCobj_All_->P - _l + _l/lpCobj_All_->M*lpCobj_All_->M + 
								PRIVCVGET( lpCobj_PrivCV_, _Real_i, 0 )->CN );
						if( _kk >= 0 )
							MGET(_lpmA, _i, _l) = 
							DIMMGET(_lpmSR, _i/lpCobj_All_->P, _l/lpCobj_All_->M, (_i-_i/lpCobj_All_->P*lpCobj_All_->P-_l+_l/lpCobj_All_->M*lpCobj_All_->M + PRIVCVGET( lpCobj_PrivCV_, _Real_i, 0 )->CN ));
						else
							MGET(_lpmA, _i, _l)  = 0.0;
					}
				}

				for (_i = 0; _i < lpCobj_All_->p * lpCobj_All_->N; _i++)
					for (_l = 0; _l < lpCobj_All_->m; _l++)
						MGET(_lpmAb, _i, _l) = DIMMGET(_lpmSR, _i/lpCobj_All_->N, _l, (_i-_i/lpCobj_All_->N*lpCobj_All_->N));
			}
		}

//note: when the Matrix R will be freed?

	if( lpCobj_All_->Count == 1 || lpCobj_All_->Flag_ReSP >= 1 || lpCobj_All_->Flag_AM == 2 )
	{
		for( _i = 0; _i < lpCobj_All_->m; _i++ )
		{
			PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MRDirty = 1;
			PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MPDirty = 1;
			PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MCT_history = 4;
		}
		for( _j = 0; _j < lpCobj_All_->p; _j++ )
		{
			PRIVCVGET( lpCobj_PrivCV_, _j, 0 )->CDirty = 1;
			PRIVCVGET( lpCobj_PrivCV_, _j, 0 )->CCT_history = 4;
		}
// here is not needed maybe
	}

	for( _i = 0; _i < lpCobj_All_->m; _i++ )
		if( PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MRDirty == 1 )
		{
			if( lpCobj_All_->Count != 1 && lpCobj_All_->Flag_ReSP == 0 && lpCobj_All_->Flag_AM != 2 )
			{
				for (_j = 0; _j < lpCobj_All_->M; _j++)
					MGET( _lpmR, (_i*lpCobj_All_->M+_j), (_i*lpCobj_All_->M+_j) ) +=
						PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MDelR;
			}
			else
			{
				for (_kk = 0; _kk < lpCobj_All_->M; _kk++)
				{
					MGET( _lpmR, (_i*lpCobj_All_->M+_kk), (_i*lpCobj_All_->M+_kk) ) =
						PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MR;
				}
			}
		}


/*	for( _i = 0; _i < lpCobj_All_->m; _i++ )
		if( PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MRDirty == 1 )
			for (_j = 0; _j < lpCobj_All_->M; _j++)
			{
				_temp = MGET( _lpmR, (_i*lpCobj_All_->M+_j), (_i*lpCobj_All_->M+_j) );
				if( _temp >= PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MRMax )
				{
					_temp = PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MRMax;
					MGET( _lpmR, (_i*lpCobj_All_->M+_j), (_i*lpCobj_All_->M+_j) ) = _temp;
				}
			}*/
//Satuation is considered later, not now

	for( _i = 0; _i < lpCobj_All_->m; _i++ )
		if( PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MRDirty == 1 )
			PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MRDirty = 0;

 ///hereherehehrhehrehrkehskajhfkdsajklfjdsakjflksajkfjdska
	if(!lpCobj_All_->Flag_Weight )
	{
		for( _i = 0; _i < lpCobj_All_->p; _i++ )
			if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDirty == 1 )
			{
				if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 1 )
				{
					for (_j = 0; _j < lpCobj_All_->P; _j++)
					{
						if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CCT_history == 1 )
							MGET( _lpmQ, (_i*lpCobj_All_->P+_j) , (_i*lpCobj_All_->P+_j) ) +=
								PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDelQ;
						else
							MGET( _lpmQ, (_i*lpCobj_All_->P+_j), ( _i*lpCobj_All_->P+_j) ) =
								PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CQ;
					}
				}

				if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 2 )
				{
					for (_j = 0; _j < lpCobj_All_->P; _j++)
					{
						if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CCT_history == 2 )
							MGET(_lpmQ, (_i*lpCobj_All_->P+_j) , (_i*lpCobj_All_->P+_j) ) +=
								PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDelQ;
						else
							MGET(_lpmQ, (_i*lpCobj_All_->P+_j) , (_i*lpCobj_All_->P+_j) ) =
								PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CQL;
					}
				}

				if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 4 ||
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 3 )
					for (_j = 0; _j < lpCobj_All_->P; _j++)
						MGET(_lpmQ, (_i*lpCobj_All_->P+_j) , (_i*lpCobj_All_->P+_j) ) = 0.0;

			}
	}
	else
	{
		for( _i = 0; _i < lpCobj_All_->p; _i++ )
			if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDirty == 1 )
			{
				if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 1 || 
						PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 2 )
				{
					if( (PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CData < 
						PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_high ) && 
							(PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CData > 
								PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_low) )
					{
/*						if( lpCobj_All_->Count != 1 && 
								PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type != 1 &&
									lpCobj_All_->Flag_Recount == 0 &&
										lpCobj_All_->MParaCount != lpCobj_All_->MParaMax && 
										lpCobj_All_->Flag_AM != 2 ) 
						{
							MessageBox( NULL, "CVERROR", "tfm_para.c", MB_OK );
							lpCobj_All_->Flag_Run = 0;
							return FALSE;
						}*/
						if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 2 )
						{
							for (_kk = 0; _kk < lpCobj_All_->P; _kk++)
								MGET( _lpmQ, (_i*lpCobj_All_->P+_kk), (_i*lpCobj_All_->P+_kk) ) =
									PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CQL;
						}
						else
						{
							for (_kk = 0; _kk < lpCobj_All_->P; _kk++)
								MGET( _lpmQ, (_i*lpCobj_All_->P+_kk), (_i*lpCobj_All_->P+_kk) ) =
									PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CQ;
						}

					}
					if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CData >= 
						  PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_high )
						if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CData >= 
							PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHigh )
						{
							for (_kk = 0; _kk < lpCobj_All_->P; _kk++)
							{
								MGET( _lpmQ, (_i*lpCobj_All_->P+_kk), (_i*lpCobj_All_->P+_kk) ) =
									PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CQMax;
							}
						}
						else
						{
							_temp = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CQMax * 
								( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CData - 
									PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_high ) /
								( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHigh - 
									PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_high );
							if( _temp < PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CQL )
								if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 2 )
								{
									_temp = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CQL;
								}
								else
									_temp = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CQ;

							for (_kk = 0; _kk < lpCobj_All_->P; _kk++)
								MGET( _lpmQ, (_i*lpCobj_All_->P+_kk), (_i*lpCobj_All_->P+_kk) ) = _temp;
								
						}

					if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CData <= 
							PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_low )
						if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CData <= 
							PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLow )
						{
							for (_kk = 0; _kk < lpCobj_All_->P; _kk++)
								MGET( _lpmQ, (_i*lpCobj_All_->P+_kk), (_i*lpCobj_All_->P+_kk) ) =
									PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CQMax;
						}
						else
						{
							_temp = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CQMax * 
								( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CData - 
									PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_low ) /
								( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLow - 
									PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_low );
							if( _temp < PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CQL )
								if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 2 )
								{
									_temp = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CQL;
								}
								else
									_temp = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CQ;
							for (_kk = 0; _kk < lpCobj_All_->P; _kk++)
								MGET( _lpmQ, (_i*lpCobj_All_->P+_kk), (_i*lpCobj_All_->P + _kk) ) = _temp;
						}
				}
				if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 4 ||
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 3 )
					for (_j = 0; _j < lpCobj_All_->P; _j++)
						MGET(_lpmQ, (_i*lpCobj_All_->P+_j) , (_i*lpCobj_All_->P+_j) ) = 0.0;

			}
	
	}
	for( _i = 0; _i < lpCobj_All_->p; _i++ )
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDirty == 1 )
		{
//	Note by ttf, 98, 12, 5, Just for test the max Q it could reach, same as, R and P
//			for (_j = 0; _j < lpCobj_All_->P; _j++)
//			{
//				_temp = MGET( _lpmQ, (_i*lpCobj_All_->P+_j), (_i*lpCobj_All_->P+_j) );
//				if( _temp > PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CQMax )
//				{
//					_temp = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CQMax;
//					MGET( _lpmQ, (_i*lpCobj_All_->P+_j), (_i*lpCobj_All_->P+_j) ) = _temp;
//				}
//			}
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDirty = 0;
		}

	if( !lpCobj_All_->Flag_Weight )
	{
		for( _i = 0; _i < lpCobj_All_->m; _i++ )
			if( PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MPDirty == 1 )
			{
				if( PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MControl_type == 1 )
					for (_j = 0; _j < lpCobj_All_->M; _j++)
						if( PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MCT_history == 1 ||
									PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MCT_history == 8 )
							MGET( _lpmP, (_i*lpCobj_All_->M+_j), (_i*lpCobj_All_->M+_j) ) +=
								PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MDelP;
						else
							MGET( _lpmP, (_i*lpCobj_All_->M+_j), (_i*lpCobj_All_->M+_j) ) =
								PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MP;

				if( PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MControl_type == 2 )
					for (_j = 0; _j < lpCobj_All_->M; _j++)
						if( PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MCT_history == 2 ||
								(PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MCT_history == 8 &&
									PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MLastCM == 2) )
							MGET( _lpmP, (_i*lpCobj_All_->M+_j), (_i*lpCobj_All_->M+_j) ) +=
								PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MDelP;
						else
							MGET( _lpmP, (_i*lpCobj_All_->M+_j), (_i*lpCobj_All_->M+_j) ) =
								PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MPL;

				if( PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MControl_type == 3 )
					for (_j = 0; _j < lpCobj_All_->M; _j++)
						MGET( _lpmP, _i*lpCobj_All_->M+_j, _i*lpCobj_All_->M+_j ) = 0.0;
				if( PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MControl_type == 4 ||
						PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MControl_type == 5 ||
							PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MControl_type == 7 )
					for (_j = 0; _j < lpCobj_All_->M; _j++)
						MGET( _lpmP, _i*lpCobj_All_->M+_j, _i*lpCobj_All_->M+_j ) = 0.0;

	//note: if MV exceed the limit, delp is added twice!
				if( PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MControl_type == 8 )
				{
					if( PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MCT_history == 1 ||
							PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MCT_history == 2 )
						for (_j = 0; _j < lpCobj_All_->M; _j++)
							MGET( _lpmP, (_i*lpCobj_All_->M+_j), (_i*lpCobj_All_->M+_j) ) +=
								PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MDelP;
					else
					{
						for (_j = 0; _j < lpCobj_All_->M; _j++)
							MGET( _lpmP, (_i*lpCobj_All_->M+_j), (_i*lpCobj_All_->M+_j) ) =
								PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MPL;
					}
				}
			}
	}
	else
	{
		for( _i = 0; _i < lpCobj_All_->m; _i++ )
			if( PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MPDirty == 1 )
			{
				if( PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MControl_type == 1 ||
						PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MControl_type == 2 ||
							PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MControl_type == 8 )
				{
					if( (PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MData <= 
						PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MIrvHigh) && 
							(PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MData >= 
								PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MIrvLow) )
					{
/*						if( lpCobj_All_->Count != 1 && 
								PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MControl_type != 1 &&
									lpCobj_All_->Flag_Recount == 0 &&
										lpCobj_All_->MParaCount != lpCobj_All_->MParaMax &&
										lpCobj_All_->Flag_AM != 2 ) 
						{
							MessageBox( NULL, "ERROR", "tfm_para.c", MB_OK );
							lpCobj_All_->Flag_Run = 0;
							return FALSE;
						}*/
						for (_kk = 0; _kk < lpCobj_All_->M; _kk++)
							MGET( _lpmP, (_i*lpCobj_All_->M+_kk), (_i*lpCobj_All_->M+_kk) ) =
								PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MP;
					}
					if( PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MData > 
						PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MIrvHigh )
					{
//						if( PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MData > 
	//						PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MIrvHigh )
	//					{
							for (_kk = 0; _kk < lpCobj_All_->M; _kk++)
							{
								MGET( _lpmP, (_i*lpCobj_All_->M+_kk), (_i*lpCobj_All_->M+_kk) ) =
									PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MPMax;
							}
	/*					}
						else
						{
							_temp = PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MPMax * 
								( PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MData - 
									PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MIrvFault_high ) /
								( PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MIrvHigh - 
									PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MIrvFault_high );
							for (_kk = 0; _kk < lpCobj_All_->M; _kk++)
								MGET( _lpmP, (_i*lpCobj_All_->M+_kk), (_i*lpCobj_All_->M+_kk) ) = _temp;
								
						}*/
					}
					if( PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MData < 
						PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MIrvLow )
					{
/*						if( PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MData <= 
							PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MIrvLow )
						{*/
							for (_kk = 0; _kk < lpCobj_All_->M; _kk++)
								MGET( _lpmP, (_i*lpCobj_All_->M+_kk), (_i*lpCobj_All_->M+_kk) ) =
									PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MPMax;
						/*}
						else
						{
							_temp = PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MPMax * 
								( PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MData - 
									PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MIrvFault_low ) /
								( PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MIrvLow - 
									PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MIrvFault_low );
							for (_kk = 0; _kk < lpCobj_All_->M; _kk++)
								MGET( _lpmP, (_i*lpCobj_All_->M+_kk), (_i*lpCobj_All_->M+_kk) ) = _temp;
						}*/
					}
				}
				if( PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MControl_type == 3 ||
						PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MControl_type == 4 ||
							PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MControl_type == 5 ||
								PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MControl_type == 7 )
				{
					for (_j = 0; _j < lpCobj_All_->M; _j++)
						MGET( _lpmP, (_i*lpCobj_All_->M+_j), (_i*lpCobj_All_->M+_j) ) = 0.0;
				}
			}

	}
	for( _i = 0; _i < lpCobj_All_->m; _i++ )
		if( PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MPDirty == 1 )
		{
/*			for (_j = 0; _j < lpCobj_All_->M; _j++)
			{
				_temp = MGET( _lpmP, (_i*lpCobj_All_->M+_j), (_i*lpCobj_All_->M+_j) );
				if( _temp >= PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MPMax )
				{
					_temp = PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MPMax;
					MGET( _lpmP, (_i*lpCobj_All_->M+_j), (_i*lpCobj_All_->M+_j) ) = _temp;
				}
			}*/
			PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MPDirty = 0;
		}

#ifdef __debug
	for( _i = 0; _i < lpCobj_All_->p; _i++ )
		for (_j = 0; _j < lpCobj_All_->P; _j++)
			_arrayq[_i][_j] = MGET( _lpmQ, (_i*lpCobj_All_->P+_j), (_i*lpCobj_All_->P+_j) );
	for( _i = 0; _i < lpCobj_All_->m; _i++ )
		for (_j = 0; _j < lpCobj_All_->M; _j++)
			_arrayr[_i][_j] = MGET( _lpmR, (_i*lpCobj_All_->M+_j), (_i*(lpCobj_All_->M)+_j) );

	for( _i = 0; _i < lpCobj_All_->m; _i++ )
		for (_j = 0; _j < lpCobj_All_->M; _j++)
		_arrayP[_i][_j] = MGET( _lpmP, (_i*lpCobj_All_->M+_j), (_i*lpCobj_All_->M+_j) );
#endif
	_Rm = 0;
	_lpmmul2 = MAKE_MATRIX( lpCobj_All_->LPMul2 );
	_lpmmul3 = MAKE_MATRIX( lpCobj_All_->LPMul3 );
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type != 4 &&
			 PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type != 5 &&
			 PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type != 7 )
			_Rm++;

	if( _Rm == 0 )
	{
    	lpCobj_All_->Flag_MCVRecount = 0;
		if ( PRIVMVGET( lpCobj_PrivMV_, 0, 0 )->MControl_type == 5 )
		{
			for( _i = 0; _i < lpCobj_All_->m * lpCobj_All_->M; _i++ )
				for( _j = 0; _j < lpCobj_All_->p * lpCobj_All_->P; _j++ )
					MGET( _lpmmul2, _i, _j ) = 0.0;

			for( _i = 0; _i < lpCobj_All_->m * lpCobj_All_->M; _i++ )
				for( _j = 0; _j < lpCobj_All_->m * lpCobj_All_->M; _j++ )
					MGET( _lpmmul3, _i, _j ) = 0.0;
		}
		goto __return;
	}
//note:  MV is satuation or DV or bad, no need to recount
//			invert of Matrix
	_lpmRAt  = MatrixAlloc( _Rm * lpCobj_All_->M, lpCobj_All_->p* lpCobj_All_->P );
	_lpmRA 	= MatrixAlloc( lpCobj_All_->p*lpCobj_All_->P, lpCobj_All_->M*_Rm );
	_lpmRAtQ = MatrixAlloc( _Rm*lpCobj_All_->M, lpCobj_All_->p*lpCobj_All_->P );
	_lpmRT 	= MatrixAlloc( lpCobj_All_->M*_Rm, lpCobj_All_->M*_Rm );
	_lpmRTP	= MatrixAlloc( (lpCobj_All_->M*_Rm), (lpCobj_All_->M*_Rm));
	_lpmRTt 	= MatrixAlloc( lpCobj_All_->M*_Rm, lpCobj_All_->M*_Rm );
	_lpmRR 	= MatrixAlloc( lpCobj_All_->M*_Rm, lpCobj_All_->M*_Rm );
	_lpmRP 	= MatrixAlloc( lpCobj_All_->M*_Rm, lpCobj_All_->M*_Rm );
	_lpmRD 	= MatrixAlloc( lpCobj_All_->M*_Rm, lpCobj_All_->M*_Rm );
	_lpmDrow	= MatrixAlloc( lpCobj_All_->M*_Rm, lpCobj_All_->M*lpCobj_All_->m );
	_lpmRmul3= MatrixAlloc( lpCobj_All_->M*_Rm, lpCobj_All_->M*_Rm );
	_lpmRmul2= MatrixAlloc( _Rm*lpCobj_All_->M, lpCobj_All_->p*lpCobj_All_->P );

	_lpmRaqa 	= MatrixAlloc( (lpCobj_All_->M*_Rm), (lpCobj_All_->M*_Rm));
	_lpmRadd  = MatrixAlloc( lpCobj_All_->M*_Rm, lpCobj_All_->M*_Rm );
	_addcol = 0;
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 4 ||
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 5 ||
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 7 )
			_addcol++;
		else
		{
			for( _kk = 0; _kk < lpCobj_All_->p*lpCobj_All_->P; _kk++ )
				for( _l = (_j-_addcol)*lpCobj_All_->M; _l < (_j+1-_addcol)*lpCobj_All_->M; _l++ )
					MGET( _lpmRA, _kk, _l ) = MGET( _lpmA, _kk, _l+_addcol*lpCobj_All_->M );
		}
	mtrans( _lpmRAt, _lpmRA );
	mmultiple( _lpmRAtQ, _lpmRAt, _lpmQ );
	mmultiple(_lpmRaqa, _lpmRAtQ, _lpmRA);
	_addcol = 0;
//here R should be diagnal matrix
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 4 ||
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 5 ||
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 7 )
			_addcol++;
		else
		{
			for( _kk = (_j-_addcol)*lpCobj_All_->M; _kk < (_j-_addcol+1)*lpCobj_All_->M; _kk++ )
				for( _l = (_j-_addcol)*lpCobj_All_->M; _l < (_j-_addcol+1)*lpCobj_All_->M; _l++ )
				{
					MGET( _lpmRR, _kk, _l ) =
						MGET( _lpmR, _kk+_addcol*lpCobj_All_->M, _l+_addcol*lpCobj_All_->M);
					MGET( _lpmRP, _kk, _l ) =
						MGET( _lpmP, _kk+_addcol*lpCobj_All_->M, _l+_addcol*lpCobj_All_->M);
				}
		}
	madd(_lpmRadd, _lpmRaqa, _lpmRR);
	for( _i = 0; _i < (_Rm*lpCobj_All_->M); _i++ )
		for( _j = 0; _j < (_Rm*lpCobj_All_->M); _j++ )
			if( (_i/lpCobj_All_->M) == (_j/lpCobj_All_->M) )
				if(_i >= _j)
					MGET(_lpmRT, _i, _j) = 1.0;
	mtrans( _lpmRTt, _lpmRT );
	mmultiple( _lpmRTP, _lpmRTt, _lpmRP );
	mmultiple(_lpmRTt, _lpmRTP, _lpmRT);
	madd( _lpmRadd, _lpmRadd, _lpmRTt );
	minvert(_lpmRD, _lpmRadd);

	mmultiple(_lpmRmul2, _lpmRD, _lpmRAtQ );
	mmultiple(_lpmRmul3, _lpmRD, _lpmRTP);
	_addcol = 0;
	for( _kk = 0; _kk < lpCobj_All_->m * lpCobj_All_->M; _kk++ )
		for( _l = 0; _l < lpCobj_All_->p * lpCobj_All_->P; _l++ )
			MGET( _lpmmul2, _kk, _l ) = 0.0;

	for( _j = 0; _j < lpCobj_All_->m; _j++ )
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 4 ||
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 5 ||
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 7 )
			_addcol++;
		else
		{
			for( _kk = _j*lpCobj_All_->M; _kk < (_j+1) * lpCobj_All_->M; _kk++ )
				for( _l = 0; _l < lpCobj_All_->p * lpCobj_All_->P; _l++ )
         			MGET( _lpmmul2, _kk, _l ) = MGET( _lpmRmul2, _kk-_addcol*lpCobj_All_->M, _l );
		}
	_addcol = 0;
	for( _kk = 0; _kk < lpCobj_All_->m*lpCobj_All_->M; _kk++ )
		for( _l = 0; _l < lpCobj_All_->M*lpCobj_All_->m; _l++ )
			MGET( _lpmmul3, _kk, _l ) = 0.0;

	for( _j = 0; _j < lpCobj_All_->m; _j++ )
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 4 ||
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 5 ||
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 7 )
			_addcol++;
		else
		{
			for( _kk = 0; _kk < _Rm*lpCobj_All_->M; _kk++ )
				for( _l = (_j*lpCobj_All_->M); _l < (_j+1)*lpCobj_All_->M; _l++ )
					MGET( _lpmDrow, _kk, _l ) =
							MGET( _lpmRmul3, _kk, _l-_addcol*lpCobj_All_->M );
		}
	_addcol = 0;
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 4 ||
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 5 ||
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 7 )
		{
			_addcol++;
		}
		else
		{
			for( _kk = (_j*lpCobj_All_->M); _kk < (_j+1)*lpCobj_All_->M; _kk++ )
				for( _l = 0; _l < lpCobj_All_->m*lpCobj_All_->M; _l++ )
					MGET( _lpmmul3, _kk, _l ) =
							MGET( _lpmDrow, _kk-_addcol*lpCobj_All_->M, _l );
		}

//note: by ttf, should be recoved..........
	for( _i = 0; _i < lpCobj_All_->p; _i++ )
	{
		_Real_i = (int)MGET( ( COBJ_2_MATRIX( lpCobj_All_->LPApartCV ) ), lpCobj_All_->Apart_i, _i ) - 1;
		WRITECVPARAMETER( lpCobj_All_->LPCV,(WORD)_Real_i, CV_REALQ, REAL, (float)MGET( _lpmQ, _i*lpCobj_All_->P, _i*lpCobj_All_->P ) );
	}
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
	{
		_Real_j = (int)MGET( ( COBJ_2_MATRIX( lpCobj_All_->LPApartMV ) ), lpCobj_All_->Apart_i, _j ) - 1;
		WRITEMVPARAMETER( lpCobj_All_->LPMV,(WORD)_Real_j, MV_REALR, REAL, (float)MGET( _lpmR, _j*lpCobj_All_->M, _j*lpCobj_All_->M ) );
		WRITEMVPARAMETER( lpCobj_All_->LPMV,(WORD)_Real_j, MV_REALS, REAL, (float)MGET( _lpmP, _j*lpCobj_All_->M, _j*lpCobj_All_->M ) );
	}
	MATRIXFREE( _lpmRaqa );
	MATRIXFREE( _lpmRadd );
	MATRIXFREE( _lpmRAt );
	MATRIXFREE( _lpmRD );
	MATRIXFREE( _lpmRA );
	MATRIXFREE( _lpmRAtQ );
	MATRIXFREE( _lpmRT );
	MATRIXFREE( _lpmRTP );
	MATRIXFREE( _lpmRTt );
	MATRIXFREE( _lpmRR );
	MATRIXFREE( _lpmRP );
	MATRIXFREE( _lpmDrow );
	MATRIXFREE( _lpmRmul2 );
	MATRIXFREE( _lpmRmul3 );
__return:

	lpCobj_All_->MakeParaCount++;
	if ( lpCobj_All_->Flag_AM == 2 )
		lpCobj_All_->Flag_AM = 3;
	if( lpCobj_All_->Flag_ReSP == 2 )
	{
		lpCobj_All_->Flag_ReSP = 0;
		for( _i = 0; _i < lpCobj_All_->p; _i++ )
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CCT_history = 
				(WORD)MGET( _lpTempCVCT, _i, 0 );
		for( _j = 0; _j < lpCobj_All_->m; _j++ )
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MCT_history = 
				(WORD)MGET( _lpTempMVCT, _j, 0 );
		WRITEALLPARAMETER( lpCobj_All_, MDMCIRVD_FLAGRESP, BOOL, 0 );
		MATRIXFREE( _lpTempMVCT );
		MATRIXFREE( _lpTempCVCT );
	}
   	lpCobj_All_->Flag_MCVRecount = 0;
	return	TRUE;
}

