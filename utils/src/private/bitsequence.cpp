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

#include "../bitsequence.hpp"

#include <functional>
#include <cstring>

template<typename T>
BitSequence<T>::BitSequence()
: contBegin(0), contSize(0), sequence(0)
{
}

template<typename T>
BitSequence<T>::BitSequence(T begin, T end)
: contBegin(getContainer(begin)), contSize(0), sequence(0)
{
	resize(begin, end);
}

template<typename T>
BitSequence<T>::BitSequence(const BitSequence& rhs)
: contBegin(rhs.contBegin), contSize(rhs.contSize), sequence(contSize > 0 ? new Container[contSize] : 0)
{
	//printf("new %x\n", sequence);
	if (this->sequence) memcpy(this->sequence, rhs.sequence, this->contSize * sizeof(Container));
}

template<typename T>
BitSequence<T>& BitSequence<T>::operator=(const BitSequence& rhs)
{
	this->resize(rhs.begin(), rhs.end());
	if (rhs.sequence) memcpy(this->sequence, rhs.sequence, this->contSize * sizeof(Container));
	return *this;
}

template<typename T>
BitSequence<T>::~BitSequence()
{
	//printf("delete %x\n", this->sequence);
	delete[] this->sequence;
}

template<typename T>
BitSequence<T>& BitSequence<T>::operator|=(const BitSequence<T>& rhs)
{
	this->resize(rhs.begin(), rhs.end());

	Container *dest = &this->sequence[rhs.contBegin - this->contBegin];

	for (int n = 0; n < rhs.contSize; ++n) dest[n].bits |= rhs.sequence[n].bits;

	return *this;

	/*// ensure this->minbit <= rhs.minbit
	if (rhs.minbit < this->minbit)
	{
		size_t offset = this->minbit - rhs.minbit;
		if (offset > rhs.sequence.size())
		{
			this->sequence.insert(this->sequence.begin(), offset - rhs.sequence.size(), false);
			this->sequence.insert(this->sequence.begin(), rhs.sequence.begin(), rhs.sequence.end());
		}
		else
			this->sequence.insert(this->sequence.begin(), rhs.sequence.begin(), rhs.sequence.begin() + offset);

		this->minbit = rhs.minbit;
	}

	size_t offset = static_cast<size_t>(rhs.minbit - this->minbit);

	// ensure "this->maxbit" >= "rhs.maxbit"
	if (this->sequence.size() < rhs.sequence.size() + offset) this->sequence.resize(rhs.sequence.size() + offset);
	std::transform(rhs.sequence.begin(), rhs.sequence.end(), this->sequence.begin() + offset, this->sequence.begin() + offset, std::logical_or<bool>());

	this->maxbit = this->minbit + static_cast<T>(this->sequence.size());

	return *this;*/
}

template<typename T>
BitSequence<T>& BitSequence<T>::operator&=(const BitSequence<T>& rhs)
{
	if (this->sequence)
	{
		/*int startbit = std::max(rhs.minbit, this->minbit);
		int endbit = std::min(rhs.maxbit, this->maxbit);
		int lend = (endbit - 1 - this->minbit) / Container::bitcount + 1;*/

		int offset = this->contBegin - rhs.contBegin;
		int lstart = std::min(std::max(0, -offset), this->contSize);
		int rstart = std::min(std::max(0, offset), rhs.contSize);
		int lend = std::min(this->contSize, lstart + rhs.contSize - rstart);

		for (int n = 0; n < lstart; ++n) this->sequence[n].bits = 0;

		const Container *src = &rhs.sequence[rstart];

		for (int n = lstart; n < lend; ++n) this->sequence[n].bits &= src[n].bits;

		for (int n = lend; n < this->contSize; ++n) this->sequence[n].bits = 0;
	}

	return *this;

/*	// ensure this->minbit <= rhs.minbit
	if (rhs.minbit < this->minbit)
	{
		this->sequence.insert(this->sequence.begin(), this->minbit - rhs.minbit, false);
		this->minbit = rhs.minbit;
	}
	size_t offset = static_cast<size_t>(rhs.minbit - this->minbit);
	if (this->sequence.size() > offset)
		std::fill_n(this->sequence.begin(), offset, false);
	else
		std::fill(this->sequence.begin(), this->sequence.end(), false);

	// ensure "this->maxbit" >= "rhs.maxbit"
	if (this->sequence.size() < rhs.sequence.size() + offset) this->sequence.resize(rhs.sequence.size() + offset, false);
	else std::fill(this->sequence.begin() + rhs.sequence.size() + offset, this->sequence.end(), false);
	std::transform(rhs.sequence.begin(), rhs.sequence.end(), this->sequence.begin() + offset, this->sequence.begin() + offset, std::logical_and<bool>());

	this->maxbit = this->minbit + static_cast<T>(this->sequence.size());

	return *this;*/
}

template<typename T>
void BitSequence<T>::set(T bit)
{
	if (this->begin() > bit || this->end() <= bit)
		this->resize(bit, bit+1);

	int cont = getContainer(bit);
	int index = cont - this->contBegin;
	int offset = (1 + cont) * Container::bitcount - bit - 1;
	this->sequence[index].bits |= (1 << offset);
}

template<typename T>
void BitSequence<T>::resize(T begin, T end)
{
	int newBegin = this->getContainer(begin);
	int newEnd = this->getContainer(end - 1) + 1;
	bool realloc = false;
	int offset = 0;
	int oldSize = this->contSize;
	if (this->contBegin > newBegin)
	{
		realloc = true;
		offset = this->contBegin - newBegin;
		this->contBegin = newBegin;
		this->contSize += offset;
	}
	if (this->contBegin + this->contSize < newEnd)
	{
		realloc = true;
		this->contSize = newEnd - this->contBegin;
	}

	if (realloc)
	{
		Container *newSequence = new Container[this->contSize];
		//printf("new %p\n", newSequence);
		Container *pCont = newSequence;
		for (int n = 0; n < offset; ++n, ++pCont) pCont->bits = 0;
		for (int n = 0; n < oldSize; ++n, ++pCont) *pCont = this->sequence[n];
		for (int n = oldSize + offset; n < this->contSize; ++n, ++pCont) pCont->bits = 0;
		delete[] this->sequence;
		//printf("delete %p\n", sequence);
		this->sequence = newSequence;
	}

	/*T newMin = (begin < 0 ? (begin - Container::bitcount + 1) / Container::bitcount : begin / Container::bitcount) * Container::bitcount;
	T newMax = (end < 0 ? end / Container::bitcount : (end + Container::bitcount - 1) / Container::bitcount) * Container::bitcount;
	bool realloc = false;
	int offset = 0;
	int oldSize = this->sequenceSize();
	if (this->minbit > newMin)
	{
		realloc = true;
		offset = (this->minbit - newMin) / Container::bitcount;
		this->minbit = newMin;
	}
	if (this->maxbit < newMax)
	{
		realloc = true;
		this->maxbit = newMax;
	}

	if (realloc)
	{
		int size = this->sequenceSize();
		Container *newSequence = new Container[size];
		Container *pCont = newSequence;
		for (int n = 0; n < offset; ++n, ++pCont) pCont->bits = 0;
		for (int n = 0; n < oldSize; ++n, ++pCont) *pCont = this->sequence[n];
		for (int n = oldSize; n < size; ++n, ++pCont) pCont->bits = 0;
		delete this->sequence;
		this->sequence = newSequence;
	}*/
}

template<typename T>
int BitSequence<T>::compare(const BitSequence& rhs) const
{
	int offset = this->contBegin - rhs.contBegin;
	if (0 > offset)
	{
		int prefixSize = std::min(-offset, this->contSize);
		for (int n = 0; n < prefixSize; ++n)
			if (0 != this->sequence[n].bits) return 1;
	}
	else if (0 < offset)
	{
		int prefixSize = std::min(offset, rhs.contSize);
		for (int n = 0; n < prefixSize; ++n)
			if (0 != rhs.sequence[n].bits) return -1;
	}

	int lbegin = std::max(-offset, 0);
	int rbegin = std::max(offset, 0);
	int lend = std::min(this->contSize, rhs.contBegin + rhs.contSize - this->contBegin);

	if (lbegin < lend)
	{
		const Container* plhs = &rhs.sequence[rbegin];
		for (int n = lbegin; n < lend; ++n)
			if (this->sequence[n].bits != plhs[n].bits)
				return rhs.sequence[n].bits < this->sequence[n].bits ? 1 : -1;

		//int result = memcmp(&this->sequence[lbegin], &rhs.sequence[rbegin], sizeof(Container) * size);
		//if (result != 0) return result;
	}

	int offset2 = (this->contBegin + this->contSize) - (rhs.contBegin + rhs.contSize);

	if (0 < offset2)
	{
		int suffixBegin = std::max(0, rhs.contBegin + rhs.contSize - this->contBegin);
		for (int n = suffixBegin; n < this->contSize; ++n)
			if (0 != this->sequence[n].bits) return 1;
	}
	else if (0 > offset2)
	{
		int suffixBegin = std::max(0, this->contBegin + this->contSize - rhs.contBegin);
		for (int n = suffixBegin; n < rhs.contSize; ++n)
			if (0 != rhs.sequence[n].bits) return -1;
	}
	return 0;
}

template<typename T>
std::string BitSequence<T>::toString(int bitsPerChar) const
{
	bool nonEmpty = false;

	size_t size = static_cast<size_t>((this->end() + bitsPerChar - 1) / bitsPerChar) * bitsPerChar;
	std::string bitstr;
	int nibble = 0;
	for (T b = size - 1; b >= 0; --b)
	{
		int bit = b % bitsPerChar;
		nibble |= (this->get(b) ? 1 : 0) << bit;
		if (bit == 0)
		{
			nonEmpty = nonEmpty || (nibble != 0);
			if (nonEmpty) bitstr.append(1, (char)nibble + ((nibble < 0xA) ? '0' : 'A'-(char)(0xA)));
			nibble = 0;
		}
	}
	return bitstr;
}

template class BitSequence<char>;
template class BitSequence<short>;
template class BitSequence<int>;
