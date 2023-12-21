//===================================================================
//csysobj.h
//	Head file of csysobj.c
//		system core-objects
//Zheng zhikai	96-8-6
//===================================================================

#ifndef __CSYSOBJ_H
#define __CSYSOBJ_H

#define IDCOBJ_BINARY		0x0001		//a general binary array
#define IDCOBJ_STRING		0x0100		//a string

BOOL RegisterSystemObjects(void);

/////////////////////////////
//macroes for for string COBJ
#define PU_GET_COBJ_STRING(_lppu_)		\
		( (LPSTR)GET_COBJ_DATA((_lppu_)->du.zzk__LPCOBJ) )

#define IS_STRING_COBJ( _lpCobj_ )	( IsStringCobj(_lpCobj_) )

_DLLEXP BOOL _ZZKDECL IsStringCobj( LPCOBJ lpCobj_ );
_DLLEXP int _ZZKDECL CobjCompareString( LPCOBJ lpCobj1_, LPCOBJ lpCobj2_ );

#endif 		//__CSYSOBJ.H

