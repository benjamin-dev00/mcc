#include "tfalgo.h"
#include "tfcmdmc.h"
#ifdef __ONSPEC
#include "Mtmfun.h"
#include "Mtdllfun.h"
#endif

void WriteSubGParameter( LPCOBJPRIV lpCobj_All_, int Apart_ )
{
//	int		_i;
	LPCOBJAPART	_lpApart;
	
//  write the global parameters of sub_modu to big_modu's
	_lpApart = COBJ_2_APART( lpCobj_All_->LPApart );
	if( Apart_ == 0 )
	{
		lpCobj_All_->MakeParaCount = 0;
		lpCobj_All_->RecountCount = 0;
		lpCobj_All_->MParaCount = 0;
	}
	lpCobj_All_->MakeParaCount += APARTGET( _lpApart, Apart_, 0 )->MakeParaCount;
	lpCobj_All_->RecountCount += APARTGET( _lpApart, Apart_, 0 )->RecountCount;
	lpCobj_All_->MParaCount += APARTGET( _lpApart, Apart_, 0 )->MParaCount;
 	if( Apart_ == 0 )
		lpCobj_All_->Freecount = APARTGET( _lpApart, Apart_, 0 )->Freecount;
	else
		lpCobj_All_->Freecount += APARTGET( _lpApart, Apart_, 0 )->Freecount;
	if( APARTGET( _lpApart, Apart_, 0 )->Flag_Run == 0 )
		lpCobj_All_->Flag_Run = 0;
	if( APARTGET( _lpApart, Apart_, 0 )->Flag_Alarm == 1 )
		lpCobj_All_->Flag_Alarm = 1;
	if( APARTGET( _lpApart, Apart_, 0 )->Flag_HopeAM == 1 )
		lpCobj_All_->Flag_HopeAM = 1;
//	if( APARTGET( _lpApart, Apart_, 0 )->Flag_LPRecount == 1 )
//		lpCobj_All_->Flag_LPRecount = 1;
	if( APARTGET( _lpApart, Apart_, 0 )->Flag_Recount == 1 )
		lpCobj_All_->Flag_Recount = 1;
	if( Apart_ == 0 && !lpCobj_All_->ErrorCode )
		lpCobj_All_->ErrorCode = 0;
	lpCobj_All_->ErrorCode += APARTGET( _lpApart, Apart_, 0 )->ErrorCode;
//  broadcast the new parameters to all sub_modus
/*	for( _i = 0; _i < lpCobj_All_->No_Apart; _i++ )
	{
		APARTGET( _lpApart, Apart_, 0 )->MakeParaCount = 
			lpCobj_All_->MakeParaCount;
		APARTGET( _lpApart, Apart_, 0 )->RecountCount = 
			lpCobj_All_->RecountCount;
//		APARTGET( _lpApart, Apart_, 0 )->MParaCount = 
//			lpCobj_All_->MParaCount;

	}
*/
	return ;
}