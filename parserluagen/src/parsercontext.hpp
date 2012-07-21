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

#ifndef PARSERCONTEXT_HPP
#define PARSERCONTEXT_HPP

#include "../../lalrparser/src/lalrparsingtable.hpp"
#include "../../lexer/src/fsm.hpp"

#include <map>

template<typename T>
class ParserContext
{
public:
	typedef T Token;
	typedef LalrParsingTable<T> Table;
	typedef size_t ContextId;

	ParserContext(const Table& parsingTable, const std::map<Token, BitSequence<Token> >& prefixes, const std::map<Token, BitSequence<Token> >& subTokens);

	ContextId getParsingTableContext(size_t i) const;
	const BitSequence<Token>& getAcceptedTokens(ContextId context) const;
	size_t getNumberOfContexts() const;

private:

	std::vector<ContextId> parsingTableContexts;
	std::vector<BitSequence<Token> > acceptedTokens;
};

#endif
