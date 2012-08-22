rem VS post build step command line sample:
rem call postbuild.bat "$(TargetDir)$(TargetName)" $(Platform)

@echo off
set targetPathWithoutExtension=%1
set platform=%2

if /I "%platform%" == "x64" set platformDir=\x64\

set libDir=%~dp0..\lib\%platformDir%
echo "%targetPathWithoutExtension%.lib" "%libDir%"

@xcopy /i /k /y /q /D "%targetPathWithoutExtension%.lib" "%libDir%" > NUL
@xcopy /i /k /y /q /D "%targetPathWithoutExtension%.pdb" "%libDir%" > NUL
