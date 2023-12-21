//===================================================================
//csysstmp.c
//	System Stamps
//Zheng zhikai	95-2-6
//===================================================================

#include "prehead.h"

#include "core.h"
#include "corerw.h"
#include "cpu.h"
#include "cface.h"
#include "cfwnd.h"
#include "cstamp.h"
#include "cstmpfn.h"
#include "cgdi.h"

#include "csysstmp.h"

_DLLEXP DWORD _ZZKDECL StampNullParam(PIC wCmd_, WRD wFace_, WORD wNo_, LONG lParam_);
_DLLEXP WORD _ZZKDECL StampNullBuild(SCMD wCmd_, WRD wFTU_, HWND hwnd_, WORD wParam_, LONG lParam_);
_DLLEXP BOOL _ZZKDECL StampNullPaint(WRD wFace_, HDC hdc_);
_DLLEXP DWORD _ZZKDECL StampRootParam(PIC wCmd_, WRD wFace_, WORD wNo_, LONG lParam_);
_DLLEXP WORD _ZZKDECL StampRootBuild(SCMD wCmd_, WRD wFTU_, HWND hwnd_, WORD wParam_, LONG lParam_);
_DLLEXP BOOL _ZZKDECL StampRootPaint(WRD wFace_, HDC hdc_);
_DLLEXP BOOL _ZZKDECL FaceWndwPaint(WRD wFTU_, HDC hdc_);
_DLLEXP DWORD _ZZKDECL FaceWndwParam(PIC wCmd_, WRD wFace_, WORD wNo_, LONG lParam_);
_DLLEXP WORD _ZZKDECL FaceWndwBuild(SCMD wCmd_, WRD wFTU_, HWND hwnd_, WORD wParam_, LONG lParam_);

extern STInfo		gSTInfo;
extern FTInfo		gFTInfo;

static STU _sSTUs[] = {
	{"@Null", IDS_NULL, (void *)StampNullPaint, (void *)StampNullParam, (void *)StampNullBuild},
	{"@FaceRoot", IDS_ROOT, (void *)StampRootPaint, (void *)StampRootParam, (void *)StampRootBuild},
	{"FaceWindow",	IDS_FACEWND, (void *)FaceWndwPaint, (void *)FaceWndwParam, (void *)FaceWndwBuild},
};

//...................................................................
//...................................................................
BOOL RegisterSystemStamps()
{
	int		_i;

	for(_i=0; _i < sizeof(_sSTUs)/sizeof(STU); _i++)
		if(!RegisterStamp( &_sSTUs[_i]) )
			return FALSE;

	//set the hiden stamps
	StampSetFlags( IDS_ROOT, StampGetFlags( IDS_ROOT ) | STF_HIDEN );
	StampSetFlags( IDS_FACEWND, StampGetFlags( IDS_FACEWND ) | STF_HIDEN );

	return TRUE;
}

BOOL AccessSystemStampDefaultParameter( LPCSTR lpszDefName_, BOOL bSave_ )
{
	BOOL	_ret = TRUE;
	int		_wrd;
	char	_szBuffer[_MAX_PATH + LENGTH_NAME_STAMP + 2];

	for(_wrd=0; _wrd < sizeof(_sSTUs)/sizeof(STU); _wrd++)
	{
		strcpy( _szBuffer, lpszDefName_ );
		strcpy( _szBuffer + strlen(_szBuffer) + 1, _sSTUs[_wrd].szName );
		if( !(*_sSTUs[_wrd].lpfnParam)( bSave_? PIC_SAVEDEFAULT: PIC_LOADDEFAULT, 
										0, 0, (LPARAM)_szBuffer ) )
		{
			LOGWARN2( "COREBASE.DLL %s defParam failed: %s", bSave_?"Save": "Load", _sSTUs[_wrd].szName );
			_ret = FALSE;
		}
	}

	return _ret;
}

///Stamp:        NULL  //////////////////////////////////////////////
//...................................................................
//Description:
//...................................................................
#pragma argsused
_DLLEXP DWORD _DLLFUNC StampNullParam(PIC wCmd_, WRD wFace_, WORD wNo_, LONG lParam_)
{
	CORE_ASSERT(wNo_ == 0);
	
	return _DefStampParam(wCmd_,
						  wFace_,
						  wNo_,
						  lParam_,
						  0,		//no param
						  IDS_NULL,
						  NULL );
}

//...................................................................
//...................................................................
#pragma argsused
_DLLEXP WORD _DLLFUNC StampNullBuild(SCMD wCmd_, WRD wFTU_, HWND hwnd_, WORD wParam_, LONG lParam_)
{
	CORE_ASSERT(0);
	return 1;
}

//...................................................................
//Description:
//		does nothing
//...................................................................
#pragma argsused
_DLLEXP BOOL _DLLFUNC StampNullPaint(WRD wFace_, HDC hdc_)
{
	CORE_ASSERT(gFTInfo.lpBase[wFace_].Stamp == IDS_NULL);

	return TRUE;
}

///Stamp:   Face Root  //////////////////////////////////////////////
//...................................................................
//Description:
//...................................................................
#pragma argsused
_DLLEXP DWORD _DLLFUNC StampRootParam(PIC wCmd_, WRD wFace_, WORD wNo_, LONG lParam_)
{
	CORE_ASSERT(wNo_ == 0);

	if( wCmd_ == PIC_CANCOPY )
		return FALSE;
	else
		return _DefStampParam(wCmd_,
						  wFace_,
						  wNo_,
						  lParam_,
						  0,		//no param
						  IDS_ROOT,
						  NULL );
}

//...................................................................
//...................................................................
#pragma argsused
_DLLEXP WORD _DLLFUNC StampRootBuild(SCMD wCmd_, WRD wFTU_, HWND hwnd_, WORD wParam_, LONG lParam_)
{
	return 1;
}


//...................................................................
//Description:
//		does nothing
//...................................................................
#pragma argsused
_DLLEXP BOOL _DLLFUNC StampRootPaint(WRD wFace_, HDC hdc_)
{
	CORE_ASSERT(gFTInfo.lpBase[wFace_].Stamp == IDS_NULL);

	return TRUE;
}

///Stamp:   Face Wndw  //////////////////////////////////////////////

//...................................................................
//...................................................................
#pragma argsused
_DLLEXP BOOL _DLLFUNC FaceWndwPaint(WRD wFTU_, HDC hdc_)
{
	return TRUE;
}

//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC FaceWndwParam(PIC wCmd_, WRD wFace_, WORD wNo_, LONG lParam_)
{
	static PDU _sPDU[] =
	{
		//maybe should not use _PDU_DEFAULT here,
		//because change the access properties to PRI_ENG
		_PDU_DEFAULT, 

		_ELE_PDU(FACEWND_NAME, 		"Name", 		VSOF_LPCOBJ_STR,		PU_NORMAL,	PRI_ENG|PDUF_NODEFAULT ),
		_ELE_PDU(FACEWND_WNDX, 		"WndX", 		VSOF_int, 				PU_NORMAL,	PRI_ENG ),
		_ELE_PDU(FACEWND_WNDY, 		"WndY", 		VSOF_int, 				PU_NORMAL,	PRI_ENG ),
		_ELE_PDU(FACEWND_WNDCX, 	"WndCX", 		VSOF_int, 				PU_NORMAL,	PRI_ENG ),
		_ELE_PDU(FACEWND_WNDCY, 	"WndCY", 		VSOF_int, 				PU_NORMAL,	PRI_ENG ),
		_ELE_PDU(FACEWND_BACKCLR, 	"BackColor", 	VSOF_DWORD_CLR,			PU_NORMAL,	PRI_ALL ),
		_ELE_PDU(FACEWND_CLIENTCX,	"ClientWidth",	VSOF_FCCRD,				PU_NIV,		PRI_ENG|PDUF_NODEFAULT ),
		_ELE_PDU(FACEWND_CLIENTCY,	"ClientHeight",	VSOF_FCCRD, 			PU_NIV,		PRI_ENG|PDUF_NODEFAULT ),
		_ELE_PDU(FACEWND_STYLE, 	"WndStyle", 	VSOF_DWORD_HEX,			PU_NORMAL,	PRI_ENG ),
		_ELE_PDU(FACEWND_EXSTYLE, 	"WndExStyle", 	VSOF_DWORD_HEX,			PU_NORMAL,	PRI_ENG ),
		_ELE_PDU(FACEWND_HWND, 		"HWND",			VSOF_HANDLE, 			PU_NIV,		PRI_DEV|PDUF_NODEFAULT ),
		_ELE_PDU(FACEWND_STATE, 	"State",		VSOF_DWORD_HEX,			PU_NIV,		PRI_DEV|PDUF_NODEFAULT ),
		_ELE_PDU(FACEWND_PICTURE, 	"Picture", 		VSOF_LPCOBJ_STR,		PU_NORMAL,	PRI_ENG|PDUF_NODEFAULT ),
		_ELE_PDU(FACEWND_DISPLAY, 	"Display", 		VSOF_DWORD_DISPLAYMODE,	PU_NORMAL,	PRI_ENG ),
	};
	DWORD	_ret;

	if( wCmd_ == PIC_PASTE )
	{
		LPCOBJ	_lpCobj;
		char	_szNewName[64];
		int		_i = 1;
		BOOL	_bOk;
		LPPU	_lpPrm;

		_lpPrm  = FaceGetParameter( wFace_ );

		do{
			wsprintf( _szNewName, "FW%d", _i++ );
		}while( !IsNewFwndName( _szNewName ) );

		//set a new, unique name here
		_lpCobj = AllocCString( _szNewName, &_bOk );
		MFPUWRITE( _lpPrm, FACEWND_NAME, LPCOBJ, _lpCobj );		//set a new, unique window name
		MFPUWRITE( _lpPrm, FACEWND_HWND, HANDLE, NULL );		//set HWND to NULL

		return TRUE;
	}

	_ret = _DefStampParam(wCmd_,
						  wFace_,
						  wNo_,
						  lParam_,
						  sizeof(_sPDU)/sizeof(PDU),
						  IDS_FACEWND,
						  &(_sPDU[0]) );

	//because the Fwnd no SCMD_INIT message, we initialize
	//the parameters here;
	//In future version, should we ckeck a unique name for
	//each FaceWindow?
	if( wCmd_ == PIC_INITIALIZEPARAM )
	{
		LPPU		_lpPrm;

		_lpPrm  = FaceGetParameter( wFace_ );

		switch( wNo_ )
		{
			case FACE_X:
				MFPUWRITE( _lpPrm, FACE_X, FCCRD, 0);
				break;
			case FACE_Y:
				MFPUWRITE( _lpPrm, FACE_Y, FCCRD, 0);
				break;
			case FACE_CX:
				MFPUWRITE( _lpPrm, FACE_CX, FCCRD, 1);
				break;
			case FACE_CY:
				MFPUWRITE( _lpPrm, FACE_CY, FCCRD, 1);
				break;

			case FACEWND_NAME:
			{
				LPCOBJ	_lpCobj;
				char	_szNewName[64];
				int		_i = 1;
				BOOL	_bOk;

				do{
					wsprintf( _szNewName, "FW%d", _i++ );
				}while( !IsNewFwndName( _szNewName ) );

				//get a new name here
				_lpCobj = AllocCString( _szNewName, &_bOk );
				MFPUWRITE( _lpPrm, FACEWND_NAME, LPCOBJ, _lpCobj );
				break;
			}

			default:
				break;
		}
	}

	return _ret;
}

//...................................................................
//Description: Stamp Build-time Proc
//	Manager a window
//...................................................................
#pragma argsused
_DLLEXP WORD _DLLFUNC FaceWndwBuild(SCMD wCmd_, WRD wFTU_, HWND hwnd_, WORD wParam_, LONG lParam_)
{
	/*
	LPPU			_lpPrm;
	LPFTU 			_lpFTU;

	_lpFTU = &((GetFaceTableInformation()->lpBase)[wFTU_]);
	_lpPrm  = _lpFTU->lpParam;
	*/
	switch(wCmd_)
	{
		case SCMD_INIT:
		{
			break;
		}

		default:
			return FALSE;
	}
	return TRUE;
}
