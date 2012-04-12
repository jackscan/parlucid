
add_executable(gramdefscanner
	src/private/alphabetgen.cpp
	src/private/cmdlineparserctrl.cpp
	src/private/fsmgen.cpp
	src/private/main.cpp
	src/private/parsercontext.cpp
	src/private/parsergen.cpp
	gen/private/cmdlineparser.cpp
	gen/private/cmdlinetokenizer.cpp)

include_directories(${LUA_INCLUDE_DIR})
include_directories(${Boost_INCLUDE_DIRS})

target_link_libraries(parserluagen
	luatemplateparser
	lexer
	lalrparser
	gramdefparser
	parserlog
	${Boost_FILESYSTEM_LIBRARY}
	${Boost_SYSTEM_LIBRARY}
	${LUA_LIBRARY})


