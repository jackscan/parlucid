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

#ifndef FSM_H
#define FSM_H

#include "nfa.hpp"
#include "alphabet.hpp"
#include "../../utils/src/bitsequence.hpp"

#include <map>
#include <set>
#include <vector>

template<typename C, typename O = int> class Fsm
{
public:
	typedef int State;
	typedef O Token;
	typedef C Char;
	const static State ERROR_STATE;

protected:
	//static std::vector<bool> emptySet;
	
	struct Transition
	{
		Transition() : nextState(ERROR_STATE) {}
		Transition(State next, const BitSequence<Token>& out) : nextState(next), output(out) {}
		State nextState;
		//std::set<Token> output;
		//std::vector<bool> output;
		BitSequence<Token> output;
		bool operator<(const Transition& rhs) const { return nextState < rhs.nextState 
			|| (nextState == rhs.nextState && output < rhs.output); }
		bool operator==(const Transition& rhs) const { return nextState == rhs.nextState && output == rhs.output; }
	};

	typedef std::map<Char, Transition> Transitions;
	typedef std::vector<Transitions> TransitionTable;
	Alphabet<Char> alphabet;
	TransitionTable transition_table;
	Interval<Token> tokenRange;

public:
	Fsm();
	Fsm(const Nfa<Char, Token>&);

	const BitSequence<Token>& next(State& state, const Char& symbol) const;
	bool error(State state) const;

	bool isAcceptingState(State state) const;
	void getAcceptedSymbols(State state, std::set<Char>& symbols) const;
	State getNumberOfStates() const;
	const Alphabet<Char>& input() const;

	//Token getMinOutput() const;
	//Token getMaxOutput() const;
	//size_t getOutputSize() const;
	/// Sets entries to true for output.
	void getOutput(BitSequence<Token>& tokens) const;

	/// Converts index in output to token.
	//Token getToken(size_t index) const;
	/// Converts token to index into output.
	//size_t getIndex(Token token) const;

	/// Resulting map maps from Tokens to corresponding prefixes.
	/// A token p is prefix of token t if ... TODO 
	void getPrefixAssociations(std::map<Token, BitSequence<Token> >& prefixes) const;
	/// Result maps from States to corresponding prefixes.
	void getStatePrefixes(std::map<State, BitSequence<Token> >& prefixes) const;
	/// Result maps from state to outputs of reachable transitions.
	void getPotentialOutputs(std::map<State,BitSequence<Token> >& outputs) const;

	//State noStates() const;

	void create(const Nfa<Char, Token>&);
	void reduce();

	/// @param outputs prio sorted list.
	void disambiguate(const std::list<Token>& outputs);
	void strip(const BitSequence<Token>& outputs, bool strict);
	/// Strips outputs of outgoing transitions from state which are not contained in outputs
	void stripTransitions(State state, const BitSequence<Token>& outputs);
	/// creates fsm where state i is start of fsms[i]
	void mergeDemux(const std::vector<Fsm<Char, Token> >& fsms);
	//void printTable();
	/// Transforms fsm into a unique form.
	void normalize();
	/// Strips transitions to states without potential output.
	void stripDeadEnds(const std::map<State,BitSequence<Token> >& outputs);

	/// Successive states of linear subpaths without intermediate output are moved to the end in a way so their state numbers are successive.
	/// @param minState States <minState are not touched.
	/// @param minLength minimum length for sequence.
	void groupLinearPaths(State minState, int minLength);

	/// Computes the beginning of linear sequences. That means, all following states are part of linear sequences.
	/// @param minLength minimal length of sequence to declare it linear.
	/// @return first state of linear sequences.
	State getFirstLinearSequence(int minLength) const;

	/// Computes linear sequence starting at state.
	/// @return exit state of sequence.
	State getNextLinearSequence(State state, std::basic_string<Char>& input, BitSequence<Token>& output) const;

protected:
	void stripTransition(Transition& transition, const BitSequence<Token>& outputs);
	void stripUnreachableTransitions();
	//void partition(const std::map<State,State>& curr_map;
};

#endif
