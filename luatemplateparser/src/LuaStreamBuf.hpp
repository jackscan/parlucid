#ifndef LUASTREAMBUF_HPP
#define LUASTREAMBUF_HPP

#include <lua.hpp>

#include <streambuf>

class LuaStreamBuf: public std::streambuf
{
    char buffer[64];
public:
	LuaStreamBuf(lua_State* state): state(state)
	{
		setp(buffer, buffer + (sizeof(buffer) - 1));
	}

	virtual ~LuaStreamBuf()
	{
        sync();
    }


private:

	void flush()
	{
		int num = static_cast<int>(pptr() - pbase());

		lua_getglobal(state, "io");
		lua_getfield(state, -1, "write");
		lua_pushlstring(state, buffer, num);
		lua_call(state, 1, 0);

        pbump(-num);
    }

	int_type overflow (int_type c)
	{
        if (c != EOF)
		{
            *pptr() = c;
            pbump(1);
        }

		flush();
        return c;
    }

    int sync ()
	{
        flush();
        return 0;
    }

	lua_State* state;
};

#endif
