
add_library(gramdefparser
	src/private/GrammarParserCtrl.cpp
	src/private/RegexParserCtrl.cpp
	gen/private/GrammarGrammar.cpp
	gen/private/GrammarParser.cpp
	gen/private/GrammarTokenizer.cpp
	gen/private/GrammarTokens.cpp
	gen/private/RegexGrammar.cpp
	gen/private/RegexParser.cpp
	gen/private/RegexTokenizer.cpp
	gen/private/RegexTokens.cpp)

target_link_libraries(gramdefparser lexer lalrparser)

add_subdirectory(test)

