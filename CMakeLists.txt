cmake_minimum_required(VERSION 3.10)
set(CMAKE_C_STANDARD 11)

project(cpwntools)

set(CMAKE_BINARY_DIR build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(COMPILER_WARNING_FLAGS -Werror -pedantic -Wall -Wextra)
set(COMPILER_OPTIMIZATION_FLAGS -O2)
set(COMPILER_SAFETY_FLAGS -fstack-protector-all)

set(STDPWN_FILES 
    "src/cpwn/stdpwn/alloc.c"
    "src/cpwn/stdpwn/error.c"
    "src/cpwn/stdpwn/bignum.c"
    "src/cpwn/stdpwn/colors.c"
    "src/cpwn/stdpwn/cstring.c"
    "cpwn_extern/tiny-bignum-c/bn.c"
)

set(STDPWN_INCLUDES
    "/usr/include"
    "src/cpwn/stdpwn"
)

set(NETWORKING_FILES
    "src/cpwn/networking/tcp/tcp.c"
)

set(NETWORKING_INCLUDES
    "src/cpwn/networking"
    "src/cpwn/networking/tcp"
)

add_library(cpwn STATIC
    ${STDPWN_FILES}
    ${NETWORKING_FILES}
)

add_library(cpwndyn SHARED
    ${STDPWN_FILES}
    ${NETWORKING_FILES}
)

add_library(stdpwn SHARED ${STDPWN_FILES})

add_library(cpwnnetworking SHARED ${NETWORKING_FILES})

target_include_directories(stdpwn PRIVATE
    ${STDPWN_INCLUDES}
)

target_include_directories(cpwn PRIVATE
    ${STDPWN_INCLUDES}
    ${NETWORKING_INCLUDES}
)


target_compile_options(cpwn PRIVATE ${COMPILER_WARNING_FLAGS} ${COMPILER_SAFETY_FLAGS} ${COMPILER_OPTIMIZATION_FLAGS})
target_compile_options(cpwnnetworking PRIVATE ${COMPILER_WARNING_FLAGS} ${COMPILER_SAFETY_FLAGS} ${COMPILER_OPTIMIZATION_FLAGS})
target_compile_options(stdpwn PRIVATE ${COMPILER_WARNING_FLAGS} ${COMPILER_SAFETY_FLAGS} ${COMPILER_OPTIMIZATION_FLAGS})

target_include_directories(cpwnnetworking PRIVATE ${NETWORKING_INCLUDES})

target_compile_definitions(cpwn PRIVATE COMPILE_TIME)
target_compile_definitions(stdpwn PRIVATE COMPILE_TIME)
