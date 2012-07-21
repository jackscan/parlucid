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
#include "../src/fsmgen.hpp"
#include "../../lexer/src/fsm.hpp"
#include "../../lalrparser/src/lalrparsingtable.hpp"
#include "../../genutils/src/codestream.hpp"

#include <limits>
#include <sstream>
#include <iostream>

#include <boost/test/unit_test.hpp>

static ParserContext<int> createDummyParserContext();
static void test_data(int, std::istream& is);

void test_fsmgen()
{
	// linear sequence handler
	{
		Nfa<char> nfa = Nfa<char>("first")|((Nfa<char>("A")|Nfa<char>("B")) + *Nfa<char>("C") + Nfa<char>("Dfirst"));
		nfa.set_output(0);
		Fsm<char> fsm(nfa);
		fsm.reduce();

		FsmGen<char>::Config config;

		fsm.groupLinearPaths(1, config.minLinearSequenceLength);

		CodeStream cs(std::cout, "   ");
		FsmGen<char> fsmgen(fsm, createDummyParserContext(), config);
		fsmgen.generateClassDef(cs);
		fsmgen.generateStateHandlerArrayInit(cs);
		fsmgen.generateStateHandlerImpls(cs);
		fsmgen.generateProcessMethod(cs);
		
	}

	return;

	// test inverse
	/*{
		Nfa<char> nfa_a(Alphabet<char>('a'));
		Nfa<char> nfa_b(Alphabet<char>('b'));

		Nfa<char> nfa_inv = ~(nfa_a + nfa_b);
		nfa_inv.set_output(1);
		Fsm<char> fsm(nfa_inv);
		fsm.reduce();

		CodeStream cs(std::cout, "\t");
		FsmGen<char> fsmgen(fsm);
		fsmgen.generateStateHandlerImpls(cs);
	}*/

	// test nfa and inverse combined
	/*{
		Nfa<char> nfa_a(Alphabet<char>('a'));
		Nfa<char> nfa_b(Alphabet<char>('b'));
		Nfa<char> nfa_ab = nfa_a + nfa_b;
		Nfa<char> nfa_inv = ~nfa_ab;

		nfa_ab.set_output(0);
		nfa_inv.set_output(1);

		std::list<Nfa<char> > nfas;
		nfas.push_back(nfa_ab);
		nfas.push_back(nfa_inv);

		Nfa<char> nfa_com = Nfa<char>::combine(nfas);
		Fsm<char> fsm(nfa_com);
		fsm.reduce();

		CodeStream cs(std::cout, "\t");
		FsmGen<char> fsmgen(fsm);
		fsmgen.generateStateHandlerImpls(cs);
	}*/

	// test 2 nfas and inverse combined
	/*{
		Nfa<char> nfa_a(Alphabet<char>('a'));
		Nfa<char> nfa_b(Alphabet<char>('b'));
		Nfa<char> nfa_ab = nfa_a + nfa_b;
		Nfa<char> nfa_ba = nfa_b + nfa_a;
		Nfa<char> nfa_inv = ~(nfa_ab | nfa_ba);

		nfa_ab.set_output(1);
		nfa_ba.set_output(2);
		nfa_inv.set_output(3);

		std::list<Nfa<char> > nfas;
		nfas.push_back(nfa_ab);
		nfas.push_back(nfa_ba);
		nfas.push_back(nfa_inv);

		Nfa<char> nfa_com = Nfa<char>::combine(nfas);
		Fsm<char> fsm(nfa_com);
		fsm.reduce();

		CodeStream cs(std::cout, "\t");
		FsmGen<char> fsmgen(fsm);
		fsmgen.generateStateHandlerImpls(cs);
	}*/
	// test char fsm
	{
		Nfa<char> nfa_0(Alphabet<char>('0'));
		Nfa<char> nfa_1(Alphabet<char>('1'));
		Nfa<char> nfa_2(Alphabet<char>('2'));
		Nfa<char> nfa_a(Alphabet<char>('a'));
		Nfa<char> nfa_b(Alphabet<char>('b'));
		Nfa<char> nfa_c(Alphabet<char>('c'));
		Nfa<char> nfa_s(Alphabet<char>('s'));
		Nfa<char> nfa_f(Alphabet<char>('f'));
		Nfa<char> nfa_dot(Alphabet<char>('.'));
		Alphabet<char> a(Interval<char>(std::numeric_limits<char>::min(), std::numeric_limits<char>::max()));
		a.del('0', '9');
		Nfa<char> nfa_09(a);
		Nfa<char> nfa0 = nfa_b+nfa_c+nfa_a|nfa_b+*nfa_c;
		nfa0.set_output(0);
		Nfa<char> nfa1 = *(nfa_0|nfa_1|nfa_2)+nfa_dot+(nfa_s+nfa_f);
		nfa1.set_output(1);
		nfa_09.set_output(2);
		Nfa<char> nfa_all = nfa0|nfa1|nfa_09;

		Fsm<char> fsm(nfa_all);
		fsm.reduce();

		std::stringstream outstr;
		CodeStream cs(outstr, "\t");

		FsmGen<char> fsmgen(fsm, createDummyParserContext());
		fsmgen.generateClassDef(cs);
		fsmgen.generateProcessMethod(cs);
		fsmgen.generateStateHandlerArrayInit(cs);
		fsmgen.generateStateHandlerImpls(cs);

		//std::cout << outstr.str();
		//todo:
		test_data(0, outstr);
		
		//fsmgen.generateFinalMethod();
	}

	// test int fsm
	{
		Nfa<int> nfa_0(Alphabet<int>(0));
		Nfa<int> nfa_1(Alphabet<int>(1));
		Nfa<int> nfa_2(Alphabet<int>(2));
		Nfa<int> nfa_10(Alphabet<int>(10));
		Nfa<int> nfa_11(Alphabet<int>(11));
		Nfa<int> nfa_12(Alphabet<int>(12));
		Nfa<int> nfa_20(Alphabet<int>(20));
		Nfa<int> nfa_25(Alphabet<int>(25));
		Nfa<int> nfa_30(Alphabet<int>(30));
		Alphabet<int> a(Interval<int>(std::numeric_limits<int>::min(), std::numeric_limits<int>::max()));
		a.del(0, 8);
		Nfa<int> nfa_08(a);
		Nfa<int> nfa0 = nfa_10+nfa_11+nfa_10|nfa_11+*nfa_12;
		nfa0.set_output(0);
		Nfa<int> nfa1 = *(nfa_0|nfa_1|nfa_2)+nfa_30+(nfa_20+nfa_25);
		nfa1.set_output(1);
		nfa_08.set_output(2);
		Nfa<int> nfa_all = nfa0|nfa1|nfa_08;

		Fsm<int> fsm(nfa_all);
		fsm.reduce();

		std::stringstream outstr;
		CodeStream cs(outstr, "   ");

		FsmGen<int> fsmgen(fsm, createDummyParserContext());
		fsmgen.generateClassDef(cs);
		fsmgen.generateStateHandlerArrayInit(cs);
		fsmgen.generateStateHandlerImpls(cs);
		fsmgen.generateProcessMethod(cs);
		//fsmgen.generateFinalMethod(cs);

		//std::cout << outstr.str();
		test_data(1, outstr);
	}
}

ParserContext<int> createDummyParserContext()
{
	LalrParsingTable<int>::Productions prods;
	LalrParsingTable<int> table(prods, 0);
	std::map<int, BitSequence<int> > prefixes;
	return ParserContext<int>(table, prefixes, std::map<int, BitSequence<int> >());
}

static const std::string test_data_array[] = {
"class Fsm\n"
"{\n"
"public:\n"
"\ttypedef int State;\n"
"\tconst static State ERROR_STATE;\n"
"\n"
"\tenum Token\n"
"\t{\n"
"\t\tTOK_0 = 0,\n"
"\t\tTOK_1 = 1,\n"
"\t\tTOK_2 = 2,\n"
"\t};\n"
"\n"
"\tstatic State process(State state, char sym, int& tok);\n"
"private:\n"
"\ttypedef State (*PfnStateHandler)(char, int&);\n"
"\tstatic const PfnStateHandler stateHandler[8];\n"
"};\n"
"inline Fsm::State Fsm::process(State state, char sym, int& tok)\n"
"{\n"
"\tif (static_cast<unsigned>(state) < sizeof(stateHandler) / sizeof(*stateHandler))\n"
"\t\treturn stateHandler[state](sym, tok);\n"
"\telse return ERROR_STATE;\n"
"}\n"
"const Fsm::State Fsm::ERROR_STATE = -1;\n"
"\n"
"static Fsm::State handleState0(char, int&);\n"
"static Fsm::State handleState2(char, int&);\n"
"static Fsm::State handleState3(char, int&);\n"
"static Fsm::State handleState4(char, int&);\n"
"static Fsm::State handleState5(char, int&);\n"
"static Fsm::State handleState6(char, int&);\n"
"static Fsm::State handleState7(char, int&);\n"
"static Fsm::State handleDeadEnd(char, int&);\n"
"\n"
"const Fsm::PfnStateHandler Fsm::stateHandler[8] = {\n"
"\thandleState0,\n"
"\thandleDeadEnd,\n"
"\thandleState2,\n"
"\thandleState3,\n"
"\thandleState4,\n"
"\thandleState5,\n"
"\thandleState6,\n"
"\thandleState7,\n"
"};\n"
"Fsm::State handleState0(char sym, int& tok)\n"
"{\n"
"\tswitch(sym)\n"
"\t{\n"
"\tcase '.': return (tok = TOK_2, 2);\n"
"\tcase '/': return (tok = TOK_2, 1);\n"
"\tcase 'b': return (tok = TOK_0, 4);\n"
"\tdefault: switch(sym >> 1) // ['\\x80', '\\x7f']\n"
"\t\t{\n"
"\t\tcase 24: return (3);\n"
"\t\tcase 25: return sym < '3' ? (3) : ERROR_STATE;\n"
"\t\tcase 26: return ERROR_STATE;\n"
"\t\tcase 27: return ERROR_STATE;\n"
"\t\tcase 28: return ERROR_STATE;\n"
"\t\tdefault: return (tok = TOK_2, 1);\n"
"\t\t}\n"
"\t}\n"
"}\n"
"\n"
"Fsm::State handleState2(char sym, int& tok)\n"
"{\n"
"\tswitch(sym)\n"
"\t{\n"
"\tcase 's': return (5);\n"
"\tdefault: return ERROR_STATE;\n"
"\t}\n"
"}\n"
"\n"
"Fsm::State handleState3(char sym, int& tok)\n"
"{\n"
"\tswitch(sym)\n"
"\t{\n"
"\tcase '.': return (2);\n"
"\tdefault: switch(sym >> 1) // ['\\x80', '\\x7f']\n"
"\t\t{\n"
"\t\tcase 24: return (3);\n"
"\t\tcase 25: return sym < '3' ? (3) : ERROR_STATE;\n"
"\t\tdefault: return ERROR_STATE;\n"
"\t\t}\n"
"\t}\n"
"}\n"
"\n"
"Fsm::State handleState4(char sym, int& tok)\n"
"{\n"
"\tswitch(sym)\n"
"\t{\n"
"\tcase 'c': return (tok = TOK_0, 6);\n"
"\tdefault: return ERROR_STATE;\n"
"\t}\n"
"}\n"
"\n"
"Fsm::State handleState5(char sym, int& tok)\n"
"{\n"
"\tswitch(sym)\n"
"\t{\n"
"\tcase 'f': return (tok = TOK_1, 1);\n"
"\tdefault: return ERROR_STATE;\n"
"\t}\n"
"}\n"
"\n"
"Fsm::State handleState6(char sym, int& tok)\n"
"{\n"
"\tswitch(sym)\n"
"\t{\n"
"\tcase 'a': return (tok = TOK_0, 1);\n"
"\tcase 'c': return (tok = TOK_0, 7);\n"
"\tdefault: return ERROR_STATE;\n"
"\t}\n"
"}\n"
"\n"
"Fsm::State handleState7(char sym, int& tok)\n"
"{\n"
"\tswitch(sym)\n"
"\t{\n"
"\tcase 'c': return (tok = TOK_0, 7);\n"
"\tdefault: return ERROR_STATE;\n"
"\t}\n"
"}\n"
"\n"
"Fsm::State handleDeadEnd(char, int&)\n"
"{\n"
"\treturn ERROR_STATE;\n"
"}\n"
,

"class Fsm\n"
"{\n"
"public:\n"
"   typedef int State;\n"
"   const static State ERROR_STATE;\n"
"\n"
"   enum Token\n"
"   {\n"
"      TOK_0 = 0,\n"
"      TOK_1 = 1,\n"
"      TOK_2 = 2,\n"
"   };\n"
"\n"
"   static State process(State state, int sym, int& tok);\n"
"private:\n"
"   typedef State (*PfnStateHandler)(int, int&);\n"
"   static const PfnStateHandler stateHandler[8];\n"
"};\n"
"const Fsm::State Fsm::ERROR_STATE = -1;\n"
"\n"
"static Fsm::State handleState0(int, int&);\n"
"static Fsm::State handleState2(int, int&);\n"
"static Fsm::State handleState3(int, int&);\n"
"static Fsm::State handleState4(int, int&);\n"
"static Fsm::State handleState5(int, int&);\n"
"static Fsm::State handleState6(int, int&);\n"
"static Fsm::State handleState7(int, int&);\n"
"static Fsm::State handleDeadEnd(int, int&);\n"
"\n"
"const Fsm::PfnStateHandler Fsm::stateHandler[8] = {\n"
"   handleState0,\n"
"   handleDeadEnd,\n"
"   handleState2,\n"
"   handleState3,\n"
"   handleState4,\n"
"   handleState5,\n"
"   handleState6,\n"
"   handleState7,\n"
"};\n"
"Fsm::State handleState0(int sym, int& tok)\n"
"{\n"
"   switch(sym)\n"
"   {\n"
"   case 9: return (tok = TOK_2, 1);\n"
"   case 10: return (tok = TOK_2, 3);\n"
"   case 11: return (tok = TOK_0, 4);\n"
"   case 30: return (tok = TOK_2, 5);\n"
"   default: switch(sym >> 1) // [-2147483648, 2147483647]\n"
"      {\n"
"      case 0: return (2);\n"
"      case 1: return sym < 3 ? (2) : ERROR_STATE;\n"
"      case 2: return ERROR_STATE;\n"
"      case 3: return ERROR_STATE;\n"
"      case 4: return ERROR_STATE;\n"
"      case 5: return ERROR_STATE;\n"
"      default: return (tok = TOK_2, 1);\n"
"      }\n"
"   }\n"
"}\n"
"\n"
"Fsm::State handleState2(int sym, int& tok)\n"
"{\n"
"   switch(sym)\n"
"   {\n"
"   case 30: return (5);\n"
"   default: switch(sym >> 1) // [-2147483648, 2147483647]\n"
"      {\n"
"      case 0: return (2);\n"
"      case 1: return sym < 3 ? (2) : ERROR_STATE;\n"
"      default: return ERROR_STATE;\n"
"      }\n"
"   }\n"
"}\n"
"\n"
"Fsm::State handleState3(int sym, int& tok)\n"
"{\n"
"   switch(sym)\n"
"   {\n"
"   case 11: return (6);\n"
"   default: return ERROR_STATE;\n"
"   }\n"
"}\n"
"\n"
"Fsm::State handleState4(int sym, int& tok)\n"
"{\n"
"   switch(sym)\n"
"   {\n"
"   case 12: return (tok = TOK_0, 4);\n"
"   default: return ERROR_STATE;\n"
"   }\n"
"}\n"
"\n"
"Fsm::State handleState5(int sym, int& tok)\n"
"{\n"
"   switch(sym)\n"
"   {\n"
"   case 20: return (7);\n"
"   default: return ERROR_STATE;\n"
"   }\n"
"}\n"
"\n"
"Fsm::State handleState6(int sym, int& tok)\n"
"{\n"
"   switch(sym)\n"
"   {\n"
"   case 10: return (tok = TOK_0, 1);\n"
"   default: return ERROR_STATE;\n"
"   }\n"
"}\n"
"\n"
"Fsm::State handleState7(int sym, int& tok)\n"
"{\n"
"   switch(sym)\n"
"   {\n"
"   case 25: return (tok = TOK_1, 1);\n"
"   default: return ERROR_STATE;\n"
"   }\n"
"}\n"
"\n"
"Fsm::State handleDeadEnd(int, int&)\n"
"{\n"
"   return ERROR_STATE;\n"
"}\n"
"inline Fsm::State Fsm::process(State state, int sym, int& tok)\n"
"{\n"
"   if (static_cast<unsigned>(state) < sizeof(stateHandler) / sizeof(*stateHandler))\n"
"      return stateHandler[state](sym, tok);\n"
"   else return ERROR_STATE;\n"
"}"

};

void test_data(int index, std::istream& is)
{
	std::stringstream data(test_data_array[index]);
	std::string left, right;
	while (getline(data, left).good() && getline(is, right).good())
		BOOST_CHECK_EQUAL(left, right);
}
