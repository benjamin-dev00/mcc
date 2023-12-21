//////////////////////////////////////////////////////
//Mtstruct.h										//
//S.D.Chen    97-9-11								//
//////////////////////////////////////////////////////
#ifndef _MTSTRUCT_H
#define _MTSTRUCT_H

#include <time.h>
//#include <timeb.h>
#include "Mtlocate.h"
#include "Onworld.h"

typedef WORD IDCVMEM;	
typedef WORD IDMVMEM;	
typedef WORD IDDVMEM;
typedef WORD IDGVMEM;	

typedef WORD IDERR;	
typedef WORD MEMTYPE; 
typedef WORD MTHANDLE;

typedef struct tm     TIME,*LPTIME;
typedef struct _timeb TIMECOUNTER,*LPTIMECOUNTER;
#ifndef DLLEXPORT
#define DLLEXPORT __declspec(dllexport)
#endif

#ifndef DLLIMPORT
#define DLLIMPORT __declspec(dllimport)
#endif

#define TIMESECOND    time_t 
typedef struct _timeb TIMECOUNT;
#define GET_TIMECOUNT(lpTimeb)  _ftime((lpTimeb)) 
            //lpTimeb must be of TIMECOUNT type
#define TIME_LENGTH_SECOND(begin_time_t,end_time_t)	difftime((end_time_t),(begin_time_t)) 
			//begin_time_t and end_time_t must be of TIMESECOND type
#define TIME_LENGTH_MSECOND(tb1,tb2) (((tb2).time+((tb2).millitm*0.001))-((tb1).time+((tb1).millitm)*0.001)) 
            //tb1 and tb2 must be of TIMECOUNT type			 


extern MTHANDLE InitOnspec(void);
#define UNINITONSPEC()  { uninitialize();}


extern InitVarCvObj();
extern InitVarMvObj();
extern InitVarDvObj();
extern InitVarGvObj();

extern MallocAndInitVarCvObj();
extern MallocAndInitVarMvObj();
extern MallocAndInitVarDvObj();
extern MallocAndInitVarGvObj();

extern MEMTYPE IsWhatTypeCvMem(IDCVMEM);
extern MEMTYPE IsWhatTypeMvMem(IDMVMEM);
extern MEMTYPE IsWhatTypeDvMem(IDDVMEM);
extern MEMTYPE IsWhatTypeGvMem(IDGVMEM);

extern MTHANDLE TransCvID(WORD*,WORD*);
extern MTHANDLE TransMvID(WORD*,WORD*);
extern MTHANDLE TransDvID(WORD*,WORD*);
extern MTHANDLE TransGvID(WORD*,WORD*);

#endif