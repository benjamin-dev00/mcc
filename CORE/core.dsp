# Microsoft Developer Studio Project File - Name="core" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=core - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "core.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "core.mak" CFG="core - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "core - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "core - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/core", FPBAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "core - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "\Core\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib zsyslib.lib /nologo /subsystem:windows /dll /machine:I386 /out:"\Core\Release/corebase.dll"

!ELSEIF  "$(CFG)" == "core - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /MDd /W3 /Gm /Gi /GX /ZI /Od /I "\z5\core" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /Yu"prehead.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib zsyslib.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\Debug\corebase.dll" /libpath:"..\Debug"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "core - Win32 Release"
# Name "core - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\calarm.c
# ADD CPP /Yu"prehead.h"
# End Source File
# Begin Source File

SOURCE=.\calgo.c
# ADD CPP /Yu"prehead.h"
# End Source File
# Begin Source File

SOURCE=.\cbdshll.c
# ADD CPP /Yu"prehead.h"
# End Source File
# Begin Source File

SOURCE=.\cbool.c
# ADD CPP /Yu"prehead.h"
# End Source File
# Begin Source File

SOURCE=.\cdbi.c
# ADD CPP /Yu"prehead.h"
# End Source File
# Begin Source File

SOURCE=.\cface.c
# ADD CPP /Yu"prehead.h"
# End Source File
# Begin Source File

SOURCE=.\cfile.c
# ADD CPP /Yu"prehead.h"
# End Source File
# Begin Source File

SOURCE=.\cfwnd.c
# ADD CPP /Yu"prehead.h"
# End Source File
# Begin Source File

SOURCE=.\cgdi.c
# ADD CPP /Yu"prehead.h"
# End Source File
# Begin Source File

SOURCE=.\cmodu.c

!IF  "$(CFG)" == "core - Win32 Release"

# ADD CPP /Yu"prehead.h"

!ELSEIF  "$(CFG)" == "core - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\cobj.c
# ADD CPP /Yu"prehead.h"
# End Source File
# Begin Source File

SOURCE=.\Codbc.c
# End Source File
# Begin Source File

SOURCE=.\core.c
# ADD CPP /Yu"prehead.h"
# End Source File
# Begin Source File

SOURCE=.\core.rc
# End Source File
# Begin Source File

SOURCE=.\corerw.c
# ADD CPP /Yu"prehead.h"
# End Source File
# Begin Source File

SOURCE=.\cpu.c
# ADD CPP /Yu"prehead.h"
# End Source File
# Begin Source File

SOURCE=.\creal.c
# ADD CPP /Yu"prehead.h"
# End Source File
# Begin Source File

SOURCE=.\crun.c
# ADD CPP /Yu"prehead.h"
# End Source File
# Begin Source File

SOURCE=.\cstamp.c
# ADD CPP /Yu"prehead.h"
# End Source File
# Begin Source File

SOURCE=.\cstmpfn.c
# ADD CPP /Yu"prehead.h"
# End Source File
# Begin Source File

SOURCE=.\csysalgo.c
# ADD CPP /Yu"prehead.h"
# End Source File
# Begin Source File

SOURCE=.\csysobj.c
# ADD CPP /Yu"prehead.h"
# End Source File
# Begin Source File

SOURCE=.\csysstmp.c
# ADD CPP /Yu"prehead.h"
# End Source File
# Begin Source File

SOURCE=.\ctint.c
# ADD CPP /Yu"prehead.h"
# End Source File
# Begin Source File

SOURCE=.\pre.c
# ADD CPP /Yc"prehead.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\calarm.h
# End Source File
# Begin Source File

SOURCE=.\calgo.h
# End Source File
# Begin Source File

SOURCE=.\cbool.h
# End Source File
# Begin Source File

SOURCE=.\cdbi.h
# End Source File
# Begin Source File

SOURCE=.\cerror.h
# End Source File
# Begin Source File

SOURCE=.\cface.h
# End Source File
# Begin Source File

SOURCE=.\cfile.h
# End Source File
# Begin Source File

SOURCE=.\cfwnd.h
# End Source File
# Begin Source File

SOURCE=.\cgdi.h
# End Source File
# Begin Source File

SOURCE=.\cmodu.h
# End Source File
# Begin Source File

SOURCE=.\cobj.h
# End Source File
# Begin Source File

SOURCE=.\core.h
# End Source File
# Begin Source File

SOURCE=.\coreall.h
# End Source File
# Begin Source File

SOURCE=.\CoreRes.h
# End Source File
# Begin Source File

SOURCE=.\corerw.h
# End Source File
# Begin Source File

SOURCE=.\cpu.h
# End Source File
# Begin Source File

SOURCE=.\creal.h
# End Source File
# Begin Source File

SOURCE=.\crun.h
# End Source File
# Begin Source File

SOURCE=.\cstamp.h
# End Source File
# Begin Source File

SOURCE=.\CStmpfn.h
# End Source File
# Begin Source File

SOURCE=.\csysalgo.h
# End Source File
# Begin Source File

SOURCE=.\csysobj.h
# End Source File
# Begin Source File

SOURCE=.\csysstmp.h
# End Source File
# Begin Source File

SOURCE=.\ctint.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Break.ico
# End Source File
# Begin Source File

SOURCE=.\res\call.ico
# End Source File
# Begin Source File

SOURCE=.\res\End.ico
# End Source File
# Begin Source File

SOURCE=.\res\Execute.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\Loop.ico
# End Source File
# Begin Source File

SOURCE=.\res\NULL.ico
# End Source File
# Begin Source File

SOURCE=.\res\Return.ico
# End Source File
# Begin Source File

SOURCE=.\res\Start.ico
# End Source File
# Begin Source File

SOURCE=.\res\stop.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\core.txt
# End Source File
# End Target
# End Project
