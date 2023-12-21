//===================================================================
//
//Zheng zhikai 97-07-17
//
//	Prepare for Core Project Compile definitions
//===================================================================

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
	#define MAKEPOINT(l) (* ChangePoint(l))

	#define GetWindowWord(l,p) GetWindowLong(l, p)

	#define MoveTo( hDC,x,y) MoveToEx(hDC,x,y,NULL )

	#define GWW_ID GWL_ID
	#define GWW_PARENT GWL_HWNDPARENT
#else
	//win16 here
	#define _DLLEXP
	#define _ZZKDECL
	#define _DLLFUNC	_export
	#define _NONEXPORTED
#endif

#ifdef _MSC_VER
	//using Microsoft C++ here (VC++)

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

