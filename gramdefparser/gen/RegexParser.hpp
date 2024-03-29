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

/*
*/
#ifndef REGEXPARSER_HPP
#define REGEXPARSER_HPP

class RegexParserCtrl;

/*
0: S -> E
1: S -> E "-" E
2: E -> E "|" D
3: E -> D
4: D -> D F
5: D -> F
6: F -> "(" E ")"
7: F -> F "*"
8: F -> F "+"
9: F -> F "?"
10: F -> I
11: F -> "[" G "]"
12: F -> "[" "^" G "]"
13: G -> I
14: G -> J "-" I
15: G -> G I
16: G -> G J "-" I
17: J -> I
18: I -> C
19: I -> "\\" A
20: $ -> $ S
*/
class RegexParser
{
public:
	RegexParser(RegexParserCtrl& ctrl);

	void parse();

private:

	int shift(int);
	int push(int);
	int recall(int);

	RegexParserCtrl& parserCtrl;
	int (RegexParser::*next)(int);
	int token;
	int symbol;

	struct TokC
	{
		const char* str;
	};
	struct TokJ
	{
		struct {
			struct {
				const char* str;
			} C;
		} I;
	};
	struct TokI
	{
		struct {
			const char* str;
		} C;
	};
	struct TokA
	{
		const char* str;
	};
	struct
	{
		TokC *C;
		TokJ *J;
		TokI *I;
		TokA *A;
	} curr;

	union
	{
		TokC C;
		TokJ J;
		TokI I;
		TokA A;
	} parent;

	void reduce(int, int);
	void ignore(int);

	void reduce0();
	void reduce1();
	void reduce2();
	void reduce3();
	void reduce4();
	void reduce5();
	void reduce6();
	void reduce7();
	void reduce8();
	void reduce9();
	void reduce10();
	void reduce11();
	void reduce12();
	void reduce13();
	void reduce14();
	void reduce15();
	void reduce16();
	void reduce17();
	void reduce18();
	void reduce19();
	void reduce20();

	int handleState0();
	int handleState1();
	int handleState2();
	int handleState3();
	int handleState4();
	int handleState5();
	int handleState6();
	int handleState7();
	int handleState8();
	int handleState9();
	int handleState10();
	int handleState11();
	int handleState12();
	int handleState13();
	int handleState14();
	int handleState15();
	int handleState16();
	int handleState17();
	int handleState18();
	int handleState19();
	int handleState20();
	int handleState21();
	int handleState22();
	int handleState23();
	int handleState24();
	int handleState25();
	int handleState26();
	int handleState27();
	int handleState28();
	int handleState29();
	int handleState30();
	int handleState31();
	int handleState32();
	int handleState33();
};

#endif
