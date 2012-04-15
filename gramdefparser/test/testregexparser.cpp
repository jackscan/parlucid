#include "../src/RegexParserCtrl.hpp"

#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_CASE(test_regexparser)
{
	std::string regex("\\/([^\\/]|\\\\\\/)*\\/");
	RegexParserCtrl parser(regex);
	BOOST_CHECK_EQUAL((int)regex.length(), parser.parse());
	
	RegexParserCtrl::NfaChar nfa = parser.getNfa();
	BOOST_CHECK(nfa.input().contains('/'));
}
