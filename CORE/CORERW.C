//===================================================================
//
//Zheng zhikai	95-04-18
//===================================================================

#include "prehead.h"
#include "coreall.h"

_DLLEXP BOOL _ZZKDECL  PUReadRealTable(SimplePU FAR* to_, LPPU from_);
_DLLEXP BOOL _ZZKDECL  PUReadTintTable(SimplePU FAR* to_, LPPU from_);
_DLLEXP BOOL _ZZKDECL  PUReadBoolTable(SimplePU FAR* to_, LPPU from_);
_DLLEXP BOOL _ZZKDECL  PUReadModuTable(SimplePU FAR* to_, LPPU from_);
_DLLEXP BOOL _ZZKDECL  PUReadFaceTable(SimplePU FAR* to_, LPPU from_);
_DLLEXP BOOL _ZZKDECL  PUWriteRealTable(LPPU to_, SimplePU FAR* from_);
_DLLEXP BOOL _ZZKDECL  PUWriteTintTable(LPPU to_, SimplePU FAR* from_);
_DLLEXP BOOL _ZZKDECL  PUWriteBoolTable(LPPU to_, SimplePU FAR* from_);
_DLLEXP BOOL _ZZKDECL  PUWriteModuTable(LPPU to_, SimplePU FAR* from_);
_DLLEXP BOOL _ZZKDECL  PUWriteFaceTable(LPPU to_, SimplePU FAR* from_);

extern RTInfo		gRTInfo;
extern TTInfo		gTTInfo;
extern BTInfo		gBTInfo;
extern MTInfo		gMTInfo;
extern FTInfo		gFTInfo;
extern ATInfo		gATInfo;
extern STInfo		gSTInfo;

extern CoreInfo		gCoreInfo;

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC PURead(SimplePU FAR* to_, LPPU from_)
{
	if( !to_ || !from_ )
		return FALSE;

	switch((from_->prefix) & PF_MASK)
	{
		case PF_IMM:
		{
			if( (to_->prefix & IMM_MASK) == IMM_LPCOBJ )
			{	// dest buffer must be clean!
				CORE_ASSERT( to_->du.zzk__LPCOBJ == NULL );
				to_->du.zzk__LPCOBJ = NULL;
			}

			return PUCast(to_, (SimplePU FAR*)from_);
		}

		case PF_TBX:
		{
			switch((from_->prefix) & TBX_MASK)
			{
				case TBX_RT:
					return PUReadRealTable(to_, from_);
				case TBX_TT:
					return PUReadTintTable(to_, from_);
				case TBX_BT:
					return PUReadBoolTable(to_, from_);
				case TBX_MT:
					return PUReadModuTable(to_, from_);
				case TBX_FT:
					return PUReadFaceTable(to_, from_);
				default:
					CORE_ASSERT(0);
					return FALSE;
			}
		}

		case PF_MPX:
		{
			LPMTU	_lpMTU = gMTInfo.lpBase + from_->du.zzk__WRD;
			return PURead(to_, &((_lpMTU->lpParam)[(from_->prefix) & MPX_INDEX_MASK]) );
		}

		case PF_FPX:
		{
			LPFTU	_lpFTU = gFTInfo.lpBase + from_->du.zzk__WRD;
			return PURead(to_, &((_lpFTU->lpParam)[(from_->prefix) & FPX_INDEX_MASK]) );
		}

		default:
			CORE_ASSERT( FALSE );
			return FALSE;
	}
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC PUWrite(LPPU to_, SimplePU FAR* from_)
{
	if( !to_ || !from_ )
		return FALSE;

	switch((to_->prefix) & PF_MASK)
	{
		case PF_IMM:
			return PUCast(to_, (SimplePU FAR*)from_);

		case PF_TBX:
		{
			switch((to_->prefix) & TBX_MASK )
			{
				case TBX_RT:
					return PUWriteRealTable(to_, from_);
				case TBX_TT:
					return PUWriteTintTable(to_, from_);
				case TBX_BT:
					return PUWriteBoolTable(to_, from_);
				case TBX_MT:
					return PUWriteModuTable(to_, from_);
				case TBX_FT:
					return PUWriteFaceTable(to_, from_);
				default:
					CORE_ASSERT(0);
					return FALSE;
			}
		}

		case PF_MPX:
			return PUWrite(&((((gMTInfo.lpBase)[to_->du.zzk__WRD]).lpParam)[(to_->prefix) & MPX_INDEX_MASK]), from_);

		case PF_FPX:
			return PUWrite(&((((gFTInfo.lpBase)[to_->du.zzk__WRD]).lpParam)[(to_->prefix) & FPX_INDEX_MASK]), from_);

		default:
		{
			CORE_ASSERT(0);
			return FALSE;
		}
	}//end switch
}

/* 
Note:
	Don't use the following code:
	if( to_->du.zzk__##__to == (__to)from_->du.zzk__##__from ) 
		...

	because if the __to is 'REAL', but to_->du.zzk__##__to is not a
	valid REAL, for example, to_->du.zzk__DWORD == 0xFFFFFFFFL, then
	comparation returns '==';
*/
#define _pu_cast(__to, __from)										\
	case (IMM_##__to >> 4) | IMM_##__from :							\
		if( to_->du.zzk__FULL == from_->du.zzk__FULL )				\
			return TRUE;											\
		to_->du.zzk__##__to = (__to)from_->du.zzk__##__from;		\
		to_->flags |= PU_DIRTY;										\
		return TRUE;

//...................................................................
//...................................................................
BOOL _NONEXPORTED PUCast(SimplePU FAR* to_, SimplePU FAR* from_)
{
	CORE_ASSERT( (from_->prefix & PF_MASK) == PF_IMM );
	CORE_ASSERT( (to_->prefix & PF_MASK) == PF_IMM );

	if( (to_->prefix & IMM_MASK) == IMM_NULL )
	{
		memcpy(to_, from_, sizeof(PU) );
		return TRUE;
	}

	switch( ((to_->prefix & 0x0F00)>>4) | (from_->prefix & 0x0F00) )
	{
		_pu_cast(int, int)
		_pu_cast(int, WORD)
		_pu_cast(int, long)
		_pu_cast(int, DWORD)
		_pu_cast(int, REAL)
		_pu_cast(int, BOOL)
		_pu_cast(int, FPTR)
		_pu_cast(int, NPTR)
		//_pu_cast(int, WRD)		//incompatible type conversion

		_pu_cast(WORD, int)
		_pu_cast(WORD, WORD)
		_pu_cast(WORD, long)
		_pu_cast(WORD, DWORD)
		_pu_cast(WORD, REAL)
		_pu_cast(WORD, BOOL)
		_pu_cast(WORD, FPTR)
		_pu_cast(WORD, NPTR)
		//_pu_cast(WORD, WRD)		//incompatible type conversion

		_pu_cast(long, int)
		_pu_cast(long, WORD)
		_pu_cast(long, long)
		_pu_cast(long, DWORD)
		_pu_cast(long, REAL)
		_pu_cast(long, BOOL)
		_pu_cast(long, FPTR)
		_pu_cast(long, NPTR)
		//_pu_cast(long, WRD)		//incompatible type conversion

		_pu_cast(DWORD, int)
		_pu_cast(DWORD, WORD)
		_pu_cast(DWORD, long)
		_pu_cast(DWORD, DWORD)
		_pu_cast(DWORD, REAL)
		_pu_cast(DWORD, BOOL)
		_pu_cast(DWORD, FPTR)
		_pu_cast(DWORD, NPTR)
		//_pu_cast(DWORD, WRD)//incompatible type conversion

		_pu_cast(REAL, int)
		_pu_cast(REAL, WORD)
		_pu_cast(REAL, long)
		_pu_cast(REAL, DWORD)
		_pu_cast(REAL, REAL)
		_pu_cast(REAL, BOOL)
		//_pu_cast(REAL, WRD)  	incompatible type conversion
		//_pu_cast(REAL, FPTR)  incompatible type conversion
		//_pu_cast(REAL, NPTR)	incompatible type conversion

		_pu_cast(BOOL, int)
		_pu_cast(BOOL, WORD)
		_pu_cast(BOOL, long)
		_pu_cast(BOOL, DWORD)
		_pu_cast(BOOL, REAL)
		_pu_cast(BOOL, BOOL)
		_pu_cast(BOOL, FPTR)
		_pu_cast(BOOL, NPTR)
		//_pu_cast(BOOL, WRD)	incompatible type conversion

		_pu_cast(FPTR, int)
		_pu_cast(FPTR, WORD)
		_pu_cast(FPTR, long)
		_pu_cast(FPTR, DWORD)
		//_pu_cast(FPTR, REAL)	incompatible type conversion
		_pu_cast(FPTR, BOOL)
		_pu_cast(FPTR, FPTR)
		_pu_cast(FPTR, NPTR)
		//_pu_cast(FPTR, WRD)	incompatible type conversion

		_pu_cast(NPTR, int)
		_pu_cast(NPTR, WORD)
		_pu_cast(NPTR, long)
		_pu_cast(NPTR, DWORD)
		//_pu_cast(NPTR, REAL)	incompatible type conversion
		_pu_cast(NPTR, BOOL)
		_pu_cast(NPTR, FPTR)
		_pu_cast(NPTR, NPTR)
		//_pu_cast(NPTR, WRD)	incompatible type conversion

		case ( IMM_WRD >> 4) | (IMM_WRD):
		{
			if( to_->prefix != from_->prefix )
			{
				//not the same table index
				if ( to_->prefix == (PF_IMM|IMM_WRD) )
					to_->prefix = from_->prefix;
				else
					return FALSE;	//incompatible type conversion
			}

			to_->du.zzk__WRD = from_->du.zzk__WRD;
			to_->flags |= PU_DIRTY;
			return TRUE;
		}

		case ( IMM_LPCOBJ >> 4) | (IMM_LPCOBJ):
		{
			//in multithreat system
			//if the PU's cobject is busy
			//do wait ...

			if( to_->du.zzk__LPCOBJ != from_->du.zzk__LPCOBJ )
			{
				if( (!from_->du.zzk__LPCOBJ) || IsCobj( from_->du.zzk__LPCOBJ ) )
				{
					//at first, free the old cObject
					CobjFree( to_->du.zzk__LPCOBJ );

					//copy the new cobject
					to_->du.zzk__LPCOBJ = from_->du.zzk__LPCOBJ;
				}
				else	//not a legal COBJ, writting failed
					return FALSE;

				//DO NOT clear from_->du.zzk__LPCOBJ !
				//ref. to Tech. Note
			}
			//else only set dirty bits

			to_->flags |= PU_DIRTY;
			return TRUE;
		}

		default:
			if(to_->prefix == 0)
			{
				memcpy( to_, from_, sizeof(PU) );
				return TRUE;
			}
			else
			{
				CORE_ASSERT(0);
				return FALSE;
			}
	}
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC PUCopy(LPPU to_, LPPU from_)
{
	memcpy(to_, from_, sizeof(PU));
	return TRUE;
}

//...................................................................
//Description:
//	Free the resource in FragPool of a given PU
//...................................................................
_DLLEXP BOOL _DLLFUNC PUFree(LPPU lpPU_)
{
	switch(lpPU_->prefix & PF_MASK)
	{
		case PF_IMM:
		{
			if( (lpPU_->prefix & IMM_MASK) == IMM_LPCOBJ )
				return CobjFree(lpPU_->du.zzk__LPCOBJ);
			else
				return TRUE;
		}

		default:
			return TRUE;
	}
}


//...................................................................
//Note:
//	Calling this function directly, is not safely, for example, when
//a PU with PF_IMM|IMM_LPCOBJ and the cobject is valid, ie, the pointer
//isn't equal to zero, but this function only set the new data link, 
//and the result is the old cobject is leaked!
//...................................................................
_DLLEXP BOOL _DLLFUNC PUEncode(LPPU lpPU_, LPENCODE lpEncode_)
{
	char		_szBuffer[512];

	LPSTR		_p1 = NULL;
	LPSTR		_p2 = NULL;
	LPSTR		_p3 = NULL;

	PU			_pu;

	WRD			_wrd;
	WORD		_ele;
	BOOL		_bOk;
	PDT			_pdt;

	CORE_ASSERT( lpEncode_ && lpEncode_->lpszBuffer );

	//backup the lpPU_
	memcpy(&_pu, lpPU_, sizeof(PU) );

	//analyze the string
	StringCopyn( _szBuffer, lpEncode_->lpszBuffer, sizeof( _szBuffer ) );
	_p1 = &(_szBuffer[0]);
	while( (*_p1) && (*_p1<' ') )_p1++;	//omit pre-spaces
	if(*_p1 == 0)	//NULL string
	{
		/*
		lpPU_->prefix = NULL;
		lpPU_->flags |= PU_DIRTY;

		*/
		//CORE_ASSERT( FALSE );	//normally, can't come here
		return FALSE;
	}

	_p2 = strchr(_p1,  '.');
	if(_p2)
	{
		*_p2 = 0;	//set terminate char for string _p1
		_p2++;

		_p3 = strchr(_p2, '.');
		if(_p3)
		{
			*_p3 = 0;	//set terminate char for string _p2
			_p3++;
			strupr(_p3);
		}
	}

	_pdt = GetPDT( _p1, &_bOk );
	if( _bOk )
	{
		if( _p2 || _p3 )	//now the _p2 and _p3 must be NULL
		{
			lpEncode_->wError = ILLEGAL_SZBODY;
			goto _PUEncode_FALSE;
		}
		else
		{
			lpPU_->prefix = PF_IMM|_pdt;
			lpPU_->flags |= PU_DIRTY;
		}

		return TRUE;
	}

	if( strcmpi( _p1, "Imm" ) == 0 )	//Imm.xxxx
	{
		WORD	_imm;

		if( _p3 )
		{
			lpEncode_->wError = ILLEGAL_SZTAIL;
			goto _PUEncode_FALSE;
		}

		_imm = GetPDT( _p2, &_bOk );
		if( !_bOk )
		{
			lpEncode_->wError = ILLEGAL_SZBODY;
			goto _PUEncode_FALSE;
		}
		else
		{
			lpPU_->prefix = PF_IMM|_imm;
			lpPU_->flags |= PU_DIRTY;
			return TRUE;
		}
	}	//end Imm.xxx

	if( strcmpi( _p1, "RT" ) == 0 )	//RT.xxx.xxx
	{
		if(!_p2)
		{
			lpEncode_->wError = EXPECTED_SZBODY;
			goto _PUEncode_FALSE;
		}

		_wrd = RealGetWRD(_p2);
		if(!_wrd)
		{
			lpEncode_->wError = ILLEGAL_SZBODY;
			goto _PUEncode_FALSE;
		}

		if(_p3)
		{
			_ele = RealGetElement(_p3);
			if(!_ele)
			{
				lpEncode_->wError = ILLEGAL_SZTAIL;
				goto _PUEncode_FALSE;
			}
		}
		else
			_ele = RTU_VALUE;

		lpPU_->prefix = PF_TBX|TBX_RT|_ele;
		lpPU_->du.zzk__WRD = _wrd;
		lpPU_->flags |= PU_DIRTY;
		return TRUE;
	}

	if( strcmpi( _p1, "TT" ) == 0 )	//TT.xxx.xxx
	{
		if(!_p2)
		{
			lpEncode_->wError = EXPECTED_SZBODY;
			goto _PUEncode_FALSE;
		}

		_wrd = TintGetWRD(_p2);
		if(!_wrd)
		{
			lpEncode_->wError = ILLEGAL_SZBODY;
			goto _PUEncode_FALSE;
		}

		if(_p3)
		{
			_ele = TintGetElement(_p3);
			if(!_ele)
			{
				lpEncode_->wError = ILLEGAL_SZTAIL;
				goto _PUEncode_FALSE;
			}
		}
		else
			_ele = TTU_VALUE;

		lpPU_->prefix = PF_TBX|TBX_TT|_ele;
		lpPU_->du.zzk__WRD = _wrd;
		lpPU_->flags |= PU_DIRTY;
		return TRUE;
	}

	if( strcmpi( _p1, "BT" ) == 0 )	//BT.xxx.xxx
	{
		if(!_p2)
		{
			lpEncode_->wError = EXPECTED_SZBODY;
			goto _PUEncode_FALSE;
		}

		_wrd = BoolGetWRD(_p2);
		if(!_wrd)
		{
			lpEncode_->wError = ILLEGAL_SZBODY;
			goto _PUEncode_FALSE;
		}

		if(_p3)
		{
			_ele = BoolGetElement(_p3);
			if(!_ele)
			{
				lpEncode_->wError = ILLEGAL_SZTAIL;
				goto _PUEncode_FALSE;
			}
		}
		else
			_ele = BTU_VALUE;

		lpPU_->prefix = PF_TBX|TBX_BT|_ele;
		lpPU_->du.zzk__WRD = _wrd;
		lpPU_->flags |= PU_DIRTY;
		return TRUE;
	}

	if( strcmpi( _p1, "MT" ) == 0 )	//MT.xxx.xxx
	{
		if(!_p2)
		{
			lpEncode_->wError = EXPECTED_SZBODY;
			goto _PUEncode_FALSE;
		}

		_wrd = ModuGetWRD(_p2);
		if(!_wrd)
		{
			lpEncode_->wError = ILLEGAL_SZBODY;
			goto _PUEncode_FALSE;
		}

		if(!_p3)	//if _p3 is omitted
		{
			_ele = MODU_RESULT;
			lpPU_->prefix = PF_MPX|_ele;
			lpPU_->du.zzk__WRD = _wrd;
			lpPU_->flags |= PU_DIRTY;
			return TRUE;
		}
		else
		{
			_ele = ModuGetElement(_p3);

			if(_ele) {
				lpPU_->prefix = PF_TBX|TBX_MT|_ele;
				lpPU_->du.zzk__WRD = _wrd;
				lpPU_->flags |= PU_DIRTY;
				return TRUE;
			}
			else
			{
				if( 0 == strcmp(_p3, MODU_RESULT_NAME ) )
				{
					lpPU_->prefix = PF_MPX|MODU_RESULT;
					lpPU_->du.zzk__WRD = _wrd;
					lpPU_->flags |= PU_DIRTY;
				}
				else
				{
					GETPARAMORDER	_gpo;
					_gpo.lpszParamName = _p3;

					if( !AlgoGetParamOrder( ModuGetAlgo(_wrd), &_gpo) )
					{
						lpEncode_->wError = ILLEGAL_SZTAIL;
						goto _PUEncode_FALSE;
					}

					lpPU_->prefix = PF_MPX|_gpo.wOrder;
					lpPU_->du.zzk__WRD = _wrd;
					lpPU_->flags |= PU_DIRTY;
				}
				return TRUE;
			}
		}
	}

	if( strcmpi( _p1, "FT" ) == 0 )	//FT.xxx.xxx
	{
		if(!_p2)
		{
			lpEncode_->wError = EXPECTED_SZBODY;
			goto _PUEncode_FALSE;
		}

		_wrd = FaceGetWRD(_p2);
		if(!_wrd) {
			lpEncode_->wError = ILLEGAL_SZBODY;
			goto _PUEncode_FALSE;
		}

		if(!_p3)	//if _p3 is omitted
		{
			lpEncode_->wError = EXPECTED_SZTAIL;
			goto _PUEncode_FALSE;
		}
		else
		{
			_ele = FaceGetElement(_p3);
			if(_ele) {
				lpPU_->prefix = PF_TBX|TBX_FT|_ele;
				lpPU_->du.zzk__WRD = _wrd;
				lpPU_->flags |= PU_DIRTY;
				return TRUE;
			}
			else
			{
				GETPARAMORDER	_gpo;

				_gpo.lpszParamName = _p3;

				if( !StampGetParamOrder( FaceGetStamp(_wrd), &_gpo) )
				{
					lpEncode_->wError = ILLEGAL_SZTAIL;
					goto _PUEncode_FALSE;
				}

				lpPU_->prefix = PF_FPX|_gpo.wOrder;
				lpPU_->du.zzk__WRD = _wrd;
				lpPU_->flags |= PU_DIRTY;
				return TRUE;
			}
		}
	}

	//if not matching above strings, come into _PUEncode_FALSE

_PUEncode_FALSE:
	//recover the old lpPU_
	memcpy( lpPU_, &_pu, sizeof(PU) );
	return FALSE;
}

//...................................................................
//Description:
//...................................................................
_DLLEXP BOOL _DLLFUNC PUDecode(LPPU lpPU_, LPDECODE lpDecode_)
{
	//initialize struct
	*(lpDecode_->szHead) = *(lpDecode_->szBody) = *(lpDecode_->szTail) = 0;
	lpDecode_->wBody = lpDecode_->wTail = 0;
	lpDecode_->bImm = FALSE;

	switch(lpPU_->prefix & PF_MASK)
	{
		case PF_IMM:
		{
			PDT	_pdt = (lpPU_->prefix & IMM_MASK);

			if( !GetPDTName(_pdt, lpDecode_->szBody, sizeof(lpDecode_->szBody), lpDecode_->wAccess) )
			{
				CORE_ASSERT(0);
				return FALSE;
			}

			//strcpy(lpDecode_->szHead, "Imm");
			lpDecode_->bImm = TRUE;
			break;
		}

		case PF_TBX:
		{
			switch(lpPU_->prefix & TBX_MASK)
			{
				case TBX_RT:
					strcpy(lpDecode_->szHead, "RT");
					RealGetTag(lpPU_->du.zzk__WRD, lpDecode_->szBody, sizeof(lpDecode_->szBody) );
					//RealGetElementName(WORD element_, LPSTR szName_, WORD wSize_);

					RealGetElementName( (WORD)(lpPU_->prefix & XT_MASK), lpDecode_->szTail, sizeof(lpDecode_->szTail) );
					break;

				case TBX_TT:
					strcpy(lpDecode_->szHead, "TT");
					TintGetTag(lpPU_->du.zzk__WRD, lpDecode_->szBody, sizeof(lpDecode_->szBody) );
					TintGetElementName( (WORD)(lpPU_->prefix & XT_MASK), lpDecode_->szTail, sizeof(lpDecode_->szTail) );
					break;

				case TBX_BT:
					strcpy(lpDecode_->szHead, "BT");
					BoolGetTag(lpPU_->du.zzk__WRD, lpDecode_->szBody, sizeof(lpDecode_->szBody) );
					BoolGetElementName( (WORD)(lpPU_->prefix & XT_MASK), lpDecode_->szTail, sizeof(lpDecode_->szTail) );
					break;

				case TBX_MT:
					strcpy(lpDecode_->szHead, "MT");
					ModuGetTag(lpPU_->du.zzk__WRD, lpDecode_->szBody, sizeof(lpDecode_->szBody) );
					ModuGetElementName( (WORD)(lpPU_->prefix & XT_MASK), lpDecode_->szTail, sizeof(lpDecode_->szTail) );
					break;

				case TBX_FT:
					strcpy(lpDecode_->szHead, "FT");
					*(lpDecode_->szBody) = 0;	//clear the string buffer
					FaceGetElementName( (WORD)(lpPU_->prefix & XT_MASK), lpDecode_->szTail, sizeof(lpDecode_->szTail) );
					break;

				default:
					CORE_ASSERT(0);
					return FALSE;
			}
			lpDecode_->wBody = lpPU_->du.zzk__WRD;
			lpDecode_->wTail = lpPU_->prefix & XT_MASK;
			break;
		}

		case PF_MPX:
		{
			IDALGO	_id_algo;

			ModuGetTag(lpPU_->du.zzk__WRD, lpDecode_->szBody, sizeof(lpDecode_->szBody) );
			strcpy(lpDecode_->szHead, "MT");
			_id_algo = ModuGetAlgo(lpPU_->du.zzk__WRD);

			if(_id_algo)
			{
				GETPARAMNAME _gpn;
				AlgoGetParamName(_id_algo,
								 (WORD)(lpPU_->prefix & MPX_INDEX_MASK),
								 &_gpn );
				StringCopyn( lpDecode_->szTail, _gpn.szName, sizeof(lpDecode_->szTail) );
			}
			ModuGetTag(lpPU_->du.zzk__WRD, lpDecode_->szBody, sizeof(lpDecode_->szBody) );
			lpDecode_->wBody = lpPU_->du.zzk__WRD;
			lpDecode_->wTail = lpPU_->prefix & MPX_INDEX_MASK;
			break;
		}

		case PF_FPX:
		{
			IDSTAMP	_id_stamp;

			strcpy(lpDecode_->szHead, "FT");
			_id_stamp = FaceGetStamp(lpPU_->du.zzk__WRD);

			if(_id_stamp)
			{
				GETPARAMNAME _gpn;
				StampGetParamName(_id_stamp,
								 (WORD)(lpPU_->prefix & FPX_INDEX_MASK),
								 &_gpn);
				StringCopyn( lpDecode_->szTail, _gpn.szName, sizeof(lpDecode_->szTail) );
			}
			WRDToTag(lpPU_->du.zzk__WRD, lpDecode_->szBody, sizeof(lpDecode_->szBody) );
			lpDecode_->wBody = lpPU_->du.zzk__WRD;
			lpDecode_->wTail = lpPU_->prefix & FPX_INDEX_MASK;
			break;
		}

		default:
			if( lpPU_->prefix == NULL )
			{
				//StringCopyn( lpDecode_->szHead, "NULL", sizeof(lpDecode_->szHead) );
				return TRUE;
			}
			else
			{
				CORE_ASSERT(0);
				return FALSE;
			}
	}

	//CORE_ASSERT(*lpDecode_->szHead);
	if(lpDecode_->wAccess & AM_ENABLE_UL)
	{
		if(lpDecode_->wAccess & AM_UPPER)
			strupr(lpDecode_->szHead);
		else
			strlwr(lpDecode_->szHead);
	}

	return TRUE;
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC PUIsDirty(LPPU lppu_)
{
	if( !lppu_ )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}
		
	if( lppu_->flags & PU_DIRTY )
		return TRUE;

	switch((lppu_->prefix) & PF_MASK)
	{
		case PF_IMM:
			return FALSE;

		case PF_TBX:
		{
			switch((lppu_->prefix) & TBX_MASK)
			{
				case TBX_RT:
					return PURealDirty(lppu_);
				case TBX_TT:
					return PUTintDirty(lppu_);
				case TBX_BT:
					return PUBoolDirty(lppu_);
				case TBX_MT:
					return PUModuDirty(lppu_);
				case TBX_FT:
					return PUFaceDirty(lppu_);
				default:
					CORE_ASSERT(0);
					return FALSE;
			}
		}

		case PF_MPX:
			return PUIsDirty(&((((gMTInfo.lpBase)[lppu_->du.zzk__WRD]).lpParam)[(lppu_->prefix) & MPX_INDEX_MASK]) );

		case PF_FPX:
			return PUIsDirty(&((((gFTInfo.lpBase)[lppu_->du.zzk__WRD]).lpParam)[(lppu_->prefix) & FPX_INDEX_MASK]) );

		default:
			CORE_ASSERT(lppu_->prefix == 0);
			return FALSE;
	}
}

//...................................................................
//...................................................................
_DLLEXP LPPU _DLLFUNC PUGetTerminal(LPPU lpPU_)
{
_again:
	if( (lpPU_->prefix & PF_MASK) == PF_MPX )
	{
		lpPU_ = (gMTInfo.lpBase + lpPU_->du.zzk__WRD)->lpParam + ( (lpPU_->prefix) & 0x0FFF );
		goto _again;
	}
	else if( (lpPU_->prefix & PF_MASK) == PF_FPX )
	{
		lpPU_ = (gFTInfo.lpBase + lpPU_->du.zzk__WRD)->lpParam + ( (lpPU_->prefix) & 0x0FFF );
		goto _again;
	}
	else
		return lpPU_;
}


