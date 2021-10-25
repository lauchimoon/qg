.PHONY: all, clean, install

CC = gcc
OBJS = qg.o \
	   qglfw.o
CFLAGS = -Wall
QG_LOG = -DQG_NO_LOG
ROOT = $(shell whoami)

default: qg

qg: $(OBJS)
	ar rcs libqg.a $(OBJS)

qg.o: qg.c
	$(CC) $(CFLAGS) $(QG_LOG) -c qg.c

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
