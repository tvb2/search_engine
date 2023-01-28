cmake_minimum_required(VERSION 3.23)
project(search_engine)

set(CMAKE_CXX_STANDARD 17)
set(JSON_BuildTests OFF CACHE INTERNAL "")

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
# Don't use include(nlohmann_json/CMakeLists.txt) since that carries with it
# unintended consequences that will break the build.  It's generally
# discouraged (although not necessarily well documented as such) to use
# include(...) for pulling in other CMake projects anyways.

add_subdirectory(nlohmann_json)
add_subdirectory(src)
add_subdirectory(tests)

target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)