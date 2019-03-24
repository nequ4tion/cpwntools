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
EXTERN_LIBS_DIR=./cpwn_extern
COMPILATION_CONSTANTS=-DCOMPILE_TIME
STDPWN_OBJECT_FILES=${STDPWN_DIR}/alloc.o ${STDPWN_DIR}/colors.o ${STDPWN_DIR}/cstring.o ${STDPWN_DIR}/error.o
NETWORKING_OBJECT_FILES=${NETWORKING_DIR}/tcp/tcp.o

compile: stdpwn networking

########################################################################################################################
#	Networking
########################################################################################################################

networking: .tcp.o ${NETWORKING_OBJECT_FILES}
	@echo Linking dynamic networking library...
	${CC} ${CFLAGS} ${LIBRARY_FLAGS} ${LDFLAGS} ${NETWORKING_DIR}/tcp/tcp.o -o ${NETWORKING_DIR}/networking.so ${COMPILATION_CONSTANTS}

networking_object_files: .tcp.o

.tcp.o: ${NETWORKING_DIR}/tcp/tcp.c
	@echo Compiling the tcp library...
	${CC} ${CFLAGS} ${LIBRARY_FLAGS} -c ${NETWORKING_DIR}/tcp/tcp.c -o ${NETWORKING_DIR}/tcp/tcp.o ${COMPILATION_CONSTANTS}

########################################################################################################################
#	stdpwn
########################################################################################################################

stdpwn: stdpwn_object_files
	${CC} ${CFLAGS} ${LIBRARY_FLAGS} ${LDFLAGS} -o ${STDPWN_DIR}/stdpwn.so \
														${STDPWN_DIR}/alloc.o \
														${STDPWN_DIR}/colors.o \
														${STDPWN_DIR}/cstring.o \
														${STDPWN_DIR}/error.o

stdpwn_object_files: .alloc.o .colors.o .cstring.o .error.o .bignum.o

.alloc.o: ${STDPWN_DIR}/alloc.c
	${CC} ${CFLAGS} ${LIBRARY_FLAGS} -c ${STDPWN_DIR}/alloc.c -o ${STDPWN_DIR}/alloc.o ${COMPILATION_CONSTANTS}

.colors.o: ${STDPWN_DIR}/colors.c
	${CC} ${CFLAGS} ${LIBRARY_FLAGS} -c ${STDPWN_DIR}/colors.c -o ${STDPWN_DIR}/colors.o ${COMPILATION_CONSTANTS}

.cstring.o: ${STDPWN_DIR}/cstring.c
	${CC} ${CFLAGS} ${LIBRARY_FLAGS} -c ${STDPWN_DIR}/cstring.c -o ${STDPWN_DIR}/cstring.o ${COMPILATION_CONSTANTS}

.error.o: ${STDPWN_DIR}/error.c
	${CC} ${CFLAGS} ${LIBRARY_FLAGS} -c ${STDPWN_DIR}/error.c -o ${STDPWN_DIR}/error.o ${COMPILATION_CONSTANTS}

.bignum.o: compile_bignum_extern
	${CC} ${CFLAGS} ${LIBRARY_FLAGS} -c ${STDPWN_DIR}/bignum.c -o ${STDPWN_DIR}/bignum.o ${COMPILATION_CONSTANTS}

########################################################################################################################
#	System install
########################################################################################################################
install: stdpwn_object_files networking_object_files
	@echo Compiling static library...
	@mkdir -p ${BUILD_DIR}
	${AR} rcs ${BUILD_DIR}/libcpwn.a $(shell find ./src/ -name \*.o) $(shell find ./cpwn_extern/ -name \*.o)
	sudo cp ${BUILD_DIR}/libcpwn.a /usr/lib
	@echo Compiling shared library...
	${CC} ${CFLAGS} ${LIBRARY_FLAGS} ${LDFLAGS} $(shell find ./src/ -name \*.o) -o ${BUILD_DIR}/libcpwndyn.so
	sudo cp ${BUILD_DIR}/libcpwndyn.so ${SYSTEM_LIB_PATH}
	sudo chmod 755 ${SYSTEM_LIB_PATH}/libcpwndyn.so

	sudo ldconfig -v -n /usr/lib | grep cpwn
	sudo find ${EXTERN_LIBS_DIR} -name '*.h' -exec cp --parents \{\} ${SYSTEM_INCLUDE_PATH} \;
	cd ./src; sudo find . -name '*.h' -exec cp --parents \{\} ${SYSTEM_INCLUDE_PATH} \;

########################################################################################################################
#	System deinstall
########################################################################################################################

deinstall:
	sudo rm -rf ${SYSTEM_INCLUDE_PATH}/cpwn ${SYSTEM_INCLUDE_PATH}/cpwn.h
	sudo rm -rf ${SYSTEM_INCLUDE_PATH}/${EXTERN_LIBS_DIR}
	sudo rm -f ${SYSTEM_LIB_PATH}/libcpwndyn.so
	sudo rm -f ${SYSTEM_LIB_PATH}/libcpwn.a

########################################################################################################################
#	System reinstall
########################################################################################################################

reinstall: deinstall install

########################################################################################################################
#	Compile/Pull extern libraries
########################################################################################################################

compile_bignum_extern: ${EXTERN_LIBS_DIR}/tiny-bignum-c/bn.c
	${CC} ${CFLAGS} ${LIBRARY_FLAGS} -c ${EXTERN_LIBS_DIR}/tiny-bignum-c/bn.c -o ${EXTERN_LIBS_DIR}/tiny-bignum-c/build/bn.o

pull_extern:
	git pull --recurse-submodules

########################################################################################################################
#	clean
########################################################################################################################

clean:
	rm -f $(shell find ./src -name \*.o)
	rm -f $(shell find ./src -name \*.so)
	rm -f ${OUTFILE}
	rm -rf ${BUILD_DIR}
	make -C ${EXTERN_LIBS_DIR}/tiny-bignum-c clean

.PHONY: clean stdpwn networking
