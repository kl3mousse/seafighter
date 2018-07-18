@REM cd include\NeoDev\m68k\bin

@REM gcc -I..\include -m68000 -O3 -Wall -fomit-frame-pointer -ffast-math -fno-builtin -nostartfiles -nodefaultlibs -D__cart__ -c ..\..\..\..\main.c -o ..\..\tmp\main.o

@REM cd ..\..\..\..

gcc -Iinclude\NeoDev\m68k\include -m68000 -O3 -Wall -fomit-frame-pointer -ffast-math -fno-builtin -nostartfiles -nodefaultlibs -D__cart__ -c main.c -o include\NeoDev\tmp\main.o
