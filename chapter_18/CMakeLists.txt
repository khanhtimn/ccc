if(MSVC)
    catch_add_test(algorithms algorithms.cpp)
    target_link_libraries(algorithms PRIVATE Boost::headers)

    catch_add_test(numerics numerics.cpp)
endif()

add_executable(sample sample.cpp)
add_executable(shuffle shuffle.cpp)
add_executable(explore explore.cpp)
