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
