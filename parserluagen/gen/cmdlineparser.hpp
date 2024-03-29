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

#ifndef CMDLINEPARSER_HPP
#define CMDLINEPARSER_HPP

class CmdLineParserCtrl;

// Grammar:
// 0: cmds->cmds file, file(file.path); 
// 1: cmds->cmds loutput
// 2: cmds->cmds grammar
// 3: cmds->cmds assign, assign(assign.name, assign.value); 
// 4: cmds->
// 5: file->name
// 6: file->path
// 7: loutput->oarg file, loutput(file.path); 
// 8: grammar->iarg file, grammar(file.path); 
// 9: assign->name "=" value
// 10: assign->name "=" path, stringize(path); 
// 11: END->/[ \t]+/
// 12: END->END cmds
class CmdLineParser
{
public: 
	CmdLineParser(CmdLineParserCtrl &);
	~CmdLineParser();

	void parse();
protected: 
	int shift(int);
	int push(int);
	int recall(int);

	void reduce0();
	void reduce1();
	void reduce2();
	void reduce3();
	void reduce4();
	void reduce5();
	void reduce6();
	void reduce7();
	void reduce8();
	void reduce9();
	void reduce10();
	void reduce11();
	void reduce12();
	void reduce(int, int);
	void ignore(int);

	int handleState0();
	int handleState1();
	int handleState2(int);
	int handleState3();
	int handleState4();
	int handleState5();
	int handleState6();
	int handleState7();
	int handleState8();
	int handleState9();
	int handleState10();
	int handleState11();
	int handleState12();
	int handleState13();
	int handleState14();
	int handleState15();
	int handleState16();

	CmdLineParserCtrl& mediator;
	int (CmdLineParser::*next)(int);
	int token;
	int symbol;
	struct TokName
	{ const char* str; };
	struct TokPath
	{ const char* str; };
	struct TokValue
	{ const char* str; };
	struct TokFile
	{ struct { const char* str; } path; };
	struct TokAssign
	{ struct { const char* str; } name; struct { const char* str; } value; };
	struct
	{
		TokName *name;
		TokPath *path;
		TokValue *value;
		TokFile *file;
		TokAssign *assign;
	} curr;
	union
	{
		TokName name;
		TokPath path;
		TokValue value;
		TokFile file;
		TokAssign assign;
	} parent;
};

#endif
