#ifndef GRAMMARTOKENIZER_HPP
#define GRAMMARTOKENIZER_HPP

class GrammarTokenizer
{
public:
	/*
	0 = 0x16FDF733FFFF
	1 = 0xDF716643E
	*/
	void begin(unsigned context);
	int end(int& tok);
	bool process(const char* symbols, int len);

	typedef int State;
	const static State ERROR_STATE;

private:	
	typedef State (*PfnStateHandler)(char, int*);

	//static const PfnStateHandler stateHandler[11];
	static const PfnStateHandler stateHandler[11];

	int context[2];
	int tokens[2];
	int length;
	int pos;
	State state;

};

#endif
