#include "tfalgo.h"
#include "tfcmdmc.h"
#define	pp						4
#define mm						2
#define ndnd					3
#define	NN						60
#define PP						25
#define MM                      9
extern 	DWORD _type_MV[NUMBER_MVPARAM];
extern	DWORD _type_CV[NUMBER_CVPARAM];
extern 	DWORD _type_DV[NUMBER_DVPARAM];
REAL 	_MVReal[mm][NUMBER_MVREAL] = {
					{245, 285, 205, 0, 245,
					  245, 20, -20, 285, 205,
					  285, 205, 284, 206, 284,
					  206, 280, 210, 280, 210,
					  245, 4, 245, 1, 50, 0.01,
					  0.001, 50, 0.1, 0.0001, 0.6, 0.01},
					{34, 44, 24, 0, 34,
					  34, 5, -5, 44, 24,
					  44, 24, 43, 25, 43,
					  25, 40, 28, 40, 28,
					  34, 5, 34, 1, 50, 0.01,
					  0.001, 50, 0.1, 0.0001, 1, 0.01},
				};
WORD 	_MVWord[mm][NUMBER_MVWORD] =
				{
					{ 0, 4, 0, 0 },
					{ 1, 4, 1, 0 },
				};
BOOL 	_MVBool[mm][NUMBER_MVBOOL] =
					{
						{ 0, 0, 0, 0, 0},
						{ 0, 0, 0, 0, 0},
					};

REAL 	_CVReal[pp][NUMBER_CVREAL] =
					{
						{ 934, 1034, 834, 934, 934,
						  1034, 834, 1034, 834,
						  1033, 835, 1033, 835,
						  1030, 838, 1030, 838,
						  934, 1, 934, 10, 5, 50,
						  1, 10 },
						{ 1238, 1338, 1138, 1238, 1238,
						  1338, 1138, 1338, 1138, 
						  1337, 1139, 1337, 1139, 
						  1335, 1141, 1335, 1141, 
						  1238, 2, 1238, 0.5, 0.5, 50, 
						  0.001, 0.5 },
						{ 0.0015, 0.0025, 0.0005, 0.0015, 0.0015,
						  0.0025, 0.0005, 0.0025, 0.0005,  
						  0.0024, 0.0006, 0.0024, 0.0006, 
						  0.0022, 0.0008, 0.0022, 0.0008, 
						  0.0015, 3, 0.0015, 1, 5, 50,
						  0.1, 10 },
						{ 0.02, 0.03, 0.01, 0.02, 0.02,
						  0.03, 0.01, 0.03, 0.01, 
						  0.029, 0.011, 0.029, 0.011,
						  0.027, 0.013, 0.027, 0.013,
						  0.02, 3, 0.02, 10, 5, 50, 
						  1, 10 },

					 };
//
WORD 	_CVWord[pp][NUMBER_CVWORD] =
						{
							{ 4, 4, 0 },
							{ 4, 4, 5 },
							{ 1, 4, 1 },
							{ 4, 4, 10 },
						 };
// Control_mode, Real Control-mode, Priority
BOOL 	_CVBool[pp][NUMBER_CVBOOL] =
						{
							{ 0, 1, 1, 0, 0 },
							{ 0, 1, 1, 0, 0 },
							{ 0, 1, 1, 0, 0 },
							{ 0, 1, 1, 0, 0 },
						 };
// Q, Err, Valid, Alarm, Limit;
REAL 	_DVReal[ndnd][NUMBER_DVREAL] =
				{
					{2000, 2200, 1800, 2000, 0, 2000},
					{2000, 2200, 1800, 2000, 0, 2000},
					{2000, 2200, 1800, 2000, 0, 2000},
				};
BOOL 	_DVBool[ndnd][NUMBER_DVBOOL] =
				{
					{1 },
					{0 },
					{0 },
				};
WORD	_DVWord[ndnd][NUMBER_DVWORD] = {{0}};
char	_DVCobj[ndnd][NUMBER_DVCOBJ][20] =
				{
					{"SORRY", "LCD IS A DISTURBANCE", "IT IS TRUE"},
					{"SORRY", "LCD IS A DISTURBANCE", "IT IS TRUE"},
					{"SORRY", "LCD IS A DISTURBANCE", "IT IS TRUE"},
				};
char	_MVCobj[mm][NUMBER_MVCOBJ][20] =
				{
					{"SORRY", "LCD", "IT IS TRUE"},
					{"SORRY", "LCD", "IT IS TRUE"},
				};
char	_CVCobj[pp][NUMBER_CVCOBJ][20] =
				{
					{"SORRY", "LCD IS A DISTURBANCE", "IT IS TRUE"},
					{"SORRY", "LCD IS A DISTURBANCE", "IT IS TRUE"},
					{"SORRY", "LCD IS A DISTURBANCE", "IT IS TRUE"},
				};

void PrivParamInitialize( WRD wMTU_, WRD wNo_ )
{
	LPPU 			_lppu, _lptempPU;
	LPLINKMATRIX	_lpLinkMatrix;
	LPCOBJ			_lpCobj_CV, _lpCobj_MV, _lpCobj_DV;
	int				_i, _j, _kr, _kb, _kw, _kc;
//	LPMATRIX		_lpStepDV, _lpStepMV;
//	LPCOBJ			_lpCobj_StepDV, _lpCobj_StepMV;
//	LPCOBJ			_lpCobj_PrivDV, _lpCobj_PrivMV, _lpCobj_PrivCV;

	_lppu = MeterGetParameter(wMTU_);
	if ( wNo_ == MDMCIRVD_p )
		MFPUWRITE( _lppu, MDMCIRVD_p, WORD, pp );
	if ( wNo_ == MDMCIRVD_m )
		MFPUWRITE( _lppu, MDMCIRVD_m, WORD, mm );
	if ( wNo_ == MDMCIRVD_nd )
		MFPUWRITE( _lppu, MDMCIRVD_nd, WORD, ndnd );
	if ( wNo_ == MDMCIRVD_N )
		MFPUWRITE( _lppu, MDMCIRVD_N, WORD, 60 );
	if ( wNo_ == MDMCIRVD_P )
		MFPUWRITE( _lppu, MDMCIRVD_P, WORD, 25 );
	if ( wNo_ == MDMCIRVD_M )
		MFPUWRITE( _lppu, MDMCIRVD_M, WORD, 9 );
	if ( wNo_ == MDMCIRVD_alpha )
		MFPUWRITE( _lppu, MDMCIRVD_alpha, REAL, 0.5 );
	if ( wNo_ == MDMCIRVD_count )
		MFPUWRITE( _lppu, MDMCIRVD_count, WORD, 0 );
	if ( wNo_ == MDMCIRVD_FLAGAM )
		MFPUWRITE( _lppu, MDMCIRVD_FLAGAM, WORD, 0 );
	if ( wNo_ == MDMCIRVD_FLAGALARM )
		MFPUWRITE( _lppu, MDMCIRVD_FLAGALARM, WORD, 0 );
	if ( wNo_ == MDMCIRVD_FLAGLP )
		MFPUWRITE( _lppu, MDMCIRVD_FLAGLP, WORD, 1 );
	if ( wNo_ == MDMCIRVD_FLAGFAULT )
		MFPUWRITE( _lppu, MDMCIRVD_FLAGFAULT, WORD, 0 );
	if ( wNo_ == MDMCIRVD_BIGCOND )
		MFPUWRITE( _lppu, MDMCIRVD_BIGCOND, REAL, 100 );
	if ( wNo_ == MDMCIRVD_MCLASSIF )
		MFPUWRITE( _lppu, MDMCIRVD_MCLASSIF, WORD, 0 );
	if ( wNo_ == MDMCIRVD_CCLASSIF )
		MFPUWRITE( _lppu, MDMCIRVD_CCLASSIF, WORD, 1 );
	if ( wNo_ == MDMCIRVD_PBROKEN )
		MFPUWRITE( _lppu, MDMCIRVD_PBROKEN, REAL, 50 );
	if ( wNo_ == MDMCIRVD_RBROKEN )
		MFPUWRITE( _lppu, MDMCIRVD_RBROKEN, REAL, 50 );
	if ( wNo_ == MDMCIRVD_QBROKEN )
		MFPUWRITE( _lppu, MDMCIRVD_QBROKEN, REAL, 50 );
	if( wNo_ == MDMCIRVD_CV )
	{
		_lpLinkMatrix = CVStructAlloc( NUMBER_CVPARAM, pp );
		for( _i = 0; _i < pp; _i++ )
		{
			_kr = _kb = _kw = _kc = 0;
			for( _j = 0; _j < NUMBER_CVPARAM; _j++ )
			{
				_lptempPU = DLM_GET_LPPU( _lpLinkMatrix, _j, _i );
				if( _type_CV[_j] == VSOF_REAL )
				{
					_lptempPU->wPrefix = PF_IMM|IMM_REAL;
					_lptempPU->du.zzk__REAL = _CVReal[_i][_kr];
					_kr++;
				}

				if( _type_CV[_j] == VSOF_BOOL )
				{
					_lptempPU->wPrefix = PF_IMM|IMM_BOOL;
					_lptempPU->du.zzk__BOOL = _CVBool[_i][_kb];
					_kb++;
				}
				if( _type_CV[_j] == VSOF_WORD )
				{
					_lptempPU->wPrefix = PF_IMM|IMM_WORD;
					_lptempPU->du.zzk__WORD = _CVWord[_i][_kw];
					_kw++;
				}
				if( _type_CV[_j] == VSOF_COBJ )
				{
					_lptempPU->wPrefix = PF_IMM|IMM_COBJ;
					_lptempPU->du.zzk__COBJ = NULL;

				}
			}
//			if( ( _kr + _kb + _kc + _kw ) != NUMBER_CVPARAM )
//				MessageBox( NULL, "number of CV", "error", MB_OK );
		}
		_lpCobj_CV = LINKMATRIX_2_COBJ( _lpLinkMatrix );
		MFPUWRITE( _lppu, MDMCIRVD_CV, COBJ, _lpCobj_CV );
	}
	if( wNo_ == MDMCIRVD_MV )
	{
		_lpLinkMatrix = MVStructAlloc( NUMBER_MVPARAM, mm );
		for( _i = 0; _i < mm; _i++ )
		{
			_kr = _kb = _kw = _kc = 0;
			for( _j = 0; _j < NUMBER_MVPARAM; _j++ )
			{
				_lptempPU = DLM_GET_LPPU( _lpLinkMatrix, _j, _i );
				if( _type_MV[_j] == VSOF_REAL )
				{
					_lptempPU->wPrefix = PF_IMM|IMM_REAL;
					_lptempPU->du.zzk__REAL = _MVReal[_i][_kr];
					_kr++;
				}

				if( _type_MV[_j] == VSOF_BOOL )
				{
					_lptempPU->wPrefix = PF_IMM|IMM_BOOL;
					_lptempPU->du.zzk__BOOL = _MVBool[_i][_kb];
					_kb++;
				}
				if( _type_MV[_j] == VSOF_WORD )
				{
					_lptempPU->wPrefix = PF_IMM|IMM_WORD;
					_lptempPU->du.zzk__WORD = _MVWord[_i][_kw];
					_kw++;
				}
				if( _type_MV[_j] == VSOF_COBJ )
				{
					_lptempPU->wPrefix = PF_IMM|IMM_COBJ;
					_lptempPU->du.zzk__COBJ = NULL;
					_kc++;
				}
			}
//			if( ( _kr + _kb + _kc + _kw ) != NUMBER_MVPARAM )
 //				MessageBox( NULL, "number of MV", "error", MB_OK );
		}
		_lpCobj_MV = LINKMATRIX_2_COBJ( _lpLinkMatrix );
		MFPUWRITE( _lppu, MDMCIRVD_MV, COBJ, _lpCobj_MV );
	}
	if( wNo_ == MDMCIRVD_DV )
	{
		_lpLinkMatrix = DVStructAlloc( NUMBER_DVPARAM, ndnd );
		for( _i = 0; _i < ndnd; _i++ )
		{
			_kr = _kb = _kw = _kc = 0;
			for( _j = 0; _j < NUMBER_DVPARAM; _j++ )
			{
				_lptempPU = DLM_GET_LPPU( _lpLinkMatrix, _j, _i );
				if( _type_DV[_j] == VSOF_REAL )
				{
					_lptempPU->wPrefix = PF_IMM|IMM_REAL;
					_lptempPU->du.zzk__REAL = _DVReal[_i][_kr];
					_kr++;
				}

				if( _type_DV[_j] == VSOF_BOOL )
				{
					_lptempPU->wPrefix = PF_IMM|IMM_BOOL;
					_lptempPU->du.zzk__BOOL = _DVBool[_i][_kb];
                    _kb++;
				}
				if( _type_DV[_j] == VSOF_WORD )
				{
					_lptempPU->wPrefix = PF_IMM|IMM_WORD;
					_lptempPU->du.zzk__WORD = _DVWord[_i][_kw];
					_kw++;
				}
				if( _type_DV[_j] == VSOF_COBJ )
				{
					_lptempPU->wPrefix = PF_IMM|IMM_COBJ;
					_lptempPU->du.zzk__COBJ = NULL;
					_kc++;
				}
			}
//			if( ( _kr + _kb + _kc + _kw ) != NUMBER_DVPARAM )
//				MessageBox( NULL, "number of DV", "error", MB_OK );
		}
 		_lpCobj_DV = LINKMATRIX_2_COBJ( _lpLinkMatrix );
		MFPUWRITE( _lppu, MDMCIRVD_DV, COBJ, _lpCobj_DV );
	}

	return;
}
