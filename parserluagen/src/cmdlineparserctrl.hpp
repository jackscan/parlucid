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
