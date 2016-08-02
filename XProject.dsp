# Microsoft Developer Studio Project File - Name="XProject" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=XProject - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "XProject.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "XProject.mak" CFG="XProject - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "XProject - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "XProject - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "XProject - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "XProject - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"nafxcw.lib" /nodefaultlib:"MSVCRT" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "XProject - Win32 Release"
# Name "XProject - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ARACrypt.cpp
# End Source File
# Begin Source File

SOURCE=.\Archiev.cpp
# End Source File
# Begin Source File

SOURCE=.\Barter.cpp
# End Source File
# Begin Source File

SOURCE=.\Fight.cpp
# End Source File
# Begin Source File

SOURCE=.\Helper.cpp
# End Source File
# Begin Source File

SOURCE=.\INI.cpp
# End Source File
# Begin Source File

SOURCE=.\Inventory.cpp
# End Source File
# Begin Source File

SOURCE=.\Safe.cpp
# End Source File
# Begin Source File

SOURCE=.\XProject.cpp
# End Source File
# Begin Source File

SOURCE=.\XProject.rc
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ARACrypt.h
# End Source File
# Begin Source File

SOURCE=.\Archiev.h
# End Source File
# Begin Source File

SOURCE=.\Barter.h
# End Source File
# Begin Source File

SOURCE=.\Fight.h
# End Source File
# Begin Source File

SOURCE=.\fmod.h
# End Source File
# Begin Source File

SOURCE=.\Helper.h
# End Source File
# Begin Source File

SOURCE=.\INI.h
# End Source File
# Begin Source File

SOURCE=.\Inventory.h
# End Source File
# Begin Source File

SOURCE=.\lua5.1\include\lauxlib.h
# End Source File
# Begin Source File

SOURCE=.\lua5.1\include\lua.h
# End Source File
# Begin Source File

SOURCE=.\lua5.1\include\luaconf.h
# End Source File
# Begin Source File

SOURCE=.\lua5.1\include\lualib.h
# End Source File
# Begin Source File

SOURCE=..\..\..\UTILITY\dx6_1docs\dx7\dx7include\NukeDX.h
# End Source File
# Begin Source File

SOURCE=..\..\..\UTILITY\dx6_1docs\dx7\dx7include\NukeFile.h
# End Source File
# Begin Source File

SOURCE=.\qdata.h
# End Source File
# Begin Source File

SOURCE=.\quest.h
# End Source File
# Begin Source File

SOURCE=.\Safe.h
# End Source File
# Begin Source File

SOURCE=.\sound.h
# End Source File
# Begin Source File

SOURCE=.\XProject.h
# End Source File
# Begin Source File

SOURCE=..\..\..\UTILITY\dx6_1docs\dx7\dx7include\zconf.h
# End Source File
# Begin Source File

SOURCE=..\..\..\UTILITY\dx6_1docs\dx7\dx7include\zlib.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\fmodvc.lib
# End Source File
# Begin Source File

SOURCE="D:\Program Files\Microsoft Visual Studio\Vc98\Lib\Winmm.lib"
# End Source File
# Begin Source File

SOURCE=..\..\..\UTILITY\dx6_1docs\dx7\dx7lib1\ddraw.lib
# End Source File
# Begin Source File

SOURCE=..\..\..\UTILITY\dx6_1docs\dx7\dx7lib1\dsound.lib
# End Source File
# Begin Source File

SOURCE=..\..\..\UTILITY\dx6_1docs\dx7\dx7lib1\dxguid.lib
# End Source File
# Begin Source File

SOURCE=..\..\..\UTILITY\dx6_1docs\dx7\dx7lib1\NukeDX.lib
# End Source File
# Begin Source File

SOURCE=..\..\..\UTILITY\Game\nukeDX\NukeFile\lib\NukeFile.lib
# End Source File
# Begin Source File

SOURCE=..\..\..\UTILITY\Game\nukeDX\NukeFile\lib\zlib.lib
# End Source File
# Begin Source File

SOURCE=.\lua5.1\lib\static\lua5.1_lib.lib
# End Source File
# Begin Source File

SOURCE="D:\Program Files\Microsoft Visual Studio\VC98\Lib\AMSTRMID.LIB"
# End Source File
# End Target
# End Project
