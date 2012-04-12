#ifndef LUATEMPLATEGRAMMAR_HPP
#define LUATEMPLATEGRAMMAR_HPP

namespace LuaTemplateGrammar
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
