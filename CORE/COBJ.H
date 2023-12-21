//===================================================================
//
//Zheng zhikai	96-8-6
//===================================================================

#ifndef __COBJ_H
#define __COBJ_H

/////////////////////////////
//Object Table
#define LENGTH_NAME_COBJ	32

//object edit proc
//?? UNCERTAIN action
typedef DWORD (FAR* COBJPROC)( LPPU lp_, WORD wMode_, HWND hwnd_ );

//define wMode_ for COBJPROC, which is access mode
#define	OA_RW	0x0000
#define OA_RO	0x0001

typedef struct tagOBJ_TABLE_UNIT
{
	char		szName[LENGTH_NAME_COBJ];
	IDCOBJ		wID;
	COBJPROC	lpfnCobj;		//Cobj routine
}OTU, FAR* LPOTU;

#define	IDCOBJ_ERR		((IDCOBJ)(-1))

#define CAST_TO_COBJ( p )			\
		( (LPCOBJ)(LPVOID)(p) )
#define GET_COBJ_DATA( p )			\
		( (p)? (p->data) : NULL )

typedef struct tagOBJECT_TABLE_INFO
{
	WRD			wCount;
	WRD			wAdding;
	HGLOBAL		hBase;
	LPOTU 		lpBase;
}OTInfo;

_DLLEXP LPCOBJ _ZZKDECL CobjAlloc( IDCOBJ id_, DWORD size_ );

_DLLEXP BOOL _ZZKDECL CobjFree( LPCOBJ lpCobj_ );
_DLLEXP DWORD _ZZKDECL CobjSize( LPCOBJ lpCobj_ );
_DLLEXP BOOL _ZZKDECL CobjLock( LPCOBJ lpCobj_ );

_DLLEXP LPCOBJ _ZZKDECL CobjCopy( LPCOBJ lpCobj_, BOOL FAR* lpbOk_ );

_DLLEXP OTInfo FAR* _ZZKDECL GetCObjTableInformation(void);
_DLLEXP BOOL  _ZZKDECL RegisterCobj(LPOTU lp_);

_DLLEXP BOOL _ZZKDECL CobjGetIDName(IDCOBJ id_, LPSTR szBuffer_, WORD wBufferSize_);
_DLLEXP BOOL _ZZKDECL IsCobj( LPCOBJ lpCobj_ );
_DLLEXP IDCOBJ _ZZKDECL CobjGetID( LPCOBJ lpCobj_ );

//?? UNCERTAIN action, returns a BOOL ?
_DLLEXP BOOL _ZZKDECL CobjAccess( LPPU lpPU_, HWND hwnd_ );

#endif	//__COBJ_H

