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