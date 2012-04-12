#include "../RegexTokenizer.hpp"
#include <cstring>

void RegexTokenizer::begin(unsigned icontext)
{
	const int contexts[][1] =
	{
		{ 0xBFFFF, }, // 0
		{ 0x40000, }, // 1
		{ 0x0,}
	};
	this->context = icontext < 2 ? contexts[icontext] : 2;
	this->state = 0;
	for (int c = 0; c < 1; ++c)
		this->tokens[c] = 0;
	this->length = 0;
}

int RegexTokenizer::end(int& token)
{
	if (this->tokens)
	{
		token = 0;
		for (int ts = this->tokens; 0 == ts & 1; ++token, ts >>= 1)
			;
	}
	return this->length;
}

bool RegexTokenizer::process(const char* symbols, int len)
{
	const State nNumOfStates = 2;
	int i = 0;
	int tok = 0;
	while(state >= 0 && i < len && state < nNumOfStates)
	{
		state = stateHandler[state](symbols[i++], tok);

		int anytok = 0;
		for (int c = 0; c < 1; ++c)	
			anytok |= (tok[c] &= context[c]); 

		if (anytok != 0)
		{
			for (int c = 0; c < 1; ++c)	
				tokens[c] = tok[c];
			length = i;
			tok = 0;
		}
	}

	return i < len && state >= 0;
}

const RegexTokenizer::State ERROR_STATE = -1;

static RegexTokenizer::State handleState0(char sym, int* tokens)
{
	switch(sym)
	{
	case '(': return (tokens[0] = 0x40100, 1);
	case ')': return (tokens[0] = 0x40200, 1);
	case '*': return (tokens[0] = 0x40400, 1);
	case '+': return (tokens[0] = 0x40800, 1);
	case ',': return (tokens[0] = 0x40002, 1);
	case '-': return (tokens[0] = 0x40010, 1);
	case '?': return (tokens[0] = 0x41000, 1);
	case '[': return (tokens[0] = 0x44000, 1);
	case '\\': return (tokens[0] = 0xC0000, 1);
	case ']': return (tokens[0] = 0x50000, 1);
	case '^': return (tokens[0] = 0x60000, 1);
	case '|': return (tokens[0] = 0x40020, 1);
	default: return (tokens[0] = 0x40002, 1);
	}
}

static RegexTokenizer::State handleDeadEnd(char, int*)
{
	return ERROR_STATE;
}

const RegexTokenizer::PfnStateHandler RegexTokenizer::stateHandler[2] =
{
	handleState0,
	handleDeadEnd,
};
