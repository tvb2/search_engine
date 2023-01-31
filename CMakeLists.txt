cmake_minimum_required(VERSION 3.23)
project(search_engine)

# copy configuration and requests files into build directory
configure_file(config.json src/config.json COPYONLY)
configure_file(requests.json src/requests.json COPYONLY)
configure_file(libstdc++-6.dll src/libstdc++-6.dll COPYONLY)
# creating database directory for files to be indexed
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/src/database)

# copying some files in the database directory to immediately provide some examples
configure_file(database/file001.txt src/database/file001.txt COPYONLY)
configure_file(database/file002.txt src/database/file002.txt COPYONLY)
configure_file(database/file003.txt src/database/file003.txt COPYONLY)
configure_file(database/file004.txt src/database/file004.txt COPYONLY)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(json
        GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
        GIT_TAG v3.11.2
        )

FetchContent_MakeAvailable(json)

set(JSON_BuildTests OFF CACHE INTERNAL "")

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
# Don't use include(nlohmann_json/CMakeLists.txt) since that carries with it
# unintended consequences that will break the build.  It's generally
# discouraged (although not necessarily well documented as such) to use
# include(...) for pulling in other CMake projects anyways.

#add_subdirectory(nlohmann_json)
add_subdirectory(src)
add_subdirectory(tests)

target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)