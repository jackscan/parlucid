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
