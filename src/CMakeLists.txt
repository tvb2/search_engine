cmake_minimum_required(VERSION 3.23)
project(search_engine)

# copy configuration and requests files into build directory
configure_file(../config.json config.json COPYONLY)
configure_file(../requests.json requests.json COPYONLY)
configure_file(../libstdc++-6.dll libstdc++-6.dll COPYONLY)
# creating database directory for files to be indexed
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/src/database)

# copying some files in the database directory to immediately provide some examples
configure_file(../database/file001.txt database/file001.txt COPYONLY)
configure_file(../database/file002.txt database/file002.txt COPYONLY)
configure_file(../database/file003.txt database/file003.txt COPYONLY)
configure_file(../database/file004.txt database/file004.txt COPYONLY)

set(CMAKE_CXX_STANDARD 17)
set(JSON_BuildTests OFF CACHE INTERNAL "")

add_executable(search_engine main.cpp
        ${INCLUDE_DIR}/converterjson.h converterjson.cpp
        ${INCLUDE_DIR}/invertedindex.h invertedindex.cpp
        ${INCLUDE_DIR}/entry.h
        ${INCLUDE_DIR}/searchserver.h searchserver.cpp
        ${INCLUDE_DIR}/relativeindex.h
    )

target_include_directories(search_engine PUBLIC ${INCLUDE_DIR})