#include "../LuaTemplateTokens.hpp"

namespace LuaTemplateTokens
{

static const char* sTokenNames[] =
{
	"@",
	"spaces",
	"name",
	"nl",
	"eof",
	"textintro",
	"code",
	"quote",
	"chunk",
	"snippet",
	"filename",
	"template",
	"codeline",
	"codeindent",
	"term",
	"eol",
	"text",
	"\"%}\"",
	"\"$\"",
	"var",
	"chunks",
	"embedtextbegin",
	"embedtext",
	"textlines",
	"lastline",
	"embedif",
	"elsechunk",
	"\"@;\"",
	"condembed",
	"\"@{\"",
	"\"}?\"",
	"\"{\"",
	"\"}\"",
	"call",
	"embed",
	"embedelse",
	"\"@:\"",
	"\"@\\\"\"",
	"includefile",
	"include",
	"textbegin",
	"textline",
	"textindent",
	"realchunk",
	"weakchunk",
	"textchunk",
};

const char* getTokenName(int tok)
{
	if (0 <= tok && (unsigned)tok < sizeof(sTokenNames) / sizeof(const char*))
		return sTokenNames[tok];
	else return "invalid token";
}

}

