#project(tests)

# Have to disable threads to avoid errors such as the following: ``error: 'AutoHandle' does not name a type''
set (gtest_disable_pthreads on)

add_subdirectory(cache_tests)
add_subdirectory(lib/googletest)
