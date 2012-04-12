#ifndef LUAHELPER_HPP
#define LUAHELPER_HPP

#include <lua.hpp>

namespace luautils
{
	template<typename T>
	void push(lua_State* L, const T& val)
	{
		lua_pushnumber(L, (lua_Number)val);
	}

	template<>
	void push<std::string>(lua_State* L, const std::string& val)
	{
		lua_pushlstring(L, val.c_str(), val.length());
	}

	template<class Cont>
	void createList(lua_State* L, const Cont& container)
	{
		lua_newtable(L);
		int n = 0;
		for (typename Cont::const_iterator iter = container.begin(); iter != container.end(); ++iter)
		{
			lua_pushinteger(L, ++n);
			push(L, *iter);
			lua_settable(L, -3);
		}
	}
}

#endif
