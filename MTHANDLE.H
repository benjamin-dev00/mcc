//////////////////////////////////////////////////////////
//Mthandle.h											//
//S.D.Chen 97-10-23										//
//////////////////////////////////////////////////////////

//Define ID of member type 
#define ISNOTCVMEM		0
#define ISCVCHARMEM 	1
#define ISCVDOUBLEMEM	2
#define ISCVINTMEM		3
#define ISCVBOOLMEM		4

#define ISNOTMVMEM		0
#define ISMVCHARMEM 	1
#define ISMVDOUBLEMEM	2
#define ISMVINTMEM		3
#define ISMVBOOLMEM		4

#define ISNOTDVMEM		0
#define ISDVCHARMEM 	1
#define ISDVDOUBLEMEM	2
#define ISDVINTMEM		3
#define ISDVBOOLMEM		4

#define ISNOTGVMEM		0
#define ISGVCHARMEM 	1
#define ISGVDOUBLEMEM	2
#define ISGVINTMEM		3
#define ISGVBOOLMEM		4

/////////////////////////////////////////////////
//Define No. of error
#define NOERROR					0
#define STOP_BY_ONSPEC			10

#define ONSPEC_NOT_RUNNING		100
#define ERROR_WRITING_ONSPEC	101
#define INVALID_PREFIX_NUMBER	102
#define INDEX_OUT_OF_RANGE		103
#define TAG_NAME_NOT_FOUND		104

#define INVALID_CVINDEX			201
#define INVALID_MVINDEX			202
#define INVALID_DVINDEX			203

#define INVALID_CVMEMINDEX		211
#define INVALID_MVMEMINDEX		212
#define INVALID_DVMEMINDEX		213
#define INVALID_GVMEMINDEX		214

#define FAIL_INITIALIZE_ONSPEC  301
#define FAIL_MEMORY_ALLOC		302

#define INVALID_TTFID			401