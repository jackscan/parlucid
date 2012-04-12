#ifndef LUAFSM_HPP
#define LUAFSM_HPP

#include "../../lexer/src/fsm.hpp"

#include <lua.hpp>
#include <cassert>

void pushLuaFsm(lua_State * lua, const Fsm<char>& fsm)
{
	const char* members[] = { "firstLinearState", "fsm" };
	const int nMembers = static_cast<int>(sizeof(members) / sizeof(members[0]));
	const char* transition[] = { "next", "tokens" };
	const int nTransEntries = static_cast<int>(sizeof(transition) / sizeof(transition[0]));
	lua_createtable(lua, fsm.getNumberOfStates(), nMembers);
	int top = lua_gettop(lua);
	for (Fsm<char>::State state = 0; state < fsm.getNumberOfStates(); ++state)
	{
		std::set<char> symbols;
		fsm.getAcceptedSymbols(state, symbols);
		// push as index into state table
		lua_pushinteger(lua, state + 1);
		// create transition table for state
		lua_createtable(lua, 0, static_cast<int>(symbols.size()));
		for (std::set<char>::const_iterator isym = symbols.begin(); isym != symbols.end(); ++isym)
		{
			lua_pushlstring(lua, &(*isym), 1);
			// create transition entry
			lua_createtable(lua, 0, nTransEntries);
			for (int t = 0; t < nTransEntries; ++t)
				lua_pushstring(lua, transition[t]);

			// create tokens list
			lua_newtable(lua);
			Fsm<char>::State next = state;
			BitSequence<int> tokens = fsm.next(next, *isym);
			for (int i = 0, iTok = tokens.begin(); iTok < tokens.end(); ++iTok)
				if (tokens.get(iTok))
				{
					// add token to tokens list
					lua_pushinteger(lua, ++i);
					lua_pushinteger(lua, iTok);
					lua_settable(lua, -3);
				}

			// assign tokens list to entry.tokens
			lua_settable(lua, -nTransEntries - 2);
			// assign next state to entry.next
			lua_pushinteger(lua, next + 1);
			lua_settable(lua, -nTransEntries - 1);
			// assign entry to transitiontable[*isym]
			lua_settable(lua, -3);
		}
		// assign transition table to state
		lua_settable(lua, -3);
	}

	for (int m = 0; m < nMembers; ++m)
		lua_pushstring(lua, members[m]);

	// assign fsm
	lua_pushlightuserdata(lua, const_cast<Fsm<char>*>(&fsm));
	lua_settable(lua, -2-nMembers);
	// assign firstLinearState
	lua_pushinteger(lua, fsm.getFirstLinearSequence(2));
	lua_settable(lua, -1-nMembers);

	assert(lua_gettop(lua) == top);
}


#endif
