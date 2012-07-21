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

#include "../RegexParserCtrl.hpp"
#include "../../gen/RegexTokens.hpp"

#include <iostream>
#include <limits>
#include <cassert>

using namespace RegexTokens;

RegexParserCtrl::RegexParserCtrl(const std::string& regex)
: regexstr(regex)
, nextToken(TOK_END)
, start(0)
, end(0)
, parser(self())
{ }

int RegexParserCtrl::parse()
{
	parser.parse();
	return end;
}

const RegexParserCtrl::NfaChar& RegexParserCtrl::getNfa() const
{
	return nfas.top();
}

const RegexParserCtrl::NfaChar& RegexParserCtrl::getLookahead() const
{
	return lookaheadNfa;
}

int RegexParserCtrl::shift(int context)
{
	start = end;
	int len = static_cast<int>(regexstr.size()) - start;

	if (len > 0)
	{
		fsm.begin(context);
		fsm.process(&regexstr.c_str()[start], len);
		end = start + fsm.end(nextToken);
	}
	else nextToken = TOK_END;

	return nextToken;
}

int RegexParserCtrl::error(size_t n, const int *expected)
{
	std::cerr << "error in regex `" << regexstr << "' at ";

	if (start < end)
		std::cerr << "token `" << regexstr.substr(start, end - start) << "'" << std::endl;
	else std::cerr << "`" << regexstr.substr(start) << "'" << std::endl;

	std::cerr << "\texpected:";
	for (size_t i = 0; i < n; ++i)
		std::cerr << (i > 0 ? ", " : " ") << getTokenName(expected[i]);
	std::cerr << std::endl;
	return 1;
}

void RegexParserCtrl::reduce(int prod)
{
	//std::cout << "regex reduce " << prod << std::endl;
}

void RegexParserCtrl::lookahead()
{
	lookaheadNfa = popNfa();
}

void RegexParserCtrl::alternative()
{
	NfaChar nfa2 = popNfa();
	NfaChar nfa1 = popNfa();
	nfas.push(nfa1 | nfa2);
}

void RegexParserCtrl::concat()
{
	NfaChar nfa2 = popNfa();
	NfaChar nfa1 = popNfa();
	nfas.push(nfa1 + nfa2);
}

void RegexParserCtrl::asterisk()
{
	nfas.push(*popNfa());
}

void RegexParserCtrl::plus()
{
	NfaChar nfa = popNfa();
	nfas.push(*nfa + nfa);
}

void RegexParserCtrl::optional()
{
	nfas.push(popNfa() | NfaChar());
}

/*void RegexParserCtrl::inverse()
{
	nfas.push(~popNfa());
}*/

void RegexParserCtrl::character(const char* ch)
{
	nfas.push(NfaChar((Char)*ch));
}

void RegexParserCtrl::group()
{
	nfas.push(NfaChar(alphabet));
}

void RegexParserCtrl::inverseGroup()
{
	AlphabetChar inv(Interval<Char>(std::numeric_limits<Char>::min(), std::numeric_limits<Char>::max()));
	inv.del(alphabet);
	nfas.push(NfaChar(inv));
}

void RegexParserCtrl::singleton(const char* ch)
{
	alphabet = AlphabetChar((Char)*ch);
}

void RegexParserCtrl::interval(const char* first, const char* last)
{
	alphabet = AlphabetChar(Interval<Char>((Char)*first, (Char)*last));
}

void RegexParserCtrl::concatSingleton(const char* ch)
{
	alphabet.add((Char)*ch);
}

void RegexParserCtrl::concatInterval(const char* first, const char* last)
{
	alphabet.add((Char)*first, (Char)*last);
}

const char* RegexParserCtrl::escChar(const char* ch)
{
	const char *str = "\a\b\f\n\r\t\v";
	const char* result = ch;

	switch(*ch)
	{
	case 'a': result = str + 0; break;
	case 'b': result = str + 1; break;
	case 'f': result = str + 2; break;
	case 'n': result = str + 3; break;
	case 'r': result = str + 4; break;
	case 't': result = str + 5; break;
	case 'v': result = str + 6; break;
	case '0': result = str + 7; break;
	}
	return result;
}

RegexParserCtrl::NfaChar RegexParserCtrl::popNfa()
{
	if (!nfas.empty())
	{
		NfaChar result = nfas.top();
		nfas.pop();
		return result;
	}
	else
	{
		std::cerr << "nfa stack empty!" << std::endl;
		return NfaChar();
	}
}

const char* RegexParserCtrl::string()
{
	return &regexstr[start];
}

void RegexParserCtrl::release(const char* str)
{
}
