#ifndef __MTDLLPV_H
#define __MTDLLPV_H

#define DATA_CV1	1001
#define DATA_CV2	1051
#define DATA_CV3	1101
#define DATA_MV1	3001
#define DATA_MV2	3051
#define DATA_MV3	3101

//The following for MV
#define DATA_T101			3001
#define DATA_G205			3051
#define DATA_G104			3101
#define DATA_G204			3151
#define DATA_T101_30		3201
//
#define DATA_T202			3251
#define DATA_G212			3301
#define DATA_G213			3351
#define DATA_T203			3401
#define DATA_G215			3451
//The following for CV
#define DATA_A101_2			1001
#define DATA_Delt_P116II	1051
#define DATA_Delt_P110II	1101
#define DATA_Delt_P117II	1151
#define DATA_T113_3			1201
#define DATA_Delt_T111II_8	1251  
#define DATA_T110_50		1301
#define DATA_LG_207			1351
#define DATA_LG_204			1401
#define DATA_LG_205			1451
#define DATA_1_402			1501
#define DATA_1_403			1551
#define DATA_T110_43		1601
#define DATA_T211_2			1651
#define DATA_T110_41		1701
//The following for PV
#define DATA_TI_72			401
#define DATA_TI_20			402
#define DATA_T211_3			403
#define DATA_T110_42		404
#define DATA_G305			405
#define DATA_TI_70			406
#define DATA_TI_33			407
#define DATA_P301			408

	
#ifdef __cplusplus
extern "C" {
#endif
int ReadDataFromOnspec( WORD _i_UnitIndex, double *_lpData);
int WriteDataToOnspec( WORD _i_UnitIndex, double *_lpData);
int ReadTagDataFromOnspec( char *_cTag, double *_lpData);
int WriteTagDataToOnspec( char *_cTag, double *_lpData);
int ReadTagBoolDataFromOnspec( char *_cTag, BOOL *_lpData);
int WriteTagBoolDataToOnspec( char *_cTag, BOOL *_lpData);

#ifdef __cplusplus
}
#endif

#endif