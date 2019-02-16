ifneq (,)
This makefile requires GNU Make.
endif

CC?=cc
AR?=ar
CFLAGS:=-O2 -fstack-protector-all -fPIE -fPIC -pipe
LIBRARY_FLAGS=-Werror -pedantic -Wall -Wextra
LDFLAGS:=-shared
STDPWN_DIR=src/cpwn/stdpwn
NETWORKING_DIR=src/cpwn/networking
CFILES:=${STDPWN_DIR}/alloc.c ${STDPWN_DIR}/colors.c ${STDPWN_DIR}/cstring.c ${STDPWN_DIR}/error.c ${NETWORKING_DIR}/tcp/tcp.c
SYSTEM_LIB_PATH?=/usr/lib
SYSTEM_INCLUDE_PATH?=/usr/include
BUILD_DIR?=./build

compile: stdpwn networking

########################################################################################################################
#	Networking
########################################################################################################################

networking: .tcp.o
	@echo Linking dynamic networking library...
	${CC} ${CFLAGS} ${LIBRARY_FLAGS} ${LDFLAGS} ${NETWORKING_DIR}/tcp/tcp.o -o ${NETWORKING_DIR}/networking.so

networking_object_files: .tcp.o

.tcp.o:
	@echo Compiling the tcp library...
	${CC} ${CFLAGS} ${LIBRARY_FLAGS} -c ${NETWORKING_DIR}/tcp/tcp.c -o ${NETWORKING_DIR}/tcp/tcp.o

########################################################################################################################
#	stdpwn
########################################################################################################################

stdpwn: stdpwn_object_files
	${CC} ${CFLAGS} ${LIBRARY_FLAGS} ${LDFLAGS} -o ${STDPWN_DIR}/stdpwn.so ${STDPWN_DIR}/alloc.o ${STDPWN_DIR}/colors.o ${STDPWN_DIR}/cstring.o ${STDPWN_DIR}/error.o

stdpwn_object_files: .alloc.o .colors.o .cstring.o .error.o

.alloc.o:
	${CC} ${CFLAGS} ${LIBRARY_FLAGS} -c ${STDPWN_DIR}/alloc.c -o ${STDPWN_DIR}/alloc.o

.colors.o:
	${CC} ${CFLAGS} ${LIBRARY_FLAGS} -c ${STDPWN_DIR}/colors.c -o ${STDPWN_DIR}/colors.o

.cstring.o:
	${CC} ${CFLAGS} ${LIBRARY_FLAGS} -c ${STDPWN_DIR}/cstring.c -o ${STDPWN_DIR}/cstring.o

.error.o:
	${CC} ${CFLAGS} ${LIBRARY_FLAGS} -c ${STDPWN_DIR}/error.c -o ${STDPWN_DIR}/error.o

########################################################################################################################
#	System install
########################################################################################################################
install: stdpwn_object_files networking_object_files
	@echo Compiling static library...
	@mkdir -p ${BUILD_DIR}
	${AR} rcs ${BUILD_DIR}/libcpwn.a $(shell find ./src/ -name \*.o)
	sudo cp ${BUILD_DIR}/libcpwn.a /usr/lib
	@echo Compiling shared library...
	${CC} ${CFLAGS} ${LIBRARY_FLAGS} ${LDFLAGS} $(shell find ./src/ -name \*.o) -o ${BUILD_DIR}/libcpwndyn.so
	sudo cp ${BUILD_DIR}/libcpwndyn.so ${SYSTEM_LIB_PATH}
	sudo chmod 755 ${SYSTEM_LIB_PATH}/libcpwndyn.so

	sudo ldconfig -v -n /usr/lib | grep cpwn
	cd ./src; sudo find . -name '*.h' -exec cp --parents \{\} ${SYSTEM_INCLUDE_PATH} \;

########################################################################################################################
#	System deinstall
########################################################################################################################

deinstall:
	sudo rm -rf ${SYSTEM_INCLUDE_PATH}/cpwn ${SYSTEM_INCLUDE_PATH}/cpwn.h
	sudo rm -f ${SYSTEM_LIB_PATH}/libcpwndyn.so
	sudo rm -f ${SYSTEM_LIB_PATH}/libcpwn.a

########################################################################################################################
#	System reinstall
########################################################################################################################

reinstall: deinstall install



########################################################################################################################
#	clean
########################################################################################################################

clean:
	rm -f $(shell find ./src -name \*.o) $(shell find ./src -name \*.o)
	rm -f ${OUTFILE}
	rm -rf ${BUILD_DIR}

.PHONY: clean stdpwn networking
