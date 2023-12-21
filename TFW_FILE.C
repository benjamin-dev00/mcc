#include "tfalgo.h"
#include "tfcmdmc.h"
extern const char	GlobalDiscription[NUMBER_GLOBAL][40];

extern char LocalDirectory[128];
ACRET Write_FileAll( LPCOBJPRIV lpCobj_PrivAll_ )
{
	char			_FileName[128], _szBuffer[128];
	WORD			_GetWord;
	BOOL			_GetBool;
	double			_GetDouble;
	int				_i;
	BYTE FAR*		_lp;

	lpCobj_PrivAll_->Flag_AM = 0;
	strcpy( _FileName, LocalDirectory );
#ifdef __ONSPEC	
	strcat( _FileName, "tfmcc.ini" );
#else
	strcat( _FileName, "tfmcccore.ini" );
#endif

	GetPrivateProfileString( "MCC",
							 "Save",
							 "0",
							 _szBuffer,
							 sizeof(_szBuffer),
							 _FileName );
	sscanf( _szBuffer, "%d", &lpCobj_PrivAll_->Flag_Save );
	if( ( !( lpCobj_PrivAll_->Flag_Save ) ) ||
			lpCobj_PrivAll_->Flag_Alarm )
	{
		strcpy( _FileName, LocalDirectory );
		strcat( _FileName, "tfoutall.ini" );
	}
	for( _i = 0; _i < MDMCIRVD_DFILTER; _i++ )
	{
		if( _i < NUMBER_GLOBALPARAMWORD )
		{
			if( _i < MDMCIRVD_FLAGAM )
			{
				_GetWord = ( * ( WORD * )( (BYTE FAR*)lpCobj_PrivAll_ + 
								_i * sizeof(WORD) ) );
				sprintf( _szBuffer, "%d", _GetWord );
			}
			else
			{
				if( _i == MDMCIRVD_FLAGAM )
				{
					sprintf( _szBuffer, "%d", lpCobj_PrivAll_->Flag_AM );
				}
				else
					if( _i == MDMCIRVD_FREECOUNT )
					{
						sprintf( _szBuffer, "%d", lpCobj_PrivAll_->Freecount );
					}
					else
					{
						_lp = (BYTE FAR* )( &(lpCobj_PrivAll_->MaxDelay ) );
						_GetWord = ( * ( WORD * )( (BYTE FAR*)_lp + 
								( _i - MDMCIRVD_MAXDELAY ) * sizeof(WORD) ) );
						sprintf( _szBuffer, "%d", _GetWord );
					}
			}
		}
		else
			if( _i < NUMBER_GLOBALPARAMWORD + NUMBER_GLOBALPARAMBOOL )
			{
				_lp = (BYTE FAR* )( &(lpCobj_PrivAll_->Flag_CControl) );
				_GetBool = ( * ( ( BOOL * )( _lp + ( _i - NUMBER_GLOBALPARAMWORD ) * sizeof( BOOL ) ) ) );
				sprintf( _szBuffer, "%d", _GetBool );
			}
			else
				if( _i < NUMBER_GLOBALPARAMWORD + NUMBER_GLOBALPARAMBOOL + NUMBER_GLOBALPARAMDOUBLE )
				{
					_lp = (BYTE FAR* )( &( lpCobj_PrivAll_->LpK ) );
					_GetDouble = ( * ( ( double *)( _lp + ( _i - NUMBER_GLOBALPARAMWORD - NUMBER_GLOBALPARAMBOOL )* sizeof(double))) );
					sprintf( _szBuffer, "%f", _GetDouble );
				}
		WritePrivateProfileString( "MCC",
							   GlobalDiscription[_i],
							   _szBuffer,
							   _FileName );
	}
/*	sprintf( _szBuffer, "%d", lpCobj_PrivAll_->wModu );
	WritePrivateProfileString( "MCC",
							   "Modu",
							   _szBuffer,
							   _FileName );
	sprintf( _szBuffer, "%d", lpCobj_PrivAll_->Flag_Save );
	WritePrivateProfileString( "MCC",
							   "Save",
							   _szBuffer,
							   _FileName );
	sprintf( _szBuffer, "%d", lpCobj_PrivAll_->ErrorCode );
	WritePrivateProfileString( "MCC",
							   "ErrorCode",
							   _szBuffer,
							   _FileName );
	sprintf( _szBuffer, "%d", lpCobj_PrivAll_->Flag_Steady );
	WritePrivateProfileString( "MCC",
							   "Steady",
							   _szBuffer,
							   _FileName );
	_GetDouble = lpCobj_PrivAll_->C;
	sprintf( _szBuffer, "%f", _GetDouble );
	WritePrivateProfileString( "MCC",
							   "Fault_limit",
							   _szBuffer,
							   _FileName );
	sprintf( _szBuffer, "%d", lpCobj_PrivAll_->SteadyStep );
	WritePrivateProfileString( "MCC",
							   "SteadyStep",
							   _szBuffer,
							   _FileName );*/
	return ACRET_TRUE;
}