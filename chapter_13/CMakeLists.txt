catch_add_test(associative_containers associative_containers.cpp)
catch_add_test(graph graph.cpp)
target_link_libraries(graph PRIVATE
    Boost::headers
    Boost::graph
)

catch_add_test(ptree ptree.cpp)
target_link_libraries(ptree PRIVATE Boost::headers)

catch_add_test(sequential_containers sequential_containers.cpp)
