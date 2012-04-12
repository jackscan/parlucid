#ifndef REGEXTOKENIZER_HPP
#define REGEXTOKENIZER_HPP

class RegexTokenizer
{
public:
	/*
	0 = 0x17FFFF
	1 = 0x80000
	*/
	void begin(unsigned context);
	int end(int& tok);
	bool process(const char* symbols, int len);

	typedef int State;
	const static State ERROR_STATE;

private:	
	typedef State (*PfnStateHandler)(char, int*);

	//static const PfnStateHandler stateHandler[2];
	static const PfnStateHandler stateHandler[2];

	int context[1];
	int tokens[1];
	int length;
	int pos;
	State state;

};

#endif
