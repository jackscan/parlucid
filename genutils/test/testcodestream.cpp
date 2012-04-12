#include "testcodestream.hpp"
#include "../src/codestream.hpp"

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <sstream>
#include <iostream>

BOOST_AUTO_TEST_CASE(test_codestream)
{
	std::stringstream resultstr;
	CodeStream cs(resultstr, "\t");
	
	cs.incIndent();
	cs << "switch()" << newline;
	cs << "{" << newline;
	cs << indent << "case 1:" << newline;
	cs << indent << "break;" << newline;
	cs << revindent << revindent << "}" << newline;

	//std::cout << resultstr.str() << std::endl;

	std::string teststr("\tswitch()\n\t{\n\t\tcase 1:\n\t\t\tbreak;\n\t}\n");

	BOOST_CHECK_EQUAL(resultstr.str(), teststr);
}

