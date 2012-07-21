/*
 * Copyright 2004-2012 Mathias Fiedler. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "../parsergen.hpp"

#include <lua.hpp>
#include <string>
#include <cassert>

typedef int Token;

typedef GrammarParserCtrl::Action Action;
typedef GrammarParserCtrl::ActionList ActionList;
typedef LalrParsingTable<Token> Table;
typedef LalrGrammar<Token> Gram;
typedef Gram::Production Production;
typedef std::set<Token> Set;

static void createLuaTokenString(lua_State* lua, std::basic_string<Token> str)
{
	lua_newtable(lua);
	for (size_t t = 0; t < str.size(); ++t)
	{
		lua_pushinteger(lua, (lua_Integer)(t + 1));
		lua_pushinteger(lua, (lua_Integer)str[t]);
		lua_settable(lua, -3);
	}
}

static void createLuaInformation(lua_State* lua, const Gram& gram, size_t state)
{
	lua_newtable(lua);
	const Gram::Information& info = gram.getInformation(state);
	if (info.isIgnorable())
	{
		lua_pushboolean(lua, 1);
		lua_setfield(lua, -2, "ignorable");
	}
	lua_Integer nItem = 1;
	for (Gram::InfoItems::const_iterator iItem = info.items.begin(); iItem != info.items.end(); ++iItem, ++nItem)
	{
		const Gram::ItemCore& itemCore = iItem->first;
		lua_pushinteger(lua, nItem);
		lua_newtable(lua);
		lua_pushinteger(lua, (lua_Integer)(itemCore.productionNo));
		lua_setfield(lua, -2, "production");
		lua_pushinteger(lua, (lua_Integer)itemCore.dot);
		lua_setfield(lua, -2, "dot");
		if (itemCore.ignore)
		{
			lua_pushboolean(lua, 1);
			lua_setfield(lua, -2, "ignore");
		}
		lua_newtable(lua);
		lua_Integer nFollow = 1;
		for (Set::const_iterator iFollow = iItem->second.begin(); iFollow != iItem->second.end(); ++iFollow, ++nFollow)
		{
			lua_pushinteger(lua, nFollow);
			lua_pushinteger(lua, (lua_Integer)(*iFollow));
			lua_settable(lua, -3);
		}
		lua_setfield(lua, -2, "follows");

		lua_settable(lua, -3);
	}
}

static void createLuaActionList(lua_State* lua, const ActionList& actionList)
{
	lua_newtable(lua);
	for (size_t a = 0; a < actionList.size(); ++a)
	{
		lua_pushinteger(lua, (lua_Integer)(a + 1));
		const Action& action = actionList[a];
		lua_newtable(lua);
		if (! action.lparameter.empty())
		{
			lua_pushinteger(lua, 0);
			createLuaTokenString(lua, action.lparameter);
			lua_settable(lua, -3);
		}
		if (! action.function.empty())
		{
			lua_pushstring(lua, action.function.c_str());
			lua_setfield(lua, -2, "function");
		}
		for (size_t p = 0; p < action.parameter.size(); ++p)
		{
			lua_pushinteger(lua, (lua_Integer)(p + 1));
			createLuaTokenString(lua, action.parameter[p]);
			lua_settable(lua, -3);
		}
		lua_settable(lua, -3);
	}
}

static void createLuaProductions(lua_State* lua, const Gram& gram, const std::vector<ActionList>& actionLists)
{
	lua_newtable(lua);
	for (size_t p = 0; p < gram.getProductions().size(); ++p)
	{
		const Production& production = gram.getProductions()[p];
		lua_pushinteger(lua, (lua_Integer)p);
		lua_newtable(lua);
		lua_pushinteger(lua, (lua_Integer)production.head());
		lua_setfield(lua, -2, "head");
		createLuaTokenString(lua, production.body());
		lua_setfield(lua, -2, "body");

		if (p < actionLists.size())
		{
			createLuaActionList(lua, actionLists[p]);
			lua_setfield(lua, -2, "actions");
		}

		lua_settable(lua, -3);
	}
}

template<typename T>
static
void createLuaActionTable(lua_State* lua, const typename std::map<T, Set>& actions)
{
	lua_newtable(lua);
	for (typename std::map<T, Set>::const_iterator iAction = actions.begin(); iAction != actions.end(); ++iAction)
	{
		lua_pushinteger(lua, (lua_Integer)iAction->first);
		lua_newtable(lua);
		lua_Integer nToken = 1;
		for (Set::const_iterator iToken = iAction->second.begin(); iToken != iAction->second.end(); ++iToken, ++nToken)
		{
			lua_pushinteger(lua, nToken);
			lua_pushinteger(lua, (lua_Integer)*iToken);
			lua_settable(lua, -3);
		}
		lua_settable(lua, -3);
	}
}

static void createLuaReduceTable(lua_State* lua, const std::map<Table::ProductionNo, Set>& actions, const std::vector<ActionList>& actionLists)
{
	lua_newtable(lua);
	for (std::map<Table::ProductionNo, Set>::const_iterator iAction = actions.begin(); iAction != actions.end(); ++iAction)
	{
		lua_pushinteger(lua, (lua_Integer)iAction->first);
		lua_newtable(lua);
		lua_Integer nToken = 1;
		for (Set::const_iterator iToken = iAction->second.begin(); iToken != iAction->second.end(); ++iToken, ++nToken)
		{
			lua_pushinteger(lua, nToken);
			lua_pushinteger(lua, (lua_Integer)*iToken);
			lua_settable(lua, -3);
		}

		if (iAction->first < actionLists.size() && !actionLists[iAction->first].empty())
		{
			createLuaActionList(lua, actionLists[iAction->first]);
			lua_setfield(lua, -2, "actions");
		}

		lua_settable(lua, -3);
	}
}

//typedef std::map<Token, std::set<std::basic_string<Token> > > tCapturedTokens;
typedef std::set<std::basic_string<Token> > tCapturedTokens;

static void addCapturedParameter(tCapturedTokens& capturedTokens, const std::basic_string<Token>& param)
{
	if (!param.empty())
	{
		for (size_t t = 0; t < param.length(); ++t)
		{
			//capturedTokens[param[t]].insert(param.substr(t+1));
			capturedTokens.insert(param.substr(t));
			//nonTerminalTokens.insert(param[t]);
			//dependencies[param[t]].insert(param[t+1]);
		}
		//capturedTokens[]*param.rbegin()].insert(std::basic_string<Token>());
		//terminalTokens.insert(*param.rbegin());
	}
}

/*static void createLuaCapturedParameter(lua_State* lua, Token token, const std::set<std::basic_string<Token> >& subtree)
{
	int top = lua_gettop(lua);

	lua_Integer index = static_cast<lua_Integer>(token);
	lua_pushinteger(lua, index);
	lua_newtable(lua);

	lua_Integer n = 0;
	for(std::set<std::basic_string<Token> >::const_iterator iStr = subtree.begin(); iStr != subtree.end(); ++iStr)
		if (!iStr->empty())
		{
			lua_pushinteger(lua, ++n);
			createLuaTokenString(lua, *iStr);
			lua_settable(lua, -3);
		}

	lua_settable(lua, -3);

	assert(top == lua_gettop(lua));
}*/

static void createLuaBranch(lua_State* lua, const std::basic_string<Token>& branch)
{
	if (!branch.empty())
	{
		lua_Integer tok = (lua_Integer)*branch.begin();
		lua_pushinteger(lua, tok);
		lua_gettable(lua, -2);
		if (lua_isnil(lua, -1))
		{
			lua_pop(lua, 1);
			lua_newtable(lua);
			lua_pushinteger(lua, tok);
			lua_pushvalue(lua, -2);
			lua_settable(lua, -4);
		}
		createLuaBranch(lua, branch.substr(1));
		lua_pop(lua, 1);
	}
}

static void createLuaCaptures(lua_State* lua, const std::vector<ActionList>& actionLists)
{
	int top = lua_gettop(lua);
	tCapturedTokens capturedTokens;

	for (size_t prod = 0; prod < actionLists.size(); ++prod)
	{
		const ActionList& actionList = actionLists[prod];
		for (size_t a = 0; a < actionList.size(); ++a)
		{
			const Action& action = actionList[a];
			//addCapturedParameter(capturedTokens, action.lparameter);
			//capturedTokens.insert(action.lparameter);
			addCapturedParameter(capturedTokens, action.lparameter);

			for (size_t p = 0; p < action.parameter.size(); ++p)
				//capturedTokens.insert(action.parameter[p]);
				addCapturedParameter(capturedTokens, action.parameter[p]);
		}
	}

	lua_newtable(lua);
	/*for (tCapturedTokens::const_iterator iCap = capturedTokens.begin(); iCap != capturedTokens.end(); ++iCap)
	{
		lua_pushinteger(lua, (lua_Integer)iCap->first);
		lua_newtable(lua);
		//lua_pushstring(lua, "bla");
		lua_settable(lua, -3);
	}*/

	for (tCapturedTokens::const_iterator iCap = capturedTokens.begin(); iCap != capturedTokens.end(); ++iCap)
		createLuaBranch(lua, *iCap);
		/*if (!iCap->second.empty())
		{
			lua_Integer cap = (lua_Integer)iCap->first;
			lua_pushinteger(lua, cap);
			lua_gettable(lua, -2);
			assert(lua_istable(lua, -1));
			for (std::set<Token>::const_iterator iDep = iCap->second.begin(); iDep != iCap->second.end(); ++iDep)
			{
				lua_Integer dep = (lua_Integer)*iDep;
				lua_pushinteger(lua, dep);
				lua_pushvalue(lua, -1);
				lua_gettable(lua, -4);
				assert(lua_istable(lua, -1));
				lua_settable(lua, -3);
			}
			lua_pop(lua, 1);
		}*/

	//createLuaCapturedParameter(lua, iCap->first, iCap->second);

	assert(top + 1 == lua_gettop(lua));
}

void createLuaNonTerminals(lua_State* lua, const LalrGrammar<int>& gram)
{
	lua_newtable(lua);
	for (size_t prodNo = 0; prodNo < gram.getProductions().size(); ++prodNo)
	{
		lua_pushinteger(lua, (lua_Integer)gram.getProductions()[prodNo].head());
		lua_pushboolean(lua, 1);
		lua_settable(lua, -3);
	}
}

void createLuaParsingTable(lua_State* lua, const Table& table, const ParserContext<Token>& context, const LalrGrammar<int>& gram, const std::vector<ActionList>& actionLists)
{
	lua_newtable(lua);

	createLuaNonTerminals(lua, gram);
	lua_setfield(lua, -2, "nonterminals");

	createLuaProductions(lua, gram, actionLists);
	lua_setfield(lua, -2, "productions");

	createLuaCaptures(lua, actionLists);
	lua_setfield(lua, -2, "captures");

	for (size_t r = 0; r < table.size(); ++r)
	{
		lua_pushinteger(lua, static_cast<int>(r));
		lua_newtable(lua);

		std::map<Token, size_t> shifts;
		std::map<Table::ProductionNo, Set> reduces;
		std::map<Table::ProductionNo, Set> ambiguous;

		// sort actions into maps 'shifts', 'reduces' and 'ambiguous'
		const Table::ActionMap& actions = table[r];
		for (Table::ActionMap::const_iterator iAction = actions.begin(); iAction != actions.end(); ++iAction)
		{
			switch(iAction->second.type)
			{
			case Table::SHIFT:
				shifts[iAction->first] = iAction->second.nextState;
				//shifts[iAction->second.nextState].insert(iAction->first);
				break;
			case Table::REDUCE:
				reduces[iAction->second.productionNo].insert(iAction->first);
				break;
			default:
				ambiguous[iAction->second.productionNo].insert(iAction->first);
				break;
			}
		}

		if (!shifts.empty())
		{
			lua_newtable(lua);
			for (std::map<Token, size_t>::const_iterator iShift = shifts.begin(); iShift != shifts.end(); ++iShift)
			{
				lua_pushinteger(lua, (lua_Integer)iShift->first);
				lua_pushinteger(lua, (lua_Integer)iShift->second);
				lua_settable(lua, -3);
			}
			lua_setfield(lua, -2, "shifts");
		}

		if (!reduces.empty())
		{
			createLuaReduceTable(lua, reduces, actionLists);
			lua_setfield(lua, -2, "reduces");
		}

		if (!ambiguous.empty())
		{
			createLuaActionTable<Table::ProductionNo>(lua, ambiguous);
			lua_setfield(lua, -2, "ambiguous");
		}

		createLuaInformation(lua, gram, r);
		lua_setfield(lua, -2, "info");

		lua_pushinteger(lua, (lua_Integer)context.getParsingTableContext(r));
		lua_setfield(lua, -2, "context");

		lua_settable(lua, -3);
	}
}
/*
static luagen::LuaOutput& getLuaOutput(lua_State* L)
{
	luagen::LuaOutput& output = *reinterpret_cast<luagen::LuaOutput*>(lua_touserdata(L, lua_upvalueindex(1)));
	return output;
}

static LalrParserGen<Token>& getGenerator(lua_State* L)
{
	LalrParserGen<Token>& generator = *reinterpret_cast<LalrParserGen<Token>*>(lua_touserdata(L, 1));
	return generator;
}

static int generateParserClassDef(lua_State* lua)
{
	CodeStream *cs = getLuaOutput(lua).current;
	if (cs && lua_gettop(lua) == 1)
	{
		LalrParserGen<Token>& generator = getGenerator(lua);
		generator.generateClassDef(*cs);
	}
	return 0;
}

static int generateParserStateHandlerImpls(lua_State* lua)
{
	CodeStream *cs = getLuaOutput(lua).current;
	if (cs && lua_gettop(lua) == 1)
	{
		LalrParserGen<Token>& generator = getGenerator(lua);
		generator.generateStateHandlerImpls(*cs);
	}
	return 0;
}

static int setParserNames(lua_State* lua)
{
	if (lua_gettop(lua) == 3)
	{
		LalrParserGen<Token>& generator = getGenerator(lua);
		generator.setClassName(lua_tostring(lua, 2));
		generator.setMediatorClassName(lua_tostring(lua, 3));
	}
	return 0;
}

static int generatorIndex(lua_State* lua)
{
	if (lua_gettop(lua) == 2)
	{
		const char* function = lua_tostring(lua, 2);
		if (0 == strcmp(function, "classDef"))
		{
			lua_pushlightuserdata(lua, &getLuaOutput(lua));
			lua_pushcclosure (lua, &generateParserClassDef, 1);
			return 1;
		}
		else if(0 == strcmp(function, "stateHandler"))
		{
			lua_pushlightuserdata(lua, &getLuaOutput(lua));
			lua_pushcclosure (lua, &generateParserStateHandlerImpls, 1);
			return 1;
		}
		else if(0 == strcmp(function, "setNames"))
		{
			lua_pushcclosure (lua, &setParserNames, 0);
			return 1;
		}
		else
		{
			std::string errorMsg(function);
			errorMsg += " is not a member of parser generator";
			lua_pushstring(lua, errorMsg.c_str());
			lua_error(lua);
		}
	}
	return 0;
}

void createLuaParserGen(lua_State* lua, luagen::LuaOutput& luaOutput, LalrParserGen<int>& parserGen)
{
	lua_pushlightuserdata(lua, &parserGen);

	// metatable
	lua_newtable(lua);

	lua_pushstring(lua, "__index");
	lua_pushlightuserdata(lua, &luaOutput);
	lua_pushcclosure (lua, &generatorIndex, 1);
	lua_settable(lua, -3);

	lua_setmetatable(lua, -2);
}*/

