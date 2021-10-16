.PHONY: all, clean, install

# On Windows the CC variable may throw the following error:
# process_begin: CreateProcess(NULL, cc, ...) failed.
# Please, when compiling make sure to specify your compiler.
# It can be gcc, it can be tcc, anything.
# mingw32-make.exe CC=gcc
CC ?= gcc
OBJS = qg.o \
	   qglfw.o
CFLAGS = -Wall
ROOT = $(shell whoami)

default: qg

qg: $(OBJS)
	ar rcs libqg.a $(OBJS)

qg.o: qg.c
	$(CC) $(CFLAGS) -c qg.c

qglfw.o: qglfw.c
	$(CC) -c $(CFLAGS) qglfw.c

clean:
ifeq ($(OS), Windows_NT)
	del *.o libqg.a
else
	rm -f *.o libqg.a
endif
	@echo Cleaning done

install:
ifeq ($(ROOT),root)
	cp qg.h /usr/include/
else
	@echo To install you must be root.
endif
