@echo off

set SolutionDir=%~p0

if not "%VS120COMNTOOLS%" == "" call :BuildVS120Targets
if not "%VS140COMNTOOLS%" == "" call :BuildVS140Targets

pause
exit /b


:BuildVS120Targets
echo Invoking %VS120COMNTOOLS%\..\..\VC\vcvarsall.bat x86_amd64
call "%VS120COMNTOOLS%\..\..\VC\vcvarsall.bat" x86_amd64
call :BuildTargets v120
@goto:eof


:BuildVS140Targets
echo Invoking %VS140COMNTOOLS%\..\..\VC\vcvarsall.bat x86_amd64
call "%VS140COMNTOOLS%\..\..\VC\vcvarsall.bat" x86_amd64
call :BuildTargets v140
@goto:eof



:BuildTargets
set platformToolset=%1

call :BuildTarget Release Win32 %platformToolset%
call :BuildTarget Release x64 %platformToolset%

call :BuildTarget Debug Win32 %platformToolset%
call :BuildTarget Debug x64 %platformToolset%
@goto:eof


:BuildTarget
set target=%1
set platform=%2
set platformToolset=%3

@echo [ Building Hypodermic (%target%-%platform%-%platformToolset%) ]

echo MSBuild %SolutionDir%\Hypodermic.sln /m /t:Clean /p:Configuration=%target% /p:Platform=%platform% /p:PlatformToolset=%platformToolset%
MSBuild %SolutionDir%\Hypodermic.sln /m /t:Clean /p:Configuration=%target% /p:Platform=%platform% /p:PlatformToolset=%platformToolset%
MSBuild %SolutionDir%\Hypodermic.sln /m /p:Configuration=%target% /p:Platform=%platform% /p:PlatformToolset=%platformToolset%

@goto:eof
