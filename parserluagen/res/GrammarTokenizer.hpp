#ifndef GRAMMARTOKENIZER_HPP
#define GRAMMARTOKENIZER_HPP

class GrammarTokenizer
{
public:
	/*
	0 = 0xFFFDFFFF
	1 = 0x62103E
	*/
	void begin(unsigned context);
	int end(int& tok);
	bool process(const char* symbols, int len);

private:
	typedef int State;
	const static State ERROR_STATE;

	typedef State (*PfnStateHandler)(char, int*);

	static const PfnStateHandler stateHandler[11];

	int context[1];
	int tokens[1];
	int length;
	State state;

};

#endif
