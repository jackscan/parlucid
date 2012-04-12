#ifndef CMDLINETOKENS_HPP
#define CMDLINETOKENS_HPP

namespace CmdLineTokens
{
	enum Token
	{
		TOK_END = 0, // END
		TOK_iarg = 1, // iarg
		TOK_oarg = 2, // oarg
		TOK_name = 3, // name
		TOK_path = 4, // path
		TOK_value = 5, // value
		TOK_cmds = 6, // cmds
		TOK_file = 7, // file
		TOK_loutput = 8, // loutput
		TOK_grammar = 9, // grammar
		TOK_assign = 10, // assign
		TOK_11 = 11, // "="
		TOK_12 = 12, // /[ \t]+/
		TOK_NUMOFTOKS = 13, // NUMOFTOKS
	};
}

#endif
