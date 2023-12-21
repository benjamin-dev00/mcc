#include "tfalgo.h"
#include "tfcmdmc.h"
#ifdef __ONSPEC
#include "Mtmfun.h"
#include "Mtdllfun.h"
#endif


ACRET	DVStructWrite( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVDV lpCobj_PrivDV_, WORD	j_ )
{
	WRITEDVPARAMETER( lpCobj_All_->LPDV, j_, DV_VALUE, 	REAL,     (REAL)PRIVDVGET( lpCobj_PrivDV_, j_, 0 )->DData );				//����ֵ
	WRITEDVPARAMETER( lpCobj_All_->LPDV, j_, DV_VALUE, 	REAL,     (REAL)PRIVDVGET( lpCobj_PrivDV_, j_, 0 )->DData );				//����ֵ
	WRITEDVPARAMETER( lpCobj_All_->LPDV, j_, DV_HSCALE, 	REAL,  (REAL)PRIVDVGET( lpCobj_PrivDV_, j_, 0 )->DHScale );			//��һ������
	WRITEDVPARAMETER( lpCobj_All_->LPDV, j_, DV_LSCALE, 	REAL,  (REAL)PRIVDVGET( lpCobj_PrivDV_, j_, 0 )->DLScale );			//��һ������
	WRITEDVPARAMETER( lpCobj_All_->LPDV, j_, DV_INI, 	REAL,  (REAL)PRIVDVGET( lpCobj_PrivDV_, j_, 0 )->DIni );				//��ʼֵ
	WRITEDVPARAMETER( lpCobj_All_->LPDV, j_, DV_DELT, 	REAL,  (REAL)PRIVDVGET( lpCobj_PrivDV_, j_, 0 )->DDelt );				//����
	WRITEDVPARAMETER( lpCobj_All_->LPDV, j_, DV_HISTORY, 	REAL,  (REAL)PRIVDVGET( lpCobj_PrivDV_, j_, 0 )->DHistory );			//��ʷ����ֵ
	WRITEDVPARAMETER( lpCobj_All_->LPDV, j_, DV_DISPLAYH, 	REAL,  (REAL)PRIVDVGET( lpCobj_PrivDV_, j_, 0 )->DDisplayH );			//��ʾ����
	WRITEDVPARAMETER( lpCobj_All_->LPDV, j_, DV_DISPLAYL, 	REAL,  (REAL)PRIVDVGET( lpCobj_PrivDV_, j_, 0 )->DDisplayL );			//��ʾ����
	WRITEDVPARAMETER( lpCobj_All_->LPDV, j_, DV_DISPLAY, 	REAL,  (REAL)PRIVDVGET( lpCobj_PrivDV_, j_, 0 )->DDisplay );			//��ʾ
	WRITEDVPARAMETER( lpCobj_All_->LPDV, j_, DV_STEADYFIELD, 	REAL,  (REAL)PRIVDVGET( lpCobj_PrivDV_, j_, 0 )->DSteadyField );		//��̬��С����
	WRITEDVPARAMETER( lpCobj_All_->LPDV, j_, DV_STEADYFIELD, 	REAL,  (REAL)PRIVDVGET( lpCobj_PrivDV_, j_, 0 )->DSteadyField );		//��̬��С����
	WRITEDVPARAMETER( lpCobj_All_->LPDV, j_, DV_STATE, 		BOOL, PRIVDVGET( lpCobj_PrivDV_, j_, 0 )->DInvalid );		//��Ч
#ifdef __ZZK
	WRITEDVPARAMETER( lpCobj_All_->LPDV, j_, DV_ALARM, 		BOOL, PRIVDVGET( lpCobj_PrivDV_, j_, 0 )->DAlarm );		//����
	WRITEDVPARAMETER( lpCobj_All_->LPDV, j_, DV_BAD, 		WORD, PRIVDVGET( lpCobj_PrivDV_, j_, 0 )->DBad );		//���ϴ���
#endif
	return	ACRET_TRUE;
}