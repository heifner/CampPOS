set CAMPPOS_ROOT=C:\dev\heifner\CampPOS\trunk
set MPC_ROOT=D:\dev_tools\MPC-MPC_4_1_2
set BSAPI_ROOT=%CAMPPOS_ROOT%\bsapi
set PATH=%MPC_ROOT%;%PATH%;%BSAPI_ROOT%\bin\32-bit
mwc.pl -type vc12 CampPOS.mwc
REM Update Platform target of CampAdminGUI and CampSaleGUI to x86