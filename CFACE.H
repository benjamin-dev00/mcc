//===================================================================
//
//Zheng zhikai  95-10-4
//===================================================================

#ifndef __CFACE_H
#define __CFACE_H

/////////////////////////////
//Face Table
typedef struct tagFACE_TABLE_UNIT
{
	//public but read-only data
	IDSTAMP     Stamp;
	WORD        NumParam;   //the number of the parameters(include parameter x, y, cx, cy)
	WRD         Parent;		//the Parent unit
	WRD         Child;
	WRD         Next;       
	//private data
	LPPU        lpParam;
	STATUS      Status;
	RECT        Rect;       //the rectangle of the face
	DIRTY       Dirty;
}FTU, FAR* LPFTU;

//public and comment data access const
#define FTU_NUMPARAM		0x0001
#define FTU_PARENT			0x0002
#define FTU_NEXT			0x0003
#define FTU_CHILD			0x0004
//private and comment data access const
#define FTU_STATUS			0x0005
#define FTU_DIRTY			0x0006
#define FTU_PARAM			0x0007
//public but diffent from face table access const
#define FTU_IDSTAMP			0x0008
#define FTU_POSITION		0x0009

//define Face table status flags
#define FTUBIT_VALID               0x8000
#define FTUBIT_TEMP                0x4000
#define FTUBIT_SYSTEM              0x2000
#define FTUBIT_WANTTICK            0x1000
#define FTUBIT_AUTOSIZECHILDREN    0x0200
#define FTUBIT_SELECT              0x0001
#define FTUBIT_EXPANDED            0x0002
#define FTUBIT_BLINK               0x0004	//not implemented yet
#define FTUBIT_REDRAW              0x0008

//define DIRTY flags
#define FTUBIT_DIRTY_           0x0001
#define FTUBIT_DIRTY_STATUS     (FTUBIT_DIRTY_ << FTU_STATUS)
#define FTUBIT_DIRTY_IDSTAMP    (FTUBIT_DIRTY_ << FTU_IDSTAMP)
#define FTUBIT_DIRTY_NUMPARAM   (FTUBIT_DIRTY_ << FTU_NUMPARAM)
#define FTUBIT_DIRTY_PARENT     (FTUBIT_DIRTY_ << FTU_PARENT)
#define FTUBIT_DIRTY_NEXT       (FTUBIT_DIRTY_ << FTU_NEXT)
#define FTUBIT_DIRTY_CHILD      (FTUBIT_DIRTY_ << FTU_CHILD)
#define FTUBIT_DIRTY_PARAM      (FTUBIT_DIRTY_ << FTU_PARAM)
//private dirty: 
//the implementation don't ensure the following private dirty 
//bits can be set properly, these definitions only for integrity.
#define FTUBIT_DIRTY_POSITION   (FTUBIT_DIRTY_ << FTU_POSITION)
#define FTUBIT_DIRTY_DIRTY		(FTUBIT_DIRTY_ << FTU_DIRTY)

#define FTUBIT_DIRTY_ALL        (-1)

//the root node of the whole Face Table
#define FACE_ROOT           ((WRD)1)

//in all faces, the first 4 parameters are x, y, cx and cy
#define FACE_X              (0)
#define FACE_Y              (1)
#define FACE_CX             (2)
#define FACE_CY             (3)
#define FACE_VISIBLE		(4)
#define FACE_ACTIVE			(5)
#define FACE_TRIGGER		(6)

//then the order of the next, i.e., the first user defined parameter
#define FACE_FIRSTPARAM     (7)

#define FACE_X_PDT          PDT_FCCRD
#define FACE_Y_PDT          PDT_FCCRD
#define FACE_CX_PDT         PDT_FCCRD
#define FACE_CY_PDT         PDT_FCCRD
#define FACE_VISIBLE_PDT	PDT_BOOL
#define FACE_ACTIVE_PDT		PDT_int
#define FACE_TRIGGER_PDT	PDT_WRDMTU

//face actions at run-time
#define FA_DISABLE			0	//disable run-time action
#define FA_STANDARD			1	//stamp-defined action
#define FA_DEFAULT			2	//system default action

typedef struct tagFACE_TABLE_INFO
{
	WRD         wCount;
	WRD         wAdding;
	HGLOBAL     hBase;
	LPFTU       lpBase;
	long        lAccessTimes;
}FTInfo;


//define FTU Error Code
#define FTU_ERR_STATUSVALID     -1
#define FTU_ERR_STAMP           -2
#define FTU_ERR_PARENT          -3

_DLLEXP FTInfo FAR* _ZZKDECL GetFaceTableInformation(void);

_DLLEXP WRD _ZZKDECL FaceCreate(WRD     wParent_,
								IDSTAMP idStamp_,
								LONG    lData_);

_DLLEXP BOOL _ZZKDECL FaceDelete(WRD wFTU_);

_DLLEXP WRD _ZZKDECL FaceGetWRD(LPCSTR szTag_);

_DLLEXP STATUS _ZZKDECL FaceGetStatus(WRD wFTU_ );
_DLLEXP DIRTY _ZZKDECL FaceGetDirty(WRD wFTU_ );
_DLLEXP LPRECT _ZZKDECL FaceGetRect(WRD wFTU_, LPRECT lpRect_);
_DLLEXP IDSTAMP _ZZKDECL FaceGetStamp(WRD wFTU_);
_DLLEXP WORD _ZZKDECL FaceGetNumParam(WRD wFTU_);
_DLLEXP WRD _ZZKDECL FaceGetParent(WRD wFTU_);
_DLLEXP WRD _ZZKDECL FaceGetChild(WRD wFTU_);
_DLLEXP WRD _ZZKDECL FaceGetNext(WRD wFTU_);
_DLLEXP LPPU _ZZKDECL FaceGetParameter(WRD wFTU_);

_DLLEXP BOOL _ZZKDECL FaceIsValid( WRD wrd_ );
_DLLEXP BOOL _ZZKDECL FaceCheckLegality(WRD wFTU_);
_DLLEXP WRD _ZZKDECL FaceGetPrevious(WRD wFTU_);
_DLLEXP WRD _ZZKDECL FaceGetLast(WRD wFTU_);

_DLLEXP BOOL _ZZKDECL FaceExchangeTreeNode(WRD wFTU1_, WRD wFTU2_);
_DLLEXP BOOL _ZZKDECL FaceStoreAsLastChild(WRD wFTU_);
_DLLEXP BOOL _ZZKDECL FaceStoreAsFirstChild(WRD wFTU_);
_DLLEXP BOOL _ZZKDECL FaceInsertTree( WRD wrdParent_, WRD wrdInsert_ );

//Functions Operating On Face Status Bit
_DLLEXP BOOL _ZZKDECL IsFaceSelect(WRD wFTU_);
_DLLEXP void _ZZKDECL FaceSetSelect(WRD wFTU_);
_DLLEXP void _ZZKDECL FaceClearSelect(WRD wFTU_);
_DLLEXP BOOL _ZZKDECL FaceToggleSelect( WRD wFTU_ );

//the following bits are returns by FaceDirty(WRD wFTU_);
#define HEAD_DIRTY          0x0001  //the head of FTU is dirty
#define POSITION_DIRTY      0x0002  //the postion of FTU is dirty
#define NORMAL_DIRTY        0x0004  //others parameters are dirty
#define NO_DIRTY            0x0000  //no dirty
#define DEEP_DIRTY          (HEAD_DIRTY|POSITION_DIRTY)
//for FWND only
#define BACKCLR_DIRTY       0x0010

_DLLEXP WORD _ZZKDECL FaceDirty(WRD wFTU_);

_DLLEXP void _ZZKDECL FaceClearDirty(WRD wFTU_);

_DLLEXP void _ZZKDECL FaceSetStatus(WRD wFTU_, STATUS status_ );

_DLLEXP BOOL _ZZKDECL IsFaceBlink(WRD wFTU_);
_DLLEXP void _ZZKDECL FaceSetBlink(WRD wFTU_);
_DLLEXP void _ZZKDECL FaceClearBlink(WRD wFTU_);

_DLLEXP BOOL _ZZKDECL IsFaceRedraw( WRD wFTU_ );
_DLLEXP void _ZZKDECL FaceSetRedraw( WRD wFTU_ );
_DLLEXP void _ZZKDECL FaceClearRedraw( WRD wFTU_ );

_DLLEXP void _ZZKDECL FaceToggleExpand(WRD wFTU_);

_DLLEXP void _ZZKDECL FaceSetAutoSizeChildren( WRD wFTU_ );
//end Functions Operating On Face Status Bit

_DLLEXP void _ZZKDECL FaceSetRect( WRD wFTU_, LPRECT lpRect_);

_DLLEXP BOOL _ZZKDECL FaceDeleteParam( WRD wFTU_, WORD wFirstDelete_, WORD wNumDelete_ );
_DLLEXP BOOL _ZZKDECL FaceDeleteParamGroup( WRD wFTU_, WORD wFirstDelete_ );
_DLLEXP BOOL _ZZKDECL FaceAppendParam( WRD wFTU_, WORD wNumAppend_ );
_DLLEXP BOOL _ZZKDECL FaceAppendParamGroup( WRD wFTU_ );
_DLLEXP BOOL _ZZKDECL FaceInsertParam( WRD wFTU_, WORD wInsertBefore_, WORD wNumInsert_ );
_DLLEXP BOOL _ZZKDECL FaceInsertParamGroup( WRD wFTU_, WORD wInsertBefore_ );
_DLLEXP BOOL _ZZKDECL FaceChangeStamp(WRD wFTU_, IDSTAMP id_);
_DLLEXP BOOL _ZZKDECL FaceChangeParameter(WRD wFTU_, WORD wNumParam_, LPPU lpPU_);

_DLLEXP BOOL _ZZKDECL FaceUpdateParameter( WRD wFace_, WRD wBuffer_ );

_DLLEXP WORD _ZZKDECL FaceGetElementType( WORD element_ );
_DLLEXP BOOL _ZZKDECL FaceGetElementName(WORD element_, LPSTR szName_, WORD wSize_);
_DLLEXP WORD _ZZKDECL FaceGetElement(LPSTR szName_);

_DLLEXP LPSTR _ZZKDECL FaceGetData(LPSTR lpsz_, int size_, WRD wrd_, WORD order_);
_DLLEXP BOOL _ZZKDECL FaceSetData(LPCSTR lpsz_, WRD wrd_, WORD order_);

_DLLEXP BOOL _ZZKDECL FacePaint(WRD wFTU_, HDC hdc_);

_DLLEXP WORD _ZZKDECL SetSelectHandleSize(WORD wSHSize_);
_DLLEXP WORD _ZZKDECL GetSelectHandleSize(void);

_DLLEXP BOOL _ZZKDECL IsFaceVisible( WRD wFace_ );

_DLLEXP BOOL _ZZKDECL IsFaceTableDirty( BOOL bPublicDirty_ );

_DLLEXP BOOL _ZZKDECL FaceUndo( WRD wFace_, WRD wBuffer_ );
_DLLEXP int _ZZKDECL FaceGetTag( WRD wFTU_, LPSTR lpszBuffer_, WORD wSize_ );

_DLLEXP BOOL _ZZKDECL FaceDeleteTemp(WRD wFTU_);
_DLLEXP WRD _ZZKDECL FaceCopyTemp( WRD wFTU_ );

//_DLLEXP BOOL _ZZKDECL FaceClearPUFlagBit( WRD wFTU_, WORD  wOrder_, BYTE bFlag_ );
//_DLLEXP BOOL _ZZKDECL FaceSetPUFlagBit( WRD wFTU_, WORD wOrder_, BYTE bFlag_ );
//_DLLEXP BOOL _ZZKDECL IsFaceHasInvalidPU( WRD wrd_ );
_DLLEXP BOOL _ZZKDECL PUWriteFaceTable(LPPU to_, SimplePU FAR* from_);
_DLLEXP BOOL _ZZKDECL PUReadFaceTable(SimplePU FAR* to_, LPPU from_);
_DLLEXP BOOL _ZZKDECL PUFaceDirty( LPPU lppu_ );

_DLLEXP BOOL _ZZKDECL FaceChangeDataLink( WRD wrd_, WORD order_, LPCSTR lpszDataLink_ );

_DLLEXP BOOL _ZZKDECL FaceMove(WRD wrd_, WRD wrdParentNew_, WRD wrdPrevNew_ );
_DLLEXP BOOL _ZZKDECL FaceIsInTree( WRD wrdRoot_, WRD wrdTest_ );
_DLLEXP BOOL _ZZKDECL FacePaste( WRD wrdParent_, WRD wrdCopy_, BOOL bTree_ );

#endif  //__CFACE_H
