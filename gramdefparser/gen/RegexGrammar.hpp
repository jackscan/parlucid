#ifndef REGEXGRAMMAR_HPP
#define REGEXGRAMMAR_HPP

namespace RegexGrammar
{
struct Production
{
	int head;
	int len;
	const int* body;
};

const Production& getProduction(int prod);
}

#endif
