
function generateParser(name, table, tokenNames, context, path)	
	local parser = Parser(name, table, tokenNames)
	local parserfile = path.."/"..name.."/".."parser.go"
	local result = output(parserfile)
	if result then print(result) end
	print("generating "..parserfile)
	{%
	package $name
	
	import (
		"io"
		"fmt"
		"strings"
	)

		@{--printTable(parser.names)}
		@{--printTable(parser.captures)}
		@{--printTable(parser)}
	
	@{generateTypeDefinitions()}

	@{parser:generateImpl()}
	%}
	
	flush()
	
	local tablefile = path.."/"..name.."/".."parsertable.go"
	result = output(tablefile)
	if result then print(result) end
	print("generating "..tablefile)
	{%
	package $name

	@{parser:generateParserTable()}
	%}
	
	flush()
end

function Parser(name, parser, tokenNames)
	local tokentype = "int"
	
	parser.names = tokenNames
		
	function getCaptureStackName(tok)
		return parser:getTokenName(tok)
	end
	
	function getCaptureType(tok)
		if next(parser.captures[tok]) then
			return parser:getTokenName(tok).."Attr"
		else
			return "Lexeme"
		end
	end
	
	function getReductionHandlerName(production)
		return "reduce"..production
	end
	
	function parser:generateImpl()
		{%
		
		@{generateParseFunction()}

		@{generateMethods()}
		
		@{generateHelperFunctions()}

		@{self:generateReductionHandlers()}
		
		@{generatePushHandler()}
		%}
	end
	
	function generateTypeDefinitions()
		{%
		type Lexeme []byte
		type terminalList []Lexeme
		type parseraction func(p *parser, str Lexeme)
		type parsermap map[int]parseraction
		type ParserState int
		type parser struct {
			listener ParserListener
			attribs attributeStack 
			stack []ParserState
			unread bool
		}
		
		@{generateErrorType()}
				
		@{parser:generateInterface()}
	
		@{parser:generateTokenStructures()}
		
		type attributeStack struct {
			@{generateStackDef()}
		}
		%}
	end
	
	function parser:generateTokenStructures(deps)
		if deps then
			for tok, dep in pairs(deps) do
				local name = self:getTokenName(tok)
				{%
				$name ${getCaptureType(tok)}
				%}
			end
		else
			for tok, deps in pairs(self.captures) do
				local name = self:getTokenName(tok)
				local Name = self:getTokenName(tok, 1)
				if next(deps) then
				{%
				type ${getCaptureType(tok)} struct {
					@{self:generateTokenStructures(deps)}
				}
				%}
				--[[else
				{%
				type ${getCaptureType(tok)} Lexeme
				%}]]
				end
			end
		end
	end
		
	function getStackType(tok)
		if not next(parser.captures[tok]) then
			return "terminalList"
		else
			return "[]"..getCaptureType(tok)
		end
	end
	
	function generateErrorType()
		{%
		type ParserError struct {
			token    int
			lex      Lexeme
			expected TokenSet
			stack    []ParserState
		}

		func (e ParserError) Error() string {
			expstr := make([]string, len(e.expected))
			for i, exp := range e.expected {
				expstr[i] = TokenNames[exp]
			}
			return fmt.Sprintf("error at <%s>: %s\\n expected: %s\\n",
				TokenNames[e.token],
				e.lex,
				strings.Join(expstr, ", "))
		}
		%}
	end
	
	function generateStackDef()
		for tok, deps in pairs(parser.captures) do
			{%
			${getCaptureStackName(tok)} ${getStackType(tok)}
			%}
		end 
	end

	function generateAttribStackInit()
		for tok, deps in pairs(parser.captures) do
			{%
				${getCaptureStackName(tok)}: make(${getStackType(tok)}, 0),
			%}
		end
	end

	function parser:tokenMember(tokenList)
		local result = ""
		local sep = ""
		for i = 1, #tokenList do
			local tok = tokenList[i]
			local name = self:getTokenName(tok)
			result = result..sep..name
			sep = "."
		end
		return result
	end
	
	
	function parser:generateReductionHandlers()
		for i = 0, #self.productions do
			local prod = self.productions[i]
			local actions = prod.actions
			local caps = self.captures[prod.head]
			--printTable(prod.actions)
			if (actions and next(actions)) or hasCaptures(prod) then
				self:generateReductionHandler(i)
			end
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
		
		local headname = self:getTokenComment(head)
		local headtok = genTokenName(head, tokenNames)
		local bodylen = #body
		local caps = self.captures[head]
		local headstack = getCaptureStackName(head)
		{%
		// $headname ->@{for i,v in ipairs(body) do} ${self:getTokenComment(v)}@{end}
		func reduce$prodNo(p *parser, str Lexeme) {
			@{self:generateCapturesUpdate(prodNo, lparams)}
			@{if actions then for i, action in pairs(actions) do self:generateAction(self.productions[prodNo], action) end end}
			@{self:generateStackUpdate(body)}
			@{caps and next(caps)}?p.attribs.$headstack = append(p.attribs.$headstack, ${self:getTokenName(head)}Parent)@;
			@{head == 0}?p.remove($bodylen)@:p.reduce($headtok, $bodylen)@;
		}
		%}
	end
	
	function parser:generateStackUpdate(body)
		local updates = {}
		for i, tok in ipairs(body) do
			if self.captures[tok] then
				updates[tok] = (updates[tok] or 0) + 1
			end
		end
		for tok, count in pairs(updates) do
			local stack = "p.attribs."..getCaptureStackName(tok)
			{%
			$stack = $stack[:len($stack) - $count]
			%}
		end
	end
	
	function parser:getStackMember(container, arg, body)
		local count = 0
		for i,t in ipairs(body) do
			if t == arg[1] then
				count = count + 1
			end
		end
		
		local result = "Lexeme{}"
		
		if #arg > 0 then
			local stackName = container.."."..getCaptureStackName(arg[1])
			result = stackName.."[len("..stackName..") - "..count.."]"
			for i = 2, #arg do
				result = result.."."..self:getTokenName(arg[i])
			end
		end
		
		--[[if #arg > 1 then
			result = "Lexeme("..result..")"
		end]]
		
		return result
	end
	
	function parser:generateAction(prod, action)
		local headname = self:getTokenName(prod.head)
		if action[0] then
			{%${headname}Parent.${self:tokenMember(action[0])} = %}
		end
		local func = action["function"]
		local attribs = "p.attribs"
		if func then
			--printTable(action)
			{%p.listener.$func(%}
			for i, arg in ipairs(action) do
				{%@{1<i}?, @;${self:getStackMember(attribs, arg, prod.body)}%}
			end
			{%)
			%}
		elseif action[1] then
			{%${self:getStackMember(attribs, action[1], prod.body)}
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
		if caps and next(caps) then
			self:generateCaptureUpdate(prod, caps, lparams)
		end
	end
		
	function parser:generateCaptureUpdate(prod, deps, lparams)
		local name = self:getTokenName(prod.head)
		local actions = prod.actions
		local captoks = {}
		for i,tok in ipairs(prod.body) do
			if deps[tok] then
				captoks[tok] = (captoks[tok] or 0) + 1
			end
		end
		
		{%
		${name}Parent := ${getCaptureType(prod.head)}{@{for tok, offset in pairs(captoks) do}
			${self:getTokenName(tok)}: p.attribs.${getCaptureStackName(tok)}[len(p.attribs.${getCaptureStackName(tok)}) - $offset],
		@{end}}
		%}
	end

	function parser:generateInterface()
		local methods = { }

		for prodNo = 0, #self.productions do
			local actions = self.productions[prodNo].actions
			if actions then
				for iact, action in pairs(actions) do
					local func = action["function"]
					if func then
						if not methods[func] then
							methods[func] = { action = action }
						end
						if action[0] then
							methods[func].ret = true
						end
					end
				end
			end
		end

		function generateMethodArgs(action)
			local sep = ""
			for ia, arg in ipairs(action) do
				write(sep)
				for it, tok in ipairs(arg) do
					write(self:getTokenName(tok, it > 1))
				end
				sep = ", "
			end
			-- has at least on argument
			if action[1] then
				write(" Lexeme")
			end
		end

		{%
		type ParserListener interface {
			Error(token int, str Lexeme, expected TokenSet, stack []ParserState)
			Shift(str Lexeme)
		@{for f, m in pairs(methods) do}
			$f(@{generateMethodArgs(m.action)})@{m.ret}? Lexeme@;
		@{end}
		}
		%}
	end
	
	function parser:generateParserTable()
		--printTable(self)

		{%
		var parsertable = [${#self + 1}] struct {
			context int
			parsermap
		} {
		@{for state=0,#self do
			local v = self[state]
			local accepted = self:getAccepted(v)}
			// state $state
			@{self[state].info.ignorable}?//<IGNORABLE>@;
			@{for i, inf in ipairs(self[state].info) do}
			//@{self:generateInfoComment(inf)}
			@{end}
			{ ${v.context}, parsermap{
				@{self:generateStateHandlerMap(state, v)}
			} },
		@{end}
		}
		%}
	end
	
	function parser:generateInfoComment(info)
		--printTable(info)
		if info.ignore then {%<ignore> %} end
		local prod = self.productions[info.production]
		if prod then
			local head = self:getTokenComment(prod.head)
			function printBody()
				for i,tok in ipairs(prod.body) do
					local tokenname = self:getTokenComment(tok) 
					{%@{i-1==info.dot}?.@: @;$tokenname%}
				end
				if info.dot == #prod.body then {%.%} end
			end
			function printFollow()
				for i,tok in ipairs(info.follows) do
					local tokenname = self:getTokenComment(tok)
					{%@{i>1}? @;$tokenname%}						
				end
			end
			{%[$head ->@{printBody()}], (@{printFollow()})%}
		end
	end
	
	function parser:generateStateHandlerMap(state, statetable)
		local defaultHandled = false
		{%
		@{if statetable.shifts then
			for tok, nextState in pairs(statetable.shifts) do
				local name = genTokenName(tok, tokenNames)
				local nextContext = nil
				if self[nextState].info.ignorable then nextContext = context end
		}
		$name: @{self:generateStateHandling(tok, nextState, nextContext)}
		@{	end
		end
		if statetable.reduces then
			local defaultProd, defaultTok = next(statetable.reduces)
			for production, tokens in pairs(statetable.reduces) do
				--printTable(self.productions[production].body)
				local bodylen = #self.productions[production].body
				-- check for non empty body, otherwise we got endlessloop
				if bodylen > 0 and production == defaultProd then
					write(genTokenName(0, tokenNames)..": ")
					self:generateReduce(production)
					newline(1)
				else
					for ic, tok in ipairs(tokens) do
						write(genTokenName(tok, tokenNames)..": ")
						self:generateReduce(production)
						newline(1)
					end
				end
		}
		@{	end
		end}
		%}
	end
	
	function hasCaptures(prod)
		local caps = parser.captures
		
		if caps[prod.head] then
			return true
		end
		
		for i, tok in ipairs(prod.body) do
			if caps[tok] then
				return true
			end
		end
		
		return false
	end

	function parser:generateReduce(prodNo)
		local prod = self.productions[prodNo]
		local head = prod.head
		local caps = self.captures[head]
		local actions = prod.actions
		local headName = genTokenName(head, tokenNames)
		local bodyLen = #prod.body
		if (actions and next(actions)) or hasCaptures(prod) then
			{%reduce$prodNo,%}
		else
			{%reduce($headName, $bodyLen),%}
		end
		{% // ${self:getTokenComment(head)} ->%}
		for i,v in ipairs(prod.body) do
			write(" "..self:getTokenComment(v))
		end
	end
	
	function parser:generateCases(tokens)
		if tokens then
			local fallthrough = nil
			for ic, tok in ipairs(tokens) do
				local name = genTokenName(tok, tokenNames)
				{%@{fallthrough}?$fallthrough@;
					case $name: %}
				fallthrough = "fallthrough"
			end
		else
			{%
			@{genTokenName(0, tokenNames)}:%}
		end
	end
	
	function parser:generateSingleReduce(state, context, production, reduce)
		if self[state].info.ignorable then context = "context" end
		{%
			p.next($context)
			@{self:generateReductionHandling(production)}
			return ${#self.productions[production].body - 1}
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
		default: panic([]int{ @{for i,tok in ipairs(accepted) do
			}${genTokenName(tok, tokenNames)}, @{
			end}})
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
	
	function parser:getTokenComment(tok)
		local tokenname = self.names[tok]
		if tokenname then
			local firstChar = string.sub(tokenname, 1, 1)
			if firstChar == '"' or firstChar == '/' then
				tokenname = string.gsub(tokenname, '%*/', '*\/')
			end
		else
			tokenname = "$"
		end
		return tokenname
	end
	
	function parser:generateStateHandling(tok, state, context)
		-- if captured and terminal
		if self.captures[tok] and (next(self.captures[tok]) == nil) then
			local Name = self:getTokenName(tok, 1)
			{%push$Name($state),%}
		else
			{%shift($state),%}
		end
	end
	
	function generatePushHandler()
		for tok, deps in pairs(parser.captures) do
			if next(deps) == nil then
				local Name = parser:getTokenName(tok, 1)
				{%
				func push$Name(state int) parseraction {
					return func(p *parser, str Lexeme) {
						p.push(state, str, &p.attribs.${getCaptureStackName(tok)})
					}
				}
				%}
			end
		end
	end
	
	function generateMethods()
		{%
		func (p *parser) shift(state int, str Lexeme) {
			p.stack = append(p.stack, ParserState(state))
			if str != nil {
				p.listener.Shift(str)
			}
		}

		func (p *parser) push(state int, str Lexeme, lexemeList *terminalList) {
			s := make(Lexeme, len(str))
			copy(s, str)
			*lexemeList = append(*lexemeList, s)
			p.shift(state, str)
		}

		func (p *parser) traverse(token int, str Lexeme) {
			pmap := parsertable[p.stack[len(p.stack) - 1]].parsermap
			action, ok := pmap[token]
			
			if !ok {
				// recheck for end token
				action, ok = pmap[TokEND]
			}
			
			if !ok {
				expected := make([]int, 0, len(pmap))
				for tok := range pmap {
					if tok != TokEND {
						expected = append(expected, tok)
					}
				}
				p.listener.Error(token, str, expected, p.stack)
			}
			
			action(p, str)
		}

		func (p *parser) reduce(head int, bodylen int) {
			p.remove(bodylen)
			p.traverse(head, nil)
		}

		func (p *parser) remove(bodylen int) {
			p.unread = true
			p.stack = p.stack[:len(p.stack) - bodylen]
		}
		%}
	end
	
	function generateHelperFunctions()
		{%
		func shift(nextstate int) parseraction {
			return func(p *parser, str Lexeme) {
				p.shift(nextstate, str)
			}
		}

		func push(nextstate int, lexemeList *terminalList) parseraction {
			return func(p *parser, str Lexeme) {
				s := make(Lexeme, len(str))
				copy(s, str)
				*lexemeList = append(*lexemeList, s)
				p.shift(nextstate, str)
			}
		}

		func reduce(head int, bodylen int) parseraction {
			if head == TokEND {
				return func(p *parser, str Lexeme) {
					p.remove(bodylen)
				}
			}
			return func(p *parser, str Lexeme) {
				p.reduce(head, bodylen)
			}
		}
		%}
	end
	
	function generateParseFunction()
		{%
		func Parse(rd io.Reader, listener ParserListener) {
			tokenizer := createTokenizer(rd)
			parser := parser{
				listener: listener,
				attribs: attributeStack{@{generateAttribStackInit()}},
				stack: make([]ParserState, 1),
				unread: false,
			}
			var token int
			var str Lexeme
			
		ParserLoop:
			for len(parser.stack) > 0 {
				state := parsertable[parser.stack[len(parser.stack) - 1]]
				// check for special case: single-reduce
				if len(state.parsermap) == 1 && state.parsermap[TokEND] != nil {
					unread := parser.unread
					state.parsermap[TokEND](&parser, nil)
					parser.unread = unread
					continue
				}
				
				if !parser.unread {
					token, str = tokenizer(state.context)
				} else {
					parser.unread = false
				}
				
				parser.traverse(token, str)
			}
			
			if token != TokEND {
				parser.stack = append(parser.stack, 0)
				goto ParserLoop
			}
		}
		%}
	end
	
	return parser
end


