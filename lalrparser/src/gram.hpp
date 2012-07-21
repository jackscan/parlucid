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
