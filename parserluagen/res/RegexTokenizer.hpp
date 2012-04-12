#ifndef REGEXTOKENIZER_HPP
#define REGEXTOKENIZER_HPP

class RegexTokenizer
{
public:
	/*
	0 = 0xBFFFF
	1 = 0x40000
	*/
	void begin(unsigned context);
	int end(int& tok);
	bool process(const char* symbols, int len);

private:
	typedef int State;
	const static State ERROR_STATE;

	typedef State (*PfnStateHandler)(char, int*);

	static const PfnStateHandler stateHandler[2];

	int context[1];
	int tokens[1];
	int length;
	State state;

};

#endif
