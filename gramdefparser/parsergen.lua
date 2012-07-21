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

function getParserName(name)
	return name.."Parser"
end

function generateParser(name, table, tokenNames, context, path)	
	local parser = Parser(name, table, tokenNames)
	local headerFile = path.."/"..getParserName(name)..".hpp"
	local result = output(headerFile)
	if result then print(result) end
	--print("header:"..headerFile)
	print("generating "..name.." parser")
	{%/*
		@{--printTable(parser.names)}
		@{--printTable(parser.captures)}
		@{--printTable(parser)}
	*/
	%}
	parser:generateHeader()
	flush()
	local implFile = path.."/private/"..getParserName(name)..".cpp"
	result = output(implFile)
	if result then print(result) end
	parser:generateImpl()
	flush()

end

function Parser(name, parser, tokenNames)
	local classname = getParserName(name)
	local ctrlClass = getParserName(name).."Ctrl"
	local ctrl = "parserCtrl"
	local tokentype = "int"
	
	parser.names = tokenNames
	
	function getStateHandlerName(state)
		return "handleState"..state
	end
	
	function getReductionHandlerName(production)
		return "reduce"..production
	end
	
	function parser:generateHeader()
		--printTable(self)
		{%
		#ifndef ${string.upper(classname)}_HPP
		#define ${string.upper(classname)}_HPP
		
		class $ctrlClass;
		
		class $classname
		{
		public:
			$classname($ctrlClass& ctrl);
			
			void parse();
			
		private:
		
			int shift(int);
			int push(int);
			int recall(int);
		
			$ctrlClass& $ctrl;
			int ($classname::*next)(int);
			int token;
			int symbol;
			
			@{self:generateTokenStructures()}
			
			void reduce(int, int);
			void ignore(int);
			
			@{for i = 0, #self.productions do}
			void ${getReductionHandlerName(i)}();
			@{end}
			
			@{for state=0,#self do}
			int ${getStateHandlerName(state)}(@{self[state].info.ignorable}?int context@;);
			@{end}
		};
		
		#endif
		%}	
	end
	
	function parser:generateTokenStructures(deps)
		if deps then
			if next(deps) then
				for tok, dep in pairs(deps) do
					local name = self:getTokenName(tok)
					{%struct {
						@{self:generateTokenStructures(dep)}
					} $name;
					%}
				end
			else
				{%const char* str;%}
			end
		else
			for tok, deps in pairs(self.captures) do
				local name = self:getTokenName(tok)
				local Name = self:getTokenName(tok, 1)
				{%
				struct Tok$Name
				{
					@{self:generateTokenStructures(deps)}
				};
				%}
			end
			
			{%
			struct
			{
			@{for tok, deps in pairs(self.captures) do
				local name = self:getTokenName(tok)
				local Name = self:getTokenName(tok, 1)}
				Tok$Name *$name;
			@{end}
			} curr;
			
			union
			{
			@{for tok, deps in pairs(self.captures) do
				local name = self:getTokenName(tok)
				local Name = self:getTokenName(tok, 1)}
				Tok$Name $name;
			@{end}
			} parent;
			%}
		end
	end
	
	function parser:generateImpl()
		{%
		#include "../$classname.hpp"
		#include "../${name}Tokens.hpp"
		#include "../../src/$ctrlClass.hpp"
		
		using namespace ${name}Tokens;
		
		@{self:generateCtor()}
		@{self:generateDtor()}
		
		void $classname::parse()
		{
			${getStateHandlerName(0)}();
		}
		
		$tokentype $classname::shift(int context)
		{
			return token = $ctrl.shift(context);
		}
		
		$tokentype $classname::push(int)
		{
			next = &$classname::recall;
			return symbol;
		}
		
		$tokentype $classname::recall(int)
		{
			next = &$classname::shift;
			return token;
		}
		
		void $classname::reduce(int production, $tokentype sym)
		{
			symbol = sym;
			next = &$classname::push;
			$ctrl.reduce(production);
		}
		
		void $classname::ignore(int production)
		{
			next = &$classname::recall;
			$ctrl.reduce(production);
		}
		
		@{self:generateReductionHandlers()}
		
		@{self:generateStateHandlers()}
		
		%}
	end
	
	function parser:generateCtor()
		local caps = self.captures
		--printTable(caps)
		 
		{%
		$classname::$classname($ctrlClass &ctrl) : $ctrl(ctrl), next(&$classname::shift), token(0), symbol(0)
		{
		@{for token, deps in pairs(caps) do}
			curr.${self:getTokenName(token)} = 0; // $token
		@{end}
		}
		%}
		--[[for (typename std::map<Token, std::set<std::basic_string<Token> > >::const_iterator iCap = capturedTokens.begin(); iCap != capturedTokens.end(); ++iCap)
		stream << "curr." << getTokenName(iCap->first) << " = 0;" << newline;
		stream << revindent << "}" << newline;*/]]
	end
	
	function parser:generateDtor()
		{%
		%}
	end
	

	function parser:tokenMember(tokenList, ptr)
		local result = ""
		--printTable(tokenList)
		for i = 1, #tokenList do
			local tok = tokenList[i]
			local name = self:getTokenName(tok)
			result = result..name
			if ptr and i == 1 then result = result.."->" 
			else result = result.."." end
		end
		return result
	end
	
	
	function parser:generateReductionHandlers()
		for i = 0, #self.productions do
			self:generateReductionHandler(i)
		end
	end
		
	function parser:generateReductionHandler(prodNo)
		local head = self.productions[prodNo].head
		local body = self.productions[prodNo].body
		local actions = self.productions[prodNo].actions
		local lparams = {}
		if actions then
			for i, action in ipairs(actions) do
				if action[0] then
					local lparam = {head}
					for ia, t in ipairs(action[0]) do
						table.insert(lparam, t)
					end
					table.insert(lparams, lparam)
				end
			end
		end
		
		local headname = self:getTokenName(head)		
		{%
		// $headname ->@{for i,v in ipairs(body) do} ${self:getTokenName(v)}@{end}
		void $classname::${getReductionHandlerName(prodNo)}()
		{
			@{if head == 0 then}ignore($prodNo);@{else}reduce($prodNo, TOK_$headname);@{end}
			@{if actions then for i, action in pairs(actions) do self:generateAction(action) end end}
			@{self:generateCapturesUpdate(prodNo, lparams)}
		}
		%}
	end
	
	function parser:generateAction(action)
		if action[0] then	
			{%parent.${self:tokenMember(action[0])}str = %}
		end
		local func = action["function"]
		if func then
			--printTable(action)
			{%$ctrl.$func(%}
			for i, arg in ipairs(action) do
				{%@{1<i}?, @;curr.${self:tokenMember(arg, true)}str%}
			end
			{%);
			%}
		elseif action[1] then
			{%curr.${self:tokenMember(action[1], true)}str;
			%}
		end
	end
	
	function parser:generateCapturesUpdate(prodNo, lparams)
		local prod = self.productions[prodNo]
		local head = prod.head
		local caps = self.captures[head] 
		local actions = prod.actions
		local tokenlist = { head }
		--printTable(lparams)
		--printTable(caps)
		if caps then
			self:generateCaptureUpdate(tokenlist, caps, lparams, prod)
		end
	end
	
	function parser:generateCaptureUpdate(tokenlist, deps, lparams, prod)
		if next(deps) then
			for tok, dependencies in pairs(deps) do
				table.insert(tokenlist, tok)
				self:generateCaptureUpdate(tokenlist, dependencies, lparams, prod)
				table.remove(tokenlist)
			end
		else
			local islparam = false
			--printTable(tokenlist)
			for i, lparam in ipairs(lparams) do
				--printTable(lparam)
				if list.equal(lparam, tokenlist) then
					islparam = true
					break
				end
			end
			if not islparam then
				{%
				parent.${self:tokenMember(tokenlist)}str = %}
				if #tokenlist > 1 and list.contains(prod.body, tokenlist[2]) then
					local member = self:tokenMember(list.sublist(tokenlist, 2), true)
					{%curr.${member}str;%}
				else
					{%0;%}
				end
			end
		end
	end
	
	function parser:generateStateHandlers()
		--printTable(self)
		for state=0,#self do
			local v = self[state]
			local accepted = self:getAccepted(v)
			local simpleState = true
			for i, acc in ipairs(accepted) do
				if self.nonterminals[acc] then
					simpleState = false
					--printTable(accepted)
					--write(acc.." not in terminals")
					break;
				end
			end
			--printTable(v.reduces)
			if v.reduces then
				for prodNo, reduce in pairs(v.reduces) do
					if #self.productions[prodNo].body == 0 then
						--printTable(self.productions[prodNo])
						simpleState = false;
					end
				end
			end
			
			--TODO: check if there's a head of ignore production accepted
			simpleState = false;
			
			--printTable(self[state])
			{%
			@{self[state].info.ignorable}?//<IGNORABLE>@;
			@{for i, inf in ipairs(self[state].info) do}
			//@{self:generateInfoComment(inf)}
			@{end}
			int $classname::${getStateHandlerName(state)}(@{self[state].info.ignorable}?int context@;)
			{
			@{if v.reduces and not v.shifts and nil == next(v.reduces, next(v.reduces))	then}
			@{self:generateSingleReduce(state, v.context, next(v.reduces))}
			@{elseif simpleState then}
				int result = 0;
				@{self:generateStateHandlerSwitch(state, v)}
				return --result;
			@{else}
				int result = 0;
				do
				{
					@{self:generateStateHandlerSwitch(state, v)}
				}
				while(result == 0);
				return --result;
			@{end}
			}
			
			%}
		end
	end
	
	function parser:generateInfoComment(info)
		--printTable(info)
		if info.ignore then {%<ignore> %} end
		local prod = self.productions[info.production]
		if prod then
			local head = self:getTokenName(prod.head)
			function printBody()
				for i,tok in ipairs(prod.body) do
					local tokenname = self:getTokenName(tok) 
					{%@{i-1==info.dot}?.@: @;$tokenname%}
				end
				if info.dot == #prod.body then {%.%} end
			end
			function printFollow()
				for i,tok in ipairs(info.follows) do
					local tokenname = self:getTokenName(tok)
					{%@{i>1}? @;$tokenname%}						
				end
			end
			{%[$head ->@{printBody()}], (@{printFollow()})%}
		end
	end
	
	function parser:generateStateHandlerSwitch(state, statetable)
		local context = statetable.context
		if self[state].info.ignorable then context = "context" end
		local defaultHandled = false
		{%
		switch((this->*next)($context))
		{
		@{if statetable.shifts then for tok, nextState in pairs(statetable.shifts) do
			local name = self:getTokenName(tok)
			local nextContext = nil
			if self[nextState].info.ignorable then nextContext = context end
			}
		case TOK_$name: @{self:generateStateHandling(tok, nextState, nextContext)}
		@{end end}
		@{if statetable.reduces then
			local defaultProd, defaultTok = next(statetable.reduces)
			for production, tokens in pairs(statetable.reduces) do
				--printTable(self.productions[production].body)
				local bodylen = #self.productions[production].body
				if bodylen > 0 and production == defaultProd then
					self:generateCases()
					defaultHandled = true
				else
					self:generateCases(tokens)
				end
				} result = $bodylen; ${getReductionHandlerName(production)}(); break;
		@{	end
		end}
		@{if not defaultHandled then self:generateErrorHandling(state, statetable) end}
		}
		%}
	end
	
	function parser:generateCases(tokens)
		if tokens then
			for ic, tok in ipairs(tokens) do
				local name = self:getTokenName(tok)
				{%
				case TOK_$name:%}
			end
		else
			{%
			default:%}
		end
	end
	
	function parser:generateSingleReduce(state, context, production, reduce)
		if self[state].info.ignorable then context = "context" end
		{%
			(this->*next)($context);
			${getReductionHandlerName(production)}();
			return ${#self.productions[production].body - 1};
		%}
	end
	
	function parser:getAccepted(row)
		local accepted = {}
		if row.shifts then for tok, nextState in pairs(row.shifts) do 
			table.insert(accepted, tok)
		end end
		if row.reduces then for prod, tokens in pairs(row.reduces) do
			for i, tok in ipairs(tokens) do table.insert(accepted, tok) end
		end end
		return accepted
	end
	
	function parser:generateErrorHandling(state, row)
		local accepted = self:getAccepted(row)
		table.sort(accepted)
		{%
		default:
			{
				const $tokentype expected[] = { @{for i,tok in ipairs(accepted) do local name = self:getTokenName(tok)}TOK_$name, @{end}};
				result = $ctrl.error(sizeof(expected) / sizeof(expected[0]), expected);
				break;
			}
		%}
	end
	
	function parser:getTokenName(tok, uppercase)
		local tokenname = self.names[tok]
		if tokenname then
			local firstChar = string.sub(tokenname, 1, 1)
			if firstChar == '"' or firstChar == '/' then
				tokenname = tok
			elseif uppercase then
				tokenname = string.upper(firstChar)..string.sub(tokenname, 2)
			end
		else
			tokenname = "END"
		end
		return tokenname
	end
	
	function parser:generateStateHandling(tok, nextState, context)
		
		if self.captures[tok] then
			local tokenname = self:getTokenName(tok)
			local tokenName = self:getTokenName(tok, 1)
			local terminal = (next(self.captures[tok]) == nil);	
			{%
				{
					Tok$tokenName tok@{not terminal}? = parent.$tokenname@;;
					@{terminal}?tok.str = $ctrl.string();@;
					Tok$tokenName *prev = curr.$tokenname;
					curr.$tokenname = &tok;
					result = ${getStateHandlerName(nextState)}($context);
					@{terminal}?$ctrl.release(curr.$tokenname->str);@;
					curr.$tokenname = prev;
					break;
				}
			%}
		else
			{%result = ${getStateHandlerName(nextState)}($context); break;%}
		end
	end
	
	return parser
end


