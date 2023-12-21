# Microsoft Developer Studio Project File - Name="Mtonsmcc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Mtonsmcc - Win32 Run
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Mtonsmcc.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Mtonsmcc.mak" CFG="Mtonsmcc - Win32 Run"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Mtonsmcc - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Mtonsmcc - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Mtonsmcc - Win32 Run" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/MCC/OldMCC/Mtonsmcc", VFEAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Mtonsmcc - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "\mcc\Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Mtonsmcc - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Mtonsmcc"
# PROP BASE Intermediate_Dir "Mtonsmcc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /Yu"tfalgo.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Mtonsmcc - Win32 Run"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Mtonsmcc___Win32_Run"
# PROP BASE Intermediate_Dir "Mtonsmcc___Win32_Run"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Run_Debug"
# PROP Intermediate_Dir "Mtonsmcc___Win32_Run"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /Yu"tfalgo.h" /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /Yu"tfalgo.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"Debug\Mtonsmcc.dll" /implib:"Debug\Mtonsmcc.lib" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Mtonsmcc - Win32 Release"
# Name "Mtonsmcc - Win32 Debug"
# Name "Mtonsmcc - Win32 Run"
# Begin Source File

SOURCE=.\MtIdchec.c
# End Source File
# Begin Source File

SOURCE=.\Mtidtran.c

!IF  "$(CFG)" == "Mtonsmcc - Win32 Release"

!ELSEIF  "$(CFG)" == "Mtonsmcc - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "Mtonsmcc - Win32 Run"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Mtiniall.c
# End Source File
# Begin Source File

SOURCE=.\Mtrdcvme.c

!IF  "$(CFG)" == "Mtonsmcc - Win32 Release"

!ELSEIF  "$(CFG)" == "Mtonsmcc - Win32 Debug"

# ADD CPP /Yc"tfalgo.h"

!ELSEIF  "$(CFG)" == "Mtonsmcc - Win32 Run"

# ADD BASE CPP /Yc"tfalgo.h"
# ADD CPP /Yc"tfalgo.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Mtrdcvst.c
# End Source File
# Begin Source File

SOURCE=.\Mtrddata.c
# End Source File
# Begin Source File

SOURCE=.\Mtrddvme.c
# End Source File
# Begin Source File

SOURCE=.\Mtrddvst.c
# End Source File
# Begin Source File

SOURCE=.\Mtrdgvme.c
# End Source File
# Begin Source File

SOURCE=.\Mtrdgvst.c
# End Source File
# Begin Source File

SOURCE=.\Mtrdmvme.c
# End Source File
# Begin Source File

SOURCE=.\Mtrdmvst.c
# End Source File
# Begin Source File

SOURCE=.\Mtrdtagd.c
# End Source File
# Begin Source File

SOURCE=.\Mtwtcvme.c
# End Source File
# Begin Source File

SOURCE=.\Mtwtcvst.c
# End Source File
# Begin Source File

SOURCE=.\Mtwtdata.c
# End Source File
# Begin Source File

SOURCE=.\Mtwtdvme.c
# End Source File
# Begin Source File

SOURCE=.\Mtwtdvst.c
# End Source File
# Begin Source File

SOURCE=.\Mtwtgvme.c
# End Source File
# Begin Source File

SOURCE=.\Mtwtgvst.c
# End Source File
# Begin Source File

SOURCE=.\Mtwtmvme.c
# End Source File
# Begin Source File

SOURCE=.\Mtwtmvst.c
# End Source File
# Begin Source File

SOURCE=.\Mtwttagd.c
# End Source File
# Begin Source File

SOURCE=.\Onworld.lib
# End Source File
# End Target
# End Project
