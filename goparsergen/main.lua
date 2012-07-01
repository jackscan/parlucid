
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
