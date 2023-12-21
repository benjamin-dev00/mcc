#include "tfalgo.h"
#include "tfwsqp.h"

ACRET Seq( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpCobj_PrivMV_, LPCOBJPRIVCV lpCobj_PrivCV_ )
{
	int			_i, _j, _row, _col, _l, _kk;
	TINT		_freetemp;
//	double		_mG[pp][mm];
	WORD		_IRVcount, _flag_IRVError = 0;

	_row = lpCobj_All_->p;
	_col = lpCobj_All_->m;

//	free some IRV if freecount is not enough

//	if IRV is not allowable, then it can't be set again here.
//	Don't consider the CV field control, SEQ first

	_freetemp = lpCobj_All_->Freecount;
	for( _i = 0; _i < lpCobj_All_->p; _i++ )
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 2 )
			_freetemp++;

//	Satisfied the IRVs as many as possible	
	_IRVcount = 0;
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 1 )
			_IRVcount++;

	if( _freetemp >= _IRVcount )
		_freetemp -= _IRVcount;
	else
	{
		if( _freetemp == 0 )
		{
			for( _j = 0; _j < lpCobj_All_->m; _j++ )
				if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 1 )
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type = 2;
		}
		else
		{
			MVPrioritySort( lpCobj_All_, lpCobj_PrivMV_ );
			_j = 0;
			for( _kk = 0; _kk < lpCobj_All_->m && _j < (_IRVcount-_freetemp); _kk++ )
			{
				_l = (int)MGET( ( COBJ_2_MATRIX( lpCobj_All_->LPMVSort ) ), _kk, 0 );
				if( PRIVMVGET( lpCobj_PrivMV_, _l, 0 )->MControl_type == 1 )
				{
					_j++;
//note:here _j or _l?? 2 or 3? maybe 3 is more suitable
					PRIVMVGET( lpCobj_PrivMV_, _l, 0 )->MControl_type = 2;
				}
			}
//	MV Priority Sort
		}
		_freetemp = 0;
	}

	//get an effective transfer function

	for( _i = 0; _i < lpCobj_All_->p; _i++ )
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 4 )
			_row--;

	for( _j = 0; _j < lpCobj_All_->m; _j++ )
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 4 )
			_col--;

	if( _row == 0 || _col == 0 )
	{
//		MessageBox( NULL, "No MV or CV to be controled", "ERROR", MB_OK );
		MessageBeep( -1 );
        LogError( "No Control Need", "tfseq.c" );
    }

	return ACRET_TRUE;
}
