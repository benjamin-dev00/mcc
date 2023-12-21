//===================================================================
//
//Zheng zhikai	96-8-6
//===================================================================

#include "prehead.h"

#include "core.h"
#include "corerw.h"
#include "cobj.h"
#include "cerror.h"

typedef struct _tagCOBJ_BLOCK
{
	//PRIVATE-HEAD
	//	the following elements: hGlobal and mark, is owned by system privately,
	//	programmers should never change it!
	//	if the COBJ is allocated from golbal pool, that means by function
	//	GlobalAlloc, the hGlobal is its golbal handle, otherwise, it is NULL;
	WORD	mark;		//if the Cobj valid, it contents 'CO'
	WORD	count;		//the reference counter of this cobj
#ifndef _WIN32
	HGLOBAL	hGlobal;
#else	
	// at WIN32 version, we use the presice size of Cobject instead
	// of the size returned from API xxxSize function;
	DWORD	size;		//size of the object
#endif
	IDCOBJ	id;
	//end of PRIVATE-HEAD

	BYTE	data[1];
}COBJBLK, FAR* LPCOBJBLK;

#ifndef _WIN32
	#define _COBJ_HEAD_SIZE		( 2*sizeof(WORD) + sizeof(HGLOBAL) + sizeof(IDCOBJ) )
#else
	#define _COBJ_HEAD_SIZE		( 2*sizeof(WORD) + sizeof(DWORD) + sizeof(IDCOBJ) )
#endif

#define LPTO_COBJBLK(lpCobj)	\
( (LPCOBJBLK)((LPBYTE)(lpCobj) - _COBJ_HEAD_SIZE) )

#define GET_COBJ_ID( p )		\
		( LPTO_COBJBLK(p)->id )

static WRD _near CobjGetFreeRoom(void);

extern DWORD		gdwLastError;
extern CoreInfo		gCoreInfo;

extern OTInfo		gOTInfo;

#define COBJ_MARK 	('CO')

//...................................................................
//Description:
//	Registers an Cobj in the Cobj Table
//Entry:
//	OTU FAR* lp_
//		source Cobj-unit. The function copies the whole of lp_ to the
//		new unit.
//Return:
//		returns TRUE if the function succeeds. Otherwise, it is FALSE.
//...................................................................
_DLLEXP BOOL _DLLFUNC RegisterCobj(LPOTU lp_)
{
	WRD			_i;

	//if no free room
	if(gOTInfo.wAdding >= gOTInfo.wCount)
		return FALSE;

	//check whether a legal register
	for( _i=1; _i<gOTInfo.wAdding; _i++ )
	{
		if( !strnicmp(gOTInfo.lpBase[_i].szName, lp_->szName, LENGTH_NAME_COBJ) )
			return FALSE;	//the name is existed
		if( gOTInfo.lpBase[_i].wID == lp_->wID )
			return FALSE;	//the id is existed
	}

	_i = CobjGetFreeRoom();
	if( !_i )
	{
		//if no free room, function CobjGetFreeRoom(...) has set gdwLastError
		return FALSE;
	}
	memset( gOTInfo.lpBase + _i, 0, sizeof(OTU) );
	memcpy( &((gOTInfo.lpBase)[_i]), lp_, sizeof(OTU) );

	return TRUE;
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC UnregisterCobj( IDCOBJ wID_ )
{
	WRD		_i;

	//check whether a legal register
	for( _i=1; _i<gOTInfo.wAdding; _i++ )
	{
		if( gOTInfo.lpBase[_i].wID == wID_ )
		{
			//clear this in order to unregister
			memset( &((gOTInfo.lpBase)[_i]), 0, sizeof(OTU) );
			return TRUE;	//find the cobj
		}
	}

	return FALSE;
}

//...................................................................
//Description:
//...................................................................
static WRD _near CobjGetFreeRoom()
{
	WRD		_i;

	CLEAR_LAST_ERROR;

	for(_i=1; _i<gOTInfo.wAdding; _i++)
	{
		if( (gOTInfo.lpBase + _i)->wID == 0 )
			break;
	}

	if( _i < gOTInfo.wAdding )
		return _i;
	else	// _i == gOTInfo.wAdding
	{
		if( gOTInfo.wAdding < gOTInfo.wCount )
		{
			gOTInfo.wAdding ++;
			return _i;
		}
		else	//no free room
		{
			//if no free room, set last error
			SET_LAST_ERROR( ERR_OTU_No_Free_Room );
			return 0;
		}
	}
}

//...................................................................
//Description:
//	Allocate a new core-Cobj in Frag pool
//...................................................................
_DLLEXP LPCOBJ _DLLFUNC CobjAlloc( IDCOBJ id_, DWORD size_ )
{
	LPCOBJBLK	_lp;
	DWORD		_org_size = size_;

	if( !size_ )	//zero memory allocation
		return NULL;

	size_ += _COBJ_HEAD_SIZE;

#ifndef _WIN32
	//WIN16
	if( size_ >= 0x8000 )
	{
		HGLOBAL		_hglb;

		//allocate it in global pool
		_hglb = GlobalAlloc( GHND, size_ );
		if( !_hglb )
			return NULL;

		_lp = (LPCOBJBLK)GlobalLock( _hglb );
		_lp->hGlobal = _hglb;	//record the HANDLE in Cobj private-head
	}
	else
	{
		_lp = (LPCOBJBLK)FragAlloc( (UINT)size_ );
		if( !_lp )
			return NULL;

		_lp->hGlobal = NULL;
	}
#else
	//WIN32
	_lp = (LPCOBJBLK)FragAlloc( (UINT)size_ );
	if( !_lp )
		return NULL;
	_lp->size = _org_size;
#endif

	_lp->mark = COBJ_MARK;
	_lp->count = 1;
	_lp->id = id_;
	return (LPCOBJ)(_lp->data);
}

//...................................................................
//Description:
//	Increase the lock times of a cobject
//...................................................................
_DLLEXP BOOL _DLLFUNC CobjLock( LPCOBJ lpCobj_ )
{
	LPCOBJBLK	_lp;

	if( !lpCobj_ )
		return TRUE;

	_lp = LPTO_COBJBLK(lpCobj_);

	if( _lp->mark != COBJ_MARK )
	{
		LOGERR1( "Cobj is corrupted :%Fp", lpCobj_ );
		CORE_ASSERT( FALSE );
		return FALSE;
	}
	else
	{
		if( _lp->count == 0 )
		{
			LOGERR1( "Cobj is corrupted :%Fp", lpCobj_ );
			return FALSE;
		}

		if( _lp->count >= 0x7FFF )
			LOGWARN1( "Cobj is corrupted :%Fp", lpCobj_ );

		_lp->count ++;
		return TRUE;
	}
}

//...................................................................
//Description:
//	Frees a core-cobj in Frag pool
//...................................................................
_DLLEXP BOOL _DLLFUNC CobjFree( LPCOBJ lpCobj_ )
{
	LPCOBJBLK	_lp;

	if( !lpCobj_ )
		return TRUE;

	_lp = LPTO_COBJBLK(lpCobj_);

	if( _lp->mark != COBJ_MARK )
	{
		LOGERR1( "Cobj is corrupted :%Fp", lpCobj_ );
		CORE_ASSERT( FALSE );
		return FALSE;
	}
	else
	{
		if( _lp->count == 0 )
		{
			LOGERR1( "Cobj is corrupted :%Fp", lpCobj_ );
			return FALSE;
		}

		if( _lp->count >= 0x7FFF )
			LOGWARN1( "Cobj is corrupted :%Fp", lpCobj_ );

		_lp->count --;
		if( _lp->count )
			return TRUE;

		_lp->mark = "cl";	//set clear mark, note: "cl" is low-case here

#ifndef _WIN32
		if( _lp->hGlobal )
		{
			GlobalUnlock( _lp->hGlobal );

			//Note:
			// GlobalFree return NULL if successful
			return GlobalFree( _lp->hGlobal ) ? FALSE : TRUE;
		}
		else
			return FragFree( _lp );
#else
	//WIN32
	return FragFree( _lp );
#endif
	}
}

//...................................................................
//Description:
//	if	lpCobj_ is
//					NULL, returns NULL
//					not a cobj, returns IDCOBJ_ERR
//					a cobj, returns the ID of it
//...................................................................
_DLLEXP IDCOBJ _DLLFUNC CobjGetID( LPCOBJ lpCobj_ )
{
	if( !lpCobj_ )
		return NULL;

	if( !IsCobj(lpCobj_) )
	{
		LOGERR1( "Cobj is corrupted :%Fp", lpCobj_ );
		CORE_ASSERT( FALSE );
		return IDCOBJ_ERR;
	}

	return GET_COBJ_ID( lpCobj_ );
}

//...................................................................
//Description:
//...................................................................
_DLLEXP LPCOBJ _DLLFUNC CobjCopy( LPCOBJ lpCobj_, BOOL FAR* lpbOk_ )
{
	if( lpbOk_ )
		*lpbOk_ = TRUE;		//set initial value

	if( !lpCobj_ )
		return NULL;
	else
	{
		DWORD 	_dwSize;
		LPCOBJ	_lpCobjNew;

		if( !IsCobj(lpCobj_) )
		{
			LOGERR1( "Cobj is corrupted :%Fp", lpCobj_ );
			CORE_ASSERT( FALSE );
			if( lpbOk_ )
				*lpbOk_ = FALSE;
			return FALSE;
		}

		_dwSize = CobjSize( lpCobj_ );
		_lpCobjNew = CobjAlloc( GET_COBJ_ID( lpCobj_ ), _dwSize );

		if( !_lpCobjNew )
		{
			if( lpbOk_ )
				*lpbOk_ = FALSE;
			return NULL;
		}
		else
		{
			memcpy( _lpCobjNew, lpCobj_, (size_t)_dwSize );
			//lpbOk has set
			return _lpCobjNew;
		}
	}
}

//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC CobjSize( LPCOBJ lpCobj_ )
{
	LPCOBJBLK	_lp;

	if( !lpCobj_ )
	{
		CORE_ASSERT( FALSE );
		return 0;
	}

	_lp = LPTO_COBJBLK(lpCobj_);

	if( _lp->mark != COBJ_MARK )
	{
		LOGERR1( "Cobj is corrupted :%Fp", lpCobj_ );
		CORE_ASSERT( FALSE );
		return 0;
	}
	else
	{
#ifndef _WIN32
		DWORD	_dwSize;

		if( _lp->hGlobal )
			_dwSize = GlobalSize( _lp->hGlobal );
		else
			_dwSize = FragSize( _lp ) ;

		if( _dwSize )
			return _dwSize - _COBJ_HEAD_SIZE;
		else
		{
			CORE_ASSERT( FALSE );
			return 0;
		}
#else
		// WIN32 here
		return _lp->size;
#endif

	}
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC IsCobj( LPCOBJ lpCobj_ )
{
	WRD			_i;
	LPCOBJBLK	_lp;

	if( !lpCobj_ )
		return TRUE;

#ifdef WIN32
	__try
	{
#endif
		//////////////
		//try body begin:
		_lp = LPTO_COBJBLK(lpCobj_);
		if( _lp->mark != COBJ_MARK )
			return FALSE;

#ifdef WIN32
		//need to test write-access, which implies read-access
		if( IsBadWritePtr( _lp, _COBJ_HEAD_SIZE + _lp->size ) )
			return FALSE;
#endif

		for(_i=1; _i<gOTInfo.wAdding; _i++)
		{
			if(gOTInfo.lpBase[_i].wID == _lp->id )
				return TRUE;
		}
		return FALSE;	//no found ID

		//try body end
		//////////////

#ifdef WIN32
	}
	__except( GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? 
                EXCEPTION_EXECUTE_HANDLER : 
                EXCEPTION_CONTINUE_SEARCH )
	{
		return FALSE;	//exception occurred, return FALSE;
	}
#endif
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC CobjGetIDName(IDCOBJ id_, LPSTR szBuffer_, WORD wBufferSize_)
{
	WRD		_i;

	if(!szBuffer_)
		return FALSE;	//invalid string buffer

	for(_i=1; _i<gOTInfo.wAdding; _i++)
	{
		if(gOTInfo.lpBase[_i].wID == id_)
		{
			StringCopyn(szBuffer_, gOTInfo.lpBase[_i].szName, wBufferSize_);
			return TRUE;
		}
	}

	return FALSE;	//the id of object is not found
}

//...................................................................
//Description:
//	This function calls the object routine to edit the object
//	instance;
//Entries:
//
//	LPPU lpPU_		source PU
//	HWND hwnd_		HWND of the parent window of the object
//					routine windows
//Note:
//?? UNCERTAIN action, returns a BOOL ?
//
//...................................................................
_DLLEXP BOOL _DLLFUNC CobjAccess( LPPU lpPU_, HWND hwndParent_ )
{
	WORD		_i;
	SimplePU	_spu;
	WORD		_wMode = OA_RW;		//set initial value
	LPCOBJ		_lpCobj;
	IDCOBJ		_id;

	memset( &_spu, 0, sizeof(SimplePU) );
	_spu.prefix = PF_IMM|IMM_LPCOBJ;
	_spu.du.zzk__LPCOBJ = NULL;
	if( !PURead(&_spu, lpPU_) )
		return FALSE;

	_lpCobj = _spu.du.zzk__LPCOBJ;
	if( !_lpCobj )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	if( ! IsCobj( _lpCobj ) )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	//access mode is not supported yet
	_wMode = OA_RW;

	_id = GET_COBJ_ID( _lpCobj );
	for(_i=1; _i<gOTInfo.wAdding; _i++)
		if( gOTInfo.lpBase[_i].wID == _id )
			return (BOOL)(*gOTInfo.lpBase[_i].lpfnCobj)(lpPU_, _wMode, hwndParent_);

	return FALSE;
}

