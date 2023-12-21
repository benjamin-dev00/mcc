#include "tfalgo.h"
#include "tfcmdmc.h"
#include "tfcond.h"
#ifdef __ONSPEC
#include "Mtmfun.h"
#include "Mtdllfun.h"
#endif


//note: if there are 2 ill-condioned cv, then....?????!!!
void RemoveIll( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpCobj_PrivMV_, LPCOBJPRIVCV lpCobj_PrivCV_ )
{
	int 		_i, _j, _row = 0, _col = 0, _kk, _num, _addrow, _addcol, _l, _freecount, _k, _Real_i;
	double		_Cond_Num, _eigmax, _eigmin;
	WORD 		_flag_ill = 1;
	LPMATRIX	_lpmu, _lpmv, _lpmG, _lpmGtemp;
	LPCOBJ		_lpCobjG;
	LPDIMMATRIX	_lpmSR;
	BOOL		_bOK = TRUE, _flag_found, _FlagFound = 1;
	WORD		_MaxIllCount = 0;
/*#ifdef __debug
#include "tfdebug.h"
	double			_mG[pp][mm];
	FILE * _fin;
#endif*/
//	double		_mG[pp][mm];

// if CV's control_type is 3, it may be changed to 2,
// for avoiding the vibration, it should be considered
// into Matrix G;
	_freecount = 0;
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type != 4 &&
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type != 5 &&
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type != 7 &&
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type != 1 )
			_freecount++;
	for( _i = 0; _i < lpCobj_All_->p; _i++ )
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 1 )
				_freecount--;
	if( _freecount < 0 )
	{
		CVPrioritySort( lpCobj_All_, lpCobj_PrivCV_ );
		for( _l = 0; _l < (-_freecount); _l++ )
		{
			_flag_found = 0;
			for( _i = 0; (_i < lpCobj_All_->p) && ( _flag_found == 0 ); _i++ )
			{
				_k = (int)MGET( ( COBJ_2_MATRIX( lpCobj_All_->LPCVSort ) ), _i, 0 );
				if( PRIVCVGET( lpCobj_PrivCV_, _k, 0 )->CControl_type == 1 )
				{
					_Real_i = (int)MGET( ( COBJ_2_MATRIX( lpCobj_All_->LPApartCV ) ), lpCobj_All_->Apart_i, _k ) - 1;
					if( lpCobj_All_->Flag_CControl )
						WRITECVPARAMETER( lpCobj_All_->LPCV, (WORD)_Real_i, CV_CONTROLMODE, BOOL, 0 );
					PRIVCVGET( lpCobj_PrivCV_, _k, 0 )->CControl_type = 3;
					_flag_found = 1;
					PRIVCVGET( lpCobj_PrivCV_, _k, 0 )->CNoLP = 1;
				}
			}
		}
	}
	for( _i = 0; _i < lpCobj_All_->p; _i++ )
	{
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type > 4 )
			MessageBox( NULL, "ERROR", "CONTROL TYPE", MB_OK );
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CNoIll != 0 )
			MessageBox( NULL, "ERROR", "ILL", MB_OK );
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type != 4 &&
				( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHOver == 1 ||
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLOver == 1 ||
						PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDelayCount != 0 ||
						PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 1 ) &&
						PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CNoLP == 0 )
			_row++;
	}
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
	{
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 6 ||
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 8 )
			MessageBox( NULL, "ERROR", "ILL", MB_OK );

		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type != 4 &&
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type != 5 &&
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type != 7 )
			_col++;
	}
	if( _row <= 1 || _col <= 1 )
		return;

// outlook 原始静态增益矩阵计算
	_lpCobjG = CobjCopy( lpCobj_All_->LPG, &_bOK );
	_lpmG = COBJ_2_MATRIX( _lpCobjG );
	_lpmSR = MAKE_DIMMATRIX( lpCobj_All_->LPSR );
	for ( _i = 0; _i < lpCobj_All_->p; _i++)
		for ( _j = 0; _j < lpCobj_All_->m; _j++)
			MGET( _lpmG, _i, _j ) = DIMMGET( _lpmSR, _i, _j, (lpCobj_All_->N-1) );
	
/*#ifdef __debug
	if ((_fin=fopen( "C:\\MATLAB\\MCC\\tfing.txt", "w"))==NULL)
	{
		MessageBox( NULL, "cannot open stp file\n", "", MB_OK );
		return ;
	}
	for ( _i = 0; _i < lpCobj_All_->p; _i++)
	{
		for ( _j = 0; _j < lpCobj_All_->m; _j++)
		{
			_mG[_i][_j] = MGET( _lpmG, _i, _j );
			fprintf( _fin, "%f%s", (float)(_mG[_i][_j]), " " );
		}
		fprintf( _fin, "%s\n", ";" );
	}
	fclose( _fin );
#endif
*/	while( _flag_ill == 1 &&
			_FlagFound == 1 )
	{
		_MaxIllCount++;
		if( _row <= 1 || _col <= 1 )
			goto __return ;

		_num = min( _row, _col );
		_lpmGtemp = MatrixAlloc( _row, _col );
		_addrow	= 0;
		for( _i = 0; _i < _lpmG->row; _i++ )
		{
			if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 4 ||
					( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHOver == 0 &&
						PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLOver == 0 &&
							PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDelayCount == 0 &&
							PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type != 1 ) ||
								PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CNoIll == 1 ||
								PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CNoLP == 1 )
				_addrow++;
			else
			{
				_addcol = 0;
				for( _j = 0; _j < _lpmG->col; _j++ )
				{
					if(  PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 4 ||
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 5 ||
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 7 ||
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 8 ||
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 6 )
						_addcol++;
					else
						MGET( _lpmGtemp , _i-_addrow, _j-_addcol ) =
								MGET( _lpmG, _i, _j );
				}
			}
		}
		_lpmu = MatrixAlloc( _row, _row );
		_lpmv = MatrixAlloc( _col, _col );
/*		for( _i = 0; _i < _row; _i++ )
			for( _j = 0; _j < _col; _j++ )
				_mG[_i][_j] = MGET( _lpmGtemp, _i, _j );
 */
		svd( _lpmGtemp, _lpmu, _lpmv,  eps_svd );
		_eigmax = -10.0;
		_eigmin = lpCobj_All_->Big_Cond*100;
		for( _i = 0; _i < _num; _i++ )
			_eigmax = max( _eigmax, ( fabs( MGET( _lpmGtemp, _i, _i ) ) ) );

		for( _i = 0; _i < _num; _i++ )
//		_temp = MGET( _lpmbc, _i, 0 );
			_eigmin = min( _eigmin, ( fabs( MGET( _lpmGtemp, _i, _i ) ) ) );

		if( fabs(_eigmin) <= eps_cond )
			_Cond_Num = 2 * lpCobj_All_->Big_Cond;
		else
			_Cond_Num = _eigmax / _eigmin;

		MATRIXFREE(	_lpmGtemp );

		if( _Cond_Num >= lpCobj_All_->Big_Cond )
			_flag_ill = 1;
		else
		{
			MATRIXFREE( _lpmu );
			MATRIXFREE( _lpmv );
			goto __return ;
		}

//here ill-conditioned CV is sorted by its effecion priority
		if( _row <= _col )
		{
			CVPrioritySort( lpCobj_All_, lpCobj_PrivCV_ );
			_FlagFound = 0;
			for( _l = 0; (_l < (lpCobj_All_->p))&&(_FlagFound == 0 ); _l++ )
			{
//Chose the most unimportant MV and set it to satuation temporarily
				_i = (int)MGET( COBJ_2_MATRIX( lpCobj_All_->LPCVSort ), _l, 0 );
				if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type !=4 &&
					( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHOver != 0 ||
						PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLOver != 0 ||
							PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDelayCount != 0 ||
							PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 1 ) &&
								PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CNoIll != 1 && 
								PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CNoLP == 0 )
				{
					if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 1 )
						PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CNoLP = 2;
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type = 3;
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHOver = 0;
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLOver = 0;
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CNoIll = 1;
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDelayCount = lpCobj_All_->MaxDelay;
					lpCobj_All_->Freecount++;
					_row--;
					_FlagFound = 1;
				 }
			}
		}
		else
		{
			_FlagFound = 0;
//Chose the most unimportant MV and set it to satuation temporarily
			MVPrioritySort( lpCobj_All_, lpCobj_PrivMV_ );
			for( _kk = 0; ( _kk< lpCobj_All_->m)&&( _FlagFound == 0 ); _kk++ )
			{
				_j = (int)MGET( COBJ_2_MATRIX( lpCobj_All_->LPMVSort ), _kk, 0 );
				if(  PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type != 4 &&
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type != 5 &&
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type != 7 &&
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type != 6 &&
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type != 8 )
				{
					if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 1 )
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type = 6;
					else
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type = 8;
							
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDist = 1;
					lpCobj_All_->Freecount--;
		            _col--;
					_FlagFound = 1;
				}
			}
		}
		if( _FlagFound == 0 )
		{
			MessageBeep( -1 );
			lpCobj_All_->ErrorCode = 16;
			LogError( "Find Ill", "tfrm_ill.c, not found" );
//			MessageBox( NULL, "对不起", "tfrm_ill.c", MB_OK );
		}
		MATRIXFREE( _lpmu );
		MATRIXFREE( _lpmv );
	}
	if( _flag_ill )
	{
		MessageBeep( -1 );
		lpCobj_All_->ErrorCode = 18;
		LogError( "Ill", "tfrm_ill.c" );
	}	
		//MessageBox( NULL, "ILL", "IMPOSSIBLE", MB_OK );
__return:
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
	{
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDist == 1 )
		{
			if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 6 )
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type = 1;
			else
				if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 8 )
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type = 2;
				//LP should consider these MV as field control or IRV, but not disturbance
				//	MessageBox( NULL, "ERROR", "TFRm_ILL.C", MB_OK );
//			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 5;
		}
	}
	CobjFree( _lpCobjG );
	return;
}
