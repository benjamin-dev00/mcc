#include "tfalgo.h"
#include "tfcmdmc.h"
void CVBegin( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVCV lpCobj_PrivCV_ )
{
	int				_i, _kk;
	LPMATRIX		_lpmypredN;
	double			_temp;
	BOOL			_Flag_Weight;
	WORD			_HOverCount, _LOverCount;
//  It is for another weight changing mode;

//	check CV, decide CVs's Control_type( no considering freedom )
//	flag of Over is set to be 0;
	_Flag_Weight = lpCobj_All_->Flag_Weight;
	for( _i = 0; _i < lpCobj_All_->p; _i++ )
	{
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHOver = 0;
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLOver = 0;
	}
	_lpmypredN = MAKE_MATRIX( lpCobj_All_->LPPredN );
//	Decide whether ypredN is over limit.
	for( _i = 0; _i < lpCobj_All_->p; _i++ )
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type != 4 )
		{
			if( _Flag_Weight == 0 || _Flag_Weight == 1 )
			{
				_HOverCount = 0;
				_LOverCount = 0;
				for( _kk = 0; (_kk < lpCobj_All_->P); _kk++ )
				{
					_temp = MGET( _lpmypredN, (_i*lpCobj_All_->N+_kk), 0 );
					if( _temp > PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_high )
					{
						PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHOver = 1;
						_HOverCount++;
					}
					if( _temp < PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_low )
					{
						PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLOver = 1;
						_LOverCount++;
					}
				}
				if( _HOverCount != 0 || _LOverCount != 0 )
				{
					if( _HOverCount >= _LOverCount )
					{
						PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHOver = 1;
						PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLOver = 0;
					}
					else
					{
						PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHOver = 0;
						PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLOver = 1;
					}
				}
			// Here decide High limit over or Low limit Over by the count number, but if
			// the sample data exceeds limit, its direct is more important;
			}
			if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CData > PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_high )
			{
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHOver = 1;
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLOver = 0;
			}
			if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CData < PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_low )
			{
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHOver = 0;
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLOver = 1;
			}
		}

	for( _i = 0; _i < lpCobj_All_->p; _i++ )
	{
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 2 )
		{
			if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHOver == 0 &&
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLOver == 0 )
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type = 3;
	//it should return to 2
		}
/*		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHOver == 1 ||
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLOver == 1 )
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CNoIll = 1;
		else
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CNoIll = 0;*/
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHOver == 1 )
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHOverCount++;
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLOver == 1 )
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLOverCount++;
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CNoIll = 0;
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CNoLP = 0;
	//is it right? --~
//	Maybe control-type 0 is equal to control-type 2;
// If Control_type is still 0, then it is equal to 4;
	}
	return ;
}
