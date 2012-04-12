#include "../RegexParser.hpp"


void RegexParser::parse()
{
	handleState0();
}

int RegexParser::shift(int context)
{
	return token = parserCtrl.shift(context);
}

int RegexParser::push(int)
{
	next = &RegexParser::recall;
	return symbol;
}

int RegexParser::recall(int)
{
	next = &RegexParser::shift;
	return token;
}

void RegexParser::reduce(int production, int sym)
{
	symbol = sym;
	next = &RegexParser::push;
	parserCtrl.reduce(production);
}

void RegexParser::ignore(int production)
{
	next = &RegexParser::recall;
	parserCtrl.reduce(production);
}

void RegexParser::reduce0()
{
	reduce(0, 3);
}
void RegexParser::reduce1()
{
	reduce(1, 3);
	lookahead();
}
void RegexParser::reduce2()
{
	reduce(2, 2);
	alternative();
}
void RegexParser::reduce3()
{
	reduce(3, 2);
}
void RegexParser::reduce4()
{
	reduce(4, 6);
	concat();
}
void RegexParser::reduce5()
{
	reduce(5, 6);
}
void RegexParser::reduce6()
{
	reduce(6, 7);
}
void RegexParser::reduce7()
{
	reduce(7, 7);
	asterisk();
}
void RegexParser::reduce8()
{
	reduce(8, 7);
	plus();
}
void RegexParser::reduce9()
{
	reduce(9, 7);
	optional();
}
void RegexParser::reduce10()
{
	reduce(10, 7);
	character();
}
void RegexParser::reduce11()
{
	reduce(11, 7);
	group();
}
void RegexParser::reduce12()
{
	reduce(12, 7);
	inverseGroup();
}
void RegexParser::reduce13()
{
	reduce(13, 15);
	singleton();
}
void RegexParser::reduce14()
{
	reduce(14, 15);
	interval();
}
void RegexParser::reduce15()
{
	reduce(15, 15);
	concatSingleton();
}
void RegexParser::reduce16()
{
	reduce(16, 15);
	concatInterval();
}
void RegexParser::reduce17()
{
	reduce(17, 13);
	pushChar(curr.C->str);
}
void RegexParser::reduce18()
{
	reduce(18, 13);
	pushChar(curr.A->str);
}
void RegexParser::reduce19()
{
	ignore(19);
}

int RegexParser::handleState0()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_C: 
			{
				TokC tok;
				tok.str = parserCtrl.string();
				TokC *prev = curr.C;
				curr.C = &tok;
				result = handleState1();
				curr.C = prev;
				break;
			}
		case TOK_E: result = handleState2(); break;
		case TOK_S: result = handleState3(); break;
		case TOK_D: result = handleState4(); break;
		case TOK_F: result = handleState5(); break;
		case TOK_8: result = handleState6(); break;
		case TOK_14: result = handleState8(); break;
		case TOK_I: result = handleState7(); break;
		case TOK_19: result = handleState9(); break;
		default:
			{
				const int expected[] = { TOK_C, TOK_E, TOK_S, TOK_D, TOK_F, TOK_8, TOK_I, TOK_14, TOK_19, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

int RegexParser::handleState1()
{
	(this->next)(0);
	reduce17();
	return 0;
}

int RegexParser::handleState2()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_4: result = handleState10(); break;
		case TOK_5: result = handleState11(); break;
		default: result = reduce0(); break;
		}
	}
	while(result == 0);
	return --result;
}

int RegexParser::handleState3()
{
	(this->next)(0);
	reduce19();
	return 1;
}

int RegexParser::handleState4()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_C: 
			{
				TokC tok;
				tok.str = parserCtrl.string();
				TokC *prev = curr.C;
				curr.C = &tok;
				result = handleState1();
				curr.C = prev;
				break;
			}
		case TOK_F: result = handleState12(); break;
		case TOK_8: result = handleState6(); break;
		case TOK_14: result = handleState8(); break;
		case TOK_19: result = handleState9(); break;
		case TOK_I: result = handleState7(); break;
		default: result = reduce3(); break;
		}
	}
	while(result == 0);
	return --result;
}

int RegexParser::handleState5()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_11: result = handleState14(); break;
		case TOK_10: result = handleState13(); break;
		case TOK_12: result = handleState15(); break;
		default: result = reduce5(); break;
		}
	}
	while(result == 0);
	return --result;
}

int RegexParser::handleState6()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_C: 
			{
				TokC tok;
				tok.str = parserCtrl.string();
				TokC *prev = curr.C;
				curr.C = &tok;
				result = handleState1();
				curr.C = prev;
				break;
			}
		case TOK_E: result = handleState16(); break;
		case TOK_D: result = handleState4(); break;
		case TOK_F: result = handleState5(); break;
		case TOK_8: result = handleState6(); break;
		case TOK_14: result = handleState8(); break;
		case TOK_I: result = handleState7(); break;
		case TOK_19: result = handleState9(); break;
		default:
			{
				const int expected[] = { TOK_C, TOK_E, TOK_D, TOK_F, TOK_8, TOK_I, TOK_14, TOK_19, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

int RegexParser::handleState7()
{
	(this->next)(0);
	reduce10();
	return 0;
}

int RegexParser::handleState8()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_C: 
			{
				TokC tok;
				tok.str = parserCtrl.string();
				TokC *prev = curr.C;
				curr.C = &tok;
				result = handleState1();
				curr.C = prev;
				break;
			}
		case TOK_19: result = handleState9(); break;
		case TOK_I: result = handleState17(); break;
		case TOK_G: result = handleState18(); break;
		case TOK_17: result = handleState19(); break;
		default:
			{
				const int expected[] = { TOK_C, TOK_I, TOK_G, TOK_17, TOK_19, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

int RegexParser::handleState9()
{
	int result = 0;
	switch((this->*next)(1))
	{
	case TOK_A: 
		{
			TokA tok;
			tok.str = parserCtrl.string();
			TokA *prev = curr.A;
			curr.A = &tok;
			result = handleState20();
			curr.A = prev;
			break;
		}
	default:
		{
			const int expected[] = { TOK_A, };
			result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
			break;
		}
	}
	return --result;
}

int RegexParser::handleState10()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_C: 
			{
				TokC tok;
				tok.str = parserCtrl.string();
				TokC *prev = curr.C;
				curr.C = &tok;
				result = handleState1();
				curr.C = prev;
				break;
			}
		case TOK_E: result = handleState21(); break;
		case TOK_D: result = handleState4(); break;
		case TOK_F: result = handleState5(); break;
		case TOK_8: result = handleState6(); break;
		case TOK_14: result = handleState8(); break;
		case TOK_I: result = handleState7(); break;
		case TOK_19: result = handleState9(); break;
		default:
			{
				const int expected[] = { TOK_C, TOK_E, TOK_D, TOK_F, TOK_8, TOK_I, TOK_14, TOK_19, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

int RegexParser::handleState11()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_C: 
			{
				TokC tok;
				tok.str = parserCtrl.string();
				TokC *prev = curr.C;
				curr.C = &tok;
				result = handleState1();
				curr.C = prev;
				break;
			}
		case TOK_D: result = handleState22(); break;
		case TOK_I: result = handleState7(); break;
		case TOK_8: result = handleState6(); break;
		case TOK_14: result = handleState8(); break;
		case TOK_19: result = handleState9(); break;
		case TOK_F: result = handleState5(); break;
		default:
			{
				const int expected[] = { TOK_C, TOK_D, TOK_F, TOK_8, TOK_I, TOK_14, TOK_19, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

int RegexParser::handleState12()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_11: result = handleState14(); break;
		case TOK_10: result = handleState13(); break;
		case TOK_12: result = handleState15(); break;
		default: result = reduce4(); break;
		}
	}
	while(result == 0);
	return --result;
}

int RegexParser::handleState13()
{
	(this->next)(0);
	reduce7();
	return 1;
}

int RegexParser::handleState14()
{
	(this->next)(0);
	reduce8();
	return 1;
}

int RegexParser::handleState15()
{
	(this->next)(0);
	reduce9();
	return 1;
}

int RegexParser::handleState16()
{
	int result = 0;
	switch((this->*next)(0))
	{
	case TOK_5: result = handleState11(); break;
	case TOK_9: result = handleState23(); break;
	default:
		{
			const int expected[] = { TOK_5, TOK_9, };
			result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
			break;
		}
	}
	return --result;
}

int RegexParser::handleState17()
{
	int result = 0;
	switch((this->*next)(0))
	{
	case TOK_4: result = handleState24(); break;
	default: result = reduce13(); break;
	}
	return --result;
}

int RegexParser::handleState18()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_C: 
			{
				TokC tok;
				tok.str = parserCtrl.string();
				TokC *prev = curr.C;
				curr.C = &tok;
				result = handleState1();
				curr.C = prev;
				break;
			}
		case TOK_16: result = handleState26(); break;
		case TOK_19: result = handleState9(); break;
		case TOK_I: result = handleState25(); break;
		default:
			{
				const int expected[] = { TOK_C, TOK_I, TOK_16, TOK_19, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

int RegexParser::handleState19()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_C: 
			{
				TokC tok;
				tok.str = parserCtrl.string();
				TokC *prev = curr.C;
				curr.C = &tok;
				result = handleState1();
				curr.C = prev;
				break;
			}
		case TOK_I: result = handleState17(); break;
		case TOK_G: result = handleState27(); break;
		case TOK_19: result = handleState9(); break;
		default:
			{
				const int expected[] = { TOK_C, TOK_I, TOK_G, TOK_19, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

int RegexParser::handleState20()
{
	(this->next)(0);
	reduce18();
	return 1;
}

int RegexParser::handleState21()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_5: result = handleState11(); break;
		default: result = reduce1(); break;
		}
	}
	while(result == 0);
	return --result;
}

int RegexParser::handleState22()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_C: 
			{
				TokC tok;
				tok.str = parserCtrl.string();
				TokC *prev = curr.C;
				curr.C = &tok;
				result = handleState1();
				curr.C = prev;
				break;
			}
		case TOK_F: result = handleState12(); break;
		case TOK_8: result = handleState6(); break;
		case TOK_14: result = handleState8(); break;
		case TOK_19: result = handleState9(); break;
		case TOK_I: result = handleState7(); break;
		default: result = reduce2(); break;
		}
	}
	while(result == 0);
	return --result;
}

int RegexParser::handleState23()
{
	(this->next)(0);
	reduce6();
	return 2;
}

int RegexParser::handleState24()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_C: 
			{
				TokC tok;
				tok.str = parserCtrl.string();
				TokC *prev = curr.C;
				curr.C = &tok;
				result = handleState1();
				curr.C = prev;
				break;
			}
		case TOK_I: result = handleState28(); break;
		case TOK_19: result = handleState9(); break;
		default:
			{
				const int expected[] = { TOK_C, TOK_I, TOK_19, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

int RegexParser::handleState25()
{
	int result = 0;
	switch((this->*next)(0))
	{
	case TOK_4: result = handleState29(); break;
	default: result = reduce15(); break;
	}
	return --result;
}

int RegexParser::handleState26()
{
	(this->next)(0);
	reduce11();
	return 2;
}

int RegexParser::handleState27()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_C: 
			{
				TokC tok;
				tok.str = parserCtrl.string();
				TokC *prev = curr.C;
				curr.C = &tok;
				result = handleState1();
				curr.C = prev;
				break;
			}
		case TOK_16: result = handleState30(); break;
		case TOK_19: result = handleState9(); break;
		case TOK_I: result = handleState25(); break;
		default:
			{
				const int expected[] = { TOK_C, TOK_I, TOK_16, TOK_19, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

int RegexParser::handleState28()
{
	(this->next)(0);
	reduce14();
	return 2;
}

int RegexParser::handleState29()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_C: 
			{
				TokC tok;
				tok.str = parserCtrl.string();
				TokC *prev = curr.C;
				curr.C = &tok;
				result = handleState1();
				curr.C = prev;
				break;
			}
		case TOK_I: result = handleState31(); break;
		case TOK_19: result = handleState9(); break;
		default:
			{
				const int expected[] = { TOK_C, TOK_I, TOK_19, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

int RegexParser::handleState30()
{
	(this->next)(0);
	reduce12();
	return 3;
}

int RegexParser::handleState31()
{
	(this->next)(0);
	reduce16();
	return 3;
}


