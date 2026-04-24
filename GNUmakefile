# $Id: GNUmakefile 69899 2013-05-17 10:05:33Z gcosmo $
# --------------------------------------------------------------
# GNUmakefile for examples module.  Gabriele Cosmo, 06/04/98.
# --------------------------------------------------------------

name := QF_ND5x2
G4TARGET := $(name)
G4EXLIB := true

ROOTCFLAGS    = $(shell root-config --cflags)
ROOTLIBS      = $(shell root-config --libs)
ROOTGLIBS     = $(shell root-config --glibs)

CPPFLAGS  += $(ROOTCFLAGS)
EXTRALIBS += $(ROOTLIBS) $(ROOTGLIBS)

ifndef G4INSTALL
  G4INSTALL = ../../..
endif

.PHONY: all
all: lib bin

include $(G4INSTALL)/config/binmake.gmk
