add_executable(tests
    tests.cpp
)

include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(Catch2)

target_compile_options(cache PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(tests PRIVATE cache Catch2::Catch2WithMain)