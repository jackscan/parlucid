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
