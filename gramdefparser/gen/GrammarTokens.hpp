#ifndef GRAMMARTOKENS_HPP
#define GRAMMARTOKENS_HPP

namespace GrammarTokens
{

enum
{
	TOK_END,
	TOK_ws,
	TOK_nl,
	TOK_string,
	TOK_regex,
	TOK_identifier,
	TOK_parser,
	TOK_grammar,
	TOK_8,
	TOK_9,
	TOK_definition,
	TOK_11,
	TOK_12,
	TOK_13,
	TOK_14,
	TOK_production,
	TOK_body,
	TOK_action,
	TOK_no,
	TOK_19,
	TOK_tokendef,
	TOK_21,
	TOK_22,
	TOK_bodyexpr,
	TOK_bodyterm,
	TOK_id,
	TOK_term,
	TOK_27,
	TOK_28,
	TOK_29,
	TOK_30,
	TOK_31,
	TOK_32,
	TOK_commandlist,
	TOK_command,
	TOK_35,
	TOK_list,
	TOK_lparameter,
	TOK_parameter,
	TOK_39,
	NUMOFTOKS
};

const char* getTokenName(int tok);

}

#endif
