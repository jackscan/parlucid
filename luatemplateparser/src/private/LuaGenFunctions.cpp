#include "../LuaGenFunctions.hpp"

#include <fstream>
#include <cassert>

#include <boost/filesystem.hpp>

// lua functions
//static const char* g_luaOutput = "g_output";

namespace luagen
{

LuaOutput::~LuaOutput()
{
	for (std::map<std::string, std::fstream*>::iterator i = fstreams.begin(); i != fstreams.end(); ++i)
		delete i->second;
}

LuaOutput& getLuaOutput(lua_State* L)
{
	LuaOutput& output = *reinterpret_cast<LuaOutput*>(lua_touserdata(L, lua_upvalueindex(1)));
	return output;
}

int write(lua_State *L)
{
	CodeStream *cs = getLuaOutput(L).current;
	if (cs)
	{
		int n = lua_gettop(L);
		for (int i = 1; i <= n; i++)
		{
			const char* str = lua_tostring(L, i);
			if (str) *cs << str;
		}
	}
	return 0;
}

int flush(lua_State *L)
{
	CodeStream *cs = getLuaOutput(L).current;
	if (cs) cs->flush();
	return 0;
}

int setIndent(lua_State *L)
{
	CodeStream *cs = getLuaOutput(L).current;
	if (cs) cs->setIndent(lua_tostring(L, 1));
	return 0;
}

int pushIndent(lua_State *L)
{
	CodeStream *cs = getLuaOutput(L).current;
	if (cs) cs->incIndent();
	return 0;
}

int popIndent(lua_State *L)
{
	CodeStream *cs = getLuaOutput(L).current;
	if (cs) cs->decIndent();
	return 0;
}

int newline(lua_State *L)
{
	CodeStream *cs = getLuaOutput(L).current;
	if (cs)
	{
		int nlines = (lua_gettop(L) > 0) ? static_cast<int>(lua_tointeger(L, 1)) : 0;
		cs->newline(nlines);
	}
	return 0;
}

int output(lua_State *L)
{
	int args = lua_gettop(L);
	assert(0 < args);
	const char* filename = lua_tostring(L, 1);
	LuaOutput& output = getLuaOutput(L);
	int result = 0;
	if (filename)
	{
		std::string filenameStr(filename);
		std::map<std::string, CodeStream>::iterator ics = output.cstreams.find(filenameStr);
		std::map<std::string, std::fstream*>::iterator ifs = output.fstreams.find(filenameStr);

		if (ifs == output.fstreams.end())
			ifs = output.fstreams.insert(std::make_pair(filenameStr, new std::fstream)).first;

		std::fstream& file = *ifs->second;

		if (ics == output.cstreams.end())
			ics = output.cstreams.insert(std::make_pair(filenameStr, CodeStream(file, std::string("")))).first;

		if (!file.is_open())
		{
			boost::filesystem::path path(filename);
			path.remove_filename();
			boost::filesystem::create_directories(path);
			file.open(filename, std::ios::out);

		}
		if (file.is_open()) output.current = &ics->second;
		else
		{
			lua_pushfstring(L, "could not open file `%s'", filename);
			output.current = 0;
			result = 1;
		}
	}
	else
	{
		output.current = &output.stdoutstream;
	}

	return result;
}

void registerFunctions(lua_State* lua, LuaOutput& luaOutput)
{
	CREATELUAOUTPUTFUNC(lua, luaOutput, output);
	CREATELUAOUTPUTFUNC(lua, luaOutput, write);
	CREATELUAOUTPUTFUNC(lua, luaOutput, flush);
	CREATELUAOUTPUTFUNC(lua, luaOutput, pushIndent);
	CREATELUAOUTPUTFUNC(lua, luaOutput, popIndent);
	CREATELUAOUTPUTFUNC(lua, luaOutput, setIndent);
	CREATELUAOUTPUTFUNC(lua, luaOutput, newline);
}

}
