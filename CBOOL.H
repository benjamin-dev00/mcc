//===================================================================
//
//Zheng zhikai	95-10-4
//===================================================================

#ifndef __CBOOL_H
#define __CBOOL_H

/////////////////////////////
//Bool Table
typedef struct tagBOOL_TABLE_UNIT
{
	BOOL		Value;
	//cobjects
	LPCOBJ		lpTag;
	LPCOBJ		lpUnit;		//unit 
	LPCOBJ		lpComment;	//comment
	//other data
	DWORD		TraceTime;	//the duration of run-time logging
	DWORD		UserData;	//user defined data
	//private data
	STATUS		Status;
	//private dynamic data, which data will be clear while saving
	ALARM		Alarm;
	DIRTY		Dirty;
	LPTRACE		lpTraceData;	//point to trace data struct
	BOOL		LastLog;
}BTU, FAR* LPBTU;

#define BTU_VALUE			0x0001
#define BTU_TAG				0x0002
#define BTU_UNIT			0x0003
#define BTU_COMMENT			0x0004
#define BTU_USERDATA		0x0005
#define BTU_TRACETIME		0x000E
//private data
#define BTU_TRACEDATA		0x000F
#define BTU_ALARM			0x0010
#define BTU_STATUS			0x0011
#define BTU_DIRTY			0x0012
#define BTU_LASTLOG			0x0013

//Value
#define BTU_VALUENOT		0x0030	//do operator "not" for the value

//Status data
#define BTU_ENABLEFALSE		0x0040	//enable FALSE-ALARM
#define BTU_ENABLETRUE		0x0041	//enable TRUE-ALARM
#define BTU_TRACE			0x004C
#define BTU_LOGHIS			0x004D
#define BTU_TEMP			0x004E
#define BTU_VALID			0x004F

#define BTU_ALM				0x0080	//any alarm on
#define BTU_ALMFALSE		0x0081	//TRUE-ALARM on
#define BTU_ALMTRUE			0x0082	//FALSE-ALARM on

#define BTU_ANA				0x0090
#define BTU_ANAFALSE		0x0091	//TRUE-ALARM on without ACKed
#define BTU_ANATRUE			0x0092	//FALSE-ALARM on without ACKed

#define BTU_ACK				0x00A0
#define BTU_ACKFALSE		0x00A1	//TRUE-ALARM on without ACKed
#define BTU_ACKTRUE			0x00A2	//FALSE-ALARM on without ACKed

//access alarm types enabled
#define BTU_ENABLEALM		0x00B0

//Status:
//	0b0000-0000-0000-0000
//    ||||         |   ||
//    ||||         |   |+----	Alarm FALSE-ALARM(alarm when value is FALSE) Enable
//    ||||         |   +-----	Alarm TRUE-ALARM(alarm when value is TRUE)   Enable
//    ||||         | 
//    ||||         +---------	Alarm Swtich-Times enable( not implemented )
//    |||| 
//    |||+-------------------	Trace enable
//    ||+--------------------	Log History Enable
//    |+---------------------	TempBTU Enable
//    +----------------------	Valid bit Enable

//Alarm:
//	0b0000-0000-0000-0000
//    |            |   ||
//    |            |   |+----	Alarm FALSE-ALARM State
//    |            |   +-----	Alarm TRUE-ALARM State
//    |            +---------	Alarm Swtich-Times State( not implemented )
//    +----------------------	Ack bit( 1: ACKed, 0: not ACKed )

//the bits in BTU.Alarm
#define BTUBIT_ACK					0x8000
#define BTUBIT_ALMSTATE_FALSE		0x0001
#define BTUBIT_ALMSTATE_TRUE		0x0002
//definition for mask
#define BTUBIT_ALMSTATE_MASK		0x0003

//the bits in BTU.Status
#define BTUBIT_VALID				0x8000
#define BTUBIT_TEMP					0x4000
#define BTUBIT_LOGHIS				0x2000
#define BTUBIT_TRACE				0x1000
#define BTUBIT_DISABLE_CHECK		0x0800	//if this bit set, donot check legality when data updated
#define BTUBIT_ENABLETRUE			0x0002
#define BTUBIT_ENABLEFALSE			0x0001
//definition for mask
#define BTUBIT_ENABLEMASK			0x0003

//define DIRTY bits
#define BTUBIT_DIRTY_				0x0001
#define BTUBIT_DIRTY_TAG			(BTUBIT_DIRTY_ << BTU_TAG)
#define BTUBIT_DIRTY_STATUS			(BTUBIT_DIRTY_ << BTU_STATUS)
#define BTUBIT_DIRTY_VALUE			(BTUBIT_DIRTY_ << BTU_VALUE)
#define BTUBIT_DIRTY_UNIT			(BTUBIT_DIRTY_ << BTU_UNIT)
#define BTUBIT_DIRTY_COMMENT		(BTUBIT_DIRTY_ << BTU_COMMENT)
#define BTUBIT_DIRTY_TRACETIME		(BTUBIT_DIRTY_ << BTU_TRACETIME)
#define BTUBIT_DIRTY_USERDATA		(BTUBIT_DIRTY_ << BTU_USERDATA)
//private dirty: 
//the implementation don't ensure the following private dirty 
//bits can be set properly, these definitions only for integrity.
#define BTUBIT_DIRTY_ALARM			(BTUBIT_DIRTY_ << BTU_ALARM)
#define BTUBIT_DIRTY_LASTLOG		(BTUBIT_DIRTY_ << BTU_LASTLOG)
#define BTUBIT_DIRTY_TRACEDATA		(BTUBIT_DIRTY_ << BTU_TRACEDATA)
#define BTUBIT_DIRTY_DIRTY			(BTUBIT_DIRTY_ << BTU_DIRTY)

#define BTUBIT_DIRTY_ALL		(-1)

#define pBOOL_GET_TAG( _lpBool_ )	\
	((LPSTR)( ((_lpBool_)->lpTag)? ((_lpBool_)->lpTag->data): NULL ) )

#define BOOL_GET_TAG( wrd )										\
	((LPSTR)( ((gBTInfo.lpBase + wrd)->lpTag)?					\
					 ((gBTInfo.lpBase + wrd)->lpTag->data):		\
					 NULL ) )

typedef struct tagBOOL_TABLE_INFO
{
	WRD			wCount;
	WRD			wAdding;
	HGLOBAL		hBase;
	LPBTU 		lpBase;
	long		lAccessTimes;
}BTInfo;

_DLLEXP WRD _ZZKDECL BoolCreate(LPCSTR lpszTag_, BOOL bValue_);
_DLLEXP BOOL _ZZKDECL BoolDelete(WRD wrd_);

_DLLEXP WRD _ZZKDECL BoolCopyTemp( WRD wBTU_ );
_DLLEXP BOOL _ZZKDECL BoolDeleteTemp(WRD wBTU_);
_DLLEXP BOOL _ZZKDECL BoolUndo( WRD wBool_, WRD wBuffer_ );

_DLLEXP WRD _ZZKDECL BoolGetWRD(LPCSTR szTag_);
_DLLEXP int _ZZKDECL BoolGetTag(WRD wBTU_, LPSTR szTag_, WORD wSize_);

_DLLEXP STATUS _ZZKDECL BoolGetStatus(WRD wBTU_);
_DLLEXP STATUS _ZZKDECL BoolSetStatus( WRD wRTU_, STATUS status_ );

_DLLEXP DIRTY _ZZKDECL BoolGetDirty(WRD wBTU_);

_DLLEXP BTInfo FAR* _ZZKDECL GetBoolTableInformation(void);

_DLLEXP BOOL _ZZKDECL BoolIsValid(WRD wBTU_);
_DLLEXP BOOL _ZZKDECL BoolCheckLegality(WRD wBTU_);
_DLLEXP BOOL _ZZKDECL BoolCheckTag(WRD wrd_);

_DLLEXP WORD _ZZKDECL BoolGetElementType( WORD element_ );
_DLLEXP BOOL _ZZKDECL BoolGetElementName(WORD element_, LPSTR szName_, WORD wSize_);
_DLLEXP WORD _ZZKDECL BoolGetElement(LPSTR szName_);

_DLLEXP BOOL _ZZKDECL BoolSetTag(WRD wBTU_, LPCSTR szTag_);

_DLLEXP BOOL _ZZKDECL BoolSetData(LPCSTR lpsz_, WRD wrd_, WORD order_);
_DLLEXP LPSTR _ZZKDECL BoolGetData(LPSTR lpsz_, int size_, WRD wrd_, WORD order_);

_DLLEXP BOOL _ZZKDECL IsBoolDirty(WRD);
_DLLEXP BOOL _ZZKDECL IsBoolTableDirty( BOOL bPublicDirty_ );
_DLLEXP void _ZZKDECL BoolClearTableDirty(void);

_DLLEXP BOOL _ZZKDECL PUWriteBoolTable(LPPU to_, SimplePU FAR* from_);
_DLLEXP BOOL _ZZKDECL PUReadBoolTable(SimplePU FAR* to_, LPPU from_);
_DLLEXP BOOL _ZZKDECL PUBoolDirty( LPPU lppu_ );

#endif	//__CBOOL_H

