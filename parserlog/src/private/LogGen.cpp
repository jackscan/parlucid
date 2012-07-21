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

#include "../LogGen.hpp"

static void createLuaLogNode(lua_State *lua, const ParserLog::Node& node)
{
	lua_pushinteger(lua, (lua_Integer)node.getType());
	lua_setfield(lua, -2, "type");

	const std::string& token = node.getToken();

	if (!token.empty())
	{
		lua_pushlstring(lua, token.c_str(), token.length());
		lua_setfield(lua, -2, "token");
	}

	const std::vector<ParserLog::Node*>& childs = node.getChilds();
	for (size_t i = 0; i < childs.size(); ++i)
		if (childs[i])
		{
			if (lua_checkstack(lua, 4))
			{
				lua_pushinteger(lua, (lua_Integer)(i+1));
				lua_newtable(lua);			
				createLuaLogNode(lua, *(childs[i]));
				lua_settable(lua, -3);
			}
			else
			{
				fprintf(stderr, "lua stack overflow\n");
			}
		}
}

void createLuaParserLog(lua_State * lua, const ParserLog& log)
{
	lua_newtable(lua);
	if (log.root())
		createLuaLogNode(lua, *log.root());
}

void createLuaTokenNames(lua_State * lua, int numOfToks, const char* (*getTokenName)(int))
{
	lua_newtable(lua);
	for (int tok = 0; tok < numOfToks; ++tok)
	{
		lua_pushinteger(lua, (lua_Integer)tok);
		lua_pushstring(lua, getTokenName(tok));
		lua_settable(lua, -3);
	}
}

