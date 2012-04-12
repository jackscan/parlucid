#include "../RegexTokenizer.hpp"
#include "../RegexTokens.hpp"
#include <cstring>

const RegexTokenizer::State ERROR_STATE = -1;

struct Transition
{
	const char symbol;
	const RegexTokenizer::State next;
	const int tokens[1];
};

static const Transition transitions0[17] =
{
	{ '\x80', 1, { 0x80002, } },
	{ '(', 1, { 0x80100, } },
	{ ')', 1, { 0x80200, } },
	{ '*', 1, { 0x80400, } },
	{ '+', 1, { 0x80800, } },
	{ ',', 1, { 0x80002, } },
	{ '-', 1, { 0x80010, } },
	{ '.', 1, { 0x80002, } },
	{ '?', 1, { 0x81000, } },
	{ '@', 1, { 0x80002, } },
	{ '[', 1, { 0x84000, } },
	{ '\\', 1, { 0x180000, } },
	{ ']', 1, { 0x90000, } },
	{ '^', 1, { 0xA0000, } },
	{ '_', 1, { 0x80002, } },
	{ '|', 1, { 0x80020, } },
	{ '}', 1, { 0x80002, } },
};

static const Transition deadEnd = { '\x80', ERROR_STATE, { 0 } };


const struct StateTransitions
{
	const int size;
	const int output[1];
	const Transition* transitions;
}
transitionTable[2] =
{
	{ 17, { 0x1B5F32, }, transitions0 },
	{ 1, { 0 }, &deadEnd },
};


static inline const Transition& getTransition(RegexTokenizer::State state, char sym, const int context[1])
{
	const StateTransitions& entry = transitionTable[state];

	int anyoutput = 0;
	for (int c = 0; c < 1; ++c)
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

void RegexTokenizer::begin(unsigned icontext)
{
	const int contexts[][1] =
	{
		{ 0x17FFFF, }, // 0
		{ 0x80000, }, // 1
	};

	if (icontext < 2)
	{
		this->context[0] = contexts[icontext][0];
	}
	else
	{
		this->context[0] = 0x0;
	}
	this->state = 0;
	this->token = sizeof(int) * 8;
	this->length = 0;
	this->pos = 0;
}

int RegexTokenizer::end(int& token)
{
	token = this->length > 0 ? this->token : RegexTokens::TOK_END;
	return this->length;
}

bool RegexTokenizer::process(const char* symbols, int len)
{
	int i = 0;
	int tok[1] = { 0 };
	int tokenIndex = 1 - 1;
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
				for (int c = tokenIndex + 1; c < 1; ++c) context[c] = 0x0;
			}
			length = pos + i;
		}
	}

	pos += len;

	return state >= 0;
}
