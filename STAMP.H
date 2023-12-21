//////////////////////////////////////////////////////
//Macroes
#define _PDU_DEFAULT									\
	{FACE_X,		"X",		VSOF_FCCRD,			PRI_ALL|PDUF_NODEFAULT, {PF_IMM|PDT_FCCRD,  PU_NORMAL, NULL } },	\
	{FACE_Y,		"Y",		VSOF_FCCRD,			PRI_ALL|PDUF_NODEFAULT, {PF_IMM|PDT_FCCRD,  PU_NORMAL, NULL } },	\
	{FACE_CX,		"cX",		VSOF_FCCRD,			PRI_ALL|PDUF_NODEFAULT, {PF_IMM|PDT_FCCRD,  PU_NORMAL, NULL } },	\
	{FACE_CY,		"cY",		VSOF_FCCRD,			PRI_ALL|PDUF_NODEFAULT, {PF_IMM|PDT_FCCRD,  PU_NORMAL, NULL } },	\
	{FACE_VISIBLE,	"Visible",	VSOF_BOOL,			PRI_ENG|PDUF_CANNTCOPY,	{PF_IMM|PDT_BOOL,   PU_NORMAL, TRUE } },	\
	{FACE_ACTIVE,	"Active",	VSOF_int_ACTIVE,	PRI_ENG|PDUF_CANNTCOPY, {PF_IMM|PDT_int,    PU_NORMAL, NULL } },	\
	{FACE_TRIGGER,	"Trigger",	VSOF_WRDMTU,		PRI_ENG|PDUF_CANNTCOPY, {PF_IMM|IMM_WRDMTU, PU_NORMAL, NULL } }

#define _DefStampParam(wCmd_, wrd_, wNo_, lParam_, wNum_, lpPDU_)		\
	DefProcessPIC(wCmd_, wrd_, wNo_, lParam_, (WORD)TBX_FT, wNum_, lpPDU_)

//////////////////////////////////////////////////////
//Face parameters
/////////////////
#define IDS_COMBINE			0X01030100L
#define IDS_BUTTON			0X01040100L
#define IDS_PICTURE			0X01050100L

#define IDS_RECT			0X02010100L
#define IDS_RECTFRAME		0X02011100L

#define IDS_ELLIPSE			0X02020100L
#define IDS_ELLIPSEFRAME	0X02021100L

#define IDS_ROUNDRECT		0X02030100L
#define IDS_ROUNDRECTFRAME	0X02031100L

#define IDS_ARC				0X02040100L

#define IDS_LINE			0X02100100L
#define IDS_MULTILINE		0X02210100L

#define IDS_STRING			0X02200100L

#define IDS_SLIDE			0x02300200L
#define IDS_FILL			0x02400100L

#define IDS_TREND			0X04010100L
#define IDS_HISTREND		0x04020100L

//_DLLEXP void _ZZKDECL _ReadFaceXYCXCY(LPPU lpPrm_, LPXYCXCY lpPst_);
_DLLEXP void _ZZKDECL WriteFaceXYCXCY(WRD wFace_, LPRECT lpRc_);
_DLLEXP void _ZZKDECL WriteFaceXY(WRD wFace_, int x_, int y_);

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

_DLLEXP void _ZZKDECL WriteFaceCXCY(WRD wFace_, int cx_, int cy_);




