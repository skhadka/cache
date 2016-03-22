#project(cache_tests)

include_directories(
        "${gtest_SOURCE_DIR}/include"
        "${gtest_SOURCE_DIR}" )

add_executable(runBasicCacheTests
        basic_cache_test.cpp)

target_link_libraries(runBasicCacheTests gtest gtest_main)
#target_link_libraries(runBasicCacheTests CacheLib)
