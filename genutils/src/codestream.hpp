#ifndef CODESTREAM_HPP
#define CODESTREAM_HPP

#include <vector>
#include <string>
#include <ostream>

class CodeStream;

void indent(CodeStream& cs);
void revindent(CodeStream& cs);
void newline(CodeStream& cs);

struct split;
struct newline;

template<typename T> const char* typenamestr();

class CodeStream
{
	template<typename T> friend CodeStream& operator<< (CodeStream& cs, const T& rhs);
public:
	CodeStream(std::ostream& os, const std::string& indent);
	void incIndent() { indentstr.push_back(defaultIndent); }
	void decIndent() { indentstr.pop_back(); }
	void newline(int padding = 0);

	void split(const std::string& tok) { splitTok = tok; };
	void resetSplit() { splitTok.clear(); }

	void reset();
	void flush() { stream.flush(); }

	void setIndent(const std::string& i);
	size_t getIndentLength() const;

	CodeStream& operator<< (void (*pfn)(CodeStream&)) { pfn(*this); return *this; }
	CodeStream& operator<< (std::ios_base& (*pfn)(std::ios_base& _Str)) { stream << pfn; return *this; }
	CodeStream& operator<< (const struct split&);
	CodeStream& operator<< (const struct newline&);


private:

	void checkNewline();

	void doSplit() { if (!splitTok.empty()) { stream << splitTok; resetSplit(); } }

	std::ostream& stream;
	std::vector<std::string> indentstr;
	const std::string defaultIndent;
	std::string splitTok;
	int newlines;
	int newlinePadding;
};

inline void CodeStream::checkNewline()
{
	if (newlines > 0)
	{
		for (size_t i = 0; i < indentstr.size(); ++i)
			stream << indentstr[i];
		newlines = 0;
		newlinePadding = 0;
	}
}

inline void indent(CodeStream& cs)
{
	cs.incIndent();
}

inline void revindent(CodeStream& cs)
{
	cs.decIndent();
}

inline void newline(CodeStream& cs)
{
	cs.newline();
}

struct split
{
	split(const char* tok): tok(tok) {};
	const char* tok;
};

struct newline
{
	newline(int n): num(n) {}
	int num;
};

inline CodeStream& CodeStream::operator<< (const struct split& s)
{
	this->split(s.tok);
	return *this;
}

inline CodeStream& CodeStream::operator<< (const struct newline& n)
{
	this->newline(n.num);
	return *this;
}

template<>
inline const char* typenamestr<char>()
{
	return "char";
}

template<>
inline const char* typenamestr<int>()
{
	return "int";
}

template<typename T>
CodeStream& operator<< (CodeStream& cs, const T& rhs)
{
	cs.checkNewline();
	cs.doSplit();
	cs.stream << rhs;
	return cs;
}

template<>
inline CodeStream& operator<< (CodeStream& cs, const char& rhs)
{
	cs.checkNewline();
	cs.doSplit();
	cs.stream << "'";
	switch(rhs)
	{
	case '\0': cs.stream << "\\0"; break;
	case '\r': cs.stream << "\\r"; break;
	case '\n': cs.stream << "\\n"; break;
	case '\t': cs.stream << "\\t"; break;
	case '\'': cs.stream << "\\'"; break;
	case '\b': cs.stream << "\\b"; break;
	case '\a': cs.stream << "\\a"; break;
	case '\f': cs.stream << "\\f"; break;
	case '\v': cs.stream << "\\v"; break;
	case '\\': cs.stream << "\\\\"; break;
	default:
		if (' ' <= rhs && rhs < 127)
			cs.stream << rhs;
		else
			cs.stream << "\\x" << std::hex << (static_cast<int>(rhs)& 0xFF) << std::dec;
		break;
	}
	cs.stream << "'";
	return cs;
}

#endif
