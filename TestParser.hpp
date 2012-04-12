/*
*/
#ifndef TESTPARSER_HPP
#define TESTPARSER_HPP

class TestParserCtrl;

class TestParser
{
public:
	TestParser(TestParserCtrl& ctrl);

	void parse();

private:

	int shift(int);
	int push(int);
	int recall(int);

	TestParserCtrl& parserCtrl;
	int (TestParser::*next)(int);
	int token;
	int symbol;

	struct
	{
	} curr;

	union
	{
	} parent;

	void reduce(int, int);
	void ignore(int);

	void reduce0();
	void reduce1();
	void reduce2();

	int handleState0();
	int handleState1();
	int handleState2();
	int handleState3();
	int handleState4();
};

#endif
