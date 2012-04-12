#include "../GrammarTokenizer.hpp"
#include <cstring>

void GrammarTokenizer::begin(unsigned icontext)
{
	const int contexts[][1] =
	{
		{ 0xFFFDFFFF, }, // 0
		{ 0x62103E, }, // 1
		{ 0x0,}
	};
	this->context = icontext < 2 ? contexts[icontext] : 2;
	this->state = 0;
	for (int c = 0; c < 1; ++c)
		this->tokens[c] = 0;
	this->length = 0;
}

int GrammarTokenizer::end(int& token)
{
	if (this->tokens)
	{
		token = 0;
		for (int ts = this->tokens; 0 == ts & 1; ++token, ts >>= 1)
			;
	}
	return this->length;
}

bool GrammarTokenizer::process(const char* symbols, int len)
{
	const State nNumOfStates = 11;
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

const GrammarTokenizer::State ERROR_STATE = -1;

static GrammarTokenizer::State handleState0(char sym, int* tokens)
{
	switch(sym)
	{
	case '\t': return (tokens[0] = 0x2, 1);
	case '\n': return (tokens[0] = 0x4, 2);
	case ' ': return (tokens[0] = 0x2, 1);
	case '"': return 3;
	case '(': return (tokens[0] = 0x4000000, 2);
	case ')': return (tokens[0] = 0x10000000, 2);
	case ',': return (tokens[0] = 0x2000000, 2);
	case '.': return (tokens[0] = 0x80000000, 2);
	case '/': return 4;
	case ':': return (tokens[0] = 0x400000, 5);
	case ';': return (tokens[0] = 0x1000, 2);
	case '=': return (tokens[0] = 0x100000, 2);
	case '@': return (tokens[0] = 0x40000, 2);
	case '[': return 7;
	case '_': return (tokens[0] = 0x20, 6);
	case '{': return (tokens[0] = 0x200, 2);
	case '}': return (tokens[0] = 0x800, 2);
	default: switch(sym >> 4)
		{
		case 0: switch(sym >> 2)
			{
			case 2: return sym < '\n' ? ERROR_STATE : (tokens[0] = 0x2, 1);
			case 3: return sym < '\xe' ? (tokens[0] = 0x2, 1) : ERROR_STATE;
			default: return ERROR_STATE;
			}
		case 4: return (tokens[0] = 0x20, 6);
		case 5: return sym < '\\' ? (tokens[0] = 0x20, 6) : ERROR_STATE;
		case 6: return sym < 'a' ? ERROR_STATE : (tokens[0] = 0x20, 6);
		case 7: return sym < '|' ? (tokens[0] = 0x20, 6) : ERROR_STATE;
		default: return ERROR_STATE;
		}
	}
}

static GrammarTokenizer::State handleState1(char sym, int* tokens)
{
	switch(sym)
	{
	case '\t': return (tokens[0] = 0x2, 1);
	case ' ': return (tokens[0] = 0x2, 1);
	default: switch(sym >> 2)
		{
		case 2: return sym < '\v' ? ERROR_STATE : (tokens[0] = 0x2, 1);
		case 3: return sym < '\xe' ? (tokens[0] = 0x2, 1) : ERROR_STATE;
		default: return ERROR_STATE;
		}
	}
}

static GrammarTokenizer::State handleState3(char sym, int* tokens)
{
	switch(sym)
	{
	case '\\': return 9;
	default: switch(sym)
		{
		case 34: return ERROR_STATE;
		default: return 8;
		}
	}
}

static GrammarTokenizer::State handleState4(char sym, int* tokens)
{
	switch(sym)
	{
	case '/': return (tokens[0] = 0x10, 2);
	case '\\': return 10;
	default: return 4;
	}
}

static GrammarTokenizer::State handleState5(char sym, int* tokens)
{
	switch(sym)
	{
	case '=': return (tokens[0] = 0x100, 2);
	case ']': return (tokens[0] = 0x4000, 2);
	default: return ERROR_STATE;
	}
}

static GrammarTokenizer::State handleState6(char sym, int* tokens)
{
	switch(sym)
	{
	case '_': return (tokens[0] = 0x20, 6);
	default: switch(sym >> 4)
		{
		case 3: return sym < ':' ? (tokens[0] = 0x20, 6) : ERROR_STATE;
		case 4: return sym < 'A' ? ERROR_STATE : (tokens[0] = 0x20, 6);
		case 5: return sym < '[' ? (tokens[0] = 0x20, 6) : ERROR_STATE;
		case 6: return sym < 'a' ? ERROR_STATE : (tokens[0] = 0x20, 6);
		case 7: return sym < '{' ? (tokens[0] = 0x20, 6) : ERROR_STATE;
		default: return ERROR_STATE;
		}
	}
}

static GrammarTokenizer::State handleState7(char sym, int* tokens)
{
	switch(sym)
	{
	case ':': return (tokens[0] = 0x2000, 2);
	default: return ERROR_STATE;
	}
}

static GrammarTokenizer::State handleState8(char sym, int* tokens)
{
	switch(sym)
	{
	case '"': return (tokens[0] = 0x8, 2);
	case '\\': return 9;
	default: return 8;
	}
}

static GrammarTokenizer::State handleState9(char sym, int* tokens)
{
	switch(sym)
	{
	case '"': return (tokens[0] = 0x8, 8);
	case '\\': return 9;
	default: return 8;
	}
}

static GrammarTokenizer::State handleState10(char sym, int* tokens)
{
	switch(sym)
	{
	case '/': return (tokens[0] = 0x10, 4);
	case '\\': return 10;
	default: return 4;
	}
}

static GrammarTokenizer::State handleDeadEnd(char, int*)
{
	return ERROR_STATE;
}

const GrammarTokenizer::PfnStateHandler GrammarTokenizer::stateHandler[11] =
{
	handleState0,
	handleState1,
	handleDeadEnd,
	handleState3,
	handleState4,
	handleState5,
	handleState6,
	handleState7,
	handleState8,
	handleState9,
	handleState10,
};
