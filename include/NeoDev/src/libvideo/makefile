# $Id: makefile,v 1.4 2001/05/03 07:14:29 fma Exp $

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

#######################################
# Path to libraries and include files #
#######################################

INCDIR = $(BASEDIR)/m68k/include
LIBDIR = $(BASEDIR)/m68k/lib
TMPDIR = $(BASEDIR)/tmp

#####################
# Compilation Flags #
#####################

ASFLAGS = -m68000 --register-prefix-optional
LDFLAGS = -Wl,-Ttext=0x0
CCFLAGS = -m68000 -O3 -Wall -fomit-frame-pointer -ffast-math -fno-builtin
ARFLAGS = cr
DEBUG = -g

##############
# Make rules #
##############

all: $(LIBDIR)/libvideo.a

$(LIBDIR)/libvideo.a:	$(TMPDIR)/libvideo.o $(TMPDIR)/video.o \
						$(TMPDIR)/fade.o $(TMPDIR)/fadetoblack.o \
						$(TMPDIR)/fadetowhite.o
	$(AR) $(ARFLAGS) $@ $^

$(TMPDIR)/fadetoblack.o: fadetoblack.bin
	$(BIN2O) $< _fade_to_black $@

$(TMPDIR)/fadetowhite.o: fadetowhite.bin
	$(BIN2O) $< _fade_to_white $@

$(TMPDIR)/%.o: %.c
	$(CC) -I$(INCDIR) $(DEBUG) $(CCFLAGS) -c $< -o $@

$(TMPDIR)/%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -f $(TMPDIR)/*.o
	rm -f $(LIBDIR)/libvideo.a

