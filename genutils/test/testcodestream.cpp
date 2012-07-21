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

