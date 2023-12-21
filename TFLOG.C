#include "tfalgo.h"
#include "tfcmdmc.h"

extern char LocalDirectory[128];

void LogError( LPCTSTR lpSection_, LPCTSTR lpsz_ )
{
	char	_szBuffer[128];
	char	_error_time[128], _error_date[128], _sztimes[128];
	int		_times;

	sprintf( _szBuffer, "%s%s", LocalDirectory, "tflog.ini" );
	GetPrivateProfileString( lpSection_,
							 "ErrorTimes",
							 "0",
							 _sztimes,
							 sizeof(_sztimes ),
							 _szBuffer );

	_times = atoi( _sztimes );
	if( _times == 0 )
	{
		WritePrivateProfileString( lpSection_,
				   (LPCTSTR)"ErrorMessage",
				   lpsz_,
				   (LPCTSTR)_szBuffer );
		_strtime( _error_time );
		WritePrivateProfileString( lpSection_,
				   (LPCTSTR)"ErrorStartTime",
				   _error_time,
				   (LPCTSTR)_szBuffer );
		_strdate( _error_date);
		WritePrivateProfileString( lpSection_,
				   "ErrorStartDate",
				   _error_date,
				   _szBuffer );
		WritePrivateProfileString( lpSection_,
				   "ErrorTimes",
				   "1",
				   _szBuffer );
	}
	else
	{
		_strtime( _error_time );
		WritePrivateProfileString( lpSection_,
				   "ErrorRecentTime",
				   _error_time,
				   _szBuffer );
		_strdate( _error_date);
		WritePrivateProfileString( lpSection_,
				   "ErrorRecentDate",
				   _error_date,
				   _szBuffer );
		_times++;
		itoa( _times, _sztimes, 10 );
		WritePrivateProfileString( lpSection_,
				   "ErrorTimes",
				   _sztimes,
				   _szBuffer );
	}
	return;
}