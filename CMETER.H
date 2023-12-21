//===================================================================
//
//Zheng zhikai	95-10-4
//===================================================================

#ifndef __CMETER_H
#define __CMETER_H

/////////////////////////////
//Meter Table
typedef struct tagMETER_TABLE_UNIT
{
	LPCOBJ		lpTag;		//tag name
	STATUS		Status;		//status word
	WORD		Dirty;		//dirty flags
	IDALGO		Algo;		//algorithm id
	WORD		NumParam;	//the number of the parameters
	WRD			Parent;		//the Parent unit
	WRD			Next;		//the next unit in a meter link
	WRD			Child;
	LPPU		lpParam;	//parameter entry
}MTU, FAR* LPMTU;


#define METER_ROOT			1	//the root node of the whole Meter Table
#define METER_STEPROOT		2	//the root node of the all step meter
#define METER_TRIGGERROOT	3	//the root node of the all trigger meter

typedef struct tagMETER_TABLE_INFO
{
	WRD			wCount;
	WRD			wAdding;
	HGLOBAL		hBase;
	LPMTU 		lpBase;
	long		lAccessTimes;
}MTInfo;

#define MT_IDALGO		0x0001
#define MT_STATUS		0x0002
#define MT_DIRTY		0x0003
#define MT_TAG			0x0004
#define MT_PARAM		0x0005
#define MT_PARENT		0x0006
#define MT_NEXT			0x0007
#define MT_CHILD		0x0008
#define MT_NUMPARAM		0x0009

//define Meter table status flags
#define MTU_VALID				0x8000
#define MTU_TEMP				0x4000
#define MTU_SYSTEM				0x2000
#define MTU_SKIPCHILD			0x1000
#define MTU_EXPANDED			0x0002

//define RUNDIRTY flags
#define MTU_RUNDIRTY_			(0x0001)
#define MTU_RUNDIRTY_TAG		(MTU_RUNDIRTY_ << MT_TAG)
#define MTU_RUNDIRTY_STATUS		(MTU_RUNDIRTY_ << MT_STATUS)
//#define MTU_RUNDIRTY_DIRTY		(MTU_RUNDIRTY_ << MT_DIRTY)
#define MTU_RUNDIRTY_IDALGO		(MTU_RUNDIRTY_ << MT_IDALGO)
#define MTU_RUNDIRTY_NUMPARAM	(MTU_RUNDIRTY_ << MT_NUMPARAM)
#define MTU_RUNDIRTY_PARENT		(MTU_RUNDIRTY_ << MT_PARENT)
#define MTU_RUNDIRTY_NEXT		(MTU_RUNDIRTY_ << MT_NEXT)
#define MTU_RUNDIRTY_CHILD		(MTU_RUNDIRTY_ << MT_CHILD)
#define MTU_RUNDIRTY_PARAM		(MTU_RUNDIRTY_ << MT_PARAM)

#define MTU_RUNDIRTY_ALL		(MTU_RUNDIRTY_TAG			\
								|MTU_RUNDIRTY_STATUS		\
								|MTU_RUNDIRTY_IDALGO		\
								|MTU_RUNDIRTY_NUMPARAM		\
								|MTU_RUNDIRTY_PARENT		\
								|MTU_RUNDIRTY_NEXT			\
								|MTU_RUNDIRTY_CHILD			\
								|MTU_RUNDIRTY_PARAM)

//define DIRTY flags
#define MTU_DIRTY				0x0800
#define MTU_DIRTY_TAG			(MTU_DIRTY | MTU_RUNDIRTY_TAG)
#define MTU_DIRTY_STATUS		(MTU_DIRTY | MTU_RUNDIRTY_STATUS)
//#define MTU_DIRTY_DIRTY			(MTU_DIRTY | MTU_RUNDIRTY_DIRTY)
#define MTU_DIRTY_IDALGO		(MTU_DIRTY | MTU_RUNDIRTY_IDALGO)
#define MTU_DIRTY_NUMPARAM		(MTU_DIRTY | MTU_RUNDIRTY_NUMPARAM)
#define MTU_DIRTY_PARENT		(MTU_DIRTY | MTU_RUNDIRTY_PARENT)
#define MTU_DIRTY_NEXT			(MTU_DIRTY | MTU_RUNDIRTY_NEXT)
#define MTU_DIRTY_CHILD			(MTU_DIRTY | MTU_RUNDIRTY_CHILD)
#define MTU_DIRTY_PARAM			(MTU_DIRTY | MTU_RUNDIRTY_PARAM)

#define MTU_DIRTY_ALL			(MTU_DIRTY | MTU_RUNDIRTY_ALL )

//define MTU Error Code
#define MTU_ERR_STATUSVALID		-1
#define MTU_ERR_TAGUNIQUE		-2
#define MTU_ERR_TAGNAMING		-3
#define MTU_ERR_ALGO			-4
#define MTU_ERR_PARENT			-5

//at default, parameter 0 is the result of the algorithm
#define METER_RESULT			0

#define pMETER_GET_TAG( _lpMeter_ )	\
	((LPSTR)( ((_lpMeter_)->lpTag)? ((_lpMeter_)->lpTag->data): NULL ) )

#define METER_GET_TAG( wrd )										\
	((LPSTR)( ((gMTInfo.lpBase + wrd)->lpTag)?					\
					 ((gMTInfo.lpBase + wrd)->lpTag->data):		\
					 NULL ) )

_DLLEXP MTInfo FAR* _ZZKDECL GetMeterTableInformation(void);
_DLLEXP WRD _ZZKDECL MeterCreate( LPCSTR	szTag_,
							WRD		wParent_,
							IDALGO	idAlgo_,
							LONG	lData_);

_DLLEXP BOOL _ZZKDECL MeterDelete(WRD wMTU_);

//the following tree functions provide motheds for "temp meter"
_DLLEXP WRD _ZZKDECL MeterCopyTemp( WRD wMTU_ );
_DLLEXP BOOL _ZZKDECL MeterDeleteTemp(WRD wMTU_);

_DLLEXP int _ZZKDECL MeterGetTag(WRD wMTU_, LPSTR szTag_, WORD wSize_);
_DLLEXP WRD _ZZKDECL MeterGetWRD(LPCSTR szTag_);
_DLLEXP STATUS _ZZKDECL MeterGetStatus(WRD wMTU_);
_DLLEXP WORD _ZZKDECL MeterGetDirty(WRD wMTU_);
_DLLEXP IDALGO _ZZKDECL MeterGetAlgo(WRD wMTU_);
_DLLEXP WORD _ZZKDECL MeterGetNumParam(WRD wMTU_);
_DLLEXP WRD _ZZKDECL MeterGetParent(WRD wMTU_);
_DLLEXP WRD _ZZKDECL MeterGetChild(WRD wMTU_);
_DLLEXP WRD _ZZKDECL MeterGetNext(WRD wMTU_);
_DLLEXP LPPU _ZZKDECL MeterGetParameter(WRD wMTU_);

_DLLEXP WRD _ZZKDECL MeterGetPrevious( WRD wMTU_ );
_DLLEXP WRD _ZZKDECL MeterGetLast(WRD wMTU_);

_DLLEXP WORD _ZZKDECL MeterGetElementType( WORD element_ );
_DLLEXP BOOL _ZZKDECL MeterGetElementName(WORD element_, LPSTR szName_, WORD wSize_);
_DLLEXP WORD _ZZKDECL MeterGetElement(LPSTR szName_);

_DLLEXP BYTE _ZZKDECL MeterGetPUFlag( WRD wMTU_, WORD _i);

_DLLEXP BOOL _ZZKDECL MeterUndo( WRD wMeter_, WRD wBuffer_ );

_DLLEXP BOOL _ZZKDECL MeterSetTag(WRD wMTU_, LPSTR szTag_);
_DLLEXP BOOL _ZZKDECL MeterChangeAlgo(WRD wMTU_, IDALGO id_ );
_DLLEXP void _ZZKDECL MeterSetStatus( WRD wMTU_, STATUS status_ );

_DLLEXP BOOL _ZZKDECL MeterDeleteParam( WRD wMTU_, WORD wFirstDelete_, WORD wNumDelete_ );
_DLLEXP BOOL _ZZKDECL MeterDeleteParamGroup( WRD wMTU_, WORD wFirstDelete_ );
_DLLEXP BOOL _ZZKDECL MeterAppendParam( WRD wMTU_, WORD wNumAppend_ );
_DLLEXP BOOL _ZZKDECL MeterAppendParamGroup( WRD wMTU_ );
_DLLEXP BOOL _ZZKDECL MeterInsertParam( WRD wMTU_, WORD wInsertBefore_, WORD wNumInsert_ );
_DLLEXP BOOL _ZZKDECL MeterInsertParamGroup( WRD wMTU_, WORD wInsertBefore_ );

_DLLEXP void _ZZKDECL MeterToggleExpand( WRD wMTU_ );

_DLLEXP BOOL _ZZKDECL MeterSetPUFlagBit( WRD wMTU_, WORD _i, BYTE bFlag_ );
_DLLEXP BOOL _ZZKDECL MeterClearPUFlagBit( WRD wMTU_, WORD _i, BYTE bFlag_ );

_DLLEXP void _ZZKDECL MeterClearDirty( WRD wMTU_ );

_DLLEXP BOOL _ZZKDECL MeterIsValid( WRD wrd_ );
_DLLEXP BOOL _ZZKDECL MeterCheckLegality(WRD wMTU_);
_DLLEXP BOOL _ZZKDECL MeterCheckTag(WRD wMeter_);

_DLLEXP LPSTR _ZZKDECL MeterGetData(LPSTR lpsz_, int size_, WRD wrd_, WORD order_);
_DLLEXP BOOL _ZZKDECL MeterSetData(LPSTR lpsz_, WRD wrd_, WORD order_);

_DLLEXP BOOL _ZZKDECL IsMeterTableDirty(void);
_DLLEXP WRD _ZZKDECL GetStepRoot(void);

_DLLEXP BOOL _ZZKDECL MeterMove(WRD wrd_, WRD wrdParentNew_, WRD wrdPrevNew_ );

#endif	//__CMETER_H

