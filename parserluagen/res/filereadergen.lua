
function generateFileReader(path)
	local name = "FileReader"
	local headerFile = path.."/"..name..".hpp"
	local result = output(headerFile)
	if result then print(result) end
	print("generating "..name.." fsm");
	generateFsmHeader(getFsmName(name), tokens)
	flush()
	local implFile = path.."/private/"..getFsmName(name)..".cpp"
	result = output(implFile)
	if result then print(result) end
	generateFsmImpl(getFsmName(name), tokens)
	flush()
end