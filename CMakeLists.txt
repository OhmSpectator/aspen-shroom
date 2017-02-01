cmake_minimum_required(VERSION 3.6)

set(CMAKE_VERBOSE_MAKEFILE ON)

project(aspen_shroom C ASM)
set(CMAKE_SYSTEM_PROCESSOR arm)

include_directories(.)

set(TOOLCHAIN_PREFIX $ENV{CROSS_COMPILE})

if(NOT ${TOOLCAIN_PREFIX})
    set(TOOLCHAIN_PREIX arm-linux-gnueabihf-)
endif()

set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_C_FLAGS "")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pipe")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-builtin")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffreestanding")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-common")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-stack-protector")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-delete-null-pointer-checks")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-pic")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffunction-sections")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fdata-sections")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-toplevel-reorder")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffixed-r9")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -marm")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=armv7ve")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mabi=aapcs")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mhard-float")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mno-thumb-interwork")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mword-relocations")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mno-unaligned-access")

set(STANDALONE_START_ADDR 0x81000000)
set(CMAKE_EXE_LINKER_FLAGS "")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Ttext ${STANDALONE_START_ADDR}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -e main")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -o <TARGET>")
set(CMAKE_C_LINK_EXECUTABLE "${TOOLCHAIN_PREFIX}ld.bfd ${CMAKE_EXE_LINKER_FLAGS} <OBJECTS>")

set(SOURCE_FILES
        show_mode.c
        tinyprintf.c)

add_executable(testing-standalone ${SOURCE_FILES})
