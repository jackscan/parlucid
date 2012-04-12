add_executable(test_lalrparser
	testgram.cpp
	testlalrgram.cpp
	testmain.cpp)

add_definitions(-DBOOST_TEST_DYN_LINK)

target_link_libraries(test_lalrparser lalrparser boost_unit_test_framework)

add_test(lalrparser test_lalrparser)
