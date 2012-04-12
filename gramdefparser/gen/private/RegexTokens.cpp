#include "../RegexTokens.hpp"

namespace RegexTokens
{

static const char* sTokenNames[] =
{
	"@",
	"C",
	"E",
	"S",
	"\"-\"",
	"\"|\"",
	"D",
	"F",
	"\"(\"",
	"\")\"",
	"\"*\"",
	"\"+\"",
	"\"?\"",
	"I",
	"\"[\"",
	"G",
	"\"]\"",
	"\"^\"",
	"J",
	"A",
	"\"\\\\\"",
};

const char* getTokenName(int tok)
{
	if (0 <= tok && (unsigned)tok < sizeof(sTokenNames) / sizeof(const char*))
		return sTokenNames[tok];
	else return "invalid token";
}

}

