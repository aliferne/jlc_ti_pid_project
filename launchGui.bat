@echo off

:: TI SDK nw.exe GUI launcher
set NW_EXECUTOR="E:/TI/sysconfig/nw/nw.exe"
:: TI SDK sysconfig 
set SYSCONF="E:/TI/sysconfig"
:: Project path
set SYSCONF_FILE="./empty/empty.syscfg"

%NW_EXECUTOR% %SYSCONF% %SYSCONF_FILE%
