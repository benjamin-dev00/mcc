//===================================================================
//Core.h
//Zheng zhikai  95-4-16
//===================================================================

#ifndef __CORE_H
#define __CORE_H
	
#include <time.h>
typedef     WORD            BITS16;
typedef     DWORD           BITS32;

typedef     float           REAL;
typedef     signed long     TINT;
typedef     BITS16          WRD;
typedef     BITS16          PREFIX;

typedef     BITS16          CORERR;
typedef     BITS16          STATUS;
typedef     BITS16          ALARM;
typedef		BITS32			DIRTY;
typedef     BITS32          IDALGO;
typedef     BITS32          IDSTAMP;
typedef     WORD            IDCOBJ;
typedef     long	        CORETIME;	//now this is as same as time_t
typedef		COLORREF		CCLR;		//define my color

typedef     WRD             WRDXTU;     //index for xTU
typedef     WRD             WRDRTU;     //index for RTU
typedef     WRD             WRDTTU;     //index for TTU
typedef     WRD             WRDBTU;     //index for BTU
typedef     WRD             WRDMTU;     //index for MTU
typedef     WRD             WRDFTU;     //index for FTU
typedef     WRD             WRDATU;     //index for ATU
typedef     WRD             WRDSTU;     //index for STU
typedef     WRD             WRDOTU;     //index for OTU

typedef     REAL FAR*       LPREAL;
typedef     TINT FAR*       LPTINT;

typedef     void _far *     FPTR;
typedef     void _near *    NPTR;

//Face coordinate type
#define _FCCRD_as_REAL

#ifdef  _FCCRD_as_REAL
	typedef     REAL            FCCRD;
	typedef     FCCRD FAR*      LPFCCRD;
#endif

//define core-object

typedef struct tagCOBJ
{
	//Note:
	//the the prevous version, the head of COBJ has the following
	//data members:
	//		IDCOBJ  id;
	//		WORD    flags;
	//now this members have been moved to the private head of COBJ, 
	//so COBJ contents user data directly now;
	//
	//If the COBJ object is allocated by GlobalAlloc function,
	//the system certified that the data is packed at 8-byte
	//bound to void GP error while accessing 64k-bound through
	//double type;
	BYTE    data[1];
} COBJ, FAR* LPCOBJ;

/////// obsoleted impliments //////////////////
//
	//define for COBJ.flag
	//#define COBJ_READONLY   0x0001
typedef union tagDATA_UNION
{

	//base data types
	int             zzk__int;
	long            zzk__long;
	WORD            zzk__WORD;
	DWORD           zzk__DWORD;
	REAL            zzk__REAL;
	BOOL            zzk__BOOL;
	LPVOID          zzk__LPVOID;
	FPTR            zzk__FPTR;
	NPTR            zzk__NPTR;
	LPCOBJ          zzk__LPCOBJ;
	UINT		    zzk__UINT;

	//non-base data types
	WRD             zzk__WRD;
	TINT            zzk__TINT;
	BITS16          zzk__BITS16;
	BITS32          zzk__BITS32;
	STATUS          zzk__STATUS;
	ALARM           zzk__ALARM;
	IDALGO			zzk__IDALGO;
	IDCOBJ			zzk__IDCOBJ;
	IDSTAMP			zzk__IDSTAMP;
	CORERR          zzk__CORERR;
	FCCRD           zzk__FCCRD;
	DIRTY			zzk__DIRTY;

	WRDXTU          zzk__WRDXTU;
	WRDRTU          zzk__WRDRTU;
	WRDTTU          zzk__WRDTTU;
	WRDBTU          zzk__WRDBTU;
	WRDMTU          zzk__WRDMTU;
	WRDFTU          zzk__WRDFTU;

	CCLR			zzk__CCLR;
	COLORREF        zzk__COLORREF;
	HANDLE          zzk__HANDLE;
	CORETIME		zzk__CORETIME;

	//obsoleted impliments, now this is replaced by LPCOBJ
	//LPSTR           zzk__LPSTR;

	//this element size == the largest size
	BITS32          zzk__FULL;
	
}DataUnion;

typedef struct tagPARAM_UNIT
{
	PREFIX      prefix;
	WORD        flags;
	DataUnion   du;                 //depend on prefix
}PU, FAR* LPPU;

//as a SimplePU, PF_bits are ONLY PF_IMM
typedef PU  SimplePU;


//the codes in this section are not implemented well, and some
//flags are obsoleted;
#define PU_DIRTY                0x8000
#define PU_NIV                  0x0001  //no initial value
#define PU_IGNOREDIRTY			0x0002	//ignore the dirty bit, now for FTU only
#define PU_NORMAL               0x0000    

#define IS_PU_IGNOREDIRTY(_lppu_)	( _lppu_->flags & PU_IGNOREDIRTY )

/*
PU.prefix  means:
(1) prefix = 0x0000
				||||
				||++-- no use but IMM_WRD
				|+---- IMM_ bits
				+----- PF_IMM

(2) prefix = 0x0000
				||||
				||++-- xTU elements bits
				|+---- TBX_ bits
				+----- PF_TBX

(3) prefix = 0x0000
				||||
				||||
				|+++-- the index of PU to refered
				+----- PF_MPX, PF_FPX
*/

//deine masks

//define constants PF_XXXX
#define PF_IMM      0x1000      //an immediate data
#define PF_TBX      0x3000      //an index of base-table or complex-table head
#define PF_MPX      0x4000      //an index of paramter of the Modu table
#define PF_FPX      0x5000      //an index of paramter of the Face table

#define PF_MASK     0xF000

//with PF_IMM, or
//with PF_NIV,
//  high 4-bit is PF_IMM or PF_NIV
//  low 12-bit of prefix is IMM_xxx, thich is a PDT type data
//  PU.du is value
//
#define IMM_NULL    0x0000      //un-certain
#define IMM_int     0x0100      //for signed int
#define IMM_WORD    0x0200      //for unsigned int
#define IMM_long    0x0300      //for signed long
#define IMM_DWORD   0x0400      //for unsigned long
#define IMM_REAL    0x0500      //for float
#define IMM_BOOL    0x0600      //for BOOL
#define IMM_FPTR    0x0700      //for (far* void)
#define IMM_NPTR    0x0800      //for (near* void)
#define IMM_LPCOBJ  0x0900      //for pointer to cobj
#define IMM_WRD     0x0B00      //for a WRD
#define IMM_USER    0x0C00      //for a user defined
	#define WRD_XTU     0x0000
	#define WRD_RTU     0x0001
	#define WRD_TTU     0x0002
	#define WRD_BTU     0x0003
	#define WRD_MTU     0x0004
	#define WRD_FTU     0x0005
#define IMM_WRDXTU      (IMM_WRD|WRD_XTU)
#define IMM_WRDRTU      (IMM_WRD|WRD_RTU)
#define IMM_WRDTTU      (IMM_WRD|WRD_TTU)
#define IMM_WRDBTU      (IMM_WRD|WRD_BTU)
#define IMM_WRDMTU      (IMM_WRD|WRD_MTU)
#define IMM_WRDFTU      (IMM_WRD|WRD_FTU)

#define IMM_TINT        IMM_long
#define IMM_BITS16      IMM_WORD
#define IMM_UINT        IMM_WORD
#define IMM_BITS32      IMM_DWORD
#define IMM_STATUS      IMM_BITS32
#define IMM_CORERR      IMM_BITS16
#define IMM_COLORREF    IMM_DWORD
#define IMM_CCLR		IMM_COLORREF
#define IMM_ALARM       IMM_BITS16
#define IMM_HANDLE      IMM_NPTR
#define IMM_CORETIME	IMM_long
#define IMM_DIRTY		IMM_BITS32
#define IMM_IDALGO		IMM_BITS32
#define IMM_IDSTAMP		IMM_BITS32
#define IMM_IDCOBJ		IMM_WORD

#ifdef  _FCCRD_as_REAL
#define IMM_FCCRD   IMM_REAL
#endif

#define IMM_MASK    0x0FFF

//with PF_TBX,
//  PF_XXXX flags is valid
//  low 8 bits depand on the table which refered
//  PU.p is the index to the specified table
#define TBX_MASK        0x0F00
#define TBN_MASK        0x0F00

#define TB_RT           0x0100
#define TBX_RT          0x0100

#define TB_TT           0x0200
#define TBX_TT          0x0200

#define TB_BT           0x0300
#define TBX_BT          0x0300

#define TB_MT           0x0400
#define TBX_MT          0x0400

#define TB_FT           0x0500
#define TBX_FT          0x0500

//for TBX_XT
#define XT_MASK         0x00FF

//reserved for Algorithm table and Stamp table
#define TB_AT           0x0600
#define TBX_AT          0x0600
#define TB_ST           0x0700
#define TBX_ST          0x0700

//with PF_MPX,
//  low 12 bits is parameter position (0 is the first param, 1 is the second, and so on)
//  PU.p is the index to Modu table
#define MPX_INDEX_MASK  0x0FFF

//with PF_FPX,
//  low 12 bits is parameter position (0 is the first param, 1 is the secend, and so on)
//  PU.p is the index to Face table
#define FPX_INDEX_MASK  0x0FFF

//////////////////////////////////////
//define for PDU
#define LENGTH_NAME_PARAM       32

//////////////////////////////////////
//define data type name
#define LENGTH_NAME_TYPE     (8)

//Parameter data type:
//PDT is all of IMM_xxxx
typedef WORD        PDT;
#define PDT_NULL        IMM_NULL        //=zero,  un-certainly
#define PDT_int         IMM_int
#define PDT_long        IMM_long
#define PDT_WORD        IMM_WORD
#define PDT_DWORD       IMM_DWORD
#define PDT_REAL        IMM_REAL
#define PDT_BOOL        IMM_BOOL
#define PDT_LPCOBJ      IMM_LPCOBJ
#define PDT_FPTR        IMM_FPTR
#define PDT_NPTR        IMM_NPTR
#define PDT_WRD         IMM_WRD
#define PDT_USER        IMM_USER

#define PDT_WRDXTU      IMM_WRDXTU
#define PDT_WRDRTU      IMM_WRDRTU
#define PDT_WRDTTU      IMM_WRDTTU
#define PDT_WRDBTU      IMM_WRDBTU
#define PDT_WRDMTU      IMM_WRDMTU
#define PDT_WRDFTU      IMM_WRDFTU

#define PDT_TINT        PDT_long
#define PDT_BITS16      PDT_WORD
#define PDT_BITS32      PDT_DWORD
#define PDT_COLORREF    PDT_DWORD
#define PDT_CCLR		PDT_COLORREF
#define PDT_CORETIME    PDT_long
#define PDT_HANDLE      PDT_NPTR

#ifdef _WIN32 
	#define PDT_UINT    PDT_DWORD
#else
	#define PDT_UINT    PDT_WORD
#endif

#ifdef  _FCCRD_as_REAL
#define PDT_FCCRD       PDT_REAL
#else
#error ...
#endif

//define limits
//max length of tag, unit and comment
#define CORE_MAX_TAG_LENGTH			32
#define CORE_MAX_UNIT_LENGTH		32
#define CORE_MAX_COMMENT_LENGTH		512

#define COREVERSION                 (0x00030000)

typedef struct tagCORE_INFO
{
	BITS32      lVersion;           //version information
	char        szDescription[64];	//a text string for descriping of the corebase
	long        lRandMark;			//a unique long integer to identify the whole of core

	//members for Core options
	DWORD		CoreOptions;		//options, see below
	DWORD		reserved;			//must be zero

	//run-time settings
	UINT		uInterval;				//the interval betreen the two run-time timer ticks, unit is ms

////////////////
//Section of dynamic data:
//the follow members will not be saved to files
	int			dynamic_data_begin;		//all dymanic member must be placed after this member

	//the full name of the corebase files that is retrieved
	//in OFSTRUCT struction by OpenFile function;
	//For a new corebase that has not been saved, this element is NULL;
	//so that, Don't use this element to check whether the corebase is
	//in use;
	char        szCoreFullName[_MAX_PATH];

	BOOL		bDirty;			//the user changed the corebase

	//run-time data member
	BOOL		bRunTime;       //whether in run mode
	UINT		idTimer;        //the id of the timer
	DWORD		tick;           //the counter for the ticks
	BOOL		bBlink;         //for blink color is on or off
	UINT		uBlinkCounter;  //inner counter
	UINT		uBlinkUpper;    //upper limit for uBlinkCounter
	CORETIME	LastLogTime;    //the time at last history log
	WRD			wCurFace;		//the face owned the focus
}CoreInfo;

//Core Options:
//
//	CO_TAGCASESENSITIVITY:
//		If set, comparing tags with case sensitivity;
//	CO_ACCESSPRIVILEGE:
//		If set, the corebase accessing mode is set PREVILEGE mode,
//		otherwise, that mode is set to NORMAL mode;
//	CO_LOGHISTORY;			
//		If set, logging history data is enabled;
//	CO_BOOLDOUBLELIMITALARM
//		If set, both of alarm types of BTU, i.e., 
//			TRUE-ALARM and FALSE-ALARM alarm type, 
//		are permitted at the same time; othervise, only one kind 
//		of alarm type is permitted to be enabled at any time;
//	CO_USEFINIT
//		If set, create the new face object with the parameters
//		which specified in FINIT struct, otherwise, create it
//		with the default parameters.

//definition bits in CoreInfo.CoreOptions
//0b0000-0000-0000-0000=0000-0000-0000-0000
//                                  || ||||
//                                  || |||+---- CO_ACCESSPRIVILEGE
//                                  || ||+-----	CO_TAGCASESENSITIVITY
//                                  || |+------	CO_LOGHISTORY
//                                  || +-------	CO_BOOLDOUBLELIMITALARM
//                                  |+---------	CO_USEFINIT
//                                  +----------	CO_EXCEPTIONTRAP
//                                         
#define	CO_TAGCASESENSITIVITY		0x00000001L
#define	CO_ACCESSPRIVILEGE			0x00000002L	
#define	CO_LOGHISTORY				0x00000004L	
#define	CO_BOOLDOUBLELIMITALARM		0x00000008L	
#define	CO_USEFINIT					0x00000010L
#define	CO_EXCEPTIONTRAP			0x00000020L

#define	IS_CO_TAGCASESENSITIVITY()		(gCoreInfo.CoreOptions & CO_TAGCASESENSITIVITY)
#define	IS_CO_ACCESSPRIVILEGE()			(gCoreInfo.CoreOptions & CO_ACCESSPRIVILEGE)
#define	IS_CO_LOGHISTORY()				(gCoreInfo.CoreOptions & CO_LOGHISTORY)
#define	IS_CO_BOOLDOUBLELIMITALARM()	(gCoreInfo.CoreOptions & CO_BOOLDOUBLELIMITALARM)
#define	IS_CO_USEFINIT()				(gCoreInfo.CoreOptions & CO_USEFINIT)
#define	IS_CO_EXCEPTIONTRAP()			(gCoreInfo.CoreOptions & CO_EXCEPTIONTRAP)

#ifndef _WIN32	//Frag_pool: only need in WIN16
	#define POOL_MAX_PAGE               64
	#define POOL_DEFAULT_PAGE_SIZE      (DWORD)0xF000
	typedef struct tagFRAG_POOL
	{
		HGLOBAL     hPage[POOL_MAX_PAGE];   //global handle of the page
		WORD        wPage[POOL_MAX_PAGE];   //selector of the page
		WORD        wCurrentPage;
	}FragPool;
#endif	//_WIN32 for definition for FRAGPOOL

#define IS_RUN_MODE()       (gCoreInfo.bRunTime)

//define Table Element Type(TET_xxx) to descript the table elements
#define TET_READWRITE		0x0000
#define TET_PRIMARY         0x0001  //is a true element
#define TET_READONLY        0x0002  //only can be read by PU 
#define TET_PRIVATE         0x0004  //is a private element

//functions declaration
_DLLEXP BOOL _ZZKDECL CreateCore(LPCSTR szCorePathName_,
						 WRD    cReal_,
						 WRD    cTint_,
						 WRD    cBool_,
						 WRD    cModu_,
						 WRD    cFace_);
_DLLEXP BOOL _ZZKDECL LoadCore( LPCSTR szCorePathName_ );
_DLLEXP void _ZZKDECL FreeCore(void);
_DLLEXP void _ZZKDECL CoreSetDescription( LPCSTR lpszProjDesc_ );

_DLLEXP void FAR* _ZZKDECL FragAlloc(DWORD wBytes_);
_DLLEXP void FAR* _ZZKDECL FragRealloc(void FAR* lp_, DWORD wSize_);
_DLLEXP BOOL _ZZKDECL FragFree(void FAR* lp_);
_DLLEXP DWORD _ZZKDECL FragSize(void FAR* lp_);
_DLLEXP BOOL _ZZKDECL FragDiscard(void);

_DLLEXP CoreInfo FAR* _ZZKDECL GetCoreInformation(void);
_DLLEXP long _ZZKDECL NewRandMark(void);

_DLLEXP BOOL _ZZKDECL IsTagLegal(LPCSTR szTag_);
_DLLEXP WRD _ZZKDECL TagToWRD(LPCSTR szTag_);
_DLLEXP WRD _ZZKDECL TagToWRDEx(LPCSTR szTag_, BOOL FAR* lpbOkey_ );
_DLLEXP void _ZZKDECL WRDToTag(WRD wrd_, LPSTR lpszTag_, WORD wSize_);

_DLLEXP LPCOBJ _ZZKDECL AllocCString( LPCSTR lpsz_, BOOL FAR* bOk_ );
_DLLEXP LPCOBJ _ZZKDECL AllocTag( LPCSTR lpszTag_, BOOL FAR* bOk_ );
_DLLEXP int _ZZKDECL CmpTag(LPCSTR s1_, LPCSTR s2_, size_t maxlen_);

#define GSPDTN_FULLNAME     (0x0000)
#define GSPDTN_SHORTNAME    (0x0001)
#define GSPDTN_NORMAL       (0x0000)
#define GSPDTN_UPPER        (0x0010)
#define GSPDTN_LOWER        (0x0020)
_DLLEXP BOOL _ZZKDECL GetPDTName(PDT pdt_, LPSTR szName_, int size_, WORD wMode_);
_DLLEXP WORD _ZZKDECL GetPDT(LPSTR szName_, BOOL FAR* lpbOk_);

_DLLEXP void _ZZKDECL CoreSetDirty(void);
_DLLEXP void _ZZKDECL CoreClearDirty(void);
_DLLEXP BOOL _ZZKDECL IsCoreDirty(void);

//record the description of a reference information
typedef struct _tagCoreReference{
	WORD    wTable;     //table contained referenced xTU, is TB_MT or TB_FT
	WRD     wrd;        //the WRD of the referenced xTU
	WORD    wParam;     //the index of the referenced parameter
	PU      pu;         //copy of the referenced PU
}COREREF, FAR* LPCOREREF;

_DLLEXP BOOL _ZZKDECL GetReference(WORD wTable_, WRD wrd_, BOOL bSkipSelf_, LPCOREREF lpCoreRef_ );

#define	CGD_VERSION					0x0001
#define	CGD_DESCRIPTION				0x0002
#define	CGD_FULLNAME				0x0003	
#define	CGD_RANDMARK				0x0004
#define	CGD_TAGCASESENSITIVITY		0x0005
#define	CGD_ACCESSPRIVILEGE			0x0006
#define	CGD_LOGHISTORY				0x0007
#define	CGD_INTERVAL				0x0008
#define	CGD_COREFULLNAME			0x0009
#define	CGD_EXCEPTIONTRAP			0x000A
#define	CGD_RUNTIME					0x000B
#define	CGD_IDTIMER					0x000C
#define	CGD_TICK					0x000D
#define	CGD_LASLOGTIME				0x000E
#define	CGD_BOOLDOUBLELIMITALARM	0x000F
#define CGD_USEFINIT				0x0010	//otherwise, create new face with default parameters
#define CGD_CURFACE					0x0011

_DLLEXP DWORD _ZZKDECL CoreGetData( WORD iCoreElement_ );
_DLLEXP BOOL  _ZZKDECL CoreSetData( WORD iCoreElement_, DWORD data_ );

//inner function
BOOL _AllocCore( WRD cReal_,
				 WRD cTint_,
				 WRD cBool_,
				 WRD cModu_,
				 WRD cFace_);

_DLLEXP BOOL _ZZKDECL FreeParameter( LPPU lpPrm_, WORD wNumParam_ );

_DLLEXP BOOL _ZZKDECL CoreTime2tm( CORETIME ct_, struct tm _FAR* lptm_ );
_DLLEXP CORETIME _ZZKDECL tm2CoreTime(  struct tm _FAR* lptm_ );
_DLLEXP CORETIME _ZZKDECL CoreTimeNow(void);
_DLLEXP BOOL _ZZKDECL IsCorebaseValid(void);
_DLLEXP BOOL _ZZKDECL GetBoolStringValue(LPCSTR lpsz_, LPBOOL lpError_ );

_DLLEXP BOOL _ZZKDECL AccessDefaultParameter( BOOL bSave_ );

typedef struct tagTRACE_HEAD
{
	DWORD	Items;			//count of items in struct
	DWORD	Adding;			//adding point
	DWORD	ValidItems;		//count of valid items in struct
	DWORD	ItemBits;		//the number of bit of each items	
	//data are following here
	union {
		REAL	data_REAL[1];	//for Real table
		TINT	data_TINT[1];	//for Tint table
		DWORD	data_DWORD[1];	//for Bool table, each item a bit
	}data;
}TRACEHEAD, FAR* LPTRACE;

BOOL _NONEXPORTED _FreeTraceData( LPTRACE lp_ );

_DLLEXP LRESULT _ZZKDECL DispatchCoreMessage( WPARAM wParam_ );
_DLLEXP LRESULT _ZZKDECL DispatchCoreRunMessage( WPARAM wParam_, LPARAM lParam );

#endif  //__CORE_H
