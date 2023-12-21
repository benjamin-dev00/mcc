#include "tfalgo.h"
#include "tfcmdmc.h"
#ifdef debug
#include "tfdebug.h"
#endif
extern DWORD		_type_MV[NUMBER_MVPARAM];
extern DWORD		_type_CV[NUMBER_MVPARAM];
extern DWORD		_type_DV[NUMBER_MVPARAM];

double 	_MVReal[NUMBER_MVREAL] =
					{0, 1, -1, 0, 0,
					  0, 0.5, -0.5, 1, -1,
					  1, -1, 0.998, -0.998, 0.998, -0.998,
					  0.992, -0.992, 0.992, -0.992,
					  0, 4, 0, 1, 50, 0.01,
					  0.001, 50, 0.1, 0.0001, 0.6, 0.01, 1, -1
					};
WORD 	_MVWord[NUMBER_MVWORD] =
					{ 0, 0 };

BOOL 	_MVBool[NUMBER_MVBOOL] =
						{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

double	_CVReal[NUMBER_CVREAL] =
						{ 0, 1, -1, 0, 0,
						  1, -1, 1, -1,
						  0.998, -0.998, 0.998, -0.998,
						  0.992, -0.992, 0.992, -0.992,
						  0, 1, 0, 10, 5, 50,
						  1, 10, 1, -1
						};
//
WORD 	_CVWord[NUMBER_CVWORD] =
							{ 4, 0 };

// Control_mode, Real Control-mode, Priority
BOOL 	_CVBool[NUMBER_CVBOOL] =
							{ 0, 0, 1, 0, 0, 0, 0 };
// Q, Err, Valid, Alarm, Limit;
double	_DVReal[NUMBER_DVREAL] =
					{0, 1, -1, 0, 0, 0};
BOOL 	_DVBool[NUMBER_DVBOOL] =
					{1 };
WORD	_DVWord[NUMBER_DVWORD] = {0};
char	_DVCobj[NUMBER_DVCOBJ][20] =
					{"SORRY", "LCD IS A DISTURBANCE", "IT IS TRUE"};
char	_MVCobj[NUMBER_MVCOBJ][20] =
					{"SORRY", "LCD", "IT IS TRUE"};
char	_CVCobj[NUMBER_CVCOBJ][20] =
					{"SORRY", "LCD IS A DISTURBANCE", "IT IS TRUE"};

void PrivParamInitialize( WRD wMTU_, WRD wNo_ )
{
	LPPU 			_lppu, _lptempPU;
	LPLINKMATRIX	_lpLinkMatrix;
	LPCOBJ			_lpCobj_CV, _lpCobj_MV, _lpCobj_DV;
	int				_i, _j, _kr, _kb, _kw, _kc;
	WORD		 	_mm, _pp, _ndnd, _No;
	FILE 			*fin;
	char			_Buffer[64], _FileName[64]="C:\\Z4\\CORE\\input";
//	LPMATRIX		_lpStepDV, _lpStepMV;
//	LPCOBJ			_lpCobj_StepDV, _lpCobj_StepMV;
//	LPCOBJ			_lpCobj_PrivDV, _lpCobj_PrivMV, _lpCobj_PrivCV;

	_lppu = MeterGetParameter(wMTU_);
	if ( wNo_ == MDMCIRVD_FILENO )
		MFPUWRITE( _lppu, MDMCIRVD_FILENO, WORD, 0 );
	if ( wNo_ == MDMCIRVD_p )
	{
		MFPUREAD( _lppu, MDMCIRVD_FILENO, WORD, _No );
		itoa( _No, _Buffer, 10 );
		strcat( _FileName, _Buffer );
		if ((fin=fopen( _FileName, "r"))==NULL)
		{
			MessageBox( NULL, "cannot open this file\n", "", MB_OK );
			return ;
		}
		fscanf( fin, "%5d", &_mm );
		fscanf( fin, "%5d", &_pp );
		fclose(fin);

		MFPUWRITE( _lppu, MDMCIRVD_p, WORD, _pp );
    }
	if ( wNo_ == MDMCIRVD_m )
	{
		MFPUREAD( _lppu, MDMCIRVD_FILENO, WORD, _No );
		itoa( _No, _Buffer, 10 );
		strcat( _FileName, _Buffer );
		if ((fin=fopen( _FileName, "r"))==NULL)
		{
			MessageBox( NULL, "cannot open this file\n", "", MB_OK );
			return ;
		}
		fscanf( fin, "%5d", &_mm );
        fclose(fin);
		MFPUWRITE( _lppu, MDMCIRVD_m, WORD, _mm );
	}
	if ( wNo_ == MDMCIRVD_nd )
	{
		MFPUREAD( _lppu, MDMCIRVD_FILENO, WORD, _No );
		itoa( _No, _Buffer, 10 );
		strcat( _FileName, _Buffer );
		if ((fin=fopen( _FileName, "r"))==NULL)
		{
			MessageBox( NULL, "cannot open this file\n", "", MB_OK );
			return ;
		}
		fscanf( fin, "%5d", &_mm );
		fscanf( fin, "%5d", &_pp );
		fscanf( fin, "%5d", &_ndnd );
		fclose(fin);
		MFPUWRITE( _lppu, MDMCIRVD_nd, WORD, _ndnd );
    }
#ifdef debug
	if ( wNo_ == MDMCIRVD_N )
		MFPUWRITE( _lppu, MDMCIRVD_N, WORD, NN );
	if ( wNo_ == MDMCIRVD_P )
		MFPUWRITE( _lppu, MDMCIRVD_P, WORD, PP );
	if ( wNo_ == MDMCIRVD_M )
		MFPUWRITE( _lppu, MDMCIRVD_M, WORD, MM );
#endif
	if ( wNo_ == MDMCIRVD_alpha )
		MFPUWRITE( _lppu, MDMCIRVD_alpha, REAL, 0.5 );
	if ( wNo_ == MDMCIRVD_count )
		MFPUWRITE( _lppu, MDMCIRVD_count, WORD, 0 );
	if ( wNo_ == MDMCIRVD_FLAGAM )
		MFPUWRITE( _lppu, MDMCIRVD_FLAGAM, BOOL, 0 );
	if ( wNo_ == MDMCIRVD_FLAGHOPEAM )
		MFPUWRITE( _lppu, MDMCIRVD_FLAGHOPEAM, BOOL, 0 );
	if ( wNo_ == MDMCIRVD_FLAGALARM )
		MFPUWRITE( _lppu, MDMCIRVD_FLAGALARM, BOOL, 0 );
	if ( wNo_ == MDMCIRVD_FLAGLP )
		MFPUWRITE( _lppu, MDMCIRVD_FLAGLP, BOOL, 1 );
	if ( wNo_ == MDMCIRVD_FLAGFAULT )
		MFPUWRITE( _lppu, MDMCIRVD_FLAGFAULT, BOOL, 0 );
	if ( wNo_ == MDMCIRVD_BIGCOND )
		MFPUWRITE( _lppu, MDMCIRVD_BIGCOND, REAL, 100 );
	if ( wNo_ == MDMCIRVD_MCLASSIF )
		MFPUWRITE( _lppu, MDMCIRVD_MCLASSIF, BOOL, 0 );
	if ( wNo_ == MDMCIRVD_CCLASSIF )
		MFPUWRITE( _lppu, MDMCIRVD_CCLASSIF, BOOL, 1 );
	if ( wNo_ == MDMCIRVD_PBROKEN )
		MFPUWRITE( _lppu, MDMCIRVD_PBROKEN, REAL, 50 );
	if ( wNo_ == MDMCIRVD_RBROKEN )
		MFPUWRITE( _lppu, MDMCIRVD_RBROKEN, REAL, 50 );
	if ( wNo_ == MDMCIRVD_QBROKEN )
		MFPUWRITE( _lppu, MDMCIRVD_QBROKEN, REAL, 50 );
	if ( wNo_ == MDMCIRVD_FLAGSTOP )
		MFPUWRITE( _lppu, MDMCIRVD_FLAGSTOP, BOOL, 0 );
	if ( wNo_ == MDMCIRVD_FLAGRESP )
		MFPUWRITE( _lppu, MDMCIRVD_FLAGRESP, BOOL, 0 );
	if ( wNo_ == MDMCIRVD_FLAGINTER )
		MFPUWRITE( _lppu, MDMCIRVD_FLAGINTER, BOOL, 0 );
	if ( wNo_ == MDMCIRVD_FLAGMCHANGE )
		MFPUWRITE( _lppu, MDMCIRVD_FLAGMCHANGE, BOOL, 0 );
	if ( wNo_ == MDMCIRVD_FLAGLPCHANGE )
		MFPUWRITE( _lppu, MDMCIRVD_FLAGLPCHANGE, BOOL, 0 );
	if( wNo_ == MDMCIRVD_CV )
	{
		MFPUREAD( _lppu, MDMCIRVD_p, WORD, _pp );
		_lpLinkMatrix = CVStructAlloc( NUMBER_CVPARAM, _pp );
		for( _i = 0; _i < _pp; _i++ )
		{
			_kr = _kb = _kw = _kc = 0;
			for( _j = 0; _j < NUMBER_CVPARAM; _j++ )
			{
				_lptempPU = DLM_GET_LPPU( _lpLinkMatrix, _j, _i );
				if( _type_CV[_j] == VSOF_REAL )
				{
					_lptempPU->wPrefix = PF_IMM|IMM_REAL;
					_lptempPU->du.zzk__REAL = (float)_CVReal[_kr];
					_kr++;
				}

				if( _type_CV[_j] == VSOF_BOOL )
				{
					_lptempPU->wPrefix = PF_IMM|IMM_BOOL;
					_lptempPU->du.zzk__BOOL = _CVBool[_kb];
					_kb++;
				}
				if( _type_CV[_j] == VSOF_WORD )
				{
					_lptempPU->wPrefix = PF_IMM|IMM_WORD;
					_lptempPU->du.zzk__WORD = _CVWord[_kw];
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
		MFPUREAD( _lppu, MDMCIRVD_m, WORD, _mm );
		_lpLinkMatrix = MVStructAlloc( NUMBER_MVPARAM, _mm );
		for( _i = 0; _i < _mm; _i++ )
		{
			_kr = _kb = _kw = _kc = 0;
			for( _j = 0; _j < NUMBER_MVPARAM; _j++ )
			{
				_lptempPU = DLM_GET_LPPU( _lpLinkMatrix, _j, _i );
				if( _type_MV[_j] == VSOF_REAL )
				{
					_lptempPU->wPrefix = PF_IMM|IMM_REAL;
					_lptempPU->du.zzk__REAL = (float)_MVReal[_kr];
					_kr++;
				}

				if( _type_MV[_j] == VSOF_BOOL )
				{
					_lptempPU->wPrefix = PF_IMM|IMM_BOOL;
					_lptempPU->du.zzk__BOOL = _MVBool[_kb];
					_kb++;
				}
				if( _type_MV[_j] == VSOF_WORD )
				{
					_lptempPU->wPrefix = PF_IMM|IMM_WORD;
					_lptempPU->du.zzk__WORD = _MVWord[_kw];
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
		MFPUREAD( _lppu, MDMCIRVD_nd, WORD, _ndnd );
		if( _ndnd == 0 )	return;
		_lpLinkMatrix = DVStructAlloc( NUMBER_DVPARAM, _ndnd );
		for( _i = 0; _i < _ndnd; _i++ )
		{
			_kr = _kb = _kw = _kc = 0;
			for( _j = 0; _j < NUMBER_DVPARAM; _j++ )
			{
				_lptempPU = DLM_GET_LPPU( _lpLinkMatrix, _j, _i );
				if( _type_DV[_j] == VSOF_REAL )
				{
					_lptempPU->wPrefix = PF_IMM|IMM_REAL;
					_lptempPU->du.zzk__REAL = (float)_DVReal[_kr];
					_kr++;
				}

				if( _type_DV[_j] == VSOF_BOOL )
				{
					_lptempPU->wPrefix = PF_IMM|IMM_BOOL;
					_lptempPU->du.zzk__BOOL = _DVBool[_kb];
					_kb++;
				}
				if( _type_DV[_j] == VSOF_WORD )
				{
					_lptempPU->wPrefix = PF_IMM|IMM_WORD;
					_lptempPU->du.zzk__WORD = _DVWord[_kw];
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
