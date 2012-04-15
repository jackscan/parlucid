#ifndef REGEXPARSERCTRL_HPP
#define REGEXPARSERCTRL_HPP

#include "../gen/RegexTokenizer.hpp"
#include "../gen/RegexParser.hpp"
#include "../../lexer/src/nfa.hpp"
#include "../../lexer/src/alphabet.hpp"

#include <istream>
#include <string>
#include <stack>

class RegexParserCtrl
{
public:
	typedef unsigned char Char;
	typedef Nfa<Char> NfaChar;
	typedef Alphabet<Char> AlphabetChar;
	typedef int Token;

	RegexParserCtrl(const std::string& regex);

	int parse();

	int shift(int);
	int error(size_t, const int*);
	void reduce(int);

	void lookahead();
	void alternative();
	void concat();
	void asterisk();
	void plus();
	void optional();
	void character(const char*);
	void group();
	void inverseGroup();
	void singleton(const char*);
	void interval(const char*, const char*);
	void concatSingleton(const char*);
	void concatInterval(const char*, const char*);
	const char* escChar(const char*);

	const NfaChar& getNfa() const;
	const NfaChar& getLookahead() const;

	const char* string();
	void release(const char*);

private:

	NfaChar popNfa();

	RegexParserCtrl& self() { return *this; }

	NfaChar lookaheadNfa;

	std::stack<NfaChar> nfas;
	AlphabetChar alphabet;

	const std::string& regexstr;
	RegexTokenizer fsm;

	Token nextToken;
	int start, end;

	RegexParser parser;
};

#endif
