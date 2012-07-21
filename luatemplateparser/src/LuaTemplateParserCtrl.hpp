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

#ifndef TEMPLATEPARSERCTRL_HPP
#define TEMPLATEPARSERCTRL_HPP

#include <ios>
#include <fstream>

#include "../gen/LuaTemplateTokenizer.hpp"
#include "../gen/LuaTemplateParser.hpp"
#include "../gen/LuaTemplateTokens.hpp"

#include "../../genutils/src/codestream.hpp"
#include "../../parserlog/src/ParserLog.hpp"
//#include "luastreambuf.hpp"



#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <iostream>

class LuaTemplateParserCtrl
{
public:
	typedef int Token;

	LuaTemplateParserCtrl();
	~LuaTemplateParserCtrl();

	const char* parse(const char *filename = 0);

	friend class LuaTemplateParser;

	const ParserLog& log() const;

private:
	int shift(int);
	int error(int, const int*);
	void reduce(int);

	//void text(const char* indent, const char* chunk, const char* nl);
	void code(const char* chunk);
	void quote();
	void codenl();
	void codeindent(const char* indent = "");
	void variable(const char* varname);
	void snippet(const char* code);
	void embedtextbegin();
	void embedtextend();
	void embed(const char* code);
	void embedif(const char* code);
	void embedelse();
	void embedend();
	void chunk(const char* chunk);
	void textindent(const char* spaces);
	void textindent();
	void textnl();
	void weaknl();
	void textbegin();
	void textend();
	void include(const char* filename);

	const char* string();
	void release(const char*);

	void openFile(const char*);
	void closeFiles();

	LuaTemplateParserCtrl& self();

	struct FileInfo
	{
		FileInfo(FILE* file = 0, const char* name = 0)
			:  file(file), filename(name ? name : ""), validBuffer(0), lineCounter(1)
		{
			buffer.reserve(64);
		}

		FileInfo(const FileInfo& rhs)
			: file(rhs.file), filename(rhs.filename), buffer(rhs.buffer), validBuffer(rhs.validBuffer), lineCounter(rhs.lineCounter)
		{
			buffer.reserve(64);
		}

		FileInfo& operator= (const FileInfo& rhs)
		{
			file = rhs.file;
			filename = rhs.filename;
			buffer = rhs.buffer;
			validBuffer = rhs.validBuffer;
			lineCounter = rhs.lineCounter;
			buffer.reserve(64);
			return *this;
		}

		FILE *file;
		std::string filename;
		std::vector<char> buffer;
		int validBuffer;
		int lineCounter;
	};

	std::vector<FileInfo> files;
	std::vector<const char*> tokens;

	std::stringstream result;
	CodeStream cstream;
	std::string currentCodeIndent;
	std::string resultstr;
	std::string currentTextIndent;
	Token token;

	bool errors;
	ParserLog parserLog;

	LuaTemplateParser parser;
	LuaTemplateTokenizer fsm;
};

#endif
