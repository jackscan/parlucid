
add_library(luatemplateparser
	src/private/LuaGenFunctions.cpp
	src/private/LuaHelper.cpp
	src/private/LuaTemplateParserCtrl.cpp
	gen/private/LuaTemplateGrammar.cpp
	gen/private/LuaTemplateParser.cpp
	gen/private/LuaTemplateTokenizer.cpp
	gen/private/LuaTemplateTokens.cpp)

include_directories(${LUA_INCLUDE_DIR})
include_directories(${Boost_INCLUDE_DIRS})

target_link_libraries(luatemplateparser genutils ${Boost_system_LIBRARY} ${Boost_filesystem_LIBRARY} ${LUA_LIBRARY})


