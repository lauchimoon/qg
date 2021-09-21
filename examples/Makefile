CC ?= tcc
SRC ?= animation.c
LIB ?= libqg.a
LDLIBS = -lGL -lpthread -ldl -lm
OUT ?= animation

default:
	$(CC) $(SRC) $(LIB) $(LDLIBS) -o $(OUT)

clean:
ifeq ($(OS),Windows_NT)
	del *.exe
else
	find . -type f -executable -delete
endif

