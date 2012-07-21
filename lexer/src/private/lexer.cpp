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

#include "../lexer.hpp"

template class Lexer<char>;
template class Lexer<unsigned char>;

template<typename T, class F>
void Lexer<T,F>::init(const Fsm& fsm)
{
	this->fsm = fsm;
}

template<typename T, class F>
void Lexer<T,F>::reset()
{
	this->state = 0;
	this->count = 0;
}

template<typename T, class F>
bool Lexer<T,F>::error() const
{
	return this->fsm.error(this->state);
}

template<typename T, class F>
std::streamsize Lexer<T,F>::gcount() const
{
	return this->count;
}

template<typename T, class F>
bool Lexer<T,F>::read(std::basic_istream<T>& is, int& token_type)
{
	//TODO:
	/*this->reset();
	typename std::basic_istream<T>::pos_type istr_pos = is.tellg(), start_pos = istr_pos;
	typename Fsm::State accept_state = this->state;
	std::streamsize cnt = 0;

	// initialize with empty set
	typename Fsm::State errorState = Fsm::ERROR_STATE;
	const BitSequence<int>* result = &this->fsm.next(errorState, static_cast<T>(0));

	while(is.good())
	{
		T ch;
		is.get(ch);
		if(is.fail()) break;
		const BitSequence<int>& outs = this->fsm.next(this->state, ch);
		if(this->fsm.error(this->state))
		{
			is.unget();
			break;
		}
		cnt++;
		if(!outs.empty())
		{
			this->count = cnt;
			result = &outs;
			accept_state = this->state;
			istr_pos = is.tellg();
		}
	}

	is.seekg(istr_pos);

	if (!result->empty()) token_type = *result->begin();
	return !result->empty();*/
	return false;
}

template<typename T, class F>
const std::set<int>& Lexer<T,F>::read(std::basic_istream<T>& is)
{
	/*this->reset();
	typename std::basic_istream<T>::pos_type istr_pos = is.tellg(), start_pos = istr_pos;
	typename Fsm::State accept_state = this->state;
	std::streamsize cnt = 0;

	// initialize with empty set
	typename Fsm::State errorState = Fsm::ERROR_STATE;
	const std::set<int>* result = &this->fsm.next(errorState, static_cast<T>(0));

	while(is.good())
	{
		T ch;
		is.get(ch);
		if(is.fail()) break;
		const std::set<int>& outs = this->fsm.next(this->state, ch);
		if(this->fsm.error(this->state))
		{
			is.unget();
			break;
		}
		cnt++;
		if(!outs.empty())
		{
			this->count = cnt;
			result = &outs;
			accept_state = this->state;
			istr_pos = is.tellg();
		}
	}

	is.seekg(istr_pos);

	return *result;*/
	static std::set<int> empty;
	return empty;
}
