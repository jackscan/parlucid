/*
*/
#ifndef LUATEMPLATEPARSER_HPP
#define LUATEMPLATEPARSER_HPP

class LuaTemplateParserCtrl;

/*
0: template -> template codeline
1: template -> codeline
2: term -> term textintro text "%}"
3: term -> term code
4: term ->
5: embedtextbegin -> chunks nl
6: embedtext -> chunks
7: embedtext -> embedtextbegin textlines lastline
8: embedif -> "@{" snippet "}?"
9: embedelse -> "@:"
10: includefile -> filename
11: codeindent -> spaces
12: codeindent ->
13: text -> textbegin textlines lastline
14: text -> chunks
15: textbegin -> chunks nl
16: textlines -> textlines textline
17: textlines ->
18: textline -> textindent nl
19: textline -> textindent chunks realchunk nl
20: textline -> textindent chunks weakchunk nl
21: lastline -> textindent chunks
22: chunks ->
23: realchunk -> quote
24: realchunk -> chunk
25: weakchunk -> condembed
26: textindent -> spaces
27: textindent ->
28: codeline -> codeindent term nl
29: codeline -> codeindent term eof
30: weakchunk -> "$" name
31: condembed -> embedif embedtext embedelse embedtext "@;"
32: condembed -> embedif embedtext "@;"
33: weakchunk -> "$" "{" snippet "}"
34: weakchunk -> "@{" snippet "}"
35: weakchunk -> "@\"" includefile quote
36: chunks -> chunks realchunk
37: chunks -> chunks weakchunk
38: $ -> $ template
*/
class LuaTemplateParser
{
public:
	LuaTemplateParser(LuaTemplateParserCtrl& ctrl);

	void parse();

private:

	int shift(int);
	int push(int);
	int recall(int);

	LuaTemplateParserCtrl& parserCtrl;
	int (LuaTemplateParser::*next)(int);
	int token;
	int symbol;

	struct TokSpaces
	{
		const char* str;
	};
	struct TokName
	{
		const char* str;
	};
	struct TokChunk
	{
		const char* str;
	};
	struct TokCode
	{
		const char* str;
	};
	struct TokSnippet
	{
		const char* str;
	};
	struct TokFilename
	{
		const char* str;
	};
	struct
	{
		TokSpaces *spaces;
		TokName *name;
		TokChunk *chunk;
		TokCode *code;
		TokSnippet *snippet;
		TokFilename *filename;
	} curr;

	union
	{
		TokSpaces spaces;
		TokName name;
		TokChunk chunk;
		TokCode code;
		TokSnippet snippet;
		TokFilename filename;
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
};

#endif
