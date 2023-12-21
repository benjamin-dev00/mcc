//===================================================================
//
//Zheng zhikai	97-03-10
//===================================================================

#include "prehead.h"

#include "core.h"
#include "cobj.h"
#include "csysobj.h"
#include "creal.h"
#include "cerror.h"
#include "calarm.h"

typedef struct _tagALARM_PACKAGE{
	WORD 	wTable;	
	WRD 	wrd;
	BOOL 	state;
	ALARM 	alarm;
}ALARMPACKAGE, FAR* LPALARMPACKAGE;

typedef struct tagALARM_INFO
{
	WORD				wCount;
	WORD				wAdding;
	LPALARMPACKAGE 		lpBase;
}AlarmInfo;

BOOL _EnlargeAlarmHeap(void);
WORD _FindAlarm( LPALARMPACKAGE lpap_ );
WORD _AddAlarm( LPALARMPACKAGE lpap_ );
void _DeleteAlarm( WORD _i );

static AlarmInfo	sAlarmInfo;

//...................................................................
//...................................................................
void _NONEXPORTED PostAlarm( WORD wTable_, WRD wrd_, BOOL state_, ALARM alarm_ )
{
	WORD			_i;
	ALARMPACKAGE	_ap;

	_ap.wTable = wTable_;
	_ap.wrd = wrd_;
	_ap.state = state_?TRUE:FALSE;
	_ap.alarm = alarm_;

	//logAlarm

	//update current alarm
	_ap.state = !_ap.state;
	_i = _FindAlarm( &_ap );
	_ap.state = !_ap.state;

	if( _i )
		_DeleteAlarm( _i );
	else
		_AddAlarm( &_ap );

	return;
}

//...................................................................
//...................................................................
WORD _FindAlarm( LPALARMPACKAGE lpap_ )
{
	LPALARMPACKAGE	_lpap;
	WORD			_i;

	for( _i=1, _lpap = sAlarmInfo.lpBase;
		 _i < sAlarmInfo.wAdding;
		 _i++, _lpap++ )
	{
		if( !memcmp( _lpap, lpap_, sizeof(ALARMPACKAGE) ) )
			return _i;
	}

	return NULL;	//no found
}

//...................................................................
//...................................................................
WORD _AddAlarm( LPALARMPACKAGE lpap_ )
{
	//check the size of the alarm heap
	if( sAlarmInfo.wCount >= sAlarmInfo.wAdding + 1 )
		if( !_EnlargeAlarmHeap() )
			return FALSE;

	memcpy( sAlarmInfo.lpBase + sAlarmInfo.wAdding, lpap_, sizeof(ALARMPACKAGE) );
	sAlarmInfo.wCount ++;

	return TRUE;
}

//...................................................................
//...................................................................
void _DeleteAlarm( WORD _i )
{
	if( _i >= sAlarmInfo.wAdding )
	{
		CORE_ASSERT( FALSE );
		return;
	}

	memmove( sAlarmInfo.lpBase + _i,
			 sAlarmInfo.lpBase + _i + 1,
			 sizeof(ALARMPACKAGE)*(sAlarmInfo.wAdding - _i - 1) );

	if( sAlarmInfo.wAdding > 1 )
		sAlarmInfo.wAdding --;

	return;
}

//...................................................................
//...................................................................
BOOL _NONEXPORTED InitAlarmHeap()
{
	LPVOID	_lp;

	_lp = FragAlloc( 64*sizeof(ALARMPACKAGE) );

	if( !_lp )
		return FALSE;

	sAlarmInfo.wCount = (WORD)( FragSize( _lp ) / sizeof(ALARMPACKAGE) );
	sAlarmInfo.lpBase = (LPALARMPACKAGE)_lp;
	sAlarmInfo.wAdding = 1;
	return TRUE;
}

//...................................................................
//...................................................................
BOOL _EnlargeAlarmHeap()
{
	LPVOID	_lp;

	_lp = FragRealloc( sAlarmInfo.lpBase, (sAlarmInfo.wCount + 64)*sizeof(ALARMPACKAGE) );

	if( !_lp )
		return FALSE;

	sAlarmInfo.wCount = (WORD)FragSize( _lp ) / sizeof(ALARMPACKAGE);
	sAlarmInfo.lpBase = (LPALARMPACKAGE)_lp;
	return TRUE;
}

