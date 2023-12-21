/////////////////////////////////////////////////////////////////////////////
// ACR.C:
// 

#include "_acr.h"

SHAREFILEINFO	gFloatFile;
SHAREFILEINFO	gLongFile;
SHAREFILEINFO	gBoolFile;

#define	Wait_For_Single_Object( _handle_, _tmo_ )	WaitForSingleObject( _handle_, _tmo_ )

/*

#define	Wait_For_Single_Object( _handle_, _tmo_ )		\
	{		\
		DWORD	_ret;		\
		_ret = WaitForSingleObject( _handle_, _tmo_ );		\
		LOGMSG1( "Wait_For_Single_Object, ret=%s", 		\
				 (_ret == WAIT_ABANDONED)? "WAIT_ABANDONED" :		\
				 (_ret == WAIT_OBJECT_0 )? "WAIT_OBJECT_0 " : 		\
				 (_ret == WAIT_TIMEOUT )? "WAIT_TIMEOUT" : "WAIT_FAILED" );		\
	}

*/

///////////////////////////////////////////////////////////
void _ExitAccessCorebase( LPCSTR lpszName_ )
{
	_ResetDirtyFloat();
	_Exit( &gFloatFile );

	_ResetDirtyLong();
	_Exit( &gLongFile );

	_ResetDirtyBool();
	_Exit( &gBoolFile );

	return;
}

void _Exit( LPSHAREFILEINFO lpsfi_ )
{
	int		_i;
	DWORD	_pid = GetCurrentProcessId();
	LPSHAREFILEHEAD		_lpSfh = (LPSHAREFILEHEAD)(lpsfi_->m_lpMapAddr);

	//unregister myself first
	Wait_For_Single_Object( lpsfi_->m_hMutex, 500 );
	for( _i=0; _i<32; _i++ )
	{
		if( _lpSfh->Pid[_i] == _pid )
		{
			_lpSfh->Pid[_i] = 0;
			break;
		}
	}
	ReleaseMutex( lpsfi_->m_hMutex );

	if( !UnmapViewOfFile( lpsfi_->m_lpMapAddr ) )
	{
		LOGERR1( "ACR: Unmapping failed: err=%lu", GetLastError() );
	}

	CloseHandle( lpsfi_->m_hMapFile );
	CloseHandle( lpsfi_->m_hMutex );

	return;
}

///////////////////////////////////////////////////////////
BOOL _InitializeAccessCorebase( LPCSTR lpszName_ )
{
	char	_szBuffer[256];

	sprintf( _szBuffer, "ZZK.COREBASE.FLOAT.%s", lpszName_ );
	gFloatFile.m_Type = SF_FLOAT;
	gFloatFile.m_dwSize = 4096;
	if( !_Init(_szBuffer, &gFloatFile ) )
	{
		_ExitAccessCorebase( lpszName_ );
		return FALSE;
	}

	sprintf( _szBuffer, "ZZK.COREBASE.LONG.%s", lpszName_ );
	gLongFile.m_Type = SF_LONG;
	gLongFile.m_dwSize = 4096;
	if( !_Init(_szBuffer, &gLongFile ) )
	{
		_ExitAccessCorebase( lpszName_ );
		return FALSE;
	}
	
	sprintf( _szBuffer, "ZZK.COREBASE.BOOL.%s", lpszName_ );
	gBoolFile.m_Type = SF_BOOL;
	gBoolFile.m_dwSize = 4096;
	if( !_Init(_szBuffer, &gBoolFile ) )
	{
		_ExitAccessCorebase( lpszName_ );
		return FALSE;
	}
	
	return TRUE;
}

BOOL _Init( LPCSTR lpszName_, LPSHAREFILEINFO lpsfi_ )
{
	BOOL	_bFirst = TRUE;
	char	_szBuffer[256];

	sprintf( _szBuffer, "%s.MUTEX", lpszName_ );
	lpsfi_->m_hMutex = CreateMutex( NULL, FALSE, _szBuffer );
	if (lpsfi_->m_hMutex == NULL) { 
		LOGERR1("Could not create mutex object. %lu", GetLastError() ); 
		return FALSE;
	}

	lpsfi_->m_hMapFile = CreateFileMapping( (HANDLE)0xFFFFFFFF,	// file handle 
		NULL,													// Default security 
		PAGE_READWRITE, 										// Read/write permission 
		0,														// size-high 
		lpsfi_->m_dwSize,										// size-low 
		lpszName_ );											// Name of mapping object 
 
	if (lpsfi_->m_hMapFile == NULL) { 
		LOGERR1("Could not create file-mapping object. %lu", GetLastError() ); 
		return FALSE;
	} 

	if( GetLastError() == ERROR_ALREADY_EXISTS) { 
		_bFirst = FALSE;
		LOGMSG0("already existed!"); 
	}

	lpsfi_->m_lpMapAddr = MapViewOfFile(lpsfi_->m_hMapFile,		// Handle to mapping object. 
		FILE_MAP_ALL_ACCESS,					// Read/write permission 
		0,										// offset-high
		0,										// offset-low
		0); 									// Map entire file. 
 
	if (lpsfi_->m_lpMapAddr == NULL) { 
		LOGERR1("Could not map view of file. %lu", GetLastError() ); 
		return FALSE;
	} 

	if(	_bFirst == TRUE )
	{
		if( !_InitializeShareFile(lpsfi_) )
			return FALSE;
	}

	if( !_RegisterInstance( lpsfi_ ) ) 
		return FALSE;

	return TRUE;
}

BOOL _RegisterInstance( LPSHAREFILEINFO lpsfi_ )
{
	LPSHAREFILEHEAD		_lpSfh = (LPSHAREFILEHEAD)(lpsfi_->m_lpMapAddr);
	int		_i;

	Wait_For_Single_Object( lpsfi_->m_hMutex, 500 );

	for( _i=0; _i<32; _i++ )
	{
		if( !_lpSfh->Pid[_i] ) 
		{
			DWORD	_pid = GetCurrentProcessId();

			_lpSfh->Pid[_i] = _pid;
			lpsfi_->m_dwDirtyMask = 1 << _i;

			LOGMSG2( "ACR: PID=0x%X, No=%d", _pid, _i );

			ReleaseMutex( lpsfi_->m_hMutex );
			return TRUE;
		}
	}

	ReleaseMutex( lpsfi_->m_hMutex );
	return FALSE;
}

BOOL _InitializeShareFile( LPSHAREFILEINFO lpsfi_ )
{
	LPSHAREFILEHEAD		_lpSfh = (LPSHAREFILEHEAD)(lpsfi_->m_lpMapAddr);
	size_t				_data_size;

	Wait_For_Single_Object( lpsfi_->m_hMutex, 500 );

	MEMCLR( lpsfi_->m_lpMapAddr, lpsfi_->m_dwSize );

	if( lpsfi_->m_Type == SF_FLOAT )
	{
		_data_size = sizeof( SHAREFLOAT );
	}
	else if( lpsfi_->m_Type == SF_LONG )
	{
		_data_size = sizeof( SHARELONG );
	}
	else if( lpsfi_->m_Type == SF_BOOL )
	{
		_data_size = sizeof( SHAREBOOL );
	}
	else
	{
		CORE_ASSERT( FALSE );
		ReleaseMutex( lpsfi_->m_hMutex );
		return FALSE;
	}

	_lpSfh->MaxItem = (lpsfi_->m_dwSize - sizeof(SHAREFILEHEAD)) / _data_size;

	ReleaseMutex( lpsfi_->m_hMutex );
	return TRUE;
}

///////////////////////////////////////////////////////////
_DLLEXP int _DLLFUNC ReadCorebaseFloat( LPCSTR lpszName_, float* pData_ )
{
	LPSHAREFILEHEAD		_lpSfh = (LPSHAREFILEHEAD)(gFloatFile.m_lpMapAddr);
	LPSHAREFLOAT		_pFloat = (LPSHAREFLOAT)((LPBYTE)(gFloatFile.m_lpMapAddr) + sizeof(SHAREFILEHEAD));
	UINT				_i;
	int					_ret = 0;

	Wait_For_Single_Object( gFloatFile.m_hMutex, 500 );

	for( _i=0; 
		 _i<_lpSfh->MaxItem; 
		 _i++, _pFloat++ )
	{
		if( _pFloat->szTag[0] == 0 )	//it is the end 
			break;

		if( 0 == strncmp( _pFloat->szTag, lpszName_, S_TAG_LENGTH_MAX ) )
		{
			*pData_ = _pFloat->Data;

			if( _pFloat->Dirty & gFloatFile.m_dwDirtyMask )
			{
				//clear the dirty bit
				_pFloat->Dirty &= (~gFloatFile.m_dwDirtyMask);
				_ret = 1;
			}
			else
				_ret = 2;

			break;
		}
	}

	//not found!
	ReleaseMutex( gFloatFile.m_hMutex );
	return _ret;
}

_DLLEXP BOOL _DLLFUNC WriteCorebaseFloat( LPCSTR lpszName_, float Data_ )
{
	LPSHAREFILEHEAD		_lpSfh = (LPSHAREFILEHEAD)(gFloatFile.m_lpMapAddr);
	LPSHAREFLOAT		_pFloat = (LPSHAREFLOAT)((LPBYTE)(gFloatFile.m_lpMapAddr) + sizeof(SHAREFILEHEAD));
	UINT				_i;
	BOOL				_ret = TRUE;

	Wait_For_Single_Object( gFloatFile.m_hMutex, 500 );

	for( _i=0; 
		 _i<_lpSfh->MaxItem; 
		 _i++, _pFloat++ )
	{
		if( _pFloat->szTag[0] == 0 )	//it is the end 
		{
			//not found! then add it
			strncpy( _pFloat->szTag, lpszName_, S_TAG_LENGTH_MAX );
			_pFloat->Data = Data_;
			_pFloat->Dirty = ((DWORD)-1) & (~gFloatFile.m_dwDirtyMask);
			goto _END;
		}

		if( 0 == strncmp( _pFloat->szTag, lpszName_, S_TAG_LENGTH_MAX ) )
		{
			_pFloat->Data = Data_;
			_pFloat->Dirty = ((DWORD)-1) & (~gFloatFile.m_dwDirtyMask);
			goto _END;
		}
	}

	//here, the share file is full!	
	_ret = FALSE;

_END:
	ReleaseMutex( gFloatFile.m_hMutex );
	return _ret;
}

void _ResetDirtyFloat()
{
	LPSHAREFILEHEAD		_lpSfh = (LPSHAREFILEHEAD)(gFloatFile.m_lpMapAddr);
	LPSHAREFLOAT		_pFloat = (LPSHAREFLOAT)((LPBYTE)(gFloatFile.m_lpMapAddr) + sizeof(SHAREFILEHEAD));
	UINT				_i;
	int					_ret = 0;

	Wait_For_Single_Object( gFloatFile.m_hMutex, 500 );

	for( _i=0; 
		 _i<_lpSfh->MaxItem; 
		 _i++, _pFloat++ )
	{
		if( _pFloat->szTag[0] == 0 )	//it is the end 
			break;
		else
			_pFloat->Dirty |= gFloatFile.m_dwDirtyMask;
	}

	//not found!
	ReleaseMutex( gFloatFile.m_hMutex );
	return;
}

///////////////////////////////////////////////////////////
_DLLEXP int _DLLFUNC ReadCorebaseLong( LPCSTR lpszName_, long* pData_ )
{
	LPSHAREFILEHEAD		_lpSfh = (LPSHAREFILEHEAD)(gLongFile.m_lpMapAddr);
	LPSHARELONG			_pLong = (LPSHARELONG)((LPBYTE)(gLongFile.m_lpMapAddr) + sizeof(SHAREFILEHEAD));
	UINT				_i;
	int					_ret = 0;

	Wait_For_Single_Object( gLongFile.m_hMutex, 500 );

	for( _i=0; 
		 _i<_lpSfh->MaxItem; 
		 _i++, _pLong++ )
	{
		if( _pLong->szTag[0] == 0 )	//it is the end 
			break;

		if( 0 == strncmp( _pLong->szTag, lpszName_, S_TAG_LENGTH_MAX ) )
		{
			*pData_ = _pLong->Data;

			if( _pLong->Dirty & gLongFile.m_dwDirtyMask )
			{
				//clear the dirty bit
				_pLong->Dirty &= (~gLongFile.m_dwDirtyMask);
				_ret = 1;
			}
			else
				_ret = 2;

			break;
		}
	}

	//not found!
	ReleaseMutex( gLongFile.m_hMutex );
	return _ret;
}

_DLLEXP BOOL _DLLFUNC WriteCorebaseLong( LPCSTR lpszName_, long Data_ )
{
	LPSHAREFILEHEAD		_lpSfh = (LPSHAREFILEHEAD)(gLongFile.m_lpMapAddr);
	LPSHARELONG			_pLong = (LPSHARELONG)((LPBYTE)(gLongFile.m_lpMapAddr) + sizeof(SHAREFILEHEAD));
	UINT				_i;
	BOOL				_ret = TRUE;

	Wait_For_Single_Object( gLongFile.m_hMutex, 500 );

	for( _i=0; 
		 _i<_lpSfh->MaxItem; 
		 _i++, _pLong++ )
	{
		if( _pLong->szTag[0] == 0 )	//it is the end 
		{
			//not found! then add it
			strncpy( _pLong->szTag, lpszName_, S_TAG_LENGTH_MAX );
			_pLong->Data = Data_;
			_pLong->Dirty = ((DWORD)-1) & (~gLongFile.m_dwDirtyMask);
			goto _END;
		}

		if( 0 == strncmp( _pLong->szTag, lpszName_, S_TAG_LENGTH_MAX ) )
		{
			_pLong->Data = Data_;
			_pLong->Dirty = ((DWORD)-1) & (~gLongFile.m_dwDirtyMask);
			goto _END;
		}
	}

	//here, the share file is full!	
	_ret = FALSE;

_END:
	ReleaseMutex( gLongFile.m_hMutex );
	return _ret;
}

void _ResetDirtyLong()
{
	LPSHAREFILEHEAD		_lpSfh = (LPSHAREFILEHEAD)(gLongFile.m_lpMapAddr);
	LPSHARELONG			_pLong = (LPSHARELONG)((LPBYTE)(gLongFile.m_lpMapAddr) + sizeof(SHAREFILEHEAD));
	UINT				_i;
	int					_ret = 0;

	Wait_For_Single_Object( gLongFile.m_hMutex, 500 );

	for( _i=0; 
		 _i<_lpSfh->MaxItem; 
		 _i++, _pLong++ )
	{
		if( _pLong->szTag[0] == 0 )	//it is the end 
			break;
		else
			_pLong->Dirty |= gLongFile.m_dwDirtyMask;
	}

	//not found!
	ReleaseMutex( gLongFile.m_hMutex );
	return;
}

///////////////////////////////////////////////////////////
_DLLEXP int _DLLFUNC ReadCorebaseBool( LPCSTR lpszName_, BOOL* pData_ )
{
	LPSHAREFILEHEAD		_lpSfh = (LPSHAREFILEHEAD)(gBoolFile.m_lpMapAddr);
	LPSHAREBOOL			_pBool = (LPSHAREBOOL)((LPBYTE)(gBoolFile.m_lpMapAddr) + sizeof(SHAREFILEHEAD));
	UINT				_i;
	int					_ret = 0;

	Wait_For_Single_Object( gBoolFile.m_hMutex, 500 );

	for( _i=0; 
		 _i<_lpSfh->MaxItem; 
		 _i++, _pBool++ )
	{
		if( _pBool->szTag[0] == 0 )	//it is the end 
			break;

		if( 0 == strncmp( _pBool->szTag, lpszName_, S_TAG_LENGTH_MAX ) )
		{
			*pData_ = _pBool->Data;

			if( _pBool->Dirty & gBoolFile.m_dwDirtyMask )
			{
				//clear the dirty bit
				_pBool->Dirty &= (~gBoolFile.m_dwDirtyMask);
				_ret = 1;
			}
			else
				_ret = 2;

			break;
		}
	}

	//not found!
	ReleaseMutex( gBoolFile.m_hMutex );
	return _ret;
}

_DLLEXP BOOL _DLLFUNC WriteCorebaseBool( LPCSTR lpszName_, BOOL Data_ )
{
	LPSHAREFILEHEAD		_lpSfh = (LPSHAREFILEHEAD)(gBoolFile.m_lpMapAddr);
	LPSHAREBOOL			_pBool = (LPSHAREBOOL)((LPBYTE)(gBoolFile.m_lpMapAddr) + sizeof(SHAREFILEHEAD));
	UINT				_i;
	BOOL				_ret = TRUE;

	Wait_For_Single_Object( gBoolFile.m_hMutex, 500 );

	for( _i=0; 
		 _i<_lpSfh->MaxItem; 
		 _i++, _pBool++ )
	{
		if( _pBool->szTag[0] == 0 )	//it is the end 
		{
			//not found! then add it
			strncpy( _pBool->szTag, lpszName_, S_TAG_LENGTH_MAX );
			_pBool->Data = Data_;
			_pBool->Dirty = ((DWORD)-1) & (~gBoolFile.m_dwDirtyMask);
			goto _END;
		}

		if( 0 == strncmp( _pBool->szTag, lpszName_, S_TAG_LENGTH_MAX ) )
		{
			_pBool->Data = Data_;
			_pBool->Dirty = ((DWORD)-1) & (~gBoolFile.m_dwDirtyMask);
			goto _END;
		}
	}

	//here, the share file is full!	
	_ret = FALSE;

_END:
	ReleaseMutex( gBoolFile.m_hMutex );
	return _ret;
}

void _ResetDirtyBool()
{
	LPSHAREFILEHEAD		_lpSfh = (LPSHAREFILEHEAD)(gBoolFile.m_lpMapAddr);
	LPSHAREBOOL			_pBool = (LPSHAREBOOL)((LPBYTE)(gBoolFile.m_lpMapAddr) + sizeof(SHAREFILEHEAD));
	UINT				_i;
	int					_ret = 0;

	Wait_For_Single_Object( gBoolFile.m_hMutex, 500 );

	for( _i=0; 
		 _i<_lpSfh->MaxItem; 
		 _i++, _pBool++ )
	{
		if( _pBool->szTag[0] == 0 )	//it is the end 
			break;
		else
			_pBool->Dirty |= gBoolFile.m_dwDirtyMask;
	}

	//not found!
	ReleaseMutex( gBoolFile.m_hMutex );
	return;
}


/*
W
{
	wait Ws ==0, ++;
	wait Semp == 0;
	
		write

	Ws --;
}

R
{
	wait Ws == 0;
	Rs ++;

		read

	Rs --;
}

*/