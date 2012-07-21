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

#ifndef ALPHABET_HPP
#define ALPHABET_HPP

#include "interval.hpp"

#include <set>
#include <string>

template<typename T> class Alphabet
{
public:
	Alphabet(){}
	Alphabet(const T& symbol);
	Alphabet(const Interval<T>&);
	Alphabet(const Alphabet& a);

	template<class InputIterator>
	Alphabet(InputIterator first, InputIterator last);

	void add(const T& symbol);
	void add(const T& first, const T& last);
	void add(const Alphabet& a);
	void del(const T& symbol);
	void del(const T& first, const T& last);
	void del(const Alphabet& a);

	void clear();

	Alphabet intersection(const Alphabet& a);

	bool contains(const T& symbol) const;
	bool empty() const;

	T lower_bound(const T& symbol) const;
	T upper_bound(const T& symbol) const;

	std::set<T> lower_bound() const;

	T min() const;

	void addDisjunct(const T& symbol);
	void addDisjunct(const T& first, const T& last);
	void addDisjunct(const Alphabet& a);

	/// Merges adjacent intervals and singletons containing symbols of set.
	void merge(const std::set<T>&);

	/// Merges adjacent intervals and singletons.
	void merge();

	/// Moves the boundaries of the intervals within space.
	/// regarding power of two
	/// @todo for now only intervals are changed.
	void roundBoundaries(const std::set<Interval<T> >& space);

	const std::set<T>& singletons() const;
	const std::set<Interval<T> >& intervals() const;

protected:

	T succ(const T& symbol) const { T s = symbol; return ++s; }
	T pred(const T& symbol) const { T s = symbol; return --s; }

	std::set<T> singleton;
	std::set<Interval<T> > interval;
};

template<typename T>
template<class InputIterator>
inline Alphabet<T>::Alphabet(InputIterator first, InputIterator last)
: singleton(first, last)
{
}

#endif
