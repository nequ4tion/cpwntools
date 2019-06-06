cmake_minimum_required(VERSION 3.10.1)
set(CMAKE_C_STANDARD 11)

project(cpwntools)

set(CMAKE_BINARY_DIR build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if (UNIX)
    set(COMPILER_WARNING_FLAGS -Werror -Wall -Wextra)
    set(COMPILER_OPTIMIZATION_FLAGS -O2)
    set(COMPILER_SAFETY_FLAGS -fstack-protector-all)
else()
if(WIN32)
    set(COMPILER_SAFETY_FLAGS /GS)
endif(WIN32)
endif (UNIX)

find_package(Git)
if(NOT Git_FOUND)
    message(FATAL_ERROR "Need git to clone dependencies.")
endif()

function(pull_extern_git name url)

    if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/cpwn_extern/${name})
        execute_process(
            COMMAND ${GIT_EXECUTABLE} clone --recursive ${url} ${name}
            COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cpwn_extern
        )

    else()
        # directory exists, attempting to pull
        execute_process(
            COMMAND ${GIT_EXECUTABLE} -C ${name} pull origin master
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cpwn_extern
        )
    endif()

endfunction()

pull_extern_git(tiny-bignum-c https://github.com/kokke/tiny-bignum-c)
pull_extern_git(libssh https://git.libssh.org/projects/libssh.git)

set(STDPWN_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cpwn/stdpwn/alloc.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cpwn/stdpwn/error.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cpwn/stdpwn/bignum.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cpwn/stdpwn/colors.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cpwn/stdpwn/cstring.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/cpwn_extern/tiny-bignum-c/bn.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cpwn/stdpwn/stdpwn.c"
)


set(STDPWN_INCLUDES
    "/usr/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cpwn/stdpwn"
)

set(NETWORKING_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cpwn/networking/tcp/tcp.c"
)

set(NETWORKING_INCLUDES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cpwn/networking"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cpwn/networking/tcp"
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

add_definitions(-DCOMPILE_TIME)
target_compile_options(cpwn PRIVATE ${COMPILER_WARNING_FLAGS} ${COMPILER_SAFETY_FLAGS} ${COMPILER_OPTIMIZATION_FLAGS})
target_compile_options(cpwnnetworking PRIVATE ${COMPILER_WARNING_FLAGS} ${COMPILER_SAFETY_FLAGS} ${COMPILER_OPTIMIZATION_FLAGS})
target_compile_options(stdpwn PRIVATE ${COMPILER_WARNING_FLAGS} ${COMPILER_SAFETY_FLAGS} ${COMPILER_OPTIMIZATION_FLAGS})

target_include_directories(cpwnnetworking PRIVATE ${NETWORKING_INCLUDES})

target_compile_definitions(cpwn PRIVATE COMPILE_TIME)
target_compile_definitions(stdpwn PRIVATE COMPILE_TIME)
