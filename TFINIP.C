#include "tfalgo.h"
#include "tfcmdmc.h"
#include "Mtmfun.h"
#include "Mtdllfun.h"
#ifdef debug
#include "tfdebug.h"
#endif
extern DWORD		_type_MV[NUMBER_MVPARAM];
extern DWORD		_type_CV[NUMBER_MVPARAM];
extern DWORD		_type_DV[NUMBER_MVPARAM];
//This array should be arranged again ,Ask ttf !!!
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
//Also arranged again
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

void PrivParamInitialize( LPCOBJPRIV _lpCobj_All_, WRD wNo_ )
{
	//LPPU 			_lppu, _lptempPU;
	LPLINKMATRIX	_lpLinkMatrix;
	LPCOBJ			_lpCobj_CV, _lpCobj_MV, _lpCobj_DV;
//	int				_i, _j, _kr, _kb, _kw, _kc;
	WORD		 	_mm, _pp, _ndnd, _No;
	FILE 			*fin;
	char			_Buffer[64], _FileName[64]="C:\\Z4\\CORE\\input";
//	LPMATRIX		_lpStepDV, _lpStepMV;
//	LPCOBJ			_lpCobj_StepDV, _lpCobj_StepMV;
//	LPCOBJ			_lpCobj_PrivDV, _lpCobj_PrivMV, _lpCobj_PrivCV;

//	_lppu = MeterGetParameter(wMTU_);
	if ( wNo_ == MDMCIRVD_FILENO )
		ONSPECWRITEALL( _lpCobj_All_, MDMCIRVD_FILENO, WORD, 0 );
	if ( wNo_ == MDMCIRVD_p )
	{
		ONSPECREADALL( _lpCobj_All_, MDMCIRVD_FILENO, WORD, _No );
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

		ONSPECWRITEALL( _lpCobj_All_, MDMCIRVD_p, WORD, _pp );
    }
	if ( wNo_ == MDMCIRVD_m )
	{
		ONSPECREADALL( _lpCobj_All_, MDMCIRVD_FILENO, WORD, _No );
		itoa( _No, _Buffer, 10 );
		strcat( _FileName, _Buffer );
		if ((fin=fopen( _FileName, "r"))==NULL)
		{
			MessageBox( NULL, "cannot open this file\n", "", MB_OK );
			return ;
		}
		fscanf( fin, "%5d", &_mm );
        fclose(fin);
		ONSPECWRITEALL( _lpCobj_All_, MDMCIRVD_m, WORD, _mm );
	}
	if ( wNo_ == MDMCIRVD_nd )
	{
		ONSPECREADALL( _lpCobj_All_, MDMCIRVD_FILENO, WORD, _No );
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
		ONSPECWRITEALL( _lpCobj_All_, MDMCIRVD_nd, WORD, _ndnd );
    }
#ifdef debug
	if ( wNo_ == MDMCIRVD_N )
		ONSPECWRITEALL( _lpCobj_All_, MDMCIRVD_N, WORD, NN );
	if ( wNo_ == MDMCIRVD_P )
		ONSPECWRITEALL( _lpCobj_All_, MDMCIRVD_P, WORD, PP );
	if ( wNo_ == MDMCIRVD_M )
		ONSPECWRITEALL( _lpCobj_All_, MDMCIRVD_M, WORD, MM );
#endif
	if ( wNo_ == MDMCIRVD_alpha )
		ONSPECWRITEALL( _lpCobj_All_, MDMCIRVD_alpha, REAL, 0.5 );
	if ( wNo_ == MDMCIRVD_count )
		ONSPECWRITEALL( _lpCobj_All_, MDMCIRVD_count, WORD, 0 );
	if ( wNo_ == MDMCIRVD_FLAGAM )
		ONSPECWRITEALL( _lpCobj_All_, MDMCIRVD_FLAGAM, BOOL, 0 );
	if ( wNo_ == MDMCIRVD_FLAGHOPEAM )
		ONSPECWRITEALL( _lpCobj_All_, MDMCIRVD_FLAGHOPEAM, BOOL, 0 );
	if ( wNo_ == MDMCIRVD_FLAGALARM )
		ONSPECWRITEALL( _lpCobj_All_, MDMCIRVD_FLAGALARM, BOOL, 0 );
	if ( wNo_ == MDMCIRVD_FLAGLP )
		ONSPECWRITEALL( _lpCobj_All_, MDMCIRVD_FLAGLP, BOOL, 1 );
	if ( wNo_ == MDMCIRVD_FLAGFAULT )
		ONSPECWRITEALL( _lpCobj_All_, MDMCIRVD_FLAGFAULT, BOOL, 0 );
	if ( wNo_ == MDMCIRVD_BIGCOND )
		ONSPECWRITEALL( _lpCobj_All_, MDMCIRVD_BIGCOND, REAL, 100 );
	if ( wNo_ == MDMCIRVD_MCLASSIF )
		ONSPECWRITEALL( _lpCobj_All_, MDMCIRVD_MCLASSIF, BOOL, 0 );
	if ( wNo_ == MDMCIRVD_CCLASSIF )
		ONSPECWRITEALL( _lpCobj_All_, MDMCIRVD_CCLASSIF, BOOL, 1 );
	if ( wNo_ == MDMCIRVD_PBROKEN )
		ONSPECWRITEALL( _lpCobj_All_, MDMCIRVD_PBROKEN, REAL, 50 );
	if ( wNo_ == MDMCIRVD_RBROKEN )
		ONSPECWRITEALL( _lpCobj_All_, MDMCIRVD_RBROKEN, REAL, 50 );
	if ( wNo_ == MDMCIRVD_QBROKEN )
		ONSPECWRITEALL( _lpCobj_All_, MDMCIRVD_QBROKEN, REAL, 50 );
	if ( wNo_ == MDMCIRVD_FLAGSTOP )
		ONSPECWRITEALL( _lpCobj_All_, MDMCIRVD_FLAGSTOP, BOOL, 0 );
	if ( wNo_ == MDMCIRVD_FLAGRESP )
		ONSPECWRITEALL( _lpCobj_All_, MDMCIRVD_FLAGRESP, BOOL, 0 );
	if ( wNo_ == MDMCIRVD_FLAGINTER )
		ONSPECWRITEALL( _lpCobj_All_, MDMCIRVD_FLAGINTER, BOOL, 0 );
	if ( wNo_ == MDMCIRVD_FLAGMCHANGE )
		ONSPECWRITEALL( _lpCobj_All_, MDMCIRVD_FLAGMCHANGE, BOOL, 0 );
	if ( wNo_ == MDMCIRVD_FLAGLPCHANGE )
		ONSPECWRITEALL( _lpCobj_All_, MDMCIRVD_FLAGLPCHANGE, BOOL, 0 );
	if( wNo_ == MDMCIRVD_CV )
	{
		ONSPECREADALL( _lpCobj_All_, MDMCIRVD_p, WORD, _pp );
//here maybe need a CVStructAlloc funtion ,ask ttf!!
		_lpCobj_CV = LINKMATRIX_2_COBJ( _lpLinkMatrix );
		ONSPECWRITEALL( _lpCobj_All_, MDMCIRVD_CV, LPCOBJ, _lpCobj_CV );
	}
	if( wNo_ == MDMCIRVD_MV )
	{
		ONSPECREADALL( _lpCobj_All_, MDMCIRVD_m, WORD, _mm );
//the MVStructAlloc function don't exist ,ask ttf!!!
		_lpLinkMatrix = MVStructAlloc( NUMBER_MVPARAM, _mm );
		_lpCobj_MV = LINKMATRIX_2_COBJ( _lpLinkMatrix );
		ONSPECWRITEALL( _lpCobj_All_, MDMCIRVD_MV, LPCOBJ, _lpCobj_MV );
	}
	if( wNo_ == MDMCIRVD_DV )
	{
		ONSPECREADALL( _lpCobj_All_, MDMCIRVD_nd, WORD, _ndnd );
		if( _ndnd == 0 )	return;
//the DVStructAlloc function don't exist ,ask ttf!!!

		_lpLinkMatrix = DVStructAlloc( NUMBER_DVPARAM, _ndnd );
 		_lpCobj_DV = LINKMATRIX_2_COBJ( _lpLinkMatrix );
		ONSPECWRITEALL( _lpCobj_All_, MDMCIRVD_DV, LPCOBJ, _lpCobj_DV );
	}


	return;
}
