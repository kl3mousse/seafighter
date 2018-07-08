
@set MAMEDIR=c:\MAME
@set PROJECTDIR=c:\neogeo\NeoDev\src\neothunder-seafighter

cd %MAMEDIR%

del roms\puzzledp\*.c*
del roms\puzzledp\*.m*
del roms\puzzledp\*.p*
del roms\puzzledp\*.s*
del roms\puzzledp\*.v*

ren roms\puzzledp\202-c1.bin 202-c1.c1
ren roms\puzzledp\202-c2.bin 202-c2.c2
ren roms\puzzledp\202-m1.bin 202-m1.m1
ren roms\puzzledp\202-p1.bin 202-p1.p1
ren roms\puzzledp\202-s1.bin 202-s1.s1
ren roms\puzzledp\202-v1.bin 202-v1.v1

mame64 neogeo puzzledp

cd %PROJECTDIR%