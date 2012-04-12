#ifndef CMDLINETOKENIZER_HPP
#define CMDLINETOKENIZER_HPP

class CmdLineTokenizer
{
public:
	typedef int State;
	const static State ERROR_STATE;

	// context 0: [ TOK_END TOK_iarg TOK_oarg TOK_name TOK_path TOK_value TOK_cmds TOK_file TOK_loutput TOK_grammar TOK_assign TOK_11 TOK_12 ]
	// context 1: [ TOK_name TOK_path TOK_file TOK_12 ]
	// context 2: [ TOK_path TOK_value TOK_12 ]
	static int process(State& state, const char* symbols, int, int& tok);
private:
	typedef State (*PfnStateHandler)(char, int&);
	static const PfnStateHandler stateHandler[12];
};

#endif
