#ifndef GRAMMARGRAMMAR_HPP
#define GRAMMARGRAMMAR_HPP

namespace GrammarGrammar
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
