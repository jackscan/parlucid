#include "../cmdlineparserctrl.hpp"
#include "../../gen/cmdlinetokens.hpp"

#include <string>
#include <cstring>

using namespace CmdLineTokens;

CmdLineParserCtrl::CmdLineParserCtrl(int argc, char* argv[], lua_State *luastate)
: argc(argc), argv(argv), lua(luastate), tokbegin(0), pos(1, 0), luaOutput(0)
{
}

CmdLineParserCtrl::~CmdLineParserCtrl()
{
	for (size_t i = 0; i < tokens.size(); ++i)
		delete[] tokens[i];
}

void CmdLineParserCtrl::assign(const char* var, const char* val)
{
	//printf("%s = %s\n", var, val);
	const char* ival = val;
	int count = 0;
	for (; *ival; ++ival) if (*ival == '\\') ++count;
	int varlen = strlen(var);
	int dstlen = varlen + 1 + (ival - val) + count;
	char* dst = new char[dstlen + 1];
	strncpy(dst, var, varlen)[varlen] = '=';
	const char* iSrc = val;
	for (char* iDst = &dst[varlen+1]; iDst <= dst + dstlen && (*iDst = *iSrc); ++iSrc, ++iDst)
		if (*iDst == '\\') *(++iDst) = '\\';

	//printf("varlen %i, dstlen %i\n", varlen, dstlen);
	
	if (luaL_dostring(lua, dst))
	{
		const char* error = lua_tostring(lua, -1);
		fprintf(stderr, "error executing string: %s\n", error);
	}
	else
	{
		printf("%s = %s\n", var, val);
	}
}

void CmdLineParserCtrl::file(const char* filename)
{
	files.push_back(filename);
	printf("file: %s\n", filename);
}

void CmdLineParserCtrl::grammar(const char* filename)
{
	grammarFiles.push_back(filename);
	printf("grammar: %s\n", filename);
}

void CmdLineParserCtrl::loutput(const char* filename)
{
	luaOutput = filename;
	printf("lua output: %s\n", filename);
}

const char* CmdLineParserCtrl::stringize(const char* path)
{
	size_t len = strlen(path);
	char* str = new char[len + 3];
	str[0] = '"';
	str[len+1] = '"';
	str[len+2] = '\0';
	strncpy(&str[1], path, len);
	tokens.push_back(str);
	return str;
}

int CmdLineParserCtrl::shift(int context)
{
	int result = static_cast<int>(TOK_END);
	if (pos.first < argc && argv[pos.first][pos.second] == '\0')
	{
		++pos.first;
		pos.second = 0;
	}

	tokbegin= pos.second;
	
	if (pos.first < argc)
	{
		const char* arg = argv[pos.first];
		int len = static_cast<int>(strlen(arg));
		int state = context;
		int i = pos.second;
		int tok = TOK_NUMOFTOKS;
		while(state != CmdLineTokenizer::ERROR_STATE && i < len)
		{
			i += fsm.process(state, &arg[i], len - i, tok);
			if (tok != TOK_NUMOFTOKS)
			{
				result = tok;
				pos.second = i;
				tok = TOK_NUMOFTOKS;
			}
		}
	}

	return result;
}

int CmdLineParserCtrl::error(int, const int*)
{
	fprintf(stderr, "error at %s\n", &argv[pos.first][tokbegin]);
	return 1;
}

void CmdLineParserCtrl::reduce(int)
{
}

const char* CmdLineParserCtrl::string()
{
	char* str = 0;
	if (pos.first < argc)
	{
		int len = pos.second - tokbegin;
		str = new char[len + 1];
		str[len] = '\0';
		strncpy(str, &argv[pos.first][tokbegin], len);
		tokens.push_back(str);
	}
	//printf("token: %s\n", str);
	return str;
}

void CmdLineParserCtrl::release(const char* str)
{
}
