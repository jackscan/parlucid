#include "../genhelper.hpp"

namespace genutils
{

std::string& replace_escapes(std::string& str)
{
	bool escaped = false;
	std::string::iterator iWrite = str.begin();
	for (std::string::const_iterator iRead = str.begin(); iRead != str.end(); ++iRead)
		if (!escaped && *iRead == '\\') escaped = true;
		else
		{
			char ch = *iRead;
			if (escaped) switch(ch)
				{
				case 'a': ch = '\a'; break;
				case 'b': ch = '\b'; break;
				case 'f': ch = '\f'; break;
				case 'n': ch = '\n'; break;
				case 'r': ch = '\r'; break;
				case 't': ch = '\t'; break;
				case 'v': ch = '\v'; break;
				}
			escaped = false;
			*iWrite = ch;
			++iWrite;
		}
	str.resize(iWrite - str.begin());
	return str;
}

}
