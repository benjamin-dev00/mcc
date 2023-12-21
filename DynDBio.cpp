#include "DynDBio.h"
//////////////////////////////////////////////////
// Functions
//////////////////////////////////////////////////
BOOL DLLEXPORT DynInitialize()
{
	GetLocalNodeName();
	GetTagIndex();
	if(!HasGotLocalNodeName || !HasGotTagIndex)return FALSE;

	for(int i=0 ; i<MAX_CV_NUM;i++)
		for(int j=0;j<MAX_CV_PAR_NUM;j++)
		{
			CVTagName[i][j]=(char*)malloc(MAX_NAME_SIZE);
				sprintf(CVTagName[i][j],"CV_%d_%d",i,j);
		}
	for(i=0 ; i<MAX_MV_NUM;i++)
		for(int j=0;j<MAX_MV_PAR_NUM;j++)
		{
			MVTagName[i][j]=(char*)malloc(MAX_NAME_SIZE);
				sprintf(MVTagName[i][j],"MV_%d_%d",i,j);
		}
	for(i=0 ; i<MAX_DV_NUM;i++)
		for(int j=0;j<MAX_DV_PAR_NUM;j++)
		{
			DVTagName[i][j]=(char*)malloc(MAX_NAME_SIZE);
				sprintf(DVTagName[i][j],"DV_%d_%d",i,j);
		}
	for(i=0 ; i<MAX_GV_NUM;i++)
		for(int j=0;j<MAX_GV_PAR_NUM;j++)
		{
			GVTagName[i][j]=(char*)malloc(MAX_NAME_SIZE);
			sprintf(GVTagName[i][j],"GV_%d_%d",i,j);
		}
	HasInitialized = TRUE;
	return TRUE;
}

BOOL DLLEXPORT DynUninitialize()
{
	for(int i=0 ; i<MAX_CV_NUM;i++)
		for(int j=0;j<MAX_CV_PAR_NUM;j++)
			free(CVTagName[i][j]);
	for(i=0 ; i<MAX_MV_NUM;i++)
		for(int j=0;j<MAX_MV_PAR_NUM;j++)
			free(MVTagName[i][j]);
	for(i=0 ; i<MAX_DV_NUM;i++)
		for(int j=0;j<MAX_DV_PAR_NUM;j++)
			free(DVTagName[i][j]);
	for(i=0 ; i<MAX_GV_NUM;i++)
		for(int j=0;j<MAX_GV_PAR_NUM;j++)
			free(GVTagName[i][j]);
	HasInitialized = FALSE;
	return TRUE;
}

void DLLEXPORT GetLocalNodeName()
{
	FixGetMyname(LocalNodeName,MAX_NAME_SIZE);
	HasGotLocalNodeName = TRUE;
}

void DLLEXPORT GetTagIndex()
{
	if(!HasGotLocalNodeName)return ;
	CV_IN_TAG_INDEX  = eda_type_to_index(LocalNodeName, CV_IN_TAG_TYPE);
	MV_IN_TAG_INDEX  = eda_type_to_index(LocalNodeName, MV_IN_TAG_TYPE);
	DV_IN_TAG_INDEX  = eda_type_to_index(LocalNodeName, DV_IN_TAG_TYPE);
	MV_OUT_TAG_INDEX = eda_type_to_index(LocalNodeName, MV_OUT_TAG_TYPE);
	GV_TAG_INDEX	 = eda_type_to_index(LocalNodeName, GV_TAG_TYPE);
	COMMON_TAG_INDEX = eda_type_to_index(LocalNodeName, COMMON_TAG_TYPE);
	HasGotTagIndex = TRUE;
}

BOOL DLLEXPORT Create_Blocks()
{
	if(!Create_CV_Blocks())return FALSE;
	if(!Create_MV_Blocks())return FALSE;
	if(!Create_DV_Blocks())return FALSE;
	if(!Create_GV_Blocks())return FALSE;
	return TRUE;
}

BOOL DLLEXPORT Create_CV_Blocks()
{
	if(!HasInitialized)return FALSE;

	for(int i=0;i<MAX_CV_NUM;i++)
		for(int j=0;j<MAX_CV_PAR_NUM;j++)
		{
			switch(j)
			{
			case CV_IN_BLOCK:
				eda_add_block(LocalNodeName, CVTagName[i][j], CV_IN_TAG_INDEX);
				break;
			default:
				eda_add_block(LocalNodeName, CVTagName[i][j], COMMON_TAG_INDEX);
				break;
			}
		}
	return TRUE;
}
//CStatic
BOOL DLLEXPORT Create_MV_Blocks()
{
	if(!HasInitialized)return FALSE;
	for(int i=0;i<MAX_MV_NUM;i++)
		for(int j=0;j<MAX_MV_PAR_NUM;j++)
		{
			switch(j)
			{
			case MV_IN_BLOCK:
				eda_add_block(LocalNodeName, MVTagName[i][j], MV_IN_TAG_INDEX);
				break;
			case MV_OUT_BLOCK:
				eda_add_block(LocalNodeName, MVTagName[i][j], MV_OUT_TAG_INDEX);
				break;
			default:
				eda_add_block(LocalNodeName, MVTagName[i][j], COMMON_TAG_INDEX);
			}
		}
	return TRUE;
}

BOOL DLLEXPORT Create_DV_Blocks()
{
	if(!HasInitialized)return FALSE;
	for(int i=0;i<MAX_DV_NUM;i++)
		for(int j=0;j<MAX_DV_PAR_NUM;j++)
		{
			switch(j)
			{
			case DV_IN_BLOCK:
				eda_add_block(LocalNodeName, DVTagName[i][j], DV_IN_TAG_INDEX);
				break;
			default:
				eda_add_block(LocalNodeName, DVTagName[i][j], COMMON_TAG_INDEX);
			}
		}
	return TRUE;
}

BOOL DLLEXPORT Create_GV_Blocks()
{
	if(!HasInitialized)return FALSE;
	for(int i=0;i<MAX_GV_NUM;i++)
		for(int j=0;j<MAX_GV_PAR_NUM;j++)
		{
			eda_add_block(LocalNodeName, GVTagName[i][j], COMMON_TAG_INDEX);
		}
	return TRUE;
}

BOOL DLLEXPORT Delete_Blocks()
{
	if(!Delete_CV_Blocks())return FALSE;
	if(!Delete_MV_Blocks())return FALSE;
	if(!Delete_DV_Blocks())return FALSE;
	if(!Delete_GV_Blocks())return FALSE;
	return TRUE;
}

BOOL DLLEXPORT Delete_CV_Blocks()
{
	if(!HasInitialized)return FALSE;
	for(int i=0;i<MAX_CV_NUM;i++)
		for(int j=0;j<MAX_CV_PAR_NUM;j++)
		{
			eda_delete_block(LocalNodeName, CVTagName[i][j], 1);
		}
	return TRUE;
}

BOOL DLLEXPORT Delete_MV_Blocks()
{
	if(!HasInitialized)return FALSE;
	for(int i=0;i<MAX_MV_NUM;i++)
		for(int j=0;j<MAX_MV_PAR_NUM;j++)
		{
			eda_delete_block(LocalNodeName, MVTagName[i][j], 1);
		}
	return TRUE;
}

BOOL DLLEXPORT Delete_DV_Blocks()
{
	if(!HasInitialized)return FALSE;
	for(int i=0;i<MAX_DV_NUM;i++)
		for(int j=0;j<MAX_DV_PAR_NUM;j++)
		{
			eda_delete_block(LocalNodeName, DVTagName[i][j], 1);
		}
	return TRUE;
}

BOOL DLLEXPORT Delete_GV_Blocks()
{
	if(!HasInitialized)return FALSE;
	for(int i=0;i<MAX_GV_NUM;i++)
	{
		for(int j=0;j<MAX_GV_PAR_NUM;j++)
		{
			eda_delete_block(LocalNodeName, GVTagName[i][j], 1);
		}
	}
	return TRUE;
}

BOOL DLLEXPORT GetGroup()
{
	if(!GetCVGroup())return FALSE;
	if(!GetMVGroup())return FALSE;
	if(!GetDVGroup())return FALSE;
	if(!GetGVGroup())return FALSE;
	return TRUE;
}

BOOL DLLEXPORT GetCVGroup()
{
	for(int i=0;i<MAX_CV_NUM;i++)
		CVGroup[i] = eda_define_group(1,CV_BASE+i);
	return TRUE;
}

BOOL DLLEXPORT GetMVGroup()
{
	for(int i=0;i<MAX_MV_NUM;i++)
		MVGroup[i] = eda_define_group(1,MV_BASE+i);
	return TRUE;
}

BOOL DLLEXPORT GetDVGroup()
{
	for(int i=0;i<MAX_DV_NUM;i++)
		DVGroup[i] = eda_define_group(1,DV_BASE+i);
	return TRUE;
}

BOOL DLLEXPORT GetGVGroup()
{
	for(int i=0;i<MAX_GV_NUM;i++)
		GVGroup[i] = eda_define_group(1,GV_BASE+i);
	return TRUE;
}

BOOL DLLEXPORT GetBlock()
{
	if(!GetCVBlock())return FALSE;
	if(!GetMVBlock())return FALSE;
	if(!GetDVBlock())return FALSE;
	if(!GetGVBlock())return FALSE;
	return TRUE;
}

BOOL DLLEXPORT GetCVBlock()
{
	for(int i=0;i<MAX_CV_NUM;i++)
	{
		for(int j=0;j<MAX_CV_PAR_NUM;j++)
		{
			switch(j)
			{
			case CV_DATA_INDEX:	//0
#ifndef __Simul
				CVBlock[i][j]=eda_define_ntf(CVGroup[i],LocalNodeName,CVTagName[i][CV_IN_BLOCK],"f_cv" , NULL);
#else
				CVBlock[i][j]=eda_define_ntf(CVGroup[i],LocalNodeName,CVTagName[i][MAX_CV_PAR_NUM-1],"f_cv" , NULL);
#endif
				break;
			case CV_HI_INDEX:	//3
				CVBlock[i][j]=eda_define_ntf(CVGroup[i],LocalNodeName,CVTagName[i][CV_IN_BLOCK],"f_hi" , NULL);
				break;
			case CV_LO_INDEX:	//1
				CVBlock[i][j]=eda_define_ntf(CVGroup[i],LocalNodeName,CVTagName[i][CV_IN_BLOCK],"f_lo" , NULL);
				break;
			case CV_HIHI_INDEX:	//4
				CVBlock[i][j]=eda_define_ntf(CVGroup[i],LocalNodeName,CVTagName[i][CV_IN_BLOCK],"f_hihi" , NULL);
				break;
			case CV_LOLO_INDEX:	//2
				CVBlock[i][j]=eda_define_ntf(CVGroup[i],LocalNodeName,CVTagName[i][CV_IN_BLOCK],"f_lolo" , NULL);
				break;
			default:
				CVBlock[i][j]=eda_define_ntf(CVGroup[i],LocalNodeName,CVTagName[i][j-CV_NULL_INDEX],"f_cv" , NULL);
			}
		}
		eda_lookup(CVGroup[i]);
		eda_wait(CVGroup[i]);
	}
	return TRUE;	
}

BOOL DLLEXPORT GetMVBlock()
{
	for(int i=0;i<MAX_MV_NUM;i++)
	{
		for(int j=0;j<MAX_MV_PAR_NUM;j++)
		{
			switch(j)
			{
			case MV_SAMPLE_INDEX:	//0
#ifndef __Simul
				MVBlock[i][j]=eda_define_ntf(MVGroup[i],LocalNodeName,MVTagName[i][MV_IN_BLOCK],"f_cv" , NULL);
#else
				MVBlock[i][j]=eda_define_ntf(MVGroup[i],LocalNodeName,MVTagName[i][MAX_MV_PAR_NUM-1],"f_cv" , NULL);
#endif
				break;
			case MV_HI_INDEX:		//1
				MVBlock[i][j]=eda_define_ntf(MVGroup[i],LocalNodeName,MVTagName[i][MV_IN_BLOCK],"f_hi" , NULL);
				break;
			case MV_LO_INDEX:		//2
				MVBlock[i][j]=eda_define_ntf(MVGroup[i],LocalNodeName,MVTagName[i][MV_IN_BLOCK],"f_lo" , NULL);
				break;
			case MV_HIHI_INDEX:		//3
				MVBlock[i][j]=eda_define_ntf(MVGroup[i],LocalNodeName,MVTagName[i][MV_IN_BLOCK],"f_hihi" , NULL);
				break;
			case MV_LOLO_INDEX:		//4
				MVBlock[i][j]=eda_define_ntf(MVGroup[i],LocalNodeName,MVTagName[i][MV_IN_BLOCK],"f_lolo" , NULL);
				break;
			case MV_VALUE_INDEX:	//5
				MVBlock[i][j]=eda_define_ntf(MVGroup[i],LocalNodeName,MVTagName[i][MV_OUT_BLOCK],"f_cv" , NULL);
				break;
			default:
				MVBlock[i][j]=eda_define_ntf(MVGroup[i],LocalNodeName,MVTagName[i][j-MV_NULL_INDEX],"f_CV" , NULL);
			}
		}
		eda_lookup(MVGroup[i]);
		eda_wait(MVGroup[i]);
	}
	return TRUE;
}

BOOL DLLEXPORT GetDVBlock()
{
	for(int i=0;i<MAX_DV_NUM;i++)
	{
		for(int j=0;j<MAX_DV_PAR_NUM;j++)
		{
			switch(j)
			{
			case DV_DATA_INDEX:	//0
#ifndef __Simul
				DVBlock[i][j]=eda_define_ntf(DVGroup[i],LocalNodeName,DVTagName[i][DV_IN_BLOCK],"f_cv" , NULL);
#else
				DVBlock[i][j]=eda_define_ntf(DVGroup[i],LocalNodeName,DVTagName[i][MAX_DV_PAR_NUM-1],"f_cv" , NULL);
#endif
				break;
			default:
				DVBlock[i][j]=eda_define_ntf(DVGroup[i],LocalNodeName,DVTagName[i][j-MV_NULL_INDEX],"f_CV" , NULL);
			}
		}
		eda_lookup(DVGroup[i]);
		eda_wait(DVGroup[i]);
	}
	return TRUE;
}

BOOL DLLEXPORT GetGVBlock()
{
	for(int i=0;i<MAX_GV_NUM;i++)
	{
		for(int j=0;j<MAX_GV_PAR_NUM;j++)
		{
			GVBlock[i][j]=eda_define_ntf(GVGroup[i],LocalNodeName,GVTagName[i][j-DV_NULL_INDEX],"f_CV" , NULL);
		}
		eda_lookup(GVGroup[i]);
		eda_wait(GVGroup[i]);
	}
	return TRUE;
}

//////////////////////////////////////////////////
//	Read ONE CV
//////////////////////////////////////////////////
BOOL DLLEXPORT ReadCV(LPCOBJPRIVCV lpcObjPrivCV , long Index)
{
	float _Val = 0.0;
	unsigned _block_index = CV_NULL_INDEX;
	
	if( Index<0 || !HasInitialized )
		return FALSE;

	eda_read(CVGroup[Index]);
	eda_wait(CVGroup[Index]);

	//////////////////////////////////////////////////
	// double parameters
	//////////////////////////////////////////////////
	eda_get_float(CVGroup[Index], CVBlock[Index][CV_DATA_INDEX], &_Val);
	lpcObjPrivCV->data[Index]->CData = (double)_Val;				//当前采样数据	the first double member for display

	eda_get_float(CVGroup[Index], CVBlock[Index][CV_HIHI_INDEX], &_Val);
	lpcObjPrivCV->data[Index]->CHAlarm = (double)_Val;				//高限报警

	eda_get_float(CVGroup[Index], CVBlock[Index][CV_HI_INDEX], &_Val);
	lpcObjPrivCV->data[Index]->CHigh = (double)_Val;				//区域上限

	eda_get_float(CVGroup[Index], CVBlock[Index][CV_LO_INDEX], &_Val);
	lpcObjPrivCV->data[Index]->CLow = (double)_Val;

	eda_get_float(CVGroup[Index], CVBlock[Index][CV_LOLO_INDEX], &_Val);
	lpcObjPrivCV->data[Index]->CLAlarm = (double)_Val;
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CHScale = (double)_Val;			//规一化上限

	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CLScale = (double)_Val;			//规一化下限

	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CSetpoint = (double)_Val;		//设定值

	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CIni = (double)_Val;				//初始值
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CMax = (double)_Val;				//区域上限
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CMin = (double)_Val;				//区域下限
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CFault_max = (double)_Val;		//上超限
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CFault_min = (double)_Val;		//下超限
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CFault_high = (double)_Val;		//上超限
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CFault_low = (double)_Val;		//下超限
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CFault_xsetpoint = (double)_Val;	//上超限定值
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CFault_nsetpoint = (double)_Val;	//下超限定值
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CFault_hsetpoint = (double)_Val;	//上超限定值
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CFault_lsetpoint = (double)_Val;	//下超限定值
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CLPIni = (double)_Val;			//LP初始点
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CLPIndex = (double)_Val;			//LP系数
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CSteadOptim = (double)_Val;		//LP优化值
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CQ = (double)_Val;				//定值Q
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CQL = (double)_Val;				//超限Q
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CQMax = (double)_Val;			//最大Q
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CDelQ = (double)_Val;			//增量Q
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CRealQ = (double)_Val;			//实际Q
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CError = (double)_Val;			//失配
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CDisplayH = (double)_Val;		//显示上限
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CDisplayL = (double)_Val;		//显示下限
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CDisplay = (double)_Val;			//显示
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CSteadyDelt = (double)_Val;		//稳态振荡幅度
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CSteadyField = (double)_Val;		//稳定最小区域	the last double member for display
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CLastSet = (double)_Val;			//上一时刻设定值
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CLastHigh = (double)_Val;		//上一时刻区域上限
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CLastLow = (double)_Val;			//上一时刻区域下限
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CIniError = (double)_Val;		//手自动切换预测初值偏差
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CLowFault = (double)_Val;		//低限超限
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CHighFault = (double)_Val;		//高限超限
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CLowSet = (double)_Val;			//低限设定
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CHighSet = (double)_Val;			//高限设定
	
	//////////////////////////////////////////////////
	// WORD parameters
	//////////////////////////////////////////////////
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CRControl_type = (WORD)_Val;		//实际控制类型	added by chen
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CPriority = (WORD)_Val;			//优先权
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CNoLP = (WORD)_Val;				//禁止LP
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CHOverCount = (WORD)_Val;		//超高限计数
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CLOverCount = (WORD)_Val;		//超低限计数
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CControl_type = (WORD)_Val;		//控制类型设定	It's BOOL type in ONSPEC
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CFault_Priority = (WORD)_Val;	//内部优先权
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CCT_history = (WORD)_Val;		//上时刻实际控制类型
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CDirty = (WORD)_Val;				//矩阵逆重算标记
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CQDirtyCount = (WORD)_Val;		//矩阵逆重算计数
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CDelayCount = (WORD)_Val;		//保持计数
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CBad = (WORD)_Val;				//故障次数
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CN = (WORD)_Val;					//起始预测点
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CInvalid = (WORD)_Val;			//无效位	Added by Chen
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CAlarm = (WORD)_Val;				//报警位
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CKey = (WORD)_Val;				//关键位
	
	//////////////////////////////////////////////////
	// BOOL parameters
	//////////////////////////////////////////////////
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CHOver = (BOOL)_Val;				//超高限
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CLOver = (BOOL)_Val;				//超低限
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CLPWrong = (BOOL)_Val;			//LP无法实现
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CNoIll = (BOOL)_Val;				//禁止去除病态
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CHEdge = (BOOL)_Val;				//高卡边
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CLEdge = (BOOL)_Val;				//低卡边
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CQMode = (BOOL)_Val;				//Q校正模式	Added by Chen
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CErrorMode = (BOOL)_Val;			//偏差校正模式	Added by Chen 未用
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CLimit = (BOOL)_Val;				//超限模式 Added by Chen 未用
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CQBroken = (BOOL)_Val;			//Q饱和
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CSDirty = (BOOL)_Val;			//LP优化
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CLastCM = (BOOL)_Val;			//上次控制设定
	
	eda_get_float(CVGroup[Index], CVBlock[Index][++_block_index], &_Val);
	lpcObjPrivCV->data[Index]->CCM = (BOOL)_Val;				//控制设定

	return TRUE;
}

BOOL DLLEXPORT ReadMV(LPCOBJPRIVMV lpcObjPrivMV , long Index)
{
	float _Val = 0.0;
	unsigned _block_index = MV_NULL_INDEX;
	
	if( Index<0 || !HasInitialized )
		return FALSE;

	eda_read(MVGroup[Index]);
	eda_wait(MVGroup[Index]);

	//////////////////////////////////////////////////
	// double parameters
	//////////////////////////////////////////////////
	eda_get_float(MVGroup[Index], MVBlock[Index][MV_VALUE_INDEX], &_Val);
	lpcObjPrivMV->data[Index]->MData = (double)_Val;				//采样值	the first double member for display
	
	eda_get_float(MVGroup[Index], MVBlock[Index][MV_HIHI_INDEX], &_Val);
	lpcObjPrivMV->data[Index]->MHAlarm = (double)_Val;			//高限报警
	
	eda_get_float(MVGroup[Index], MVBlock[Index][MV_HI_INDEX], &_Val);
	lpcObjPrivMV->data[Index]->MIrvHigh = (double)_Val;				//区域上限

	eda_get_float(MVGroup[Index], MVBlock[Index][MV_SAMPLE_INDEX], &_Val);
	lpcObjPrivMV->data[Index]->MSample = (double)_Val;			//采样MV  The last double member for display
	
	eda_get_float(MVGroup[Index], MVBlock[Index][MV_LO_INDEX], &_Val);
	lpcObjPrivMV->data[Index]->MIrvLow = (double)_Val;				//区域下限
	
	eda_get_float(MVGroup[Index], MVBlock[Index][MV_LOLO_INDEX], &_Val);
	lpcObjPrivMV->data[Index]->MLAlarm = (double)_Val;			//低限报警
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MHScale = (double)_Val;			//规一化上限
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MLScale = (double)_Val;			//规一化下限
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MDelt = (double)_Val;				//控制增量
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MIrv = (double)_Val;				//设定值
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MIni = (double)_Val;				//初始值
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MDelmax = (double)_Val;			//最大增量
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MDelmin = (double)_Val;			//最小增量
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MMax = (double)_Val;			//区域上限
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MMin = (double)_Val;			//区域下限
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MFault_max = (double)_Val;			//上超限
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MFault_min = (double)_Val;			//下超限
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MIrvFault_high = (double)_Val;		//上超限
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MIrvFault_low = (double)_Val;		//下超限
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MFault_xirv = (double)_Val;		//上超限设定
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MFault_nirv = (double)_Val;		//下超限设定
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MFault_hirv = (double)_Val;		//上超限设定
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MFault_lirv = (double)_Val;		//下超限设定
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MLPIni = (double)_Val;				//LP初始点
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MLPIndex = (double)_Val;			//LP系数
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MSteadOptim = (double)_Val;		//LP优化值
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MR = (double)_Val;					//R
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MRMax = (double)_Val;				//最大R
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MP = (double)_Val;					//P
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MPL = (double)_Val;				//超限P
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MPMax = (double)_Val;				//最大P
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MDelR = (double)_Val;				//增量R
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MDelP = (double)_Val;				//增量P
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MRealR = (double)_Val;				//实际R
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MRealP = (double)_Val;				//实际P
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MDisplayH = (double)_Val;		//显示上限
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MDisplayL = (double)_Val;			//显示下限
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MDisplay = (double)_Val;			//显示
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MSteadyField = (double)_Val;		//稳定最小区域	
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MHistory = (double)_Val;			//历史采样值
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MLastIRV = (double)_Val;			//历史实际设定值
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MLastHigh = (double)_Val;			//历史区域上限
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MLastLow = (double)_Val;			//历史区域下限

	//////////////////////////////////////////////////
	// WORD parameters
	//////////////////////////////////////////////////
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MRControl_type = (WORD)_Val;		//实际控制类型	added ty chen
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MPriority = (WORD)_Val;			//有限权
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MHOverCount = (WORD)_Val;		//超高限计数
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MLOverCount = (WORD)_Val;		//超低限计数
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MControl_type = (WORD)_Val;		//控制类型	it's BOOL type in ONSPEC
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MFault_Priority = (WORD)_Val;		//内部有限权
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MPDirtyCount = (WORD)_Val;			//超限计数
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MRDirtyCount = (WORD)_Val;			//增量超限计数
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MCT_history = (WORD)_Val;			//历史实际控制类型
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MBad = (WORD)_Val;					//故障计数

	//////////////////////////////////////////////////
	// BOOL parameters
	//////////////////////////////////////////////////
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MInvalidate = (BOOL)_Val;			//无效位	
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MDist = (BOOL)_Val;					//扰动位
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MSatuation = (BOOL)_Val;				//饱和标记
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MAlarm = (BOOL)_Val;					//报警位
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MKey = (BOOL)_Val;					//关键位
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MSetSatuation = (BOOL)_Val;			//设置饱和
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MIRVWrong = (BOOL)_Val;				//错误设定
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MApartAM = (BOOL)_Val;				//子模块手自动
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MAM = (BOOL)_Val;					//手自动
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MHope_AM = (BOOL)_Val;				//希望手自动
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MHEdge = (BOOL)_Val;					//高卡边
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MLEdge = (BOOL)_Val;					//低卡边
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MRDirty = (BOOL)_Val;				//增量超限
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MPDirty = (BOOL)_Val;				//超限
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MSDirty = (BOOL)_Val;				//LP优化
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MRBroken = (BOOL)_Val;				//R饱和
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MPBroken = (BOOL)_Val;				//P饱和
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MHOver = (BOOL)_Val;					//超高限
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MLOver = (BOOL)_Val;					//超低限
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MLastCM = (BOOL)_Val;				//历史控制设定
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MRCorrectMode = (BOOL)_Val;			//R校正模式	Added by Chen 未用
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MSCorrectMode = (BOOL)_Val;			//P校正模式	Added by Chen 未用
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MErrorMode = (BOOL)_Val;				//偏差校正模式	Added by Chen 未用
	
	eda_get_float(MVGroup[Index], MVBlock[Index][++_block_index], &_Val);
	lpcObjPrivMV->data[Index]->MCM = (BOOL)_Val;					//控制设定

	return TRUE;
	
}

BOOL DLLEXPORT ReadDV(LPCOBJPRIVDV lpcObjPrivDV , long Index)
{
	float _Val = 0.0;
	unsigned _block_index = DV_NULL_INDEX;
	
	if( Index<0 || !HasInitialized )
		return FALSE;

//	eda_lookup(DVGroup[Index]);
//	eda_wait(DVGroup[Index]);
	eda_read(DVGroup[Index]);
	eda_wait(DVGroup[Index]);

	//////////////////////////////////////////////////
	// double parameters
	//////////////////////////////////////////////////
	eda_get_float(DVGroup[Index], DVBlock[Index][DV_DATA_INDEX], &_Val);
	lpcObjPrivDV->data[Index]->DData = (double)_Val;				
	
	eda_get_float(DVGroup[Index], DVBlock[Index][++_block_index], &_Val);
	lpcObjPrivDV->data[Index]->DHScale = (double)_Val;			//规一化上限
	
	eda_get_float(DVGroup[Index], DVBlock[Index][++_block_index], &_Val);
	lpcObjPrivDV->data[Index]->DLScale = (double)_Val;			//规一化下限
	
	eda_get_float(DVGroup[Index], DVBlock[Index][++_block_index], &_Val);
	lpcObjPrivDV->data[Index]->DIni = (double)_Val;				//初始值
	
	eda_get_float(DVGroup[Index], DVBlock[Index][++_block_index], &_Val);
	lpcObjPrivDV->data[Index]->DDelt = (double)_Val;				//增量
	
	eda_get_float(DVGroup[Index], DVBlock[Index][++_block_index], &_Val);
	lpcObjPrivDV->data[Index]->DHistory = (double)_Val;			//历史采样值
	
	eda_get_float(DVGroup[Index], DVBlock[Index][++_block_index], &_Val);
	lpcObjPrivDV->data[Index]->DDisplayH = (double)_Val;			//显示上限
	
	eda_get_float(DVGroup[Index], DVBlock[Index][++_block_index], &_Val);
	lpcObjPrivDV->data[Index]->DDisplayL = (double)_Val;			//显示下限
	
	eda_get_float(DVGroup[Index], DVBlock[Index][++_block_index], &_Val);
	lpcObjPrivDV->data[Index]->DDisplay = (double)_Val;			//显示
	
	eda_get_float(DVGroup[Index], DVBlock[Index][++_block_index], &_Val);
	lpcObjPrivDV->data[Index]->DSteadyField = (double)_Val;		//稳态最小区域

	//////////////////////////////////////////////////
	// BOOL parameters
	//////////////////////////////////////////////////
	eda_get_float(DVGroup[Index], DVBlock[Index][++_block_index], &_Val);
	lpcObjPrivDV->data[Index]->DInvalid = (BOOL)_Val;			//无效位
	
	eda_get_float(DVGroup[Index], DVBlock[Index][++_block_index], &_Val);
	lpcObjPrivDV->data[Index]->DAlarm = (BOOL)_Val;				//报警位
	
	//////////////////////////////////////////////////
	// WORD parameters
	//////////////////////////////////////////////////
	eda_get_float(DVGroup[Index], DVBlock[Index][++_block_index], &_Val);
	lpcObjPrivDV->data[Index]->DBad = (WORD)_Val;				//故障次数
	return TRUE;
}

BOOL DLLEXPORT ReadGV(LPCOBJPRIV lpcObjPrivGV )
{
	float _Val = 0.0;
	unsigned _block_index = GV_NULL_INDEX;
	long Index = 0;
	
	if( Index<0 || !HasInitialized )
		return FALSE;

//	eda_lookup(GVGroup[Index]);
//	eda_wait(GVGroup[Index]);
	eda_read(GVGroup[Index]);
	eda_wait(GVGroup[Index]);

	//////////////////////////////////////////////////
	// double parameters
	//////////////////////////////////////////////////
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->File_No = (WORD)_Val;		//仿真文件序号
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->N = (WORD)_Val;				//模型长度
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->P = (WORD)_Val;				//预测时域
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->M = (WORD)_Val;				//控制时域
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->p = (WORD)_Val;				//被控变量个数
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->m = (WORD)_Val;				//操纵变量个数
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->nd = (WORD)_Val;				//扰动变量个数
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->Count = (WORD)_Val;			//控制周期
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->MakeParaCount = (WORD)_Val;  //矩阵重算计数
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->RecountCount = (WORD)_Val;	//动态重算计数
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->LPCount = (WORD)_Val;		//稳态优化次数
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->No_Apart = (WORD)_Val;		//字模块数
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->MParaCount = (WORD)_Val;		//权矩阵保持计数
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->MParaMax = (WORD)_Val;		//最大权矩阵保持次数
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->SteadyStep = (WORD)_Val;		//判稳步数
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->ErrorCode = (WORD)_Val;		//错误代码
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->Flag_AM = (WORD)_Val;		//手自动开关		it's BOOL in ONSPEC
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->Freecount = (int)_Val;		//自由度
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->MaxDelay = (WORD)_Val;		//最大维持约束次数	don't display in ONSPEC
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->wModu = (WORD)_Val;			//模块序号
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->N1 = (WORD)_Val;				//预测时域起始点		不用
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->N2 = (WORD)_Val;				//预测时域中止点		不用
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->SteadyCount = (WORD)_Val;	//稳态次数
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->CalSteadyMax = (WORD)_Val;	//稳态计算的条件数病态下限
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->Apart_i = (WORD)_Val;		//字模块序号
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->BadMax = (WORD)_Val;			//数据坏最大容忍次数
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->Flag_CControl = (BOOL)_Val;	//CV自由度控制方式	the first BOOL member for display
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->Flag_MControl = (BOOL)_Val;	//GV自由度控制方式	
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->Flag_Fault = (BOOL)_Val;		//虚拟限方式
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->Flag_LP = (BOOL)_Val;		//实行LP
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->Flag_Alarm = (BOOL)_Val;		//报警
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->Flag_ReSP = (BOOL)_Val;		//权系数重设	this flag is for the fixed index matrix
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->Flag_SRMode = (BOOL)_Val;	//阶越响应构造方式
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->Flag_MChange = (BOOL)_Val;	//模型变更
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->Flag_LPChange = (BOOL)_Val;	//LP模型变更
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->Flag_HopeAM = (BOOL)_Val;	//希望手自动方式
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->Flag_Recount = (BOOL)_Val;	//动态规划重算
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->Flag_LPRecount = (BOOL)_Val;	//LP重算
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->StartMode = (BOOL)_Val;		//启动方式
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->Flag_Weight = (BOOL)_Val;	//权系数调整方式
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->Flag_Debug = (BOOL)_Val;		//SYSOP/OPERATOR模式
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->Flag_HMode = (BOOL)_Val;		//反馈校正方式
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->Flag_AM1 = (BOOL)_Val;		//反再手自动
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->Flag_AM2 = (BOOL)_Val;		//分馏手自动
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->Flag_AM3 = (BOOL)_Val;		//吸稳手自动
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->Flag_Run = (BOOL)_Val;		//运行标志
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->Flag_Steady = (BOOL)_Val;	//稳态系数生成标记
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->Flag_MCVRecount = (BOOL)_Val;//权矩阵重组	don't display in ONSPEC
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->Flag_RDebug = (BOOL)_Val;	//内部调试位	don't display in ONSPEC
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->Flag_Save = (BOOL)_Val;		//存储位
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->IniStart = (BOOL)_Val;		//初始值启动
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->Flag_CorSteady = (BOOL)_Val;	//有效稳态
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->CalSteady = (BOOL)_Val;		//稳态模型更正
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->Error = (BOOL)_Val;			//出错
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->LpK = (double)_Val;			//LP目标函数常数
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->Alpha = (double)_Val;			//反馈校正系数
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->Big_Cond = (double)_Val;		//最大条件数
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->PBroken = (double)_Val;		//最大P
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->QBroken = (double)_Val;		//最大Q
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->RBroken = (double)_Val;		//最大R
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->SRError = (double)_Val;		//模型偏差
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->DSRError = (double)_Val;		//扰动模型偏差
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->Cor_Steady = (double)_Val;		//稳态校正系数
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->SampleTime = (double)_Val;		//采样时间
	
	eda_get_float(GVGroup[Index], GVBlock[Index][++_block_index], &_Val);
	lpcObjPrivGV->DFilter = (double)_Val;		//扰动滤波系数
	return TRUE;
}

BOOL DLLEXPORT WriteCV(LPCOBJPRIVCV lpcObjPrivCV , long Index)
{
	unsigned _block_index = CV_NULL_INDEX;
	float _val;

	if( Index<0 || !HasInitialized )
		return FALSE;

	_val = (float)lpcObjPrivCV->data[Index]->CData;				//当前采样数据	the first double member for display
	eda_set_float(CVGroup[Index], CVBlock[Index][CV_DATA_INDEX] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CHScale;			//规一化上限
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CLScale;			//规一化下限
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CSetpoint;			//设定值
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CIni;				//初始值
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CMax;				//区域上限
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CMin;				//区域下限
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CHigh;				//区域上限
//	eda_set_float(CVGroup[Index], CVBlock[Index][CV_HI_INDEX] , &_val);
	eda_set_float(CVGroup[Index], CVBlock[Index][3] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CLow;				//区域下限
	eda_set_float(CVGroup[Index], CVBlock[Index][CV_LO_INDEX] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CFault_max;			//上超限
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CFault_min;			//下超限
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CFault_high;		//上超限
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CFault_low;			//下超限
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CFault_xsetpoint;	//上超限定值
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CFault_nsetpoint;	//下超限定值
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CFault_hsetpoint;	//上超限定值
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CFault_lsetpoint;	//下超限定值
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CLPIni;				//LP初始点
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CLPIndex;			//LP系数
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CSteadOptim;		//LP优化值
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CQ;					//定值Q
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CQL;				//超限Q
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CQMax;				//最大Q
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CDelQ;				//增量Q
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CRealQ;				//实际Q
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CHAlarm;			//高限报警
	eda_set_float(CVGroup[Index], CVBlock[Index][CV_HIHI_INDEX] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CLAlarm;			//低限报警
	eda_set_float(CVGroup[Index], CVBlock[Index][CV_LOLO_INDEX] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CError;				//失配
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CDisplayH;			//显示上限
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CDisplayL;			//显示下限
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CDisplay;			//显示
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CSteadyDelt;		//稳态振荡幅度
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CSteadyField;		//稳定最小区域	the last 
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CLastSet;			//上一时刻设定值
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CLastHigh;			//上一时刻区域上限
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CLastLow;			//上一时刻区域下限
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CIniError;			//手自动切换预测初值偏差
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CLowFault;			//低限超限
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CHighFault;			//高限超限
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CLowSet;			//低限设定
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CHighSet;			//高限设定
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

    _val = (float)lpcObjPrivCV->data[Index]->CRControl_type;		//实际控制类型	added by chen
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CPriority;			//优先权
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CNoLP;				//禁止LP
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CHOverCount;		//超高限计数
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CLOverCount;		//超低限计数
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CControl_type;		//控制类型设定	It's BOOL type in ONSPEC
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CFault_Priority;	//内部优先权
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CCT_history;		//上时刻实际控制类型
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CDirty;				//矩阵逆重算标记
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CQDirtyCount;		//矩阵逆重算计数
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

    _val = (float)lpcObjPrivCV->data[Index]->CDelayCount;		//保持计数
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CBad;				//故障次数
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CN;					//起始预测点
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CInvalid;			//无效位	Added by Chen
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CAlarm;				//报警位
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CKey;				//关键位
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CHOver;				//超高限
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CLOver;				//超低限
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

    _val = (float)lpcObjPrivCV->data[Index]->CLPWrong;			//LP无法实现
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CNoIll;				//禁止去除病态
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CHEdge;				//高卡边
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CLEdge;				//低卡边
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CQMode;				//Q校正模式	Added by Chen
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CErrorMode;			//偏差校正模式	Added by Chen 未用
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CLimit;				//超限模式 Added by Chen 未用
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CQBroken;			//Q饱和
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CSDirty;			//LP优化
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CLastCM;			//上次控制设定
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivCV->data[Index]->CCM;				//控制设定
	eda_set_float(CVGroup[Index], CVBlock[Index][++_block_index] , &_val);

	eda_write(CVGroup[Index]);
//	eda_wait(CVGroup[Index]);
	return TRUE;
}

BOOL DLLEXPORT WriteMV(LPCOBJPRIVMV lpcObjPrivMV , long Index)
{
	unsigned _block_index = MV_NULL_INDEX;
	float _val;

	if( Index<0 || !HasInitialized )
		return FALSE;
	
	_val = (float)lpcObjPrivMV->data[Index]->MData;				//采样值
	eda_set_float(MVGroup[Index], MVBlock[Index][MV_VALUE_INDEX] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MHScale;			//规一化上限
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MLScale;			//规一化下限
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MDelt;				//控制增量
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MIrv;				//设定值
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MIni;				//初始值
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MDelmax;			//最大增量
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MDelmin;			//最小增量
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MMax;				//区域上限
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MMin;				//区域下限
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MIrvHigh;			//区域上限
	eda_set_float(MVGroup[Index], MVBlock[Index][MV_HI_INDEX] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MIrvLow;			//区域下限
	eda_set_float(MVGroup[Index], MVBlock[Index][MV_LO_INDEX] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MFault_max;			//上超限
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MFault_min;			//下超限
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MIrvFault_high;		//上超限
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MIrvFault_low;		//下超限
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MFault_xirv;		//上超限设定
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MFault_nirv;		//下超限设定
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MFault_hirv;		//上超限设定
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MFault_lirv;		//下超限设定
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MLPIni;				//LP初始点
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MLPIndex;			//LP系数
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MSteadOptim;		//LP优化值
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MR;					//R
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MRMax;				//最大R
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

    _val = (float)lpcObjPrivMV->data[Index]->MP;					//P
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MPL;				//超限P
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MPMax;				//最大P
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MDelR;				//增量R
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MDelP;				//增量P
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MRealR;				//实际R
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MRealP;				//实际P
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MHAlarm;			//高限报警
	eda_set_float(MVGroup[Index], MVBlock[Index][MV_HIHI_INDEX] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MLAlarm;			//低限报警
	eda_set_float(MVGroup[Index], MVBlock[Index][MV_LOLO_INDEX] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MDisplayH;			//显示上限
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MDisplayL;			//显示下限
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MDisplay;			//显示
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MSample;			//采样MV  
	eda_set_float(MVGroup[Index], MVBlock[Index][MV_SAMPLE_INDEX] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MSteadyField;		//稳定最小区域	
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MHistory;			//历史采样值
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MLastIRV;			//历史实际设定值
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MLastHigh;			//历史区域上限
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

    _val = (float)lpcObjPrivMV->data[Index]->MLastLow;			//历史区域下限
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

    _val = (float)lpcObjPrivMV->data[Index]->MRControl_type;		//实际控制类型	added ty chen
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MPriority;			//有限权
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MHOverCount;		//超高限计数
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MLOverCount;		//超低限计数
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MControl_type;		//控制类型	it's BOOL type in ONSPEC
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MFault_Priority;		//内部有限权
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MPDirtyCount;			//超限计数
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MRDirtyCount;			//增量超限计数
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MCT_history;			//历史实际控制类型
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MBad;					//故障计数
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MInvalidate;			//无效位
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MDist;					//扰动位
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MSatuation;				//饱和标记
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

    _val = (float)lpcObjPrivMV->data[Index]->MAlarm;					//报警位
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MKey;					//关键位
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MSetSatuation;			//设置饱和
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MIRVWrong;				//错误设定
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

    _val = (float)lpcObjPrivMV->data[Index]->MApartAM;				//子模块手自动
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MAM;					//手自动
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MHope_AM;				//希望手自动
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MHEdge;					//高卡边
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MLEdge;					//低卡边
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MRDirty;				//增量超限
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MPDirty;				//超限
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

    _val = (float)lpcObjPrivMV->data[Index]->MSDirty;				//LP优化
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MRBroken;				//R饱和
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MPBroken;				//P饱和
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MHOver;					//超高限
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MLOver;					//超低限
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MLastCM;				//历史控制设定
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MRCorrectMode;			//R校正模式	Added by Chen 未用
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MSCorrectMode;			//P校正模式	Added by Chen 未用
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MErrorMode;				//偏差校正模式	Added by Chen 未用
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivMV->data[Index]->MCM;					//控制设定
	eda_set_float(MVGroup[Index], MVBlock[Index][++_block_index] , &_val);

	eda_write(MVGroup[Index]);
	eda_wait(MVGroup[Index]);
	return TRUE;
}

BOOL DLLEXPORT WriteDV(LPCOBJPRIVDV lpcObjPrivDV , long Index)
{
	unsigned _block_index = DV_NULL_INDEX;
	float _val;

	if( Index<0 || !HasInitialized )
		return FALSE;

	_val = (float)lpcObjPrivDV->data[Index]->DData;				//采样值
	eda_set_float(DVGroup[Index], DVBlock[Index][DV_DATA_INDEX] , &_val);

	_val = (float)lpcObjPrivDV->data[Index]->DHScale;			//规一化上限
	eda_set_float(DVGroup[Index], DVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivDV->data[Index]->DLScale;			//规一化下限
	eda_set_float(DVGroup[Index], DVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivDV->data[Index]->DIni;				//初始值
	eda_set_float(DVGroup[Index], DVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivDV->data[Index]->DDelt;				//增量
	eda_set_float(DVGroup[Index], DVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivDV->data[Index]->DHistory;			//历史采样值
	eda_set_float(DVGroup[Index], DVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivDV->data[Index]->DDisplayH;			//显示上限
	eda_set_float(DVGroup[Index], DVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivDV->data[Index]->DDisplayL;			//显示下限
	eda_set_float(DVGroup[Index], DVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivDV->data[Index]->DDisplay;			//显示
	eda_set_float(DVGroup[Index], DVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivDV->data[Index]->DSteadyField;		//稳态最小区域
	eda_set_float(DVGroup[Index], DVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivDV->data[Index]->DInvalid;			//无效位
	eda_set_float(DVGroup[Index], DVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivDV->data[Index]->DAlarm;				//报警位
	eda_set_float(DVGroup[Index], DVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivDV->data[Index]->DBad;				//故障次数
	eda_set_float(DVGroup[Index], DVBlock[Index][++_block_index] , &_val);

	eda_write(DVGroup[Index]);
	eda_wait(DVGroup[Index]);
	return TRUE;
}

BOOL DLLEXPORT WriteGV(LPCOBJPRIV lpcObjPrivGV )
{
	unsigned _block_index = GV_NULL_INDEX;
	float _val;
	long	Index = 0;
	
	if( Index<0 || !HasInitialized )
		return FALSE;

	_val = (float)lpcObjPrivGV->File_No;		//仿真文件序号
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->N;				//模型长度
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->P;				//预测时域
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->M;				//控制时域
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->p;				//被控变量个数
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->m;				//操纵变量个数
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->nd;				//扰动变量个数
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->Count;			//控制周期
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->MakeParaCount;  //矩阵重算计数
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->RecountCount;	//动态重算计数
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->LPCount;		//稳态优化次数
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->No_Apart;		//字模块数
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->MParaCount;		//权矩阵保持计数
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->MParaMax;		//最大权矩阵保持次数
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->SteadyStep;		//判稳步数
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->ErrorCode;		//错误代码
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->Flag_AM;		//手自动开关		it's BOOL in ONSPEC
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->Freecount;		//自由度
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->MaxDelay;		//最大维持约束次数	don't display in ONSPEC
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->wModu;			//模块序号
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->N1;				//预测时域起始点		不用
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->N2;				//预测时域中止点		不用
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->SteadyCount;	//稳态次数
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->CalSteadyMax;	//稳态计算的条件数病态下限
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->Apart_i;		//字模块序号
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->BadMax;			//数据坏最大容忍次数
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->Flag_CControl;	//CV自由度控制方式	
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->Flag_MControl;	//MV自由度控制方式	
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->Flag_Fault;		//虚拟限方式
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->Flag_LP;		//实行LP
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->Flag_Alarm;		//报警
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->Flag_ReSP;		//权系数重设	
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->Flag_SRMode;	//阶越响应构造方式
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->Flag_MChange;	//模型变更
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->Flag_LPChange;	//LP模型变更
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->Flag_HopeAM;	//希望手自动方式
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->Flag_Recount;	//动态规划重算
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->Flag_LPRecount;	//LP重算
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->StartMode;		//启动方式
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->Flag_Weight;	//权系数调整方式
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->Flag_Debug;		//SYSOP/OPERATOR模式
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->Flag_HMode;		//反馈校正方式
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->Flag_AM1;		//反再手自动
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->Flag_AM2;		//分馏手自动
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->Flag_AM3;		//吸稳手自动
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->Flag_Run;		//运行标志
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->Flag_Steady;	//稳态系数生成标记
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->Flag_MCVRecount;//权矩阵重组	don't display in ONSPEC
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->Flag_RDebug;	//内部调试位	don't display in ONSPEC
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->Flag_Save;		//存储位
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->IniStart;		//初始值启动
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->Flag_CorSteady;	//有效稳态
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->CalSteady;		//稳态模型更正
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->Error;			//出错
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->LpK;			//LP目标函数常数
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->Alpha;			//反馈校正系数
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->Big_Cond;		//最大条件数
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->PBroken;		//最大P
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->QBroken;		//最大Q
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->RBroken;		//最大R
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->SRError;		//模型偏差
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->DSRError;		//扰动模型偏差
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->Cor_Steady;		//稳态校正系数
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->SampleTime;		//采样时间
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	_val = (float)lpcObjPrivGV->DFilter;		//扰动滤波系数
	eda_set_float(GVGroup[Index], GVBlock[Index][++_block_index] , &_val);

	eda_write(GVGroup[Index]);
	eda_wait(GVGroup[Index]);
	return TRUE;
}

DLLEXPORT void* ReadCVMember(LPCOBJ lpcObjCV , long Index , WORD PAR_ID, void* pNewVal )
{
	long	_block_index = CV_PAR_ACTU_NUM;
	void * pvoid = NULL;
	TYPE_ID	t;
	LPCOBJPRIVCV	lpcObjPrivCV;
	float			_ft;
	lpcObjPrivCV = COBJ_2_PRIVCV( lpcObjCV );

	if( Index<0 || !HasInitialized )
		return FALSE;

	eda_read(CVGroup[Index]);
	eda_wait(CVGroup[Index]);

	switch(PAR_ID)
	{
	case CV_VALUE:		
		_block_index = CV_DATA_INDEX;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CData;
		t=TY_DOUBLE;
		break;
	case CV_HIGH:
		_block_index = CV_HI_INDEX;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CHigh;
		t=TY_DOUBLE;
		break;
	case CV_LOW:
		_block_index = CV_LO_INDEX;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CLow;
		t=TY_DOUBLE;
		break;
	case CV_ALARMMAX:
		_block_index = CV_HIHI_INDEX;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CHAlarm;
		t=TY_DOUBLE;
		break;
	case CV_ALARMMIN:
		_block_index = CV_LOLO_INDEX;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CLAlarm;
		t=TY_DOUBLE;
		break;
	case CV_HSCALE:		
		_block_index = 5;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CHScale;
		t = TY_DOUBLE;
		break;
	case CV_LSCALE:
		_block_index = 6;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CLScale;
		t = TY_DOUBLE;
		break;
	case CV_SET:
		_block_index = 7;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CSetpoint;
		t = TY_DOUBLE;
		break;
	case CV_INI:
		_block_index = 8;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CIni;
		t = TY_DOUBLE;
		break;
	case CV_MAX:
		_block_index = 9;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CMax;
		t = TY_DOUBLE;
		break;
	case CV_MIN:
		_block_index = 10;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CMin;
		t = TY_DOUBLE;
		break;
	case CV_FMAX:
		_block_index = 11;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CFault_max;
		t = TY_DOUBLE;
		break;
	case CV_FMIN:
		_block_index = 12;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CFault_min;
		t = TY_DOUBLE;
		break;
	case CV_FHIGH:
		_block_index = 13;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CFault_high;
		t = TY_DOUBLE;
		break;
	case CV_FLOW:
		_block_index = 14;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CFault_min;
		t = TY_DOUBLE;
		break;
	case CV_FXSET:
		_block_index = 15;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CFault_xsetpoint;
		t = TY_DOUBLE;
		break;
	case CV_FNSET:
		_block_index = 16;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CFault_nsetpoint;
		t = TY_DOUBLE;
		break;
	case CV_FHSET:
		_block_index = 17;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CFault_hsetpoint;
		t = TY_DOUBLE;
		break;
	case CV_FLSET:
		_block_index = 18;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CFault_lsetpoint;
		t = TY_DOUBLE;
		break;
	case CV_LPINI:
		_block_index = 19;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CLPIni;
		t = TY_DOUBLE;
		break;
	case CV_LPINDEX:
		_block_index = 20;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CLPIndex;
		t = TY_DOUBLE;
		break;
	case CV_LPOUT:
		_block_index = 21;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CSteadOptim;
		t = TY_DOUBLE;
		break;
	case CV_Q:
		_block_index = 22;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CQ;
		t = TY_DOUBLE;
		break;
	case CV_QL:
		_block_index = 23;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CQL;
		t = TY_DOUBLE;
		break;
	case CV_QBROKEN:
		_block_index = 24;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CQMax;
		t = TY_DOUBLE;
		break;
	case CV_DELQ:
		_block_index = 25;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CDelQ;
		t = TY_DOUBLE;
		break;
	case CV_REALQ:
		_block_index = 26;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CRealQ;
		t = TY_DOUBLE;
		break;
	case CV_ERROR:
		_block_index = 27;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CError;
		t = TY_DOUBLE;
		break;
	case CV_DISPLAYH:
		_block_index = 28;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CDisplayH;
		t = TY_DOUBLE;
		break;
	case CV_DISPLAYL:
		_block_index = 29;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CDisplayL;
		t = TY_DOUBLE;
		break;
	case CV_DISPLAY:
		_block_index = 30;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CDisplay;
		t = TY_DOUBLE;
		break;
	case CV_STEADYDELT:
		_block_index = 31;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CSteadyDelt;
		t = TY_DOUBLE;
		break;
	case CV_STEADYFIELD:
		_block_index = 32;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CSteadyField;
		t = TY_DOUBLE;
		break;
	case CV_LASTSET:
		_block_index = 33;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CLastSet;
		t = TY_DOUBLE;
		break;
	case CV_LASTHIGH:
		_block_index = 34;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CLastHigh;
		t = TY_DOUBLE;
		break;
	case CV_LASTLOW:
		_block_index = 35;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CLastLow;
		t = TY_DOUBLE;
		break;
	case CV_INIERROR:
		_block_index = 36;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CIniError;
		t = TY_DOUBLE;
		break;
	case CV_LFAULT:
		_block_index = 37;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CLowFault;
		t = TY_DOUBLE;
		break;
	case CV_HFAULT:
		_block_index = 38;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CHighFault;
		t = TY_DOUBLE;
		break;
	case CV_LSET:
		_block_index = 39;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CLowSet;
		t = TY_DOUBLE;
		break;
	case CV_HSET:
		_block_index = 40;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CHighSet;
		t = TY_DOUBLE;
		break;
	case CV_RCONTROLMODE:
		_block_index = 41;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CRControl_type;
		t = TY_WORD;
		break;
	case CV_PRIORITY:
		_block_index = 42;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CPriority;
		t = TY_WORD;
		break;
	case CV_NOLP:
		_block_index = 43;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CNoLP;
		t = TY_WORD;
		break;
	case CV_HOVERCOUNT:
		_block_index = 44;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CHOverCount;
		t = TY_WORD;
		break;
	case CV_LOVERCOUNT:
		_block_index = 45;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CLOverCount;
		t = TY_DOUBLE;
		break;
	case CV_CONTROLMODE:
		_block_index = 46;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CControl_type;
		t = TY_WORD;
		break;
	case CV_FPRIORITY:
		_block_index = 47;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CFault_Priority;
		t = TY_WORD;
		break;
	case CV_CTHISTORY:
		_block_index = 48;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CCT_history;
		t = TY_WORD;
		break;
	case CV_DIRTY:
		_block_index = 49;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CDirty;
		t = TY_WORD;
		break;
	case CV_DIRTYCOUNT:
		_block_index = 50;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CQDirtyCount;
		t = TY_WORD;
		break;
	case CV_DELAYCOUNT:
		_block_index = 51;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CDelayCount;
		t = TY_WORD;
		break;
	case CV_BAD:
		_block_index = 52;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CBad;
		t = TY_WORD;
		break;
	case CV_N:
		_block_index = 53;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CN;
		t = TY_WORD;
		break;
	case CV_STATE:
		_block_index = 54;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CInvalid;
		t = TY_BOOL;
		break;
	case CV_ALARM:
		_block_index = 55;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CAlarm;
		t = TY_BOOL;
		break;
	case CV_KEYCV:
		_block_index = 56;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CKey;
		t = TY_BOOL;
		break;
	case CV_HOVER:
		_block_index = 57;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CHOver;
		t = TY_BOOL;
		break;
	case CV_LOVER:
		_block_index = 58;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CLOver;
		t = TY_BOOL;
		break;
	case CV_LPWRONG:
		_block_index = 59;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CLPWrong;
		t = TY_BOOL;
		break;
	case CV_NOILL:
		_block_index = 60;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CNoIll;
		t = TY_BOOL;
		break;
	case CV_HEDGE:
		_block_index = 61;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CHEdge;
		t = TY_BOOL;
		break;
	case CV_LEDGE:
		_block_index = 62;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CLEdge;
		t = TY_BOOL;
		break;
	case CV_QMODE:
		_block_index = 63;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CQMode;
		t = TY_BOOL;
		break;
	case CV_ERRMODE:
		_block_index = 64;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CErrorMode;
		t = TY_BOOL;
		break;
	case CV_LIMIT:
		_block_index = 65;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CLimit;
		t = TY_BOOL;
		break;
	case CV_BROKEN:
		_block_index = 66;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CQBroken;
		t = TY_BOOL;
		break;
	case CV_SDIRTY:
		_block_index = 67;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CSDirty;
		t = TY_BOOL;
		break;
	case CV_LASTCM:
		_block_index = 68;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CLastCM;
		t = TY_BOOL;
		break;
	case CV_CM:
		_block_index = 69;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CCM;
		t = TY_BOOL;
		break;
	case CV_NAME:
		pvoid = (void*)&lpcObjPrivCV->data[Index]->LPName;
		t = TY_LPCOBJ;
		break;
	case CV_TAG:
		pvoid = (void*)&lpcObjPrivCV->data[Index]->LPTag;
		t = TY_LPCOBJ;
		break;
	case CV_UNIT:
		pvoid = (void*)&lpcObjPrivCV->data[Index]->LPUnit;
		t = TY_LPCOBJ;
		break;
	case CV_DATANAME:
		pvoid = (void*)&lpcObjPrivCV->data[Index]->LPBaseName;
		t = TY_LPCOBJ;
		break;
	case CV_PRIV:
		pvoid = (void*)&lpcObjPrivCV->data[Index]->LPPriv;
		t = TY_LPCOBJ;
		break;
	default:;
	}

	if(t != TY_LPCOBJ)
	{
		eda_get_float(CVGroup[Index], CVBlock[Index][_block_index], &_ft);
		switch( t)
		{
			case TY_BOOL:
				*(BOOL *)pvoid = (BOOL)_ft;
				*(BOOL *)pNewVal = (BOOL)_ft;
				break;
			case TY_DOUBLE:
				*(double *)pvoid = (double)_ft;
				*(double *)pNewVal = (double)_ft;
				break;
			case TY_WORD:
				*(WORD *)pvoid = (WORD)_ft;
				*(WORD *)pNewVal = (WORD)_ft;
				break;
			case TY_INT:
				*(int *)pvoid = (int)_ft;
				*(int *)pNewVal = (int)_ft;
				break;
			default: return FALSE;
		}
	}
	else
		*(LPCOBJ*)pNewVal = *(LPCOBJ*)pvoid;

	return pvoid;
}

DLLEXPORT void* ReadMVMember(LPCOBJ lpcObjMV , long Index , WORD PAR_ID, void* pNewVal )
{
	void*		pvoid = NULL;
	long		_block_index = MV_PAR_ACTU_NUM;
	TYPE_ID	t;
	LPCOBJPRIVMV	lpcObjPrivMV;
	float			_ft;
	lpcObjPrivMV = COBJ_2_PRIVMV( lpcObjMV );
	if( Index<0 || !HasInitialized )
		return FALSE;

	eda_read(MVGroup[Index]);
	eda_wait(MVGroup[Index]);
/*
	switch(PAR_ID)
	{
	case MV_VALUE:
		_block_index = MV_VALUE_INDEX;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MData;
		t=TY_DOUBLE;
		break;
	case MV_HIGH:
		_block_index = MV_HI_INDEX;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MIrvHigh;
		t=TY_DOUBLE;
		break;
	case MV_LOW:
		_block_index = MV_LO_INDEX;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MIrvLow;
		t=TY_DOUBLE;
		break;
	case MV_ALARMMAX:
		_block_index = MV_HIHI_INDEX;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MHAlarm;
		t=TY_DOUBLE;
		break;
	case MV_ALARMMIN:
		_block_index = MV_LOLO_INDEX;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MLAlarm;
		t=TY_DOUBLE;
		break;
	case MV_SAMPLE:
		_block_index = MV_SAMPLE_INDEX;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MSample;
		t=TY_DOUBLE;
		break;
	case MV_HSCALE:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MHScale;
			t = TY_DOUBLE;
		}		
	case MV_LSCALE:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MLScale;
			t = TY_DOUBLE;
		}		
	case MV_DELT:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MDelt;
			t = TY_DOUBLE;
		}		
	case MV_SET:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MIrv;
			t = TY_DOUBLE;
		}		
	case MV_INI:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MIni;
			t = TY_DOUBLE;
		}		
	case MV_DELMAX:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MDelmax;
			t = TY_DOUBLE;
		}		
	case MV_DELMIN:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MDelmin;
			t = TY_DOUBLE;
		}		
	case MV_MAX:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MMax;
			t = TY_DOUBLE;
		}		
	case MV_MIN:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MMin;
			t = TY_DOUBLE;
		}		
	case MV_FMAX:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MFault_max;
			t = TY_DOUBLE;
		}		
	case MV_FMIN:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MFault_min;
			t = TY_DOUBLE;
		}		
	case MV_FHIGH:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MIrvFault_high;
			t = TY_DOUBLE;
		}		
	case MV_FLOW:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MIrvFault_low;
			t = TY_DOUBLE;
		}		
	case MV_FXSET:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MFault_xirv;
			t = TY_DOUBLE;
		}		
	case MV_FNSET:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MFault_nirv;
			t = TY_DOUBLE;
		}		
	case MV_FHSET:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MFault_hirv;
			t = TY_DOUBLE;
		}		
	case MV_FLSET:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MFault_lirv;
			t = TY_DOUBLE;
		}		
	case MV_LPINI:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MLPIni;
			t = TY_DOUBLE;
		}		
	case MV_LPINDEX:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MLPIndex;
			t = TY_DOUBLE;
		}		
	case MV_LPOUT:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MSteadOptim;
			t = TY_DOUBLE;
		}		
	case MV_R:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MR;
			t = TY_DOUBLE;
		}		
	case MV_RBROKEN:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MRMax;
			t = TY_DOUBLE;
		}		
	case MV_S:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MP;
			t = TY_DOUBLE;
		}		
	case MV_SL:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MPL;
			t = TY_DOUBLE;
		}		
	case MV_SBROKEN:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MPMax;
			t = TY_DOUBLE;
		}		
	case MV_DELR:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MDelR;
			t = TY_DOUBLE;
		}		
	case MV_DELS:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MDelP;
			t = TY_DOUBLE;
		}		
	case MV_REALR:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MRealR;
			t = TY_DOUBLE;
		}		
	case MV_REALS:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MRealP;
			t = TY_DOUBLE;
		}		
	case MV_DISPLAYH:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MDisplayH;
			t = TY_DOUBLE;
		}		
	case MV_DISPLAYL:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MDisplayL;
			t = TY_DOUBLE;
		}		
	case MV_DISPLAY:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MDisplay;
			t = TY_DOUBLE;
		}		
	
	case MV_STEADYFIELD:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MSteadyField;
			t = TY_DOUBLE;
		}		
	case MV_MHISTORY:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MHistory;
			t = TY_DOUBLE;
		}		
	case MV_MLASTIRV:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MLastIRV;
			t = TY_DOUBLE;
		}		
	case MV_MLASTHIGH:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MLastHigh;
			t = TY_DOUBLE;
		}		
	case MV_MLASTLOW:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MLastLow;
			t = TY_DOUBLE;
		}		
	case MV_RCONTROLMODE:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MRControl_type;
			t = TY_WORD;
		}		
	case MV_PRIORITY:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MPriority;
			t = TY_WORD;
		}		
	case MV_HOVERCOUNT:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MHOverCount;
			t = TY_WORD;
		}		
	case MV_LOVERCOUNT:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MLOverCount;
			t = TY_WORD;
		}		
	case MV_CONTROLMODE:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MControl_type;
			t = TY_WORD;
		}		
	case MV_FPRIORITY:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MFault_Priority;
			t = TY_WORD;
		}		
	case MV_PDirtyCount:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MPDirtyCount;
			t = TY_WORD;
		}		
	case MV_RDirtyCount:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MRDirtyCount;
			t = TY_WORD;
		}		
	case MV_CTHistory:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MCT_history;
			t = TY_WORD;
		}		
	case MV_BAD:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MBad;
			t = TY_WORD;
		}		
	case MV_STATE:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MInvalidate;
			t = TY_BOOL;
		}		
	case MV_DIST:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MDist;
			t = TY_BOOL;
		}		
	case MV_SATUATION:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MSatuation;
			t = TY_BOOL;
		}		
	case MV_ALARM:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MAlarm;
			t = TY_BOOL;
		}		
	case MV_KEYMV:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MKey;
			t = TY_BOOL;
		}	
	case MV_MSETSATUATION:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MSetSatuation;
			t = TY_BOOL;
		}		
	case MV_IRVWRONG:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MIRVWrong;
			t = TY_BOOL;
		}		
	case MV_MAPARTAM:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MApartAM;
			t = TY_BOOL;
		}		
	case MV_AM:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MAM;
			t = TY_BOOL;
		}		
	case MV_HOPEAM:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MHope_AM;
			t = TY_BOOL;
		}		
	case MV_HEDGE:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MHEdge;
			t = TY_BOOL;
		}		
	case MV_LEDGE:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MLEdge;
			t = TY_BOOL;
		}		
	case MV_RDIRTY:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MRDirty;
			t = TY_BOOL;
		}		
	case MV_PDIRTY:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MPDirty;
			t = TY_BOOL;
		}		
	case MV_SDIRTY:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MSDirty;
			t = TY_BOOL;
		}		
	case MV_FLAGRBROKEN:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MRBroken;
			t = TY_BOOL;
		}		
	case MV_PBROKEN:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MPBroken;
			t = TY_BOOL;
		}		
	case MV_HOVER:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MHOver;
			t = TY_BOOL;
		}		
	case MV_LOVER:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MLOver;
			t = TY_BOOL;
		}		
	case MV_LASTCM:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MLastCM;
			t = TY_BOOL;
		}		
	case MV_RMODE:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MRCorrectMode;
			t = TY_BOOL;
		}		
	case MV_SMODE:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MSCorrectMode;
			t = TY_BOOL;
		}		
	case MV_ERRMODE:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MErrorMode;
			t = TY_BOOL;
		}		
	case MV_MCM:
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivMV->data[Index]->MCM;
			t = TY_BOOL;
		}	
		break;
	case MV_NAME:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->LPName;
		break;
	case MV_TAG:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->LPTag;
		break;
	case MV_UNIT:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->LPUnit;
		break;
	case MV_DATANAME:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->LPBaseName;
		break;
	case MV_PRIV:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->LPPriv;
		break;
	default:;
	}
*/
	switch(PAR_ID)
	{
	case MV_VALUE:
		_block_index = MV_VALUE_INDEX;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MData;
		t=TY_DOUBLE;
		break;
	case MV_HIGH:
		_block_index = MV_HI_INDEX;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MIrvHigh;
		t=TY_DOUBLE;
		break;
	case MV_LOW:
		_block_index = MV_LO_INDEX;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MIrvLow;
		t=TY_DOUBLE;
		break;
	case MV_ALARMMAX:
		_block_index = MV_HIHI_INDEX;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MHAlarm;
		t=TY_DOUBLE;
		break;
	case MV_ALARMMIN:
		_block_index = MV_LOLO_INDEX;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MLAlarm;
		t=TY_DOUBLE;
		break;
	case MV_SAMPLE:
		_block_index = MV_SAMPLE_INDEX;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MSample;
		t=TY_DOUBLE;
		break;
	case MV_HSCALE:
		_block_index = 6;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MHScale;
		t = TY_DOUBLE;
		break;
	case MV_LSCALE:
		_block_index = 7;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MLScale;
		t = TY_DOUBLE;
		break;		
	case MV_DELT:
		_block_index = 8;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MDelt;
		t = TY_DOUBLE;
		break;		
	case MV_SET:
		_block_index = 9;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MIrv;
		t = TY_DOUBLE;
		break;		
	case MV_INI:
		_block_index = 10;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MIni;
		t = TY_DOUBLE;
		break;		
	case MV_DELMAX:
		_block_index = 11;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MDelmax;
		t = TY_DOUBLE;
		break;		
	case MV_DELMIN:
		_block_index = 12;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MDelmin;
		t = TY_DOUBLE;
		break;		
	case MV_MAX:
		_block_index = 13;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MMax;
		t = TY_DOUBLE;
		break;		
	case MV_MIN:
		_block_index = 14;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MMin;
		t = TY_DOUBLE;
		break;		
	case MV_FMAX:
		_block_index = 15;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MFault_max;
		t = TY_DOUBLE;
		break;		
	case MV_FMIN:
		_block_index = 16;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MFault_min;
		t = TY_DOUBLE;
		break;		
	case MV_FHIGH:
		_block_index = 17;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MIrvFault_high;
		t = TY_DOUBLE;
		break;		
	case MV_FLOW:
		_block_index = 18;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MIrvFault_low;
		t = TY_DOUBLE;
		break;		
	case MV_FXSET:
		_block_index = 19;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MFault_xirv;
		t = TY_DOUBLE;
		break;		
	case MV_FNSET:
		_block_index = 20;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MFault_nirv;
		t = TY_DOUBLE;
		break;		
	case MV_FHSET:
		_block_index = 21;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MFault_hirv;
		t = TY_DOUBLE;
		break;		
	case MV_FLSET:
		_block_index = 22;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MFault_lirv;
		t = TY_DOUBLE;
		break;		
	case MV_LPINI:
		_block_index = 23;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MLPIni;
		t = TY_DOUBLE;
		break;		
	case MV_LPINDEX:
		_block_index = 24;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MLPIndex;
		t = TY_DOUBLE;
		break;		
	case MV_LPOUT:
		_block_index = 25;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MSteadOptim;
		t = TY_DOUBLE;
		break;		
	case MV_R:
		_block_index = 26;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MR;
		t = TY_DOUBLE;
		break;		
	case MV_RBROKEN:
		_block_index = 27;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MRMax;
		t = TY_DOUBLE;
		break;		
	case MV_S:
		_block_index = 28;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MP;
		t = TY_DOUBLE;
		break;		
	case MV_SL:
		_block_index = 29;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MPL;
		t = TY_DOUBLE;
		break;		
	case MV_SBROKEN:
		_block_index = 30;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MPMax;
		t = TY_DOUBLE;
		break;		
	case MV_DELR:
		_block_index = 31;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MDelR;
		t = TY_DOUBLE;
		break;		
	case MV_DELS:
		_block_index = 32;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MDelP;
		t = TY_DOUBLE;
		break;		
	case MV_REALR:
		_block_index = 33;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MRealR;
		t = TY_DOUBLE;
		break;		
	case MV_REALS:
		_block_index = 34;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MRealP;
		t = TY_DOUBLE;
		break;		
	case MV_DISPLAYH:
		_block_index = 35;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MDisplayH;
		t = TY_DOUBLE;
		break;		
	case MV_DISPLAYL:
		_block_index = 36;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MDisplayL;
		t = TY_DOUBLE;
		break;		
	case MV_DISPLAY:
		_block_index = 37;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MDisplay;
		t = TY_DOUBLE;
		break;		
	case MV_STEADYFIELD:
		_block_index = 38;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MSteadyField;
		t = TY_DOUBLE;
		break;		
	case MV_MHISTORY:
		_block_index = 39;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MHistory;
		t = TY_DOUBLE;
		break;		
	case MV_MLASTIRV:
		_block_index = 40;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MLastIRV;
		t = TY_DOUBLE;
		break;		
	case MV_MLASTHIGH:
		_block_index = 41;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MLastHigh;
		t = TY_DOUBLE;
		break;		
	case MV_MLASTLOW:
		_block_index = 42;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MLastLow;
		t = TY_DOUBLE;
		break;		
	case MV_RCONTROLMODE:
		_block_index = 43;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MRControl_type;
		t = TY_WORD;
		break;		
	case MV_PRIORITY:
		_block_index = 44;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MPriority;
		t = TY_WORD;
		break;		
	case MV_HOVERCOUNT:
		_block_index = 45;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MHOverCount;
		t = TY_WORD;
		break;		
	case MV_LOVERCOUNT:
		_block_index = 46;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MLOverCount;
		t = TY_WORD;
		break;		
	case MV_CONTROLMODE:
		_block_index = 47;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MControl_type;
		t = TY_WORD;
		break;		
	case MV_FPRIORITY:
		_block_index = 48;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MFault_Priority;
		t = TY_WORD;
		break;		
	case MV_PDirtyCount:
		_block_index = 49;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MPDirtyCount;
		t = TY_WORD;
		break;		
	case MV_RDirtyCount:
		_block_index = 50;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MRDirtyCount;
		t = TY_WORD;
		break;		
	case MV_CTHistory:
		_block_index = 51;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MCT_history;
		t = TY_WORD;
		break;		
	case MV_BAD:
		_block_index = 52;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MBad;
		t = TY_WORD;
		break;		
	case MV_STATE:
		_block_index = 53;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MInvalidate;
		t = TY_BOOL;
		break;		
	case MV_DIST:
		_block_index = 54;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MDist;
		t = TY_BOOL;
		break;		
	case MV_SATUATION:
		_block_index = 55;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MSatuation;
		t = TY_BOOL;
		break;		
	case MV_ALARM:
		_block_index = 56;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MAlarm;
		t = TY_BOOL;
		break;		
	case MV_KEYMV:
		_block_index = 57;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MKey;
		t = TY_BOOL;
		break;	
	case MV_MSETSATUATION:
		_block_index = 58;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MSetSatuation;
		t = TY_BOOL;
		break;		
	case MV_IRVWRONG:
		_block_index = 59;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MIRVWrong;
		t = TY_BOOL;
		break;		
	case MV_MAPARTAM:
		_block_index = 60;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MApartAM;
		t = TY_BOOL;
		break;		
	case MV_AM:
		_block_index = 61;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MAM;
		t = TY_BOOL;
		break;		
	case MV_HOPEAM:
		_block_index = 62;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MHope_AM;
		t = TY_BOOL;
		break;		
	case MV_HEDGE:
		_block_index = 63;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MHEdge;
		t = TY_BOOL;
		break;		
	case MV_LEDGE:
		_block_index = 64;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MLEdge;
		t = TY_BOOL;
		break;		
	case MV_RDIRTY:
		_block_index = 65;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MRDirty;
		t = TY_BOOL;
		break;		
	case MV_PDIRTY:
		_block_index = 66;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MPDirty;
		t = TY_BOOL;
		break;		
	case MV_SDIRTY:
		_block_index = 67;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MSDirty;
		t = TY_BOOL;
		break;		
	case MV_FLAGRBROKEN:
		_block_index = 68;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MRBroken;
		t = TY_BOOL;
		break;		
	case MV_PBROKEN:
		_block_index = 69;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MPBroken;
		t = TY_BOOL;
		break;		
	case MV_HOVER:
		_block_index = 70;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MHOver;
		t = TY_BOOL;
		break;		
	case MV_LOVER:
		_block_index = 71;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MLOver;
		t = TY_BOOL;
		break;		
	case MV_LASTCM:
		_block_index = 72;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MLastCM;
		t = TY_BOOL;
		break;		
	case MV_RMODE:
		_block_index = 73;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MRCorrectMode;
		t = TY_BOOL;
		break;		
	case MV_SMODE:
		_block_index = 74;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MSCorrectMode;
		t = TY_BOOL;
		break;		
	case MV_ERRMODE:
		_block_index = 75;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MErrorMode;
		t = TY_BOOL;
		break;		
	case MV_MCM:
		_block_index = 76;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MCM;
		t = TY_BOOL;
		break;
	case MV_NAME:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->LPName;
		//
		break;
	case MV_TAG:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->LPTag;
		//
		break;
	case MV_UNIT:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->LPUnit;
		//
		break;
	case MV_DATANAME:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->LPBaseName;
		//
		break;
	case MV_PRIV:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->LPPriv;
		//
		break;
	default:;
	}

	if(t != TY_LPCOBJ)
	{
		eda_get_float(MVGroup[Index], MVBlock[Index][_block_index], &_ft );
		switch( t)
		{
			case TY_BOOL:
				*(BOOL *)pvoid = (BOOL)_ft;
				*(BOOL *)pNewVal = (BOOL)_ft;
				break;
			case TY_DOUBLE:
				*(double *)pvoid = (double)_ft;
				*(double *)pNewVal = (double)_ft;
				break;
			case TY_WORD:
				*(WORD *)pvoid = (WORD)_ft;
				*(WORD *)pNewVal = (WORD)_ft;
				break;
			case TY_INT:
				*(int *)pvoid = (int)_ft;
				*(int *)pNewVal = (int)_ft;
				break;
			default: return FALSE;
		}
	}
	else
		*(LPCOBJ*)pNewVal = *(LPCOBJ*)pvoid;

	return pvoid;
}

DLLEXPORT void* ReadDVMember(LPCOBJ lpcObjDV , long Index , WORD PAR_ID, void* pNewVal )
{
	long	_block_index = DV_PAR_ACTU_NUM;
	void* pvoid = NULL;
	float	_ft;

	TYPE_ID	t;
	LPCOBJPRIVDV	lpcObjPrivDV;
	lpcObjPrivDV = COBJ_2_PRIVDV( lpcObjDV );

	if( Index<0 || !HasInitialized )
		return FALSE;

	eda_read(DVGroup[Index]);
	eda_wait(DVGroup[Index]);
/*
	switch(PAR_ID)
	{
	case DV_VALUE:
		_block_index = DV_DATA_INDEX;
		pvoid  = (void*)&lpcObjPrivDV->data[Index]->DData;
		t=TY_DOUBLE;
		break;
	case DV_HSCALE:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivDV->data[Index]->DHScale;
			t = TY_DOUBLE;
		}		
	case DV_LSCALE:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivDV->data[Index]->DLScale;
			t = TY_DOUBLE;
		}		
	case DV_INI:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivDV->data[Index]->DIni;
			t = TY_DOUBLE;
		}		
	case DV_DELT:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivDV->data[Index]->DDelt;
			t = TY_DOUBLE;
		}		
	case DV_HISTORY:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivDV->data[Index]->DHistory;
			t = TY_DOUBLE;
		}		
	case DV_DISPLAYH:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivDV->data[Index]->DDisplayH;
			t = TY_DOUBLE;
		}		
	case DV_DISPLAYL:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivDV->data[Index]->DDisplayL;
			t = TY_DOUBLE;
		}		
	case DV_DISPLAY:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivDV->data[Index]->DDisplay;
			t = TY_DOUBLE;
		}		
	case DV_STEADYFIELD:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivDV->data[Index]->DSteadyField;
			t = TY_DOUBLE;
		}		
	case DV_STATE:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivDV->data[Index]->DInvalid;
			t = TY_BOOL;
		}		
	case DV_ALARM:
		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivDV->data[Index]->DAlarm;
			t = TY_BOOL;
		}		
	case DV_BAD:
//		-- _block_index;
		if(pvoid  == NULL)
		{
			pvoid  = (void*)&lpcObjPrivDV->data[Index]->DBad;
			t = TY_WORD;
		}		
		break;
	case DV_NAME:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivDV->data[Index]->LPName;
		break;
	case DV_TAG:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivDV->data[Index]->LPTag;
		break;
	case DV_UNIT:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivDV->data[Index]->LPUnit;
		break;
	case DV_DATANAME:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivDV->data[Index]->LPBaseName;
		break;
	case DV_PRIV:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivDV->data[Index]->LPPriv;
		break;
	default:;
	}
*/
	switch(PAR_ID)
	{
	case DV_VALUE:
		_block_index = DV_DATA_INDEX;
		pvoid  = (void*)&lpcObjPrivDV->data[Index]->DData;
		t=TY_DOUBLE;
		break;
	case DV_HSCALE:
		_block_index = 1;
		pvoid  = (void*)&lpcObjPrivDV->data[Index]->DHScale;
		t = TY_DOUBLE;
		break;
	case DV_LSCALE:
		_block_index = 2;
		pvoid  = (void*)&lpcObjPrivDV->data[Index]->DLScale;
		t = TY_DOUBLE;
		break;
	case DV_INI:
		_block_index = 3;
		pvoid  = (void*)&lpcObjPrivDV->data[Index]->DIni;
		t = TY_DOUBLE;
		break;
	case DV_DELT:
		_block_index = 4;
		pvoid  = (void*)&lpcObjPrivDV->data[Index]->DDelt;
		t = TY_DOUBLE;
		break;
	case DV_HISTORY:
		_block_index = 5;
		pvoid  = (void*)&lpcObjPrivDV->data[Index]->DHistory;
		t = TY_DOUBLE;
		break;		
	case DV_DISPLAYH:
		_block_index = 6;
		pvoid  = (void*)&lpcObjPrivDV->data[Index]->DDisplayH;
		t = TY_DOUBLE;
		break;		
	case DV_DISPLAYL:
		_block_index = 7;
		pvoid  = (void*)&lpcObjPrivDV->data[Index]->DDisplayL;
		t = TY_DOUBLE;
		break;		
	case DV_DISPLAY:
		_block_index = 8;
		pvoid  = (void*)&lpcObjPrivDV->data[Index]->DDisplay;
		t = TY_DOUBLE;
		break;		
	case DV_STEADYFIELD:
		_block_index = 9;
		pvoid  = (void*)&lpcObjPrivDV->data[Index]->DSteadyField;
		t = TY_DOUBLE;
		break;		
	case DV_STATE:
		_block_index = 10;
		pvoid  = (void*)&lpcObjPrivDV->data[Index]->DInvalid;
		t = TY_BOOL;
		break;		
	case DV_ALARM:
		_block_index = 11;
		pvoid  = (void*)&lpcObjPrivDV->data[Index]->DAlarm;
		t = TY_BOOL;
		break;		
	case DV_BAD:
		_block_index = 12;
		pvoid  = (void*)&lpcObjPrivDV->data[Index]->DBad;
		t = TY_WORD;
		break;		
	case DV_NAME:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivDV->data[Index]->LPName;
		break;
	case DV_TAG:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivDV->data[Index]->LPTag;
		break;
	case DV_UNIT:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivDV->data[Index]->LPUnit;
		break;
	case DV_DATANAME:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivDV->data[Index]->LPBaseName;
		break;
	case DV_PRIV:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivDV->data[Index]->LPPriv;
		break;
	default:;
	}

	if( t != TY_LPCOBJ)
	{
		eda_get_float(DVGroup[Index], DVBlock[Index][_block_index], &_ft);
		switch( t)
		{
			case TY_BOOL:
				*(BOOL *)pvoid = (BOOL)_ft;
				*(BOOL *)pNewVal = (BOOL)_ft;
				break;
			case TY_DOUBLE:
				*(double *)pvoid = (double)_ft;
				*(double *)pNewVal = (double)_ft;
				break;
			case TY_WORD:
				*(WORD *)pvoid = (WORD)_ft;
				*(WORD *)pNewVal = (WORD)_ft;
				break;
			case TY_INT:
				*(int *)pvoid = (int)_ft;
				*(int *)pNewVal = (int)_ft;
				break;
			default: return FALSE;
		}
	}
	else
		*(LPCOBJ*)pNewVal = *(LPCOBJ*)pvoid;
	return pvoid;

}

DLLEXPORT void* ReadGVMember(LPCOBJPRIV lpcObjPrivGV , WORD PAR_ID, void* pNewVal )
{
	long	_block_index = GV_PAR_ACTU_NUM;
	void*	pvoid = NULL;
	long	Index = 0;
	float	_ft;
	TYPE_ID	t;
	if( Index<0 || !HasInitialized )
		return FALSE;

	eda_read(GVGroup[Index]);
	eda_wait(GVGroup[Index]);

	switch(PAR_ID)
	{
	case MDMCIRVD_FILENO:
		_block_index = 0;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->File_No;
		t = TY_WORD;
		break;
	case MDMCIRVD_N:
		_block_index = 1;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->N;
		t = TY_WORD;
		break;
	case MDMCIRVD_P:
		_block_index = 2;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->P;
		t = TY_WORD;
		break;
	case MDMCIRVD_M:
		_block_index = 3;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->M;
		t = TY_WORD;
		break;
	case MDMCIRVD_p:
		_block_index = 4;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->p;
		t = TY_WORD;
		break;
	case MDMCIRVD_m:
		_block_index = 5;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->m;
		t = TY_WORD;
		break;
	case MDMCIRVD_nd:
		_block_index = 6;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->nd;
		t = TY_WORD;
		break;
	case MDMCIRVD_count:
		_block_index = 7;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->Count;
		t = TY_WORD;
		break;
	case MDMCIRVD_MAKEPARACOUNT:
		_block_index = 8;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->MakeParaCount;
		t = TY_WORD;
		break;
	case MDMCIRVD_RECOUNTCOUNT:
		_block_index = 9;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->RecountCount;
		t = TY_WORD;
		break;
	case MDMCIRVD_LPCOUNT:
		_block_index = 10;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->LPCount;
		t = TY_WORD;
		break;
	case MDMCIRVD_NOAPART:
		_block_index = 11;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->No_Apart;
		t = TY_WORD;
		break;
	case MDMCIRVD_MPARACOUNT:
		_block_index = 12;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->MParaCount;
		t = TY_WORD;
		break;
	case MDMCIRVD_MPARAMAX:
		_block_index = 13;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->MParaMax;
		t = TY_WORD;
		break;
	case MDMCIRVD_STEADYSTEP:
		_block_index = 14;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->SteadyStep;
		t = TY_WORD;
		break;
	case MDMCIRVD_ERRORCODE:
		_block_index = 15;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->ErrorCode;
		t = TY_WORD;
		break;
	case MDMCIRVD_FLAGAM:
		_block_index = 16;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_AM;
		t = TY_WORD;
		break;
	case MDMCIRVD_FREECOUNT:
		_block_index = 17;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->Freecount;
		t = TY_INT;
		break;
	case MDMCIRVD_MAXDELAY:
		_block_index = 18;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->MaxDelay;
		t = TY_WORD;
		break;
	case MDMCIRVD_Modu:
		_block_index = 19;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->wModu;
		t = TY_WORD;
		break;
	case MDMCIRVD_N1:
		_block_index = 20;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->N1;
		t = TY_WORD;
		break;
	case MDMCIRVD_N2:
		_block_index = 21;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->N2;
		t = TY_WORD;
		break;
	case MDMCIRVD_STEADYCOUNT:
		_block_index = 22;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->SteadyCount;
		t = TY_WORD;
		break;
	case MDMCIRVD_CALSTEADYMAX:
		_block_index = 23;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->CalSteadyMax;
		t = TY_WORD;
		break;
	case MDMCIRVD_APARTI:
		_block_index = 24;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->Apart_i;
		t = TY_WORD;
		break;
	case MDMCIRVD_BADMAX:
		_block_index = 25;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->BadMax;
		t = TY_WORD;
		break;
	case MDMCIRVD_CCLASSIF:
		_block_index = 26;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_CControl;
		t = TY_BOOL;
		break;
	case MDMCIRVD_MCLASSIF:
		_block_index = 27;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_MControl;
		t = TY_BOOL;
		break;
	case MDMCIRVD_FLAGFAULT:
		_block_index = 28;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_Fault;
		t = TY_BOOL;
		break;
	case MDMCIRVD_FLAGLP:
		_block_index = 29;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_LP;
		t = TY_BOOL;
		break;
	case MDMCIRVD_FLAGALARM:
		_block_index = 30;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_Alarm;
		t = TY_BOOL;
		break;
	case MDMCIRVD_FLAGRESP:
		_block_index = 31;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_ReSP;
		t = TY_BOOL;
		break;
	case MDMCIRVD_SRMODE:
		_block_index = 32;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_SRMode;
		t = TY_BOOL;
		break;
	case MDMCIRVD_FLAGMCHANGE:
		_block_index = 33;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_MChange;
		t = TY_BOOL;
		break;
	case MDMCIRVD_FLAGLPCHANGE:
		_block_index = 34;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_LPChange;
		t = TY_BOOL;
		break;
	case MDMCIRVD_FLAGHOPEAM:
		_block_index = 35;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_HopeAM;
		t = TY_BOOL;
		break;
	case MDMCIRVD_FLAGRECOUNT:
		_block_index = 36;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_Recount;
		t = TY_BOOL;
		break;
	case MDMCIRVD_FLAGLPRECOUNT:
		_block_index = 37;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_LPRecount;
		t = TY_BOOL;
		break;
	case MDMCIRVD_STARTMODE:
		_block_index = 38;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->StartMode;
		t = TY_BOOL;
		break;
	case MDMCIRVD_FLAGWEIGHT:
		_block_index = 39;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_Weight;
		t = TY_BOOL;
		break;
	case MDMCIRVD_FLAGDEBUG:
		_block_index = 40;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_Debug;
		t = TY_BOOL;
		break;
	case MDMCIRVD_FLAGHMODE:
		_block_index = 41;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_HMode;
		t = TY_BOOL;
		break;
	case MDMCIRVD_FLAGAM1:
		_block_index = 42;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_AM1;
		t = TY_BOOL;
		break;
	case MDMCIRVD_FLAGAM2:
		_block_index = 43;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_AM2;
		t = TY_BOOL;
		break;
	case MDMCIRVD_FLAGAM3:
		_block_index = 44;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_AM3;
		t = TY_BOOL;
		break;
	case MDMCIRVD_FLAGRUN:
		_block_index = 45;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_Run;
		t = TY_BOOL;
		break;
	case MDMCIRVD_STEADY:
		_block_index = 46;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_Steady;
		t = TY_BOOL;
		break;
	case MDMCIRVD_MCVRECOUNT:
		_block_index = 47;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_MCVRecount;
		t = TY_BOOL;
		break;
	case MDMCIRVD_FLAGRDEBUG:
		_block_index = 48;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_RDebug;
		t = TY_BOOL;
		break;
	case MDMCIRVD_Save:
		_block_index = 49;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_Save;
		t = TY_BOOL;
		break;
	case MDMCIRVD_INISTART:
		_block_index = 50;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->IniStart;
		t = TY_BOOL;
		break;
	case MDMCIRVD_FLAGCORSTEADY:
		_block_index = 51;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_CorSteady;
		t = TY_BOOL;
		break;
	case MDMCIRVD_CALSTEADY:
		_block_index = 52;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->CalSteady;
		t = TY_BOOL;
		break;
	case MDMCIRVD_ERROR:
		_block_index = 53;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->Error;
		t = TY_BOOL;
		break;
	case MDMCIRVD_LPK:
		_block_index = 55;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->LpK;
		t = TY_DOUBLE;
		break;
	case MDMCIRVD_alpha:
		_block_index = 55;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->Alpha;
		t = TY_DOUBLE;
		break;
	case MDMCIRVD_BIGCOND:
		_block_index = 56;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->Big_Cond;
		t = TY_DOUBLE;
		break;
	case MDMCIRVD_PBROKEN:
		_block_index = 57;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->PBroken;
		t = TY_DOUBLE;
		break;
	case MDMCIRVD_QBROKEN:
		_block_index = 58;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->QBroken;
		t = TY_DOUBLE;
		break;
	case MDMCIRVD_RBROKEN:
		_block_index = 59;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->RBroken;
		t = TY_DOUBLE;
		break;
	case MDMCIRVD_SRERROR:
		_block_index = 60;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->SRError;
		t = TY_DOUBLE;
		break;
	case MDMCIRVD_DSRERROR:
		_block_index = 61;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->DSRError;
		t = TY_DOUBLE;
		break;
	case MDMCIRVD_CORSTEADY:
		_block_index = 62;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->Cor_Steady;
		t = TY_DOUBLE;
		break;
	case MDMCIRVD_SAMPLETIME:
		_block_index = 63;//-- _block_index;
		pvoid  = (void*)&lpcObjPrivGV->SampleTime;
		t = TY_DOUBLE;
		break;
	case MDMCIRVD_DFILTER:
		_block_index = 64;
		t = TY_DOUBLE;
		pvoid  = (void*)&lpcObjPrivGV->DFilter;
		break;
	case MDMCIRVD_mG:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPG;
		break;
	case MDMCIRVD_mA:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPA;
		break;
	case MDMCIRVD_mAb:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPAb;
		break;
	case MDMCIRVD_mmul2:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPMul2;
		break;
	case MDMCIRVD_mmul3:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPMul3;
		break;
	case MDMCIRVD_mSR:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPSR;
		break;
	case MDMCIRVD_mDSR:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPDSR;
		break;
	case MDMCIRVD_mTempSR:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPTempSR;
		break;
	case MDMCIRVD_mTempDSR:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPTempDSR;
		break;
	case MDMCIRVD_mypredN:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPPredN;
		break;
	case MDMCIRVD_mH:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPH;
		break;
	case MDMCIRVD_MV:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPMV;
		break;
	case MDMCIRVD_CV:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPCV;
		break;
	case MDMCIRVD_DV:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPDV;
		break;
	case MDMCIRVD_mP:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPS;
		break;
	case MDMCIRVD_mQ:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPQ;
		break;
	case MDMCIRVD_mR:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPR;
		break;
	case MDMCIRVD_LPPRIV:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPLPPriv;
		break;
	case MDMCIRVD_HISTORY:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPHistory;
		break;
	case MDMCIRVD_CVSort:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPCVSort;
		break;
	case MDMCIRVD_MVSort:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPMVSort;
		break;
	case MDMCIRVD_LPDeluM:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPDeluM;
		break;
	case MDMCIRVD_YUHis:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPYUHis;
		break;
	case MDMCIRVD_YDHis:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPYDHis;
		break;
	case MDMCIRVD_ApartMV:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPApartMV;
		break;
	case MDMCIRVD_ApartCV:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPApartCV;
		break;
	case MDMCIRVD_ApartDV:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPApartDV;
		break;
	case MDMCIRVD_Apart:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPApart;
		break;
	case MDMCIRVD_OUTSR:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPOutSR;
		break;
	case MDMCIRVD_OUTDSR:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPOutDSR;
		break;
	case MDMCIRVD_LPCOBJGLOBAL:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPGlobal;
		break;
	case MDMCIRVD_INMV:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPINMV;
		break;
	case MDMCIRVD_INCV:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPINCV;
		break;
	case MDMCIRVD_INDV:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPINDV;
		break;
	case MDMCIRVD_LPPredU:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPPredU;
		break;
	case MDMCIRVD_LPPredD:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPPredD;
		break;
	case MDMCIRVD_LPISSTEADY:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPIsSteady;
		break;
	case MDMCIRVD_LPSTEADY:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPSteady;
		break;
	default:;
	}

	if(t != TY_LPCOBJ)
	{
		eda_get_float(GVGroup[Index], GVBlock[Index][_block_index], &_ft);
		switch( t)
		{
			case TY_BOOL:
				*(BOOL *)pvoid = (BOOL)_ft;
				*(BOOL *)pNewVal = (BOOL)_ft;
				break;
			case TY_DOUBLE:
				*(double *)pvoid = (double)_ft;
				*(double *)pNewVal = (double)_ft;
				break;
			case TY_WORD:
				*(WORD *)pvoid = (WORD)_ft;
				*(WORD *)pNewVal = (WORD)_ft;
				break;
			case TY_INT:
				*(int *)pvoid = (int)_ft;
				*(int *)pNewVal = (int)_ft;
				break;
			default: return FALSE;
		}
	}
	else
	{
		*(LPCOBJ*)pNewVal = *(LPCOBJ*)pvoid;
	}
	return pvoid;
}

DLLEXPORT void*  WriteCVMember(void* lpCV , long Index , WORD PAR_ID, int _block_index,TYPE_ID type_val_ )
{
	float _fval;

	switch( type_val_ )
	{
	case TY_WORD:
		_fval = (float)*(WORD* )lpCV;
		break;
	case TY_DOUBLE:
		_fval = (float)*(DOUBLE* )lpCV;
		break;
	case TY_INT:
		_fval = (float)*(INT* )lpCV;
		break;
	case TY_BOOL:
		_fval = (float)*(BOOL* )lpCV;
		break;
	default:
		return NULL;
	}
	eda_set_float(CVGroup[Index], CVBlock[Index][_block_index], &_fval);
	eda_write(CVGroup[Index]);
	return NULL;
}

DLLEXPORT void*  GetLP_CVMember(LPCOBJ lpcObjCV , long Index , WORD PAR_ID,int * _pblock_index, TYPE_ID* type_val_ )
{
	void * pvoid = NULL;
	*_pblock_index = MAX_CV_PAR_NUM - 1;

	TYPE_ID	t;
	LPCOBJPRIVCV	lpcObjPrivCV;
	lpcObjPrivCV = COBJ_2_PRIVCV( lpcObjCV);

	if( Index<0 || !HasInitialized )
		return pvoid;

	switch(PAR_ID)
	{
	case CV_VALUE:		
		*_pblock_index = CV_DATA_INDEX;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CData;
		t=TY_DOUBLE;
		break;
	case CV_HIGH:
		*_pblock_index = CV_HI_INDEX;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CHigh;
		t=TY_DOUBLE;
		break;
	case CV_LOW:
		*_pblock_index = CV_LO_INDEX;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CLow;
		t=TY_DOUBLE;
		break;
	case CV_ALARMMAX:
		*_pblock_index = CV_HIHI_INDEX;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CHAlarm;
		t=TY_DOUBLE;
		break;
	case CV_ALARMMIN:
		*_pblock_index = CV_LOLO_INDEX;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CLAlarm;
		t=TY_DOUBLE;
		break;
	case CV_HSCALE:		
		* _pblock_index = 5;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CHScale;
		t = TY_DOUBLE;
		break;
	case CV_LSCALE:
		* _pblock_index = 6;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CLScale;
		t = TY_DOUBLE;
		break;
	case CV_SET:
		* _pblock_index = 7;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CSetpoint;
		t = TY_DOUBLE;
		break;
	case CV_INI:
		* _pblock_index = 8;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CIni;
		t = TY_DOUBLE;
		break;
	case CV_MAX:
		* _pblock_index = 9;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CMax;
		t = TY_DOUBLE;
		break;
	case CV_MIN:
		* _pblock_index = 10;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CMin;
		t = TY_DOUBLE;
		break;
	case CV_FMAX:
		* _pblock_index = 11;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CFault_max;
		t = TY_DOUBLE;
		break;
	case CV_FMIN:
		* _pblock_index = 12;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CFault_min;
		t = TY_DOUBLE;
		break;
	case CV_FHIGH:
		* _pblock_index = 13;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CFault_high;
		t = TY_DOUBLE;
		break;
	case CV_FLOW:
		* _pblock_index = 14;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CFault_min;
		t = TY_DOUBLE;
		break;
	case CV_FXSET:
		* _pblock_index = 15;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CFault_xsetpoint;
		t = TY_DOUBLE;
		break;
	case CV_FNSET:
		* _pblock_index = 16;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CFault_nsetpoint;
		t = TY_DOUBLE;
		break;
	case CV_FHSET:
		* _pblock_index = 17;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CFault_hsetpoint;
		t = TY_DOUBLE;
		break;
	case CV_FLSET:
		* _pblock_index = 18;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CFault_lsetpoint;
		t = TY_DOUBLE;
		break;
	case CV_LPINI:
		* _pblock_index = 19;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CLPIni;
		t = TY_DOUBLE;
		break;
	case CV_LPINDEX:
		* _pblock_index = 20;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CLPIndex;
		t = TY_DOUBLE;
		break;
	case CV_LPOUT:
		* _pblock_index = 21;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CSteadOptim;
		t = TY_DOUBLE;
		break;
	case CV_Q:
		* _pblock_index = 22;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CQ;
		t = TY_DOUBLE;
		break;
	case CV_QL:
		* _pblock_index = 23;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CQL;
		t = TY_DOUBLE;
		break;
	case CV_QBROKEN:
		* _pblock_index = 24;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CQMax;
		t = TY_DOUBLE;
		break;
	case CV_DELQ:
		* _pblock_index = 25;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CDelQ;
		t = TY_DOUBLE;
		break;
	case CV_REALQ:
		* _pblock_index = 26;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CRealQ;
		t = TY_DOUBLE;
		break;
	case CV_ERROR:
		* _pblock_index = 27;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CError;
		t = TY_DOUBLE;
		break;
	case CV_DISPLAYH:
		* _pblock_index = 28;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CDisplayH;
		t = TY_DOUBLE;
		break;
	case CV_DISPLAYL:
		* _pblock_index = 29;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CDisplayL;
		t = TY_DOUBLE;
		break;
	case CV_DISPLAY:
		* _pblock_index = 30;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CDisplay;
		t = TY_DOUBLE;
		break;
	case CV_STEADYDELT:
		* _pblock_index = 31;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CSteadyDelt;
		t = TY_DOUBLE;
		break;
	case CV_STEADYFIELD:
		* _pblock_index = 32;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CSteadyField;
		t = TY_DOUBLE;
		break;
	case CV_LASTSET:
		* _pblock_index = 33;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CLastSet;
		t = TY_DOUBLE;
		break;
	case CV_LASTHIGH:
		* _pblock_index = 34;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CLastHigh;
		t = TY_DOUBLE;
		break;
	case CV_LASTLOW:
		* _pblock_index = 35;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CLastLow;
		t = TY_DOUBLE;
		break;
	case CV_INIERROR:
		* _pblock_index = 36;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CIniError;
		t = TY_DOUBLE;
		break;
	case CV_LFAULT:
		* _pblock_index = 37;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CLowFault;
		t = TY_DOUBLE;
		break;
	case CV_HFAULT:
		* _pblock_index = 38;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CHighFault;
		t = TY_DOUBLE;
		break;
	case CV_LSET:
		* _pblock_index = 39;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CLowSet;
		t = TY_DOUBLE;
		break;
	case CV_HSET:
		* _pblock_index = 40;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CHighSet;
		t = TY_DOUBLE;
		break;
	case CV_RCONTROLMODE:
		* _pblock_index = 41;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CRControl_type;
		t = TY_WORD;
		break;
	case CV_PRIORITY:
		* _pblock_index = 42;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CPriority;
		t = TY_WORD;
		break;
	case CV_NOLP:
		* _pblock_index = 43;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CNoLP;
		t = TY_WORD;
		break;
	case CV_HOVERCOUNT:
		* _pblock_index = 44;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CHOverCount;
		t = TY_WORD;
		break;
	case CV_LOVERCOUNT:
		* _pblock_index = 45;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CLOverCount;
		t = TY_DOUBLE;
		break;
	case CV_CONTROLMODE:
		* _pblock_index = 46;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CControl_type;
		t = TY_WORD;
		break;
	case CV_FPRIORITY:
		* _pblock_index = 47;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CFault_Priority;
		t = TY_WORD;
		break;
	case CV_CTHISTORY:
		* _pblock_index = 48;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CCT_history;
		t = TY_WORD;
		break;
	case CV_DIRTY:
		* _pblock_index = 49;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CDirty;
		t = TY_WORD;
		break;
	case CV_DIRTYCOUNT:
		* _pblock_index = 50;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CQDirtyCount;
		t = TY_WORD;
		break;
	case CV_DELAYCOUNT:
		* _pblock_index = 51;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CDelayCount;
		t = TY_WORD;
		break;
	case CV_BAD:
		* _pblock_index = 52;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CBad;
		t = TY_WORD;
		break;
	case CV_N:
		* _pblock_index = 53;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CN;
		t = TY_WORD;
		break;
	case CV_STATE:
		* _pblock_index = 54;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CInvalid;
		t = TY_BOOL;
		break;
	case CV_ALARM:
		* _pblock_index = 55;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CAlarm;
		t = TY_BOOL;
		break;
	case CV_KEYCV:
		* _pblock_index = 56;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CKey;
		t = TY_BOOL;
		break;
	case CV_HOVER:
		* _pblock_index = 57;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CHOver;
		t = TY_BOOL;
		break;
	case CV_LOVER:
		* _pblock_index = 58;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CLOver;
		t = TY_BOOL;
		break;
	case CV_LPWRONG:
		* _pblock_index = 59;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CLPWrong;
		t = TY_BOOL;
		break;
	case CV_NOILL:
		* _pblock_index = 60;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CNoIll;
		t = TY_BOOL;
		break;
	case CV_HEDGE:
		* _pblock_index = 61;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CHEdge;
		t = TY_BOOL;
		break;
	case CV_LEDGE:
		* _pblock_index = 62;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CLEdge;
		t = TY_BOOL;
		break;
	case CV_QMODE:
		* _pblock_index = 63;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CQMode;
		t = TY_BOOL;
		break;
	case CV_ERRMODE:
		* _pblock_index = 64;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CErrorMode;
		t = TY_BOOL;
		break;
	case CV_LIMIT:
		* _pblock_index = 65;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CLimit;
		t = TY_BOOL;
		break;
	case CV_BROKEN:
		* _pblock_index = 66;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CQBroken;
		t = TY_BOOL;
		break;
	case CV_SDIRTY:
		* _pblock_index = 67;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CSDirty;
		t = TY_BOOL;
		break;
	case CV_LASTCM:
		* _pblock_index = 68;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CLastCM;
		t = TY_BOOL;
		break;
	case CV_CM:
		* _pblock_index = 69;
		pvoid = (void*)&lpcObjPrivCV->data[Index]->CCM;
		t = TY_BOOL;
		break;
	case CV_NAME:
		pvoid = (void*)&lpcObjPrivCV->data[Index]->LPName;
		t = TY_LPCOBJ;
		
		break;
	case CV_TAG:
		pvoid = (void*)&lpcObjPrivCV->data[Index]->LPTag;
		t = TY_LPCOBJ;
		
		break;
	case CV_UNIT:
		pvoid = (void*)&lpcObjPrivCV->data[Index]->LPUnit;
		t = TY_LPCOBJ;
		
		break;
	case CV_DATANAME:
		pvoid = (void*)&lpcObjPrivCV->data[Index]->LPBaseName;
		t = TY_LPCOBJ;
		
		break;
	case CV_PRIV:
		pvoid = (void*)&lpcObjPrivCV->data[Index]->LPPriv;
		t = TY_LPCOBJ;
		
		break;
	default:;
	}
	(*type_val_) = t;
	return pvoid;
}

DLLEXPORT void* WriteMVMember(void* lpMV , long Index , WORD PAR_ID ,int _block_index ,TYPE_ID type_val_ )
{
	float _fval;

	switch( type_val_ )
	{
	case TY_WORD:
		_fval = (float)*(WORD* )lpMV;
		break;
	case TY_DOUBLE:
		_fval = (float)*(DOUBLE* )lpMV;
		break;
	case TY_INT:
		_fval = (float)*(INT* )lpMV;
		break;
	case TY_BOOL:
		_fval = (float)*(BOOL* )lpMV;
		break;
	default:
		return NULL;
	}
	eda_set_float(MVGroup[Index], MVBlock[Index][_block_index], &_fval);
	eda_write1(MVGroup[Index],MVBlock[Index][_block_index]);
	return NULL;
}

DLLEXPORT void* GetLP_MVMember(LPCOBJ lpcObjMV , long Index , WORD PAR_ID,int * _pblock_index,TYPE_ID* type_val_ )
{
	
	void * pvoid = NULL;
	*_pblock_index = MAX_MV_PAR_NUM-1;
	TYPE_ID	t;
	
	LPCOBJPRIVMV lpcObjPrivMV;
	lpcObjPrivMV = COBJ_2_PRIVMV(lpcObjMV);


	if( Index<0 || !HasInitialized )
		return pvoid;

	switch(PAR_ID)
	{
	case MV_VALUE:
		* _pblock_index = MV_VALUE_INDEX;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MData;
		t=TY_DOUBLE;
		break;
	case MV_HIGH:
		* _pblock_index = MV_HI_INDEX;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MIrvHigh;
		t=TY_DOUBLE;
		break;
	case MV_LOW:
		* _pblock_index = MV_LO_INDEX;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MIrvLow;
		t=TY_DOUBLE;
		break;
	case MV_ALARMMAX:
		* _pblock_index = MV_HIHI_INDEX;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MHAlarm;
		t=TY_DOUBLE;
		break;
	case MV_ALARMMIN:
		* _pblock_index = MV_LOLO_INDEX;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MLAlarm;
		t=TY_DOUBLE;
		break;
	case MV_SAMPLE:
		* _pblock_index = MV_SAMPLE_INDEX;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MSample;
		t=TY_DOUBLE;
		break;
	case MV_HSCALE:
		* _pblock_index = 6;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MHScale;
		t = TY_DOUBLE;
		break;
	case MV_LSCALE:
		* _pblock_index = 7;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MLScale;
		t = TY_DOUBLE;
		break;		
	case MV_DELT:
		* _pblock_index = 8;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MDelt;
		t = TY_DOUBLE;
		break;		
	case MV_SET:
		* _pblock_index = 9;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MIrv;
		t = TY_DOUBLE;
		break;		
	case MV_INI:
		* _pblock_index = 10;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MIni;
		t = TY_DOUBLE;
		break;		
	case MV_DELMAX:
		* _pblock_index = 11;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MDelmax;
		t = TY_DOUBLE;
		break;		
	case MV_DELMIN:
		* _pblock_index = 12;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MDelmin;
		t = TY_DOUBLE;
		break;		
	case MV_MAX:
		* _pblock_index = 13;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MMax;
		t = TY_DOUBLE;
		break;		
	case MV_MIN:
		* _pblock_index = 14;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MMin;
		t = TY_DOUBLE;
		break;		
	case MV_FMAX:
		* _pblock_index = 15;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MFault_max;
		t = TY_DOUBLE;
		break;		
	case MV_FMIN:
		* _pblock_index = 16;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MFault_min;
		t = TY_DOUBLE;
		break;		
	case MV_FHIGH:
		* _pblock_index = 17;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MIrvFault_high;
		t = TY_DOUBLE;
		break;		
	case MV_FLOW:
		* _pblock_index = 18;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MIrvFault_low;
		t = TY_DOUBLE;
		break;		
	case MV_FXSET:
		* _pblock_index = 19;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MFault_xirv;
		t = TY_DOUBLE;
		break;		
	case MV_FNSET:
		* _pblock_index = 20;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MFault_nirv;
		t = TY_DOUBLE;
		break;		
	case MV_FHSET:
		* _pblock_index = 21;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MFault_hirv;
		t = TY_DOUBLE;
		break;		
	case MV_FLSET:
		* _pblock_index = 22;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MFault_lirv;
		t = TY_DOUBLE;
		break;		
	case MV_LPINI:
		* _pblock_index = 23;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MLPIni;
		t = TY_DOUBLE;
		break;		
	case MV_LPINDEX:
		* _pblock_index = 24;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MLPIndex;
		t = TY_DOUBLE;
		break;		
	case MV_LPOUT:
		* _pblock_index = 25;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MSteadOptim;
		t = TY_DOUBLE;
		break;		
	case MV_R:
		* _pblock_index = 26;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MR;
		t = TY_DOUBLE;
		break;		
	case MV_RBROKEN:
		* _pblock_index = 27;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MRMax;
		t = TY_DOUBLE;
		break;		
	case MV_S:
		* _pblock_index = 28;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MP;
		t = TY_DOUBLE;
		break;		
	case MV_SL:
		* _pblock_index = 29;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MPL;
		t = TY_DOUBLE;
		break;		
	case MV_SBROKEN:
		* _pblock_index = 30;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MPMax;
		t = TY_DOUBLE;
		break;		
	case MV_DELR:
		* _pblock_index = 31;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MDelR;
		t = TY_DOUBLE;
		break;		
	case MV_DELS:
		* _pblock_index = 32;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MDelP;
		t = TY_DOUBLE;
		break;		
	case MV_REALR:
		* _pblock_index = 33;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MRealR;
		t = TY_DOUBLE;
		break;		
	case MV_REALS:
		* _pblock_index = 34;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MRealP;
		t = TY_DOUBLE;
		break;		
	case MV_DISPLAYH:
		* _pblock_index = 35;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MDisplayH;
		t = TY_DOUBLE;
		break;		
	case MV_DISPLAYL:
		* _pblock_index = 36;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MDisplayL;
		t = TY_DOUBLE;
		break;		
	case MV_DISPLAY:
		* _pblock_index = 37;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MDisplay;
		t = TY_DOUBLE;
		break;		
	case MV_STEADYFIELD:
		* _pblock_index = 38;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MSteadyField;
		t = TY_DOUBLE;
		break;		
	case MV_MHISTORY:
		* _pblock_index = 39;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MHistory;
		t = TY_DOUBLE;
		break;		
	case MV_MLASTIRV:
		* _pblock_index = 40;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MLastIRV;
		t = TY_DOUBLE;
		break;		
	case MV_MLASTHIGH:
		* _pblock_index = 41;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MLastHigh;
		t = TY_DOUBLE;
		break;		
	case MV_MLASTLOW:
		* _pblock_index = 42;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MLastLow;
		t = TY_DOUBLE;
		break;		
	case MV_RCONTROLMODE:
		* _pblock_index = 43;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MRControl_type;
		t = TY_WORD;
		break;		
	case MV_PRIORITY:
		* _pblock_index = 44;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MPriority;
		t = TY_WORD;
		break;		
	case MV_HOVERCOUNT:
		* _pblock_index = 45;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MHOverCount;
		t = TY_WORD;
		break;		
	case MV_LOVERCOUNT:
		* _pblock_index = 46;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MLOverCount;
		t = TY_WORD;
		break;		
	case MV_CONTROLMODE:
		* _pblock_index = 47;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MControl_type;
		t = TY_WORD;
		break;		
	case MV_FPRIORITY:
		* _pblock_index = 48;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MFault_Priority;
		t = TY_WORD;
		break;		
	case MV_PDirtyCount:
		* _pblock_index = 49;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MPDirtyCount;
		t = TY_WORD;
		break;		
	case MV_RDirtyCount:
		* _pblock_index = 50;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MRDirtyCount;
		t = TY_WORD;
		break;		
	case MV_CTHistory:
		* _pblock_index = 51;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MCT_history;
		t = TY_WORD;
		break;		
	case MV_BAD:
		* _pblock_index = 52;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MBad;
		t = TY_WORD;
		break;		
	case MV_STATE:
		* _pblock_index = 53;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MInvalidate;
		t = TY_BOOL;
		break;		
	case MV_DIST:
		* _pblock_index = 54;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MDist;
		t = TY_BOOL;
		break;		
	case MV_SATUATION:
		* _pblock_index = 55;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MSatuation;
		t = TY_BOOL;
		break;		
	case MV_ALARM:
		* _pblock_index = 56;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MAlarm;
		t = TY_BOOL;
		break;		
	case MV_KEYMV:
		* _pblock_index = 57;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MKey;
		t = TY_BOOL;
		break;	
	case MV_MSETSATUATION:
		* _pblock_index = 58;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MSetSatuation;
		t = TY_BOOL;
		break;		
	case MV_IRVWRONG:
		* _pblock_index = 59;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MIRVWrong;
		t = TY_BOOL;
		break;		
	case MV_MAPARTAM:
		* _pblock_index = 60;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MApartAM;
		t = TY_BOOL;
		break;		
	case MV_AM:
		* _pblock_index = 61;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MAM;
		t = TY_BOOL;
		break;		
	case MV_HOPEAM:
		* _pblock_index = 62;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MHope_AM;
		t = TY_BOOL;
		break;		
	case MV_HEDGE:
		* _pblock_index = 63;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MHEdge;
		t = TY_BOOL;
		break;		
	case MV_LEDGE:
		* _pblock_index = 64;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MLEdge;
		t = TY_BOOL;
		break;		
	case MV_RDIRTY:
		* _pblock_index = 65;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MRDirty;
		t = TY_BOOL;
		break;		
	case MV_PDIRTY:
		* _pblock_index = 66;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MPDirty;
		t = TY_BOOL;
		break;		
	case MV_SDIRTY:
		* _pblock_index = 67;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MSDirty;
		t = TY_BOOL;
		break;		
	case MV_FLAGRBROKEN:
		* _pblock_index = 68;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MRBroken;
		t = TY_BOOL;
		break;		
	case MV_PBROKEN:
		* _pblock_index = 69;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MPBroken;
		t = TY_BOOL;
		break;		
	case MV_HOVER:
		* _pblock_index = 70;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MHOver;
		t = TY_BOOL;
		break;		
	case MV_LOVER:
		* _pblock_index = 71;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MLOver;
		t = TY_BOOL;
		break;		
	case MV_LASTCM:
		* _pblock_index = 72;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MLastCM;
		t = TY_BOOL;
		break;		
	case MV_RMODE:
		* _pblock_index = 73;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MRCorrectMode;
		t = TY_BOOL;
		break;		
	case MV_SMODE:
		* _pblock_index = 74;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MSCorrectMode;
		t = TY_BOOL;
		break;		
	case MV_ERRMODE:
		* _pblock_index = 75;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MErrorMode;
		t = TY_BOOL;
		break;		
	case MV_MCM:
		* _pblock_index = 76;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->MCM;
		t = TY_BOOL;
		break;
	case MV_NAME:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->LPName;
		
		break;
	case MV_TAG:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->LPTag;
		
		break;
	case MV_UNIT:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->LPUnit;
		
		break;
	case MV_DATANAME:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->LPBaseName;
		
		break;
	case MV_PRIV:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivMV->data[Index]->LPPriv;
		
		break;
	default:;
	}
	(*type_val_) = t;
	return pvoid;
}

DLLEXPORT void* WriteDVMember(void* lpDV , long Index , WORD PAR_ID ,int _block_index, TYPE_ID type_val_ )
{
	float _fval;

	switch( type_val_ )
	{
	case TY_WORD:
		_fval = (float)*(WORD* )lpDV;
		break;
	case TY_DOUBLE:
		_fval = (float)*(DOUBLE* )lpDV;
		break;
	case TY_INT:
		_fval = (float)*(INT* )lpDV;
		break;
	case TY_BOOL:
		_fval = (float)*(BOOL* )lpDV;
		break;
	default:
		return NULL;
	}
	eda_set_float(DVGroup[Index], DVBlock[Index][_block_index], &_fval);
	eda_write1(DVGroup[Index],DVBlock[Index][_block_index]);

	return NULL;
}

DLLEXPORT void* GetLP_DVMember(LPCOBJ lpcObjDV , long Index , WORD PAR_ID ,int * _pblock_index, TYPE_ID* type_val_ )
{
	void* pvoid = NULL;
	*_pblock_index = MAX_DV_PAR_NUM-1;
	TYPE_ID	t;
	LPCOBJPRIVDV	lpcObjPrivDV;
	lpcObjPrivDV = COBJ_2_PRIVDV( lpcObjDV);


	if( Index<0 || !HasInitialized )
		return pvoid;

	switch(PAR_ID)
	{
	case DV_VALUE:
		*_pblock_index = DV_DATA_INDEX;
		pvoid  = (void*)&lpcObjPrivDV->data[Index]->DData;
		t=TY_DOUBLE;
		break;
	case DV_HSCALE:
		*_pblock_index = 1;
		pvoid  = (void*)&lpcObjPrivDV->data[Index]->DHScale;
		t = TY_DOUBLE;
		break;
	case DV_LSCALE:
		*_pblock_index = 2;
		pvoid  = (void*)&lpcObjPrivDV->data[Index]->DLScale;
		t = TY_DOUBLE;
		break;
	case DV_INI:
		*_pblock_index = 3;
		pvoid  = (void*)&lpcObjPrivDV->data[Index]->DIni;
		t = TY_DOUBLE;
		break;
	case DV_DELT:
		*_pblock_index = 4;
		pvoid  = (void*)&lpcObjPrivDV->data[Index]->DDelt;
		t = TY_DOUBLE;
		break;
	case DV_HISTORY:
		*_pblock_index = 5;
		pvoid  = (void*)&lpcObjPrivDV->data[Index]->DHistory;
		t = TY_DOUBLE;
		break;		
	case DV_DISPLAYH:
		*_pblock_index = 6;
		pvoid  = (void*)&lpcObjPrivDV->data[Index]->DDisplayH;
		t = TY_DOUBLE;
		break;		
	case DV_DISPLAYL:
		*_pblock_index = 7;
		pvoid  = (void*)&lpcObjPrivDV->data[Index]->DDisplayL;
		t = TY_DOUBLE;
		break;		
	case DV_DISPLAY:
		*_pblock_index = 8;
		pvoid  = (void*)&lpcObjPrivDV->data[Index]->DDisplay;
		t = TY_DOUBLE;
		break;		
	case DV_STEADYFIELD:
		*_pblock_index = 9;
		pvoid  = (void*)&lpcObjPrivDV->data[Index]->DSteadyField;
		t = TY_DOUBLE;
		break;		
	case DV_STATE:
		*_pblock_index = 10;
		pvoid  = (void*)&lpcObjPrivDV->data[Index]->DInvalid;
		t = TY_BOOL;
		break;		
	case DV_ALARM:
		*_pblock_index = 11;
		pvoid  = (void*)&lpcObjPrivDV->data[Index]->DAlarm;
		t = TY_BOOL;
		break;		
	case DV_BAD:
		*_pblock_index = 12;
		pvoid  = (void*)&lpcObjPrivDV->data[Index]->DBad;
		t = TY_WORD;
		break;		
	case DV_NAME:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivDV->data[Index]->LPName;
		
		break;
	case DV_TAG:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivDV->data[Index]->LPTag;
		
		break;
	case DV_UNIT:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivDV->data[Index]->LPUnit;
		
		break;
	case DV_DATANAME:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivDV->data[Index]->LPBaseName;
		
		break;
	case DV_PRIV:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivDV->data[Index]->LPPriv;
		
		break;
	default:;
	}
	(*type_val_) = t;
	return pvoid;
}

DLLEXPORT void* WriteGVMember2DB(void* lpGV, WORD PAR_ID , int _block_index , TYPE_ID type_val_ )
{
	
	long Index = 0;

	float _fval;

	switch( type_val_ )
	{
	case TY_WORD:
		_fval = (float)*(WORD* )lpGV;
		break;
	case TY_DOUBLE:
		_fval = (float)*(DOUBLE* )lpGV;
		break;
	case TY_INT:
		_fval = (float)*(INT* )lpGV;
		break;
	case TY_BOOL:
		_fval = (float)*(BOOL* )lpGV;
		break;
	default:
		return NULL;
	}
	eda_set_float(GVGroup[Index], GVBlock[Index][_block_index], &_fval);
	eda_write1(GVGroup[Index],GVBlock[Index][_block_index]);
	return NULL;
}

DLLEXPORT void* GetLP_GVMember(LPCOBJPRIV lpcObjPrivGV , WORD PAR_ID ,int * _pblock_index, TYPE_ID* type_val_ )
{
	void*	pvoid = NULL;
	long	Index = 0;
	TYPE_ID	t;

	*_pblock_index=MAX_CV_PAR_NUM-1;

	if( !HasInitialized )
		return pvoid;

	switch(PAR_ID)
	{
	case MDMCIRVD_FILENO:
		*_pblock_index = 0;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->File_No;
		t = TY_WORD;
		break;
	case MDMCIRVD_N:
		*_pblock_index = 1;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->N;
		t = TY_WORD;
		break;
	case MDMCIRVD_P:
		*_pblock_index = 2;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->P;
		t = TY_WORD;
		break;
	case MDMCIRVD_M:
		*_pblock_index = 3;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->M;
		t = TY_WORD;
		break;
	case MDMCIRVD_p:
		*_pblock_index = 4;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->p;
		t = TY_WORD;
		break;
	case MDMCIRVD_m:
		*_pblock_index = 5;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->m;
		t = TY_WORD;
		break;
	case MDMCIRVD_nd:
		*_pblock_index = 6;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->nd;
		t = TY_WORD;
		break;
	case MDMCIRVD_count:
		*_pblock_index = 7;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->Count;
		t = TY_WORD;
		break;
	case MDMCIRVD_MAKEPARACOUNT:
		*_pblock_index = 8;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->MakeParaCount;
		t = TY_WORD;
		break;
	case MDMCIRVD_RECOUNTCOUNT:
		*_pblock_index = 9;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->RecountCount;
		t = TY_WORD;
		break;
	case MDMCIRVD_LPCOUNT:
		*_pblock_index = 10;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->LPCount;
		t = TY_WORD;
		break;
	case MDMCIRVD_NOAPART:
		*_pblock_index = 11;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->No_Apart;
		t = TY_WORD;
		break;
	case MDMCIRVD_MPARACOUNT:
		*_pblock_index = 12;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->MParaCount;
		t = TY_WORD;
		break;
	case MDMCIRVD_MPARAMAX:
		*_pblock_index = 13;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->MParaMax;
		t = TY_WORD;
		break;
	case MDMCIRVD_STEADYSTEP:
		*_pblock_index = 14;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->SteadyStep;
		t = TY_WORD;
		break;
	case MDMCIRVD_ERRORCODE:
		*_pblock_index = 15;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->ErrorCode;
		t = TY_WORD;
		break;
	case MDMCIRVD_FLAGAM:
		*_pblock_index = 16;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_AM;
		t = TY_WORD;
		break;
	case MDMCIRVD_FREECOUNT:
		*_pblock_index = 17;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->Freecount;
		t = TY_INT;
		break;
	case MDMCIRVD_MAXDELAY:
		*_pblock_index = 18;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->MaxDelay;
		t = TY_WORD;
		break;
	case MDMCIRVD_Modu:
		*_pblock_index = 19;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->wModu;
		t = TY_WORD;
		break;
	case MDMCIRVD_N1:
		*_pblock_index = 20;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->N1;
		t = TY_WORD;
		break;
	case MDMCIRVD_N2:
		*_pblock_index = 21;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->N2;
		t = TY_WORD;
		break;
	case MDMCIRVD_STEADYCOUNT:
		*_pblock_index = 22;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->SteadyCount;
		t = TY_WORD;
		break;
	case MDMCIRVD_CALSTEADYMAX:
		*_pblock_index = 23;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->CalSteadyMax;
		t = TY_WORD;
		break;
	case MDMCIRVD_APARTI:
		*_pblock_index = 24;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->Apart_i;
		t = TY_WORD;
		break;
	case MDMCIRVD_BADMAX:
		*_pblock_index = 25;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->BadMax;
		t = TY_WORD;
		break;
	case MDMCIRVD_CCLASSIF:
		*_pblock_index = 26;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_CControl;
		t = TY_BOOL;
		break;
	case MDMCIRVD_MCLASSIF:
		*_pblock_index = 27;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_MControl;
		t = TY_BOOL;
		break;
	case MDMCIRVD_FLAGFAULT:
		*_pblock_index = 28;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_Fault;
		t = TY_BOOL;
		break;
	case MDMCIRVD_FLAGLP:
		*_pblock_index = 29;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_LP;
		t = TY_BOOL;
		break;
	case MDMCIRVD_FLAGALARM:
		*_pblock_index = 30;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_Alarm;
		t = TY_BOOL;
		break;
	case MDMCIRVD_FLAGRESP:
		*_pblock_index = 31;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_ReSP;
		t = TY_BOOL;
		break;
	case MDMCIRVD_SRMODE:
		*_pblock_index = 32;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_SRMode;
		t = TY_BOOL;
		break;
	case MDMCIRVD_FLAGMCHANGE:
		*_pblock_index = 33;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_MChange;
		t = TY_BOOL;
		break;
	case MDMCIRVD_FLAGLPCHANGE:
		*_pblock_index = 34;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_LPChange;
		t = TY_BOOL;
		break;
	case MDMCIRVD_FLAGHOPEAM:
		*_pblock_index = 35;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_HopeAM;
		t = TY_BOOL;
		break;
	case MDMCIRVD_FLAGRECOUNT:
		*_pblock_index = 36;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_Recount;
		t = TY_BOOL;
		break;
	case MDMCIRVD_FLAGLPRECOUNT:
		*_pblock_index = 37;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_LPRecount;
		t = TY_BOOL;
		break;
	case MDMCIRVD_STARTMODE:
		*_pblock_index = 38;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->StartMode;
		t = TY_BOOL;
		break;
	case MDMCIRVD_FLAGWEIGHT:
		*_pblock_index = 39;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_Weight;
		t = TY_BOOL;
		break;
	case MDMCIRVD_FLAGDEBUG:
		*_pblock_index = 40;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_Debug;
		t = TY_BOOL;
		break;
	case MDMCIRVD_FLAGHMODE:
		*_pblock_index = 41;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_HMode;
		t = TY_BOOL;
		break;
	case MDMCIRVD_FLAGAM1:
		*_pblock_index = 42;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_AM1;
		t = TY_BOOL;
		break;
	case MDMCIRVD_FLAGAM2:
		*_pblock_index = 43;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_AM2;
		t = TY_BOOL;
		break;
	case MDMCIRVD_FLAGAM3:
		*_pblock_index = 44;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_AM3;
		t = TY_BOOL;
		break;
	case MDMCIRVD_FLAGRUN:
		*_pblock_index = 45;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_Run;
		t = TY_BOOL;
		break;
	case MDMCIRVD_STEADY:
		*_pblock_index = 46;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_Steady;
		t = TY_BOOL;
		break;
	case MDMCIRVD_MCVRECOUNT:
		*_pblock_index = 47;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_MCVRecount;
		t = TY_BOOL;
		break;
	case MDMCIRVD_FLAGRDEBUG:
		*_pblock_index = 48;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_RDebug;
		t = TY_BOOL;
		break;
	case MDMCIRVD_Save:
		*_pblock_index = 49;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_Save;
		t = TY_BOOL;
		break;
	case MDMCIRVD_INISTART:
		*_pblock_index = 50;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->IniStart;
		t = TY_BOOL;
		break;
	case MDMCIRVD_FLAGCORSTEADY:
		*_pblock_index = 51;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->Flag_CorSteady;
		t = TY_BOOL;
		break;
	case MDMCIRVD_CALSTEADY:
		*_pblock_index = 52;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->CalSteady;
		t = TY_BOOL;
		break;
	case MDMCIRVD_ERROR:
		*_pblock_index = 53;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->Error;
		t = TY_BOOL;
		break;
	case MDMCIRVD_LPK:
		*_pblock_index = 55;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->LpK;
		t = TY_DOUBLE;
		break;
	case MDMCIRVD_alpha:
		*_pblock_index = 55;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->Alpha;
		t = TY_DOUBLE;
		break;
	case MDMCIRVD_BIGCOND:
		*_pblock_index = 56;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->Big_Cond;
		t = TY_DOUBLE;
		break;
	case MDMCIRVD_PBROKEN:
		*_pblock_index = 57;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->PBroken;
		t = TY_DOUBLE;
		break;
	case MDMCIRVD_QBROKEN:
		*_pblock_index = 58;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->QBroken;
		t = TY_DOUBLE;
		break;
	case MDMCIRVD_RBROKEN:
		*_pblock_index = 59;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->RBroken;
		t = TY_DOUBLE;
		break;
	case MDMCIRVD_SRERROR:
		*_pblock_index = 60;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->SRError;
		t = TY_DOUBLE;
		break;
	case MDMCIRVD_DSRERROR:
		*_pblock_index = 61;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->DSRError;
		t = TY_DOUBLE;
		break;
	case MDMCIRVD_CORSTEADY:
		*_pblock_index = 62;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->Cor_Steady;
		t = TY_DOUBLE;
		break;
	case MDMCIRVD_SAMPLETIME:
		*_pblock_index = 63;//-- *_pblock_index;
		pvoid  = (void*)&lpcObjPrivGV->SampleTime;
		t = TY_DOUBLE;
		break;
	case MDMCIRVD_DFILTER:
		*_pblock_index = 64;
		t = TY_DOUBLE;
		pvoid  = (void*)&lpcObjPrivGV->DFilter;
		break;
	case MDMCIRVD_mG:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPG;
		break;
	case MDMCIRVD_mA:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPA;
		break;
	case MDMCIRVD_mAb:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPAb;
		break;
	case MDMCIRVD_mmul2:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPMul2;
		break;
	case MDMCIRVD_mmul3:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPMul3;
		break;
	case MDMCIRVD_mSR:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPSR;
		break;
	case MDMCIRVD_mDSR:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPDSR;
		break;
	case MDMCIRVD_mTempSR:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPTempSR;
		break;
	case MDMCIRVD_mTempDSR:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPTempDSR;
		break;
	case MDMCIRVD_mypredN:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPPredN;
		
		break;
	case MDMCIRVD_mH:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPH;
		
		break;
	case MDMCIRVD_MV:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPMV;
		
		break;
	case MDMCIRVD_CV:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPCV;
		break;
	case MDMCIRVD_DV:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPDV;
		
		break;
	case MDMCIRVD_mP:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPS;
		break;
	case MDMCIRVD_mQ:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPQ;
		break;
	case MDMCIRVD_mR:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPR;
		break;
	case MDMCIRVD_LPPRIV:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPLPPriv;
		break;
	case MDMCIRVD_HISTORY:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPHistory;
		break;
	case MDMCIRVD_CVSort:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPCVSort;
		
		break;
	case MDMCIRVD_MVSort:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPMVSort;
		break;
	case MDMCIRVD_LPDeluM:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPDeluM;
		break;
	case MDMCIRVD_YUHis:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPYUHis;
		break;
	case MDMCIRVD_YDHis:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPYDHis;
		break;
	case MDMCIRVD_ApartMV:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPApartMV;
		break;
	case MDMCIRVD_ApartCV:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPApartCV;
		break;
	case MDMCIRVD_ApartDV:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPApartDV;
		break;
	case MDMCIRVD_Apart:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPApart;
		break;
	case MDMCIRVD_OUTSR:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPOutSR;
		break;
	case MDMCIRVD_OUTDSR:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPOutDSR;
		break;
	case MDMCIRVD_LPCOBJGLOBAL:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPGlobal;
		break;
	case MDMCIRVD_INMV:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPINMV;
		break;
	case MDMCIRVD_INCV:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPINCV;
		break;
	case MDMCIRVD_INDV:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPINDV;
		
		break;
	case MDMCIRVD_LPPredU:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPPredU;
		
		break;
	case MDMCIRVD_LPPredD:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPPredD;
		break;
	case MDMCIRVD_LPISSTEADY:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPIsSteady;
		
		break;
	case MDMCIRVD_LPSTEADY:
		t = TY_LPCOBJ;
		pvoid  = (void*)&lpcObjPrivGV->LPSteady;
		break;
	default:;
	}
	(*type_val_) = t;
	return pvoid;
}
DLLEXPORT void*  DebugCVWrite( void* lpCVMember, TYPE_ID __val_type__ )
{
	return NULL;
}



BOOL DLLEXPORT TEST_ARInitialize()
{
	FixGetMyname(LocalNodeName,MAX_NAME_SIZE);
	ARINDEX = eda_type_to_index(LocalNodeName, "AR");
	TEST_GetARGroup();
	TEST_GetARBlock();
	return TRUE;
}

BOOL DLLEXPORT TEST_Create_AR_Block()
{
	eda_add_block(LocalNodeName, ARTagName, ARINDEX);
	return TRUE;
}
BOOL DLLEXPORT TEST_Delete_AR_Block()
{
	eda_delete_block(LocalNodeName, ARTagName, 1);
	return TRUE;
}
BOOL DLLEXPORT TEST_GetARGroup()
{
	ARGroup = eda_define_group(1,0);
	return TRUE;
}
BOOL DLLEXPORT TEST_GetARBlock()
{
	ARBlock=eda_define_ntf(ARGroup,LocalNodeName,ARTagName,"f_0" , NULL);
	return TRUE;
}

float DLLEXPORT TEST_ReadARBlock()
{
	float _val = 0.0;
	eda_read(ARGroup);
	eda_wait(ARGroup);
	eda_get_float(ARGroup,ARBlock,&_val);
	return _val;
}

BOOL DLLEXPORT TEST_WriteARBlock(float _val)
{
	eda_set_float(ARGroup, ARBlock, &_val);
	eda_write(ARGroup);
	return TRUE;
}