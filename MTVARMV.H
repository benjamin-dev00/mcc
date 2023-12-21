//Valuate all fields of the struct variable *_lp_VarMv.
	//////
	do
	{
		//char
		_i_Counter=0;
		while(_i_Counter<STRINGLEN)
	    {
			_lp_VarMv->TAG[_i_Counter]=_c_MvChar[0][_i_Counter];
			_i_Counter++;
		}
        _i_Counter=0;
		while(_i_Counter<STRINGLEN)
	    {
			_lp_VarMv->DataName[_i_Counter]=_c_MvChar[1][_i_Counter];
			_i_Counter++;
		}
		_i_Counter=0;
		while(_i_Counter<STRINGLEN)
	    {
			_lp_VarMv->Name[_i_Counter]=_c_MvChar[2][_i_Counter];
			_i_Counter++;
		}
		_i_Counter=0;
		while(_i_Counter<STRINGLEN)
	    {
			_lp_VarMv->Unit[_i_Counter]=_c_MvChar[0][_i_Counter];
			_i_Counter++;
		}
	
	
		//double
		_lp_VarMv->MData=_db_MvDouble[0];
		_lp_VarMv->MHScale=_db_MvDouble[1];
		_lp_VarMv->MLScale=_db_MvDouble[2];
		_lp_VarMv->MDelt=_db_MvDouble[3];
		_lp_VarMv->MIrv=_db_MvDouble[4];
		_lp_VarMv->MIni=_db_MvDouble[5];
		_lp_VarMv->MDelmax=_db_MvDouble[6];
		_lp_VarMv->MDelmin=_db_MvDouble[7];
		_lp_VarMv->MMax=_db_MvDouble[8];
		_lp_VarMv->MMin=_db_MvDouble[9];
		_lp_VarMv->MIrvHigh=_db_MvDouble[10];
		_lp_VarMv->MIrvLow=_db_MvDouble[11];
		_lp_VarMv->MFault_max=_db_MvDouble[12];
		_lp_VarMv->MFault_min=_db_MvDouble[13];
		_lp_VarMv->MIrvFault_high=_db_MvDouble[14];
		_lp_VarMv->MIrvFault_low=_db_MvDouble[15];
		_lp_VarMv->MFault_xirv=_db_MvDouble[16];
		_lp_VarMv->MFault_nirv=_db_MvDouble[17];
		_lp_VarMv->MFault_hirv=_db_MvDouble[18];
		_lp_VarMv->MFault_lirv=_db_MvDouble[19];
		_lp_VarMv->MLPIni=_db_MvDouble[20];
		_lp_VarMv->MLPIndex=_db_MvDouble[21];
		_lp_VarMv->MSteadOptim=_db_MvDouble[22];
		_lp_VarMv->MR=_db_MvDouble[23];
		_lp_VarMv->MRMax=_db_MvDouble[24];
		_lp_VarMv->MP=_db_MvDouble[25];
		_lp_VarMv->MPL=_db_MvDouble[26];
		_lp_VarMv->MPMax=_db_MvDouble[27];
		_lp_VarMv->MDelR=_db_MvDouble[28];
		_lp_VarMv->MDelP=_db_MvDouble[29];
		_lp_VarMv->MRR=_db_MvDouble[30];
		_lp_VarMv->MRP=_db_MvDouble[31];
		_lp_VarMv->MHistory=_db_MvDouble[32];
	
		//int
		_lp_VarMv->MControl_type=_i_MvInt[0];
		_lp_VarMv->MSCT=_i_MvInt[1];
		_lp_VarMv->MPriority=_i_MvInt[2];
		_lp_VarMv->MFault_Priority=_i_MvInt[3];
		_lp_VarMv->MCT_history=_i_MvInt[4];
	
		//bool
		_lp_VarMv->RMode=_b_MvBool[0];
		_lp_VarMv->SMode=_b_MvBool[1];
		_lp_VarMv->ErrMode=_b_MvBool[2];
		_lp_VarMv->MValidate=_b_MvBool[3];			
		_lp_VarMv->MAlarm=_b_MvBool[4];
		_lp_VarMv->MAM=_b_MvBool[5];			
		_lp_VarMv->MRBroken=_b_MvBool[6];
		_lp_VarMv->MPBroken=_b_MvBool[7];
		_lp_VarMv->MConstrain_type=_b_MvBool[8];
		_lp_VarMv->MRDirty=_b_MvBool[9];
		_lp_VarMv->MPDirty=_b_MvBool[10];
		_lp_VarMv->MSDirty=_b_MvBool[11];
		_lp_VarMv->MIRVWrong=_b_MvBool[12];
	}while(0);
do
	{
		//Double
		_db_MvDouble[0]=_lp_VarMv->MData;
		_db_MvDouble[1]=_lp_VarMv->MHScale;
		_db_MvDouble[2]=_lp_VarMv->MLScale;
		_db_MvDouble[3]=_lp_VarMv->MDelt;
		_db_MvDouble[4]=_lp_VarMv->MIrv;
		_db_MvDouble[5]=_lp_VarMv->MIni;
		_db_MvDouble[6]=_lp_VarMv->MDelmax;
		_db_MvDouble[7]=_lp_VarMv->MDelmin;
		_db_MvDouble[8]=_lp_VarMv->MMax;
		_db_MvDouble[9]=_lp_VarMv->MMin;
		_db_MvDouble[10]=_lp_VarMv->MIrvHigh;
		_db_MvDouble[11]=_lp_VarMv->MIrvLow;
		_db_MvDouble[12]=_lp_VarMv->MFault_max;
		_db_MvDouble[13]=_lp_VarMv->MFault_min;
		_db_MvDouble[14]=_lp_VarMv->MIrvFault_high;
		_db_MvDouble[15]=_lp_VarMv->MIrvFault_low;
		_db_MvDouble[16]=_lp_VarMv->MFault_xirv;
		_db_MvDouble[17]=_lp_VarMv->MFault_nirv;
		_db_MvDouble[18]=_lp_VarMv->MFault_hirv;
		_db_MvDouble[19]=_lp_VarMv->MFault_lirv;
		_db_MvDouble[20]=_lp_VarMv->MLPIni;
		_db_MvDouble[21]=_lp_VarMv->MLPIndex;
		_db_MvDouble[22]=_lp_VarMv->MSteadOptim;
		_db_MvDouble[23]=_lp_VarMv->MR;
		_db_MvDouble[24]=_lp_VarMv->MRMax;
		_db_MvDouble[25]=_lp_VarMv->MP;
		_db_MvDouble[26]=_lp_VarMv->MPL;
		_db_MvDouble[27]=_lp_VarMv->MPMax;
		_db_MvDouble[28]=_lp_VarMv->MDelR;
		_db_MvDouble[29]=_lp_VarMv->MDelP;
		_db_MvDouble[30]=_lp_VarMv->MRR;
		_db_MvDouble[31]=_lp_VarMv->MRP;
		_db_MvDouble[32]=_lp_VarMv->MHistory;
	
		//Int
		_i_MvInt[0]=_lp_VarMv->MControl_type;
		_i_MvInt[1]=_lp_VarMv->MSCT;
		_i_MvInt[2]=_lp_VarMv->MPriority;
		_i_MvInt[3]=_lp_VarMv->MFault_Priority;
		_i_MvInt[4]=_lp_VarMv->MCT_history;
	
		//Bool
		_b_MvBool[0]=_lp_VarMv->RMode;
		_b_MvBool[1]=_lp_VarMv->SMode;
		_b_MvBool[2]=_lp_VarMv->ErrMode;
		_b_MvBool[3]=_lp_VarMv->MValidate;			
		_b_MvBool[4]=_lp_VarMv->MAlarm;
		_b_MvBool[5]=_lp_VarMv->MAM;			
		_b_MvBool[6]=_lp_VarMv->MRBroken;
		_b_MvBool[7]=_lp_VarMv->MPBroken;
		_b_MvBool[8]=_lp_VarMv->MConstrain_type;
		_b_MvBool[9]=_lp_VarMv->MRDirty;
		_b_MvBool[10]=_lp_VarMv->MPDirty;
		_b_MvBool[11]=_lp_VarMv->MSDirty;
		_b_MvBool[12]=_lp_VarMv->MIRVWrong;

		//Char
		_i_Counter=0;
		while(_i_Counter<STRINGLEN)
	    {
			_lp_VarMv->TAG[_i_Counter]=_c_MvChar[0][_i_Counter];
			_i_Counter++;
		}
        _i_Counter=0;
		while(_i_Counter<STRINGLEN)
	    {
			_lp_VarMv->DataName[_i_Counter]=_c_MvChar[1][_i_Counter];
			_i_Counter++;
		}
		_i_Counter=0;
		while(_i_Counter<STRINGLEN)
	    {
			_lp_VarMv->Name[_i_Counter]=_c_MvChar[2][_i_Counter];
			_i_Counter++;
		}
		_i_Counter=0;
		while(_i_Counter<STRINGLEN)
	    {
			_lp_VarMv->Unit[_i_Counter]=_c_MvChar[0][_i_Counter];
			_i_Counter++;
		}
	}while(0);
	char	TAG[10];
	char	DataName[10];
	char	Name[10];
	char	Unit[10];
#define DOUBLE_MV[1a;
	double	MHScale;
	double	MLScale;
	int		MControl_type;   //0 ----- Free;
							 //1 ----- IRV control;
							 //2 ----- field control;
							 //3 ----- No constrained;
							 //4 ----- Bad
							 //5 ----- SATUATION
							 //6 ----- SEQ
							 //7 ----- DV
							 //8 ----- Fault-satuation
							 //9 ----- Unreasonable IRV,
							 //			changed from MControl_type = 1
    double	MDelt;
	double	MIrv;
	double	MIni;
	double	MDelmax;
	double	MDelmin;
	double	MMax;
	double	MMin;
	double	MIrvHigh;
	double	MIrvLow;
	double	MFault_max;
	double	MFault_min;
	double	MIrvFault_high;
	double	MIrvFault_low;
	double	MFault_xirv;
	double	MFault_nirv;
	double	MFault_hirv;
	double  MFault_lirv;
	double	MLPIni;
	double  MLPIndex;
	double	MSteadOptim;
	double	MR;
	double	MRMax;
	double  MP;
    double	MPL;
	double	MPMax;
	double	MDelR;
    double  MDelP;
	double	MRR;
	double	MRP;
	int		MSCT;
	BOOL	RMode;
	BOOL	SMode;
	BOOL	ErrMode;
	int	MPriority;
	BOOL	MValidate;				//0--------Validate
											//1--------detected MV
								 //2--------undetected MV
	BOOL	MAlarm;
	BOOL	MAM;			
	BOOL	MRBroken;
	BOOL	MPBroken;
	int	MFault_Priority;
	int	MCT_history;
	BOOL	MConstrain_type;
	BOOL	MRDirty;
	BOOL	MPDirty;
	BOOL	MSDirty;
    BOOL	MIRVWrong;
	double	MHistory;
}VARMV,* LPVARMV;
