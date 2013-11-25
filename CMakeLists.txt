# Written to work with cmake 2.6
cmake_minimum_required (VERSION 2.6)
set (CMAKE_BACKWARDS_COMPATIBILITY 2.6)

project(AutoLogonTM)

add_definitions(-DUNICODE -D_UNICODE)

add_subdirectory(libLogonTM)
