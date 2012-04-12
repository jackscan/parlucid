#include "../LuaTemplateParserCtrl.hpp"
#include "../../gen/LuaTemplateGrammar.hpp"

#include <boost/filesystem.hpp>

#include <streambuf>
#include <algorithm>
#include <cassert>
#include <float.h>
#include <cstring>



using namespace LuaTemplateTokens;

LuaTemplateParserCtrl::LuaTemplateParserCtrl()
: cstream(result, "\t")
, errors(false)
, parser(self())
{
}

LuaTemplateParserCtrl::~LuaTemplateParserCtrl()
{
	closeFiles();
}

LuaTemplateParserCtrl& LuaTemplateParserCtrl::self()
{
	return *this;
}

void LuaTemplateParserCtrl::openFile(const char* filename)
{
	FILE* file = fopen(filename, "rb");
	if (file)
	{
		this->files.push_back(FileInfo(file, filename));
	}
	else
	{
		fprintf(stderr, "failed to open '%s': %s\n", filename, strerror(errno));
	}
}

void LuaTemplateParserCtrl::closeFiles()
{
	for (size_t t = 0; t < this->tokens.size(); ++t)
		delete[] this->tokens[t];
	this->tokens.clear();
	for (size_t f = 0; f < this->files.size(); ++f)
		fclose(this->files[f].file);
	this->files.clear();
}

const char* LuaTemplateParserCtrl::parse(const char* filename)
{
	this->result.str("");
	this->cstream.reset();
	this->parserLog.reset();
	this->errors = false;

	closeFiles();
	openFile(filename);
	parser.parse();

	if (!this->files.empty() && !feof(this->files.back().file))
	{
		fprintf(stderr, "%s not finished\n", this->files.back().filename.c_str());
	}
	this->cstream.flush();
	resultstr = result.str();
	return resultstr.c_str();
}

/*void LuaTemplateParserCtrl::text(const char* indent, const char* chunk, const char* nl)
{
	//if (chunk) fprintf(outfile, "stream:print(\"%s\");", chunk);
	//fprintf(outfile, nl ? "stream:newline();\n" : "\n");
}*/

void LuaTemplateParserCtrl::code(const char* chunk)
{
	cstream << chunk;
}

void LuaTemplateParserCtrl::quote()
{
	cstream << "write(\"\\\"\")";
}

void LuaTemplateParserCtrl::codeindent(const char* indent)
{
	cstream.setIndent(indent);
	currentCodeIndent = indent;
}

void LuaTemplateParserCtrl::codenl()
{
	cstream << newline;
	//fprintf(outfile, "\n");
}

void LuaTemplateParserCtrl::variable(const char* varname)
{
	cstream << "write(" << varname << ")";
}

void LuaTemplateParserCtrl::snippet(const char* code)
{
	cstream << "write(" << code << ")";
}

void LuaTemplateParserCtrl::embedtextbegin()
{
	cstream << "newline(1)" << indent << newline;
}

void LuaTemplateParserCtrl::embedtextend()
{
	cstream << revindent;
}

void LuaTemplateParserCtrl::embed(const char* code)
{
	if (code)
	{
		cstream << code;
		// if code ends on alphanum char insert space
		if (isalnum(code[strlen(code) - 1]))
			cstream << " ";
	}
}

void LuaTemplateParserCtrl::embedif(const char* code)
{
	cstream << "if " << (code ? code : "false") << " then ";
}

void LuaTemplateParserCtrl::embedelse()
{
	cstream << " else ";
}

void LuaTemplateParserCtrl::embedend()
{
	cstream << " end ";
}

void LuaTemplateParserCtrl::chunk(const char* chunk)
{
	cstream << "write(\"" << chunk << "\")";
}

void LuaTemplateParserCtrl::textindent(const char* spaces)
{
	size_t textindent = strlen(spaces);
	size_t codeindent = currentCodeIndent.length();

	spaces += textindent >= codeindent ? codeindent : textindent;

	if (currentTextIndent != spaces)
		cstream << "setIndent(\"" << (currentTextIndent = spaces) << "\")" << newline;
}

void LuaTemplateParserCtrl::textindent()
{
	textindent("");
}

void LuaTemplateParserCtrl::textnl()
{
	cstream << "newline()" << newline;
}

void LuaTemplateParserCtrl::weaknl()
{
	cstream << "newline(1)" << newline;
}

void LuaTemplateParserCtrl::textbegin()
{
	currentTextIndent.clear();
	cstream << "pushIndent()newline(1)" << indent << newline;
}

void LuaTemplateParserCtrl::textend()
{
	currentTextIndent.clear();
	cstream << revindent << "popIndent()";
}

void LuaTemplateParserCtrl::include(const char* filename)
{
	FileInfo& current = files.back();
	current.buffer.erase(current.buffer.begin(), current.buffer.begin() + current.validBuffer);
	current.validBuffer = 0;

	boost::filesystem::path currentpath(current.filename);
	boost::filesystem::path newpath(currentpath.parent_path());
	newpath /= filename;
	openFile(newpath.string().c_str());
	cstream << "pushIndent()" << indent << newline;
}

const char* LuaTemplateParserCtrl::string()
{
	if (files.back().buffer.empty()) return 0;
	else
	{
		char* str = new char[files.back().validBuffer + 1];
		str[files.back().validBuffer] = '\0';
		strncpy(str, &files.back().buffer.front(), files.back().validBuffer);
		tokens.push_back(str);
		return str;
	}
}

void LuaTemplateParserCtrl::release(const char* str)
{
}

int LuaTemplateParserCtrl::shift(int context)
{
	{
		FileInfo& current = files.back();
		current.buffer.erase(current.buffer.begin(), current.buffer.begin() + current.validBuffer);
		current.validBuffer = 0;
	}

	this->token = TOK_END;

	while (!files.empty() && files.back().buffer.empty() && feof(files.back().file))
	{
		fclose(files.back().file);
		files.pop_back();
		if (!files.empty()) cstream << revindent << "popIndent()";
	}

	if (!files.empty())
	{
		FileInfo& current = files.back();

		fsm.begin(context);
		
		int i = 0;
		int len = static_cast<int> (current.buffer.size());

		while (len == 0 || fsm.process(&current.buffer[i], len))
		{
			i += len;
			current.buffer.resize(static_cast<size_t> (i) < current.buffer.capacity() ? current.buffer.capacity() : current.buffer.capacity() * 2);
			len = fread(&current.buffer[i], 1, current.buffer.size() - i, current.file);
			current.buffer.resize(i + len);
			if (len == 0) break;
		}

		current.validBuffer = fsm.end(this->token);

		if (TOK_nl == this->token) ++current.lineCounter;

		parserLog.shift(this->token, current.validBuffer, current.buffer.empty() ? 0 : &current.buffer.front());
	}
	else
	{
		parserLog.shift(this->token, 0, 0);
	}
	
	return this->token;
}

int LuaTemplateParserCtrl::error(int, const int *)
{
	const FileInfo& curr = files.back();
	fprintf(stderr, "%s(%i): error reading token <%s>: '",
		!curr.filename.empty() ? curr.filename.c_str() : "line ",
		curr.lineCounter,
		LuaTemplateTokens::getTokenName(this->token));
	if (!curr.buffer.empty()) fwrite(&curr.buffer.front(), 1, curr.validBuffer, stderr);
	else fprintf(stderr, "\\0");
	fprintf(stderr, "'\n");
	return 1;
}

void LuaTemplateParserCtrl::reduce(int prod)
{
	const LuaTemplateGrammar::Production& production = LuaTemplateGrammar::getProduction(prod);
	parserLog.reduce(production.head, production.len);
	//fprintf(stdout, "reduce %i\n", prod);
}

const ParserLog& LuaTemplateParserCtrl::log() const
{
	return this->parserLog;
}


