@echo off

:: TI SDK nw.exe GUI launcher
set NW_EXECUTOR="E:\TI\sysconfig\nw\nw.exe"
:: TI SDK sysconfig 
set SYSCONF="E:\TI\sysconfig"
:: Project path
set SYSCONF_FILE="E:\Interests\Code\micro_chip\jlc_training_course\course_1_pid_pcb_ti_project\ti-pid\empty\empty.syscfg"

%NW_EXECUTOR% %SYSCONF% %SYSCONF_FILE%
