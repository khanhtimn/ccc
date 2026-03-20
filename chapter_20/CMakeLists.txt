catch_add_test(buffers buffers.cpp)
target_link_libraries(buffers PRIVATE
    Boost::headers
    Boost::system
    Threads::Threads
)

boost_add_executable(asio asio.cpp)
target_link_libraries(asio PRIVATE
    Boost::system
    Threads::Threads
)

boost_add_executable(name_resolution name_resolution.cpp)
target_link_libraries(name_resolution PRIVATE
    Boost::system
    Threads::Threads
)

boost_add_executable(name_resolution_async name_resolution_async.cpp)
target_link_libraries(name_resolution_async PRIVATE
    Boost::system
    Threads::Threads
)

boost_add_executable(connect connect.cpp)
target_link_libraries(connect PRIVATE
    Boost::system
    Threads::Threads
)

boost_add_executable(connect_async connect_async.cpp)
target_link_libraries(connect_async PRIVATE
    Boost::system
    Threads::Threads
)

boost_add_executable(web_request web_request.cpp)
target_link_libraries(web_request PRIVATE
    Boost::system
    Threads::Threads
)

boost_add_executable(web_request_async web_request_async.cpp)
target_link_libraries(web_request_async PRIVATE
    Boost::system
    Threads::Threads
)

boost_add_executable(web_request_async_multi web_request_async_multi.cpp)
target_link_libraries(web_request_async_multi PRIVATE
    Boost::system
    Threads::Threads
)

boost_add_executable(upper_echo_server upper_echo_server.cpp)
target_link_libraries(upper_echo_server PRIVATE
    Boost::system
    Threads::Threads
)

boost_add_executable(upper_echo_server_async upper_echo_server_async.cpp)
target_link_libraries(upper_echo_server_async PRIVATE
    Boost::system
    Threads::Threads
)

boost_add_executable(upper_echo_server_multi upper_echo_server_multi.cpp)
target_link_libraries(upper_echo_server_multi PRIVATE
    Boost::system
    Threads::Threads
)
