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

#ifndef INTERVAL_HPP
#define INTERVAL_HPP

template<typename T> struct Interval
{
	explicit Interval(const T& s): first(s), last(s) {}
	Interval(const T& a, const T& b): first(a), last(b) {}
	bool operator< (const Interval& other) const { return this->last < other.first; }
	bool operator==(const Interval& other) const { return this->first == other.first && this->last == other.last; }
	bool operator!=(const Interval& other) const { return this->first != other.first || this->last != other.last; }
	bool empty() const { return *this < *this; }
	bool singleton() const { return this->first == this->last; }
	bool contains(const T& s) const { return !(s < this->first) && !(this->last < s); }
	bool contains(const Interval& other) const { return contains(other.first) && contains(other.last); }
	Interval intersection(const Interval& other) const { return Interval( (this->first<other.first)?other.first:this->first, (this->last<other.last)?this->last:other.last ); }

	T first, last;
};

#endif
