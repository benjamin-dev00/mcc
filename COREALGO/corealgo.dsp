# Microsoft Developer Studio Project File - Name="corealgo" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=corealgo - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "corealgo.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "corealgo.mak" CFG="corealgo - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "corealgo - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "corealgo - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/corealgo", OQAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "corealgo - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "\Core\Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib zsyslib.lib corebase.lib acr.lib /nologo /subsystem:windows /dll /machine:I386

!ELSEIF  "$(CFG)" == "corealgo - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /Gz /MTd /W3 /Gm /GX /ZI /Od /I "\z5\core" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /Yu"prehead.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib zsyslib.lib corebase.lib acr.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept /libpath:"..\Debug"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "corealgo - Win32 Release"
# Name "corealgo - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ".c"
# Begin Source File

SOURCE=.\A_Arithmetic.c
# End Source File
# Begin Source File

SOURCE=.\A_cbdx.c

!IF  "$(CFG)" == "corealgo - Win32 Release"

# ADD CPP /Yu"prehead.h"

!ELSEIF  "$(CFG)" == "corealgo - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\A_stair.c

!IF  "$(CFG)" == "corealgo - Win32 Release"

# ADD CPP /Yu"prehead.h"

!ELSEIF  "$(CFG)" == "corealgo - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\A_system.c

!IF  "$(CFG)" == "corealgo - Win32 Release"

# ADD CPP /Yu"prehead.h"

!ELSEIF  "$(CFG)" == "corealgo - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\A_timer.c

!IF  "$(CFG)" == "corealgo - Win32 Release"

# ADD CPP /Yu"prehead.h"

!ELSEIF  "$(CFG)" == "corealgo - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\A_wave.c

!IF  "$(CFG)" == "corealgo - Win32 Release"

# ADD CPP /Yu"prehead.h"

!ELSEIF  "$(CFG)" == "corealgo - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Algo.c

!IF  "$(CFG)" == "corealgo - Win32 Release"

# ADD CPP /Yu"prehead.h"

!ELSEIF  "$(CFG)" == "corealgo - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Algoreg.c

!IF  "$(CFG)" == "corealgo - Win32 Release"

# ADD CPP /Yu"prehead.h"

!ELSEIF  "$(CFG)" == "corealgo - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Cadshll.c

!IF  "$(CFG)" == "corealgo - Win32 Release"

# ADD CPP /Yu"prehead.h"

!ELSEIF  "$(CFG)" == "corealgo - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\corealgo.rc
# End Source File
# Begin Source File

SOURCE=.\pre.c
# ADD CPP /Yc"prehead.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Algo.h
# End Source File
# Begin Source File

SOURCE=.\Algodef.h
# End Source File
# Begin Source File

SOURCE=.\AlgoRes.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\AddInt1.ico
# End Source File
# Begin Source File

SOURCE=.\res\AddR1.ico
# End Source File
# Begin Source File

SOURCE=.\res\AddS1.ico
# End Source File
# Begin Source File

SOURCE=.\res\andb.ico
# End Source File
# Begin Source File

SOURCE=.\res\CloseWindow.ico
# End Source File
# Begin Source File

SOURCE=.\res\divider.ico
# End Source File
# Begin Source File

SOURCE=.\res\ExchangeX.ico
# End Source File
# Begin Source File

SOURCE=.\res\Generator.ico
# End Source File
# Begin Source File

SOURCE=.\res\minusn.ico
# End Source File
# Begin Source File

SOURCE=.\res\minusr.ico
# End Source File
# Begin Source File

SOURCE=.\res\multir.ico
# End Source File
# Begin Source File

SOURCE=.\res\orb.ico
# End Source File
# Begin Source File

SOURCE=.\res\RanxI.ico
# End Source File
# Begin Source File

SOURCE=.\res\RanxR1.ico
# End Source File
# Begin Source File

SOURCE=.\res\ReplaceWindow.ico
# End Source File
# Begin Source File

SOURCE=.\res\RndxR.ico
# End Source File
# Begin Source File

SOURCE=.\res\ShowWindow.ico
# End Source File
# Begin Source File

SOURCE=.\res\sine.ico
# End Source File
# Begin Source File

SOURCE=.\res\stair.ico
# End Source File
# Begin Source File

SOURCE=.\res\test.ico
# End Source File
# Begin Source File

SOURCE=.\res\Timer.ico
# End Source File
# Begin Source File

SOURCE=.\res\timer1.ico
# End Source File
# End Group
# End Target
# End Project
