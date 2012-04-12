#ifndef LALRPARSER_HPP
#define LALRPARSER_HPP

#include "gram.hpp"
#include "lalrparsingtable.hpp"
#include <deque>
#include <vector>

template<typename T, T N> class LalrParser
{
public:
	typedef T Token;
	typedef typename Grammar<T>::String String;
	typedef LalrParsingTable<T> Table;
	typedef typename Grammar<T>::Production Production;

	enum{ PARSE_ERR = -1, SHIFT = -2, AMBIGUOUS = -3 };

	struct StackElement : protected std::pair<Token,size_t> {
		StackElement() {}
		StackElement(const Token& symbol, size_t state): std::pair<Token,size_t>(std::make_pair(symbol,state)) {}
		Token& symbol() { return this->first; }
		size_t& state() { return this->second; }
		const Token& symbol() const { return this->first; }
		const size_t& state() const { return this->second; }
	};

	LalrParser(const Table&);

	bool setSpecialTokens(const std::set<Token>&);

	void reset();
	int next(const Token&);
	Token accepted() const;
	Token special() const;

	size_t getStackSize() const;
	const StackElement& getStackElement(size_t i) const { return stack[i]; }

	const Production& getProduction(size_t);

	bool isValid(Token) const;
	std::set<Token> validTokens() const;

protected:

	struct Stack : public std::deque<StackElement>
	{
		typename std::deque<StackElement>::reference top() { return this->back(); }
		typename std::deque<StackElement>::const_reference top() const { return this->back(); }
		void push(const typename std::deque<StackElement>::value_type& val) { this->push_back(val); }
	};

	const Table table;
	std::vector<Token> specialTokens;
	Stack stack;
	Token acceptedSymbol;
};

#endif
