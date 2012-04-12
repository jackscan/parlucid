/*
*/
#ifndef REGEXPARSER_HPP
#define REGEXPARSER_HPP

class RegexParserCtrl;

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
