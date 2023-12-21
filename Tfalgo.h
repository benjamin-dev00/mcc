#ifndef __ALGO_COMPILED
#define __ALGO_COMPILED
#include <windows.h>
#include "Mccprehead.h"
#include "core.h"
#include "cmodu.h"
#include "calgo.h"
#include "corerw.h"
#include "cpu.h"
#include "algo.h"
#include "algodef.h"
#include "tfmat.h"
#include "tfstruct.h"
#include "cobj.h"
#include "obj.h"
#include "tfmcv.h"
#ifdef __FIX
#include "DynDBio.h"
#endif
#ifdef __cplusplus
extern "C"
{
#endif

/*#ifndef __ZZK
#define	__ZZK
#endif*/
/*#ifndef __ONSPEC
#define	__ONSPEC
#endif
*/
#ifndef __ZZK
#define REAL double
#endif

#ifdef __ONSPEC

#ifndef __FIX
	#define READALLSTRUCT ONSPECREADALLSTRUCT
	#define WRITEALLSTRUCT ONSPECWRITEALLSTRUCT
	#define READMVSTRUCT( _lpCobj_All, lpCobj_PrivMV_, _j ) ONSPECREADMVSTRUCT( lpCobj_PrivMV_, _j )
	#define READCVSTRUCT( _lpCobj_All, lpCobj_PrivCV_, _i ) ONSPECREADCVSTRUCT( lpCobj_PrivCV_, _i )
	#define READDVSTRUCT( _lpCobj_All, lpCobj_PrivDV_, _j ) ONSPECREADDVSTRUCT( lpCobj_PrivDV_, _j )
	#define WRITEMVSTRUCT( _lpCobj_All, lpCobj_PrivMV_, _j ) ONSPECWRITEMVSTRUCT( lpCobj_PrivMV_, _j )
	#define WRITECVSTRUCT( _lpCobj_All, lpCobj_PrivCV_, _j ) ONSPECWRITECVSTRUCT( lpCobj_PrivCV_, _j )
	#define WRITEDVSTRUCT( _lpCobj_All, lpCobj_PrivDV_, _j ) ONSPECWRITEDVSTRUCT( lpCobj_PrivDV_, _j )
	#define READALLPARAMETER ONSPECREADALL
	#define READMVPARAMETER ONSPECREADMV
	#define READCVPARAMETER ONSPECREADCV
	#define READDVPARAMETER ONSPECREADDV
	#define WRITEALLPARAMETER ONSPECWRITEALL
	#define WRITEMVPARAMETER ONSPECWRITEMV
	#define WRITECVPARAMETER ONSPECWRITECV
	#define WRITEDVPARAMETER ONSPECWRITEDV
#else
	#define READALLSTRUCT ReadGV
	#define WRITEALLSTRUCT WriteGV
	#define READMVSTRUCT( _lpCobj_All, lpCobj_PrivMV_, _j ) ReadMV( lpCobj_PrivMV_, _j )
	#define READCVSTRUCT( _lpCobj_All, lpCobj_PrivCV_, _i ) ReadCV( lpCobj_PrivCV_, _i )
	#define READDVSTRUCT( _lpCobj_All, lpCobj_PrivDV_, _j ) ReadDV( lpCobj_PrivDV_, _j )
	#define WRITEMVSTRUCT( _lpCobj_All, lpCobj_PrivMV_, _j ) WriteMV( lpCobj_PrivMV_, _j )
	#define WRITECVSTRUCT( _lpCobj_All, lpCobj_PrivCV_, _j ) WriteCV( lpCobj_PrivCV_, _j )
	#define WRITEDVSTRUCT( _lpCobj_All, lpCobj_PrivDV_, _j ) WriteDV( lpCobj_PrivDV_, _j )
	#define READALLPARAMETER DYNREADGVMEM
	#define READMVPARAMETER DYNREADMVMEM
	#define READCVPARAMETER DYNREADCVMEM
	#define READDVPARAMETER DYNREADDVMEM
	#define WRITEALLPARAMETER DYNWRITEGVMEM
	#define WRITEMVPARAMETER DYNWRITEMVMEM
	#define WRITECVPARAMETER DYNWRITECVMEM
	#define WRITEDVPARAMETER DYNWRITEDVMEM
#endif

#else
	#ifdef __ZZK
		#define READALLSTRUCT Display_2_PrivAll
		#define WRITEALLSTRUCT PrivAll_2_Display
		#define READMVSTRUCT MVStructRead
		#define READCVSTRUCT CVStructRead
		#define READDVSTRUCT DVStructRead
		#define WRITEMVSTRUCT MVStructWrite
		#define WRITECVSTRUCT CVStructWrite
		#define WRITEDVSTRUCT DVStructWrite
		#define	READALLPARAMETER( lpSource__, __param_, __type_, __value_)			\
			{																\
				LPPU	__lpPUBase_ = ModuGetParameter( lpSource__->wModu );		\
				SimplePU	__pu_;                       				\
				CORE_ASSERT( __param_##_PDT == PDT_##__type_ );			\
				__pu_.prefix = PF_IMM|IMM_##__type_;       			\
				__pu_.du.zzk__FULL = NULL;  /* while read cobj, this must be clear */ \
				PURead( &__pu_, (__lpPUBase_ + __param_) );       		\
				(__value_) = __pu_.du.zzk__##__type_;					\
				SetGlobalParameter( lpSource__, __param_, __lpPUBase_ );		\
			}
		#define READMVPARAMETER READMCDSTRUCT
		#define READCVPARAMETER READMCDSTRUCT
		#define READDVPARAMETER	READMCDSTRUCT
		#define	WRITEALLPARAMETER( lpSource__, __param_, __type_, __value_)			\
			{																\
				LPPU	__lpPUBase_ = ModuGetParameter( lpSource__->wModu );		\
				SimplePU	__pu_;                       				\
				CORE_ASSERT( __param_##_PDT == PDT_##__type_ );			\
				__pu_.prefix = PF_IMM|IMM_##__type_;       			\
				__pu_.du.zzk__##__type_ = (__type_)__value_;          	\
				PUWrite( (__lpPUBase_ + __param_), &__pu_);       		\
				SetGlobalParameter( lpSource__, __param_, __lpPUBase_ );		\
			}
		#define WRITEMVPARAMETER WRITEMCDSTRUCT
		#define WRITECVPARAMETER WRITEMCDSTRUCT
		#define WRITEDVPARAMETER WRITEMCDSTRUCT
	#endif
#endif

#define COBJ_GLOBAL				0x80

#define RESETMCVDATA( __PARAM__, __MAX__, __RESET__ )	\
		if(  __PARAM__ > __MAX__ ) __PARAM__ = __RESET__ 
#define RESETGDATA( __PARAM__, __MAX__, __RESET__, __FLAG__ )	\
		if(  __PARAM__ > __MAX__ )	\
		{			\
			__PARAM__ = __RESET__;	\
			(*__FLAG__) = TRUE;		\
		}
//Algorithm: DMC
#define IDA_MDMCIRVD			0X04010403L //Multiple-varaible DMC with IRV and disturbance
#define IDA_NONBUTTON			0X04010301L //Multiple-varaible DMC with IRV and disturbance

typedef struct _tagCOBJ_BLOCK
{
	//PRIVATE-HEAD
	//	the following elements: hGlobal and mark, is owned by system privately,
	//	programmers should never change it!
	//	if the COBJ is allocated from golbal pool, that means by function
	//	GlobalAlloc, the hGlobal is its golbal handle, otherwise, it is NULL;
	WORD	mark;		//if the Cobj valid, it contents 'CO'
	WORD	count;		//the reference counter of this cobj
#ifndef _WIN32
	HGLOBAL	hGlobal;
#else	
	// at WIN32 version, we use the presice size of Cobject instead
	// of the size returned from API xxxSize function;
	DWORD	size;		//size of the object
#endif
	IDCOBJ	id;
	//end of PRIVATE-HEAD

	BYTE	data[1];
}COBJBLK, FAR* LPCOBJBLK;

#ifndef _WIN32
	#define _COBJ_HEAD_SIZE		( 2*sizeof(WORD) + sizeof(HGLOBAL) + sizeof(IDCOBJ) )
#else
	#define _COBJ_HEAD_SIZE		( 2*sizeof(WORD) + sizeof(DWORD) + sizeof(IDCOBJ) )
#endif

#define LPTO_COBJBLK(lpCobj)	\
( (LPCOBJBLK)((LPBYTE)(lpCobj) - _COBJ_HEAD_SIZE) )

typedef struct _tagDimMatrix{
	int		row;
	int 	col;
	int		axe;
#ifndef _WIN32
	BYTE		Nomeaning[2];
#endif
	double 	data[1];
}DimMatrix, * LPDIMMATRIX;

//get Cobj pointer from Matrix pointer
#define	MAKE_COBJ(_pm_)			\
	( _pm_ == NULL )? NULL : ((LPCOBJ) (_pm_))
//get matrix pointer from a Cobj pointer
#define MAKE_MATRIX(_lpCobj_)  \
	   ( _lpCobj_ == NULL ) ? NULL : ( (LPMATRIX)((void *)&(_lpCobj_->data[0])) )
#define MAKE_DIMMATRIX(_lpCobj_) \
		( _lpCobj_ == NULL ) ? NULL : ( (LPDIMMATRIX)((void *)&(_lpCobj_->data[0])) )

//lp->data 's type should be (double *) , orelse some big matrix can't work well
/*#define 	MGET(lp, _row, _col)		\
			(*( (( double *)((lp)->data)) + ((lp)->col)*(_row)*1L + (_col)*1L ))*/
#define 	LMGET(lp, _row, _col)		\
			(*( (( double *)((lp)->data)) + (long)((lp)->col)*(_row)*1L + (long)(_col)*1L ))

#define 	DIMMGET(lp, _row, _col, _axe)		\
			(*( double *)(( ((lp)->data) + ((((lp)->col)*	\
			((lp)->axe)*(_row)) + (_col*((lp)->axe)) + _axe))))
#ifndef DLLEXPORT 
	#define DLLEXPORT __declspec( dllexport )
#endif


//void eastrq( double a[], int n, double q[], double b[], double c[] );

#define DIMMATRIXFREE( lp )		\
	if( !(lp) )                 \
		MessageBox(NULL, "Error", "DIMMATRIXFREE", MB_OK);    \
	else                        \
	{                           \
		DimMatrixFree( lp );       \
		(lp) = 0;               \
	}
#define PRIVCVFREE( lp )		\
	if( !(lp) ) ;               \
	else                        \
	{                           \
		PrivCVCobjFree( lp );       \
		(lp) = 0;               \
	}
#define PRIVAPARTFREE( lp )		\
	if( !(lp) )   ;             \
	else                        \
	{                           \
		PrivApartFree( lp );       \
		(lp) = 0;               \
	}
#define PRIVMVFREE( lp )		\
	if( !(lp) )   ;             \
	else                        \
	{                           \
		PrivMVCobjFree( lp );       \
		(lp) = 0;               \
	}
#define PRIVDVFREE( lp )		\
	if( !(lp) )   ;             \
	else                        \
	{                           \
		PrivDVCobjFree( lp );       \
		(lp) = 0;               \
	}
#define	PRIV_2_COBJ(_pm_)			\
	( _pm_ == NULL ) ? NULL :		\
	( (LPCOBJ)( (BYTE *)(_pm_)  ) )
#define	APART_2_COBJ(_pm_)			\
	( _pm_ == NULL ) ? NULL :		\
	( (LPCOBJ)( (BYTE *)(_pm_)  ) )
#define	PRIVCV_2_COBJ(_pm_)			\
	( _pm_ == NULL ) ? NULL :		\
	( (LPCOBJ)( (BYTE *)(_pm_)  ) )
#define	PRIVMV_2_COBJ(_pm_)			\
	( _pm_ == NULL ) ? NULL :		\
	( (LPCOBJ)( (BYTE *)(_pm_)  ) )
#define	PRIVDV_2_COBJ(_pm_)			\
	( _pm_ == NULL ) ? NULL :		\
	( (LPCOBJ)( (BYTE *)(_pm_)  ) )
//get matrix pointer from a Cobj pointer
#define COBJ_2_APART(_lpCobj_)		\
	( _lpCobj_ == NULL ) ? NULL : 	\
	( (LPCOBJAPART)((void *)&(_lpCobj_->data[0])) )
#define COBJ_2_PRIVCV(_lpCobj_)		\
	( _lpCobj_ == NULL ) ? NULL : 	\
	( (LPCOBJPRIVCV)((void *)&(_lpCobj_->data[0])) )
#define COBJ_2_PRIVMV(_lpCobj_)		\
	( _lpCobj_ == NULL ) ? NULL : 	\
	( (LPCOBJPRIVMV)((void *)&(_lpCobj_->data[0])) )
#define COBJ_2_PRIVDV(_lpCobj_)		\
	( _lpCobj_ == NULL ) ? NULL : 	\
	( (LPCOBJPRIVDV)((void *)&(_lpCobj_->data[0])) )
#define COBJ_2_PRIV(_lpCobj_)		\
	( _lpCobj_ == NULL ) ? NULL : 	\
	( (LPCOBJPRIV)((void *)&(_lpCobj_->data[0])) )
#define 	APARTGET(lp, _row, _col)		\
			(*( (( LPCOBJPRIV *)((lp)->data)) + (lp)->col*(_row) + (_col) ))
#define 	PRIVCVGET(lp, _row, _col)		\
			(*( (( LPVARCV *)((lp)->data)) + (lp)->col*(_row) + (_col) ))
#define 	PRIVMVGET(lp, _row, _col)		\
			(*( (( LPVARMV *)((lp)->data)) + (lp)->col*(_row) + (_col) ))
#define 	PRIVDVGET(lp, _row, _col)		\
			(*( (( LPVARDV *)((lp)->data)) + (lp)->col*(_row) + (_col) ))
#define 	PRIVGET(lp, _row, _col)		\
			(*( (( LPCOBJ *)((lp)->data)) + (lp)->col*(_row) + (_col) ))
#define		COMPARE( Low_, High_ )	if( Low_ > High_ )	Low_ = High_

DEF_DEF( MDMCIRVD )
DEF_DEF( NonButton )
_DLLEXP 	int madd(LPMATRIX lpmResult, LPMATRIX lpma, LPMATRIX lpmb );
int			 _DLLFUNC 	mminus(LPMATRIX lpmResult, LPMATRIX lpma, LPMATRIX lpmb );
int			 _DLLFUNC   mmultiple(LPMATRIX lpmResult, LPMATRIX lpma, LPMATRIX lpmb );
int			 _DLLFUNC   Lmmultiple(LPMATRIX lpmResult, LPMATRIX lpma, LPMATRIX lpmb );
int			 _DLLFUNC   minvert(LPMATRIX lpmResult, LPMATRIX lpmsrc );
int			 _DLLFUNC   mtrans(LPMATRIX lpmResult, LPMATRIX lpmsrc);
//LPMATRIX	 	MatrixAlloc(int row, int col);
LPMATRIX	 __declspec( dllexport ) LMatrixAlloc(int row, int col);
void		 __declspec( dllexport ) MatrixFree(LPMATRIX lpm);
LPDIMMATRIX  DimAlloc(int _row, int _col, int _axe);
void 		 DimMatrixFree(LPDIMMATRIX lpm);
WORD		 _DLLFUNC 	ebstq( LPMATRIX lpmbc_, LPMATRIX lpmq_, double eps, int l );
void		 _DLLFUNC   eastrq( LPMATRIX lpmGtG_, LPMATRIX lpmq_, LPMATRIX lpmbc_ );

LPCOBJAPART _DLLFUNC PrivApartAlloc( int row, int col );
void _DLLFUNC PrivApartFree( LPCOBJAPART lpm );
LPCOBJPRIVCV _DLLFUNC PrivCVAlloc( int row, int col );
void _DLLFUNC PrivCVCobjFree( LPCOBJPRIVCV lpm );
LPCOBJPRIVMV _DLLFUNC PrivMVAlloc( int row, int col );
void _DLLFUNC PrivMVCobjFree( LPCOBJPRIVMV lpm );
LPCOBJPRIVDV _DLLFUNC PrivDVAlloc( int row, int col );
void _DLLFUNC PrivDVCobjFree( LPCOBJPRIVDV lpm );
void _DLLFUNC CobjKeepCopy( LPCOBJ lpCobjDest_, LPCOBJ lpCobjSource_ );
LPCOBJ TFCobjCopy( LPCOBJ lpCobjS_, BOOL FAR* lpbOk_ );
#ifdef __cplusplus
}
#endif
#endif