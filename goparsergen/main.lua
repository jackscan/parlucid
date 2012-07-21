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


function printTable(table)
	local typ = type(table)
	if typ == "table" then
		for k,v in pairs(table) do
			{%@{type(k) == "string"}?`$k'@:[$k]@; = %}
			
			local typ = type(v)
			if typ == "table" then
				{%{
				   @{printTable(v)}
				}
				%}
			else
				write(v)newline(1)
			end
			
		end
	else
		{%
			$typ: $table
		%}
	end
end

if not path then path = "." end

function main(name, fsm, context, table, tokenNames)	
	--printTable(table)
	--printTable(fsm)
	tokenNames[0] = "END"
	name = string.lower(name)
	generateFsm(name, fsm, tokenNames, context, path)
	generateParser(name, table, tokenNames, context, path)
	generateTokens(name, tokenNames, path)
	--generateGrammar(name, table.productions, tokenNames, path)
end

function genTokenName(tok, tokenNames)
	local tokenname = tokenNames[tok]
	local firstChar
	if tokenname then
		firstChar = string.sub(tokenname, 1, 1)
		if firstChar == '"' or firstChar == '/' then
			tokenname = tok
		else
			tokenname = "Tok"..string.upper(firstChar)..string.sub(tokenname, 2)
		end
	else
		tokenname = tok
	end
	return tokenname
end

function hexChar(ch)
	return string.format("\\x%02X", string.byte(ch))
end

list = 
{
	equal = function (list1, list2)
		local result = false
		if #list1 == #list2 then
			result = true
			for i, e in ipairs(list1) do
				if e ~= list2[i] then
					result = false
					break
				end
			end
		end
		return result
	end,

	count = function (list, entry)
		local c = 0
		for i,v in ipairs(list) do
			if v == entry then
				c = c + 1
			end
		end
		return c
	end,
	
	contains = function (list, entry)
		local result = false
		for i,v in ipairs(list) do
			if v == entry then
				result = true
				break
			end
		end
		return result
	end,
	
	sublist = function (list, first)
		local result = {}
		for i = first, #list  do
			result[i - first + 1] = list[i]
		end
		return result
	end,
}

function escape(str)
	local esc = string.gsub(str, "[\\']", "\\%0")
	esc = string.gsub(esc, "\r", "\\r")
	esc = string.gsub(esc, "\n", "\\n")
	esc = string.gsub(esc, "\t", "\\t")
	esc = string.gsub(esc, "\b", "\\b")
	esc = string.gsub(esc, "\a", "\\a")
	esc = string.gsub(esc, "\f", "\\f")
	esc = string.gsub(esc, "\v", "\\v")
	esc = string.gsub(esc, "[%z\1-\31\127-\255]", hexChar)
	return esc
end
