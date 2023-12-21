# Microsoft Developer Studio Project File - Name="corestmp" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=corestmp - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CORESTMP.MAK".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CORESTMP.MAK" CFG="corestmp - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "corestmp - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "corestmp - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/corestmp", PQAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "corestmp - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "\Core\Release"
# PROP Intermediate_Dir "Release"
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
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib zsyslib.lib corebase.lib /nologo /subsystem:windows /dll /machine:I386

!ELSEIF  "$(CFG)" == "corestmp - Win32 Debug"

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
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib zsyslib.lib corebase.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept /libpath:"..\Debug"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "corestmp - Win32 Release"
# Name "corestmp - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Corestmp.rc
# End Source File
# Begin Source File

SOURCE=.\Csdshll.c

!IF  "$(CFG)" == "corestmp - Win32 Release"

# ADD CPP /Yu"prehead.h"

!ELSEIF  "$(CFG)" == "corestmp - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\pre.c
# ADD CPP /Yc"prehead.h"
# End Source File
# Begin Source File

SOURCE=.\Stamp.c

!IF  "$(CFG)" == "corestmp - Win32 Release"

# ADD CPP /Yu"prehead.h"

!ELSEIF  "$(CFG)" == "corestmp - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Stampreg.c

!IF  "$(CFG)" == "corestmp - Win32 Release"

# ADD CPP /Yu"prehead.h"

!ELSEIF  "$(CFG)" == "corestmp - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Stmpbtn.c

!IF  "$(CFG)" == "corestmp - Win32 Release"

# ADD CPP /Yu"prehead.h"

!ELSEIF  "$(CFG)" == "corestmp - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Stmpcmbn.c

!IF  "$(CFG)" == "corestmp - Win32 Release"

# ADD CPP /Yu"prehead.h"

!ELSEIF  "$(CFG)" == "corestmp - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Stmpfill.c

!IF  "$(CFG)" == "corestmp - Win32 Release"

# ADD CPP /Yu"prehead.h"

!ELSEIF  "$(CFG)" == "corestmp - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Stmphtrd.c

!IF  "$(CFG)" == "corestmp - Win32 Release"

# ADD CPP /Yu"prehead.h"

!ELSEIF  "$(CFG)" == "corestmp - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Stmpline.c

!IF  "$(CFG)" == "corestmp - Win32 Release"

# ADD CPP /Yu"prehead.h"

!ELSEIF  "$(CFG)" == "corestmp - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Stmppg.c

!IF  "$(CFG)" == "corestmp - Win32 Release"

# ADD CPP /Yu"prehead.h"

!ELSEIF  "$(CFG)" == "corestmp - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Stmppict.c

!IF  "$(CFG)" == "corestmp - Win32 Release"

# ADD CPP /Yu"prehead.h"

!ELSEIF  "$(CFG)" == "corestmp - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Stmppie.c

!IF  "$(CFG)" == "corestmp - Win32 Release"

# ADD CPP /Yu"prehead.h"

!ELSEIF  "$(CFG)" == "corestmp - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Stmpply.c

!IF  "$(CFG)" == "corestmp - Win32 Release"

# ADD CPP /Yu"prehead.h"

!ELSEIF  "$(CFG)" == "corestmp - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Stmpslde.c

!IF  "$(CFG)" == "corestmp - Win32 Release"

# ADD CPP /Yu"prehead.h"

!ELSEIF  "$(CFG)" == "corestmp - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\stmpstr.c

!IF  "$(CFG)" == "corestmp - Win32 Release"

# ADD CPP /Yu"prehead.h"

!ELSEIF  "$(CFG)" == "corestmp - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Stmptrnd.c

!IF  "$(CFG)" == "corestmp - Win32 Release"

# ADD CPP /Yu"prehead.h"

!ELSEIF  "$(CFG)" == "corestmp - Win32 Debug"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Stmpdef.h
# End Source File
# Begin Source File

SOURCE=.\StmpRes.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\res\Stmpd.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Stmpl.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Stmpr.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Stmpu.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\res\left.ico
# End Source File
# Begin Source File

SOURCE=.\res\left2.ico
# End Source File
# Begin Source File

SOURCE=.\res\left3.ico
# End Source File
# Begin Source File

SOURCE=.\res\top.ico
# End Source File
# End Target
# End Project
