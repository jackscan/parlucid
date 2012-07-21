--[[
Copyright 2004-2012 Mathias Fiedler. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
]]



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
		NUMOFTOKS
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

