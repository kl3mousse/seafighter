# $Id: Makefile,v 1.3 2001/05/03 13:43:42 fma Exp $

#######################################
# Base dir of your m68k gcc toolchain #
#######################################

BASEDIR = $(NEODEV)
AS = as
LD = gcc
CC = gcc
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
OUTPUT = cart

############################
# Settings for cart output #
############################
ROMSIZE = 0x20000
PADBYTE = 0x00

##############################
# Object Files and Libraries #
##############################

OBJS = $(TMPDIR)/crt0_$(OUTPUT).o $(TMPDIR)/main.o $(TMPDIR)/background.o $(TMPDIR)/gamelogic.o $(TMPDIR)/enemies.o
LIBS = -linput -lvideo -lc -lgcc

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
FIXFILES = fix_font.bmp

#############
# GFX Files #
#############

GRAPHICS = fix.pal starfield.bmp playership.bmp bullet.bmp enemies.bmp

####################
# GFX Object Files #
####################

GFXOBJ = $(TMPDIR)/palettes.o $(TMPDIR)/starfield.o $(TMPDIR)/playership.o $(TMPDIR)/bullet.o

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

$(TMPDIR)/starfield.o: starfield.map
	$(BIN2O) $< starfield $@

$(TMPDIR)/playership.o: playership.map
	$(BIN2O) $< playership $@

$(TMPDIR)/bullet.o: bullet.map
	$(BIN2O) $< bullet $@

$(TMPDIR)/enemies.o: enemies.map
	$(BIN2O) $< enemies $@
