//===================================================================
//
//ttf  97-10-06
//===================================================================
//Read data from file, calculate the step-response one by one

#include "tfm_sr.h"
#include "tflm.h"

#define		VARSR_SR		0
#define		VARSR_PRIV		1
#define		VARSR_N			2
#define		VARSR_ORDER 	3
#define		VARSR_TD		4
#define		VARSR_TS		5
#define		VARSR_FILENO	6
#define		VARSR_RANK		7
#define		VARSR_NUM		0
#define		VARSR_DOM		1
#define		No_Const		8

#define		VARSR_RANK_PDT		PDT_WORD
#define		VARSR_FILENO_PDT	PDT_WORD
#define		VARSR_SR_PDT		PDT_COBJ
#define		VARSR_PRIV_PDT		PDT_COBJ
#define		VARSR_N_PDT			PDT_WORD
#define		VARSR_ORDER_PDT		PDT_WORD
#define		VARSR_TD_PDT		PDT_REAL
#define		VARSR_TS_PDT		PDT_REAL
#define		VARSR_NUM_PDT		PDT_REAL
#define		VARSR_DOM_PDT		PDT_REAL

DWORD _DLLFUNC MakeVarStepresponseParam(PIC wCmd_, WRD wMTU_, WORD wNo_, LONG lParam_)
{
	int			_i, _j, _in, _out, _Num_fix, _Dist, _Num_All;
	DWORD		_ret;
	WORD		_N, _Rank, _Order, _Num_var, _end, _Num, _No;
	LPPU		_lppu, _lppuvar, _lppunow;
	LPMATRIX    _lpSR, _lpAllNumDom;
	LPCOBJ		_lpCobj_SR, _lpCobj_gsr;
	FILE 		*fin;
	float		_temp, _Ts;
	char		_Buffer[64], _FileName[64]="C:\\Z4\\CORE\\input";

	static PDU	_sfixPDU[] = {
		_ELE_PDU( VARSR_SR,		"Stepres",		VSOF_COBJ,		PU_000, PU_PRI_RW),
		_ELE_PDU( VARSR_PRIV,	"Private",		VSOF_COBJ, 		PU_000, PU_PRI_RW),
		_ELE_PDU( VARSR_N,		"N",		    VSOF_WORD,		PU_000, PU_PRI_RW),
		_ELE_PDU( VARSR_ORDER, 	"Order",		VSOF_WORD,		PU_000, PU_PRI_RW),
		_ELE_PDU( VARSR_TD, 	"Td",			VSOF_REAL,		PU_000, PU_PRI_RW),
		_ELE_PDU( VARSR_TS, 	"Ts",			VSOF_REAL,		PU_000, PU_PRI_RW),
		_ELE_PDU( VARSR_FILENO,	"File_No",		VSOF_WORD,		PU_000, PU_PRI_RW),
		_ELE_PDU( VARSR_RANK,	"Rank",			VSOF_WORD,		PU_000, PU_PRI_RW),
	};

	static PDU	_svarPDU[] = {
		_ELE_PDU( VARSR_NUM,	"Num",			VSOF_REAL,		PU_000, PU_PRI_RW),
		_ELE_PDU( VARSR_DOM,	"Dom",			VSOF_REAL,		PU_000, PU_PRI_RW),
	};

	static VPSTRUCT	_sVPD = {
		1,			//the default number of groups of var parameters
		0,			//the low bound of var counter
		sizeof( _sfixPDU ) / sizeof( _sfixPDU[0] ),
		&( _sfixPDU[0] ),
		sizeof( _svarPDU ) / sizeof( _svarPDU[0] ),
		&( _svarPDU[0] )
	};

	switch( wCmd_ )
	{
		case PIC_INITIALIZEPARAM:
		{
			LPPU	_lppu;

			_ret = DefProcessVarPIC(wCmd_, wMTU_, wNo_, lParam_, TBX_MT, &_sVPD);

			if( _ret == FALSE ) return _ret;
			_lppu = MeterGetParameter( wMTU_ );
			_lppuvar = _lppu + sizeof( _sfixPDU ) / sizeof( _sfixPDU[0] );

			_Num_fix = sizeof( _sfixPDU ) / sizeof( _sfixPDU[0] );
			if( wNo_ < _Num_fix )
			{
				if( wNo_ == VARSR_N ) 		MFPUWRITE( _lppu, VARSR_N, WORD, 60 );
				if( wNo_ == VARSR_ORDER ) 	MFPUWRITE( _lppu, VARSR_ORDER, WORD, 1 );
				if( wNo_ == VARSR_TD )		MFPUWRITE( _lppu, VARSR_TD,   REAL,0 );
				if( wNo_ == VARSR_TS )		MFPUWRITE( _lppu, VARSR_TS,   REAL,1 );
				if( wNo_ == VARSR_RANK )	MFPUWRITE( _lppu, VARSR_RANK, WORD,3 );
			}
			else
			{
				_Order = ( wNo_ - _Num_fix ) % ( sizeof( _svarPDU ) / sizeof( _svarPDU[0] ) );
				_Num = ( wNo_ - _Num_fix ) / ( sizeof( _svarPDU ) / sizeof( _svarPDU[0] ) );
				_lppunow = _lppuvar + 2*_Num;
				if( ( _Order ) == VARSR_NUM )
				{
					MFPUWRITE( _lppunow, VARSR_NUM, REAL, 1.0 );
				}
				else
					MFPUWRITE( _lppunow, VARSR_DOM, REAL,	1.0 );
			}
			break;
		}
		case PIC_INITIATE:
		{
			WORD	_NoPara;

			_lppu = MeterGetParameter( wMTU_ );
			_NoPara = MeterGetNumParam( wMTU_ );
			_lppuvar = _lppu + sizeof( _sfixPDU ) / sizeof( _sfixPDU[0] );
			MFPUREAD( _lppu, VARSR_N, WORD, _N );
			_Num_var = sizeof( _svarPDU ) / sizeof( _svarPDU[0] );
			_Rank = ( _NoPara - sizeof( _sfixPDU ) / sizeof( _sfixPDU[0] ) )
						/ _Num_var - 1;
			MFPUWRITE( _lppu, VARSR_RANK, WORD, _Rank );
			MFPUREAD( _lppu, VARSR_ORDER, WORD, _Order );
			MFPUREAD( _lppu, VARSR_FILENO, WORD, _No );
			_lpSR = MatrixAlloc( _N, 1 );
			itoa( _No, _Buffer, 10 );
			strcat( _FileName, _Buffer );
			if ((fin=fopen( _FileName, "r"))==NULL)
			{
				MessageBox( NULL, "cannot open this file\n", "", MB_OK );
				return FALSE;
			}
			fscanf( fin, "%5d", &_in );
			fscanf( fin, "%5d", &_out );
			fscanf( fin, "%5d", &_Dist );
			fscanf( fin, "%5g", &_Ts );
			fscanf( fin, "\n" );

			_lpAllNumDom = MatrixAlloc( 2*(_in*_out), _Rank+2 );
			_lpCobj_SR = MAKE_COBJ( _lpSR );

			_Num_All = 2 * ( _in + _Dist ) * _out;
			for ( _i = 0; _i < _Num_All; _i++ )
			{
				if( (_i % 2) == 0 )
					_end = _Rank+1;
				else
					_end = _Rank+2;
				for( _j = 0; _j < _end; _j++ )
				{
					fscanf( fin, "%16g", &_temp );
					MGET( _lpAllNumDom, _i, _j ) = _temp;
				}
				fscanf( fin, "\n" );
			}
			fclose(fin);

			for( _i = 0; _i <= _Rank; _i++ )
			{
				MFPUWRITE( _lppuvar+_i*_Num_var, VARSR_NUM, REAL, MGET( _lpAllNumDom, 2*_Order-1, _Rank - _i ) );
				MFPUWRITE( _lppuvar+_i*_Num_var, VARSR_DOM, REAL, MGET( _lpAllNumDom, 2*(_Order-1), _Rank - _i ) );
			}
			MFPUWRITE( _lppu, VARSR_TS,   REAL, _Ts );
			MFPUWRITE( _lppu, VARSR_TD,   REAL, MGET( _lpAllNumDom, 2*_Order-1, _Rank+1 ) );
			MATRIXFREE( _lpAllNumDom );
			MFPUWRITE( _lppu, VARSR_SR, COBJ, _lpCobj_SR );
			_lpCobj_gsr = CobjAlloc(sizeof(COBJ) + sizeof(MSR));
			MFPUWRITE( _lppu, VARSR_PRIV, COBJ, _lpCobj_gsr );

			return TRUE;
		}

		case PIC_STOP:
		{
			_lppu = MeterGetParameter( wMTU_ );
			MFPUWRITE( _lppu, VARSR_SR, COBJ, NULL );
			MFPUWRITE( _lppu, VARSR_PRIV, COBJ, NULL );

			return	TRUE;
		}

		default:
			return DefProcessVarPIC(wCmd_, wMTU_, wNo_, lParam_, TBX_MT, &_sVPD);
	}
	return _ret;
}

//...................................................................
//Description:
//	Algorithm:	sum = n0 + n1 + n2 + ... + nn
//	ai transforms to TINT and 'sum' is TINT at default case.
//...................................................................
ACRET _DLLFUNC MakeVarStepresponseCalcu(WRD wMTU_)
{
	int		    _i;
	WORD		_N, _Rank;
	LPPU		_lppu, _lppuvar;
	LPMATRIX    _lpSR, _lpmNum, _lpmDom;
	LPCOBJ		_lpCobj_SR, _lpCobj_gsr;
	LPMATRIXL 	_lpsNum, _lpsDen, _lpmstepRes;
	double		_up, _Ts, _Td;
//   double		_testsr[NN];

	_lppu = MeterGetParameter( wMTU_ );
	ASSERT( MeterGetAlgo(wMTU_) == IDA_VARSTEPRESPONSE );
	MFPUREAD( _lppu, VARSR_SR, 	COBJ, _lpCobj_SR );
	MFPUREAD( _lppu, VARSR_N, 		WORD, _N );
	MFPUREAD( _lppu, VARSR_TS, 	REAL, _Ts );
	MFPUREAD( _lppu, VARSR_TD, 	REAL, _Td );
	MFPUREAD( _lppu, VARSR_RANK,WORD, _Rank );
	_lpmDom = MatrixAlloc( _Rank+1, 1 );
	_lpmNum = MatrixAlloc( _Rank+1, 1 );

	_lppu = MeterGetParameter( wMTU_ );
	_lppuvar = _lppu + No_Const;

	for( _i = 0; _i <= _Rank; _i++ )
	{
		MFPUREAD( _lppuvar+_i*2, VARSR_NUM, REAL, MGET( _lpmNum, _i, 0) );
		MFPUREAD( _lppuvar+_i*2, VARSR_DOM, REAL, MGET( _lpmDom, _i, 0) );
	}

	MFPUREAD( _lppu, VARSR_PRIV, COBJ, _lpCobj_gsr );
	MFPUREAD( _lppu, VARSR_TD,   REAL, _Td );
	MFPUREAD( _lppu, VARSR_TS,   REAL, _Ts );

	_lpSR = MAKE_MATRIX( _lpCobj_SR );
	_lpsNum = ( LPMATRIXL )malloc( sizeof(char)*mxMAXNAM+sizeof(int)*2+sizeof(double));
	_lpsDen = ( LPMATRIXL )malloc( sizeof(char)*mxMAXNAM+sizeof(int)*2+sizeof(double));
	_lpmstepRes = ( LPMATRIXL )malloc( sizeof(char)*mxMAXNAM+sizeof(int)*2+sizeof(double));

	_lpsNum->row = 1;
	_lpsNum->col = _Rank + 1;
	_lpsNum->pr = _lpmNum->data;
	_lpsDen->row = 1;
	_lpsDen->col = _Rank + 1;
	_lpsDen->pr = _lpmDom->data;
	_lpmstepRes->row = _N;
	_lpmstepRes->col = 1;
	_lpmstepRes->pr = &(_lpSR->data[0]);

	_up = 0.0;
	for( _i = 0; _i <= _Rank; _i++ )
		_up += fabs( MGET( _lpmNum, _i, 0 ) );

	if( fabs( MGET( _lpmDom, _Rank, 0 ) ) < epssr )
		MessageBox( NULL, "ERROR", "order illegal in VARSR", MB_OK );

	if( fabs( _up ) < epssr )
		for( _i = 0; _i < _N; _i++ )
			MGET( _lpSR, _i, 0 ) = 0.0;
	tf2step( _lpsNum, _lpsDen, _Td, _Ts, _N, _lpmstepRes );
	MFPUWRITE( _lppu, VARSR_SR, COBJ, _lpCobj_SR );
	MFPUWRITE( _lppu, VARSR_PRIV, COBJ, _lpCobj_gsr );
	MATRIXFREE( _lpmDom );
	MATRIXFREE( _lpmNum );
	return	TRUE;
}

