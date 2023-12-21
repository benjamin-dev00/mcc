////////////////////////////////////////////////////////////////////////
//Mtlocate.h															  //
//S.D.Chen   97-9-11												  //
////////////////////////////////////////////////////////////////////////	
#include "Mtid.h"
#include "Mthandle.h"
#include "Mtconst.h"


//Define the Max number of the global variables.
#define MAXNUM_GV_DBMEM		50	//Max_Number of double member of CobjPriv
#define MAXNUM_GV_INTMEM	50 //Max_Number of int member of CobjPriv
#define MAXNUM_GV_BOOLMEM	50 //Max_Number of BOOL member of CobjPriv
#define MAXNUM_GV_CHARMEM	50 //Max_Number of char-string member of CobjPriv 

////////////////////////////////////////////////////////////
//Define MultiVar Scale
#define CV_NUMBER    40 //Max_number of Cv 
#define MV_NUMBER    20 //Max_number of Mv 
#define DV_NUMBER    10 //Max_number of Dv  

#define MAXNUM_CV_DBMEM		50  //Max_Number of double member of every Cv.
#define MAXNUM_MV_DBMEM		50  //Max_Number of double member of every Mv
#define MAXNUM_DV_DBMEM		20  //Max_Number of double member of every Dv

#define MAXNUM_CV_INTMEM    50  //Max_Number of int member of every Cv.
#define MAXNUM_MV_INTMEM    50  //Max_Number of int member of every Mv
#define MAXNUM_DV_INTMEM    20  //Max_Number of int member of every Dv

#define MAXNUM_CV_CHARMEM   50  //Max_Number of char-string member of every Cv.
#define MAXNUM_MV_CHARMEM   50  //Max_Number of char_string member of every Mv
#define MAXNUM_DV_CHARMEM   20  //Max_Number of char-string member of every Dv

#define MAXNUM_CV_BOOLMEM	50  //Max_Number of BOOL member of every Cv.
#define MAXNUM_MV_BOOLMEM   50  //Max_Number of BOOL member of every Mv
#define MAXNUM_DV_BOOLMEM   20  //Max_Number of BOOL member of every Dv

///////////////////////////////////////////////////////////////
//The following allocate the EUR table.
//Allocate the location of all double-member in Onspec database
#define CV_DOUBLE_TOP    1001     //EUR4001                                                                                                                             
#define CV_DOUBLE_BOTTOM (CV_DOUBLE_TOP+CV_NUMBER*MAXNUM_CV_DBMEM-1) 

#define MV_DOUBLE_TOP    3001                      
#define MV_DOUBLE_BOTTOM (MV_DOUBLE_TOP+MV_NUMBER*MAXNUM_MV_DBMEM-1) 
                          
#define DV_DOUBLE_TOP    501                      
#define DV_DOUBLE_BOTTOM (DV_DOUBLE_TOP+DV_NUMBER*MAXNUM_DV_DBMEM-1) 

//Allocate the location of Globa double variables in Onspec database
#define GV_DOUBLE_TOP    1   //EUR1                      
#define GV_DOUBLE_BOTTOM 50   //EUR50

#define SAMPTIME_LOCATION 51
///////////////////////////////////////////////////////////////
//The following allocate the EUI table.
//Allocate the location of all int-member in Onspec database
#define CV_INT_TOP    1001        //EUI4001                                                                                                                             
#define CV_INT_BOTTOM (CV_INT_TOP+CV_NUMBER*MAXNUM_CV_INTMEM-1) 

#define MV_INT_TOP    3001                       
#define MV_INT_BOTTOM (MV_INT_TOP+MV_NUMBER*MAXNUM_MV_INTMEM-1) 
                          
#define DV_INT_TOP    501                       
#define DV_INT_BOTTOM (DV_INT_TOP+DV_NUMBER*MAXNUM_DV_INTMEM-1) 
/*
//Allocate the location of all status of VAR        
#define CV_STATUS_TOP 7001		//EUI7001
#define MV_STATUS_TOP (CV_STATUS_TOP+CV_NUMBER)
#define DV_STATUS_TOP (MV_STATUS_TOP+MV_NUMBER)
*/
//Allocate the location of the global int variable in Onspec database
#define GV_INT_TOP     1   //EUI1
#define GV_INT_BOTTOM  50	//EUI50

///////////////////////////////////////////////////////////////
//The following allocate the STR table.
//Allocate the lacation of all char-member in Onspec database
#define CV_CHAR_TOP    1001     //STR4001                                                                                                                             
#define CV_CHAR_BOTTOM (CV_CHAR_TOP+CV_NUMBER*MAXNUM_CV_CHARMEM-1) 

#define MV_CHAR_TOP    3001                        
#define MV_CHAR_BOTTOM (MV_CHAR_TOP+MV_NUMBER*MAXNUM_MV_CHARMEM-1) 
                          
#define DV_CHAR_TOP    501                        
#define DV_CHAR_BOTTOM (DV_CHAR_TOP+DV_NUMBER*MAXNUM_DV_CHARMEM-1) 
        
//Allocate the location of the global char variable in Onspec database
#define GV_CHAR_TOP    1	//STR8001                      
#define GV_CHAR_BOTTOM 50	//STR8100

///////////////////////////////////////////////////////////////
//The following allocate the DII table.
//Allocate the lacation of all BOOL-member in Onspec database
#define CV_BOOL_TOP    1001     //DII4001                                                                                                                             
#define CV_BOOL_BOTTOM (CV_BOOL_TOP+CV_NUMBER*MAXNUM_CV_BOOLMEM-1) 

#define MV_BOOL_TOP    3001                        
#define MV_BOOL_BOTTOM (MV_BOOL_TOP+MV_NUMBER*MAXNUM_MV_BOOLMEM-1) 
                          
#define DV_BOOL_TOP    501                        
#define DV_BOOL_BOTTOM (DV_BOOL_TOP+DV_NUMBER*MAXNUM_DV_BOOLMEM-1) 

//Allocate the location of the global BOOL variable in Onspec database
#define GV_BOOL_TOP    1	//DII1
#define GV_BOOL_BOTTOM 50	//DII50


///////////////////////////////////////////////////////////////
//The following allocate the FLG table.
//CV:4001-5000
//MV:5001-6000