/*
 * Copyright 2004-2012 Mathias Fiedler. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "../../../gramdefparser/src/grammarparserctrl.hpp"

#include <cstdlib>

FILE* openFile(const char* filename, const char* mode)
{
	FILE * file = 0;
	if (!(file = fopen(filename, mode)))
		fprintf(stderr, "failed to open '%s': %s\n", filename, strerror(errno));
	return file;
}

int main(int argc, char *argv[])
{
	int result = 0;

	GrammarParserCtrl parserCtrl;

	for (int i = 1; i < argc; ++i)
	{
		FILE *infile = openFile(argv[i], "rb");
		if (infile)
		{
			std::string filename(argv[i]);
			parserCtrl.parse(infile, filename);
		}
		else result = 1;
	}


	std::map<std::string, GrammarParserCtrl::AnnotatedGrammar> grammars = parserCtrl.getGrammars();

	for (std::map<std::string, GrammarParserCtrl::AnnotatedGrammar>::iterator igram = grammars.begin(); igram != grammars.end(); ++igram)
		fprintf(stdout, igram == grammars.begin() ? "%s" : " %s", igram->first.c_str());

	//fprintf(stdout, "\n");

	return result;
}


