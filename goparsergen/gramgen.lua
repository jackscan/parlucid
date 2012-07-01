

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

