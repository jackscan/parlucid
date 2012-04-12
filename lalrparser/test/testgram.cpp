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
