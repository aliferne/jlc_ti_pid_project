@echo off

:: please change the path to yours
set NW_EXECUTOR="E:\TI\sysconfig\nw\nw.exe"
:: please change the path to yours
set SYSCONF="E:\TI\sysconfig"
:: please change the path to yours
set SYSCONF_FILE="E:\Interests\Code\micro_chip\jlc_training_course\course_1_pid_pcb_ti_project\ti-pid\empty\empty.syscfg"

%NW_EXECUTOR% %SYSCONF% %SYSCONF_FILE%
