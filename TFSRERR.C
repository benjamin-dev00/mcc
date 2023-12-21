#include "tfalgo.h"
#include "tfcmdmc.h"
//===================================================================
//
//ttf  97-10-27
//===================================================================
////////////  STEPERROOR  //////////////////////////////////////////////

#define STEPERROR_RESULT			0
#define STEPERROR_SOURCE			1
#define STEPERROR_INDEX				2
#define STEPERROR_RESULT_PDT		PDT_COBJ
#define STEPERROR_SOURCE_PDT		PDT_COBJ
#define STEPERROR_INDEX_PDT			PDT_REAL
//...................................................................
//...................................................................
DWORD _DLLFUNC StepErrorParam(PIC wCmd_, WRD wMTU_, WORD wNo_, LONG lParam_)
{
	LPPU		_lppu;
	DWORD 		_ret;

	static PDU _sPDU[] =
	{
		_ELE_PDU(STEPERROR_RESULT,	"Result",	VSOF_COBJ, PU_000, PU_PRI_RW),
		_ELE_PDU(STEPERROR_SOURCE,	"Source",	VSOF_COBJ, PU_000, PU_PRI_RW),
		_ELE_PDU(STEPERROR_INDEX,	"Index",	VSOF_REAL, PU_000, PU_PRI_RW)
	};

	_ret = _DefAlgoParam( wCmd_,
						  wMTU_,
						  wNo_,
						  lParam_,
						  sizeof(_sPDU)/sizeof(PDU),
						  &(_sPDU[0]) );
	switch( wCmd_ )
	{
		case PIC_INITIALIZEPARAM:
		{
			_lppu = MeterGetParameter( wMTU_ );
			if( wNo_ == STEPERROR_INDEX )
				MFPUWRITE( _lppu, STEPERROR_INDEX, REAL, 0.025 );
			return TRUE;
		}
		case PIC_INITIATE:
		{
			LPCOBJ		_lpCobj_Source, _lpCobj_Result;
			LPDIMMATRIX	_lpmSource, _lpmResult;
            int			_row, _col, _axe;

			_lppu = MeterGetParameter( wMTU_ );
			MFPUREAD( _lppu, STEPERROR_SOURCE, COBJ, _lpCobj_Source );
			if( _lpCobj_Source == NULL )
			{
				MessageBox( NULL, "ERROR", "NO STEPERROR INPUT", MB_OK );
				return	FALSE;
			}
			_lpmSource = MAKE_DIMMATRIX( _lpCobj_Source );
			_row = _lpmSource->row;
			_col = _lpmSource->col;
			_axe = _lpmSource->axe;
			_lpmResult = DimAlloc( _row, _col, _axe );
			_lpCobj_Result = MAKE_COBJ( _lpmResult );
			MFPUWRITE( _lppu, STEPERROR_RESULT, COBJ, _lpCobj_Result );
            return TRUE;
		}
		case PIC_STOP:
		{
			_lppu = MeterGetParameter( wMTU_ );
			MFPUWRITE( _lppu, STEPERROR_RESULT, COBJ, NULL );
            return TRUE;
		}
	}
	return _ret;

}

//...................................................................
//Description:
//	Algorithm:	 Result = Source .* Index 
//...................................................................
ACRET _DLLFUNC STEPERRORCalcu(WRD wMTU_)
{
	double		_Index, _Shi;
	LPCOBJ		_lpCobj_Source, _lpCobj_Result;
	LPDIMMATRIX	_lpmSource, _lpmResult;
	int			_row, _col, _axe, _i, _j, _k;
	LPPU 		_lppu;

	ASSERT( MeterGetAlgo(wMTU_) == IDA_STEPERROR );
	ASSERT( MeterGetNumParam(wMTU_) == 3 );

	_lppu = MeterGetParameter( wMTU_ );
	MFPUREAD( _lppu, STEPERROR_SOURCE, COBJ, _lpCobj_Source );
	MFPUREAD( _lppu, STEPERROR_RESULT, COBJ, _lpCobj_Result );
	if( _lpCobj_Source == NULL )
    	return FALSE;
	_lpmResult = MAKE_DIMMATRIX( _lpCobj_Result );
    _lpmSource = MAKE_DIMMATRIX( _lpCobj_Source );
	MFPUREAD( _lppu, STEPERROR_INDEX, REAL, _Index );
	_Shi = 1 - _Index;
	_row = _lpmSource->row;
	_col = _lpmSource->col;
	_axe = _lpmSource->axe;
	for( _i = 0; _i < _row; _i++ )
		for( _j = 0; _j < _col; _j++ )
			for( _k = 0; _k < _axe; _k++ )
				DIMMGET( _lpmResult, _i, _j, _k ) =
					DIMMGET( _lpmSource, _i, _j, _k ) * _Shi;

	return  ACRET_TRUE;
}
