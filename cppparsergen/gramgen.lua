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



function generateGrammar(name, productions, tokenNames, path)

	local classname = name.."Grammar"

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

	function generateHeader()
		--printTable(productions)
		{%
		#ifndef ${string.upper(classname)}_HPP
		#define ${string.upper(classname)}_HPP

		namespace $classname
		{
		struct Production
		{
			int head;
			int len;
			const int* body;
		};
		
		const Production& getProduction(int prod);
		}

		#endif
		%}
	end

	function generateImpl()
		{%
		#include "../$classname.hpp"

		namespace $classname
		{
		
			@{for i=0,#productions do local body = productions[i].body}
				@{if #body > 0 then}
			static const int body$i[${#body + 1}] = { @{for j,v in ipairs(body) do}$v, @{end} };
				@{end}
			@{end}
			
			static const Production productions[${#productions + 1}] =
			{
			@{for i=0,#productions do local body = productions[i].body}
				{ ${productions[i].head}, ${#body}, @{#body > 0}?body$i@:0@; },
			@{end}
			};
			
			const Production& getProduction(int prod)
			{
				return productions[prod];
			}
		
		}

		%}

	end

	local headerFile = path.."/"..name.."Grammar.hpp"
	local result = output(headerFile)
	if result then print(result) end
	print("generating "..name.." grammar");

	generateHeader()
	
	
	flush()
	
	local implFile = path.."/private/"..classname..".cpp"
	local result = output(implFile)
	if result then print(result) end
	
	generateImpl()
	
	flush()
end

