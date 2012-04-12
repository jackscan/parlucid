#ifndef TESTTOKENIZER_HPP
#define TESTTOKENIZER_HPP

class TestTokenizer
{
public:
	/*
	0 = 0xF
	*/
	void begin(unsigned context);
	int end(int& tok);
	bool process(const char* symbols, int len);

	typedef int State;
	const static State ERROR_STATE;

private:	
	typedef State (*PfnStateHandler)(char, int*);

	//static const PfnStateHandler stateHandler[1];
	static const PfnStateHandler stateHandler[1];

	int context[1];
	int tokens[1];
	int length;
	int pos;
	State state;

};

#endif
