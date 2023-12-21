#include "tfalgo.h"
#include "tfcmdmc.h"
#ifdef __ONSPEC
#include "mtmfun.h"
#include "mtdllfun.h"
#endif

extern char LocalDirectory[128];
extern DWORD		_type_DV[NUMBER_DVPARAM];
extern const char		_DVDiscription[NUMBER_DVPARAM][20];
ACRET Read_FileDV( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVDV lpCobj_PrivDV_ )
{
#ifdef __ONSPEC
	char			_szBuffer[128];
	char			_szSection[128], _sz_i[128], _FileName[128];
	BYTE FAR*		_lpStart;
	float			_TempFloat;
#endif
	WORD			_j, _i;
	WORD			_nd;

	_nd = lpCobj_All_->nd;
#ifdef __ONSPEC
	strcpy( _FileName, LocalDirectory );
	strcat( _FileName, "TFMCCDV0.INI" );
	for( _i = 0; _i < _nd; _i++ )
	{
		strcpy( _szSection, "MCCDV" );
		itoa(_i, _sz_i , 10 );
		strcat( _szSection, _sz_i );
		for( _j = 0; _j < NUMBER_DVPARAM; _j++ )
		{
			if( _type_DV[_j] != VSOF_LPCOBJ )
				GetPrivateProfileString( 	 _szSection,
											 _DVDiscription[_j],
											 "0",
											 _szBuffer,
											 sizeof(_szBuffer),
											 _FileName );
			if( _j <= DV_UNIT )
			{
				if( _j == DV_VALUE )
				{
					sscanf( _szBuffer, "%f", &_TempFloat );
					PRIVDVGET( lpCobj_PrivDV_, _i, 0 )->DData = (double)_TempFloat;
				}
			}
			else
				if( _j <= DV_STEADYFIELD )
				{
					_lpStart = (BYTE FAR*)( &( PRIVDVGET( lpCobj_PrivDV_, _i, 0 )->DHScale ) );
					sscanf( _szBuffer, "%f", &_TempFloat );
					( * ( double* )( _lpStart + ( _j - DV_HSCALE ) * sizeof(double) ) ) = (double)_TempFloat;
				}
				else
					if( _j == DV_STATE )
						sscanf( _szBuffer, "%d", &PRIVDVGET( lpCobj_PrivDV_, _i, 0 )->DInvalid );
		}
	}
#endif
#ifdef _SetIni
	for( _j = 0; _j < _nd; _j++ )
		PRIVDVGET( lpCobj_PrivDV_, _j, 0 )->DIni =
			PRIVDVGET( lpCobj_PrivDV_, _j, 0 )->DData;
#endif
	return ACRET_TRUE;
}
