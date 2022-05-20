@echo off
vdel -all
vlib work

cl -c /Zi /EHsc -I%MTI_HOME%\include src\fli_pipe.cpp
link -dll /EXPORT:open_pipe /EXPORT:read_pipe /EXPORT:close_pipe fli_pipe.obj %MTI_HOME%\win64pe\mtipli.lib /out:fli_pipe.dll

start cmd /k "python pipewrite.py"

vcom -quiet -2008 rtl\tb.vhd
vsim -quiet -batch tb -do "run -all; quit -f"