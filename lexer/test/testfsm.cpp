
//#include "testlexer.hpp"
#include "../src/fsm.hpp"

#include <iostream>
#include <limits>

#include <boost/test/unit_test.hpp>

static void test_getAcceptedSymbols(const Fsm<char>& fsm, Fsm<char>::State state, const Alphabet<char>& expectedInput);
static void test_getPrefixAssociations(const Fsm<char>& fsm, const std::map<int,BitSequence<int> >& expectedPrefixes);
static void test_getStatePrefixes(const Fsm<char>& fsm, Fsm<char>::State, const char *expectedPrefixes);
static void test_next(const Fsm<char>& fsm, Fsm<char>::State state, const char *symbols, Fsm<char>::State nextState, const char* expectedOutput);
static void test_output(const Fsm<char>& fsm, const char* expectedOutput);

/*
void test_lineargroup(const Fsm<char>& fsm, Fsm<char>::State startState, const char *symbols, Fsm<char>::State endState, const char* expectedOutput)
{
	BitSequence<int> outs;
	while (*expectedOutput) outs.set(*(expectedOutput++) - '0');
	Fsm<char>::State state = startState;

	char sym = *symbols;
	while (*(++symbols))
	{
		Fsm<char>::State nextState = state;
		BOOST_CHECK(fsm.next(nextState, *symbols).empty());
		BOOST_CHECK_EQUAL(nextState, state+1);
		state = nextState;
		sym = *symbols;
	}
	const BitSequence<int>& output = fsm.next(state, sym);
	BOOST_CHECK_EQUAL(state, endState);
	BOOST_CHECK_EQUAL(outs, output);
}*/

#define CHECK_LINEARGROUP(fsm, startState, symbols, endState, expectedOutput) \
{\
	const char* pSym = symbols; const char* pOut = expectedOutput;\
	BitSequence<int> outs;\
	while (*pOut) outs.set(*(pOut++) - '0');\
	Fsm<char>::State state = startState; char sym = *symbols;\
	while (*(++pSym)) {\
		Fsm<char>::State nextState = state;\
		BOOST_CHECK(fsm.next(nextState, sym).empty());\
		BOOST_CHECK_EQUAL(nextState, state+1);\
		state = nextState; sym = *pSym; }\
	const BitSequence<int>& output = fsm.next(state, sym);\
	BOOST_CHECK_EQUAL(state, endState);\
	BOOST_CHECK_EQUAL(outs, output);\
}

BOOST_AUTO_TEST_CASE(test_fsm)
{
	// test groupLinearPaths
	{
		//1. check loop
		{
			// check loop from first state
			Nfa<char> nfa = *Nfa<char>("first");
			nfa.set_output(0);
			Fsm<char> fsm(nfa);
			fsm.reduce();
			fsm.normalize();

			fsm.groupLinearPaths(1, 0);

			BOOST_CHECK_EQUAL(0, fsm.getFirstLinearSequence(0));

			CHECK_LINEARGROUP(fsm, 0, "first", 0, "0");
		}
		//2. check loops without output
		{
			Nfa<char> nfa = Nfa<char>("f") + *Nfa<char>("irst");
			Fsm<char> fsm(nfa);
			fsm.reduce();
			fsm.normalize();

			fsm.groupLinearPaths(1, 0);

			BOOST_CHECK_EQUAL(0, fsm.getFirstLinearSequence(0));

			CHECK_LINEARGROUP(fsm, 1, "irst", 1, "");
		}

		//3. check sequence with entry state larger than lowest state of sequence
		{
			Nfa<char> nfa = Nfa<char>("first")|((Nfa<char>("A")|Nfa<char>("B")) + *Nfa<char>("C") + Nfa<char>("Dfirst"));
			nfa.set_output(0);
			Fsm<char> fsm(nfa);
			fsm.reduce();
			fsm.normalize();

			test_next(fsm, 0, "A", 1, "");
			test_next(fsm, 0, "B", 1, "");
			test_next(fsm, 0, "f", 2, "");
			test_next(fsm, 1, "C", 1, "");
			test_next(fsm, 1, "D", 3, "");
			test_next(fsm, 2, "i", 4, "");
			test_next(fsm, 3, "f", 2, "");
			test_next(fsm, 4, "r", 5, "");
			test_next(fsm, 5, "s", 6, "");
			test_next(fsm, 6, "t", 7, "0");

			fsm.groupLinearPaths(1, 0);

			BOOST_CHECK_EQUAL(3, fsm.getFirstLinearSequence(0));

			// first group is "irst"
			// second group is "first"

			test_next(fsm, 0, "A", 1, "");
			test_next(fsm, 0, "B", 1, "");
			test_next(fsm, 0, "f", 4, "");
			test_next(fsm, 1, "C", 1, "");
			test_next(fsm, 1, "D", 3, "");
			test_next(fsm, 2, "firstABCD", Fsm<char>::ERROR_STATE, "");
			CHECK_LINEARGROUP(fsm, 3, "first", 2, "0");
		}

		//4. check embedded alternative
		{
			Nfa<char> nfa = Nfa<char>("fi") + (Nfa<char>("rst") | Nfa<char>("st"));
			nfa.set_output(0);
			Fsm<char> fsm(nfa);
			fsm.reduce();
			fsm.normalize();

			fsm.groupLinearPaths(1, 0);

			BOOST_CHECK_EQUAL(3, fsm.getFirstLinearSequence(0));

			test_next(fsm, 0, "f", 3, "");
			test_next(fsm, 1, "r", 4, "");
			test_next(fsm, 1, "s", 5, "");
			test_next(fsm, 2, "first", Fsm<char>::ERROR_STATE, "");

			CHECK_LINEARGROUP(fsm, 3, "i", 1, "");
			CHECK_LINEARGROUP(fsm, 4, "st", 2, "0");
			CHECK_LINEARGROUP(fsm, 5, "t", 2, "0");
		}

		//5. check option at end
		{
			Nfa<char> nfa = Nfa<char>("firs") + (Nfa<char>("t") | Nfa<char>("s"));
			nfa.set_output(0);
			Fsm<char> fsm(nfa);
			fsm.reduce();
			fsm.normalize();

			fsm.groupLinearPaths(1, 0);

			BOOST_CHECK_EQUAL(3, fsm.getFirstLinearSequence(0));

			test_next(fsm, 0, "f", 3, "");
			test_next(fsm, 1, "t", 2, "0");
			test_next(fsm, 1, "s", 2, "0");
			test_next(fsm, 2, "first", Fsm<char>::ERROR_STATE, "");

			CHECK_LINEARGROUP(fsm, 3, "irs", 1, "");
		}

		//6. check output in the middle
		{
			Nfa<char> nfa = Nfa<char>("fir") | Nfa<char>("first");
			nfa.set_output(0);
			Fsm<char> fsm(nfa);
			fsm.reduce();
			fsm.normalize();

			fsm.groupLinearPaths(1, 0);

			BOOST_CHECK_EQUAL(2, fsm.getFirstLinearSequence(0));

			test_next(fsm, 0, "f", 2, "");
			test_next(fsm, 1, "first", Fsm<char>::ERROR_STATE, "");
			CHECK_LINEARGROUP(fsm, 2, "ir", 4, "0");
			CHECK_LINEARGROUP(fsm, 4, "st", 1, "0");
		}
		//7. check common sequence with different sequential entry; check if entry transitions are still correct
		{
			Nfa<char> nfa = Nfa<char>("fsirst") | Nfa<char>("sfirst");
			nfa.set_output(0);
			Fsm<char> fsm(nfa);
			fsm.reduce();
			fsm.normalize();

			fsm.groupLinearPaths(1, 0);

			BOOST_CHECK_EQUAL(2, fsm.getFirstLinearSequence(0));

			test_next(fsm, 0, "f", 2, "");
			test_next(fsm, 0, "s", 7, "");
			test_next(fsm, 1, "first", Fsm<char>::ERROR_STATE, "");
			CHECK_LINEARGROUP(fsm, 2, "sirst", 1, "0");
			CHECK_LINEARGROUP(fsm, 7, "first", 1, "0");
		}
		//8. check linear sequence of sufficient length with second entry resulting in a sequence of insufficient length
		{
			Nfa<char> nfa = Nfa<char>("first") | Nfa<char>("rst");
			nfa.set_output(0);
			Fsm<char> fsm(nfa);
			fsm.reduce();
			fsm.normalize();

			fsm.groupLinearPaths(1, 4);

			BOOST_CHECK_EQUAL(2, fsm.getFirstLinearSequence(4));

			CHECK_LINEARGROUP(fsm, 2, "irst", 1, "0");
		}
		//9. check linear sequence with exit to the inside of another linear sequence
		{
			Nfa<char> nfa = Nfa<char>("first") | Nfa<char>("firstsecond") | Nfa<char>("sirstsecond");

			nfa.set_output(0);
			Fsm<char> fsm(nfa);
			fsm.reduce();
			fsm.normalize();

			fsm.groupLinearPaths(1, 0);

			BOOST_CHECK_EQUAL(2, fsm.getFirstLinearSequence(0));

			test_next(fsm, 0, "f", 2, "");
			test_next(fsm, 0, "s", 6, "");
			test_next(fsm, 1, "firstecond", Fsm<char>::ERROR_STATE, "");
			CHECK_LINEARGROUP(fsm, 2, "irst", 10, "0");
			CHECK_LINEARGROUP(fsm, 6, "irstsecond", 1, "0");
		}
		//10. check linear sequence with exit to start of another linear sequence
		{
			Nfa<char> nfa = Nfa<char>("first") | Nfa<char>("firstsecond");
			nfa.set_output(0);
			Fsm<char> fsm(nfa);
			fsm.reduce();
			fsm.normalize();

			fsm.groupLinearPaths(1, 0);

			BOOST_CHECK_EQUAL(2, fsm.getFirstLinearSequence(0));

			CHECK_LINEARGROUP(fsm, 2, "irst", 6, "0");
			CHECK_LINEARGROUP(fsm, 6, "second", 1, "0");
		}
	}

	// test disambiguate with fsm where smallest output is >0
	{
		Nfa<char> nfa_1 = Nfa<char>("ab");
		Nfa<char> nfa_2 = Nfa<char>("abc");

		nfa_1.set_output(1);
		nfa_2.set_output(2);

		Fsm<char> fsm(nfa_1|nfa_2);
		fsm.reduce();

		std::list<int> outputs;
		outputs.push_back(1);
		fsm.disambiguate(outputs);

		test_next(fsm, 0, "a", 1, "");
		test_next(fsm, 1, "b", 2, "1");
		test_next(fsm, 2, "abc", Fsm<char>::ERROR_STATE, "");
	}

	// test disambiguate with conflict of expected and unexpected output at first state
	{
		Nfa<char> nfa_0 = *(Nfa<char>("a")|Nfa<char>("c"));
		Nfa<char> nfa_1 = Nfa<char>("a");
		Nfa<char> nfa_2 = Nfa<char>("b");
		Nfa<char> nfa_3 = Nfa<char>("cc");

		nfa_0.set_output(0);
		nfa_1.set_output(1);
		nfa_2.set_output(2);
		nfa_3.set_output(3);

		Fsm<char> fsm(nfa_0 | nfa_1 | nfa_2 | nfa_3);
		fsm.reduce();

		std::list<int> outputs;
		outputs.push_back(1);
		outputs.push_back(3);
		fsm.disambiguate(outputs);
		fsm.normalize();

		test_next(fsm, 0, "a", 1, "1");
		test_next(fsm, 0, "b", 1, "2");
		test_next(fsm, 0, "c", 2, "0");
		test_next(fsm, 2, "c", 1, "3");
	}

	// test disambiguate with conflict of expected and unexpected output at first state
	{
		Nfa<char> nfa_11 = Nfa<char>("+")|Nfa<char>("-");
		Nfa<char> nfa_12 = Nfa<char>("+")|Nfa<char>("-")|Nfa<char>("A")|Nfa<char>("B")|Nfa<char>("C")|Nfa<char>("D")|Nfa<char>("N")|Nfa<char>("P");
		Nfa<char> nfa_35 = Nfa<char>("-");

		nfa_11.set_output(11);
		nfa_12.set_output(12);
		nfa_35.set_output(35);

		Fsm<char> fsm(nfa_11|nfa_12|nfa_35);
		fsm.reduce();

		std::list<int> outputs;
		outputs.push_back(3);
		outputs.push_back(4);
		outputs.push_back(35);
		fsm.disambiguate(outputs);

		Fsm<char>::State state(0);
		const BitSequence<int>& output = fsm.next(state, '-');

		BitSequence<int> expected;
		expected.set(35);

		BOOST_CHECK_EQUAL(expected, output);
	}

	// test disambiguate
	{
		Nfa<char> loops = *(Nfa<char>("ab") | Nfa<char>("d"));
		Nfa<char> nfa_0 = loops + Nfa<char>("a");
		Nfa<char> nfa_1 = loops + Nfa<char>("d");
		Nfa<char> nfa_2 = loops + (Nfa<char>("bc") | Nfa<char>("c")) + *Nfa<char>("c");
		Nfa<char> nfa_3 = loops + Nfa<char>("b") + *Nfa<char>("b");

		nfa_0.set_output(0);
		nfa_1.set_output(1);
		nfa_2.set_output(2);
		nfa_3.set_output(3);

		Fsm<char> fsm(nfa_0 | nfa_1 | nfa_2 | nfa_3);
		fsm.reduce();

		std::list<int> outputs;
		outputs.push_back(0);
		outputs.push_back(1);
		outputs.push_back(2);
		fsm.disambiguate(outputs);
		fsm.normalize();

		test_next(fsm, 0, "a", 1, "0");
		test_next(fsm, 0, "b", 2, "3");
		test_next(fsm, 0, "c", 3, "2");
		test_next(fsm, 0, "d", 4, "1");

		test_next(fsm, 1, "a", fsm.ERROR_STATE, "");
		test_next(fsm, 1, "b", 5, "");
		test_next(fsm, 1, "c", fsm.ERROR_STATE, "");
		test_next(fsm, 1, "d", fsm.ERROR_STATE, "");

		test_next(fsm, 2, "a", fsm.ERROR_STATE, "");
		test_next(fsm, 2, "b", 6, "3");
		test_next(fsm, 2, "c", 3, "2");
		test_next(fsm, 2, "d", fsm.ERROR_STATE, "");

		test_next(fsm, 3, "a", fsm.ERROR_STATE, "");
		test_next(fsm, 3, "b", fsm.ERROR_STATE, "");
		test_next(fsm, 3, "c", 3, "2");
		test_next(fsm, 3, "d", fsm.ERROR_STATE, "");

		test_next(fsm, 4, "a", 1, "0");
		test_next(fsm, 4, "b", fsm.ERROR_STATE, "");
		test_next(fsm, 4, "c", fsm.ERROR_STATE, "");
		test_next(fsm, 4, "d", 4, "1");

		test_next(fsm, 5, "a", 1, "0");
		test_next(fsm, 5, "b", fsm.ERROR_STATE, "");
		test_next(fsm, 5, "c", fsm.ERROR_STATE, "");
		test_next(fsm, 5, "d", 5, "");

		test_next(fsm, 6, "a", fsm.ERROR_STATE, "");
		// if disambiguate fsm accepts only minimal tokens no loop at end:
		//test_next(fsm, 6, "b", fsm.ERROR_STATE, "");
		// else if disambiguate fsm is greedy there's a loop at end:
		test_next(fsm, 6, "b", 6, "3");
		test_next(fsm, 6, "c", fsm.ERROR_STATE, "");
		test_next(fsm, 6, "d", fsm.ERROR_STATE, "");
	}

	// test getPotentialOutputs
	{
		Nfa<char> nfa_0 = *Nfa<char>("ab") + (Nfa<char>("a") | Nfa<char>("c"));
		Nfa<char> nfa_1 = *Nfa<char>("ab") + (Nfa<char>("b") | Nfa<char>("ca"));
		nfa_0.set_output(0);
		nfa_1.set_output(1);

		Fsm<char> fsm(nfa_0 | nfa_1);
		fsm.reduce();
		//std::map<int, std::set<int> > outputs;
		std::map<int, BitSequence<int> > outputs;
		fsm.getPotentialOutputs(outputs);
		//std::map<int, std::set<int> > expected;
		//expected[0].insert(0);
		//expected[0].insert(1);
		//expected[1].insert(0);
		//expected[1].insert(1);
		//expected[3].insert(1);
		std::map<int, BitSequence<int> > expected;
		//expected[0].resize(2, false);
		expected[0].set(0);
		expected[0].set(1);
		//expected[1].resize(2, false);
		expected[1].set(0);
		expected[1].set(1);
		//expected[3].resize(2, false);
		expected[3].set(1);

		//BOOST_CHECK_EQUAL_COLLECTIONS
		for (std::map<int, BitSequence<int> >::const_iterator iter = expected.begin(); iter != expected.end(); ++iter)
		{
			std::map<int, BitSequence<int> >::const_iterator iOutputs = outputs.find(iter->first);
			if (iOutputs != outputs.end())
			{
				//BOOST_CHECK_EQUAL(iter->first, iter->first);
				BOOST_CHECK_EQUAL(iter->second, iOutputs->second);
				//BOOST_CHECK_EQUAL_COLLECTIONS(iter->second.begin(), iter->second.end(),
				//							iOutputs->second.begin(), iOutputs->second.end());
			}
			else
				BOOST_ERROR("state missing");
		}
	}

	// test strict strip
	{
		Nfa<char> nfa_0 = *Nfa<char>("ab") + (Nfa<char>("a") | Nfa<char>("c"));
		Nfa<char> nfa_1 = *Nfa<char>("ab") + (Nfa<char>("b") | Nfa<char>("ca"));
		nfa_0.set_output(0);
		nfa_1.set_output(1);

		Fsm<char> fsm(nfa_0 | nfa_1);
		fsm.reduce();
		test_output(fsm, "01");
		//std::set<int> outputs;
		//outputs.insert(0);
		BitSequence<int> outputs(0, 2);
		outputs.set(0);
		fsm.strip(outputs, true);
		test_output(fsm, "0");
		BOOST_CHECK_EQUAL(3, fsm.getNumberOfStates());
		test_next(fsm, 0, "a", 1, "0");
		test_next(fsm, 0, "b", fsm.ERROR_STATE, "");
		test_next(fsm, 0, "c", 2, "0");

		test_next(fsm, 1, "a", fsm.ERROR_STATE, "");
		test_next(fsm, 1, "b", 0, "");
		test_next(fsm, 1, "c", fsm.ERROR_STATE, "");

		test_next(fsm, 2, "a", fsm.ERROR_STATE, "");
		test_next(fsm, 2, "b", fsm.ERROR_STATE, "");
		test_next(fsm, 2, "c", fsm.ERROR_STATE, "");

		/*BOOST_CHECK(!fsm.final(0));
		BOOST_CHECK(fsm.final(1));
		BOOST_CHECK(fsm.final(2));
		BOOST_CHECK_EQUAL(0, fsm.output(1));
		BOOST_CHECK_EQUAL(0, fsm.output(2));
		BOOST_CHECK_EQUAL(1, fsm.next(0, 'a'));
		BOOST_CHECK_EQUAL(0, fsm.next(1, 'b'));
		BOOST_CHECK_EQUAL(2, fsm.next(0, 'c'));*/
	}

	// test mergeDemux
	{
		Nfa<char> nfa_cc_0("cc"); nfa_cc_0.set_output(0);
		Nfa<char> nfa_cc_1("cc"); nfa_cc_1.set_output(1);
		Nfa<char> nfa_ab("ab"); nfa_ab.set_output(1);
		Nfa<char> nfa_b_0("b"); nfa_b_0.set_output(0);
		Nfa<char> nfa_b_1("b"); nfa_b_1.set_output(1);
		std::vector<Fsm<char> > fsms;
		fsms.push_back(nfa_cc_0 | nfa_ab | nfa_b_0);
		fsms.push_back(nfa_cc_1 | nfa_ab | nfa_b_1);
		Fsm<char> fsm;
		fsm.mergeDemux(fsms);

		test_output(fsm, "01");
		BOOST_CHECK_EQUAL(6, fsm.getNumberOfStates());

		test_next(fsm, 0, "a", 2, "");
		test_next(fsm, 0, "b", 3, "0");
		test_next(fsm, 0, "c", 4, "");

		test_next(fsm, 1, "a", 2, "");
		test_next(fsm, 1, "b", 3, "1");
		test_next(fsm, 1, "c", 5, "");

		test_next(fsm, 2, "a", fsm.ERROR_STATE, "");
		test_next(fsm, 2, "b", 3, "1");
		test_next(fsm, 2, "c", fsm.ERROR_STATE, "");

		test_next(fsm, 3, "a", fsm.ERROR_STATE, "");
		test_next(fsm, 3, "b", fsm.ERROR_STATE, "");
		test_next(fsm, 3, "c", fsm.ERROR_STATE, "");

		test_next(fsm, 4, "a", fsm.ERROR_STATE, "");
		test_next(fsm, 4, "b", fsm.ERROR_STATE, "");
		test_next(fsm, 4, "c", 3, "0");

		test_next(fsm, 5, "a", fsm.ERROR_STATE, "");
		test_next(fsm, 5, "b", fsm.ERROR_STATE, "");
		test_next(fsm, 5, "c", 3, "1");
	}

	// test prefixes
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
		test_output(fsm, "012345");

		// just a positive check for transitions
		test_next(fsm, 0, "a", 1, "1");
		test_next(fsm, 0, "b", 2, "2");
		test_next(fsm, 1, "b", 3, "0");
		test_next(fsm, 3, "c", 2, "03");
		test_next(fsm, 3, "d", 4, "");
		test_next(fsm, 4, "c", 5, "45");
		test_next(fsm, 5, "a", 6, "");
		test_next(fsm, 6, "b", 7, "");
		test_next(fsm, 7, "d", 4, "");

		std::map<int, BitSequence<int> > expectedPrefixes; //(6, BitSequence<int>(0, 6));
		expectedPrefixes[0].set(1);
		expectedPrefixes[0].set(3);
		expectedPrefixes[3].set(1);
		expectedPrefixes[4].set(0);
		expectedPrefixes[4].set(1);
		expectedPrefixes[4].set(5);
		expectedPrefixes[5].set(0);
		expectedPrefixes[5].set(1);

		/*expectedPrefixes[0].insert(1);
		expectedPrefixes[0].insert(3);
		expectedPrefixes[3].insert(1);
		expectedPrefixes[4].insert(0);
		expectedPrefixes[4].insert(1);
		expectedPrefixes[4].insert(5);
		expectedPrefixes[5].insert(0);
		expectedPrefixes[5].insert(1);*/

		test_getStatePrefixes(fsm, 0, "");
		test_getStatePrefixes(fsm, 1, "1");
		test_getStatePrefixes(fsm, 2, "0123");
		test_getStatePrefixes(fsm, 3, "01");
		test_getStatePrefixes(fsm, 4, "0145");
		test_getStatePrefixes(fsm, 5, "0145");
		test_getStatePrefixes(fsm, 6, "0145");
		test_getStatePrefixes(fsm, 7, "0145");

		test_getPrefixAssociations(fsm, expectedPrefixes);
	}
	{
		Nfa<char> nfa_b(Alphabet<char>('b'));
		Nfa<char> nfa_c(Alphabet<char>('c'));

		nfa_b.set_output(0);
		nfa_c.set_output(1);

		Nfa<char> nfa_bc = nfa_b | nfa_c;

		Fsm<char> fsm(nfa_bc);

		BOOST_CHECK_EQUAL(3, fsm.getNumberOfStates());
		test_next(fsm, 0, "b", 1, "0");
		test_next(fsm, 0, "c", 2, "1");
		test_next(fsm, 1, "b", fsm.ERROR_STATE, "");
		test_next(fsm, 1, "c", fsm.ERROR_STATE, "");
		test_next(fsm, 2, "b", fsm.ERROR_STATE, "");
		test_next(fsm, 2, "c", fsm.ERROR_STATE, "");

		fsm.reduce();
		test_output(fsm, "01");
		// test accepted symbols of state 0
		// create alphabet of expected input for state 0
		Alphabet<char> expectedInput;
		expectedInput.add('b');
		expectedInput.add('c');
		test_getAcceptedSymbols(fsm, 0, expectedInput);

		BOOST_CHECK_EQUAL(2, fsm.getNumberOfStates());
		test_next(fsm, 0, "b", 1, "0");
		test_next(fsm, 1, "c", fsm.ERROR_STATE, "");
		test_next(fsm, 0, "c", 1, "1");
		test_next(fsm, 1, "b", fsm.ERROR_STATE, "");

		// test isAcceptingState
		BOOST_CHECK(fsm.isAcceptingState(0));
		BOOST_CHECK(!fsm.isAcceptingState(1));
	}

	{
		Nfa<char> nfa_0(Alphabet<char>('0'));
		Nfa<char> nfa_1(Alphabet<char>('1'));
		Nfa<char> nfa_2(Alphabet<char>('2'));
		Nfa<char> nfa_a(Alphabet<char>('a'));
		Nfa<char> nfa_b(Alphabet<char>('b'));
		Nfa<char> nfa_c(Alphabet<char>('c'));
		Nfa<char> nfa_s(Alphabet<char>('s'));
		Nfa<char> nfa_f(Alphabet<char>('f'));
		Nfa<char> nfa_dot(Alphabet<char>('.'));
		Alphabet<char> a(Interval<char>(std::numeric_limits<char>::min(), std::numeric_limits<char>::max()));
		a.del('0', '9');
		Nfa<char> nfa_09(a);
		Nfa<char> nfa0 = nfa_b+nfa_c+nfa_a | nfa_b+*nfa_c;
		nfa0.set_output(0);
		Nfa<char> nfa1 = *(nfa_0|nfa_1|nfa_2)+nfa_dot+(nfa_s+nfa_f);
		nfa1.set_output(1);
		nfa_09.set_output(2);
		Nfa<char> nfa_all = nfa0|nfa1|nfa_09;

		Fsm<char> fsm(nfa_all);
		fsm.reduce();
		test_output(fsm, "012");
		// test accepted symbols of state 0
		// create alphabet of expected input for state 0
		Alphabet<char> expectedInput;
		// add input from nfa_09
		expectedInput.add(std::numeric_limits<char>::min(), '0'-1);
		expectedInput.add('9'+1, std::numeric_limits<char>::max());
		// add input from nfa0
		expectedInput.add('b');
		// add input from nfa1
		expectedInput.add('0');
		expectedInput.add('1');
		expectedInput.add('2');
		expectedInput.add('.');
		test_getAcceptedSymbols(fsm, 0, expectedInput);

		// test isAcceptingState
		BOOST_CHECK(fsm.isAcceptingState(0));

		// check number of states
		BOOST_CHECK_EQUAL(8, fsm.getNumberOfStates());

		// check transitions from state 0
		test_next(fsm, 0, "-acdfs", 1, "2");
		test_next(fsm, 0, ".", 2, "2");
		test_next(fsm, 0, "012", 3, "");
		test_next(fsm, 0, "b", 4, "02");
		test_next(fsm, 0, "3", fsm.ERROR_STATE, "");
		// check transitions from state 1
		test_next(fsm, 1, "-abcdfs.0123", fsm.ERROR_STATE, "");
		// check transitions from state 2
		test_next(fsm, 2, "s", 5, "");
		test_next(fsm, 2, "-abcdf.0123", fsm.ERROR_STATE, "");
		// check transitions from state 3
		test_next(fsm, 3, ".", 2, "");
		test_next(fsm, 3, "012", 3, "");
		test_next(fsm, 3, "-abcdfs3", fsm.ERROR_STATE, "");
		// check transitions from state 4
		test_next(fsm, 4, "c", 6, "0");
		test_next(fsm, 4, "-abdfs.0123", fsm.ERROR_STATE, "");
		// check transitions from state 5
		test_next(fsm, 5, "f", 1, "1");
		test_next(fsm, 5, "-abcds.0123", fsm.ERROR_STATE, "");
		// check transitions from state 6
		test_next(fsm, 6, "a", 1, "0");
		test_next(fsm, 6, "c", 7, "0");
		test_next(fsm, 6, "-bdfs.0123", fsm.ERROR_STATE, "");
		// check transitions from state 7
		test_next(fsm, 7, "c", 7, "0");
		test_next(fsm, 7, "-abdfs.0123", fsm.ERROR_STATE, "");
	}

	// test non strict strip
	{
		Nfa<char> nfa_0 = *Nfa<char>("ab") + (Nfa<char>("a") | Nfa<char>("c"));
		Nfa<char> nfa_1 = *Nfa<char>("ab") + (Nfa<char>("b") | Nfa<char>("ca"));
		nfa_0.set_output(0);
		nfa_1.set_output(1);

		Fsm<char> fsm(nfa_0 | nfa_1);
		fsm.reduce();
		test_output(fsm, "01");
		//std::set<int> outputs;
		//outputs.insert(0);
		BitSequence<int> outputs(0, 2);
		outputs.set(0);
		fsm.strip(outputs, false);
		test_output(fsm, "01");
		BOOST_CHECK_EQUAL(4, fsm.getNumberOfStates());

		test_next(fsm, 0, "a", 3, "0");
		test_next(fsm, 0, "b", 1, "1");
		test_next(fsm, 0, "c", 1, "0");

		test_next(fsm, 1, "a", fsm.ERROR_STATE, "");
		test_next(fsm, 1, "b", fsm.ERROR_STATE, "");
		test_next(fsm, 1, "c", fsm.ERROR_STATE, "");

		test_next(fsm, 2, "a", 3, "0");
		test_next(fsm, 2, "b", fsm.ERROR_STATE, "");
		test_next(fsm, 2, "c", 1, "0");

		test_next(fsm, 3, "a", fsm.ERROR_STATE, "");
		test_next(fsm, 3, "b", 2, "");
		test_next(fsm, 3, "c", fsm.ERROR_STATE, "");
	}
}

void test_getAcceptedSymbols(const Fsm<char>& fsm, Fsm<char>::State state, const Alphabet<char>& expectedInput)
{
	// add symbols from fsm.input() contained in expectedInput to expectedSymbols
	std::set<char> expectedSymbols;
	std::set<char> input = fsm.input().lower_bound();
	for(std::set<char>::const_iterator i = input.begin(); i != input.end(); ++i)
		if (expectedInput.contains(*i))
			expectedSymbols.insert(*i);
	// test for all expectedSymbols
	std::set<char> symbols;
	fsm.getAcceptedSymbols(state, symbols);
	for(std::set<char>::const_iterator i = expectedSymbols.begin(); i != expectedSymbols.end(); ++i)
		BOOST_CHECK(symbols.erase(*i) > 0);

	BOOST_CHECK(symbols.empty());
}

/*static bool checkEqualSets(const std::pair<int, std::set<int> >& lhs, const std::pair<int, std::set<int> >& rhs)
{
	BOOST_CHECK_EQUAL(lhs.first, rhs.first);
	BOOST_CHECK_EQUAL_COLLECTIONS(lhs.second.begin(), lhs.second.end(), rhs.second.begin(), rhs.second.end());
	return true;
}*/

void test_getPrefixAssociations(const Fsm<char>& fsm, const std::map<int, BitSequence<int> >& expectedPrefixes)
{
	//std::map<int, std::set<int> > prefixes;
	std::map<int, BitSequence<int> > prefixes;
	fsm.getPrefixAssociations(prefixes);
	//size_t act_size = prefixes.size();
	//size_t exp_size = expectedPrefixes.size();
	//BOOST_REQUIRE_EQUAL(act_size, exp_size);
	for (std::map<int, BitSequence<int> >::const_iterator i = expectedPrefixes.begin(); i != expectedPrefixes.end(); ++i)
	{
		std::map<int, BitSequence<int> >::const_iterator ip = prefixes.find(i->first);
		if (ip != prefixes.end())
		{
			BOOST_CHECK_EQUAL(i->second, ip->second);
			prefixes.erase(ip->first);
		}
		else
			BOOST_CHECK(i->second.empty());
	}

	for (std::map<int, BitSequence<int> >::const_iterator i = prefixes.begin(); i != prefixes.end(); ++i)
		BOOST_CHECK(i->second.empty());

	//BOOST_CHECK_EQUAL_COLLECTIONS(prefixes.begin(), prefixes.end(), expectedPrefixes.begin(), expectedPrefixes.end());
	//std::equal(prefixes.begin(), prefixes.end(), expectedPrefixes.begin(), checkEqualSets);
}

void test_getStatePrefixes(const Fsm<char>& fsm, Fsm<char>::State state, const char *expectedPrefixes)
{
	BitSequence<int> expected;
	while(*expectedPrefixes) expected.set(*(expectedPrefixes++) - '0');
	std::map<int, BitSequence<int> > statePrefixes;
	fsm.getStatePrefixes(statePrefixes);
	BOOST_CHECK_EQUAL(fsm.getNumberOfStates(), static_cast<Fsm<char>::State>(statePrefixes.size()));
	BOOST_REQUIRE(state < fsm.getNumberOfStates());
	//BOOST_CHECK_EQUAL_COLLECTIONS(expected.begin(), expected.end(), statePrefixes[state].begin(), statePrefixes[state].end());
	BOOST_CHECK_EQUAL(expected, statePrefixes[state]);
}

void test_next(const Fsm<char>& fsm, Fsm<char>::State state, const char *symbols, Fsm<char>::State nextState, const char* expectedOutput)
{
	BitSequence<int> outs;
	//while (*expectedOutput) outs.insert(*(expectedOutput++) - '0');
	while (*expectedOutput) outs.set(*(expectedOutput++) - '0');

	while (*symbols)
	{
		Fsm<char>::State tmpState = state;
		const BitSequence<int>& output = fsm.next(tmpState, *symbols);
		BOOST_CHECK_EQUAL(tmpState, nextState);
		//BOOST_CHECK_EQUAL_COLLECTIONS(outs.begin(), outs.end(), output.begin(), output.end());
		BOOST_CHECK_EQUAL(outs, output);
		++symbols;
	}
}

void test_output(const Fsm<char>& fsm, const char* expectedOutput)
{
	//std::set<int> outs;
	//while (*expectedOutput) outs.insert(*(expectedOutput++) - '0');
	//std::set<int> output;
	BitSequence<int> output;
	//size_t len = strlen(expectedOutput);
	//int minEl = fsm.getMinOutput();
	BitSequence<int> outs;
	while (*expectedOutput) outs.set(*(expectedOutput++) - '0');

	fsm.getOutput(output);
	//BOOST_CHECK_EQUAL_COLLECTIONS(outs.begin(), outs.end(), output.begin(), output.end());
	BOOST_CHECK_EQUAL(outs, output);
}

//void test_strip(const Fsm<char>& fsm, const std::


/*
using namespace std;

void TestFsm::printTable() const
{

	for(size_t i = 0; i < transition_table.size(); ++i)
	{
		std::map<char,State>::const_iterator t;
		for(t = transition_table[i].begin(); t != transition_table[i].end(); ++t)
			cout << "d(" << static_cast<int>(i) << "," << t->first << ") = " << t->second << endl;
	}

	std::map<State,std::set<int> >::const_iterator f;
	for(f = final_states.begin(); f != final_states.end(); ++f)
	{
		cout << "(" << f->first << ", ";
		std::set<int>::const_iterator oi;
		for(oi = f->second.begin(); oi != f->second.end(); ++oi)
			cout << *oi << " ";
		cout << "); ";
	}

	std::cout << std::endl;
	this->alphabet.write(std::cout);

	cout << endl;
}*/
