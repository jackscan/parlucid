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

#include "../cmdlinetokenizer.hpp"
#include "../cmdlinetokens.hpp"
#include <cstring>

using namespace CmdLineTokens;

int CmdLineTokenizer::process(State& state, const char* symbols, int len, int& tok)
{
	const State nStateHandlers = sizeof(stateHandler) / sizeof(*stateHandler);
	if (state < 0) return 0;
	if (state < nStateHandlers)
		return ((state = stateHandler[state](*symbols, tok)), 1);
	else return ((state = ERROR_STATE), 0);
}

const CmdLineTokenizer::State ERROR_STATE = CmdLineTokenizer::ERROR_STATE;

static CmdLineTokenizer::State handleState0(char sym, int& tok)
{
	switch(sym)
	{
	case '\t': return (tok = TOK_12, 6);
	case ' ': return (tok = TOK_12, 6);
	case '"': return (4);
	case '-': return (tok = TOK_name, 7);
	case '=': return (tok = TOK_11, 3);
	case '\\': return (tok = TOK_name, 8);
	case '_': return (tok = TOK_name, 8);
	default: switch(sym >> 4) // ['\x80', '\x7f']
		{
		case 2: return sym < '.' ? ERROR_STATE : (tok = TOK_name, 8);
		case 3: return sym < ';' ? (tok = TOK_name, 8) : ERROR_STATE;
		case 4: return sym < 'A' ? ERROR_STATE : (tok = TOK_name, 8);
		case 5: return sym < '[' ? (tok = TOK_name, 8) : ERROR_STATE;
		case 6: return sym < 'a' ? ERROR_STATE : (tok = TOK_name, 8);
		case 7: return sym < '{' ? (tok = TOK_name, 8) : ERROR_STATE;
		default: return ERROR_STATE;
		}
	}
}

static CmdLineTokenizer::State handleState1(char sym, int& tok)
{
	switch(sym)
	{
	case '\t': return (tok = TOK_12, 6);
	case ' ': return (tok = TOK_12, 6);
	case '"': return (4);
	case '=': return (tok = TOK_11, 3);
	case '\\': return (tok = TOK_name, 8);
	case '_': return (tok = TOK_name, 8);
	default: switch(sym >> 4) // ['\x80', '\x7f']
		{
		case 2: return sym < '-' ? ERROR_STATE : (tok = TOK_name, 8);
		case 3: return sym < ';' ? (tok = TOK_name, 8) : ERROR_STATE;
		case 4: return sym < 'A' ? ERROR_STATE : (tok = TOK_name, 8);
		case 5: return sym < '[' ? (tok = TOK_name, 8) : ERROR_STATE;
		case 6: return sym < 'a' ? ERROR_STATE : (tok = TOK_name, 8);
		case 7: return sym < '{' ? (tok = TOK_name, 8) : ERROR_STATE;
		default: return ERROR_STATE;
		}
	}
}

static CmdLineTokenizer::State handleState2(char sym, int& tok)
{
	switch(sym)
	{
	case '\t': return (tok = TOK_12, 6);
	case ' ': return (tok = TOK_12, 6);
	case '"': return (4);
	case ':': return (tok = TOK_name, 8);
	case '=': return (tok = TOK_11, 3);
	case '\\': return (tok = TOK_path, 9);
	case '_': return (tok = TOK_path, 9);
	default: switch(sym >> 4) // ['\x80', '\x7f']
		{
		case 2: return sym < '-' ? ERROR_STATE : (tok = TOK_path, 9);
		case 3: return sym < ':' ? (tok = TOK_path, 9) : ERROR_STATE;
		case 4: return sym < 'A' ? ERROR_STATE : (tok = TOK_path, 10);
		case 5: return sym < '[' ? (tok = TOK_path, 10) : ERROR_STATE;
		case 6: return sym < 'a' ? ERROR_STATE : (tok = TOK_path, 10);
		case 7: return sym < '{' ? (tok = TOK_path, 10) : ERROR_STATE;
		default: return ERROR_STATE;
		}
	}
}

static CmdLineTokenizer::State handleState4(char sym, int& tok)
{
	switch(sym)
	{
	case '"': return (tok = TOK_value, 3);
	case '\\': return (5);
	default: return (4);
	}
}

static CmdLineTokenizer::State handleState5(char sym, int& tok)
{
	switch(sym)
	{
	case '"': return (tok = TOK_value, 4);
	case '\\': return (5);
	default: return (4);
	}
}

static CmdLineTokenizer::State handleState6(char sym, int& tok)
{
	switch(sym)
	{
	case '\t': return (tok = TOK_12, 6);
	case ' ': return (tok = TOK_12, 6);
	default: return ERROR_STATE;
	}
}

static CmdLineTokenizer::State handleState7(char sym, int& tok)
{
	switch(sym)
	{
	case '\\': return (tok = TOK_name, 8);
	case '_': return (tok = TOK_name, 8);
	case 'i': return (tok = TOK_iarg, 3);
	case 'o': return (tok = TOK_oarg, 3);
	default: switch(sym >> 4) // ['\x80', '\x7f']
		{
		case 2: return sym < '-' ? ERROR_STATE : (tok = TOK_name, 8);
		case 3: return sym < ';' ? (tok = TOK_name, 8) : ERROR_STATE;
		case 4: return sym < 'A' ? ERROR_STATE : (tok = TOK_name, 8);
		case 5: return sym < '[' ? (tok = TOK_name, 8) : ERROR_STATE;
		case 6: return sym < 'a' ? ERROR_STATE : (tok = TOK_name, 8);
		case 7: return sym < '{' ? (tok = TOK_name, 8) : ERROR_STATE;
		default: return ERROR_STATE;
		}
	}
}

static CmdLineTokenizer::State handleState8(char sym, int& tok)
{
	switch(sym)
	{
	case '\\': return (tok = TOK_name, 8);
	case '_': return (tok = TOK_name, 8);
	default: switch(sym >> 4) // ['\x80', '\x7f']
		{
		case 2: return sym < '-' ? ERROR_STATE : (tok = TOK_name, 8);
		case 3: return sym < ';' ? (tok = TOK_name, 8) : ERROR_STATE;
		case 4: return sym < 'A' ? ERROR_STATE : (tok = TOK_name, 8);
		case 5: return sym < '[' ? (tok = TOK_name, 8) : ERROR_STATE;
		case 6: return sym < 'a' ? ERROR_STATE : (tok = TOK_name, 8);
		case 7: return sym < '{' ? (tok = TOK_name, 8) : ERROR_STATE;
		default: return ERROR_STATE;
		}
	}
}

static CmdLineTokenizer::State handleState9(char sym, int& tok)
{
	switch(sym)
	{
	case '\\': return (tok = TOK_path, 9);
	case '_': return (tok = TOK_path, 9);
	default: switch(sym >> 4) // ['\x80', '\x7f']
		{
		case 2: return sym < '-' ? ERROR_STATE : (tok = TOK_path, 9);
		case 3: return sym < ':' ? (tok = TOK_path, 9) : ERROR_STATE;
		case 4: return sym < 'A' ? ERROR_STATE : (tok = TOK_path, 9);
		case 5: return sym < '[' ? (tok = TOK_path, 9) : ERROR_STATE;
		case 6: return sym < 'a' ? ERROR_STATE : (tok = TOK_path, 9);
		case 7: return sym < '{' ? (tok = TOK_path, 9) : ERROR_STATE;
		default: return ERROR_STATE;
		}
	}
}

static CmdLineTokenizer::State handleState10(char sym, int& tok)
{
	switch(sym)
	{
	case ':': return (11);
	case '\\': return (tok = TOK_path, 9);
	case '_': return (tok = TOK_path, 9);
	default: switch(sym >> 4) // ['\x80', '\x7f']
		{
		case 2: return sym < '-' ? ERROR_STATE : (tok = TOK_path, 9);
		case 3: return sym < ':' ? (tok = TOK_path, 9) : ERROR_STATE;
		case 4: return sym < 'A' ? ERROR_STATE : (tok = TOK_path, 9);
		case 5: return sym < '[' ? (tok = TOK_path, 9) : ERROR_STATE;
		case 6: return sym < 'a' ? ERROR_STATE : (tok = TOK_path, 9);
		case 7: return sym < '{' ? (tok = TOK_path, 9) : ERROR_STATE;
		default: return ERROR_STATE;
		}
	}
}

static CmdLineTokenizer::State handleState11(char sym, int& tok)
{
	switch(sym)
	{
	case '/': return (tok = TOK_path, 9);
	case '\\': return (tok = TOK_path, 9);
	default: return ERROR_STATE;
	}
}

static CmdLineTokenizer::State handleDeadEnd(char, int&)
{
	return ERROR_STATE;
}

const CmdLineTokenizer::State CmdLineTokenizer::ERROR_STATE = -1;

const CmdLineTokenizer::PfnStateHandler CmdLineTokenizer::stateHandler[12] = {
	handleState0,
	handleState1,
	handleState2,
	handleDeadEnd,
	handleState4,
	handleState5,
	handleState6,
	handleState7,
	handleState8,
	handleState9,
	handleState10,
	handleState11,
};
