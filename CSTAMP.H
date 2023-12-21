//===================================================================
//
//Zheng zhikai	95-10-4
//===================================================================

#ifndef __CSTAMP_H
#define __CSTAMP_H

#ifndef __CPU_H
#include "cpu.h"
#endif

/////////////////////////////
//Stamp Table

//declare here: forward reference
typedef enum _tagSCMD SCMD;

typedef BOOL (_ZZKDECL FAR* SRTPROC)(WRD wFace_, HDC hdc_);
typedef DWORD (_ZZKDECL FAR* SBTPROC)(SCMD wCmd_, WRD wFTU_, HWND hwndParent_, WORD wParam_, LONG lParam_);

#define LENGTH_NAME_STAMP	32
typedef struct tagSTAMP_TABLE_UNIT
{
	char		szName[LENGTH_NAME_STAMP];
	IDSTAMP		wID;
	SRTPROC		lpfnPaint;			//Run-time(drawing) routine
	PARAMPROC	lpfnParam;			//access parameter infomation
	SBTPROC		lpfnBuild;			//Build-time routine
	DWORD		Flags;
}STU, FAR* LPSTU;

#define ST_NAME			0x0001
#define ST_ID    		0x0002
#define ST_PAINTPROC	0x0003
#define ST_PARAMPROC	0x0004
#define ST_BUILDPROC	0x0005

//define STU.Flags bit
#define STF_HIDEN		0x00000001L		//hiden stamp

typedef struct tagSTAMP_TABLE_INFO
{
	WRD			wCount;
	WRD			wAdding;
	HGLOBAL		hBase;
	LPSTU 		lpBase;
}STInfo;

/////////////
//Detail information of IDs of stamps references to csysstmp.h
/////////////

typedef struct _tagXYCXCY
{
	FCCRD x;
	FCCRD y;
	FCCRD cx;
	FCCRD cy;
}XYCXCY, FAR* LPXYCXCY;

typedef XYCXCY const FAR * LPCXYCXCY;

#define		MOUSEON_NULL	( (WORD)(0xFFFF) )
#define		MOUSEON_BODY	( (WORD)(0xFFFE) )

typedef struct _tagFACE_INPUT_ACTION
{
	WORD		wPrecision;     //select precision in pixels
	WORD		wSHSize;		//the size of Selecting-handle
	HDC			hdc;			//the HDC on current window

	//those two elements should be maitained by StampBuildProc
	POINT		dtPrimary;
	POINT		dtLast;

//FIA.MsgCode
#define		MSG_TRIGGER		0x0010	//VK_SPACE, VK_RETURN, WM_LBUTTONDOWN, WM_LBUTTONDBLCLK
#define		MSG_ARROW		0x0020	//VK_arrows and pages, home, end
#define		MSG_TAB			0x0040	//VK_TAB
#define		MSG_OTHERSKEYS	0x0080	//other keys
#define		MSG_MOUSE		0x8000	//is a mouse message, otherwise, it is a keyboard message
//mask code
#define		MSG_WANTMASK	0x0FF0

	UINT		MsgCode;		//MSG_arrow, MSG_TAB, MSG_TRIGGER

	UINT		msg;			//input message
	WPARAM		wParam;			//wParam of the message
	LPARAM		lParam;			//LParam of the message

	void FAR*	lpPrivate;		//point to face owned private data
	WORD		wPid;			//the id of processor;
								//the id of default processor(DefStampBuild) is 0;
								//and DefStampBuild ignore this value;
								//this value is only for face-build process;
								//to identify whether trap the message by itself;
} FIA, FAR* LPFIA;

typedef struct _tagFACE_INIT
{
	//current line setting:
	LOGPEN		line_pen;			//logPen struct

	//current border setting:
	LOGPEN		border_pen;			//logPen struct

	//current background setting:
	COLORREF	back_clr;

	//current text setting:
	LOGFONT		text_font;		//LOGFONT struct
	UINT		text_horalign;	//horizontal align usage: flags for DrawText()
	UINT		text_veralign;	//vertical align usage: flags for DrawText()
	COLORREF	text_clr;		//Text foreground color

} FINIT, FAR* LPFINIT;

/////////////////////////////////////////////////
//SCMD_INIT
//Description:
//	This command is sent to initialize the parameters of
//	the face when a face is just created.
//Note:
//	if returns FALSE, the face will be freed by caller.
//Parameters:
//	wFTU_:			specifies the Face (never be NULL)
//	hwndParent_:	the HWND of window owning the Face
//	wParam_:		n/a
//	lParam_:		points to struct FINIT
//Return:
//	TRUE/FALSE, if returns FALSE, the Face will
//	be freed by caller.

/////////////////////////////////////////////////
//SCMD_CONSTRUCT
//Description:
//	When a face is just created, this cmd is sent to
//	initialize parameter and query whether the face
//	entry to construction-capture mode. If the face
//	returns TRUE, the system entries to construction
//	mode for the face and dispatchs the all subsequent
//	input messages to the face by command SCMD_INIT_INPUTMSG.
//	If the face returns FALSE, the system skips con-
//	struction-capture mode.
//	Refer to SCMD_INIT_INPUTMSG for more information.
//Parameters:
//	wFTU_:			specifies the Face (never be NULL)
//	hwndParent_:	the HWND of window owning the Face
//	wParam_:		n/a
//	lParam_:		points to struct FIA
//Return:
//	TRUE/FALSE

/////////////////////////////////////////////////
//SCMD_CONSTRUCT_INPUTMSG
//Description:
//	During the period of construction of the new face,
//	every input action, mouse and key, causes the system
//	to send the command to the new face until the face
//	returns FIA_PROCESSEND.
//Note:
//	After the face returns FIA_PROCESSEND, the new face
//	should finish construction.
//Parameters:
//	wFTU_:			specifies the Face (never be NULL)
//	hwndParent_:	the HWND of window owning the Face
//	wParam_:		n/a
//	lParam_:		points to struct FIA
//Return:
//	return FIA_xxxx.

/////////////////////////////////////////////////
//SCMD_EDIT
//Description:
//	Standard operation is:
//	When mouse left button is down at a selected face,
//	this cmd is sent to it to query whether the face
//	entry to editing-capture mode. If the face returns
//	TRUE, the system entry to editing-capture mode for
//	the face and dispatchs all subsequent input messages
//	to the face by command SCMD_EDIT_INPUTMSG; If the
//	face returns FALSE, the system skips editing-capture
//	mode;
//	The default procedure, calculate whether the mouse is
//	on the face's select handle or body, if so, returns
//	TRUE; Otherwise, returns FALSE. For detail, refers
//	to the function DefStampBuild(...);
//	Refer to SCMD_EDIT_INPUTMSG for more information;
//Parameters:
//	wFTU_:			specifies the Face (never be NULL)
//	hwndParent_:	the HWND of window owning the Face
//	wParam_:		n/a
//	lParam_:		points to struct FIA
//Return:
//	TRUE/FALSE

/////////////////////////////////////////////////
//SCMD_EDIT_INPUTMSG
//Description:
//	During the period of editing of a face, every input
//	action, mouse and key, causes the system to send
//	the command to the face until the face returns
//	FIA_PROCESSEND;
//Note:
//	The face should update the window, if necessary, to show
//	the new shape by itself after editing.
//Parameters:
//	wFTU_:			specifies the Face (never be NULL)
//	hwndParent_:	the HWND of window owning the Face
//	wParam_:		n/a
//	lParam_:		points to struct FIA
//Return:
//	return FIA_xxxx.

/////////////////////////////////////////////////
//SCMD_RUNTIME
//Description:
//	Standard operation is:
//	While an input message is received by a face window at 
//	run-time. This cmd is sent to the active face to query
//	if the face needs the input message and whether the face 
//	entry to RT-capture mode. If the face
//	returns TRUE, the system entry to RT-capture mode for
//	the face and dispatchs all subsequent input messages
//	to the face by command SCMD_RUNTIME_INPUTMSG; If the
//	face returns FALSE, the system skips RT-capture
//	mode;
//	Refer to SCMD_EDIT_INPUTMSG for more information;
//Parameters:
//	wFTU_:			specifies the Face (never be NULL)
//	hwndParent_:	the HWND of window owning the Face
//	wParam_:		n/a
//	lParam_:		points to struct FIA
//Return:
//	RT_CAPTURE:		prepare to entry RT-capture
//	RT_HDC			if this bit set, system prepare HDC
//					for face;
//
#define		RT_CAPTURE			0x0001			//want to capture mouse message
#define		RT_HDC				0x0002			//want to get a HDC
#define		RT_WANTTRIGGERS		MSG_TRIGGER		//want to accept tigger messages, i.e. VK_SPACE, VK_RETURN, WM_LBUTTONDOWN, WM_LBUTTONDBLCLK
#define		RT_WANTARROWS		MSG_ARROW		//want to arrows keys
#define		RT_WANTTAB			MSG_TAB			//want to VK_TAB
#define		RT_WANTALLKEYS		MSG_OTHERSKEYS	//want to accept any key message

/////////////////////////////////////////////////
//SCMD_RUNTIME_INPUTMSG
//Description:
//	For run-time active face, when the face is active
//	state, every input action, mouse and key, causes
//	the system to send the command to the face until
//	the face returns FIA_PROCESSEND;
//Note:
//	The face should update the window, if necessary, to show
//	the new shape by itself after editing.
//Parameters:
//	wFTU_:			specifies the Face (never be NULL)
//	hwndParent_:	the HWND of window owning the Face
//	wParam_:		n/a
//	lParam_:		points to struct FIA
//Return:
//	return FIA_xxxx.

/////////////////////////////////////////////////
//SCMD_MOVE
//Description:
//	this cmd is sent to the face to perform to prepare
//	the invalid rectangles for face-moving.
//	The delta(x,y) to move is in Windows device coor.
//Parameters:
//	wFTU_:			specifies the Face (never be NULL)
//	hwndParent_:	the HWND of window owning the Face
//	wParam_:		select-handle size
//	lParam_:		struct POINT(deltaY, deltaY)
//					(using MAKEPOINT to access it)
//					for WIN32, using macro X(), Y() to access it
//Return:
//	(TRUE/FALSE , igored)

/////////////////////////////////////////////////
//SCMD_MOUSEONBODY
//Description:
//	Tests whether mouse is on body of the face, this
//	cmd is sent.
//Parameters:
//	wFTU_:			specifies the Face (never be NULL)
//	hwndParent_:	the HWND of window owning the Face
//	wParam_:		the select precision
//	lParam_:		struct POINT (using MAKEPOINT access it)
//					for WIN32, using macro X(), Y() to access it
//
//					
//Return:
//	MOUSEON_NULL / MOUSEON_BODY

/////////////////////////////////////////////////
//SCMD_MOUSEONHANDLE
//Description:
//	Tests whether mouse is on select-handle of the face,
//	this cmd is sent.
//Result:
//	This cmd changes FIA.eMouseOn and FACEOPTENV.wHandle
//Parameters:
//	wFTU_:			specifies the Face (never be NULL)
//	hwndParent_:	the HWND of window owning the Face
//	wParam_:		the wSHSize
//	lParam_:		struct POINT (using MAKEPOINT access it)
//					for WIN32, using macro X(), Y() to access it
//Return:
//	MOUSEON_NULL or the handle selected
//Note:
//	The user type handle id is greater than 0x7FFF
//	The standard rect. sel. handle is as the following
//
//	RSH	:Rectangle selecting handle
//
//		0	1	2
//		7		3
//		6	5	4

/////////////////////////////////////////////////
//SCMD_TOGGLESELECTHANDLES
//Description:
//	When needs to toggle select-handles, the cmd is sent.
//Parameters:
//	wFTU_:			specifies the Face (never be NULL)
//	hwndParent_:	the HWND of window owning the Face
//	wParam_:		the wSHSize
//	lParam_:		HDC
//Return:
//	(TRUE/FALSE , igored)

/////////////////////////////////////////////////
//SCMD_UPDATE
//Description:
//	The cmd is sent when the window need to update.
//	The stamp procedure needs to invalidate the old
//	face position and update the Face.Rect;
//	The main routine calls this command at two cases:
//		1.	update the Face.Rect when the system
//			initiates from biuld-time to run-time;
//		2.	update the face windows at the build-time
//			ref. to function _BT_Update_FaceTree() in
//			file crun.c;
//Parameters:
//	wFTU_:			specifies the Face (never be NULL)
//	hwndParent_:	the HWND of window owning the Face
//	wParam_:		the select handle size
//	lParam_:        the LOWORD is the first dirty PU
//Return:
//	(TRUE/FALSE , igored)

/////////////////////////////////////////////////
//SCMD_BLINK
//Description:
//	The cmd just like SCMD_UPDATE, is sent when the face
//	has blink color previously and the blink area needs
//	to blink at run-time. The stamp procedure needs to
//	calculate the blink area and invalidate it; If no
//	blink in face, the procedure should clear FU_BLINK
//	to avoid this cmd being sent again;
//Note:
//	For each dirty face, after sending SCMD_UPDATE, this
//	cmd is sent; for each clean face, no cmd SCMD_UPDATE
//	sent, only this cmd sent to the face;
//Parameters:
//	wFTU_:			specifies the Face (never be NULL)
//	hwndParent_:	the HWND of window owning the Face
//	wParam_:		the select handle size
//	lParam_:        NULL
//Return:
//	(TRUE/FALSE , igored)

/////////////////////////////////////////////////
//SCMD_GETREGION
//Description:
//	When , the cmd is sent. Sorry, I forget it!
//Parameters:
//	wFTU_:			specifies the Face (never be NULL)
//	hwndParent_:	the HWND of window owning the Face
//	wParam_:		the delta to enlarge the region
//	lParam_:        n/a
//Return:
//	HRGN or NULL

/////////////////////////////////////////////////
//SCMD_GETINVALIDRGN
//Description:
//	This cmd is sent for calculating the invalid Rgn;
//Parameters:
//	wFTU_:			specifies the Face (never be NULL)
//	hwndParent_:	the HWND of window owning the Face
//	wParam_:		n/a
//	lParam_:		the pointer to GIR struction
//Return:
//	n/a
//Note:
//	reference to core.txt
typedef struct _tag_Get_Invalid_RGN{
	HRGN	girDirtyRgn;
} GIR, FAR* LPGIR;

/////////////////////////////////////////////////
//SCMD_GETFOCUSRECT
//Description:
//	This cmd is sent for calculating the invalid Rgn;
//Parameters:
//	wFTU_:			specifies the Face (never be NULL)
//	hwndParent_:	the HWND of window owning the Face
//	wParam_:		n/a
//	lParam_:		the pointer to RECT struction
//Return:
//	n/a

/////////////////////////////////////////////////
//SCMD_TICK
//Description:
//	This cmd is sent to the face which needs ticks, such as trend,
//	at each tick;
//Parameters:
//	wFTU_:			specifies the Face (never be NULL)
//	hwndParent_:	the HWND of window owning the Face
//	wParam_:		n/a
//	lParam_:		tick number
//Return:
//	n/a


typedef enum _tagSCMD{
SCMD_INIT = 0X0001,
SCMD_CONSTRUCT,
SCMD_CONSTRUCT_INPUTMSG,
SCMD_EDIT,
SCMD_EDIT_INPUTMSG,
SCMD_MOVE,
SCMD_MOUSEONBODY,
SCMD_MOUSEONHANDLE,
SCMD_TOGGLESELECTHANDLES,
SCMD_UPDATE,
SCMD_GETREGION,
SCMD_RUNTIME,
SCMD_RUNTIME_INPUTMSG,
SCMD_BLINK,
SCMD_GETINVALIDRGN,
SCMD_GETFOCUSRECT,
SCMD_TICK,
} SCMD;

//the return value as the result of the cmd SCMD_xxx_INPUTMSG
#define FIA_NOTPROCESSED			0x0000
#define FIA_PROCESSED				0x0001
#define FIA_DELETEFACE				0X8000
#define FIA_PROCESSEND				0x0003

_DLLEXP BOOL _ZZKDECL RegisterStamp(STU FAR* lp_);
_DLLEXP BOOL _ZZKDECL UnregisterStamp( IDSTAMP wID_ );

_DLLEXP STInfo FAR* _ZZKDECL GetStampTableInformation(void);
_DLLEXP BOOL _ZZKDECL IsStamp( IDSTAMP id_ );

_DLLEXP DWORD _ZZKDECL StampGetFlags(IDSTAMP id_);
_DLLEXP void _ZZKDECL StampSetFlags(IDSTAMP id_, DWORD dwFlags_ );

_DLLEXP BOOL _ZZKDECL StampGetName(IDSTAMP id_, LPSTR szBuffer_, WORD wBufferSize_);
_DLLEXP WORD _ZZKDECL StampGetNumParam(IDSTAMP id_);

_DLLEXP WORD _ZZKDECL StampGetParamPrivilege(IDSTAMP id_, WORD wNo_);
_DLLEXP BOOL _ZZKDECL StampGetVarParamStruct(IDSTAMP id_, LPVPS lpVps_ );

_DLLEXP PDT  _ZZKDECL StampGetParamType(IDSTAMP id_, WORD wNo_);
_DLLEXP BOOL _ZZKDECL StampGetParamTypeName(IDSTAMP id_, WORD wNo_, LPGETPARAMTYPENAME lpGPTN_);
_DLLEXP BOOL _ZZKDECL StampGetParamName(IDSTAMP id_, WORD wNo_, LPGETPARAMNAME lpGPN_);
_DLLEXP BOOL _ZZKDECL StampGetParamOrder(IDSTAMP id_, LPGETPARAMORDER lpGPO_);

_DLLEXP BOOL _ZZKDECL StampEncode(IDSTAMP id_, WRD wFace_, WORD wNo_, LPENCODE lpDecode_);
_DLLEXP BOOL _ZZKDECL StampDecode(IDSTAMP id_, WRD wFace_, WORD wNo_, LPDECODE lpDecode_);

_DLLEXP BOOL _ZZKDECL StampGetValueString(IDSTAMP id_, WRD wFace_, WORD wNo_, LPGETVALUESTRING lpvs_);
_DLLEXP BOOL _ZZKDECL StampSetValueString(IDSTAMP id_, WRD wFace_, WORD wNo_, LPCSTR lpsz_);

_DLLEXP BOOL _ZZKDECL StampInitiate(IDSTAMP id_, WRD wFace_, LONG lSampleInterval_);
_DLLEXP BOOL _ZZKDECL StampStop(IDSTAMP id_, WRD wFace_);

_DLLEXP BOOL _ZZKDECL StampPICCreate(IDSTAMP id_, WRD wFace_, LONG lData_);
_DLLEXP BOOL _ZZKDECL StampPICPaste(IDSTAMP id_, WRD wFace_);
_DLLEXP BOOL _ZZKDECL StampPICDelete(IDSTAMP id_, WRD wFace_);

_DLLEXP PREFIX  _ZZKDECL StampGetParamDefaultPrefix(IDSTAMP id_, WORD wNo_);

_DLLEXP LPSTR _ZZKDECL StampGetData(LPSTR lpsz_, int size_, WRD wrd_, WORD order_);
_DLLEXP BOOL _ZZKDECL StampSetData(LPCSTR lpsz_, WRD wrd_, WORD order_);
_DLLEXP BOOL _ZZKDECL StampGetElementName(WORD element_, LPSTR szName_, WORD wSize_);
_DLLEXP WORD _ZZKDECL StampGetElement(LPSTR szName_);

_DLLEXP SBTPROC _ZZKDECL StampGetBuildProc(IDSTAMP id_);
_DLLEXP SRTPROC _ZZKDECL StampGetPaintProc(IDSTAMP id_);
_DLLEXP PARAMPROC _ZZKDECL StampGetParamProc(IDSTAMP id_);
_DLLEXP DWORD _ZZKDECL StampCallBuildProc(SCMD	wCmd_,
								 WRD	wFTU_,
								 HWND	hwndParent_,
								 WORD	wParam_,
								 LONG	lParam_);

_DLLEXP DWORD _ZZKDECL StampCallParamProc(IDSTAMP id_, PIC wCmd_, WRD wrd_, WORD wNo_, LONG lParam_);

_DLLEXP void _ZZKDECL SetMsgCode( LPFIA lpFia_ );


#endif 	//__CSTAMP_H



