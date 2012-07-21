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
