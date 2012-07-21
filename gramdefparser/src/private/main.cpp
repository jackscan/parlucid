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

#include "../grammarparserctrl.hpp"
#include "../../../lalrparser/src/lalrgram.hpp"
#include "../../../lexer/src/fsm.hpp"

#include <cstdlib>
#include <sstream>

typedef int Token;

FILE* openFile(const char* filename, const char* mode)
{
	FILE * file = 0;
	if (!(file = fopen(filename, mode)))
		fprintf(stderr, "failed to open '%s': %s\n", filename, strerror(errno));
	return file;
}

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

int main(int argc, char *argv[])
{
	int result = 0;

	GrammarParserCtrl parserCtrl;

	for (int i = 1; i < argc; ++i)
	{
		FILE *infile = openFile(argv[i], "rb");
		if (infile)
		{
			std::string filename(argv[i]);
			parserCtrl.parse(infile, filename);
		}
		else result = 1;
	}


	std::map<std::string, GrammarParserCtrl::AnnotatedGrammar> grammars = parserCtrl.getGrammars();

	for (std::map<std::string, GrammarParserCtrl::AnnotatedGrammar>::iterator igram = grammars.begin(); igram != grammars.end(); ++igram)
	{
		Fsm<char> fsm(Nfa<char>::combine(igram->second.nfas));
		fsm.reduce();
		igram->second.grammar.createParsingTable();

		// check for errors
		{
			if (igram->second.grammar.getAmbiguousProductions().size() > 0)
			{
				fprintf(stderr, "ambiguous productions!\n");
			}

			if (igram->second.grammar.getErrors().size() > 0)
			{
				fprintf(stderr, "errors in grammar %s\n", igram->first.c_str());
				for (size_t i = 0; i < igram->second.grammar.getErrors().size(); ++i)
				{
					const LalrGrammar<int>::Error& error = igram->second.grammar.getErrors()[i];

					fprintf(stderr, "conflict at token %s in state %u\n",
							igram->second.tokenNames[error.token].c_str(),
							static_cast<unsigned>(error.state));

					printState(igram->second.grammar.getInformation(error.state),
							igram->second.grammar.getProductions(),
							igram->second.tokenNames);
				}
			}
		}

		const std::vector<Grammar<Token>::Production>& productions = igram->second.grammar.getProductions();
		for (size_t i = 0; i < productions.size(); ++i)
		{
			const Grammar<Token>::Production& production = productions[i];
			printf("%u: %s ->", static_cast<unsigned>(i), getTokenName(igram->second.tokenNames, production.head()).c_str());
			for (size_t b = 0; b < production.body().length(); ++b)
				printf(" %s", getTokenName(igram->second.tokenNames, production.body()[b]).c_str());
			if (i < igram->second.actions.size())
				printActionList(igram->second.actions[i], igram->second.tokenNames);
			printf("\n");
		}
	}

	//fprintf(stdout, "\n");

	return result;
}


