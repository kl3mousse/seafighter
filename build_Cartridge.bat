@REM You only need to modify this value to where you installed your NeoDev dev kit
@REM  @set NEODEV= %cd%\include\NeoDev
@REM  @set path=%NEODEV%\m68k\bin;%path%

@set MAMEDIR=c:\MAME

del %NEODEV%\tmp\*.o
del *.pal
del *.o
del *.fix
del *.spr
del *.map
del *.bin
del *.prg
del dev_p1.rom

@REM Generated palettes and fix
.\include\NeoDev\m68k\bin\fixcnv fix_font.bmp -o test.fix -pal fix.pal
@REM it was fixcnv fix_font.bmp -o test.fix -pal fix.pal

.\include\NeoDev\m68k\bin\gfxcc -black fix.pal starfield.bmp playership.bmp playership_2.bmp playership_3.bmp bullet.bmp enemies.bmp -o test.spr

@REM Compile CRT0 (C Run Time library)
.\include\NeoDev\m68k\bin\as -m68000 --register-prefix-optional .\crt0_cart.s -o %NEODEV%\tmp\crt0_cart.o

@REM Compile program
gcc -I%NEODEV%\m68k\include -m68000 -O3 -Wall -fomit-frame-pointer -ffast-math -fno-builtin -nostartfiles -nodefaultlibs -D__cart__ -c main.c -o %NEODEV%\tmp\main.o
gcc -I%NEODEV%\m68k\include -m68000 -O3 -Wall -fomit-frame-pointer -ffast-math -fno-builtin -nostartfiles -nodefaultlibs -D__cart__ -c background.c -o %NEODEV%\tmp\background.o
gcc -I%NEODEV%\m68k\include -m68000 -O3 -Wall -fomit-frame-pointer -ffast-math -fno-builtin -nostartfiles -nodefaultlibs -D__cart__ -c gamelogic.c -o %NEODEV%\tmp\gamelogic.o

@REM "Compile" images
.\include\NeoDev\m68k\bin\bin2elf enemies.map enemies %NEODEV%\tmp\enemies.o
.\include\NeoDev\m68k\bin\bin2elf test.pal palettes %NEODEV%\tmp\palettes.o
.\include\NeoDev\m68k\bin\bin2elf starfield.map starfield %NEODEV%\tmp\starfield.o
.\include\NeoDev\m68k\bin\bin2elf playership.map playership %NEODEV%\tmp\playership.o
.\include\NeoDev\m68k\bin\bin2elf playership_2.map playership_2 %NEODEV%\tmp\playership_2.o
.\include\NeoDev\m68k\bin\bin2elf playership_3.map playership_3 %NEODEV%\tmp\playership_3.o
.\include\NeoDev\m68k\bin\bin2elf bullet.map bullet %NEODEV%\tmp\bullet.o

@REM Link
.\include\NeoDev\m68k\bin\gcc -L%NEODEV%\m68k\lib -m68000 -O3 -Wall -fomit-frame-pointer -ffast-math -fno-builtin -nostartfiles -nodefaultlibs -D__cart__ -Wl,-T%NEODEV%\src\system\neocart.x %NEODEV%\tmp\crt0_cart.o %NEODEV%\tmp\main.o %NEODEV%\tmp\background.o %NEODEV%\tmp\gamelogic.o %NEODEV%\tmp\enemies.o %NEODEV%\tmp\palettes.o %NEODEV%\tmp\starfield.o %NEODEV%\tmp\playership.o %NEODEV%\tmp\playership_2.o %NEODEV%\tmp\playership_3.o %NEODEV%\tmp\bullet.o -linput -lvideo -lc -lgcc -o test.o

@REM Pad program rom
.\include\NeoDev\m68k\bin\objcopy --gap-fill=0x00 --pad-to=0x20000 -R .data -O binary test.o dev_p1.rom


@REM Pad modified roms. These are the result of the compilation
.\include\NeoDev\m68k\bin\romwak /f dev_p1.rom 202-p1.bin
.\include\NeoDev\m68k\bin\romwak /p 202-p1.bin 202-p1.bin 1024 255
.\include\NeoDev\m68k\bin\romwak /w test.spr 202-c1.bin 202-c2.bin
.\include\NeoDev\m68k\bin\romwak /f 202-c1.bin
.\include\NeoDev\m68k\bin\romwak /f 202-c2.bin
.\include\NeoDev\m68k\bin\romwak /p 202-c1.bin 202-c1.bin 1024 255
.\include\NeoDev\m68k\bin\romwak /p 202-c2.bin 202-c2.bin 1024 255
copy test.fix 202-s1.bin

@REM Pad Puzzle De Pon! roms which are unchanged. They are copied from the puzzledepon_original_roms subdirectory
@REM into the current directory
copy puzzledepon_original_roms\202-m1.bin .\
.\include\NeoDev\m68k\bin\romwak /p 202-s1.bin 202-s1.bin 128 255
copy puzzledepon_original_roms\202-v1.bin .\
.\include\NeoDev\m68k\bin\romwak /p 202-v1.bin 202-v1.bin 512 255

@REM Copy all game roms to output directory
del output\*.bin
move *.bin output\cartridge\

@REM Copy all game roms to MAME (should be six .bin files)
del %MAMEDIR%\roms\puzzledp\*.bin
copy output\cartridge\*.bin %MAMEDIR%\roms\puzzledp\

@REM Clean
del %NEODEV%\tmp\*.o
del *.pal
del *.o
del *.fix
del *.spr
del *.map
del *.bin
del *.prg
del dev_p1.rom
