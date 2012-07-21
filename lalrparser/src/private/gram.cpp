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

#include "../gram.hpp"
#include "../../../utils/src/helper.hpp"

template<typename T> const typename Grammar<T>::Set Grammar<T>::emptySet;

template<typename T> void Grammar<T>::addProduction(const Token& head, const String& body)
{
	if (this->productions.empty()) this->setStartSymbol(head);
	this->productionIndex[head].insert(this->productions.size());
	this->productions.push_back(Production(head,body));
}

template<typename T> void Grammar<T>::delProduction(ProductionNo prodNo)
{
	Token head = this->productions[prodNo].head();
	this->productions.erase(this->productions.begin() + prodNo);
	typename std::map<Token,std::set<ProductionNo> >::iterator iProds = this->productionIndex.find(head);
	iProds->second.erase(prodNo);
	if (iProds->second.empty()) this->productionIndex.erase(iProds);
    // update index for productions > prodNo
    for (typename std::map<Token,std::set<ProductionNo> >::iterator iIndex = productionIndex.begin(); iIndex != productionIndex.end(); ++iIndex)
    {
    	std::set<ProductionNo> newProds;
        for (typename std::set<ProductionNo>::iterator iProdNo = iIndex->second.begin(); iProdNo != iIndex->second.end(); ++iProdNo)
            newProds.insert(*iProdNo > prodNo ? *iProdNo - 1 : *iProdNo);
        iIndex->second.swap(newProds);
    }
}

template<typename T> void Grammar<T>::delProductions(const Token& head)
{
	this->productionIndex.erase(head);
	for (typename Productions::iterator iter = this->productions.begin(); iter != this->productions.end();)
		if (iter->head() == head)
			iter = this->productions.erase(iter);
		else
			++iter;

	if (this->startSymbol == head)
	{
		this->startSymbol = this->productions.empty() ? 0 : this->productions.front().head();
	}

	this->reset();
}

template<typename T> const typename Grammar<T>::Productions& Grammar<T>::getProductions() const
{
	return this->productions;
}

template<typename T> const std::set<typename Grammar<T>::ProductionNo>& Grammar<T>::getProductions(const Token& head) const
{
	typename ProductionIndex::const_iterator iProds = productionIndex.find(head);
	if (iProds != productionIndex.end()) return iProds->second;
	else
	{
		static const std::set<ProductionNo> emptyIndex;
		return emptyIndex;
	}
}


template<typename T> void Grammar<T>::clear()
{
	this->productions.clear();
	this->productionIndex.clear();
	this->reset();
}

template<typename T> void Grammar<T>::reset()
{
	this->epsFirst.clear();
	this->firstSets.clear();
	this->followSets.clear();
}

template<typename T> bool Grammar<T>::ready()
{
	return !this->firstSets.empty() && !this->followSets.empty();
}

template<typename T> const typename Grammar<T>::Set& Grammar<T>::first(const Token& symbol) const
{
	typename MapSet::const_iterator is = this->firstSets.find(symbol);
	if(is == this->firstSets.end()) return emptySet;
	else return is->second;
}

template<typename T> const typename Grammar<T>::Set& Grammar<T>::follow(const Token& symbol) const
{
	typename MapSet::const_iterator is = this->firstSets.find(symbol);
	if(is == this->firstSets.end()) return emptySet;
	else return is->second;
}

template<typename T> typename Grammar<T>::Set& Grammar<T>::first(const String& str, Set& result, bool& bEpsFirst) const
{
	size_t i;
	size_t length = str.length();
	for(i = 0; i < length; ++i)
	{
		Token tok = str[i];
		const Set& firstSet = this->first(tok);
		if(firstSet.empty()) result.insert(tok);
		else set_union_add(result, firstSet);
		if(!this->epsilon(tok)) break;
	}
	bEpsFirst = i == length;
	return result;
}

template<typename T> void Grammar<T>::calcFirst()
{
	size_t size = 0;
	size_t oldsize;
	do
	{
		oldsize = size;
		size = this->epsFirst.size();
		typename Productions::const_iterator ip;
		for(ip = this->productions.begin();ip != this->productions.end(); ++ip)
		{
			const Token& head = ip->head();
			const String& body = ip->body();
			Set& first = this->firstSets[head];
			typename String::const_iterator is;
			for(is = body.begin(); is != body.end(); ++is)
			{
				first.insert(*is);
				typename MapSet::iterator iFirst = this->firstSets.find(*is);
				if(iFirst != this->firstSets.end())
					set_union_add(first, iFirst->second);
				if(!this->epsilon(*is)) break;
			}
			if(is == body.end()) this->epsFirst.insert(head);
			size += first.size();
		}
	}
	while(oldsize != size);
}

template<typename T> void Grammar<T>::calcFollow()
{
	// stores non-terminals, which could be last of derivation
	// B is in deps[A], if A->aBy and y-><epsilon>
	MapSet deps;

	typename Productions::const_iterator ip;
	for(ip = this->productions.begin();ip != this->productions.end(); ++ip)
	{
		const Token& head = ip->head();
		const String& body = ip->body();
		bool epsSuffix = true;
		Set& dep = deps[head];
		Set tempFollows;
		typename String::const_reverse_iterator is;
		for(is = body.rbegin(); is != body.rend(); ++is)
		{
			typename MapSet::const_iterator ifirst = this->firstSets.find(*is);
			if(ifirst != this->firstSets.end())
			{// to calculate follow sets for all symbols, remove if-condition
				set_union_add(this->followSets[*is], tempFollows);
				if(epsSuffix) dep.insert(*is);
			}
			if(!this->epsilon(*is))
			{
				tempFollows.clear();
				epsSuffix = false;
			}
			if(ifirst != this->firstSets.end())
				set_union_add(tempFollows, ifirst->second);
			tempFollows.insert(*is);
		}
	}

	size_t size = 0;
	size_t oldsize = 0;

	do
	{
		oldsize = size;
		size = 0;

		typename MapSet::const_iterator iDeps = deps.begin();
		for(; iDeps != deps.end(); ++iDeps)
		{
			Set& follows = this->followSets[iDeps->first];
			const Set& dep = iDeps->second;
			typename Set::const_iterator iDep = dep.begin();
			for(; iDep != dep.end(); ++iDep)
			{
				Set& depFollows = this->followSets[*iDep];
				set_union_add(depFollows, follows);
				size += depFollows.size();
			}
		}
	}
	while(oldsize != size);
}

template class Grammar<char>;
template class Grammar<int>;
