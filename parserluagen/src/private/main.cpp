#include "../cmdlineparserctrl.hpp"
#include "../parsercontext.hpp"
#include "../luafsm.hpp"
#include "../fsmgen.hpp"
#include "../parsergen.hpp"
#include "../../gen/cmdlineparser.hpp"
#include "../../../gramdefparser/src/GrammarParserCtrl.hpp"
#include "../../../gramdefparser/gen/GrammarTokens.hpp"
#include "../../../luatemplateparser/src/LuaTemplateParserCtrl.hpp"
#include "../../../luatemplateparser/src/LuaGenFunctions.hpp"
#include "../../../parserlog/src/LogGen.hpp"

#include <lua.hpp>

#include <cstdlib>
#include <cstring>
#include <cstdio>

FILE* openFile(const char* filename, const char* mode)
{
	FILE * file = 0;
	if (!(file = fopen(filename, mode)))
		perror(filename);
		//fprintf(stderr, "failed to open '%s': %s\n", filename, strerror(errno));
	return file;
}

typedef int Token;
typedef unsigned char Char;

std::string getTokenName(const std::map<Token, std::string>& tokenNames, Token token)
{
	std::map<Token, std::string>::const_iterator iter = tokenNames.find(token);
	if (iter != tokenNames.end()) return iter->second;
	else
	{
		std::stringstream str;
		str << token;
		return str.str();
	}
}

void printState(const LalrGrammar<Token>::Information& info,
		const LalrGrammar<Token>::Productions productions,
		const std::map<Token, std::string>& tokenNames)
{
	for (LalrGrammar<Token>::InfoItems::const_iterator iItem = info.items.begin(); iItem != info.items.end(); ++iItem)
	{
		const LalrGrammar<Token>::Production& prod = productions[iItem->first.productionNo];
		fprintf(stderr, "\t[%s ->", getTokenName(tokenNames, prod.head()).c_str());
		for (size_t i = 0; i < prod.body().size(); ++i)
			fprintf(stderr, (iItem->first.dot == i) ? ".%s" : " %s", getTokenName(tokenNames, prod.body()[i]).c_str());
		if (iItem->first.dot == prod.body().size()) fprintf(stderr, ".");
		fprintf(stderr, "], (");
		for (std::set<Token>::const_iterator iFollow = iItem->second.begin(); iFollow != iItem->second.end(); ++iFollow)
			fprintf(stderr, iFollow == iItem->second.begin() ? "%s" : ", %s", getTokenName(tokenNames, *iFollow).c_str());
		fprintf(stderr, ")\n");
	}
}

void printParameter(const std::basic_string<Token>& parameter, const std::map<Token, std::string>& tokenNames)
{
	for (size_t i = 0; i < parameter.size(); ++i)
		fprintf(stdout, i == 0 ? "%s" : ".%s", getTokenName(tokenNames, parameter[i]).c_str());
}

void printAction(const GrammarParserCtrl::Action& action, const std::map<Token, std::string>& tokenNames)
{
	if (!action.lparameter.empty())
	{
		printParameter(action.lparameter, tokenNames);
		fprintf(stdout, " = ");
	}

	if (!action.function.empty())
	{
		fprintf(stdout, "%s(", action.function.c_str());
		for (size_t i = 0; i < action.parameter.size(); ++i)
		{
			if (i > 0) fprintf(stdout, ", ");
			printParameter(action.parameter[i], tokenNames);
		}

		fprintf(stdout, ")");
	}
	else if(!action.parameter.empty())
		printParameter(action.parameter[0], tokenNames);
}

void printActionList(const GrammarParserCtrl::ActionList& actionList, const std::map<Token, std::string>& tokenNames)
{
	for (size_t i = 0; i < actionList.size(); ++i)
	{
		fprintf(stdout, i == 0 ? " : " : ", ");
		printAction(actionList[i], tokenNames);
	}
}

int errorHandler(lua_State* L)
{
	if (!lua_isstring(L, 1)) /* 'message' not a string? */
		return 1; /* keep it intact */
	lua_getfield(L, LUA_GLOBALSINDEX, "debug");
	if (!lua_istable(L, -1))
	{
		lua_pop(L, 1);
		return 1;
	}
	lua_getfield(L, -1, "traceback");
	if (!lua_isfunction(L, -1))
	{
		lua_pop(L, 2);
		return 1;
	}
	lua_pushvalue(L, 1); /* pass error message */
	lua_pushinteger(L, 2); /* skip this function and traceback */
	lua_call(L, 2, 1); /* call debug.traceback */
	return 1;
}

void hook(lua_State *L, lua_Debug *ar)
{

}

typedef int Token;

void createLuaTokenNames(lua_State* lua, const std::map<Token, std::string>& tokenNames)
{
	lua_newtable(lua);
	for (std::map<Token, std::string>::const_iterator iName = tokenNames.begin(); iName != tokenNames.end(); ++iName)
	{
		lua_pushinteger(lua, (lua_Integer)iName->first);
		lua_pushstring(lua, iName->second.c_str());
		lua_settable(lua, -3);
	}
}

int main(int argc, char *argv[])
{
	int result = 0;

	lua_State * lua = luaL_newstate();
	if (!lua)
	{
		fprintf(stderr, "could not create lua state\n");
		return -2;
	}

	luaL_openlibs(lua);

	{
		luagen::LuaOutput luaOutput;
		luagen::registerFunctions(lua, luaOutput);

		CmdLineParserCtrl cmdLineCtrl(argc, argv, lua);
		CmdLineParser cmdLineParser(cmdLineCtrl);
		cmdLineParser.parse();

		FILE * luaOutFile = 0;

		bool luaerror = false;

		if (cmdLineCtrl.getLuaOutput())
			luaOutFile = openFile(cmdLineCtrl.getLuaOutput(), "w");

		for (size_t f = 0; f < cmdLineCtrl.getFiles().size(); ++f)
		{
			LuaTemplateParserCtrl luaParserCtrl;
			const char* luastr = luaParserCtrl.parse(cmdLineCtrl.getFiles()[f]);
			
			if (luaOutFile)
			{
				fprintf(luaOutFile, "--%s:\n%s\n", cmdLineCtrl.getFiles()[f], luastr);
			}
			else if (luaL_dostring(lua, luastr))
			{
				const char* error = lua_tostring(lua, -1);
				fprintf(stderr, "error executing string: %s\n", error);
				//fprintf(stderr, "%s:\n%s\n", cmdLineCtrl.getFiles()[f], luastr);
				luaerror = true;
			}
		}

		if (luaOutFile)
		{
			fclose(luaOutFile);
			if (luaL_dofile(lua, cmdLineCtrl.getLuaOutput()))
			{
				const char* error = lua_tostring(lua, -1);
				fprintf(stderr, "error executing string: %s\n", error);
				//fprintf(stderr, "%s:\n%s\n", cmdLineCtrl.getFiles()[f], luastr);
				luaerror = true;
			}
		}

		if (!luaerror)
		{
			lua_pushcfunction(lua, errorHandler);
			int errorFuncIndex = lua_gettop(lua);

			GrammarParserCtrl parserCtrl;

			for (size_t i = 0; i < cmdLineCtrl.getGrammarFiles().size(); ++i)
			{
				FILE *infile = openFile(cmdLineCtrl.getGrammarFiles()[i], "rb");
				if (infile)
				{
					parserCtrl.parse(infile, cmdLineCtrl.getGrammarFiles()[i]);
                    if (!feof(infile))
				    {
					    fprintf(stderr, "%s not read completly\n", cmdLineCtrl.getGrammarFiles()[i]);
					    result = 1;
				    }

					lua_getfield(lua, LUA_GLOBALSINDEX, "log");
					if (lua_isfunction(lua, -1))
					{
						lua_pushstring(lua, cmdLineCtrl.getGrammarFiles()[i]);
						createLuaParserLog(lua, parserCtrl);
						createLuaTokenNames(lua, GrammarTokens::NUMOFTOKS, &GrammarTokens::getTokenName);

						if (lua_pcall(lua, 3, 0, errorFuncIndex))
						{
							fflush(stdout);
							const char* error = lua_tostring(lua, -1);
							fprintf(stderr, "%s\n", error);
							fflush(stderr);
							result = 1;
						}
					}
					else
					{
						lua_pop(lua, 1);
					}
				}
			}

			lua_sethook(lua, hook, LUA_MASKLINE, 0);

			std::map<std::string, GrammarParserCtrl::AnnotatedGrammar> grammars = parserCtrl.getGrammars();

			for (std::map<std::string, GrammarParserCtrl::AnnotatedGrammar>::iterator igram = grammars.begin(); igram != grammars.end(); ++igram)
			{
				Fsm<Char> fsm(Nfa<Char>::combine(igram->second.nfas));
				fsm.reduce();
				LalrParsingTable<Token> table = igram->second.grammar.createParsingTable();

				// check for errors
				{
					if (igram->second.grammar.getAmbiguousProductions().size() > 0)
					{
						std::cerr << "ambiguous productions!" << std::endl;
					}

					if (igram->second.grammar.getErrors().size() > 0)
					{
						std::cerr << "errors in grammar" << std::endl;
						for (size_t i = 0; i < igram->second.grammar.getErrors().size(); ++i)
						{
							const LalrGrammar<int>::Error& error = igram->second.grammar.getErrors()[i];
							std::cerr << "conflict at token `" << igram->second.tokenNames[error.token] << "'" << std::endl;
							std::cerr << "	in state " << error.state << std::endl;

                     printState(igram->second.grammar.getInformation(error.state),
							   igram->second.grammar.getProductions(),
							   igram->second.tokenNames);
						}
					}

					for (size_t u = 0; u < igram->second.unresolvedParameters.size(); ++u)
					{
						const std::basic_string<int>& parameter = igram->second.unresolvedParameters[u];
						std::cerr << "could not resolve parameter: ";
						for (size_t i = 0; i < parameter.size(); ++i)
						{
							if (i > 0) std::cerr << ".";
							std::cerr << igram->second.tokenNames[parameter[i]];
						}
						std::cerr << std::endl;
					}
				}


				std::map<Token, BitSequence<Token> > prefixes;
				std::map<Token, BitSequence<Token> > subTokens;
				fsm.getPrefixAssociations(prefixes);
				ParserContext<Token> context(table, prefixes, subTokens);

				lua_getfield(lua, LUA_GLOBALSINDEX, "main");
				lua_pushstring(lua, igram->first.c_str());
				createLuaFsm(lua, fsm);
				createLuaContext(lua, context);
				//LalrParserGen<int> parserGen(igram->second.grammar, table, context);
				//createLuaParserGen(lua, luaOutput, parserGen);
				createLuaParsingTable(lua, table, context, igram->second.grammar, igram->second.actions);
				createLuaTokenNames(lua, igram->second.tokenNames);
				if (lua_pcall(lua, 5, 0, errorFuncIndex))
				{
					fflush(stdout);
					const char* error = lua_tostring(lua, -1);
					fprintf(stderr, "%s\n", error);
					fflush(stderr);
					result = 1;
				}
			}
		}
		else result = 1;
	}

	lua_close(lua);

	return result;
}


