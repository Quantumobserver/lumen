
cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(Lumen
        VERSION 0.1
        LANGUAGES C CXX)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
        message(FATAL_ERROR "In-source build is not allowed")
endif ()

# enable Link Time Optimization (LTO) if supported by compiler
include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)
if (supported)
        message(STATUS "IPO / LTO enabled")
        set(INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()

if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
        message("Use default toolchain config.")
        include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/SetupToolchain.cmake)
endif ()

#find_package(SFML 3
#             COMPONENTS 
#             system window graphics
#             network audio
#             REQUIRED)

add_subdirectory(Dependencies)

set(DEPENDENCIES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies)

add_subdirectory(src)
