#ifdef  __MTEXPORT_
#define __MTEXPORT_
DLLEXPORT MTHANDLE ReadCvMember(LPVARCV _lp_VarCv, WORD _i_CvIndex, WORD _i_ttfID);
DLLEXPORT MTHANDLE ReadDvMember(LPVARDV _lp_VarDv, WORD _i_DvIndex, WORD _i_ttfID);
DLLEXPORT MTHANDLE ReadMvMember(LPVARMV _lp_VarMv, WORD _i_MvIndex, WORD _i_ttfID);
DLLEXPORT MTHANDLE ReadGvMember(LPCOBJPRIV _lpCobj_Priv,WORD _i_ttfID);

DLLEXPORT MTHANDLE ReadCvAllMember( LPVARCV _lp_VarCv, WORD _i_CvIndex );
DLLEXPORT MTHANDLE ReadDvAllMember( LPVARDV _lp_VarDv, WORD _i_DvIndex );
DLLEXPORT MTHANDLE ReadMvAllMember( LPVARMV _lp_VarMv, WORD _i_MvIndex );
DLLEXPORT MTHANDLE ReadGvAllMember(LPCOBJPRIV _lp_CobjPriv);

DLLEXPORT MTHANDLE WriteCvMember(LPVARCV _lp_VarCv,WORD _i_CvIndex,WORD _i_ttfID);
DLLEXPORT MTHANDLE WriteDvMember(LPVARDV _lp_VarDv,WORD _i_DvIndex,WORD _i_ttfID);
DLLEXPORT MTHANDLE WriteMvMember(LPVARMV _lp_VarMv,WORD _i_MvIndex,WORD _i_ttfID);
DLLEXPORT MTHANDLE WriteGvMember(LPCOBJPRIV _lpCobj_Priv,WORD _i_ttfID);

DLLEXPORT MTHANDLE WriteCvAllMember( LPVARCV _lp_VarCv, WORD _i_CvIndex );
DLLEXPORT MTHANDLE WriteDvAllMember( LPVARDV _lp_VarDv, WORD _i_DvIndex );
DLLEXPORT MTHANDLE WriteMvAllMember( LPVARMV _lp_VarMv, WORD _i_MvIndex );
DLLEXPORT MTHANDLE WriteGvAllMember( LPCOBJPRIV _lp_CobjPriv );


DLLEXPORT MTHANDLE ReadDataFromOnspec( WORD _i_UnitIndex, double *_lpData);
DLLEXPORT MTHANDLE WriteDataToOnspec( WORD _i_UnitIndex, double *_lpData);

DLLEXPORT MTHANDLE ReadTagDataFromOnspec( char *_cTag, double *_lpData)
DLLEXPORT MTHANDLE WriteTagDataToOnspec( char *_cTag, double *_lpData)

#endif