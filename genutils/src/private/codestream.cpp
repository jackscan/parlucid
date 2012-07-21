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

#include "../codestream.hpp"
#include <sstream>
#include <ostream>

CodeStream::CodeStream(std::ostream& os, const std::string& indent)
: stream(os), defaultIndent(indent), newlines(1), newlinePadding(0)
{
}

void CodeStream::newline(int padding)
{
	if (padding == 0)
	{
		if (newlinePadding > 0) --newlinePadding;
		else stream << '\n';
		++newlines;
	}
	else if (padding > newlines)
	{
		for (int i = newlines; i < padding; ++i) stream << '\n';
		newlines = padding;
		newlinePadding += padding - newlines;
	}
}

void CodeStream::setIndent(const std::string &i)
{
	if (indentstr.empty()) indentstr.push_back(i);
	else indentstr.back() = i;
}

size_t CodeStream::getIndentLength() const
{
	size_t result = 0;
	for (size_t i = 0; i < indentstr.size(); ++i)
		result += indentstr[i].length();
	return result;
}

void CodeStream::reset()
{
	newlines = 1;
	newlinePadding = 0;
	indentstr.clear();
	splitTok.clear();
}
