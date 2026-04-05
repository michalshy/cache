# lib/CMakeLists.txt
include_directories(
    public
)

add_library(cache INTERFACE)

target_include_directories(cache INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})