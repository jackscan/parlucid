#ifndef GRAMMARTOKENIZER_HPP
#define GRAMMARTOKENIZER_HPP

class GrammarTokenizer
{
public:
	/*
	0 = 0xFFFDF9FFFF
	1 = 0x1FDCB1A3E
	*/
	void begin(unsigned context);
	int end(int& tok);
	bool process(const char* symbols, int len);

	typedef int State;
	const static State ERROR_STATE;

private:	
	typedef State (*PfnStateHandler)(char, int*);

	//static const PfnStateHandler stateHandler[13];
	static const PfnStateHandler stateHandler[13];

	int context[2];
	int token;
	int length;
	int pos;
	State state;

};

#endif
