

function generateTokens(name, tokenNames, path)

	local file = path.."/"..name.."/".."tokens.go"
	local result = output(file)
	if result then print(result) end
	print("generating "..file);
	
	function getTokenString(tok)
		local tokenname = tokenNames[tok]
		local firstChar = string.sub(tokenname, 1, 1)
		if firstChar == '"' or firstChar == '/' then
			tokenname = escape(tokenname)
			tokenname = string.gsub(tokenname, '"', '\\"')
		end
		return tokenname
	end
	
	{%
	package $name
	
	const (
		TokEND = 0
	@{for tok, name in ipairs(tokenNames) do
		local firstChar = string.sub(name, 1, 1)
		if firstChar ~= '"' and firstChar ~= '/' then}
		${genTokenName(tok, tokenNames)} = $tok
		@{end
	end}
	)
	
	var tokenNames = [${#tokenNames+1}]string{
		"\@",
		@{for i=1, #tokenNames do}
		"${getTokenString(i)}",
		@{end}
	}
	
	%}
	
	flush()
end

