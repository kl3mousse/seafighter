@REM You only need to modify this value to where you installed your NeoDev dev kit
@set NEODEV=c:\NeoDev

@set path=%NEODEV%\m68k\bin;%path%

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

@REM Generated palettes and fix
fixcnv fix_font.bmp -o test.fix -pal fix.pal
gfxcc -black fix.pal starfield.bmp playership.bmp bullet.bmp enemies.bmp -o test.spr

@REM Compile CRT0 (C Run Time library)
as -m68000 --register-prefix-optional crt0_cd.s -o %NEODEV%\tmp\crt0_cd.o

@REM Compile program
gcc -I%NEODEV%\m68k\include -m68000 -O3 -Wall -fomit-frame-pointer -ffast-math -fno-builtin -nostartfiles -nodefaultlibs -D__cd__ -c main.c -o %NEODEV%\tmp\main.o
gcc -I%NEODEV%\m68k\include -m68000 -O3 -Wall -fomit-frame-pointer -ffast-math -fno-builtin -nostartfiles -nodefaultlibs -D__cd__ -c background.c -o %NEODEV%\tmp\background.o
gcc -I%NEODEV%\m68k\include -m68000 -O3 -Wall -fomit-frame-pointer -ffast-math -fno-builtin -nostartfiles -nodefaultlibs -D__cd__ -c gamelogic.c -o %NEODEV%\tmp\gamelogic.o

@REM "Compile" images
bin2elf enemies.map enemies %NEODEV%\tmp\enemies.o
bin2elf test.pal palettes %NEODEV%\tmp\palettes.o
bin2elf starfield.map starfield %NEODEV%\tmp\starfield.o
bin2elf playership.map playership %NEODEV%\tmp\playership.o
bin2elf bullet.map bullet %NEODEV%\tmp\bullet.o

@REM Link
gcc -L%NEODEV%\m68k\lib -m68000 -O3 -Wall -fomit-frame-pointer -ffast-math -fno-builtin -nostartfiles -nodefaultlibs -D__cd__ -Wl,-T%NEODEV%\src\system\neocd.x %NEODEV%\tmp\crt0_cd.o %NEODEV%\tmp\main.o %NEODEV%\tmp\background.o %NEODEV%\tmp\gamelogic.o %NEODEV%\tmp\enemies.o %NEODEV%\tmp\palettes.o %NEODEV%\tmp\starfield.o %NEODEV%\tmp\playership.o %NEODEV%\tmp\bullet.o -linput -lvideo -lc -lgcc -o test.o

@REM Pad program rom
objcopy -O binary test.o test.prg


@REM Pad modified roms. These are the result of the compilation
romwak /f dev_p1.rom 202-p1.bin 
romwak /p 202-p1.bin 202-p1.bin 1024 255 
romwak /w test.spr 202-c1.bin 202-c2.bin 
romwak /f 202-c1.bin 
romwak /f 202-c2.bin 
romwak /p 202-c1.bin 202-c1.bin 1024 255 
romwak /p 202-c2.bin 202-c2.bin 1024 255 
copy test.fix 202-s1.bin 

@REM Pad Puzzle De Pon! roms which are unchanged. They are copied from the puzzledepon_original_roms subdirectory
@REM into the current directory
copy puzzledepon_original_roms\202-m1.bin .\
romwak /p 202-s1.bin 202-s1.bin 128 255 
copy puzzledepon_original_roms\202-v1.bin .\
romwak /p 202-v1.bin 202-v1.bin 512 255 

@REM Copy modified game roms to CD output directory
del output\cd\*.spr
del output\cd\*.fix
del output\cd\*.prg
ren test.spr PB_CHR.SPR
ren test.fix PB_FIX.FIX
ren test.prg PB_PROG.PRG
move PB_CHR.SPR output\cd\
move PB_FIX.FIX output\cd\
move PB_PROG.PRG output\cd\

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

