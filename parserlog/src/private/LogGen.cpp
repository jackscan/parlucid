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

