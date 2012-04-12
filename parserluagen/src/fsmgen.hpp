#ifndef FSMGEN_HPP
#define FSMGEN_HPP

#include "../../lexer/src/fsm.hpp"
#include "../../luatemplateparser/src/LuaGenFunctions.hpp"
#include "parsercontext.hpp"

#include <string>

void createLuaFsm(lua_State * lua, const Fsm<char>& fsm);
void createLuaContext(lua_State * lua, const ParserContext<int>& context);

#endif