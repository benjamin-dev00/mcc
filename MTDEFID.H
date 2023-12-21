//////////////////////////////////////////////////////////
//Mtdefid.h												//														
//S.D.Chen 97-10-23										//
//////////////////////////////////////////////////////////
//Define the actual number of the global variables.
#define GV_DOUBLE_NUMBER	6    //Allow to be added up to 192
#define GV_INT_NUMBER		8    //Allow to be added up to 192
#define GV_BOOL_NUMBER		12   //Allow to be added up to 192
#define GV_CHAR_NUMBER		0    //Allow to be added up to 192
//Define the actual number of each member  
#define CV_DOUBLE_NUMBER	27    //Allow to be added up to 50
#define CV_INT_NUMBER		2     //Allow to be added up to 20
#define CV_BOOL_NUMBER		7     //Allow to be added up to 20
#define CV_CHAR_NUMBER		4     //Allow to be added up to 20

#define MV_DOUBLE_NUMBER	34    //Allow to be added up to 50 
#define MV_INT_NUMBER		2     //Allow to be added up to 20 
#define MV_BOOL_NUMBER		11    //Allow to be added up to 20 
#define MV_CHAR_NUMBER		4     //Allow to be added up to 20 

#define DV_DOUBLE_NUMBER	6     //Allow to be added up to 20
#define DV_INT_NUMBER		0     //Allow to be added up to 20
#define DV_BOOL_NUMBER		1     //Allow to be added up to 20
#define DV_CHAR_NUMBER		4     //Allow to be added up to 20


//////////////////////////////////////////////////////////
//Define index-micro of char member  in CV,MV,and DV
#define ID_TAG       0
#define ID_DataName  1
#define ID_Name      2
#define ID_Unit      3

//Define ID-micro of double member in CV
#define ID_CData			100
#define ID_CHScale			101
#define ID_CLScale			102
#define ID_CSetpoint		103
#define ID_CIni				104
#define ID_CMax				105
#define ID_CMin				106
#define ID_CHigh			107
#define ID_CLow				108
#define ID_CFault_max		109
#define ID_CFault_min		110
#define ID_CFault_high		111
#define ID_CFault_low		112
#define ID_CFault_xsetpoint	113
#define ID_CFault_nsetpoint	114
#define ID_CFault_hsetpoint	115
#define ID_CFault_lsetpoint	116
#define ID_CLPIni			117
#define ID_CLPIndex			118
#define ID_CSteadOptim		119
#define ID_CQ				120
#define ID_CQL				121
#define ID_CQMax			122
#define ID_CDelQ			123
#define ID_CRealQ			124
#define ID_CHAlarm			125
#define ID_CLAlarm			126
//Define index-micro of int member in CV
#define ID_CRControl_type	200
#define ID_CPriority		201
//Define index-micro of BOOL member in CV
#define ID_CControl_type	300
#define ID_CQMode			301
#define ID_CErrorMode		302
#define ID_CInValid			303
#define ID_CAlarm			304
#define ID_CLimit			305
#define ID_CKey				306

//Define index-micro of double member in MV
#define ID_MData			100
#define ID_MHScale			101
#define ID_MLScale			102
#define ID_MDelt			103
#define ID_MIrv				104
#define ID_MIni				105
#define ID_MDelmax			106
#define ID_MDelmin			107
#define ID_MMax				108
#define ID_MMin				109
#define ID_MIrvHigh			110
#define ID_MIrvLow			111
#define ID_MFault_max		112
#define ID_MFault_min		113
#define ID_MIrvFault_high	114
#define ID_MIrvFault_low	115
#define ID_MFault_xirv		116
#define ID_MFault_nirv		117
#define ID_MFault_hirv		118
#define ID_MFault_lirv		119
#define ID_MLPIni			120
#define ID_MLPIndex			121
#define ID_MSteadOptim		122
#define ID_MR				123
#define ID_MRMax			124
#define ID_MP				125
#define ID_MPL				126
#define ID_MPMax			127
#define ID_MR				128
#define ID_MDelP			129
#define ID_MRealR			130
#define ID_MRealP			131
#define ID_MHAlarm			132
#define ID_MLAlarm			133
//Define index-micro of int member in MV
#define ID_MRControl		200
#define ID_MPriority		201
//Define index-micro of BOOL member in MV
#define ID_MControl_type	300
#define ID_MRCorrectMode	301
#define ID_MSCorrectMode	302
#define ID_MErrorMode		303
#define ID_MInValid			304
#define ID_MDist			305
#define ID_MSatuation		306
#define ID_MAlarm			307
#define ID_MAM				308
#define ID_MHopeAM			309
#define	ID_MKey				310

//Define index-micro of double member in DV
#define ID_DData			100
#define ID_DHScale			101
#define ID_DLScale			102
#define ID_DIni				103
#define ID_DDelt			104
#define ID_DHistory			105
//Define index-micro of BOOL member in DV
#define ID_DInValid			300

///////////////////////////////////////////
//define the ID of the double member of GV
#define ID_LPK				100
#define ID_Alpha			101
#define ID_Big_Cond			102
#define ID_PBroken			103
#define ID_QBroken			104
#define ID_RBroken			105
//define the ID of the int member of GV
#define ID_File_No			200
#define ID_N				201
#define ID_P				202
#define ID_M				203
#define ID_p				204	
#define ID_m				205
#define ID_nd				206
#define ID_Count			207
//define the ID of the BOOL member of GV
#define ID_Flag_CControl	300
#define ID_Flag_MControl	301
#define ID_Flag_Fault		302
#define ID_Flag_LP			303
#define ID_Flag_AM			304
#define ID_Flag_Alarm		305
#define ID_Flag_ReSP		306
#define ID_Flag_Run			307
#define ID_Flag_InterAction	308
#define ID_Flag_MChange		309
#define ID_Flag_LPChange	310

////////////////////////////////////////////////////////////
//Define first_ID of each member-type of struct CV,MV and DV
#define IDCV_FIRSTCHARMEM		ID_TAG
#define IDCV_FIRSTDOUBLEMEM	    ID_CData
#define IDCV_FIRSTINTMEM		ID_CRControl_type
#define IDCV_FIRSTBOOLMEM		ID_CControl_type

#define IDMV_FIRSTCHARMEM		ID_TAG
#define IDMV_FIRSTDOUBLEMEM		ID_MData
#define IDMV_FIRSTINTMEM		ID_MRControl_type
#define IDMV_FIRSTBOOLMEM		ID_MControl_type

#define IDDV_FIRSTCHARMEM		ID_TAG
#define IDDV_FIRSTDOUBLEMEM		ID_DData
#define IDDV_FIRSTINTMEM
#define IDDV_FIRSTBOOLMEM		ID_DInValid

#define IDGV_FIRSTCHARMEM	
#define IDGV_FIRSTDOUBLEMEM		ID_LPK
#define IDGV_FIRSTINTMEM		ID_File_No
#define IDGV_FIRSTBOOLMEM		ID_Flag_CControl

 
//Define last_ID of each member-type of struct CV,MV and DV
#define IDCV_LASTCHARMEM	(ID_FIRSTCHARMEM+CV_CHAR_NUMBER-1)
#define IDCV_LASTDOUBLEMEM	(ID_FIRSTDOUBLEMEM+CV_DOUBLE_NUMBER-1)
#define IDCV_LASTINTMEM		(ID_FIRSTINTMEM+CV_INT_NUMBER-1)
#define IDCV_LASTBOOLMEM	(ID_FIRSTBOOLMEM+CV_BOOL_NUMBER-1)
 
#define IDMV_LASTCHARMEM	(ID_FIRSTCHARMEM+MV_CHAR_NUMBER-1)
#define IDMV_LASTDOUBLEMEM	(ID_FIRSTDOUBLEMEM+MV_DOUBLE_NUMBER-1)
#define IDMV_LASTINTMEM	    (ID_FIRSTINTMEM+MV_INT_NUMBER-1)
#define IDMV_LASTBOOLMEM	(ID_FIRSTBOOLMEM+MV_BOOL_NUMBER-1)

#define IDDV_LASTCHARMEM	(ID_FIRSTCHARMEM+DV_CHAR_NUMBER-1)
#define IDDV_LASTDOUBLEMEM	(ID_FIRSTDOUBLEMEM+DV_DOUBLE_NUMBER-1)
#define IDDV_LASTINTMEM		
#define IDDV_LASTBOOLMEM	(ID_FIRSTBOOLMEM+DV_BOOL_NUMBER-1)

#define IDGV_LASTCHARMEM	
#define IDGV_LASTDOUBLEMEM	(ID_FIRSTDOUBLEMEM+GV_DOUBLE_NUMBER-1)
#define IDGV_LASTINTMEM		(ID_FIRSTINTMEM+GV_INT_NUMBER-1)
#define IDGV_LASTBOOLMEM	

//Define the micros of the name of first member of each type.
#define FIRST_CVCHARMEM     TAG
#define FIRST_CVDOUBLEMEM   CData
#define FIRST_CVINTMEM		CControl_type	
#define FIRST_CVBOOLMEM		QMode

#define FIRST_MVCHARMEM     TAG
#define FIRST_MVDOUBLEMEM   MData
#define FIRST_MVINTMEM		MControl_type	
#define FIRST_MVBOOLMEM		RMode

#define FIRST_DVCHARMEM     TAG
#define FIRST_DVDOUBLEMEM   DData
#define FIRST_DVINTMEM			
#define FIRST_DVBOOLMEM		Validate

#define FIRST_GVCHARMEM     
#define FIRST_GVDOUBLEMEM   LpK
#define FIRST_GVINTMEM		m	
#define FIRST_GVBOOLMEM		