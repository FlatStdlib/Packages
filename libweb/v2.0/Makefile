DEBUG=""
.PHONY: all

all: compile

install:
	mkdir build
	cd build; git clone https://github.com/FlatStdlib/fsl.git
	cd build; git clone https://github.com/FlatStdlib/Stdlib.git
	cd build/Stdlib; sudo make ${DEBUG}
	rm -rf build


compile:
	fsl --o t main.c src/*.c --cflags -ggdb