
add_library(parserlog
	src/private/ParserLog.cpp
	src/private/LogGen.cpp)

include_directories(${LUA_INCLUDE_DIR})

target_link_libraries(parserlog genutils)


