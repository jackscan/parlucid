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

#ifndef ALPHABETGEN_HPP
#define ALPHABETGEN_HPP

#include <set>
#include <string>
#include <map>

#include <lua.hpp>

template<typename T> class Alphabet;
template<typename T> struct Interval;
class CodeStream;

template<typename C>
class AlphabetGen
{
public:
	typedef C Char;
	typedef std::map<std::string, std::string> Result;
	typedef std::map<Char, Result> Results;
	AlphabetGen(lua_State *L, const Alphabet<Char>& alphabet, const Results& results);

private:
	typedef long long LongInt;
	typedef unsigned long long ULongInt;

	void generateIntervalHandling(const Interval<Char>& interval);

	const Result& singletonResult(Char);
	const Result& intervalResult(Char);

	void setLuaResult(const Result& result);
	void pushLuaChar(Char);

	typedef std::map<Char, Result> SingletonMap;
	typedef std::map<Interval<Char>, Result> IntervalMap;
	SingletonMap singletonResults;
	IntervalMap intervalResults;
	lua_State *lua;
	static const Result errorResult;
};

#endif
