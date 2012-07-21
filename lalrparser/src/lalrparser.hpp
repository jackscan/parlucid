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

#ifndef LALRPARSER_HPP
#define LALRPARSER_HPP

#include "gram.hpp"
#include "lalrparsingtable.hpp"
#include <deque>
#include <vector>

template<typename T, T N> class LalrParser
{
public:
	typedef T Token;
	typedef typename Grammar<T>::String String;
	typedef LalrParsingTable<T> Table;
	typedef typename Grammar<T>::Production Production;

	enum{ PARSE_ERR = -1, SHIFT = -2, AMBIGUOUS = -3 };

	struct StackElement : protected std::pair<Token,size_t> {
		StackElement() {}
		StackElement(const Token& symbol, size_t state): std::pair<Token,size_t>(std::make_pair(symbol,state)) {}
		Token& symbol() { return this->first; }
		size_t& state() { return this->second; }
		const Token& symbol() const { return this->first; }
		const size_t& state() const { return this->second; }
	};

	LalrParser(const Table&);

	bool setSpecialTokens(const std::set<Token>&);

	void reset();
	int next(const Token&);
	Token accepted() const;
	Token special() const;

	size_t getStackSize() const;
	const StackElement& getStackElement(size_t i) const { return stack[i]; }

	const Production& getProduction(size_t);

	bool isValid(Token) const;
	std::set<Token> validTokens() const;

protected:

	struct Stack : public std::deque<StackElement>
	{
		typename std::deque<StackElement>::reference top() { return this->back(); }
		typename std::deque<StackElement>::const_reference top() const { return this->back(); }
		void push(const typename std::deque<StackElement>::value_type& val) { this->push_back(val); }
	};

	const Table table;
	std::vector<Token> specialTokens;
	Stack stack;
	Token acceptedSymbol;
};

#endif
