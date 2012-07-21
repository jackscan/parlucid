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

#include "../alphabetgen.hpp"
#include "../../../lexer/src/alphabet.hpp"
//#include "../../../genutils/src/codestream.hpp"
#include <ostream>
#include <cassert>
#include <limits>
#include <functional>
#include <algorithm>

template class AlphabetGen<char>;

template<typename C>
const typename AlphabetGen<C>::Result AlphabetGen<C>::errorResult;

template<typename C>
AlphabetGen<C>::AlphabetGen(lua_State *L, const Alphabet<Char>& alphabet, const Results& results)
: lua(L)
{
	Alphabet<Char> singletons(alphabet.singletons().begin(), alphabet.singletons().end());
	singletons.merge();
	Alphabet<Char> intervals(alphabet);
	intervals.del(singletons);
	std::set<Interval<Char> > spaces(singletons.intervals());
	for (typename std::set<Char>::const_iterator iter = singletons.singletons().begin(); iter != singletons.singletons().end(); ++iter)
		spaces.insert(Interval<Char>(*iter));
	intervals.roundBoundaries(spaces);

	// do merge after round again
	std::map<Result, std::set<Char> > resultMap;

	{
		for (typename Results::const_iterator iResults = results.begin(); iResults != results.end(); ++iResults)
			if (!singletons.contains(iResults->first))
				resultMap[iResults->second].insert(iResults->first);
	}

	for (typename std::map<Result, std::set<Char> >::const_iterator iResultMap = resultMap.begin(); iResultMap != resultMap.end(); ++iResultMap)
		intervals.merge(iResultMap->second);

	assert(intervals.singletons().empty());

	for(typename std::set<Char>::const_iterator iter = alphabet.singletons().begin(); iter != alphabet.singletons().end(); ++iter)
	{
		typename Results::const_iterator iEntry = results.find(*iter);
		singletonResults[*iter] = iEntry != results.end() ? iEntry->second : errorResult;
	}

	for(typename Results::const_iterator iter = results.begin(); iter != results.end(); ++iter)
		if (!singletons.contains(iter->first))
		{
			Interval<Char> interval(intervals.lower_bound(iter->first), intervals.upper_bound(iter->first));
			if (!interval.empty())
				intervalResults[interval] = iter->second;
			else
				assert(singletonResults.find(iter->first) != singletonResults.end());
		}

	lua_createtable(lua, static_cast<int>(alphabet.singletons().size()), 1);

	if (!singletons.empty())
	{
		int i = 0;
		for (typename std::set<Char>::const_iterator iter = alphabet.singletons().begin(); iter != alphabet.singletons().end(); ++iter)
			//stream << "case " << *iter << ": return " << singletonResult(*iter) << ";" << newline;
		{
			lua_pushinteger(lua, ++i);
			lua_createtable(lua, 0, 3);
			pushLuaChar(*iter);
			lua_setfield(lua, -2, "char");

			typename Results::const_iterator iResults = results.find(*iter);
			if (iResults != results.end())
			{
				setLuaResult(iResults->second);
			}
			lua_settable(lua, -3);
		}
	}

	lua_newtable(lua);

	generateIntervalHandling(Interval<Char>(std::numeric_limits<Char>::min(), std::numeric_limits<Char>::max()));

	// check if table is not empty
	lua_pushnil(lua);
	if (lua_next(L, -2) != 0)
	{
		// remove key-value pair
		lua_pop(lua, 2);
		// assign table to interval member
		lua_setfield(lua, -2, "interval");
	}
	else // remove empty table
		lua_pop(lua, 1);
	//lua_setfield(lua, -2, "interval");


	/*
	{
		[case] = {char, nextState, token} },
		[case] = {char, nextState, token} },
		...
		interval =
		{
			shift,
			[case] = {char, nextState, token},
			[case] = {char, shift, [case] = ... },
		},
	}
	*/
}

template<typename C>
void AlphabetGen<C>::generateIntervalHandling(const Interval<Char>& interval)
{
	//std::set<Char> boundaries;
	std::set<Char> innerBoundaries;
	for (typename IntervalMap::const_iterator iterIntervals = intervalResults.begin(); iterIntervals != intervalResults.end(); ++iterIntervals)
	{
		if (interval.contains(iterIntervals->first.first))
		{
			if (iterIntervals->first.first != interval.first)
				innerBoundaries.insert(iterIntervals->first.first);
			//boundaries.insert(iterIntervals->first.first);
		}
		Char end = iterIntervals->first.last;
		if (end < std::numeric_limits<Char>::max() && interval.contains(end))
		{
			if (end != interval.last)
				innerBoundaries.insert(++end);
			//boundaries.insert(end);
		}
		//if (end < std::numeric_limits<Char>::max() && interval.contains(++end))
		//{
		//	if (end - 1 != interval.last)
		//		innerBoundaries.insert(end);
		//	//boundaries.insert(end);
		//}
	}

	//const char * arg = "sym";

	if (innerBoundaries.empty())
		//stream << "return "  << defaultResult << ";" << newline;
	{
		setLuaResult(intervalResult(interval.first));
	}
	else if (innerBoundaries.size() == 1)
	{
		/*todo:
		Char bound = *innerBoundaries.begin();
		stream << "return ";
		if (bound > interval.first)
			 stream << arg << " < " << bound << " ? " << intervalResult(interval.first) << " : ";
		stream << intervalResult(bound) << ";" << newline;*/
		Char bound = *innerBoundaries.begin();
		pushLuaChar(bound);
		lua_setfield(lua, -2, "bound");

		lua_pushinteger(lua, 1);
		lua_newtable(lua);
		setLuaResult(intervalResult(interval.first));
		lua_settable(lua, -3);

		lua_pushinteger(lua, 2);
		lua_newtable(lua);
		setLuaResult(intervalResult(bound));
		lua_settable(lua, -3);
	}
	else
	{
		//LongInt first = *boundaries.begin();
		//LongInt last = *boundaries.rbegin();

		LongInt first = *innerBoundaries.begin();
		LongInt last = *innerBoundaries.rbegin();

		LongInt firstDelta = first - (LongInt)interval.first;
		LongInt lastDelta = (LongInt)interval.last - last;

		const Result& defaultResult = firstDelta > lastDelta ? intervalResult(interval.first) : intervalResult(interval.last);

		// TODO: offset not working yet
		LongInt offset = 0;//interval.first > first ? first : 0;
		std::set<Char> offsBounds(innerBoundaries);
		std::for_each(offsBounds.begin(), offsBounds.end(), std::binder2nd<std::minus<LongInt> >(std::minus<LongInt>(), offset));

		typename std::set<Char>::const_iterator iter = offsBounds.begin();
		Char prev = *iter;
		LongInt minDelta = std::numeric_limits<Char>::max();
		LongInt intervalSize = static_cast<LongInt>(interval.last) - static_cast<LongInt>(interval.first) + 1;
		if (minDelta > intervalSize) minDelta = intervalSize;
		LongInt maxCases = 8;

		LongInt minDiff = (last - first + maxCases - 1) / maxCases;
		if (minDiff == 0) minDiff = 1;

		LongInt deltaLimit = 1;
		//if (boundaries.size() > maxCases)
			for (; deltaLimit < minDiff; deltaLimit <<= 1)
				;

		for (++iter; iter != offsBounds.end(); ++iter)
		{
			Char val = *iter;
			LongInt delta = 1;
			//LongInt mask = 1;
			//for (LongInt diff = prev ^ val; (diff > 1 || 0 == (prev & mask) || 0 == (val & mask)) && (delta << 1) < minDelta; diff >>= 1, delta <<= 1, mask = (mask << 1) + 1)
			//	;

			LongInt mask = 1;
			for (Char diff = prev ^ val; ((diff >> 1) != 0) && (delta << 1) <= minDelta; diff = std::numeric_limits<Char>::max() & (diff >> 1), delta <<= 1, mask = (mask << 1) + 1)
				;

			if (delta < minDelta)
				if (delta < deltaLimit)
				{
					minDelta = deltaLimit;
				}
				else minDelta = delta;

			prev = val;
		}

		assert(minDelta < std::numeric_limits<Char>::max());

		Char delta = static_cast<Char>(minDelta);
		Char mask = ~(delta - 1);

		int shift;
		for (shift = 1; (delta >> shift) > 0; ++shift)
			;
		--shift;
/*
		stream << "switch(";
		//if (interval.first < 0) stream << "(";
		if (offset > 0) stream << "(";
		stream << arg;
		if (offset > 0)	stream << " - " << offset << ")";
		if (shift > 0) stream << " >> " << static_cast<int>(shift);
		//if (interval.first < 0)
		//	stream << ") & 0x" << std::hex << static_cast<int>(std::numeric_limits<Char>::max() >> shift) << std::dec;

		stream << ") // [" << interval.first << ", " << interval.last << "]" << newline;
		stream << "{" << newline;
*/

		lua_pushinteger(lua, shift);
		lua_setfield(lua, -2, "shift");

		LongInt offsFirst = first - offset;
		LongInt offsLast = last - offset;

		int index = 0;

		for (LongInt pattern = (offsFirst & mask); pattern <= (offsLast & mask); pattern += delta)
		{
			std::set<Char> currentBounds;
			for (typename std::set<Char>::const_iterator iter = offsBounds.lower_bound(static_cast<Char>(pattern)); iter != offsBounds.end() && ((*iter) & mask) == static_cast<Char>(pattern); ++iter)
				if (pattern != *iter)
					currentBounds.insert(*iter);

			if (!currentBounds.empty() || intervalResult(static_cast<Char>(pattern)) != defaultResult)
			{
				lua_pushinteger(lua, ++index);
				lua_newtable(lua);
				lua_pushinteger(lua, static_cast<int>(shift > 0 ? static_cast<Char>(pattern) >> shift : static_cast<Char>(pattern)));
				lua_setfield(lua, -2, "pattern");

				//stream << "case "  << static_cast<int>(shift > 0 ? static_cast<Char>(pattern) >> shift : static_cast<Char>(pattern)) << ": ";
				//stream << indent;
				generateIntervalHandling(Interval<Char>(static_cast<Char>(pattern + offset),
														static_cast<Char>(pattern + offset + delta - 1)));
				lua_settable(lua, -3);
				//stream << revindent;
			}
		}

		setLuaResult(defaultResult);

		//stream << "default: return " << defaultResult << ";" << newline;
		//stream << "}" << newline;
	}
}

/*
template<typename C>
AlphabetGen<C>::AlphabetGen(CodeStream& stream, const Alphabet<Char>& alphabet, const std::map<Char, std::string>& results, const std::string& errorResult)
: stream(stream), errorResult(errorResult)
{
	Alphabet<Char> singletons(alphabet.singletons().begin(), alphabet.singletons().end());
	singletons.merge();
	Alphabet<Char> intervals(alphabet);
	intervals.del(singletons);
	std::set<Interval<Char> > spaces(singletons.intervals());
	for (typename std::set<Char>::const_iterator iter = singletons.singletons().begin(); iter != singletons.singletons().end(); ++iter)
		spaces.insert(Interval<Char>(*iter));
	intervals.roundBoundaries(spaces);

	// do merge after round again
	std::map<std::string, std::set<Char> > resultMap;
	for (typename std::map<Char, std::string>::const_iterator iResult = results.begin(); iResult != results.end(); ++iResult)
		if (!singletons.contains(iResult->first))
			resultMap[iResult->second].insert(iResult->first);

	for (typename std::map<std::string, std::set<Char> >::const_iterator iResultMap = resultMap.begin(); iResultMap != resultMap.end(); ++iResultMap)
		intervals.merge(iResultMap->second);

	assert(intervals.singletons().empty());

	for(typename std::set<Char>::const_iterator iter = alphabet.singletons().begin(); iter != alphabet.singletons().end(); ++iter)
	{
		typename std::map<Char,std::string>::const_iterator iEntry = results.find(*iter);
		singletonResults[*iter] = iEntry != results.end() ? iEntry->second : errorResult;
	}

	for(typename std::map<Char, std::string>::const_iterator iter = results.begin(); iter != results.end(); ++iter)
		if (!singletons.contains(iter->first))
		{
			Interval<Char> interval(intervals.lower_bound(iter->first), intervals.upper_bound(iter->first));
			if (!interval.empty())
				intervalResults[interval] = iter->second;
			else
				assert(singletonResults.find(iter->first) != singletonResults.end());
		}

	const char * arg = "sym";

	if (!singletons.empty())
	{
		stream << "switch(" << arg << ")" << newline;
		stream << "{" << newline;
		for (typename std::set<Char>::const_iterator iter = alphabet.singletons().begin(); iter != alphabet.singletons().end(); ++iter)
			stream << "case " << *iter << ": return " << singletonResult(*iter) << ";" << newline;
		stream << "default: " << indent;
	}
	generateIntervalHandling(Interval<Char>(std::numeric_limits<Char>::min(), std::numeric_limits<Char>::max()));

	if (!singletons.empty())
		stream << revindent << "}" << newline;

}

template<typename C>
void AlphabetGen<C>::generateIntervalHandling(const Interval<Char>& interval)
{
	std::set<Char> boundaries;
	std::set<Char> innerBoundaries;
	for (typename IntervalMap::const_iterator iterIntervals = intervalResults.begin(); iterIntervals != intervalResults.end(); ++iterIntervals)
	{
		if (interval.contains(iterIntervals->first.first))
		{
			if (iterIntervals->first.first != interval.first)
				innerBoundaries.insert(iterIntervals->first.first);
			boundaries.insert(iterIntervals->first.first);
		}
		Char end = iterIntervals->first.last;
		if (end < std::numeric_limits<Char>::max() && interval.contains(++end))
		{
			if (end - 1 != interval.last)
				innerBoundaries.insert(end);
			boundaries.insert(end);
		}
	}

	std::string defaultResult = intervalResult(interval.first);



	const char * arg = "sym";

	if (innerBoundaries.empty())
		stream << "return "  << defaultResult << ";" << newline;
	else if (innerBoundaries.size() == 1)
	{
		Char bound = *innerBoundaries.begin();
		stream << "return ";
		if (bound > interval.first)
			 stream << arg << " < " << bound << " ? " << intervalResult(interval.first) << " : ";
		stream << intervalResult(bound) << ";" << newline;
	}
	else
	{
		//LongInt first = *boundaries.begin();
		//LongInt last = *boundaries.rbegin();

		LongInt first = *innerBoundaries.begin();
		LongInt last = *innerBoundaries.rbegin();

		// TODO: offset not working yet
		LongInt offset = 0;//interval.first > first ? first : 0;
		std::set<Char> offsBounds(innerBoundaries);
		std::for_each(offsBounds.begin(), offsBounds.end(), std::binder2nd<std::minus<LongInt> >(std::minus<LongInt>(), offset));

		typename std::set<Char>::const_iterator iter = offsBounds.begin();
		Char prev = *iter;
		LongInt minDelta = std::numeric_limits<Char>::max();
		LongInt intervalSize = static_cast<LongInt>(interval.last) - static_cast<LongInt>(interval.first) + 1;
		if (minDelta > intervalSize) minDelta = intervalSize;
		LongInt maxCases = 8;

		LongInt minDiff = (last - first + maxCases - 1) / maxCases;
		if (minDiff == 0) minDiff = 1;

		LongInt deltaLimit = 1;
		//if (boundaries.size() > maxCases)
			for (; deltaLimit < minDiff; deltaLimit <<= 1)
				;

		for (++iter; iter != offsBounds.end(); ++iter)
		{
			Char val = *iter;
			LongInt delta = 1;
			//LongInt mask = 1;
			//for (LongInt diff = prev ^ val; (diff > 1 || 0 == (prev & mask) || 0 == (val & mask)) && (delta << 1) < minDelta; diff >>= 1, delta <<= 1, mask = (mask << 1) + 1)
			//	;

			LongInt mask = 1;
			for (Char diff = prev ^ val; ((diff >> 1) != 0) && (delta << 1) <= minDelta; diff = std::numeric_limits<Char>::max() & (diff >> 1), delta <<= 1, mask = (mask << 1) + 1)
				;

			if (delta < minDelta)
				if (delta < deltaLimit)
				{
					minDelta = deltaLimit;
				}
				else minDelta = delta;

			prev = val;
		}

		assert(minDelta < std::numeric_limits<Char>::max());

		Char delta = static_cast<Char>(minDelta);
		Char mask = ~(delta - 1);

		int shift;
		for (shift = 1; (delta >> shift) > 0; ++shift)
			;
		--shift;

		stream << "switch(";
		//if (interval.first < 0) stream << "(";
		if (offset > 0) stream << "(";
		stream << arg;
		if (offset > 0)	stream << " - " << offset << ")";
		if (shift > 0) stream << " >> " << static_cast<int>(shift);
		//if (interval.first < 0)
		//	stream << ") & 0x" << std::hex << static_cast<int>(std::numeric_limits<Char>::max() >> shift) << std::dec;

		stream << ") // [" << interval.first << ", " << interval.last << "]" << newline;
		stream << "{" << newline;

		LongInt offsFirst = first - offset;
		LongInt offsLast = last - offset;

		for (LongInt pattern = (offsFirst & mask); pattern <= (offsLast & mask); pattern += delta)
		{
			std::set<Char> currentBounds;
			for (typename std::set<Char>::const_iterator iter = offsBounds.lower_bound(static_cast<Char>(pattern)); iter != offsBounds.end() && ((*iter) & mask) == static_cast<Char>(pattern); ++iter)
				if (pattern != *iter)
					currentBounds.insert(*iter);

			if (!currentBounds.empty() || intervalResult(static_cast<Char>(pattern)) != defaultResult)
			{
				stream << "case "  << static_cast<int>(shift > 0 ? static_cast<Char>(pattern) >> shift : static_cast<Char>(pattern)) << ": ";
				stream << indent;
				generateIntervalHandling(Interval<Char>(static_cast<Char>(pattern + offset),
														static_cast<Char>(pattern + offset + delta - 1)));
				stream << revindent;
			}
		}

		stream << "default: return " << defaultResult << ";" << newline;
		stream << "}" << newline;
	}
}*/

template<typename C>
const typename AlphabetGen<C>::Result& AlphabetGen<C>::singletonResult(Char symbol)
{
	typename SingletonMap::const_iterator iter = singletonResults.find(symbol);
	return iter != singletonResults.end() ? iter->second : errorResult;
}

template<typename C>
const typename AlphabetGen<C>::Result& AlphabetGen<C>::intervalResult(Char symbol)
{
	typename IntervalMap::const_iterator iter = intervalResults.find(Interval<Char>(symbol));
	return iter != intervalResults.end() && iter->first.first <= symbol ? iter->second : errorResult;
}

template<typename C>
void AlphabetGen<C>::setLuaResult(const Result& result)
{
	for (Result::const_iterator iResult = result.begin(); iResult != result.end(); ++iResult)
	{
		lua_pushlstring(lua, iResult->second.c_str(), iResult->second.length());
		lua_setfield(lua, -2, iResult->first.c_str());
	}
}

template<>
void AlphabetGen<char>::pushLuaChar(Char symbol)
{
	lua_pushlstring(lua, &symbol, 1);
}

/*template<typename C>
void AlphabetGen<C>::pushLuaChar(Char symbol)
{
	lua_pushnumber(lua, static_cast<lua_Number>(symbol));
}*/

