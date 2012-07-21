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

#include "../cmdlineparser.hpp"
#include "../../src/cmdlineparserctrl.hpp"

CmdLineParser::CmdLineParser(CmdLineParserCtrl &arg)
: mediator(arg), next(&CmdLineParser::shift), token(0), symbol(0)
{
	curr.name = 0;
	curr.path = 0;
	curr.value = 0;
	curr.file = 0;
	curr.assign = 0;
}
CmdLineParser::~CmdLineParser()
{
	curr.name = 0;
	curr.path = 0;
	curr.value = 0;
	curr.file = 0;
	curr.assign = 0;
}
void CmdLineParser::parse()
{
	handleState0();
}

int CmdLineParser::shift(int context)
{
	return token = mediator.shift(context);
}

int CmdLineParser::push(int)
{
	next = &CmdLineParser::recall;
	return symbol;
}

int CmdLineParser::recall(int)
{
	next = &CmdLineParser::shift;
	return token;
}

void CmdLineParser::reduce(int production, int sym)
{
	symbol = sym;
	next = &CmdLineParser::push;
	mediator.reduce(production);
}

void CmdLineParser::ignore(int production)
{
	next = &CmdLineParser::recall;
	mediator.reduce(production);
}

// cmds->cmds file
void CmdLineParser::reduce0()
{
	reduce(0, 6); 
	mediator.file(curr.file->path.str);
}
// cmds->cmds loutput
void CmdLineParser::reduce1()
{
	reduce(1, 6); 
}
// cmds->cmds grammar
void CmdLineParser::reduce2()
{
	reduce(2, 6); 
}
// cmds->cmds assign
void CmdLineParser::reduce3()
{
	reduce(3, 6); 
	mediator.assign(curr.assign->name.str, curr.assign->value.str);
}
// cmds->
void CmdLineParser::reduce4()
{
	reduce(4, 6); 
}
// file->name
void CmdLineParser::reduce5()
{
	reduce(5, 7); 
	parent.file.path.str = curr.name->str;
}
// file->path
void CmdLineParser::reduce6()
{
	reduce(6, 7); 
	parent.file.path.str = curr.path->str;
	curr.path->str = 0;
}
// loutput->oarg file
void CmdLineParser::reduce7()
{
	reduce(7, 8); 
	mediator.loutput(curr.file->path.str);
}
// grammar->iarg file
void CmdLineParser::reduce8()
{
	reduce(8, 9); 
	mediator.grammar(curr.file->path.str);
}
// assign->name "=" value
void CmdLineParser::reduce9()
{
	reduce(9, 10); 
	parent.assign.name.str = curr.name->str;
	curr.name->str = 0;
	parent.assign.value.str = curr.value->str;
	curr.value->str = 0;
}
// assign->name "=" path
void CmdLineParser::reduce10()
{
	reduce(10, 10); 
	parent.assign.value.str = mediator.stringize(curr.path->str);
	parent.assign.name.str = curr.name->str;
	curr.name->str = 0;
}
// END->/[ \t]+/
void CmdLineParser::reduce11()
{
	ignore(11); 
}
// END->END cmds
void CmdLineParser::reduce12()
{

}
// [cmds->.cmds file],
//    (END iarg oarg name path)
// [cmds->.cmds loutput],
//    (END iarg oarg name path)
// [cmds->.cmds grammar],
//    (END iarg oarg name path)
// [cmds->.cmds assign],
//    (END iarg oarg name path)
// [cmds->.],
//    (END iarg oarg name path)
// [END->END.cmds],
//    (END)
//<ignore> [END->./[ \t]+/],
//    (END iarg oarg name path cmds file loutput grammar assign /[ \t]+/)
int CmdLineParser::handleState0()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case 6: result = handleState1(); break;
		case 12: result = handleState2(0); break;
		default: result = 0; reduce4(); break;
		}
	}
	while (result == 0);
	return --result;
}

// [cmds->cmds.file],
//    (END iarg oarg name path)
// [cmds->cmds.loutput],
//    (END iarg oarg name path)
// [cmds->cmds.grammar],
//    (END iarg oarg name path)
// [cmds->cmds.assign],
//    (END iarg oarg name path)
// [file->.name],
//    (END iarg oarg name path)
// [file->.path],
//    (END iarg oarg name path)
// [loutput->.oarg file],
//    (END iarg oarg name path)
// [grammar->.iarg file],
//    (END iarg oarg name path)
// [assign->.name "=" value],
//    (END iarg oarg name path)
// [assign->.name "=" path],
//    (END iarg oarg name path)
// [END->END cmds.],
//    (END)
//<ignore> [END->./[ \t]+/],
//    (END iarg oarg name path /[ \t]+/)
int CmdLineParser::handleState1()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case 1: result = handleState3(); break;
		case 2: result = handleState4(); break;
		case 3: 
			{
				TokName tok;
				tok.str = mediator.string();
				TokName *prev = curr.name;
				curr.name = &tok;
				result = handleState5(); 
				mediator.release(tok.str);
				curr.name = prev;
			}
			break;
		case 4: 
			{
				TokPath tok;
				tok.str = mediator.string();
				TokPath *prev = curr.path;
				curr.path = &tok;
				result = handleState6(); 
				mediator.release(tok.str);
				curr.path = prev;
			}
			break;
		case 7: 
			{
				TokFile tok = parent.file;
				TokFile *prev = curr.file;
				curr.file = &tok;
				result = handleState7(); 
				mediator.release(tok.path.str);
				curr.file = prev;
			}
			break;
		case 8: result = handleState8(); break;
		case 9: result = handleState9(); break;
		case 10: 
			{
				TokAssign tok = parent.assign;
				TokAssign *prev = curr.assign;
				curr.assign = &tok;
				result = handleState10(); 
				mediator.release(tok.name.str);
				mediator.release(tok.value.str);
				curr.assign = prev;
			}
			break;
		case 12: result = handleState2(0); break;
		default: result = 2; reduce12(); break;
		}
	}
	while (result == 0);
	return --result;
}

//<ignore> [END->/[ \t]+/.],
//    (END iarg oarg name path value cmds file loutput grammar assign "=" /[ \t]+/)
int CmdLineParser::handleState2(int context)
{
	(this->*next)(context);
	reduce11();
	return 0;
}

// [file->.name],
//    (END iarg oarg name path)
// [file->.path],
//    (END iarg oarg name path)
// [grammar->iarg.file],
//    (END iarg oarg name path)
//<ignore> [END->./[ \t]+/],
//    (name path /[ \t]+/)
int CmdLineParser::handleState3()
{
	int result = 0;
	do
	{
		switch((this->*next)(1))
		{
		case 3: 
			{
				TokName tok;
				tok.str = mediator.string();
				TokName *prev = curr.name;
				curr.name = &tok;
				result = handleState11(); 
				mediator.release(tok.str);
				curr.name = prev;
			}
			break;
		case 4: 
			{
				TokPath tok;
				tok.str = mediator.string();
				TokPath *prev = curr.path;
				curr.path = &tok;
				result = handleState6(); 
				mediator.release(tok.str);
				curr.path = prev;
			}
			break;
		case 7: 
			{
				TokFile tok = parent.file;
				TokFile *prev = curr.file;
				curr.file = &tok;
				result = handleState12(); 
				mediator.release(tok.path.str);
				curr.file = prev;
			}
			break;
		case 12: result = handleState2(1); break;
		default:
			{
				const int expected[] = { 3, 4, 7, 12, };
				result = mediator.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while (result == 0);
	return --result;
}

// [file->.name],
//    (END iarg oarg name path)
// [file->.path],
//    (END iarg oarg name path)
// [loutput->oarg.file],
//    (END iarg oarg name path)
//<ignore> [END->./[ \t]+/],
//    (name path /[ \t]+/)
int CmdLineParser::handleState4()
{
	int result = 0;
	do
	{
		switch((this->*next)(1))
		{
		case 3: 
			{
				TokName tok;
				tok.str = mediator.string();
				TokName *prev = curr.name;
				curr.name = &tok;
				result = handleState11(); 
				mediator.release(tok.str);
				curr.name = prev;
			}
			break;
		case 4: 
			{
				TokPath tok;
				tok.str = mediator.string();
				TokPath *prev = curr.path;
				curr.path = &tok;
				result = handleState6(); 
				mediator.release(tok.str);
				curr.path = prev;
			}
			break;
		case 7: 
			{
				TokFile tok = parent.file;
				TokFile *prev = curr.file;
				curr.file = &tok;
				result = handleState13(); 
				mediator.release(tok.path.str);
				curr.file = prev;
			}
			break;
		case 12: result = handleState2(1); break;
		default:
			{
				const int expected[] = { 3, 4, 7, 12, };
				result = mediator.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while (result == 0);
	return --result;
}

// [file->name.],
//    (END iarg oarg name path)
// [assign->name."=" value],
//    (END iarg oarg name path)
// [assign->name."=" path],
//    (END iarg oarg name path)
//<ignore> [END->./[ \t]+/],
//    (END iarg oarg name path "=" /[ \t]+/)
int CmdLineParser::handleState5()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case 11: result = handleState14(); break;
		case 12: result = handleState2(0); break;
		default: result = 1; reduce5(); break;
		}
	}
	while (result == 0);
	return --result;
}

// [file->path.],
//    (END iarg oarg name path)
//<ignore> [END->./[ \t]+/],
//    (END iarg oarg name path /[ \t]+/)
int CmdLineParser::handleState6()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case 12: result = handleState2(0); break;
		default: result = 1; reduce6(); break;
		}
	}
	while (result == 0);
	return --result;
}

// [cmds->cmds file.],
//    (END iarg oarg name path)
//<ignore> [END->./[ \t]+/],
//    (END iarg oarg name path /[ \t]+/)
int CmdLineParser::handleState7()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case 12: result = handleState2(0); break;
		default: result = 2; reduce0(); break;
		}
	}
	while (result == 0);
	return --result;
}

// [cmds->cmds loutput.],
//    (END iarg oarg name path)
//<ignore> [END->./[ \t]+/],
//    (END iarg oarg name path /[ \t]+/)
int CmdLineParser::handleState8()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case 12: result = handleState2(0); break;
		default: result = 2; reduce1(); break;
		}
	}
	while (result == 0);
	return --result;
}

// [cmds->cmds grammar.],
//    (END iarg oarg name path)
//<ignore> [END->./[ \t]+/],
//    (END iarg oarg name path /[ \t]+/)
int CmdLineParser::handleState9()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case 12: result = handleState2(0); break;
		default: result = 2; reduce2(); break;
		}
	}
	while (result == 0);
	return --result;
}

// [cmds->cmds assign.],
//    (END iarg oarg name path)
//<ignore> [END->./[ \t]+/],
//    (END iarg oarg name path /[ \t]+/)
int CmdLineParser::handleState10()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case 12: result = handleState2(0); break;
		default: result = 2; reduce3(); break;
		}
	}
	while (result == 0);
	return --result;
}

// [file->name.],
//    (END iarg oarg name path)
//<ignore> [END->./[ \t]+/],
//    (END iarg oarg name path /[ \t]+/)
int CmdLineParser::handleState11()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case 12: result = handleState2(0); break;
		default: result = 1; reduce5(); break;
		}
	}
	while (result == 0);
	return --result;
}

// [grammar->iarg file.],
//    (END iarg oarg name path)
//<ignore> [END->./[ \t]+/],
//    (END iarg oarg name path /[ \t]+/)
int CmdLineParser::handleState12()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case 12: result = handleState2(0); break;
		default: result = 2; reduce8(); break;
		}
	}
	while (result == 0);
	return --result;
}

// [loutput->oarg file.],
//    (END iarg oarg name path)
//<ignore> [END->./[ \t]+/],
//    (END iarg oarg name path /[ \t]+/)
int CmdLineParser::handleState13()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case 12: result = handleState2(0); break;
		default: result = 2; reduce7(); break;
		}
	}
	while (result == 0);
	return --result;
}

// [assign->name "=".value],
//    (END iarg oarg name path)
// [assign->name "=".path],
//    (END iarg oarg name path)
//<ignore> [END->./[ \t]+/],
//    (path value /[ \t]+/)
int CmdLineParser::handleState14()
{
	int result = 0;
	do
	{
		switch((this->*next)(2))
		{
		case 4: 
			{
				TokPath tok;
				tok.str = mediator.string();
				TokPath *prev = curr.path;
				curr.path = &tok;
				result = handleState15(); 
				mediator.release(tok.str);
				curr.path = prev;
			}
			break;
		case 5: 
			{
				TokValue tok;
				tok.str = mediator.string();
				TokValue *prev = curr.value;
				curr.value = &tok;
				result = handleState16(); 
				mediator.release(tok.str);
				curr.value = prev;
			}
			break;
		case 12: result = handleState2(2); break;
		default:
			{
				const int expected[] = { 4, 5, 12, };
				result = mediator.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while (result == 0);
	return --result;
}

// [assign->name "=" path.],
//    (END iarg oarg name path)
//<ignore> [END->./[ \t]+/],
//    (END iarg oarg name path /[ \t]+/)
int CmdLineParser::handleState15()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case 12: result = handleState2(0); break;
		default: result = 3; reduce10(); break;
		}
	}
	while (result == 0);
	return --result;
}

// [assign->name "=" value.],
//    (END iarg oarg name path)
//<ignore> [END->./[ \t]+/],
//    (END iarg oarg name path /[ \t]+/)
int CmdLineParser::handleState16()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case 12: result = handleState2(0); break;
		default: result = 3; reduce9(); break;
		}
	}
	while (result == 0);
	return --result;
}

