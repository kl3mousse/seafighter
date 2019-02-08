#######################################
# Usage:
# make (rom)				# compile the whole
# make gfx					# rebuild Graphics
# make fix_gfx			# rebuild FIX rom
# make clean				# remove temp files
#######################################

.DEFAULT_GOAL := rom
.PHONY: rom gfx fix_gfx clean

rom: gfx fix_gfx

#######################################
# NEOGEO GRAPHIC ROMS
#######################################
gfx: src\chardata.xml
	@echo "GFX..."
	include\DATlib\NeoDev\m68k\bin\Buildchar.exe src\chardata.xml
	include\DATlib\NeoDev\m68k\bin\charSplit.exe gfxout\char.bin -rom gfxout\444

#######################################
# NEOGEO FIX ROM
#######################################
fix_gfx: gfx\fix\systemFont.bin src\fixData.xml
	@echo "FIX..."
	include\DATlib\NeoDev\m68k\bin\Buildchar.exe src\fixData.xml
	cp gfxout\fix.bin gfxout\444.S1

#######################################

clean:
	cls
	@echo "Cleaning up..."
	rm -f gfxout\*.*
