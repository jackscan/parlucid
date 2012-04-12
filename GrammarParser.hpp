/*
*/
#ifndef GRAMMARPARSER_HPP
#define GRAMMARPARSER_HPP

class GrammarParserCtrl;

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

	struct TokBodyexpr
	{
		struct {
			const char* str;
		} no;
	};
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
		const char* str;
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
	struct TokNo
	{
		const char* str;
	};
	struct TokString
	{
		const char* str;
	};
	struct
	{
		TokBodyexpr *bodyexpr;
		TokBodyterm *bodyterm;
		TokId *id;
		TokTerm *term;
		TokRegex *regex;
		TokBody *body;
		TokIdentifier *identifier;
		TokNo *no;
		TokString *string;
	} curr;

	union
	{
		TokBodyexpr bodyexpr;
		TokBodyterm bodyterm;
		TokId id;
		TokTerm term;
		TokRegex regex;
		TokBody body;
		TokIdentifier identifier;
		TokNo no;
		TokString string;
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
	int handleState81();
};

#endif
