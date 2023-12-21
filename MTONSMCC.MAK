# Microsoft Developer Studio Generated NMAKE File, Based on Mtonsmcc.dsp
!IF "$(CFG)" == ""
CFG=Mtonsmcc - Win32 Debug
!MESSAGE No configuration specified. Defaulting to Mtonsmcc - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Mtonsmcc - Win32 Release" && "$(CFG)" != "Mtonsmcc - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Mtonsmcc.mak" CFG="Mtonsmcc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Mtonsmcc - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Mtonsmcc - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "Mtonsmcc - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\Mtonsmcc.dll" "$(OUTDIR)\Mtonsmcc.bsc"


CLEAN :
	-@erase "$(INTDIR)\MtIdchec.obj"
	-@erase "$(INTDIR)\MtIdchec.sbr"
	-@erase "$(INTDIR)\Mtidtran.obj"
	-@erase "$(INTDIR)\Mtidtran.sbr"
	-@erase "$(INTDIR)\Mtiniall.obj"
	-@erase "$(INTDIR)\Mtiniall.sbr"
	-@erase "$(INTDIR)\Mtrdcvme.obj"
	-@erase "$(INTDIR)\Mtrdcvme.sbr"
	-@erase "$(INTDIR)\Mtrdcvst.obj"
	-@erase "$(INTDIR)\Mtrdcvst.sbr"
	-@erase "$(INTDIR)\Mtrddata.obj"
	-@erase "$(INTDIR)\Mtrddata.sbr"
	-@erase "$(INTDIR)\Mtrddvme.obj"
	-@erase "$(INTDIR)\Mtrddvme.sbr"
	-@erase "$(INTDIR)\Mtrddvst.obj"
	-@erase "$(INTDIR)\Mtrddvst.sbr"
	-@erase "$(INTDIR)\Mtrdgvme.obj"
	-@erase "$(INTDIR)\Mtrdgvme.sbr"
	-@erase "$(INTDIR)\Mtrdgvst.obj"
	-@erase "$(INTDIR)\Mtrdgvst.sbr"
	-@erase "$(INTDIR)\Mtrdmvme.obj"
	-@erase "$(INTDIR)\Mtrdmvme.sbr"
	-@erase "$(INTDIR)\Mtrdmvst.obj"
	-@erase "$(INTDIR)\Mtrdmvst.sbr"
	-@erase "$(INTDIR)\Mtrdtagd.obj"
	-@erase "$(INTDIR)\Mtrdtagd.sbr"
	-@erase "$(INTDIR)\Mtwtcvme.obj"
	-@erase "$(INTDIR)\Mtwtcvme.sbr"
	-@erase "$(INTDIR)\Mtwtcvst.obj"
	-@erase "$(INTDIR)\Mtwtcvst.sbr"
	-@erase "$(INTDIR)\Mtwtdata.obj"
	-@erase "$(INTDIR)\Mtwtdata.sbr"
	-@erase "$(INTDIR)\Mtwtdvme.obj"
	-@erase "$(INTDIR)\Mtwtdvme.sbr"
	-@erase "$(INTDIR)\Mtwtdvst.obj"
	-@erase "$(INTDIR)\Mtwtdvst.sbr"
	-@erase "$(INTDIR)\Mtwtgvme.obj"
	-@erase "$(INTDIR)\Mtwtgvme.sbr"
	-@erase "$(INTDIR)\Mtwtgvst.obj"
	-@erase "$(INTDIR)\Mtwtgvst.sbr"
	-@erase "$(INTDIR)\Mtwtmvme.obj"
	-@erase "$(INTDIR)\Mtwtmvme.sbr"
	-@erase "$(INTDIR)\Mtwtmvst.obj"
	-@erase "$(INTDIR)\Mtwtmvst.sbr"
	-@erase "$(INTDIR)\Mtwttagd.obj"
	-@erase "$(INTDIR)\Mtwttagd.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\Mtonsmcc.bsc"
	-@erase "$(OUTDIR)\Mtonsmcc.dll"
	-@erase "$(OUTDIR)\Mtonsmcc.exp"
	-@erase "$(OUTDIR)\Mtonsmcc.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Mtonsmcc.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32 
RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Mtonsmcc.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\MtIdchec.sbr" \
	"$(INTDIR)\Mtidtran.sbr" \
	"$(INTDIR)\Mtiniall.sbr" \
	"$(INTDIR)\Mtrdcvme.sbr" \
	"$(INTDIR)\Mtrdcvst.sbr" \
	"$(INTDIR)\Mtrddata.sbr" \
	"$(INTDIR)\Mtrddvme.sbr" \
	"$(INTDIR)\Mtrddvst.sbr" \
	"$(INTDIR)\Mtrdgvme.sbr" \
	"$(INTDIR)\Mtrdgvst.sbr" \
	"$(INTDIR)\Mtrdmvme.sbr" \
	"$(INTDIR)\Mtrdmvst.sbr" \
	"$(INTDIR)\Mtrdtagd.sbr" \
	"$(INTDIR)\Mtwtcvme.sbr" \
	"$(INTDIR)\Mtwtcvst.sbr" \
	"$(INTDIR)\Mtwtdata.sbr" \
	"$(INTDIR)\Mtwtdvme.sbr" \
	"$(INTDIR)\Mtwtdvst.sbr" \
	"$(INTDIR)\Mtwtgvme.sbr" \
	"$(INTDIR)\Mtwtgvst.sbr" \
	"$(INTDIR)\Mtwtmvme.sbr" \
	"$(INTDIR)\Mtwtmvst.sbr" \
	"$(INTDIR)\Mtwttagd.sbr"

"$(OUTDIR)\Mtonsmcc.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\Mtonsmcc.pdb" /machine:I386 /out:"$(OUTDIR)\Mtonsmcc.dll" /implib:"$(OUTDIR)\Mtonsmcc.lib" 
LINK32_OBJS= \
	"$(INTDIR)\MtIdchec.obj" \
	"$(INTDIR)\Mtidtran.obj" \
	"$(INTDIR)\Mtiniall.obj" \
	"$(INTDIR)\Mtrdcvme.obj" \
	"$(INTDIR)\Mtrdcvst.obj" \
	"$(INTDIR)\Mtrddata.obj" \
	"$(INTDIR)\Mtrddvme.obj" \
	"$(INTDIR)\Mtrddvst.obj" \
	"$(INTDIR)\Mtrdgvme.obj" \
	"$(INTDIR)\Mtrdgvst.obj" \
	"$(INTDIR)\Mtrdmvme.obj" \
	"$(INTDIR)\Mtrdmvst.obj" \
	"$(INTDIR)\Mtrdtagd.obj" \
	"$(INTDIR)\Mtwtcvme.obj" \
	"$(INTDIR)\Mtwtcvst.obj" \
	"$(INTDIR)\Mtwtdata.obj" \
	"$(INTDIR)\Mtwtdvme.obj" \
	"$(INTDIR)\Mtwtdvst.obj" \
	"$(INTDIR)\Mtwtgvme.obj" \
	"$(INTDIR)\Mtwtgvst.obj" \
	"$(INTDIR)\Mtwtmvme.obj" \
	"$(INTDIR)\Mtwtmvst.obj" \
	"$(INTDIR)\Mtwttagd.obj" \
	".\Onworld.lib"

"$(OUTDIR)\Mtonsmcc.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Mtonsmcc - Win32 Debug"

OUTDIR=.\Mtonsmcc
INTDIR=.\Mtonsmcc
# Begin Custom Macros
OutDir=.\Mtonsmcc
# End Custom Macros

ALL : "..\MCC\DEBUG\Mtonsmcc.dll" "$(OUTDIR)\Mtonsmcc.bsc"


CLEAN :
	-@erase "$(INTDIR)\MtIdchec.obj"
	-@erase "$(INTDIR)\MtIdchec.sbr"
	-@erase "$(INTDIR)\Mtidtran.obj"
	-@erase "$(INTDIR)\Mtidtran.sbr"
	-@erase "$(INTDIR)\Mtiniall.obj"
	-@erase "$(INTDIR)\Mtiniall.sbr"
	-@erase "$(INTDIR)\Mtonsmcc.pch"
	-@erase "$(INTDIR)\Mtrdcvme.obj"
	-@erase "$(INTDIR)\Mtrdcvme.sbr"
	-@erase "$(INTDIR)\Mtrdcvst.obj"
	-@erase "$(INTDIR)\Mtrdcvst.sbr"
	-@erase "$(INTDIR)\Mtrddata.obj"
	-@erase "$(INTDIR)\Mtrddata.sbr"
	-@erase "$(INTDIR)\Mtrddvme.obj"
	-@erase "$(INTDIR)\Mtrddvme.sbr"
	-@erase "$(INTDIR)\Mtrddvst.obj"
	-@erase "$(INTDIR)\Mtrddvst.sbr"
	-@erase "$(INTDIR)\Mtrdgvme.obj"
	-@erase "$(INTDIR)\Mtrdgvme.sbr"
	-@erase "$(INTDIR)\Mtrdgvst.obj"
	-@erase "$(INTDIR)\Mtrdgvst.sbr"
	-@erase "$(INTDIR)\Mtrdmvme.obj"
	-@erase "$(INTDIR)\Mtrdmvme.sbr"
	-@erase "$(INTDIR)\Mtrdmvst.obj"
	-@erase "$(INTDIR)\Mtrdmvst.sbr"
	-@erase "$(INTDIR)\Mtrdtagd.obj"
	-@erase "$(INTDIR)\Mtrdtagd.sbr"
	-@erase "$(INTDIR)\Mtwtcvme.obj"
	-@erase "$(INTDIR)\Mtwtcvme.sbr"
	-@erase "$(INTDIR)\Mtwtcvst.obj"
	-@erase "$(INTDIR)\Mtwtcvst.sbr"
	-@erase "$(INTDIR)\Mtwtdata.obj"
	-@erase "$(INTDIR)\Mtwtdata.sbr"
	-@erase "$(INTDIR)\Mtwtdvme.obj"
	-@erase "$(INTDIR)\Mtwtdvme.sbr"
	-@erase "$(INTDIR)\Mtwtdvst.obj"
	-@erase "$(INTDIR)\Mtwtdvst.sbr"
	-@erase "$(INTDIR)\Mtwtgvme.obj"
	-@erase "$(INTDIR)\Mtwtgvme.sbr"
	-@erase "$(INTDIR)\Mtwtgvst.obj"
	-@erase "$(INTDIR)\Mtwtgvst.sbr"
	-@erase "$(INTDIR)\Mtwtmvme.obj"
	-@erase "$(INTDIR)\Mtwtmvme.sbr"
	-@erase "$(INTDIR)\Mtwtmvst.obj"
	-@erase "$(INTDIR)\Mtwtmvst.sbr"
	-@erase "$(INTDIR)\Mtwttagd.obj"
	-@erase "$(INTDIR)\Mtwttagd.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\Mtonsmcc.bsc"
	-@erase "$(OUTDIR)\Mtonsmcc.exp"
	-@erase "$(OUTDIR)\Mtonsmcc.lib"
	-@erase "$(OUTDIR)\Mtonsmcc.pdb"
	-@erase "..\MCC\DEBUG\Mtonsmcc.dll"
	-@erase "..\MCC\DEBUG\Mtonsmcc.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Mtonsmcc.pch" /Yu"tfalgo.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32 
RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Mtonsmcc.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\MtIdchec.sbr" \
	"$(INTDIR)\Mtidtran.sbr" \
	"$(INTDIR)\Mtiniall.sbr" \
	"$(INTDIR)\Mtrdcvme.sbr" \
	"$(INTDIR)\Mtrdcvst.sbr" \
	"$(INTDIR)\Mtrddata.sbr" \
	"$(INTDIR)\Mtrddvme.sbr" \
	"$(INTDIR)\Mtrddvst.sbr" \
	"$(INTDIR)\Mtrdgvme.sbr" \
	"$(INTDIR)\Mtrdgvst.sbr" \
	"$(INTDIR)\Mtrdmvme.sbr" \
	"$(INTDIR)\Mtrdmvst.sbr" \
	"$(INTDIR)\Mtrdtagd.sbr" \
	"$(INTDIR)\Mtwtcvme.sbr" \
	"$(INTDIR)\Mtwtcvst.sbr" \
	"$(INTDIR)\Mtwtdata.sbr" \
	"$(INTDIR)\Mtwtdvme.sbr" \
	"$(INTDIR)\Mtwtdvst.sbr" \
	"$(INTDIR)\Mtwtgvme.sbr" \
	"$(INTDIR)\Mtwtgvst.sbr" \
	"$(INTDIR)\Mtwtmvme.sbr" \
	"$(INTDIR)\Mtwtmvst.sbr" \
	"$(INTDIR)\Mtwttagd.sbr"

"$(OUTDIR)\Mtonsmcc.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\Mtonsmcc.pdb" /debug /machine:I386 /out:"\MCC\DEBUG\Mtonsmcc.dll" /implib:"$(OUTDIR)\Mtonsmcc.lib" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\MtIdchec.obj" \
	"$(INTDIR)\Mtidtran.obj" \
	"$(INTDIR)\Mtiniall.obj" \
	"$(INTDIR)\Mtrdcvme.obj" \
	"$(INTDIR)\Mtrdcvst.obj" \
	"$(INTDIR)\Mtrddata.obj" \
	"$(INTDIR)\Mtrddvme.obj" \
	"$(INTDIR)\Mtrddvst.obj" \
	"$(INTDIR)\Mtrdgvme.obj" \
	"$(INTDIR)\Mtrdgvst.obj" \
	"$(INTDIR)\Mtrdmvme.obj" \
	"$(INTDIR)\Mtrdmvst.obj" \
	"$(INTDIR)\Mtrdtagd.obj" \
	"$(INTDIR)\Mtwtcvme.obj" \
	"$(INTDIR)\Mtwtcvst.obj" \
	"$(INTDIR)\Mtwtdata.obj" \
	"$(INTDIR)\Mtwtdvme.obj" \
	"$(INTDIR)\Mtwtdvst.obj" \
	"$(INTDIR)\Mtwtgvme.obj" \
	"$(INTDIR)\Mtwtgvst.obj" \
	"$(INTDIR)\Mtwtmvme.obj" \
	"$(INTDIR)\Mtwtmvst.obj" \
	"$(INTDIR)\Mtwttagd.obj" \
	".\Onworld.lib"

"..\MCC\DEBUG\Mtonsmcc.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Mtonsmcc.dep")
!INCLUDE "Mtonsmcc.dep"
!ELSE 
!MESSAGE Warning: cannot find "Mtonsmcc.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Mtonsmcc - Win32 Release" || "$(CFG)" == "Mtonsmcc - Win32 Debug"
SOURCE=.\MtIdchec.c

"$(INTDIR)\MtIdchec.obj"	"$(INTDIR)\MtIdchec.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Mtidtran.c

!IF  "$(CFG)" == "Mtonsmcc - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Mtonsmcc.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Mtidtran.obj"	"$(INTDIR)\Mtidtran.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Mtonsmcc - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Mtonsmcc.pch" /Yu"tfalgo.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Mtidtran.obj"	"$(INTDIR)\Mtidtran.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Mtonsmcc.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Mtiniall.c

"$(INTDIR)\Mtiniall.obj"	"$(INTDIR)\Mtiniall.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Mtrdcvme.c

!IF  "$(CFG)" == "Mtonsmcc - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Mtonsmcc.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Mtrdcvme.obj"	"$(INTDIR)\Mtrdcvme.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Mtonsmcc - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Mtonsmcc.pch" /Yc"tfalgo.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Mtrdcvme.obj"	"$(INTDIR)\Mtrdcvme.sbr"	"$(INTDIR)\Mtonsmcc.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Mtrdcvst.c

"$(INTDIR)\Mtrdcvst.obj"	"$(INTDIR)\Mtrdcvst.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Mtrddata.c

"$(INTDIR)\Mtrddata.obj"	"$(INTDIR)\Mtrddata.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Mtrddvme.c

"$(INTDIR)\Mtrddvme.obj"	"$(INTDIR)\Mtrddvme.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Mtrddvst.c

"$(INTDIR)\Mtrddvst.obj"	"$(INTDIR)\Mtrddvst.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Mtrdgvme.c

"$(INTDIR)\Mtrdgvme.obj"	"$(INTDIR)\Mtrdgvme.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Mtrdgvst.c

"$(INTDIR)\Mtrdgvst.obj"	"$(INTDIR)\Mtrdgvst.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Mtrdmvme.c

"$(INTDIR)\Mtrdmvme.obj"	"$(INTDIR)\Mtrdmvme.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Mtrdmvst.c

"$(INTDIR)\Mtrdmvst.obj"	"$(INTDIR)\Mtrdmvst.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Mtrdtagd.c

"$(INTDIR)\Mtrdtagd.obj"	"$(INTDIR)\Mtrdtagd.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Mtwtcvme.c

"$(INTDIR)\Mtwtcvme.obj"	"$(INTDIR)\Mtwtcvme.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Mtwtcvst.c

"$(INTDIR)\Mtwtcvst.obj"	"$(INTDIR)\Mtwtcvst.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Mtwtdata.c

"$(INTDIR)\Mtwtdata.obj"	"$(INTDIR)\Mtwtdata.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Mtwtdvme.c

"$(INTDIR)\Mtwtdvme.obj"	"$(INTDIR)\Mtwtdvme.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Mtwtdvst.c

"$(INTDIR)\Mtwtdvst.obj"	"$(INTDIR)\Mtwtdvst.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Mtwtgvme.c

"$(INTDIR)\Mtwtgvme.obj"	"$(INTDIR)\Mtwtgvme.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Mtwtgvst.c

"$(INTDIR)\Mtwtgvst.obj"	"$(INTDIR)\Mtwtgvst.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Mtwtmvme.c

"$(INTDIR)\Mtwtmvme.obj"	"$(INTDIR)\Mtwtmvme.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Mtwtmvst.c

"$(INTDIR)\Mtwtmvst.obj"	"$(INTDIR)\Mtwtmvst.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Mtwttagd.c

"$(INTDIR)\Mtwttagd.obj"	"$(INTDIR)\Mtwttagd.sbr" : $(SOURCE) "$(INTDIR)"



!ENDIF 

