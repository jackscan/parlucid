function log(filename, parserlog, tokennames)
	
	function tokenClass(type)
		local str = tokennames[type]
		if string.find(str, "[\"/@]") then
			str = "TOK_"..type
		end
		return str
	end

	function header()
		{%
		<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN"
			"http://www.w3.org/TR/html4/strict.dtd">
		<html>
		<head>
		<title>$filename log</title>
		<style type="text/css">
			@{styles()}
		</style>
		</head>
		%}
	end
	
	function styles()
		local num = #tokennames
		for tok=0,num do
			local class = tokenClass(tok)
			local col = tok * (0x7FFFFF / num)
			local red = col % 128;
			local green = (col / 128) % 128;
			local blue = (col / 16384) % 128;
			local color1 = string.format("%02X%02X%02X", red, green, blue)
			local color2 = string.format("%02X%02X%02X", red+127, green+127, blue+127)
			{%
			.$class { color:#$color1; }
			.$class:hover { background-color:#$color2; }
			%}
		end
	end
	
	function body()
		{%
		<pre>
			<code>
		@{generateNode(parserlog)}
			</code>
		</pre>
		%}
	end
	
	function htmlConvert(str)
		str = string.gsub(str, "[\n\r]+", "\n")
		str = string.gsub(str, "<", "&lt;")
		str = string.gsub(str, ">", "&gt;")
		str = string.gsub(str, "&", "&amp;")
		str = string.gsub(str, "\"", "&quot;")
		return str
	end
	
	function stringConvert(str)
		str = string.gsub(str, "\"", "\\\"")
		return str
	end
	
	function generateNode(node)
		local type = tokenClass(node.type)
		local title = tokennames[node.type]
		local token = node.token
		
		if token then
			token = htmlConvert(token)
			--title = type
			title = htmlConvert(title)
			{%<span class="$type" title="$title">$token</span>%}
		else
			for i,c in ipairs(node) do generateNode(c) end
			--{%<span class="$type" title="$type">@{for i,c in ipairs(node) do generateNode(c) end}</span>%}
		end
		
		
	end

	local logFile = filename..".html"
	local result = output(logFile)
	if result then print(result) end
	
	--printTable(parserlog)
	{%
	@{header()}
	<body>
	@{body()}
	</body>
	%}
	flush()
	
end


