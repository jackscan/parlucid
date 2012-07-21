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

#ifndef HELPER_H
#define HELPER_H

#include <set>
#include <algorithm>

template<class T> std::set<T>& set_union_add(std::set<T>& dest, const std::set<T>& other)
{
	dest.insert(other.begin(), other.end());

	return dest;
}

template<class T> std::set<T>& set_intersect(std::set<T>& dest, const std::set<T>& other)
{
	typename std::set<T>::iterator iDest = dest.begin();
	while (iDest != dest.end())
		if (other.find(*iDest) == other.end())
		{
			typename std::set<T>::iterator iErase = iDest;
			++iDest;
			dest.erase(iErase);
		}
		else ++iDest;
	return dest;
}

template<class T> bool intersect(const T& set1, const T& set2)
{
	for (typename T::const_iterator i = set1.begin(); i != set1.end(); ++i)
		if (set2.find(*i) != set2.end())
			return true;
	return false;
}

template<class T> inline T& minimize(T& var, const T& other)
{
	return var < other ? var : var = other;
}

template<class T> inline T& maximize(T& var, const T& other)
{
	return var > other ? var : var = other;
}

template<class T> inline T gcd(T a, T b)
{
	if (a == 0) return b;
	while (b != 0)
		if (a > b) a -= b;
		else b -= a;
	return a;
}

#endif
