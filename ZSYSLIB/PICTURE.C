//===================================================================
//
//Zheng zhikai	95-11-6
//===================================================================
#include "prehead.h"

#include <tchar.h>
#include "zsyslib.h"

#define IS_WIN30_DIB(lpbi)  ((*(LPDWORD)(lpbi)) == sizeof(BITMAPINFOHEADER))

DWORD DIBWidth(LPSTR lpDIB);
DWORD DIBHeight(LPSTR lpDIB);

typedef struct _tagMETAFILEHEADER
{
	DWORD	key;
	HANDLE	hmf;
	RECT	bbox;
	WORD	inch;
	DWORD	res;
	WORD	checksum;
}METAFILEHEADER, _FAR* LPMETAFILEHEADER;

_DLLEXP BOOL _DLLFUNC DrawPicture( LPCSTR lpszFileName_, //picture file name
					  LPRECT lpRect_,		//the position to drawn
					  LPSTR lpszErr_,		//error string buffer
					  DWORD flags_,			//if error report, or if clip it
					  DWORD mode_,			//display mode
					  HDC hdc_ )			//display context
{
	char		_szExt[8];
	LPSTR		_lpdot;
	DWORD		_len;
	BOOL		_ret = FALSE;
	LPCSTR		_lpszFileName = lpszFileName_;

	if( lpszErr_ )
		*lpszErr_ = 0;

	if( !_lpszFileName )
		return FALSE;

	while( _istspace(*_lpszFileName) ) _lpszFileName++;
	if( !*_lpszFileName )	//is an empty string
		return FALSE;

	_lpdot = strrchr( lpszFileName_, '.' );
	_len = strlen( lpszFileName_ );

	if( ( lpszFileName_ + _len - _lpdot ) > 4 )
	{
		//file is no extern name
		if( lpszErr_ )
			strcpy( lpszErr_, "invalid file name" );
		goto _ERROR;
	}

	StringCopyn( _szExt, _lpdot+1, 4 );
	strlwr( _szExt );

	if( !strcmp( _szExt, "bmp" ) )
		_ret = DrawBmpPicture( lpszFileName_, lpRect_, lpszErr_, flags_, mode_, hdc_ );
	else if( !strcmp( _szExt, "wmf" ) )
		_ret = DrawWmfPicture( lpszFileName_, lpRect_, lpszErr_, flags_, mode_, hdc_ );
	else
	{
		strcpy( lpszErr_, "invalid file name" );
		_ret = FALSE;
	}

_ERROR:
	if( !_ret && (flags_ & DPFLAG_REPORTERROR) )
		DrawErrorString( lpRect_, lpszErr_, hdc_ );

	return _ret;
}

//...................................................................
//Description:
//	To display a BMP file in a rectangle which is specified by 
//	lpRect_, and the error string is retrieved via lpszErr_;
//
//Entries:
//	LPCSTR lpszFileName_
//				The BMP file name
//
//  LPRECT lpRect_
//				the position where the picture will be shown. if
//				the mode == DM_SMART, the new rectangle is also 
//				return via this
//
//  LPSTR lpszErr_
//				Pointer to a buffer to keep error string, and the
//				buffer size must be greater than 128 BYTES, or it
//				is equal to NULL
//
//  DWORD flags_
//				if error report, or if clip it
//  DWORD mode_
//				such as DM_FIX, DM_STRETCH, ...
//
//  HDC hdc_
//				the handle of display context
//
//Returns:
//	TRUE if successful, otherwise, FALSE is retrieved
//...................................................................
_DLLEXP BOOL _DLLFUNC DrawBmpPicture( LPCSTR lpszFileName_, 
					  LPRECT lpRect_, 
					  LPSTR lpszErr_, 
					  DWORD flags_,			//if error report, or if clip it
					  DWORD mode_, 
					  HDC hdc_ )
{
	RECT                    _rcClip;
	HRGN					_hrgn;
	BITMAPFILEHEADER        _bmfh;
	HFILE                   _hFile = NULL;
	OFSTRUCT                _of;
	HGLOBAL                 _hglb = NULL;
	RECT					_rcShow;

	BITMAPINFO FAR*         _lpbmi;
	BITMAPINFOHEADER FAR*   _lpbmih;
	BYTE FAR*               _lpvBits;
	BYTE FAR*               _lp = NULL;
	int						_mode = mode_;	
	int						_width, _height;	//for _rect
	int						_dib_width, _dib_height;
	BOOL					_changed = FALSE;
	BOOL					_clip_changed = FALSE;

	_hFile = OpenFile( lpszFileName_, &_of, OF_READ );
	if( _hFile == HFILE_ERROR )
	{
		if( lpszErr_ )
			strcpy( lpszErr_, "open file error" );
		goto _ERROR;
	}

	_lread( _hFile, &_bmfh, sizeof(_bmfh) );
	if( _bmfh.bfType != 'MB' )
	{
		if( lpszErr_ )
			strcpy( lpszErr_, "invalid bmp file" );
		goto _ERROR;
	}

	_hglb = GlobalAlloc( GMEM_MOVEABLE, _bmfh.bfSize - sizeof(_bmfh) );
	if( !_hglb )
	{
		if( lpszErr_ )
			strcpy( lpszErr_, "alloc mem error" );
		goto _ERROR;
	}

	_lp = GlobalLock(_hglb);
	if( !_lp )
	{
		if( lpszErr_ )
			strcpy( lpszErr_, "lock mem error" );
		goto _ERROR;
	}

	if( _lread( _hFile, _lp, (UINT)(_bmfh.bfSize - sizeof(_bmfh)) )
		== (UINT)HFILE_ERROR )
	{
		if( lpszErr_ )
			strcpy( lpszErr_, "read file error" );
		goto _ERROR;
	}

	_lpvBits = _lp + (UINT)(_bmfh.bfOffBits - sizeof(_bmfh)) ;
	_lpbmi = (BITMAPINFO FAR*)_lp;
	_lpbmih = (BITMAPINFOHEADER FAR*)_lp;

	_dib_width = DIBWidth(_lp);
	_dib_height = DIBHeight(_lp);

	_width = lpRect_->right - lpRect_->left;
	_height = lpRect_->bottom - lpRect_->top;

	if( (_mode == DM_STRETCH) ||(_mode == DM_SMART) )
	{
		if( _mode == DM_SMART )
		{
			_changed = FALSE;	//set initial value
			if( _width % _lpbmih->biWidth )
			{
				_changed = TRUE;
				_width = (WORD)((_width/_lpbmih->biWidth + 1) * _lpbmih->biWidth);
			}

			if( _height % _lpbmih->biHeight )
			{
				_changed = TRUE;
				_height = (WORD)((_height/_lpbmih->biHeight + 1) * _lpbmih->biHeight);
			}

			if( _changed )
			{
				lpRect_->right = lpRect_->left + _width;
				lpRect_->bottom = lpRect_->top + _height;
			}

			CopyRect( &_rcShow, lpRect_ );

			if( _changed && (flags_ & DPFLAG_NOTCLIP) )
			{
				_clip_changed = TRUE;
				//change the clip box
				//

				//using rgn is better than box?
				GetClipBox( hdc_, &_rcClip );
				_hrgn = CreateRectRgnIndirect( lpRect_ );
				SelectClipRgn( hdc_, _hrgn );
				DeleteObject( _hrgn );
			}
		}

		StretchDIBits(hdc_,
					  lpRect_->left,
					  lpRect_->top,
					  _width,
					  _height,
					  0,
					  0,
					  (UINT)_lpbmih->biWidth,
					  (UINT)_lpbmih->biHeight,
					  _lpvBits,
					  _lpbmi,
					  DIB_RGB_COLORS,
					  SRCCOPY);

		if( _clip_changed )
		{
			//restore the clip rect
			_hrgn = CreateRectRgnIndirect( &_rcClip );
			SelectClipRgn( hdc_, _hrgn );
			DeleteObject( _hrgn );
		}
	}
	else if( mode_ == DM_TILE )  
	{
		int _left, _top;

		for( _left = lpRect_->left; _left < lpRect_->right; _left += _dib_width )
			for( _top = lpRect_->top; _top < lpRect_->bottom; _top += _dib_height )
				SetDIBitsToDevice(hdc_,
								  _left,
								  _top,
								  _dib_width,
								  _dib_height,
								  0,
								  0,
								  0,
								  (UINT)_lpbmih->biHeight,
								  _lpvBits,
								  _lpbmi,
								  DIB_RGB_COLORS);
	}
	else // if( (mode_ == DM_CENTER) || mode_ == DM_FIX ) )
	{
		CopyRect( &_rcShow, lpRect_ );
		if( mode_ == DM_CENTER )
		{
			_rcShow.left += (_width - _dib_width ) / 2;
			_rcShow.top  += (_height - _dib_height ) / 2;
			_rcShow.right = _rcShow.left + _width;
			_rcShow.bottom = _rcShow.top + _height;
		}

		SetDIBitsToDevice(hdc_,
						  _rcShow.left,
						  _rcShow.top,
						  _width,
						  _height,
						  0,
						  0,
						  0,
						  (UINT)_lpbmih->biHeight,
						  _lpvBits,
						  _lpbmi,
						  DIB_RGB_COLORS);
		/****
		//get minimum of them
		_width = (int)( _width > _lpbmih->biWidth ? _lpbmih->biWidth : _width );
		_height = (int)( _height > _lpbmih->biHeight ? _lpbmih->biHeight : _height );
		****/
	}

	GlobalUnlock( _hglb );
	GlobalFree( _hglb );

	_lclose( _hFile );
	return TRUE;

_ERROR:
	_lclose( _hFile );
	return FALSE;
}

#ifndef _WIN32

void SetPlaceableExts(HDC hdc_, LPRECT lprcWin_, LPRECT lprcView_ )
{
	SetMapMode(hdc_, MM_ANISOTROPIC);

#ifndef _WIN32	
	SetWindowOrg(hdc_, lprcWin_->left, lprcWin_->top);
	SetWindowExt( hdc_,
				  lprcWin_->right - lprcWin_->left,
				  lprcWin_->bottom - lprcWin_->top );
#else
	SetWindowOrgEx(hdc_, lprcWin_->left, lprcWin_->top, NULL);
	SetWindowExtEx( hdc_,
				    lprcWin_->right - lprcWin_->left,
				    lprcWin_->bottom - lprcWin_->top,
					NULL );
#endif

#ifndef _WIN32
	SetViewportOrg(hdc_, lprcView_->left, lprcView_->top );
	SetViewportExt(	hdc_,
					lprcView_->right - lprcView_->left,
					lprcView_->bottom - lprcView_->top );
#else
	SetViewportOrgEx(hdc_, lprcView_->left, lprcView_->top, NULL );
	SetViewportExtEx(hdc_,
					 lprcView_->right - lprcView_->left,
					 lprcView_->bottom - lprcView_->top,
					 NULL );
#endif
	
	return;
}

//...................................................................
//Note:
//	Because this function is cut from other source code, here, this
//function for WIN16 is not corrected yet!
//
//Picture
//	for WIN16
//...................................................................
BOOL DrawWmfPicture( WRD wFTU_, HDC hdc_ )
{
	METAFILEHEADER         	_mfh;
	METAHEADER         		_mh;
	_hFile                   _hFile = NULL;
	OFSTRUCT                _of;
	HGLOBAL                 _hglb = NULL;
	BYTE FAR*               _lp = NULL;

	LPPU			_lpPrm;
	LPCOBJ			_lpCobj = NULL;
	RECT			_rect;
	//HMETAFILE		_hmf;
	HANDLE			_hmf;
	//struct find_t	_find_t;

	_lpPrm = FaceGetParameter( wFTU_ );
	FaceXycxcyToRect( wFTU_, &_rect );
	MFPUREAD( _lpPrm, PICT_NAME, LPCOBJ, _lpCobj );

	_hFile = OpenFile( (LPSTR)GET_COBJ_DATA(_lpCobj), &_of, OF_READ );
	if( _hFile == HFILE_ERROR )
	{
		return _PicturePaintNone( wFTU_, hdc_, "open file error" );
	}

	_lread( _hFile, &_mfh, sizeof(_mfh) );
	if( _mfh.key != 0x9AC6CDD7L )
		return _PicturePaintNone( wFTU_, hdc_, "invalid .wmf file" );

	_lread( _hFile, &_mh, sizeof(_mh) );
	_hglb = GlobalAlloc( GMEM_MOVEABLE /* |GMEM_ZEROINIT */, _mh.mtSize * 2 );
	if( !_hglb )
	{
		if( _hFile ) _lclose( _hFile );
		return _PicturePaintNone( wFTU_, hdc_, "alloc mem error" );
	}

	_lp = GlobalLock(_hglb);
	if( !_lp )
	{
		if( _hFile ) _lclose( _hFile );
		if( _hglb ) GlobalFree( _hglb );
		return _PicturePaintNone( wFTU_, hdc_, "lock mem error" );
	}

	_llseek( _hFile, sizeof(_mfh), 0 );

	if( _lread( _hFile, _lp, (UINT)(_mh.mtSize * 2) )
		== (UINT)HFILE_ERROR )
	{
		if( _hFile ) _lclose( _hFile );
		if( _lp ) GlobalUnlock( _hglb );
		if( _hglb ) GlobalFree( _hglb );
		return _PicturePaintNone( wFTU_, hdc_, "read file error" );
	}

	_hmf = SetMetaFileBits( _hglb );	//(HMETAFILE)
	if( !_hmf )
	{
		if( _hFile ) _lclose( _hFile );
		return FALSE;
	}

	if( !SaveDC( hdc_ ) )
	{
		if( _hFile ) _lclose( _hFile );
		return FALSE;
	}

	SetPlaceableExts( hdc_, &_mfh.bbox, &_rect );
	PlayMetaFile( hdc_, _hmf );
	DeleteMetaFile( _hmf );

	RestoreDC( hdc_, -1 );

	if( _hFile ) _lclose( _hFile );

	//Note:
	//	should not free the mem block because the memory owned by window now

	return TRUE;
}

#else	//now, WIN32

HENHMETAFILE LoadMetafile( LPCSTR szFile, HDC hdc_, LPSTR lpszErr_ );

//...................................................................
//Description:
//	To display to WMF and enhanced WMF file
//
//Picture
//	for WIN32
//...................................................................
_DLLEXP BOOL _DLLFUNC DrawWmfPicture( LPCSTR lpszFileName_, 
					  LPRECT lpRect_, 
					  LPSTR lpszErr_, 
					  DWORD flags_,			//if error report, or if clip it
					  DWORD mode_, 
					  HDC hdc_ )
{
	char			_szBuffer[128];
	HENHMETAFILE	_hemf;

	_szBuffer[0] = 0;
	_hemf = LoadMetafile( lpszFileName_, hdc_, _szBuffer );

	if(_hemf)
	{
		RECT	_rc;
		CopyRect( &_rc, lpRect_ );
		_rc.bottom--, _rc.right--;	//in order not to reach right and bottom edges
		PlayEnhMetaFile( hdc_, _hemf, &_rc );
		DeleteEnhMetaFile(_hemf);  
		return TRUE;
	}
	else
	{
		if( lpszErr_ )
			strcpy( lpszErr_, _szBuffer );
		return FALSE;
	}
}
#endif

#ifdef _WIN32
HENHMETAFILE LoadMetafile( LPCSTR szFile, HDC hdc_, LPSTR lpszErr_ )
{
	LPMETAFILEHEADER        _pmfh;
	LPMETAHEADER         	_pmh;

	HANDLE					_hFile, _hMapFile;
	LPVOID					_pMapFile;
	LPENHMETAHEADER			_pemh;
	HENHMETAFILE			_hemf;
	BOOL					_bOkey;
	
	_bOkey=FALSE;

	if ((_hFile = CreateFile(szFile, GENERIC_READ, FILE_SHARE_READ, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL)) == (HANDLE)-1) 
	{
		strcpy( lpszErr_, "open file error" );
		CloseHandle(_hFile);
		return NULL; 
	}
	
	// Create a map file of the opened file
	if ((_hMapFile = CreateFileMapping(_hFile, NULL,
							 PAGE_READONLY, 0, 0, NULL)) == (HANDLE)-1) 
	{
		strcpy( lpszErr_, "mapping file error" );
		CloseHandle(_hMapFile);
		return NULL; 
	}

	// Map a view of the whole file
	if ((_pMapFile = MapViewOfFile(_hMapFile, FILE_MAP_READ, 0, 0, 0)) == NULL)
	{
		strcpy( lpszErr_, "mapping view error" );
		CloseHandle(_hMapFile);
		return NULL; 
	}

	// First check that if it is an enhanced metafile
	_pemh = (LPENHMETAHEADER) _pMapFile;
	
	if (_pemh->dSignature == ENHMETA_SIGNATURE ) 
	{
		_hemf = GetEnhMetaFile(szFile);
		if(!_hemf)
			goto _EXIT_ERROR;
		else
			_bOkey=TRUE;
			goto _EXIT;
	}
	
	_pmfh = (LPMETAFILEHEADER) _pMapFile;
	_pmh = (LPMETAHEADER)( (PBYTE)_pMapFile + sizeof(_pMapFile) );
	if( _pmfh->key == 0x9AC6CDD7L )
	{
		_hemf = SetWinMetaFileBits( _pmh->mtSize*2L, (LPBYTE)_pmh, hdc_, NULL);
		if (!_hemf) 
			goto _EXIT_ERROR;
		else
		{
			_bOkey=TRUE;
			goto _EXIT;
		}
	}
	else
	{
		strcpy( lpszErr_, "invalid file format" );
	}

_EXIT_ERROR:
_EXIT:
	UnmapViewOfFile(_pMapFile);
	CloseHandle(_hMapFile);
	CloseHandle(_hFile);
		
	if (_bOkey)
		return _hemf;
	else
		return NULL;
}
#endif

//...................................................................
//Picture
//	draw an error object
//...................................................................
_DLLEXP BOOL _DLLFUNC DrawErrorString(LPRECT lpRect_, LPSTR lpszErr_, HDC hdc_ )
{
	int			_old_rop2;
	int			_old_bkmode;
	HBRUSH		_old_brush;

	_old_rop2 = SetROP2( hdc_, R2_NOT );
	_old_brush = SelectObject( hdc_, GetStockObject(NULL_BRUSH) );
	_old_bkmode = SetBkMode( hdc_, TRANSPARENT );

	DrawText( hdc_, lpszErr_, -1, lpRect_, DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_CENTER );

	Rectangle( hdc_, lpRect_->left, lpRect_->top, lpRect_->right, lpRect_->bottom );
	MoveTo( hdc_, lpRect_->left, lpRect_->top );
	LineTo( hdc_, lpRect_->right, lpRect_->bottom );
	MoveTo( hdc_, lpRect_->right, lpRect_->top );
	LineTo( hdc_, lpRect_->left, lpRect_->bottom );

	//restore the HDC
	SetBkMode( hdc_, _old_bkmode );
	SetROP2( hdc_, _old_rop2 );
	SelectObject( hdc_, _old_brush );

	return TRUE;
}

//...................................................................
//DIBHeight()
//
//Description:
//
//	This function gets the height of the DIB from the BITMAPINFOHEADER
//	height field if it is a Windows 3.0-style DIB or from the BITMAPCOREHEADER
//	height field if it is an other-style DIB.
//
//Parameter:
//
//	LPSTR lpbi       - pointer to packed-DIB memory block
//
//Return Value:
//
//	DWORD            - height of the DIB
//
//...................................................................
DWORD DIBHeight(LPSTR lpDIB)
{
	LPBITMAPINFOHEADER lpbmi;  // pointer to a Win 3.0-style DIB
	LPBITMAPCOREHEADER lpbmc;  // pointer to an other-style DIB

	/* point to the header (whether old or Win 3.0 */

	lpbmi = (LPBITMAPINFOHEADER)lpDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpDIB;

	/* return the DIB height if it is a Win 3.0 DIB */
	if (IS_WIN30_DIB(lpDIB))
		return lpbmi->biHeight;
	else  /* it is an other-style DIB, so return its height */
		return (DWORD)lpbmc->bcHeight;
}

//...................................................................
//
//DIBWidth()
//
//Description:
//
//	This function gets the width of the DIB from the BITMAPINFOHEADER
//	width field if it is a Windows 3.0-style DIB or from the BITMAPCOREHEADER
//	width field if it is an other-style DIB.
//
//Parameter:
//
//	LPSTR lpbi       - pointer to packed-DIB memory block
//
//Return Value:
//
//	DWORD            - width of the DIB
//
//...................................................................
DWORD DIBWidth(LPSTR lpDIB)
{
	LPBITMAPINFOHEADER lpbmi;  // pointer to a Win 3.0-style DIB
	LPBITMAPCOREHEADER lpbmc;  // pointer to an other-style DIB

	/* point to the header (whether Win 3.0 and old) */

	lpbmi = (LPBITMAPINFOHEADER)lpDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpDIB;

	/* return the DIB width if it is a Win 3.0 DIB */
	if (IS_WIN30_DIB(lpDIB))
		return lpbmi->biWidth;
	else  /* it is an other-style DIB, so return its width */
		return (DWORD)lpbmc->bcWidth;
}
