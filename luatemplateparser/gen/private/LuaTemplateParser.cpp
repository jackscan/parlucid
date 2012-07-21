/*
 * Copyright 2004-2012 Mathias Fiedler. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "../LuaTemplateParser.hpp"
#include "../LuaTemplateTokens.hpp"
#include "../../src/LuaTemplateParserCtrl.hpp"

using namespace LuaTemplateTokens;

LuaTemplateParser::LuaTemplateParser(LuaTemplateParserCtrl &ctrl) : parserCtrl(ctrl), next(&LuaTemplateParser::shift), token(0), symbol(0)
{
	curr.spaces = 0; // 1
	curr.name = 0; // 2
	curr.chunk = 0; // 8
	curr.code = 0; // 6
	curr.snippet = 0; // 9
	curr.filename = 0; // 10
}

void LuaTemplateParser::parse()
{
	handleState0();
}

int LuaTemplateParser::shift(int context)
{
	return token = parserCtrl.shift(context);
}

int LuaTemplateParser::push(int)
{
	next = &LuaTemplateParser::recall;
	return symbol;
}

int LuaTemplateParser::recall(int)
{
	next = &LuaTemplateParser::shift;
	return token;
}

void LuaTemplateParser::reduce(int production, int sym)
{
	symbol = sym;
	next = &LuaTemplateParser::push;
	parserCtrl.reduce(production);
}

void LuaTemplateParser::ignore(int production)
{
	next = &LuaTemplateParser::recall;
	parserCtrl.reduce(production);
}

// template -> template codeline
void LuaTemplateParser::reduce0()
{
	reduce(0, TOK_template);
}
// template -> codeline
void LuaTemplateParser::reduce1()
{
	reduce(1, TOK_template);
}
// term -> term textintro text 17
void LuaTemplateParser::reduce2()
{
	reduce(2, TOK_term);
}
// term -> term code
void LuaTemplateParser::reduce3()
{
	reduce(3, TOK_term);
	parserCtrl.code(curr.code->str);
}
// term ->
void LuaTemplateParser::reduce4()
{
	reduce(4, TOK_term);
}
// embedtextbegin -> chunks nl
void LuaTemplateParser::reduce5()
{
	reduce(5, TOK_embedtextbegin);
	parserCtrl.embedtextbegin();
}
// embedtext -> chunks
void LuaTemplateParser::reduce6()
{
	reduce(6, TOK_embedtext);
}
// embedtext -> embedtextbegin textlines lastline
void LuaTemplateParser::reduce7()
{
	reduce(7, TOK_embedtext);
	parserCtrl.embedtextend();
}
// embedif -> 29 snippet 30
void LuaTemplateParser::reduce8()
{
	reduce(8, TOK_embedif);
	parserCtrl.embedif(curr.snippet->str);
}
// embedelse -> 36
void LuaTemplateParser::reduce9()
{
	reduce(9, TOK_embedelse);
	parserCtrl.embedelse();
}
// includefile -> filename
void LuaTemplateParser::reduce10()
{
	reduce(10, TOK_includefile);
	parserCtrl.include(curr.filename->str);
}
// codeindent -> spaces
void LuaTemplateParser::reduce11()
{
	reduce(11, TOK_codeindent);
	parserCtrl.codeindent(curr.spaces->str);
}
// codeindent ->
void LuaTemplateParser::reduce12()
{
	reduce(12, TOK_codeindent);
	parserCtrl.codeindent();
}
// text -> textbegin textlines lastline
void LuaTemplateParser::reduce13()
{
	reduce(13, TOK_text);
	parserCtrl.textend();
}
// text -> chunks
void LuaTemplateParser::reduce14()
{
	reduce(14, TOK_text);
}
// textbegin -> chunks nl
void LuaTemplateParser::reduce15()
{
	reduce(15, TOK_textbegin);
	parserCtrl.textbegin();
}
// textlines -> textlines textline
void LuaTemplateParser::reduce16()
{
	reduce(16, TOK_textlines);
}
// textlines ->
void LuaTemplateParser::reduce17()
{
	reduce(17, TOK_textlines);
}
// textline -> textindent nl
void LuaTemplateParser::reduce18()
{
	reduce(18, TOK_textline);
	parserCtrl.textnl();
}
// textline -> textindent chunks realchunk nl
void LuaTemplateParser::reduce19()
{
	reduce(19, TOK_textline);
	parserCtrl.textnl();
}
// textline -> textindent chunks weakchunk nl
void LuaTemplateParser::reduce20()
{
	reduce(20, TOK_textline);
	parserCtrl.weaknl();
}
// lastline -> textindent chunks
void LuaTemplateParser::reduce21()
{
	reduce(21, TOK_lastline);
}
// chunks ->
void LuaTemplateParser::reduce22()
{
	reduce(22, TOK_chunks);
}
// realchunk -> quote
void LuaTemplateParser::reduce23()
{
	reduce(23, TOK_realchunk);
	parserCtrl.quote();
}
// realchunk -> chunk
void LuaTemplateParser::reduce24()
{
	reduce(24, TOK_realchunk);
	parserCtrl.chunk(curr.chunk->str);
}
// weakchunk -> condembed
void LuaTemplateParser::reduce25()
{
	reduce(25, TOK_weakchunk);
}
// textindent -> spaces
void LuaTemplateParser::reduce26()
{
	reduce(26, TOK_textindent);
	parserCtrl.textindent(curr.spaces->str);
}
// textindent ->
void LuaTemplateParser::reduce27()
{
	reduce(27, TOK_textindent);
	parserCtrl.textindent();
}
// codeline -> codeindent term nl
void LuaTemplateParser::reduce28()
{
	reduce(28, TOK_codeline);
	parserCtrl.codenl();
}
// codeline -> codeindent term eof
void LuaTemplateParser::reduce29()
{
	reduce(29, TOK_codeline);
	parserCtrl.codenl();
}
// weakchunk -> 18 name
void LuaTemplateParser::reduce30()
{
	reduce(30, TOK_weakchunk);
	parserCtrl.variable(curr.name->str);
}
// condembed -> embedif embedtext embedelse embedtext 27
void LuaTemplateParser::reduce31()
{
	reduce(31, TOK_condembed);
	parserCtrl.embedend();
}
// condembed -> embedif embedtext 27
void LuaTemplateParser::reduce32()
{
	reduce(32, TOK_condembed);
	parserCtrl.embedend();
}
// weakchunk -> 18 31 snippet 32
void LuaTemplateParser::reduce33()
{
	reduce(33, TOK_weakchunk);
	parserCtrl.snippet(curr.snippet->str);
}
// weakchunk -> 29 snippet 32
void LuaTemplateParser::reduce34()
{
	reduce(34, TOK_weakchunk);
	parserCtrl.embed(curr.snippet->str);
}
// weakchunk -> 37 includefile quote
void LuaTemplateParser::reduce35()
{
	reduce(35, TOK_weakchunk);
}
// chunks -> chunks realchunk
void LuaTemplateParser::reduce36()
{
	reduce(36, TOK_chunks);
}
// chunks -> chunks weakchunk
void LuaTemplateParser::reduce37()
{
	reduce(37, TOK_chunks);
}
// END -> END template
void LuaTemplateParser::reduce38()
{
	ignore(38);
}

//[template ->.template codeline], (END spaces nl eof textintro code codeindent term)
//[template ->.codeline], (END spaces nl eof textintro code codeindent term)
//[codeindent ->.spaces], (nl eof textintro code term)
//[codeindent ->.], (nl eof textintro code term)
//[codeline ->.codeindent term nl], (END spaces nl eof textintro code codeindent term)
//[codeline ->.codeindent term eof], (END spaces nl eof textintro code codeindent term)
//[END -> END.template], (END)
int LuaTemplateParser::handleState0()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_spaces: 
			{
				TokSpaces tok;
				tok.str = parserCtrl.string();
				TokSpaces *prev = curr.spaces;
				curr.spaces = &tok;
				result = handleState1();
				parserCtrl.release(curr.spaces->str);
				curr.spaces = prev;
				break;
			}
		case TOK_template: result = handleState2(); break;
		case TOK_codeindent: result = handleState4(); break;
		case TOK_codeline: result = handleState3(); break;
		case TOK_nl:
		case TOK_eof:
		case TOK_textintro:
		case TOK_code:
		case TOK_term: result = 0; reduce12(); break;
		default:
			{
				const int expected[] = { TOK_spaces, TOK_nl, TOK_eof, TOK_textintro, TOK_code, TOK_template, TOK_codeline, TOK_codeindent, TOK_term, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[codeindent -> spaces.], (nl eof textintro code term)
int LuaTemplateParser::handleState1()
{
	(this->*next)(1);
	reduce11();
	return 0;
}

//[template -> template.codeline], (END spaces nl eof textintro code codeindent term)
//[codeindent ->.spaces], (nl eof textintro code term)
//[codeindent ->.], (nl eof textintro code term)
//[codeline ->.codeindent term nl], (END spaces nl eof textintro code codeindent term)
//[codeline ->.codeindent term eof], (END spaces nl eof textintro code codeindent term)
//[END -> END template.], (END)
int LuaTemplateParser::handleState2()
{
	int result = 0;
	do
	{
		switch((this->*next)(0))
		{
		case TOK_spaces: 
			{
				TokSpaces tok;
				tok.str = parserCtrl.string();
				TokSpaces *prev = curr.spaces;
				curr.spaces = &tok;
				result = handleState1();
				parserCtrl.release(curr.spaces->str);
				curr.spaces = prev;
				break;
			}
		case TOK_codeline: result = handleState5(); break;
		case TOK_codeindent: result = handleState4(); break;
		case TOK_nl:
		case TOK_eof:
		case TOK_textintro:
		case TOK_code:
		case TOK_term: result = 0; reduce12(); break;
		case TOK_END: result = 2; reduce38(); break;
		default:
			{
				const int expected[] = { TOK_END, TOK_spaces, TOK_nl, TOK_eof, TOK_textintro, TOK_code, TOK_codeline, TOK_codeindent, TOK_term, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[template -> codeline.], (END spaces nl eof textintro code codeindent term)
int LuaTemplateParser::handleState3()
{
	(this->*next)(0);
	reduce1();
	return 0;
}

//[term ->.term textintro text 17], (nl eof textintro code)
//[term ->.term code], (nl eof textintro code)
//[term ->.], (nl eof textintro code)
//[codeline -> codeindent.term nl], (END spaces nl eof textintro code codeindent term)
//[codeline -> codeindent.term eof], (END spaces nl eof textintro code codeindent term)
int LuaTemplateParser::handleState4()
{
	int result = 0;
	do
	{
		switch((this->*next)(1))
		{
		case TOK_term: result = handleState6(); break;
		case TOK_nl:
		case TOK_eof:
		case TOK_textintro:
		case TOK_code: result = 0; reduce4(); break;
		default:
			{
				const int expected[] = { TOK_nl, TOK_eof, TOK_textintro, TOK_code, TOK_term, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[template -> template codeline.], (END spaces nl eof textintro code codeindent term)
int LuaTemplateParser::handleState5()
{
	(this->*next)(0);
	reduce0();
	return 1;
}

//[term -> term.textintro text 17], (nl eof textintro code)
//[term -> term.code], (nl eof textintro code)
//[codeline -> codeindent term.nl], (END spaces nl eof textintro code codeindent term)
//[codeline -> codeindent term.eof], (END spaces nl eof textintro code codeindent term)
int LuaTemplateParser::handleState6()
{
	int result = 0;
	do
	{
		switch((this->*next)(1))
		{
		case TOK_nl: result = handleState7(); break;
		case TOK_code: 
			{
				TokCode tok;
				tok.str = parserCtrl.string();
				TokCode *prev = curr.code;
				curr.code = &tok;
				result = handleState10();
				parserCtrl.release(curr.code->str);
				curr.code = prev;
				break;
			}
		case TOK_eof: result = handleState8(); break;
		case TOK_textintro: result = handleState9(); break;
		default:
			{
				const int expected[] = { TOK_nl, TOK_eof, TOK_textintro, TOK_code, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[codeline -> codeindent term nl.], (END spaces nl eof textintro code codeindent term)
int LuaTemplateParser::handleState7()
{
	(this->*next)(0);
	reduce28();
	return 2;
}

//[codeline -> codeindent term eof.], (END spaces nl eof textintro code codeindent term)
int LuaTemplateParser::handleState8()
{
	(this->*next)(0);
	reduce29();
	return 2;
}

//[term -> term textintro.text 17], (nl eof textintro code)
//[text ->.textbegin textlines lastline], (17)
//[text ->.chunks], (17)
//[textbegin ->.chunks nl], (spaces nl quote chunk 17 18 chunks textlines embedif condembed 29 37 textline textindent realchunk weakchunk)
//[chunks ->.], (nl quote chunk 17 18 embedif condembed 29 37)
//[chunks ->.chunks realchunk], (nl quote chunk 17 18 embedif condembed 29 37)
//[chunks ->.chunks weakchunk], (nl quote chunk 17 18 embedif condembed 29 37)
int LuaTemplateParser::handleState9()
{
	int result = 0;
	do
	{
		switch((this->*next)(2))
		{
		case TOK_textbegin: result = handleState13(); break;
		case TOK_text: result = handleState11(); break;
		case TOK_chunks: result = handleState12(); break;
		case TOK_nl:
		case TOK_quote:
		case TOK_chunk:
		case TOK_17:
		case TOK_18:
		case TOK_embedif:
		case TOK_condembed:
		case TOK_29:
		case TOK_37: result = 0; reduce22(); break;
		default:
			{
				const int expected[] = { TOK_nl, TOK_quote, TOK_chunk, TOK_text, TOK_17, TOK_18, TOK_chunks, TOK_embedif, TOK_condembed, TOK_29, TOK_37, TOK_textbegin, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[term -> term code.], (nl eof textintro code)
int LuaTemplateParser::handleState10()
{
	(this->*next)(1);
	reduce3();
	return 1;
}

//[term -> term textintro text.17], (nl eof textintro code)
int LuaTemplateParser::handleState11()
{
	int result = 0;
	do
	{
		switch((this->*next)(2))
		{
		case TOK_17: result = handleState14(); break;
		default:
			{
				const int expected[] = { TOK_17, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[embedif ->.29 snippet 30], (nl quote chunk 18 chunks embedtextbegin embedif 27 condembed 29 36 37 realchunk weakchunk)
//[text -> chunks.], (17)
//[textbegin -> chunks.nl], (spaces nl quote chunk 17 18 chunks textlines embedif condembed 29 37 textline textindent realchunk weakchunk)
//[realchunk ->.quote], (nl quote chunk 17 18 embedif condembed 29 37)
//[realchunk ->.chunk], (nl quote chunk 17 18 embedif condembed 29 37)
//[weakchunk ->.condembed], (nl quote chunk 17 18 embedif condembed 29 37)
//[weakchunk ->.18 name], (nl quote chunk 17 18 embedif condembed 29 37)
//[condembed ->.embedif embedtext embedelse embedtext 27], (nl quote chunk 17 18 embedif condembed 29 37)
//[condembed ->.embedif embedtext 27], (nl quote chunk 17 18 embedif condembed 29 37)
//[weakchunk ->.18 31 snippet 32], (nl quote chunk 17 18 embedif condembed 29 37)
//[weakchunk ->.29 snippet 32], (nl quote chunk 17 18 embedif condembed 29 37)
//[weakchunk ->.37 includefile quote], (nl quote chunk 17 18 embedif condembed 29 37)
//[chunks -> chunks.realchunk], (nl quote chunk 17 18 embedif condembed 29 37)
//[chunks -> chunks.weakchunk], (nl quote chunk 17 18 embedif condembed 29 37)
int LuaTemplateParser::handleState12()
{
	int result = 0;
	do
	{
		switch((this->*next)(2))
		{
		case TOK_embedif: result = handleState19(); break;
		case TOK_quote: result = handleState16(); break;
		case TOK_37: result = handleState22(); break;
		case TOK_condembed: result = handleState20(); break;
		case TOK_29: result = handleState21(); break;
		case TOK_chunk: 
			{
				TokChunk tok;
				tok.str = parserCtrl.string();
				TokChunk *prev = curr.chunk;
				curr.chunk = &tok;
				result = handleState17();
				parserCtrl.release(curr.chunk->str);
				curr.chunk = prev;
				break;
			}
		case TOK_18: result = handleState18(); break;
		case TOK_nl: result = handleState15(); break;
		case TOK_weakchunk: result = handleState24(); break;
		case TOK_realchunk: result = handleState23(); break;
		default: result = 1; reduce14(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[text -> textbegin.textlines lastline], (17)
//[textlines ->.textlines textline], (spaces nl quote chunk 17 18 chunks embedif condembed 29 37 textindent realchunk weakchunk)
//[textlines ->.], (spaces nl quote chunk 17 18 chunks embedif condembed 29 37 textindent realchunk weakchunk)
int LuaTemplateParser::handleState13()
{
	int result = 0;
	do
	{
		switch((this->*next)(3))
		{
		case TOK_textlines: result = handleState25(); break;
		case TOK_spaces:
		case TOK_nl:
		case TOK_quote:
		case TOK_chunk:
		case TOK_17:
		case TOK_18:
		case TOK_chunks:
		case TOK_embedif:
		case TOK_condembed:
		case TOK_29:
		case TOK_37:
		case TOK_textindent:
		case TOK_realchunk:
		case TOK_weakchunk: result = 0; reduce17(); break;
		default:
			{
				const int expected[] = { TOK_spaces, TOK_nl, TOK_quote, TOK_chunk, TOK_17, TOK_18, TOK_chunks, TOK_textlines, TOK_embedif, TOK_condembed, TOK_29, TOK_37, TOK_textindent, TOK_realchunk, TOK_weakchunk, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[term -> term textintro text 17.], (nl eof textintro code)
int LuaTemplateParser::handleState14()
{
	(this->*next)(1);
	reduce2();
	return 3;
}

//[textbegin -> chunks nl.], (spaces nl quote chunk 17 18 chunks textlines embedif condembed 29 37 textline textindent realchunk weakchunk)
int LuaTemplateParser::handleState15()
{
	(this->*next)(3);
	reduce15();
	return 1;
}

//[realchunk -> quote.], (nl quote chunk 17 18 embedif 27 condembed 29 36 37)
int LuaTemplateParser::handleState16()
{
	(this->*next)(2);
	reduce23();
	return 0;
}

//[realchunk -> chunk.], (nl quote chunk 17 18 embedif 27 condembed 29 36 37)
int LuaTemplateParser::handleState17()
{
	(this->*next)(2);
	reduce24();
	return 0;
}

//[weakchunk -> 18.name], (nl quote chunk 17 18 embedif 27 condembed 29 36 37)
//[weakchunk -> 18.31 snippet 32], (nl quote chunk 17 18 embedif 27 condembed 29 36 37)
int LuaTemplateParser::handleState18()
{
	int result = 0;
	do
	{
		switch((this->*next)(4))
		{
		case TOK_name: 
			{
				TokName tok;
				tok.str = parserCtrl.string();
				TokName *prev = curr.name;
				curr.name = &tok;
				result = handleState26();
				parserCtrl.release(curr.name->str);
				curr.name = prev;
				break;
			}
		case TOK_31: result = handleState27(); break;
		default:
			{
				const int expected[] = { TOK_name, TOK_31, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[embedtextbegin ->.chunks nl], (spaces nl quote chunk 18 chunks textlines embedif condembed 29 36 37 textline textindent realchunk weakchunk)
//[embedtext ->.chunks], (27 36)
//[embedtext ->.embedtextbegin textlines lastline], (27 36)
//[chunks ->.], (nl quote chunk 18 embedif condembed 29 36 37)
//[condembed -> embedif.embedtext embedelse embedtext 27], (nl quote chunk 17 18 embedif 27 condembed 29 36 37)
//[condembed -> embedif.embedtext 27], (nl quote chunk 17 18 embedif 27 condembed 29 36 37)
//[chunks ->.chunks realchunk], (nl quote chunk 18 embedif condembed 29 36 37)
//[chunks ->.chunks weakchunk], (nl quote chunk 18 embedif condembed 29 36 37)
int LuaTemplateParser::handleState19()
{
	int result = 0;
	do
	{
		switch((this->*next)(2))
		{
		case TOK_embedtextbegin: result = handleState29(); break;
		case TOK_embedtext: result = handleState30(); break;
		case TOK_chunks: result = handleState28(); break;
		case TOK_nl:
		case TOK_quote:
		case TOK_chunk:
		case TOK_18:
		case TOK_embedif:
		case TOK_condembed:
		case TOK_29:
		case TOK_36:
		case TOK_37: result = 0; reduce22(); break;
		default:
			{
				const int expected[] = { TOK_nl, TOK_quote, TOK_chunk, TOK_18, TOK_chunks, TOK_embedtextbegin, TOK_embedtext, TOK_embedif, TOK_condembed, TOK_29, TOK_36, TOK_37, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[weakchunk -> condembed.], (nl quote chunk 17 18 embedif 27 condembed 29 36 37)
int LuaTemplateParser::handleState20()
{
	(this->*next)(2);
	reduce25();
	return 0;
}

//[embedif -> 29.snippet 30], (nl quote chunk 18 chunks embedtextbegin embedif 27 condembed 29 36 37 realchunk weakchunk)
//[weakchunk -> 29.snippet 32], (nl quote chunk 17 18 embedif 27 condembed 29 36 37)
int LuaTemplateParser::handleState21()
{
	int result = 0;
	do
	{
		switch((this->*next)(5))
		{
		case TOK_snippet: 
			{
				TokSnippet tok;
				tok.str = parserCtrl.string();
				TokSnippet *prev = curr.snippet;
				curr.snippet = &tok;
				result = handleState31();
				parserCtrl.release(curr.snippet->str);
				curr.snippet = prev;
				break;
			}
		default:
			{
				const int expected[] = { TOK_snippet, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[includefile ->.filename], (quote)
//[weakchunk -> 37.includefile quote], (nl quote chunk 17 18 embedif 27 condembed 29 36 37)
int LuaTemplateParser::handleState22()
{
	int result = 0;
	do
	{
		switch((this->*next)(6))
		{
		case TOK_filename: 
			{
				TokFilename tok;
				tok.str = parserCtrl.string();
				TokFilename *prev = curr.filename;
				curr.filename = &tok;
				result = handleState32();
				parserCtrl.release(curr.filename->str);
				curr.filename = prev;
				break;
			}
		case TOK_includefile: result = handleState33(); break;
		default:
			{
				const int expected[] = { TOK_filename, TOK_includefile, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[chunks -> chunks realchunk.], (nl quote chunk 17 18 embedif 27 condembed 29 36 37)
int LuaTemplateParser::handleState23()
{
	(this->*next)(2);
	reduce36();
	return 1;
}

//[chunks -> chunks weakchunk.], (nl quote chunk 17 18 embedif 27 condembed 29 36 37)
int LuaTemplateParser::handleState24()
{
	(this->*next)(2);
	reduce37();
	return 1;
}

//[text -> textbegin textlines.lastline], (17)
//[textlines -> textlines.textline], (spaces nl quote chunk 17 18 chunks embedif condembed 29 37 textindent realchunk weakchunk)
//[textline ->.textindent nl], (spaces nl quote chunk 17 18 chunks embedif condembed 29 37 textindent realchunk weakchunk)
//[textline ->.textindent chunks realchunk nl], (spaces nl quote chunk 17 18 chunks embedif condembed 29 37 textindent realchunk weakchunk)
//[textline ->.textindent chunks weakchunk nl], (spaces nl quote chunk 17 18 chunks embedif condembed 29 37 textindent realchunk weakchunk)
//[lastline ->.textindent chunks], (17)
//[textindent ->.spaces], (nl quote chunk 17 18 chunks embedif condembed 29 37 realchunk weakchunk)
//[textindent ->.], (nl quote chunk 17 18 chunks embedif condembed 29 37 realchunk weakchunk)
int LuaTemplateParser::handleState25()
{
	int result = 0;
	do
	{
		switch((this->*next)(3))
		{
		case TOK_spaces: 
			{
				TokSpaces tok;
				tok.str = parserCtrl.string();
				TokSpaces *prev = curr.spaces;
				curr.spaces = &tok;
				result = handleState34();
				parserCtrl.release(curr.spaces->str);
				curr.spaces = prev;
				break;
			}
		case TOK_lastline: result = handleState35(); break;
		case TOK_textindent: result = handleState37(); break;
		case TOK_textline: result = handleState36(); break;
		case TOK_nl:
		case TOK_quote:
		case TOK_chunk:
		case TOK_17:
		case TOK_18:
		case TOK_chunks:
		case TOK_embedif:
		case TOK_condembed:
		case TOK_29:
		case TOK_37:
		case TOK_realchunk:
		case TOK_weakchunk: result = 0; reduce27(); break;
		default:
			{
				const int expected[] = { TOK_spaces, TOK_nl, TOK_quote, TOK_chunk, TOK_17, TOK_18, TOK_chunks, TOK_lastline, TOK_embedif, TOK_condembed, TOK_29, TOK_37, TOK_textline, TOK_textindent, TOK_realchunk, TOK_weakchunk, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[weakchunk -> 18 name.], (nl quote chunk 17 18 embedif 27 condembed 29 36 37)
int LuaTemplateParser::handleState26()
{
	(this->*next)(2);
	reduce30();
	return 1;
}

//[weakchunk -> 18 31.snippet 32], (nl quote chunk 17 18 embedif 27 condembed 29 36 37)
int LuaTemplateParser::handleState27()
{
	int result = 0;
	do
	{
		switch((this->*next)(5))
		{
		case TOK_snippet: 
			{
				TokSnippet tok;
				tok.str = parserCtrl.string();
				TokSnippet *prev = curr.snippet;
				curr.snippet = &tok;
				result = handleState38();
				parserCtrl.release(curr.snippet->str);
				curr.snippet = prev;
				break;
			}
		default:
			{
				const int expected[] = { TOK_snippet, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[embedtextbegin -> chunks.nl], (spaces nl quote chunk 18 chunks textlines embedif 27 condembed 29 36 37 textline textindent realchunk weakchunk)
//[embedtext -> chunks.], (27 36)
//[embedif ->.29 snippet 30], (nl quote chunk 18 chunks embedtextbegin embedif 27 condembed 29 36 37 realchunk weakchunk)
//[realchunk ->.quote], (nl quote chunk 18 embedif 27 condembed 29 36 37)
//[realchunk ->.chunk], (nl quote chunk 18 embedif 27 condembed 29 36 37)
//[weakchunk ->.condembed], (nl quote chunk 18 embedif 27 condembed 29 36 37)
//[weakchunk ->.18 name], (nl quote chunk 18 embedif 27 condembed 29 36 37)
//[condembed ->.embedif embedtext embedelse embedtext 27], (nl quote chunk 18 embedif 27 condembed 29 36 37)
//[condembed ->.embedif embedtext 27], (nl quote chunk 18 embedif 27 condembed 29 36 37)
//[weakchunk ->.18 31 snippet 32], (nl quote chunk 18 embedif 27 condembed 29 36 37)
//[weakchunk ->.29 snippet 32], (nl quote chunk 18 embedif 27 condembed 29 36 37)
//[weakchunk ->.37 includefile quote], (nl quote chunk 18 embedif 27 condembed 29 36 37)
//[chunks -> chunks.realchunk], (nl quote chunk 18 embedif 27 condembed 29 36 37)
//[chunks -> chunks.weakchunk], (nl quote chunk 18 embedif 27 condembed 29 36 37)
int LuaTemplateParser::handleState28()
{
	int result = 0;
	do
	{
		switch((this->*next)(2))
		{
		case TOK_embedif: result = handleState19(); break;
		case TOK_quote: result = handleState16(); break;
		case TOK_37: result = handleState22(); break;
		case TOK_condembed: result = handleState20(); break;
		case TOK_29: result = handleState21(); break;
		case TOK_chunk: 
			{
				TokChunk tok;
				tok.str = parserCtrl.string();
				TokChunk *prev = curr.chunk;
				curr.chunk = &tok;
				result = handleState17();
				parserCtrl.release(curr.chunk->str);
				curr.chunk = prev;
				break;
			}
		case TOK_18: result = handleState18(); break;
		case TOK_nl: result = handleState39(); break;
		case TOK_weakchunk: result = handleState24(); break;
		case TOK_realchunk: result = handleState23(); break;
		default: result = 1; reduce6(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[embedtext -> embedtextbegin.textlines lastline], (27 36)
//[textlines ->.textlines textline], (spaces nl quote chunk 18 chunks embedif 27 condembed 29 36 37 textindent realchunk weakchunk)
//[textlines ->.], (spaces nl quote chunk 18 chunks embedif 27 condembed 29 36 37 textindent realchunk weakchunk)
int LuaTemplateParser::handleState29()
{
	int result = 0;
	do
	{
		switch((this->*next)(3))
		{
		case TOK_textlines: result = handleState40(); break;
		case TOK_spaces:
		case TOK_nl:
		case TOK_quote:
		case TOK_chunk:
		case TOK_18:
		case TOK_chunks:
		case TOK_embedif:
		case TOK_27:
		case TOK_condembed:
		case TOK_29:
		case TOK_36:
		case TOK_37:
		case TOK_textindent:
		case TOK_realchunk:
		case TOK_weakchunk: result = 0; reduce17(); break;
		default:
			{
				const int expected[] = { TOK_spaces, TOK_nl, TOK_quote, TOK_chunk, TOK_18, TOK_chunks, TOK_textlines, TOK_embedif, TOK_27, TOK_condembed, TOK_29, TOK_36, TOK_37, TOK_textindent, TOK_realchunk, TOK_weakchunk, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[embedelse ->.36], (nl quote chunk 18 chunks embedtextbegin embedif 27 condembed 29 37 realchunk weakchunk)
//[condembed -> embedif embedtext.embedelse embedtext 27], (nl quote chunk 17 18 embedif 27 condembed 29 36 37)
//[condembed -> embedif embedtext.27], (nl quote chunk 17 18 embedif 27 condembed 29 36 37)
int LuaTemplateParser::handleState30()
{
	int result = 0;
	do
	{
		switch((this->*next)(2))
		{
		case TOK_27: result = handleState41(); break;
		case TOK_36: result = handleState43(); break;
		case TOK_embedelse: result = handleState42(); break;
		default:
			{
				const int expected[] = { TOK_27, TOK_embedelse, TOK_36, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[embedif -> 29 snippet.30], (nl quote chunk 18 chunks embedtextbegin embedif 27 condembed 29 36 37 realchunk weakchunk)
//[weakchunk -> 29 snippet.32], (nl quote chunk 17 18 embedif 27 condembed 29 36 37)
int LuaTemplateParser::handleState31()
{
	int result = 0;
	do
	{
		switch((this->*next)(4))
		{
		case TOK_30: result = handleState44(); break;
		case TOK_32: result = handleState45(); break;
		default:
			{
				const int expected[] = { TOK_30, TOK_32, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[includefile -> filename.], (quote)
int LuaTemplateParser::handleState32()
{
	(this->*next)(2);
	reduce10();
	return 0;
}

//[weakchunk -> 37 includefile.quote], (nl quote chunk 17 18 embedif 27 condembed 29 36 37)
int LuaTemplateParser::handleState33()
{
	int result = 0;
	do
	{
		switch((this->*next)(2))
		{
		case TOK_quote: result = handleState46(); break;
		default:
			{
				const int expected[] = { TOK_quote, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[textindent -> spaces.], (nl quote chunk 17 18 chunks embedif 27 condembed 29 36 37 realchunk weakchunk)
int LuaTemplateParser::handleState34()
{
	(this->*next)(2);
	reduce26();
	return 0;
}

//[text -> textbegin textlines lastline.], (17)
int LuaTemplateParser::handleState35()
{
	(this->*next)(2);
	reduce13();
	return 2;
}

//[textlines -> textlines textline.], (spaces nl quote chunk 17 18 chunks embedif 27 condembed 29 36 37 textindent realchunk weakchunk)
int LuaTemplateParser::handleState36()
{
	(this->*next)(3);
	reduce16();
	return 1;
}

//[textline -> textindent.nl], (spaces nl quote chunk 17 18 chunks embedif 27 condembed 29 36 37 textindent realchunk weakchunk)
//[textline -> textindent.chunks realchunk nl], (spaces nl quote chunk 17 18 chunks embedif 27 condembed 29 36 37 textindent realchunk weakchunk)
//[textline -> textindent.chunks weakchunk nl], (spaces nl quote chunk 17 18 chunks embedif 27 condembed 29 36 37 textindent realchunk weakchunk)
//[lastline -> textindent.chunks], (17 27 36)
//[chunks ->.], (quote chunk 17 18 embedif 27 condembed 29 36 37)
//[chunks ->.chunks realchunk], (quote chunk 17 18 embedif 27 condembed 29 36 37)
//[chunks ->.chunks weakchunk], (quote chunk 17 18 embedif 27 condembed 29 36 37)
int LuaTemplateParser::handleState37()
{
	int result = 0;
	do
	{
		switch((this->*next)(2))
		{
		case TOK_nl: result = handleState47(); break;
		case TOK_chunks: result = handleState48(); break;
		case TOK_quote:
		case TOK_chunk:
		case TOK_17:
		case TOK_18:
		case TOK_embedif:
		case TOK_27:
		case TOK_condembed:
		case TOK_29:
		case TOK_36:
		case TOK_37: result = 0; reduce22(); break;
		default:
			{
				const int expected[] = { TOK_nl, TOK_quote, TOK_chunk, TOK_17, TOK_18, TOK_chunks, TOK_embedif, TOK_27, TOK_condembed, TOK_29, TOK_36, TOK_37, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[weakchunk -> 18 31 snippet.32], (nl quote chunk 17 18 embedif 27 condembed 29 36 37)
int LuaTemplateParser::handleState38()
{
	int result = 0;
	do
	{
		switch((this->*next)(5))
		{
		case TOK_32: result = handleState49(); break;
		default:
			{
				const int expected[] = { TOK_32, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[embedtextbegin -> chunks nl.], (spaces nl quote chunk 18 chunks textlines embedif 27 condembed 29 36 37 textline textindent realchunk weakchunk)
int LuaTemplateParser::handleState39()
{
	(this->*next)(3);
	reduce5();
	return 1;
}

//[embedtext -> embedtextbegin textlines.lastline], (27 36)
//[textlines -> textlines.textline], (spaces nl quote chunk 18 chunks embedif 27 condembed 29 36 37 textindent realchunk weakchunk)
//[textline ->.textindent nl], (spaces nl quote chunk 18 chunks embedif 27 condembed 29 36 37 textindent realchunk weakchunk)
//[textline ->.textindent chunks realchunk nl], (spaces nl quote chunk 18 chunks embedif 27 condembed 29 36 37 textindent realchunk weakchunk)
//[textline ->.textindent chunks weakchunk nl], (spaces nl quote chunk 18 chunks embedif 27 condembed 29 36 37 textindent realchunk weakchunk)
//[lastline ->.textindent chunks], (27 36)
//[textindent ->.spaces], (nl quote chunk 18 chunks embedif 27 condembed 29 36 37 realchunk weakchunk)
//[textindent ->.], (nl quote chunk 18 chunks embedif 27 condembed 29 36 37 realchunk weakchunk)
int LuaTemplateParser::handleState40()
{
	int result = 0;
	do
	{
		switch((this->*next)(3))
		{
		case TOK_spaces: 
			{
				TokSpaces tok;
				tok.str = parserCtrl.string();
				TokSpaces *prev = curr.spaces;
				curr.spaces = &tok;
				result = handleState34();
				parserCtrl.release(curr.spaces->str);
				curr.spaces = prev;
				break;
			}
		case TOK_lastline: result = handleState50(); break;
		case TOK_textindent: result = handleState37(); break;
		case TOK_textline: result = handleState36(); break;
		case TOK_nl:
		case TOK_quote:
		case TOK_chunk:
		case TOK_18:
		case TOK_chunks:
		case TOK_embedif:
		case TOK_27:
		case TOK_condembed:
		case TOK_29:
		case TOK_36:
		case TOK_37:
		case TOK_realchunk:
		case TOK_weakchunk: result = 0; reduce27(); break;
		default:
			{
				const int expected[] = { TOK_spaces, TOK_nl, TOK_quote, TOK_chunk, TOK_18, TOK_chunks, TOK_lastline, TOK_embedif, TOK_27, TOK_condembed, TOK_29, TOK_36, TOK_37, TOK_textline, TOK_textindent, TOK_realchunk, TOK_weakchunk, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[condembed -> embedif embedtext 27.], (nl quote chunk 17 18 embedif 27 condembed 29 36 37)
int LuaTemplateParser::handleState41()
{
	(this->*next)(2);
	reduce32();
	return 2;
}

//[embedtextbegin ->.chunks nl], (spaces nl quote chunk 18 chunks textlines embedif 27 condembed 29 37 textline textindent realchunk weakchunk)
//[embedtext ->.chunks], (27)
//[embedtext ->.embedtextbegin textlines lastline], (27)
//[chunks ->.], (nl quote chunk 18 embedif 27 condembed 29 37)
//[condembed -> embedif embedtext embedelse.embedtext 27], (nl quote chunk 17 18 embedif 27 condembed 29 36 37)
//[chunks ->.chunks realchunk], (nl quote chunk 18 embedif 27 condembed 29 37)
//[chunks ->.chunks weakchunk], (nl quote chunk 18 embedif 27 condembed 29 37)
int LuaTemplateParser::handleState42()
{
	int result = 0;
	do
	{
		switch((this->*next)(2))
		{
		case TOK_embedtextbegin: result = handleState29(); break;
		case TOK_embedtext: result = handleState51(); break;
		case TOK_chunks: result = handleState28(); break;
		case TOK_nl:
		case TOK_quote:
		case TOK_chunk:
		case TOK_18:
		case TOK_embedif:
		case TOK_27:
		case TOK_condembed:
		case TOK_29:
		case TOK_37: result = 0; reduce22(); break;
		default:
			{
				const int expected[] = { TOK_nl, TOK_quote, TOK_chunk, TOK_18, TOK_chunks, TOK_embedtextbegin, TOK_embedtext, TOK_embedif, TOK_27, TOK_condembed, TOK_29, TOK_37, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[embedelse -> 36.], (nl quote chunk 18 chunks embedtextbegin embedif 27 condembed 29 37 realchunk weakchunk)
int LuaTemplateParser::handleState43()
{
	(this->*next)(2);
	reduce9();
	return 0;
}

//[embedif -> 29 snippet 30.], (nl quote chunk 18 chunks embedtextbegin embedif 27 condembed 29 36 37 realchunk weakchunk)
int LuaTemplateParser::handleState44()
{
	(this->*next)(2);
	reduce8();
	return 2;
}

//[weakchunk -> 29 snippet 32.], (nl quote chunk 17 18 embedif 27 condembed 29 36 37)
int LuaTemplateParser::handleState45()
{
	(this->*next)(2);
	reduce34();
	return 2;
}

//[weakchunk -> 37 includefile quote.], (nl quote chunk 17 18 embedif 27 condembed 29 36 37)
int LuaTemplateParser::handleState46()
{
	(this->*next)(2);
	reduce35();
	return 2;
}

//[textline -> textindent nl.], (spaces nl quote chunk 17 18 chunks embedif 27 condembed 29 36 37 textindent realchunk weakchunk)
int LuaTemplateParser::handleState47()
{
	(this->*next)(3);
	reduce18();
	return 1;
}

//[embedif ->.29 snippet 30], (nl quote chunk 18 chunks embedtextbegin embedif 27 condembed 29 36 37 realchunk weakchunk)
//[textline -> textindent chunks.realchunk nl], (spaces nl quote chunk 17 18 chunks embedif 27 condembed 29 36 37 textindent realchunk weakchunk)
//[textline -> textindent chunks.weakchunk nl], (spaces nl quote chunk 17 18 chunks embedif 27 condembed 29 36 37 textindent realchunk weakchunk)
//[lastline -> textindent chunks.], (17 27 36)
//[realchunk ->.quote], (nl quote chunk 17 18 embedif 27 condembed 29 36 37)
//[realchunk ->.chunk], (nl quote chunk 17 18 embedif 27 condembed 29 36 37)
//[weakchunk ->.condembed], (nl quote chunk 17 18 embedif 27 condembed 29 36 37)
//[weakchunk ->.18 name], (nl quote chunk 17 18 embedif 27 condembed 29 36 37)
//[condembed ->.embedif embedtext embedelse embedtext 27], (nl)
//[condembed ->.embedif embedtext 27], (nl)
//[weakchunk ->.18 31 snippet 32], (nl quote chunk 17 18 embedif 27 condembed 29 36 37)
//[weakchunk ->.29 snippet 32], (nl quote chunk 17 18 embedif 27 condembed 29 36 37)
//[weakchunk ->.37 includefile quote], (nl quote chunk 17 18 embedif 27 condembed 29 36 37)
//[chunks -> chunks.realchunk], (quote chunk 17 18 embedif 27 condembed 29 36 37)
//[chunks -> chunks.weakchunk], (quote chunk 17 18 embedif 27 condembed 29 36 37)
int LuaTemplateParser::handleState48()
{
	int result = 0;
	do
	{
		switch((this->*next)(2))
		{
		case TOK_embedif: result = handleState19(); break;
		case TOK_quote: result = handleState16(); break;
		case TOK_37: result = handleState22(); break;
		case TOK_condembed: result = handleState20(); break;
		case TOK_29: result = handleState21(); break;
		case TOK_chunk: 
			{
				TokChunk tok;
				tok.str = parserCtrl.string();
				TokChunk *prev = curr.chunk;
				curr.chunk = &tok;
				result = handleState17();
				parserCtrl.release(curr.chunk->str);
				curr.chunk = prev;
				break;
			}
		case TOK_18: result = handleState18(); break;
		case TOK_weakchunk: result = handleState53(); break;
		case TOK_realchunk: result = handleState52(); break;
		default: result = 2; reduce21(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[weakchunk -> 18 31 snippet 32.], (nl quote chunk 17 18 embedif 27 condembed 29 36 37)
int LuaTemplateParser::handleState49()
{
	(this->*next)(2);
	reduce33();
	return 3;
}

//[embedtext -> embedtextbegin textlines lastline.], (27 36)
int LuaTemplateParser::handleState50()
{
	(this->*next)(2);
	reduce7();
	return 2;
}

//[condembed -> embedif embedtext embedelse embedtext.27], (nl quote chunk 17 18 embedif 27 condembed 29 36 37)
int LuaTemplateParser::handleState51()
{
	int result = 0;
	do
	{
		switch((this->*next)(2))
		{
		case TOK_27: result = handleState54(); break;
		default:
			{
				const int expected[] = { TOK_27, };
				result = parserCtrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		}
	}
	while(result == 0);
	return --result;
}

//[textline -> textindent chunks realchunk.nl], (spaces nl quote chunk 17 18 chunks embedif 27 condembed 29 36 37 textindent realchunk weakchunk)
//[chunks -> chunks realchunk.], (quote chunk 17 18 embedif 27 condembed 29 36 37)
int LuaTemplateParser::handleState52()
{
	int result = 0;
	do
	{
		switch((this->*next)(2))
		{
		case TOK_nl: result = handleState55(); break;
		default: result = 2; reduce36(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[textline -> textindent chunks weakchunk.nl], (spaces nl quote chunk 17 18 chunks embedif 27 condembed 29 36 37 textindent realchunk weakchunk)
//[chunks -> chunks weakchunk.], (quote chunk 17 18 embedif 27 condembed 29 36 37)
int LuaTemplateParser::handleState53()
{
	int result = 0;
	do
	{
		switch((this->*next)(2))
		{
		case TOK_nl: result = handleState56(); break;
		default: result = 2; reduce37(); break;
		}
	}
	while(result == 0);
	return --result;
}

//[condembed -> embedif embedtext embedelse embedtext 27.], (nl quote chunk 17 18 embedif 27 condembed 29 36 37)
int LuaTemplateParser::handleState54()
{
	(this->*next)(2);
	reduce31();
	return 4;
}

//[textline -> textindent chunks realchunk nl.], (spaces nl quote chunk 17 18 chunks embedif 27 condembed 29 36 37 textindent realchunk weakchunk)
int LuaTemplateParser::handleState55()
{
	(this->*next)(3);
	reduce19();
	return 3;
}

//[textline -> textindent chunks weakchunk nl.], (spaces nl quote chunk 17 18 chunks embedif 27 condembed 29 36 37 textindent realchunk weakchunk)
int LuaTemplateParser::handleState56()
{
	(this->*next)(3);
	reduce20();
	return 3;
}


