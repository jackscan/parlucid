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

#include "../RegexParser.hpp"
#include "../RegexTokens.hpp"
#include "../../src/RegexParserCtrl.hpp"

using namespace RegexTokens;

RegexParser::RegexParser(RegexParserCtrl &ctrl) : parserCtrl(ctrl), next(&RegexParser::shift), token(0), symbol(0)
{
	curr.C = 0; // 1
	curr.J = 0; // 18
	curr.I = 0; // 13
	curr.A = 0; // 19
}

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

// S -> E
void RegexParser::reduce0()
{
	reduce(0, TOK_S);
}
// S -> E 4 E
void RegexParser::reduce1()
{
	reduce(1, TOK_S);
	parserCtrl.lookahead();
}
// E -> E 5 D
void RegexParser::reduce2()
{
	reduce(2, TOK_E);
	parserCtrl.alternative();
}
// E -> D
void RegexParser::reduce3()
{
	reduce(3, TOK_E);
}
// D -> D F
void RegexParser::reduce4()
{
	reduce(4, TOK_D);
	parserCtrl.concat();
}
// D -> F
void RegexParser::reduce5()
{
	reduce(5, TOK_D);
}
// F -> 8 E 9
void RegexParser::reduce6()
{
	reduce(6, TOK_F);
}
// F -> F 10
void RegexParser::reduce7()
{
	reduce(7, TOK_F);
	parserCtrl.asterisk();
}
// F -> F 11
void RegexParser::reduce8()
{
	reduce(8, TOK_F);
	parserCtrl.plus();
}
// F -> F 12
void RegexParser::reduce9()
{
	reduce(9, TOK_F);
	parserCtrl.optional();
}
// F -> I
void RegexParser::reduce10()
{
	reduce(10, TOK_F);
	parserCtrl.character(curr.I->C.str);
}
// F -> 14 G 16
void RegexParser::reduce11()
{
	reduce(11, TOK_F);
	parserCtrl.group();
}
// F -> 14 17 G 16
void RegexParser::reduce12()
{
	reduce(12, TOK_F);
	parserCtrl.inverseGroup();
}
// G -> I
void RegexParser::reduce13()
{
	reduce(13, TOK_G);
	parserCtrl.singleton(curr.I->C.str);
}
// G -> J 4 I
void RegexParser::reduce14()
{
	reduce(14, TOK_G);
	parserCtrl.interval(curr.J->I.C.str, curr.I->C.str);
}
// G -> G I
void RegexParser::reduce15()
{
	reduce(15, TOK_G);
	parserCtrl.concatSingleton(curr.I->C.str);
}
// G -> G J 4 I
void RegexParser::reduce16()
{
	reduce(16, TOK_G);
	parserCtrl.concatInterval(curr.J->I.C.str, curr.I->C.str);
}
// J -> I
void RegexParser::reduce17()
{
	reduce(17, TOK_J);
	parent.J.I.C.str = curr.I->C.str;
}
// I -> C
void RegexParser::reduce18()
{
	reduce(18, TOK_I);
	parent.I.C.str = curr.C->str;
}
// I -> 20 A
void RegexParser::reduce19()
{
	reduce(19, TOK_I);
	parent.I.C.str = parserCtrl.escChar(curr.A->str);
}
// END -> END S
void RegexParser::reduce20()
{
	ignore(20);
}

//[S ->.E], (END)
//[S ->.E 4 E], (END)
//[E ->.E 5 D], (END 4 5)
//[E ->.D], (END 4 5)
//[D ->.D F], (END C 4 5 F 8 I 14 20)
//[D ->.F], (END C 4 5 F 8 I 14 20)
//[F ->.8 E 9], (END C 4 5 F 8 10 11 12 I 14 20)
//[F ->.F 10], (END C 4 5 F 8 10 11 12 I 14 20)
//[F ->.F 11], (END C 4 5 F 8 10 11 12 I 14 20)
//[F ->.F 12], (END C 4 5 F 8 10 11 12 I 14 20)
//[F ->.I], (END C 4 5 F 8 10 11 12 I 14 20)
//[F ->.14 G 16], (END C 4 5 F 8 10 11 12 I 14 20)
//[F ->.14 17 G 16], (END C 4 5 F 8 10 11 12 I 14 20)
//[I ->.C], (END C 4 5 F 8 10 11 12 I 14 20)
//[I ->.20 A], (END C 4 5 F 8 10 11 12 I 14 20)
//[END -> END.S], (END)
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
				parserCtrl.release(curr.C->str);
				curr.C = prev;
				break;
			}
		case TOK_E: result = handleState2(); break;
		case TOK_S: result = handleState3(); break;
		case TOK_D: result = handleState4(); break;
		case TOK_F: result = handleState5(); break;
		case TOK_8: result = handleState6(); break;
		case TOK_14: result = handleState8(); break;
		case TOK_I: 
			{
				TokI tok = parent.I;
				TokI *prev = curr.I;
				curr.I = &tok;
				result = handleState7();
				curr.I = prev;
				break;
			}
		case TOK_20: result = handleState9(); break;
		default:
			{
				const int expected[] = { TOK_C, TOK_E, TOK_S, TOK_D, TOK_F, TOK_8, TOK_I, TOK_14, TOK_20, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[I -> C.], (END C 4 5 F 8 9 10 11 12 I 14 16 20)
int RegexParser::handleState1()
{
	(this->*next)(0);
	reduce18();
	return 0;
}

//[S -> E.], (END)
//[S -> E.4 E], (END)
//[E -> E.5 D], (END 4 5)
int RegexParser::handleState2()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_4: result = handleState10(); break;
		case TOK_5: result = handleState11(); break;
		default: result = 1; reduce0(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[END -> END S.], (END)
int RegexParser::handleState3()
{
	(this->*next)(0);
	reduce20();
	return 1;
}

//[E -> D.], (END 4 5 9)
//[D -> D.F], (END C 4 5 F 8 9 I 14 20)
//[F ->.8 E 9], (END C 4 5 F 8 9 10 11 12 I 14 20)
//[F ->.F 10], (END C 4 5 F 8 9 10 11 12 I 14 20)
//[F ->.F 11], (END C 4 5 F 8 9 10 11 12 I 14 20)
//[F ->.F 12], (END C 4 5 F 8 9 10 11 12 I 14 20)
//[F ->.I], (END C 4 5 F 8 9 10 11 12 I 14 20)
//[F ->.14 G 16], (END C 4 5 F 8 9 10 11 12 I 14 20)
//[F ->.14 17 G 16], (END C 4 5 F 8 9 10 11 12 I 14 20)
//[I ->.C], (END C 4 5 F 8 9 10 11 12 I 14 20)
//[I ->.20 A], (END C 4 5 F 8 9 10 11 12 I 14 20)
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
				parserCtrl.release(curr.C->str);
				curr.C = prev;
				break;
			}
		case TOK_20: result = handleState9(); break;
		case TOK_F: result = handleState12(); break;
		case TOK_8: result = handleState6(); break;
		case TOK_14: result = handleState8(); break;
		case TOK_I: 
			{
				TokI tok = parent.I;
				TokI *prev = curr.I;
				curr.I = &tok;
				result = handleState7();
				curr.I = prev;
				break;
			}
		default: result = 1; reduce3(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[D -> F.], (END C 4 5 F 8 9 I 14 20)
//[F -> F.10], (END C 4 5 F 8 9 10 11 12 I 14 20)
//[F -> F.11], (END C 4 5 F 8 9 10 11 12 I 14 20)
//[F -> F.12], (END C 4 5 F 8 9 10 11 12 I 14 20)
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
		default: result = 1; reduce5(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[E ->.E 5 D], (5 9)
//[E ->.D], (5 9)
//[D ->.D F], (C 5 F 8 9 I 14 20)
//[D ->.F], (C 5 F 8 9 I 14 20)
//[F ->.8 E 9], (C 5 F 8 9 10 11 12 I 14 20)
//[F -> 8.E 9], (END C 4 5 F 8 9 10 11 12 I 14 20)
//[F ->.F 10], (C 5 F 8 9 10 11 12 I 14 20)
//[F ->.F 11], (C 5 F 8 9 10 11 12 I 14 20)
//[F ->.F 12], (C 5 F 8 9 10 11 12 I 14 20)
//[F ->.I], (C 5 F 8 9 10 11 12 I 14 20)
//[F ->.14 G 16], (C 5 F 8 9 10 11 12 I 14 20)
//[F ->.14 17 G 16], (C 5 F 8 9 10 11 12 I 14 20)
//[I ->.C], (C 5 F 8 9 10 11 12 I 14 20)
//[I ->.20 A], (C 5 F 8 9 10 11 12 I 14 20)
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
				parserCtrl.release(curr.C->str);
				curr.C = prev;
				break;
			}
		case TOK_E: result = handleState16(); break;
		case TOK_D: result = handleState4(); break;
		case TOK_F: result = handleState5(); break;
		case TOK_8: result = handleState6(); break;
		case TOK_14: result = handleState8(); break;
		case TOK_I: 
			{
				TokI tok = parent.I;
				TokI *prev = curr.I;
				curr.I = &tok;
				result = handleState7();
				curr.I = prev;
				break;
			}
		case TOK_20: result = handleState9(); break;
		default:
			{
				const int expected[] = { TOK_C, TOK_E, TOK_D, TOK_F, TOK_8, TOK_I, TOK_14, TOK_20, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[F -> I.], (END C 4 5 F 8 9 10 11 12 I 14 20)
int RegexParser::handleState7()
{
	(this->*next)(0);
	reduce10();
	return 0;
}

//[F -> 14.G 16], (END C 4 5 F 8 9 10 11 12 I 14 20)
//[F -> 14.17 G 16], (END C 4 5 F 8 9 10 11 12 I 14 20)
//[G ->.I], (C I 16 20)
//[G ->.J 4 I], (C I 16 20)
//[G ->.G I], (C I 16 20)
//[G ->.G J 4 I], (C I 16 20)
//[J ->.I], (4)
//[I ->.C], (4 16)
//[I ->.20 A], (4 16)
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
				parserCtrl.release(curr.C->str);
				curr.C = prev;
				break;
			}
		case TOK_20: result = handleState9(); break;
		case TOK_17: result = handleState19(); break;
		case TOK_G: result = handleState18(); break;
		case TOK_J: 
			{
				TokJ tok = parent.J;
				TokJ *prev = curr.J;
				curr.J = &tok;
				result = handleState20();
				curr.J = prev;
				break;
			}
		case TOK_I: 
			{
				TokI tok = parent.I;
				TokI *prev = curr.I;
				curr.I = &tok;
				result = handleState17();
				curr.I = prev;
				break;
			}
		default:
			{
				const int expected[] = { TOK_C, TOK_I, TOK_G, TOK_17, TOK_J, TOK_20, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[I -> 20.A], (END C 4 5 F 8 9 10 11 12 I 14 16 20)
int RegexParser::handleState9()
{
	int result = 0;
	do
	{
		switch((this->*next)(1))
		{
		case TOK_A: 
			{
				TokA tok;
				tok.str = parserCtrl.string();
				TokA *prev = curr.A;
				curr.A = &tok;
				result = handleState21();
				parserCtrl.release(curr.A->str);
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
	}
	while(result == 0);
	return --result;
}

//[S -> E 4.E], (END)
//[E ->.E 5 D], (END 5)
//[E ->.D], (END 5)
//[D ->.D F], (END C 5 F 8 I 14 20)
//[D ->.F], (END C 5 F 8 I 14 20)
//[F ->.8 E 9], (END C 5 F 8 10 11 12 I 14 20)
//[F ->.F 10], (END C 5 F 8 10 11 12 I 14 20)
//[F ->.F 11], (END C 5 F 8 10 11 12 I 14 20)
//[F ->.F 12], (END C 5 F 8 10 11 12 I 14 20)
//[F ->.I], (END C 5 F 8 10 11 12 I 14 20)
//[F ->.14 G 16], (END C 5 F 8 10 11 12 I 14 20)
//[F ->.14 17 G 16], (END C 5 F 8 10 11 12 I 14 20)
//[I ->.C], (END C 5 F 8 10 11 12 I 14 20)
//[I ->.20 A], (END C 5 F 8 10 11 12 I 14 20)
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
				parserCtrl.release(curr.C->str);
				curr.C = prev;
				break;
			}
		case TOK_E: result = handleState22(); break;
		case TOK_D: result = handleState4(); break;
		case TOK_F: result = handleState5(); break;
		case TOK_8: result = handleState6(); break;
		case TOK_14: result = handleState8(); break;
		case TOK_I: 
			{
				TokI tok = parent.I;
				TokI *prev = curr.I;
				curr.I = &tok;
				result = handleState7();
				curr.I = prev;
				break;
			}
		case TOK_20: result = handleState9(); break;
		default:
			{
				const int expected[] = { TOK_C, TOK_E, TOK_D, TOK_F, TOK_8, TOK_I, TOK_14, TOK_20, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[E -> E 5.D], (END 4 5 9)
//[D ->.D F], (END C 4 5 F 8 9 I 14 20)
//[D ->.F], (END C 4 5 F 8 9 I 14 20)
//[F ->.8 E 9], (END C 4 5 F 8 9 10 11 12 I 14 20)
//[F ->.F 10], (END C 4 5 F 8 9 10 11 12 I 14 20)
//[F ->.F 11], (END C 4 5 F 8 9 10 11 12 I 14 20)
//[F ->.F 12], (END C 4 5 F 8 9 10 11 12 I 14 20)
//[F ->.I], (END C 4 5 F 8 9 10 11 12 I 14 20)
//[F ->.14 G 16], (END C 4 5 F 8 9 10 11 12 I 14 20)
//[F ->.14 17 G 16], (END C 4 5 F 8 9 10 11 12 I 14 20)
//[I ->.C], (END C 4 5 F 8 9 10 11 12 I 14 20)
//[I ->.20 A], (END C 4 5 F 8 9 10 11 12 I 14 20)
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
				parserCtrl.release(curr.C->str);
				curr.C = prev;
				break;
			}
		case TOK_D: result = handleState23(); break;
		case TOK_I: 
			{
				TokI tok = parent.I;
				TokI *prev = curr.I;
				curr.I = &tok;
				result = handleState7();
				curr.I = prev;
				break;
			}
		case TOK_8: result = handleState6(); break;
		case TOK_14: result = handleState8(); break;
		case TOK_20: result = handleState9(); break;
		case TOK_F: result = handleState5(); break;
		default:
			{
				const int expected[] = { TOK_C, TOK_D, TOK_F, TOK_8, TOK_I, TOK_14, TOK_20, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[D -> D F.], (END C 4 5 F 8 9 I 14 20)
//[F -> F.10], (END C 4 5 F 8 9 10 11 12 I 14 20)
//[F -> F.11], (END C 4 5 F 8 9 10 11 12 I 14 20)
//[F -> F.12], (END C 4 5 F 8 9 10 11 12 I 14 20)
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
		default: result = 2; reduce4(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[F -> F 10.], (END C 4 5 F 8 9 10 11 12 I 14 20)
int RegexParser::handleState13()
{
	(this->*next)(0);
	reduce7();
	return 1;
}

//[F -> F 11.], (END C 4 5 F 8 9 10 11 12 I 14 20)
int RegexParser::handleState14()
{
	(this->*next)(0);
	reduce8();
	return 1;
}

//[F -> F 12.], (END C 4 5 F 8 9 10 11 12 I 14 20)
int RegexParser::handleState15()
{
	(this->*next)(0);
	reduce9();
	return 1;
}

//[E -> E.5 D], (5 9)
//[F -> 8 E.9], (END C 4 5 F 8 9 10 11 12 I 14 20)
int RegexParser::handleState16()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_5: result = handleState11(); break;
		case TOK_9: result = handleState24(); break;
		default:
			{
				const int expected[] = { TOK_5, TOK_9, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[G -> I.], (C I 16 20)
//[J -> I.], (4)
int RegexParser::handleState17()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		default: result = 1; reduce13(); break;
		case TOK_4: result = 1; reduce17(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[F -> 14 G.16], (END C 4 5 F 8 9 10 11 12 I 14 20)
//[G -> G.I], (C I 16 20)
//[G -> G.J 4 I], (C I 16 20)
//[J ->.I], (4)
//[I ->.C], (C 4 I 16 20)
//[I ->.20 A], (C 4 I 16 20)
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
				parserCtrl.release(curr.C->str);
				curr.C = prev;
				break;
			}
		case TOK_J: 
			{
				TokJ tok = parent.J;
				TokJ *prev = curr.J;
				curr.J = &tok;
				result = handleState27();
				curr.J = prev;
				break;
			}
		case TOK_16: result = handleState26(); break;
		case TOK_20: result = handleState9(); break;
		case TOK_I: 
			{
				TokI tok = parent.I;
				TokI *prev = curr.I;
				curr.I = &tok;
				result = handleState25();
				curr.I = prev;
				break;
			}
		default:
			{
				const int expected[] = { TOK_C, TOK_I, TOK_16, TOK_J, TOK_20, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[F -> 14 17.G 16], (END C 4 5 F 8 9 10 11 12 I 14 20)
//[G ->.I], (C I 16 20)
//[G ->.J 4 I], (C I 16 20)
//[G ->.G I], (C I 16 20)
//[G ->.G J 4 I], (C I 16 20)
//[J ->.I], (4)
//[I ->.C], (4 16)
//[I ->.20 A], (4 16)
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
				parserCtrl.release(curr.C->str);
				curr.C = prev;
				break;
			}
		case TOK_J: 
			{
				TokJ tok = parent.J;
				TokJ *prev = curr.J;
				curr.J = &tok;
				result = handleState20();
				curr.J = prev;
				break;
			}
		case TOK_I: 
			{
				TokI tok = parent.I;
				TokI *prev = curr.I;
				curr.I = &tok;
				result = handleState17();
				curr.I = prev;
				break;
			}
		case TOK_G: result = handleState28(); break;
		case TOK_20: result = handleState9(); break;
		default:
			{
				const int expected[] = { TOK_C, TOK_I, TOK_G, TOK_J, TOK_20, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[G -> J.4 I], (C I 16 20)
int RegexParser::handleState20()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_4: result = handleState29(); break;
		default:
			{
				const int expected[] = { TOK_4, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[I -> 20 A.], (END C 4 5 F 8 9 10 11 12 I 14 16 20)
int RegexParser::handleState21()
{
	(this->*next)(0);
	reduce19();
	return 1;
}

//[S -> E 4 E.], (END)
//[E -> E.5 D], (END 5)
int RegexParser::handleState22()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_5: result = handleState11(); break;
		default: result = 3; reduce1(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[E -> E 5 D.], (END 4 5 9)
//[D -> D.F], (END C 4 5 F 8 9 I 14 20)
//[F ->.8 E 9], (END C 4 5 F 8 9 10 11 12 I 14 20)
//[F ->.F 10], (END C 4 5 F 8 9 10 11 12 I 14 20)
//[F ->.F 11], (END C 4 5 F 8 9 10 11 12 I 14 20)
//[F ->.F 12], (END C 4 5 F 8 9 10 11 12 I 14 20)
//[F ->.I], (END C 4 5 F 8 9 10 11 12 I 14 20)
//[F ->.14 G 16], (END C 4 5 F 8 9 10 11 12 I 14 20)
//[F ->.14 17 G 16], (END C 4 5 F 8 9 10 11 12 I 14 20)
//[I ->.C], (END C 4 5 F 8 9 10 11 12 I 14 20)
//[I ->.20 A], (END C 4 5 F 8 9 10 11 12 I 14 20)
int RegexParser::handleState23()
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
				parserCtrl.release(curr.C->str);
				curr.C = prev;
				break;
			}
		case TOK_20: result = handleState9(); break;
		case TOK_F: result = handleState12(); break;
		case TOK_8: result = handleState6(); break;
		case TOK_14: result = handleState8(); break;
		case TOK_I: 
			{
				TokI tok = parent.I;
				TokI *prev = curr.I;
				curr.I = &tok;
				result = handleState7();
				curr.I = prev;
				break;
			}
		default: result = 3; reduce2(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[F -> 8 E 9.], (END C 4 5 F 8 9 10 11 12 I 14 20)
int RegexParser::handleState24()
{
	(this->*next)(0);
	reduce6();
	return 2;
}

//[G -> G I.], (C I 16 20)
//[J -> I.], (4)
int RegexParser::handleState25()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		default: result = 2; reduce15(); break;
		case TOK_4: result = 1; reduce17(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[F -> 14 G 16.], (END C 4 5 F 8 9 10 11 12 I 14 20)
int RegexParser::handleState26()
{
	(this->*next)(0);
	reduce11();
	return 2;
}

//[G -> G J.4 I], (C I 16 20)
int RegexParser::handleState27()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_4: result = handleState30(); break;
		default:
			{
				const int expected[] = { TOK_4, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[F -> 14 17 G.16], (END C 4 5 F 8 9 10 11 12 I 14 20)
//[G -> G.I], (C I 16 20)
//[G -> G.J 4 I], (C I 16 20)
//[J ->.I], (4)
//[I ->.C], (C 4 I 16 20)
//[I ->.20 A], (C 4 I 16 20)
int RegexParser::handleState28()
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
				parserCtrl.release(curr.C->str);
				curr.C = prev;
				break;
			}
		case TOK_J: 
			{
				TokJ tok = parent.J;
				TokJ *prev = curr.J;
				curr.J = &tok;
				result = handleState27();
				curr.J = prev;
				break;
			}
		case TOK_16: result = handleState31(); break;
		case TOK_20: result = handleState9(); break;
		case TOK_I: 
			{
				TokI tok = parent.I;
				TokI *prev = curr.I;
				curr.I = &tok;
				result = handleState25();
				curr.I = prev;
				break;
			}
		default:
			{
				const int expected[] = { TOK_C, TOK_I, TOK_16, TOK_J, TOK_20, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[G -> J 4.I], (C I 16 20)
//[I ->.C], (C I 16 20)
//[I ->.20 A], (C I 16 20)
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
				parserCtrl.release(curr.C->str);
				curr.C = prev;
				break;
			}
		case TOK_I: 
			{
				TokI tok = parent.I;
				TokI *prev = curr.I;
				curr.I = &tok;
				result = handleState32();
				curr.I = prev;
				break;
			}
		case TOK_20: result = handleState9(); break;
		default:
			{
				const int expected[] = { TOK_C, TOK_I, TOK_20, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[G -> G J 4.I], (C I 16 20)
//[I ->.C], (C I 16 20)
//[I ->.20 A], (C I 16 20)
int RegexParser::handleState30()
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
				parserCtrl.release(curr.C->str);
				curr.C = prev;
				break;
			}
		case TOK_I: 
			{
				TokI tok = parent.I;
				TokI *prev = curr.I;
				curr.I = &tok;
				result = handleState33();
				curr.I = prev;
				break;
			}
		case TOK_20: result = handleState9(); break;
		default:
			{
				const int expected[] = { TOK_C, TOK_I, TOK_20, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[F -> 14 17 G 16.], (END C 4 5 F 8 9 10 11 12 I 14 20)
int RegexParser::handleState31()
{
	(this->*next)(0);
	reduce12();
	return 3;
}

//[G -> J 4 I.], (C I 16 20)
int RegexParser::handleState32()
{
	(this->*next)(0);
	reduce14();
	return 2;
}

//[G -> G J 4 I.], (C I 16 20)
int RegexParser::handleState33()
{
	(this->*next)(0);
	reduce16();
	return 3;
}


