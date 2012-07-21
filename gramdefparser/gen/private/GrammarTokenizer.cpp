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

#include "../GrammarTokenizer.hpp"
#include "../GrammarTokens.hpp"
#include <cstring>

const GrammarTokenizer::State ERROR_STATE = -1;

struct Transition
{
	const char symbol;
	const GrammarTokenizer::State next;
	const int tokens[2];
};

static const Transition transitions0[35] =
{
	{ '\t', 1, { 0x2, } },
	{ '\n', 2, { 0x4, } },
	{ '\v', 1, { 0x2, } },
	{ '\r', 3, { 0x4, } },
	{ '\xe', -1, { 0 } },
	{ ' ', 1, { 0x2, } },
	{ '!', -1, { 0 } },
	{ '"', 4, { 0 } },
	{ '#', -1, { 0 } },
	{ '$', 5, { 0x80000000, } },
	{ '%', -1, { 0 } },
	{ '(', 5, { 0x20000000, } },
	{ ')', 5, { 0x40000000, } },
	{ '*', -1, { 0 } },
	{ ',', 5, { 0x00000000, 0x8, } },
	{ '-', -1, { 0 } },
	{ '.', 5, { 0x00000000, 0x80, } },
	{ '/', 6, { 0 } },
	{ '0', -1, { 0 } },
	{ ':', 7, { 0x00000000, 0x1, } },
	{ ';', 5, { 0x1000, } },
	{ '<', -1, { 0 } },
	{ '=', 5, { 0x200000, } },
	{ '>', -1, { 0 } },
	{ '@', 5, { 0x80000, } },
	{ 'A', 8, { 0x20, } },
	{ '[', 9, { 0x8000000, } },
	{ '\\', -1, { 0 } },
	{ ']', 5, { 0x10000000, } },
	{ '^', -1, { 0 } },
	{ 'a', 8, { 0x20, } },
	{ '{', 5, { 0x200, } },
	{ '|', 5, { 0x400000, } },
	{ '}', 5, { 0x800, } },
	{ '~', -1, { 0 } },
};

static const Transition transitions1[6] =
{
	{ '\t', 1, { 0x2, } },
	{ '\n', -1, { 0 } },
	{ '\v', 1, { 0x2, } },
	{ '\r', -1, { 0 } },
	{ ' ', 1, { 0x2, } },
	{ '!', -1, { 0 } },
};

static const Transition transitions2[2] =
{
	{ '\r', 5, { 0x4, } },
	{ '\xe', -1, { 0 } },
};

static const Transition transitions3[2] =
{
	{ '\n', 5, { 0x4, } },
	{ '\v', -1, { 0 } },
};

static const Transition transitions4[5] =
{
	{ '\x80', 10, { 0 } },
	{ '"', -1, { 0 } },
	{ '#', 10, { 0 } },
	{ '\\', 11, { 0 } },
	{ ']', 10, { 0 } },
};

static const Transition transitions6[5] =
{
	{ '\x80', 6, { 0 } },
	{ '/', 5, { 0x10, } },
	{ '0', 6, { 0 } },
	{ '\\', 12, { 0 } },
	{ ']', 6, { 0 } },
};

static const Transition transitions7[4] =
{
	{ '=', 5, { 0x100, } },
	{ '>', -1, { 0 } },
	{ ']', 5, { 0x4000, } },
	{ '^', -1, { 0 } },
};

static const Transition transitions8[8] =
{
	{ '0', 8, { 0x20, } },
	{ ':', -1, { 0 } },
	{ 'A', 8, { 0x20, } },
	{ '[', -1, { 0 } },
	{ '_', 8, { 0x20, } },
	{ '`', -1, { 0 } },
	{ 'a', 8, { 0x20, } },
	{ '{', -1, { 0 } },
};

static const Transition transitions9[2] =
{
	{ ':', 5, { 0x2000, } },
	{ ';', -1, { 0 } },
};

static const Transition transitions10[5] =
{
	{ '\x80', 10, { 0 } },
	{ '"', 5, { 0x8, } },
	{ '#', 10, { 0 } },
	{ '\\', 11, { 0 } },
	{ ']', 10, { 0 } },
};

static const Transition transitions11[3] =
{
	{ '\x80', 10, { 0 } },
	{ '\0', -1, { 0 } },
	{ '\x1', 10, { 0 } },
};

static const Transition transitions12[5] =
{
	{ '\x80', 6, { 0 } },
	{ '/', 6, { 0x10, } },
	{ '0', 6, { 0 } },
	{ '\\', 12, { 0 } },
	{ ']', 6, { 0 } },
};

static const Transition deadEnd = { '\x80', ERROR_STATE, { 0 } };


const struct StateTransitions
{
	const int size;
	const int output[2];
	const Transition* transitions;
}
transitionTable[13] =
{
	{ 35, { 0xF8687B3E, 0x89, }, transitions0 },
	{ 6, { 0x2, }, transitions1 },
	{ 2, { 0x4, }, transitions2 },
	{ 2, { 0x4, }, transitions3 },
	{ 5, { 0x8, }, transitions4 },
	{ 1, { 0 }, &deadEnd },
	{ 5, { 0x10, }, transitions6 },
	{ 4, { 0x4100, }, transitions7 },
	{ 8, { 0x20, }, transitions8 },
	{ 2, { 0x2000, }, transitions9 },
	{ 5, { 0x8, }, transitions10 },
	{ 3, { 0x8, }, transitions11 },
	{ 5, { 0x10, }, transitions12 },
};


static inline const Transition& getTransition(GrammarTokenizer::State state, char sym, const int context[2])
{
	const StateTransitions& entry = transitionTable[state];

	int anyoutput = 0;
	for (int c = 0; c < 2; ++c)
		anyoutput |= (entry.output[c] & context[c]);

	if (anyoutput)
	{
		int lower = 0;
		int upper = entry.size;
		while (lower < upper - 1)
		{
			int index = (upper + lower) / 2;
			if (sym < entry.transitions[index].symbol) upper = index;
			else lower = index;
		}
		return sym < entry.transitions[lower].symbol ? deadEnd : entry.transitions[lower];
	}
	else return deadEnd;
}

void GrammarTokenizer::begin(unsigned icontext)
{
	const int contexts[][2] =
	{
		{ 0xFDF9FFFF, 0xFF, }, // 0
		{ 0xFDCB1A3E, 0x1, }, // 1
	};

	if (icontext < 2)
	{
		this->context[0] = contexts[icontext][0];
		this->context[1] = contexts[icontext][1];
	}
	else
	{
		this->context[0] = 0x0;
		this->context[1] = 0x0;
	}
	this->state = 0;
	this->token = sizeof(int) * 8;
	this->length = 0;
	this->pos = 0;
}

int GrammarTokenizer::end(int& token)
{
	token = this->length > 0 ? this->token : GrammarTokens::TOK_END;
	return this->length;
}

bool GrammarTokenizer::process(const char* symbols, int len)
{
	int i = 0;
	int tok[2] = { 0 };
	int tokenIndex = 2 - 1;
	while(i < len)
	{
		const Transition& trans = getTransition(state, symbols[i++], context);
		state = trans.next;

		if (state < 0) break;

		int anytok = 0;
		for (int c = 0; c <= tokenIndex; ++c)
			anytok |= (tok[c] = (trans.tokens[c] & context[c]));

		if (anytok != 0)
		{
			const int setsize = sizeof(int) * 8;
			int newIndex = 0;
			for (; newIndex < tokenIndex && 0 == tok[newIndex]; ++newIndex)
				;
			if (newIndex != tokenIndex || tok[tokenIndex] != (1 << (token - tokenIndex * setsize)))
			{
				tokenIndex = newIndex;
				token = tokenIndex * setsize;
				for (int ts = tok[tokenIndex]; 0 == (ts & 1); ++token, ts >>= 1)
					;
				// update context to not accept any tokens with lower prio (i.e. greater tokens)
				context[tokenIndex] &= ~(-1 << (1 + token - tokenIndex * setsize));
				for (int c = tokenIndex + 1; c < 2; ++c) context[c] = 0x0;
			}
			length = pos + i;
		}
	}

	pos += len;

	return state >= 0;
}
