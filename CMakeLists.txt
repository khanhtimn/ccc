cmake_minimum_required(VERSION 3.20)

project(ccc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()
find_package(Threads REQUIRED)

include(FetchContent)

#################
# Boost Library #
#################
set(BOOST_ENABLE_COMPATIBILITY_TARGETS ON)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
	boost
	GIT_REPOSITORY https://github.com/boostorg/boost.git
	GIT_TAG        boost-1.90.0
)
FetchContent_MakeAvailable(boost)

macro(boost_add_executable TARGET)
	add_executable(${TARGET} ${ARGN})
	target_link_libraries(${TARGET} PRIVATE Boost::headers)
endmacro()

macro(boosttest_add_test TESTNAME)
	add_executable(${TESTNAME} ${ARGN})
	target_link_libraries(${TESTNAME} PRIVATE
		Boost::headers
		Boost::filesystem
		Boost::program_options
		Boost::regex
		Boost::date_time
		Boost::unit_test_framework
	)
	add_test(NAME ${TESTNAME} COMMAND ${TESTNAME})
	set_target_properties(${TESTNAME} PROPERTIES FOLDER tests)
endmacro()

#######################
# Google Test Library #
#######################
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
mark_as_advanced(
	BUILD_GMOCK BUILD_GTEST
	gmock_build_tests gtest_build_samples gtest_build_tests
	gtest_disable_pthreads gtest_force_shared_crt gtest_hide_internal_symbols
)

FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG        v1.17.0
)
FetchContent_MakeAvailable(googletest)

set_target_properties(gtest PROPERTIES FOLDER tests)
set_target_properties(gtest_main PROPERTIES FOLDER tests)
set_target_properties(gmock PROPERTIES FOLDER tests)
set_target_properties(gmock_main PROPERTIES FOLDER tests)

macro(googletest_add_test TESTNAME)
	add_executable(${TESTNAME} ${ARGN})
	target_link_libraries(${TESTNAME} PRIVATE gtest gmock gtest_main)
	add_test(NAME ${TESTNAME} COMMAND ${TESTNAME})
	set_target_properties(${TESTNAME} PROPERTIES FOLDER tests)
endmacro()

#################
# Catch Library #
#################
FetchContent_Declare(
	Catch2
	GIT_REPOSITORY https://github.com/catchorg/Catch2.git
	GIT_TAG        v3.13.0
)
FetchContent_MakeAvailable(Catch2)

macro(catch_add_test TESTNAME)
	add_executable(${TESTNAME} ${ARGN})
	target_link_libraries(${TESTNAME} PRIVATE Catch2::Catch2WithMain)
	add_test(NAME ${TESTNAME} COMMAND ${TESTNAME})
	set_target_properties(${TESTNAME} PROPERTIES FOLDER tests)
endmacro()

############
# Chapters #
############

add_subdirectory(preface)
add_subdirectory(chapter_1)
add_subdirectory(chapter_2)
add_subdirectory(chapter_3)
add_subdirectory(chapter_4)
add_subdirectory(chapter_5)
add_subdirectory(chapter_6)
add_subdirectory(chapter_7)
add_subdirectory(chapter_8)
add_subdirectory(chapter_9)
add_subdirectory(chapter_10)
add_subdirectory(chapter_11)
add_subdirectory(chapter_12)
add_subdirectory(chapter_13)
add_subdirectory(chapter_14)
add_subdirectory(chapter_15)
add_subdirectory(chapter_16)
add_subdirectory(chapter_17)
add_subdirectory(chapter_18)
add_subdirectory(chapter_19)
add_subdirectory(chapter_20)
add_subdirectory(chapter_21)
