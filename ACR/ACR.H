/////////////////////////////////////////////////////////////////////////////
// ACR.H:
// 

#ifndef __ACR_H_
#define __ACR_H_

#ifdef __cplusplus
extern "C"
{
#endif

//Return:
//	0:	failed
//	1:	successful
//	2:	successful, and the value is not changed since the last time
int  _stdcall ReadCorebaseFloat( LPCSTR lpszName_, float* pData_ );
int  _stdcall ReadCorebaseLong( LPCSTR lpszName_, long* pData_ );
int  _stdcall ReadCorebaseBool( LPCSTR lpszName_, BOOL* pData_ );

BOOL _stdcall WriteCorebaseFloat( LPCSTR lpszName_, float Data_ );
BOOL _stdcall WriteCorebaseLong( LPCSTR lpszName_, long Data_ );
BOOL _stdcall WriteCorebaseBool( LPCSTR lpszName_, BOOL Data_ );

#ifdef __cplusplus
}
#endif

#endif	//__ACR_H_
