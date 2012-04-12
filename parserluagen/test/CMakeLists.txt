add_executable(test_parserluagen
	testfsmgen.cpp
	testparsercontext.cpp
	testalphabetgen.cpp
	testlalrparsergen.cpp
	testparsergen.cpp)

add_definitions(-DBOOST_TEST_DYN_LINK)

target_link_libraries(test_parserluagen)

#add_test(gramdefparser test_gramdefparser)
