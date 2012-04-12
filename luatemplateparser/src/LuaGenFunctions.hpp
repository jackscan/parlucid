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
