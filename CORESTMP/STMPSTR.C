//===================================================================
//
//Zheng zhikai	96-8-12
//===================================================================
#include "prehead.h"

#include "coreall.h"
#include "stmpdef.h"
#include <stdarg.h>
#include <limits.h>

#include "StmpRes.h"

#pragma warning( disable : 4244 )	//'conversion' conversion from 'type1' to 'type2', possible loss of data

extern	HINSTANCE	ghInst;

#define	_DEF_VALUE_STR		0x0001
#define	_ENABLE_MM			0x0002

#define	_STRING_INPUTENABLE		0x0001
#define	_STRING_RESTRUCT_BAND	0x0002
	//if this bit set, the BAND specified at min/max enable

DWORD _MyStringParam(PIC wCmd_, WRD wFTU_, WORD wNo_, LONG lParam_);

WORD _GetDataString( WRD wFTU_, LPSTR lpsz_, WORD size_ );
BOOL _SetDataValue( WRD wFTU_, LPSTR lpsz_, BOOL bEnableMMChecking_, REAL min_, REAL max_ );
#ifdef _WIN32
BOOL CALLBACK
#else
BOOL CALLBACK _export 
#endif
InputDlgProc( HWND hwnd_, UINT msg_, WPARAM wParam_, LPARAM lParam_ );

enum _string_param
{
	STRING_BORDERSTYLE = FACE_FIRSTPARAM,
	STRING_BORDERWIDTH,
	STRING_BORDERCLR,
	STRING_BACKCLR,
	STRING_FORMAT,
	STRING_DATA,
	STRING_MIN,
	STRING_MAX,
	STRING_MMCHECK,
	STRING_FONT,
	STRING_SIZE,
	STRING_ITALIC,
	STRING_BOLD,
	STRING_TEXTCOLOR,
	STRING_TEXTHORALIGN,
	STRING_TEXTVERALIGN,
};

#define STRING_FORMAT_PDT		PDT_LPCOBJ
#define	STRING_DATA_PDT			PDT_USER
#define	STRING_FONT_PDT			PDT_LPCOBJ
#define	STRING_SIZE_PDT			PDT_WORD
#define	STRING_ITALIC_PDT		PDT_BOOL
#define	STRING_BOLD_PDT			PDT_BOOL
#define	STRING_TEXTCOLOR_PDT	PDT_COLORREF
#define	STRING_TEXTHORALIGN_PDT	PDT_UINT
#define	STRING_TEXTVERALIGN_PDT	PDT_UINT
#define	STRING_DATATYPE_PDT		PDT_WORD
#define	STRING_MMCHECK_PDT		PDT_BOOL
#define STRING_MIN_PDT			PDT_REAL
#define STRING_MAX_PDT			PDT_REAL
#define STRING_BORDERSTYLE_PDT	PDT_int
#define STRING_BORDERWIDTH_PDT	PDT_DWORD
#define STRING_BORDERCLR_PDT	PDT_COLORREF
#define STRING_BACKCLR_PDT		PDT_COLORREF

//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC StringParam(PIC wCmd_, WRD wFTU_, WORD wNo_, LONG lParam_)
{
	static PDU _sPDU[] =
	{
		_PDU_DEFAULT,

		_ELE_PDU(STRING_BORDERSTYLE, 	"BorderStyle", 	VSOF_int_RECTBS,	PU_NORMAL, PRI_ALL ),
		_ELE_PDU(STRING_BORDERWIDTH, 	"BorderWidth", 	VSOF_DWORD_BWIDTH,	PU_NORMAL, PRI_ALL ),
		_ELE_PDU(STRING_BORDERCLR, 		"BorderColor",	VSOF_DWORD_CLR,		PU_NORMAL, PRI_ALL ),
		_ELE_PDU(STRING_BACKCLR, 		"BackColor", 	VSOF_DWORD_CLR,		PU_NORMAL, PRI_ALL ),

		//the format string if data is valid, or the string is content
		_ELE_PDU(STRING_FORMAT,		"Format",			VSOF_LPCOBJ_STR,	PU_NORMAL, PRI_ALL ),

		//the data source
		_ELE_PDU(STRING_DATA,			"Data",			VSOF_USER,			PU_NORMAL, PRI_ALL|PDUF_NODEFAULT ),
		_ELE_PDU(STRING_MIN,			"Min",			VSOF_REAL,			PU_NORMAL, PRI_ALL ),
		_ELE_PDU(STRING_MAX,			"Max",			VSOF_REAL,			PU_NORMAL, PRI_ALL ),

		_ELE_PDU(STRING_MMCHECK,		"LimitCheck",	VSOF_BOOL,			PU_NORMAL, PRI_ALL ),

		_ELE_PDU(STRING_FONT,			"Font",			VSOF_LPCOBJ_STR,		PU_NORMAL, PRI_ALL|PDUF_NODEFAULT ),
		_ELE_PDU(STRING_SIZE,			"Size",			VSOF_WORD,				PU_NORMAL, PRI_ALL ),
		_ELE_PDU(STRING_ITALIC,			"Italic",		VSOF_BOOL,				PU_NORMAL, PRI_ALL ),
		_ELE_PDU(STRING_BOLD,			"Bold",			VSOF_BOOL,				PU_NORMAL, PRI_ALL ),
		_ELE_PDU(STRING_TEXTCOLOR,		"TextColor",	VSOF_DWORD_CLR,			PU_NORMAL, PRI_ALL ),
		_ELE_PDU(STRING_TEXTHORALIGN,	"TextHorAlign",	VSOF_DWORD_TXTHORALIGN,	PU_NORMAL, PRI_ALL ),
		_ELE_PDU(STRING_TEXTVERALIGN,	"TextVerAlign",	VSOF_DWORD_TXTVERALIGN,	PU_NORMAL, PRI_ALL ),
	};

	DWORD	_ret;

	if( wNo_ == STRING_DATA )
	{
		switch(wCmd_)
		{
			case PIC_GETVALUESTRING:
			case PIC_SETVALUESTRING:
			case PIC_GETPARAMVSOF:
			case PIC_GETVSOFENUMSTRUCT:
				return _MyStringParam(wCmd_, wFTU_, wNo_, lParam_);
		}
	}

	_ret = _DefStampParam(wCmd_,
				  wFTU_,
				  wNo_,
				  lParam_,
				  sizeof(_sPDU)/sizeof(PDU),
				  IDS_STRING,
				  &(_sPDU[0]) );

	if( _ret && (wCmd_ == PIC_INITIALIZEPARAM) && (wNo_ == STRING_FORMAT) )
	{
		LPCOBJ	_lpCobjDefault;
		BOOL	_bOk;
		LPPU	_lpPrm = FaceGetParameter( wFTU_ );

		_lpCobjDefault = AllocCString( "Text", &_bOk );
		if( !_bOk )
			return FALSE;

		MFPUWRITE(_lpPrm, STRING_FORMAT, LPCOBJ, _lpCobjDefault );
	}

	if( _ret && (wCmd_ == PIC_INITIALIZEPARAM) && (wNo_ == STRING_FONT) )
	{
		LPCOBJ	_lpCobjFontFace;
		BOOL	_bOk;
		LPPU	_lpPrm  = FaceGetParameter( wFTU_ );

		_lpCobjFontFace = AllocCString( "MS Serif", &_bOk );
		if( !_bOk )
			return FALSE;

		MFPUWRITE(_lpPrm, STRING_FONT, LPCOBJ, _lpCobjFontFace );
	}

	return _ret;
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC StringPaint(WRD wFTU_, HDC hdc_)
{
	LPPU		_lpPrm;
	char		_buffer[512];

	CCLR		_clrFill, _clrBorder, _clrText;
	COLORREF	_rgbText;
	RECT		_rect;
	HFONT		_hFontOld, _hFontNew;
	UINT		_uAlignHor, _uAlignVer, _uAlign;
	BOOL		_bBold, _bItalic;
	WORD		_wSize;
	LPCOBJ		_lpCobjFont;

	LOGFONT		_logFont;

	DWORD		_dwPenWidth;
	int			_nPenStyle;
	int			_iBkModeOld;
	BOOL		_retClr;	//return value by CclrGetRGB
	//COLORREF	_rgb;

	_lpPrm = FaceGetParameter(wFTU_);
	if( ! _lpPrm )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	FaceXycxcyToRect(wFTU_, &_rect);
	MFPUREAD( _lpPrm, STRING_BORDERCLR, COLORREF, _clrBorder );
	MFPUREAD( _lpPrm, STRING_BACKCLR, COLORREF, _clrFill );
	MFPUREAD( _lpPrm, STRING_BORDERWIDTH, DWORD, _dwPenWidth );
	MFPUREAD( _lpPrm, STRING_BORDERSTYLE, int, _nPenStyle );

	CoreFillRect( hdc_, &_rect, _clrFill );
	CoreDrawBox( hdc_, &_rect, _nPenStyle, _dwPenWidth, _clrBorder );

	_GetDataString( wFTU_, _buffer, sizeof(_buffer) );

	//read text color and apply it
	MFPUREAD( _lpPrm, STRING_TEXTCOLOR, COLORREF, _clrText );

	_retClr = CclrGetRGB( _clrText, &_rgbText );
	SetTextColor( hdc_, _rgbText );

	//read font information, create the font and apply it
	MFPUREAD( _lpPrm, STRING_FONT, LPCOBJ, _lpCobjFont );
	MFPUREAD( _lpPrm, STRING_SIZE, WORD, _wSize );
	MFPUREAD( _lpPrm, STRING_ITALIC, BOOL, _bItalic );
	MFPUREAD( _lpPrm, STRING_BOLD, BOOL, _bBold );

	memset( &_logFont, 0, sizeof(LOGFONT) );

	_logFont.lfHeight = _wSize;
	_logFont.lfHeight = -1 * MulDiv(_wSize, GetDeviceCaps(hdc_, LOGPIXELSY), 72);
	_logFont.lfItalic = _bItalic;
	_logFont.lfWeight = _bBold? FW_BOLD : FW_NORMAL;
	_logFont.lfQuality = PROOF_QUALITY;
	_logFont.lfOutPrecision = OUT_DEVICE_PRECIS;

	StringCopyn( _logFont.lfFaceName,
				 (LPSTR)GET_COBJ_DATA(_lpCobjFont),
				 sizeof(_logFont.lfFaceName) );

	_hFontNew = CreateFontIndirect( &_logFont );
	_hFontOld = SelectObject( hdc_, _hFontNew );
	_iBkModeOld = SetBkMode(hdc_, TRANSPARENT);
	//end of creating the font

	MFPUREAD( _lpPrm, STRING_TEXTHORALIGN, UINT, _uAlignHor );
	MFPUREAD( _lpPrm, STRING_TEXTVERALIGN, UINT, _uAlignVer );
	_uAlign = _uAlignHor|_uAlignVer|DT_SINGLELINE;

	if( _retClr )	//text color is not transparent
		DrawText( hdc_, _buffer, -1, &_rect, _uAlign );

	SelectObject( hdc_, _hFontOld );
	DeleteObject( _hFontNew );

	SetBkMode(hdc_, _iBkModeOld );

	return TRUE;
}

//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC StringBuild(SCMD wCmd_, WRD wFTU_, HWND hwnd_, WORD wParam_, LONG lParam_)
{
	switch(wCmd_)
	{
		case SCMD_INIT:
		{
			LPFINIT			_lpFinit = (LPFINIT)lParam_;
			LPPU			_lpPrm;
			LPCOBJ			_lpCobjFontFace;
			BOOL			_bOk;

			_lpPrm  = FaceGetParameter( wFTU_ );

			_lpCobjFontFace = AllocCString( _lpFinit->text_font.lfFaceName, &_bOk );
			if( !_bOk )
				return FALSE;

			MFPUWRITE(_lpPrm, STRING_BORDERSTYLE, int, _lpFinit->border_pen.lopnStyle);
			MFPUWRITE(_lpPrm, STRING_BORDERWIDTH, DWORD, _lpFinit->border_pen.lopnWidth.x);
			MFPUWRITE(_lpPrm, STRING_BORDERCLR, COLORREF, _lpFinit->border_pen.lopnColor);
			MFPUWRITE(_lpPrm, STRING_BACKCLR, COLORREF, _lpFinit->back_clr);

			//MFPUWRITE(_lpPrm, STRING_FORMAT, LPCOBJ, _lpCobjDefault );
			MFPUWRITE(_lpPrm, STRING_FONT, LPCOBJ, _lpCobjFontFace );
			MFPUWRITE(_lpPrm, STRING_SIZE, WORD, _lpFinit->text_font.lfHeight );
			MFPUWRITE(_lpPrm, STRING_ITALIC, BOOL, _lpFinit->text_font.lfItalic );
			MFPUWRITE(_lpPrm, STRING_BOLD, BOOL, (_lpFinit->text_font.lfWeight >= FW_BOLD) );
			MFPUWRITE(_lpPrm, STRING_TEXTCOLOR, COLORREF, _lpFinit->text_clr );

			MFPUWRITE(_lpPrm, STRING_TEXTVERALIGN, UINT, _lpFinit->text_veralign );
			MFPUWRITE(_lpPrm, STRING_TEXTHORALIGN, UINT, _lpFinit->text_horalign );

			MFPUWRITE(_lpPrm, STRING_MAX, REAL, 0);
			MFPUWRITE(_lpPrm, STRING_MIN, REAL, 100);
			break;
		}

		case SCMD_RUNTIME:
		{
			LPFIA			_lpFia = (LPFIA)lParam_;

			if( _lpFia->MsgCode & MSG_TRIGGER )
			{
				LPPU			_lpPrm;
				SimplePU		_spu;

				_lpPrm  = FaceGetParameter( wFTU_ );
				_spu.prefix = PF_IMM|IMM_NULL;
				PURead( &_spu, (_lpPrm + STRING_DATA) );

				if( _spu.prefix == (PF_IMM|IMM_BOOL) )
					DialogBoxParam( ghInst, MAKEINTRESOURCE(DLG_BOOL), hwnd_, InputDlgProc, wFTU_ );
				else if( _spu.prefix == (PF_IMM|IMM_LPCOBJ) )
					DialogBoxParam( ghInst, MAKEINTRESOURCE(DLG_STRING), hwnd_, InputDlgProc, wFTU_ );
				else
					DialogBoxParam( ghInst, MAKEINTRESOURCE(DLG_NUMBER), hwnd_, InputDlgProc, wFTU_ );
			}

			return FALSE;
		}

		default:
			return DefStampBuild( wCmd_, wFTU_, hwnd_, wParam_, lParam_ );
	}

	return TRUE;
}

//...................................................................
//Description:
//	The function gets value string;
//Return:
//	Returns the datatype of the aim data(PDT_xxx) if success,
//	otherwise, returns NULL;
//...................................................................
WORD _GetDataString( WRD wFTU_, LPSTR lpsz_, WORD size_ )
{
	SimplePU 	_spu;

	LPPU		_lpPrm = FaceGetParameter(wFTU_);
	LPCOBJ		_lpCobj;
	LPSTR		_lpszFormat;
	BOOL		_rt;

	*lpsz_ = 0;

	MFPUREAD( _lpPrm, STRING_FORMAT, LPCOBJ, _lpCobj );
	_lpszFormat = (LPSTR)GET_COBJ_DATA(_lpCobj);
	if(_lpszFormat)
		StringCopyn( lpsz_, _lpszFormat, size_ );	//default value string

	_spu.prefix = PF_IMM|IMM_NULL;
	_rt = PURead( &_spu, (_lpPrm + STRING_DATA) );

	if( (_lpPrm + STRING_DATA)->prefix != (PF_IMM|IMM_USER) )
	{
		if( _lpszFormat && *_lpszFormat )	//has the format string
		{
			if( _rt )	//if the data is valid, set data string
			{
				switch( _spu.prefix & (~PF_MASK) )
				{
					case IMM_REAL:
						sprintf( lpsz_, _lpszFormat, _spu.du.zzk__REAL );
						break;
					case IMM_long:
						sprintf( lpsz_, _lpszFormat, _spu.du.zzk__long );
						break;
					case IMM_DWORD:
						sprintf( lpsz_, _lpszFormat, _spu.du.zzk__DWORD );
						break;
					case IMM_FPTR:
					case IMM_NPTR:
						sprintf( lpsz_, _lpszFormat, _spu.du.zzk__FPTR );
						break;
					case IMM_int:
					case IMM_BOOL:
						sprintf( lpsz_, _lpszFormat, _spu.du.zzk__int );
						break;
					case IMM_WRD:
					case IMM_WORD:
						sprintf( lpsz_, _lpszFormat, _spu.du.zzk__WORD );
						break;
					case IMM_LPCOBJ:
					{
						//only support string-cobj
						if( _spu.du.zzk__LPCOBJ )
						{
							if( CobjGetID(_spu.du.zzk__LPCOBJ) == IDCOBJ_STRING )
								sprintf(lpsz_, _lpszFormat, (LPSTR)GET_COBJ_DATA(_spu.du.zzk__LPCOBJ) );
							else
								StringCopyn(lpsz_, "#Unknown", size_ );
						}
						break;
					}

					case IMM_NULL:
					case IMM_USER:
					default:
						sprintf(lpsz_, _lpszFormat, _spu.du.zzk__FULL );
						break;
				}//end of switch
			}//end of if
			else
			{
				StringCopyn(lpsz_, "#Error", size_ );
			}
		}
		else	//no format string
		{
			GETVALUESTRING	_gvs;

			_gvs.lpszBuffer = lpsz_;
			_gvs.wSize = size_;
			StringParam( PIC_GETVALUESTRING, wFTU_, STRING_DATA, (LONG)&_gvs );
		}
	}//end of if

	return _spu.prefix & (~PF_MASK);
}

DWORD _MyStringParam(PIC wCmd_, WRD wFTU_, WORD wNo_, LONG lParam_)
{
	LPPU		_lppuData;
	PREFIX		_pf;

	_lppuData = FaceGetParameter(wFTU_) + STRING_DATA;
	_pf = _lppuData->prefix & PF_MASK;

	if( ( _pf == PF_MPX) || (_pf == PF_FPX) )
	{
		WRD		_wrd = _lppuData->du.zzk__WRD;
		WORD	_wNo = _lppuData->prefix & MPX_INDEX_MASK;

		if( (_lppuData->prefix & PF_MASK) == PF_MPX )
			return AlgoCallParamProc( ModuGetAlgo(_wrd), wCmd_, _wrd, _wNo, lParam_);
		else if ( (_lppuData->prefix & PF_MASK) == PF_FPX )
			return StampCallParamProc( FaceGetStamp(_wrd), wCmd_, _wrd, _wNo, lParam_);
		else
		{
			CORE_ASSERT( FALSE );
			return FALSE;
		}
	}
	else
	{
		switch(wCmd_)
		{
			case PIC_GETVSOFENUMSTRUCT:
				return 0;

			case PIC_GETVALUESTRING:
			{
				LPGETVALUESTRING	_lpGVS;
				SimplePU			_spu;
				WORD				_vsof;

				_lpGVS = (LPGETVALUESTRING)lParam_;

				_spu.prefix = PF_IMM|IMM_NULL;
				PURead( &_spu, _lppuData );
				_vsof = _spu.prefix & (~PF_MASK);

				return PUGetValueString(_lppuData, _lpGVS, _vsof);
			}

			case PIC_SETVALUESTRING:
			{
				SimplePU	_spu;
				LPSTR		_lpsz;
				VSOF		_vsof;

				_lpsz = (LPSTR)lParam_;

				_spu.prefix = PF_IMM|IMM_NULL;
				PURead( &_spu, _lppuData );
				_vsof = MAKEVSOF( _spu.prefix & (~PF_MASK), 0);
				if( _vsof == VSOF_LPCOBJ)
					_vsof = VSOF_LPCOBJ_STR;

				return PUSetValueString(_lppuData, _lpsz, _vsof);
			}

			case PIC_GETPARAMVSOF:
			{
				SimplePU	_spu;
				LPSTR		_lpsz;
				VSOF		_vsof;

				_lpsz = (LPSTR)lParam_;

				_spu.prefix = PF_IMM|IMM_NULL;
				PURead( &_spu, _lppuData );
				_vsof = MAKEVSOF( _spu.prefix & (~PF_MASK), 0);
				if( _vsof == VSOF_LPCOBJ)
					_vsof = VSOF_LPCOBJ_STR;
				return _vsof;
			}

			default:
				CORE_ASSERT( FALSE );
				return 0L;
		}//end switch
	}
}

//...................................................................
//Description:
//	The function gets data value from a string;
//	sets the string to format string when the data source is undefined;
//Entries:
//	bEnableMMChecking_		if sets, min/max checking is enabled;
//
//Return:
//	TRUE/FALSE
//...................................................................
BOOL _SetDataValue( WRD wFTU_, LPSTR lpsz_, BOOL bEnableMMChecking_, REAL min_, REAL max_ )
{
	SimplePU	_spu;
	LPPU		_lpPrm;

	_lpPrm = FaceGetParameter(wFTU_);

	if( (_lpPrm + STRING_DATA)->prefix == (PF_IMM|IMM_USER) )
	{
		//set the data value as the format string
		BOOL	_bOk;
		LPCOBJ	_lpCobj = AllocCString( lpsz_, &_bOk );
		if( !_bOk )
			return FALSE;

		_spu.prefix = PF_IMM|IMM_LPCOBJ;
		_spu.du.zzk__LPCOBJ = _lpCobj;
		return PUWrite( _lpPrm+STRING_FORMAT, &_spu );
	}
	else
	{
		if( bEnableMMChecking_ && ScanNumber( lpsz_, &_spu ) )
		{
			REAL	_val;

			READPU( &_spu, REAL, _val );
			if( (_val > max_) || (_val < min_) )
				return FALSE;
		}
		return StringParam( PIC_SETVALUESTRING, wFTU_, STRING_DATA, (LONG)lpsz_ );
	}
}

//...................................................................
//...................................................................
#ifdef _WIN32
BOOL CALLBACK
#else
BOOL CALLBACK _export 
#endif
InputDlgProc( HWND hwnd_, UINT msg_, WPARAM wParam_, LPARAM lParam_ )
{
	static WRD		_swFTU;
	static WORD		_swDataType;
	static LPPU		_slpPrm;
	static BOOL		_sbEnableMMChecking;

	static float	_srMin, _srMax;
	SimplePU 		_spu;
	char			_buffer[256];

	switch(msg_)
	{
		case WM_INITDIALOG:
		{
			_swFTU = (WRD)lParam_;
			_slpPrm = FaceGetParameter( _swFTU );

			_swDataType = _GetDataString( _swFTU, _buffer, sizeof(_buffer) );

			_spu.prefix = PF_IMM|IMM_NULL;
			PURead( &_spu, (_slpPrm + STRING_DATA) );
			MFPUREAD( _slpPrm, STRING_MMCHECK, BOOL, _sbEnableMMChecking );

			if( _swDataType == PDT_BOOL )
			{
				//to input value as bool
				CheckRadioButton(hwnd_,
								 IDC_TRUE,
								 IDC_FALSE,
								 _spu.du.zzk__BOOL? IDC_TRUE: IDC_FALSE );
			}
			else if( _swDataType == PDT_LPCOBJ )
			{
				//to input value as string
				SetDlgItemText( hwnd_, IDC_VALUE, _buffer );
			}
			else
			{
				SetDlgItemText( hwnd_, IDC_VALUE, _buffer );

				MFPUREAD(_slpPrm, STRING_MAX, REAL, _srMax );
				MFPUREAD(_slpPrm, STRING_MIN, REAL, _srMin );
				if( _sbEnableMMChecking )
				{
					sprintf( _buffer, "the min value is %f", _srMin );
					SetDlgItemText( hwnd_, IDC_MINVALUE, _buffer );
					sprintf( _buffer, "the max value is %f", _srMax );
					SetDlgItemText( hwnd_, IDC_MAXVALUE, _buffer );
				}
				else
				{
					SetDlgItemText( hwnd_, IDC_MINVALUE, "" );
					SetDlgItemText( hwnd_, IDC_MAXVALUE, "" );
				}
			}
			break;
		}

		case WM_COMMAND:
			switch(wParam_)
			{
				case IDOK:
				{
					BOOL	_ret;

					if( _swDataType == IMM_USER )
					{
						//the data source is undefined
						GetDlgItemText( hwnd_, IDC_VALUE, _buffer, sizeof(_buffer) );
						_ret = _SetDataValue(_swFTU, _buffer, FALSE, 0, 0);
					}
					else if( _swDataType == IMM_BOOL )
					{
						BOOL	_bv = FALSE;

						if( IsDlgButtonChecked(hwnd_, IDC_TRUE) )
							_bv = TRUE;

						_ret = _SetDataValue(_swFTU, _bv?"1":"0", FALSE, 0, 0);
					}
					else if( _swDataType == IMM_LPCOBJ )
					{
						GetDlgItemText( hwnd_, IDC_VALUE, _buffer, sizeof(_buffer) );
						_ret = _SetDataValue(_swFTU, _buffer, FALSE, 0, 0);
					}
					else
					{
						GetDlgItemText( hwnd_, IDC_VALUE, _buffer, sizeof(_buffer) );
						_ret = _SetDataValue( _swFTU,
											  _buffer,
											  _sbEnableMMChecking,
											  _srMin,
											  _srMax );
					}

					if( _ret )	//if success, end dlg box
						EndDialog(hwnd_, IDOK);
					else
						MessageBeep( MB_ICONHAND );

					break;
				}

				case IDCANCEL:
					EndDialog(hwnd_, IDCANCEL);
					return TRUE;

				default:
					return FALSE;
			}

		default:
			return FALSE;
	}	//end of switch( msg_ )

	return TRUE;
}

