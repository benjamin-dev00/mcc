#include "tfalgo.h"
#include "tfcmdmc.h"
#ifdef __ONSPEC
#include "Mtmfun.h"
#include "Mtdllfun.h"
#endif


ACRET	DVStructWrite( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVDV lpCobj_PrivDV_, WORD	j_ )
{
	WRITEDVPARAMETER( lpCobj_All_->LPDV, j_, DV_VALUE, 	REAL,     (REAL)PRIVDVGET( lpCobj_PrivDV_, j_, 0 )->DData );				//采样值
	WRITEDVPARAMETER( lpCobj_All_->LPDV, j_, DV_VALUE, 	REAL,     (REAL)PRIVDVGET( lpCobj_PrivDV_, j_, 0 )->DData );				//采样值
	WRITEDVPARAMETER( lpCobj_All_->LPDV, j_, DV_HSCALE, 	REAL,  (REAL)PRIVDVGET( lpCobj_PrivDV_, j_, 0 )->DHScale );			//规一化上限
	WRITEDVPARAMETER( lpCobj_All_->LPDV, j_, DV_LSCALE, 	REAL,  (REAL)PRIVDVGET( lpCobj_PrivDV_, j_, 0 )->DLScale );			//规一化下限
	WRITEDVPARAMETER( lpCobj_All_->LPDV, j_, DV_INI, 	REAL,  (REAL)PRIVDVGET( lpCobj_PrivDV_, j_, 0 )->DIni );				//初始值
	WRITEDVPARAMETER( lpCobj_All_->LPDV, j_, DV_DELT, 	REAL,  (REAL)PRIVDVGET( lpCobj_PrivDV_, j_, 0 )->DDelt );				//增量
	WRITEDVPARAMETER( lpCobj_All_->LPDV, j_, DV_HISTORY, 	REAL,  (REAL)PRIVDVGET( lpCobj_PrivDV_, j_, 0 )->DHistory );			//历史采样值
	WRITEDVPARAMETER( lpCobj_All_->LPDV, j_, DV_DISPLAYH, 	REAL,  (REAL)PRIVDVGET( lpCobj_PrivDV_, j_, 0 )->DDisplayH );			//显示上限
	WRITEDVPARAMETER( lpCobj_All_->LPDV, j_, DV_DISPLAYL, 	REAL,  (REAL)PRIVDVGET( lpCobj_PrivDV_, j_, 0 )->DDisplayL );			//显示下限
	WRITEDVPARAMETER( lpCobj_All_->LPDV, j_, DV_DISPLAY, 	REAL,  (REAL)PRIVDVGET( lpCobj_PrivDV_, j_, 0 )->DDisplay );			//显示
	WRITEDVPARAMETER( lpCobj_All_->LPDV, j_, DV_STEADYFIELD, 	REAL,  (REAL)PRIVDVGET( lpCobj_PrivDV_, j_, 0 )->DSteadyField );		//稳态最小区域
	WRITEDVPARAMETER( lpCobj_All_->LPDV, j_, DV_STEADYFIELD, 	REAL,  (REAL)PRIVDVGET( lpCobj_PrivDV_, j_, 0 )->DSteadyField );		//稳态最小区域
	WRITEDVPARAMETER( lpCobj_All_->LPDV, j_, DV_STATE, 		BOOL, PRIVDVGET( lpCobj_PrivDV_, j_, 0 )->DInvalid );		//无效
#ifdef __ZZK
	WRITEDVPARAMETER( lpCobj_All_->LPDV, j_, DV_ALARM, 		BOOL, PRIVDVGET( lpCobj_PrivDV_, j_, 0 )->DAlarm );		//报警
	WRITEDVPARAMETER( lpCobj_All_->LPDV, j_, DV_BAD, 		WORD, PRIVDVGET( lpCobj_PrivDV_, j_, 0 )->DBad );		//故障次数
#endif
	return	ACRET_TRUE;
}