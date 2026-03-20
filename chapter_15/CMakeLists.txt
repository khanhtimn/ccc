catch_add_test(strings strings.cpp)
target_link_libraries(strings PRIVATE Boost::headers)

add_executable(c_str_example c_str_example.cpp)
