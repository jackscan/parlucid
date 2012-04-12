#ifndef PARSERGEN_HPP
#define PARSERGEN_HPP

//#include "lalrparsergen.hpp"
#include "parsercontext.hpp"
//#include "../../lalrparser/src/lalrgram.hpp"
#include "../../luatemplateparser/src/LuaGenFunctions.hpp"
#include "../../gramdefparser/src/GrammarParserCtrl.hpp"

//void createLuaParserGen(lua_State* lua, luagen::LuaOutput& luaOutput, LalrParserGen<int>& parserGen);
void createLuaParsingTable(lua_State* lua, const LalrParsingTable<int>& table, const ParserContext<int>& context, const LalrGrammar<int>& gram, const std::vector<GrammarParserCtrl::ActionList>& actionLists);

#endif
