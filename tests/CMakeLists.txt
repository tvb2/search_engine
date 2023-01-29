# 'Google_test' is the subproject name

project(Google_tests)

set(JSON_BuildTests OFF CACHE INTERNAL "")

include(FetchContent)
FetchContent_Declare(
        googletest
        URL
        https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

include_directories(
        ${gtest_SOURCE_DIR}/include
        ${gtest_SOURCE_DIR}
        ${INCLUDE_DIR}
)

set(gtest_disable_pthreads on)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
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

include(GoogleTest)
gtest_discover_tests(Google_Tests_run)

target_link_libraries(Google_Tests_run gtest gtest_main)
target_include_directories(Google_Tests_run PUBLIC ../nlohmann_json/include)