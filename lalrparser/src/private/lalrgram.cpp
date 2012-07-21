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

#include "../lalrgram.hpp"
#include "../../../utils/src/helper.hpp"

#include <queue>
#include <cassert>

template<typename T, T N> const T LalrGrammar<T,N>::endToken = N;

template<typename T, T N> void LalrGrammar<T,N>::clear()
{
	this->gotos.clear();
	this->informations.clear();
	this->collection.clear();
	static_cast<Grammar<T>*>(this)->clear();
}

template<typename T, T N> typename LalrGrammar<T,N>::Table LalrGrammar<T,N>::createParsingTable()
{
	this->errors.clear();
	this->ambiguousProductions.clear();

	// if firsts or follows are empty reset calculated sets
	if (this->firstSets.empty() || this->followSets.empty())
	{
		this->firstSets.clear();
		this->followSets.clear();
		this->gotos.clear();
		this->informations.clear();
		this->collection.clear();

		this->calcFirst();

		// add firsts of ignore productions to all firsts of all non terminals
		/*{
			Set ignoreFirsts;
			typename ProductionIndex::const_iterator iIgnoreProds = this->productionIndex.find(N);
			if (iIgnoreProds != this->productionIndex.end())
			{
				const std::set<ProductionNo>& ignoreProds = iIgnoreProds->second;

				for(typename std::set<ProductionNo>::const_iterator iProd = ignoreProds.begin(); iProd != ignoreProds.end(); ++iProd)
				// dont check for start productions since it's not added yet
				//if (isIgnoreProduction(*iProd))
				{
					bool bEpsFirst;
					this->first(this->productions[*iProd].body(), ignoreFirsts, bEpsFirst);
				}
			}

			for (typename MapSet::iterator iter = this->firstSets.begin(); iter != this->firstSets.end(); ++iter)
				set_union_add(iter->second, ignoreFirsts);
		}*/

		this->calcFollow();
		this->calcIgnoreStartInfo();
	}

	if (this->collection.empty()) this->calcCollection();

	Table table(this->productions, this->collection.size());

	//printf("creating table\n");

	for(typename Collection::const_iterator ic = this->collection.begin(); ic != this->collection.end(); ++ic)
	{
		typename Table::ActionMap &row = table[ic->id];
		for(typename InfoItems::const_iterator ii = ic->info.items.begin(); ii != ic->info.items.end(); ++ii)
		{
			const ItemCore &item = ii->first;
			const String &body = this->productions[item.productionNo].body();

			if(item.dot == body.length())
			{	// reduce
				const Set &follows = ii->second;
				for(typename Set::const_iterator is = follows.begin(); is != follows.end(); ++is)
				{
					const Token& next = *is;

					Action action;
					action.type = Table::REDUCE;
					action.productionNo = item.productionNo;

					std::pair<typename Table::ActionMap::iterator, bool> result = row.insert(std::make_pair(next, action));

					if(!result.second)
					{
						//printf("conflict at %u\n", ic->id);
						bool newIgnorable = isIgnorableAction(action);
						bool oldIgnorable = isIgnorableAction(result.first->second);

						// check if new and old action have same ignore state
						if (newIgnorable == oldIgnorable)
						{
							// if both are ignorable and it's a shift-reduce conflict prefer reduce
							// we could also prefer shifts, but this would reverse reduction order
							if (newIgnorable && result.first->second.type == Table::SHIFT)
								;// keep old action
							//there's an error
							else this->errors.push_back(Error(ic->id, next, result.first->second, action));

							/*if(result.first->second.type == Table::SHIFT)
								this->errors.push_back(Error(ic->id, next, result.first->second, action));
							else
							{
								this->errors.push_back(Error(ic->id, next, result.first->second, action));
								//this->ambiguousProductions.insert(result.first->second.productionNo);
								//this->ambiguousProductions.insert(item.productionNo);
							}*/
						}
						// check if old action leads into ignore and new action does not
						else if (oldIgnorable)
							//overwrite with new one
							result.first->second = action;
						// else ignore new ignorable action
					}
				}
			}
			else
			{	// shift

				Token next = body[item.dot];
				Action action;
				action.type = Table::SHIFT;
				action.nextState = this->gotos[ic->id].find(next)->second;

				std::pair<typename Table::ActionMap::iterator, bool> result = row.insert(std::make_pair(next, action));

				if(!result.second && !(result.first->second == action))
				{
					//printf("conflict at %u\n", ic->id);

					bool newIgnorable = isIgnorableAction(action);
					bool oldIgnorable = isIgnorableAction(result.first->second);

					// check if new and old action have same ignore state
					if (newIgnorable == oldIgnorable)
					{
						// if both are ignorable and it's a shift-reduce conflict prefer reduce
						// we could also prefer shifts, but this would reverse reduction order
						if (newIgnorable && result.first->second.type == Table::REDUCE)
							result.first->second = action;
						else //there's an error
							//assert(result.first->second.type == Table::REDUCE
							//	|| result.first->second.nextState == action.nextState);
							this->errors.push_back(Error(ic->id, next, result.first->second, action));
					}
					// check if old action leads into ignore and new action does not
					else if (oldIgnorable)
						//overwrite with new one
						result.first->second = action;
					// else ignore new ignorable action
				}
			}
		}
	}
	return table;
}

template<typename T, T N> const typename LalrGrammar<T, N>::Information& LalrGrammar<T, N>::getInformation(size_t state) const
{
	return informations[state]->info;
}

template<typename T, T N> size_t LalrGrammar<T, N>::getNumberOfStates() const
{
	return informations.size();
}

template<typename T, T N> const std::vector<typename LalrGrammar<T,N>::Error>& LalrGrammar<T,N>::getErrors() const
{
	return this->errors;
}

template<typename T, T N> const std::set<typename LalrGrammar<T,N>::ProductionNo>& LalrGrammar<T,N>::getAmbiguousProductions() const
{
	return this->ambiguousProductions;
}

template<typename T, T N> bool LalrGrammar<T,N>::ItemCore::operator==(const ItemCore& other) const
{
	return this->productionNo == other.productionNo && this->dot == other.dot && this->ignore == other.ignore;
}

template<typename T, T N> bool LalrGrammar<T,N>::ItemCore::operator<(const ItemCore& other) const
{
	return this->ignore < other.ignore
		|| (this->ignore == other.ignore
			&& (this->productionNo < other.productionNo
				|| (this->productionNo == other.productionNo && this->dot < other.dot)));
}

template<typename T, T N> bool LalrGrammar<T,N>::CoreCompare::operator()(const Item& item1, const Item& item2) const
{
	return item1.first < item2.first;
}

template<typename T, T N> bool LalrGrammar<T,N>::InfoCoreCompare::operator()(const TaggedInfo& tinfo1, const TaggedInfo& tinfo2) const
{
	const Information& info1 = tinfo1.info;
	const Information& info2 = tinfo2.info;
	//todo: check calc
	 return (info1.items.size() < info2.items.size()
       || (info1.items.size() == info2.items.size() &&
			std::lexicographical_compare(info1.items.begin(), info1.items.end(), info2.items.begin(), info2.items.end(), CoreCompare())));
	//old: return info1.ignore < info2.ignore || info1.ignore == info2.ignore &&
	//	(info1.items.size() < info2.items.size() || info1.items.size() == info2.items.size() &&
	//		std::lexicographical_compare(info1.items.begin(), info1.items.end(), info2.items.begin(), info2.items.end(), CoreCompare()));
}

template<typename T, T N> std::pair<size_t, bool> LalrGrammar<T,N>::addInfo(Information& info)
{
	size_t i = this->collection.size();
	std::pair<typename Collection::iterator,bool> insResult = this->collection.insert(TaggedInfo(info,i));
	bool bNewInfo = true;
	if(!insResult.second)
	{
		// workaround for sgi stl
		// because set::iterator is a const_iterator
		// Information is removed from collection and reinserted after modification
		TaggedInfo resInfo = *insResult.first;
		//size_t before = this->collection.size();
		this->collection.erase(insResult.first);
		//size_t after = this->collection.size();
		bNewInfo = LalrGrammar<T, N>::info_union_add(resInfo.info, info);
		insResult = this->collection.insert(resInfo);
		//update informations
		this->informations[resInfo.id] = insResult.first;

		//size_t final = this->collection.size();
		//bNewInfo = LalrGrammar<T, N>::info_union_add(insResult.first->info, info);
		//size_t after = this->collection.size();
	}
	else
	{
		this->informations.push_back(insResult.first);
		this->gotos.push_back(typename GotoInformations::value_type());
	}
	return std::make_pair(insResult.first->id, bNewInfo);
}

template<typename T, T N> void LalrGrammar<T,N>::calcCollection()
{
	std::queue<size_t> unmarkedInfos;
	// if there are no productions, nothing to calculate
	if(this->productions.empty()) return;
	//Token startSymbol = this->productions.front().head();

	// add start production for reducing to zero stack size
	String startBody(2, N); startBody[1] = this->startSymbol;
	ProductionNo startProd = Grammar<T>::getProductions().size();
	this->addProduction(N, startBody);
	// add info [N->N.S, N]
	Information startInfo;
	Set follow; follow.insert(N);
	startInfo.items[ItemCore(startProd, 1, false)] = follow;
	unmarkedInfos.push(this->addInfo(closure(startInfo)).first);

	while(!unmarkedInfos.empty())
	{
		size_t nInfo = unmarkedInfos.front();
		const Information &info = this->informations[nInfo]->info;
		// calculate following infos (i.e. states)
		std::map<Token,Information> gotoInfos;
		for(typename InfoItems::const_iterator i = info.items.begin(); i != info.items.end(); ++i)
		{
			const ItemCore &core = i->first;
			const String& body = this->productions[core.productionNo].body();
			if(core.dot < body.length())
			{
				//bool ignore = info.ignore || this->isIgnoreProduction(core.productionNo);
				Item item(ItemCore(core.productionNo, core.dot+1, core.ignore), i->second);

				// todo: check this calc
				gotoInfos[body[core.dot]].items.insert(item);

				// old:
				/*typename std::map<Token,Information>::iterator iGotoInfo = gotoInfos.find(body[core.dot]);
				if(iGotoInfo == gotoInfos.end() || iGotoInfo->second.ignore == ignore)
				{ // old goto info does not exist or ignore flag is the same, add item
					gotoInfos[body[core.dot]].items.insert(item);
					gotoInfos[body[core.dot]].ignore = ignore;
				}
				else if(iGotoInfo->second.ignore)
				{ // new is no ignore info but old is, replace old items with new one
					iGotoInfo->second.items.clear();
					iGotoInfo->second.items.insert(item);
					iGotoInfo->second.ignore = ignore;
				}*/


			}
		}

		for(typename std::map<Token,Information>::iterator igi = gotoInfos.begin(); igi != gotoInfos.end(); ++igi)
		{
			std::pair<size_t, bool> result = this->addInfo(closure(igi->second));
			// if new info or info changed, store as unmarked
			if(result.second)
				unmarkedInfos.push(result.first);
			this->gotos[nInfo][igi->first] = result.first;
		}

		unmarkedInfos.pop();
	}
}

template<typename T, T N> bool LalrGrammar<T,N>::core_equal(const Information& info1, const Information& info2)
{
	if(info1.items.size() < info2.items.size()) return false;
	return std::equal(info1.items.begin(), info1.items.end(), info2.items.begin(), CoreCompare());
}

template<typename T, T N> bool LalrGrammar<T,N>::info_union_add(Information& info1, const Information& info2)
{
	bool bChanged = false;
	size_t oldInfoSize = info1.items.size();
	for(typename InfoItems::const_iterator i2 = info2.items.begin(); i2 != info2.items.end(); ++i2)
	{
		Set& follow = info1.items[i2->first]; size_t oldFollowSize = follow.size();
		if(set_union_add(follow, i2->second).size() > oldFollowSize) bChanged = true;
	}
	return bChanged || info1.items.size() > oldInfoSize;
}

template<typename T, T N> void LalrGrammar<T,N>::calcIgnoreStartInfo()
{
	ignoreStartInfo.items.clear();
	ignoreFirsts.clear();

	typename ProductionIndex::const_iterator iIgnoreProds = this->productionIndex.find(N);
	if (iIgnoreProds != this->productionIndex.end())
	{
		const std::set<ProductionNo>& ignoreProds = iIgnoreProds->second;

		for(typename std::set<ProductionNo>::const_iterator iProd = ignoreProds.begin(); iProd != ignoreProds.end(); ++iProd)
			//if (!isStartProduction(*iProd))
			{
				//bool bEpsFirst;
				//Set firsts;
				//this->first(this->productions[*iProd].body(), firsts, bEpsFirst);
				//for(typename Set::const_iterator iFirst = firsts.begin(); iFirst != firsts.end(); ++iFirst)
				{
					std::pair<typename InfoItems::iterator, bool> insResult = ignoreStartInfo.items.insert(std::make_pair(ItemCore(*iProd, 0, true), Set()));
					//if (insResult.second) toVisit.insert(insResult.first->first);
				}
			}
	}

	// calc closure and add firsts to follows
	calcClosure(ignoreStartInfo, ignoreFirsts);
	ignoreStartInfo.addFollow(ignoreFirsts);
}

template<typename T, T N> typename LalrGrammar<T,N>::Information& LalrGrammar<T,N>::calcClosure(Information& info, Set& infoFirsts) const
{
	std::set<ItemCore> toVisit;
	for(typename InfoItems::const_iterator i = info.items.begin(); i != info.items.end(); ++i)
		toVisit.insert(i->first);

	while(!toVisit.empty())
	{
		typename InfoItems::const_iterator i = info.items.find(*toVisit.begin());
		toVisit.erase(toVisit.begin());

		const String &body = this->productions[i->first.productionNo].body();

		size_t dot = i->first.dot;
		if (dot < body.length())
		{
			Token next = body[dot];
			String remain = body.substr(dot+1);
			//calculate set of tokens following current token 'next' in this item
			Set follow;
			bool bEpsFirst = false;
			this->first(remain, follow, bEpsFirst);
			// add follows of current item if remain can evaluate to epsilon
			if(bEpsFirst) set_union_add(follow, i->second);

			// calc firsts of this item
			// todo: check this calculation
			//	old was: set_union_add(infoFirsts, follow);
			{
				bool eps = false;
				this->first(body.substr(dot), infoFirsts, eps);
				if(eps) set_union_add(infoFirsts, i->second);
			}

			// iterate through all productions with 'next' as head
			typename ProductionIndex::const_iterator ips = this->productionIndex.find(next);
			if(ips != this->productionIndex.end())
			{
				const std::set<ProductionNo>& prods = ips->second;
				for(typename std::set<ProductionNo>::const_iterator ip = prods.begin(); ip != prods.end(); ++ip)
				{
					// add starting items with productions where 'next' is head
					std::pair<typename InfoItems::iterator, bool> insResult = info.items.insert(std::make_pair(ItemCore(*ip, 0, i->first.ignore), Set()));
					set_union_add(insResult.first->second, follow);
					if (insResult.second) toVisit.insert(insResult.first->first);
				}
			}
			// else next is terminal
		}
		if (dot >= body.length()) // add follow of item to firsts of this info
			set_union_add(infoFirsts, i->second);
	}

	return info;
}

template<typename T, T N> typename LalrGrammar<T,N>::Information& LalrGrammar<T,N>::closure(Information& info) const
{
	Set infoFirsts;

	bool ignoreInfo = info.isIgnorable();

	// calc closure without ignore items
	calcClosure(info, infoFirsts);
	if (!ignoreInfo && !ignoreStartInfo.items.empty())
	{
		bool onlyReduce = true;
		// check if info contains only reduceable items, i.e. item dot points to end of production body
		for (typename InfoItems::const_iterator iter = info.items.begin(); iter != info.items.end(); ++iter)
		{
			const String &body = this->productions[iter->first.productionNo].body();
			size_t dot = iter->first.dot;
			if (dot < body.length())
			{
				onlyReduce = false;
				break;
			}
		}
		
		if (!onlyReduce)
		{
			// for all items of ignoreStartInfo
			for (typename InfoItems::const_iterator iter = ignoreStartInfo.items.begin(); iter != ignoreStartInfo.items.end(); ++iter)
			{
				const String &body = this->productions[iter->first.productionNo].body();
				size_t dot = iter->first.dot;
				assert(0 == dot);
				// // with first not in firsts of regular items
				if (dot < body.length() /*&& infoFirsts.find(body[dot]) == infoFirsts.end()*/)
				{
					// make copy
					Item ignoreItem = *iter;
					// add firsts of regular items to follows of ignore item
					set_union_add(ignoreItem.second, infoFirsts);
					// and add to info
					info.items.insert(ignoreItem);
				}
				// else this is an ignore production with empty body?
			}
			// calc closure with ignore items
			calcClosure(info, infoFirsts);
		}
	}


	/*if (!ignoreInfo)
	{
		// add firsts of ignore productions to infoFirsts
		typename ProductionIndex::const_iterator iIgnoreProds = this->productionIndex.find(N);
		if (iIgnoreProds != this->productionIndex.end())
		{
			const std::set<ProductionNo>& ignoreProds = iIgnoreProds->second;

			for(typename std::set<ProductionNo>::const_iterator iProd = ignoreProds.begin(); iProd != ignoreProds.end(); ++iProd)
				if (!isStartProduction(*iProd))
				{
					bool bEpsFirst;
					Set firsts;
					this->first(this->productions[*iProd].body(), firsts, bEpsFirst);
					set_union_add(infoFirsts, firsts);
				}
		}
	}*/



	// if this is no pure ignore info add ignore start items
	/*if(!ignoreInfo && toVisit.empty())
	{
		typename ProductionIndex::const_iterator iIgnoreProds = this->productionIndex.find(N);
		if (iIgnoreProds != this->productionIndex.end())
		{
			const std::set<ProductionNo>& ignoreProds = iIgnoreProds->second;

			for(typename std::set<ProductionNo>::const_iterator iProd = ignoreProds.begin(); iProd != ignoreProds.end(); ++iProd)
				if (!isStartProduction(*iProd))
				{
					//bool bEpsFirst;
					//Set firsts;
					//this->first(this->productions[*iProd].body(), firsts, bEpsFirst);
					//for(typename Set::const_iterator iFirst = firsts.begin(); iFirst != firsts.end(); ++iFirst)
					{
						std::pair<typename InfoItems::iterator, bool> insResult = info.items.insert(std::make_pair(ItemCore(*iProd, 0, true), infoFirsts));
						if (insResult.second) toVisit.insert(insResult.first->first);
					}
				}
		}
	}*/

	/*do
	{
		oldsize = size; size = 0;
		for(typename InfoItems::const_iterator i = info.items.begin(); i != info.items.end(); ++i)
		{
			const String &body = this->productions[i->first.productionNo].body();

			size_t dot = i->first.dot;
			//for (; dot < body.length(); ++dot)
			if (dot < body.length())
			{
				Token next = body[dot];
				String remain = body.substr(dot+1);
				//calculate set of tokens following current token 'next' in this item
				Set follow;
				bool bEpsFirst = false;
				this->first(remain, follow, bEpsFirst);
				// add follows of current item if remain can evaluate to epsilon
				if(bEpsFirst) set_union_add(follow, i->second);

				// add to firsts of this information
				set_union_add(infoFirsts, follow);

				// iterate through all productions with 'next' as head
				typename ProductionIndex::const_iterator ips = this->productionIndex.find(next);
				if(ips != this->productionIndex.end())
				{
					const std::set<ProductionNo>& prods = ips->second;
					for(typename std::set<ProductionNo>::const_iterator ip = prods.begin(); ip != prods.end(); ++ip)
					{
						// add starting items with productions where 'next' is head
						Set& itemFollow = info.items[ItemCore(*ip, 0)];
						size_t nFollows = itemFollow.size();
						set_union_add(itemFollow, follow);
						size += (itemFollow.size() - nFollows);
					}
				}
				// else next is terminal

				//if (!this->epsilon(next)) break;
				//else
				//{
				//	Set& itemFollow = info.items[ItemCore(i->first.productionNo, dot+1)];
				//	size_t nFollows = itemFollow.size();
				//	set_union_add(itemFollow, follow);
				//	size += (itemFollow.size() - nFollows);
				//}
			}
			if (dot >= body.length()) // add follow of item to firsts of this info
				set_union_add(infoFirsts, i->second);
		}

		// if this is no ignore info add ignore start items
		if(!info.ignore && oldsize == size)
		{
			typename ProductionIndex::const_iterator iIgnoreProds = this->productionIndex.find(N);
			if (iIgnoreProds != this->productionIndex.end())
			{
				const std::set<ProductionNo>& ignoreProds = iIgnoreProds->second;

				for(typename std::set<ProductionNo>::const_iterator iProd = ignoreProds.begin(); iProd != ignoreProds.end(); ++iProd)
					if (!isStartProduction(*iProd))
					{
						bool bEpsFirst;
						Set firsts;
						this->first(this->productions[*iProd].body(), firsts, bEpsFirst);
						for(typename Set::const_iterator iFirst = firsts.begin(); iFirst != firsts.end(); ++iFirst)
						{
							size += infoFirsts.size() - info.items[ItemCore(*iProd, 0)].size();
							info.items[ItemCore(*iProd, 0)] = infoFirsts;
						}
					}
			}
		}
	}
	while(oldsize != size);*/

	return info;
}

template<typename T, T N> bool LalrGrammar<T,N>::isStartProduction(ProductionNo prodNo) const
{
	return prodNo == this->productions.size() - 1;
}

template<typename T, T N> bool LalrGrammar<T,N>::isIgnoreProduction(ProductionNo prodNo) const
{
	return this->productions[prodNo].head() == N && !isStartProduction(prodNo);
}

template<typename T, T N> bool LalrGrammar<T,N>::Information::isIgnorable() const
{
	for (typename InfoItems::const_iterator iter = items.begin(); iter != items.end(); ++iter)
		if (!iter->first.ignore) return false;
	return true;
}

template<typename T, T N> void LalrGrammar<T,N>::Information::addFollow(const Set& follows)
{
	for (typename InfoItems::iterator iter = items.begin(); iter != items.end(); ++iter)
		set_union_add(iter->second, follows);
}

template<typename T, T N> bool LalrGrammar<T,N>::isIgnorableAction(const Action& action) const
{
	return (action.type == Table::SHIFT && getInformation(action.nextState).isIgnorable())
		|| (action.type == Table::REDUCE && isIgnoreProduction(action.productionNo));
}

template class LalrGrammar<char>;
template class LalrGrammar<int>;

