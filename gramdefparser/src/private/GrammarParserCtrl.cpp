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

#include "../GrammarParserCtrl.hpp"
#include "../RegexParserCtrl.hpp"

#include "../../gen/GrammarTokens.hpp"

#include <sstream>
#include <algorithm>
#include <deque>
#include <cassert>
#include <cstring>

using namespace GrammarTokens;

GrammarParserCtrl::AnnotatedGrammar::~AnnotatedGrammar()
{
}

GrammarParserCtrl::GrammarParserCtrl()
: parser(self())
, infile(0)
, lineCounter(0)
, nextToken(TOK_END)
, validBuffer(0)
, currentStartSymbol(0)
{
   buffer.reserve(64);
}

GrammarParserCtrl::~GrammarParserCtrl()
{
   deleteTokens();
   deleteBodies();
}

void GrammarParserCtrl::deleteTokens()
{
   for (size_t i = 0; i < tokens.size(); ++i)
      delete[] tokens[i];
   tokens.clear();
}

void GrammarParserCtrl::deleteBodies()
{
   currentBodies.clear();
}

void GrammarParserCtrl::parse(FILE* file, const std::string& filename)
{
   this->infile = file;
   this->filename = filename;
   this->lineCounter = 1;
   this->buffer.clear();
   this->validBuffer = 0;
   this->currentStartSymbol = 0;
   deleteTokens();
   deleteBodies();
   this->reset();

   parser.parse();
}

GrammarParserCtrl::Action::Action(const std::string& name)
: function(name) { }

//void GrammarParserCtrl::finish(){}

const std::map<std::string, GrammarParserCtrl::AnnotatedGrammar>& GrammarParserCtrl::getGrammars() const
{
   return grammars;
}

std::string GrammarParserCtrl::getTokenName(Token token) const
{
   std::map<Token, std::string>::const_iterator iter = currentGrammar.tokenNames.find(token);
   if (iter != currentGrammar.tokenNames.end()
      && !iter->second.empty()
      && iter->second.find_first_of("\"/") == std::string::npos)
   {
      return iter->second;
   }
   else
   {
      std::stringstream tokstr;
      tokstr << token;
      return tokstr.str();
   }
}

std::string GrammarParserCtrl::createTokenName(const std::basic_string<Token>& body) const
{
   std::string result;
   for (size_t i = 0; i < body.size(); ++i)
   {
      result.append(1, '_');
      result.append(getTokenName(body[i]));
   }
   return result;
}

void GrammarParserCtrl::grammar(const char* name)
{
   currentGrammar.grammar.setStartSymbol(currentStartSymbol);
   inlineProductions();
   expandActionParameters();
   this->grammars.insert(std::make_pair(std::string(name), currentGrammar));

   currentGrammar = AnnotatedGrammar();
   currentTokens.clear();
   currentStartSymbol = 0;
}

const char* GrammarParserCtrl::alternative(const char* bodyNo1, const char* bodyNo2)
{
   const std::basic_string<Token>& body1 = currentBodies[reinterpret_cast<size_t> (bodyNo1)];
   const std::basic_string<Token>& body2 = currentBodies[reinterpret_cast<size_t> (bodyNo2)];

   Token newHead = newToken(std::string("_alt") + createTokenName(body1) + "_or" + createTokenName(body2));

   currentGrammar.grammar.addProduction(newHead, body1);
   action();
   currentGrammar.grammar.addProduction(newHead, body2);
   action();

   /*std::set<Token> tokensChanged;
   for (size_t i = 0; i < body1.size(); ++i)
   {
      Token token = body1[i];
      currentProdTokens[token].insert(0, 1, newHead);
      tokensChanged.insert(token);
   }

   for (size_t i = 0; i < body2.size(); ++i)
   {
      Token token = body2[i];
      if (tokensChanged.count(token) == 0)
         currentProdTokens[token].insert(0, 1, newHead);
   }*/

   int bodyNo = static_cast<int> (currentBodies.size());
   currentBodies.push_back(std::basic_string<Token > (1, newHead));
   return reinterpret_cast<const char*> (bodyNo);
}

/*void GrammarParserCtrl::newBody()
{
   currentBody.swap(currentBodyExpr);
   currentBodyExpr.clear();
}*/

void GrammarParserCtrl::addTerm(const char* bodyNo, const char* id)
{
   std::basic_string<Token>& body = currentBodies[reinterpret_cast<size_t> (bodyNo)];
   Token token = static_cast<Token>(reinterpret_cast<size_t>(id));
   body.append(1, token);
}

const char* GrammarParserCtrl::newBodyExpr(const char* id)
{
   Token token = static_cast<Token>(reinterpret_cast<size_t>(id));
   int bodyNo = static_cast<int> (currentBodies.size());
   currentBodies.push_back(std::basic_string<Token > (1, token));
   return reinterpret_cast<const char*> (bodyNo);
}

const char* GrammarParserCtrl::optional(const char* bodyNo)
{
   const std::basic_string<Token>& body = currentBodies[reinterpret_cast<size_t> (bodyNo)];
   Token newHead = newToken(std::string("_opt") + createTokenName(body));

   currentGrammar.grammar.addProduction(newHead, body);
   action();
   currentGrammar.grammar.addProduction(newHead, std::basic_string<Token > ());
   action();

   /*for (size_t i = 0; i < body.size(); ++i)
   {
      Token token = body[i];
      currentProdTokens[token].insert(0, 1, newHead);
   }*/

   return reinterpret_cast<const char*> (newHead);
}

const char* GrammarParserCtrl::list(const char* bodyNo)
{
   std::basic_string<Token> body = currentBodies[reinterpret_cast<size_t> (bodyNo)];
   Token newHead = newToken(std::string("_list") + createTokenName(body));

   body.insert(0, 1, newHead);
   currentGrammar.grammar.addProduction(newHead, body);
   action();
   currentGrammar.grammar.addProduction(newHead, std::basic_string<Token > ());
   action();

   /*for (size_t i = 0; i < body.size(); ++i)
   {
      Token token = body[i];
      currentProdTokens[token].insert(0, 1, newHead);
   }*/

   return reinterpret_cast<const char*> (newHead);
}

const char* GrammarParserCtrl::newSubExpr(const char* bodyNo)
{
   const std::basic_string<Token>& body = currentBodies[reinterpret_cast<size_t> (bodyNo)];
   Token newHead = newToken(std::string("_sub") + createTokenName(body));

   currentGrammar.grammar.addProduction(newHead, body);
   action();

   /*for (size_t i = 0; i < body.size(); ++i)
   {
      Token token = body[i];
      currentProdTokens[token].insert(0, 1, newHead);
   }*/

   return reinterpret_cast<const char*> (newHead);
}

const char* GrammarParserCtrl::endToken()
{
   return 0;
}

void GrammarParserCtrl::regexGroup(const char* name)
{
   printLine();
   fprintf(stderr, "regex groups not implemented; ignoring %s", name);
}

void GrammarParserCtrl::production(const char* head, const char* bodyNo)
{
   Token token = getToken(head);
   if (currentStartSymbol == 0) currentStartSymbol = token;
   const std::basic_string<Token>& body = currentBodies[reinterpret_cast<size_t> (bodyNo)];
   currentGrammar.grammar.addProduction(token, body);
   currentGrammar.tokenNames[token] = head;
   deleteBodies();
   currentActionList.clear();
   //currentProdTokens.clear();
}

void GrammarParserCtrl::production(const char* head)
{
   Token token = getToken(head);
   if (currentStartSymbol == 0) currentStartSymbol = token;
   currentGrammar.grammar.addProduction(token, std::basic_string<Token > ());
   currentGrammar.tokenNames[token] = head;
   currentActionList.clear();
   //currentProdTokens.clear();
}

void GrammarParserCtrl::action()
{
   currentGrammar.actions.push_back(currentActionList);
   currentActionList.clear();
}

void GrammarParserCtrl::ignoreProduction(const char* bodyNo)
{
   const std::basic_string<Token>& body = currentBodies[reinterpret_cast<size_t> (bodyNo)];
   currentGrammar.grammar.addProduction(Gram::endToken, body);
   deleteBodies();
}

void GrammarParserCtrl::token(const char* name, const char* regex)
{
   bool regexIsNew = currentTokens.end() == currentTokens.find(regex);
   bool nameIsNew = currentTokens.end() == currentTokens.find(name);

   if (!nameIsNew && !regexIsNew)
   {
      printLine();
      fprintf(stderr, "redefinition of tokens %s", name);
   }

   Token token = regexIsNew ? (nameIsNew ? newToken(regex) : getToken(name)) : getToken(regex);

   //printf("%s = %s = %i\n", name, regex, token);

   if (regexIsNew)
   {
      currentTokens[std::string(regex)] = token;
      addTerminal(token, regex);
   }
   if (nameIsNew) currentTokens[std::string(name)] = token;

   currentGrammar.tokenNames[token] = name;
}

const char* GrammarParserCtrl::terminal(const char* regex)
{
   std::string tokenStr(regex);
   bool isNew = currentTokens.end() == currentTokens.find(tokenStr);
   Token token = getToken(tokenStr);
   if (isNew) addTerminal(token, tokenStr);

   //printf("terminal %s = %i\n", regex, token);

   //currentBody.push_back(token);
   return reinterpret_cast<const char*> (token);
}

const char* GrammarParserCtrl::nonTerminal(const char* name)
{
   Token token = getToken(name);
   //printf("nonterminal %s = %i\n", name, token);
   currentGrammar.tokenNames[token] = name;
   //currentBody.push_back(token);
   return reinterpret_cast<const char*> (token);
}

GrammarParserCtrl::Token GrammarParserCtrl::getToken(const std::string& name)
{
   std::map<std::string, int>::const_iterator i = currentTokens.find(name);
   if (i != currentTokens.end()) return i->second;
   else return currentTokens[name] = newToken(name);
}

/*int GrammarParserCtrl::getNonTerminal(const std::string& name)
{
   std::map<std::string, int>::const_iterator i = nonTerminals.find(name);
   if (i != nonTerminals.end()) return i->second;
   else return nonTerminals[name] = newToken(name);
}*/

void GrammarParserCtrl::function(const char* name)
{
   currentAction.function = name;
   currentActionList.push_back(currentAction);
   currentAction = Action();
}

void GrammarParserCtrl::assignment()
{
   currentAction.parameter.push_back(currentParameter);
   currentActionList.push_back(currentAction);
   currentAction = Action();
   currentParameter.clear();
}

void GrammarParserCtrl::lparameter()
{
   currentAction.lparameter.swap(currentParameter);
   currentParameter.clear();
}

/*static void printTokens(const std::basic_string<int>& tokens)
{
   for (size_t i = 0; i < tokens.size(); ++i)
   {
      printf("%i ", tokens[i]);
   }
   printf("\n");
}*/

void GrammarParserCtrl::parameter()
{
   //printTokens(currentParameter);
   currentAction.parameter.push_back(currentParameter);
   currentParameter.clear();
}

void GrammarParserCtrl::concatParameter(const char* part)
{
   currentParameter.append(1, getToken(part));
}

void GrammarParserCtrl::newLine()
{
   ++lineCounter;
}

GrammarParserCtrl::Token GrammarParserCtrl::newToken(const std::string& name)
{
   Token token = currentGrammar.numOfTokens;
   if (!name.empty()) currentGrammar.tokenNames.insert(std::make_pair(token, name));
   currentGrammar.numOfTokens = token + 1;
   return token;
}

void GrammarParserCtrl::addTerminal(Token token, const std::string& regex)
{
   Nfa<Char> nfa;

   switch (regex[0])
   {
      case '/':
      {
         std::string str(regex.substr(1, regex.length() - 2));
         RegexParserCtrl parserCtrl(str);
         int end = parserCtrl.parse();
         if (static_cast<size_t> (end) < str.length())
         {
            printLine();
            fprintf(stderr, "%s not parsed completly, aborted at `%s'\n", regex.c_str(), str.substr(end).c_str());
         }
         else nfa = parserCtrl.getNfa();

         //currentLookahead = parserCtrl.getLookahead();
         break;
      }
      case '"':
      {
         std::string unquotedRegex = regex.substr(1, regex.length() - 2);
         nfa = Nfa<Char>(replaceEscapes(unquotedRegex));
         //currentLookahead = Nfa<char>();
         break;
      }
      default:
         printLine();
         fprintf(stderr, "terminal %s not recognized\n", regex.c_str());
         break;
   }

   nfa.set_output(token);

   currentGrammar.nfas.push_back(nfa);
}

GrammarParserCtrl::String GrammarParserCtrl::replaceEscapes(std::string& str)
{
   bool escaped = false;
   String result(str.length(), '\0');
   String::iterator iWrite = result.begin();
   for (std::string::const_iterator iRead = str.begin(); iRead != str.end(); ++iRead)
      if (!escaped && *iRead == '\\') escaped = true;
      else
      {
         char ch = *iRead;
         if (escaped) switch (ch)
            {
               case 'a': ch = '\a';
                  break;
               case 'b': ch = '\b';
                  break;
               case 'f': ch = '\f';
                  break;
               case 'n': ch = '\n';
                  break;
               case 'r': ch = '\r';
                  break;
               case 't': ch = '\t';
                  break;
               case 'v': ch = '\v';
                  break;
            }
         escaped = false;
         *iWrite = (Char)ch;
         ++iWrite;
      }
   result.resize(iWrite - result.begin());
   return result;
}

int GrammarParserCtrl::shift(int context)
{
   buffer.erase(buffer.begin(), buffer.begin() + validBuffer);
   validBuffer = 0;
   nextToken = TOK_END;

   fsm.begin(context);

   int i = 0;
   int len = static_cast<int> (buffer.size());

   while (len == 0 || fsm.process(&buffer[i], len))
   {
      i += len;
      buffer.resize(static_cast<size_t> (i) < buffer.capacity() ? buffer.capacity() : buffer.capacity() * 2);
      len = fread(&buffer[i], 1, buffer.size() - i, infile);
      buffer.resize(i + len);
      if (len == 0) break;
   }

   validBuffer = fsm.end(this->nextToken);

   ParserLog::shift(nextToken, validBuffer, buffer.empty() ? 0 : &buffer.front());
   //printf("%i[%i] ", nextToken, validBuffer);

   return this->nextToken;
}

void GrammarParserCtrl::reduce(int prod)
{
   const GrammarGrammar::Production& production = GrammarGrammar::getProduction(prod);
   ParserLog::reduce(production.head, production.len);
   //printf("reduce %i[%i]\n%i: %i ", prod, production.len, ParserLog::stackSize(), production.head);
}

int GrammarParserCtrl::error(size_t n, const int* expected)
{
   printLine();
   fprintf(stderr, "error reading token %i", this->nextToken);
   if (0 < validBuffer)
   {
      fprintf(stderr, ": '");
      fwrite(&buffer.front(), 1, validBuffer, stderr);
      fprintf(stderr, "'\n");
   }
   if (!buffer.empty())
   {
      fprintf(stderr, " at: '");
      char nl[] = "\n\r";
      std::vector<char>::const_iterator iEnd = std::find_first_of(buffer.begin(), buffer.end(), &nl[0], &nl[2]);
      fwrite(&buffer.front(), 1, iEnd - buffer.begin(), stderr);
      fprintf(stderr, "'\n");
   }

   fprintf(stderr, "\texpected:");
   for (size_t i = 0; i < n; ++i)
      fprintf(stderr, i > 0 ? ", %s" : " %s", GrammarTokens::getTokenName(expected[i]));
   fprintf(stderr, "\n");

   ParserLog::reduce(0, 1);

   return 1;
}

void GrammarParserCtrl::printLine()
{
   fprintf(stderr, "%s(%i): ", filename.c_str(), lineCounter);
}

const char* GrammarParserCtrl::string()
{
   if (buffer.empty()) return 0;
   else
   {
      char* str = new char[validBuffer + 1];
      str[validBuffer] = '\0';
      strncpy(str, &buffer.front(), validBuffer);
      tokens.push_back(str);
      return str;
   }
}

void GrammarParserCtrl::release(const char*) { }

void GrammarParserCtrl::inlineProductions()
{
   std::map<Token, std::set<Gram::ProductionNo> > references;
   // get references
   for (Gram::ProductionNo prodNo = 0; prodNo < currentGrammar.grammar.getProductions().size(); ++prodNo)
      if (!currentGrammar.grammar.isIgnoreProduction(prodNo))
      {
         const std::basic_string<Token>& body = currentGrammar.grammar.getProductions()[prodNo].body();
         for (size_t i = 0; i < body.size(); ++i)
            references[body[i]].insert(prodNo);
      }

   // look for non-terminal tokens referenced only once
   std::set<Token> inlTokens;
   for (std::map<Token, std::set<Gram::ProductionNo> >::const_iterator iRef = references.begin(); iRef != references.end(); ++iRef)
      if (!currentGrammar.grammar.terminal(iRef->first) && iRef->second.size() == 1)
      {
         const std::set<Gram::ProductionNo>& prods = currentGrammar.grammar.getProductions(iRef->first);
         bool noActions = true;
         bool recursive = false;
         for (std::set<Gram::ProductionNo>::const_iterator iProdNo = prods.begin(); iProdNo != prods.end(); ++iProdNo)
         {
            if (currentGrammar.grammar.getProductions()[*iProdNo].body().find(iRef->first) != std::basic_string<Token>::npos)
               recursive = true;
            if (*iProdNo < currentGrammar.actions.size() && !currentGrammar.actions[*iProdNo].empty())
               noActions = false;
         }
         // only inline productions with no actions and non recursive
         if (noActions && !recursive)
         {
            inlTokens.insert(iRef->first);
         }
      }

   for (std::set<Token>::const_iterator iTok = inlTokens.begin(); iTok != inlTokens.end(); ++iTok)
      inlineProductions(*iTok);
}

void GrammarParserCtrl::inlineProductions(Token head)
{
   const std::set<Gram::ProductionNo>& inlineProds = currentGrammar.grammar.getProductions(head);
   const Gram::Productions& allProds = currentGrammar.grammar.getProductions();
   std::set<Gram::ProductionNo> prodsToDelete(inlineProds);

   for (Gram::ProductionNo p = 0; p < allProds.size(); ++p)
   {
      const Gram::Production& production = allProds[p];
      std::vector<std::basic_string<Token> > newBodies;
      for (size_t i = 0; i < production.body().size(); ++i)
         if (production.body()[i] == head)
         {
            for (std::set<Gram::ProductionNo>::const_iterator iInlProd = inlineProds.begin(); iInlProd != inlineProds.end(); ++iInlProd)
            {
               std::basic_string<Token> newBody = production.body();
               const std::basic_string<Token>& inlBody = allProds[*iInlProd].body();
               if (inlBody.find(head) != std::basic_string<Token>::npos)
               {
                  fprintf(stderr, "%i is recursive\n", head);
               }
               else
               {
                  //assert(inlBody.find(head) == std::basic_string<Token>::npos);
                  newBody.replace(i, 1, inlBody);
                  newBodies.push_back(newBody);
               }
            }
            prodsToDelete.insert(p);
         }

      Token newHead = production.head();
      for (size_t n = 0; n < newBodies.size(); ++n)
      {
         currentGrammar.grammar.addProduction(newHead, newBodies[n]);
         ActionList actions = currentGrammar.actions[p];
         currentGrammar.actions.push_back(actions);
      }
   }

   for (std::set<Gram::ProductionNo>::const_reverse_iterator iDel = prodsToDelete.rbegin(); iDel != prodsToDelete.rend(); ++iDel)
   {
      currentGrammar.grammar.delProduction(*iDel);
      currentGrammar.actions.erase(currentGrammar.actions.begin() + *iDel);
   }
}

void GrammarParserCtrl::expandActionParameters()
{
   for (size_t al = 0; al < currentGrammar.actions.size(); ++al)
      for (size_t a = 0; a < currentGrammar.actions[al].size(); ++a)
      {
         Gram::Production prod = currentGrammar.grammar.getProductions()[al];
         Action& action = currentGrammar.actions[al][a];
         for (size_t p = 0; p < action.parameter.size(); ++p)
            expandParameter(action.parameter[p], prod.body());
      }
}

void GrammarParserCtrl::expandParameter(std::basic_string<Token>& parameter, const std::basic_string<Token>& body)
{
   std::basic_string<Token> result;
   if (!parameter.empty())
   {
      size_t p = 0;
      while(result.empty() && p < parameter.size())
      {
         for (size_t b = 0; b < body.size(); ++b)
         {
            if (body[b] == parameter[p])
            {
               result = parameter.substr(p, 1);
               break;
            }
            std::basic_string<Token> path = getPath(body[b], parameter[p]);
            if (!path.empty() && (result.empty() || path.length() + 1 < result.length()))
               result = std::basic_string<Token>(1, body[b]) + path;
         }
         ++p;
      }

      if (!result.empty())
      {

         for (; p < parameter.size(); ++p)
         {
            Token prev = *result.rbegin();
            result.append(getPath(prev, parameter[p]));
         }
      }

      if (result.empty() || *result.rbegin() != *parameter.rbegin())
      {
         currentGrammar.unresolvedParameters.push_back(parameter);
      }
   }
   parameter.swap(result);
}

std::basic_string<GrammarParserCtrl::Token> GrammarParserCtrl::getPath(Token parent, Token child)
{
   std::set<Token> visited;
   std::deque<std::basic_string<Token> > paths(1, std::basic_string<Token>(1, parent));

   std::basic_string<Token> result;

   do
   {
      const std::basic_string<Token>& currentPath = paths.front();
      Token nextHead = currentPath[currentPath.length() - 1];
      if (nextHead == child)
      {
         result = currentPath.substr(1);
         break;
      }
      visited.insert(nextHead);
      const std::set<Gram::ProductionNo>& prods = currentGrammar.grammar.getProductions(nextHead);
      for (std::set<Gram::ProductionNo>::const_iterator iProd = prods.begin(); iProd != prods.end(); ++iProd)
      {
         const Gram::Production& production = currentGrammar.grammar.getProductions()[*iProd];
         for (size_t a = 0; a < currentGrammar.actions[*iProd].size(); ++a)
         {
            const std::basic_string<Token>& lparam = currentGrammar.actions[*iProd][a].lparameter;
            if (!lparam.empty() && child == lparam[0])
               paths.push_back(currentPath + child);
         }

         for (size_t b = 0; b < production.body().size(); ++b)
         {
            Token tok = production.body()[b];
            if (visited.find(tok) == visited.end())
               paths.push_back(currentPath + tok);
         }
      }
      paths.pop_front();
   } while (!paths.empty());

   return result;
}


