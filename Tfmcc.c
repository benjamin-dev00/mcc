//===================================================================
//Written by Zhang
//1.3, 1998
//Main program of MCC software
//===================================================================

#include "tfalgo.h"
#include "tfcmdmc.h"
#ifdef __ONSPEC
#include "Mtmfun.h"
#include "Mtdllfun.h"
#include "onworld.h"
#endif

_DLLEXP	ACRET _ZZKDECL MDMCIRVDCalcu( WRD wModu_ )
{
	LPCOBJPRIV		_lpCobjPrivAll, _lpCobjGlobalAll;
	LPCOBJPRIVCV	_lpCobjPrivCV, _lpGlobalCV;
	LPCOBJPRIVMV	_lpCobjPrivMV, _lpGlobalMV;
	LPCOBJPRIVDV	_lpCobjPrivDV, _lpGlobalDV;
	LPCOBJ			_lpCobj_Priv;
	LPCOBJAPART		_lpApart;
	WORD			_i, _j, _flag_ini, _Apart_i, _recount, _TempCount = 0;
	BOOL 			_flag_nRecount = TRUE;
	DWORD			_SampleTime;

	double			_flag_0, _flag_1;
	char			_state;

	_flag_0 = 1;
	_flag_1 = 0;
	_state = 1;

#ifdef __ZZK
	LPPU			_lppu = ModuGetParameter( wModu_ );

//Get the Global structure
	MFPUREAD( _lppu, MDMCIRVD_LPCOBJGLOBAL, LPCOBJ, _lpCobj_Priv );
	_lpCobjGlobalAll = COBJ_2_PRIV( _lpCobj_Priv );
//If Error == 1, then the core algorithm would be skipped
	READALLPARAMETER( _lpCobjGlobalAll, MDMCIRVD_ERROR, BOOL, _lpCobjGlobalAll->Error );
	if( !_lpCobjGlobalAll->Error )
		_lpCobjGlobalAll->Flag_Run = 1;
	else
		_lpCobjGlobalAll->Flag_Run = 0;
#endif

#ifndef __Simul
	Sleep( 1000 );
#endif

#ifdef __ONSPEC
	_lpCobj_Priv = CobjAlloc( IDCOBJ_MATRIX, sizeof(COBJ) + sizeof( CobjPriv ) );
	_lpCobjGlobalAll = COBJ_2_PRIV( _lpCobj_Priv );
	_lpCobjGlobalAll->LPGlobal = _lpCobj_Priv;
// On Onspec, initial memory alloc. tfini.c
	PrivInitiate( _lpCobjGlobalAll );	
#endif

	WRITEALLPARAMETER( _lpCobjGlobalAll, MDMCIRVD_FLAGRUN, BOOL, _lpCobjGlobalAll->Flag_Run );

// If count <= 2, nothing would happen
// outlook: count means what??
// count：当前以算步数；开始空算2次，完成初始值计算
	while( _lpCobjGlobalAll->Flag_Run != 0 )
	{

#ifdef __Simul_Matlab
//  目的：
//		  Mablab 仿真控制步长
//	Date: 
//		  2001-3-20  2:30 PM	  		

//	initialize(&eur, &eui, &flg, &euf, &eufi,
//      &dii, &doo, &scan, &out,&str, &stf);
//标志位
	//writemem(EUI, 1304, _flag_0, 0);
	while ( (int)_flag_1 != (int)_flag_0 )
	{	
		readmem(EUI,1305,&_flag_1,&_state);
		Sleep(10);
	}

	if ((int)_flag_0 == 0)
	{
		_flag_0 = 1;
		_flag_1 = 0;
	}
	else if ((int)_flag_0 == 1)
	{
		_flag_0 = 0;
		_flag_1 = 1;
	}
		
//标志发生变化，MCC运行
//	_flag_0 = _flag_1;

#endif

		_SampleTime = (DWORD)( _lpCobjGlobalAll->SampleTime * 1000 );

//date: 2001-3-19 zck 	
//		if( _lpCobjGlobalAll->Count >= 3 )
//			Sleep( _SampleTime );
		
//Read global parameters，<--tfr_all.c
		RealAll( _lpCobjGlobalAll );
		_lpGlobalMV = COBJ_2_PRIVMV( _lpCobjGlobalAll->LPINMV );
		_lpGlobalCV = COBJ_2_PRIVCV( _lpCobjGlobalAll->LPINCV );
		_lpGlobalDV = COBJ_2_PRIVDV( _lpCobjGlobalAll->LPINDV );

#ifdef __Simul
//when simulating, read the history data of u-y or d-y.  tfg_prev.c
		GetPrevUD( _lpCobjGlobalAll );
		if( _lpCobjGlobalAll->Count >= 2 )
//generate output data.  tfflout.c
			File_OUT( _lpCobjGlobalAll, _lpGlobalMV, _lpGlobalCV, _lpGlobalDV );
#endif
		
//Outlook: ErrorCode means what?
		if( _lpCobjGlobalAll->ErrorCode >= 100 )
			_lpCobjGlobalAll->ErrorCode = 0;
		WRITEALLPARAMETER( _lpCobjGlobalAll, MDMCIRVD_ERRORCODE, WORD, _lpCobjGlobalAll->ErrorCode );
//Read MV CV and DV data.  tfr_mcv.c
		ReadMCV( _lpCobjGlobalAll, _lpGlobalMV, _lpGlobalCV, _lpGlobalDV );
//get display data
		Display( _lpCobjGlobalAll, _lpGlobalMV, _lpGlobalCV, _lpGlobalDV );

//		begin to run this algorithm
//		ApartStruct( _lpCobjGlobalAll, _lpCobjPrivMV, _lpCobjPrivCV, _lpCobjPrivDV );
//LPApart--分块矩阵, very important
		_lpApart = COBJ_2_APART( _lpCobjGlobalAll->LPApart );
		if( !_lpCobjGlobalAll->ErrorCode )
			_lpCobjGlobalAll->ErrorCode = 100;
		WRITEALLPARAMETER( _lpCobjGlobalAll, MDMCIRVD_ERRORCODE, WORD, _lpCobjGlobalAll->ErrorCode );

/********************************************** 
 *  MCC运作开始，稳态优化->动态优化->输出
 *  Comment: Outlook
 *********************************************/

		// 如果 _lpCobjGlobalAll->Count <= 1, 不作运算，进入下一循环;
		if( _lpCobjGlobalAll->Flag_Run == 1 && _lpCobjGlobalAll->Count != 1 )
		{
//>>>>>>>>>>>>稳态处理>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>		
			for( _Apart_i = 0; _Apart_i < _lpCobjGlobalAll->No_Apart; _Apart_i++ )
			{
				if ( _lpCobjGlobalAll->ErrorCode == 100 )
					_lpCobjGlobalAll->ErrorCode = 101;
				WRITEALLPARAMETER( _lpCobjGlobalAll, MDMCIRVD_ERRORCODE, WORD, _lpCobjGlobalAll->ErrorCode );
				_lpCobjPrivMV = COBJ_2_PRIVMV( APARTGET( _lpApart, _Apart_i, 0 )->LPINMV );
				_lpCobjPrivCV = COBJ_2_PRIVCV( APARTGET( _lpApart, _Apart_i, 0 )->LPINCV );
				_lpCobjPrivDV = COBJ_2_PRIVDV( APARTGET( _lpApart, _Apart_i, 0 )->LPINDV );
				_lpCobjPrivAll = APARTGET( _lpApart, _Apart_i, 0 );
				_lpCobjPrivAll->Apart_i = _Apart_i;
//				_lpCobjPrivAll->Flag_AM = PRIVMVGET( _lpCobjPrivMV, 0, 0 )->MApartAM;
//  determine steady state.  tfsteady.c
//  判断对象是否处于稳态;
				IsSteady( _lpCobjPrivAll, _lpCobjPrivCV, _lpCobjPrivMV, _lpCobjPrivDV );
//  correct model gain.  tfcorstd.c
//  稳态增益校正;
				CorSteady( _lpCobjPrivAll, _lpCobjPrivMV, _lpCobjPrivCV, _lpCobjPrivDV );
//  get fault field.  tfc_para.c
//  参数校正;
				CorrectParam( _lpCobjPrivAll, _lpCobjPrivMV, _lpCobjPrivCV );
//  归一化Unite all the variables.  tfunit.c
				UnitMCV( _lpCobjPrivAll, _lpCobjPrivMV, _lpCobjPrivCV, _lpCobjPrivDV );
//  count the increment of all the variables. tfdelt.c
//  计算△U，△Y, etc；
				DeltMCV( _lpCobjPrivAll, _lpCobjPrivMV, _lpCobjPrivCV, _lpCobjPrivDV );
//	Get the Step-response data tfg_sr.c
				GET_SR( _lpCobjPrivAll, _lpCobjPrivMV, _lpCobjPrivCV, _lpCobjPrivDV );
//	make the feedback correcting matrix tfg_h.c
//  生成反馈校正矩阵；
				GET_H( _lpCobjPrivAll );
				if( _lpCobjGlobalAll->ErrorCode == 101 )
					_lpCobjGlobalAll->ErrorCode = 102;
				WRITEALLPARAMETER( _lpCobjGlobalAll, MDMCIRVD_ERRORCODE, WORD, _lpCobjGlobalAll->ErrorCode );
//  if ouput has been sampled, then use it to correct the predictive CV.  tfc_y.c
				Correct_y( _lpCobjPrivAll, _lpCobjPrivMV, _lpCobjPrivCV, _lpCobjPrivDV );
				if( _lpCobjGlobalAll->ErrorCode == 102 )
					_lpCobjGlobalAll->ErrorCode = 103;
				WRITEALLPARAMETER( _lpCobjGlobalAll, MDMCIRVD_ERRORCODE, WORD, _lpCobjGlobalAll->ErrorCode );
				if( _lpCobjPrivAll->Flag_Alarm == 1 )
				{
//Alarm state, algorithm output halt. tfexit.c
					LookOut( _lpCobjPrivAll, _lpCobjPrivMV, _lpCobjPrivCV, _lpCobjPrivDV );
				}
				if( _lpCobjGlobalAll->ErrorCode == 103 )
					_lpCobjGlobalAll->ErrorCode = 104;
				WRITEALLPARAMETER( _lpCobjGlobalAll, MDMCIRVD_ERRORCODE, WORD, _lpCobjGlobalAll->ErrorCode );
//	automatic state...
				if( _lpCobjPrivAll->Flag_AM > 1 && _lpCobjPrivAll->Flag_Run == 1 )
				{
//  make sure all the parameters are effective	
//note: if SR or H changed, then....	
//Answer: SR shouldn't be changed when MCC is running
//  estimate the CV state for the first time, exceed or not. tfb_cv.c
					CVBegin( _lpCobjPrivAll, _lpCobjPrivCV );
//  count the freedom
//  remove ill-conditioning variables All CV should be controled!
					if( _lpCobjGlobalAll->ErrorCode == 104 )
						_lpCobjGlobalAll->ErrorCode = 105;
					WRITEALLPARAMETER( _lpCobjGlobalAll, MDMCIRVD_ERRORCODE, WORD, _lpCobjGlobalAll->ErrorCode );
//ill-condition. tfrm_ill.c
					RemoveIll( _lpCobjPrivAll, _lpCobjPrivMV, _lpCobjPrivCV );
					if( _lpCobjGlobalAll->ErrorCode == 105 )
						_lpCobjGlobalAll->ErrorCode = 106;
					WRITEALLPARAMETER( _lpCobjGlobalAll, MDMCIRVD_ERRORCODE, WORD, _lpCobjGlobalAll->ErrorCode );
//get freedom tfg_free.c
					GetFreedom( _lpCobjPrivAll, _lpCobjPrivMV, _lpCobjPrivCV );
//Cv recount should be decided here...! _flag_CVrecount??
// determine CV state tfc_cv.c
// 检查是否应放松一些CV;
					CVCorrect( _lpCobjPrivAll, _lpCobjPrivCV );
//  recount the freedom
					GetFreedom( _lpCobjPrivAll, _lpCobjPrivMV, _lpCobjPrivCV );
//free some CV if freedom is less than 0, and the cv priority control
					FreeCV( _lpCobjPrivAll, _lpCobjPrivCV );
					if( _lpCobjPrivAll->Freecount < 0 && !_lpCobjPrivAll->Flag_Alarm )
					{
//  free some CV, it is impossible!!
//						MessageBox( NULL, "ERROR", "Impossible freecount", MB_OK );
						MessageBeep( -1 );
                        LogError( "Freedom", "tfmcc.c 172" );
						_lpCobjGlobalAll->ErrorCode = 2;
//						_lpCobjPrivAll->Flag_Run = 0;
//						_lpCobjPrivAll->Flag_Alarm = 1;
					}
//  stead state optimize
					if( _lpCobjPrivAll->Flag_LP == 1 && _lpCobjPrivAll->Flag_Run == 1 )
					{
// nearly a null function tfseq.c
						Seq( _lpCobjPrivAll, _lpCobjPrivMV, _lpCobjPrivCV );
					}
				}
				if( _lpCobjPrivAll->Flag_Run == 0 )
					_lpCobjGlobalAll->Flag_Run = 0;
				if( _lpCobjPrivAll->Flag_Alarm == 1 )
					_lpCobjGlobalAll->Flag_Alarm = 1;
				if( _lpCobjGlobalAll->ErrorCode == 106 ||
						_lpCobjGlobalAll->ErrorCode == 104 )
					_lpCobjGlobalAll->ErrorCode = 100;
				WRITEALLPARAMETER( _lpCobjGlobalAll, MDMCIRVD_ERRORCODE, WORD, _lpCobjGlobalAll->ErrorCode );
			}
			if( _lpCobjGlobalAll->Flag_LP == 1 && _lpCobjGlobalAll->Flag_Run == 1 && _lpCobjGlobalAll->Flag_AM > 1 )
			{
//compound LP, merge all the sub-model first tfmerge.c
// 将各个模块的MV,CV组合成整个模型，进行组合线性规划
				MergeLPG( _lpCobjGlobalAll, _lpGlobalMV, _lpGlobalCV, _lpGlobalDV );
				if( _lpCobjGlobalAll->ErrorCode != 1 && !_lpCobjGlobalAll->Flag_Alarm )
				{
// outlook: What is Apart_i & No_Apart?
					_lpCobjGlobalAll->Apart_i = _lpCobjGlobalAll->No_Apart;
//compound LP.  tfcomplp.c
					CompoundLP( _lpCobjGlobalAll, _lpGlobalMV, _lpGlobalCV, _lpGlobalDV );
				}
				if( _lpCobjGlobalAll->ErrorCode == 100 )
					_lpCobjGlobalAll->ErrorCode = 110;
				WRITEALLPARAMETER( _lpCobjGlobalAll, MDMCIRVD_ERRORCODE, WORD, _lpCobjGlobalAll->ErrorCode );
			}

//>>>>>>>>>>>>动态开始>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		 	for( _Apart_i = 0; _Apart_i < _lpCobjGlobalAll->No_Apart; _Apart_i++ )
			{
				_lpCobjPrivMV = COBJ_2_PRIVMV( APARTGET( _lpApart, _Apart_i, 0 )->LPINMV );
				_lpCobjPrivCV = COBJ_2_PRIVCV( APARTGET( _lpApart, _Apart_i, 0 )->LPINCV );
				_lpCobjPrivDV = COBJ_2_PRIVDV( APARTGET( _lpApart, _Apart_i, 0 )->LPINDV );
				_lpCobjPrivAll = APARTGET( _lpApart, _Apart_i, 0 );
				_lpCobjPrivAll->Apart_i = _Apart_i;
				if( _lpCobjPrivAll->Flag_AM > 1 && _lpCobjPrivAll->Flag_Run == 1 )
				{
// recover some CV, if not priority CV control mode.  tfreccv.c
					RecoverCV( _lpCobjPrivAll, _lpCobjPrivCV );
// Outlook: Flag_MControl--MV自由度控制方式	
//					if( _lpCobjPrivAll->Flag_MControl == 1 )
//  estimate MV state tfb_mve.c
//						MVBeginPri( _lpCobjPrivAll, _lpCobjPrivMV );
//					else
					MVBeginErr( _lpCobjPrivAll, _lpCobjPrivMV );
//  estimate MV state again tfc_mv.c
					MVCorrect( _lpCobjPrivAll, _lpCobjPrivMV );
// Flag_MCVRecount--权矩阵重组,?  Flag_ReSP--权系数重设,?  Flag_AM--手自动开关
					if(  ( _lpCobjPrivAll->Flag_Run == 1 ) && ( (_lpCobjPrivAll->Count == 1)
					|| ( _lpCobjPrivAll->Flag_MCVRecount == 1 )
					||( _lpCobjPrivAll->Flag_ReSP == 2 ) ||
						( _lpCobjPrivAll->Flag_AM == 2 ) ))
					{
//  make the matrix what would be used next tfm_para.c(耗时?)
						_flag_nRecount = MAKE_PARAM( _lpCobjPrivAll, _lpCobjPrivMV, _lpCobjPrivCV );
						//if( _flag_nRecount == FALSE )
						//	MessageBox( NULL, "ERROR", "No effective MV", MB_OK );
//  if here it is false, then I think no need to do other things
//  MParaCount--权矩阵保持计数
						_lpCobjPrivAll->MParaCount = 0;
					}
					else
					{
//  MParaMax--最大"权矩阵保持"次数; Flag_ReSP--权系数重设;
						if( _lpCobjPrivAll->MParaCount == _lpCobjPrivAll->MParaMax )
						{	
							_lpCobjPrivAll->Flag_ReSP = 2;
							_flag_nRecount = MAKE_PARAM( _lpCobjPrivAll, _lpCobjPrivMV, _lpCobjPrivCV );
							if( _flag_nRecount == FALSE )
//								MessageBox( NULL, "ERROR", "No effective MV", MB_OK );
							{
								MessageBeep( -1 );
                                LogError( "MakeParam", "tfmcc.c, 239" );
							}
						}
						_lpCobjPrivAll->MParaCount++;
					}
					if( _lpCobjGlobalAll->ErrorCode == 110 )
						_lpCobjGlobalAll->ErrorCode = 120;
					WRITEALLPARAMETER( _lpCobjGlobalAll, MDMCIRVD_ERRORCODE, WORD, _lpCobjGlobalAll->ErrorCode );
					_recount = 0;
					_flag_ini = 0;

//  Flag_Recount--动态规划重算标志
					_lpCobjPrivAll->Flag_Recount = 0;
					
					while( ( _lpCobjPrivAll->Flag_Run == 1 ) &&
						( (_lpCobjPrivAll->Flag_Recount == 1) || ( _flag_ini == 0 ) ) &&
							!_lpCobjPrivAll->Flag_Alarm )
					{
						if( _flag_ini != 0 )
						{
							WORD	_flag_Satuation = 0;
							for( _j = 0; (_j < _lpCobjPrivAll->m) &&
									( _flag_Satuation == 0 ); _j++ )
								if( PRIVMVGET( _lpCobjPrivMV, _j, 0 )->MControl_type == 8 )
									_flag_Satuation = 1;
							if( _lpCobjGlobalAll->ErrorCode == 120 )
								_lpCobjGlobalAll->ErrorCode = 130;
							WRITEALLPARAMETER( _lpCobjGlobalAll, MDMCIRVD_ERRORCODE, WORD, _lpCobjGlobalAll->ErrorCode );
							if( _flag_Satuation == 1 )
//	if satuaion state, correct the predictive CV tfc_pred.c
								CorrectPred( _lpCobjPrivAll, _lpCobjPrivMV );
//  re-make the matrix
							if( _lpCobjGlobalAll->ErrorCode == 130 )
								_lpCobjGlobalAll->ErrorCode = 140;
							WRITEALLPARAMETER( _lpCobjGlobalAll, MDMCIRVD_ERRORCODE, WORD, _lpCobjGlobalAll->ErrorCode );
							_flag_nRecount = MAKE_PARAM( _lpCobjPrivAll, _lpCobjPrivMV, _lpCobjPrivCV );
							if( _lpCobjGlobalAll->ErrorCode == 140 )
								_lpCobjGlobalAll->ErrorCode = 150;
							WRITEALLPARAMETER( _lpCobjGlobalAll, MDMCIRVD_ERRORCODE, WORD, _lpCobjGlobalAll->ErrorCode );
							for( _j = 0; (_j < _lpCobjPrivAll->m); _j++ )
								if( PRIVMVGET( _lpCobjPrivMV, _j, 0 )->MControl_type == 8 )
								{
									if( PRIVMVGET( _lpCobjPrivMV, _j, 0 )->MCT_history == 1 )
										PRIVMVGET( _lpCobjPrivMV, _j, 0 )->MControl_type = 1;
									else
									{	if( PRIVMVGET( _lpCobjPrivMV, _j, 0 )->MCT_history == 3 )
											PRIVMVGET( _lpCobjPrivMV, _j, 0 )->MControl_type = 3;
										else
											PRIVMVGET( _lpCobjPrivMV, _j, 0 )->MControl_type = 2;
									}
								}
							_lpCobjPrivAll->Flag_Recount = 0;
							_lpCobjPrivAll->Count--;
							_lpCobjPrivAll->RecountCount++;
						}

						if( _flag_nRecount == TRUE )
						{
							_flag_ini = 1;
//	use mcc algorithm to determine MV.   tfdyopti.c
							Dyna_Opti( _lpCobjPrivAll, _lpCobjPrivMV, _lpCobjPrivCV );
							_lpCobjPrivAll->Count++;
							if( _recount < repeatcount )
							{
//	decide whether it should be recount or not tfis_re.c
								_lpCobjPrivAll->Freecount = Isrecount( _lpCobjPrivAll,
												_lpCobjPrivMV, _lpCobjPrivCV, _recount );
								for( _j = 0; _j < _lpCobjPrivAll->m; _j++ )
									if( _lpCobjPrivAll->Flag_Recount == 0 )
										if( PRIVMVGET( _lpCobjPrivMV, _j, 0 )->MControl_type == 8 ||
											PRIVMVGET( _lpCobjPrivMV, _j, 0 )->MPDirty == 1 )
											_lpCobjPrivAll->Flag_Recount = 1;

								for( _i = 0; _i < _lpCobjPrivAll->p; _i++ )
									if( _lpCobjPrivAll->Flag_Recount == 0 )
										if( PRIVCVGET( _lpCobjPrivCV, _i, 0 )->CDirty == 1 )
											_lpCobjPrivAll->Flag_Recount = 1;
								_recount++;
							}
						}
						else
						{
							for( _j = 0; _j < _lpCobjPrivAll->m; _j++ )
								MGET( ( COBJ_2_MATRIX( _lpCobjPrivAll->LPDeluM ) ), _j*_lpCobjPrivAll->M, 0 ) = 0.0;
							_lpCobjPrivAll->Count++;
							_flag_nRecount = TRUE;
//					MessageBox( NULL, "ERROR", "No effective MV", MB_OK );
						}
					}
//	Force MV not to exceed the limit.  tff_mv.c
					ForceMV( _lpCobjPrivAll, _lpCobjPrivMV );
				}

				GetFinalFreedom( _lpCobjPrivAll, _lpCobjPrivMV, _lpCobjPrivCV );
//  ununite all variables.   tfununit.c
				UnUnit( _lpCobjPrivAll,  _lpCobjPrivMV, _lpCobjPrivCV, _lpCobjPrivDV );
//  write sub-model parameter.   tfw_subg.c
				WriteSubGParameter( _lpCobjGlobalAll, _Apart_i );
			}

//  when flag_AM is reset????;
//  write data to database
			WriteMCV( _lpCobjGlobalAll, _lpGlobalMV,_lpGlobalCV, _lpGlobalDV  );
// Alarm handle
			if( _lpCobjGlobalAll->Flag_Alarm == 1 )
			{
				static _Flag_Error = 1;

				if( _Flag_Error )
				{
					LogError( "Alarm", "报警" );
					_lpCobjGlobalAll->Flag_Alarm = 1;
					Write_FileAll( _lpCobjGlobalAll );
					Write_FileMV( _lpCobjGlobalAll, _lpGlobalMV );
					Write_FileCV( _lpCobjGlobalAll, _lpGlobalCV );
					Write_FileDV( _lpCobjGlobalAll, _lpGlobalDV );
					_Flag_Error = 0;
				}
			}
		}
// 手动状态情况
		else
		{
// UnUnit( _lpCobjPrivAll,  _lpCobjPrivMV, _lpCobjPrivCV );
			_lpCobjGlobalAll->Count++;
//
			WRITEALLPARAMETER( _lpCobjGlobalAll, MDMCIRVD_count, WORD, _lpCobjGlobalAll->Count );
		}

#ifdef __ZZK
		_lpCobjGlobalAll->Error = 0;	
		if( _lpCobjGlobalAll->Flag_Run == 0 )
			_lpCobjGlobalAll->Error = 1;		
		WRITEALLPARAMETER( _lpCobjGlobalAll, MDMCIRVD_ERROR, BOOL, _lpCobjGlobalAll->Error );
		_lpCobjGlobalAll->Flag_Run = 0;
		WRITEALLPARAMETER( _lpCobjGlobalAll, MDMCIRVD_FLAGRUN, BOOL, 0 );
#endif
	
#ifdef __Simul_Matlab
//  目的：
//		  Mablab 仿真控制步长
//	Date: 
//		  2001-3-20  2:30 PM	  		

//标志位
	writemem(EUI, 1304, _flag_1, 0);

#endif

	}
#ifdef __ONSPEC
	CobjFree( _lpCobjGlobalAll->LPPredU );
	_lpCobjGlobalAll->LPPredU = NULL;
	if( _lpCobjGlobalAll->nd )
	{
		CobjFree( _lpCobjGlobalAll->LPPredD );
		_lpCobjGlobalAll->LPPredD = NULL;
	}
//free memory tfstop.c
	PrivStop( _lpCobjGlobalAll, _lpGlobalMV, _lpGlobalCV, _lpGlobalDV );
	CobjFree( PRIV_2_COBJ( _lpCobjGlobalAll ) );
#endif
	return ACRET_TRUE;
}