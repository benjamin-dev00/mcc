//===================================================================
//
//Zheng zhikai	95-10-4
//===================================================================

#ifndef __CTINT_H
#define __CTINT_H

/////////////////////////////
//Tint Table
typedef struct tagTINT_TABLE_UNIT
{
	//basic data
	TINT		Value;
	TINT		Min;
	TINT		Max;
	//Alarm thresholds
	TINT		HiHi;		
	TINT		Hi;
	TINT		Lo;
	TINT		LoLo;
	//cobjects
	LPCOBJ		lpTag;
	LPCOBJ		lpUnit;		//unit 
	LPCOBJ		lpComment;	//comment
	//other data
	TINT		LogBand;	//the band for log
	TINT		AlarmBand;	//the band for alarm
	DWORD		TraceTime;	//the duration of trace time in ms
	DWORD		UserData;	//user defined data, reserved for future
	//private data 
	STATUS		Status;
	//private dynamic data, which data will be clear while saving
	ALARM		Alarm;			//alarm states
	DIRTY		Dirty;			//dirty states
	LPTRACE		lpTraceData;	//point to run-time log data struct
	TINT		LastLog;		//the last loged value
}TTU, FAR* LPTTU;

#define TTU_VALUE		0x0001
#define TTU_TAG			0x0002
#define TTU_UNIT		0x0003
#define TTU_COMMENT		0x0004
#define TTU_USERDATA	0x0005
#define TTU_MIN			0x0006
#define TTU_MAX			0x0007
#define TTU_HIHI		0x0008
#define TTU_HI			0x0009
#define TTU_LO			0x000A
#define TTU_LOLO		0x000B
#define TTU_LOGBAND		0x000C
#define TTU_ALARMBAND	0x000D
#define TTU_TRACETIME	0x000E
//private data
#define TTU_TRACEDATA	0x000F
#define TTU_ALARM		0x0010
#define TTU_STATUS		0x0011
#define TTU_DIRTY		0x0012
#define TTU_LASTLOG		0x0013

//Status data
#define TTU_ENABLELOLO	0x0040
#define TTU_ENABLELO	0x0041
#define TTU_ENABLEHI	0x0042
#define TTU_ENABLEHIHI	0x0043
#define TTU_TRACE		0x004C
#define TTU_LOGHIS		0x004D
#define TTU_TEMP		0x004E
#define TTU_VALID		0x004F

//data accessing defines:
#define TTU_SPAN			0x0071	//read-only

//alarm on: ALM_xxx; data type: BOOL
#define TTU_ALM			0x0080	//any alarm on
#define TTU_ALMHIHI		0x0081	//HiHi on
#define TTU_ALMHI		0x0082	//Hi on
#define TTU_ALMLO		0x0083	//Lo on
#define TTU_ALMLOLO		0x0084	//LoLo on

//data accessing defines: Alarm without(No) Ack: ANA_xxx
//data type: BOOL
#define TTU_ANA			0x0090	//if alarm without ACKed
#define TTU_ANAHIHI		0x0091	//HiHi on without ACKed
#define TTU_ANAHI		0x0092	//Hi on without ACKed
#define TTU_ANALO		0x0093	//Lo on without ACKed
#define TTU_ANALOLO		0x0094	//LoLo on without ACKed

#define TTU_ACK			0x00A0	//if ACK bit set(when read) / or set ACK (when write)
#define TTU_ACKHIHI		0x00A1	//if HIHI on and ACKed
#define TTU_ACKHI		0x00A2	//if HI on and ACKed
#define TTU_ACKLO		0x00A3	//if LO on and ACKed
#define TTU_ACKLOLO		0x00A4	//if LOLO on and ACKed
//access alarm types enabled
#define TTU_ENABLEALM	0x00B0	//if any alarm is enabled
//the detail bit defined in STATUS group

//Status bit definitions
#define TTUBIT_VALID				0x8000	//validity
#define TTUBIT_TEMP					0x4000
#define TTUBIT_LOGHIS				0x2000
#define TTUBIT_TRACE				0x1000
#define TTUBIT_DISABLE_CHECK		0x0800	//if this bit set, donot check legality when data updated
#define TTUBIT_ENABLEHIHI			0x0008
#define TTUBIT_ENABLEHI				0x0004
#define TTUBIT_ENABLELO				0x0002
#define TTUBIT_ENABLELOLO			0x0001
#define TTUBIT_ENABLEMASK			0x000F

//Status:
//	0b0000-0000-0000-0000
//    |||| |      || ||||
//    |||| |      || |||+----	Alarm LoLo Enable
//    |||| |      || ||+-----	Alarm Lo   Enable
//    |||| |      || |+------	Alarm Hi   Enable
//    |||| |      || +-------	Alarm HiHi Enable
//    |||| |      |+---------	Alarm Speed-Low-Limit Enable( not implemented )
//    |||| |      +----------	Alarm Speed-Upper-Limit Enable( not implemented )
//    |||| +-----------------	Disable Check Enable(for:Min-Max, Alarm-Threshold check only)
//    |||+-------------------	Trace Enable
//    ||+--------------------	Log History Enable
//    |+---------------------	TempTTU Enable
//    +----------------------	Valid bit Enable

//Alarm:
//	0b0000-0000-0000-0000
//    | ||        || ||||
//    | ||        || |||+----	Alarm LoLo State
//    | ||        || ||+-----	Alarm Lo   State
//    | ||        || |+------	Alarm Hi   State
//    | ||        || +-------	Alarm HiHi State 
//    | ||        |+---------	Alarm Speed-Low-Limit State( not implemented )
//    | ||        +----------	Alarm Speed-Upper-Limit State( not implemented )
//    | |+-------------------	Less-Than-Min Sign( not implemented )
//    | +--------------------	Great-Than-Max Sign( not implemented )
//    +----------------------	Ack bit( 1: ACKed, 0: not ACKed )

#define TTUBIT_ACK					0x8000
#define TTUBIT_ALMSTATE_HIHI		0x0008
#define TTUBIT_ALMSTATE_HI      	0x0004
#define TTUBIT_ALMSTATE_LO			0x0002
#define TTUBIT_ALMSTATE_LOLO		0x0001
#define TTUBIT_ALMSTATE_MASK		0x000F

//define DIRTY bits
#define TTUBIT_DIRTY_				0x0001
#define TTUBIT_DIRTY_STATUS			(TTUBIT_DIRTY_ << TTU_STATUS)
#define TTUBIT_DIRTY_VALUE			(TTUBIT_DIRTY_ << TTU_VALUE)
#define TTUBIT_DIRTY_MIN			(TTUBIT_DIRTY_ << TTU_MIN)
#define TTUBIT_DIRTY_MAX			(TTUBIT_DIRTY_ << TTU_MAX)
#define TTUBIT_DIRTY_LOGBAND		(TTUBIT_DIRTY_ << TTU_LOGBAND)
#define TTUBIT_DIRTY_HIHI			(TTUBIT_DIRTY_ << TTU_HIHI)
#define TTUBIT_DIRTY_HI				(TTUBIT_DIRTY_ << TTU_HI)
#define TTUBIT_DIRTY_LO				(TTUBIT_DIRTY_ << TTU_LO)
#define TTUBIT_DIRTY_LOLO			(TTUBIT_DIRTY_ << TTU_LOLO)
#define TTUBIT_DIRTY_ALARMBAND		(TTUBIT_DIRTY_ << TTU_ALARMBAND)
#define TTUBIT_DIRTY_TAG			(TTUBIT_DIRTY_ << TTU_TAG)
#define TTUBIT_DIRTY_UNIT			(TTUBIT_DIRTY_ << TTU_UNIT)
#define TTUBIT_DIRTY_COMMENT		(TTUBIT_DIRTY_ << TTU_COMMENT)
#define TTUBIT_DIRTY_TRACETIME		(TTUBIT_DIRTY_ << TTU_TRACETIME)
#define TTUBIT_DIRTY_USERDATA		(TTUBIT_DIRTY_ << TTU_USERDATA)
//private dirty: 
//the implementation don't ensure the following private dirty 
//bits can be set properly, these definitions only for integrity.
#define TTUBIT_DIRTY_ALARM			(TTUBIT_DIRTY_ << TTU_ALARM)
#define TTUBIT_DIRTY_LASTLOG		(TTUBIT_DIRTY_ << TTU_LASTLOG)
#define TTUBIT_DIRTY_TRACEDATA		(TTUBIT_DIRTY_ << TTU_TRACEDATA)
#define TTUBIT_DIRTY_DIRTY			(TTUBIT_DIRTY_ << TTU_DIRTY)

#define TTUBIT_DIRTY_ALL			(-1)

#define pTINT_GET_TAG( _lpTint_ )	\
	((LPSTR)( ((_lpTint_)->lpTag)? ((_lpTint_)->lpTag->data): NULL ) )

#define TINT_GET_TAG( wrd )										\
	((LPSTR)( ((gTTInfo.lpBase + wrd)->lpTag)?					\
					 ((gTTInfo.lpBase + wrd)->lpTag->data):		\
					 NULL ) )

typedef struct tagTINT_TABLE_INFO
{
	WRD			wCount;
	WRD			wAdding;
	HGLOBAL		hBase;
	LPTTU 		lpBase;
	long		lAccessTimes;
}TTInfo;

_DLLEXP TTInfo FAR* _ZZKDECL GetTintTableInformation(void);

_DLLEXP WRD _ZZKDECL TintCreate(LPCSTR	lpszTag_,
						TINT	rValue_,
						TINT	rMin_,
						TINT	rMax_);
_DLLEXP BOOL _ZZKDECL TintDelete(WRD wrd_);

_DLLEXP WRD _ZZKDECL TintCopyTemp( WRD wTTU_ );
_DLLEXP BOOL _ZZKDECL TintDeleteTemp(WRD wTTU_);
_DLLEXP BOOL _ZZKDECL TintUndo( WRD wTint_, WRD wBuffer_ );

_DLLEXP WRD _ZZKDECL TintGetWRD(LPCSTR szTag_);
_DLLEXP int _ZZKDECL TintGetTag(WRD wTTU_, LPSTR szTag_, WORD wSize_);
_DLLEXP STATUS _ZZKDECL TintGetStatus(WRD wTTU_);
_DLLEXP DIRTY _ZZKDECL TintGetDirty(WRD wTTU_);

_DLLEXP WORD _ZZKDECL TintGetElementType( WORD element_ );
_DLLEXP BOOL _ZZKDECL TintGetElementName(WORD element_, LPSTR szName_, WORD wSize_);
_DLLEXP WORD _ZZKDECL TintGetElement(LPSTR szName_);

_DLLEXP BOOL _ZZKDECL TintIsValid( WRD wrd_ );
_DLLEXP BOOL _ZZKDECL TintCheckLegality(WRD wTTU_);
_DLLEXP BOOL _ZZKDECL TintCheckTag(WRD wrd_);

_DLLEXP BOOL _ZZKDECL TintSetTag(WRD wTTU_, LPCSTR szTag_);
_DLLEXP STATUS _ZZKDECL TintSetStatus( WRD wTTU_, STATUS status_ );

_DLLEXP LPSTR _ZZKDECL TintGetData(LPSTR lpsz_, int size_, WRD wrd_, WORD order_);
_DLLEXP BOOL _ZZKDECL TintSetData(LPCSTR lpsz_, WRD wrd_, WORD order_);

_DLLEXP BOOL _ZZKDECL IsTintDirty(WRD);
_DLLEXP BOOL _ZZKDECL IsTintTableDirty( BOOL bPublicDirty_ );
_DLLEXP void _ZZKDECL TintClearTableDirty(void);

_DLLEXP BOOL _ZZKDECL TintCheckMinMax(WRD wrd_);
_DLLEXP BOOL _ZZKDECL TintCheckAlarmThreshold(WRD wrd_);

_DLLEXP ALARM _ZZKDECL TintAlarm( WRD wTTU_ );
_DLLEXP BOOL _ZZKDECL TintAckAlarm( WRD wTTU_ );
_DLLEXP BOOL _ZZKDECL PUWriteTintTable(LPPU to_, SimplePU FAR* from_);
_DLLEXP BOOL _ZZKDECL PUReadTintTable(SimplePU FAR* to_, LPPU from_);
_DLLEXP BOOL _ZZKDECL PUTintDirty( LPPU lppu_ );

#endif	//__CTINT_H
