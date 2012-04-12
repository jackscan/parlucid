#include "../parsercontext.hpp"

#include "../../../utils/src/helper.hpp"

#include <limits>
#include <cassert>

template<typename Token>
static bool tokenConflicts(Token tok, const BitSequence<Token>& tokens, const std::map<Token, BitSequence<Token> >& prefixes)
{
	if (!tokens.get(tok))
	{
		typename std::map<Token, BitSequence<Token> >::const_iterator iPrefixes = prefixes.find(tok);
		return (iPrefixes != prefixes.end() && (iPrefixes->second && tokens));
	}
		/*for (typename std::set<Token>::const_iterator iOtherTok = tokens.begin(); iOtherTok != tokens.end(); ++iOtherTok)
		{
			
			
			//if (prefixes[static_cast<size_t>(tok - minToken)][static_cast<size_t>(*iOtherTok - minToken)])
			//	return true;
		}*/
	return false;
}

template<typename Token>
static bool tokensConflict(const BitSequence<Token>& lhs, const BitSequence<Token>& rhs, const std::map<Token, BitSequence<Token> >& prefixes)
{
	for (Token iTok = lhs.begin(); iTok < lhs.end(); ++iTok)
		if (lhs.get(iTok) && tokenConflicts(iTok, rhs, prefixes))
			return true;
	for (Token iTok = rhs.begin(); iTok < rhs.end(); ++iTok)
		if (rhs.get(iTok) && tokenConflicts(iTok, lhs, prefixes))
			return true;
	return false;
}

namespace
{
	template<typename T>
	struct Context
	{
		BitSequence<T> accepted;
		std::set<typename ParserContext<T>::ContextId> states;
	};
}

template<typename T>
ParserContext<T>::ParserContext(const Table& parsingTable, const std::map<Token, BitSequence<Token> >& prefixes, const std::map<Token, BitSequence<Token> >& subTokens)
{
	std::vector<Context<T> > contexts;
	for (size_t state = 0; state < parsingTable.size(); ++state)
	{
		BitSequence<Token> validTokens;
		for (typename Table::ActionMap::const_iterator iAction = parsingTable[state].begin(); iAction != parsingTable[state].end(); ++iAction)
			validTokens.set(iAction->first);

		// insert subTokens of valid tokens
		for (Token iTok = validTokens.begin(); iTok < validTokens.end(); ++iTok)
			if (validTokens.get(iTok))
			{
				typename std::map<Token, BitSequence<Token> >::const_iterator iSubToks = subTokens.find(iTok);
				if (iSubToks != subTokens.end())
					validTokens |= iSubToks->second;
					//for (typename std::set<Token>::const_iterator iSub = iSubToks->second.begin(); iSub != iSubToks->second.end(); ++iSub)
					//	validTokens.insert(*iSub);
			}

		// find non conflicting context to insert state in to.
		bool conflict = true;
		for (size_t iCtxt = 0; iCtxt < contexts.size(); ++iCtxt)
		{
			Context<T>& context = contexts[iCtxt];
			conflict = tokensConflict(context.accepted, validTokens, prefixes);
			
			if (!conflict)
			{	
				//set_union_add(context.accepted, validTokens);
				context.accepted |= validTokens;
				context.states.insert(state);
				break;
			}
		}

		if (conflict)
		{
			Context<T> newContext;
			newContext.accepted = validTokens;
			newContext.states.insert(state);
			contexts.push_back(newContext);
		}
	}

	ContextId invalidContext = std::numeric_limits<ContextId>::max();
	this->parsingTableContexts.resize(parsingTable.size(), invalidContext);
	this->acceptedTokens.resize(contexts.size());

	for (size_t iCtxt = 0; iCtxt < contexts.size(); ++iCtxt)
	{
		const Context<T>& context = contexts[iCtxt];
		for (typename std::set<ContextId>::const_iterator iState = context.states.begin(); iState != context.states.end(); ++iState)
		{
			assert(this->parsingTableContexts[*iState] == invalidContext);
			this->parsingTableContexts[*iState] = static_cast<ContextId>(iCtxt);
		}

		this->acceptedTokens[iCtxt] = context.accepted;
	}
}

template<typename T>
typename ParserContext<T>::ContextId ParserContext<T>::getParsingTableContext(size_t i) const
{
	return this->parsingTableContexts[i];
}

template<typename T>
const BitSequence<T>& ParserContext<T>::getAcceptedTokens(ContextId context) const
{
	return this->acceptedTokens[context];
}

template<typename T>
size_t ParserContext<T>::getNumberOfContexts() const
{
	return this->acceptedTokens.size();
}

template class ParserContext<int>;
template class ParserContext<char>;
