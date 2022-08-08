cmake_minimum_required(VERSION 3.13.0)
project(test_main)
get_filename_component(SOFTWARE_DIR ../ ABSOLUTE)

# Download CppUTest
include(FetchContent)
FetchContent_Declare(
        CppUTest
        GIT_REPOSITORY https://github.com/cpputest/cpputest.git
        GIT_TAG latest-passing-build)
FetchContent_MakeAvailable(CppUTest)

# Build the binary with tests
add_executable(${PROJECT_NAME} )
target_sources(${PROJECT_NAME} PUBLIC
        main.cpp
        test_motors.cpp
        stubs/delay.c
        stubs/esp_idf/driver/mcpwm.cpp
        stubs/log.cx/src/log.c
        ${SOFTWARE_DIR}/main/src/motors_on_esp32.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC
        stubs
        stubs/esp_idf
        stubs/log.cx/src
        ${SOFTWARE_DIR}/main/include)
target_link_libraries(${PROJECT_NAME} PRIVATE CppUTest CppUTestExt)

# Cmake Testing
enable_testing()
add_test(test_motors ${PROJECT_NAME} -g Motors)