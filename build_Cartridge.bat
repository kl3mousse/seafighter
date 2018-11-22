@REM  clearing command prompt
CLS
echo off

@REM You only need to modify this value to where you installed your NeoDev dev kit
@set MAMEDIR=%cd%\tools\MAME

@REM cleaning potential temporary files in case previous Build was interrupted
echo.
echo ******************************
echo *****   files cleanup     ****
echo ******************************

del %NEODEV%\tmp\*.o
del *.pal
del *.o
del *.fix
del *.spr
del *.map
del *.bin
del *.prg
del dev_p1.rom

@REM Generate palettes and fix
echo.
echo ******************************
echo *****  Pal /Fix generation ***
echo ******************************
.\include\NeoDev\m68k\bin\fixcnv gfx\fix_font.bmp -o test.fix -pal fix.pal
.\include\NeoDev\m68k\bin\gfxcc -black fix.pal gfx\starfield.bmp gfx\playership.bmp gfx\playership_2.bmp gfx\playership_3.bmp gfx\bullet.bmp gfx\enemies.bmp -o test.spr

@REM Compile CRT0 (C Run Time library)
echo.
echo ******************************
echo ***** AS -O CART HEADER   ****
echo ******************************
.\include\NeoDev\m68k\bin\as -m68000 --register-prefix-optional .\src\crt0_cart.s -o %NEODEV%\tmp\crt0_cart.o

@REM Compile program
echo.
echo ******************************
echo ***** COMPILATION GCC -c  ****
echo ******************************

gcc -I%NEODEV%\m68k\include -m68000 -O3 -Wall -fomit-frame-pointer -ffast-math -fno-builtin -nostartfiles -nodefaultlibs -D__cart__ -c src\main.c -o %NEODEV%\tmp\main.o
gcc -I%NEODEV%\m68k\include -m68000 -O3 -Wall -fomit-frame-pointer -ffast-math -fno-builtin -nostartfiles -nodefaultlibs -D__cart__ -c src\background.c -o %NEODEV%\tmp\background.o
gcc -I%NEODEV%\m68k\include -m68000 -O3 -Wall -fomit-frame-pointer -ffast-math -fno-builtin -nostartfiles -nodefaultlibs -D__cart__ -c src\gamelogic.c -o %NEODEV%\tmp\gamelogic.o
gcc -I%NEODEV%\m68k\include -m68000 -O3 -Wall -fomit-frame-pointer -ffast-math -fno-builtin -nostartfiles -nodefaultlibs -D__cart__ -c src\mylib.c -o %NEODEV%\tmp\mylib.o
pause

@REM "Compile" images
echo.
echo ******************************
echo ***** BIN2ELF (images to .o) *
echo ******************************
.\include\NeoDev\m68k\bin\bin2elf gfx\enemies.map enemies %NEODEV%\tmp\enemies.o
.\include\NeoDev\m68k\bin\bin2elf gfx\starfield.map starfield %NEODEV%\tmp\starfield.o
.\include\NeoDev\m68k\bin\bin2elf gfx\playership.map playership %NEODEV%\tmp\playership.o
.\include\NeoDev\m68k\bin\bin2elf gfx\playership_2.map playership_2 %NEODEV%\tmp\playership_2.o
.\include\NeoDev\m68k\bin\bin2elf gfx\playership_3.map playership_3 %NEODEV%\tmp\playership_3.o
.\include\NeoDev\m68k\bin\bin2elf gfx\bullet.map bullet %NEODEV%\tmp\bullet.o

@REM compile palettes
echo.
echo ******************************
echo ***** Palettes            ****
echo ******************************
.\include\NeoDev\m68k\bin\bin2elf test.pal palettes %NEODEV%\tmp\palettes.o

@REM Link
echo.
echo ******************************
echo ***** Linking GCC         ****
echo ******************************
.\include\NeoDev\m68k\bin\gcc -L%NEODEV%\m68k\lib -m68000 -O3 -Wall -fomit-frame-pointer -ffast-math -fno-builtin -nostartfiles -nodefaultlibs -D__cart__ -Wl,-T%NEODEV%\src\system\neocart.x %NEODEV%\tmp\crt0_cart.o %NEODEV%\tmp\main.o %NEODEV%\tmp\background.o %NEODEV%\tmp\gamelogic.o %NEODEV%\tmp\enemies.o %NEODEV%\tmp\mylib.o %NEODEV%\tmp\palettes.o %NEODEV%\tmp\starfield.o %NEODEV%\tmp\playership.o %NEODEV%\tmp\playership_2.o %NEODEV%\tmp\playership_3.o %NEODEV%\tmp\bullet.o -linput -lvideo -lc -lgcc -o test.o
pause

@REM Pad program rom
.\include\NeoDev\m68k\bin\objcopy --gap-fill=0x00 --pad-to=0x20000 -R .data -O binary test.o dev_p1.rom

@REM Pad modified roms. These are the result of the compilation
.\include\NeoDev\m68k\bin\romwak /f dev_p1.rom 444-p1.bin
.\include\NeoDev\m68k\bin\romwak /p 444-p1.bin 444-p1.bin 1024 255
.\include\NeoDev\m68k\bin\romwak /w test.spr 444-c1.bin 444-c2.bin
.\include\NeoDev\m68k\bin\romwak /f 444-c1.bin
.\include\NeoDev\m68k\bin\romwak /f 444-c2.bin
.\include\NeoDev\m68k\bin\romwak /p 444-c1.bin 444-c1.bin 1024 255
.\include\NeoDev\m68k\bin\romwak /p 444-c2.bin 444-c2.bin 1024 255


copy test.fix 444-s1.bin
.\include\NeoDev\m68k\bin\romwak /p 444-s1.bin 444-s1.bin 128 255

@REM Sound/Music roms
copy sound\444-m1.bin .\
copy sound\444-v1.bin .\
@REM .\include\NeoDev\m68k\bin\romwak /p 202-v1.bin 202-v1.bin 512 255

@REM Copy all game roms to output directory
del output\cartridge\*.bin
move *.bin output\cartridge\

@REM Build ZIP rom
cd output\cartridge
del ..\rom\seafighter.zip
..\..\tools\7zip\7za a ..\rom\seafighter.zip *.bin

@REM Build NeoSD rom (.Neo)
cd ..\..\tools\neosd-neoBuilder
NeoBuilder -n seafighter -m klemousse -y 2018 -g Shooter -s 444 ..\..\output\rom\seafighter.zip
move seafighter.neo ..\..\output\neoSD
cd ..\..

@REM Clean
del %NEODEV%\tmp\*.o
del *.pal
del *.o
del *.fix
del *.spr
del gfx\*.map
del *.bin
@REM del *.prg
del dev_p1.rom
