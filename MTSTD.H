//////////////////////////////////////////////////////
//Mtstruct.h										//
//S.D.Chen    97-9-11								//
//////////////////////////////////////////////////////
#include <windows.h>
#include <time.h>
#include <timeb.h>
#include "Mtlocate.h"
#include "Onworld.h"

typedef int IDCVMEM;	
typedef int IDMVMEM;	
typedef int IDDVMEM;
typedef int IDGVMEM;	

typedef int IDERR;	
typedef int MEMTYPE; 
typedef int MTHANDLE;
 

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


extern DLLEXPORT MTHANDLE InitOnspec(void);
#define UnInitOnspec()  uninitialize()


