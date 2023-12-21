//===================================================================
//
//Zheng zhikai	95-11-6
//===================================================================
#include "prehead.h"

#include <tchar.h>
#include <dos.h>
#include <stdarg.h>

#ifdef	__BORLANDC__	//borland c here
	#include <dir.h>
	#include "zsyslibb.rh"
#else
	#include "zsysres.h"
#endif

#ifndef	__ZSYSLIB_H
#include "zsyslib.h"
#endif

#ifndef __FORM_H
#include "form.h"
#endif

LRESULT CALLBACK ZzkFormWndProc(HWND hwnd_, UINT msg_, WPARAM wParam_, LPARAM lParam_);

BOOL _near _InitZsyslib(void);

static HFILE 	shErrLogFile;

static DWORD	sdwLogOption = 
#ifndef NDEBUG
	(LO_COMPRESS|LO_OUTPUT_DEBUG);
#else
	(LO_COMPRESS|LO_OUTPUT_FILE|LO_FORMAT_DATE|LO_FORMAT_TIME);
#endif

HINSTANCE		ghInst;

//...................................................................
//...................................................................
#ifndef WIN32

#pragma argsused
int WINAPI LibMain(HINSTANCE	hInstance_,
				   WORD			wDataSegment_,
				   WORD			wHeapSize_,
				   LPSTR		lpszCmdLine_)
{
	ghInst = hInstance_;
	//if( wHeapSize_ != 0)
	//	UnlockData(0);

	if( !_InitZsyslib() )
		return FALSE;

	return 1;	// return result 1 = success; 0 = fail
}

//...................................................................
//...................................................................
#pragma argsused
int WINAPI WEP (int  bSystemExit)
{
	_lclose( shErrLogFile );
	return 1;
}

#else
	//WIN32 here
BOOL APIENTRY DllMain(HANDLE hModule,
					  DWORD  ul_reason_for_call,
					  LPVOID lpReserved)
{
	switch( ul_reason_for_call )
	{
		case DLL_PROCESS_ATTACH:
			ghInst = hModule;
			if( !_InitZsyslib() )
				return FALSE;
			break;

		case DLL_THREAD_ATTACH:
			break;

		case DLL_THREAD_DETACH:
			break;

		case DLL_PROCESS_DETACH:
			_lclose( shErrLogFile );
			break;
	}
	return TRUE;
}

#endif	//endif define WIN32


//...................................................................
//...................................................................
BOOL _near _InitZsyslib()
{
	char		_szModuleName[MAXPATH];
	char		_szBuffer[MAXPATH];
	LPSTR		_pszFileName;
	OFSTRUCT	_of;
	WNDCLASS 	_wc;

	GetModuleFileName( ghInst, _szModuleName, sizeof(_szModuleName) );
	GetFullPathName( _szModuleName, sizeof(_szBuffer), _szBuffer, &_pszFileName );
	strcpy( _pszFileName, "corelog.txt" );

	shErrLogFile = OpenFile( _szBuffer, &_of, OF_WRITE|OF_SHARE_COMPAT );

_again:
	if( shErrLogFile == HFILE_ERROR )
	{
		if( _of.nErrCode == 2)	//file no found
		{
			shErrLogFile = OpenFile( _szBuffer, &_of, OF_CREATE|OF_SHARE_COMPAT );
			goto _again;
		}
		return FALSE;
	}

	_llseek(shErrLogFile, 0L, 2 );	//move to end of file for appending

	//register global general form table window
	_wc.hInstance     = ghInst;
	_wc.lpfnWndProc   = ZzkFormWndProc;
	_wc.hIcon         = LoadIcon( ghInst, MAKEINTRESOURCE(ICON_FORM) );
	_wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	_wc.style		  = CS_DBLCLKS|CS_GLOBALCLASS;
	_wc.lpszMenuName  = NULL;
	_wc.cbClsExtra    = 0;
	_wc.cbWndExtra    = sizeof(VOID FAR*);
	_wc.lpszClassName = ZZKFORMCLASSNAME;
	_wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	if( !RegisterClass(&_wc) )
		return FALSE;

	return TRUE;
}

//...................................................................
//Description:
//Note:
//	The figure this function draws extends up to, but does not include,
//	the right and bottom coordinates. This means that the height of
//	the figure is nBottomRect - nTopRect and the width of the figure is
//	nRightRect - nLeftRect.
//	this action is as same as Window function: Rectangle(...)
//...................................................................
_DLLEXP BOOL _DLLFUNC Draw3DRect(HDC hdc_, RECT FAR* lpRect_, WORD mode_)
{
	HPEN	_hLPen;
	HPEN	_hSPen;
	HPEN	_hOldPen;
	RECT	_rc;

	switch(mode_ & RECT3D_WEIGHT)
	{
		case RECT3D_LG_DG:
			_hLPen= CreatePen(PS_SOLID, 1, RGB(0xC0, 0xC0, 0xC0));
			_hSPen= CreatePen(PS_SOLID, 1, RGB(0x40, 0x40, 0x40));
			break;

		case RECT3D_LG_DK:
			_hLPen= CreatePen(PS_SOLID, 1, RGB(0xC0, 0xC0, 0xC0));
			_hSPen= CreatePen(PS_SOLID, 1, RGB(0x00, 0x00, 0x00));
			break;

		case RECT3D_GR_DK:
			_hLPen= CreatePen(PS_SOLID, 1, RGB(0x80, 0x80, 0x80));
			_hSPen= CreatePen(PS_SOLID, 1, RGB(0x00, 0x00, 0x00));
			break;

		case RECT3D_WH_GR:
			_hLPen= CreatePen(PS_SOLID, 1, RGB(0xFF, 0xFF, 0xFF));
			_hSPen= CreatePen(PS_SOLID, 1, RGB(0x80, 0x80, 0x80));
			break;

		case RECT3D_WH_DG:
			_hLPen= CreatePen(PS_SOLID, 1, RGB(0xFF, 0xFF, 0xFF));
			_hSPen= CreatePen(PS_SOLID, 1, RGB(0x40, 0x40, 0x40));
			break;

		case RECT3D_WH_DK:
			_hLPen= CreatePen(PS_SOLID, 1, RGB(0xFF, 0xFF, 0xFF));
			_hSPen= CreatePen(PS_SOLID, 1, RGB(0x00, 0x00, 0x00));
			break;

		case RECT3D_BH_BS:
		default:
			_hLPen= CreatePen(PS_SOLID, 1, GetSysColor(COLOR_BTNHIGHLIGHT) );
			_hSPen= CreatePen(PS_SOLID, 1, GetSysColor(COLOR_BTNSHADOW) );
			break;
	}

	_rc.left = lpRect_->left;
	_rc.top  = lpRect_->top;
	_rc.right = lpRect_->right - 1;
	_rc.bottom = lpRect_->bottom - 1;

	switch(RECT3D_STYLE & mode_)
	{
		case RECT3D_DOWN:
			_hOldPen = SelectObject(hdc_, _hSPen);
			MoveTo(hdc_, _rc.left, _rc.bottom);
			LineTo(hdc_, _rc.left, _rc.top);
			LineTo(hdc_, _rc.right, _rc.top);

			SelectObject(hdc_, _hLPen);
			LineTo(hdc_, _rc.right, _rc.bottom);
			LineTo(hdc_, _rc.left, _rc.bottom);
			break;

		case RECT3D_BUMP:
			_hOldPen = SelectObject(hdc_, _hLPen);
			MoveTo(hdc_, _rc.left, _rc.bottom);
			LineTo(hdc_, _rc.left, _rc.top);
			LineTo(hdc_, _rc.right, _rc.top);

			SelectObject(hdc_, _hSPen);
			LineTo(hdc_, _rc.right, _rc.bottom);
			LineTo(hdc_, _rc.left, _rc.bottom);

			MoveTo(hdc_, _rc.left + 2, _rc.bottom - 2);
			LineTo(hdc_, _rc.left + 2, _rc.top + 2);
			LineTo(hdc_, _rc.right - 2, _rc.top + 2);

			SelectObject(hdc_, _hLPen);
			LineTo(hdc_, _rc.right - 2, _rc.bottom - 2);
			LineTo(hdc_, _rc.left + 2, _rc.bottom - 2);
			break;

		case RECT3D_DIP:
			_hOldPen = SelectObject(hdc_, _hSPen);
			MoveTo(hdc_, _rc.left, _rc.bottom);
			LineTo(hdc_, _rc.left, _rc.top);
			LineTo(hdc_, _rc.right, _rc.top);

			SelectObject(hdc_, _hLPen);
			LineTo(hdc_, _rc.right, _rc.bottom);
			LineTo(hdc_, _rc.left, _rc.bottom);

			MoveTo(hdc_, _rc.left + 2, _rc.bottom - 2);
			LineTo(hdc_, _rc.left + 2, _rc.top + 2);
			LineTo(hdc_, _rc.right - 2, _rc.top + 2);

			SelectObject(hdc_, _hSPen);
			LineTo(hdc_, _rc.right - 2, _rc.bottom - 2);
			LineTo(hdc_, _rc.left + 2, _rc.bottom - 2);
			break;

		case RECT3D_UP:
		default:	//default is RECT3D_UP
			_hOldPen = SelectObject(hdc_, _hLPen);
			MoveTo(hdc_, _rc.left, _rc.bottom);
			LineTo(hdc_, _rc.left, _rc.top);
			LineTo(hdc_, _rc.right, _rc.top);

			SelectObject(hdc_, _hSPen);
			LineTo(hdc_, _rc.right, _rc.bottom);
			LineTo(hdc_, _rc.left, _rc.bottom);
			break;
	}

	SelectObject(hdc_, _hOldPen);
	DeleteObject(_hLPen);
	DeleteObject(_hSPen);

	return TRUE;
}

//...................................................................
//...................................................................
_DLLEXP void _DLLFUNC ShowError(LPCSTR szErr_)
{
	MessageBox(NULL, szErr_, "Error", MB_ERROR);
	return;
}

//...................................................................
//...................................................................
#ifdef _WIN32
	_DLLEXP void _cdecl 
#else
	void _export _cdecl 
#endif
CoreOutputDebugString(LPCSTR szfn_, UINT ln_, LPCSTR szMsg_, ... )
{
	char		_szBuffer[128];
	va_list 	_argptr;

	sprintf( _szBuffer, "%s, %u, ", szfn_, ln_ );
	
	va_start( _argptr, szMsg_ );
	vsprintf( _szBuffer + strlen(_szBuffer), szMsg_, _argptr );
	va_end( _argptr );

	/****
	if( *(_szBuffer + strlen(_szBuffer) - 1) != '\n' )
		strcat( _szBuffer, "\n" );
	****/

	OutputDebugString( _szBuffer );
	
	return;
}

//...................................................................
//...................................................................
#ifdef _WIN32
	_DLLEXP void _cdecl 
#else
	void _export _cdecl 
#endif
LogMessage(LPCSTR lpszMsgSign_, LPCSTR szMsg_, ... )
{
	static char		_szBufferOld[512];
	static char		_szBuffer[512];
	static char		_szDate[] = "9999-99-99 ";
	static char		_szTime[] = "99:99:99 ";

	va_list 		_argptr;

#ifdef WIN32
	SYSTEMTIME	_st;
	GetLocalTime(&_st);

	sprintf(_szDate, "%04d-%02d-%02d ", _st.wYear, _st.wMonth, _st.wDay );
	sprintf(_szTime, "%02d:%02d:%02d ", _st.wHour, _st.wMinute, _st.wSecond );
#else
	struct dosdate_t	_dosdate;
	struct dostime_t	_dostime;
	_dos_getdate( &_dosdate );
	_dos_gettime( &_dostime );

	sprintf(_szDate, "%04d-%02d-%02d ", _dosdate.year, _dosdate.month, _dosdate.day );
	sprintf(_szTime, "%02d:%02d:%02d ", _dostime.hour, _dostime.minute, _dostime.second );
#endif

	strcpy( _szBuffer, lpszMsgSign_ );
	strcat( _szBuffer, " " );
	if( sdwLogOption & LO_FORMAT_DATE )
		strcat( _szBuffer, _szDate );
	if( sdwLogOption & LO_FORMAT_TIME )
		strcat( _szBuffer, _szTime );

	va_start( _argptr, szMsg_ );
	vsprintf( _szBuffer + strlen(_szBuffer), szMsg_, _argptr );
	va_end( _argptr );

	strcat( _szBuffer, "\r\n" );

	if( sdwLogOption & LO_OUTPUT_FILE )
	{
		if( !(sdwLogOption & LO_COMPRESS) || strncmp(_szBufferOld, _szBuffer, sizeof(_szBuffer) ) )
			_lwrite( shErrLogFile, _szBuffer, strlen(_szBuffer) );

		strcpy( _szBufferOld, _szBuffer );
	}

#ifdef _DEBUG
	OutputDebugString( _szBuffer );
#else
	if( sdwLogOption & LO_OUTPUT_DEBUG )
		OutputDebugString( _szBuffer );
#endif

	return;
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC ZzkAssertFail(LPCSTR file_,
							int line_)
{
	static	BOOL	_sbIsAssert;

	char	_szBuffer[120];
	UINT	_ret;
	BOOL	_code;

	if( _sbIsAssert )
		return FALSE;	//

	_sbIsAssert = TRUE;

	wsprintf(_szBuffer, "Assert fail:  %s,  %i", file_, line_);
	//LogMessage( "E", _szBuffer ); 
	LOGMSG0(_szBuffer);	
	_ret = MessageBox( NULL, _szBuffer, "Assert failed", MB_ABORTRETRYIGNORE );
	if( _ret == IDABORT )
	{
		abort();
		_code = FALSE;
	}
	else if( _ret == IDRETRY )
	{
		_code = TRUE;	//trap into debug break!
	}
	else	//ignore it, do nothing here 
		_code = FALSE;

	_sbIsAssert = FALSE;

	return _code;
}

//...................................................................
//Return:
//	returns the number of characters copied, included terminator.
//...................................................................
_DLLEXP int  _DLLFUNC StringCopyn(LPSTR s1_, LPCSTR s2_, size_t maxlen_)
{
	UINT	_i;

	//CORE_ASSERT( s1_ && s2_ );
	if( !s1_ )
		return 0;

	if( !s2_ )
	{
		*s1_ = 0;
		return 0;
	}

	for(_i=0;
		_i<maxlen_;
		_i++, s1_++, s2_++)
	{
		*s1_ = *s2_;
		if( *s2_ == 0 )
			return (_i+1);
	}

	s1_--;
	*s1_ = 0;

	return _i;
}

//...................................................................
//Description:
//	Insert a string to another string
//Entry:
//	LPSTR szBuffer_		string to be insert
//	LPSTR szInsert_		string to insert
//	WORD wPosit_		position that begins to insert in string szBuffer_
//	WORD wSize_			sizeof the szBuffer
//...................................................................
_DLLEXP LPSTR  _DLLFUNC StringInsert(LPSTR szBuffer_, LPCSTR szInsert_, WORD wPosit_, WORD wSize_)
{
	unsigned	_len = strlen(szInsert_);
	LPSTR		_p, _end;
	LPCSTR		_c;

	CORE_ASSERT(wPosit_ < wSize_);

	if(wSize_ > wPosit_ + _len)
		memmove(szBuffer_ + wPosit_ + _len,
				szBuffer_ + wPosit_,
				wSize_ - wPosit_ - _len);

	_c = szInsert_;
	_p = szBuffer_ + wPosit_;
	_end = szBuffer_ + wSize_ - 1;
	while( *_c && (_p <_end) )*_p++ = *_c++;

	*_end = 0;	//ASCIIZ string terminator char

	return szBuffer_;
}

//...................................................................
//Description: 
//	strip the head and tail white space characters
//Entry:
//	LPSTR szDst_	the destinction string buffer
//	WORD wSize_		the size of destinction string buffer
//	LPCSTR szSrc_ 	the source string	
//...................................................................
_DLLEXP LPSTR  _DLLFUNC StringStripSpace(LPSTR szDst_, WORD wSize_, LPCSTR szSrc_ )
{
	LPTSTR	_lpszDup;

	LPTSTR lpsz;
	LPTSTR lpszStart;
	LPTSTR lpszLast;

	_lpszDup =  _tcsdup( szSrc_ );

	// find first non-space character
	lpszStart = (LPTSTR)_lpszDup;
	while (_istspace(*lpszStart))
		lpszStart = _tcsinc(lpszStart);

	// find beginning of trailing spaces by starting at beginning (DBCS aware)
	lpsz = lpszStart;
	lpszLast = NULL;
	while (*lpsz != '\0')
	{
		if (_istspace(*lpsz))
		{
			if (lpszLast == NULL)
				lpszLast = lpsz;
		}
		else
			lpszLast = NULL;
		lpsz = _tcsinc(lpsz);
	}

	if (lpszLast != NULL)
	{
		// truncate at trailing space start
		*lpszLast = '\0';
	}

	StringCopyn( szDst_, lpszStart, wSize_ );
	free(_lpszDup);

	return szDst_;
}

_DLLEXP void _DLLFUNC SetLogOption( DWORD newOption_ )
{
	sdwLogOption = newOption_;
	return;
}

_DLLEXP DWORD _DLLFUNC GetLogOption( void )
{
	return sdwLogOption;
}

//...................................................................
//Note:
//	If the directory already exists, this function return TRUE;
//...................................................................
_DLLEXP BOOL _DLLFUNC CoreCreateDirectory( LPCTSTR lpDir_ )
{
	if( !CreateDirectory( lpDir_, NULL ) )
	{
		TCHAR	_szBuffer[MAX_PATH];
		TCHAR*	_lp;

		if( GetLastError() == ERROR_ALREADY_EXISTS )
			return TRUE;

		strcpy( _szBuffer, lpDir_ );

		 _lp = strrchr( _szBuffer, '\\' );
		if( !_lp )
			return FALSE;
		else
		{
			*_lp = 0;
			if( !CoreCreateDirectory( _szBuffer ) )
				return FALSE;
			else
				return CreateDirectory( lpDir_, NULL );
		}
	}
	else
		return TRUE;
}
