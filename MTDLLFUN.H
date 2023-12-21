//Mtoutfun.h
#ifndef _MTDLLFUN_H
#define _MTDLLFUN_H

int  ReadGvMember( LPCOBJPRIV _lpCobjPriv,WORD _MemIndex );
int  ReadCvMember( LPVARCV _lpVarCv, WORD _CvIndex, WORD _CvMemIndex );
int  ReadDvMember( LPVARDV _lpVarDv, WORD _DvIndex, WORD _DvMemIndex );
int  ReadMvMember( LPVARMV _lpVarMv, WORD _MvIndex, WORD _MvMemIndex );

int  WriteGvMember( LPCOBJPRIV _lpCobjPriv,WORD _MemIndex );
int  WriteCvMember( LPVARCV _lpVarCv, WORD _CvIndex, WORD _CvMemIndex );
int  WriteMvMember( LPVARMV _lpVarDv, WORD _DvIndex, WORD _DvMemIndex );
int  WriteDvMember( LPVARDV _lpVarMv, WORD _MvIndex, WORD _MvMemIndex );

int  ReadGvAllMember( LPCOBJPRIV _lpCobjPriv );
int  ReadCvAllMember( LPVARCV _lpVarCv, WORD _CvMemIndex );
int  ReadDvAllMember( LPVARDV _lpVarDv, WORD _DvMemIndex );
int  ReadMvAllMember( LPVARMV _lpVarMv, WORD _MvMemIndex );

int  WriteGvAllMember( LPCOBJPRIV _lpCobjPriv );
int  WriteCvAllMember( LPVARCV _lpVarCv, WORD _CvMemIndex );
int  WriteDvAllMember( LPVARDV _lpVarDv, WORD _DvMemIndex );
int  WriteMvAllMember( LPVARMV _lpVarMv, WORD _MvMemIndex );

#endif
