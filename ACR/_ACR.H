/////////////////////////////////////////////////////////////////////////////
// _ACR.H:
// 

#ifndef __INNER_ACR_H_
#define __INNER_ACR_H_

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpcdef.h"
#include "zsyslib.h"
#include "zassert.h"		//my assert file

//#include "acr.h"

#define	S_TAG_LENGTH_MAX	12

#define	SF_FLOAT			1
#define	SF_LONG				2
#define SF_BOOL				3

typedef struct _tagSHAREFILEINFO
{
	UINT	m_Type;			//SF_FLOAT, SF_LONG or SF_BOOL
	DWORD	m_dwSize;
	HANDLE	m_hMapFile;		
	LPVOID	m_lpMapAddr;
	DWORD	m_dwDirtyMask;
	HANDLE	m_hMutex;
}SHAREFILEINFO, FAR* LPSHAREFILEINFO;

typedef struct _tagSHAREFILEHEAD
{
	HINSTANCE	Pid[32];	//process ID
	UINT		MaxItem;
}SHAREFILEHEAD, FAR* LPSHAREFILEHEAD;

typedef struct _tagSHAREFLOAT
{
	char	szTag[S_TAG_LENGTH_MAX];
	DWORD	Dirty;
	float	Data;
}SHAREFLOAT, FAR* LPSHAREFLOAT;

typedef struct _tagSHARELONG
{
	char	szTag[S_TAG_LENGTH_MAX];
	DWORD	Dirty;	
	LONG	Data;
}SHARELONG, FAR* LPSHARELONG;

typedef struct _tagSHAREBOOL
{
	char	szTag[S_TAG_LENGTH_MAX];
	DWORD	Dirty;	
	BOOL	Data;
}SHAREBOOL, FAR* LPSHAREBOOL;

#ifdef __cplusplus
extern "C" {
#endif


//Return:
//	0:	failed
//	1:	successful
//	2:	successful, and the value is not changed since the last time
_DLLEXP int  _ZZKDECL ReadCorebaseFloat( LPCSTR lpszName_, float* pData_ );
_DLLEXP int  _ZZKDECL ReadCorebaseLong( LPCSTR lpszName_, long* pData_ );
_DLLEXP int  _ZZKDECL ReadCorebaseBool( LPCSTR lpszName_, BOOL* pData_ );

_DLLEXP BOOL _ZZKDECL WriteCorebaseFloat( LPCSTR lpszName_, float Data_ );
_DLLEXP BOOL _ZZKDECL WriteCorebaseLong( LPCSTR lpszName_, long Data_ );
_DLLEXP BOOL _ZZKDECL WriteCorebaseBool( LPCSTR lpszName_, BOOL Data_ );

void _ExitAccessCorebase( LPCSTR lpszName_ );
void _Exit( LPSHAREFILEINFO lpsfi_ );
void _ResetDirtyFloat(void);
void _ResetDirtyLong(void);
void _ResetDirtyBool(void);

BOOL _InitializeAccessCorebase( LPCSTR lpszName_ );
BOOL _Init( LPCSTR lpszName_, LPSHAREFILEINFO lpsfi_ );
BOOL _InitializeShareFile( LPSHAREFILEINFO lpsfi_ );
BOOL _RegisterInstance( LPSHAREFILEINFO lpsfi_ );

#ifdef __cplusplus
}	//end extern "C"
#endif

#endif //__INNER_ACR_H_
