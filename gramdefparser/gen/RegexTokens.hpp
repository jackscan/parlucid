#ifndef REGEXTOKENS_HPP
#define REGEXTOKENS_HPP

namespace RegexTokens
{

enum
{
	TOK_END,
	TOK_C,
	TOK_E,
	TOK_S,
	TOK_4,
	TOK_5,
	TOK_D,
	TOK_F,
	TOK_8,
	TOK_9,
	TOK_10,
	TOK_11,
	TOK_12,
	TOK_I,
	TOK_14,
	TOK_G,
	TOK_16,
	TOK_17,
	TOK_J,
	TOK_A,
	TOK_20,
	NUMOFTOKS
};

const char* getTokenName(int tok);

}

#endif
