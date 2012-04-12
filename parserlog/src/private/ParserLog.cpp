#include "../ParserLog.hpp"

#include <cassert>

ParserLog::ParserLog()
: next(0)
{
}

ParserLog::~ParserLog()
{
	reset();
}

void ParserLog::shift(int token, int size, const char* str)
{
	if (next) stack.push_back(next);
	next = str ? new TerminalNode(token, size, str) : 0;
}

void ParserLog::reduce(int head, int bodysize)
{
	if (bodysize <= (int)stack.size())
	{
		NonTerminalNode* node = 0;
		if (bodysize > 0)
		{
			int reducesize = 0;
			int remain = bodysize;
			while (reducesize < (int)stack.size() && remain > 0)
				if (stack[(int)stack.size() - 1 - reducesize++]->getType() != 0)
					remain--;

			node = new NonTerminalNode(head, reducesize, &stack.back() - reducesize + 1);
			stack.resize(stack.size() - reducesize);
		}
		else node = head ? new NonTerminalNode(head) : 0;
		
		if (node) stack.push_back(node);
	}
}

void ParserLog::reset()
{
	for (size_t s = 0; s < stack.size(); ++s)
		delete stack[s];
	stack.clear();
}
