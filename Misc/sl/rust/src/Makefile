#==========================================
#    Makefile: makefile for sl 5.1
#	Copyright 1993, 1998, 2014
#                 Toyoda Masashi
#		  (mtoyoda@acm.org)
#	Last Modified: 2014/03/31
#==========================================

CC=gcc
CFLAGS=-O -Wall
AR=ar
ARFLAGS=rcs

all: sl libsl rust

sl:
	$(CC) $(CFLAGS) -fPIC -lncurses -c sl.c -o build/sl.o

libsl:
	$(AR) $(ARFLAGS) lib/libsl.a build/sl.o

rust: 
	yes | cp lib/libsl.a ../deps/.

clean:
	rm -r build/sl.o

distclean: clean
