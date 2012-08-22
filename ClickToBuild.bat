@echo off

set SolutionDir=%~p0

if not "%VS100COMNTOOLS%" == "" call :BuildVS100Targets
if not "%VS110COMNTOOLS%" == "" call :BuildVS110Targets

pause
exit /b


:BuildVS100Targets
echo Invoking %VS100COMNTOOLS%\..\..\VC\vcvarsall.bat x86_amd64
call "%VS100COMNTOOLS%\..\..\VC\vcvarsall.bat" x86_amd64
call :BuildTargets v100
@goto:eof


:BuildVS110Targets
echo Invoking %VS110COMNTOOLS%\..\..\VC\vcvarsall.bat x86_amd64
call "%VS110COMNTOOLS%\..\..\VC\vcvarsall.bat" x86_amd64
call :BuildTargets v110
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

MSBuild %SolutionDir%\Hypodermic.sln /m /t:Clean /p:Configuration=%target% /p:Platform=%platform% /p:PlatformToolset=%platformToolset%
MSBuild %SolutionDir%\Hypodermic.sln /m /p:Configuration=%target% /p:Platform=%platform% /p:PlatformToolset=%platformToolset%

@goto:eof
