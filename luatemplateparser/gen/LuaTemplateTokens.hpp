#ifndef LUATEMPLATETOKENS_HPP
#define LUATEMPLATETOKENS_HPP

namespace LuaTemplateTokens
{

enum
{
	TOK_END,
	TOK_spaces,
	TOK_name,
	TOK_nl,
	TOK_eof,
	TOK_textintro,
	TOK_code,
	TOK_quote,
	TOK_chunk,
	TOK_snippet,
	TOK_filename,
	TOK_template,
	TOK_codeline,
	TOK_codeindent,
	TOK_term,
	TOK_eol,
	TOK_text,
	TOK_17,
	TOK_18,
	TOK_var,
	TOK_chunks,
	TOK_embedtextbegin,
	TOK_embedtext,
	TOK_textlines,
	TOK_lastline,
	TOK_embedif,
	TOK_elsechunk,
	TOK_27,
	TOK_condembed,
	TOK_29,
	TOK_30,
	TOK_31,
	TOK_32,
	TOK_call,
	TOK_embed,
	TOK_embedelse,
	TOK_36,
	TOK_37,
	TOK_includefile,
	TOK_include,
	TOK_textbegin,
	TOK_textline,
	TOK_textindent,
	TOK_realchunk,
	TOK_weakchunk,
	TOK_textchunk,
	NUMOFTOKS
};

const char* getTokenName(int tok);

}

#endif
