//===================================================================
//
//Zheng zhikai	95-10-4
//===================================================================

#ifndef __CMODU_H
#define __CMODU_H

/////////////////////////////
//Modu Table
typedef struct tagMODU_TABLE_UNIT
{
	LPCOBJ		lpTag;		//tag name
	LPCOBJ		lpComment;	//comment
	//public but read-only data
	IDALGO		Algo;		//algorithm id
	WORD		NumParam;	//the number of the parameters
	WRD			Parent;		//the Parent unit
	WRD			Next;		//the next unit in a modu link
	WRD			Child;
	DWORD		UserData;	//user defined data, reserved for future
	//private data
	LPPU		lpParam;	//parameter entry
	STATUS		Status;		//status word
	//dynamic data
	DIRTY		Dirty;		//dirty flags
}MTU, FAR* LPMTU;

#define MODU_ROOT			((WRD)1)	//the root node of the whole Modu Table
#define MODU_STEPROOT		((WRD)2)	//the root node of the all step modu
#define MODU_TRIGGERROOT	((WRD)3)	//the root node of the all trigger modu

typedef struct tagMODU_TABLE_INFO
{
	WRD			wCount;
	WRD			wAdding;
	HGLOBAL		hBase;
	LPMTU 		lpBase;
	long		lAccessTimes;
}MTInfo;

//public and comment data access const
#define MTU_NUMPARAM		0x0001
#define MTU_PARENT			0x0002
#define MTU_NEXT			0x0003
#define MTU_CHILD			0x0004
//private and comment data access const
#define MTU_STATUS			0x0005
#define MTU_DIRTY			0x0006
#define MTU_PARAM			0x0007
//public but diffent from face table access const
#define MTU_IDALGO			0x0008
#define MTU_TAG				0x0009
#define MTU_COMMENT			0x000A

//define Modu table status flags
#define MTUBIT_VALID			0x8000
#define MTUBIT_TEMP				0x4000
#define MTUBIT_SYSTEM			0x2000
#define MTUBIT_SKIPCHILD		0x1000
#define MTUBIT_EXPANDED			0x0002

//define DIRTY flags
#define MTUBIT_DIRTY_				(0x0001)
#define MTUBIT_DIRTY_TAG			(MTUBIT_DIRTY_ << MTU_TAG)
#define MTUBIT_DIRTY_COMMENT		(MTUBIT_DIRTY_ << MTU_COMMENT)
#define MTUBIT_DIRTY_STATUS			(MTUBIT_DIRTY_ << MTU_STATUS)
#define MTUBIT_DIRTY_IDALGO			(MTUBIT_DIRTY_ << MTU_IDALGO)
#define MTUBIT_DIRTY_NUMPARAM		(MTUBIT_DIRTY_ << MTU_NUMPARAM)
#define MTUBIT_DIRTY_PARENT			(MTUBIT_DIRTY_ << MTU_PARENT)
#define MTUBIT_DIRTY_NEXT			(MTUBIT_DIRTY_ << MTU_NEXT)
#define MTUBIT_DIRTY_CHILD			(MTUBIT_DIRTY_ << MTU_CHILD)
#define MTUBIT_DIRTY_PARAM			(MTUBIT_DIRTY_ << MTU_PARAM)
//private dirty: 
//the implementation don't ensure the following private dirty 
//bits can be set properly, these definitions only for integrity.
#define MTUBIT_DIRTY_DIRTY			(MTUBIT_DIRTY_ << MTU_DIRTY)

#define MTUBIT_DIRTY_ALL			(-1)

//define MTU Error Code
#define MTU_ERR_STATUSVALID		(-1)
#define MTU_ERR_TAGUNIQUE		(-2)
#define MTU_ERR_TAGNAMING		(-3)
#define MTU_ERR_ALGO			(-4)
#define MTU_ERR_PARENT			(-5)

//at default, parameter 0 is the result of the algorithm
#define MODU_RESULT				(0)
#define MODU_RESULT_NAME		"Result"

#define pMODU_GET_TAG( _lpModu_ )	\
	((LPSTR)( ((_lpModu_)->lpTag)? ((_lpModu_)->lpTag->data): NULL ) )

#define MODU_GET_TAG( wrd )										\
	((LPSTR)( ((gMTInfo.lpBase + wrd)->lpTag)?					\
					 ((gMTInfo.lpBase + wrd)->lpTag->data):		\
					 NULL ) )

_DLLEXP MTInfo FAR* _ZZKDECL GetModuTableInformation(void);
_DLLEXP WRD _ZZKDECL ModuCreate( LPCSTR	szTag_,
							WRD		wParent_,
							IDALGO	idAlgo_,
							LONG	lData_);

_DLLEXP BOOL _ZZKDECL ModuDelete(WRD wrd_);

//the following tree functions provide motheds for "temp modu"
_DLLEXP WRD _ZZKDECL ModuCopyTemp( WRD wrd_ );
_DLLEXP BOOL _ZZKDECL ModuDeleteTemp(WRD wrd_);

_DLLEXP int _ZZKDECL ModuGetTag(WRD wrd_, LPSTR szTag_, WORD wSize_);
_DLLEXP WRD _ZZKDECL ModuGetWRD(LPCSTR szTag_);
_DLLEXP STATUS _ZZKDECL ModuGetStatus(WRD wrd_);
_DLLEXP DIRTY _ZZKDECL ModuGetDirty(WRD wrd_);
_DLLEXP IDALGO _ZZKDECL ModuGetAlgo(WRD wrd_);
_DLLEXP WORD _ZZKDECL ModuGetNumParam(WRD wrd_);
_DLLEXP WRD _ZZKDECL ModuGetParent(WRD wrd_);
_DLLEXP WRD _ZZKDECL ModuGetChild(WRD wrd_);
_DLLEXP WRD _ZZKDECL ModuGetNext(WRD wrd_);
_DLLEXP LPPU _ZZKDECL ModuGetParameter(WRD wrd_);

_DLLEXP WRD _ZZKDECL ModuGetPrevious( WRD wrd_ );
_DLLEXP WRD _ZZKDECL ModuGetLast(WRD wrd_);

_DLLEXP WORD _ZZKDECL ModuGetElementType( WORD element_ );
_DLLEXP BOOL _ZZKDECL ModuGetElementName(WORD element_, LPSTR szName_, WORD wSize_);
_DLLEXP WORD _ZZKDECL ModuGetElement(LPSTR szName_);

//_DLLEXP WORD _ZZKDECL ModuGetPUFlag( WRD wrd_, WORD _i);

_DLLEXP BOOL _ZZKDECL ModuUndo( WRD wModu_, WRD wBuffer_ );

_DLLEXP BOOL _ZZKDECL ModuSetTag(WRD wrd_, LPCSTR szTag_);
_DLLEXP BOOL _ZZKDECL ModuChangeAlgo(WRD wrd_, IDALGO id_ );
_DLLEXP void _ZZKDECL ModuSetStatus( WRD wrd_, STATUS status_ );

_DLLEXP BOOL _ZZKDECL ModuDeleteParam( WRD wrd_, WORD wFirstDelete_, WORD wNumDelete_ );
_DLLEXP BOOL _ZZKDECL ModuDeleteParamGroup( WRD wrd_, WORD wFirstDelete_ );
_DLLEXP BOOL _ZZKDECL ModuAppendParam( WRD wrd_, WORD wNumAppend_ );
_DLLEXP BOOL _ZZKDECL ModuAppendParamGroup( WRD wrd_ );
_DLLEXP BOOL _ZZKDECL ModuInsertParam( WRD wrd_, WORD wInsertBefore_, WORD wNumInsert_ );
_DLLEXP BOOL _ZZKDECL ModuInsertParamGroup( WRD wrd_, WORD wInsertBefore_ );

_DLLEXP void _ZZKDECL ModuToggleExpand( WRD wrd_ );

_DLLEXP BOOL _ZZKDECL ModuSetPUFlagBit( WRD wrd_, WORD _i, WORD Flag_ );
_DLLEXP BOOL _ZZKDECL ModuClearPUFlagBit( WRD wrd_, WORD _i, WORD Flag_ );

_DLLEXP void _ZZKDECL ModuClearDirty( WRD wrd_ );

_DLLEXP BOOL _ZZKDECL ModuIsValid( WRD wrd_ );
_DLLEXP BOOL _ZZKDECL ModuCheckLegality(WRD wrd_);
_DLLEXP BOOL _ZZKDECL ModuCheckTag(WRD wModu_);

_DLLEXP LPSTR _ZZKDECL ModuGetData(LPSTR lpsz_, int size_, WRD wrd_, WORD order_);
_DLLEXP BOOL _ZZKDECL ModuSetData(LPCSTR lpsz_, WRD wrd_, WORD order_);

_DLLEXP BOOL _ZZKDECL IsModuTableDirty( BOOL bPublicDirty_ );
_DLLEXP WRD _ZZKDECL GetStepRoot(void);

_DLLEXP BOOL _ZZKDECL ModuIsInTree( WRD wrdRoot_, WRD wrdTest_ );
_DLLEXP BOOL _ZZKDECL ModuMove(WRD wrd_, WRD wrdParentNew_, WRD wrdPrevNew_ );

_DLLEXP BOOL _ZZKDECL PUWriteModuTable(LPPU to_, SimplePU FAR* from_);
_DLLEXP BOOL _ZZKDECL PUReadModuTable(SimplePU FAR* to_, LPPU from_);
_DLLEXP BOOL _ZZKDECL PUModuDirty( LPPU lppu_ );

_DLLEXP BOOL _ZZKDECL ModuPaste( WRD wrdParent_, WRD wrdCopy_, BOOL bTree_ );
_DLLEXP BOOL _ZZKDECL IsNewLegalModuTag( LPCSTR lpszTag_ );

_DLLEXP BOOL _ZZKDECL ModuChangeDataLink( WRD wrd_, WORD order_, LPCSTR lpszDataLink_ );
#endif	//__CMODU_H
