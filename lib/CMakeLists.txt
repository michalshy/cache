# lib/CMakeLists.txt
include_directories(
    public
)

add_library(cache
    cache.cpp
)

target_include_directories(cache PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})