#include "../GrammarParser.hpp"
#include "../GrammarTokens.hpp"
#include "../../src/GrammarParserCtrl.hpp"

using namespace GrammarTokens;

GrammarParser::GrammarParser(GrammarParserCtrl &ctrl) : parserCtrl(ctrl), next(&GrammarParser::shift), token(0), symbol(0)
{
	curr.bodyterm = 0; // 24
	curr.id = 0; // 25
	curr.term = 0; // 26
	curr.regex = 0; // 4
	curr.body = 0; // 16
	curr.identifier = 0; // 5
	curr.string = 0; // 3
	curr.bodyexpr = 0; // 23
	curr.no = 0; // 18
}

void GrammarParser::parse()
{
	handleState0();
}

int GrammarParser::shift(int context)
{
	return token = parserCtrl.shift(context);
}

int GrammarParser::push(int)
{
	next = &GrammarParser::recall;
	return symbol;
}

int GrammarParser::recall(int)
{
	next = &GrammarParser::shift;
	return token;
}

void GrammarParser::reduce(int production, int sym)
{
	symbol = sym;
	next = &GrammarParser::push;
	parserCtrl.reduce(production);
}

void GrammarParser::ignore(int production)
{
	next = &GrammarParser::recall;
	parserCtrl.reduce(production);
}

// parser -> parser grammar
void GrammarParser::reduce0()
{
	reduce(0, TOK_parser);
}
// parser -> grammar
void GrammarParser::reduce1()
{
	reduce(1, TOK_parser);
}
// grammar -> identifier 8 9 definition 11 12
void GrammarParser::reduce2()
{
	reduce(2, TOK_grammar);
	parserCtrl.grammar(curr.identifier->str);
}
// grammar -> 13 identifier 14 8 regex 12
void GrammarParser::reduce3()
{
	reduce(3, TOK_grammar);
	parserCtrl.regexGroup(curr.identifier->str);
}
// definition ->
void GrammarParser::reduce4()
{
	reduce(4, TOK_definition);
}
// definition -> definition production
void GrammarParser::reduce5()
{
	reduce(5, TOK_definition);
}
// production -> identifier 8 body action 12
void GrammarParser::reduce6()
{
	reduce(6, TOK_production);
	parserCtrl.production(curr.identifier->str, curr.body->no.str);
}
// production -> identifier 8 19 action 12
void GrammarParser::reduce7()
{
	reduce(7, TOK_production);
	parserCtrl.production(curr.identifier->str);
}
// production -> 19 8 body action 12
void GrammarParser::reduce8()
{
	reduce(8, TOK_production);
	parserCtrl.ignoreProduction(curr.body->no.str);
}
// production -> tokendef 12
void GrammarParser::reduce9()
{
	reduce(9, TOK_production);
}
// tokendef -> identifier 21 string
void GrammarParser::reduce10()
{
	reduce(10, TOK_tokendef);
	parserCtrl.token(curr.identifier->str, curr.string->str);
}
// tokendef -> identifier 21 regex
void GrammarParser::reduce11()
{
	reduce(11, TOK_tokendef);
	parserCtrl.token(curr.identifier->str, curr.regex->str);
}
// body -> body 22 bodyexpr
void GrammarParser::reduce12()
{
	reduce(12, TOK_body);
	parent.body.no.str = parserCtrl.alternative(curr.body->no.str, curr.bodyexpr->no.str);
}
// body -> bodyexpr
void GrammarParser::reduce13()
{
	reduce(13, TOK_body);
	parent.body.no.str = curr.bodyexpr->no.str;
}
// bodyexpr -> bodyexpr bodyterm
void GrammarParser::reduce14()
{
	reduce(14, TOK_bodyexpr);
	parserCtrl.addTerm(curr.bodyexpr->no.str, curr.bodyterm->id.str);
	parent.bodyexpr.no.str = curr.bodyexpr->no.str;
}
// bodyexpr -> bodyterm
void GrammarParser::reduce15()
{
	reduce(15, TOK_bodyexpr);
	parent.bodyexpr.no.str = parserCtrl.newBodyExpr(curr.bodyterm->id.str);
}
// bodyterm -> term
void GrammarParser::reduce16()
{
	reduce(16, TOK_bodyterm);
	parent.bodyterm.id.str = curr.term->id.str;
}
// bodyterm -> 27 body 28
void GrammarParser::reduce17()
{
	reduce(17, TOK_bodyterm);
	parent.bodyterm.id.str = parserCtrl.optional(curr.body->no.str);
}
// bodyterm -> 9 body 11
void GrammarParser::reduce18()
{
	reduce(18, TOK_bodyterm);
	parent.bodyterm.id.str = parserCtrl.list(curr.body->no.str);
}
// bodyterm -> 29 body 30
void GrammarParser::reduce19()
{
	reduce(19, TOK_bodyterm);
	parent.bodyterm.id.str = parserCtrl.newSubExpr(curr.body->no.str);
}
// term -> identifier
void GrammarParser::reduce20()
{
	reduce(20, TOK_term);
	parent.term.id.str = parserCtrl.nonTerminal(curr.identifier->str);
}
// term -> string
void GrammarParser::reduce21()
{
	reduce(21, TOK_term);
	parent.term.id.str = parserCtrl.terminal(curr.string->str);
}
// term -> regex
void GrammarParser::reduce22()
{
	reduce(22, TOK_term);
	parent.term.id.str = parserCtrl.terminal(curr.regex->str);
}
// term -> 31
void GrammarParser::reduce23()
{
	reduce(23, TOK_term);
	parent.term.id.str = parserCtrl.endToken();
}
// action ->
void GrammarParser::reduce24()
{
	reduce(24, TOK_action);
	parserCtrl.action();
}
// action -> 32 commandlist
void GrammarParser::reduce25()
{
	reduce(25, TOK_action);
	parserCtrl.action();
}
// commandlist -> command
void GrammarParser::reduce26()
{
	reduce(26, TOK_commandlist);
}
// commandlist -> commandlist 35 command
void GrammarParser::reduce27()
{
	reduce(27, TOK_commandlist);
}
// command -> identifier 29 30
void GrammarParser::reduce28()
{
	reduce(28, TOK_command);
	parserCtrl.function(curr.identifier->str);
}
// command -> identifier 29 list 30
void GrammarParser::reduce29()
{
	reduce(29, TOK_command);
	parserCtrl.function(curr.identifier->str);
}
// command -> lparameter 21 identifier 29 30
void GrammarParser::reduce30()
{
	reduce(30, TOK_command);
	parserCtrl.function(curr.identifier->str);
}
// command -> lparameter 21 identifier 29 list 30
void GrammarParser::reduce31()
{
	reduce(31, TOK_command);
	parserCtrl.function(curr.identifier->str);
}
// command -> lparameter 21 parameter
void GrammarParser::reduce32()
{
	reduce(32, TOK_command);
	parserCtrl.assignment();
}
// lparameter -> parameter
void GrammarParser::reduce33()
{
	reduce(33, TOK_lparameter);
	parserCtrl.lparameter();
}
// list -> list 35 parameter
void GrammarParser::reduce34()
{
	reduce(34, TOK_list);
	parserCtrl.parameter();
}
// list -> parameter
void GrammarParser::reduce35()
{
	reduce(35, TOK_list);
	parserCtrl.parameter();
}
// parameter -> parameter 39 identifier
void GrammarParser::reduce36()
{
	reduce(36, TOK_parameter);
	parserCtrl.concatParameter(curr.identifier->str);
}
// parameter -> identifier
void GrammarParser::reduce37()
{
	reduce(37, TOK_parameter);
	parserCtrl.concatParameter(curr.identifier->str);
}
// END -> nl
void GrammarParser::reduce38()
{
	ignore(38);
	parserCtrl.newLine();
}
// END -> ws
void GrammarParser::reduce39()
{
	ignore(39);
}
// END -> END parser
void GrammarParser::reduce40()
{
	ignore(40);
}

//[parser ->.parser grammar], (END identifier 13)
//[parser ->.grammar], (END identifier 13)
//[grammar ->.identifier 8 9 definition 11 12], (END identifier 13)
//[grammar ->.13 identifier 14 8 regex 12], (END identifier 13)
//[END -> END.parser], (END)
//<ignore> [END ->.nl], (ws nl identifier parser grammar 13)
//<ignore> [END ->.ws], (ws nl identifier parser grammar 13)
int GrammarParser::handleState0()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		case TOK_identifier: 
			{
				TokIdentifier tok;
				tok.str = parserCtrl.string();
				TokIdentifier *prev = curr.identifier;
				curr.identifier = &tok;
				result = handleState3();
				parserCtrl.release(curr.identifier->str);
				curr.identifier = prev;
				break;
			}
		case TOK_parser: result = handleState4(); break;
		case TOK_grammar: result = handleState5(); break;
		case TOK_13: result = handleState6(); break;
		default:
			{
				const int expected[] = { TOK_ws, TOK_nl, TOK_identifier, TOK_parser, TOK_grammar, TOK_13, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//<IGNORABLE>
//<ignore> [END -> ws.], (END ws nl string regex identifier parser grammar 8 9 definition 11 12 13 14 production body 19 tokendef 21 22 bodyexpr bodyterm term 27 28 29 30 31 32 commandlist command 35 list lparameter parameter 39)
int GrammarParser::handleState1(int context)
{
	(this->*next)(context);
	reduce39();
	return 0;
}

//<IGNORABLE>
//<ignore> [END -> nl.], (END ws nl string regex identifier parser grammar 8 9 definition 11 12 13 14 production body 19 tokendef 21 22 bodyexpr bodyterm term 27 28 29 30 31 32 commandlist command 35 list lparameter parameter 39)
int GrammarParser::handleState2(int context)
{
	(this->*next)(context);
	reduce38();
	return 0;
}

//[grammar -> identifier.8 9 definition 11 12], (END identifier 13)
//<ignore> [END ->.nl], (ws nl 8)
//<ignore> [END ->.ws], (ws nl 8)
int GrammarParser::handleState3()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		case TOK_8: result = handleState7(); break;
		default:
			{
				const int expected[] = { TOK_ws, TOK_nl, TOK_8, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[parser -> parser.grammar], (END identifier 13)
//[grammar ->.identifier 8 9 definition 11 12], (END identifier 13)
//[grammar ->.13 identifier 14 8 regex 12], (END identifier 13)
//[END -> END parser.], (END)
//<ignore> [END ->.nl], (END ws nl identifier 13)
//<ignore> [END ->.ws], (END ws nl identifier 13)
int GrammarParser::handleState4()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		case TOK_identifier: 
			{
				TokIdentifier tok;
				tok.str = parserCtrl.string();
				TokIdentifier *prev = curr.identifier;
				curr.identifier = &tok;
				result = handleState3();
				parserCtrl.release(curr.identifier->str);
				curr.identifier = prev;
				break;
			}
		case TOK_13: result = handleState6(); break;
		case TOK_grammar: result = handleState8(); break;
		default: result = 2; reduce40(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[parser -> grammar.], (END identifier 13)
//<ignore> [END ->.nl], (END ws nl identifier 13)
//<ignore> [END ->.ws], (END ws nl identifier 13)
int GrammarParser::handleState5()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		default: result = 1; reduce1(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[grammar -> 13.identifier 14 8 regex 12], (END identifier 13)
//<ignore> [END ->.nl], (ws nl identifier)
//<ignore> [END ->.ws], (ws nl identifier)
int GrammarParser::handleState6()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		case TOK_identifier: 
			{
				TokIdentifier tok;
				tok.str = parserCtrl.string();
				TokIdentifier *prev = curr.identifier;
				curr.identifier = &tok;
				result = handleState9();
				parserCtrl.release(curr.identifier->str);
				curr.identifier = prev;
				break;
			}
		default:
			{
				const int expected[] = { TOK_ws, TOK_nl, TOK_identifier, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[grammar -> identifier 8.9 definition 11 12], (END identifier 13)
//<ignore> [END ->.nl], (ws nl 9)
//<ignore> [END ->.ws], (ws nl 9)
int GrammarParser::handleState7()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		case TOK_9: result = handleState10(); break;
		default:
			{
				const int expected[] = { TOK_ws, TOK_nl, TOK_9, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[parser -> parser grammar.], (END identifier 13)
//<ignore> [END ->.nl], (END ws nl identifier 13)
//<ignore> [END ->.ws], (END ws nl identifier 13)
int GrammarParser::handleState8()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		default: result = 2; reduce0(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[grammar -> 13 identifier.14 8 regex 12], (END identifier 13)
//<ignore> [END ->.nl], (ws nl 14)
//<ignore> [END ->.ws], (ws nl 14)
int GrammarParser::handleState9()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		case TOK_14: result = handleState11(); break;
		default:
			{
				const int expected[] = { TOK_ws, TOK_nl, TOK_14, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[grammar -> identifier 8 9.definition 11 12], (END identifier 13)
//[definition ->.], (identifier 11 19 tokendef)
//[definition ->.definition production], (identifier 11 19 tokendef)
//<ignore> [END ->.nl], (ws nl identifier definition 11 production 19 tokendef)
//<ignore> [END ->.ws], (ws nl identifier definition 11 production 19 tokendef)
int GrammarParser::handleState10()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		case TOK_definition: result = handleState12(); break;
		case TOK_identifier:
		case TOK_11:
		case TOK_19:
		case TOK_tokendef: result = 0; reduce4(); break;
		default:
			{
				const int expected[] = { TOK_ws, TOK_nl, TOK_identifier, TOK_definition, TOK_11, TOK_19, TOK_tokendef, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[grammar -> 13 identifier 14.8 regex 12], (END identifier 13)
//<ignore> [END ->.nl], (ws nl 8)
//<ignore> [END ->.ws], (ws nl 8)
int GrammarParser::handleState11()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		case TOK_8: result = handleState13(); break;
		default:
			{
				const int expected[] = { TOK_ws, TOK_nl, TOK_8, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[grammar -> identifier 8 9 definition.11 12], (END identifier 13)
//[definition -> definition.production], (identifier 11 19 tokendef)
//[production ->.identifier 8 body action 12], (identifier 11 19 tokendef)
//[production ->.identifier 8 19 action 12], (identifier 11 19 tokendef)
//[production ->.19 8 body action 12], (identifier 11 19 tokendef)
//[production ->.tokendef 12], (identifier 11 19 tokendef)
//[tokendef ->.identifier 21 string], (12)
//[tokendef ->.identifier 21 regex], (12)
//<ignore> [END ->.nl], (ws nl identifier 11 19 tokendef)
//<ignore> [END ->.ws], (ws nl identifier 11 19 tokendef)
int GrammarParser::handleState12()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		case TOK_11: result = handleState15(); break;
		case TOK_production: result = handleState16(); break;
		case TOK_19: result = handleState17(); break;
		case TOK_identifier: 
			{
				TokIdentifier tok;
				tok.str = parserCtrl.string();
				TokIdentifier *prev = curr.identifier;
				curr.identifier = &tok;
				result = handleState14();
				parserCtrl.release(curr.identifier->str);
				curr.identifier = prev;
				break;
			}
		case TOK_tokendef: result = handleState18(); break;
		default:
			{
				const int expected[] = { TOK_ws, TOK_nl, TOK_identifier, TOK_11, TOK_production, TOK_19, TOK_tokendef, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[grammar -> 13 identifier 14 8.regex 12], (END identifier 13)
//<ignore> [END ->.nl], (ws nl regex)
//<ignore> [END ->.ws], (ws nl regex)
int GrammarParser::handleState13()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		case TOK_regex: 
			{
				TokRegex tok;
				tok.str = parserCtrl.string();
				TokRegex *prev = curr.regex;
				curr.regex = &tok;
				result = handleState19();
				parserCtrl.release(curr.regex->str);
				curr.regex = prev;
				break;
			}
		default:
			{
				const int expected[] = { TOK_ws, TOK_nl, TOK_regex, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[production -> identifier.8 body action 12], (identifier 11 19 tokendef)
//[production -> identifier.8 19 action 12], (identifier 11 19 tokendef)
//[tokendef -> identifier.21 string], (12)
//[tokendef -> identifier.21 regex], (12)
//<ignore> [END ->.nl], (ws nl 8 21)
//<ignore> [END ->.ws], (ws nl 8 21)
int GrammarParser::handleState14()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		case TOK_8: result = handleState20(); break;
		case TOK_21: result = handleState21(); break;
		default:
			{
				const int expected[] = { TOK_ws, TOK_nl, TOK_8, TOK_21, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[grammar -> identifier 8 9 definition 11.12], (END identifier 13)
//<ignore> [END ->.nl], (ws nl 12)
//<ignore> [END ->.ws], (ws nl 12)
int GrammarParser::handleState15()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		case TOK_12: result = handleState22(); break;
		default:
			{
				const int expected[] = { TOK_ws, TOK_nl, TOK_12, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[definition -> definition production.], (identifier 11 19 tokendef)
//<ignore> [END ->.nl], (ws nl identifier 11 19 tokendef)
//<ignore> [END ->.ws], (ws nl identifier 11 19 tokendef)
int GrammarParser::handleState16()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		default: result = 2; reduce5(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[production -> 19.8 body action 12], (identifier 11 19 tokendef)
//<ignore> [END ->.nl], (ws nl 8)
//<ignore> [END ->.ws], (ws nl 8)
int GrammarParser::handleState17()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		case TOK_8: result = handleState23(); break;
		default:
			{
				const int expected[] = { TOK_ws, TOK_nl, TOK_8, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[production -> tokendef.12], (identifier 11 19 tokendef)
//<ignore> [END ->.nl], (ws nl 12)
//<ignore> [END ->.ws], (ws nl 12)
int GrammarParser::handleState18()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		case TOK_12: result = handleState24(); break;
		default:
			{
				const int expected[] = { TOK_ws, TOK_nl, TOK_12, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[grammar -> 13 identifier 14 8 regex.12], (END identifier 13)
//<ignore> [END ->.nl], (ws nl 12)
//<ignore> [END ->.ws], (ws nl 12)
int GrammarParser::handleState19()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		case TOK_12: result = handleState25(); break;
		default:
			{
				const int expected[] = { TOK_ws, TOK_nl, TOK_12, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[production -> identifier 8.body action 12], (identifier 11 19 tokendef)
//[production -> identifier 8.19 action 12], (identifier 11 19 tokendef)
//[body ->.body 22 bodyexpr], (12 22 32)
//[body ->.bodyexpr], (12 22 32)
//[bodyexpr ->.bodyexpr bodyterm], (string regex identifier 9 12 22 term 27 29 31 32)
//[bodyexpr ->.bodyterm], (string regex identifier 9 12 22 term 27 29 31 32)
//[bodyterm ->.term], (string regex identifier 9 12 22 term 27 29 31 32)
//[bodyterm ->.27 body 28], (string regex identifier 9 12 22 term 27 29 31 32)
//[bodyterm ->.9 body 11], (string regex identifier 9 12 22 term 27 29 31 32)
//[bodyterm ->.29 body 30], (string regex identifier 9 12 22 term 27 29 31 32)
//[term ->.identifier], (string regex identifier 9 12 22 term 27 29 31 32)
//[term ->.string], (string regex identifier 9 12 22 term 27 29 31 32)
//[term ->.regex], (string regex identifier 9 12 22 term 27 29 31 32)
//[term ->.31], (string regex identifier 9 12 22 term 27 29 31 32)
//<ignore> [END ->.nl], (ws nl string regex identifier 9 body 19 bodyexpr bodyterm term 27 29 31)
//<ignore> [END ->.ws], (ws nl string regex identifier 9 body 19 bodyexpr bodyterm term 27 29 31)
int GrammarParser::handleState20()
{
	int result = 0;
	do
	{
		switch((this->*next)(1))
		{
		case TOK_ws: result = handleState1(1); break;
		case TOK_nl: result = handleState2(1); break;
		case TOK_string: 
			{
				TokString tok;
				tok.str = parserCtrl.string();
				TokString *prev = curr.string;
				curr.string = &tok;
				result = handleState26();
				parserCtrl.release(curr.string->str);
				curr.string = prev;
				break;
			}
		case TOK_regex: 
			{
				TokRegex tok;
				tok.str = parserCtrl.string();
				TokRegex *prev = curr.regex;
				curr.regex = &tok;
				result = handleState27();
				parserCtrl.release(curr.regex->str);
				curr.regex = prev;
				break;
			}
		case TOK_identifier: 
			{
				TokIdentifier tok;
				tok.str = parserCtrl.string();
				TokIdentifier *prev = curr.identifier;
				curr.identifier = &tok;
				result = handleState28();
				parserCtrl.release(curr.identifier->str);
				curr.identifier = prev;
				break;
			}
		case TOK_bodyterm: 
			{
				TokBodyterm tok = parent.bodyterm;
				TokBodyterm *prev = curr.bodyterm;
				curr.bodyterm = &tok;
				result = handleState33();
				curr.bodyterm = prev;
				break;
			}
		case TOK_term: 
			{
				TokTerm tok = parent.term;
				TokTerm *prev = curr.term;
				curr.term = &tok;
				result = handleState34();
				curr.term = prev;
				break;
			}
		case TOK_27: result = handleState35(); break;
		case TOK_29: result = handleState36(); break;
		case TOK_body: 
			{
				TokBody tok = parent.body;
				TokBody *prev = curr.body;
				curr.body = &tok;
				result = handleState30();
				curr.body = prev;
				break;
			}
		case TOK_9: result = handleState29(); break;
		case TOK_19: result = handleState31(); break;
		case TOK_bodyexpr: 
			{
				TokBodyexpr tok = parent.bodyexpr;
				TokBodyexpr *prev = curr.bodyexpr;
				curr.bodyexpr = &tok;
				result = handleState32();
				curr.bodyexpr = prev;
				break;
			}
		case TOK_31: result = handleState37(); break;
		default:
			{
				const int expected[] = { TOK_ws, TOK_nl, TOK_string, TOK_regex, TOK_identifier, TOK_9, TOK_body, TOK_19, TOK_bodyexpr, TOK_bodyterm, TOK_term, TOK_27, TOK_29, TOK_31, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[tokendef -> identifier 21.string], (12)
//[tokendef -> identifier 21.regex], (12)
//<ignore> [END ->.nl], (ws nl string regex)
//<ignore> [END ->.ws], (ws nl string regex)
int GrammarParser::handleState21()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		case TOK_string: 
			{
				TokString tok;
				tok.str = parserCtrl.string();
				TokString *prev = curr.string;
				curr.string = &tok;
				result = handleState38();
				parserCtrl.release(curr.string->str);
				curr.string = prev;
				break;
			}
		case TOK_regex: 
			{
				TokRegex tok;
				tok.str = parserCtrl.string();
				TokRegex *prev = curr.regex;
				curr.regex = &tok;
				result = handleState39();
				parserCtrl.release(curr.regex->str);
				curr.regex = prev;
				break;
			}
		default:
			{
				const int expected[] = { TOK_ws, TOK_nl, TOK_string, TOK_regex, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[grammar -> identifier 8 9 definition 11 12.], (END identifier 13)
//<ignore> [END ->.nl], (END ws nl identifier 13)
//<ignore> [END ->.ws], (END ws nl identifier 13)
int GrammarParser::handleState22()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		default: result = 6; reduce2(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[production -> 19 8.body action 12], (identifier 11 19 tokendef)
//[body ->.body 22 bodyexpr], (12 22 32)
//[body ->.bodyexpr], (12 22 32)
//[bodyexpr ->.bodyexpr bodyterm], (string regex identifier 9 12 22 term 27 29 31 32)
//[bodyexpr ->.bodyterm], (string regex identifier 9 12 22 term 27 29 31 32)
//[bodyterm ->.term], (string regex identifier 9 12 22 term 27 29 31 32)
//[bodyterm ->.27 body 28], (string regex identifier 9 12 22 term 27 29 31 32)
//[bodyterm ->.9 body 11], (string regex identifier 9 12 22 term 27 29 31 32)
//[bodyterm ->.29 body 30], (string regex identifier 9 12 22 term 27 29 31 32)
//[term ->.identifier], (string regex identifier 9 12 22 term 27 29 31 32)
//[term ->.string], (string regex identifier 9 12 22 term 27 29 31 32)
//[term ->.regex], (string regex identifier 9 12 22 term 27 29 31 32)
//[term ->.31], (string regex identifier 9 12 22 term 27 29 31 32)
//<ignore> [END ->.nl], (ws nl string regex identifier 9 body bodyexpr bodyterm term 27 29 31)
//<ignore> [END ->.ws], (ws nl string regex identifier 9 body bodyexpr bodyterm term 27 29 31)
int GrammarParser::handleState23()
{
	int result = 0;
	do
	{
		switch((this->*next)(1))
		{
		case TOK_ws: result = handleState1(1); break;
		case TOK_nl: result = handleState2(1); break;
		case TOK_string: 
			{
				TokString tok;
				tok.str = parserCtrl.string();
				TokString *prev = curr.string;
				curr.string = &tok;
				result = handleState26();
				parserCtrl.release(curr.string->str);
				curr.string = prev;
				break;
			}
		case TOK_regex: 
			{
				TokRegex tok;
				tok.str = parserCtrl.string();
				TokRegex *prev = curr.regex;
				curr.regex = &tok;
				result = handleState27();
				parserCtrl.release(curr.regex->str);
				curr.regex = prev;
				break;
			}
		case TOK_identifier: 
			{
				TokIdentifier tok;
				tok.str = parserCtrl.string();
				TokIdentifier *prev = curr.identifier;
				curr.identifier = &tok;
				result = handleState28();
				parserCtrl.release(curr.identifier->str);
				curr.identifier = prev;
				break;
			}
		case TOK_27: result = handleState35(); break;
		case TOK_bodyterm: 
			{
				TokBodyterm tok = parent.bodyterm;
				TokBodyterm *prev = curr.bodyterm;
				curr.bodyterm = &tok;
				result = handleState33();
				curr.bodyterm = prev;
				break;
			}
		case TOK_31: result = handleState37(); break;
		case TOK_body: 
			{
				TokBody tok = parent.body;
				TokBody *prev = curr.body;
				curr.body = &tok;
				result = handleState40();
				curr.body = prev;
				break;
			}
		case TOK_29: result = handleState36(); break;
		case TOK_term: 
			{
				TokTerm tok = parent.term;
				TokTerm *prev = curr.term;
				curr.term = &tok;
				result = handleState34();
				curr.term = prev;
				break;
			}
		case TOK_bodyexpr: 
			{
				TokBodyexpr tok = parent.bodyexpr;
				TokBodyexpr *prev = curr.bodyexpr;
				curr.bodyexpr = &tok;
				result = handleState32();
				curr.bodyexpr = prev;
				break;
			}
		case TOK_9: result = handleState29(); break;
		default:
			{
				const int expected[] = { TOK_ws, TOK_nl, TOK_string, TOK_regex, TOK_identifier, TOK_9, TOK_body, TOK_bodyexpr, TOK_bodyterm, TOK_term, TOK_27, TOK_29, TOK_31, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[production -> tokendef 12.], (identifier 11 19 tokendef)
//<ignore> [END ->.nl], (ws nl identifier 11 19 tokendef)
//<ignore> [END ->.ws], (ws nl identifier 11 19 tokendef)
int GrammarParser::handleState24()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		default: result = 2; reduce9(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[grammar -> 13 identifier 14 8 regex 12.], (END identifier 13)
//<ignore> [END ->.nl], (END ws nl identifier 13)
//<ignore> [END ->.ws], (END ws nl identifier 13)
int GrammarParser::handleState25()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		default: result = 6; reduce3(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[term -> string.], (string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//<ignore> [END ->.nl], (ws nl string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//<ignore> [END ->.ws], (ws nl string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
int GrammarParser::handleState26()
{
	int result = 0;
	do
	{
		switch((this->*next)(1))
		{
		case TOK_ws: result = handleState1(1); break;
		case TOK_nl: result = handleState2(1); break;
		default: result = 1; reduce21(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[term -> regex.], (string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//<ignore> [END ->.nl], (ws nl string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//<ignore> [END ->.ws], (ws nl string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
int GrammarParser::handleState27()
{
	int result = 0;
	do
	{
		switch((this->*next)(1))
		{
		case TOK_ws: result = handleState1(1); break;
		case TOK_nl: result = handleState2(1); break;
		default: result = 1; reduce22(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[term -> identifier.], (string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//<ignore> [END ->.nl], (ws nl string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//<ignore> [END ->.ws], (ws nl string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
int GrammarParser::handleState28()
{
	int result = 0;
	do
	{
		switch((this->*next)(1))
		{
		case TOK_ws: result = handleState1(1); break;
		case TOK_nl: result = handleState2(1); break;
		default: result = 1; reduce20(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[body ->.body 22 bodyexpr], (11 22)
//[body ->.bodyexpr], (11 22)
//[bodyexpr ->.bodyexpr bodyterm], (string regex identifier 9 11 22 term 27 29 31)
//[bodyexpr ->.bodyterm], (string regex identifier 9 11 22 term 27 29 31)
//[bodyterm ->.term], (string regex identifier 9 11 22 term 27 29 31)
//[bodyterm ->.27 body 28], (string regex identifier 9 11 22 term 27 29 31)
//[bodyterm ->.9 body 11], (string regex identifier 9 11 22 term 27 29 31)
//[bodyterm -> 9.body 11], (string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//[bodyterm ->.29 body 30], (string regex identifier 9 11 22 term 27 29 31)
//[term ->.identifier], (string regex identifier 9 11 22 term 27 29 31)
//[term ->.string], (string regex identifier 9 11 22 term 27 29 31)
//[term ->.regex], (string regex identifier 9 11 22 term 27 29 31)
//[term ->.31], (string regex identifier 9 11 22 term 27 29 31)
//<ignore> [END ->.nl], (ws nl string regex identifier 9 body bodyexpr bodyterm term 27 29 31)
//<ignore> [END ->.ws], (ws nl string regex identifier 9 body bodyexpr bodyterm term 27 29 31)
int GrammarParser::handleState29()
{
	int result = 0;
	do
	{
		switch((this->*next)(1))
		{
		case TOK_ws: result = handleState1(1); break;
		case TOK_nl: result = handleState2(1); break;
		case TOK_string: 
			{
				TokString tok;
				tok.str = parserCtrl.string();
				TokString *prev = curr.string;
				curr.string = &tok;
				result = handleState26();
				parserCtrl.release(curr.string->str);
				curr.string = prev;
				break;
			}
		case TOK_regex: 
			{
				TokRegex tok;
				tok.str = parserCtrl.string();
				TokRegex *prev = curr.regex;
				curr.regex = &tok;
				result = handleState27();
				parserCtrl.release(curr.regex->str);
				curr.regex = prev;
				break;
			}
		case TOK_identifier: 
			{
				TokIdentifier tok;
				tok.str = parserCtrl.string();
				TokIdentifier *prev = curr.identifier;
				curr.identifier = &tok;
				result = handleState28();
				parserCtrl.release(curr.identifier->str);
				curr.identifier = prev;
				break;
			}
		case TOK_27: result = handleState35(); break;
		case TOK_bodyterm: 
			{
				TokBodyterm tok = parent.bodyterm;
				TokBodyterm *prev = curr.bodyterm;
				curr.bodyterm = &tok;
				result = handleState33();
				curr.bodyterm = prev;
				break;
			}
		case TOK_31: result = handleState37(); break;
		case TOK_body: 
			{
				TokBody tok = parent.body;
				TokBody *prev = curr.body;
				curr.body = &tok;
				result = handleState41();
				curr.body = prev;
				break;
			}
		case TOK_29: result = handleState36(); break;
		case TOK_term: 
			{
				TokTerm tok = parent.term;
				TokTerm *prev = curr.term;
				curr.term = &tok;
				result = handleState34();
				curr.term = prev;
				break;
			}
		case TOK_bodyexpr: 
			{
				TokBodyexpr tok = parent.bodyexpr;
				TokBodyexpr *prev = curr.bodyexpr;
				curr.bodyexpr = &tok;
				result = handleState32();
				curr.bodyexpr = prev;
				break;
			}
		case TOK_9: result = handleState29(); break;
		default:
			{
				const int expected[] = { TOK_ws, TOK_nl, TOK_string, TOK_regex, TOK_identifier, TOK_9, TOK_body, TOK_bodyexpr, TOK_bodyterm, TOK_term, TOK_27, TOK_29, TOK_31, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[production -> identifier 8 body.action 12], (identifier 11 19 tokendef)
//[body -> body.22 bodyexpr], (12 22 32)
//[action ->.], (12)
//[action ->.32 commandlist], (12)
//<ignore> [END ->.nl], (ws nl 12 22 32)
//<ignore> [END ->.ws], (ws nl 12 22 32)
int GrammarParser::handleState30()
{
	int result = 0;
	do
	{
		switch((this->*next)(1))
		{
		case TOK_ws: result = handleState1(1); break;
		case TOK_nl: result = handleState2(1); break;
		case TOK_22: result = handleState43(); break;
		case TOK_action: result = handleState42(); break;
		case TOK_32: result = handleState44(); break;
		case TOK_12: result = 0; reduce24(); break;
		default:
			{
				const int expected[] = { TOK_ws, TOK_nl, TOK_12, TOK_action, TOK_22, TOK_32, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[production -> identifier 8 19.action 12], (identifier 11 19 tokendef)
//[action ->.], (12)
//[action ->.32 commandlist], (12)
//<ignore> [END ->.nl], (ws nl 12 32)
//<ignore> [END ->.ws], (ws nl 12 32)
int GrammarParser::handleState31()
{
	int result = 0;
	do
	{
		switch((this->*next)(1))
		{
		case TOK_ws: result = handleState1(1); break;
		case TOK_nl: result = handleState2(1); break;
		case TOK_action: result = handleState45(); break;
		case TOK_32: result = handleState44(); break;
		case TOK_12: result = 0; reduce24(); break;
		default:
			{
				const int expected[] = { TOK_ws, TOK_nl, TOK_12, TOK_action, TOK_32, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[body -> bodyexpr.], (11 12 22 28 30 32)
//[bodyexpr -> bodyexpr.bodyterm], (string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//[bodyterm ->.term], (string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//[bodyterm ->.27 body 28], (string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//[bodyterm ->.9 body 11], (string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//[bodyterm ->.29 body 30], (string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//[term ->.identifier], (string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//[term ->.string], (string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//[term ->.regex], (string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//[term ->.31], (string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//<ignore> [END ->.nl], (ws nl string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//<ignore> [END ->.ws], (ws nl string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
int GrammarParser::handleState32()
{
	int result = 0;
	do
	{
		switch((this->*next)(1))
		{
		case TOK_ws: result = handleState1(1); break;
		case TOK_nl: result = handleState2(1); break;
		case TOK_string: 
			{
				TokString tok;
				tok.str = parserCtrl.string();
				TokString *prev = curr.string;
				curr.string = &tok;
				result = handleState26();
				parserCtrl.release(curr.string->str);
				curr.string = prev;
				break;
			}
		case TOK_regex: 
			{
				TokRegex tok;
				tok.str = parserCtrl.string();
				TokRegex *prev = curr.regex;
				curr.regex = &tok;
				result = handleState27();
				parserCtrl.release(curr.regex->str);
				curr.regex = prev;
				break;
			}
		case TOK_identifier: 
			{
				TokIdentifier tok;
				tok.str = parserCtrl.string();
				TokIdentifier *prev = curr.identifier;
				curr.identifier = &tok;
				result = handleState28();
				parserCtrl.release(curr.identifier->str);
				curr.identifier = prev;
				break;
			}
		case TOK_27: result = handleState35(); break;
		case TOK_bodyterm: 
			{
				TokBodyterm tok = parent.bodyterm;
				TokBodyterm *prev = curr.bodyterm;
				curr.bodyterm = &tok;
				result = handleState46();
				curr.bodyterm = prev;
				break;
			}
		case TOK_29: result = handleState36(); break;
		case TOK_term: 
			{
				TokTerm tok = parent.term;
				TokTerm *prev = curr.term;
				curr.term = &tok;
				result = handleState34();
				curr.term = prev;
				break;
			}
		case TOK_9: result = handleState29(); break;
		case TOK_31: result = handleState37(); break;
		default: result = 1; reduce13(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[bodyexpr -> bodyterm.], (string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//<ignore> [END ->.nl], (ws nl string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//<ignore> [END ->.ws], (ws nl string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
int GrammarParser::handleState33()
{
	int result = 0;
	do
	{
		switch((this->*next)(1))
		{
		case TOK_ws: result = handleState1(1); break;
		case TOK_nl: result = handleState2(1); break;
		default: result = 1; reduce15(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[bodyterm -> term.], (string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//<ignore> [END ->.nl], (ws nl string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//<ignore> [END ->.ws], (ws nl string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
int GrammarParser::handleState34()
{
	int result = 0;
	do
	{
		switch((this->*next)(1))
		{
		case TOK_ws: result = handleState1(1); break;
		case TOK_nl: result = handleState2(1); break;
		default: result = 1; reduce16(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[body ->.body 22 bodyexpr], (22 28)
//[body ->.bodyexpr], (22 28)
//[bodyexpr ->.bodyexpr bodyterm], (string regex identifier 9 22 term 27 28 29 31)
//[bodyexpr ->.bodyterm], (string regex identifier 9 22 term 27 28 29 31)
//[bodyterm ->.term], (string regex identifier 9 22 term 27 28 29 31)
//[bodyterm ->.27 body 28], (string regex identifier 9 22 term 27 28 29 31)
//[bodyterm -> 27.body 28], (string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//[bodyterm ->.9 body 11], (string regex identifier 9 22 term 27 28 29 31)
//[bodyterm ->.29 body 30], (string regex identifier 9 22 term 27 28 29 31)
//[term ->.identifier], (string regex identifier 9 22 term 27 28 29 31)
//[term ->.string], (string regex identifier 9 22 term 27 28 29 31)
//[term ->.regex], (string regex identifier 9 22 term 27 28 29 31)
//[term ->.31], (string regex identifier 9 22 term 27 28 29 31)
//<ignore> [END ->.nl], (ws nl string regex identifier 9 body bodyexpr bodyterm term 27 29 31)
//<ignore> [END ->.ws], (ws nl string regex identifier 9 body bodyexpr bodyterm term 27 29 31)
int GrammarParser::handleState35()
{
	int result = 0;
	do
	{
		switch((this->*next)(1))
		{
		case TOK_ws: result = handleState1(1); break;
		case TOK_nl: result = handleState2(1); break;
		case TOK_string: 
			{
				TokString tok;
				tok.str = parserCtrl.string();
				TokString *prev = curr.string;
				curr.string = &tok;
				result = handleState26();
				parserCtrl.release(curr.string->str);
				curr.string = prev;
				break;
			}
		case TOK_regex: 
			{
				TokRegex tok;
				tok.str = parserCtrl.string();
				TokRegex *prev = curr.regex;
				curr.regex = &tok;
				result = handleState27();
				parserCtrl.release(curr.regex->str);
				curr.regex = prev;
				break;
			}
		case TOK_identifier: 
			{
				TokIdentifier tok;
				tok.str = parserCtrl.string();
				TokIdentifier *prev = curr.identifier;
				curr.identifier = &tok;
				result = handleState28();
				parserCtrl.release(curr.identifier->str);
				curr.identifier = prev;
				break;
			}
		case TOK_27: result = handleState35(); break;
		case TOK_bodyterm: 
			{
				TokBodyterm tok = parent.bodyterm;
				TokBodyterm *prev = curr.bodyterm;
				curr.bodyterm = &tok;
				result = handleState33();
				curr.bodyterm = prev;
				break;
			}
		case TOK_31: result = handleState37(); break;
		case TOK_body: 
			{
				TokBody tok = parent.body;
				TokBody *prev = curr.body;
				curr.body = &tok;
				result = handleState47();
				curr.body = prev;
				break;
			}
		case TOK_29: result = handleState36(); break;
		case TOK_term: 
			{
				TokTerm tok = parent.term;
				TokTerm *prev = curr.term;
				curr.term = &tok;
				result = handleState34();
				curr.term = prev;
				break;
			}
		case TOK_bodyexpr: 
			{
				TokBodyexpr tok = parent.bodyexpr;
				TokBodyexpr *prev = curr.bodyexpr;
				curr.bodyexpr = &tok;
				result = handleState32();
				curr.bodyexpr = prev;
				break;
			}
		case TOK_9: result = handleState29(); break;
		default:
			{
				const int expected[] = { TOK_ws, TOK_nl, TOK_string, TOK_regex, TOK_identifier, TOK_9, TOK_body, TOK_bodyexpr, TOK_bodyterm, TOK_term, TOK_27, TOK_29, TOK_31, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[body ->.body 22 bodyexpr], (22 30)
//[body ->.bodyexpr], (22 30)
//[bodyexpr ->.bodyexpr bodyterm], (string regex identifier 9 22 term 27 29 30 31)
//[bodyexpr ->.bodyterm], (string regex identifier 9 22 term 27 29 30 31)
//[bodyterm ->.term], (string regex identifier 9 22 term 27 29 30 31)
//[bodyterm ->.27 body 28], (string regex identifier 9 22 term 27 29 30 31)
//[bodyterm ->.9 body 11], (string regex identifier 9 22 term 27 29 30 31)
//[bodyterm ->.29 body 30], (string regex identifier 9 22 term 27 29 30 31)
//[bodyterm -> 29.body 30], (string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//[term ->.identifier], (string regex identifier 9 22 term 27 29 30 31)
//[term ->.string], (string regex identifier 9 22 term 27 29 30 31)
//[term ->.regex], (string regex identifier 9 22 term 27 29 30 31)
//[term ->.31], (string regex identifier 9 22 term 27 29 30 31)
//<ignore> [END ->.nl], (ws nl string regex identifier 9 body bodyexpr bodyterm term 27 29 31)
//<ignore> [END ->.ws], (ws nl string regex identifier 9 body bodyexpr bodyterm term 27 29 31)
int GrammarParser::handleState36()
{
	int result = 0;
	do
	{
		switch((this->*next)(1))
		{
		case TOK_ws: result = handleState1(1); break;
		case TOK_nl: result = handleState2(1); break;
		case TOK_string: 
			{
				TokString tok;
				tok.str = parserCtrl.string();
				TokString *prev = curr.string;
				curr.string = &tok;
				result = handleState26();
				parserCtrl.release(curr.string->str);
				curr.string = prev;
				break;
			}
		case TOK_regex: 
			{
				TokRegex tok;
				tok.str = parserCtrl.string();
				TokRegex *prev = curr.regex;
				curr.regex = &tok;
				result = handleState27();
				parserCtrl.release(curr.regex->str);
				curr.regex = prev;
				break;
			}
		case TOK_identifier: 
			{
				TokIdentifier tok;
				tok.str = parserCtrl.string();
				TokIdentifier *prev = curr.identifier;
				curr.identifier = &tok;
				result = handleState28();
				parserCtrl.release(curr.identifier->str);
				curr.identifier = prev;
				break;
			}
		case TOK_27: result = handleState35(); break;
		case TOK_bodyterm: 
			{
				TokBodyterm tok = parent.bodyterm;
				TokBodyterm *prev = curr.bodyterm;
				curr.bodyterm = &tok;
				result = handleState33();
				curr.bodyterm = prev;
				break;
			}
		case TOK_31: result = handleState37(); break;
		case TOK_body: 
			{
				TokBody tok = parent.body;
				TokBody *prev = curr.body;
				curr.body = &tok;
				result = handleState48();
				curr.body = prev;
				break;
			}
		case TOK_29: result = handleState36(); break;
		case TOK_term: 
			{
				TokTerm tok = parent.term;
				TokTerm *prev = curr.term;
				curr.term = &tok;
				result = handleState34();
				curr.term = prev;
				break;
			}
		case TOK_bodyexpr: 
			{
				TokBodyexpr tok = parent.bodyexpr;
				TokBodyexpr *prev = curr.bodyexpr;
				curr.bodyexpr = &tok;
				result = handleState32();
				curr.bodyexpr = prev;
				break;
			}
		case TOK_9: result = handleState29(); break;
		default:
			{
				const int expected[] = { TOK_ws, TOK_nl, TOK_string, TOK_regex, TOK_identifier, TOK_9, TOK_body, TOK_bodyexpr, TOK_bodyterm, TOK_term, TOK_27, TOK_29, TOK_31, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[term -> 31.], (string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//<ignore> [END ->.nl], (ws nl string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//<ignore> [END ->.ws], (ws nl string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
int GrammarParser::handleState37()
{
	int result = 0;
	do
	{
		switch((this->*next)(1))
		{
		case TOK_ws: result = handleState1(1); break;
		case TOK_nl: result = handleState2(1); break;
		default: result = 1; reduce23(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[tokendef -> identifier 21 string.], (12)
//<ignore> [END ->.nl], (ws nl 12)
//<ignore> [END ->.ws], (ws nl 12)
int GrammarParser::handleState38()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		default: result = 3; reduce10(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[tokendef -> identifier 21 regex.], (12)
//<ignore> [END ->.nl], (ws nl 12)
//<ignore> [END ->.ws], (ws nl 12)
int GrammarParser::handleState39()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		default: result = 3; reduce11(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[production -> 19 8 body.action 12], (identifier 11 19 tokendef)
//[body -> body.22 bodyexpr], (12 22 32)
//[action ->.], (12)
//[action ->.32 commandlist], (12)
//<ignore> [END ->.nl], (ws nl 12 22 32)
//<ignore> [END ->.ws], (ws nl 12 22 32)
int GrammarParser::handleState40()
{
	int result = 0;
	do
	{
		switch((this->*next)(1))
		{
		case TOK_ws: result = handleState1(1); break;
		case TOK_nl: result = handleState2(1); break;
		case TOK_22: result = handleState43(); break;
		case TOK_action: result = handleState49(); break;
		case TOK_32: result = handleState44(); break;
		case TOK_12: result = 0; reduce24(); break;
		default:
			{
				const int expected[] = { TOK_ws, TOK_nl, TOK_12, TOK_action, TOK_22, TOK_32, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[body -> body.22 bodyexpr], (11 22)
//[bodyterm -> 9 body.11], (string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//<ignore> [END ->.nl], (ws nl 11 22)
//<ignore> [END ->.ws], (ws nl 11 22)
int GrammarParser::handleState41()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		case TOK_11: result = handleState50(); break;
		case TOK_22: result = handleState43(); break;
		default:
			{
				const int expected[] = { TOK_ws, TOK_nl, TOK_11, TOK_22, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[production -> identifier 8 body action.12], (identifier 11 19 tokendef)
//<ignore> [END ->.nl], (ws nl 12)
//<ignore> [END ->.ws], (ws nl 12)
int GrammarParser::handleState42()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		case TOK_12: result = handleState51(); break;
		default:
			{
				const int expected[] = { TOK_ws, TOK_nl, TOK_12, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[body -> body 22.bodyexpr], (11 12 22 28 30 32)
//[bodyexpr ->.bodyexpr bodyterm], (string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//[bodyexpr ->.bodyterm], (string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//[bodyterm ->.term], (string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//[bodyterm ->.27 body 28], (string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//[bodyterm ->.9 body 11], (string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//[bodyterm ->.29 body 30], (string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//[term ->.identifier], (string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//[term ->.string], (string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//[term ->.regex], (string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//[term ->.31], (string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//<ignore> [END ->.nl], (ws nl string regex identifier 9 bodyexpr bodyterm term 27 29 31)
//<ignore> [END ->.ws], (ws nl string regex identifier 9 bodyexpr bodyterm term 27 29 31)
int GrammarParser::handleState43()
{
	int result = 0;
	do
	{
		switch((this->*next)(1))
		{
		case TOK_ws: result = handleState1(1); break;
		case TOK_nl: result = handleState2(1); break;
		case TOK_string: 
			{
				TokString tok;
				tok.str = parserCtrl.string();
				TokString *prev = curr.string;
				curr.string = &tok;
				result = handleState26();
				parserCtrl.release(curr.string->str);
				curr.string = prev;
				break;
			}
		case TOK_regex: 
			{
				TokRegex tok;
				tok.str = parserCtrl.string();
				TokRegex *prev = curr.regex;
				curr.regex = &tok;
				result = handleState27();
				parserCtrl.release(curr.regex->str);
				curr.regex = prev;
				break;
			}
		case TOK_identifier: 
			{
				TokIdentifier tok;
				tok.str = parserCtrl.string();
				TokIdentifier *prev = curr.identifier;
				curr.identifier = &tok;
				result = handleState28();
				parserCtrl.release(curr.identifier->str);
				curr.identifier = prev;
				break;
			}
		case TOK_27: result = handleState35(); break;
		case TOK_bodyterm: 
			{
				TokBodyterm tok = parent.bodyterm;
				TokBodyterm *prev = curr.bodyterm;
				curr.bodyterm = &tok;
				result = handleState33();
				curr.bodyterm = prev;
				break;
			}
		case TOK_31: result = handleState37(); break;
		case TOK_29: result = handleState36(); break;
		case TOK_term: 
			{
				TokTerm tok = parent.term;
				TokTerm *prev = curr.term;
				curr.term = &tok;
				result = handleState34();
				curr.term = prev;
				break;
			}
		case TOK_9: result = handleState29(); break;
		case TOK_bodyexpr: 
			{
				TokBodyexpr tok = parent.bodyexpr;
				TokBodyexpr *prev = curr.bodyexpr;
				curr.bodyexpr = &tok;
				result = handleState52();
				curr.bodyexpr = prev;
				break;
			}
		default:
			{
				const int expected[] = { TOK_ws, TOK_nl, TOK_string, TOK_regex, TOK_identifier, TOK_9, TOK_bodyexpr, TOK_bodyterm, TOK_term, TOK_27, TOK_29, TOK_31, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[action -> 32.commandlist], (12)
//[commandlist ->.command], (12 35)
//[commandlist ->.commandlist 35 command], (12 35)
//[command ->.identifier 29 30], (12 35)
//[command ->.identifier 29 list 30], (12 35)
//[command ->.lparameter 21 identifier 29 30], (12 35)
//[command ->.lparameter 21 identifier 29 list 30], (12 35)
//[command ->.lparameter 21 parameter], (12 35)
//[lparameter ->.parameter], (21)
//[parameter ->.parameter 39 identifier], (21 39)
//[parameter ->.identifier], (21 39)
//<ignore> [END ->.nl], (ws nl identifier commandlist command lparameter parameter)
//<ignore> [END ->.ws], (ws nl identifier commandlist command lparameter parameter)
int GrammarParser::handleState44()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		case TOK_lparameter: result = handleState56(); break;
		case TOK_parameter: result = handleState57(); break;
		case TOK_commandlist: result = handleState54(); break;
		case TOK_command: result = handleState55(); break;
		case TOK_identifier: 
			{
				TokIdentifier tok;
				tok.str = parserCtrl.string();
				TokIdentifier *prev = curr.identifier;
				curr.identifier = &tok;
				result = handleState53();
				parserCtrl.release(curr.identifier->str);
				curr.identifier = prev;
				break;
			}
		default:
			{
				const int expected[] = { TOK_ws, TOK_nl, TOK_identifier, TOK_commandlist, TOK_command, TOK_lparameter, TOK_parameter, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[production -> identifier 8 19 action.12], (identifier 11 19 tokendef)
//<ignore> [END ->.nl], (ws nl 12)
//<ignore> [END ->.ws], (ws nl 12)
int GrammarParser::handleState45()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		case TOK_12: result = handleState58(); break;
		default:
			{
				const int expected[] = { TOK_ws, TOK_nl, TOK_12, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[bodyexpr -> bodyexpr bodyterm.], (string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//<ignore> [END ->.nl], (ws nl string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//<ignore> [END ->.ws], (ws nl string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
int GrammarParser::handleState46()
{
	int result = 0;
	do
	{
		switch((this->*next)(1))
		{
		case TOK_ws: result = handleState1(1); break;
		case TOK_nl: result = handleState2(1); break;
		default: result = 2; reduce14(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[body -> body.22 bodyexpr], (22 28)
//[bodyterm -> 27 body.28], (string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//<ignore> [END ->.nl], (ws nl 22 28)
//<ignore> [END ->.ws], (ws nl 22 28)
int GrammarParser::handleState47()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		case TOK_22: result = handleState43(); break;
		case TOK_28: result = handleState59(); break;
		default:
			{
				const int expected[] = { TOK_ws, TOK_nl, TOK_22, TOK_28, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[body -> body.22 bodyexpr], (22 30)
//[bodyterm -> 29 body.30], (string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//<ignore> [END ->.nl], (ws nl 22 30)
//<ignore> [END ->.ws], (ws nl 22 30)
int GrammarParser::handleState48()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		case TOK_22: result = handleState43(); break;
		case TOK_30: result = handleState60(); break;
		default:
			{
				const int expected[] = { TOK_ws, TOK_nl, TOK_22, TOK_30, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[production -> 19 8 body action.12], (identifier 11 19 tokendef)
//<ignore> [END ->.nl], (ws nl 12)
//<ignore> [END ->.ws], (ws nl 12)
int GrammarParser::handleState49()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		case TOK_12: result = handleState61(); break;
		default:
			{
				const int expected[] = { TOK_ws, TOK_nl, TOK_12, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[bodyterm -> 9 body 11.], (string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//<ignore> [END ->.nl], (ws nl string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//<ignore> [END ->.ws], (ws nl string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
int GrammarParser::handleState50()
{
	int result = 0;
	do
	{
		switch((this->*next)(1))
		{
		case TOK_ws: result = handleState1(1); break;
		case TOK_nl: result = handleState2(1); break;
		default: result = 3; reduce18(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[production -> identifier 8 body action 12.], (identifier 11 19 tokendef)
//<ignore> [END ->.nl], (ws nl identifier 11 19 tokendef)
//<ignore> [END ->.ws], (ws nl identifier 11 19 tokendef)
int GrammarParser::handleState51()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		default: result = 5; reduce6(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[body -> body 22 bodyexpr.], (11 12 22 28 30 32)
//[bodyexpr -> bodyexpr.bodyterm], (string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//[bodyterm ->.term], (string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//[bodyterm ->.27 body 28], (string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//[bodyterm ->.9 body 11], (string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//[bodyterm ->.29 body 30], (string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//[term ->.identifier], (string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//[term ->.string], (string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//[term ->.regex], (string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//[term ->.31], (string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//<ignore> [END ->.nl], (ws nl string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//<ignore> [END ->.ws], (ws nl string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
int GrammarParser::handleState52()
{
	int result = 0;
	do
	{
		switch((this->*next)(1))
		{
		case TOK_ws: result = handleState1(1); break;
		case TOK_nl: result = handleState2(1); break;
		case TOK_string: 
			{
				TokString tok;
				tok.str = parserCtrl.string();
				TokString *prev = curr.string;
				curr.string = &tok;
				result = handleState26();
				parserCtrl.release(curr.string->str);
				curr.string = prev;
				break;
			}
		case TOK_regex: 
			{
				TokRegex tok;
				tok.str = parserCtrl.string();
				TokRegex *prev = curr.regex;
				curr.regex = &tok;
				result = handleState27();
				parserCtrl.release(curr.regex->str);
				curr.regex = prev;
				break;
			}
		case TOK_identifier: 
			{
				TokIdentifier tok;
				tok.str = parserCtrl.string();
				TokIdentifier *prev = curr.identifier;
				curr.identifier = &tok;
				result = handleState28();
				parserCtrl.release(curr.identifier->str);
				curr.identifier = prev;
				break;
			}
		case TOK_27: result = handleState35(); break;
		case TOK_bodyterm: 
			{
				TokBodyterm tok = parent.bodyterm;
				TokBodyterm *prev = curr.bodyterm;
				curr.bodyterm = &tok;
				result = handleState46();
				curr.bodyterm = prev;
				break;
			}
		case TOK_29: result = handleState36(); break;
		case TOK_term: 
			{
				TokTerm tok = parent.term;
				TokTerm *prev = curr.term;
				curr.term = &tok;
				result = handleState34();
				curr.term = prev;
				break;
			}
		case TOK_9: result = handleState29(); break;
		case TOK_31: result = handleState37(); break;
		default: result = 3; reduce12(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[command -> identifier.29 30], (12 35)
//[command -> identifier.29 list 30], (12 35)
//[parameter -> identifier.], (21 39)
//<ignore> [END ->.nl], (ws nl 21 29 39)
//<ignore> [END ->.ws], (ws nl 21 29 39)
int GrammarParser::handleState53()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		case TOK_29: result = handleState62(); break;
		default: result = 1; reduce37(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[action -> 32 commandlist.], (12)
//[commandlist -> commandlist.35 command], (12 35)
//<ignore> [END ->.nl], (ws nl 12 35)
//<ignore> [END ->.ws], (ws nl 12 35)
int GrammarParser::handleState54()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		case TOK_35: result = handleState63(); break;
		default: result = 2; reduce25(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[commandlist -> command.], (12 35)
//<ignore> [END ->.nl], (ws nl 12 35)
//<ignore> [END ->.ws], (ws nl 12 35)
int GrammarParser::handleState55()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		default: result = 1; reduce26(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[command -> lparameter.21 identifier 29 30], (12 35)
//[command -> lparameter.21 identifier 29 list 30], (12 35)
//[command -> lparameter.21 parameter], (12 35)
//<ignore> [END ->.nl], (ws nl 21)
//<ignore> [END ->.ws], (ws nl 21)
int GrammarParser::handleState56()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		case TOK_21: result = handleState64(); break;
		default:
			{
				const int expected[] = { TOK_ws, TOK_nl, TOK_21, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[lparameter -> parameter.], (21)
//[parameter -> parameter.39 identifier], (21 39)
//<ignore> [END ->.nl], (ws nl 21 39)
//<ignore> [END ->.ws], (ws nl 21 39)
int GrammarParser::handleState57()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		case TOK_39: result = handleState65(); break;
		default: result = 1; reduce33(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[production -> identifier 8 19 action 12.], (identifier 11 19 tokendef)
//<ignore> [END ->.nl], (ws nl identifier 11 19 tokendef)
//<ignore> [END ->.ws], (ws nl identifier 11 19 tokendef)
int GrammarParser::handleState58()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		default: result = 5; reduce7(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[bodyterm -> 27 body 28.], (string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//<ignore> [END ->.nl], (ws nl string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//<ignore> [END ->.ws], (ws nl string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
int GrammarParser::handleState59()
{
	int result = 0;
	do
	{
		switch((this->*next)(1))
		{
		case TOK_ws: result = handleState1(1); break;
		case TOK_nl: result = handleState2(1); break;
		default: result = 3; reduce17(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[bodyterm -> 29 body 30.], (string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//<ignore> [END ->.nl], (ws nl string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
//<ignore> [END ->.ws], (ws nl string regex identifier 9 11 12 22 term 27 28 29 30 31 32)
int GrammarParser::handleState60()
{
	int result = 0;
	do
	{
		switch((this->*next)(1))
		{
		case TOK_ws: result = handleState1(1); break;
		case TOK_nl: result = handleState2(1); break;
		default: result = 3; reduce19(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[production -> 19 8 body action 12.], (identifier 11 19 tokendef)
//<ignore> [END ->.nl], (ws nl identifier 11 19 tokendef)
//<ignore> [END ->.ws], (ws nl identifier 11 19 tokendef)
int GrammarParser::handleState61()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		default: result = 5; reduce8(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[command -> identifier 29.30], (12 35)
//[command -> identifier 29.list 30], (12 35)
//[list ->.list 35 parameter], (30 35)
//[list ->.parameter], (30 35)
//[parameter ->.parameter 39 identifier], (30 35 39)
//[parameter ->.identifier], (30 35 39)
//<ignore> [END ->.nl], (ws nl identifier 30 list parameter)
//<ignore> [END ->.ws], (ws nl identifier 30 list parameter)
int GrammarParser::handleState62()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		case TOK_30: result = handleState67(); break;
		case TOK_list: result = handleState68(); break;
		case TOK_parameter: result = handleState69(); break;
		case TOK_identifier: 
			{
				TokIdentifier tok;
				tok.str = parserCtrl.string();
				TokIdentifier *prev = curr.identifier;
				curr.identifier = &tok;
				result = handleState66();
				parserCtrl.release(curr.identifier->str);
				curr.identifier = prev;
				break;
			}
		default:
			{
				const int expected[] = { TOK_ws, TOK_nl, TOK_identifier, TOK_30, TOK_list, TOK_parameter, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[commandlist -> commandlist 35.command], (12 35)
//[command ->.identifier 29 30], (12 35)
//[command ->.identifier 29 list 30], (12 35)
//[command ->.lparameter 21 identifier 29 30], (12 35)
//[command ->.lparameter 21 identifier 29 list 30], (12 35)
//[command ->.lparameter 21 parameter], (12 35)
//[lparameter ->.parameter], (21)
//[parameter ->.parameter 39 identifier], (21 39)
//[parameter ->.identifier], (21 39)
//<ignore> [END ->.nl], (ws nl identifier lparameter parameter)
//<ignore> [END ->.ws], (ws nl identifier lparameter parameter)
int GrammarParser::handleState63()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		case TOK_command: result = handleState70(); break;
		case TOK_lparameter: result = handleState56(); break;
		case TOK_parameter: result = handleState57(); break;
		case TOK_identifier: 
			{
				TokIdentifier tok;
				tok.str = parserCtrl.string();
				TokIdentifier *prev = curr.identifier;
				curr.identifier = &tok;
				result = handleState53();
				parserCtrl.release(curr.identifier->str);
				curr.identifier = prev;
				break;
			}
		default:
			{
				const int expected[] = { TOK_ws, TOK_nl, TOK_identifier, TOK_command, TOK_lparameter, TOK_parameter, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[command -> lparameter 21.identifier 29 30], (12 35)
//[command -> lparameter 21.identifier 29 list 30], (12 35)
//[command -> lparameter 21.parameter], (12 35)
//[parameter ->.parameter 39 identifier], (12 35 39)
//[parameter ->.identifier], (12 35 39)
//<ignore> [END ->.nl], (ws nl identifier parameter)
//<ignore> [END ->.ws], (ws nl identifier parameter)
int GrammarParser::handleState64()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		case TOK_identifier: 
			{
				TokIdentifier tok;
				tok.str = parserCtrl.string();
				TokIdentifier *prev = curr.identifier;
				curr.identifier = &tok;
				result = handleState71();
				parserCtrl.release(curr.identifier->str);
				curr.identifier = prev;
				break;
			}
		case TOK_parameter: result = handleState72(); break;
		default:
			{
				const int expected[] = { TOK_ws, TOK_nl, TOK_identifier, TOK_parameter, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[parameter -> parameter 39.identifier], (12 21 30 35 39)
//<ignore> [END ->.nl], (ws nl identifier)
//<ignore> [END ->.ws], (ws nl identifier)
int GrammarParser::handleState65()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		case TOK_identifier: 
			{
				TokIdentifier tok;
				tok.str = parserCtrl.string();
				TokIdentifier *prev = curr.identifier;
				curr.identifier = &tok;
				result = handleState73();
				parserCtrl.release(curr.identifier->str);
				curr.identifier = prev;
				break;
			}
		default:
			{
				const int expected[] = { TOK_ws, TOK_nl, TOK_identifier, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[parameter -> identifier.], (30 35 39)
//<ignore> [END ->.nl], (ws nl 30 35 39)
//<ignore> [END ->.ws], (ws nl 30 35 39)
int GrammarParser::handleState66()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		default: result = 1; reduce37(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[command -> identifier 29 30.], (12 35)
//<ignore> [END ->.nl], (ws nl 12 35)
//<ignore> [END ->.ws], (ws nl 12 35)
int GrammarParser::handleState67()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		default: result = 3; reduce28(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[command -> identifier 29 list.30], (12 35)
//[list -> list.35 parameter], (30 35)
//<ignore> [END ->.nl], (ws nl 30 35)
//<ignore> [END ->.ws], (ws nl 30 35)
int GrammarParser::handleState68()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		case TOK_30: result = handleState74(); break;
		case TOK_35: result = handleState75(); break;
		default:
			{
				const int expected[] = { TOK_ws, TOK_nl, TOK_30, TOK_35, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[list -> parameter.], (30 35)
//[parameter -> parameter.39 identifier], (30 35 39)
//<ignore> [END ->.nl], (ws nl 30 35 39)
//<ignore> [END ->.ws], (ws nl 30 35 39)
int GrammarParser::handleState69()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		case TOK_39: result = handleState65(); break;
		default: result = 1; reduce35(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[commandlist -> commandlist 35 command.], (12 35)
//<ignore> [END ->.nl], (ws nl 12 35)
//<ignore> [END ->.ws], (ws nl 12 35)
int GrammarParser::handleState70()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		default: result = 3; reduce27(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[command -> lparameter 21 identifier.29 30], (12 35)
//[command -> lparameter 21 identifier.29 list 30], (12 35)
//[parameter -> identifier.], (12 35 39)
//<ignore> [END ->.nl], (ws nl 12 29 35 39)
//<ignore> [END ->.ws], (ws nl 12 29 35 39)
int GrammarParser::handleState71()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		case TOK_29: result = handleState76(); break;
		default: result = 1; reduce37(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[command -> lparameter 21 parameter.], (12 35)
//[parameter -> parameter.39 identifier], (12 35 39)
//<ignore> [END ->.nl], (ws nl 12 35 39)
//<ignore> [END ->.ws], (ws nl 12 35 39)
int GrammarParser::handleState72()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		case TOK_39: result = handleState65(); break;
		default: result = 3; reduce32(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[parameter -> parameter 39 identifier.], (12 21 30 35 39)
//<ignore> [END ->.nl], (ws nl 12 21 30 35 39)
//<ignore> [END ->.ws], (ws nl 12 21 30 35 39)
int GrammarParser::handleState73()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		default: result = 3; reduce36(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[command -> identifier 29 list 30.], (12 35)
//<ignore> [END ->.nl], (ws nl 12 35)
//<ignore> [END ->.ws], (ws nl 12 35)
int GrammarParser::handleState74()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		default: result = 4; reduce29(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[list -> list 35.parameter], (30 35)
//[parameter ->.parameter 39 identifier], (30 35 39)
//[parameter ->.identifier], (30 35 39)
//<ignore> [END ->.nl], (ws nl identifier parameter)
//<ignore> [END ->.ws], (ws nl identifier parameter)
int GrammarParser::handleState75()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		case TOK_identifier: 
			{
				TokIdentifier tok;
				tok.str = parserCtrl.string();
				TokIdentifier *prev = curr.identifier;
				curr.identifier = &tok;
				result = handleState66();
				parserCtrl.release(curr.identifier->str);
				curr.identifier = prev;
				break;
			}
		case TOK_parameter: result = handleState77(); break;
		default:
			{
				const int expected[] = { TOK_ws, TOK_nl, TOK_identifier, TOK_parameter, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[command -> lparameter 21 identifier 29.30], (12 35)
//[command -> lparameter 21 identifier 29.list 30], (12 35)
//[list ->.list 35 parameter], (30 35)
//[list ->.parameter], (30 35)
//[parameter ->.parameter 39 identifier], (30 35 39)
//[parameter ->.identifier], (30 35 39)
//<ignore> [END ->.nl], (ws nl identifier 30 list parameter)
//<ignore> [END ->.ws], (ws nl identifier 30 list parameter)
int GrammarParser::handleState76()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		case TOK_30: result = handleState78(); break;
		case TOK_list: result = handleState79(); break;
		case TOK_parameter: result = handleState69(); break;
		case TOK_identifier: 
			{
				TokIdentifier tok;
				tok.str = parserCtrl.string();
				TokIdentifier *prev = curr.identifier;
				curr.identifier = &tok;
				result = handleState66();
				parserCtrl.release(curr.identifier->str);
				curr.identifier = prev;
				break;
			}
		default:
			{
				const int expected[] = { TOK_ws, TOK_nl, TOK_identifier, TOK_30, TOK_list, TOK_parameter, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[list -> list 35 parameter.], (30 35)
//[parameter -> parameter.39 identifier], (30 35 39)
//<ignore> [END ->.nl], (ws nl 30 35 39)
//<ignore> [END ->.ws], (ws nl 30 35 39)
int GrammarParser::handleState77()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		case TOK_39: result = handleState65(); break;
		default: result = 3; reduce34(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[command -> lparameter 21 identifier 29 30.], (12 35)
//<ignore> [END ->.nl], (ws nl 12 35)
//<ignore> [END ->.ws], (ws nl 12 35)
int GrammarParser::handleState78()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		default: result = 5; reduce30(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[command -> lparameter 21 identifier 29 list.30], (12 35)
//[list -> list.35 parameter], (30 35)
//<ignore> [END ->.nl], (ws nl 30 35)
//<ignore> [END ->.ws], (ws nl 30 35)
int GrammarParser::handleState79()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		case TOK_30: result = handleState80(); break;
		case TOK_35: result = handleState75(); break;
		default:
			{
				const int expected[] = { TOK_ws, TOK_nl, TOK_30, TOK_35, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[command -> lparameter 21 identifier 29 list 30.], (12 35)
//<ignore> [END ->.nl], (ws nl 12 35)
//<ignore> [END ->.ws], (ws nl 12 35)
int GrammarParser::handleState80()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(0); break;
		case TOK_nl: result = handleState2(0); break;
		default: result = 6; reduce31(); break;
		}
	}
	while(result == 0);
	return --result;
}


