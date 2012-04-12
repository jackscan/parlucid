#ifndef TESTTOKENS_HPP
#define TESTTOKENS_HPP

namespace TestTokens
{

enum
{
	TOK_END,
	TOK_lines,
	TOK_S,
	TOK_line,
	TOK_NUMOFTOKS = TOK_line
};

const char* getTokenName(int tok);

}

#endif
