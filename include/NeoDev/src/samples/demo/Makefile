# $Id: Makefile,v 1.10 2001/07/11 12:23:35 fma Exp $

#######################################
# Base dir of your m68k gcc toolchain #
#######################################

BASEDIR = $(NEODEV)
AS = as
LD = gcc
CC = gcc
AR = ar
OBJC = objcopy
BIN2O = bin2elf
GFXCC = gfxcc
FIXCNV = fixcnv

#######################################
# Path to libraries and include files #
#######################################

INCDIR = $(BASEDIR)/m68k/include
LIBDIR = $(BASEDIR)/m68k/lib
TMPDIR = $(BASEDIR)/tmp

###################################
# Output: {cart, cd} *lower case* #
###################################
OUTPUT = cd

############################
# Settings for cart output #
############################
ROMSIZE = 0x20000
PADBYTE = 0x00

#####################
# Compilation Flags #
#####################

ASFLAGS = -m68000 --register-prefix-optional
LDFLAGS = -Wl,-T$(BASEDIR)/src/system/neo$(OUTPUT).x
CCFLAGS = -m68000 -O3 -Wall -fomit-frame-pointer -ffast-math -fno-builtin -nostartfiles -nodefaultlibs -D__$(OUTPUT)__

ARFLAGS = cr

DEBUG = -g

##################
# FIX Definition #
##################
FIXFILES = $(BASEDIR)/src/shared/fix_font.bmp

#############
# GFX Files #
#############

GRAPHICS = fix.pal gfx/font.bmp gfx/logo_supremacy.bmp \
	gfx/luciole_small1.bmp gfx/luciole_small2.bmp \
	gfx/luciole_big1.bmp gfx/luciole_big2.bmp gfx/backgnd.bmp \
	gfx/boule1.bmp gfx/boule2.bmp gfx/boule3.bmp gfx/boule4.bmp \
	gfx/border.bmp
	
####################
# GFX Object Files #
####################

GFXOBJ =	$(TMPDIR)/palettes.o $(TMPDIR)/map_font.o $(TMPDIR)/map_logo.o \
			$(TMPDIR)/map_luciola_small1.o $(TMPDIR)/map_luciola_small2.o \
			$(TMPDIR)/map_luciola_big1.o $(TMPDIR)/map_luciola_big2.o \
			$(TMPDIR)/map_backgnd.o $(TMPDIR)/map_boule1.o \
			$(TMPDIR)/map_boule2.o $(TMPDIR)/map_boule3.o \
			$(TMPDIR)/map_boule4.o $(TMPDIR)/map_border.o

##############################
# Object Files and Libraries #
##############################

OBJS =	$(TMPDIR)/crt0_$(OUTPUT).o $(TMPDIR)/main.o \
		$(TMPDIR)/task_disp.o \
		$(TMPDIR)/task_fader.o $(TMPDIR)/task_scroll.o \
		$(TMPDIR)/task_luciola.o $(TMPDIR)/charmap.o

LIBS = -linput -lprocess -lmath -lvideo -lc -lgcc

##############
# Make rules #
##############

ifeq ($(OUTPUT),cart)
dev_p1.rom : test.o
	$(OBJC) --gap-fill=$(PADBYTE) --pad-to=$(ROMSIZE) -R .data -O binary $< $@
else
test.prg : test.o
	$(OBJC) -O binary $< $@
endif

test.o : test.fix test.spr $(OBJS) $(GFXOBJ)
	$(LD) -L$(LIBDIR) $(CCFLAGS) $(LDFLAGS) $(OBJS) $(GFXOBJ) $(LIBS) -o $@

$(TMPDIR)/charmap.o: charmap.bin
	$(BIN2O) $< scroll_charmap $@

$(TMPDIR)/%.o: %.c
	$(CC) -I$(INCDIR) $(CCFLAGS) -c $< -o $@

$(TMPDIR)/%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -f $(TMPDIR)/*.*
	rm -f fix.pal
	rm -f test.o
	rm -f test.prg
	rm -f test.fix
	rm -f test.spr
	rm -f dev_p1.rom

############
# FIX Rule #
############

test.fix : $(FIXFILES)
	$(FIXCNV) $(FIXFILES) -o $@ -pal fix.pal

#############
# GFX Rules #
#############

test.spr : $(GRAPHICS)
	$(GFXCC) -black $(GRAPHICS) -o $@

$(TMPDIR)/palettes.o: test.pal
	$(BIN2O) $< palettes $@

$(TMPDIR)/map_font.o: gfx/font.map
	$(BIN2O) $< map_font $@

$(TMPDIR)/map_logo.o: gfx/logo_supremacy.map
	$(BIN2O) $< map_logo $@

$(TMPDIR)/map_luciola_small1.o: gfx/luciole_small1.map
	$(BIN2O) $< map_luciola_small1 $@

$(TMPDIR)/map_luciola_small2.o: gfx/luciole_small2.map
	$(BIN2O) $< map_luciola_small2 $@

$(TMPDIR)/map_luciola_big1.o: gfx/luciole_big1.map
	$(BIN2O) $< map_luciola_big1 $@

$(TMPDIR)/map_luciola_big2.o: gfx/luciole_big2.map
	$(BIN2O) $< map_luciola_big2 $@

$(TMPDIR)/map_backgnd.o: gfx/backgnd.map
	$(BIN2O) $< map_backgnd $@

$(TMPDIR)/map_boule1.o: gfx/boule1.map
	$(BIN2O) $< map_boule1 $@

$(TMPDIR)/map_boule2.o: gfx/boule2.map
	$(BIN2O) $< map_boule2 $@

$(TMPDIR)/map_boule3.o: gfx/boule3.map
	$(BIN2O) $< map_boule3 $@

$(TMPDIR)/map_boule4.o: gfx/boule4.map
	$(BIN2O) $< map_boule4 $@

$(TMPDIR)/map_border.o: gfx/border.map
	$(BIN2O) $< map_border $@
