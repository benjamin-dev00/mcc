//#include "stdafx.h"
#ifndef	DYNAMICS_DBIO_COMPILED
#define DYNAMICS_DBIO_COMPILED

#include "windows.h"
#include "stdio.h"
#include "conio.h"
#include "stdlib.h"
#include "time.h"
#include "tfalgo.h"
#include "tfcmdmc.h"

#ifdef __cplusplus
	extern "C"
	{
	#define OS_WNT
	#include <datatype.h>
	#include "Fixtkerr.h"
	#include "Dmacsnai.h"
	#include "Dmacsdba.h"
	#include "fixeda.h"
	#include "fixtools.h"
	#include "Fixerror.h"
	}
#else
	#define OS_WNT
	#include <datatype.h>
	#include "Fixtkerr.h"
	#include "Dmacsnai.h"
	#include "Dmacsdba.h"
	#include "fixeda.h"
	#include "fixtools.h"
	#include "Fixerror.h"
#endif

#define OS_DOS
#define OS_WNT

#ifndef DLLEXPORT 
	#define DLLEXPORT __declspec( dllexport )
#endif

#define TEMP_GROUP			100
#define CV_PAR_ACTU_NUM		69
#define MV_PAR_ACTU_NUM		76
#define DV_PAR_ACTU_NUM		12
#define GV_PAR_ACTU_NUM		64

#define MAX_NAME_SIZE		16
#define MAX_CV_NUM			30
#define MAX_CV_PAR_NUM		80
#define MAX_MV_NUM			30
#define MAX_MV_PAR_NUM		90
#define MAX_DV_NUM			15
#define MAX_DV_PAR_NUM		20
#define MAX_GV_NUM			1
#define MAX_GV_PAR_NUM		80

#define MAX_VAR_NUM			MAX_CV_NUM+MAX_MV_NUM+MAX_DV_NUM+MAX_GV_NUM
#define MAX_PAR_NUM			( MAX_CV_PAR_NUM > MAX_MV_PAR_NUM && MAX_CV_PAR_NUM > MAX_DV_PAR_NUM && MAX_CV_PAR_NUM > MAX_GV_PAR_NUM ) ? MAX_CV_PAR_NUM:	\
							( MAX_MV_PAR_NUM > MAX_CV_PAR_NUM && MAX_MV_PAR_NUM > MAX_DV_PAR_NUM && MAX_MV_PAR_NUM > MAX_GV_PAR_NUM ) ? MAX_MV_PAR_NUM:	\
							( MAX_DV_PAR_NUM > MAX_CV_PAR_NUM && MAX_DV_PAR_NUM > MAX_MV_PAR_NUM && MAX_DV_PAR_NUM > MAX_GV_PAR_NUM ) ? MAX_DV_PAR_NUM:	\
							 MAX_GV_PAR_NUM 

#define CV_IN_BLOCK			0
#define CV_DATA_INDEX		0
#define CV_LO_INDEX			1
#define CV_LOLO_INDEX		2
#define CV_HI_INDEX			3
#define CV_HIHI_INDEX		4
#define CV_NULL_INDEX		4	//is the maxium num of the _DATA_, _LO_, _HI_, _LOLO_ & _HIHI_ index

//#define CV_SPACE			1

#define DV_IN_BLOCK			0
#define DV_DATA_INDEX		0
#define DV_NULL_INDEX		0

//#define DV_SPACE			1

#define MV_IN_BLOCK			0
#define MV_SAMPLE_INDEX		0
#define MV_HI_INDEX			1
#define MV_LO_INDEX			2
#define MV_HIHI_INDEX		3
#define MV_LOLO_INDEX		4

#define MV_OUT_BLOCK		1
#define MV_VALUE_INDEX		5

#define MV_NULL_INDEX		5

//#define MV_SPACE			2

#define GV_NULL_INDEX		-1

//#define GV_SPACE			0

#define CV_IN_TAG_TYPE		"AI"
#define MV_IN_TAG_TYPE		"AI"
#define MV_OUT_TAG_TYPE		"AO"
#define DV_IN_TAG_TYPE		"AI"
#define GV_TAG_TYPE			"TT"//"AR"
#define COMMON_TAG_TYPE		"TT"//"AR"

#define CV_BASE				0
#define MV_BASE				MAX_CV_NUM+CV_BASE
#define DV_BASE				MAX_MV_NUM+MV_BASE
#define GV_BASE				MAX_DV_NUM+DV_BASE

#define CV_PAR_INDEX		0
#define MV_PAR_INDEX		1
#define DV_PAR_INDEX		2
#define GV_PAR_INDEX		3
#define MAX_XV_NUM			4

//#define MAX_AR_NUM			1024

#define CV_INF_TAGNAME		"CV_INF"
#define MV_INF_TAGNAME		"MV_INF"
#define DV_INF_TAGNAME		"DV_INF"
#define GV_INF_TAGNAME		"GV_INF"


//#define FE_OK 11000

typedef enum {TY_INT = 0 , TY_WORD ,TY_DOUBLE,TY_BOOL,TY_LPCOBJ} TYPE_ID;

//static long	AR_Counts=0;

static	char *CVTagName[MAX_CV_NUM][MAX_CV_PAR_NUM];
static	char *MVTagName[MAX_MV_NUM][MAX_MV_PAR_NUM];
static	char *DVTagName[MAX_DV_NUM][MAX_DV_PAR_NUM];
static	char *GVTagName[MAX_GV_NUM][MAX_GV_PAR_NUM];

//Only for testing AR block
static	char *ARTagName="AR_TEST";
static	GNUM ARGroup;
static	THANDLE  ARBlock;
INT16	ARINDEX;

static	GNUM CVGroup[MAX_CV_NUM];
static	GNUM MVGroup[MAX_MV_NUM];
static	GNUM DVGroup[MAX_DV_NUM];
static	GNUM GVGroup[MAX_GV_NUM];

static	THANDLE CVBlock[MAX_CV_NUM][MAX_CV_PAR_NUM];
static	THANDLE MVBlock[MAX_MV_NUM][MAX_MV_PAR_NUM];
static	THANDLE DVBlock[MAX_DV_NUM][MAX_DV_PAR_NUM];
static	THANDLE GVBlock[MAX_GV_NUM][MAX_GV_PAR_NUM];

static	char LocalNodeName[MAX_NAME_SIZE];

BOOL	HasGotLocalNodeName = FALSE;
BOOL	HasGotTagIndex = FALSE;
BOOL	HasInitialized = FALSE;

INT16	CV_IN_TAG_INDEX;
INT16	MV_IN_TAG_INDEX;
INT16	DV_IN_TAG_INDEX;
INT16	MV_OUT_TAG_INDEX;
INT16	GV_TAG_INDEX;
INT16	COMMON_TAG_INDEX;

#ifdef __cplusplus
extern "C"
{
#endif
//////////////////////////////////////////////////
//	Initialization & Uninitialization
//////////////////////////////////////////////////
BOOL DLLEXPORT DynInitialize();
BOOL DLLEXPORT DynUninitialize();
BOOL DLLEXPORT GetGroup();
BOOL DLLEXPORT GetBlock();
//////////////////////////////////////////////////
//	Get Local Node Name
//////////////////////////////////////////////////
void DLLEXPORT GetLocalNodeName();
//////////////////////////////////////////////////
//	Get Tag Index
//////////////////////////////////////////////////
void DLLEXPORT GetTagIndex();
//////////////////////////////////////////////////
//	Create Blocks
//////////////////////////////////////////////////
BOOL DLLEXPORT Create_Blocks();
BOOL DLLEXPORT Create_CV_Blocks();
BOOL DLLEXPORT Create_MV_Blocks();
BOOL DLLEXPORT Create_DV_Blocks();
BOOL DLLEXPORT Create_GV_Blocks();
//////////////////////////////////////////////////
//	Delete Blocks
//////////////////////////////////////////////////
BOOL DLLEXPORT Delete_Blocks();
BOOL DLLEXPORT Delete_CV_Blocks();
BOOL DLLEXPORT Delete_MV_Blocks();
BOOL DLLEXPORT Delete_DV_Blocks();
BOOL DLLEXPORT Delete_GV_Blocks();
//////////////////////////////////////////////////
//	Get Group and Block
//////////////////////////////////////////////////

BOOL DLLEXPORT GetCVGroup();
BOOL DLLEXPORT GetMVGroup();
BOOL DLLEXPORT GetDVGroup();
BOOL DLLEXPORT GetGVGroup();


BOOL DLLEXPORT GetCVBlock();
BOOL DLLEXPORT GetMVBlock();
BOOL DLLEXPORT GetDVBlock();
BOOL DLLEXPORT GetGVBlock();
//////////////////////////////////////////////////
//	Read ONE CV/MV/DV/GV
//////////////////////////////////////////////////
	BOOL DLLEXPORT ReadCV(LPCOBJPRIVCV lpcObjPrivCV , long Index);
	BOOL DLLEXPORT ReadMV(LPCOBJPRIVMV lpcObjPrivMV , long Index);
	BOOL DLLEXPORT ReadDV(LPCOBJPRIVDV lpcObjPrivDV , long Index);
	BOOL DLLEXPORT ReadGV(LPCOBJPRIV lpcObjPrivGV );
//////////////////////////////////////////////////
//	Write ONE CV/MV/DV/GV
//////////////////////////////////////////////////
	BOOL DLLEXPORT WriteCV(LPCOBJPRIVCV lpcObjPrivCV , long Index);
	BOOL DLLEXPORT WriteMV(LPCOBJPRIVMV lpcObjPrivMV , long Index);
	BOOL DLLEXPORT WriteDV(LPCOBJPRIVDV lpcObjPrivDV , long Index);
	BOOL DLLEXPORT WriteGV(LPCOBJPRIV lpcObjPrivGV );
//////////////////////////////////////////////////
//	Read CV/GV/MV/DV Member
//////////////////////////////////////////////////
	/*BOOL DLLEXPORT ReadCVMember(LPCOBJ lpcObjPrivCV , long Index , WORD PAR_ID );
	BOOL DLLEXPORT ReadMVMember(LPCOBJ lpcObjPrivMV , long Index , WORD PAR_ID );
	BOOL DLLEXPORT ReadDVMember(LPCOBJ lpcObjPrivDV , long Index , WORD PAR_ID );
	BOOL DLLEXPORT ReadGVMember(LPCOBJPRIV lpcObjPrivGV , WORD PAR_ID );*/
	DLLEXPORT void*  ReadCVMember(LPCOBJ lpcObjPrivCV , long Index , WORD PAR_ID , void* pNewVal);
	DLLEXPORT void*  ReadMVMember(LPCOBJ lpcObjPrivMV , long Index , WORD PAR_ID , void* pNewVal);
	DLLEXPORT void*  ReadDVMember(LPCOBJ lpcObjPrivDV , long Index , WORD PAR_ID , void* pNewVal);
	DLLEXPORT void*  ReadGVMember(LPCOBJPRIV lpcObjPrivGV , WORD PAR_ID , void* pNewVal);
//////////////////////////////////////////////////
//	Write CV/MV/DV/GV Member
//////////////////////////////////////////////////
	DLLEXPORT void*  WriteCVMember(void* LPCVMember, long Index , WORD PAR_ID , int _block_index,TYPE_ID val_type__ );
	DLLEXPORT void*  GetLP_CVMember(LPCOBJ lpcObjPrivCV , long Index , WORD PAR_ID , int * _pblock_index,TYPE_ID* val_type__ );
	DLLEXPORT void*  WriteMVMember(void* lpMVMember, long Index , WORD PAR_ID ,int _block_index, TYPE_ID val_type__ );
	DLLEXPORT void*  GetLP_MVMember(LPCOBJ lpcObjPrivMV , long Index , WORD PAR_ID , int * _pblock_index,TYPE_ID* val_type__ );
	DLLEXPORT void*  WriteDVMember(void* lpDVMember, long Index , WORD PAR_ID,int _block_index,  TYPE_ID val_type__ );
	DLLEXPORT void*  GetLP_DVMember(LPCOBJ lpcObjPrivDV , long Index , WORD PAR_ID , int * _pblock_index,TYPE_ID* val_type__ );
	DLLEXPORT void*  WriteGVMember2DB(void* lpGVMember, WORD PAR_ID, int _block_index , TYPE_ID type_val_ );
	DLLEXPORT void*  GetLP_GVMember(LPCOBJPRIV lpcObjPrivGV , WORD PAR_ID , int * _pblock_index , TYPE_ID* type_val__ );
	DLLEXPORT void*  DebugCVWrite( void* lpCVMember, TYPE_ID __val_type__ );
#ifdef __cplusplus
}
#endif
#define READCVMEM( lpobj,index,id,type)  (type)ReadCVMember(lpobj , index , id)
#define READMVMEM( lpobj,index,id,type )  (type)ReadMVMember(lpobj , index , id)
#define READDVMEM( lpobj,index,id,type)  (type)ReadDVMember(lpobj , index , id)
#define READGVMEM( lpobj,id,type)  (type)ReadGVMember(lpobj ,id)

#define DYNWRITECVMEM( lpobj,index,id,__type__,val) \
{	TYPE_ID __val_type__;	\
	int		_BlockIndex;	\
	void* LPCVMember = GetLP_CVMember( lpobj , index , id, &_BlockIndex,&__val_type__ );	\
	DebugCVWrite( LPCVMember, __val_type__ );		\
	*(##__type__ *)LPCVMember = (##__type__)val;		\
	WriteCVMember( LPCVMember, index , id,_BlockIndex, __val_type__ );	\
}
#define DYNWRITEMVMEM( lpobj,index,id,type,val)		\
{	TYPE_ID __val_type__;	\
	int		_BlockIndex;	\
	void* LPMVMember = GetLP_MVMember( lpobj , index , id, &_BlockIndex,&__val_type__ );	\
	*(##type *)LPMVMember = (##type)val;		\
	WriteMVMember( LPMVMember, index , id, _BlockIndex,__val_type__ );	\
}
#define DYNWRITEDVMEM( lpobj,index,id,type,val)		\
{	TYPE_ID __val_type__;	\
	int		_BlockIndex;	\
	void* LPDVMember = GetLP_DVMember( lpobj , index , id, &_BlockIndex,&__val_type__);	\
	*(##type *)LPDVMember = ((##type)(val));		\
	WriteDVMember( LPDVMember, index , id, _BlockIndex,__val_type__ );	\
}
#define DYNWRITEGVMEM( lpobj, id,type,val)	\
{	TYPE_ID	__type_val__;	\
	int		_BlockIndex;	\
	void* LPGVMember = GetLP_GVMember(lpobj , id, &_BlockIndex, &__type_val__);	\
	*(##type *)LPGVMember = ((##type)(val));		\
	WriteGVMember2DB( LPGVMember, id, _BlockIndex, __type_val__);	\
}

#define DYNREADCVMEM( lpobj,index,id,type,val) ReadCVMember( lpobj,index,id,(void*)&val)
#define DYNREADMVMEM( lpobj,index,id,type,val) ReadMVMember( lpobj,index,id,(void*)&val)
#define DYNREADDVMEM( lpobj,index,id,type,val) ReadDVMember( lpobj,index,id,(void*)&val)
#define DYNREADGVMEM( lpobj,id,type,val) ReadGVMember(lpobj,id,(void*)&val);
/*#define DYNREADDVMEM( lpobj,index,id,type,val) val=READDVMEM( lpobj,index,id,type)
#define DYNREADMVMEM( lpobj,index,id,type,val) val=READMVMEM( lpobj,index,id,type )
#define DYNREADGVMEM( lpobj,id,type,val)	val=READGVMEM( lpobj,id,type)*/
#endif

//Only for testing AR block
BOOL DLLEXPORT TEST_ARInitialize();
BOOL DLLEXPORT TEST_Create_AR_Block();
BOOL DLLEXPORT TEST_Delete_AR_Block();
BOOL DLLEXPORT TEST_GetARGroup();
BOOL DLLEXPORT TEST_GetARBlock();
float DLLEXPORT TEST_ReadARBlock();
BOOL DLLEXPORT TEST_WriteARBlock(float _val);

