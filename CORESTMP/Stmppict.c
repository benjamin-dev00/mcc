//===================================================================
//
//Zheng zhikai  96-10-29
//===================================================================
#include "prehead.h"

#include "coreall.h"
#include <dos.h>
#include "stmpdef.h"


#define PICT_NAME             (FACE_FIRSTPARAM + 0)
#define PICT_DISPLAY          (FACE_FIRSTPARAM + 1)
#define PICT_NAME_PDT         PDT_LPCOBJ
#define PICT_DISPLAY_PDT      PDT_DWORD

//...................................................................
//Picture
//...................................................................
_DLLEXP BOOL _DLLFUNC PicturePaint(WRD wFTU_, HDC hdc_)
{
	LPPU		_lpPrm = NULL;
	LPCOBJ		_lpCobj = NULL;
	RECT		_rect, _rcBack;
	DWORD		_mode;
	BOOL		_ret;
	char		_szError[128];

	_lpPrm = FaceGetParameter(wFTU_);
	if( ! _lpPrm )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	MFPUREAD( _lpPrm, PICT_NAME, LPCOBJ, _lpCobj );
	MFPUREAD( _lpPrm, PICT_DISPLAY, DWORD, _mode );
	FaceXycxcyToRect(wFTU_, &_rect);

	if(  ( !_lpCobj )
	   ||( !GET_COBJ_DATA(_lpCobj) ) )
	{
		//return if no file name
		DrawErrorString( &_rect, "no file", hdc_ );
		return TRUE;
	}

	CopyRect( &_rcBack, &_rect );

	_ret = DrawPicture( (LPCSTR)GET_COBJ_DATA(_lpCobj), 
					  &_rect, _szError, DPFLAG_REPORTERROR|DPFLAG_NOTCLIP, _mode, hdc_ );

	if( !EqualRect(&_rcBack, &_rect ) )	//FTU's position has been changed
		WriteFaceXYCXCY( wFTU_, &_rect );

	return TRUE;
}

//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC PictureParam(PIC wCmd_, WRD wFTU_, WORD wNo_, LONG lParam_)
{
	static PDU _sPDU[] =
	{
		_PDU_DEFAULT,
		_ELE_PDU(PICT_NAME,		"Name",		VSOF_LPCOBJ_STR,		PU_NORMAL, PRI_ALL|PDUF_NODEFAULT ),
		_ELE_PDU(PICT_DISPLAY,  "Display",	VSOF_DWORD_DISPLAYMODE,	PU_NORMAL, PRI_ALL ),
	};

	DWORD _ret;

	_ret = _DefStampParam(wCmd_,
						  wFTU_,
						  wNo_,
						  lParam_,
						  sizeof(_sPDU)/sizeof(PDU),
						  IDS_PICTURE,
						  &(_sPDU[0]) );


	if( _ret && (wCmd_ == PIC_INITIALIZEPARAM) && (wNo_ == PICT_NAME) )
	{
		LPPU            _lpPrm;
		LPCOBJ          _lpCobjDefault;
		BOOL            _bOk;

		_lpPrm  = FaceGetParameter( wFTU_ );
		_lpCobjDefault = AllocCString( "DEFAULT.BMP", &_bOk );
		if( !_bOk )
			return FALSE;

		MFPUWRITE(_lpPrm, PICT_NAME, LPCOBJ, _lpCobjDefault );
	}

	return _ret;

}

//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC PictureBuild(SCMD wCmd_, WRD wFTU_, HWND hwnd_, WORD wParam_, LONG lParam_)
{
	switch(wCmd_)
	{
		case SCMD_INIT:
			break;

		default:
			return DefStampBuild( wCmd_, wFTU_, hwnd_, wParam_, lParam_ );
	}

	return TRUE;
}


