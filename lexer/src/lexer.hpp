#ifndef LEXER_HPP
#define LEXER_HPP

#include "fsm.hpp"
#include <set>
#include <sstream>
#include <istream>

template<typename T, class F = Fsm<T> > class Lexer
{
public:
	typedef F Fsm;

	Lexer() {}
	Lexer(const Fsm& fsm): fsm(fsm) {}

	void init(const Fsm&);
	bool error() const;
	std::streamsize gcount() const;
	bool read(std::basic_istream<T>& is, int& token_type);
	const std::set<int>& read(std::basic_istream<T>& is);

private:
	void reset();
	Fsm fsm;
	std::streamsize count;
	typename Fsm::State state;
};

#endif
