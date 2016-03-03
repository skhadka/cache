cmake_minimum_required (VERSION 3.3)
project (Main)
add_executable(Main main.cpp cache.cpp hash_functions.cpp replacement_policies.cpp)

SET(GCC_DEBUG_COMPILE_FLAGS "-Werror")
SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_DEBUG_COMPILE_FLAGS}" )

