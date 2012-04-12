#ifndef GRAMMARPARSERCTRL_HPP
#define GRAMMARPARSERCTRL_HPP

#include "../gen/GrammarTokenizer.hpp"
#include "../gen/GrammarParser.hpp"
#include "../gen/GrammarGrammar.hpp"

#include "../../lexer/src/alphabet.hpp"
#include "../../lexer/src/nfa.hpp"

#include "../../lalrparser/src/lalrgram.hpp"

#include "../../parserlog/src/ParserLog.hpp"

//#include "../../utils/src/bitsequence.hpp"

#include <cstdio>

#include <istream>
#include <stack>

class GrammarParserCtrl: public ParserLog
{
public:
	typedef char Char;
	typedef int Token;
	typedef LalrGrammar<Token> Gram;
	typedef std::basic_string<Token> Parameter;

	struct Action
	{
		explicit Action(const std::string& name = std::string());
		std::basic_string<Token> lparameter;
		std::string function;
		std::vector<std::basic_string<Token> > parameter;
	};
	typedef std::vector<Action> ActionList;

	struct AnnotatedGrammar
	{

		AnnotatedGrammar() : numOfTokens(1) { }
		~AnnotatedGrammar();

		Gram grammar;
		std::vector<ActionList> actions;
		std::map<Token, std::string> tokenNames;
		std::list<Nfa<Char> > nfas;
		Token numOfTokens;
		std::vector<std::basic_string<Token> > unresolvedParameters;
	};

	GrammarParserCtrl();
	~GrammarParserCtrl();

	const std::map<std::string, AnnotatedGrammar>& getGrammars() const;

	void parse(FILE* file, const std::string& filename);

	int shift(int);
	int error(size_t n, const int* expected);
	void reduce(int);

	const char* string();
	void release(const char*);

	void grammar(const char* name);
	void regexGroup(const char* name);
	void production(const char* head, const char* body);
	void production(const char* head);
	void ignoreProduction(const char* body);
	void action();
	void token(const char* name, const char* regex);
	const char* terminal(const char* regex);
	const char* nonTerminal(const char* name);
	void function(const char* name);
	void assignment();
	void lparameter();
	void parameter();
	void concatParameter(const char* part);
	void newLine();

	const char* alternative(const char*, const char*);
	//void newBody();
	void addTerm(const char*, const char* id);
	const char* newBodyExpr(const char* id);
	const char* optional(const char*);
	const char* list(const char*);
	const char* newSubExpr(const char*);
	const char* endToken();

private:

	static std::string& replaceEscapes(std::string&);

	/// Inlines productions without actions where head is referenced once.
	void inlineProductions();
	/// Inlines productions with specified head.
	void inlineProductions(Token head);
	/// todo: erase productions + actions, tokens in parameters,
	void eraseToken(Token token);
	/// todo: breadth-first search incl. lparams in actions
	void expandActionParameters();
	/// todo:
	void expandParameter(std::basic_string<Token>& parameter, const std::basic_string<Token>& body);
	/// todo:
	std::basic_string<Token> getPath(Token parent, Token child);

	//int getNonTerminal(const std::string& name);
	Token getToken(const std::string& name);
	void addTerminal(Token token, const std::string& regex);
	int newToken(const std::string& name);
	//bool tokenNameExists(const std::string& name);
	void addAction();

	std::string getTokenName(Token token) const;
	std::string createTokenName(const std::basic_string<Token>& body) const;

	void deleteTokens();
	void deleteBodies();

	void printLine();

	GrammarParserCtrl& self()
	{
		return *this;
	};


	GrammarParser parser;
	GrammarTokenizer fsm;

	FILE *infile;
	std::string filename;
	int lineCounter;
	Token nextToken;
	std::vector<char> buffer;
	std::vector<const char*> tokens;
	int validBuffer;

	std::map<std::string, AnnotatedGrammar> grammars;
	//std::map<std::string, Nfa<Char> > groups;
	//std::map<std::string, Token> nonTerminals;
	//std::map<std::string, Terminal> terminals;

	//Nfa<Char> currentNfa;
	//Nfa<Char> currentLookahead;
	AnnotatedGrammar currentGrammar;
	Token currentStartSymbol;
	std::map<std::string, Token> currentTokens;
	//std::string currentRegex;
	Action currentAction;
	ActionList currentActionList;
	Parameter currentParameter;

	//std::basic_string<Token> currentBody, currentBodyExpr;
	std::vector<std::basic_string<Token> > currentBodies;
};

#endif
