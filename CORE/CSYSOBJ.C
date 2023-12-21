//===================================================================
//csysobj.c
//	System Objects
//Zheng zhikai	96-8-6
//===================================================================

#include "prehead.h"

#include "core.h"
#include "cobj.h"

#include "csysobj.h"

_DLLEXP DWORD _DLLFUNC ObjectBinary(LPPU lpPU_, WORD wMode_, HWND hwnd_);
_DLLEXP DWORD _DLLFUNC ObjectString(LPPU lpPU_, WORD wMode_, HWND hwnd_);

//extern OTInfo		gOTInfo;

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC IsStringCobj( LPCOBJ lpCobj_ )
{
	DWORD	_dwSize, _i;
	
	//first, test if it is a valid cobj pointer
	if( !IsCobj(lpCobj_) )
		return FALSE;

	if( !lpCobj_ )
		return TRUE;
	
	//test id
	if( CobjGetID(lpCobj_) != IDCOBJ_STRING )
		return FALSE;

	//then test validity of the string
	_dwSize = CobjSize(lpCobj_);
	_i = 0;
	while( (_i<_dwSize) && (lpCobj_->data[_i])  )
		_i++;

	if( _i<_dwSize )
		return TRUE;
	else
		return FALSE;	//string is too long
}

_DLLEXP int _DLLFUNC CobjCompareString( LPCOBJ lpCobj1_, LPCOBJ lpCobj2_ )
{
	char	_data1, _data2;
	BOOL	_bNULL = FALSE;

	if( IsStringCobj( lpCobj1_ ) &&	IsStringCobj( lpCobj2_ ) )
	{
#ifdef WIN32
		__try{
#endif
		if( !lpCobj1_ )
		{
			_bNULL = TRUE;
			_data1 = 0;
		}
		else
			_data1 = lpCobj1_->data;

		if( !lpCobj2_ )
		{
			_bNULL = TRUE;
			_data2 = 0;
		}
		else
			_data2 = lpCobj2_->data;

		if( _bNULL && ( _data1 == _data2 ) )
			return 0;

		return strcmp( lpCobj1_->data, lpCobj2_->data );

#ifdef WIN32
		}
		__except( GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? 
					EXCEPTION_EXECUTE_HANDLER : 
					EXCEPTION_CONTINUE_SEARCH )
		{
			return 1;	//exception occurred, return 1;
		}
#endif
	}
	else
		return 1;	//error occurred, returns unequal
}

//...................................................................
//...................................................................
BOOL RegisterSystemObjects()
{
	static OTU _sOTUs[] = {
		{"Binary", 		IDCOBJ_BINARY, 		(void *)ObjectBinary},
		{"String", 		IDCOBJ_STRING, 		(void *)ObjectString},
	};

	int		_i;

	for(_i=0; _i < sizeof(_sOTUs)/sizeof(OTU); _i++)
		if(!RegisterCobj( &_sOTUs[_i]) )
			return FALSE;

	return TRUE;
}

//...................................................................
//...................................................................
#pragma argsused
_DLLEXP DWORD _DLLFUNC ObjectBinary(LPPU lpPU_, WORD wMode_, HWND hwnd_)
{
	MessageBox(hwnd_,
				"Sorry, not implemented yet!",
				"", MB_OK);

	return TRUE;
}

//...................................................................
//...................................................................
#pragma argsused
_DLLEXP DWORD _DLLFUNC ObjectString(LPPU lpPU_, WORD wMode_, HWND hwnd_)
{
	MessageBox(hwnd_,
				"Sorry, not implemented yet!",
				"", MB_OK);

	return TRUE;
}


