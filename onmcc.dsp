# Microsoft Developer Studio Project File - Name="onmcc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=onmcc - Win32 Run
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "onmcc.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "onmcc.mak" CFG="onmcc - Win32 Run"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "onmcc - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "onmcc - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "onmcc - Win32 Simul" (based on "Win32 (x86) Application")
!MESSAGE "onmcc - Win32 Dynamics" (based on "Win32 (x86) Application")
!MESSAGE "onmcc - Win32 Dynamics Runtime" (based on "Win32 (x86) Application")
!MESSAGE "onmcc - Win32 Run" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/MCC/CurrentMCC/onmcc", ZAFAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "onmcc - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib \zmcc\zmcc___Win32_ONSPEC_Debug\tfmcc.lib /nologo /subsystem:windows /machine:I386 /out:"\sjzmcc/onmcc.exe"

!ELSEIF  "$(CFG)" == "onmcc - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "onmcc___"
# PROP BASE Intermediate_Dir "onmcc___"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "__Simul" /D "__ONSPEC" /FR /Yu /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib tfs_mcc.lib corebase.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Debug\s_mcc.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "onmcc - Win32 Simul"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "onmcc___Win32_Simul"
# PROP BASE Intermediate_Dir "onmcc___Win32_Simul"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "onmcc___Win32_Simul"
# PROP Intermediate_Dir "onmcc___Win32_Simul"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /FD /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "__ONSPEC" /D "__Simul" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib \mcc\debug/tfr_mcc.lib /nologo /subsystem:windows /debug /machine:I386 /out:"\sjzmcc/r_onmcc.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib tfs_mcc.lib dyndbio.lib /nologo /subsystem:windows /debug /machine:I386 /out:"\mcc/s_onmcc.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "onmcc - Win32 Dynamics"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "onmcc___Win32_Dynamics"
# PROP BASE Intermediate_Dir "onmcc___Win32_Dynamics"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_Onmcc_Fix"
# PROP Intermediate_Dir "Debug_Onmcc_Fix"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "__Simul" /D "__FIX" /FR /Yu /FD /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "__Simul" /D "__FIX" /D "__ONSPEC" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib tfs_mcc.lib /nologo /subsystem:windows /pdb:"\mcc\debug/r_onmcc.pdb" /debug /machine:I386 /out:"\MCC\Debug/s_mcc.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib tfs_mcc.lib DynDBio.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Debug\s_fixmcc.exe" /pdbtype:sept /libpath:"Debug"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "onmcc - Win32 Dynamics Runtime"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "onmcc___Win32_Dynamics_Runtime"
# PROP BASE Intermediate_Dir "onmcc___Win32_Dynamics_Runtime"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug1"
# PROP Intermediate_Dir "onmcc_Dynamics_Runtime"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "__Simul" /D "__FIX" /FR /Yu /FD /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "__FIX" /FR /Yu /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib tfs_mcc.lib DynDBio.lib /nologo /subsystem:windows /pdb:"\mcc\debug/r_onmcc.pdb" /debug /machine:I386 /out:"\MCC\Debug/s_fixmcc.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib tfs_mcc.lib DynDBio.lib /nologo /subsystem:windows /pdb:"Debug1\r_onmcc.pdb" /debug /machine:I386 /out:"Debug1\r_fixmcc.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "onmcc - Win32 Run"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "onmcc___Win32_Run"
# PROP BASE Intermediate_Dir "onmcc___Win32_Run"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Run_Debug"
# PROP Intermediate_Dir "onmcc___Win32_Run"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "__Simul" /D "__ONSPEC" /FR /Yu /FD /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "__ONSPEC" /FR /Yu /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib tfs_mcc.lib corebase.lib /nologo /subsystem:windows /pdb:"\mcc\debug/r_onmcc.pdb" /debug /machine:I386 /out:"\MCC\Debug/s_mcc.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib Run_Debug\tfr_mcc.lib corebase.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Run_Debug\R_mcc.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none /incremental:no

!ENDIF 

# Begin Target

# Name "onmcc - Win32 Release"
# Name "onmcc - Win32 Debug"
# Name "onmcc - Win32 Simul"
# Name "onmcc - Win32 Dynamics"
# Name "onmcc - Win32 Dynamics Runtime"
# Name "onmcc - Win32 Run"
# Begin Source File

SOURCE=.\onmcc.c

!IF  "$(CFG)" == "onmcc - Win32 Release"

!ELSEIF  "$(CFG)" == "onmcc - Win32 Debug"

# ADD CPP /Yu"tfalgo.h"

!ELSEIF  "$(CFG)" == "onmcc - Win32 Simul"

# ADD BASE CPP /Yu"tfalgo.h"
# ADD CPP /Yu"tfalgo.h"

!ELSEIF  "$(CFG)" == "onmcc - Win32 Dynamics"

# ADD BASE CPP /Yu"tfalgo.h"
# ADD CPP /Yu"tfalgo.h"

!ELSEIF  "$(CFG)" == "onmcc - Win32 Dynamics Runtime"

# ADD BASE CPP /Yu"tfalgo.h"
# ADD CPP /Yu"tfalgo.h"

!ELSEIF  "$(CFG)" == "onmcc - Win32 Run"

# ADD BASE CPP /Yu"tfalgo.h"
# ADD CPP /Yu"tfalgo.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\tfonpre.c

!IF  "$(CFG)" == "onmcc - Win32 Release"

!ELSEIF  "$(CFG)" == "onmcc - Win32 Debug"

# ADD CPP /Yc"tfalgo.h"

!ELSEIF  "$(CFG)" == "onmcc - Win32 Simul"

# ADD BASE CPP /Yc"tfalgo.h"
# ADD CPP /Yc"tfalgo.h"

!ELSEIF  "$(CFG)" == "onmcc - Win32 Dynamics"

# ADD BASE CPP /Yc"tfalgo.h"
# ADD CPP /Yc"tfalgo.h"

!ELSEIF  "$(CFG)" == "onmcc - Win32 Dynamics Runtime"

# ADD BASE CPP /Yc"tfalgo.h"
# ADD CPP /Yc"tfalgo.h"

!ELSEIF  "$(CFG)" == "onmcc - Win32 Run"

# ADD BASE CPP /Yc"tfalgo.h"
# ADD CPP /Yc"tfalgo.h"

!ENDIF 

# End Source File
# End Target
# End Project
