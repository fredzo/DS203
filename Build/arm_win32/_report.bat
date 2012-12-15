@echo off
rem this script helps to find functios and variables which occupy largest amounts of ROM and RAM
if exist ..\..\bin\APP_G251_1.elf (
  echo Generating report...
  C:\Programs\Devel\Gcc\arm-2011.03\bin\arm-none-eabi-nm.exe --print-size --size-sort -gC ..\..\bin\APP_G251_1.elf > report.txt
) else (
  echo Cannot find .elf file
)