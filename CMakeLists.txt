cmake_minimum_required (VERSION 3.4)
project(Ducksim)

SET(GCC_DEBUG_COMPILE_FLAGS "-Werror")
SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_DEBUG_COMPILE_FLAGS}" )

# All subprojects will rely on the cache library include directory
include_directories("${PROJECT_SOURCE_DIR}/cache/include")

add_subdirectory(cache)
add_subdirectory(app)
#add_subdirectory(tests)
