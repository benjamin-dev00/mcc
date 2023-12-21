//===================================================================
//
//Zheng zhikai	95-7-8
//===================================================================

#include "prehead.h"

#include "core.h"
#include "cmodu.h"
#include "calgo.h"

#include "algo.h"
#include "algodef.h"

static ATU		sATUs[] = {
	/////////////////////////////////////////////////////////
	//Arithmetic Operation	:	Defined in Arithmetic.c 
	/////////////////////////////////////////////////////////
	{"AddN", IDA_ADDN, AddNCalcu, AddNParam},
	{"AddR", IDA_ADDR, AddRCalcu, AddRParam},
	{"MinusN", IDA_MINUSN, MinusNCalcu, MinusNParam},
	{"MinusR", IDA_MINUSR, MinusRCalcu, MinusRParam},
	{"MultiR", IDA_MULTIR, MultiRCalcu, MultiRParam},
	{"DivideR", IDA_DIVIDER, DivideRCalcu, DivideRParam},

	{"OrB", IDA_ORB, OrBCalcu, OrBParam},
	{"AndB", IDA_ANDB, AndBCalcu, AndBParam},

	{"RandN", IDA_RANDN, RandNCalcu, RandNParam},
	{"RandR", IDA_RANDR, RandRCalcu, RandRParam},

	{"Stair", IDA_STAIR, StairCalcu, StairParam},
	{"Sin", IDA_SIN, SinCalcu, SinParam},
	{"Timer", IDA_TIMER, TimerCalcu, TimerParam},

	{"ShowWnd", IDA_SHOWWND, ShowFwndCalcu, ShowFwndParam},
	{"CloseWnd", IDA_CLOSEWND, CloseFwndCalcu, CloseFwndParam},
	{"ReplaceWnd", IDA_REPLACEWND, ReplaceFwndCalcu, ReplaceFwndParam},

	{"ExchangeR", IDA_EXCHANGER, ExchangeRCalcu, ExchangeRParam},
};

//...................................................................
//Description:
//...................................................................
BOOL ProvideAlgorithms()
{
	int		_i;

	for(_i=0; _i < sizeof(sATUs)/sizeof(ATU); _i++)
		if(!RegisterAlgorithm(&sATUs[_i]))
			return FALSE;

	return TRUE;
}

//...................................................................
//Description:
//...................................................................
BOOL ReleaseAlgorithms()
{
	int		_i;

	for(_i=0; _i < sizeof(sATUs)/sizeof(ATU); _i++)
		UnregisterAlgorithm( sATUs[_i].wID );

	return TRUE;
}

BOOL AccessAlgoDefaultParam( LPCSTR lpszDefName_, BOOL bSave_ )
{
	BOOL	_ret = TRUE;	
	int		_wrd;
	char	_szBuffer[_MAX_PATH + LENGTH_NAME_ALGO + 2];

	for(_wrd=0; _wrd < sizeof(sATUs)/sizeof(ATU); _wrd++)
	{
		strcpy( _szBuffer, lpszDefName_ );
		strcpy( _szBuffer + strlen(_szBuffer) + 1, sATUs[_wrd].szName );
		if( !(*sATUs[_wrd].lpfnParam)( bSave_? PIC_SAVEDEFAULT: PIC_LOADDEFAULT, 
										0, 0, (LPARAM)_szBuffer ) )
		{
			LOGWARN2( "COREALGO.DLL %s defParam failed: %s", bSave_?"Save": "Load", sATUs[_wrd].szName );
			_ret = FALSE;
		}
	}

	return _ret;
}
