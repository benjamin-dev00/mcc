//===================================================================
//
//Zheng zhikai 97-07-17
//
//	Prepare for Core Project Compile definitions
//===================================================================

#ifndef _CPCDEF_H__
#define _CPCDEF_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
	#define _near
	#define _FAR
	#define _far

	#define _DLLFUNC		_stdcall
	#define _ZZKDECL		_stdcall
	#define _NONEXPORTED	_stdcall

	#ifdef _CORE_EXE	//this is a predefine sym for indicating producing .exe file
		#define _DLLEXP __declspec( dllimport )
	#else	//here while producing .dll file
		#define _DLLEXP __declspec( dllexport )
	#endif


	//add by sc so that ensure the function is compiled only once
	_DLLEXP POINT *  _ZZKDECL ChangePoint( DWORD var_ );
	//#define MAKEPOINT(l) (* ChangePoint(l))

	#define GetWindowWord(l,p) GetWindowLong(l, p)
	#define GET_WINDOW_ID(hwnd_)	( (int)GetWindowLong(hwnd_, GWL_ID) )


	#define MoveTo( hDC,x,y) MoveToEx(hDC,x,y,NULL )

	#define GWW_ID GWL_ID
	#define GWW_PARENT GWL_HWNDPARENT
#else
	//win16 here
	#define _DLLEXP
	#define _ZZKDECL
	#define _DLLFUNC	_export
	#define _NONEXPORTED

	#define GET_WINDOW_ID(hwnd_)	( (int)GetWindowWord(hwnd_, GWW_ID) )

#endif

#ifdef _MSC_VER
	//using Microsoft C/C++ here (VC/C++)

	//warnings:
	#pragma warning( 3       : 4706 )	//	assignment within conditional expression
	#pragma warning( disable : 4047 )	//	'identifier1' : 'operator' : different levels of indirection from 'identifier2'
	#pragma warning( disable : 4068 )	//	unknown pragma
	
	//add by sc for file operator function change from bc to vc
	#define MAXPATH	 _MAX_PATH
	#define MAXDRIVE _MAX_DRIVE
	#define MAXDIR   _MAX_DIR
	#define MAXFILE  _MAX_FNAME
	#define MAXEXT   _MAX_EXT
	#define fnsplit _splitpath
	#define fnmerge _makepath

	//#define CORE_ASSERT assert
#endif	//_MSC_VER

#define ASSIGN_POINT( _pt_, _dw_ )		\
	{									\
		(_pt_).x = (short)LOWORD(_dw_);	\
		(_pt_).y = (short)HIWORD(_dw_);	\
	}

#ifdef  __cplusplus
}	//extern "C"
#endif

#endif //_CPCDEF_H__
