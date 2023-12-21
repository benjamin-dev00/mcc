#include "tfalgo.h"
#include "tfcmdmc.h"

extern DWORD		_type_CV[NUMBER_CVPARAM];
extern const char		_CVDiscription[NUMBER_CVPARAM][20];
extern char LocalDirectory[128];
ACRET Write_FileCV( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVCV lpCobj_PrivCV_ )
{
	char			_FileName[128], _SectionName[128], _szCVNo[128], _szBuffer[128];
	int				_i, _p, _j;
	BYTE FAR*		_lpStart;

	if( lpCobj_All_->Flag_Save && !lpCobj_All_->Flag_Alarm )
	{
		strcpy( _FileName, LocalDirectory );
		strcat( _FileName, "tfmcccv0.ini" );
	}
	else
	{
		strcpy( _FileName, LocalDirectory );
		strcat( _FileName, "tfoutcv.ini" );
	}
	_p = lpCobj_All_->p;
	for( _i = 0; _i < _p; _i++ )
	{
#ifndef __Simul
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CIni =
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CData;
#endif
		_lpStart = (BYTE FAR* )( PRIVCVGET( lpCobj_PrivCV_, _i, 0 ) );
		strcpy( _SectionName, "MCCCV" );
		itoa( _i, _szCVNo, 10 );
		strcat( _SectionName, _szCVNo );
		for( _j = 0; _j < NUMBER_CVPARAM; _j++ )
		{
			if( _j <= CV_UNIT )
			{
#ifndef __Simul
				if( _j == CV_VALUE )
					sprintf( _szBuffer, "%f", PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CData );
#else	
				if( _j == CV_VALUE )
					sprintf( _szBuffer, "%f", PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CIni );
#endif
			}
			else
//				if( _j <= CV_HSET && _j != CV_INI )
				if( _j <= CV_HSET )
				{
					_lpStart = (BYTE FAR*)( &( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale ) );
					sprintf( _szBuffer, "%f", ( * ( double* )( _lpStart + ( _j - CV_HSCALE ) * sizeof(double) ) ) );
				}
				else
					if( _j <= CV_N )
					{
						_lpStart = (BYTE FAR*)( &(PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CRControl_type) );
						sprintf( _szBuffer, "%d", ( * ( WORD* ) ( _lpStart + 
								( _j - CV_RCONTROLMODE ) * sizeof(WORD) ) ) );
					}
					else
						if( _j < CV_PRIV )
						{
							_lpStart = (BYTE FAR*)( &(PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CInvalid ) );
							sprintf( _szBuffer, "%d", ( * ( BOOL* )( _lpStart + ( _j - CV_STATE ) * sizeof(BOOL) ) ) );
						}
//			if( _type_CV[_j] != VSOF_LPCOBJ && _j != CV_INI )
			if( _type_CV[_j] != VSOF_LPCOBJ )
				WritePrivateProfileString( _SectionName,
						   _CVDiscription[_j],
						   _szBuffer,
						   _FileName );
		}
	}

	return ACRET_TRUE;
}
