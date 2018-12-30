cmake_minimum_required(VERSION 3.13)
project(roi_handler)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(serial_handler)
include_directories(serial_handler/RS-232)

add_executable(roi_handler
        serial_handler/RS-232/rs232.c
        serial_handler/RS-232/rs232.h
        serial_handler/serial_handler.cpp
        serial_handler/serial_handler.h
        roi_opcodes.h
        roomba_core.cpp
        roomba_core.h)
