#include "../src/lalrgram.hpp"
#include "../src/lalrparser.hpp"

#include <boost/test/unit_test.hpp>

#include <cstdio>
#include <cstdarg>

#include <iostream>

template<typename T>
std::ostream& operator<<(std::ostream& os, const typename LalrParsingTable<T>::Action& action);

template<typename T>
std::ostream& printToken(std::ostream& os, T tok);

template<typename T>
std::ostream& operator<<(std::ostream& os, const typename LalrParsingTable<T>::Action& action)
{
	os << "[";
	switch(action.type)
	{
	case LalrParsingTable<T>::AMBIGUOUS: os << "A"; break;
	case LalrParsingTable<T>::REDUCE: os << "R, " << action.productionNo; break;
	case LalrParsingTable<T>::SHIFT: os << "S->" << action.nextState; break;
	}
	os << "]";

	return os;
}

//template<class C>
//std::ostream& printContainer(std::ostream& os, const C& container);

template<typename T>
std::ostream& printActionMap(std::ostream& os, const typename LalrParsingTable<T>::ActionMap& actionMap);

template<typename T>
std::ostream& printInformation(std::ostream& os, const typename LalrGrammar<T>::Information& info, const typename LalrGrammar<T>::Productions productions);

template<class T, T N> void set_insert(std::set<T>& s, T first, ...)
{
	T i = first;
	va_list marker;
	va_start( marker, first );
	while(i!=N)
	{
		s.insert(i);
		i = va_arg( marker, T);
	}
	va_end( marker );
}

//static const char* getTestData(int sample, size_t index);

BOOST_AUTO_TEST_CASE(test_cs_lalrgram)
{
	// test contexts of actions with several follow-distance pairs
	LalrGrammar<char> gram;
	gram.addProduction('S', "A");
	gram.addProduction('S', "Ee");
	gram.addProduction('A', "BCd");
	gram.addProduction('A', "BC");
	gram.addProduction('B', "b");
	gram.addProduction('C', "c");
	gram.addProduction('E', "A");

	LalrGrammar<char>::Table table = gram.createParsingTable();

	BOOST_CHECK(gram.getErrors().empty());

	for (size_t s = 0; s < table.size(); ++s)
	{
		const LalrGrammar<char>::Information &info = gram.getInformation(s);
		std::cout << std::endl << s << ": ";
		printActionMap<char>(std::cout, table[s]);
		std::cout << std::endl;
		printInformation<char>(std::cout, info, gram.getProductions());
	}
}

BOOST_AUTO_TEST_CASE(test_lalrgram)
{
	// test ignore productions
	{
		LalrGrammar<char> gram;
		gram.addProduction('A', "BC");
		gram.addProduction('A', "d");
		gram.addProduction('B', "");
		gram.addProduction('\0', "e");
		gram.addProduction('\0', "d");

		LalrGrammar<char>::Table table = gram.createParsingTable();

		BOOST_CHECK(gram.getErrors().empty());

		LalrParser<char, '\0'> parser(table);

		/*for (size_t state = 0; state < table.size(); ++state)
		{
			std::cout << state << ": " << std::endl;
			printInformation<char>(std::cout, gram.getInformation(state), gram.getProductions());
		}*/
		// todo: test ignore items should not conflict each other

		//test ignore reductions are preferred over empty regulare reductions
		BOOST_CHECK_EQUAL(parser.next('e'), static_cast<int>(LalrParser<char, '\0'>::SHIFT));
		BOOST_CHECK_EQUAL(parser.next('d'), 3);
		// test non-empty regulare item is preferred over ignore-item
		BOOST_CHECK_EQUAL(parser.next('d'), static_cast<int>(LalrParser<char, '\0'>::SHIFT));
		// test reduce of productions with follow containing ignore-firsts
		BOOST_CHECK_EQUAL(parser.next('e'), static_cast<int>(LalrParser<char, '\0'>::SHIFT));
		BOOST_CHECK_EQUAL(parser.next('\0'), 3);
		BOOST_CHECK_EQUAL(parser.next('\0'), 1);
		/*size_t state = parser.getStackElement(parser.getStackSize() - 1).state();
		std::cout << state << ": " << std::endl;
		printInformation<char>(std::cout, gram.getInformation(state), gram.getProductions());*/

		/*
		conflict at token `nl'
		  shift
		    [END->.nl]
		    [END->nl.]
		  reduce END->nl
		conflict at token `ws'
		  shift
		    [END->.ws]
		    [END->nl.]
		  reduce END->nl
		conflict at token `"APIENTRY"'
		  shift
		    [funcattrib->."APIENTRY"]
		    [END->.funcattrib]
		    [END->nl.]
		  reduce END->nl
		conflict at token `"GLAPI"'
		  shift
		    [funcattrib->."GLAPI"]
		    [END->.funcattrib]
		    [END->nl.]
		  reduce END->nl
		conflict at token `multilinecomment'
		  reduce END->nl
		  shift
		    [END->nl.]
		    [END->.multilinecomment]
		conflict at token `singlelinecomment'
		  reduce END->nl
		  shift
		    [END->nl.]
		    [END->.singlelinecomment]
		*/


		// states of reduce should also accept ignore firsts
	}

	{
		LalrGrammar<char> gram;
		gram.addProduction('T', "It");
		gram.addProduction('I', "w");
		gram.addProduction('I', "");

		BOOST_CHECK_EQUAL(3u, gram.getProductions().size());
		BOOST_CHECK_EQUAL(0u, gram.getErrors().size());

		LalrGrammar<char>::Table table = gram.createParsingTable();

		BOOST_CHECK_EQUAL(0u, gram.getErrors().size());
		for (size_t i = 0; i < gram.getErrors().size(); ++i)
		{
			const LalrGrammar<char>::Error& error = gram.getErrors()[i];
			std::cout << "conflict in state " << error.state << " at token ";
			printToken(std::cout, error.token);
			std::cout << ": (" << error.actions.first.type << ", " << error.actions.first.nextState << ") - ";
			std::cout << "(" << error.actions.second.type << ", " << error.actions.second.nextState << ")" << std::endl;
		}

		const char *expTable[] =
		{
			"{([I], S->1), ([T], S->2), ([t], R, 2), ([w], S->3)}",
			"{([t], S->4)}",
			"{([\\0], R, 3)}",
			"{([t], R, 1)}",
			"{([\\0], R, 0)}",
		};

		BOOST_CHECK_EQUAL(sizeof(expTable)/sizeof(expTable[0]), table.size());

		if (sizeof(expTable)/sizeof(expTable[0]) == table.size())
		{
			for(size_t a = 0; a < table.size(); ++a)
			{
				std::stringstream row;
				printActionMap<char>(row, table[a]);

				std::string s1(row.str());
				std::string s2(expTable[a]);

				BOOST_CHECK_EQUAL(s1, s2);
			}
		}

		//if (table.size() != 5)
		//{
		//	for (size_t s = 0; s < table.size(); ++s)
		//	{
		//		const LalrGrammar<char>::Information &info = gram.getInformation(s);
		//		std::cout << std::endl << s << ": ";
		//		printActionMap<char>(std::cout, table[s]);
		//		std::cout << std::endl;
		//		printInformation<char>(std::cout, info, gram.getProductions());
		//	}
		//}
	}

	/*{
		//term := text;
		//term := "{%" code "%}";
		//
		//spaces := /[\t ]+/ : spaces;
		//spaces := @;
		//
		//text := text textline;
		//text := @;
		//code := code codeline;
		//code := @;
		//
		//textline := spaces /([^{\n]|{[^%\n]|\\{%)+/ : text;
		//codeline := spaces /([^%\n]|%[^}\n]|\\%})+/ : code;
		//
		//nl := "\n";
		//@ := nl : newline;

		LalrGrammar<char> gram;
		gram.addProduction('S', "X");
		gram.addProduction('S', "{C}");
		gram.addProduction('I', "w");
		gram.addProduction('I', "");
		gram.addProduction('X', "XT");
		gram.addProduction('X', "");
		gram.addProduction('C', "CL");
		gram.addProduction('C', "");
		gram.addProduction('T', "It");
		gram.addProduction('L', "Ie");
		gram.addProduction('N', "n");
		gram.addProduction('\0', "N");

		LalrGrammar<char>::Table table = gram.createParsingTable();

		for (size_t s = 0; s < table.size(); ++s)
		{
			const LalrGrammar<char>::Information &info = gram.getInformation(s);
			std::cout << s << ": ";
			printActionMap<char>(std::cout, table[s]);
			std::cout << std::endl;
			printInformation<char>(std::cout, info, gram.getProductions());
		}
	}*/
	/*{
		LalrGrammar<char> gram;
		gram.addProduction('E', "F");
		gram.addProduction('\0', "OFC");
		//gram.addProduction('E', std::string(1, '\0'));

		LalrGrammar<char>::Table table = gram.createParsingTable();

		for (size_t s = 0; s < table.size(); ++s)
		{
			const LalrGrammar<char>::Information &info = gram.getInformation(s);
			std::cout << s << ": ";
			printActionMap<char>(std::cout, table[s]);
			std::cout << std::endl;
			printInformation<char>(std::cout, info, gram.getProductions());
		}
	}*/
	{
		LalrGrammar<char> gram;
		gram.addProduction('E', "E|D");
		gram.addProduction('E', "D");
		gram.addProduction('D', "DF");
		gram.addProduction('D', "F");
		gram.addProduction('F', "(E)");
		gram.addProduction('F', "F*");
		gram.addProduction('F', "I");
		gram.addProduction('F', "[II]");
		gram.addProduction('F', "[^II]");
		//gram.addProduction('\0', "OFC");
		//gram.addProduction('E', std::string(1, '\0'));

		LalrGrammar<char>::Table table = gram.createParsingTable();
		BOOST_CHECK(gram.getErrors().empty());

		//for(size_t a = 0; a < table.size(); ++a)
		//{
		//	printActionMap<char>(std::cout, table[a]);
		//	std::cout << std::endl;
		//}

		/*for (size_t s = 0; s < table.size(); ++s)
		{
			const LalrGrammar<char>::Information &info = gram.getInformation(s);
			std::cout << s << ": ";
			printActionMap<char>(std::cout, table[s]);
			std::cout << std::endl;
			printInformation<char>(std::cout, info, gram.getProductions());
		}*/

		const char *expTable[] =
		{
			"{([(], S->1), ([D], S->2), ([E], S->3), ([F], S->4), ([I], S->5), ([[], S->6)}",
			"{([(], S->1), ([D], S->2), ([E], S->7), ([F], S->4), ([I], S->5), ([[], S->6)}",
			"{([\\0], R, 1), ([(], S->1), ([)], R, 1), ([F], S->8), ([I], S->5), ([[], S->6), ([|], R, 1)}",
			"{([\\0], R, 9), ([|], S->9)}",
			"{([\\0], R, 3), ([(], R, 3), ([)], R, 3), ([*], S->10), ([F], R, 3), ([I], R, 3), ([[], R, 3), ([|], R, 3)}",
			"{([\\0], R, 6), ([(], R, 6), ([)], R, 6), ([*], R, 6), ([F], R, 6), ([I], R, 6), ([[], R, 6), ([|], R, 6)}",
			"{([I], S->11), ([^], S->12)}",
			"{([)], S->13), ([|], S->9)}",
			"{([\\0], R, 2), ([(], R, 2), ([)], R, 2), ([*], S->10), ([F], R, 2), ([I], R, 2), ([[], R, 2), ([|], R, 2)}",
			"{([(], S->1), ([D], S->14), ([F], S->4), ([I], S->5), ([[], S->6)}",
			"{([\\0], R, 5), ([(], R, 5), ([)], R, 5), ([*], R, 5), ([F], R, 5), ([I], R, 5), ([[], R, 5), ([|], R, 5)}",
			"{([I], S->15)}",
			"{([I], S->16)}",
			"{([\\0], R, 4), ([(], R, 4), ([)], R, 4), ([*], R, 4), ([F], R, 4), ([I], R, 4), ([[], R, 4), ([|], R, 4)}",
			"{([\\0], R, 0), ([(], S->1), ([)], R, 0), ([F], S->8), ([I], S->5), ([[], S->6), ([|], R, 0)}",
			"{([]], S->17)}",
			"{([I], S->18)}",
			"{([\\0], R, 7), ([(], R, 7), ([)], R, 7), ([*], R, 7), ([F], R, 7), ([I], R, 7), ([[], R, 7), ([|], R, 7)}",
			"{([]], S->19)}",
			"{([\\0], R, 8), ([(], R, 8), ([)], R, 8), ([*], R, 8), ([F], R, 8), ([I], R, 8), ([[], R, 8), ([|], R, 8)}",
		};

		BOOST_CHECK_EQUAL(sizeof(expTable)/sizeof(expTable[0]), table.size());

		if (sizeof(expTable)/sizeof(expTable[0]) == table.size())
		{
			for(size_t a = 0; a < table.size(); ++a)
			{
				std::stringstream row;
				printActionMap<char>(row, table[a]);

				std::string s1(row.str());
				std::string s2(expTable[a]);

				BOOST_CHECK_EQUAL(s1, s2);
			}
		}
	}

	// Test grammars with ignore production
	{
		LalrGrammar<char> gram;
		gram.addProduction('E', "E|D");
		gram.addProduction('E', "D");
		gram.addProduction('D', "DF");
		gram.addProduction('D', "F");
		gram.addProduction('F', "(E)");
		gram.addProduction('F', "F*");
		gram.addProduction('F', "I");
		gram.addProduction('F', "[II]");
		gram.addProduction('F', "[^II]");
		gram.addProduction('\0', "OFC");

		LalrGrammar<char>::Table table = gram.createParsingTable();
		BOOST_CHECK(gram.getErrors().empty());

		/*for(size_t a = 0; a < table.size(); ++a)
		{
			printActionMap<char>(std::cout, table[a]);
			std::cout << std::endl;
		}*/

		/*for (size_t s = 0; s < table.size(); ++s)
		{
			const LalrGrammar<char>::Information &info = gram.getInformation(s);
			std::cout << s << ": ";
			printActionMap<char>(std::cout, table[s]);
			std::cout << std::endl;
			printInformation<char>(std::cout, info, gram.getProductions());
		}*/

		const char *expTable[] =
		{
			"{([(], S->1), ([D], S->2), ([E], S->3), ([F], S->4), ([I], S->5), ([O], S->6), ([[], S->7)}",
			"{([(], S->1), ([D], S->2), ([E], S->8), ([F], S->4), ([I], S->5), ([O], S->6), ([[], S->7)}",
			"{([\\0], R, 1), ([(], S->1), ([)], R, 1), ([F], S->9), ([I], S->5), ([O], S->6), ([[], S->7), ([|], R, 1)}",
			"{([\\0], R, 10), ([O], S->6), ([|], S->10)}",
			"{([\\0], R, 3), ([(], R, 3), ([)], R, 3), ([*], S->11), ([F], R, 3), ([I], R, 3), ([O], S->6), ([[], R, 3), ([|], R, 3)}",
			"{([\\0], R, 6), ([(], R, 6), ([)], R, 6), ([*], R, 6), ([F], R, 6), ([I], R, 6), ([O], S->6), ([[], R, 6), ([|], R, 6)}",
			"{([(], S->12), ([F], S->13), ([I], S->14), ([[], S->15)}",
			"{([I], S->16), ([O], S->6), ([^], S->17)}",
			"{([)], S->18), ([O], S->6), ([|], S->10)}",
			"{([\\0], R, 2), ([(], R, 2), ([)], R, 2), ([*], S->11), ([F], R, 2), ([I], R, 2), ([O], S->6), ([[], R, 2), ([|], R, 2)}",
			"{([(], S->1), ([D], S->19), ([F], S->4), ([I], S->5), ([O], S->6), ([[], S->7)}",
			"{([\\0], R, 5), ([(], R, 5), ([)], R, 5), ([*], R, 5), ([F], R, 5), ([I], R, 5), ([O], S->6), ([[], R, 5), ([|], R, 5)}",
			"{([(], S->12), ([D], S->20), ([E], S->21), ([F], S->22), ([I], S->14), ([[], S->15)}",
			"{([*], S->23), ([C], S->24)}",
			"{([(], R, 6), ([)], R, 6), ([*], R, 6), ([C], R, 6), ([F], R, 6), ([I], R, 6), ([[], R, 6), ([|], R, 6)}",
			"{([I], S->25), ([^], S->26)}",
			"{([I], S->27), ([O], S->6)}",
			"{([I], S->28), ([O], S->6)}",
			"{([\\0], R, 4), ([(], R, 4), ([)], R, 4), ([*], R, 4), ([F], R, 4), ([I], R, 4), ([O], S->6), ([[], R, 4), ([|], R, 4)}",
			"{([\\0], R, 0), ([(], S->1), ([)], R, 0), ([F], S->9), ([I], S->5), ([O], S->6), ([[], S->7), ([|], R, 0)}",
			"{([(], S->12), ([)], R, 1), ([F], S->29), ([I], S->14), ([[], S->15), ([|], R, 1)}",
			"{([)], S->30), ([|], S->31)}",
			"{([(], R, 3), ([)], R, 3), ([*], S->23), ([F], R, 3), ([I], R, 3), ([[], R, 3), ([|], R, 3)}",
			"{([(], R, 5), ([)], R, 5), ([*], R, 5), ([C], R, 5), ([F], R, 5), ([I], R, 5), ([[], R, 5), ([|], R, 5)}",
			"{([\\0], R, 9), ([(], R, 9), ([)], R, 9), ([*], R, 9), ([D], R, 9), ([E], R, 9), ([F], R, 9), ([I], R, 9), ([O], R, 9), ([[], R, 9), ([]], R, 9), ([^], R, 9), ([|], R, 9)}",
			"{([I], S->32)}",
			"{([I], S->33)}",
			"{([O], S->6), ([]], S->34)}",
			"{([I], S->35), ([O], S->6)}",
			"{([(], R, 2), ([)], R, 2), ([*], S->23), ([F], R, 2), ([I], R, 2), ([[], R, 2), ([|], R, 2)}",
			"{([(], R, 4), ([)], R, 4), ([*], R, 4), ([C], R, 4), ([F], R, 4), ([I], R, 4), ([[], R, 4), ([|], R, 4)}",
			"{([(], S->12), ([D], S->36), ([F], S->22), ([I], S->14), ([[], S->15)}",
			"{([]], S->37)}",
			"{([I], S->38)}",
			"{([\\0], R, 7), ([(], R, 7), ([)], R, 7), ([*], R, 7), ([F], R, 7), ([I], R, 7), ([O], S->6), ([[], R, 7), ([|], R, 7)}",
			"{([O], S->6), ([]], S->39)}",
			"{([(], S->12), ([)], R, 0), ([F], S->29), ([I], S->14), ([[], S->15), ([|], R, 0)}",
			"{([(], R, 7), ([)], R, 7), ([*], R, 7), ([C], R, 7), ([F], R, 7), ([I], R, 7), ([[], R, 7), ([|], R, 7)}",
			"{([]], S->40)}",
			"{([\\0], R, 8), ([(], R, 8), ([)], R, 8), ([*], R, 8), ([F], R, 8), ([I], R, 8), ([O], S->6), ([[], R, 8), ([|], R, 8)}",
			"{([(], R, 8), ([)], R, 8), ([*], R, 8), ([C], R, 8), ([F], R, 8), ([I], R, 8), ([[], R, 8), ([|], R, 8)}",
		};

		BOOST_CHECK_EQUAL(sizeof(expTable)/sizeof(expTable[0]), table.size());

		if (sizeof(expTable)/sizeof(expTable[0]) == table.size())
		{
			for(size_t a = 0; a < table.size(); ++a)
			{
				std::stringstream row;
				printActionMap<char>(row, table[a]);

				std::string s1(row.str());
				std::string s2(expTable[a]);

				BOOST_CHECK_EQUAL(s1, s2);

				if (s1 != s2)
				{
					const LalrGrammar<char>::Information& info = gram.getInformation(a);
					printInformation<char>(std::cout, info, gram.getProductions());
				}
			}
		}
	}
}

//template<typename T1>
//std::ostream& operator<<(std::ostream& os, const std::pair<T1, typename LalrParsingTable<T1>::Action>& pair)
//{
//}

template<typename T>
std::ostream& printToken(std::ostream& os, T tok)
{
	switch(tok)
	{
	case '\0': os << "\\0"; break;
	default: os << tok; break;
	}
	return os;
}

template<typename T>
std::ostream& printInformation(std::ostream& os, const typename LalrGrammar<T>::Information& info, const typename LalrGrammar<T>::Productions productions)
{
	for (typename LalrGrammar<T>::InfoItems::const_iterator i = info.items.begin(); i != info.items.end(); ++i)
	{
		const typename LalrGrammar<T>::Production &prod = productions[i->first.productionNo];
		size_t dot = i->first.dot;
		typename LalrGrammar<T>::String front = prod.body().substr(0, dot);
		typename LalrGrammar<T>::String remain = prod.body().substr(dot);
		os << "[";
		printToken(os, prod.head());
		os << "->" << front;
		os << "." << remain;
		os << "], (";
		for (typename LalrGrammar<T>::Set::const_iterator ifollow = i->second.begin(); ifollow != i->second.end(); ++ifollow)
		{
			os << (ifollow == i->second.begin() ? "" : " ");
			printToken(os, *ifollow);
		}
		os << ")" << std::endl;;
	}

	return os;
}

template<typename T>
std::ostream& printActionMap(std::ostream& os, const typename LalrParsingTable<T>::ActionMap& actionMap)
{
	typename LalrParsingTable<T>::ActionMap::const_iterator s;
	os << "{";
	for(s = actionMap.begin(); s != actionMap.end(); ++s)
	{
		if( s != actionMap.begin() ) os << ", ";
		os << "(";
		os << "[";
		printToken(os, s->first);
		os << "], ";
		switch(s->second.type)
		{
		case LalrParsingTable<T>::AMBIGUOUS: os << "A"; break;
		case LalrParsingTable<T>::REDUCE: os << "R, " << s->second.productionNo; break;
		case LalrParsingTable<T>::SHIFT: os << "S->" << s->second.nextState; break;
		}
		os << ")";
	}
	os << "}";
	return os;
}

/*
void printInfo(std::ostream& os, const LalrGrammar<char>&gram, const LalrGrammar<char>::Information& info)
{
	for (LalrGrammar<char>::InfoItems::const_iterator i = info.items.begin(); i != info.items.end(); ++i)
	{
		const LalrGrammar<char>::Production &prod = gram.getProductions()[i->first.productionNo];
		size_t dot = i->first.dot;
		std::string front = prod.body().substr(0, dot);
		std::string remain = prod.body().substr(dot);
		os << "//" << (i->first.ignore ? "<ignore>" : "") << " [" << prod.head() << "->" << front;
		os << "." << remain;
		os << "]," << std::endl << "//    (";
		for (std::set<char>::const_iterator ifollow = i->second.begin(); ifollow != i->second.end(); ++ifollow)
			os << (ifollow == i->second.begin() ? "" : " ") << *ifollow;
		os << ")" << std::endl;
	}
}*/

static const char *testData[] =
{
	"{([(], S->1), ([D], S->2), ([E], S->3), ([F], S->4), ([I], S->5), ([[], S->6)}",
	"{([(], S->1), ([D], S->2), ([E], S->7), ([F], S->4), ([I], S->5), ([[], S->6)}",
	"{([\\0], R, 1), ([(], S->1), ([)], R, 1), ([F], S->8), ([I], S->5), ([[], S->6), ([|], R, 1)}",
	"{([\\0], R, 10), ([|], S->9)}",
	"{([\\0], R, 3), ([(], R, 3), ([)], R, 3), ([*], S->10), ([F], R, 3), ([I], R, 3), ([[], R, 3), ([|], R, 3)}",
	"{([\\0], R, 6), ([(], R, 6), ([)], R, 6), ([*], R, 6), ([F], R, 6), ([I], R, 6), ([[], R, 6), ([|], R, 6)}",
	"{([I], S->11), ([^], S->12)}",
	"{([)], S->13), ([|], S->9)}",
	"{([\\0], R, 2), ([(], R, 2), ([)], R, 2), ([*], S->10), ([F], R, 2), ([I], R, 2), ([[], R, 2), ([|], R, 2)}",
	"{([(], S->1), ([D], S->14), ([F], S->4), ([I], S->5), ([[], S->6)}",
	"{([\\0], R, 5), ([(], R, 5), ([)], R, 5), ([*], R, 5), ([F], R, 5), ([I], R, 5), ([[], R, 5), ([|], R, 5)}",
	"{([I], S->15)}",
	"{([I], S->16)}",
	"{([\\0], R, 4), ([(], R, 4), ([)], R, 4), ([*], R, 4), ([F], R, 4), ([I], R, 4), ([[], R, 4), ([|], R, 4)}",
	"{([\\0], R, 0), ([(], S->1), ([)], R, 0), ([F], S->8), ([I], S->5), ([[], S->6), ([|], R, 0)}",
	"{([]], S->17)}",
	"{([I], S->18)}",
	"{([\\0], R, 7), ([(], R, 7), ([)], R, 7), ([*], R, 7), ([F], R, 7), ([I], R, 7), ([[], R, 7), ([|], R, 7)}",
	"{([]], S->19)}",
	"{([\\0], R, 8), ([(], R, 8), ([)], R, 8), ([*], R, 8), ([F], R, 8), ([I], R, 8), ([[], R, 8), ([|], R, 8)}",
};

const char* getTestData(size_t index)
{
	return testData[index];
}

