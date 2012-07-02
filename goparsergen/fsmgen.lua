local maxHexDigits = 8

function generateFsm(name, fsm, tokenNames, context, path)

	function generateFsmImpl(fsm)
		local name = fsm.name
		{%
		package $name

		@{generateContexts(fsm)}
	
		@{generateStateHandlerArrayInit(fsm)}

		%}
	end

	function getTokenName(tok)
		return genTokenName(tok, tokenNames)
	end

	function splitTokenSet(set)
		local result = {}
		local i = 1
		local c

		if set then
			c = #set
			--print("context: "..v.." len: "..c)
			repeat
				local c1, c2 = math.max(1, 1 + c - maxHexDigits), c
				result[i] = string.sub(set, c1, c2)
				i = i + 1
				c = c - maxHexDigits
			until c <= 0
		else
			result[1] = 0
		end
		return result
	end

	function generateTokenSet(set)
		local n
		local b = 0
		local s = false
		for i,o in ipairs(splitTokenSet(set)) do
			n = tonumber(o, 16)
			for j = 0, 31 do
				if (n % 2) > 0 then
					{%@{s}?,@; ${getTokenName(b)}%}
					s = true
				end
				n = math.floor(n / 2)
				b = b + 1
			end
		end
		{% %}
	end


	function generateContexts(fsm)

		local firstLinearState = fsm.firstLinearState
		local nrOfStates = fsm.nrOfStates
		--assert(fsm.firstLinearState >= fsm.nrOfStates, "linear states not implemented yet")
		{%
		
		var tokenizerContexts = [${#fsm.context}][]int {
		@{for i,c in ipairs(fsm.context) do}
			// Context ${i - 1}
			{@{generateTokenSet(c)}},
		@{end}
		}
		
		%}
	end

	function generateTransitions(fsm, state)
		local classname = fsm.classname
		local accTable = fsm[state]
	
	
		{%
		@{for i,v in ipairs(accTable) do}
		{ '${escape(string.char(v.char))}', ${v.next}, TokenSet{@{generateTokenSet(v.tokens)}} },
		@{end}
		%}
	end

	function generateStateHandlerArrayInit(fsm)
		local classname = fsm.classname
		{%
		var transitionTable = []struct {
			potential TokenSet
			transitions []transition
		} {
			@{for state=0,fsm.nrOfStates-1 do}
			// State $state
			{ TokenSet{@{generateTokenSet(fsm[state].potential)}}, []transition{
				@{generateTransitions(fsm, state)}
			} },
			@{end}
		}

	
		%}
	end
	
	function generateTokenizer(fsm)
		local name = fsm.name
		{%
		package $name

		import (
			"io"
		)

		type state int
		type TokenSet []int

		type transition struct {
			symbol byte
			next   state
			tokens TokenSet
		}

		func createTokenizer(reader io.Reader) func(int) (int, Lexeme) {

			buffer := make(Lexeme, 0, 1)
			pos := 0
			state := state(0)

			reset := func() {
				copy(buffer, buffer[pos:])
				buffer = buffer[:len(buffer)-pos]
				pos = 0
				state = 0
			}

			read := func() (sym byte) {
				if pos >= len(buffer) {
					if len(buffer) == cap(buffer) {
						newlen := 2 * len(buffer)
						newbuffer := make(Lexeme, newlen)
						copy(newbuffer, buffer)
						buffer = newbuffer
					}
					n, err := reader.Read(buffer[pos:cap(buffer)])
					
					if n == 0 && err != nil {
						if err == io.EOF {
							return 0
						} else {
							panic(err)
						}
					}
					buffer = buffer[:pos+n]
				}
				sym = buffer[pos]
				pos++
				return
			}

			traverse := func(sym byte) TokenSet {
				trans := transitionTable[state].transitions
				lower, upper := 0, len(trans)
				if upper > 0 {
					for lower < upper-1 {
						index := (upper + lower) / 2
						if sym < trans[index].symbol {
							upper = index
						} else {
							lower = index
						}
					}
					
					state = trans[lower].next
					
					if sym >= trans[lower].symbol && state >= 0 {
						return trans[lower].tokens
					}
				}
				return nil
			}

			updateContext := func(tokens, context TokenSet) (TokenSet, bool) {
				for i, j := 0, 0; i < len(tokens); i++ {
					t := tokens[i]
					for ; j < len(context) && t > context[j]; j++ {
					}
					if t == context[j] {
						return context[:j+1], true
					}
				}
				
				potential := transitionTable[state].potential
				for i, j := 0, 0; i < len(context); i++ {
					t := context[i]
					for ; j < len(potential) && t > potential[j]; j++ {
					}
					if t == potential[j] {
						return context[i:], false
					}
				}
				
				return nil, false
			}

			return func(ictx int) (token int, str Lexeme) {

				context := tokenizerContexts[ictx]
				str = nil
				reset()

				for len(context) > 0 {
					tokens := traverse(read())
					if tokens == nil {
						break
					}
					var valid bool
					context, valid = updateContext(tokens, context)
					if valid {
						token = context[len(context)-1]
						str = buffer[:pos]
					}
				}
				
				pos = len(str)

				return
			}
		}
		%}
	end

	fsm.name = name
	--fsm.tokennames = tokennames
	fsm.context = context

	local file = path.."/"..name.."/".."transitions.go"
	result = output(file)
	if result then print(result) end
	print("generating "..file)
	generateFsmImpl(fsm)
	flush()
	
	file = path.."/"..name.."/".."tokenizer.go"
	result = output(file)
	if result then print(result) end
	print("generating "..file)
	generateTokenizer(fsm)
	flush()

end


