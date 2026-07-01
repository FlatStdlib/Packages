.PHONY: all

all: compile_w_ssl

download_lb:
	git clone https://github.com/CoreSTD/base.git
	git clone https://github.com/CoreSTD/fsl.git
	cd base; sudo make; cd ..
	sudo make lib
	rm -rf base
	rm -rf fsl-compiler

compile_w_ssl:
	gcc -ffreestanding \
	tests/web.c src/*.c \
	/usr/lib/libfsl.a \
	-lssl -lcrypto


lib:
	fsl -c --output a.out \
    src/webserver.c \
    src/parser.c \
    src/router.c \
    src/utils.c
	ar rcs libweb.a *.o
	cp libweb.a /usr/lib
	rm *.o

# Bloatware
NOSSL:
	fsl --output a.out \
	tests/web.c \
	src/webserver.c \
	src/parser.c \
	src/router.c \
	src/utils.c 
