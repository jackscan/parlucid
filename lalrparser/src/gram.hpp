#ifndef GRAM_HPP
#define GRAM_HPP

#include <string>
#include <utility>
#include <vector>
#include <set>
#include <map>

template<typename T> class Grammar
{
public:
	typedef T Token;
	typedef std::basic_string<Token> String;
	typedef std::set<Token> Set;
	typedef std::map<Token,Set> MapSet;

	struct Production: private std::pair<Token,String> {
		Production(const Token& h, const String& b): std::pair<Token,String>(h,b) {}
		const Token& head() const { return this->first; }
		const String& body() const { return this->second; }
	};

	typedef std::vector<Production> Productions;
	typedef typename Productions::size_type ProductionNo;

	Grammar() : startSymbol(0) {}

	void addProduction(const Token&, const String&);
	void delProductions(const Token&);
	void delProduction(ProductionNo);
	void clear();
	bool epsilon(const Token&) const;
	bool terminal(const Token&) const;
	const Set& first(const Token&) const;
	const Set& follow(const Token&) const;
	Set& first(const String&, Set&, bool&) const;
	const Productions& getProductions() const;
	const std::set<ProductionNo>& getProductions(const Token& head) const;
	void setStartSymbol(const Token& );
	const Token& getStartSymbol() const;

protected:
	void reset();
	bool ready();
	void calcFirst();
	void calcFollow();

	typedef std::map<Token,std::set<ProductionNo> > ProductionIndex;

	Productions productions;
	Token startSymbol;
	ProductionIndex productionIndex;
	MapSet firstSets, followSets;
	Set epsFirst;

	static const Set emptySet;
};

template<typename T>
inline bool Grammar<T>::epsilon(const Token& symbol) const
{
	return this->epsFirst.count(symbol) > 0;
}

template<typename T>
inline bool Grammar<T>::terminal(const Token& symbol) const
{
	return this->productionIndex.count(symbol) == 0;
}

template<typename T>
inline void Grammar<T>::setStartSymbol(const Token& sym)
{
	this->startSymbol = sym;
}

template<typename T>
inline const typename Grammar<T>::Token& Grammar<T>::getStartSymbol() const
{
	return this->startSymbol;
}

#endif
