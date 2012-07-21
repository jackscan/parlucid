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


