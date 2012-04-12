
function getFsmName(name)
	return name.."Tokenizer"
end

local maxHexDigits = 8

function generateFsm(name, fsm, tokenNames, context, path)

	fsm.name = name
	fsm.classname = getFsmName(name)
	--fsm.tokennames = tokennames
	fsm.context = calcContexts(context)
	fsm.tokensize = calcTokenSize(fsm)
	fsm.tokensettype = "int"
	fsm.tokentype = "int"
	fsm.chartype = "char"
	fsm.statetype = "PfnStateHandler"
	fsm.sequencetype = "PfnSequenceHandler"

	local headerFile = path.."/"..getFsmName(name)..".hpp"
	local result = output(headerFile)
	if result then print(result) end
	print("generating "..name.." fsm");
	generateFsmHeader(fsm)
	flush()
	local implFile = path.."/private/"..getFsmName(name)..".cpp"
	result = output(implFile)
	if result then print(result) end
	generateFsmImpl(fsm)
	flush()
end

function calcTokenSize(fsm)
	local max = 0
	for i,v in ipairs(fsm.context) do
		max = math.max(max, #v)
	end
	return max
end

function splitTokenSet(set)
	local result = {}
	local i = 1
	local c = #set
	--print("context: "..v.." len: "..c)
	repeat
		local c1, c2 = math.max(1, 1 + c - maxHexDigits), c
		result[i] = string.sub(set, c1, c2)
		i = i + 1
		c = c - maxHexDigits
	until c <= 0
	return result
end

function calcContexts(context)
	local contexts = {}
	for i,v in ipairs(context) do
		contexts[i] = splitTokenSet(v)
	end
	return contexts
end

function generateFsmHeader(fsm)
	
	local firstLinearState = fsm.firstLinearState
	local classname = fsm.classname
	local chartype = fsm.chartype
	local statetype = fsm.statetype
	local sequencetype = fsm.sequencetype
	local tokensettype = fsm.tokensettype
	local tokentype = fsm.tokentype
	local tokensize = fsm.tokensize
	
	
	--flush()

	{%
	#ifndef ${string.upper(classname)}_HPP
	#define ${string.upper(classname)}_HPP
	
	class $classname
	{
	public:
		@{generateContextComments(fsm.context)}
		void begin(unsigned context);
		int end($tokentype& tok);
		bool process(const $chartype* symbols, int len);
		
		typedef int State;
		const static State ERROR_STATE;
		
	private:	
		typedef State (*$statetype)($chartype, $tokensettype*);
		@{firstLinearState < fsm.nrOfStates}?
		typedef int (*$sequencetype)(State&, const $chartype*, int, $tokensettype*);@;
		
		//static const $statetype stateHandler[$firstLinearState];
		static const $statetype stateHandler[${fsm.nrOfStates}];
		@{firstLinearState < fsm.nrOfStates}?
		static const $sequencetype sequenceHandler[${fsm.nrOfStates - firstLinearState}];@;
		
		$tokensettype context[$tokensize];
		$tokentype token;
		int length;
		int pos;
		State state;
		
	};
	
	#endif
	%}
end

function generateFsmImpl(fsm)
	local classname = fsm.classname
	local name = fsm.name
	{%
	#include "../$classname.hpp"
	#include "../${name}Tokens.hpp"
	#include <cstring>
	
	@{generateStateHandlerImpls(fsm)}
	
	@{generateStateHandlerArrayInit(fsm)}
	
	@{generateTransitionLookUp(fsm)}
	
	@{generateProcessMethod(fsm)}
	%}
end

function generateContextComments(context)
	{%/*
	@{for i,v in ipairs(context) do}
	${i-1} = 0x@{for i = #v, 1, -1 do}${v[i]}@{end}
	@{end}
	*/
	%}
end

function generateProcessMethod(fsm)

	local classname = fsm.classname
	local tokentype = fsm.tokentype
	local tokensettype = fsm.tokensettype
	local tokensize = fsm.tokensize
	local chartype = fsm.chartype
	local firstLinearState = fsm.firstLinearState
	local nrOfStates = fsm.nrOfStates
	--assert(fsm.firstLinearState >= fsm.nrOfStates, "linear states not implemented yet")
	{%
	void $classname::begin(unsigned icontext)
	{
		const $tokensettype contexts[][$tokensize] =
		{
		@{for i,c in ipairs(fsm.context) do}
			{@{for i,v in ipairs(c) do} 0x${v},@{end} }, // ${i - 1}
		@{end}
		};
		
		if (icontext < ${#fsm.context})
		{
		@{for i = 0, tokensize-1 do}
			this->context[$i] = contexts[icontext][$i];
		@{end}
		}
		else
		{
		@{for i = 0, tokensize-1  do}
			this->context[$i] = 0x0;
		@{end}
		}
		this->state = 0;
		this->token = sizeof($tokensettype) * 8;
		this->length = 0;
		this->pos = 0;
	}
	
	int $classname::end($tokentype& token)
	{
		token = this->length > 0 ? this->token : ${fsm.name}Tokens::TOK_END;
		return this->length;
	}
		
	bool $classname::process(const $chartype* symbols, int len)
	{
		@{fsm.firstLinearState < fsm.nrOfStates}?
		const State nStateHandlers = sizeof(stateHandler) / sizeof(*stateHandler);
		const State nSequenceHandlers = sizeof(sequenceHandler) / sizeof(*sequenceHandler);
		@;
		int i = 0;
		$tokensettype tok[$tokensize] = { 0 };
		int tokenIndex = $tokensize - 1;
		while(i < len)
		{
			@{firstLinearState < nrOfStates}?
			if (state < nStateHandlers)
			{
				const Transition& trans = getTransition(state, symbols[i++], context);
				state = trans.next;
			}
			//else if (state - nStateHandlers < nSequenceHandlers)
			//	i += sequenceHandler[state - nStateHandlers](state, symbols, len, tok);
			@:
			const Transition& trans = getTransition(state, symbols[i++], context);
			state = trans.next;
			@;
			
			if (state < 0) break;
		
			$tokentype anytok = 0;
			for (int c = 0; c <= tokenIndex; ++c)
				anytok |= (tok[c] = (trans.tokens[c] & context[c]));
			
			if (anytok != 0)
			{
				const int setsize = sizeof($tokensettype) * 8;
				int newIndex = 0;
				for (; newIndex < tokenIndex && 0 == tok[newIndex]; ++newIndex)
					;
				if (newIndex != tokenIndex || tok[tokenIndex] != (1 << (token - tokenIndex * setsize)))
				{
					tokenIndex = newIndex;
					token = tokenIndex * setsize;
					for ($tokensettype ts = tok[tokenIndex]; 0 == (ts & 1); ++token, ts >>= 1)
						;
					// update context to not accept any tokens with lower prio (i.e. greater tokens)
					context[tokenIndex] &= ~(-1 << (1 + token - tokenIndex * setsize));
					for (int c = tokenIndex + 1; c < $tokensize; ++c) context[c] = 0x0;
				}
				length = pos + i;
			}
		}
		
		pos += len;
		
		return state >= 0;
	}
	%}
end

function generateStateHandlerImpls(fsm)
	local chartype = fsm.chartype
	local tokensettype = fsm.tokensettype
	local classname = fsm.classname

	{%
	const $classname::State ERROR_STATE = -1;
	
	struct Transition
	{
		const char symbol;
		const $classname::State next;
		const int tokens[${fsm.tokensize}];
	};
	
	%}
	local containsDeadEnd = false;
	--for state = 0, fsm.firstLinearState - 1, 1 do
	for state = 0, fsm.nrOfStates - 1, 1 do
		if next(fsm[state]) then
			{%
			@{generateTransitions(fsm, state)}
			
			%}
		else
			containsDeadEnd = true
		end
	end
	if containsDeadEnd then
		{%
		static const Transition deadEnd = { '\\x80', ERROR_STATE, { 0 } };
		
		%}
	end
	
	--[[local state = fsm.firstLinearState
	repeat
		local exitState, input, output = calculateNextLinearSequence(fsm, state)
		generateLinearSequenceHandler(fsm, state, input, output, exitState)
		state = state + #input
	until state > fsm.nrOfStates]]
end

function char(str, i)
	return string.sub(input, i, i);
end

function generateTransitions(fsm, state)
	local classname = fsm.classname
	local chartype = fsm.chartype
	local tokensettype = fsm.tokensettype
	local accTable = fsm[state]
	
	
	{%
	static const Transition transitions$state[${#accTable}] =
	{
		@{for i,v in ipairs(accTable) do}
		{ '${escape(v.char)}', ${v.next}, { @{if v.tokens then for t,o in ipairs(splitTokenSet(v.tokens)) do}0x$o, @{end else}0 @{end}} },
		@{end}
	};
	%}
end

function generateLinearSequenceHandler(fsm, startState, input, output, exitState)

	local classname = fsm.classname
	local chartype = fsm.chartype
	local tokensettype = fsm.tokensettype	
	local len = #input
	local lastState = startState + len - 1
	
	{%
	int $classname::handleStates_${startState}_${lastState}(const $chartype* symbols, int len)
	{
		const $chartype expected[$len] = { '${char(input, 1)}' @{for i = 2, len, 1 do}, '${char(input, i)}'@{end} }
		int offset = state - $startState;
		int n = len < $len - offset ? len : $len - offset;
		if (0 != strncmp(&expected[offset], symbols, n)) { state = ERROR_STATE; n = 0; }
		else if (n < $len - offset) state += n;
		else @{#output > 0}?{ @;state = $exitState;@{#output > 0}? @{generateOutput(fsm, classname, output)} }@;
		return n;
	}
	%}
end

function generateOutput(fsm, classname, output)
	{%
	TODO:
	%}
end

function calculateNextLinearSequence(fsm, startState)
	local state = startState
	local input = "";
	local output = 0;
	while state <= fsm.nrOfStates and fsm[state] do
		
		state = state + 1
	end
	--[[State state = startState;
	for (; state < getNumberOfStates() && transition_table[state].size() == 1; ++state)
	{
		input.append(1, transition_table[state].begin()->first);
		if (!transition_table[state].begin()->second.output.empty() || transition_table[state].begin()->second.nextState != state + 1)
		{
			output = transition_table[state].begin()->second.output;
			state = transition_table[state].begin()->second.nextState;
			break;
		}
	}
	return state;]]
end

function generateStateHandlerArrayInit(fsm)
	local classname = fsm.classname
	local chartype = fsm.chartype
	local tokensettype = fsm.tokensettype
	{%
	const struct StateTransitions
	{
		const int size;
		const int output[${fsm.tokensize}];
		const Transition* transitions;
	}
	transitionTable[${fsm.nrOfStates}] =
	{
		@{for state=0,fsm.nrOfStates-1 do}
			@{if next(fsm[state]) then}
		{ ${#(fsm[state])}, { @{for i,o in ipairs(splitTokenSet(fsm[state].potential)) do}0x$o, @{end}}, transitions$state },
			@{else}
		{ 1, { 0 }, &deadEnd },
			@{end}
		@{end}
	};
	
	%}
end

function generateTransitionLookUp(fsm)
	local classname = fsm.classname
	local chartype = fsm.chartype
	local tokensettype = fsm.tokensettype
	local tokensize = fsm.tokensize
	{%
	static inline const Transition& getTransition($classname::State state, $chartype sym, const $tokensettype context[$tokensize])
	{
		const StateTransitions& entry = transitionTable[state];
		
		$tokensettype anyoutput = 0;
		for (int c = 0; c < $tokensize; ++c)
			anyoutput |= (entry.output[c] & context[c]);
			
		if (anyoutput)
		{
			int lower = 0;
			int upper = entry.size;
			while (lower < upper - 1)
			{
				int index = (upper + lower) / 2;
				if (sym < entry.transitions[index].symbol) upper = index;
				else lower = index;
			}
			return sym < entry.transitions[lower].symbol ? deadEnd : entry.transitions[lower];
		}
		else return deadEnd;
	}
	%}
end

