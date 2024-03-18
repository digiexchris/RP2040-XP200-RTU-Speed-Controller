cmake_minimum_required(VERSION 3.13.1)
# set (BOARD_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")
# set (DTC_OVERLAY_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ELS.ovelay")
set(NO_BUILD_TYPE_WARNING TRUE)
set(EXTRA_CONF_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../prj.conf")
add_compile_definitions(CONFIG_SPINDLE_ENCODER_TYPE_TEST)
find_package(zephyr)

# include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
project(LeadscrewWarrantyVoider LANGUAGES C CXX ASM)

FILE(GLOB app_sources ../src/Encoder/*.c* ../src/State/*.c* ./*/*.c*)
include_directories(../include)
target_sources(app PRIVATE ${app_sources})

set(ETL_BINARY_DIR ${CMAKE_BINARY_DIR}/etl_build)

add_subdirectory(../lib/etl ${ETL_BINARY_DIR})

target_link_libraries(app PUBLIC etl)
