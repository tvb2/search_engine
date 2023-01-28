# 'Google_test' is the subproject name

project(Google_tests)

set(JSON_BuildTests OFF CACHE INTERNAL "")

# 'lib' is the folder with Google Test sources
add_subdirectory(lib)

include_directories(
        ${gtest_SOURCE_DIR}/include
        ${gtest_SOURCE_DIR}
        ${INCLUDE_DIR}
)

set(gtest_disable_pthreads on)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
enable_testing()

# 'Google_Tests_run' is the target name
add_executable(Google_Tests_run
        ${INCLUDE_DIR}/converterjson.h ${SOURCE_DIR}/converterjson.cpp
        ${INCLUDE_DIR}/invertedindex.h ${SOURCE_DIR}/invertedindex.cpp
        ${INCLUDE_DIR}/entry.h
        ${INCLUDE_DIR}/searchserver.h ${SOURCE_DIR}/searchserver.cpp
        ${INCLUDE_DIR}/relativeindex.h
        invertedindextests.cpp searchservertests.cpp
        )

target_link_libraries(Google_Tests_run gtest gtest_main)
target_include_directories(Google_Tests_run PUBLIC ../nlohmann_json/include)