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

#include "testparsergen.hpp"
#include "../src/lalrparsergen.hpp"

#include "../../genutils/src/codestream.hpp"

#include "../../lalrparser/src/lalrgram.hpp"

#include <boost/test/unit_test.hpp>

#include <fstream>
#include <algorithm>
#include <cctype>
#include <cerrno>
#include <direct.h>

class TestGen: public LalrParserGen<char>
{
public:
	TestGen(LalrGrammar<char> g, LalrParsingTable<char> table, ParserContext<char> context)
			: LalrParserGen<char>(g, table, context) {}

	void test_generateTokenStructures(const std::string& expected);
};

void TestGen::test_generateTokenStructures(const std::string& expected)
{
	std::stringstream teststr;
	CodeStream cs(teststr, "\t");
	this->generateTokenStructures(cs);
	BOOST_CHECK_EQUAL(teststr.str(), expected);
}

static void test_codegen()
{
	LalrGrammar<char> g;
	g.addProduction('A', "BC");
	g.addProduction('C', "BBD");
	g.addProduction('C', "BD");
	g.addProduction('C', "B");
	g.addProduction('C', "CD");

	LalrParsingTable<char> table = g.createParsingTable();
	ParserContext<char> context(table, std::map<char, BitSequence<char> >(), std::map<char, BitSequence<char> >());

	TestGen pg(g, table, context);
	std::vector<LalrParserGen<char>::ActionList> handler(5);
	LalrParserGen<char>::Action printAction("print");
	printAction.parameter.resize(3);
	printAction.parameter[0] = "B";
	printAction.parameter[1] = "CB";
	printAction.parameter[2] = "CCD";

	LalrParserGen<char>::Action fooAction("foo");

	fooAction.parameter.resize(2);
	fooAction.parameter[0] = "CAB";
	fooAction.parameter[1] = "AB";

	handler[0].push_back(printAction);
	handler[1].push_back(fooAction);

	pg.setReductionHandler(handler);

	std::stringstream expected;
	CodeStream cs(expected, "\t");

	cs << "struct TokA" << newline;
	cs << "{ " << indent; // << newline;
	cs << "struct "; // << newline;
	cs << "{ " << indent; // << newline;
	cs << "const char* str; "; // << newline;
	cs << revindent << "} B; "; // << newline;
	cs << revindent << "};" << newline;

	cs << "struct TokB" << newline;
	cs << "{ " << indent; // << newline;
	cs << "const char* str; "; // << newline;
	cs << revindent << "};" << newline;

	cs << "struct TokC" << newline;
	cs << "{ " << indent; // << newline;
	cs << "struct "; // << newline;
	cs << "{ " << indent; // << newline;
	cs << "struct "; // << newline;
	cs << "{ " << indent; // << newline;
	cs << "const char* str; "; // << newline;
	cs << revindent << "} B; "; // << newline;
	cs << revindent << "} A; "; // << newline;
	cs << "struct "; // << newline;
	cs << "{ " << indent; // << newline;
	cs << "const char* str; "; // << newline;
	cs << revindent << "} B; "; // << newline;
	cs << "struct "; // << newline;
	cs << "{ " << indent; // << newline;
	cs << "struct "; // << newline;
	cs << "{ " << indent; // << newline;
	cs << "const char* str; "; // << newline;
	cs << revindent << "} D; "; // << newline;
	cs << revindent << "} C; "; // << newline;
	cs << "struct "; // << newline;
	cs << "{ " << indent; // << newline;
	cs << "const char* str; "; // << newline;
	cs << revindent << "} D; "; // << newline;
	cs << revindent << "};" << newline;

	cs << "struct TokD" << newline;
	cs << "{ " << indent; // << newline;
	cs << "const char* str; "; // << newline;
	cs << revindent << "};" << newline;

	cs << "struct" << newline;
	cs << "{" << indent << newline;
	cs << "TokA *A;" << newline;
	cs << "TokB *B;" << newline;
	cs << "TokC *C;" << newline;
	cs << "TokD *D;" << newline;
	cs << revindent << "} curr;" << newline;

	cs << "union" << newline;
	cs << "{" << indent << newline;
	cs << "TokA A;" << newline;
	cs << "TokB B;" << newline;
	cs << "TokC C;" << newline;
	cs << "TokD D;" << newline;
	cs << revindent << "} parent;" << newline;

	//std::cout << expected.str() << std::endl;

	pg.test_generateTokenStructures(expected.str());
}

static void test_dependencies()
{
	//A := B C : print(B, C.B, C.C.D);
	//C := B B D;
	//C := B D;
	//C := B;
	//C := C D;

	LalrGrammar<char> g;
	g.addProduction('A', "BC");
	g.addProduction('C', "BBD");
	g.addProduction('C', "BD");
	g.addProduction('C', "B");
	g.addProduction('C', "CD");

	LalrParsingTable<char> table = g.createParsingTable();
	ParserContext<char> context(table, std::map<char, BitSequence<char> >(), std::map<char, BitSequence<char> >());
	LalrParserGen<char> pg(g, table, context);
	std::vector<LalrParserGen<char>::ActionList> handler(5);

	LalrParserGen<char>::Action printAction("print");
	printAction.parameter.resize(3);
	printAction.parameter[0] = "B";
	printAction.parameter[1] = "CB";
	printAction.parameter[2] = "CCD";
	handler[0].push_back(printAction);

	pg.setReductionHandler(handler);

	// check token dependencies
	BOOST_CHECK(!pg.dependsOn('A', 'A'));
	BOOST_CHECK(!pg.dependsOn('A', 'B'));
	BOOST_CHECK(!pg.dependsOn('A', 'C'));
	BOOST_CHECK(!pg.dependsOn('A', 'D'));

	BOOST_CHECK(!pg.dependsOn('B', 'A'));
	BOOST_CHECK(!pg.dependsOn('B', 'B'));
	BOOST_CHECK(!pg.dependsOn('B', 'C'));
	BOOST_CHECK(!pg.dependsOn('B', 'D'));

	BOOST_CHECK(!pg.dependsOn('C', 'A'));
	BOOST_CHECK( pg.dependsOn('C', 'B'));
	BOOST_CHECK( pg.dependsOn('C', 'C'));
	BOOST_CHECK( pg.dependsOn('C', 'D'));

	BOOST_CHECK(!pg.dependsOn('D', 'A'));
	BOOST_CHECK(!pg.dependsOn('D', 'B'));
	BOOST_CHECK(!pg.dependsOn('D', 'C'));
	BOOST_CHECK(!pg.dependsOn('D', 'D'));

	// check action dependencies
	for (size_t state = 0; state < table.size(); ++state)
	{
		const char tokens[] = {'A', 'B', 'C', 'D'};
		for (int t = 0; t < sizeof(tokens); ++t)
		{
			bool depends = false;
			const LalrGrammar<char>::Information& info = g.getInformation(state);
			for (LalrGrammar<char>::InfoItems::const_iterator i = info.items.begin(); i != info.items.end(); ++i)
				if (	i->first.productionNo < handler.size() && i->first.dot < g.getProductions()[i->first.productionNo].body().size()
					&&	g.getProductions()[i->first.productionNo].body()[i->first.dot] == tokens[t]
				)
					for (size_t a = 0; a < handler[i->first.productionNo].size(); ++a)
						for (size_t p = 0; p < handler[i->first.productionNo][a].parameter.size(); ++p)
							if (!handler[i->first.productionNo][a].parameter[p].empty() && handler[i->first.productionNo][a].parameter[p][0] == tokens[t])
								depends = true;

			BOOST_CHECK_MESSAGE(depends == pg.dependsOn(info, tokens[t]),
				"info for state " << state << " erroneously " << (depends ? "does not depend on " : "depends on ") << tokens[t]);
		}
	}
}

static void test_parser1()
{
	// create directory structure
	char cwd[512];
	std::cout << "writing files to " << _getcwd(cwd, sizeof(cwd)) << std::endl;
	BOOST_CHECK(0 == _mkdir("gen") || errno == EEXIST);
	BOOST_CHECK(0 == _mkdir("gen\\private") || errno == EEXIST);

	// "[^II](I|I*)[II]"
	LalrGrammar<char> g;
	g.addProduction('E', "E|D"); //0
	g.addProduction('E', "D"); //1
	g.addProduction('D', "DF"); //2
	g.addProduction('D', "F"); //3
	g.addProduction('F', "(E)"); //4
	g.addProduction('F', "F*"); //5
	g.addProduction('F', "I"); //6
	g.addProduction('F', "[II]"); //7
	g.addProduction('F', "[^II]"); //8

	LalrParsingTable<char> table = g.createParsingTable();
	
	std::fstream header("gen/parser.hpp", std::ios::out);
	BOOST_CHECK(header.good());
	std::fstream source("gen/private/parser.cpp", std::ios::out);
	BOOST_CHECK(source.good());

	CodeStream csHeader(header, "\t");
	CodeStream csSource(source, "\t");

	ParserContext<char> context(table, std::map<char, BitSequence<char> >(), std::map<char, BitSequence<char> >());
	LalrParserGen<char> pg(g, table, context);
	pg.setMediatorClassName("ParserCtrl");

	std::string upperClassName = pg.getMediatorClassName();
	int (*toupperptr)(int) = std::toupper;
	std::transform(upperClassName.begin(), upperClassName.end(), upperClassName.begin(), toupperptr);
	std::string lowerClassName = pg.getMediatorClassName();
	std::transform(lowerClassName.begin(), lowerClassName.end(), lowerClassName.begin(), toupperptr);

	csHeader << "#ifndef GEN_PARSER_HPP" << newline;
	csHeader << "#define GEN_PARSER_HPP" << newline;
	csHeader << newline;
	csHeader << "#include \"" << lowerClassName << ".hpp\"" << newline;
	csHeader << newline;
	pg.generateClassDef(csHeader);
	csHeader << "#endif" << newline;

	csSource << "#include \"../parser.hpp\"" << newline << newline;
	pg.generateStateHandlerImpls(csSource);
}

void test_lalrparsergen()
{
	test_codegen();
	test_parser1();
	test_dependencies();
}

