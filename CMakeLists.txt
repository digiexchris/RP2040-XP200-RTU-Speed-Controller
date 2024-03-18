cmake_minimum_required(VERSION 3.13.1)
# set (BOARD_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")
# set (DTC_OVERLAY_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ELS.ovelay")
set(NO_BUILD_TYPE_WARNING TRUE)
set(BOARD rpi_pico)
find_package(zephyr)

# include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
project(LeadscrewWarrantyVoider LANGUAGES C CXX ASM)

FILE(GLOB_RECURSE app_sources 
#main app
./src/*.c*
#tests
./test/*.c*
./test/*.c*
)
include_directories(include)
target_sources(app PRIVATE ${app_sources})
add_subdirectory(lib/etl)
target_link_libraries(app PUBLIC etl)
