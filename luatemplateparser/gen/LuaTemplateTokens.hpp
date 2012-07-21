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
