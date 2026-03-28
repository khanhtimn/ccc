catch_add_test(smart_ptrs smart_ptrs.cpp)
catch_add_test(exercise_11_1 exercise_11_1.cpp)
target_link_libraries(smart_ptrs PRIVATE Boost::headers)
