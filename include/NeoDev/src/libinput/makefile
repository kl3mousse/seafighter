# $Id: makefile,v 1.2 2001/04/13 10:05:56 fma Exp $

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

all: $(LIBDIR)/libinput.a

$(LIBDIR)/libinput.a: $(TMPDIR)/libinput.o
	$(AR) $(ARFLAGS) $@ $^

$(TMPDIR)/%.o: %.c
	$(CC) -I$(INCDIR) $(DEBUG) $(CCFLAGS) -c $< -o $@

$(TMPDIR)/%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -f $(TMPDIR)/*.o
	rm -f $(LIBDIR)/libinput.a