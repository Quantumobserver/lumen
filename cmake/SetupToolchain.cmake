
if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
        if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
                include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Toolchain/Clang-Default.cmake)
        elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
                include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Toolchain/Gnu-Default.cmake)
        elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
                message("Intel compiler not supported")
        elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
                MESSAGE("MSVC not supported")
        endif ()
endif()
