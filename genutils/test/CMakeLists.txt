add_executable(test_genutils
	testcodestream.cpp)

add_definitions(-DBOOST_TEST_DYN_LINK)

target_link_libraries(test_genutils genutils boost_unit_test_framework)

add_test(genutils test_genutils)
