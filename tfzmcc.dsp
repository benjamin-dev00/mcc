# Microsoft Developer Studio Project File - Name="zmcc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=zmcc - Win32 Run
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "tfzmcc.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "tfzmcc.mak" CFG="zmcc - Win32 Run"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "zmcc - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "zmcc - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "zmcc - Win32 ONSPEC Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "zmcc - Win32 ONSPEC Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "zmcc - Win32 Simul" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "zmcc - Win32 ONSPEC Simul" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "zmcc - Win32 Dynamics Runtime" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "zmcc - Win32 Dynamics" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "zmcc - Win32 Run" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/MCC/CurrentMCC/zmcc", GGFAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "zmcc - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "\sjzmcc"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "__ZZK" /Yu"tfalgo.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib corebase.lib zsyslib.lib /nologo /subsystem:windows /dll /machine:I386 /out:"tfs_zmcc.dll"

!ELSEIF  "$(CFG)" == "zmcc - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "__Simul" /D "__ONSPEC" /FR /Yu"tfalgo.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib corebase.lib zsyslib.lib mtonsmcc.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"Debug\tfs_mcc.dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "zmcc - Win32 ONSPEC Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "zmcc___Win32_ONSPEC_Debug"
# PROP BASE Intermediate_Dir "zmcc___Win32_ONSPEC_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "\mcc\debug"
# PROP Intermediate_Dir "debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "__ZZK" /D "__Simul" /FR /Yu"tfalgo.h" /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "__ONSPEC" /FR /Yu"tfalgo.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib \sjzmcc\corebase.lib \sjzmcc\zsyslib.lib \sjzmcc\corealgo.lib mtonsmcc\mtonsmcc.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"\sjzmcc/tfzmcc.dll" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib \core\debug\corebase.lib \core\debug\zsyslib.lib \core\debug\corealgo.lib \mcc\debug\mtonsmcc.lib /nologo /subsystem:windows /dll /profile /debug /machine:I386 /out:"\mcc\debug/tfr_mcc.dll"

!ELSEIF  "$(CFG)" == "zmcc - Win32 ONSPEC Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "zmcc___Win32_ONSPEC_Release"
# PROP BASE Intermediate_Dir "zmcc___Win32_ONSPEC_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "zmcc___Win32_ONSPEC_Release"
# PROP Intermediate_Dir "zmcc___Win32_ONSPEC_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386

!ELSEIF  "$(CFG)" == "zmcc - Win32 Simul"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "zmcc___Win32_Simul"
# PROP BASE Intermediate_Dir "zmcc___Win32_Simul"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "\mcc\debug"
# PROP Intermediate_Dir "debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "__ZZK" /D "__Simul" /FR /Yu"tfalgo.h" /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "__ZZK" /D "__Simul" /D "__ONSPEC" /FR /Yu"tfalgo.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib \sjzmcc\corebase.lib \sjzmcc\zsyslib.lib \sjzmcc\corealgo.lib mtonsmcc\mtonsmcc.lib /nologo /subsystem:windows /dll /profile /debug /machine:I386 /out:"\sjzmcc/tfzmcc.dll"
# ADD LINK32 Dyndbio.dll kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib \core\debug\corebase.lib \core\debug\zsyslib.lib \core\debug\corealgo.lib \mcc\debug\mtonsmcc.lib /nologo /subsystem:windows /dll /profile /debug /machine:I386 /out:"\mcc\debug/tfs_mcc.dll"

!ELSEIF  "$(CFG)" == "zmcc - Win32 ONSPEC Simul"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "zmcc___Win32_ONSPEC_Simul"
# PROP BASE Intermediate_Dir "zmcc___Win32_ONSPEC_Simul"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "\mcc\debug"
# PROP Intermediate_Dir "debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "__ONSPEC" /D "__Simul" /FR /Yu"tfalgo.h" /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "__ONSPEC" /D "__Simul" /D "__FIX" /D "__debug" /FR /Yu"tfalgo.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib \sjzmcc\corebase.lib \sjzmcc\zsyslib.lib \sjzmcc\corealgo.lib mtonsmcc\mtonsmcc.lib /nologo /subsystem:windows /dll /profile /debug /machine:I386 /out:"\sjzmcc/tfmcc.dll"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib corebase.lib zsyslib.lib corealgo.lib mtonsmcc.lib DynDBio.lib /nologo /subsystem:windows /dll /profile /debug /machine:I386 /out:"\mcc/tfs_mcc.dll" /implib:"\mcc/tfs_mcc.lib"

!ELSEIF  "$(CFG)" == "zmcc - Win32 Dynamics Runtime"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "zmcc___Win32_Dynamics_Runtime"
# PROP BASE Intermediate_Dir "zmcc___Win32_Dynamics_Runtime"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug1"
# PROP Intermediate_Dir "zmcc_Dynamics_Runtime"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "__ONSPEC" /D "__Simul" /D "__FIX" /D "__debug" /FR /Yu"tfalgo.h" /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "__ONSPEC" /D "__FIX" /D "__debug" /FR /Yu"tfalgo.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib corebase.lib zsyslib.lib corealgo.lib mtonsmcc.lib DynDBio.lib /nologo /subsystem:windows /dll /profile /debug /machine:I386 /out:"\mcc/tfs_mcc.dll" /implib:"\mcc/tfs_mcc.lib"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib corebase.lib zsyslib.lib corealgo.lib mtonsmcc.lib DynDBio.lib /nologo /subsystem:windows /dll /profile /debug /machine:I386 /out:"Debug1\tfs_mcc.dll"

!ELSEIF  "$(CFG)" == "zmcc - Win32 Dynamics"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "zmcc___Win32_Dynamics"
# PROP BASE Intermediate_Dir "zmcc___Win32_Dynamics"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_zmcc_Fix"
# PROP Intermediate_Dir "Debug_zmcc_Fix"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "__ONSPEC" /D "__FIX" /D "__debug" /FR /Yu"tfalgo.h" /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "__ONSPEC" /D "__FIX" /D "__Simul" /FR /Yu"tfalgo.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib corebase.lib zsyslib.lib corealgo.lib mtonsmcc.lib DynDBio.lib /nologo /subsystem:windows /dll /profile /debug /machine:I386 /out:"\mcc/tfs_mcc.dll" /implib:"\mcc/tfs_mcc.lib"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib corebase.lib zsyslib.lib corealgo.lib mtonsmcc.lib DynDBio.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"Debug\tfs_mcc.dll" /implib:"Debug\tfs_mcc.lib" /libpath:"Debug"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "zmcc - Win32 Run"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "zmcc___Win32_Run"
# PROP BASE Intermediate_Dir "zmcc___Win32_Run"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Run_Debug"
# PROP Intermediate_Dir "zmcc___Win32_Run"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "__Simul" /D "__ONSPEC" /FR /Yu"tfalgo.h" /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "__ONSPEC" /D "__Simul_Matlab" /FR /Yu"tfalgo.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib corebase.lib zsyslib.lib mtonsmcc.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"\mcc\debug/tfs_mcc.dll" /implib:"\Workstation\Lib/tfs_mcc.lib"
# SUBTRACT BASE LINK32 /profile /map
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib corebase.lib zsyslib.lib mtonsmcc.lib onworld.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"Run_Debug\tfr_mcc.dll"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "zmcc - Win32 Release"
# Name "zmcc - Win32 Debug"
# Name "zmcc - Win32 ONSPEC Debug"
# Name "zmcc - Win32 ONSPEC Release"
# Name "zmcc - Win32 Simul"
# Name "zmcc - Win32 ONSPEC Simul"
# Name "zmcc - Win32 Dynamics Runtime"
# Name "zmcc - Win32 Dynamics"
# Name "zmcc - Win32 Run"
# Begin Group "Resouce"

# PROP Default_Filter ".rc"
# Begin Source File

SOURCE=.\tfdlg.rc
# End Source File
# Begin Source File

SOURCE=.\Tfg_ffrd.c
# End Source File
# End Group
# Begin Group "Source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\tfadshll.c
# End Source File
# Begin Source File

SOURCE=.\Tfalgo.c
# End Source File
# Begin Source File

SOURCE=.\tfapasr.c
# End Source File
# Begin Source File

SOURCE=.\Tfb_cv.c
# End Source File
# Begin Source File

SOURCE=.\Tfb_mve.c
# End Source File
# Begin Source File

SOURCE=.\Tfb_mvp.c
# End Source File
# Begin Source File

SOURCE=.\Tfc_cv.c
# End Source File
# Begin Source File

SOURCE=.\Tfc_mv.c
# End Source File
# Begin Source File

SOURCE=.\Tfc_para.c
# End Source File
# Begin Source File

SOURCE=.\Tfc_pred.c
# End Source File
# Begin Source File

SOURCE=.\Tfc_y.c
# End Source File
# Begin Source File

SOURCE=.\Tfclnkm.c
# End Source File
# Begin Source File

SOURCE=.\Tfcm.c
# End Source File
# Begin Source File

SOURCE=.\tfcomplp.c
# End Source File
# Begin Source File

SOURCE=.\Tfcond.c
# End Source File
# Begin Source File

SOURCE=.\tfcorstd.c
# End Source File
# Begin Source File

SOURCE=.\Tfcr_mcd.c
# End Source File
# Begin Source File

SOURCE=.\tfd_2_in.c
# End Source File
# Begin Source File

SOURCE=.\Tfdelt.c
# End Source File
# Begin Source File

SOURCE=.\tfdisply.c
# End Source File
# Begin Source File

SOURCE=.\Tfdyopti.c
# End Source File
# Begin Source File

SOURCE=.\tfequt.c
# End Source File
# Begin Source File

SOURCE=.\TFexit.c
# End Source File
# Begin Source File

SOURCE=.\Tff_mv.c
# End Source File
# Begin Source File

SOURCE=.\tff_o_sr.c
# End Source File
# Begin Source File

SOURCE=.\Tfflout.c
# End Source File
# Begin Source File

SOURCE=.\Tfflsr.c
# End Source File
# Begin Source File

SOURCE=.\Tffr_cv.c
# End Source File
# Begin Source File

SOURCE=.\Tffr_dv.c
# End Source File
# Begin Source File

SOURCE=.\Tffr_mv.c
# End Source File
# Begin Source File

SOURCE=.\tffr_osr.c
# End Source File
# Begin Source File

SOURCE=.\tffr_sr.c
# End Source File
# Begin Source File

SOURCE=.\Tffreecv.c
# End Source File
# Begin Source File

SOURCE=.\Tffw_cv.c
# End Source File
# Begin Source File

SOURCE=.\Tffw_dv.c
# End Source File
# Begin Source File

SOURCE=.\Tffw_mv.c
# End Source File
# Begin Source File

SOURCE=.\tfg_aprt.c
# End Source File
# Begin Source File

SOURCE=.\Tfg_free.c
# End Source File
# Begin Source File

SOURCE=.\Tfg_h.c
# End Source File
# Begin Source File

SOURCE=.\Tfg_lpg.c
# End Source File
# Begin Source File

SOURCE=.\tfg_prev.c
# End Source File
# Begin Source File

SOURCE=.\Tfg_sr.c
# End Source File
# Begin Source File

SOURCE=.\Tfgini.c
# End Source File
# Begin Source File

SOURCE=.\Tfillcon.c
# End Source File
# Begin Source File

SOURCE=.\Tfin_2_d.c
# End Source File
# Begin Source File

SOURCE=.\Tfini.c
# End Source File
# Begin Source File

SOURCE=.\Tfinimcc.c
# End Source File
# Begin Source File

SOURCE=.\Tfis_re.c
# End Source File
# Begin Source File

SOURCE=.\Tfis_yov.c
# End Source File
# Begin Source File

SOURCE=.\Tflm.c
# End Source File
# Begin Source File

SOURCE=.\Tflog.c
# End Source File
# Begin Source File

SOURCE=.\tflpdbg.c
# End Source File
# Begin Source File

SOURCE=.\Tflpini.c

!IF  "$(CFG)" == "zmcc - Win32 Release"

!ELSEIF  "$(CFG)" == "zmcc - Win32 Debug"

# ADD CPP /Yu"tfalgo.h"

!ELSEIF  "$(CFG)" == "zmcc - Win32 ONSPEC Debug"

# ADD BASE CPP /Yu"tfalgo.h"
# ADD CPP /Yu"tfalgo.h"

!ELSEIF  "$(CFG)" == "zmcc - Win32 ONSPEC Release"

!ELSEIF  "$(CFG)" == "zmcc - Win32 Simul"

# ADD BASE CPP /Yu"tfalgo.h"
# ADD CPP /Yu"tfalgo.h"

!ELSEIF  "$(CFG)" == "zmcc - Win32 ONSPEC Simul"

# ADD BASE CPP /Yu"tfalgo.h"
# ADD CPP /Yu"tfalgo.h"

!ELSEIF  "$(CFG)" == "zmcc - Win32 Dynamics Runtime"

# ADD BASE CPP /Yu"tfalgo.h"
# ADD CPP /Yu"tfalgo.h"

!ELSEIF  "$(CFG)" == "zmcc - Win32 Dynamics"

# ADD BASE CPP /Yu"tfalgo.h"
# ADD CPP /Yu"tfalgo.h"

!ELSEIF  "$(CFG)" == "zmcc - Win32 Run"

# ADD BASE CPP /Yu"tfalgo.h"
# ADD CPP /Yu"tfalgo.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Tflppara.c
# End Source File
# Begin Source File

SOURCE=.\Tfm_para.c
# End Source File
# Begin Source File

SOURCE=.\Tfmcc.c
# End Source File
# Begin Source File

SOURCE=.\tfmccp.c
# End Source File
# Begin Source File

SOURCE=.\Tfmcvstr.c
# End Source File
# Begin Source File

SOURCE=.\tfmerge.c
# End Source File
# Begin Source File

SOURCE=.\Tfnonbtn.c
# End Source File
# Begin Source File

SOURCE=.\Tfobj.c
# End Source File
# Begin Source File

SOURCE=.\tfonpre.c
# End Source File
# Begin Source File

SOURCE=.\tfpower.c
# End Source File
# Begin Source File

SOURCE=.\tfpreh.c

!IF  "$(CFG)" == "zmcc - Win32 Release"

# ADD CPP /Yc"tfalgo.h"

!ELSEIF  "$(CFG)" == "zmcc - Win32 Debug"

# ADD CPP /Yc"tfalgo.h"

!ELSEIF  "$(CFG)" == "zmcc - Win32 ONSPEC Debug"

# ADD BASE CPP /Yc"tfalgo.h"
# ADD CPP /Yc"tfalgo.h"

!ELSEIF  "$(CFG)" == "zmcc - Win32 ONSPEC Release"

!ELSEIF  "$(CFG)" == "zmcc - Win32 Simul"

# ADD BASE CPP /Yc"tfalgo.h"
# ADD CPP /Yc"tfalgo.h"

!ELSEIF  "$(CFG)" == "zmcc - Win32 ONSPEC Simul"

# ADD BASE CPP /Yc"tfalgo.h"
# ADD CPP /Yc"tfalgo.h"

!ELSEIF  "$(CFG)" == "zmcc - Win32 Dynamics Runtime"

# ADD BASE CPP /Yc"tfalgo.h"
# ADD CPP /Yc"tfalgo.h"

!ELSEIF  "$(CFG)" == "zmcc - Win32 Dynamics"

# ADD BASE CPP /Yc"tfalgo.h"
# ADD CPP /Yc"tfalgo.h"

!ELSEIF  "$(CFG)" == "zmcc - Win32 Run"

# ADD BASE CPP /Yc"tfalgo.h"
# ADD CPP /Yc"tfalgo.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Tfr_algo.c
# End Source File
# Begin Source File

SOURCE=.\Tfr_all.c
# End Source File
# Begin Source File

SOURCE=.\Tfr_file.c
# End Source File
# Begin Source File

SOURCE=.\Tfr_mcv.c
# End Source File
# Begin Source File

SOURCE=.\Tfr_scal.c
# End Source File
# Begin Source File

SOURCE=.\Tfreccv.c
# End Source File
# Begin Source File

SOURCE=.\Tfrm_ill.c
# End Source File
# Begin Source File

SOURCE=.\Tfs_cv.c
# End Source File
# Begin Source File

SOURCE=.\Tfs_mv.c
# End Source File
# Begin Source File

SOURCE=.\Tfseq.c
# End Source File
# Begin Source File

SOURCE=.\tfsetgp.c
# End Source File
# Begin Source File

SOURCE=.\Tfsqp.c
# End Source File
# Begin Source File

SOURCE=.\tfsteady.c
# End Source File
# Begin Source File

SOURCE=.\Tfstop.c
# End Source File
# Begin Source File

SOURCE=.\Tfsvd.c
# End Source File
# Begin Source File

SOURCE=.\tfundelt.c
# End Source File
# Begin Source File

SOURCE=.\Tfunit.c
# End Source File
# Begin Source File

SOURCE=.\Tfununit.c
# End Source File
# Begin Source File

SOURCE=.\tfw_cvs.c
# End Source File
# Begin Source File

SOURCE=.\tfw_dvs.c
# End Source File
# Begin Source File

SOURCE=.\Tfw_file.c
# End Source File
# Begin Source File

SOURCE=.\Tfw_mcv.c
# End Source File
# Begin Source File

SOURCE=.\tfw_mvs.c
# End Source File
# Begin Source File

SOURCE=.\tfw_subg.c
# End Source File
# Begin Source File

SOURCE=.\Tfwadd.c
# End Source File
# Begin Source File

SOURCE=.\Tfwbwdsu.c
# End Source File
# Begin Source File

SOURCE=.\Tfwchkop.c
# End Source File
# Begin Source File

SOURCE=.\Tfwchode.c
# End Source File
# Begin Source File

SOURCE=.\Tfwdrop.c
# End Source File
# Begin Source File

SOURCE=.\Tfwfwdsu.c
# End Source File
# Begin Source File

SOURCE=.\Tfwout.c
# End Source File
# Begin Source File

SOURCE=.\Tfwqpsol.c
# End Source File
# Begin Source File

SOURCE=.\Tfwqrdec.c
# End Source File
# Begin Source File

SOURCE=.\Tfwrtest.c
# End Source File
# Begin Source File

SOURCE=.\tfzinip.c
# End Source File
# Begin Source File

SOURCE=.\tobereplaced.txt
# End Source File
# Begin Source File

SOURCE=.\Onworld.lib
# End Source File
# End Group
# Begin Source File

SOURCE=.\cursor1.cur
# End Source File
# End Target
# End Project
