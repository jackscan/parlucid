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
