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

#ifndef LUAGENFUNCTIONS_HPP
#define LUAGENFUNCTIONS_HPP

#include "../../genutils/src/codestream.hpp"

#include <map>
#include <iostream>
#include <lua.hpp>

namespace luagen
{

struct LuaOutput
{
	LuaOutput() : stdoutstream(std::cout, "\t"), current(&stdoutstream) {}
	~LuaOutput();
	
	std::map<std::string, CodeStream> cstreams;
	std::map<std::string, std::fstream*> fstreams;
	CodeStream stdoutstream;
	CodeStream* current;
};

LuaOutput& getLuaOutput(lua_State* L);
int write(lua_State *L);
int flush(lua_State *L);
int setIndent(lua_State *L);
int pushIndent(lua_State *L);
int popIndent(lua_State *L);
int newline(lua_State *L);
int output(lua_State *L);
void registerFunctions(lua_State* lua, LuaOutput& luaOutput);

#define CREATELUAFUNCTION(L, F) lua_pushcfunction(L, &F); lua_setglobal(L, #F)
#define CREATELUAOUTPUTFUNC(L, O, F) lua_pushlightuserdata(L, &O); lua_pushcclosure (L, &F, 1); lua_setglobal(L, #F)

}

#endif
