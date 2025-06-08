# ================= CONFIGURING ===================================
cmake_minimum_required(VERSION 3.21)

project(QueueManager LANGUAGES CXX VERSION 0.0)

set(CMAKE_BUILD_DIR ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_BINARY_DIR ${CMAKE_BUILD_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BUILD_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BUILD_DIR}/lib)

configure_file(common/config.h.in ${CMAKE_SOURCE_DIR}/common/config.h @ONLY)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# ================= BUILDING ======================================

include_directories(common)
add_subdirectory(datalayer)
add_subdirectory(daemon)
add_subdirectory(setup)

# ================= LINKING ======================================

# add mependencies
