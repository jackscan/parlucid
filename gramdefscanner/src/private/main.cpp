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


