#ifndef CMDLINEPARSERCTRL_HPP
#define CMDLINEPARSERCTRL_HPP

#include "../gen/cmdlinetokenizer.hpp"

#include <lua.hpp>

#include <utility>
#include <vector>
//#include <string>

class CmdLineParserCtrl
{
public:
	CmdLineParserCtrl(int argc, char* argv[], lua_State *);
	~CmdLineParserCtrl();

	const std::vector<const char*>& getFiles() const { return files; }
	const std::vector<const char*>& getGrammarFiles() const { return grammarFiles; }
	const char* getLuaOutput() const { return luaOutput; }

	void assign(const char*, const char*);
	void file(const char*);
	void grammar(const char*);
	void loutput(const char*);
	const char* stringize(const char*);

	int shift(int);

	int error(int, const int*);
	void reduce(int);

	const char* string();
	void release(const char*);

private:

	CmdLineTokenizer fsm;

	int argc;
	char** argv;
	lua_State * lua;
	int tokbegin;
	std::pair<int, int> pos;
	std::vector<const char*> tokens;

	std::vector<const char*> files;
	std::vector<const char*> grammarFiles;
	const char* luaOutput;
	//std::vector<
};

#endif
