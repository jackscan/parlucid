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

#include "../src/bitsequence.hpp"

#include <boost/test/unit_test.hpp>

#include <vector>

BOOST_AUTO_TEST_CASE(test_ctor)
{
	{
		BitSequence<int> s(-2, 4);
		BOOST_CHECK_EQUAL(s.begin(), -32);
		BOOST_CHECK_EQUAL(s.end(), 32);
	}

	{
		BitSequence<int> s(2, 4);
		BOOST_CHECK_EQUAL(s.begin(), 0);
		BOOST_CHECK_EQUAL(s.end(), 32);
	}

	{
		BitSequence<int> s(-4, -2);
		BOOST_CHECK_EQUAL(s.begin(), -32);
		BOOST_CHECK_EQUAL(s.end(), 0);
	}

	{
		BitSequence<int> s(-4, -2);
		BOOST_CHECK_EQUAL(s.begin(), -32);
		BOOST_CHECK_EQUAL(s.end(), 0);
		s.set(-3);

		BitSequence<int> s2(s);
		BOOST_CHECK_EQUAL(s.begin(), -32);
		BOOST_CHECK_EQUAL(s.end(), 0);
		BOOST_CHECK(s.get(-3));
	}

	{
		BitSequence<int> s1(0, 2);
		BitSequence<int> s2(s1);
	}

	{
		BitSequence<int> s1;
		BitSequence<int> s2(s1);
	}
}

BOOST_AUTO_TEST_CASE(test_assign)
{
	{
		BitSequence<int> s1(0, 2);
		BitSequence<int> s2;

		s2 = s1;
	}
	{
		BitSequence<int> s1;
		BitSequence<int> s2;

		s2 = s1;
	}
	{
		BitSequence<int> s1;
		BitSequence<int> s2(0, 2);

		s2 = s1;
	}
	{
		BitSequence<int> s1(0, 2);
		BitSequence<int> s2(0, 2);

		s2 = s1;
	}
}

BOOST_AUTO_TEST_CASE(test_less)
{
	{
		BitSequence<int> s1(0, 6);
		BitSequence<int> s2(2, 4);

		BOOST_CHECK(!(s1 < s2));
		BOOST_CHECK(!(s2 < s1));

		s1.set(0);
		s2.set(2);
		BOOST_CHECK(!(s1 < s2));
		BOOST_CHECK(s2 < s1);

		s1.reset();
		s1.set(2);

		BOOST_CHECK(!(s1 < s2));
		BOOST_CHECK(!(s2 < s1));

		s1.reset();
		s1.set(3);

		BOOST_CHECK((s1 < s2));
		BOOST_CHECK(!(s2 < s1));

		s1.reset();
		s1.set(2);
		s1.set(5);

		BOOST_CHECK(!(s1 < s2));
		BOOST_CHECK((s2 < s1));
	}
	{
		BitSequence<int> s1(0, 6);
		BitSequence<int> s2(-2, -1);

		BOOST_CHECK(!(s1 < s2));
		BOOST_CHECK(!(s2 < s1));
	}
	{
		BitSequence<int> s1(0, 6);
		BitSequence<int> s2(2, 7);

		BOOST_CHECK(!(s1 < s2));
		BOOST_CHECK(!(s2 < s1));
	}
	{
		BitSequence<int> s1(0, 6);
		BitSequence<int> s2(7, 8);

		BOOST_CHECK(!(s1 < s2));
		BOOST_CHECK(!(s2 < s1));
	}


	//
	{
		BitSequence<int> s1(0, 60);
		BitSequence<int> s2(20, 40);

		BOOST_CHECK(!(s1 < s2));
		BOOST_CHECK(!(s2 < s1));

		s1.set(0);
		s2.set(20);
		BOOST_CHECK(!(s1 < s2));
		BOOST_CHECK(s2 < s1);

		s1.reset();
		s1.set(20);

		BOOST_CHECK(!(s1 < s2));
		BOOST_CHECK(!(s2 < s1));

		s1.reset();
		s1.set(30);

		BOOST_CHECK((s1 < s2));
		BOOST_CHECK(!(s2 < s1));

		s1.reset();
		s1.set(20);
		s1.set(50);

		BOOST_CHECK(!(s1 < s2));
		BOOST_CHECK((s2 < s1));
	}
	{
		BitSequence<int> s1(0, 60);
		BitSequence<int> s2(-20, -10);

		BOOST_CHECK(!(s1 < s2));
		BOOST_CHECK(!(s2 < s1));
	}
	{
		BitSequence<int> s1(0, 60);
		BitSequence<int> s2(20, 70);

		BOOST_CHECK(!(s1 < s2));
		BOOST_CHECK(!(s2 < s1));
	}
	{
		BitSequence<int> s1(0, 60);
		BitSequence<int> s2(70, 80);

		BOOST_CHECK(!(s1 < s2));
		BOOST_CHECK(!(s2 < s1));
	}
}

BOOST_AUTO_TEST_CASE(test_and)
{
	{
		BitSequence<int> s1(3, 4);
		BitSequence<int> s2(0, 2);
		s1.set(3);
		s2.set(0);
		s2.set(1);
		s1 &= s2;
		BOOST_CHECK(s1.empty());
	}

	{
		BitSequence<int> s1(3, 4);
		BitSequence<int> s2(0, 2);
		s1.set(3);
		s2.set(0);
		s2.set(1);
		s2 &= s1;
		BOOST_CHECK(s2.empty());
	}

	{
		BitSequence<int> s1(0, 4);
		BitSequence<int> s2(2, 3);
		s1.set(0);
		s1.set(3);
		s2.set(2);
		s1 &= s2;
		BOOST_CHECK(s1.empty());
	}

	{
		BitSequence<int> s1(0, 4);
		BitSequence<int> s2(2, 3);
		s1.set(0);
		s1.set(3);
		s2.set(2);
		s2 &= s1;
		BOOST_CHECK(s2.empty());
	}
}

BOOST_AUTO_TEST_CASE(test_or)
{
	{
		BitSequence<int> s1(-1, 0);
		BitSequence<int> s2(0, 1);
		BitSequence<int> s3(-1, 1);
		s1.set(-1);
		s2.set(0);
		s2 |= s1;
		s3.set(-1);
		s3.set(0);
		BOOST_CHECK_EQUAL(s2, s3);
	}
	{
		BitSequence<int> s1(2, 4);
		BitSequence<int> s2(0, 1);
		s1.set(2);
		s1.set(3);
		s2.set(0);
		BitSequence<int> s3(0, 4);
		s3.set(0);
		s3.set(2);
		s3.set(3);
		s1 |= s2;
		BOOST_CHECK_EQUAL(s1, s3);
	}

	{
		BitSequence<int> s1(3, 4);
		BitSequence<int> s2(0, 3);
		s1.set(3);
		s2.set(0);
		s2.set(1);
		BitSequence<int> s3(0, 4);
		s3.set(3);
		s3.set(0);
		s3.set(1);
		s1 |= s2;
		BOOST_CHECK_EQUAL(s1, s3);
	}

	{
		BitSequence<int> s1(3, 4);
		BitSequence<int> s2(0, 3);
		s1.set(3);
		s2.set(0);
		s2.set(1);
		BitSequence<int> s3(0, 4);
		s3.set(3);
		s3.set(0);
		s3.set(1);
		s2 |= s1;
		BOOST_CHECK_EQUAL(s2, s3);
	}

	{
		BitSequence<int> s1(0, 4);
		BitSequence<int> s2(2, 3);
		s1.set(0);
		s1.set(3);
		s2.set(2);
		BitSequence<int> s3(0, 4);
		s3.set(0);
		s3.set(2);
		s3.set(3);
		s1 |= s2;
		BOOST_CHECK_EQUAL(s1, s3);
	}
	{
		BitSequence<int> s1(0, 4);
		BitSequence<int> s2(2, 3);
		s1.set(0);
		s1.set(3);
		s2.set(2);
		BitSequence<int> s3(0, 4);
		s3.set(0);
		s3.set(2);
		s3.set(3);
		s2 |= s1;
		BOOST_CHECK_EQUAL(s2, s3);
	}
}

BOOST_AUTO_TEST_CASE(test_get)
{
	BitSequence<int> s(3, 4);
	BOOST_CHECK(!s.get(-1));
	BOOST_CHECK(!s.get(0));
	BOOST_CHECK(!s.get(1));
	BOOST_CHECK(!s.get(2));
	BOOST_CHECK(!s.get(3));
	BOOST_CHECK(!s.get(4));
	BOOST_CHECK(!s.get(5));
	BOOST_CHECK(!s.get(31));
	BOOST_CHECK(!s.get(32));
	BOOST_CHECK(!s.get(33));
	BOOST_CHECK(!s.get(34));
	BOOST_CHECK(!s.get(35));
	BOOST_CHECK(!s.get(36));

	s.set(3);
	BOOST_CHECK(!s.get(-1));
	BOOST_CHECK(!s.get(0));
	BOOST_CHECK(!s.get(1));
	BOOST_CHECK(!s.get(2));
	BOOST_CHECK(s.get(3));
	BOOST_CHECK(!s.get(4));
	BOOST_CHECK(!s.get(5));
	BOOST_CHECK(!s.get(31));
	BOOST_CHECK(!s.get(32));
	BOOST_CHECK(!s.get(33));
	BOOST_CHECK(!s.get(34));
	BOOST_CHECK(!s.get(35));
	BOOST_CHECK(!s.get(36));

	s.set(4);
	BOOST_CHECK(!s.get(-1));
	BOOST_CHECK(!s.get(0));
	BOOST_CHECK(!s.get(1));
	BOOST_CHECK(!s.get(2));
	BOOST_CHECK(s.get(3));
	BOOST_CHECK(s.get(4));
	BOOST_CHECK(!s.get(5));
	BOOST_CHECK(!s.get(31));
	BOOST_CHECK(!s.get(32));
	BOOST_CHECK(!s.get(33));
	BOOST_CHECK(!s.get(34));
	BOOST_CHECK(!s.get(35));
	BOOST_CHECK(!s.get(36));
}

BOOST_AUTO_TEST_CASE(test_set)
{
	{
		BitSequence<int> s(0, 1);
		s.set(-1);
	}
	{
		BitSequence<int> s(64, 65);
		s.set(63);
	}
	{
		BitSequence<int> s1(0, 1);
		BitSequence<int> s2(-1, 65);
		s1 |= s2;
	}
}

