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

#ifndef NFA_HPP
#define NFA_HPP


#include "alphabet.hpp"

#include <map>
#include <set>
#include <list>

template<typename C, typename O = int> class Nfa
{
public:
	typedef int State;
	typedef C Char;
	typedef O Token;

protected:
	Alphabet<Char> alphabet;
	std::map<State,std::set<Token> > final_states;
	std::map<State,std::map<Char,std::set<State> > > transition_table;
	State num_states;

public:

	/// Calculates nfa accepting reverse of words accepted by this nfa.
	Nfa reverse() const;

	Nfa operator*() const;
	Nfa operator|(const Nfa&) const;
	Nfa operator+(const Nfa&) const;
	/// Calculates inverse of this nfa excluding epsilon.
	//Nfa operator~() const;

	static Nfa combine(const std::list<Nfa>&);

	const std::set<State>& next(State state, const Char& symbol) const;
	std::set<State> next(const std::set<State>& states, const Char& symbol) const;
	bool final(State state) const;
	bool final(std::set<State> states) const;
	std::set<Token> output(State state) const;
	std::set<Token> output(std::set<State> states) const;
	std::set<Token> output() const;
	void set_output(Token);
	const Alphabet<Char>& input() const;
	bool empty() const;

	Nfa();
	Nfa(const std::basic_string<Char>&);
	Nfa(const Alphabet<Char>& alphabet);

protected:
	static const std::set<State> emptyStateSet;
	void add_output(State state, const std::set<Token>& o);
};

#endif
