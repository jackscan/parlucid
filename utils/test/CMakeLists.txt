add_executable(test_utils
	testmain.cpp
	testhelper.cpp
	testbitsequence.cpp)

add_definitions(-DBOOST_TEST_DYN_LINK)

target_link_libraries(test_utils utils boost_unit_test_framework)

add_test(utils test_utils)
