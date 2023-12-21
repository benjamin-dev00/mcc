#include "tfalgo.h"
#include "tfcmdmc.h"

extern DWORD		_type_DV[NUMBER_DVPARAM];
extern const char		_DVDiscription[NUMBER_DVPARAM][20];
extern char LocalDirectory[128];
ACRET Write_FileDV( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVDV lpCobj_PrivDV_ )
{
	char			_FileName[128], _SectionName[128], _szDVNo[128], _szBuffer[128];
	int				_i, _nd, _j;
	BYTE FAR*		_lpStart;

	if( lpCobj_All_->Flag_Save && !lpCobj_All_->Flag_Alarm )
	{
		strcpy( _FileName, LocalDirectory );
		strcat( _FileName, "tfmccdv0.ini" );
	}
	else
	{
		strcpy( _FileName, LocalDirectory );
		strcat( _FileName, "tfoutdv.ini" );
	}
	_nd = lpCobj_All_->nd;
	for( _i = 0; _i < _nd; _i++ )
	{
#ifndef __Simul
		PRIVDVGET( lpCobj_PrivDV_, _i, 0 )->DIni = 
			PRIVDVGET( lpCobj_PrivDV_, _i, 0 )->DData;
#endif
		_lpStart = (BYTE FAR* )( PRIVDVGET( lpCobj_PrivDV_, _i, 0 ) );
		strcpy( _SectionName, "MCCDV" );
		itoa( _i, _szDVNo, 10 );
		strcat( _SectionName, _szDVNo );
		for( _j = 0; _j < NUMBER_DVPARAM; _j++ )
		{
			if( _j <= DV_UNIT )
			{
				if( _j == DV_VALUE )
					sprintf( _szBuffer, "%f", PRIVDVGET( lpCobj_PrivDV_, _i, 0 )->DData );
			}
			else
				if( _j <= DV_STEADYFIELD )
				{
					_lpStart = (BYTE FAR*)( &( PRIVDVGET( lpCobj_PrivDV_, _i, 0 )->DHScale ) );
					sprintf( _szBuffer, "%f", ( * ( double* )( _lpStart + ( _j - DV_HSCALE ) * sizeof(double) ) ) );
				}
				else
					if( _j == DV_STATE )
						sprintf( _szBuffer, "%f", PRIVDVGET( lpCobj_PrivDV_, _i, 0 )->DInvalid );

			if( _type_DV[_j] != VSOF_LPCOBJ )
				WritePrivateProfileString( _SectionName,
							   _DVDiscription[_j],
							   _szBuffer,
							   _FileName );
		}
	}

	return ACRET_TRUE;
}
