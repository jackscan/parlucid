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
	TOK_grammar,
	TOK__list_grammar,
	TOK_parser,
	TOK_9,
	TOK_10,
	TOK_production,
	TOK__list_production,
	TOK_13,
	TOK_14,
	TOK_15,
	TOK_16,
	TOK_body,
	TOK_action,
	TOK_no,
	TOK_20,
	TOK_21,
	TOK__alt_string_or_regex,
	TOK__sub__alt_string_or_regex,
	TOK_24,
	TOK_bodyexpr,
	TOK_bodyterm,
	TOK_id,
	TOK_term,
	TOK_29,
	TOK_30,
	TOK_31,
	TOK_32,
	TOK__alt_string_or_regex,
	TOK_34,
	TOK_35,
	TOK_command,
	TOK_37,
	TOK__list_37_command,
	TOK_lparameter,
	TOK__opt_lparameter_21,
	TOK_parameter,
	TOK__list_37_parameter,
	TOK__opt_parameter__list_37_parameter,
	TOK_44,
	TOK_NUMOFTOKS = TOK_44
};

const char* getTokenName(int tok);

}

#endif
