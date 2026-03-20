catch_add_test(complex complex.cpp)
target_link_libraries(complex PRIVATE Boost::headers)

catch_add_test(data_structs data_structs.cpp)
target_link_libraries(data_structs PRIVATE Boost::headers)

catch_add_test(rand rand.cpp)

if(MSVC)
    catch_add_test(time time.cpp)
    target_link_libraries(time PRIVATE
        Boost::headers
        Boost::date_time
    )
endif()
