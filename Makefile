.PHONY: all, clean, install

CC = gcc
OBJS = qg.o \
	   qglfw.o
ROOT = $(shell whoami)

default: qg

qg: $(OBJS)
	ar rcs libqg.a $(OBJS)

gq.o: qg.c
	$(CC) -c qg.c

qglfw.o: qglfw.c
	$(CC) -c qglfw.c

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
