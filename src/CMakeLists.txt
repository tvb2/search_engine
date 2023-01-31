cmake_minimum_required(VERSION 3.23)
project(search_engine)

set(CMAKE_CXX_STANDARD 20)
set(JSON_BuildTests OFF CACHE INTERNAL "")

add_executable(search_engine main.cpp
        ${INCLUDE_DIR}/converterjson.h converterjson.cpp
        ${INCLUDE_DIR}/invertedindex.h invertedindex.cpp
        ${INCLUDE_DIR}/entry.h
        ${INCLUDE_DIR}/searchserver.h searchserver.cpp
        ${INCLUDE_DIR}/relativeindex.h
    )

target_include_directories(search_engine PUBLIC ${INCLUDE_DIR})