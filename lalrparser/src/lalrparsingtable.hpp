#ifndef LALRPARSINGTABLE_HPP
#define LALRPARSINGTABLE_HPP

#include "gram.hpp"
#include <deque>
#include <vector>
#include <iostream>

template<typename T> class LalrParsingTable
{
public:
	typedef T Token;
	typedef Grammar<Token> Gram;
	typedef typename Gram::ProductionNo ProductionNo;
	typedef typename Gram::Production Production;
	typedef typename Gram::Productions Productions;

	enum { SHIFT, REDUCE, /*FINISH,*/ AMBIGUOUS };

	struct Action {
		int type;
		union {
			ProductionNo productionNo;
			size_t nextState;
			//size_t finishReduce;
		};
		bool operator==(const Action& other) const { return this->type == other.type && this->nextState == other.nextState; }
	};

	typedef std::map<Token, Action> ActionMap;

	LalrParsingTable(const Productions& p, size_t size): productions(p), table(size) {}

	size_t size() const { return this->table.size(); }

	const ActionMap& operator[](size_t i) const { return this->table[i]; }
	ActionMap& operator[](size_t i) { return this->table[i]; }

	const Production& getProduction(size_t i) const { return this->productions[i]; }

protected:
	Productions productions;
	std::vector<ActionMap> table;
};
#endif
