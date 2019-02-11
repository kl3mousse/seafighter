##########################################
# Usage:
# make (rom)				# compile the whole
# make Install			# install for Windows
# make gfx					# rebuild Graphics
# make fix_gfx			# rebuild FIX rom
# make clean				# remove temp files
#
# note: I hate makefiles. Hoping this one
#       is easier to understand than the
#       average. It is not optimised, it
#       is made to be read to understand
#       how the set of romfiles is built
##########################################

.DEFAULT_GOAL := rom
.PHONY: rom gfx fix_gfx clean install cart_header main_pgm main_linking

rom: gfx fix_gfx

#######################################
# Test (used by TravisCI)
#######################################
test: main_pgm

#############################################
# Install: set proper Windows env. variables
#############################################
install:
	setx neodev %cd%\include\NeoDev
	.\tools\pathed\pathed /user /add "%cd%\include\NeoDev\m68k\bin"
	.\tools\7zip\7za e .\tools\mame\mame64.exe.zip -o.\tools\mame\

#############################################
# cartridge header ASM compilation
#############################################
cart_header: include\NeoDev\tmp\crt0_cart.o
include\NeoDev\tmp\crt0_cart.o: src\crt0_cart.s
	.\include\NeoDev\m68k\bin\as -m68000 --register-prefix-optional .\src\crt0_cart.s -o include\NeoDev\tmp\crt0_cart.o

#############################################
# main program C compilation
#############################################
main_pgm: include\NeoDev\tmp\main.o
include\NeoDev\tmp\main.o: src\main.c ..\gfxout\charInclude.h ..\gfxout\fixData.h
	gcc -I.\include\NeoDev\m68k\include -m68000 -O3 -Wall -fomit-frame-pointer -ffast-math -fno-builtin -nostartfiles -nodefaultlibs -D__cart__ -c src\main.c -o .\include\NeoDev\tmp\main.o

#############################################
# main Linking
#############################################
main_linking:
	.\include\NeoDev\m68k\bin\gcc -L.\include\NeoDev\m68k\lib -m68000 -O3 -Wall -fomit-frame-pointer -ffast-math -fno-builtin -nostartfiles -nodefaultlibs -D__cart__ -Wl,-T.\include\NeoDev\src\system\neocart.x .\include\NeoDev\tmp\crt0_cart.o .\include\NeoDev\tmp\main.o .\include\NeoDev\tmp\charMaps.o .\include\NeoDev\tmp\fixPals.o .\include\NeoDev\tmp\charPals.o -lDATlib -lc -lgcc -o test.o
	.\include\NeoDev\m68k\bin\objcopy --gap-fill=0x00 --pad-to=0x20000 -R .data -O binary test.o dev_p1.rom


#######################################
# NEOGEO GRAPHIC ROMS
#######################################
gfx ..\gfxout\charInclude.h: src\chardata.xml
	@echo "GFX..."
	.\include\DATlib\NeoDev\m68k\bin\Buildchar.exe src\chardata.xml
	.\include\DATlib\NeoDev\m68k\bin\charSplit.exe gfxout\char.bin -rom gfxout\444
	 .\include\NeoDev\m68k\bin\as -m68000 --register-prefix-optional .\gfxout\charPals.s -o .\include\NeoDev\tmp\charPals.o
	 .\include\NeoDev\m68k\bin\as -m68000 --register-prefix-optional .\gfxout\charMaps.s -o .\include\NeoDev\tmp\charMaps.o

#######################################
# NEOGEO FIX ROM
#######################################
fix_gfx ..\gfxout\fixData.h: gfx\fix\systemFont.bin src\fixData.xml
	@echo "FIX..."
	include\DATlib\NeoDev\m68k\bin\Buildchar.exe src\fixData.xml
	cp gfxout\fix.bin gfxout\444.S1
	.\include\NeoDev\m68k\bin\as -m68000 --register-prefix-optional .\gfxout\fixPals.s -o .\include\NeoDev\tmp\fixPals.o

#######################################

clean:
	cls
	@echo "Cleaning up..."
	rm -f gfxout\*.*
	rm -f include\NeoDev\tmp\*.*
