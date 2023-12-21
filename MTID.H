//////////////////////////////////////////////////////////
//Mtid.h												//														
//S.D.Chen 97-10-23										//
//////////////////////////////////////////////////////////
//Define the actual number of the global variables.
#define GV_DOUBLE_NUMBER	10    //Allow to be added up to 50
#define GV_INT_NUMBER		17    //Allow to be added up to 50
							//16 WORD(File_No to SteadyStep) +1 int(Freecount)		
#define GV_BOOL_NUMBER		22   //Allow to be added up to 50
#define GV_CHAR_NUMBER		0    //Allow to be added up to 50

//Define the actual number of each member of CV,MV and DV.  
#define CV_DOUBLE_NUMBER	33    //Allow to be added up to 50
#define CV_INT_NUMBER		5     //Allow to be added up to 50
#define CV_BOOL_NUMBER		10     //Allow to be added up to 50
#define CV_CHAR_NUMBER		0     //Allow to be added up to 20

#define MV_DOUBLE_NUMBER	38    //Allow to be added up to 50 
#define MV_INT_NUMBER		4     //Allow to be added up to 50 
#define MV_BOOL_NUMBER		13    //Allow to be added up to 50 
#define MV_CHAR_NUMBER		0     //Allow to be added up to 20 

#define DV_DOUBLE_NUMBER	9     //Allow to be added up to 20
#define DV_INT_NUMBER		0     //Allow to be added up to 20
#define DV_BOOL_NUMBER		1     //Allow to be added up to 20
#define DV_CHAR_NUMBER		0	  //Allow to be added up to 20


//////////////////////////////////////////////////////////
//Define index-micro of char member  in CV,MV,and DV
#define ID_TAG       0
#define ID_DataName  1
#define ID_Name      2
#define ID_Unit      3

//Define ID-micro of the members in CV

//double:
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
#define ID_CError			127
#define ID_CDisplayH		128
#define ID_CDisplayL		129
#define ID_CDisplay			130
#define ID_CSteadyDelt		131
#define ID_CSteadyField		132

//int:
#define ID_CRControl_type	200
#define ID_CPriority		201
#define ID_CNoLP			202
#define ID_CHOverCount		203
#define ID_CLOverCount		204
#define ID_WORDCCONTROLTYPE 205	//used by CControl_type
//BOOL:
#define ID_CControl_type	300	//it's WORD in CV struct
#define ID_CInvalid			301
#define ID_CAlarm			302
#define ID_CKey				303
#define ID_CHOver			304
#define ID_CLOver			305
#define ID_CLPWrong			306
#define ID_CNoIll			307
#define ID_CHEdge			308
#define ID_CLEdge			309

//Define index-micro of the members in MV
//double:
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
#define ID_MDelR			128
#define ID_MDelP			129
#define ID_MRealR			130
#define ID_MRealP			131
#define ID_MHAlarm			132
#define ID_MLAlarm			133
#define ID_MDisplayH		134
#define ID_MDisplayL		135
#define ID_MDisplay			136
#define ID_MSample			137
//int:
#define ID_MRControl_type	200
#define ID_MPriority		201
#define ID_MHOverCount		202
#define ID_MLOverCount		203
#define ID_WORDMCONTROLTYPE 204	//used by Flag_AM

//BOOL:
#define ID_MControl_type	300	//it's WORD in MV Struct
#define ID_MInvalidate		301
#define ID_MDist			302
#define ID_MSatuation		303
#define ID_MAlarm			304
#define	ID_MKey				305
#define ID_MSetSatuation	306
#define ID_MIRVWrong		307
#define ID_MApartAM			308
#define ID_MAM				309
#define ID_MHope_AM			310
#define ID_MHEdge			311
#define ID_MLEdge			312

//Define index-micro of the members in DV
//double:
#define ID_DData			100
#define ID_DHScale			101
#define ID_DLScale			102
#define ID_DIni				103
#define ID_DDelt			104
#define ID_DHistory			105
#define ID_DDisplayH		106
#define ID_DDisplayL		107
#define ID_DDisplay			108
//BOOL:
#define ID_DInvalid			300

///////////////////////////////////////////
//define the ID of the members of GV
//double:
#define ID_LpK				100
#define ID_Alpha			101
#define ID_Big_Cond			102
#define ID_PBroken			103
#define ID_QBroken			104
#define ID_RBroken			105
#define ID_SRError			106
#define ID_DSRError			107
#define ID_Cor_Steady		108
#define ID_SampleTime		109
//int:
#define ID_File_No			200
#define ID_N				201
#define ID_P				202
#define ID_M				203
#define ID_p				204	
#define ID_m				205
#define ID_nd				206
#define ID_Count			207
#define ID_MakeParaCount	208
#define ID_RecountCount		209
#define ID_LPCount			210
#define ID_No_Apart			211
#define ID_MParaCount		212
#define ID_MParaMax			213
#define ID_SteadyStep		214
#define ID_ErrorCode		215
#define ID_Freecount		216 //int type in ttf
//BOOL:
#define ID_Flag_CControl	300
#define ID_Flag_MControl	301
#define ID_Flag_Fault		302
#define ID_Flag_LP			303
#define ID_Flag_Alarm		304
#define ID_Flag_ReSP		305
#define ID_Flag_SRMode		306
#define ID_Flag_MChange		307
#define ID_Flag_LPChange	308
#define ID_Flag_HopeAM		309
#define ID_Flag_Recount		310
#define ID_Flag_LPRecount	311
#define ID_StartMode		312
#define ID_Flag_Weight		313
#define ID_Flag_Debug		314
#define ID_Flag_HMode		315
#define ID_Flag_AM1			316
#define ID_Flag_AM2			317
#define ID_Flag_AM3			318
#define ID_Flag_Run			319
#define ID_Flag_Steady		320
#define ID_Flag_AM			321 //becase it's actually WORD type in ttf ,
								//so it should be in the end of ID of Gv forever!!


////////////////////////////////////////////////////////////
//Define first_ID of each member-type of struct CV,MV and DV
#define IDCV_FIRSTCHARMEM		ID_TAG
#define IDCV_FIRSTDOUBLEMEM	    ID_CData
#define IDCV_FIRSTINTMEM		ID_CRControl_type
#define IDCV_FIRSTBOOLMEM		ID_CInvalid

#define IDMV_FIRSTCHARMEM		ID_TAG
#define IDMV_FIRSTDOUBLEMEM		ID_MData
#define IDMV_FIRSTINTMEM		ID_MRControl_type
#define IDMV_FIRSTBOOLMEM		ID_MInvalidate

#define IDDV_FIRSTCHARMEM		ID_TAG
#define IDDV_FIRSTDOUBLEMEM		ID_DData
#define IDDV_FIRSTINTMEM
#define IDDV_FIRSTBOOLMEM		ID_DInvalid

#define IDGV_FIRSTCHARMEM	
#define IDGV_FIRSTDOUBLEMEM		ID_LpK
#define IDGV_FIRSTINTMEM		ID_File_No
#define IDGV_FIRSTBOOLMEM		ID_Flag_CControl

 
//Define last_ID of each member-type of struct CV,MV and DV
#define IDCV_LASTCHARMEM	//(IDCV_FIRSTCHARMEM+CV_CHAR_NUMBER-1)
#define IDCV_LASTDOUBLEMEM	(IDCV_FIRSTDOUBLEMEM+CV_DOUBLE_NUMBER-1)
#define IDCV_LASTINTMEM		(IDCV_FIRSTINTMEM+CV_INT_NUMBER-1)
#define IDCV_LASTBOOLMEM	(IDCV_FIRSTBOOLMEM+CV_BOOL_NUMBER-1)
 
#define IDMV_LASTCHARMEM	//(IDMV_FIRSTCHARMEM+MV_CHAR_NUMBER-1)
#define IDMV_LASTDOUBLEMEM	(IDMV_FIRSTDOUBLEMEM+MV_DOUBLE_NUMBER-1)
#define IDMV_LASTINTMEM	    (IDMV_FIRSTINTMEM+MV_INT_NUMBER-1)
#define IDMV_LASTBOOLMEM	(IDMV_FIRSTBOOLMEM+MV_BOOL_NUMBER-1)

#define IDDV_LASTCHARMEM	//(IDDV_FIRSTCHARMEM+DV_CHAR_NUMBER-1)
#define IDDV_LASTDOUBLEMEM	(IDDV_FIRSTDOUBLEMEM+DV_DOUBLE_NUMBER-1)
#define IDDV_LASTINTMEM		
#define IDDV_LASTBOOLMEM	(IDDV_FIRSTBOOLMEM+DV_BOOL_NUMBER-1)

#define IDGV_LASTCHARMEM	
#define IDGV_LASTDOUBLEMEM	(IDGV_FIRSTDOUBLEMEM+GV_DOUBLE_NUMBER-1)
#define IDGV_LASTINTMEM		(IDGV_FIRSTINTMEM+GV_INT_NUMBER-1)
#define IDGV_LASTBOOLMEM	(IDGV_FIRSTBOOLMEM+GV_BOOL_NUMBER-1)

//Define the micros of the name of first member of each struct
#define FIRST_CVCHARMEM     TAG
#define FIRST_CVDOUBLEMEM   CData
#define FIRST_CVINTMEM		CRControl_type	
#define FIRST_CVBOOLMEM		CInvalid

#define FIRST_MVCHARMEM     TAG
#define FIRST_MVDOUBLEMEM   MData
#define FIRST_MVINTMEM		MRControl_type	
#define FIRST_MVBOOLMEM		MInvalidate

#define FIRST_DVCHARMEM     TAG
#define FIRST_DVDOUBLEMEM   DData
#define FIRST_DVINTMEM			
#define FIRST_DVBOOLMEM		DInvalid

#define FIRST_GVCHARMEM     
#define FIRST_GVDOUBLEMEM   LpK
#define FIRST_GVINTMEM		File_No	
#define FIRST_GVBOOLMEM		Flag_CControl
