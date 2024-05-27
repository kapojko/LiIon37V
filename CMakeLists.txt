cmake_minimum_required(VERSION 3.12)
project(LiIon37V_Test C)

# This CMakeLists.txt is for testing purposes only.
add_compile_definitions(TESTING)

# Collect all source files
set(SOURCES
    LiIon37V.c)
add_executable(LiIon37V_Test ${SOURCES} LiIon37V_Test.c)

# Platform-specific handling for linking against the math library
if (WIN32)
    # Windows: No need to link against "m" library
    # You can safely remove this line
    # target_link_libraries(LiIon37V_Test m)
else ()
    # Linux: Link against the math library ("-lm")
    target_link_libraries(LiIon37V_Test m)
endif ()

# Setup testing
enable_testing()
include(CTest)

add_test(
    NAME LiIon37V_UnitTest
    COMMAND LiIon37V_Test
)
