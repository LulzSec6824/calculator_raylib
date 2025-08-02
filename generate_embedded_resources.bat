@echo off
echo Generating embedded resource files...

:: Create output directory if it doesn't exist
if not exist "build" mkdir build

:: Export font as code
echo Exporting font as code...
build\ray.exe -exportfontascode resource\Ubuntu-Regular.ttf includes\font_ubuntu.h

:: Export icon as code
echo Exporting icon as code...
build\ray.exe -exportimageascode resource\calc.png includes\icon_calc.h

echo Done generating embedded resources!