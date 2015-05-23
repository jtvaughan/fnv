#!/usr/bin/make -f

# Compute Fowler-Noll-Vo (FNV) Hashes for Arguments and Standard Input
# Written in 2015 by Jordan Vaughan
#
# To the extent possible under law, the author(s) have dedicated all copyright
# and related and neighboring rights to this software to the public domain
# worldwide. This software is distributed without any warranty.
#
# You should have received a copy of the CC0 Public Domain Dedication along
# with this software. If not, see
# <http://creativecommons.org/publicdomain/zero/1.0/>.


# CONFIGURATION VARIABLES
# Feel free to set these at the command line.

# the name of the generated binary
BINFILE ?= fnv

# compilation flags
CFLAGS ?= -std=c11 -O2 -g -Wall -Werror
CPPFLAGS ?=

# the installation program (install(1))
INSTALL ?= install

# the user and group for the installed binary
INSTALL_USER ?= root
INSTALL_GROUP ?= root

# the installation prefix
PREFIXDIR ?= /usr

# the directory that will hold the installed binary
BINDIR ?= $(PREFIXDIR)/bin

# the name of the ptar archive that the 'dist' target builds
DISTARCHIVE ?= $(BINFILE).tar


# INTERNAL VARIABLES
# Do not modify these from the command line.
SRC = fnv.c hash/fnv.c
OBJ = $(SRC:.c=.o)
INSTALL_PROGRAM = $(INSTALL) -p -o $(INSTALL_USER) -g $(INSTALL_GROUP) -m 755 -s
DISTCONTENTS = COPYING README $(BINFILE)


# TARGETS
all: options $(BINFILE)

options:
	@echo "fnv build options:"
	@echo "CFLAGS  = $(CFLAGS)"
	@echo "LDFLAGS = $(LDFLAGS)"
	@echo "CC      = $(CC)"
	@echo

%.o: %.c
	$(CC) -c $(CPPFLAGS) -o $@ $(CFLAGS) $<

$(BINFILE): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

clean:
	rm -f $(BINFILE) $(OBJ) $(DISTARCHIVE)

install: $(BINFILE)
	$(INSTALL_PROGRAM) $(BINFILE) $(BINDIR)/$(BINFILE)

dist: $(DISTARCHIVE)

$(DISTARCHIVE): all
	tar c $(DISTCONTENTS) >$@
