ELF=menuconfig

CFLAGS = -g -Wall
LDFLAGS = -lncursesw

all:
	gcc $(CFLAGS) main.c $(LDFLAGS) -o $(ELF)

clean:
	rm -rf ${ELF}

.PHONY: clean

