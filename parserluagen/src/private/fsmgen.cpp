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

#include "../fsmgen.hpp"
#include "../alphabetgen.hpp"

#include "../../../genutils/src/codestream.hpp"
#include "../../../luatemplateparser/src/LuaGenFunctions.hpp"

#include <iostream>
#include <sstream>
#include <cctype>

typedef int Token;

/*static const Fsm<char>* lua_Fsm_getThis(lua_State *L)
{
	lua_getfield(L, 1, "self");
	return reinterpret_cast<const Fsm<char>*>(lua_touserdata(L, -1));
}*/
/*
static int lua_Fsm_getNumberOfStates(lua_State *L)
{
	const Fsm<char>* fsm = getThis(L);
	if (fsm)
	{
		lua_pushnumber(L, (lua_Number)fsm->getNumberOfStates());
		return 1;
	}
	return 0;
}
*/

/*static int lua_Fsm_getTransitions(lua_State *L)
{
	const Fsm<char>* fsm = lua_Fsm_getThis(L);
	if (fsm && lua_gettop(L) == 3)
	{
		int index = lua_tointeger(L, 2);

		lua_rawgeti(L, 1, index);
		if (lua_isnil(L, -1))
		{
			Fsm<Char>::State state = static_cast<Fsm<Char>::State>(index);

			std::set<Char> symbols;
			fsm->getAcceptedSymbols(state, symbols);

			//std::map<Fsm<Char>::State,BitSequence<Fsm<Char>::Token> > outputs;
			//fsm->getPotentialOutputs(outputs);

			//fsm->

			Alphabet<Char> accepted;
			// maps from (nextstate,output) to set of corresponding input
			//std::map<std::pair<Fsm<Char>::State, std::pair<Token,bool> >, std::set<Char> > mergeSets;
			std::map<std::pair<Fsm<Char>::State, std::pair<std::string,bool> >, std::set<Char> > mergeSets;

			for (std::set<Char>::const_iterator isym = symbols.begin(); isym != symbols.end(); ++isym)
			{
				Fsm<Char>::State nextState = state;
				const BitSequence<Token>& output = fsm->next(nextState, *isym);
				//std::pair<Token,bool> tok(0, false);
				bool nonEmpty = ! output.empty();

				std::string bitstr = output.toString(4);

				//mergeSets[std::make_pair(nextState, tok)].insert(*isym);
				mergeSets[std::make_pair(nextState, std::make_pair(bitstr,nonEmpty))].insert(*isym);
				accepted.add(fsm->input().lower_bound(*isym), fsm->input().upper_bound(*isym));
			}

			AlphabetGen<Char>::Results results;

			//for (std::map<std::pair<Fsm<Char>::State, std::pair<Token,bool> >, std::set<Char> >::const_iterator imerge = mergeSets.begin(); imerge != mergeSets.end(); ++imerge)
			for (std::map<std::pair<Fsm<Char>::State, std::pair<std::string,bool> >, std::set<Char> >::const_iterator imerge = mergeSets.begin(); imerge != mergeSets.end(); ++imerge)
			{
				//std::stringstream tokstr;
				std::stringstream nextstr;
				//tokstr << imerge->first.second.first;
				nextstr << imerge->first.first;
				accepted.merge(imerge->second);

				AlphabetGen<Char>::Result result;
				if (imerge->first.second.second)
					result["tokens"] = imerge->first.second.first;
				result["next"] = nextstr.str();

				for (std::set<Char>::const_iterator isym = imerge->second.begin(); isym != imerge->second.end(); ++isym)
					results[accepted.lower_bound(*isym)] = result;
			}


			AlphabetGen<Char> alphabetGen(L, accepted, results);
			lua_pushinteger(L, index);
			lua_pushvalue(L, -2);
			lua_settable(L, 1);
		}
		return 1;
	}
	return 0;
}*/

/*static void createPotentialOutputs(lua_State * lua, const Fsm<char>& fsm)
{
    lua_newtable(lua);
    std::map<Fsm<Char>::State,BitSequence<Token> > outputs;
    fsm.getPotentialOutputs(outputs);

    std::string bitstr;
    
    for (std::map<Fsm<Char>::State,BitSequence<Token> >::const_iterator iter = outputs.begin(); iter != outputs.end(); ++iter)
    {
        bitstr = iter->second.toString(4);
        lua_pushinteger(lua, (lua_Integer)iter->first);
        lua_pushlstring(lua, bitstr.c_str(), bitstr.length());
        lua_settable(lua, -3);
    }
}*/

static void insertLuaTransitionEntry(lua_State* lua, Char symbol, Fsm<Char>::State nextState, const BitSequence<Token>& output)
{
    lua_pushinteger(lua, (lua_Integer)lua_objlen(lua, -1) + 1);
    lua_newtable(lua);

    lua_pushinteger(lua, (lua_Integer)symbol);
    lua_setfield(lua, -2, "char");

    lua_pushinteger(lua, (lua_Integer)nextState);
    lua_setfield(lua, -2, "next");
    if (!output.empty())
    {
        std::string bitstr = output.toString(4);
        lua_pushlstring(lua, bitstr.c_str(), bitstr.length());
        lua_setfield(lua, -2, "tokens");
    }

    lua_settable(lua, -3);
}

static void createTransitions(lua_State* lua, const Fsm<Char>& fsm)
{
    std::map<Fsm<Char>::State,BitSequence<Token> > potentialOutputs;
    fsm.getPotentialOutputs(potentialOutputs);

    for (Fsm<Char>::State state = 0; state < fsm.getNumberOfStates(); ++state)
    {
        lua_pushinteger(lua, (lua_Integer)state);
        lua_newtable(lua);

        if (! potentialOutputs[state].empty())
        {
            std::string bitstr = potentialOutputs[state].toString(4);
            lua_pushlstring(lua, bitstr.c_str(), bitstr.length());
            lua_setfield(lua, -2, "potential");
        }

        std::set<Char> symbols;
		fsm.getAcceptedSymbols(state, symbols);

		Alphabet<Char> accepted;
		// maps from (nextstate,output) to set of corresponding input
		std::map<std::pair<Fsm<Char>::State, std::pair<std::string,bool> >, std::set<Char> > mergeSets;

		for (std::set<Char>::const_iterator isym = symbols.begin(); isym != symbols.end(); ++isym)
		{
			Fsm<Char>::State nextState = state;
			const BitSequence<Token>& output = fsm.next(nextState, *isym);
			bool nonEmpty = ! output.empty();
			std::string bitstr = output.toString(4);
			mergeSets[std::make_pair(nextState, std::make_pair(bitstr,nonEmpty))].insert(*isym);
			accepted.add(fsm.input().lower_bound(*isym), fsm.input().upper_bound(*isym));
		}

		for (std::map<std::pair<Fsm<Char>::State, std::pair<std::string,bool> >, std::set<Char> >::const_iterator imerge = mergeSets.begin(); imerge != mergeSets.end(); ++imerge)
		{
            accepted.merge(imerge->second);
			/*std::stringstream nextstr;
			nextstr << imerge->first.first;
			accepted.merge(imerge->second);

			AlphabetGen<Char>::Result result;
			if (imerge->first.second.second)
				result["tokens"] = imerge->first.second.first;
			result["next"] = nextstr.str();

			for (std::set<Char>::const_iterator isym = imerge->second.begin(); isym != imerge->second.end(); ++isym)
				results[accepted.lower_bound(*isym)] = result;*/
		}

        std::set<Char> lower_bounds = accepted.lower_bound();

        for (std::set<Char>::const_iterator iChar = lower_bounds.begin(); iChar != lower_bounds.end();)
        {
            Char lower = *iChar;
            Char upper = accepted.upper_bound(lower);

            Fsm<Char>::State nextState = state;
            const BitSequence<Token>& output = fsm.next(nextState, lower);

            insertLuaTransitionEntry(lua, lower, nextState, output);
            
            ++iChar;
            if (upper < std::numeric_limits<Char>::max() && (iChar == lower_bounds.end() || upper + 1 < *iChar))
            {
                char next = upper + 1;
                insertLuaTransitionEntry(lua, next, Fsm<Char>::ERROR_STATE, BitSequence<Token>());
            }
        }

        lua_settable(lua, -3);
    }
}

void createLuaFsm(lua_State * lua, const Fsm<Char>& fsm)
{
	lua_newtable(lua);

    createTransitions(lua, fsm);
	//lua_pushlightuserdata(lua, const_cast<Fsm<char>*>(&fsm));
	//lua_setfield(lua, -2, "self");
	lua_pushinteger(lua, fsm.getFirstLinearSequence(2));
	lua_setfield(lua, -2, "firstLinearState");
	lua_pushinteger(lua, fsm.getNumberOfStates());
	lua_setfield(lua, -2, "nrOfStates");
    
    //createPotentialOutputs(lua, fsm);
    //lua_setfield(lua, -2, "potentialOutputs");

	// metatable
	//lua_newtable(lua);

	// []-operator

	//lua_pushstring(lua, "__index");
	//lua_pushcfunction(lua, &lua_Fsm_getTransitions);
	//lua_settable(lua, -3);

	//lua_setmetatable(lua, -2);
}

void createLuaContext(lua_State * lua, const ParserContext<Token>& context)
{
	lua_newtable(lua);

	for (size_t c = 0; c < context.getNumberOfContexts(); ++c)
	{
		std::string bitstr = context.getAcceptedTokens(c).toString(4);
		lua_pushinteger(lua, static_cast<int>(c) + 1);
		lua_pushlstring(lua, bitstr.c_str(), bitstr.length());
		lua_settable(lua, -3);
	}

	//lua_pushlightuserdata(lua, const_cast<ParserContext<Token>*>(&context));
	//lua_setfield(lua, -2, "self");

	// metatable
	//lua_newtable(lua);

	// #-operator
	/* does not work in table
	lua_pushstring(lua, "__len");
	lua_pushcfunction(lua, &lua_Fsm_getNumberOfStates);
	lua_settable(lua, -3);*/

	// []-operator

	//lua_pushstring(lua, "__index");
	//lua_pushcfunction(lua, &lua_Fsm_getTransitions);
	//lua_settable(lua, -3);

	//lua_setmetatable(lua, -2);
}

