//===================================================================
//cpu.c
//Zheng zhikai	96-01-09
//===================================================================
#include "prehead.h"

#include "coreall.h"
#include <ctype.h>
#include <time.h>

BOOL _GetSystemVsofEnumStruct( VSOF vsof_, LPVSOFENUMSTRUCT lpes_ );

BOOL _GetUserVsofValueString( WORD wTable_, WRD wrd_, WORD wNo_, LPGETVALUESTRING lpGVS_ );
BOOL _SetUserVsofValueString( WORD wTable_, WRD wrd_, WORD wNo_, LPCSTR lpsz_ );
BOOL _GetUserVsofValueString2( WORD wTable_, DWORD id_, WORD wNo_, LPPU lppu_, LPGETVALUESTRING lpGVS_ );
BOOL _SetUserVsofValueString2( WORD wTable_, DWORD id_, WORD wNo_, LPPU lppu_, LPCSTR lpsz_ );

BOOL GetVsofName( LPVSOFENUMSTRUCT lpEnum_, DWORD value_, LPSTR lpsz_, int buf_size_ );
int MatchVsofName( LPVSOFENUMSTRUCT lpEnum_, LPCSTR lpsz_ );


BOOL BeginEnum( LPVSOFENUMSTRUCT lpes_, LPPU lpPU_, LPSTR lpsz_, UINT size_  );
BOOL EnumNext( LPVSOFENUMSTRUCT lpes_, LPPU lpPU_, LPSTR lpsz_, UINT size_ );

BOOL _binstr2dw( LPCSTR lpsz_, DWORD FAR* lpdata_ );
BOOL _w2binstr( WORD data_, LPSTR lpsz_, WORD size_ );
BOOL _dw2binstr( DWORD data_, LPSTR lpsz_, WORD size_ );

static near _PIC_Create(WRD wrd_, WORD wTable_);
static DWORD _near _PIC_CheckParam( WRD wrd_, WORD wTable_, WORD	wNumPDU_, LPPDU	lpPDU_ );
static DWORD _near _PIC_CheckParam_Var(WRD			wrd_,
									   WORD			wTable_,
									   LPVPSTRUCT	lpVPD_);

char	gszFormat_int[32] = "%i";
char	gszFormat_WORD[32] = "%u";
char	gszFormat_WORD_HEX[32] = "0x%X";
char	gszFormat_long[32] = "%li";
char	gszFormat_DWORD[32] = "%lu";
char	gszFormat_DWORD_HEX[32] = "0x%lX";
char	gszFormat_DWORD_RGB[32] = "%u.%u.%u";		//R/G/B
char	gszFormat_DWORD_MS[32] = "%d:%d:%d.%03d";		//time in millisecond
char	gszFormat_DWORD_MS_NOMS[32] = "%d:%d:%d";		//time in second
char	gszFormat_DWORD_BWIDTH[32] = "(%u,%u)";
char	gszFormat_REAL[32] = "%g";
char	gszFormat_BOOL[32] = "%s";	//	"FALSE" / "TRUE" ...
char	gszFormat_STR[32] = "%s";	//
char	gszFormat_FPTR[32] = "%Fp";
char 	gszFormat_FPTR_RECT[32] = "{%d, %d, %d, %d}";

char	gszFormat_NPTR[32] = "%Np";
char	gszFormat_WRD[32] = "#%u";

struct {
	char	szTrue[16];
	char	szFalse[16];
} static sBoolName[] =
{
	{"True",	"False"},	//VSOF_BOOL_TRUEFALSE
	{"Yes",		"No"},		//VSOF_BOOL_YESNO
	{"On",		"Off"},		//VSOF_BOOL_ONOFF
	{"Open",	"Close"},	//VSOF_BOOL_OPENCLOSE 
	{"1",		"0"},		//VSOF_BOOL_10
	{"T",		"F"},
	{"Y",		"N"},
};
#define _NUM_BOOL_STRINGS (sizeof(sBoolName)/sizeof(sBoolName[0]))

static struct _tag_CCLRName{
	CCLR	clr;
	char	szName[32];
} sCclrName[] =
{
	CCLR_NULL,					"clr_Null",
	CCLR_SCROLLBAR,				"clr_ScrollBar",
	CCLR_BACKGROUND,			"clr_BackGround",
	CCLR_ACTIVECAPTION,			"clr_ActiveCaption",
	CCLR_INACTIVECAPTION,		"clr_InactiveCaption",
	CCLR_MENU,					"clr_Menu",
	CCLR_WINDOW,				"clr_Window",
	CCLR_WINDOWFRAME,			"clr_WindowFrame",
	CCLR_MENUTEXT,				"clr_MenuText",
	CCLR_WINDOWTEXT,			"clr_WindowText",
	CCLR_CAPTIONTEXT,			"clr_CaptionText",
	CCLR_ACTIVEBORDER,			"clr_ActiveBorder",
	CCLR_INACTIVEBORDER,		"clr_InactiveBorder",
	CCLR_APPWORKSPACE,			"clr_AppWorkSpace",
	CCLR_HIGHLIGHT,				"clr_HightLight",
	CCLR_HIGHLIGHTTEXT,			"clr_HightLightText",
	CCLR_BTNFACE,				"clr_3D",
	CCLR_BTNSHADOW,				"clr_3DS",
	CCLR_GRAYTEXT,				"clr_GrayText",
	CCLR_BTNTEXT,				"clr_3DT",
	CCLR_INACTIVECAPTIONTEXT,	"clr_InactiveCaptionText",
	CCLR_BTNHIGHLIGHT,			"clr_3DHL",
	CCLR_3DDKSHADOW,			"clr_3DDS",
	CCLR_3DLIGHT,				"clr_3DL",
	CCLR_INFOTEXT,				"clr_InfoText",
	CCLR_INFOBK,				"clr_InfoBack",
};

//sign for decode of PU, using as: 	szHead.szBody.szTail
char	sszDOT[2] = ".";

extern	MTInfo		gMTInfo;
extern	FTInfo		gFTInfo;
extern	DWORD		gdwLastError;
extern 	CoreInfo	gCoreInfo;

//...................................................................
//Description:
//	Specifies the default action responses for PIC_xxxx commands
//
//...................................................................
_DLLEXP DWORD _DLLFUNC DefProcessPIC(
	PIC		wCmd_,
	WRD		wrd_,
	WORD	wNo_,
	LONG	lParam_,
	WORD	wTable_,	//TBX_FT or TBX_MT
	WORD	wNumPDU_,
	DWORD	id_,		//IDSTAMP or IDALGO
	LPPDU	lpPDU_)
{
#ifndef NDEBUG
	int	_ii;
	for(_ii=0; _ii<wNumPDU_; _ii++)
	{
		if( lpPDU_[_ii].wNo != _ii )
			CORE_ASSERT(0);
	}
#endif

	CORE_ASSERT( (wTable_ == TBX_FT) || (wTable_ == TBX_MT) );

	CLEAR_LAST_ERROR;

	switch(wCmd_)
	{
		case PIC_CREATE:
			return _PIC_Create(wrd_, wTable_);

		case PIC_GETPARAMNAME:
		{
			LPGETPARAMNAME	_lpgpn = (LPGETPARAMNAME)lParam_;
			CORE_ASSERT(_lpgpn);

			if( wNo_ < wNumPDU_ )
			{
				CORE_ASSERT( sizeof(_lpgpn->szName) >= sizeof(lpPDU_[wNo_].szName) );
				strcpy(_lpgpn->szName, lpPDU_[wNo_].szName);
			}
			else
			{
				CORE_ASSERT( !wNumPDU_ );	//no parameter
				*(_lpgpn->szName) = 0;  //null string
				return FALSE;
			}

			if(_lpgpn->wAccess & AM_ENABLE_UL)
			{
				if(_lpgpn->wAccess & AM_UPPER)
					strupr(_lpgpn->szName);
				else
					strlwr(_lpgpn->szName);
			}
			return TRUE;
		}

		case PIC_GETPARAMORDER:
		{
			LPGETPARAMORDER	_lpGPO = (LPGETPARAMORDER)lParam_;
			unsigned 		_u;

			for(_u=0; _u<wNumPDU_; _u++)
			{
				if( !strcmpi(lpPDU_[_u].szName, _lpGPO->lpszParamName) )
				{
					_lpGPO->wOrder = _u;
					return TRUE;
				}
			}

			return FALSE;
		}

		case PIC_GETNUMPARAM:
			return wNumPDU_;

		case PIC_GETPARAMPRIVILEGE:
			if( wNo_ < wNumPDU_ )
				return (WORD) ( (lpPDU_[wNo_].flags) & 0x000F);
			else
			{
				CORE_ASSERT( !wNumPDU_ );
				return NULL;
			}

		case PIC_GETPARAMTYPE:
			if( wNo_ < wNumPDU_ )
				return lpPDU_[wNo_].pu.prefix & IMM_MASK;
			else
			{
				CORE_ASSERT(!wNumPDU_);
				return NULL;
			}

		case PIC_GETPARAMVSOF:
			if( wNo_ < wNumPDU_ )
				return (lpPDU_[wNo_].vsof);
			else
			{
				CORE_ASSERT(!wNumPDU_);
				return NULL;
			}

		case PIC_GETPARAMTYPENAME:
		{
			LPGETPARAMTYPENAME	_lp = (LPGETPARAMTYPENAME)lParam_;
			CORE_ASSERT(_lp);

			if( wNo_ < wNumPDU_ )
				return GetPDTName( (PDT)(lpPDU_[wNo_].pu.prefix & IMM_MASK),
									 _lp->szName,
									 sizeof(_lp->szName),
									 _lp->wAccess );
			else
			{
				CORE_ASSERT(!wNumPDU_);
				*(_lp->szName) = 0;
				return NULL;
			}
		}

		case PIC_DECODE:
		{
			LPDECODE _lp = (LPDECODE)lParam_;
			LPPU	_lpPU;

			CORE_ASSERT( _lp );
			if( wNo_ >= wNumPDU_ )
			{
				CORE_ASSERT(!wNumPDU_);
				*_lp->szHead = *_lp->szBody = *_lp->szTail = 0;
				_lp->wBody = _lp->wTail = 0;
				return FALSE;
			}

			if(wTable_ == TBX_FT)
				_lpPU = (gFTInfo.lpBase + wrd_)->lpParam + wNo_;
			else if(wTable_ == TBX_MT)
				_lpPU = (gMTInfo.lpBase + wrd_)->lpParam + wNo_;
			else
			{
				CORE_ASSERT(0);
				return FALSE;
			}

			return PUDecode(_lpPU, _lp);
		}

		case PIC_ENCODE:
		{
			LPENCODE _lp = (LPENCODE)lParam_;
			LPPU	_lpPU;

			CORE_ASSERT( _lp );
			if( wNo_ >= wNumPDU_ )
			{
				CORE_ASSERT(!wNumPDU_);
				return FALSE;
			}

			if(wTable_ == TBX_FT)
				_lpPU = (gFTInfo.lpBase + wrd_)->lpParam + wNo_;
			else if(wTable_ == TBX_MT)
				_lpPU = (gMTInfo.lpBase + wrd_)->lpParam + wNo_;
			else
			{
				CORE_ASSERT(0);
				return FALSE;
			}

			return PUEncode(_lpPU, _lp);
		}

		case PIC_GETVALUESTRING:
		{
			DWORD				_ret_gvs;
			LPPU				_lpPU;
			LPGETVALUESTRING	_lpGVS;

			_lpGVS = (LPGETVALUESTRING)lParam_;

			if( !lpPDU_ ) {
				CORE_ASSERT( FALSE );
				return FALSE;
			}

			if(wTable_ == TBX_FT)
				_lpPU = (gFTInfo.lpBase[wrd_]).lpParam + wNo_;
			else 
				_lpPU = (gMTInfo.lpBase[wrd_]).lpParam + wNo_;

			_ret_gvs = PUGetValueString(_lpPU, _lpGVS, (lpPDU_ + wNo_)->vsof );
			if( _ret_gvs == GVSRESULT_USERDEF )	//user defined
				return _GetUserVsofValueString( wTable_, wrd_, wNo_, _lpGVS );
			else
				return _ret_gvs;
		}

		case PIC_SETVALUESTRING:
		{
			DWORD		_ret;
			LPPU		_lpPU;
			LPSTR		_lpsz = (LPSTR)lParam_;

			if( !lpPDU_ ) {
				CORE_ASSERT( FALSE );
				return FALSE;
			}

			if(wTable_ == TBX_FT)
				_lpPU = (gFTInfo.lpBase[wrd_]).lpParam + wNo_;
			else if(wTable_ == TBX_MT)
				_lpPU = (gMTInfo.lpBase[wrd_]).lpParam + wNo_;
			else
			{
				CORE_ASSERT(0);
				return FALSE;
			}

			_ret = PUSetValueString(_lpPU, _lpsz, (lpPDU_ + wNo_)->vsof );
			if( _ret==3 )
				return _SetUserVsofValueString( wTable_, wrd_, wNo_, _lpsz );
			else
				return _ret;
		}

		case PIC_CHECKPARAM:
			return _PIC_CheckParam( wrd_, wTable_, wNumPDU_, lpPDU_ );

		case PIC_INITIATE:
		case PIC_STOP:
		case PIC_DELETE:

		case PIC_ONLOAD:
		case PIC_ONEXIT:
			return TRUE;

		case PIC_ISVARPARAM:
		case PIC_CANDELETE:
		case PIC_CANINSERT:
		case PIC_CANAPPEND:
			return FALSE;

		case PIC_PASTE:
		case PIC_CANCOPY:
			return TRUE;

		case PIC_INITIALIZEPARAM:
		{
			LPPU	_lpPrm;

			if( wTable_ == TBX_FT ) {
				_lpPrm = FaceGetParameter(wrd_);
				CORE_ASSERT( FaceGetNumParam( wrd_ ) > wNo_ );
			}
			else if( wTable_ == TBX_MT ) {
				_lpPrm = ModuGetParameter(wrd_);
				CORE_ASSERT( ModuGetNumParam( wrd_ ) > wNo_ );
			}
			else {
				CORE_ASSERT( FALSE );
				return FALSE;
			}
			if( !_lpPrm )
				return FALSE;

			if( wNumPDU_ )
			{
				CORE_ASSERT( wNo_ <= wNumPDU_ );
				memcpy( _lpPrm+wNo_, &(lpPDU_[wNo_].pu), sizeof(PU) );
				(_lpPrm+wNo_)->flags |= PU_DIRTY;
			}
        	return TRUE;
		}

		case PIC_SAVEDEFAULT:
		case PIC_LOADDEFAULT:
		{
			int		_i;
			BOOL	_ret = TRUE;
			char	_szBuffer[128];
			LPSTR	_lpszSection;
			LPSTR	_lpszFile;

			_lpszFile = (LPSTR)lParam_;
			_lpszSection = _lpszFile + strlen(_lpszFile) + 1;
			
			for( _i=0; _i<wNumPDU_; _i++ )
			{
				if( lpPDU_[_i].flags & PDUF_NODEFAULT )
					continue;
				if( lpPDU_[_i].pu.prefix == (PF_IMM|IMM_LPCOBJ) )
					continue;
				if( lpPDU_[_i].pu.flags & PU_NIV )
					continue;

				if( wCmd_ == PIC_LOADDEFAULT )
				{
					DWORD	_retsvs;

					GetPrivateProfileString( _lpszSection,
											 lpPDU_[_i].szName,
											 "0",
											 _szBuffer,
											 sizeof(_szBuffer),
											 _lpszFile );

					_retsvs = PUSetValueString(&(lpPDU_[_i].pu),
										 _szBuffer,
										 lpPDU_[_i].vsof );

					if( _retsvs == GVSRESULT_USERDEF )
						_retsvs = _SetUserVsofValueString2( wTable_, id_, (WORD)_i, &(lpPDU_[_i].pu), _szBuffer );

					if( _retsvs != GVSRESULT_SUCCESS )
						_ret = FALSE;
					else
					{
						//clear DIRTY bit
						lpPDU_[_i].pu.flags &= (~PU_DIRTY);
					}
				}
				else	//PIC_SAVEDEFAULT
				{
					DWORD	_retGVS;
					GETVALUESTRING	_gvs;

					if( lpPDU_[_i].flags & PDUF_CANNTCOPY )
						continue;

					_gvs.lpszBuffer = _szBuffer;
					_gvs.wSize = sizeof(_szBuffer);
					_retGVS = PUGetValueString(&(lpPDU_[_i].pu),
										 &_gvs,
										 lpPDU_[_i].vsof );

					if( _retGVS == GVSRESULT_USERDEF)
					{
						_retGVS = _GetUserVsofValueString2( wTable_, id_, (WORD)_i, &(lpPDU_[_i].pu), &_gvs )? GVSRESULT_SUCCESS : GVSRESULT_FAILED;
					}

					if( (_retGVS != GVSRESULT_SUCCESS) && (_retGVS != GVSRESULT_COMPLEX_SUCCESS) ) 
					{
						_ret = FALSE;
						continue;
					}

					WritePrivateProfileString(_lpszSection,
											 lpPDU_[_i].szName,
											 _szBuffer,
											 _lpszFile );
				}
			}

			return _ret;
		}

		case PIC_PASTEDEFAULT:
		{
			WORD	_i;
			LPPU	_lpPrm;
			BOOL	_ret = TRUE;

			if( wTable_ == TBX_FT ) {
				_lpPrm = FaceGetParameter( wrd_ );
			}
			else if( wTable_ == TBX_MT ) {
				_lpPrm = ModuGetParameter( wrd_ );
			}
			else {
				CORE_ASSERT( FALSE );
				return TRUE;
			}

			if( !_lpPrm || !wNumPDU_ )
				return TRUE;
			
			for( _i=0; _i<wNumPDU_; _i++ )
			{
				if( lpPDU_[_i].flags & PDUF_NODEFAULT )
					continue;
				if( lpPDU_[_i].flags & PDUF_CANNTCOPY )
					continue;
				if( lpPDU_[_i].pu.prefix == (PF_IMM|IMM_LPCOBJ) )
					continue;
				if( lpPDU_[_i].pu.flags & PU_NIV )
					continue;

				if( !PUWrite( _lpPrm + _i, &(lpPDU_[_i].pu) ) )
					_ret = FALSE;
			}

			return _ret;
		}

		case PIC_SETDEFAULT:
		{
			WORD	_i;
			LPPU	_lpPrm;
			BOOL	_ret = TRUE;

			if( wTable_ == TBX_FT ) {
				_lpPrm = FaceGetParameter( (WRD)lParam_ );
			}
			else if( wTable_ == TBX_MT ) {
				_lpPrm = ModuGetParameter( (WRD)lParam_ );
			}
			else {
				CORE_ASSERT( FALSE );
				return TRUE;
			}

			if( !_lpPrm || !wNumPDU_ )
				return TRUE;
			
			for( _i=0; _i<wNumPDU_; _i++ )
			{
				if( lpPDU_[_i].flags & PDUF_NODEFAULT )
					continue;
				if( lpPDU_[_i].flags & PDUF_CANNTCOPY )
					continue;
				if( lpPDU_[_i].pu.prefix == (PF_IMM|IMM_LPCOBJ) )
					continue;
				if( lpPDU_[_i].pu.flags & PU_NIV )
					continue;

				if( !PUWrite( &(lpPDU_[_i].pu), _lpPrm + _i ) )
					_ret = FALSE;
				else
				{
					//clear DIRTY bit
					lpPDU_[_i].pu.flags &= (~PU_DIRTY);
				}
			}

			return _ret;
		}

		case PIC_GETVSOFENUMSTRUCT:
		{
			VSOF	_vsof = lpPDU_[wNo_].vsof;
			return _GetSystemVsofEnumStruct( _vsof, (LPVSOFENUMSTRUCT)lParam_ );
		}

		case PIC_BEFORESAVE:
		case PIC_AFTERLOAD:
			return TRUE;

		case PIC_GETICON:
		case PIC_GETDESCRIPTION_SHORT:
		case PIC_GETDESCRIPTION_LONG:
			return 0L;

		default:
			CORE_ASSERT(0);
			return FALSE;
	}
}

//...................................................................
//Description:
//	Specifies the default action responses for PIC_xxxx commands
//
//...................................................................
_DLLEXP DWORD _DLLFUNC DefProcessVarPIC(
	PIC			wCmd_,
	WRD			wrd_,
	WORD		wNo_,
	LONG		lParam_,
	WORD		wTable_,	//TBX_FT or TBX_MT
	DWORD		id_,		//IDSTAMP or IDALGO
	LPVPSTRUCT	lpVPD_)
{
	CLEAR_LAST_ERROR;

	switch(wCmd_)
	{
		case PIC_CREATE:
			return _PIC_Create(wrd_, wTable_);

		case PIC_GETPARAMVSOF:
			if( wNo_ < lpVPD_->wNumFix )
				return DefProcessPIC( wCmd_, wrd_, wNo_, lParam_,
							  wTable_,
							  lpVPD_->wNumFix,
							  id_,
							  lpVPD_->lpFixPdu );
			else
			{
				div_t	_dv = div( wNo_ - lpVPD_->wNumFix, lpVPD_->wNumVar );
				return (lpVPD_->lpVarPdu)[_dv.rem].vsof;
			}

		case PIC_GETPARAMNAME:
		{
			if( wNo_ < lpVPD_->wNumFix )
				return DefProcessPIC( wCmd_, wrd_, wNo_, lParam_,
							  wTable_,
							  lpVPD_->wNumFix,
							  id_,
							  lpVPD_->lpFixPdu );
			else
			{
				LPGETPARAMNAME	_lpgpn;
				char	_szBuffer[64];
				div_t	_dv;

				_lpgpn = (LPGETPARAMNAME)lParam_;
				CORE_ASSERT(_lpgpn);

				_dv = div(wNo_ - lpVPD_->wNumFix, lpVPD_->wNumVar);

				strcpy(_szBuffer, (lpVPD_->lpVarPdu + _dv.rem)->szName );
				itoa(_dv.quot + lpVPD_->wVarLowBound,
					 _szBuffer + strlen(_szBuffer),
					 10);

				strcpy(_lpgpn->szName, _szBuffer );
				if(_lpgpn->wAccess & AM_ENABLE_UL)
				{
					if(_lpgpn->wAccess & AM_UPPER)
						strupr(_lpgpn->szName);
					else
						strlwr(_lpgpn->szName);
				}

				return TRUE;
			}
		}

		case PIC_GETPARAMORDER:
		{
			LPGETPARAMORDER	_lpGPO = (LPGETPARAMORDER)lParam_;

			BOOL	_ret;
			_ret = (BOOL)DefProcessPIC( wCmd_, wrd_, wNo_, lParam_,
							  wTable_,
							  lpVPD_->wNumFix,
							  id_,
							  lpVPD_->lpFixPdu );

			if( _ret )
				return _ret;
			else
			{
				LPSTR		_lpc, _lpc1;
				unsigned	_strlen;
				unsigned	_i;
				int			_var;

				for(_i=0; _i<lpVPD_->wNumVar; _i++)
				{
					_strlen = strlen( (lpVPD_->lpVarPdu)[_i].szName );

					if( strnicmp( _lpGPO->lpszParamName,
								  (lpVPD_->lpVarPdu)[_i].szName,
								  _strlen ) )
						continue;

					_lpc = _lpGPO->lpszParamName + _strlen;
					_lpc1 = _lpc;

					while( isdigit(*_lpc) ) _lpc++;
					if(*_lpc)	//if there is an undigit char
						continue;

					_var = atoi(_lpc1) - lpVPD_->wVarLowBound;
					_lpGPO->wOrder = _var * lpVPD_->wNumVar	+ _i + lpVPD_->wNumFix;
					return TRUE;
				}

				return FALSE;
			}
		}

		case PIC_GETPARAMPRIVILEGE:
			if( wNo_ < lpVPD_->wNumFix )
				return DefProcessPIC( wCmd_, wrd_, wNo_, lParam_,
							  wTable_,
							  lpVPD_->wNumFix,
							  id_,
							  lpVPD_->lpFixPdu );
			else
			{
				div_t	_dv = div( wNo_ - lpVPD_->wNumFix, lpVPD_->wNumVar );
				return (WORD)( (lpVPD_->lpVarPdu)[_dv.rem].flags & 0x000F );
			}

		case PIC_GETPARAMTYPE:
			if( wNo_ < lpVPD_->wNumFix )
				return DefProcessPIC( wCmd_, wrd_, wNo_, lParam_,
							  wTable_,
							  lpVPD_->wNumFix,
							  id_,
							  lpVPD_->lpFixPdu );
			else
			{
				div_t	_dv = div( wNo_ - lpVPD_->wNumFix, lpVPD_->wNumVar );
				return (lpVPD_->lpVarPdu)[_dv.rem].pu.prefix & IMM_MASK;
			}

		case PIC_GETPARAMTYPENAME:
		{
			LPGETPARAMTYPENAME	_lp = (LPGETPARAMTYPENAME)lParam_;
			CORE_ASSERT(_lp);

			if( wNo_ < lpVPD_->wNumFix )
				return DefProcessPIC( wCmd_, wrd_, wNo_, lParam_,
							  wTable_,
							  lpVPD_->wNumFix,
							  id_,
							  lpVPD_->lpFixPdu );
			else
			{
				div_t	_dv = div( wNo_ - lpVPD_->wNumFix, lpVPD_->wNumVar );
				return GetPDTName((PDT)( (lpVPD_->lpVarPdu)[_dv.rem].pu.prefix & IMM_MASK ),
								  _lp->szName,
								  sizeof(_lp->szName),
								  _lp->wAccess);
			}
		}

		case PIC_GETNUMPARAM:
			return (lpVPD_->wMinVarGroup * lpVPD_->wNumVar + lpVPD_->wNumFix);

		case PIC_ENCODE:
		{
			LPENCODE _lp = (LPENCODE)lParam_;
			LPPU	_lpPU;

			CORE_ASSERT( _lp );

			if(wTable_ == TBX_FT)
				_lpPU = (gFTInfo.lpBase + wrd_)->lpParam + wNo_;
			else if(wTable_ == TBX_MT)
				_lpPU = (gMTInfo.lpBase + wrd_)->lpParam + wNo_;
			else
			{
				CORE_ASSERT(0);
				return FALSE;
			}
			return PUEncode(_lpPU, _lp);
		}

		case PIC_DECODE:
		{
			LPDECODE _lp = (LPDECODE)lParam_;
			LPPU	_lpPU;

			CORE_ASSERT( _lp );

			if(wTable_ == TBX_FT)
				_lpPU = (gFTInfo.lpBase + wrd_)->lpParam + wNo_;
			else if(wTable_ == TBX_MT)
				_lpPU = (gMTInfo.lpBase + wrd_)->lpParam + wNo_;
			else
			{
				CORE_ASSERT(0);
				return FALSE;
			}

			return PUDecode(_lpPU, _lp);
		}

		case PIC_GETVALUESTRING:
		{
			DWORD				_ret;
			LPPU				_lpPU;
			LPGETVALUESTRING	_lpGVS;

			_lpGVS = (LPGETVALUESTRING)lParam_;

			if(wTable_ == TBX_FT)
				_lpPU = (gFTInfo.lpBase[wrd_]).lpParam + wNo_;
			else if(wTable_ == TBX_MT)
				_lpPU = (gMTInfo.lpBase[wrd_]).lpParam + wNo_;
			else
			{
				CORE_ASSERT(0);
				return FALSE;
			}

			if( wNo_ < lpVPD_->wNumFix )
				_ret = PUGetValueString(_lpPU,
										 _lpGVS,
										 (lpVPD_->lpFixPdu + wNo_)->vsof );
			else
			{
				div_t	_dv = div( wNo_ - lpVPD_->wNumFix, lpVPD_->wNumVar );
				_ret = PUGetValueString(_lpPU,
										 _lpGVS,
										 (lpVPD_->lpVarPdu + _dv.rem)->vsof );
			}

			if( _ret == GVSRESULT_USERDEF )
				return _GetUserVsofValueString( wTable_, wrd_, wNo_, _lpGVS );
			else
				return _ret;
		}

		case PIC_SETVALUESTRING:
		{
			DWORD		_ret;
			LPPU		_lpPU;
			LPSTR		_lpsz = (LPSTR)lParam_;

			if(wTable_ == TBX_FT)
				_lpPU = (gFTInfo.lpBase[wrd_]).lpParam + wNo_;
			else if(wTable_ == TBX_MT)
				_lpPU = (gMTInfo.lpBase[wrd_]).lpParam + wNo_;
			else
			{
				CORE_ASSERT(0);
				return FALSE;
			}

			if( wNo_ < lpVPD_->wNumFix )
				_ret = PUSetValueString(_lpPU,
										 _lpsz,
										 (lpVPD_->lpFixPdu + wNo_)->vsof );
			else
			{
				div_t	_dv = div( wNo_ - lpVPD_->wNumFix, lpVPD_->wNumVar );
				_ret = PUSetValueString(_lpPU,
										 _lpsz,
										 (lpVPD_->lpVarPdu + _dv.rem)->vsof );
			}
			if( _ret==3 )
				return _SetUserVsofValueString( wTable_, wrd_, wNo_, _lpsz );
			else
				return _ret;
		}

		case PIC_INITIATE:
		case PIC_STOP:
		case PIC_DELETE:

		case PIC_ONLOAD:
		case PIC_ONEXIT:
			return TRUE;

		case PIC_ISVARPARAM:
			return TRUE;

		case PIC_CANDELETE:
		{
			div_t	_dv;
			WORD	_wNumPrm;

			if(wTable_ == TBX_FT)
				_wNumPrm = FaceGetNumParam(wrd_);
			else if(wTable_ == TBX_MT)
				_wNumPrm = ModuGetNumParam(wrd_);
			else
			{
				CORE_ASSERT(0);
				return FALSE;
			}

			if( wNo_ >= _wNumPrm )
				return FALSE;

			if( _wNumPrm > (lpVPD_->wNumFix + lpVPD_->wNumVar*lpVPD_->wMinVarGroup) )
			{
				_dv = div( wNo_ - lpVPD_->wNumFix, lpVPD_->wNumVar );
				return ( (_dv.quot>=0) && (_dv.rem==0) )? lpVPD_->wNumVar : 0;
			}
			else
				return FALSE;
		}

		case PIC_CANINSERT:
		{
			div_t	_dv;
			WORD	_wNumPrm;

			if(wTable_ == TBX_FT)
				_wNumPrm = FaceGetNumParam(wrd_);
			else if(wTable_ == TBX_MT)
				_wNumPrm = ModuGetNumParam(wrd_);
			else {
				CORE_ASSERT(0);
				return FALSE;
			}

			if( wNo_ >= _wNumPrm )
				return FALSE;

			_dv = div( wNo_ - lpVPD_->wNumFix, lpVPD_->wNumVar );
			return ( (_dv.quot>=0) && (_dv.rem==0) )? lpVPD_->wNumVar : 0;
		}

		case PIC_CANAPPEND:
			return lpVPD_->wNumVar;

		case PIC_INITIALIZEPARAM:
		{
			LPPU	_lpPrm;
			div_t	_dv;

			if( wNo_ < lpVPD_->wNumFix )
				return DefProcessPIC( wCmd_, wrd_, wNo_, lParam_,
							  wTable_,
							  lpVPD_->wNumFix,
							  id_,
							  lpVPD_->lpFixPdu );

			_dv = div( wNo_ - lpVPD_->wNumFix, lpVPD_->wNumVar );

			if( wTable_ == TBX_FT ) {
				_lpPrm = FaceGetParameter(wrd_);
				CORE_ASSERT( FaceGetNumParam( wrd_ ) > wNo_ );
			}
			else if( wTable_ == TBX_MT ) {
				_lpPrm = ModuGetParameter(wrd_);
				CORE_ASSERT( ModuGetNumParam( wrd_ ) > wNo_ );
			}
			else {
				CORE_ASSERT( FALSE );
				return FALSE;
			}
			if( !_lpPrm )
				return FALSE;

			memcpy( _lpPrm+wNo_,
					&( (lpVPD_->lpVarPdu)[_dv.rem].pu ),
					sizeof(PU) );
			(_lpPrm+wNo_)->flags |= PU_DIRTY;
			return TRUE;
		}

		case PIC_SAVEDEFAULT:
		case PIC_LOADDEFAULT:
		{
			int		_i;
			char	_szBuffer[128];
			LPSTR	_lpszSection;
			LPSTR	_lpszFile;
			BOOL	_ret = TRUE;
			LPPDU	_pPduVar = lpVPD_->lpVarPdu;

			_lpszFile = (LPSTR)lParam_;
			_lpszSection = _lpszFile + strlen(_lpszFile) + 1;

			_ret = DefProcessPIC( wCmd_, wrd_, wNo_, lParam_,
						  wTable_,
						  lpVPD_->wNumFix,
						  id_,
						  lpVPD_->lpFixPdu );

			for( _i=0; _i<lpVPD_->wNumVar; _i++ )
			{
				if( _pPduVar[_i].flags & PDUF_NODEFAULT )
					continue;
				if( _pPduVar[_i].pu.prefix == (PF_IMM|IMM_LPCOBJ) )
					continue;
				if( _pPduVar[_i].pu.flags & PU_NIV )
					continue;

				if( wCmd_ == PIC_LOADDEFAULT )
				{
					DWORD	_retsvs;
					GetPrivateProfileString( _lpszSection,
											 _pPduVar[_i].szName,
											 "0",
											 _szBuffer,
											 sizeof(_szBuffer),
											 _lpszFile );

					_retsvs = PUSetValueString(&(_pPduVar[_i].pu),
										 _szBuffer,
										 _pPduVar[_i].vsof );

					if( _retsvs == GVSRESULT_USERDEF )
						_retsvs = _SetUserVsofValueString2( wTable_, id_, (WORD)(_i + lpVPD_->wNumFix), &(_pPduVar[_i].pu), _szBuffer );

					if( _retsvs != GVSRESULT_SUCCESS )
						_ret = FALSE;
					else
					{
						//clear DIRTY bit
						_pPduVar[_i].pu.flags &= (~PU_DIRTY);
					}
				}
				else	//PIC_SAVEDEFAULT
				{
					DWORD	_retGVS;
					GETVALUESTRING	_gvs;

					if( _pPduVar[_i].flags & PDUF_CANNTCOPY )
						continue;

					_gvs.lpszBuffer = _szBuffer;
					_gvs.wSize = sizeof(_szBuffer);

					_retGVS = PUGetValueString(&(_pPduVar[_i].pu),
										 &_gvs,
										 _pPduVar[_i].vsof );

					if( _retGVS == GVSRESULT_USERDEF)
					{
						_retGVS = _GetUserVsofValueString2( wTable_, id_, (WORD)(_i + lpVPD_->wNumFix), &(_pPduVar[_i].pu), &_gvs )? GVSRESULT_SUCCESS : GVSRESULT_FAILED;
					}

					if( (_retGVS != GVSRESULT_SUCCESS) && (_retGVS != GVSRESULT_COMPLEX_SUCCESS) ) 
					{
						_ret = FALSE;
						continue;
					}

					WritePrivateProfileString(_lpszSection,
											 _pPduVar[_i].szName,
											 _szBuffer,
											 _lpszFile );
				}
			}

			return _ret;
		}

		case PIC_PASTEDEFAULT:
		{
			WORD	_i, _order;
			LPPU	_lpPrm, _lpPrmVar;
			BOOL	_ret = TRUE;
			WORD	_wNumPrm;
			LPPDU	_pPduVar = lpVPD_->lpVarPdu;

			_ret = DefProcessPIC( wCmd_, wrd_, wNo_, lParam_,
						  wTable_,
						  lpVPD_->wNumFix,
						  id_,
						  lpVPD_->lpFixPdu );

			if( wTable_ == TBX_FT ) {
				_lpPrm = FaceGetParameter( (WRD)lParam_ );
				_wNumPrm = FaceGetNumParam( (WRD)lParam_ );
			}
			else if( wTable_ == TBX_MT ) {
				_lpPrm = ModuGetParameter( (WRD)lParam_ );
				_wNumPrm = FaceGetNumParam( (WRD)lParam_ );
			}
			else {
				CORE_ASSERT( FALSE );
				return TRUE;
			}

			if( !_lpPrm )
				return TRUE;
			
			_lpPrmVar = _lpPrm + lpVPD_->wNumFix; 

			for( _order=lpVPD_->wNumFix; _order<_wNumPrm; _order++ )
			{
				_i = (_order - lpVPD_->wNumFix) % lpVPD_->wNumVar;

				if( _pPduVar[_i].flags & PDUF_NODEFAULT )
					continue;
				if( _pPduVar[_i].flags & PDUF_CANNTCOPY )
					continue;
				if( _pPduVar[_i].pu.prefix == (PF_IMM|IMM_LPCOBJ) )
					continue;
				if( _pPduVar[_i].pu.flags & PU_NIV )
					continue;

				if( !PUWrite( _lpPrm + _order, &(_pPduVar[_i].pu) ) )
					_ret = FALSE;
			}

			return _ret;
		}

		case PIC_SETDEFAULT:
		{
			WORD	_i;
			LPPU	_lpPrm, _lpPrmVar;
			BOOL	_ret = TRUE;
			WORD	_wNumPrm;
			LPPDU	_pPduVar = lpVPD_->lpVarPdu;

			_ret = DefProcessPIC( wCmd_, wrd_, wNo_, lParam_,
						  wTable_,
						  lpVPD_->wNumFix,
						  id_,
						  lpVPD_->lpFixPdu );

			if( wTable_ == TBX_FT ) {
				_lpPrm = FaceGetParameter( (WRD)lParam_ );
				_wNumPrm = FaceGetNumParam( (WRD)lParam_ );
			}
			else if( wTable_ == TBX_MT ) {
				_lpPrm = ModuGetParameter( (WRD)lParam_ );
				_wNumPrm = FaceGetNumParam( (WRD)lParam_ );
			}
			else {
				CORE_ASSERT( FALSE );
				return TRUE;
			}

			if( !_lpPrm )
				return TRUE;
			
			_lpPrmVar = _lpPrm + lpVPD_->wNumFix; 

			for( _i=0; _i<lpVPD_->wNumVar; _i++ )
			{
				if( lpVPD_->wNumFix + _i >= _wNumPrm )
					break;

				if( _pPduVar[_i].flags & PDUF_NODEFAULT )
					continue;
				if( _pPduVar[_i].flags & PDUF_CANNTCOPY )
					continue;
				if( _pPduVar[_i].pu.prefix == (PF_IMM|IMM_LPCOBJ) )
					continue;
				if( _pPduVar[_i].pu.flags & PU_NIV )
					continue;

				if( !PUWrite( &(_pPduVar[_i].pu), _lpPrmVar + _i ) )
					_ret = FALSE;
				else
				{
					//clear DIRTY bit
					_pPduVar[_i].pu.flags &= (~PU_DIRTY);
				}
			}

			return _ret;
		}

		case PIC_PASTE:
		case PIC_CANCOPY:
			return TRUE;

		case PIC_CHECKPARAM:
			return _PIC_CheckParam_Var(wrd_, wTable_, lpVPD_);

		case PIC_GETVSOFENUMSTRUCT:
		{
			if( wNo_ < lpVPD_->wNumFix )
				return _GetSystemVsofEnumStruct( (lpVPD_->lpFixPdu)[wNo_].vsof, (LPVSOFENUMSTRUCT)lParam_ );
			else
			{
				VSOF	_vsof;
				div_t	_dv; 
				_dv = div( wNo_ - lpVPD_->wNumFix, lpVPD_->wNumVar );

				_vsof = (lpVPD_->lpVarPdu)[_dv.rem].vsof;
				return _GetSystemVsofEnumStruct( _vsof, (LPVSOFENUMSTRUCT)lParam_ );
			}
		}

		case PIC_BEFORESAVE:
		case PIC_AFTERLOAD:
			return TRUE;

		case PIC_GETICON:
		case PIC_GETDESCRIPTION_SHORT:
		case PIC_GETDESCRIPTION_LONG:
			return 0L;

		default:
			CORE_ASSERT(0);
			return FALSE;
	}
}

//...................................................................
//Description:
//Return:
//	GVSRESULT_FAILED	failed to get the value string;
//	GVSRESULT_SUCCESS	secceed;
//	GVSRESULT_COMPLEX	it's a complex data PU, return string is a description;
//	GVSRESULT_COMPLEX_SUCCESS
//						it's a complex data PU, return string is its value string
//	GVSRESULT_USERDEF	can't get valuestring because it's a user defined value string;
//...................................................................
_DLLEXP DWORD _DLLFUNC PUGetValueString(LPPU 				lpPU_,
										LPGETVALUESTRING	lpGVS_,
										VSOF				vsof_)
{
	SimplePU			_spu;
	WORD				_ret = GVSRESULT_SUCCESS;

	CORE_ASSERT(lpGVS_);

	//initialize the buffer
	*lpGVS_->lpszBuffer = 0;

	switch(vsof_)
	{
		case VSOF_NULL:
			_ret = GVSRESULT_FAILED;
			break;

		case VSOF_int_USER:
		case VSOF_WORD_USER:
		case VSOF_long_USER:
		case VSOF_DWORD_USER:
		case VSOF_REAL_USER:
		case VSOF_BOOL_USER:
			_ret = GVSRESULT_USERDEF;
			break;

		case VSOF_int:
			_spu.prefix = PF_IMM|IMM_int;
			if( PURead(&_spu, lpPU_) )
				sprintf(lpGVS_->lpszBuffer, gszFormat_int, _spu.du.zzk__int);
			else
				_ret = GVSRESULT_FAILED;
			break;

		case VSOF_int_RECTBS:
		case VSOF_int_GBS:
		case VSOF_int_LNSTYLE:
		case VSOF_int_ACTIVE:
		case VSOF_DWORD_TXTVERALIGN:
		case VSOF_DWORD_TXTHORALIGN:
		case VSOF_DWORD_DISPLAYMODE:
		{
			VSOFENUMSTRUCT	_es; 
			_GetSystemVsofEnumStruct( vsof_, &_es );

			_spu.prefix = PF_IMM|IMM_DWORD;
			if( PURead(&_spu, lpPU_) &&
				GetVsofName( &_es, _spu.du.zzk__DWORD, lpGVS_->lpszBuffer, lpGVS_->wSize ) )
			{
				break;
			}
			else
				_ret = GVSRESULT_FAILED;
			break;
		}

		case VSOF_WORD:
			_spu.prefix = PF_IMM|IMM_WORD;
			if( PURead(&_spu, lpPU_) )
				sprintf(lpGVS_->lpszBuffer, gszFormat_WORD, _spu.du.zzk__WORD);
			else
				_ret = GVSRESULT_FAILED;
			break;

		case VSOF_WORD_HEX:
			_spu.prefix = PF_IMM|IMM_WORD;
			if( PURead(&_spu, lpPU_) )
				sprintf(lpGVS_->lpszBuffer, gszFormat_WORD_HEX, _spu.du.zzk__WORD);
			else
				_ret = GVSRESULT_FAILED;
			break;

		case VSOF_WORD_BIN:
		//for RTU
		case VSOF_STATUS_RTU:
		case VSOF_DIRTY_RTU:
		case VSOF_ALARM_RTU:
		//for TTU
		case VSOF_STATUS_TTU:
		case VSOF_DIRTY_TTU:
		case VSOF_ALARM_TTU:
		//for BTU
		case VSOF_STATUS_BTU:
		case VSOF_DIRTY_BTU:
		case VSOF_ALARM_BTU:
		//for MTU
		case VSOF_STATUS_MTU:
		case VSOF_DIRTY_MTU:
		//for FTU
		case VSOF_STATUS_FTU:
		case VSOF_DIRTY_FTU:
		{
			_spu.prefix = PF_IMM|IMM_WORD;
			if( PURead(&_spu, lpPU_) )
				if( _w2binstr( _spu.du.zzk__WORD, lpGVS_->lpszBuffer, lpGVS_->wSize ) )
					break;

				_ret = GVSRESULT_FAILED;
			break;
		}

		case VSOF_long:
			_spu.prefix = PF_IMM|IMM_long;
			if( PURead(&_spu, lpPU_) )
				sprintf(lpGVS_->lpszBuffer, gszFormat_long, _spu.du.zzk__long);
			else
				_ret = GVSRESULT_FAILED;
			break;

		case VSOF_CORETIME:	//at second
		{
			struct	tm	_tm;

			_spu.prefix = PF_IMM|IMM_CORETIME;
			if( !PURead(&_spu, lpPU_) )
			{
				_ret = GVSRESULT_FAILED;
				break;
			}

			CoreTime2tm( _spu.du.zzk__CORETIME, &_tm );

			_tm.tm_year += 1900;
			_tm.tm_mon += 1;
			sprintf(lpGVS_->lpszBuffer,
					" %04d-%02d-%02d %02d:%02d:%02d ",
					_tm.tm_year, _tm.tm_mon, _tm.tm_mday, _tm.tm_hour, _tm.tm_min, _tm.tm_sec );
			break;
		}

		case VSOF_DWORD:
			_spu.prefix = PF_IMM|IMM_DWORD;
			if( PURead(&_spu, lpPU_) )
				sprintf(lpGVS_->lpszBuffer, gszFormat_DWORD, _spu.du.zzk__DWORD);
			else
				_ret = GVSRESULT_FAILED;
			break;

		case VSOF_DWORD_HEX:
			_spu.prefix = PF_IMM|IMM_DWORD;
			if( PURead(&_spu, lpPU_) )
				sprintf(lpGVS_->lpszBuffer, gszFormat_DWORD_HEX, _spu.du.zzk__DWORD);
			else
				_ret = GVSRESULT_FAILED;
			break;

		case VSOF_DWORD_BIN:
		{
			_spu.prefix = PF_IMM|IMM_DWORD;
			if( PURead(&_spu, lpPU_) )
				if( _dw2binstr( _spu.du.zzk__DWORD, lpGVS_->lpszBuffer, lpGVS_->wSize ) )
					break;

				_ret = GVSRESULT_FAILED;
			break;
		}

		case VSOF_DWORD_CLR:
			_spu.prefix = PF_IMM|IMM_DWORD;
			if( PURead(&_spu, lpPU_) )
			{
				if( !PrintCclr( lpGVS_->lpszBuffer, _spu.du.zzk__DWORD ) )
					_ret = GVSRESULT_FAILED;
			}
			else
				_ret = GVSRESULT_FAILED;
			break;

		case VSOF_DWORD_MS:		//at MS
			_spu.prefix = PF_IMM|IMM_DWORD;
			if( PURead(&_spu, lpPU_) )
			{
				ldiv_t		_dv;
				unsigned	_hour, _min, _sec, _ms;
				_dv = ldiv( _spu.du.zzk__DWORD, 1000L );
				_ms = (int)_dv.rem;
				_dv = ldiv( _dv.quot, 60L );
				_sec = (int)_dv.rem;
				_dv = ldiv( _dv.quot, 60L );
				_min = (int)_dv.rem;
				_hour = (int)_dv.quot;

				if( _ms )
					sprintf(lpGVS_->lpszBuffer,
							gszFormat_DWORD_MS,
							_hour, _min, _sec, _ms);
				else	// no ms
					sprintf(lpGVS_->lpszBuffer,
							gszFormat_DWORD_MS_NOMS,
							_hour, _min, _sec );
			}
			else
				_ret = GVSRESULT_FAILED;
			break;

		case VSOF_DWORD_BWIDTH:
			_spu.prefix = PF_IMM|IMM_DWORD;
			if( PURead(&_spu, lpPU_) )
			{
				if( HIWORD(_spu.du.zzk__DWORD) )
					sprintf(lpGVS_->lpszBuffer,
							gszFormat_DWORD_BWIDTH,
							(unsigned)LOWORD(_spu.du.zzk__DWORD),
							(unsigned)HIWORD(_spu.du.zzk__DWORD) );
				else
					sprintf(lpGVS_->lpszBuffer,
							gszFormat_DWORD,
							_spu.du.zzk__DWORD);
			}
			else
				_ret = GVSRESULT_FAILED;
			break;

		case VSOF_IDALGO:
		{
			_spu.prefix = PF_IMM|IMM_DWORD;
			if( PURead(&_spu, lpPU_) )
			{
				char	_szName[LENGTH_NAME_ALGO + 2];

				AlgoGetName( _spu.du.zzk__DWORD, _szName, sizeof(_szName) );
				sprintf(lpGVS_->lpszBuffer,
						"%s(0x%lX)", _szName, _spu.du.zzk__DWORD);

			}
			else
				_ret = GVSRESULT_FAILED;
			break;
		}

		case VSOF_IDSTAMP:
		{
			_spu.prefix = PF_IMM|IMM_DWORD;
			if( PURead(&_spu, lpPU_) )
			{
				char	_szName[LENGTH_NAME_STAMP + 2];

				StampGetName( _spu.du.zzk__DWORD, _szName, sizeof(_szName) );
				sprintf(lpGVS_->lpszBuffer,
						"%s(0x%lX)", _szName, _spu.du.zzk__DWORD);

			}
			else
				_ret = GVSRESULT_FAILED;
			break;
		}

		case VSOF_REAL:
			_spu.prefix = PF_IMM|IMM_REAL;
			if( PURead(&_spu, lpPU_) )
				sprintf(lpGVS_->lpszBuffer, gszFormat_REAL, _spu.du.zzk__REAL);
			else
				_ret = GVSRESULT_FAILED;
			break;

		case VSOF_BOOL:	//VSOF_BOOL_TRUEFALSE
		case VSOF_BOOL_YESNO:
		case VSOF_BOOL_ONOFF:
		case VSOF_BOOL_OPENCLOSE:
		case VSOF_BOOL_10:
			_spu.prefix = PF_IMM|IMM_BOOL;
			if( PURead(&_spu, lpPU_) )
				PrintBool( lpGVS_->lpszBuffer, _spu.du.zzk__BOOL, vsof_ );
			else
				_ret = GVSRESULT_FAILED;
			break;

		case VSOF_FPTR:
			_spu.prefix = PF_IMM|IMM_FPTR;
			if( PURead(&_spu, lpPU_) )
				sprintf(lpGVS_->lpszBuffer, gszFormat_FPTR, _spu.du.zzk__FPTR);
			else
				_ret = GVSRESULT_FAILED;
			break;

		case VSOF_FPTR_RECT:
		{
			LPRECT	_lpRect;

			_spu.prefix = PF_IMM|IMM_FPTR;
			if( PURead(&_spu, lpPU_) )
			{
				_lpRect = (LPRECT)_spu.du.zzk__FPTR;
				sprintf(lpGVS_->lpszBuffer,
						gszFormat_FPTR_RECT,
						_lpRect->left,
						_lpRect->top,
						_lpRect->right,
						_lpRect->bottom);
			}
			else
				_ret = GVSRESULT_FAILED;

			break;
		}

		case VSOF_NPTR:
			_spu.prefix = PF_IMM|IMM_NPTR;
			if( PURead(&_spu, lpPU_) )
				sprintf(lpGVS_->lpszBuffer, gszFormat_NPTR, _spu.du.zzk__NPTR);
			else
				_ret = GVSRESULT_FAILED;
			break;

		case VSOF_WRDXTU:
		case VSOF_WRDRTU:
		case VSOF_WRDTTU:
		case VSOF_WRDBTU:
		case VSOF_WRDMTU:
		case VSOF_WRDMTU_TRIGGER:
		case VSOF_WRDFTU:
		{
			WRD		_wrd;
			char	_szTag[32];

			_spu.prefix = PF_IMM| VSOFGETPDT(vsof_);

			if( !PURead(&_spu, lpPU_) )
			{
				_ret = GVSRESULT_FAILED;
				break;
			}

			//_wrd = GETWRD(_spu.du.zzk__DWORD);
			//GETTBL(_spu.du.zzk__DWORD)
			_wrd = _spu.du.zzk__WRD;
			if( !_wrd )
			{
				WRDToTag( _wrd, _szTag, sizeof(_szTag) );
				_ret = 1;
			}
			else
				switch( _spu.prefix & 0x00FF )
				{
					case WRD_RTU:
						_ret = RealGetTag( _wrd, _szTag, sizeof(_szTag) )? 1: 0;
						break;

					case WRD_TTU:
						_ret = TintGetTag( _wrd, _szTag, sizeof(_szTag) )? 1: 0;
						break;

					case WRD_BTU:
						_ret = BoolGetTag( _wrd, _szTag, sizeof(_szTag) )? 1: 0;
						break;

					case WRD_MTU:
						_ret = ModuGetTag( _wrd, _szTag, sizeof(_szTag) )? 1: 0;
						break;

					case WRD_FTU:
					case 0:
						WRDToTag( _wrd, _szTag, sizeof(_szTag) );	//pseudo name
						break;

					default:
						CORE_ASSERT(0);
						_ret = GVSRESULT_FAILED;
				}

			if( _ret != GVSRESULT_FAILED )	
				strcpy( lpGVS_->lpszBuffer, _szTag );

			break;
		}

		case VSOF_LPCOBJ:
		case VSOF_LPCOBJ_STR:
			_spu.prefix = PF_IMM|IMM_LPCOBJ;
			_spu.du.zzk__LPCOBJ = NULL;
			if( PURead(&_spu, lpPU_) )
			{
				LPCOBJ	_lp = _spu.du.zzk__LPCOBJ;
				
				_ret = GVSRESULT_COMPLEX;
				if( !_lp )
				{
					StringCopyn(lpGVS_->lpszBuffer, "", lpGVS_->wSize);
					break;
				}
				else if( CobjGetID(_lp) == IDCOBJ_STRING )
				{
					StringCopyn(lpGVS_->lpszBuffer, (LPSTR)_lp, lpGVS_->wSize);
					//it's a COBJ but also get value string successfully
					_ret = GVSRESULT_COMPLEX_SUCCESS;
				}
				else
				{
					if( !CobjGetIDName( CobjGetID(_lp), lpGVS_->lpszBuffer, lpGVS_->wSize ) )
						_ret = GVSRESULT_FAILED;
				}
			}
			else
				_ret = GVSRESULT_FAILED;
			break;

		case VSOF_USER:
				_ret = GVSRESULT_FAILED;
			break;

		default:
			CORE_ASSERT(0);
				_ret = GVSRESULT_FAILED;
			break;
	}

	return _ret;
}

//...................................................................
//Description:
//...
//...................................................................
_DLLEXP DWORD _DLLFUNC PUSetValueString(LPPU 		lpPU_,
										LPCSTR		lpsz_,
										VSOF		vsof_)
{
	SimplePU			_spu;

	CORE_ASSERT(lpsz_);

	CLEAR_LAST_ERROR;

	switch(vsof_)
	{
		case VSOF_NULL:
			return GVSRESULT_FAILED;

		case VSOF_int_USER:
		case VSOF_WORD_USER:
		case VSOF_long_USER:
		case VSOF_DWORD_USER:
		case VSOF_REAL_USER:
		case VSOF_BOOL_USER:	
			return GVSRESULT_USERDEF;

		case VSOF_int:
			_spu.prefix = PF_IMM|IMM_int;
			if( 1 != sscanf(lpsz_, gszFormat_int, &(_spu.du.zzk__int) ) )
				if( !ScanNumber( lpsz_, &_spu ) )
					return GVSRESULT_FAILED;
			break;

		case VSOF_int_LNSTYLE:	//
		case VSOF_int_RECTBS:
		case VSOF_int_GBS:
		case VSOF_int_ACTIVE:
		case VSOF_DWORD_TXTVERALIGN:
		case VSOF_DWORD_TXTHORALIGN:
		case VSOF_DWORD_DISPLAYMODE:
		{
			VSOFENUMSTRUCT	_es; 
			int				_i;

			_GetSystemVsofEnumStruct( vsof_, &_es );

			_i = MatchVsofName( &_es, lpsz_ );
			if( _i < 0 )	//not matched
				return GVSRESULT_FAILED;

			_spu.prefix = PF_IMM|IMM_DWORD;
			_spu.du.zzk__DWORD = (_es.lpVsofName)[_i].Value;
			break;
		}

		case VSOF_WORD:
			_spu.prefix = PF_IMM|IMM_WORD;
			if( 1 != sscanf(lpsz_, gszFormat_WORD, &(_spu.du.zzk__WORD) ) )
				if( !ScanNumber( lpsz_, &_spu ) )
				return GVSRESULT_FAILED;
			break;

		case VSOF_WORD_HEX:
			_spu.prefix = PF_IMM|IMM_WORD;
			if( 1 != sscanf(lpsz_, gszFormat_WORD_HEX, &(_spu.du.zzk__WORD) ) )
				if( !ScanNumber( lpsz_, &_spu ) )
					return GVSRESULT_FAILED;
			break;

		case VSOF_WORD_BIN:
		//for RTU
		case VSOF_STATUS_RTU:
		case VSOF_DIRTY_RTU:
		case VSOF_ALARM_RTU:
		//for TTU
		case VSOF_STATUS_TTU:
		case VSOF_DIRTY_TTU:
		case VSOF_ALARM_TTU:
		//for BTU
		case VSOF_STATUS_BTU:
		case VSOF_DIRTY_BTU:
		case VSOF_ALARM_BTU:
		//for MTU
		case VSOF_STATUS_MTU:
		case VSOF_DIRTY_MTU:
		//for FTU
		case VSOF_STATUS_FTU:
		case VSOF_DIRTY_FTU:
		{
			DWORD	_data;
			_spu.prefix = PF_IMM|IMM_WORD;
			if( _binstr2dw( lpsz_, &_data ) )
				_spu.du.zzk__WORD = (WORD)_data;
			else
				if( !ScanNumber( lpsz_, &_spu ) )
					return GVSRESULT_FAILED;
			break;
		}

		case VSOF_long:
			_spu.prefix = PF_IMM|IMM_long;
			if( 1 != sscanf(lpsz_, gszFormat_long, &(_spu.du.zzk__long) ) )
				if( !ScanNumber( lpsz_, &_spu ) )
					return GVSRESULT_FAILED;
			break;

		case VSOF_CORETIME:
		{
			int			_year, _month, _day, _hour, _minute, _second;
			CORETIME	_ct;
			struct	tm	_tm;

			if( sscanf( lpsz_,
						" %d-%d-%d %d:%d:%d ",
						&_year, &_month, &_day, &_hour, &_minute, &_second )
				!= 6 )
			{
				return GVSRESULT_FAILED;
			}

			if( _year < 100 )
				_tm.tm_year = _year;
			else
				_tm.tm_year = _year - 1900;
			_tm.tm_mon = _month - 1;
			_tm.tm_mday = _day;
			_tm.tm_hour = _hour;
			_tm.tm_min = _minute;
			_tm.tm_sec = _second;
			_tm.tm_isdst = 0;	//DST nerver
			_ct = tm2CoreTime( &_tm );

			_spu.prefix = PF_IMM|IMM_CORETIME;
			_spu.du.zzk__CORETIME = _ct;
			break;
		}

		case VSOF_DWORD:
			_spu.prefix = PF_IMM|IMM_DWORD;
			if( 1 != sscanf(lpsz_, gszFormat_DWORD, &(_spu.du.zzk__DWORD) ) )
				if( !ScanNumber( lpsz_, &_spu ) )
					return GVSRESULT_FAILED;
			break;

		case VSOF_IDALGO:
			_spu.prefix = PF_IMM|IMM_DWORD;
			if( 1 != sscanf(lpsz_, gszFormat_DWORD_HEX, &(_spu.du.zzk__DWORD) ) )
				if( !ScanNumber( lpsz_, &_spu ) || !IsAlgo( _spu.du.zzk__DWORD ) )
					return GVSRESULT_FAILED;
			break;

		case VSOF_IDSTAMP:
			_spu.prefix = PF_IMM|IMM_DWORD;
			if( 1 != sscanf(lpsz_, gszFormat_DWORD_HEX, &(_spu.du.zzk__DWORD) ) )
				if( !ScanNumber( lpsz_, &_spu ) || !IsStamp( _spu.du.zzk__DWORD ) )
					return GVSRESULT_FAILED;
			break;

		case VSOF_DWORD_HEX:
			_spu.prefix = PF_IMM|IMM_DWORD;
			if( 1 != sscanf(lpsz_, gszFormat_DWORD_HEX, &(_spu.du.zzk__DWORD) ) )
				if( !ScanNumber( lpsz_, &_spu ) )
					return GVSRESULT_FAILED;
			break;

		case VSOF_DWORD_BIN:
		{
			DWORD	_data;
			_spu.prefix = PF_IMM|IMM_DWORD;
			if( _binstr2dw( lpsz_, &_data ) )
				_spu.du.zzk__DWORD = _data;
			else
				if( !ScanNumber( lpsz_, &_spu ) )
					return GVSRESULT_FAILED;
			break;
		}

		case VSOF_DWORD_CLR:
		{
			_spu.prefix = PF_IMM|IMM_DWORD;
			if( !ScanCclr( lpsz_, &_spu.du.zzk__DWORD ) )
				if( !ScanNumber( lpsz_, &_spu ) )
					return GVSRESULT_FAILED;
			break;
		}

		case VSOF_DWORD_MS:
		{
			unsigned 	_hour, _min, _sec, _ms;
			DWORD		_time;

			_hour = _min = _sec = _ms = 0;
			if(4 != sscanf( lpsz_, gszFormat_DWORD_MS,
							&_hour, &_min, &_sec, &_ms ) )
				if(3 != sscanf( lpsz_, gszFormat_DWORD_MS_NOMS,
								&_hour, &_min, &_sec ) )
					if( !ScanNumber( lpsz_, &_spu ) )
						return GVSRESULT_FAILED;

			_time = ((_hour*60L + _min)*60L + _sec)*1000L + _ms;
			_spu.prefix = PF_IMM|IMM_DWORD;
			_spu.du.zzk__DWORD = _time;
			break;
		}

		case VSOF_DWORD_BWIDTH:
		{
			WORD 	_low, _hi;
			DWORD	_dw;

			_spu.prefix = PF_IMM|IMM_DWORD;

			if( 2 == sscanf( lpsz_, gszFormat_DWORD_BWIDTH, &_low, &_hi ) )
				_spu.du.zzk__DWORD = MAKELONG( _low, _hi );
			else if( 1 == sscanf( lpsz_, gszFormat_DWORD, &_dw ) )
				_spu.du.zzk__DWORD = LOWORD(_dw);
			else if( !ScanNumber( lpsz_, &_spu ) )
				return GVSRESULT_FAILED;
			break;
		}

		case VSOF_REAL:
			_spu.prefix = PF_IMM|IMM_REAL;
			if( 1 != sscanf(lpsz_, gszFormat_REAL, &(_spu.du.zzk__REAL) ) )
				if( !ScanNumber( lpsz_, &_spu ) )
					return GVSRESULT_FAILED;
			break;

		case VSOF_BOOL:	//i.e. case VSOF_BOOL_TRUEFALSE:
		case VSOF_BOOL_YESNO:
		case VSOF_BOOL_ONOFF:
		case VSOF_BOOL_OPENCLOSE:
		case VSOF_BOOL_10:
		{
			_spu.prefix = PF_IMM|IMM_BOOL;
			if( !ScanBool( lpsz_, &_spu.du.zzk__BOOL ) )
				if( !ScanNumber( lpsz_, &_spu ) )
					return GVSRESULT_FAILED;	//no found
				else
					_spu.du.zzk__BOOL = _spu.du.zzk__BOOL ? TRUE : FALSE; 
			break;
		}

		case VSOF_FPTR:
			_spu.prefix = PF_IMM|IMM_FPTR;
			if( 1 != sscanf(lpsz_, gszFormat_FPTR, &(_spu.du.zzk__FPTR) ) )
				return GVSRESULT_FAILED;
			break;

		case VSOF_FPTR_RECT:
		{
			RECT	_rc;

			_spu.prefix = PF_IMM|IMM_FPTR;
			_spu.du.zzk__FPTR = &_rc;	//Note: _rc is auto-var.
			if( 4 != sscanf( lpsz_,
					gszFormat_FPTR_RECT,
					&(_rc.left),
					&(_rc.top),
					&(_rc.right),
					&(_rc.bottom) ) )
				return GVSRESULT_FAILED;

			//Note:
			//don't use "break" for jumping to common exit
			//because varible _rc is an auto-var.
			return PUWrite(lpPU_, &_spu);
		}

		case VSOF_NPTR:
			_spu.prefix = PF_IMM|IMM_NPTR;
			if( 1 != sscanf(lpsz_, gszFormat_NPTR, &(_spu.du.zzk__NPTR) ) )
				return GVSRESULT_FAILED;
			break;

		case VSOF_WRDXTU:
		case VSOF_WRDRTU:
		case VSOF_WRDTTU:
		case VSOF_WRDBTU:
		case VSOF_WRDMTU:
		case VSOF_WRDMTU_TRIGGER:
		case VSOF_WRDFTU:
		{
			WORD	_wrd;

			_spu.prefix = PF_IMM|VSOFGETPDT(vsof_);
			if( !PURead( &_spu, lpPU_ ) )
				return GVSRESULT_FAILED;

			switch( _spu.prefix & 0x00FF )
			{
				case WRD_RTU:
					_wrd = RealGetWRD( lpsz_ );
					break;

				case WRD_TTU:
					_wrd = TintGetWRD( lpsz_ );
					break;

				case WRD_BTU:
					_wrd = BoolGetWRD( lpsz_ );
					break;

				case WRD_MTU:
					_wrd = ModuGetWRD( lpsz_ );
					break;

				case WRD_FTU:
					_wrd = TagToWRD( lpsz_ );
					break;

				default:
					CORE_ASSERT( FALSE );
					_wrd = 0;
					break;
			}

			if( !_wrd )
			{
				BOOL	_bOkey;
				TagToWRDEx( lpsz_, &_bOkey );
				if( !_bOkey )
					return GVSRESULT_FAILED;
			}

			_spu.prefix = PF_IMM|VSOFGETPDT(vsof_);
			_spu.du.zzk__WRD = _wrd;
			break;
		}

		case VSOF_LPCOBJ:
			return GVSRESULT_FAILED;

		case VSOF_LPCOBJ_STR:
		{
			BOOL	_bOk;

			_spu.prefix = PF_IMM|IMM_LPCOBJ;
			_spu.du.zzk__LPCOBJ = AllocCString(lpsz_, &_bOk);
			if( !_bOk )
				return GVSRESULT_FAILED;

			if( !PUWrite(lpPU_, &_spu ) )
			{
				CobjFree( _spu.du.zzk__LPCOBJ );	//write failed
				return GVSRESULT_FAILED;	
			}
			else
				return GVSRESULT_SUCCESS;
		}

		case VSOF_USER:
			return GVSRESULT_FAILED;

		default:
			CORE_ASSERT(0);
			SET_LAST_ERROR( ERR_Invalid_Entries );
			return GVSRESULT_FAILED;
	}

	return PUWrite(lpPU_, &_spu)?GVSRESULT_SUCCESS : GVSRESULT_FAILED;
}

//...................................................................
//Description:
//	This function combines the parts of name in Decode struct
//	specified by lpDecode_ to a ASCIIZ string at three cases:
//		1.	ONLY has szHead:
//				String
//		2.	has szHead and szBody
//				Im.real Im.long		...
//		3.	has szHead, szBody and szTail
//				RT.#234.Result		...
//
//Note:
//	This function does not check the buffer length.
//...................................................................
_DLLEXP void _DLLFUNC CombineDecode(LPSTR lpszBuffer_, LPDECODE lpDecode_)
{
	if(*lpDecode_->szHead == 0)
		*lpszBuffer_ = 0;
	else
	{
		strcpy(lpszBuffer_, lpDecode_->szHead);

		if(*lpDecode_->szBody == 0)
			return;
		strcat(lpszBuffer_, sszDOT);
	}
	strcat(lpszBuffer_, lpDecode_->szBody);

	if(*lpDecode_->szTail == 0)
		return;

	strcat(lpszBuffer_, sszDOT);
	strcat(lpszBuffer_, lpDecode_->szTail);

	return;
}


static near _PIC_Create(WRD wrd_, WORD wTable_)
{
	WORD	_i;

	if(wTable_ == TBX_FT)
	{
		WORD		_wNumPrm;
		IDSTAMP		_idStamp;

		_wNumPrm = FaceGetNumParam( wrd_ );
		_idStamp = FaceGetStamp( wrd_ );
		for(_i=0; _i<_wNumPrm; _i++)
			StampCallParamProc( _idStamp, PIC_INITIALIZEPARAM, wrd_, _i, 0 );
	}
	else if(wTable_ == TBX_MT)
	{
		WORD		_wNumPrm;
		IDALGO		_idAlgo;

		_wNumPrm = ModuGetNumParam( wrd_ );
		_idAlgo = ModuGetAlgo( wrd_ );
		for(_i=0; _i<_wNumPrm; _i++)
			AlgoCallParamProc( _idAlgo, PIC_INITIALIZEPARAM, wrd_, _i, 0 );
	}

	return TRUE;
}

#pragma argsused
static DWORD _near _PIC_CheckParam( WRD wrd_, WORD wTable_, WORD wNumPDU_, LPPDU lpPDU_ )
{
	WORD	_i;
	WORD	_wNumPrm;
	LPPU	_lpPrm;

	if(wTable_ == TBX_FT) 
	{
		_wNumPrm = FaceGetNumParam( wrd_ );
		_lpPrm = FaceGetParameter( wrd_ );
	}
	else if(wTable_ == TBX_MT) {
		_wNumPrm = ModuGetNumParam( wrd_ );
		_lpPrm = ModuGetParameter( wrd_ );
	}
	else {
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	if( _wNumPrm != wNumPDU_ )
	{
		LOGERR2( "illegal number of parameter. %cTU: WRD=%d",
				(wTable_ == TBX_MT)?'M':'F', wrd_ );
		return FALSE;
	}

	for(_i=0; _i<_wNumPrm; _i++)
	{
		if( !PUCheckLegality( _lpPrm + _i ) )
		{
			LOGERR3( "illegal data-link parameter. %cTU: WRD=%d,Param#%d",
					(wTable_ == TBX_MT)?'M':'F', wrd_, _i );
			return FALSE;
		}
	}

	return TRUE;
}

static DWORD _near _PIC_CheckParam_Var(WRD			wrd_,
									   WORD			wTable_,
									   LPVPSTRUCT	lpVPD_)
{
	WORD	_i;
	WORD	_wNumPrm;
	LPPU	_lpPrm;
	div_t	_dv;

	if(wTable_ == TBX_FT) {
		_wNumPrm = FaceGetNumParam( wrd_ );
		_lpPrm = FaceGetParameter( wrd_ );
	}
	else if(wTable_ == TBX_MT) {
		_wNumPrm = ModuGetNumParam( wrd_ );
		_lpPrm = ModuGetParameter( wrd_ );
	}
	else {
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	if( _wNumPrm < (lpVPD_->wNumFix + lpVPD_->wMinVarGroup * lpVPD_->wNumVar) )
	{	//if the number of paramter less than the min number of parameter
		LOGERR2( "Too few parameters. %cTU: WRD=%d",
				(wTable_ == TBX_MT)?'M':'F', wrd_ );
		return FALSE;
	}

	_dv = div( _wNumPrm - lpVPD_->wNumFix, lpVPD_->wNumVar );
	if( _dv.rem != 0 )
	{
		LOGERR2( "illegal number of parameter. %cTU: WRD=%d",
				(wTable_ == TBX_MT)?'M':'F', wrd_ );
		return FALSE;
	}

	for(_i=0; _i<_wNumPrm; _i++)
	{
		if( !PUCheckLegality( _lpPrm + _i ) )
		{
			LOGERR3( "illegal data-link parameter. %cTU: WRD=%d,Param#%d",
					(wTable_ == TBX_MT)?'M':'F', wrd_, _i );
			return FALSE;
		}
	}

	return TRUE;
}

//...................................................................
//Description:
//	Checks the legality of the PU's data-link
//...................................................................
_DLLEXP BOOL _DLLFUNC PUCheckLegality( LPPU lppu_ )
{
	static char _sszBuffer[32];

	switch( lppu_->prefix & PF_MASK )
	{
		case PF_IMM:
			if( !GetPDTName((PDT)(lppu_->prefix & IMM_MASK),
							_sszBuffer,
							sizeof(_sszBuffer),
							0 ) )
			{
				//at this version, it must be a PDT type, NO own-
				//defined PDT type permitted!
				//at the future version, perhaps own-defined PDT type
				//permitted, but now, NO! because the PURead/PUWrite
				//does not allow the own-defined PDT type;
				return FALSE;
			}
			break;

		case PF_TBX:
		{
			switch((lppu_->prefix) & TBX_MASK)
			{
				case TBX_RT:
					if( !RealIsValid( lppu_->du.zzk__WRD ) )
						return FALSE;
					if( !RealGetElementType( (WORD)(lppu_->prefix & XT_MASK) ) )
						return FALSE;
					break;

				case TBX_TT:
					if( !TintIsValid( lppu_->du.zzk__WRD ) )
						return FALSE;
					if( !TintGetElementType( (WORD)(lppu_->prefix & XT_MASK) ) )
						return FALSE;
					break;

				case TBX_BT:
					if( !BoolIsValid( lppu_->du.zzk__WRD ) )
						return FALSE;
					if( !BoolGetElementType( (WORD)(lppu_->prefix & XT_MASK) ) )
						return FALSE;
					break;

				case TBX_MT:
					if( !ModuIsValid( lppu_->du.zzk__WRD ) )
						return FALSE;
					if( !ModuGetElementType( (WORD)(lppu_->prefix & XT_MASK) ) )
						return FALSE;
					break;

				case TBX_FT:
					if( !FaceIsValid( lppu_->du.zzk__WRD ) )
						return FALSE;
					if( !FaceGetElementType( (WORD)(lppu_->prefix & XT_MASK) ) )
						return FALSE;
					break;

				default:
					CORE_ASSERT(0);
					return FALSE;
			}
			break;
		}

		case PF_MPX:
			if(    ModuGetNumParam( lppu_->du.zzk__WRD )
				<= ( (lppu_->prefix) & MPX_INDEX_MASK ) )
				return FALSE;
			break;

		case PF_FPX:
			if(    FaceGetNumParam( lppu_->du.zzk__WRD )
				<= ( (lppu_->prefix) & MPX_INDEX_MASK ) )
				return FALSE;
			break;

		default:
			return FALSE;
	}

	return TRUE;
}

BOOL _w2binstr( WORD data_, LPSTR lpsz_, WORD size_ )
{
	int	_i;

	//0b0000-0000-0000-0000
	if( size_ < 22 )
		return FALSE;

	strcpy( lpsz_, "0b" );

	for( _i=0; _i< sizeof(WORD)*8; _i++ )
	{
		if( data_ & 0x8000 )
			strcat( lpsz_, "1" );
		else
			strcat( lpsz_, "0" );

		if( (_i%4 == 3) && (_i != 15) )
			strcat( lpsz_, "-" );

		data_ = data_ << 1;
	}

	return TRUE;
}

BOOL _dw2binstr( DWORD data_, LPSTR lpsz_, WORD size_ )
{
	int	_i;

	//0b0000-0000-0000-0000
	if( size_ < 22 )
		return FALSE;

	strcpy( lpsz_, "0b" );

	for( _i=0; _i< sizeof(DWORD)*8; _i++ )
	{
		if( data_ & 0x8000 )
			strcat( lpsz_, "1" );
		else
			strcat( lpsz_, "0" );

		if( (_i%4 == 3) && (_i != 31) )
			strcat( lpsz_, "-" );

		data_ = data_ << 1;
	}

	return TRUE;
}

BOOL _binstr2dw( LPCSTR lpsz_, DWORD FAR* lpdata_ )
{
	char	_szBuffer[128];
	LPSTR	_p1;
	DWORD	_data = 0;
	int		_ret;

	//omit pre-spaces and post-spaces
	_ret = sscanf( (char*)lpsz_, "%s", _szBuffer );
	if( (_ret == 0) || (_ret == EOF) )
		return FALSE;

	//analyze the  string
	_p1 = _szBuffer;
	if(*_p1 == 0)	//NULL string
	{
		//CORE_ASSERT( FALSE );	//normally, can't come here
		return FALSE;
	}

	if( *_p1 != '0' )
		return FALSE;

	_p1++;
	if( (*_p1 != 'B') && (*_p1 != 'b') )
		return FALSE;

	for(_p1++; *_p1; _p1++ )
	{
		_data = _data << 1;

		if( *_p1 == '1' )
			_data |= 1;
		else if( *_p1 == '0' )
			continue;
		else if( *_p1 == '-' )
			_data = _data >> 1;
		else	//meet illegal char
			return FALSE;
	}

	*lpdata_ = _data;
	return TRUE;
}

/*
_DLLEXP BOOL _DLLFUNC GetDigitStringValue( LPCSTR lpsz_, LPPU lpPU_ )
{
	return FALSE;	//no implemented!
}

_DLLEXP BOOL _DLLFUNC GetBoolStringValue(LPCSTR lpsz_, LPBOOL lpError_ )
{
	int		_i;
	BOOL	_ret, _err = FALSE;
	PU		_pu;

	for( _i=0; _i<_NUM_BOOL_STRINGS; _i ++ )
	{
		if( stricmp( lpsz_, sBoolName[_i].szTrue ) == 0	)
		{
			_ret = TRUE;
			goto _END;
		}

		if( stricmp( lpsz_, sBoolName[_i].szFalse ) == 0	)
		{
			_ret = FALSE;
			goto _END;
		}
	}

	//no found

	//is it an integer string ?
	if( GetDigitStringValue( lpsz_, &_pu ) )
	{
		PU	_pu2;

		_pu2.prefix = PF_IMM|IMM_DWORD;
		if( PURead( &_pu2, &_pu ) )
		{
			_ret = _pu2.du.zzk__DWORD? TRUE: FALSE;
			_err = FALSE;
			goto _END;
		}
	}

	_err = TRUE;
	_ret = FALSE;

_END:
	if( lpError_ )
		*lpError_ = _err;

	return _ret;
}
*/

_DLLEXP BOOL _DLLFUNC ScanNumber( LPCSTR lpsz_, LPPU lpPU_ )
{
	int	_ret;
	if( strchr( lpsz_, '.' ) )
	{
		_ret = sscanf( lpsz_, "%g", &(lpPU_->du.zzk__REAL)	);
		if( _ret == 1 )
		{
			lpPU_->prefix = PF_IMM|IMM_REAL;
			return TRUE;
		}
	}

	if( strchr( lpsz_, 'X' ) || strchr( lpsz_, 'x' ) )
	{
		_ret = sscanf( lpsz_, "%lX", &(lpPU_->du.zzk__long) );
		if( _ret == 1 )
		{
			lpPU_->prefix = PF_IMM|IMM_long;
			return TRUE;
		}
	}

	_ret = sscanf( lpsz_, "%li", &(lpPU_->du.zzk__long) );
	if( _ret == 1 )
	{
		lpPU_->prefix = PF_IMM|IMM_long;
		return TRUE;
	}
	else
		return FALSE;
}

_DLLEXP BOOL _DLLFUNC PrintBool( LPSTR lpBuffer_, BOOL val_, VSOF vsof_ )
{
	switch( vsof_ )
	{
		case VSOF_BOOL_YESNO:
			sprintf(lpBuffer_, 
					gszFormat_BOOL, 
					val_? sBoolName[1].szTrue : sBoolName[1].szFalse );
			break;

		case VSOF_BOOL_ONOFF:
			sprintf(lpBuffer_, 
					gszFormat_BOOL, 
					val_? sBoolName[2].szTrue : sBoolName[2].szFalse );
			break;

		case VSOF_BOOL_OPENCLOSE:
			sprintf(lpBuffer_, 
					gszFormat_BOOL, 
					val_? sBoolName[3].szTrue : sBoolName[3].szFalse );
			break;

		case VSOF_BOOL_10:
			sprintf(lpBuffer_, 
					gszFormat_BOOL, 
					val_? sBoolName[4].szTrue : sBoolName[4].szFalse );
			break;

		case VSOF_BOOL_TRUEFALSE:
		default:
			sprintf(lpBuffer_, 
					gszFormat_BOOL, 
					val_? sBoolName[0].szTrue : sBoolName[0].szFalse );
			break;
	}

	return TRUE;
}

_DLLEXP BOOL _DLLFUNC ScanBool( LPCSTR lpsz_, BOOL* pbool_ )
{
	int _i;
	for( _i=0; _i<_NUM_BOOL_STRINGS; _i++ )
	{
		if( 0 == strnicmp( sBoolName[_i].szTrue, lpsz_, sizeof(sBoolName[0].szTrue) ) )
		{
			*pbool_ = TRUE;
			return TRUE;
		}

		if( 0 == strnicmp( sBoolName[_i].szFalse, lpsz_, sizeof(sBoolName[0].szTrue) ) )
		{
			*pbool_ = FALSE;
			return TRUE;
		}
	}

	return FALSE;
}

_DLLEXP BOOL _DLLFUNC PrintCclr( LPSTR lpBuffer_, CCLR clr_ )
{
	switch( clr_ & CLR_MASK )
	{
		case CLR_RGB:
			sprintf(lpBuffer_,
				gszFormat_DWORD_RGB,
				(unsigned)GetRValue(clr_),
				(unsigned)GetGValue(clr_),
				(unsigned)GetBValue(clr_) );
			return TRUE;

		case CLR_CORE:
		case CLR_SYS:
		{
			int _i;
			for( _i=0; _i<sizeof(sCclrName)/sizeof(sCclrName[0]); _i++ )
			{
				if( sCclrName[_i].clr == clr_ )
				{
					strcpy( lpBuffer_, sCclrName[_i].szName );
					return TRUE;
				}
			}

			//not found
			break;
		}
	}

	lpBuffer_[0] = 0;	//set to NULL string
	return FALSE;
}

_DLLEXP BOOL _DLLFUNC ScanCclr( LPCSTR lpsz_, CCLR* pclr_ )
{
	DWORD	_r, _g, _b;

	if(3 == sscanf( lpsz_,
					gszFormat_DWORD_RGB,
					&_r, &_g, &_b ) )
	{
		*pclr_ = RGB(_r, _g, _b);
		return TRUE;
	}
	else
	{
		int _i;
		for( _i=0; _i<sizeof(sCclrName)/sizeof(sCclrName[0]); _i++ )
		{
			if( 0 == strnicmp( sCclrName[_i].szName, lpsz_, sizeof(sCclrName[0].szName) ) )
			{
				*pclr_ = sCclrName[_i].clr;
				return TRUE;
			}
		}

		//not found
		return FALSE;
	}
}

BOOL _GetUserVsofValueString( WORD wTable_, WRD wrd_, WORD wNo_, LPGETVALUESTRING lpGVS_ )
{
	LPPU				_lpPU;
	VSOFENUMSTRUCT		_es;
	SimplePU			_spu;

	if(wTable_ == TBX_FT)
	{
		_lpPU = (gFTInfo.lpBase[wrd_]).lpParam + wNo_;
		if( !StampCallParamProc( FaceGetStamp(wrd_), PIC_GETVSOFENUMSTRUCT, wrd_, wNo_, (LONG)&_es ) )
			return FALSE;
	}
	else 
	{
		_lpPU = (gMTInfo.lpBase[wrd_]).lpParam + wNo_;
		if( !AlgoCallParamProc( ModuGetAlgo(wrd_), PIC_GETVSOFENUMSTRUCT, wrd_, wNo_, (LONG)&_es ) )
			return FALSE;
	}

	_spu.prefix = PF_IMM|IMM_DWORD;
	if( !PURead(&_spu, _lpPU) )
		return FALSE;

	return GetVsofName( &_es, _spu.du.zzk__DWORD, lpGVS_->lpszBuffer, lpGVS_->wSize );
}

BOOL _SetUserVsofValueString( WORD wTable_, WRD wrd_, WORD wNo_, LPCSTR lpsz_ )
{
	SimplePU			_spu;
	LPPU				_lpPU;
	VSOFENUMSTRUCT		_es;
	int					_i;

	if(wTable_ == TBX_FT)
	{
		_lpPU = (gFTInfo.lpBase[wrd_]).lpParam + wNo_;
		if( !StampCallParamProc( FaceGetStamp(wrd_), PIC_GETVSOFENUMSTRUCT, wrd_, wNo_, (LONG)&_es ) )
			return FALSE;
	}
	else 
	{
		_lpPU = (gMTInfo.lpBase[wrd_]).lpParam + wNo_;
		if( !AlgoCallParamProc( ModuGetAlgo(wrd_), PIC_GETVSOFENUMSTRUCT, wrd_, wNo_, (LONG)&_es ) )
			return FALSE;
	}

	_i = MatchVsofName( &_es, lpsz_ );
	if( _i < 0 )	//not matched
		return FALSE;

	_spu.prefix = PF_IMM|IMM_DWORD;
	_spu.du.zzk__DWORD = (_es.lpVsofName)[_i].Value;
	return PUWrite( _lpPU, &_spu );
}

BOOL _GetUserVsofValueString2( WORD wTable_, DWORD id_, WORD wNo_, LPPU lppu_, LPGETVALUESTRING lpGVS_ )
{
	LPPU				_lpPU = lppu_;
	VSOFENUMSTRUCT		_es;
	SimplePU			_spu;

	if(wTable_ == TBX_FT)
	{
		if( !StampCallParamProc( id_, PIC_GETVSOFENUMSTRUCT, 0, wNo_, (LONG)&_es ) )
			return FALSE;
	}
	else 
	{
		if( !AlgoCallParamProc( id_, PIC_GETVSOFENUMSTRUCT, 0, wNo_, (LONG)&_es ) )
			return FALSE;
	}

	_spu.prefix = PF_IMM|IMM_DWORD;
	if( !PURead(&_spu, _lpPU) )
		return FALSE;

	return GetVsofName( &_es, _spu.du.zzk__DWORD, lpGVS_->lpszBuffer, lpGVS_->wSize );
}

BOOL _SetUserVsofValueString2( WORD wTable_, DWORD id_, WORD wNo_, LPPU lppu_, LPCSTR lpsz_ )
{
	SimplePU			_spu;
	LPPU				_lpPU = lppu_;
	VSOFENUMSTRUCT		_es;
	int					_i;

	if(wTable_ == TBX_FT)
	{
		if( !StampCallParamProc( id_, PIC_GETVSOFENUMSTRUCT, 0, wNo_, (LONG)&_es ) )
			return FALSE;
	}
	else 
	{
		if( !AlgoCallParamProc( id_, PIC_GETVSOFENUMSTRUCT, 0, wNo_, (LONG)&_es ) )
			return FALSE;
	}

	_i = MatchVsofName( &_es, lpsz_ );
	if( _i < 0 )	//not matched
		return FALSE;

	_spu.prefix = PF_IMM|IMM_DWORD;
	_spu.du.zzk__DWORD = (_es.lpVsofName)[_i].Value;
	return PUWrite( _lpPU, &_spu );
}

BOOL GetVsofName( LPVSOFENUMSTRUCT lpEnum_, DWORD value_, LPSTR lpsz_, int buf_size_ )
{
	int	_i;

	for( _i=0; _i<lpEnum_->nItem; _i++ )
	{
		if( (lpEnum_->lpVsofName)[_i].Value == value_ )
		{
			StringCopyn( lpsz_, (lpEnum_->lpVsofName)[_i].szName, buf_size_ );
			return TRUE;
		}
	}

	return FALSE;
}

//Note:
//	On error, returns -1;
int MatchVsofName( LPVSOFENUMSTRUCT lpEnum_, LPCSTR lpsz_ )
{
	int	_i;

	for( _i=0; _i<lpEnum_->nItem; _i++ )
	{
		if( 0 == stricmp( (lpEnum_->lpVsofName)[_i].szName, lpsz_) )
			return _i;
	}

	return -1;
}

/////////////////////////////////////////////////////////
//Entries:
//
//	WORD	wTable_
//	WRD		wrd_ 
//	WORD	order_ 
//	LPPU	lpPU_ 
//	LPSTR	lpsz_ 
//	UINT	size_ 
//
_DLLEXP BOOL _DLLFUNC EnumVsofString( WORD wTable_, WRD wrd_, WORD wNo_, LPPU lpPU_, LPSTR lpsz_, UINT size_ )
{
	VSOFENUMSTRUCT	_es;
	VSOF			_vsof;

	if(wTable_ == TBX_FT)
		_vsof = StampCallParamProc( FaceGetStamp(wrd_), PIC_GETPARAMVSOF, wrd_, wNo_, 0L);
	else 
		_vsof = AlgoCallParamProc( ModuGetAlgo(wrd_), PIC_GETPARAMVSOF, wrd_, wNo_, 0L);

	lpsz_[0] = 0;

	if( _vsof==VSOF_BOOL ||		//VSOF_BOOL_TRUEFALSE
		_vsof==VSOF_BOOL_YESNO ||	
		_vsof==VSOF_BOOL_ONOFF ||	
		_vsof==VSOF_BOOL_OPENCLOSE ||	
		_vsof==VSOF_BOOL_10 ||	
		_vsof==VSOF_int_LNSTYLE ||	
		_vsof==VSOF_int_RECTBS ||	
		_vsof==VSOF_int_GBS ||	
		_vsof==VSOF_int_ACTIVE ||	
		_vsof==VSOF_DWORD_TXTVERALIGN ||	
		_vsof==VSOF_DWORD_TXTHORALIGN ||	
		_vsof==VSOF_DWORD_DISPLAYMODE ||
		HIWORD(_vsof)==0xFFFF )
	{
		if(wTable_ == TBX_FT)
		{
			if( !StampCallParamProc( FaceGetStamp(wrd_), PIC_GETVSOFENUMSTRUCT, wrd_, wNo_,  (LONG)&_es ) )
				return FALSE;
		}
		else 
		{
			if( !AlgoCallParamProc( ModuGetAlgo(wrd_), PIC_GETVSOFENUMSTRUCT, wrd_, wNo_,  (LONG)&_es ) )
				return FALSE;
		}

		if( !lpPU_->prefix )
			return BeginEnum( &_es, lpPU_, lpsz_, size_ );
		else
			return EnumNext( &_es, lpPU_, lpsz_, size_ );
	}
	else
		return FALSE;
}

BOOL BeginEnum( LPVSOFENUMSTRUCT lpes_, LPPU lpPU_, LPSTR lpsz_, UINT size_  )
{
	CORE_ASSERT( lpes_->nItem );

	lpPU_->prefix = PF_IMM|IMM_DWORD;
	lpPU_->du.zzk__DWORD = (lpes_->lpVsofName)[0].Value;
	StringCopyn( lpsz_, (lpes_->lpVsofName)[0].szName, size_ );

	return TRUE;
}

BOOL EnumNext( LPVSOFENUMSTRUCT lpes_, LPPU lpPU_, LPSTR lpsz_, UINT size_ )
{
	int		_i;

	CORE_ASSERT( lpPU_->prefix == (PF_IMM|IMM_DWORD) );

	for( _i=0; _i<lpes_->nItem; _i++ )
	{
		if( _i == (lpes_->nItem - 1 ) )
			return FALSE;	//this is the last one

		if( lpPU_->du.zzk__DWORD == (lpes_->lpVsofName)[_i].Value )
		{
			lpPU_->du.zzk__DWORD = (lpes_->lpVsofName)[_i+1].Value;
			StringCopyn( lpsz_, (lpes_->lpVsofName)[_i+1].szName, size_ );
			return TRUE;
		}
	}

	CORE_ASSERT( FALSE );
	return FALSE;
}

static VSOFNAME sBoolTrueFalse[] = 
{
	{FALSE,	"False"},
	{TRUE,	"True"},
};

static VSOFNAME sBoolYesNo[] = 
{
	{FALSE,	"No"},
	{TRUE,	"Yes"},
};

static VSOFNAME sBoolOnOff[] = 
{
	{FALSE,	"Off"},
	{TRUE,	"On"},
};

static VSOFNAME sBoolOpenClose[] = 
{
	{FALSE,	"Close"},
	{TRUE,	"Open"},
};

static VSOFNAME sBoolManuAuto[] = 
{
	{FALSE,	"Manu"},
	{TRUE,	"Auto"},
};

static VSOFNAME sBool10[] = 
{
	{FALSE,	"0"},
	{TRUE,	"1"},
};

static VSOFNAME sLineStyle[] = 
{
	{PS_SOLID,		"Solid"},		//PS_SOLID			  0
	{PS_DASH,		"Dash"},		//PS_DASH			  1       /* -------  */
	{PS_DOT,		"Dot"},			//PS_DOT              2       /* .......  */
	{PS_DASHDOT,	"DashDot"},		//PS_DASHDOT          3       /* _._._._  */
	{PS_DASHDOTDOT,	"DashDotDot"},	//PS_DASHDOTDOT       4       /* _.._.._  */
	{PS_NULL,		"Null"},		//PS_NULL             5
									//PS_INSIDEFRAME      6
};

static VSOFNAME sActive[] = 
{
	{FA_DISABLE,	"Disable"},
	{FA_STANDARD,	"Standard"},
	{FA_DEFAULT,	"Default"},
};

static VSOFNAME sRectBStyle[] = 
{
	{PS_INSIDEFRAME,	"Inside"},		//PS_INSIDEFRAME      6
	{PS_DASH,			"Dash"},		//PS_DASH			  1       /* -------  */
	{PS_DOT,			"Dot"},			//PS_DOT              2       /* .......  */
	{PS_DASHDOT,		"DashDot"},		//PS_DASHDOT          3       /* _._._._  */
	{PS_DASHDOTDOT,		"DashDotDot"},	//PS_DASHDOTDOT       4       /* _.._.._  */
	{PS_NULL,			"Null"},		//PS_NULL             5
	{ZLS_DIP,			"Dip"},
	{ZLS_BUMP,			"Bump"},
	{ZLS_RAISED,		"Raised"},
	{ZLS_SUNKEN,		"Sunken"},
};

static VSOFNAME sGenBStyle[] = 
{
	{PS_INSIDEFRAME,	"Inside"},		//PS_INSIDEFRAME      6
	{PS_DASH,			"Dash"},		//PS_DASH			  1       /* -------  */
	{PS_DOT,			"Dot"},			//PS_DOT              2       /* .......  */
	{PS_DASHDOT,		"DashDot"},		//PS_DASHDOT          3       /* _._._._  */
	{PS_DASHDOTDOT,		"DashDotDot"},	//PS_DASHDOTDOT       4       /* _.._.._  */
	{PS_NULL,			"Null"},		//PS_NULL             5
};

static VSOFNAME sTextVerAlign[] =	//DrawText
{
	{DT_TOP,			"Top"},		//0x00000000
	{DT_VCENTER,		"Middle"},	//0x00000004
	{DT_BOTTOM, 		"Bottom"},	//0x00000008
};

static VSOFNAME sTextHorAlign[] =	//DrawText
{
	{DT_LEFT,			"Left"},	//0x00000000
	{DT_CENTER, 		"Center"},	//0x00000001
	{DT_RIGHT,			"Right"}, 	//0x00000002
};

static VSOFNAME sDisplayMode[] =	//picture display mode
{
	{DM_FIX,		"Fix"},
	{DM_STRETCH,	"Stretch"},
	{DM_TILE,		"Tile"},		//NOT for WMF
	{DM_CENTER,		"Center"},		//NOT for WMF
	{DM_SMART,		"Smart"},
};

BOOL _GetSystemVsofEnumStruct( VSOF vsof_, LPVSOFENUMSTRUCT lpes_ )
{
	switch( vsof_ )
	{
		case VSOF_BOOL:	//VSOF_BOOL_TRUEFALSE
			lpes_->lpVsofName = sBoolTrueFalse;
			lpes_->nItem = sizeof(sBoolTrueFalse)/sizeof(sBoolTrueFalse[0]);
			return TRUE;

		case VSOF_BOOL_YESNO:
			lpes_->lpVsofName = sBoolYesNo;
			lpes_->nItem = sizeof(sBoolYesNo)/sizeof(sBoolYesNo[0]);
			return TRUE;

		case VSOF_BOOL_ONOFF:
			lpes_->lpVsofName = sBoolOnOff;
			lpes_->nItem = sizeof(sBoolOnOff)/sizeof(sBoolOnOff[0]);
			return TRUE;

		case VSOF_BOOL_MANUAUTO:
			lpes_->lpVsofName = sBoolManuAuto;
			lpes_->nItem = sizeof(sBoolManuAuto)/sizeof(sBoolManuAuto[0]);
			return TRUE;

		case VSOF_BOOL_OPENCLOSE:
			lpes_->lpVsofName = sBoolOpenClose;
			lpes_->nItem = sizeof(sBoolOpenClose)/sizeof(sBoolOpenClose[0]);
			return TRUE;

		case VSOF_BOOL_10:
			lpes_->lpVsofName = sBool10;
			lpes_->nItem = sizeof(sBool10)/sizeof(sBool10[0]);
			return TRUE;

		case VSOF_int_ACTIVE:
			lpes_->lpVsofName = sActive;
			lpes_->nItem = sizeof(sActive)/sizeof(sActive[0]);
			return TRUE;

		case VSOF_int_LNSTYLE:
			lpes_->lpVsofName = sLineStyle;
			lpes_->nItem = sizeof(sLineStyle)/sizeof(sLineStyle[0]);
			return TRUE;

		case VSOF_int_RECTBS:	//rectanglar border style
			lpes_->lpVsofName = sRectBStyle;
			lpes_->nItem = sizeof(sRectBStyle)/sizeof(sRectBStyle[0]);
			return TRUE;

		case VSOF_int_GBS:		//general border style
			lpes_->lpVsofName = sGenBStyle;
			lpes_->nItem = sizeof(sGenBStyle)/sizeof(sGenBStyle[0]);
			return TRUE;

		case VSOF_DWORD_TXTVERALIGN:
			lpes_->lpVsofName = sTextVerAlign;
			lpes_->nItem = sizeof(sTextVerAlign)/sizeof(sTextVerAlign[0]);
			return TRUE;

		case VSOF_DWORD_TXTHORALIGN:
			lpes_->lpVsofName = sTextHorAlign;
			lpes_->nItem = sizeof(sTextHorAlign)/sizeof(sTextHorAlign[0]);
			return TRUE;

		case VSOF_DWORD_DISPLAYMODE:
			lpes_->lpVsofName = sDisplayMode;
			lpes_->nItem = sizeof(sDisplayMode)/sizeof(sDisplayMode[0]);
			return TRUE;

		default:
			return FALSE;
	}
}
