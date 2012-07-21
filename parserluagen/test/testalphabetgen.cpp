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
#include "../src/alphabetgen.hpp"
#include "../../lexer/src/alphabet.hpp"
#include "../../genutils/src/codestream.hpp"

#include <iostream>
#include <sstream>
#include <limits>

#include <boost/test/unit_test.hpp>

static void test_data(int, std::istream& is);

void test_alphabetgen()
{
	/*{
		Alphabet<int> alphabet(Interval<int>(0, 1));
		alphabet.add(2, 3);
		std::map<int, std::string> results;
		results[0] = "0";
		results[2] = "2";
		AlphabetGen<int>(cs, alphabet, results, "outside");
	}
	{
		Alphabet<int> alphabet(Interval<int>(1, 4));
		alphabet.add(0);
		std::map<int, std::string> results;
		results[0] = "0";
		results[1] = "1";
		AlphabetGen<int>(cs, alphabet, results, "outside");
	}
	{
		Alphabet<char> alphabet(Interval<char>('0', '9'));
		std::map<char, std::string> results;
		results['0'] = "0";
		AlphabetGen<char>(cs, alphabet, results, "outside");
		alphabet.addDisjunct('2', '3');
		results['2'] = "1";
		results['4'] = "2";
		AlphabetGen<char>(cs, alphabet, results, "outside");
	}
	{
		Alphabet<char> alphabet(Interval<char>(std::numeric_limits<char>::min(), std::numeric_limits<char>::max()));
		alphabet.del('2', '7');
		std::map<char, std::string> results;
		results[std::numeric_limits<char>::min()] = "1";
		results['8'] = "2";
		AlphabetGen<char>(cs, alphabet, results, "outside");
		results['8'] = "1";
		AlphabetGen<char>(cs, alphabet, results, "outside");
	}
	{
		Alphabet<int> alphabet(Interval<int>(0, 9));
		alphabet.add(10, 11);
		std::map<int, std::string> results;
		results[0] = "1";
		results[10] = "2";
		AlphabetGen<int>(cs, alphabet, results, "outside");
	}
	{
		Alphabet<short> alphabet(Interval<short>(111,113));
		alphabet.add(119, 123);
		alphabet.add(125, std::numeric_limits<short>::max());
		alphabet.add(114);
		alphabet.add(115);
		alphabet.add(116);
		alphabet.add(117);
		alphabet.add(118);
		alphabet.add(124);
		std::map<short, std::string> results;
		results[111] = "111";
		results[114] = "114";
		results[115] = "115";
		results[116] = "116";
		results[117] = "117";
		results[118] = "118";
		results[119] = "119";
		results[124] = "124";
		results[125] = "125";
		AlphabetGen<short>(cs, alphabet, results, "outside");
	}*/
	{
		Alphabet<short> alphabet(Interval<short>('0', '9'));

		int singletons[] = { 40,41,42,43,44,45,46,47,48,58,63,91,92,93,94,97,98,102,110,114,115,116,117,118,124 };
		for (unsigned i = 0; i < sizeof(singletons) / sizeof(*singletons); ++i)
			alphabet.add(singletons[i]);

		alphabet.add(std::numeric_limits<short>::min(), 39);
		alphabet.add(49, 57);
		alphabet.add(59, 62);
		alphabet.add(64, 90);
		alphabet.add(95, 96);
		alphabet.add(99, 101);
		alphabet.add(103, 109);
		alphabet.add(111, 113);
		alphabet.add(119, 123);
		alphabet.add(125, std::numeric_limits<short>::max());

		std::map<short, std::string> results;
		std::set<short> bounds = alphabet.lower_bound();
		for (std::set<short>::const_iterator ib = bounds.begin(); ib != bounds.end(); ++ib)
		{
			std::stringstream str;
			str << *ib;
			results[*ib] = str.str();
		}

		std::stringstream outstr;
		CodeStream cs(outstr, "   ");

		AlphabetGen<short>(cs, alphabet, results, "outside");

		test_data(0, outstr);
	}
}

static const std::string test_data_array[] = {
	"switch(sym)\n"
	"{\n"
	"case 40: return 40;\n"
	"case 41: return 41;\n"
	"case 42: return 42;\n"
	"case 43: return 43;\n"
	"case 44: return 44;\n"
	"case 45: return 45;\n"
	"case 46: return 46;\n"
	"case 47: return 47;\n"
	"case 58: return 58;\n"
	"case 63: return 63;\n"
	"case 91: return 91;\n"
	"case 92: return 92;\n"
	"case 93: return 93;\n"
	"case 94: return 94;\n"
	"case 97: return 97;\n"
	"case 98: return 98;\n"
	"case 102: return 102;\n"
	"case 110: return 110;\n"
	"case 114: return 114;\n"
	"case 115: return 115;\n"
	"case 116: return 116;\n"
	"case 117: return 117;\n"
	"case 118: return 118;\n"
	"case 124: return 124;\n"
	"default: switch(sym >> 4) // [-32768, 32767]\n"
	"   {\n"
	"   case 3: switch(sym >> 3) // [48, 63]\n"
	"      {\n"
	"      case 7: return sym < 58 ? 48 : 59;\n"
	"      default: return 48;\n"
	"      }\n"
	"   case 4: return 64;\n"
	"   case 5: return sym < 92 ? 64 : 95;\n"
	"   case 6: switch(sym >> 2) // [96, 111]\n"
	"      {\n"
	"      case 24: return sym < 98 ? 95 : 99;\n"
	"      case 25: return sym < 102 ? 99 : 103;\n"
	"      case 26: return 103;\n"
	"      case 27: return sym < 110 ? 103 : 111;\n"
	"      default: return 95;\n"
	"      }\n"
	"   case 7: switch(sym >> 3) // [112, 127]\n"
	"      {\n"
	"      case 14: return sym < 116 ? 111 : 119;\n"
	"      case 15: return sym < 124 ? 119 : 125;\n"
	"      default: return 111;\n"
	"      }\n"
	"   default: return -32768;\n"
	"   }\n"
	"}"
};

void test_data(int index, std::istream& is)
{
	std::stringstream data(test_data_array[index]);
	std::string left, right;
	while (getline(data, left).good() && getline(is, right).good())
		BOOST_CHECK_EQUAL(left, right);
}
