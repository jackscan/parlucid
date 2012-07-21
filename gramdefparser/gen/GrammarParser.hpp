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
#ifndef GRAMMARPARSER_HPP
#define GRAMMARPARSER_HPP

class GrammarParserCtrl;

/*
0: parser -> parser grammar
1: parser -> grammar
2: grammar -> identifier ":=" "{" definition "}" ";"
3: grammar -> "[:" identifier ":]" ":=" regex ";"
4: definition ->
5: definition -> definition production
6: production -> identifier ":=" body action ";"
7: production -> identifier ":=" "@" action ";"
8: production -> "@" ":=" body action ";"
9: production -> tokendef ";"
10: tokendef -> identifier "=" string
11: tokendef -> identifier "=" regex
12: body -> body "|" bodyexpr
13: body -> bodyexpr
14: bodyexpr -> bodyexpr bodyterm
15: bodyexpr -> bodyterm
16: bodyterm -> term
17: bodyterm -> "[" body "]"
18: bodyterm -> "{" body "}"
19: bodyterm -> "(" body ")"
20: term -> identifier
21: term -> string
22: term -> regex
23: term -> "$"
24: action ->
25: action -> ":" commandlist
26: commandlist -> command
27: commandlist -> commandlist "," command
28: command -> identifier "(" ")"
29: command -> identifier "(" list ")"
30: command -> lparameter "=" identifier "(" ")"
31: command -> lparameter "=" identifier "(" list ")"
32: command -> lparameter "=" parameter
33: lparameter -> parameter
34: list -> list "," parameter
35: list -> parameter
36: parameter -> parameter "." identifier
37: parameter -> identifier
38: $ -> nl
39: $ -> ws
40: $ -> $ parser
*/
class GrammarParser
{
public:
	GrammarParser(GrammarParserCtrl& ctrl);

	void parse();

private:

	int shift(int);
	int push(int);
	int recall(int);

	GrammarParserCtrl& parserCtrl;
	int (GrammarParser::*next)(int);
	int token;
	int symbol;

	struct TokBodyterm
	{
		struct {
			const char* str;
		} id;
	};
	struct TokId
	{
		const char* str;
	};
	struct TokTerm
	{
		struct {
			const char* str;
		} id;
	};
	struct TokRegex
	{
		const char* str;
	};
	struct TokBody
	{
		struct {
			const char* str;
		} no;
	};
	struct TokIdentifier
	{
		const char* str;
	};
	struct TokString
	{
		const char* str;
	};
	struct TokBodyexpr
	{
		struct {
			const char* str;
		} no;
	};
	struct TokNo
	{
		const char* str;
	};
	struct
	{
		TokBodyterm *bodyterm;
		TokId *id;
		TokTerm *term;
		TokRegex *regex;
		TokBody *body;
		TokIdentifier *identifier;
		TokString *string;
		TokBodyexpr *bodyexpr;
		TokNo *no;
	} curr;

	union
	{
		TokBodyterm bodyterm;
		TokId id;
		TokTerm term;
		TokRegex regex;
		TokBody body;
		TokIdentifier identifier;
		TokString string;
		TokBodyexpr bodyexpr;
		TokNo no;
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
	void reduce21();
	void reduce22();
	void reduce23();
	void reduce24();
	void reduce25();
	void reduce26();
	void reduce27();
	void reduce28();
	void reduce29();
	void reduce30();
	void reduce31();
	void reduce32();
	void reduce33();
	void reduce34();
	void reduce35();
	void reduce36();
	void reduce37();
	void reduce38();
	void reduce39();
	void reduce40();

	int handleState0();
	int handleState1(int context);
	int handleState2(int context);
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
	int handleState34();
	int handleState35();
	int handleState36();
	int handleState37();
	int handleState38();
	int handleState39();
	int handleState40();
	int handleState41();
	int handleState42();
	int handleState43();
	int handleState44();
	int handleState45();
	int handleState46();
	int handleState47();
	int handleState48();
	int handleState49();
	int handleState50();
	int handleState51();
	int handleState52();
	int handleState53();
	int handleState54();
	int handleState55();
	int handleState56();
	int handleState57();
	int handleState58();
	int handleState59();
	int handleState60();
	int handleState61();
	int handleState62();
	int handleState63();
	int handleState64();
	int handleState65();
	int handleState66();
	int handleState67();
	int handleState68();
	int handleState69();
	int handleState70();
	int handleState71();
	int handleState72();
	int handleState73();
	int handleState74();
	int handleState75();
	int handleState76();
	int handleState77();
	int handleState78();
	int handleState79();
	int handleState80();
};

#endif
