add_executable(test_gramdefparser
	testgrammarparser.cpp
	testregexparser.cpp
	testmain.cpp)

add_definitions(-DBOOST_TEST_DYN_LINK)

target_link_libraries(test_gramdefparser gramdefparser parserlog boost_unit_test_framework)

add_test(gramdefparser test_gramdefparser)
