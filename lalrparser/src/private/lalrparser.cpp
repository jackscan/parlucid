#include "../lalrparser.hpp"

template<typename T,T N> LalrParser<T,N>::LalrParser(const Table& table) :table(table)
{ }

template<typename T,T N> void LalrParser<T,N>::reset()
{
	this->acceptedSymbol = N;
	this->stack.clear();
}

template<typename T,T N> int LalrParser<T,N>::next(const Token& token)
{
	this->acceptedSymbol = N;
	size_t state = this->stack.empty() ? 0 : this->stack.top().state();
	const typename Table::ActionMap& row = this->table[state];
	typename Table::ActionMap::const_iterator ia = row.find(token);
	if(ia == row.end() && (ia = row.find(N)) == row.end()) return PARSE_ERR;
	const typename Table::Action& action = ia->second;
	if(action.type == Table::SHIFT)
	{
		this->stack.push_back(StackElement(token, action.nextState));
		return SHIFT;
	}
	else if(action.type == Table::REDUCE)
	{
		const Production& production = this->table.getProduction(action.productionNo);
		if(production.body().length() > this->stack.size()) return PARSE_ERR;
		this->stack.resize(this->stack.size() - production.body().length());
		state = this->stack.empty() ? 0 : this->stack.top().state();
		if(this->stack.empty())
			this->acceptedSymbol = production.head();
		if (production.head() != N)
		{
			ia = this->table[state].find(production.head());
			if(ia != this->table[state].end())
				this->stack.push(StackElement(production.head(), ia->second.nextState));
		}
		return static_cast<int>(action.productionNo);
	}
	else return AMBIGUOUS;
}

template<typename T,T N> T LalrParser<T,N>::accepted() const
{
	return this->acceptedSymbol;
}

template<typename T,T N> T LalrParser<T,N>::special() const
{
	size_t state = this->stack.empty() ? 0 : this->stack.top().state();
	return state < this->specialTokens.size() ? this->specialTokens[state] : N;
}

template<typename T,T N> bool LalrParser<T,N>::setSpecialTokens(const std::set<Token>& specToks)
{
	this->specialTokens.clear();
	this->specialTokens.resize(this->table.size());
	for(size_t state = 0; state < this->table.size(); ++state)
	{
		const typename Table::ActionMap& row = this->table[state];
		Token specTok = N;
		bool bSpecial = true;
		for(typename Table::ActionMap::const_iterator ir = row.begin(); ir != row.end(); ++ir)
		{
			if(specToks.find(ir->first) != specToks.end())
			{
				if(specTok != N && specTok != ir->first) return false;
				specTok = ir->first;
			}
			else bSpecial = false;
		}
		if(!bSpecial && specTok != N) return false;
		this->specialTokens[state] = specTok;
	}
	return true;
}

template<typename T,T N> size_t LalrParser<T,N>::getStackSize() const
{
	return this->stack.size();
}

template<typename T,T N> const typename LalrParser<T,N>::Production& LalrParser<T,N>::getProduction(size_t i)
{
	return this->table.getProduction(i);
}

template<typename T,T N> bool LalrParser<T,N>::isValid(Token token) const
{
	size_t state = this->stack.empty() ? 0 : this->stack.top().state();
	return this->table[state].find(token) != this->table[state].end();
}

template<typename T,T N> std::set<typename LalrParser<T,N>::Token> LalrParser<T,N>::validTokens() const
{
	std::set<Token> result;
/*	for(Stack::const_iterator is = this->stack.begin(); is != this->stack.end(); ++is)
		result.insert(is->symbol());
	return result;*/
	size_t state = this->stack.empty() ? 0 : this->stack.top().state();
	const typename Table::ActionMap& row = this->table[state];
	for(typename Table::ActionMap::const_iterator ia = row.begin(); ia != row.end(); ++ia)
		result.insert(ia->first);
	return result;
}

template class LalrParser<char,'\0'>;
template class LalrParser<int,0>;
