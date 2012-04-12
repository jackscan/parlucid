add_executable(test_lexer
	testalphabet.cpp
	testfsm.cpp
	testnfa.cpp
	testmain.cpp)

add_definitions(-DBOOST_TEST_DYN_LINK)

target_link_libraries(test_lexer lexer boost_unit_test_framework)

add_test(lexer test_lexer)
