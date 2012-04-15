#include "../src/alphabet.hpp"

#include <limits>
#include <iostream>
#include <set>
#include <string>

template<typename T>
std::ostream& operator<<(std::ostream& os,  const std::set<T>& rhs);

template<typename T>
std::ostream& operator<<(std::ostream& os,  const Interval<T>& rhs);

#include <boost/test/unit_test.hpp>

typedef unsigned short ushort;

template<typename T>
class TestAlphabet: public Alphabet<T>
{
public:
	TestAlphabet(): Alphabet<T>() {}
	TestAlphabet(const T& symbol): Alphabet<T>(symbol) {}
	TestAlphabet(const T& first, const T& last): Alphabet<T>(first, last) {}
	TestAlphabet(const Alphabet<T>& a): Alphabet<T>(a) {}
};

BOOST_AUTO_TEST_CASE(test_ctor)
{
	{
		std::string str("ab\n");
		Alphabet<char> a(str.begin(), str.end());
		BOOST_CHECK_EQUAL(a.singletons().size(), 3u);
		BOOST_CHECK(a.contains('a'));
		BOOST_CHECK(a.contains('b'));
		BOOST_CHECK(a.contains('\n'));
	}
	{
		std::string str("a\0b", 3);
		Alphabet<char> a(str.begin(), str.end());
		BOOST_CHECK_EQUAL(a.singletons().size(), 3u);
		BOOST_CHECK(a.contains('a'));
		BOOST_CHECK(a.contains('b'));
		BOOST_CHECK(a.contains('\0'));
	}
}

BOOST_AUTO_TEST_CASE(test_roundBoundaries)
{
	{
		std::set<Interval<char> > spaces;
		spaces.insert(Interval<char>(' ', ' '));
		spaces.insert(Interval<char>('"', '"'));
		spaces.insert(Interval<char>('/', '/'));
		spaces.insert(Interval<char>(':', ';'));
		spaces.insert(Interval<char>('@', '@'));
		spaces.insert(Interval<char>('[', '['));
		spaces.insert(Interval<char>('_', '_'));
		spaces.insert(Interval<char>('{', '{'));
		spaces.insert(Interval<char>('}', '}'));

		Alphabet<char> a;
		a.add('\t', '\r');
		a.add('A', 'Z');
		a.add('a', 'z');

		a.roundBoundaries(spaces);
		BOOST_CHECK_EQUAL(a.intervals().size(), 3u);
		BOOST_CHECK_EQUAL(a.upper_bound('\t'), '\r');
		BOOST_CHECK_EQUAL(a.lower_bound('\r'), '\t');
		BOOST_CHECK_EQUAL(a.upper_bound('@'), '[');
		BOOST_CHECK_EQUAL(a.lower_bound('['), '@');
		BOOST_CHECK_EQUAL(a.upper_bound('a'), '{');
		BOOST_CHECK_EQUAL(a.lower_bound('{'), 'a');
	}
	{
		std::set<Interval<char> > spaces;
		char cmin = std::numeric_limits<char>::min();
		char cmax = std::numeric_limits<char>::max();
		spaces.insert(Interval<char>(cmin, cmax));
		Alphabet<char> a;

		a.add('0', '1');
		a.roundBoundaries(spaces);
		BOOST_CHECK_EQUAL(a.intervals().size(), 0u);
	}
	{
		std::set<Interval<char> > spaces;
		spaces.insert(Interval<char>('.', '/'));
		spaces.insert(Interval<char>('b', 'b'));
		Alphabet<char> a;

		char cmin = std::numeric_limits<char>::min();
		char cmax = std::numeric_limits<char>::max();

		a.add(cmin, '-');
		a.add('0', '2');
		a.add(':', 'a');
		a.add('c', cmax);

		a.roundBoundaries(spaces);

		BOOST_CHECK_EQUAL(a.intervals().size(), 4u);
		BOOST_CHECK_EQUAL(a.upper_bound(cmin), '/');
		BOOST_CHECK_EQUAL(a.lower_bound('2'), '0');
		BOOST_CHECK_EQUAL(a.upper_bound('0'), '2');
		BOOST_CHECK_EQUAL(a.lower_bound('a'), ':');
		BOOST_CHECK_EQUAL(a.upper_bound(':'), 'a');
		BOOST_CHECK_EQUAL(a.lower_bound(cmax), 'b');
		BOOST_CHECK_EQUAL(a.upper_bound('b'), cmax);

	}
	// test decreasing intervals
	{
		std::set<Interval<char> > spaces;
		spaces.insert(Interval<char>('c', 'e'));
		spaces.insert(Interval<char>('3', '5'));
		Alphabet<char> a;

		a.add('a', 'e');
		a.add('3', '6');

		a.roundBoundaries(spaces);

		BOOST_CHECK_EQUAL(a.intervals().size(), 2u);
		BOOST_CHECK_EQUAL(a.lower_bound('c'), 'a');
		BOOST_CHECK_EQUAL(a.upper_bound('a'), 'c');
		BOOST_CHECK_EQUAL(a.lower_bound('6'), '4');
		BOOST_CHECK_EQUAL(a.upper_bound('4'), '6');
	}
	// test correct rounding
	{
		std::set<Interval<char> > spaces;
		spaces.insert(Interval<char>('4', '5'));
		Alphabet<char> a;

		a.add('5', '6');

		a.roundBoundaries(spaces);

		BOOST_CHECK_EQUAL(a.intervals().size(), 1u);
		BOOST_CHECK_EQUAL(a.lower_bound('6'), '4');
		BOOST_CHECK_EQUAL(a.upper_bound('4'), '6');
	}
	// test uneffective spaces
	{
		std::set<Interval<char> > spaces;
		spaces.insert(Interval<char>('4', '5'));
		Alphabet<char> a;

		a.add('3', '6');

		a.roundBoundaries(spaces);

		BOOST_CHECK_EQUAL(a.intervals().size(), 1u);
		BOOST_CHECK_EQUAL(a.lower_bound('6'), '3');
		BOOST_CHECK_EQUAL(a.upper_bound('3'), '6');
	}
	// test interval deletion
	{
		std::set<Interval<char> > spaces;
		spaces.insert(Interval<char>('3', '6'));
		spaces.insert(Interval<char>('a', 'c'));
		Alphabet<char> a;

		a.add('4', '5');
		a.add('a', 'c');

		a.roundBoundaries(spaces);

		BOOST_CHECK_EQUAL(a.intervals().size(), 0u);
	}
}

BOOST_AUTO_TEST_CASE(test_roundUnsignedBoundaries)
{
	{
		std::set<Interval<unsigned char> > spaces;
		spaces.insert(Interval<unsigned char>(' ', ' '));
		spaces.insert(Interval<unsigned char>('"', '"'));
		spaces.insert(Interval<unsigned char>('/', '/'));
		spaces.insert(Interval<unsigned char>(':', ';'));
		spaces.insert(Interval<unsigned char>('@', '@'));
		spaces.insert(Interval<unsigned char>('[', '['));
		spaces.insert(Interval<unsigned char>('_', '_'));
		spaces.insert(Interval<unsigned char>('{', '{'));
		spaces.insert(Interval<unsigned char>('}', '}'));

		Alphabet<unsigned char> a;
		a.add('\t', '\r');
		a.add('A', 'Z');
		a.add('a', 'z');

		a.roundBoundaries(spaces);
		BOOST_CHECK_EQUAL(a.intervals().size(), 3u);
		BOOST_CHECK_EQUAL(a.upper_bound('\t'), '\r');
		BOOST_CHECK_EQUAL(a.lower_bound('\r'), '\t');
		BOOST_CHECK_EQUAL(a.upper_bound('@'), '[');
		BOOST_CHECK_EQUAL(a.lower_bound('['), '@');
		BOOST_CHECK_EQUAL(a.upper_bound('a'), '{');
		BOOST_CHECK_EQUAL(a.lower_bound('{'), 'a');
	}
	{
		std::set<Interval<unsigned char> > spaces;
		unsigned char cmin = std::numeric_limits<unsigned char>::min();
		unsigned char cmax = std::numeric_limits<unsigned char>::max();
		spaces.insert(Interval<unsigned char>(cmin, cmax));
		Alphabet<unsigned char> a;

		a.add('0', '1');
		a.roundBoundaries(spaces);
		BOOST_CHECK_EQUAL(a.intervals().size(), 0u);
	}
	{
		std::set<Interval<unsigned char> > spaces;
		spaces.insert(Interval<unsigned char>('.', '/'));
		spaces.insert(Interval<unsigned char>('b', 'b'));
		Alphabet<unsigned char> a;

		unsigned char cmin = std::numeric_limits<unsigned char>::min();
		unsigned char cmax = std::numeric_limits<unsigned char>::max();

		a.add(cmin, '-');
		a.add('0', '2');
		a.add(':', 'a');
		a.add('c', cmax);

		a.roundBoundaries(spaces);

		BOOST_CHECK_EQUAL(a.intervals().size(), 4u);
		BOOST_CHECK_EQUAL(a.upper_bound(cmin), '/');
		BOOST_CHECK_EQUAL(a.lower_bound('2'), '0');
		BOOST_CHECK_EQUAL(a.upper_bound('0'), '2');
		BOOST_CHECK_EQUAL(a.lower_bound('a'), ':');
		BOOST_CHECK_EQUAL(a.upper_bound(':'), 'a');
		BOOST_CHECK_EQUAL(a.lower_bound(cmax), 'b');
		BOOST_CHECK_EQUAL(a.upper_bound('b'), cmax);

	}
	// test decreasing intervals
	{
		std::set<Interval<unsigned char> > spaces;
		spaces.insert(Interval<unsigned char>('c', 'e'));
		spaces.insert(Interval<unsigned char>('3', '5'));
		Alphabet<unsigned char> a;

		a.add('a', 'e');
		a.add('3', '6');

		a.roundBoundaries(spaces);

		BOOST_CHECK_EQUAL(a.intervals().size(), 2u);
		BOOST_CHECK_EQUAL(a.lower_bound('c'), 'a');
		BOOST_CHECK_EQUAL(a.upper_bound('a'), 'c');
		BOOST_CHECK_EQUAL(a.lower_bound('6'), '4');
		BOOST_CHECK_EQUAL(a.upper_bound('4'), '6');
	}
	// test correct rounding
	{
		std::set<Interval<unsigned char> > spaces;
		spaces.insert(Interval<unsigned char>('4', '5'));
		Alphabet<unsigned char> a;

		a.add('5', '6');

		a.roundBoundaries(spaces);

		BOOST_CHECK_EQUAL(a.intervals().size(), 1u);
		BOOST_CHECK_EQUAL(a.lower_bound('6'), '4');
		BOOST_CHECK_EQUAL(a.upper_bound('4'), '6');
	}
	// test uneffective spaces
	{
		std::set<Interval<unsigned char> > spaces;
		spaces.insert(Interval<unsigned char>('4', '5'));
		Alphabet<unsigned char> a;

		a.add('3', '6');

		a.roundBoundaries(spaces);

		BOOST_CHECK_EQUAL(a.intervals().size(), 1u);
		BOOST_CHECK_EQUAL(a.lower_bound('6'), '3');
		BOOST_CHECK_EQUAL(a.upper_bound('3'), '6');
	}
	// test interval deletion
	{
		std::set<Interval<unsigned char> > spaces;
		spaces.insert(Interval<unsigned char>('3', '6'));
		spaces.insert(Interval<unsigned char>('a', 'c'));
		Alphabet<unsigned char> a;

		a.add('4', '5');
		a.add('a', 'c');

		a.roundBoundaries(spaces);

		BOOST_CHECK_EQUAL(a.intervals().size(), 0u);
	}
}

BOOST_AUTO_TEST_CASE(test_clear)
{
	Alphabet<char> a;
	a.add('a', 'c');
	a.add('d');
	BOOST_CHECK_EQUAL(a.singletons().size(), 1u);
	BOOST_CHECK_EQUAL(a.intervals().size(), 1u);
	BOOST_CHECK_EQUAL(a.empty(), false);
	a.clear();
	BOOST_CHECK_EQUAL(a.singletons().size(), 0u);
	BOOST_CHECK_EQUAL(a.intervals().size(), 0u);
	BOOST_CHECK_EQUAL(a.empty(), true);
	a.add('a', 'c');
	a.add('d');
	BOOST_CHECK_EQUAL(a.singletons().size(), 1u);
	BOOST_CHECK_EQUAL(a.intervals().size(), 1u);
	BOOST_CHECK_EQUAL(a.empty(), false);
}

BOOST_AUTO_TEST_CASE(test_del)
{
	Alphabet<int> a;
	a.add(10, 12);
	BOOST_CHECK_EQUAL(a.singletons().size(), 0u);
	BOOST_CHECK_EQUAL(a.intervals().size(), 1u);
	BOOST_CHECK_EQUAL(a.lower_bound(12), 10);
	a.del(1, 2);
	BOOST_CHECK_EQUAL(a.singletons().size(), 0u);
	BOOST_CHECK_EQUAL(a.intervals().size(), 1u);
	BOOST_CHECK_EQUAL(a.lower_bound(12), 10);
}

BOOST_AUTO_TEST_CASE(test_merge)
{
	{
		Alphabet<char> a;
		a.add('0');
		a.add('1');
		a.add('2');
		a.add(':', 'a');
		std::set<char> ms;
		ms.insert('0');
		ms.insert('1');
		ms.insert('2');
		BOOST_CHECK_EQUAL(a.lower_bound('0'), '0');
		BOOST_CHECK_EQUAL(a.lower_bound('1'), '1');
		BOOST_CHECK_EQUAL(a.lower_bound('2'), '2');
		BOOST_CHECK_EQUAL(a.lower_bound('a'), ':');
		BOOST_CHECK_EQUAL(a.singletons().size(), 3u);
		BOOST_CHECK_EQUAL(a.intervals().size(), 1u);
		a.merge(ms);
		BOOST_CHECK_EQUAL(a.lower_bound('2'), '0');
		BOOST_CHECK_EQUAL(a.lower_bound('a'), ':');
		BOOST_CHECK_EQUAL(a.singletons().size(), 0u);
		BOOST_CHECK_EQUAL(a.intervals().size(), 2u);
	}
	{
		Alphabet<int> a;
		a.add(1);
		a.add(2);
		a.add(3);
		a.add(10, 12);
		std::set<int> ms;
		ms.insert(1);
		ms.insert(2);
		ms.insert(3);
		BOOST_CHECK_EQUAL(a.singletons().size(), 3u);
		BOOST_CHECK_EQUAL(a.intervals().size(), 1u);
		a.merge(ms);
		BOOST_CHECK_EQUAL(a.singletons().size(), 0u);
		BOOST_CHECK_EQUAL(a.intervals().size(), 2u);
	}
	{
		Alphabet<ushort> a1(Interval<ushort>(3, 5));
		a1.add(6);
		a1.add(7, 10);
		a1.add(std::numeric_limits<ushort>::min(), 2);
		a1.add(11, std::numeric_limits<ushort>::max());

		BOOST_CHECK_EQUAL(a1.singletons().size(), 1u);
		BOOST_CHECK_EQUAL(a1.intervals().size(), 4u);
		BOOST_CHECK_EQUAL(a1.lower_bound(std::numeric_limits<ushort>::max()), 11);
		BOOST_CHECK_EQUAL(a1.lower_bound(5), 3);
		BOOST_CHECK_EQUAL(a1.lower_bound(6), 6);

		std::set<ushort> ms;
		a1.merge(ms);
		BOOST_CHECK_EQUAL(a1.singletons().size(), 1u);
		BOOST_CHECK_EQUAL(a1.intervals().size(), 4u);
		BOOST_CHECK_EQUAL(a1.lower_bound(6), 6);

		ms.insert(std::numeric_limits<ushort>::min());
		a1.merge(ms);
		BOOST_CHECK_EQUAL(a1.singletons().size(), 1u);
		BOOST_CHECK_EQUAL(a1.intervals().size(), 4u);
		BOOST_CHECK_EQUAL(a1.lower_bound(6), 6);

		ms.insert(12);
		a1.merge(ms);
		BOOST_CHECK_EQUAL(a1.singletons().size(), 1u);
		BOOST_CHECK_EQUAL(a1.intervals().size(), 4u);
		BOOST_CHECK_EQUAL(a1.lower_bound(6), 6);

		ms.insert(10);
		a1.merge(ms);
		BOOST_CHECK_EQUAL(a1.singletons().size(), 1u);
		BOOST_CHECK_EQUAL(a1.intervals().size(), 3u);
		BOOST_CHECK_EQUAL(a1.lower_bound(std::numeric_limits<ushort>::max()), 7);
		BOOST_CHECK_EQUAL(a1.lower_bound(6), 6);

		ms.insert(3);
		a1.merge(ms);
		BOOST_CHECK_EQUAL(a1.singletons().size(), 1u);
		BOOST_CHECK_EQUAL(a1.intervals().size(), 2u);
		BOOST_CHECK_EQUAL(a1.lower_bound(std::numeric_limits<ushort>::max()), 7);
		BOOST_CHECK_EQUAL(a1.lower_bound(5), std::numeric_limits<ushort>::min());
		BOOST_CHECK_EQUAL(a1.lower_bound(6), 6);

		ms.insert(6);
		a1.merge(ms);
		BOOST_CHECK_EQUAL(a1.singletons().size(), 0u);
		BOOST_CHECK_EQUAL(a1.intervals().size(), 1u);
		BOOST_CHECK_EQUAL(a1.lower_bound(6), std::numeric_limits<ushort>::min());
		BOOST_CHECK_EQUAL(a1.lower_bound(std::numeric_limits<ushort>::max()), std::numeric_limits<ushort>::min());
	}
}

BOOST_AUTO_TEST_CASE (test_alphabet)
{
	Alphabet<ushort> az(Interval<ushort>(3, 6));
	BOOST_CHECK_EQUAL(az.intervals().size(), 1u);
	BOOST_CHECK_EQUAL(az.singletons().size(), 0u);
	BOOST_CHECK_EQUAL(az.intervals().begin()->first, 3);
	BOOST_CHECK_EQUAL(az.intervals().begin()->last, 6);

	Alphabet<ushort> all(Interval<ushort>(std::numeric_limits<ushort>::min(), std::numeric_limits<ushort>::max()));
	BOOST_CHECK_EQUAL(all.intervals().size(), 1u);
	BOOST_CHECK_EQUAL(all.singletons().size(), 0u);
	BOOST_CHECK_EQUAL(all.intervals().begin()->first, std::numeric_limits<ushort>::min());
	BOOST_CHECK_EQUAL(all.intervals().begin()->last, std::numeric_limits<ushort>::max());

	Alphabet<ushort> no0(all);
	BOOST_CHECK_EQUAL_COLLECTIONS(no0.intervals().begin(), no0.intervals().end(), all.intervals().begin(), all.intervals().end());
	BOOST_CHECK_EQUAL_COLLECTIONS(no0.singletons().begin(), no0.singletons().end(), all.singletons().begin(), all.singletons().end());
	no0.del(0);
	BOOST_CHECK_EQUAL(no0.intervals().size(), 1u);
	BOOST_CHECK_EQUAL(no0.singletons().size(), 0u);
	BOOST_CHECK_EQUAL(no0.intervals().begin()->first, 1);
	BOOST_CHECK_EQUAL(no0.intervals().begin()->last, std::numeric_limits<ushort>::max());

	Alphabet<ushort> no9(all);
	BOOST_CHECK_EQUAL_COLLECTIONS(no9.intervals().begin(), no9.intervals().end(), all.intervals().begin(), all.intervals().end());
	BOOST_CHECK_EQUAL_COLLECTIONS(no9.singletons().begin(), no9.singletons().end(), all.singletons().begin(), all.singletons().end());
	no9.del(9);
	BOOST_CHECK_EQUAL(no9.intervals().size(), 2u);
	BOOST_CHECK_EQUAL(no9.singletons().size(), 0u);
	BOOST_CHECK_EQUAL(no9.intervals().begin()->first, std::numeric_limits<ushort>::min());
	BOOST_CHECK_EQUAL(no9.intervals().begin()->last, 8);
	BOOST_CHECK_EQUAL(no9.intervals().rbegin()->first, 10);
	BOOST_CHECK_EQUAL(no9.intervals().rbegin()->last, std::numeric_limits<ushort>::max());

	az.addDisjunct(no0);

	BOOST_CHECK(!az.contains(0));
	BOOST_CHECK(az.lower_bound(0) > 0);
	BOOST_CHECK_EQUAL(az.min(), 1);
	BOOST_CHECK_EQUAL(1, az.lower_bound(1));
	BOOST_CHECK_EQUAL(1, az.lower_bound(2));
	BOOST_CHECK_EQUAL(2, az.upper_bound(1));
	BOOST_CHECK_EQUAL(6, az.upper_bound(3));
	BOOST_CHECK_EQUAL(3, az.lower_bound(6));
	BOOST_CHECK_EQUAL(7, az.lower_bound(10));
	BOOST_CHECK_EQUAL(az.upper_bound(7), std::numeric_limits<ushort>::max());

	az.addDisjunct(no9);
	BOOST_CHECK(az.contains(0));
	BOOST_CHECK(az.contains(9));

	BOOST_CHECK_EQUAL(0, az.lower_bound(0));
	BOOST_CHECK_EQUAL(1, az.lower_bound(1));
	BOOST_CHECK_EQUAL(1, az.lower_bound(2));
	BOOST_CHECK_EQUAL(3, az.lower_bound(3));
	BOOST_CHECK_EQUAL(3, az.lower_bound(4));
	BOOST_CHECK_EQUAL(3, az.lower_bound(5));
	BOOST_CHECK_EQUAL(3, az.lower_bound(6));
	BOOST_CHECK_EQUAL(7, az.lower_bound(7));
	BOOST_CHECK_EQUAL(7, az.lower_bound(8));
	BOOST_CHECK_EQUAL(9, az.lower_bound(9));
	BOOST_CHECK_EQUAL(10, az.lower_bound(10));
	BOOST_CHECK_EQUAL(10, az.lower_bound(11));

	BOOST_CHECK_EQUAL(0, az.upper_bound(0));
	BOOST_CHECK_EQUAL(2, az.upper_bound(1));
	BOOST_CHECK_EQUAL(2, az.upper_bound(2));
	BOOST_CHECK_EQUAL(6, az.upper_bound(3));
	BOOST_CHECK_EQUAL(6, az.upper_bound(4));
	BOOST_CHECK_EQUAL(6, az.upper_bound(5));
	BOOST_CHECK_EQUAL(6, az.upper_bound(6));
	BOOST_CHECK_EQUAL(8, az.upper_bound(7));
	BOOST_CHECK_EQUAL(8, az.upper_bound(8));
	BOOST_CHECK_EQUAL(9, az.upper_bound(9));
	BOOST_CHECK_EQUAL(std::numeric_limits<ushort>::max(), az.upper_bound(10));

	no0.addDisjunct(no9);
	BOOST_CHECK(no0.contains(0));
	BOOST_CHECK(no0.contains(9));
	BOOST_CHECK_EQUAL(0, no0.lower_bound(0));
	BOOST_CHECK_EQUAL(1, no0.lower_bound(1));
	BOOST_CHECK_EQUAL(1, no0.lower_bound(8));
	BOOST_CHECK_EQUAL(9, no0.lower_bound(9));
	BOOST_CHECK_EQUAL(10, no0.lower_bound(std::numeric_limits<ushort>::max()));

	BOOST_CHECK_EQUAL(0, no0.upper_bound(0));
	BOOST_CHECK_EQUAL(8, no0.upper_bound(1));
	BOOST_CHECK_EQUAL(8, no0.upper_bound(8));
	BOOST_CHECK_EQUAL(9, no0.upper_bound(9));
	BOOST_CHECK_EQUAL(std::numeric_limits<ushort>::max(), no0.upper_bound(10));

	/*test_roundBoundaries();
	test_clear();
	test_del();
	test_merge();
	test_ctor();*/
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

template<typename T> std::ostream& operator<<(std::ostream& os, const Interval<T>& rhs)
{
	os << "[" << rhs.first << "-" << rhs.last << "]";
	return os;
}
