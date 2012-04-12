#include "../GrammarParser.hpp"


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

void GrammarParser::reduce0()
{
	reduce(0, 6);
}
void GrammarParser::reduce1()
{
	reduce(1, 6);
}
void GrammarParser::reduce2()
{
	reduce(2, 7);
	grammar(curr.identifier->str);
}
void GrammarParser::reduce3()
{
	reduce(3, 7);
	regexGroup(curr.identifier->str);
}
void GrammarParser::reduce4()
{
	reduce(4, 10);
}
void GrammarParser::reduce5()
{
	reduce(5, 10);
}
void GrammarParser::reduce6()
{
	reduce(6, 15);
	production(curr.identifier->str);
}
void GrammarParser::reduce7()
{
	reduce(7, 15);
	production(curr.identifier->str);
}
void GrammarParser::reduce8()
{
	reduce(8, 15);
	ignoreProduction();
}
void GrammarParser::reduce9()
{
	reduce(9, 15);
}
void GrammarParser::reduce10()
{
	reduce(10, 19);
	token(curr.identifier->str, curr.string->str);
}
void GrammarParser::reduce11()
{
	reduce(11, 19);
	token(curr.identifier->str, curr.regex->str);
}
void GrammarParser::reduce12()
{
	reduce(12, 16);
}
void GrammarParser::reduce13()
{
	reduce(13, 16);
}
void GrammarParser::reduce14()
{
	reduce(14, 21);
	nonTerminal(curr.identifier->str);
}
void GrammarParser::reduce15()
{
	reduce(15, 21);
	terminal(curr.string->str);
}
void GrammarParser::reduce16()
{
	reduce(16, 21);
	terminal(curr.regex->str);
}
void GrammarParser::reduce17()
{
	reduce(17, 17);
	action();
}
void GrammarParser::reduce18()
{
	reduce(18, 17);
	action();
}
void GrammarParser::reduce19()
{
	reduce(19, 23);
}
void GrammarParser::reduce20()
{
	reduce(20, 23);
}
void GrammarParser::reduce21()
{
	reduce(21, 24);
	function(curr.identifier->str);
}
void GrammarParser::reduce22()
{
	reduce(22, 24);
	function(curr.identifier->str);
}
void GrammarParser::reduce23()
{
	reduce(23, 24);
	function(curr.identifier->str);
}
void GrammarParser::reduce24()
{
	reduce(24, 24);
	function(curr.identifier->str);
}
void GrammarParser::reduce25()
{
	reduce(25, 24);
	assignment();
}
void GrammarParser::reduce26()
{
	reduce(26, 29);
	lparameter();
}
void GrammarParser::reduce27()
{
	reduce(27, 27);
	parameter();
}
void GrammarParser::reduce28()
{
	reduce(28, 27);
	parameter();
}
void GrammarParser::reduce29()
{
	reduce(29, 30);
	concatParameter(curr.identifier->str);
}
void GrammarParser::reduce30()
{
	reduce(30, 30);
	concatParameter(curr.identifier->str);
}
void GrammarParser::reduce31()
{
	ignore(31);
	newLine();
}
void GrammarParser::reduce32()
{
	ignore(32);
}
void GrammarParser::reduce33()
{
	ignore(33);
}

int GrammarParser::handleState0()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(); break;
		case TOK_nl: result = handleState2(); break;
		case TOK_identifier: 
			{
				TokIdentifier tok;
				tok.str = parserCtrl.string();
				TokIdentifier *prev = curr.identifier;
				curr.identifier = &tok;
				result = handleState3();
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

int GrammarParser::handleState1()
{
	(this->next)(0);
	reduce32();
	return 0;
}

int GrammarParser::handleState2()
{
	(this->next)(0);
	reduce31();
	return 0;
}

int GrammarParser::handleState3()
{
	int result = 0;
	switch((this->*next)(0))
	{
	case TOK_ws: result = handleState1(); break;
	case TOK_nl: result = handleState2(); break;
	case TOK_8: result = handleState7(); break;
	default:
		{
			const int expected[] = { TOK_ws, TOK_nl, TOK_8, };
			result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
			break;
		}
	}
	return --result;
}

int GrammarParser::handleState4()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(); break;
		case TOK_nl: result = handleState2(); break;
		case TOK_identifier: 
			{
				TokIdentifier tok;
				tok.str = parserCtrl.string();
				TokIdentifier *prev = curr.identifier;
				curr.identifier = &tok;
				result = handleState3();
				curr.identifier = prev;
				break;
			}
		case TOK_13: result = handleState6(); break;
		case TOK_grammar: result = handleState8(); break;
		default: result = reduce33(); break;
		}
	}
	while(result == 0);
	return --result;
}

int GrammarParser::handleState5()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(); break;
		case TOK_nl: result = handleState2(); break;
		default: result = reduce1(); break;
		}
	}
	while(result == 0);
	return --result;
}

int GrammarParser::handleState6()
{
	int result = 0;
	switch((this->*next)(0))
	{
	case TOK_ws: result = handleState1(); break;
	case TOK_nl: result = handleState2(); break;
	case TOK_identifier: 
		{
			TokIdentifier tok;
			tok.str = parserCtrl.string();
			TokIdentifier *prev = curr.identifier;
			curr.identifier = &tok;
			result = handleState9();
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
	return --result;
}

int GrammarParser::handleState7()
{
	int result = 0;
	switch((this->*next)(0))
	{
	case TOK_ws: result = handleState1(); break;
	case TOK_nl: result = handleState2(); break;
	case TOK_9: result = handleState10(); break;
	default:
		{
			const int expected[] = { TOK_ws, TOK_nl, TOK_9, };
			result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
			break;
		}
	}
	return --result;
}

int GrammarParser::handleState8()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(); break;
		case TOK_nl: result = handleState2(); break;
		default: result = reduce0(); break;
		}
	}
	while(result == 0);
	return --result;
}

int GrammarParser::handleState9()
{
	int result = 0;
	switch((this->*next)(0))
	{
	case TOK_ws: result = handleState1(); break;
	case TOK_nl: result = handleState2(); break;
	case TOK_14: result = handleState11(); break;
	default:
		{
			const int expected[] = { TOK_ws, TOK_nl, TOK_14, };
			result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
			break;
		}
	}
	return --result;
}

int GrammarParser::handleState10()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(); break;
		case TOK_nl: result = handleState2(); break;
		case TOK_definition: result = handleState12(); break;
		default: result = reduce4(); break;
		}
	}
	while(result == 0);
	return --result;
}

int GrammarParser::handleState11()
{
	int result = 0;
	switch((this->*next)(0))
	{
	case TOK_ws: result = handleState1(); break;
	case TOK_nl: result = handleState2(); break;
	case TOK_8: result = handleState13(); break;
	default:
		{
			const int expected[] = { TOK_ws, TOK_nl, TOK_8, };
			result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
			break;
		}
	}
	return --result;
}

int GrammarParser::handleState12()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(); break;
		case TOK_nl: result = handleState2(); break;
		case TOK_11: result = handleState15(); break;
		case TOK_production: result = handleState16(); break;
		case TOK_18: result = handleState17(); break;
		case TOK_tokendef: result = handleState18(); break;
		case TOK_identifier: 
			{
				TokIdentifier tok;
				tok.str = parserCtrl.string();
				TokIdentifier *prev = curr.identifier;
				curr.identifier = &tok;
				result = handleState14();
				curr.identifier = prev;
				break;
			}
		default:
			{
				const int expected[] = { TOK_ws, TOK_nl, TOK_identifier, TOK_11, TOK_production, TOK_18, TOK_tokendef, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

int GrammarParser::handleState13()
{
	int result = 0;
	switch((this->*next)(0))
	{
	case TOK_ws: result = handleState1(); break;
	case TOK_nl: result = handleState2(); break;
	case TOK_regex: 
		{
			TokRegex tok;
			tok.str = parserCtrl.string();
			TokRegex *prev = curr.regex;
			curr.regex = &tok;
			result = handleState19();
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
	return --result;
}

int GrammarParser::handleState14()
{
	int result = 0;
	switch((this->*next)(0))
	{
	case TOK_ws: result = handleState1(); break;
	case TOK_nl: result = handleState2(); break;
	case TOK_8: result = handleState20(); break;
	case TOK_20: result = handleState21(); break;
	default:
		{
			const int expected[] = { TOK_ws, TOK_nl, TOK_8, TOK_20, };
			result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
			break;
		}
	}
	return --result;
}

int GrammarParser::handleState15()
{
	int result = 0;
	switch((this->*next)(0))
	{
	case TOK_ws: result = handleState1(); break;
	case TOK_nl: result = handleState2(); break;
	case TOK_12: result = handleState22(); break;
	default:
		{
			const int expected[] = { TOK_ws, TOK_nl, TOK_12, };
			result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
			break;
		}
	}
	return --result;
}

int GrammarParser::handleState16()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(); break;
		case TOK_nl: result = handleState2(); break;
		default: result = reduce5(); break;
		}
	}
	while(result == 0);
	return --result;
}

int GrammarParser::handleState17()
{
	int result = 0;
	switch((this->*next)(0))
	{
	case TOK_ws: result = handleState1(); break;
	case TOK_nl: result = handleState2(); break;
	case TOK_8: result = handleState23(); break;
	default:
		{
			const int expected[] = { TOK_ws, TOK_nl, TOK_8, };
			result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
			break;
		}
	}
	return --result;
}

int GrammarParser::handleState18()
{
	int result = 0;
	switch((this->*next)(0))
	{
	case TOK_ws: result = handleState1(); break;
	case TOK_nl: result = handleState2(); break;
	case TOK_12: result = handleState24(); break;
	default:
		{
			const int expected[] = { TOK_ws, TOK_nl, TOK_12, };
			result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
			break;
		}
	}
	return --result;
}

int GrammarParser::handleState19()
{
	int result = 0;
	switch((this->*next)(0))
	{
	case TOK_ws: result = handleState1(); break;
	case TOK_nl: result = handleState2(); break;
	case TOK_12: result = handleState25(); break;
	default:
		{
			const int expected[] = { TOK_ws, TOK_nl, TOK_12, };
			result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
			break;
		}
	}
	return --result;
}

int GrammarParser::handleState20()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(); break;
		case TOK_nl: result = handleState2(); break;
		case TOK_string: 
			{
				TokString tok;
				tok.str = parserCtrl.string();
				TokString *prev = curr.string;
				curr.string = &tok;
				result = handleState26();
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
				curr.identifier = prev;
				break;
			}
		case TOK_18: result = handleState30(); break;
		case TOK_body: result = handleState29(); break;
		case TOK_term: result = handleState31(); break;
		default:
			{
				const int expected[] = { TOK_ws, TOK_nl, TOK_string, TOK_regex, TOK_identifier, TOK_body, TOK_18, TOK_term, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

int GrammarParser::handleState21()
{
	int result = 0;
	switch((this->*next)(0))
	{
	case TOK_ws: result = handleState1(); break;
	case TOK_nl: result = handleState2(); break;
	case TOK_string: 
		{
			TokString tok;
			tok.str = parserCtrl.string();
			TokString *prev = curr.string;
			curr.string = &tok;
			result = handleState32();
			curr.string = prev;
			break;
		}
	case TOK_regex: 
		{
			TokRegex tok;
			tok.str = parserCtrl.string();
			TokRegex *prev = curr.regex;
			curr.regex = &tok;
			result = handleState33();
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
	return --result;
}

int GrammarParser::handleState22()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(); break;
		case TOK_nl: result = handleState2(); break;
		default: result = reduce2(); break;
		}
	}
	while(result == 0);
	return --result;
}

int GrammarParser::handleState23()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(); break;
		case TOK_nl: result = handleState2(); break;
		case TOK_string: 
			{
				TokString tok;
				tok.str = parserCtrl.string();
				TokString *prev = curr.string;
				curr.string = &tok;
				result = handleState26();
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
				curr.identifier = prev;
				break;
			}
		case TOK_body: result = handleState34(); break;
		case TOK_term: result = handleState31(); break;
		default:
			{
				const int expected[] = { TOK_ws, TOK_nl, TOK_string, TOK_regex, TOK_identifier, TOK_body, TOK_term, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

int GrammarParser::handleState24()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(); break;
		case TOK_nl: result = handleState2(); break;
		default: result = reduce9(); break;
		}
	}
	while(result == 0);
	return --result;
}

int GrammarParser::handleState25()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(); break;
		case TOK_nl: result = handleState2(); break;
		default: result = reduce3(); break;
		}
	}
	while(result == 0);
	return --result;
}

int GrammarParser::handleState26()
{
	int result = 0;
	switch((this->*next)(1))
	{
	case TOK_ws: result = handleState1(); break;
	case TOK_nl: result = handleState2(); break;
	default: result = reduce15(); break;
	}
	return --result;
}

int GrammarParser::handleState27()
{
	int result = 0;
	switch((this->*next)(1))
	{
	case TOK_ws: result = handleState1(); break;
	case TOK_nl: result = handleState2(); break;
	default: result = reduce16(); break;
	}
	return --result;
}

int GrammarParser::handleState28()
{
	int result = 0;
	switch((this->*next)(1))
	{
	case TOK_ws: result = handleState1(); break;
	case TOK_nl: result = handleState2(); break;
	default: result = reduce14(); break;
	}
	return --result;
}

int GrammarParser::handleState29()
{
	int result = 0;
	do
	{
		switch((this->*next)(1))
		{
		case TOK_ws: result = handleState1(); break;
		case TOK_nl: result = handleState2(); break;
		case TOK_string: 
			{
				TokString tok;
				tok.str = parserCtrl.string();
				TokString *prev = curr.string;
				curr.string = &tok;
				result = handleState26();
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
				curr.identifier = prev;
				break;
			}
		case TOK_term: result = handleState36(); break;
		case TOK_22: result = handleState37(); break;
		case TOK_action: result = handleState35(); break;
		default: result = reduce17(); break;
		}
	}
	while(result == 0);
	return --result;
}

int GrammarParser::handleState30()
{
	int result = 0;
	do
	{
		switch((this->*next)(1))
		{
		case TOK_ws: result = handleState1(); break;
		case TOK_nl: result = handleState2(); break;
		case TOK_action: result = handleState38(); break;
		case TOK_22: result = handleState37(); break;
		default: result = reduce17(); break;
		}
	}
	while(result == 0);
	return --result;
}

int GrammarParser::handleState31()
{
	int result = 0;
	switch((this->*next)(1))
	{
	case TOK_ws: result = handleState1(); break;
	case TOK_nl: result = handleState2(); break;
	default: result = reduce13(); break;
	}
	return --result;
}

int GrammarParser::handleState32()
{
	int result = 0;
	switch((this->*next)(0))
	{
	case TOK_ws: result = handleState1(); break;
	case TOK_nl: result = handleState2(); break;
	default: result = reduce10(); break;
	}
	return --result;
}

int GrammarParser::handleState33()
{
	int result = 0;
	switch((this->*next)(0))
	{
	case TOK_ws: result = handleState1(); break;
	case TOK_nl: result = handleState2(); break;
	default: result = reduce11(); break;
	}
	return --result;
}

int GrammarParser::handleState34()
{
	int result = 0;
	do
	{
		switch((this->*next)(1))
		{
		case TOK_ws: result = handleState1(); break;
		case TOK_nl: result = handleState2(); break;
		case TOK_string: 
			{
				TokString tok;
				tok.str = parserCtrl.string();
				TokString *prev = curr.string;
				curr.string = &tok;
				result = handleState26();
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
				curr.identifier = prev;
				break;
			}
		case TOK_term: result = handleState36(); break;
		case TOK_22: result = handleState37(); break;
		case TOK_action: result = handleState39(); break;
		default: result = reduce17(); break;
		}
	}
	while(result == 0);
	return --result;
}

int GrammarParser::handleState35()
{
	int result = 0;
	switch((this->*next)(0))
	{
	case TOK_ws: result = handleState1(); break;
	case TOK_nl: result = handleState2(); break;
	case TOK_12: result = handleState40(); break;
	default:
		{
			const int expected[] = { TOK_ws, TOK_nl, TOK_12, };
			result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
			break;
		}
	}
	return --result;
}

int GrammarParser::handleState36()
{
	int result = 0;
	switch((this->*next)(1))
	{
	case TOK_ws: result = handleState1(); break;
	case TOK_nl: result = handleState2(); break;
	default: result = reduce12(); break;
	}
	return --result;
}

int GrammarParser::handleState37()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(); break;
		case TOK_nl: result = handleState2(); break;
		case TOK_command: result = handleState43(); break;
		case TOK_lparameter: result = handleState44(); break;
		case TOK_parameter: result = handleState45(); break;
		case TOK_commandlist: result = handleState42(); break;
		case TOK_identifier: 
			{
				TokIdentifier tok;
				tok.str = parserCtrl.string();
				TokIdentifier *prev = curr.identifier;
				curr.identifier = &tok;
				result = handleState41();
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

int GrammarParser::handleState38()
{
	int result = 0;
	switch((this->*next)(0))
	{
	case TOK_ws: result = handleState1(); break;
	case TOK_nl: result = handleState2(); break;
	case TOK_12: result = handleState46(); break;
	default:
		{
			const int expected[] = { TOK_ws, TOK_nl, TOK_12, };
			result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
			break;
		}
	}
	return --result;
}

int GrammarParser::handleState39()
{
	int result = 0;
	switch((this->*next)(0))
	{
	case TOK_ws: result = handleState1(); break;
	case TOK_nl: result = handleState2(); break;
	case TOK_12: result = handleState47(); break;
	default:
		{
			const int expected[] = { TOK_ws, TOK_nl, TOK_12, };
			result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
			break;
		}
	}
	return --result;
}

int GrammarParser::handleState40()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(); break;
		case TOK_nl: result = handleState2(); break;
		default: result = reduce6(); break;
		}
	}
	while(result == 0);
	return --result;
}

int GrammarParser::handleState41()
{
	int result = 0;
	switch((this->*next)(0))
	{
	case TOK_ws: result = handleState1(); break;
	case TOK_nl: result = handleState2(); break;
	case TOK_26: result = handleState48(); break;
	default: result = reduce21(); break;
	case TOK_20:
	case TOK_31: result = reduce30(); break;
	}
	return --result;
}

int GrammarParser::handleState42()
{
	int result = 0;
	switch((this->*next)(0))
	{
	case TOK_ws: result = handleState1(); break;
	case TOK_nl: result = handleState2(); break;
	case TOK_25: result = handleState49(); break;
	default: result = reduce18(); break;
	}
	return --result;
}

int GrammarParser::handleState43()
{
	int result = 0;
	switch((this->*next)(0))
	{
	case TOK_ws: result = handleState1(); break;
	case TOK_nl: result = handleState2(); break;
	default: result = reduce19(); break;
	}
	return --result;
}

int GrammarParser::handleState44()
{
	int result = 0;
	switch((this->*next)(0))
	{
	case TOK_ws: result = handleState1(); break;
	case TOK_nl: result = handleState2(); break;
	case TOK_20: result = handleState50(); break;
	default:
		{
			const int expected[] = { TOK_ws, TOK_nl, TOK_20, };
			result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
			break;
		}
	}
	return --result;
}

int GrammarParser::handleState45()
{
	int result = 0;
	switch((this->*next)(0))
	{
	case TOK_ws: result = handleState1(); break;
	case TOK_nl: result = handleState2(); break;
	case TOK_31: result = handleState51(); break;
	default: result = reduce26(); break;
	}
	return --result;
}

int GrammarParser::handleState46()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(); break;
		case TOK_nl: result = handleState2(); break;
		default: result = reduce7(); break;
		}
	}
	while(result == 0);
	return --result;
}

int GrammarParser::handleState47()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(); break;
		case TOK_nl: result = handleState2(); break;
		default: result = reduce8(); break;
		}
	}
	while(result == 0);
	return --result;
}

int GrammarParser::handleState48()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(); break;
		case TOK_nl: result = handleState2(); break;
		case TOK_list: result = handleState53(); break;
		case TOK_parameter: result = handleState54(); break;
		case TOK_identifier: 
			{
				TokIdentifier tok;
				tok.str = parserCtrl.string();
				TokIdentifier *prev = curr.identifier;
				curr.identifier = &tok;
				result = handleState52();
				curr.identifier = prev;
				break;
			}
		default:
			{
				const int expected[] = { TOK_ws, TOK_nl, TOK_identifier, TOK_list, TOK_parameter, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

int GrammarParser::handleState49()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(); break;
		case TOK_nl: result = handleState2(); break;
		case TOK_command: result = handleState55(); break;
		case TOK_parameter: result = handleState45(); break;
		case TOK_lparameter: result = handleState44(); break;
		case TOK_identifier: 
			{
				TokIdentifier tok;
				tok.str = parserCtrl.string();
				TokIdentifier *prev = curr.identifier;
				curr.identifier = &tok;
				result = handleState41();
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

int GrammarParser::handleState50()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(); break;
		case TOK_nl: result = handleState2(); break;
		case TOK_identifier: 
			{
				TokIdentifier tok;
				tok.str = parserCtrl.string();
				TokIdentifier *prev = curr.identifier;
				curr.identifier = &tok;
				result = handleState56();
				curr.identifier = prev;
				break;
			}
		case TOK_parameter: result = handleState57(); break;
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

int GrammarParser::handleState51()
{
	int result = 0;
	switch((this->*next)(0))
	{
	case TOK_ws: result = handleState1(); break;
	case TOK_nl: result = handleState2(); break;
	case TOK_identifier: 
		{
			TokIdentifier tok;
			tok.str = parserCtrl.string();
			TokIdentifier *prev = curr.identifier;
			curr.identifier = &tok;
			result = handleState58();
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
	return --result;
}

int GrammarParser::handleState52()
{
	int result = 0;
	switch((this->*next)(0))
	{
	case TOK_ws: result = handleState1(); break;
	case TOK_nl: result = handleState2(); break;
	default: result = reduce30(); break;
	}
	return --result;
}

int GrammarParser::handleState53()
{
	int result = 0;
	switch((this->*next)(0))
	{
	case TOK_ws: result = handleState1(); break;
	case TOK_nl: result = handleState2(); break;
	case TOK_25: result = handleState59(); break;
	case TOK_28: result = handleState60(); break;
	default:
		{
			const int expected[] = { TOK_ws, TOK_nl, TOK_25, TOK_28, };
			result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
			break;
		}
	}
	return --result;
}

int GrammarParser::handleState54()
{
	int result = 0;
	switch((this->*next)(0))
	{
	case TOK_ws: result = handleState1(); break;
	case TOK_nl: result = handleState2(); break;
	case TOK_31: result = handleState51(); break;
	default: result = reduce28(); break;
	}
	return --result;
}

int GrammarParser::handleState55()
{
	int result = 0;
	switch((this->*next)(0))
	{
	case TOK_ws: result = handleState1(); break;
	case TOK_nl: result = handleState2(); break;
	default: result = reduce20(); break;
	}
	return --result;
}

int GrammarParser::handleState56()
{
	int result = 0;
	switch((this->*next)(0))
	{
	case TOK_ws: result = handleState1(); break;
	case TOK_nl: result = handleState2(); break;
	case TOK_26: result = handleState61(); break;
	default: result = reduce30(); break;
	}
	return --result;
}

int GrammarParser::handleState57()
{
	int result = 0;
	switch((this->*next)(0))
	{
	case TOK_ws: result = handleState1(); break;
	case TOK_nl: result = handleState2(); break;
	case TOK_31: result = handleState51(); break;
	default: result = reduce25(); break;
	}
	return --result;
}

int GrammarParser::handleState58()
{
	int result = 0;
	switch((this->*next)(0))
	{
	case TOK_ws: result = handleState1(); break;
	case TOK_nl: result = handleState2(); break;
	default: result = reduce29(); break;
	}
	return --result;
}

int GrammarParser::handleState59()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(); break;
		case TOK_nl: result = handleState2(); break;
		case TOK_identifier: 
			{
				TokIdentifier tok;
				tok.str = parserCtrl.string();
				TokIdentifier *prev = curr.identifier;
				curr.identifier = &tok;
				result = handleState52();
				curr.identifier = prev;
				break;
			}
		case TOK_parameter: result = handleState62(); break;
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

int GrammarParser::handleState60()
{
	int result = 0;
	switch((this->*next)(0))
	{
	case TOK_ws: result = handleState1(); break;
	case TOK_nl: result = handleState2(); break;
	default: result = reduce22(); break;
	}
	return --result;
}

int GrammarParser::handleState61()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_ws: result = handleState1(); break;
		case TOK_nl: result = handleState2(); break;
		case TOK_list: result = handleState63(); break;
		case TOK_28: result = handleState64(); break;
		case TOK_parameter: result = handleState54(); break;
		case TOK_identifier: 
			{
				TokIdentifier tok;
				tok.str = parserCtrl.string();
				TokIdentifier *prev = curr.identifier;
				curr.identifier = &tok;
				result = handleState52();
				curr.identifier = prev;
				break;
			}
		default:
			{
				const int expected[] = { TOK_ws, TOK_nl, TOK_identifier, TOK_list, TOK_28, TOK_parameter, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

int GrammarParser::handleState62()
{
	int result = 0;
	switch((this->*next)(0))
	{
	case TOK_ws: result = handleState1(); break;
	case TOK_nl: result = handleState2(); break;
	case TOK_31: result = handleState51(); break;
	default: result = reduce27(); break;
	}
	return --result;
}

int GrammarParser::handleState63()
{
	int result = 0;
	switch((this->*next)(0))
	{
	case TOK_ws: result = handleState1(); break;
	case TOK_nl: result = handleState2(); break;
	case TOK_25: result = handleState59(); break;
	case TOK_28: result = handleState65(); break;
	default:
		{
			const int expected[] = { TOK_ws, TOK_nl, TOK_25, TOK_28, };
			result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
			break;
		}
	}
	return --result;
}

int GrammarParser::handleState64()
{
	int result = 0;
	switch((this->*next)(0))
	{
	case TOK_ws: result = handleState1(); break;
	case TOK_nl: result = handleState2(); break;
	default: result = reduce23(); break;
	}
	return --result;
}

int GrammarParser::handleState65()
{
	int result = 0;
	switch((this->*next)(0))
	{
	case TOK_ws: result = handleState1(); break;
	case TOK_nl: result = handleState2(); break;
	default: result = reduce24(); break;
	}
	return --result;
}


