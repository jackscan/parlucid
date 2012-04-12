#include "../GrammarTokens.hpp"

namespace GrammarTokens
{

static const char* sTokenNames[] =
{
	"@",
	"ws",
	"nl",
	"string",
	"regex",
	"identifier",
	"parser",
	"grammar",
	"\":=\"",
	"\"{\"",
	"definition",
	"\"}\"",
	"\";\"",
	"\"[:\"",
	"\":]\"",
	"production",
	"body",
	"action",
	"no",
	"\"@\"",
	"tokendef",
	"\"=\"",
	"\"|\"",
	"bodyexpr",
	"bodyterm",
	"id",
	"term",
	"\"[\"",
	"\"]\"",
	"\"(\"",
	"\")\"",
	"\"$\"",
	"\":\"",
	"commandlist",
	"command",
	"\",\"",
	"list",
	"lparameter",
	"parameter",
	"\".\"",
};

const char* getTokenName(int tok)
{
	if (0 <= tok && (unsigned)tok < sizeof(sTokenNames) / sizeof(const char*))
		return sTokenNames[tok];
	else return "invalid token";
}

}

