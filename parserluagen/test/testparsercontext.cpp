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

#include "../src/parsercontext.hpp"

#include "testparsergen.hpp"

#include <boost/test/unit_test.hpp>

void test_parsercontext()
{
	{
		Nfa<char> nfa_ab("ab"); nfa_ab.set_output(0);
		Nfa<char> nfa_a("a"); nfa_a.set_output(1);
		Nfa<char> nfa_b("b"); nfa_b.set_output(2);
		Nfa<char> nfa_abc1("abc"); nfa_abc1.set_output(3);
		Nfa<char> nfa_abc2("abc"); nfa_abc2.set_output(0);
		Nfa<char> nfa_abdc1("abdc"); nfa_abdc1.set_output(4);
		Nfa<char> nfa_abdc2("abdc"); nfa_abdc2.set_output(5);

		Nfa<char> nfa_all = nfa_ab | nfa_a | nfa_b | nfa_abc1 | nfa_abc2 | ((nfa_abdc1 | nfa_abdc2) + *(nfa_abdc1 | nfa_abdc2));
		Fsm<char> fsm(nfa_all);
		fsm.reduce();

		/*std::map<int, std::set<int> > expectedPrefixes;
		expectedPrefixes[0].insert(1);
		expectedPrefixes[0].insert(3);
		expectedPrefixes[3].insert(1);
		expectedPrefixes[4].insert(0);
		expectedPrefixes[4].insert(1);
		expectedPrefixes[4].insert(5);
		expectedPrefixes[5].insert(0);
		expectedPrefixes[5].insert(1);*/

		/*
		0, 3 _
		0, 2 _\_ 0, 2, 3
		4 ____
		4, 5 _\_ 4, 5
		0, 2, 5 ____
		0, 1, 2, 5 _\_ 0, 1, 2, 5
		*/

		LalrParsingTable<int>::Productions productions;
		LalrParsingTable<int> table(productions, 6);
		
		LalrParsingTable<int>::Action action;
		action.type = LalrParsingTable<int>::SHIFT;
		action.nextState = 0;

		table[0][0] = action; table[0][3] = action;
		table[1][0] = action; table[1][2] = action;
		table[2][4] = action;
		table[3][4] = action; table[3][5] = action;
		table[4][0] = action; table[4][2] = action; table[4][5] = action;
		table[5][0] = action; table[5][1] = action; table[5][2] = action; table[5][5] = action;

		std::map<int, BitSequence<int> > prefixes;
		fsm.getPrefixAssociations(prefixes);

		ParserContext<int> parserContext(table, prefixes, std::map<int, BitSequence<int> >());

		BOOST_REQUIRE_EQUAL(3, parserContext.getNumberOfContexts());

		BOOST_CHECK_EQUAL(0, parserContext.getParsingTableContext(0));
		BOOST_CHECK_EQUAL(0, parserContext.getParsingTableContext(1));

		BOOST_CHECK_EQUAL(1, parserContext.getParsingTableContext(2));
		BOOST_CHECK_EQUAL(1, parserContext.getParsingTableContext(3));

		BOOST_CHECK_EQUAL(2, parserContext.getParsingTableContext(4));
		BOOST_CHECK_EQUAL(2, parserContext.getParsingTableContext(5));
	}
}
