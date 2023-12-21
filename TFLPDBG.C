#include "tfalgo.h"
#include "tfwsqp.h"
#ifdef __ONSPEC
#include "Mtmfun.h"
#include "Mtdllfun.h"
#endif
extern char LocalDirectory[128];

void LPDebugFile( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpCobj_PrivMV_, LPCOBJPRIVCV lpCobj_PrivCV_, LPCOBJPRIVDV lpCobj_PrivDV_ )
{
	char	_szBuffer[128];
	FILE	*_DFile;
	int		_i, _j, _k;
	WORD	_Control_type;
	double	_TempSet, _TempHigh, _TempLow;

	sprintf( _szBuffer, "%s%s", LocalDirectory, "tflp.dbg" );
	if ((_DFile = fopen( _szBuffer, "w"))==NULL)
		MessageBox( NULL, "No Debug file", "lpdebug", MB_OK );

	for( _i = 0; _i < lpCobj_All_->p; _i++ )
	{
		_Control_type = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type;
		if( _Control_type == 4 )
			fprintf( _DFile, "%s%d%s%s\n", "CV", _i+1, "   ", "Bad" );
		else
			if( _Control_type == 1 )
			{
				_TempSet = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSetpoint;
				_TempSet += PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CIni;
				UNUNIT( _TempSet, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale,
							PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
				fprintf( _DFile, "%s%d%s%s%s%f\n", "CV", _i+1, "   ", "Setpoint", "   ", _TempSet );
			}
			else
				if( _Control_type == 2 || _Control_type == 3 )
				{
					_TempHigh = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHigh;
					_TempHigh += PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CIni;
					_TempLow = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLow;
					_TempLow += PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CIni;
					UNUNIT( _TempHigh, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale,
							PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
					UNUNIT( _TempLow, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale,
							PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
					fprintf( _DFile, "%s%d%s%s%s%s%s%f%s%s%s%f\n", "CV", _i+1, "   ", "Field", "   ", 
								"Low", "   ", _TempLow, "   ", "High", "   ", _TempHigh );
				}
	}
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
	{
		_Control_type = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type;
		if( _Control_type == 4 )
		{
			fprintf( _DFile, "%s%d%s%s\n", "MV", _j+1, "   ", "Bad" );
		}
		else
			if( _Control_type == 2 || _Control_type == 3 )
			{
				_TempHigh = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvHigh;
				_TempLow = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvLow;
				_TempHigh += PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIni;
				_TempLow += PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIni;
				UNUNIT( _TempHigh, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale,
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
				UNUNIT( _TempLow, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale,
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
				fprintf( _DFile, "%s%d%s%s%s%s%s%f%s%s%s%f\n", "MV", _j+1, "   ", "Field", "   ", 
							"Low", "   ", _TempLow, "   ", "High", "   ", _TempHigh );
		}
		else
			if( _Control_type == 1 || _Control_type == 5 )
			{
				_TempSet = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv;
				_TempSet += PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIni;
				UNUNIT( _TempSet, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale,
							PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
				fprintf( _DFile, "%s%d%s%s%s%f\n", "MV", _j+1, "   ", "Setpoint", "   ", _TempSet );
			}
	}
	for( _k = 0; _k < lpCobj_All_->nd; _k++ )
	{
		_TempSet = PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DData;
		_TempSet += PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DIni;
		UNUNIT( _TempSet, PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DHScale,
					PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DLScale );
		fprintf( _DFile, "%s%d%s%s%s%f\n", "DV", _k+1, "   ", "Data", "   ", _TempSet );
	
	}

	fclose( _DFile );
	return;
}
