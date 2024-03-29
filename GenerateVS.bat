@echo off

:: Check to see if Ultra Engine is installed and defined.
if not exist "%LEADWERKS%" (
	echo ERROR: Unable to find Ultra Engine installed on this PC! Try running the editor first!
	pause
	EXIT /B 999
)
echo Found Leadwerks at: "%LEADWERKS%"

:: Go into the Tools folder.
pushd .
cd Tools

:: Define premake path.
SET PREMAKE=premake5.exe

:: To prevent hundreds of copies of premake, see if there is a copy in the engine's Tools directory.
if not exist %PREMAKE% (
	echo Redirecting premake path to "%LEADWERKS%\Tools"
	SET PREMAKE="%LEADWERKS%\Tools\premake5.exe"
)

:: Run Premake!
%PREMAKE% vs2022

:: Popd out
popd

pause