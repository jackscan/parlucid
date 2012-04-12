#ifndef LUATEMPLATETOKENIZER_HPP
#define LUATEMPLATETOKENIZER_HPP

class LuaTemplateTokenizer
{
public:
	/*
	0 = 0x787B
	1 = 0x4078
	2 = 0x19383A770188
	3 = 0x1E303B96018A
	4 = 0x1C0000004
	5 = 0x100000200
	6 = 0x4000000400
	*/
	void begin(unsigned context);
	int end(int& tok);
	bool process(const char* symbols, int len);

	typedef int State;
	const static State ERROR_STATE;

private:	
	typedef State (*PfnStateHandler)(char, int*);

	//static const PfnStateHandler stateHandler[36];
	static const PfnStateHandler stateHandler[36];

	int context[2];
	int token;
	int length;
	int pos;
	State state;

};

#endif
