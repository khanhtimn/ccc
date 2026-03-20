catch_add_test(iterators iterators.cpp)

target_link_libraries(iterators PRIVATE ${Boost_SYSTEM_LIBRARY})
