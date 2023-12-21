#ifndef __CPU_H
#define __CPU_H

//Access mode
//0b0000-0000-0000-0000
//	|||
//	||+--UPPER/LOWER string enable if set; otherwise, disable
//	|+---string upper if set; string lower if reset
//	+----short name if set; fullname if reset
#define AM_SHORTNAME		0x8000
#define AM_UPPER			0x4000
#define AM_ENABLE_UL		0x2000

/////////////////////////////////////////////////
//PIC_CREATE
//	While a new modu/face creating, the command is sent to
//	the algo/stamp handle, the default proc sends PIC_INITIALLIZEPARAM
//	command to initliaze the parameters;
//Note:
//	If the algo/stamp handles this command, it should do its private
//	initialization after the return of default PIC-proc, otherwise,
//	the parameters must be initialized by itself;
//wrd_			specifies the WRD of MTU/FTU that just created
//wNo_: 		n/a(must be zero)
//lParam_:		specifies a user defined data for the MTU/FTU
//Return:		TRUE/FALSE
//				if returns FALSE, the xTU creating will be aborted

/////////////////////////////////////////////////
//PIC_PASTE
//	While a new modu/face creating for pasting, the command is sent to
//	the algo/stamp handle, the default proc does nothing.
//wrd_			specifies the WRD of MTU/FTU that just created
//wNo_: 		n/a(must be zero)
//lParam_:		n/a(must be zero)
//Return:		TRUE/FALSE
//				if returns FALSE, the xTU creating will be aborted

/////////////////////////////////////////////////
//PIC_DELETE
//wrd_			specifies the WRD of MTU/FTU that will be delete
//wNo_: 		n/a(must be zero)
//lParam_:		specifies a user defined data for the MTU/FTU
//Return:		TRUE/FALSE
//				if returns FALSE, the xTU can't be delete

/////////////////////////////////////////////////
//PIC_GETPARAMNAME
//wrd_: 		n/a(must be zero)
//wNo_: 		specifies the parameter to get name
//lParam_:		points to struct GETPARAMNAME
//Return:		TRUE/FALSE
typedef struct _structGETPARAMNAME
{
	char		szName[LENGTH_NAME_PARAM];
	WORD		wAccess;
}GETPARAMNAME, FAR* LPGETPARAMNAME;

/////////////////////////////////////////////////
//PIC_GETPARAMORDER
//wrd_: 		n/a(must be zero)
//wNo_: 		n/a
//lParam_:		points to struct GETPARAMORDER
//Return:		TRUE/FALSE
typedef struct _structGETPARAMORDER
{
	LPSTR	lpszParamName;
	WORD	wOrder; 			//reteives the order of the parameter
}GETPARAMORDER, FAR* LPGETPARAMORDER;

/////////////////////////////////////////////////
//PIC_GETPARAMPRIVILEGE
//wrd_: 		n/a(must be zero)
//wNo_: 		specifies the parameter to get access privilege
//lParam_:		n/a
//Return:		returns the access privilege for the specified parameter

//privilege member in PDU:
//There are 16 privilege levels, from 0 to 15, level 15th is highest.
//
#define PRI_DEV 	0x000F	//visible for developer
#define PRI_ENG 	0x0007	//visible for engneer
#define PRI_ANY 	0x0000	//visible for enyone
#define PRI_ALL 	PRI_ANY //as same as PRI_ANY

/////////////////////////////////////////////////
//PIC_GETPARAMTYPE
//wrd_: 		n/a(must be zero)
//wNo_: 		specifies the parameter to get type
//lParam_:		n/a
//Return:		returns parameter data type, returns NULL if no param.

/////////////////////////////////////////////////
//PIC_GETPARAMVSOF
//wrd_: 		n/a(must be zero)
//wNo_: 		specifies the parameter to retrieve the VSOF
//lParam_:		n/a
//Return:		returns VSOF in PDU,
//				returns NULL if no param;
//

/////////////////////////////////////////////////
//PIC_GETPARAMTYPENAME
//wrd_: 		n/a(must be zero)
//wNo_: 		specifies the parameter to retrieve type name
//lParam_:		points to struct GETPARAMTYPENAME
//Return:		TRUE/FALSE
typedef struct _structGETPARAMTYPENAME
{
	char		szName[LENGTH_NAME_TYPE];
	WORD		wAccess;	// access mode
}GETPARAMTYPENAME, FAR* LPGETPARAMTYPENAME;

/////////////////////////////////////////////////
//PIC_GETNUMPARAM
//wrd_: 		n/a(must be zero)
//wNo_: 		n/a
//lParam_:		n/a
//Return:		returns the number of parameter.
//				if this is a varible-parameter algo or stamp,
//				the value is the default (recommanded) number;
//				number of parameters.

/////////////////////////////////////////////////
//PIC_ENCODE
//wrd_: 		specifies the FTU/MTU whoes parameter will be encoded
//wNo_: 		specifies the parameter to be encoded
//lParam_:		points to struct ENCODE
//Return:		TRUE/FALSE
typedef struct _tagENCODE{
	LPSTR	lpszBuffer; 		//point to the buffer(an ASCIIZ string)
	WORD	wAccess;			//an idle element
	WORD	wError;
}ENCODE, FAR* LPENCODE;

#define ILLEGAL_SZTAIL		1
#define ILLEGAL_SZBODY		2
#define EXPECTED_SZBODY 	3
#define EXPECTED_SZTAIL 	4

/////////////////////////////////////////////////
//PIC_DECODE
//wrd_: 		specifies the FTU/MTU whoes parameter will be decoded
//wNo_: 		specifies the parameter to be decoded
//lParam_:		points to DECODE struct
//Return:		TRUE/FALSE
typedef struct _tagDECODE{
	WORD		wAccess;
	char		szHead[8];
	char		szBody[CORE_MAX_TAG_LENGTH];
	char		szTail[LENGTH_NAME_PARAM];
	WRD 		wBody;
	WORD		wTail;
	WORD		bImm;
} DECODE, FAR* LPDECODE;

/////////////////////////////////////////////////
//PIC_GETVALUESTRING
//wrd_: 		specifies the FTU/MTU
//wNo_: 		specifies the parameter to retrieve value-string
//lParam_:		points to GETVALUESTRING struct
//Return:		a DWORD value
//				GVSRESULT_FAILED	failed to get the value string;
//				GVSRESULT_SUCCESS	secceed;
//				GVSRESULT_COMPLEX	can't get valuestring because it's a complex data PU;
//				GVSRESULT_USERDEF	can't get valuestring because it's a user defined value string;
//Note:
//	If the size of the buffer is too small, the call will
//	return FALSE.
typedef struct _structGETVALUESTRING
{
	LPSTR		lpszBuffer;
	WORD		wSize;
}GETVALUESTRING, FAR* LPGETVALUESTRING;

//the return value from PUGetValueString 
#define GVSRESULT_FAILED			0
#define GVSRESULT_SUCCESS			1
#define GVSRESULT_COMPLEX			2
#define GVSRESULT_USERDEF			3
#define GVSRESULT_COMPLEX_SUCCESS	4

/////////////////////////////////////////////////
//PIC_SETVALUESTRING
//wrd_: 		specifies the FTU/MTU
//wNo_: 		specifies the parameter to set value-string
//lParam_:		points to an ASCIIZ string
//Return:		TRUE/FALSE

/////////////////////////////////////////////////
//PIC_PARAMTUNE
//wrd_: 		n/a(must be zero)
//wNo_: 		n/a
//lParam_:		the WRD of the FTU/MTU
//Return:		TRUE/FALSE

/////////////////////////////////////////////////
//PIC_INITIATE
//	initiate to run-time mode
//	NOTE: the FTUs must update the FTU.Rect at this while,
//	because the FTUs need not calculate the select-handles
//	at the run-time;
//wrd_: 		the WRD of the FTU/MTU
//wNo_: 		n/a
//lParam_:		step clock interval(in ms)
//Return:		TRUE/FALSE
//				if returns FALSE, the core initiates the run-time
//				mode failure

/////////////////////////////////////////////////
//PIC_STOP
//	stop run-time mode
//wrd_: 		the WRD of the FTU/MTU
//wNo_: 		n/a
//lParam_:		n/a(must be zero)
//Return:		TRUE/FALSE

/////////////////////////////////////////////////
//Obsoleted
//PIC_GETVARPARAMSTRUCT
//	Only for variable parameters algorithm to retrieve
//	content of VPS for the algorithm, that struct is
//	public portion of the struct VPSTRUCT
//wrd_: 		n/a
//wNo_: 		n/a
//lParam_:		point to VPS struct
//Return:		a WORD, is equal to VPSTRUCT.wNumVar
typedef struct tagVARPARAMSTRUCT{
	WORD	wMinVarGroup;
	int 	wVarLowBound;
	WORD	wNumFix;
	WORD	wNumVar;
}VPS, FAR* LPVPS;

/////////////////////////////////////////////////
//Obsoleted
//PIC_GETDEFAULTPREFIX
//wrd_: 		n/a(must be zero)
//wNo_: 		specifies the parameter to get the dafault prefix
//lParam_:		n/a
//Return:		returns deafault prefix that is =(PDU.wPF|PDU.wType),
//				returns NULL if no param;
//

/////////////////////////////////////////////////
//PIC_ONLOAD
//When the code of the algo/stamp loads into memory, this notify
//message is sent; normally, the caller is the LibMain for DLL
//wrd_: 		n/a(must be zero)
//wNo_: 		n/a
//lParam_:		n/a
//Return:		TRUE/FALSE
//				if returns FALSE, the stamp can't register to corebase
//				this causes the proccess abord!

/////////////////////////////////////////////////
//PIC_ONEXIT
//When the code of the algo/stamp will remove from memory, this notify
//message is sent; normally, the caller is the LibMain for DLL
//wrd_: 		n/a(must be zero)
//wNo_: 		n/a
//lParam_:		n/a
//Return:		TRUE/FALSE
//				(return value is omitted)
//

/////////////////////////////////////////////////
//PIC_INITIALIZEPARAM
//When parameters are created, inserted, or appended, calling this
//command initializes the new parameter;
//wrd_: 		WRD of the modu/face
//wNo_: 		the order of the new parameter to be initialized
//lParam_:		n/a
//Return:		TRUE/FALSE
//				(return value is omitted)
//

/////////////////////////////////////////////////
//PIC_CANCOPY
//This command is sent to retrieve whether the node 
//specified by wNo_ can do copy operation;
//wrd_: 		WRD of the node
//wNo_: 		n/a
//lParam_:		n/a
//Return:		TRUE/FALSE
//
//



/////////////////////////////////////////////////
//PIC_CANDELETE
//This command is sent to retrieve whether the modu/face permites
//to delete the parameter group specified by wNo_;
//wrd_: 		WRD of the modu/face
//wNo_: 		specifies the first parameter to be deleted
//lParam_:		n/a
//Return:		the number of parameter in a group to be deleted if
//				successful, otherwise, returns 0;
//
//

/////////////////////////////////////////////////
//PIC_CANINSERT
//This command is sent to retrieve whether the modu/face permites
//to insert the parameter group specified by wNo_;
//wrd_: 		WRD of the modu/face
//wNo_: 		specifies the parameter before which the new parameter will be inserted
//lParam_:		n/a
//Return:		the number of parameter in a group to be inserted if
//				successful, otherwise, returns 0;
//

/////////////////////////////////////////////////
//PIC_CANAPPEND
//This command is sent to retrieve whether the modu/face permites
//to append the parameter group;
//wrd_: 		WRD of the modu/face
//wNo_: 		n/a
//lParam_:		n/a
//Return:		the number of parameter in a group to be appand if
//				successful, otherwise, returns 0;
//

/////////////////////////////////////////////////
//PIC_ISVARPARAM
//This command is sent to retrieve whether the modu/face
//has varible number of parameters
//wrd_: 		n/a
//wNo_: 		n/a
//lParam_:		n/a
//Return:		TRUE/FALSE
//

/////////////////////////////////////////////////
//PIC_CHECKPARAM
//	This command is sent to check legility of parameters;
//	The default Proc checks the data-link-validity and the integrality
//	of the number of parameters;
//wrd_: 		WRD of the modu/face
//wNo_: 		n/a
//lParam_:		n/a
//Return:		TRUE/FALSE
//

/////////////////////////////////////////////////
//PIC_LOADDEFAULT
//This command is sent to load the default values
//from .ini file
//wrd_: 		n/a
//wNo_: 		n/a
//lParam_:		points to the .ini file name and section name
//				format: "<file_name>""\0""<section_name>"
//Return:		TRUE/FALSE

/////////////////////////////////////////////////
//PIC_SAVEDEFAULT
//This command is sent to save the default values
//to .ini file
//wrd_: 		n/a
//wNo_: 		n/a
//lParam_:		points to the .ini file name and section name
//				format: "<file_name>""\0""<section_name>"
//Return:		TRUE/FALSE

/////////////////////////////////////////////////
//PIC_PASTEDEFAULT
//This command is sent to set the parameters of the module/face 
//which specified by wrd_ with the default values.
//wrd_: 		the WRD of the module/face
//wNo_: 		n/a
//lParam_:		n/a
//Return:		TRUE/FALSE
//

/////////////////////////////////////////////////
//PIC_SETDEFAULT
//This command is sent to set the default values
//wrd_: 		n/a
//wNo_: 		n/a
//lParam_:		WRD of the default module/face
//Return:		TRUE/FALSE
//
//Note:
//	The LPCOBJ parameter can't set default value
//
//if a parameter is forbidden to set the default value,
//it should combine the below const. with PDU.flags
#define PDUF_NODEFAULT		0x8000
#define PDUF_CANNTCOPY		0x4000	//it has default value, but can't copy/paste between faces

/////////////////////////////////////////////////
//PIC_GETVSOFENUMSTRUCT
//This command is sent to retreive a user-defined value string struct
//wrd_: 		n/a
//wNo_: 		n/a
//lParam_:		pointer to VSOFENUMSTRUCT
//Return:		TRUE/FALSE
//
//Note:

/////////////////////////////////////////////////
//PIC_AFTERLOAD
//This command is sent to the modu/face after it has been loaded
//wrd_: 		specify the modu/face
//wNo_: 		n/a
//lParam_:		pointer to VSOFENUMSTRUCT
//Return:		TRUE/FALSE

/////////////////////////////////////////////////
//PIC_BEFORESAVE
//This command is sent to the modu/face before its saving
//wrd_: 		specify the modu/face
//wNo_: 		n/a
//lParam_:		pointer to VSOFENUMSTRUCT
//Return:		TRUE/FALSE

typedef enum _enumPIC{
	PIC_GETPARAMNAME = 1,
	PIC_GETPARAMORDER,
	PIC_GETPARAMPRIVILEGE,
	PIC_GETPARAMTYPE,
	PIC_GETPARAMTYPENAME,
	PIC_GETNUMPARAM,
	PIC_ENCODE,
	PIC_DECODE,
	PIC_GETVALUESTRING,
	PIC_SETVALUESTRING,
	PIC_PARAMTUNE,			//not implemented yet
	PIC_INITIATE,
	PIC_STOP,
	PIC_CREATE,
	PIC_DELETE,
	PIC_ONLOAD,
	PIC_ONEXIT,
	PIC_GETPARAMVSOF,
	PIC_ISVARPARAM,
	PIC_INITIALIZEPARAM,
	PIC_CANDELETE,
	PIC_CANINSERT,
	PIC_CANAPPEND,
	PIC_CHECKPARAM,
	PIC_CANCOPY,
	PIC_PASTE,
	PIC_SETDEFAULT,
	PIC_PASTEDEFAULT,
	PIC_LOADDEFAULT,
	PIC_SAVEDEFAULT,
	PIC_GETVSOFENUMSTRUCT,
	PIC_AFTERLOAD,	//after modu/face has been loaded
	PIC_BEFORESAVE, //before modu/face saving
	PIC_GETICON,
	PIC_GETDESCRIPTION_SHORT,
	PIC_GETDESCRIPTION_LONG,
} PIC;

//parameter info proc
typedef DWORD (_ZZKDECL _FAR* PARAMPROC)(PIC wCmd_, WRD wrd_, WORD wNo_, LONG lParam_);

typedef DWORD	VSOF;
typedef struct tagPARAM_DESCRIPTION_UNIT
{
	WORD	wNo;		//debug code
	char	szName[LENGTH_NAME_PARAM];
	VSOF	vsof;		//value string output format
	WORD	flags;		//privilege and other flags

	//the default PU
	PU		pu;
}PDU, FAR* LPPDU;

#define _ELE_PDU(_order_, _szName_, _vsof_, _flags_, _pri_ )		\
	{ _order_, _szName_, _vsof_, _pri_, 							\
	  {PF_IMM|_order_##_PDT, _flags_, NULL} }

//parameter desciption table for varible paramter
typedef struct _tagVARPARAMSTRUCT{
	WORD	wMinVarGroup;
	int 	wVarLowBound;
	WORD	wNumFix;
	LPPDU	lpFixPdu;
	WORD	wNumVar;
	LPPDU	lpVarPdu;
}VPSTRUCT, FAR* LPVPSTRUCT;

typedef struct _tagVSOFNAME
{
	DWORD	Value;
	LPCSTR	szName;
}VSOFNAME, FAR* LPVSOFNAME;

typedef struct _tagVSOFENUMSTRUCT
{
	LPVSOFNAME		lpVsofName;
	int 			nItem;
}VSOFENUMSTRUCT, FAR* LPVSOFENUMSTRUCT;

#define MAKEVSOF(pdt, mode) 	MAKELONG(pdt, mode)
#define VSOFGETPDT(vsof)		(LOWORD(vsof))

//define for VSOF (Value String Output Format)
#define VSOF_NULL				MAKEVSOF(PDT_NULL  , 0x0000)
#define VSOF_int				MAKEVSOF(PDT_int   , 0x0000)
#define VSOF_int_LNSTYLE		MAKEVSOF(PDT_int   , 0x0001)
#define VSOF_int_RECTBS 		MAKEVSOF(PDT_int   , 0x0002)
#define VSOF_int_GBS			MAKEVSOF(PDT_int   , 0x0003)	
#define VSOF_int_ACTIVE 		MAKEVSOF(PDT_int   , 0x0004)
#define VSOF_int_USER			MAKEVSOF(PDT_int   , 0xFFFF)
#define VSOF_WORD				MAKEVSOF(PDT_WORD  , 0x0000)
#define VSOF_WORD_HEX			MAKEVSOF(PDT_WORD  , 0x0001)
#define VSOF_WORD_BIN			MAKEVSOF(PDT_WORD  , 0x0002)
#define VSOF_WORD_USER			MAKEVSOF(PDT_WORD  , 0xFFFF)
								
#define VSOF_long				MAKEVSOF(PDT_long  , 0x0000)
#define VSOF_CORETIME			MAKEVSOF(PDT_long  , 0x0001)
#define VSOF_long_USER			MAKEVSOF(PDT_long  , 0xFFFF)
								
#define VSOF_DWORD				MAKEVSOF(PDT_DWORD , 0x0000)
#define VSOF_DWORD_HEX			MAKEVSOF(PDT_DWORD , 0x0001)
#define VSOF_DWORD_BIN			MAKEVSOF(PDT_DWORD , 0x0002)
#define VSOF_DWORD_CLR			MAKEVSOF(PDT_DWORD , 0x0003)
#define VSOF_DWORD_MS			MAKEVSOF(PDT_DWORD , 0x0004)
#define VSOF_DWORD_BWIDTH		MAKEVSOF(PDT_DWORD , 0x0005)
#define VSOF_IDALGO 			MAKEVSOF(PDT_DWORD , 0x0006)
#define VSOF_IDSTAMP			MAKEVSOF(PDT_DWORD , 0x0007)
#define VSOF_DWORD_TXTVERALIGN	MAKEVSOF(PDT_DWORD , 0x0008)
#define VSOF_DWORD_TXTHORALIGN	MAKEVSOF(PDT_DWORD , 0x0009)
#define VSOF_DWORD_DISPLAYMODE	MAKEVSOF(PDT_DWORD , 0x000A)
#define VSOF_DWORD_USER 		MAKEVSOF(PDT_DWORD , 0xFFFF)
								
#define VSOF_REAL				MAKEVSOF(PDT_REAL  , 0x0000)
#define VSOF_REAL_USER			MAKEVSOF(PDT_REAL  , 0xFFFF)
								
#define VSOF_BOOL				MAKEVSOF(PDT_BOOL  , 0x0000)
#define VSOF_BOOL_USER			MAKEVSOF(PDT_BOOL  , 0xFFFF)
#define VSOF_BOOL_TRUEFALSE 	MAKEVSOF(PDT_BOOL  , 0x0000)
#define VSOF_BOOL_YESNO 		MAKEVSOF(PDT_BOOL  , 0x0001)
#define VSOF_BOOL_ONOFF 		MAKEVSOF(PDT_BOOL  , 0x0002)
#define VSOF_BOOL_OPENCLOSE 	MAKEVSOF(PDT_BOOL  , 0x0003)
#define VSOF_BOOL_10			MAKEVSOF(PDT_BOOL  , 0x0004)
#define VSOF_BOOL_MANUAUTO		MAKEVSOF(PDT_BOOL  , 0x0005)
#define VSOF_FPTR				MAKEVSOF(PDT_FPTR  , 0x0000)
#define VSOF_FPTR_RECT			MAKEVSOF(PDT_FPTR  , 0x0001)
#define VSOF_NPTR				MAKEVSOF(PDT_NPTR  , 0x0000)
#define VSOF_WRD				MAKEVSOF(PDT_WRD   , 0x0000)
#define VSOF_WRDXTU 			MAKEVSOF(PDT_WRDXTU, 0x0000)
#define VSOF_WRDRTU 			MAKEVSOF(PDT_WRDRTU, 0x0000)
#define VSOF_WRDTTU 			MAKEVSOF(PDT_WRDTTU, 0x0000)
#define VSOF_WRDBTU 			MAKEVSOF(PDT_WRDBTU, 0x0000)
#define VSOF_WRDMTU 			MAKEVSOF(PDT_WRDMTU, 0x0000)
#define VSOF_WRDMTU_TRIGGER		MAKEVSOF(PDT_WRDMTU, 0x0001)
#define VSOF_WRDFTU 			MAKEVSOF(PDT_WRDFTU, 0x0000)
//#define VSOF_STR				MAKEVSOF(PDT_STR   , 0x0000)
#define VSOF_LPCOBJ 			MAKEVSOF(PDT_LPCOBJ, 0x0000)
#define VSOF_LPCOBJ_STR 		MAKEVSOF(PDT_LPCOBJ, 0x0001)
#define VSOF_USER				MAKEVSOF(PDT_USER  , 0x0000)
								
//for RTU						
#define VSOF_STATUS_RTU 		MAKEVSOF(PDT_WORD  , 0x1001)
#define VSOF_DIRTY_RTU			MAKEVSOF(PDT_WORD  , 0x1002)
#define VSOF_ALARM_RTU			MAKEVSOF(PDT_WORD  , 0x1003)
//for TTU						
#define VSOF_STATUS_TTU 		MAKEVSOF(PDT_WORD  , 0x2001)
#define VSOF_DIRTY_TTU			MAKEVSOF(PDT_WORD  , 0x2002)
#define VSOF_ALARM_TTU			MAKEVSOF(PDT_WORD  , 0x2003)
//for BTU						
#define VSOF_STATUS_BTU 		MAKEVSOF(PDT_WORD  , 0x3001)
#define VSOF_DIRTY_BTU			MAKEVSOF(PDT_WORD  , 0x3002)
#define VSOF_ALARM_BTU			MAKEVSOF(PDT_WORD  , 0x3003)
//for MTU						
#define VSOF_STATUS_MTU 		MAKEVSOF(PDT_WORD  , 0x4001)
#define VSOF_DIRTY_MTU			MAKEVSOF(PDT_WORD  , 0x4002)
//for FTU						
#define VSOF_STATUS_FTU 		MAKEVSOF(PDT_WORD  , 0x5001)
#define VSOF_DIRTY_FTU			MAKEVSOF(PDT_WORD  , 0x5002)

/////////////////////
#ifdef _FCCRD_as_REAL
#define VSOF_FCCRD		VSOF_REAL
#endif

#define VSOF_TINT		VSOF_long
#define VSOF_HANDLE 	(VSOF_WORD_HEX)

#ifndef _WIN32 // add by sc ignore it when os is win32

#if sizeof(HANDLE) == sizeof(void _near*)
#else
#error sizeof(HANDLE) != sizeof(void _near*)
#endif
#endif

_DLLEXP DWORD _ZZKDECL DefProcessPIC(
	PIC 	wCmd_,
	WRD 	wrd_,
	WORD	wNo_,
	LONG	lParam_,
	WORD	wTable_,	//TBX_FT or TBX_MT
	WORD	wNumPDU_,
	DWORD	id_,		//IDSTAMP or IDALGO
	LPPDU	lpPDU_);

_DLLEXP DWORD _ZZKDECL DefProcessVarPIC(
	PIC 		wCmd_,
	WRD			wrd_,
	WORD		wNo_,
	LONG		lParam_,
	WORD		wTable_,	 //TBX_FT or TBX_MT
	DWORD		id_,	//IDSTAMP or IDALGO 
	LPVPSTRUCT	lpVPD_);

_DLLEXP DWORD _ZZKDECL PUGetValueString(LPPU				lpPU_,
								LPGETVALUESTRING	lpGVS_,
								VSOF				vsof_);

_DLLEXP DWORD _ZZKDECL PUSetValueString(LPPU	lpPU_,
								LPCSTR	 lpsz_,
								VSOF	vsof_);

_DLLEXP void _ZZKDECL CombineDecode(LPSTR lpszBuffer_, LPDECODE lpDecode_);
_DLLEXP BOOL _ZZKDECL PUCheckLegality( LPPU lppu_ );
//_DLLEXP BOOL _ZZKDECL GetBoolStringValue(LPCSTR lpsz_, LPBOOL lpError_ );
//_DLLEXP BOOL _ZZKDECL GetDigitStringValue( LPCSTR lpsz_, LPPU lpPU_ );

_DLLEXP BOOL _ZZKDECL ScanNumber( LPCSTR lpsz_, LPPU lpPU_ );
_DLLEXP BOOL _ZZKDECL PrintBool( LPSTR lpBuffer_, BOOL val_, VSOF vsof_ );
_DLLEXP BOOL _ZZKDECL ScanBool( LPCSTR lpsz_, BOOL* pbool_ );
_DLLEXP BOOL _ZZKDECL PrintCclr( LPSTR lpBuffer_, CCLR clr_ );
_DLLEXP BOOL _ZZKDECL ScanCclr( LPCSTR lpsz_, CCLR* pclr_ );

_DLLEXP BOOL _ZZKDECL EnumVsofString( WORD wTable_, WRD wrd_, WORD wNo_, LPPU lpPU_, LPSTR lpsz_, UINT size_ );

#endif	//__CPU_H

