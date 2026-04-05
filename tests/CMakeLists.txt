file(GLOB_RECURSE TEST_SOURCES "*.cpp")

add_executable(tests ${TEST_SOURCES})

include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(Catch2)

target_compile_options(tests PRIVATE -fsanitize=thread)
target_link_options(tests PRIVATE -fsanitize=thread)

target_link_libraries(tests PRIVATE cache Catch2::Catch2WithMain)

include(CTest)
include(${Catch2_SOURCE_DIR}/extras/Catch.cmake)
catch_discover_tests(tests
    DISCOVERY_MODE PRE_TEST
)