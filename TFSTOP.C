#include "tfalgo.h"
#include "tfcmdmc.h"
#ifdef __ONSPEC
#include "mtmfun.h"
#include "mtdllfun.h"
#endif

void	PrivStop( LPCOBJPRIV lpCobjAll_, LPCOBJPRIVMV lpGlobalMV_, 
					LPCOBJPRIVCV lpGlobalCV_, LPCOBJPRIVDV lpCobjPrivDV_ )
{
	LPCOBJAPART		_lpApart;
	LPCOBJPRIV		_lpCobjApart;
	LPCOBJPRIVMV	_lpCobjPrivMV;
	LPCOBJPRIVCV	_lpCobjPrivCV;
	LPCOBJPRIVDV	_lpCobjPrivDV;
	int				_i, _j;

	//_lppu = MeterGetParameter(wMTU_);
	Write_FileAll( lpCobjAll_ );
//add a file to ununit all the data to output to file,no need here.
//	UnDeltFile( _lpCobjPrivAll, _lpCobjPrivMV, _lpCobjPrivCV, _lpCobjPrivDV );
//	UnUnitFile( _lpCobjPrivAll, _lpCobjPrivMV, _lpCobjPrivCV, _lpCobjPrivDV );
	Write_FileMV( lpCobjAll_, lpGlobalMV_ );
	Write_FileCV( lpCobjAll_, lpGlobalCV_ );
	Write_FileDV( lpCobjAll_, lpCobjPrivDV_ );
	for( _i = 0; _i < lpCobjAll_->m; _i++ )
	{
		WRITEMVPARAMETER( lpCobjAll_->LPMV, (WORD)_i, MV_PRIV, LPCOBJ, NULL );
		PRIVMVGET( lpGlobalMV_, _i, 0 ) = NULL;
	}
	for( _i = 0; _i < lpCobjAll_->p; _i++ )
	{
		WRITECVPARAMETER( lpCobjAll_->LPCV, (WORD)_i, CV_PRIV, LPCOBJ, NULL );
		PRIVCVGET( lpGlobalCV_, _i, 0 ) = NULL;
	}
	for( _i = 0; _i < lpCobjAll_->nd; _i++ )
	{
		WRITEDVPARAMETER( lpCobjAll_->LPDV, (WORD)_i, DV_PRIV, LPCOBJ, NULL );
		PRIVDVGET( lpCobjPrivDV_, _i, 0 ) = NULL;
	}
	_lpApart = COBJ_2_APART( lpCobjAll_->LPApart );
//  free memory allocated by tfaptsr.c now
	for( _i = 0; _i < lpCobjAll_->No_Apart; _i++ )
	{
		_lpCobjApart = APARTGET( _lpApart, _i, 0 );
		_lpCobjPrivMV = COBJ_2_PRIVMV( _lpCobjApart->LPINMV );
		_lpCobjPrivCV = COBJ_2_PRIVCV( _lpCobjApart->LPINCV );
		_lpCobjPrivDV = COBJ_2_PRIVDV( _lpCobjApart->LPINDV );
		for( _j = 0; _j < _lpCobjApart->m; _j++ )
			PRIVMVGET( _lpCobjPrivMV, _j, 0 ) = NULL;
		for( _j = 0; _j < _lpCobjApart->p; _j++ )
			PRIVCVGET( _lpCobjPrivCV, _j, 0 ) = NULL;
		for( _j = 0; _j < _lpCobjApart->nd; _j++ )
			PRIVDVGET( _lpCobjPrivDV, _j, 0 ) = NULL;
		PRIVMVFREE( _lpCobjPrivMV );
		PRIVCVFREE( _lpCobjPrivCV );
		PRIVDVFREE( _lpCobjPrivDV );
		CobjFree( _lpCobjApart->LPSR );
		CobjFree( _lpCobjApart->LPDSR );
		CobjFree( _lpCobjApart->LPDeluM );
		CobjFree( _lpCobjApart->LPMVSort );
		CobjFree( _lpCobjApart->LPCVSort );
		CobjFree( _lpCobjApart->LPQ );
		CobjFree( _lpCobjApart->LPR );
		CobjFree( _lpCobjApart->LPS );
		CobjFree( _lpCobjApart->LPH );
		CobjFree( _lpCobjApart->LPMul2 );
		CobjFree( _lpCobjApart->LPMul3 );
		CobjFree( _lpCobjApart->LPAb );
		CobjFree( _lpCobjApart->LPPredN );
		CobjFree( _lpCobjApart->LPA );
		CobjFree( _lpCobjApart->LPHistory );
		CobjFree( _lpCobjApart->LPG );
		CobjFree( _lpCobjApart->LPTempSR );
		CobjFree( _lpCobjApart->LPTempDSR );
		CobjFree( _lpCobjApart->LPIsSteady );
		CobjFree( _lpCobjApart->LPSteady );
	}
#ifdef __ONSPEC
	CobjFree( lpCobjAll_->LPSR );
	CobjFree( lpCobjAll_->LPDSR );
	CobjFree( lpCobjAll_->LPOutSR );
	CobjFree( lpCobjAll_->LPOutDSR );
	CobjFree( lpCobjAll_->LPG );
	CobjFree( lpCobjAll_->LPLPPriv );
	CobjFree( lpCobjAll_->LPMVSort );
	CobjFree( lpCobjAll_->LPCVSort );
	CobjFree( lpCobjAll_->LPApartMV );
	CobjFree( lpCobjAll_->LPApartCV );
	if( lpCobjAll_->LPApartDV )
		CobjFree( lpCobjAll_->LPApartDV );
	CobjFree( lpCobjAll_->LPHistory );
	PRIVMVFREE( lpGlobalMV_ );
	PRIVCVFREE( lpGlobalCV_ );
	PRIVDVFREE( lpCobjPrivDV_ );
	PRIVAPARTFREE( _lpApart );
#endif
	WRITEALLPARAMETER( lpCobjAll_, MDMCIRVD_mSR, LPCOBJ, NULL  );
	WRITEALLPARAMETER( lpCobjAll_, MDMCIRVD_mDSR, LPCOBJ, NULL );
	WRITEALLPARAMETER( lpCobjAll_, MDMCIRVD_OUTSR, LPCOBJ, NULL );
	WRITEALLPARAMETER( lpCobjAll_, MDMCIRVD_OUTDSR, LPCOBJ, NULL );
	WRITEALLPARAMETER( lpCobjAll_, MDMCIRVD_mG, LPCOBJ, NULL );
	WRITEALLPARAMETER( lpCobjAll_, MDMCIRVD_LPPRIV, LPCOBJ, NULL );
	WRITEALLPARAMETER( lpCobjAll_, MDMCIRVD_MVSort, LPCOBJ, NULL );
	WRITEALLPARAMETER( lpCobjAll_, MDMCIRVD_CVSort, LPCOBJ, NULL );
	WRITEALLPARAMETER( lpCobjAll_, MDMCIRVD_ApartMV, LPCOBJ, NULL );
	WRITEALLPARAMETER( lpCobjAll_, MDMCIRVD_ApartCV, LPCOBJ, NULL );
	WRITEALLPARAMETER( lpCobjAll_, MDMCIRVD_ApartDV, LPCOBJ, NULL );
	WRITEALLPARAMETER( lpCobjAll_, MDMCIRVD_HISTORY, LPCOBJ, NULL );
	WRITEALLPARAMETER( lpCobjAll_, MDMCIRVD_INMV, LPCOBJ, NULL );
	WRITEALLPARAMETER( lpCobjAll_, MDMCIRVD_INCV, LPCOBJ, NULL );
	WRITEALLPARAMETER( lpCobjAll_, MDMCIRVD_INDV, LPCOBJ, NULL );
#ifdef __ONSPEC
	WRITEALLPARAMETER( lpCobjAll_, MDMCIRVD_MV, LPCOBJ, NULL );
	WRITEALLPARAMETER( lpCobjAll_, MDMCIRVD_CV, LPCOBJ, NULL );
	WRITEALLPARAMETER( lpCobjAll_, MDMCIRVD_DV, LPCOBJ, NULL );
#endif
	WRITEALLPARAMETER( lpCobjAll_, MDMCIRVD_Apart, LPCOBJ, NULL );
	return;
}
