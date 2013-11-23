cmake_minimum_required (VERSION 2.6)
set (CMAKE_BACKWARDS_COMPATIBILITY 2.6)

set(Proj_NAME AutoLogonTM)

#set(SRC_FILES ${FRWK_DIR}/npfrmwk.h ${FRWK_DIR}/npfrmwk_entry.cpp ${FRWK_DIR}/npfrmwkbase.h )
file(GLOB_RECURSE SUB_SRC_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    [^.]*.cpp
    [^.]*.h
    [^.]*.hpp
    [^.]*.def
    [^.]*.rc
)
list(APPEND SRC_FILES ${SUB_SRC_FILES})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/${Proj_NAME})
add_executable(${Proj_NAME} ${SRC_FILES})
