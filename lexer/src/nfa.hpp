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
