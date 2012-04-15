#include "../RegexParserCtrl.hpp"
#include "../../gen/RegexTokens.hpp"

#include <iostream>
#include <limits>
#include <cassert>

using namespace RegexTokens;

RegexParserCtrl::RegexParserCtrl(const std::string& regex)
: regexstr(regex)
, nextToken(TOK_END)
, start(0)
, end(0)
, parser(self())
{ }

int RegexParserCtrl::parse()
{
	parser.parse();
	return end;
}

const RegexParserCtrl::NfaChar& RegexParserCtrl::getNfa() const
{
	return nfas.top();
}

const RegexParserCtrl::NfaChar& RegexParserCtrl::getLookahead() const
{
	return lookaheadNfa;
}

int RegexParserCtrl::shift(int context)
{
	start = end;
	int len = static_cast<int>(regexstr.size()) - start;

	if (len > 0)
	{
		fsm.begin(context);
		fsm.process(&regexstr.c_str()[start], len);
		end = start + fsm.end(nextToken);
	}
	else nextToken = TOK_END;

	return nextToken;
}

int RegexParserCtrl::error(size_t n, const int *expected)
{
	std::cerr << "error in regex `" << regexstr << "' at ";

	if (start < end)
		std::cerr << "token `" << regexstr.substr(start, end - start) << "'" << std::endl;
	else std::cerr << "`" << regexstr.substr(start) << "'" << std::endl;

	std::cerr << "\texpected:";
	for (size_t i = 0; i < n; ++i)
		std::cerr << (i > 0 ? ", " : " ") << getTokenName(expected[i]);
	std::cerr << std::endl;
	return 1;
}

void RegexParserCtrl::reduce(int prod)
{
	//std::cout << "regex reduce " << prod << std::endl;
}

void RegexParserCtrl::lookahead()
{
	lookaheadNfa = popNfa();
}

void RegexParserCtrl::alternative()
{
	NfaChar nfa2 = popNfa();
	NfaChar nfa1 = popNfa();
	nfas.push(nfa1 | nfa2);
}

void RegexParserCtrl::concat()
{
	NfaChar nfa2 = popNfa();
	NfaChar nfa1 = popNfa();
	nfas.push(nfa1 + nfa2);
}

void RegexParserCtrl::asterisk()
{
	nfas.push(*popNfa());
}

void RegexParserCtrl::plus()
{
	NfaChar nfa = popNfa();
	nfas.push(*nfa + nfa);
}

void RegexParserCtrl::optional()
{
	nfas.push(popNfa() | NfaChar());
}

/*void RegexParserCtrl::inverse()
{
	nfas.push(~popNfa());
}*/

void RegexParserCtrl::character(const char* ch)
{
	nfas.push(NfaChar((Char)*ch));
}

void RegexParserCtrl::group()
{
	nfas.push(NfaChar(alphabet));
}

void RegexParserCtrl::inverseGroup()
{
	AlphabetChar inv(Interval<Char>(std::numeric_limits<Char>::min(), std::numeric_limits<Char>::max()));
	inv.del(alphabet);
	nfas.push(NfaChar(inv));
}

void RegexParserCtrl::singleton(const char* ch)
{
	alphabet = AlphabetChar((Char)*ch);
}

void RegexParserCtrl::interval(const char* first, const char* last)
{
	alphabet = AlphabetChar(Interval<Char>((Char)*first, (Char)*last));
}

void RegexParserCtrl::concatSingleton(const char* ch)
{
	alphabet.add((Char)*ch);
}

void RegexParserCtrl::concatInterval(const char* first, const char* last)
{
	alphabet.add((Char)*first, (Char)*last);
}

const char* RegexParserCtrl::escChar(const char* ch)
{
	const char *str = "\a\b\f\n\r\t\v";
	const char* result = ch;

	switch(*ch)
	{
	case 'a': result = str + 0; break;
	case 'b': result = str + 1; break;
	case 'f': result = str + 2; break;
	case 'n': result = str + 3; break;
	case 'r': result = str + 4; break;
	case 't': result = str + 5; break;
	case 'v': result = str + 6; break;
	case '0': result = str + 7; break;
	}
	return result;
}

RegexParserCtrl::NfaChar RegexParserCtrl::popNfa()
{
	if (!nfas.empty())
	{
		NfaChar result = nfas.top();
		nfas.pop();
		return result;
	}
	else
	{
		std::cerr << "nfa stack empty!" << std::endl;
		return NfaChar();
	}
}

const char* RegexParserCtrl::string()
{
	return &regexstr[start];
}

void RegexParserCtrl::release(const char* str)
{
}
