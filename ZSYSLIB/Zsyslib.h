#ifndef __ZSYSLIB_H
#define __ZSYSLIB_H

#ifdef __cplusplus
extern "C" {
#endif

//since MSC there are not those math constants
#ifndef __BORLANDC__
	#define M_E         2.71828182845904523536
	#define M_LOG2E     1.44269504088896340736
	#define M_LOG10E    0.434294481903251827651
	#define M_LN2       0.693147180559945309417
	#define M_LN10      2.30258509299404568402
	#define M_PI        3.14159265358979323846
	#define M_PI_2      1.57079632679489661923
	#define M_PI_4      0.785398163397448309616
	#define M_1_PI      0.318309886183790671538
	#define M_2_PI      0.636619772367581343076
	#define M_1_SQRTPI  0.564189583547756286948
	#define M_2_SQRTPI  1.12837916709551257390
	#define M_SQRT2     1.41421356237309504880
	#define M_SQRT_2    0.707106781186547524401
#endif

//macro definitions
#define DUNIT2PIXELX(x)	((x)*LOWORD(GetDialogBaseUnits())/4)
#define DUNIT2PIXELY(y)	((y)*HIWORD(GetDialogBaseUnits())/8)

#define X(_dw_)         ((short)(LOWORD(_dw_)))
#define Y(_dw_)         ((short)(HIWORD(_dw_)))

typedef union tagPR{
	RECT	rect;
	POINT	point[2];
}PR;

#ifndef _WIN32
	//WIN16 here
	#define PostCommandMessage( _hwnd_, _id_, _notify_code_, _ctrl_hwnd_ )		\
				PostMessage(_hwnd_,												\
							WM_COMMAND,											\
							_id_,												\
							MAKELPARAM( _ctrl_hwnd_, _notify_code_ ) )

	#define GET_WM_COMMAND_CTRL_ID( wParam_, lParam_ )			( wParam_ )
	#define GET_WM_COMMAND_NOTIFY_CODE( wParam_, lParam_ )		( HIWORD(lParam_) )
	#define GET_WM_COMMAND_CTRL_HWND( wParam_, lParam_ )		( (HWND)LOWORD(lParam_) )
#else
	//WIN32 here
	#define PostCommandMessage( _hwnd_, _id_, _notify_code_, _ctrl_hwnd_ )		\
				PostMessage(_hwnd_,												\
							WM_COMMAND,											\
							MAKELONG( _id_, _notify_code_ ),           			\
							(LPARAM)(_ctrl_hwnd_) )


	#define GET_WM_COMMAND_CTRL_ID( wParam_, lParam_ )			( LOWORD(wParam_) )
	#define GET_WM_COMMAND_NOTIFY_CODE( wParam_, lParam_ )		( HIWORD(wParam_) )
	#define GET_WM_COMMAND_CTRL_HWND( wParam_, lParam_ )		( (HWND)(lParam_) )
#endif

//define mode_ parameter for Draw3DRect
//
//WH: 	white
//GR:	gray
//DG:	dark gray
//DK:	dark
//LG:	light gray
#define RECT3D_WEIGHT		0x00FF
#define RECT3D_WH_GR		0x0001
#define RECT3D_WH_DG		0x0002
#define RECT3D_WH_DK		0x0003
#define RECT3D_LG_DG		0x0004
#define RECT3D_LG_DK		0x0005
#define RECT3D_GR_DK		0x0006
#define RECT3D_BH_BS		0x0007	//use Button hilite and button shadow color

#define RECT3D_STYLE		0xFF00
#define RECT3D_UP			0x0100
#define RECT3D_DOWN			0x0200
#define RECT3D_BUMP			0x0300
#define RECT3D_DIP			0x0400

#define MEMCLR(_p_, _size_ )	memset( _p_, 0, _size_ )

_DLLEXP BOOL _ZZKDECL Draw3DRect(HDC hdc_, RECT FAR* lpRect_, WORD mode_);

//picture display mode
#define DM_FIX			0
#define DM_STRETCH		1
#define DM_TILE			2
#define DM_CENTER		3
#define DM_SMART		4

//DrawPicture flags
#define DPFLAG_REPORTERROR	0x00000001L
#define DPFLAG_NOTCLIP		0x00000002L		//only with DM_SMART

_DLLEXP BOOL _ZZKDECL DrawPicture( LPCSTR lpszFileName_, //picture file name
					  LPRECT lpRect_,		//the position to drawn
					  LPSTR lpszErr_,		//error string buffer
					  DWORD flags_,			//if error report, or if clip it
					  DWORD mode_,			//display mode
					  HDC hdc_ );			//display context

_DLLEXP BOOL _ZZKDECL DrawBmpPicture( LPCSTR lpszFileName_, 
									  LPRECT lpRect_, 
									  LPSTR lpszErr_, 
									  DWORD flags_,			//if error report, or if clip it
									  DWORD mode_, 
									  HDC hdc_ );
_DLLEXP BOOL _ZZKDECL DrawWmfPicture( LPCSTR lpszFileName, 
									  LPRECT lpRect_, 
									  LPSTR lpszErr_, 
									  DWORD flags_,			//if error report, or if clip it
									  DWORD mode_, 
									  HDC hdc_ );
_DLLEXP BOOL _ZZKDECL DrawErrorString(LPRECT lpRect_, 
										LPSTR lpszErr_, 
										HDC hdc_ );


#define MB_ERROR			(MB_OK|MB_ICONEXCLAMATION)
_DLLEXP void _ZZKDECL ShowError(LPCSTR szMsg_);

//Define LogOption:
#define LO_OUTPUT_FILE		0x0001		//log to file
#define LO_OUTPUT_DEBUG		0x0002		//log to debug screen
#define LO_FORMAT_DATE		0x0004		//include DATE field in report line
#define LO_FORMAT_TIME		0x0008		//include TIME field in report line
#define LO_COMPRESS			0x0010		//compress repeated reports, only for file output

#ifdef _WIN32
	_DLLEXP void _cdecl LogMessage(LPCSTR lpszMsgSign_, LPCSTR szMsg_, ... );
	_DLLEXP void _cdecl CoreOutputDebugString(LPCSTR szfn_, UINT ln_, LPCSTR szMsg_, ... );
#else
	void _export _cdecl LogMessage(LPCSTR lpszMsgSign_, LPCSTR szMsg_, ... );
	void _export _cdecl CoreOutputDebugString(LPCSTR szfn_, UINT ln_, LPCSTR szMsg_, ... );
#endif

#ifndef NDEBUG
	//debug version
	#define CORE_DEBUG0( msg )					CoreOutputDebugString( __FILE__, __LINE__, msg )
	#define CORE_DEBUG1( msg, s1 )				CoreOutputDebugString( __FILE__, __LINE__, msg, s1 )
	#define CORE_DEBUG2( msg, s1, s2 )			CoreOutputDebugString( __FILE__, __LINE__, msg, s1, s2 )
	#define CORE_DEBUG3( msg, s1, s2, s3 )		CoreOutputDebugString( __FILE__, __LINE__, msg, s1, s2, s3 )
	#define CORE_DEBUG4( msg, s1, s2, s3, s4 )	CoreOutputDebugString( __FILE__, __LINE__, msg, s1, s2, s3, s4 )
#else
	//release version
	#define CORE_DEBUG0( msg )					
	#define CORE_DEBUG1( msg, s1 )				
	#define CORE_DEBUG2( msg, s1, s2 )			
	#define CORE_DEBUG3( msg, s1, s2, s3 )		
	#define CORE_DEBUG4( msg, s1, s2, s3, s4 )
#endif

#define LOGMSG0( msg_ )					LogMessage( "M", msg_ )
#define LOGMSG1( msg_, s1 )				LogMessage( "M", msg_, s1 )
#define LOGMSG2( msg_, s1, s2 )			LogMessage( "M", msg_, s1, s2 )
#define LOGMSG3( msg_, s1, s2, s3 )		LogMessage( "M", msg_, s1, s2, s3 )
#define LOGMSG4( msg_, s1, s2, s3, s4 )	LogMessage( "M", msg_, s1, s2, s3, s4 )

#define LOGERR0( err_ )					LogMessage( "E", err_ )
#define LOGERR1( err_, s1 )				LogMessage( "E", err_, s1 )
#define LOGERR2( err_, s1, s2 )			LogMessage( "E", err_, s1, s2 )
#define LOGERR3( err_, s1, s2, s3 )		LogMessage( "E", err_, s1, s2, s3 )
#define LOGERR4( err_, s1, s2, s3, s4 )	LogMessage( "E", err_, s1, s2, s3, s4 )

#define LOGWARN0( wrn_ )				LogMessage( "W", wrn_ )
#define LOGWARN1( wrn_, s1 ) 			LogMessage( "W", wrn_, s1 ) 
#define LOGWARN2( wrn_, s1, s2 ) 		LogMessage( "W", wrn_, s1, s2 ) 
#define LOGWARN3( wrn_, s1, s2, s3 )	LogMessage( "W", wrn_, s1, s2, s3 ) 
#define LOGWARN4( wrn_, s1, s2, s3, s4)	LogMessage( "W", wrn_, s1, s2, s3, s4 ) 

_DLLEXP int _ZZKDECL StringCopyn(LPSTR s1_, LPCSTR s2_, size_t maxlen_);
_DLLEXP LPSTR  _ZZKDECL StringStripSpace(LPSTR szDst_, WORD wSize_, LPCSTR szSrc_ );

_DLLEXP LPSTR _ZZKDECL StringInsert(LPSTR szBuffer_, LPCSTR szInsert_, WORD wPosit_, WORD wSize_);
_DLLEXP BOOL _ZZKDECL InitZsyslib(void);

_DLLEXP void _ZZKDECL SetLogOption( DWORD newOption_ );
_DLLEXP DWORD _ZZKDECL GetLogOption( void );

_DLLEXP BOOL _ZZKDECL CoreCreateDirectory( LPCTSTR lpDir_ );

#ifdef  __cplusplus
}
#endif

#endif 	//__ZSYSLIB_H

