#ifndef ERRORGEN_HPP
#define ERRORGEN_HPP

#include "../../genutils/src/codestream.hpp"
#include "../../lalrparser/src/lalrparsingtable.hpp"

template<typename T>
class ErrorGen
{
public:
	typedef T Token;
	void printAction(CodeStream& os, const typename LalrParsingTable<Token>::Action& action)	
	void printState(CodeStream& stream, size_t state)
	void printToken(CodeStream& stream, Token token)
	void printTokenString(CodeStream& stream, const std::basic_string<Token>& tokenStr)
};

#endif
