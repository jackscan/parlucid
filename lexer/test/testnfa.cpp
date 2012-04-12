//#include "testnfa.hpp"

#include "../src/nfa.hpp"

#include <iostream>
#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_CASE(test_nfa)
{
	// test inverse of nfa w/o output
	/*{
		Nfa<char> nfa_a(Alphabet<char>('a'));
		Nfa<char> nfa_b(Alphabet<char>('b'));
		Nfa<char> nfa_inv = ~(nfa_a + nfa_b);

		BOOST_CHECK(!nfa_inv.final(0));
		BOOST_CHECK(!nfa_inv.final(1));
		BOOST_CHECK(!nfa_inv.final(2));
		BOOST_CHECK(nfa_inv.final(3));
		BOOST_CHECK_EQUAL(1, nfa_inv.next(0, 'a').size());
		BOOST_CHECK_EQUAL(1, nfa_inv.next(0, 'a').count(1));
		BOOST_CHECK_EQUAL(1, nfa_inv.next(0, 'b').size());
		BOOST_CHECK_EQUAL(1, nfa_inv.next(0, 'b').count(3));
	}
	// test nfa and inverse combined
	{
		Nfa<char> nfa_a(Alphabet<char>('a'));
		Nfa<char> nfa_b(Alphabet<char>('b'));
		Nfa<char> nfa_ab = nfa_a + nfa_b;
		Nfa<char> nfa_inv = ~nfa_ab;

		nfa_ab.set_output(0);
		nfa_inv.set_output(1);

		std::list<Nfa<char> > nfas;
		nfas.push_back(nfa_ab);
		nfas.push_back(nfa_inv);

		Nfa<char> nfa_com = Nfa<char>::combine(nfas);

		BOOST_CHECK(nfa_com.final(nfa_com.next(nfa_com.next(0, 'a'), 'b')));
		BOOST_CHECK(0 < nfa_com.output(nfa_com.next(nfa_com.next(0, 'a'), 'b')).count(0));
		BOOST_CHECK_EQUAL(0, nfa_com.next(nfa_com.next(nfa_com.next(0, 'a'), 'b'), 'a').size());
		BOOST_CHECK_EQUAL(0, nfa_com.next(nfa_com.next(nfa_com.next(0, 'a'), 'b'), 'b').size());

		BOOST_CHECK(!nfa_com.final(nfa_com.next(nfa_com.next(0, 'a'), 'a')));
		BOOST_CHECK(!nfa_com.final(nfa_com.next(nfa_com.next(nfa_com.next(0, 'b'), 'a'), 'a')));
		BOOST_CHECK(!nfa_com.final(nfa_com.next(nfa_com.next(nfa_com.next(0, 'b'), 'a'), 'b')));

	}
	// test inverse
	{
		Nfa<char> nfa_b(Alphabet<char>('b'));
		Nfa<char> nfa_c(Alphabet<char>('c'));
		nfa_b.set_output(0);
		nfa_c.set_output(1);
		Nfa<char> nfa_bc = nfa_c + (nfa_b | nfa_c);

		BOOST_CHECK(!nfa_bc.final(0));
		BOOST_CHECK(!nfa_bc.final(1));
		BOOST_CHECK(nfa_bc.final(2));
		BOOST_CHECK(nfa_bc.final(3));

		Nfa<char> nfa_inv = ~nfa_bc;

		BOOST_CHECK(!nfa_inv.final(0));
		BOOST_CHECK(!nfa_inv.final(1));
		BOOST_CHECK(!nfa_inv.final(2));
		BOOST_CHECK(!nfa_inv.final(3));
		BOOST_CHECK(nfa_inv.final(4));
	}*/

	{
		Nfa<char> nfa_a(Alphabet<char>('a'));
		Nfa<char> nfa_b(Alphabet<char>('b'));
		Nfa<char> nfa_c(Alphabet<char>('c'));
		Nfa<char> nfa_r = (*nfa_a + nfa_b + *nfa_c).reverse();

		std::string accepted[] = { "b", "ba", "cb", "cbaa", "ccba" };
		std::string notaccepted[] = { "c", "a", "ca", "bc", "cc", "abcab" };

		for (size_t is = 0; is < sizeof(accepted) / sizeof(accepted[0]); ++is)
		{
			std::set<Nfa<char>::State> state;
			state.insert(0);
			for (size_t ic = 0; ic < accepted[is].size(); ++ic)
				state = nfa_r.next(state, accepted[is][ic]);
			BOOST_CHECK_MESSAGE(nfa_r.final(state), (accepted[is] + " not accepted").c_str());
		}

		for (size_t is = 0; is < sizeof(notaccepted) / sizeof(notaccepted[0]); ++is)
		{
			std::set<Nfa<char>::State> state;
			state.insert(0);
			for (size_t ic = 0; ic < notaccepted[is].size(); ++ic)
				state = nfa_r.next(state, notaccepted[is][ic]);
			BOOST_CHECK_MESSAGE(!nfa_r.final(state), (notaccepted[is] + " accepted").c_str());
		}
	}

	{
		Nfa<char> nfa_b(Alphabet<char>('b'));
		Nfa<char> nfa_c(Alphabet<char>('c'));

		nfa_b.set_output(0);
		nfa_c.set_output(1);

		BOOST_CHECK_EQUAL(nfa_b.final(1), true);
		BOOST_CHECK_EQUAL(nfa_b.next(0, 'b').size(), 1u);
		BOOST_CHECK_EQUAL(*nfa_b.next(0, 'b').begin(), 1);
		BOOST_CHECK_EQUAL(nfa_c.final(1), true);
		BOOST_CHECK_EQUAL(nfa_c.next(0, 'c').size(), 1u);
		BOOST_CHECK_EQUAL(*nfa_c.next(0, 'c').begin(), 1);

		Nfa<char> nfa_bc = nfa_b + nfa_c;
		BOOST_CHECK_EQUAL(nfa_bc.next(0, 'b').size(), 1u);
		BOOST_CHECK_EQUAL(*nfa_bc.next(0, 'b').begin(), 1);
		BOOST_CHECK_EQUAL(nfa_bc.next(1, 'c').size(), 1u);
		BOOST_CHECK_EQUAL(*nfa_bc.next(1, 'c').begin(), 2);

		BOOST_CHECK_EQUAL(nfa_bc.final(2), true);
	}
	{
		Nfa<char> nfa_b(Alphabet<char>('b'));
		Nfa<char> nfa_c(Alphabet<char>('c'));

		nfa_b.set_output(0);
		nfa_c.set_output(1);

		Nfa<char> nfa_bc = nfa_b | nfa_c;

		BOOST_CHECK_EQUAL(nfa_bc.final(1), true);
		BOOST_CHECK_EQUAL(nfa_bc.final(2), true);

	}
	{
		Nfa<char> nfa_b(Alphabet<char>('b'));
		Nfa<char> nfa_c(Alphabet<char>('c'));

		nfa_b.set_output(0);
		nfa_c.set_output(1);

		Nfa<char> nfa_bc = nfa_b + *nfa_c;
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
		Nfa<char> nfa0 = nfa_b+nfa_c+nfa_a|nfa_b+*nfa_c;
		nfa0.set_output(0);
		Nfa<char> nfa1 = *(nfa_0|nfa_1|nfa_2)+nfa_dot+(nfa_s+nfa_f);
		nfa1.set_output(1);
		Nfa<char> nfa01 = nfa0|nfa1;
	}
	{
		Nfa<char> nfa(Nfa<char>("abc"));
		Nfa<char> e;
		BOOST_CHECK(!nfa.empty());
		BOOST_CHECK(e.empty());
		BOOST_CHECK(!(nfa|e).empty());
	}
	{
		Nfa<char> nfa_a(Alphabet<char>('a'));
		Nfa<char> nfa_b(Alphabet<char>('b'));
		Nfa<char> nfa_c(Alphabet<char>('c'));

		nfa_a.set_output(0);
		nfa_b.set_output(1);
		nfa_c.set_output(2);

		Nfa<char> nfa_abc = nfa_a | nfa_b | nfa_c;


		std::list<Nfa<char> > nfas;
		nfas.push_back(nfa_a); nfas.push_back(nfa_b); nfas.push_back(nfa_c);
		Nfa<char> nfa_abc2 = Nfa<char>::combine(nfas);
	}
}
/*
using namespace std;

void TestNfa::printTable() const
{
	cout << "states: " << this->num_states << endl;
	std::map<State,std::map<char,std::set<State> > >::const_iterator curr;
	for(curr = transition_table.begin(); curr != transition_table.end(); ++curr)
	{
		std::map<char,std::set<State> >::const_iterator t;
		for(t = curr->second.begin(); t != curr->second.end(); ++t)
		{
			std::set<State>::const_iterator s;
			for(s = t->second.begin(); s != t->second.end(); ++s)
				cout << "d(" << curr->first << "," << t->first << ") = " << *s << endl;
		}
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
	cout << endl;
}*/
