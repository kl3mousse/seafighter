
@set MAMEDIR=%cd%\tools\MAME
@set PROJECTDIR=%cd%

@REM Copy all game roms to MAME (should be six .bin files)
del %MAMEDIR%\roms\seafighter\*.bin
copy output\cartridge\*.bin %MAMEDIR%\roms\seafight\

cd %MAMEDIR%

del roms\seafight\*.c*
del roms\seafight\*.m*
del roms\seafight\*.p*
del roms\seafight\*.s*
del roms\seafight\*.v*

ren roms\seafight\444-c1.bin 444-c1.c1
ren roms\seafight\444-c2.bin 444-c2.c2
ren roms\seafight\444-m1.bin 444-m1.m1
ren roms\seafight\444-p1.bin 444-p1.p1
ren roms\seafight\444-s1.bin 444-s1.s1
ren roms\seafight\444-v1.bin 444-v1.v1

@rem mame64 NEOGEO SEAFIGHTER -bios unibios32
mame64 neogeo -cart1 seafight -bios unibios32

cd %PROJECTDIR%
