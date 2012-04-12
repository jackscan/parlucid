

function generateTokens(name, tokenNames, path)

	function getTokenName(tok)
		local tokenname = tokenNames[tok]
		local firstChar = string.sub(tokenname, 1, 1)
		if firstChar == '"' or firstChar == '/' then
			tokenname = tok
		end
		return tokenname
	end
	
	function getTokenString(tok)
		local tokenname = tokenNames[tok]
		local firstChar = string.sub(tokenname, 1, 1)
		if firstChar == '"' or firstChar == '/' then
			tokenname = escape(tokenname)
			tokenname = string.gsub(tokenname, '"', '\\"')
		end
		return tokenname
	end

	local headerFile = path.."/"..name.."Tokens.hpp"
	local result = output(headerFile)
	if result then print(result) end
	print("generating "..name.." tokens");
	
	--printTable(tokenNames)
	{%
	#ifndef ${string.upper(name.."Tokens")}_HPP
	#define ${string.upper(name.."Tokens")}_HPP
	
	namespace ${name}Tokens
	{
	
	enum
	{
		TOK_END,
		@{for i=1, #tokenNames do}
		TOK_${getTokenName(i)},
		@{end}
		TOK_NUMOFTOKS = TOK_${getTokenName(#tokenNames)}
	};
	
	const char* getTokenName(int tok);
	
	}
	
	#endif
	%}
	flush()
	
	local implFile = path.."/private/"..name.."Tokens.cpp"
	local result = output(implFile)
	if result then print(result) end
	
	{%
	#include "../${name}Tokens.hpp"
	
	namespace ${name}Tokens
	{
	
	static const char* sTokenNames[] =
	{
		"\@",
		@{for i=1, #tokenNames do}
		"${getTokenString(i)}",
		@{end}
	};
	
	const char* getTokenName(int tok)
	{
		if (0 <= tok && (unsigned)tok < sizeof(sTokenNames) / sizeof(const char*))
			return sTokenNames[tok];
		else return "invalid token";
	}
	
	}
	
	%}
	
	flush()
end

