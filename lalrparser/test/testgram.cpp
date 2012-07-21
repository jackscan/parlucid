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

#include "../src/gram.hpp"

#include <boost/test/unit_test.hpp>

#include <cstdio>
#include <cstdarg>

#include <iostream>

template<typename T>
std::ostream& operator<<(std::ostream& os,  const std::set<T>& rhs);

class TestGrammar: public Grammar<char>
{
public:
	void testCalculation();
	void testDelProductions();
};

BOOST_AUTO_TEST_CASE(test_gram)
{
	TestGrammar().testCalculation();
	TestGrammar().testDelProductions();
}

template<class T, T N> void set_insert(std::set<T>& s, T first, ...)
{
	T i = first;
	va_list marker;
	va_start( marker, first );
	while(i!=N)
	{
		s.insert(i);
		i = va_arg( marker, int);
	}
	va_end( marker );
}

void TestGrammar::testDelProductions()
{
	// todo:
	{
		this->addProduction('T', "It");
		this->addProduction('I', "w");
		this->addProduction('I', "");
		this->addProduction('T', "It");

		//this->delProductions
	}
}

void TestGrammar::testCalculation()
{
	{
		this->addProduction('T', "It");
		this->addProduction('I', "w");
		this->addProduction('I', "");
		this->addProduction('T', "It");
		this->calcFirst();
		this->calcFollow();

		BOOST_CHECK(this->epsilon('I'));

		this->clear();
	}
	{
		this->addProduction('E', "E|D");
		this->addProduction('E', "D");
		this->addProduction('D', "DF");
		this->addProduction('D', "F");
		this->addProduction('F', "(E)");
		this->addProduction('F', "F*");
		this->addProduction('F', "I");
		this->addProduction('F', "[II]");
		this->addProduction('F', "[^II]");

		this->calcFirst();
		this->calcFollow();

		//std::cout << "E: " << firstSets['E'] << std::endl;
		//std::cout << "D: " << firstSets['D'] << std::endl;
		//std::cout << "F: " << firstSets['F'] << std::endl;
		//std::cout << "E: " << followSets['E'] << std::endl;
		//std::cout << "D: " << followSets['D'] << std::endl;
		this->clear();
	}

	{
		this->addProduction('S', "aR");
		this->addProduction('S', "(S)R");
		this->addProduction('R', "+S");
		this->addProduction('R', "*S");
		this->addProduction('R', "");

		std::set<char> firstSet_S, firstSet_R, eps;
		std::set<char> followSet_S, followSet_R;

		eps.insert('R');
		firstSet_S.insert('a');
		firstSet_S.insert('(');
		firstSet_R.insert('+');
		firstSet_R.insert('*');

		followSet_S.insert(')');
		followSet_R.insert(')');

		this->calcFirst();
		this->calcFollow();
		//printContainer(set<char>, this->epsFirst);
		BOOST_CHECK(this->epsFirst == eps);
		//printContainer(set<char>, this->firstSets['S']);
		BOOST_CHECK(this->firstSets['S'] == firstSet_S);
		//printContainer(set<char>, this->firstSets['R']);
		BOOST_CHECK(this->firstSets['R'] == firstSet_R);
		//printContainer(set<char>, this->followSets['S']);
		BOOST_CHECK(this->followSets['S'] == followSet_S);
		//printContainer(set<char>, this->followSets['R']);
		BOOST_CHECK(this->followSets['R'] == followSet_R);

		this->clear();

		BOOST_CHECK(this->productions.empty());
	}
// -------------------------------------------------------
	{
		std::map<char,std::set<char> > firsts, follows;
		std::set<char> eps;

		this->addProduction('S', "ABCd");
		this->addProduction('A', "e");
		this->addProduction('A', "f");
		this->addProduction('A', "");
		this->addProduction('B', "g");
		this->addProduction('B', "h");
		this->addProduction('B', "");
		this->addProduction('C', "p");
		this->addProduction('C', "q");

		set_insert<char,-1>(firsts['S'], 'A', 'B', 'C', 'e', 'f', 'g', 'h', 'p', 'q', -1);
		set_insert<char,-1>(firsts['A'], 'e', 'f', -1);
		set_insert<char,-1>(firsts['B'], 'g', 'h', -1);
		set_insert<char,-1>(firsts['C'], 'p', 'q', -1);
		set_insert<char,-1>(eps, 'A', 'B', -1);
		set_insert<char,-1>(follows['S'], -1);
		set_insert<char,-1>(follows['A'], 'B', 'C', 'g', 'h', 'p', 'q', -1);
		set_insert<char,-1>(follows['B'], 'C', 'p', 'q', -1);
		set_insert<char,-1>(follows['C'], 'd', -1);

		this->calcFirst();
		this->calcFollow();
		//printContainer(set<char>, this->epsFirst);
		BOOST_CHECK(this->epsFirst == eps);

		char terminals[] = {'S', 'A', 'B', 'C' };
		for(unsigned int i=0; i<sizeof(terminals); i++)
		{
			char t = terminals[i];
			//cout << "t = '" << t << "': ";
			//printContainer(set<char>, this->firstSets[t]);
			BOOST_CHECK(this->firstSets[t] == firsts[t]);
		}

		for(unsigned int i=0; i<sizeof(terminals); i++)
		{
			char t = terminals[i];
			//cout << "t = '" << t << "': ";
			//printContainer(set<char>, this->followSets[t]);
			BOOST_CHECK(this->followSets[t] == follows[t]);
		}

	}
}

template<typename T> std::ostream& operator<<(std::ostream& os, const std::set<T>& rhs)
{
	typename std::set<T>::const_iterator s;
	os << "{";
	for(s = rhs.begin(); s != rhs.end(); ++s)
	{
		if( s != rhs.begin() ) os << ", ";
		os << *s;
	}
	os << "}";
	return os;
}
