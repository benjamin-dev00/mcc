//===================================================================
//
//Zheng zhikai	95-10-4
//===================================================================

#ifndef __CREAL_H
#define __CREAL_H

/////////////////////////////
//Real Table
typedef struct tagREAL_TABLE_UNIT
{
	//basic data
	REAL		Value;
	REAL		Min;
	REAL		Max;
	//Alarm thresholds
	REAL		HiHi;		
	REAL		Hi;
	REAL		Lo;
	REAL		LoLo;
	//cobjects
	LPCOBJ		lpTag;
	LPCOBJ		lpUnit; 	//unit 
	LPCOBJ		lpComment;	//comment
	//other data
	REAL		LogBand;	//the band for log
	REAL		AlarmBand;	//the band for alarm
	DWORD		TraceTime;	//the duration of trace time in ms
	DWORD		UserData;	//user defined data, reserved for future
	//private data 
	STATUS		Status;
	//private dynamic data, which data will be clear while saving
	ALARM		Alarm;			//alarm states
	DIRTY		Dirty;			//dirty states
	LPTRACE 	lpTraceData;	//point to run-time log data struct
	REAL		LastLog;		//the last loged value
}RTU, FAR* LPRTU;

#define RTU_VALUE		0x0001
#define RTU_TAG 		0x0002
#define RTU_UNIT		0x0003
#define RTU_COMMENT 	0x0004
#define RTU_USERDATA	0x0005
#define RTU_MIN 		0x0006
#define RTU_MAX 		0x0007
#define RTU_HIHI		0x0008
#define RTU_HI			0x0009
#define RTU_LO			0x000A
#define RTU_LOLO		0x000B
#define RTU_LOGBAND 	0x000C
#define RTU_ALARMBAND	0x000D
#define RTU_TRACETIME	0x000E
//private data
#define RTU_TRACEDATA	0x000F
#define RTU_ALARM		0x0010
#define RTU_STATUS		0x0011
#define RTU_DIRTY		0x0012
#define RTU_LASTLOG 	0x0013

//Value
#define RTU_VALUE_PERCENT		0x0030	//percentage value

//Status data
#define RTU_ENABLELOLO	0x0040
#define RTU_ENABLELO	0x0041
#define RTU_ENABLEHI	0x0042
#define RTU_ENABLEHIHI	0x0043
#define RTU_TRACE		0x004C
#define RTU_LOGHIS		0x004D
#define RTU_TEMP		0x004E
#define RTU_VALID		0x004F

//data accessing defines:
#define RTU_SPAN			0x0071	//read-only

//alarm on: ALM_xxx; data type: BOOL
#define RTU_ALM 		0x0080	//any alarm on
#define RTU_ALMHIHI 	0x0081	//HiHi on
#define RTU_ALMHI		0x0082	//Hi on
#define RTU_ALMLO		0x0083	//Lo on
#define RTU_ALMLOLO 	0x0084	//LoLo on

//data accessing defines: Alarm without(No) Ack: ANA_xxx
//data type: BOOL
#define RTU_ANA 		0x0090	//if alarm without ACKed
#define RTU_ANAHIHI 	0x0091	//HiHi on without ACKed
#define RTU_ANAHI		0x0092	//Hi on without ACKed
#define RTU_ANALO		0x0093	//Lo on without ACKed
#define RTU_ANALOLO 	0x0094	//LoLo on without ACKed

#define RTU_ACK 		0x00A0	//if ACK bit set(when read) / or set ACK (when write)
#define RTU_ACKHIHI 	0x00A1	//if HIHI on and ACKed
#define RTU_ACKHI		0x00A2	//if HI on and ACKed
#define RTU_ACKLO		0x00A3	//if LO on and ACKed
#define RTU_ACKLOLO 	0x00A4	//if LOLO on and ACKed
//access alarm types enabled
#define RTU_ENABLEALM	0x00B0	//if any alarm is enabled
//the detail bit defined in STATUS group

//Status bit definitions
#define RTUBIT_VALID				0x8000	//validity
#define RTUBIT_TEMP 				0x4000
#define RTUBIT_LOGHIS				0x2000
#define RTUBIT_TRACE				0x1000
#define RTUBIT_DISABLE_CHECK		0x0800	//if this bit set, donot check legality when data updated
#define RTUBIT_ENABLEHIHI			0x0008
#define RTUBIT_ENABLEHI 			0x0004
#define RTUBIT_ENABLELO 			0x0002
#define RTUBIT_ENABLELOLO			0x0001
#define RTUBIT_ENABLEMASK			0x000F

//Status:
//	0b0000-0000-0000-0000
//	  |||| |	  || ||||
//	  |||| |	  || |||+----	Alarm LoLo Enable
//	  |||| |	  || ||+-----	Alarm Lo   Enable
//	  |||| |	  || |+------	Alarm Hi   Enable
//	  |||| |	  || +-------	Alarm HiHi Enable
//	  |||| |	  |+---------	Alarm Speed-Low-Limit Enable( not implemented )
//	  |||| |	  +----------	Alarm Speed-Upper-Limit Enable( not implemented )
//	  |||| +-----------------	Disable Check Enable(for:Min-Max, Alarm-Threshold check only)
//	  |||+-------------------	Trace Enable
//	  ||+--------------------	Log History Enable
//	  |+---------------------	TempRTU Enable
//	  +----------------------	Valid bit Enable

//Alarm:
//	0b0000-0000-0000-0000
//	  | ||		  || ||||
//	  | ||		  || |||+----	Alarm LoLo State
//	  | ||		  || ||+-----	Alarm Lo   State
//	  | ||		  || |+------	Alarm Hi   State
//	  | ||		  || +-------	Alarm HiHi State 
//	  | ||		  |+---------	Alarm Speed-Low-Limit State( not implemented )
//	  | ||		  +----------	Alarm Speed-Upper-Limit State( not implemented )
//	  | |+-------------------	Less-Than-Min Sign( not implemented )
//	  | +--------------------	Great-Than-Max Sign( not implemented )
//	  +----------------------	Ack bit( 1: ACKed, 0: not ACKed )

#define RTUBIT_ACK					0x8000
#define RTUBIT_ALMSTATE_HIHI		0x0008
#define RTUBIT_ALMSTATE_HI			0x0004
#define RTUBIT_ALMSTATE_LO			0x0002
#define RTUBIT_ALMSTATE_LOLO		0x0001
#define RTUBIT_ALMSTATE_MASK		0x000F

//define DIRTY bits
#define RTUBIT_DIRTY_				0x0001
#define RTUBIT_DIRTY_STATUS 		(RTUBIT_DIRTY_ << RTU_STATUS)
#define RTUBIT_DIRTY_VALUE			(RTUBIT_DIRTY_ << RTU_VALUE)
#define RTUBIT_DIRTY_MIN			(RTUBIT_DIRTY_ << RTU_MIN)
#define RTUBIT_DIRTY_MAX			(RTUBIT_DIRTY_ << RTU_MAX)
#define RTUBIT_DIRTY_LOGBAND		(RTUBIT_DIRTY_ << RTU_LOGBAND)
#define RTUBIT_DIRTY_HIHI			(RTUBIT_DIRTY_ << RTU_HIHI)
#define RTUBIT_DIRTY_HI 			(RTUBIT_DIRTY_ << RTU_HI)
#define RTUBIT_DIRTY_LO 			(RTUBIT_DIRTY_ << RTU_LO)
#define RTUBIT_DIRTY_LOLO			(RTUBIT_DIRTY_ << RTU_LOLO)
#define RTUBIT_DIRTY_ALARMBAND		(RTUBIT_DIRTY_ << RTU_ALARMBAND)
#define RTUBIT_DIRTY_TAG			(RTUBIT_DIRTY_ << RTU_TAG)
#define RTUBIT_DIRTY_UNIT			(RTUBIT_DIRTY_ << RTU_UNIT)
#define RTUBIT_DIRTY_COMMENT		(RTUBIT_DIRTY_ << RTU_COMMENT)
#define RTUBIT_DIRTY_TRACETIME		(RTUBIT_DIRTY_ << RTU_TRACETIME)
#define RTUBIT_DIRTY_USERDATA		(RTUBIT_DIRTY_ << RTU_USERDATA)
//private dirty: 
//the implementation don't ensure the following private dirty 
//bits can be set properly, these definitions only for integrity.
#define RTUBIT_DIRTY_ALARM			(RTUBIT_DIRTY_ << RTU_ALARM)
#define RTUBIT_DIRTY_LASTLOG		(RTUBIT_DIRTY_ << RTU_LASTLOG)
#define RTUBIT_DIRTY_TRACEDATA		(RTUBIT_DIRTY_ << RTU_TRACEDATA)
#define RTUBIT_DIRTY_DIRTY			(RTUBIT_DIRTY_ << RTU_DIRTY)

#define RTUBIT_DIRTY_ALL			(-1)

#define pREAL_GET_TAG( _lpReal_ )	\
	((LPSTR)( ((_lpReal_)->lpTag)? ((_lpReal_)->lpTag->data): NULL ) )

#define REAL_GET_TAG( wrd ) 									\
	((LPSTR)( ((gRTInfo.lpBase + wrd)->lpTag)?					\
					 ((gRTInfo.lpBase + wrd)->lpTag->data): 	\
					 NULL ) )

typedef struct tagREAL_TABLE_INFO
{
	WRD 		wCount;
	WRD 		wAdding;
	HGLOBAL 	hBase;
	LPRTU		lpBase;
	long		lAccessTimes;
}RTInfo;

_DLLEXP RTInfo FAR* _ZZKDECL GetRealTableInformation(void);

_DLLEXP WRD _ZZKDECL RealCreate(LPCSTR	lpszTag_,
						REAL	rValue_,
						REAL	rMin_,
						REAL	rMax_);
_DLLEXP BOOL _ZZKDECL RealDelete(WRD wrd_);

_DLLEXP WRD _ZZKDECL RealCopyTemp( WRD wRTU_ );
_DLLEXP BOOL _ZZKDECL RealDeleteTemp(WRD wRTU_);
_DLLEXP BOOL _ZZKDECL RealUndo( WRD wReal_, WRD wBuffer_ );

_DLLEXP WRD _ZZKDECL RealGetWRD(LPCSTR szTag_);
_DLLEXP int _ZZKDECL RealGetTag(WRD wRTU_, LPSTR szTag_, WORD wSize_);
_DLLEXP STATUS _ZZKDECL RealGetStatus(WRD wRTU_);
_DLLEXP DIRTY _ZZKDECL RealGetDirty(WRD wRTU_);

_DLLEXP WORD _ZZKDECL RealGetElementType( WORD element_ );
_DLLEXP BOOL _ZZKDECL RealGetElementName(WORD element_, LPSTR szName_, WORD wSize_);
_DLLEXP WORD _ZZKDECL RealGetElement(LPSTR szName_);

_DLLEXP BOOL _ZZKDECL RealIsValid( WRD wrd_ );
_DLLEXP BOOL _ZZKDECL RealCheckLegality(WRD wRTU_);
_DLLEXP BOOL _ZZKDECL RealCheckTag(WRD wrd_);

_DLLEXP BOOL _ZZKDECL RealSetTag(WRD wRTU_, LPCSTR szTag_);
_DLLEXP STATUS _ZZKDECL RealSetStatus( WRD wRTU_, STATUS status_ );

_DLLEXP LPSTR _ZZKDECL RealGetData(LPSTR lpsz_, int size_, WRD wrd_, WORD order_);
_DLLEXP BOOL _ZZKDECL RealSetData(LPCSTR lpsz_, WRD wrd_, WORD order_);

_DLLEXP BOOL _ZZKDECL IsRealDirty(WRD);
_DLLEXP BOOL _ZZKDECL IsRealTableDirty( BOOL bPublicDirty_ );
_DLLEXP void _ZZKDECL RealClearTableDirty(void);

_DLLEXP BOOL _ZZKDECL RealCheckMinMax(WRD wrd_);
_DLLEXP BOOL _ZZKDECL RealCheckAlarmThreshold(WRD wrd_);

_DLLEXP ALARM _ZZKDECL RealAlarm( WRD wRTU_ );
_DLLEXP BOOL _ZZKDECL RealAckAlarm( WRD wRTU_ );
_DLLEXP BOOL _ZZKDECL PUWriteRealTable(LPPU to_, SimplePU FAR* from_);
_DLLEXP BOOL _ZZKDECL PUReadRealTable(SimplePU FAR* to_, LPPU from_);
_DLLEXP BOOL _ZZKDECL PURealDirty( LPPU lppu_ );

#endif	//__CREAL_H
