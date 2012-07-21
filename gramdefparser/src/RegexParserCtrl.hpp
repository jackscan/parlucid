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

#ifndef REGEXPARSERCTRL_HPP
#define REGEXPARSERCTRL_HPP

#include "../gen/RegexTokenizer.hpp"
#include "../gen/RegexParser.hpp"
#include "../../lexer/src/nfa.hpp"
#include "../../lexer/src/alphabet.hpp"

#include <istream>
#include <string>
#include <stack>

class RegexParserCtrl
{
public:
	typedef unsigned char Char;
	typedef Nfa<Char> NfaChar;
	typedef Alphabet<Char> AlphabetChar;
	typedef int Token;

	RegexParserCtrl(const std::string& regex);

	int parse();

	int shift(int);
	int error(size_t, const int*);
	void reduce(int);

	void lookahead();
	void alternative();
	void concat();
	void asterisk();
	void plus();
	void optional();
	void character(const char*);
	void group();
	void inverseGroup();
	void singleton(const char*);
	void interval(const char*, const char*);
	void concatSingleton(const char*);
	void concatInterval(const char*, const char*);
	const char* escChar(const char*);

	const NfaChar& getNfa() const;
	const NfaChar& getLookahead() const;

	const char* string();
	void release(const char*);

private:

	NfaChar popNfa();

	RegexParserCtrl& self() { return *this; }

	NfaChar lookaheadNfa;

	std::stack<NfaChar> nfas;
	AlphabetChar alphabet;

	const std::string& regexstr;
	RegexTokenizer fsm;

	Token nextToken;
	int start, end;

	RegexParser parser;
};

#endif
