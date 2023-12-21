#include "tfalgo.h"
#include "tfcmdmc.h"
extern const char		_MVDiscription[NUMBER_MVPARAM][20];
extern DWORD		_type_MV[NUMBER_MVPARAM];

extern char LocalDirectory[128];
ACRET Write_FileMV( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpCobj_PrivMV_ )
{
	char			_FileName[128], _szSection[128], _sz_i[128], _szBuffer[128];
	int				_i, _m, _j;
	BYTE FAR*		_lpStart;
	WORD			_GetWord;
	BOOL			_GetBool;
	double			_GetDouble;

	if( lpCobj_All_->Flag_Save && !lpCobj_All_->Flag_Alarm )
	{
		strcpy( _FileName, LocalDirectory );
		strcat( _FileName, "tfmccmv0.ini" );
	}
	else
	{
		strcpy( _FileName, LocalDirectory );
		strcat( _FileName, "tfoutmv.ini" );
	}
	_m = lpCobj_All_->m;
#ifndef __Simul
		for( _j = 0; _j < _m; _j++ )
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIni = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData;	
#endif
	for( _i = 0; _i < _m; _i++ )
	{
		strcpy( _szSection, "MCCMV" );
		itoa(_i, _sz_i , 10 );
		strcat( _szSection, _sz_i );
		_lpStart = (BYTE FAR*)PRIVMVGET( lpCobj_PrivMV_, _i, 0 );
		for( _j = 0; _j < NUMBER_MVPARAM; _j++ )
		{
			if( _j <= MV_UNIT )
			{
				if( _j == MV_VALUE )
					sprintf( _szBuffer, "%f", PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MData );
			}
			else
			{
//				if( _j <= MV_MLASTLOW && _j != MV_INI )
				if( _j <= MV_MLASTLOW )
				{
					_lpStart = (BYTE FAR*)( &( PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MHScale ) );
					_GetDouble = ( * ( double * )( (BYTE FAR*)_lpStart + 
								( _j - MV_HSCALE ) * sizeof(double) ) );
					sprintf( _szBuffer, "%f", _GetDouble );
				}
				else
				{
					if( _j <= MV_BAD )
					{
						_lpStart = (BYTE FAR*)( &( PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MRControl_type ) );
						_GetWord = ( * ( WORD * )( (BYTE FAR*)_lpStart + 
									( _j - MV_RCONTROLMODE )  * sizeof(WORD) ) );
						sprintf( _szBuffer, "%d", _GetWord );
					}
					else
					{
						if( _j < MV_PRIV )
						{
							_lpStart = (BYTE FAR*)( &( PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MInvalidate ) );
							_GetBool = ( * ( BOOL * )( (BYTE FAR*)_lpStart + 
									( _j - MV_STATE )* sizeof(BOOL) ) );
							sprintf( _szBuffer, "%d", _GetBool );
						}
					}
				}
			}
//			if( _type_MV[_j] != VSOF_LPCOBJ && _j != MV_INI )
			if( _type_MV[_j] != VSOF_LPCOBJ  )
				WritePrivateProfileString(	_szSection,
											_MVDiscription[_j],
											_szBuffer,
											_FileName );

		}
	}
	return ACRET_TRUE;
}
