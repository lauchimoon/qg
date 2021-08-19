CC = gcc
SRC = main.c
LIBPATH = libqg.a			# Make sure to have this file in this same directory
LDLIBS = -lGL -lm -ldl -lpthread
OUT = -o maze

all:
	$(CC) $(SRC) $(LIBPATH) $(LDLIBS) $(OUT)
