//===================================================================
//Cstmpfn.h
//Zheng zhikai  95-4-16
//===================================================================

#ifndef __CSTMPFN_H
#define __CSTMPFN_H

//////////////////////////////////////////////////////
//Macroes
#define _PDU_DEFAULT									\
	{FACE_X,		"X",		VSOF_FCCRD,			PRI_ALL|PDUF_NODEFAULT, {PF_IMM|PDT_FCCRD,  PU_NORMAL, NULL } },	\
	{FACE_Y,		"Y",		VSOF_FCCRD,			PRI_ALL|PDUF_NODEFAULT, {PF_IMM|PDT_FCCRD,  PU_NORMAL, NULL } },	\
	{FACE_CX,		"cX",		VSOF_FCCRD,			PRI_ALL|PDUF_NODEFAULT, {PF_IMM|PDT_FCCRD,  PU_NORMAL, NULL } },	\
	{FACE_CY,		"cY",		VSOF_FCCRD,			PRI_ALL|PDUF_NODEFAULT, {PF_IMM|PDT_FCCRD,  PU_NORMAL, NULL } },	\
	{FACE_VISIBLE,	"Visible",	VSOF_BOOL,			PRI_ENG|PDUF_CANNTCOPY,	{PF_IMM|PDT_BOOL,   PU_NORMAL, TRUE } },	\
	{FACE_ACTIVE,	"Active",	VSOF_int_ACTIVE,	PRI_ENG|PDUF_CANNTCOPY, {PF_IMM|PDT_int,    PU_NORMAL, NULL } },	\
	{FACE_TRIGGER,	"Trigger",	VSOF_WRDMTU_TRIGGER,PRI_ENG|PDUF_CANNTCOPY, {PF_IMM|IMM_WRDMTU, PU_NORMAL, NULL } }

#define _DefStampParam(wCmd_, wrd_, wNo_, lParam_, wNum_, id_, lpPDU_)		\
	DefProcessPIC(wCmd_, wrd_, wNo_, lParam_, (WORD)TBX_FT, wNum_, (DWORD)id_, lpPDU_)


_DLLEXP void _ZZKDECL WriteFaceXYCXCY(WRD wFace_, LPRECT lpRc_);
_DLLEXP void _ZZKDECL WriteFaceXY(WRD wFace_, FCCRD x_, FCCRD y_);
_DLLEXP void _ZZKDECL WriteFaceCXCY(WRD wFace_, FCCRD cx_, FCCRD cy_);

//return MOUSEON_NULL or selecting handle
_DLLEXP WORD _ZZKDECL TestMouseOnRSH(LPRECT lpRect_, POINT point_, WORD wSHSize_);

_DLLEXP void _ZZKDECL PropertyRect(LPRECT lpRect_);
_DLLEXP void _ZZKDECL ToggleRSH(HDC hdc_, LPRECT lpRect_, WORD wSHSize_);
_DLLEXP DWORD _ZZKDECL DefStampBuild(SCMD wCmd_, WRD wFTU_, HWND hwnd_, WORD wParam_, LONG lParam_);

_DLLEXP void _ZZKDECL WindowCoorToFaceCoor(WRD wFTU_, LPFCCRD lpx_, LPFCCRD lpy_);
_DLLEXP void _ZZKDECL FaceCoorToWindowCoor(WRD wFTU_, LPFCCRD lpx_, LPFCCRD lpy_);
_DLLEXP void _ZZKDECL XycxcyToRect(WRD wFace_, LPRECT lprc_, LPXYCXCY lpcc_);
_DLLEXP void _ZZKDECL RectToXycxcy(WRD wFace_, LPRECT lprc_, LPXYCXCY lpcc_);
_DLLEXP void _ZZKDECL FaceXycxcyToRect(WRD wFace_, LPRECT lprc_);

_DLLEXP void _ZZKDECL GetFaceXycxcyInWinCoor(WRD wFTU_, LPXYCXCY lpcc_);
_DLLEXP void _DLLFUNC SetFaceXycxcyInWinCoor(WRD wFTU_, LPXYCXCY lpcc_);

#endif	//__CSTMPFN_H