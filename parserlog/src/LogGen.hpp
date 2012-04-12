#ifndef LOGGEN_HPP
#define LOGGEN_HPP

#include "ParserLog.hpp"

#include <lua.hpp>

void createLuaParserLog(lua_State * lua, const ParserLog& log);
void createLuaTokenNames(lua_State * lua, int numOfToks, const char* (*getTokenName)(int));


#endif
