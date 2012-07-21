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

#include "../src/GrammarParserCtrl.hpp"

#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_CASE(test_alternative)
{
   GrammarParserCtrl parserCtrl;
   FILE* file = tmpfile();

   const char* testgram = "G := { S := A | B : f(); };";

   fputs(testgram, file);
   rewind(file);

   parserCtrl.parse(file, "testfile");

   const std::map<std::string, GrammarParserCtrl::AnnotatedGrammar>& grammars = parserCtrl.getGrammars();

   BOOST_REQUIRE_EQUAL(1u, grammars.size());
   BOOST_CHECK_EQUAL("G", grammars.begin()->first);
   // 2 productions, due to inlining
   BOOST_CHECK_EQUAL(2u, grammars.begin()->second.grammar.getProductions().size());
   BOOST_REQUIRE_EQUAL(2u, grammars.begin()->second.actions.size());
   BOOST_REQUIRE_EQUAL(1u, grammars.begin()->second.actions[0].size());
   BOOST_REQUIRE_EQUAL(1u, grammars.begin()->second.actions[1].size());
   BOOST_CHECK_EQUAL("f", grammars.begin()->second.actions[0][0].function);
   BOOST_CHECK_EQUAL("f", grammars.begin()->second.actions[1][0].function);

   //printf("huhu\n");

  /* void parse(FILE* file, const std::string& filename);
	
	RegexParserCtrl parser(regex);
	BOOST_CHECK_EQUAL((int)regex.length(), parser.parse());
	
	Nfa<char> nfa = parser.getNfa();
	BOOST_CHECK(nfa.input().contains('/'));*/
}