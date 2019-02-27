##########################################
# Usage:
# make (rom)				# compile the whole
# make Install			# install for Windows
# make gfx					# rebuild Graphics
# make fix_gfx			# rebuild FIX rom
# make clean				# remove temp files
# make test				  # remove temp files
# make play         # launch MAME
#
# note: I hate makefiles. Hoping this one
#       is easier to understand than the
#       average. It is not optimised, it
#       is made to be read to understand
#       how the set of romfiles is built
##########################################

.DEFAULT_GOAL := rom
.PHONY: rom gfx  clean install main_pgm

rom: main_pgm

#######################################
# Test (used by TravisCI)
#######################################
test: main_pgm

#######################################
# Play using MAME
#######################################
play:
	testwithMAME.bat

#############################################
# Install: set proper Windows env. variables
#############################################
install:
	setx neodev %cd%\include\NeoDev
	.\tools\pathed\pathed /user /add "%cd%\include\NeoDev\m68k\bin"
	.\tools\7zip\7za e .\tools\mame\mame64.exe.zip -o.\tools\mame\


#############################################
# main program C compilation
#############################################
main_pgm:
	.\include\NeoDev\m68k\bin\as -m68000 --register-prefix-optional .\src\crt0_cart.s -o include\NeoDev\tmp\crt0_cart.o
	gcc -I.\include\NeoDev\m68k\include -m68000 -O3 -Wall -fomit-frame-pointer -ffast-math -fno-builtin -nostartfiles -nodefaultlibs -D__cart__ -c src\main.c -o .\include\NeoDev\tmp\main.o
	.\include\NeoDev\m68k\bin\gcc -L.\include\NeoDev\m68k\lib -m68000 -O3 -Wall -fomit-frame-pointer -ffast-math -fno-builtin -nostartfiles -nodefaultlibs -D__cart__ -Wl,-T.\include\NeoDev\src\system\neocart.x .\include\NeoDev\tmp\crt0_cart.o .\include\NeoDev\tmp\main.o .\include\NeoDev\tmp\charMaps.o .\include\NeoDev\tmp\fixPals.o .\include\NeoDev\tmp\charPals.o -lDATlib -lc -lgcc -o test.o
	.\include\NeoDev\m68k\bin\objcopy --gap-fill=0x00 --pad-to=0x20000 -R .data -O binary test.o dev_p1.rom
	.\include\NeoDev\m68k\bin\romwak /f dev_p1.rom 444-p1.bin
	.\include\NeoDev\m68k\bin\romwak /p 444-p1.bin 444-p1.bin 1024 255
	del dev_p1.rom
	del test.o
	mv 444-p1.bin output\cartridge

#######################################
# NEOGEO GRAPHIC ROMS
#######################################
gfx:
	@echo "GFX..."
	.\include\DATlib\NeoDev\m68k\bin\Buildchar.exe src\chardata.xml
	.\include\DATlib\NeoDev\m68k\bin\charSplit.exe gfxout\char.bin -rom gfxout\444
	mv gfxout\444.C1 output\cartridge\444-c1.bin
	mv gfxout\444.C2 output\cartridge\444-c2.bin
	.\include\NeoDev\m68k\bin\as -m68000 --register-prefix-optional .\gfxout\charPals.s -o .\include\NeoDev\tmp\charPals.o
	.\include\NeoDev\m68k\bin\as -m68000 --register-prefix-optional .\gfxout\charMaps.s -o .\include\NeoDev\tmp\charMaps.o
	@echo "FIX..."
	include\DATlib\NeoDev\m68k\bin\Buildchar.exe src\fixData.xml
	cp gfxout\fix.bin output\cartridge\444-s1.bin
	.\include\NeoDev\m68k\bin\as -m68000 --register-prefix-optional .\gfxout\fixPals.s -o .\include\NeoDev\tmp\fixPals.o

#######################################

clean:
	cls
	@echo "Cleaning up..."
	rm -f gfxout\*.*
	rm -f include\NeoDev\tmp\*.*
