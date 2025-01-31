AUTOMAKE_OPTIONS = foreign nostdinc

.DELETE_ON_ERROR:

bin_PROGRAMS = ntuget

include Makefile.inc

ntuget_SOURCES = $(NTUGET_FILES)

AM_CFLAGS = -I$(top_srcdir)/include $(DEPS_CFLAGS)
AM_LDFLAGS =

ntuget_CFLAGS = $(AM_CFLAGS)
ntuget_LDADD = $(DEPS_LIBS) -lm
