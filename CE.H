#define	TOOL_WIDTH		24
#define	TOOL_HEIGHT		24

//current cursor operation code
typedef enum _tagCursorOperation{
	CO_NOOPERATION = 0,
	CO_MOVE,
	CO_SELECT,
	CO_CONSTRUCT,
	CO_EDIT
} CCOC;

//Face Build-time Operation Environment
typedef struct _tagFoe
{
	//HWND		hwnd;
	BOOL		bCapture;
	IDSTAMP		idStamp;		//the current stamp
	IDSTAMP		idStampOld;		//the last stamp
	WRD			wSelectedFace;
	int			iNumSelected;	//number of objects be selected(0, 1, 2 and more)
								//Old name is wSelect
	CCOC		eOperation;		//current cursor operation

//Constants part
	WORD		wPrecision;     //select precision in pixels
	WORD		wSHSize;		//the size of Selecting-handle

	//WORD		eMouseOn;		//MOUSEON_BODY / MOUSEON_NULL / the Handle id.
} FOE, FAR* LPFOE;

LRESULT CALLBACK _export FacePropertyWndProc(HWND hwnd_, UINT msg_, WPARAM wParam_, LPARAM lParam_);
LRESULT CALLBACK _export FaceToolsWndProc(HWND hwnd_, UINT msg_, WPARAM wParam_, LPARAM lParam_);
LRESULT CALLBACK _export FaceBarWndProc(HWND hwnd_, UINT msg_, WPARAM wParam_, LPARAM lParam_);
LRESULT CALLBACK _export FaceColorWndProc(HWND hwnd_, UINT msg_, WPARAM wParam_, LPARAM lParam_);
LRESULT CALLBACK _export FaceWndProc(HWND hwnd_, UINT msg_, WPARAM wParam_, LPARAM lParam_);
void CMNewFaceWindow(void);
void CMOpenFaceWindow(void);
void CMCloseFaceWindow(void);
void CMDeleteFaceWindow(void);

LRESULT ManageChildFaceWindow(HWND hwnd_, UINT wEvent_);

//notify property window to change selected face
void FPW_ChangeFace(WRD wFace_);

void OutputCursor4Param(POINT xy_, POINT cxcy_);
void OutputCursor2Param(POINT xy_);

BOOL UnionSelectedChildrenFacesRect(WRD wrdFace_, LPRECT lprc_);

//define private message between frame window and face windows
#define UM_ACTIVEFACEWND		(WM_USER + 0x10)

//define private messages for FaceBar and its children
#define UM_CHANGECOLOR			(WM_USER + 0X20)
#define UM_CHANGETOOL			(WM_USER + 0X21)
#define UM_CHILDCLOSED			(WM_USER + 0X22)

void UpdateFoeSelectedFaces(HWND hwnd_);
HWND _OpenFwnd(WRD wFTU_);

