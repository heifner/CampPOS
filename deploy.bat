REM -----------------------------------------
REM - TARGET_DIR and CampPOS.zip are deleted.
REM - .NET Framework 4 required
REM ----------------------------------------- 

set DEPLOY_DIR=CampPOS
rem set VC_REDIST_DIR=C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\redist\x86\Microsoft.VC120.CRT
set VC_REDIST_DIR=C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Redist\MSVC\14.14.26405\x86\Microsoft.VC141.CRT
set TARGET_DIR=C:\temp\%DEPLOY_DIR%
set CURRENT_DIR=%CD%

pushd .

del /f CampPOS.zip
del /f /s %TARGET_DIR%
mkdir %TARGET_DIR%

copy Output\Release\*.exe %TARGET_DIR%
copy Output\Release\*.dll %TARGET_DIR%
copy "%VC_REDIST_DIR%"\*.dll %TARGET_DIR%
copy "%VC_REDIST_DIR%"\*.manifest %TARGET_DIR%
copy CampPOS.ini %TARGET_DIR%

copy %BSAPI_ROOT%\bin\32-bit\bsapi.dll %TARGET_DIR%

chdir /d %TARGET_DIR%
chdir ..

zip -9 -r %CURRENT_DIR%\CampPOS.zip %DEPLOY_DIR% 

popd
