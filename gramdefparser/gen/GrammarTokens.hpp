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
