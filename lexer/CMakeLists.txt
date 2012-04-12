
add_library(lexer
	src/private/alphabet.cpp
	src/private/fsm.cpp
	src/private/lexer.cpp
	src/private/nfa.cpp)

target_link_libraries(lexer utils)

add_subdirectory(test)

