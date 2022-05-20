@echo off
vdel -all
vlib work

cl -c /Zi /EHsc -I%MTI_HOME%\include src\fli_mmap.cpp
link -dll /EXPORT:CreateMemoryMap /EXPORT:FreeMemoryMap /EXPORT:ReadMemoryMap fli_mmap.obj %MTI_HOME%\win64pe\mtipli.lib /out:fli_mmap.dll

start cmd /k "python mmapwrite.py"

vcom -quiet -2008 rtl\tb.vhd
vsim -quiet -batch tb -do "run -all; quit -f"