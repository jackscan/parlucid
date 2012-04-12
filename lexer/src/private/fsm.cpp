#include "../fsm.hpp"
#include "../../../utils/src/helper.hpp"

#include <queue>
#include <functional>
#include <iostream>
#include <cassert>

template<typename T, typename O>
const typename Fsm<T, O>::State Fsm<T, O>::ERROR_STATE = static_cast<typename Fsm<T, O>::State>(-1);

template<typename T, typename O> Fsm<T, O>::Fsm()
: tokenRange(static_cast<Token>(1), static_cast<Token>(0))
{
}

template<typename T, typename O> Fsm<T, O>::Fsm(const Nfa<Char, Token>& nfa)
: tokenRange(static_cast<Token>(1), static_cast<Token>(0))
{
	this->create(nfa);
}


template<typename T, typename O> const BitSequence<O>& Fsm<T, O>::next(typename Fsm<T, O>::State& state, const T& symbol) const
{
	static const BitSequence<Token> emptyOutput;
	T c = alphabet.lower_bound(symbol);
	if( c > symbol ) return state = ERROR_STATE, emptyOutput;
	if(state < 0 || static_cast<State>(transition_table.size()) <= state) return state = ERROR_STATE, emptyOutput;
	const std::map<T,Transition>& row = transition_table[state];
	typename std::map<T,Transition>::const_iterator icol = row.find(c);
	if( icol == row.end() ) return state = ERROR_STATE, emptyOutput;
	state = icol->second.nextState;
	return icol->second.output;
}

template<typename T, typename O> bool Fsm<T, O>::error(typename Fsm<T, O>::State state) const
{
	return state == ERROR_STATE;
}

template<typename T, typename O> const Alphabet<T>& Fsm<T, O>::input() const
{
	return this->alphabet;
}

template<typename T, typename O> bool Fsm<T, O>::isAcceptingState(State state) const
{
	return (0 <= state && static_cast<size_t>(state) < transition_table.size() && !transition_table[state].empty());
}

template<typename T, typename O> void Fsm<T, O>::getAcceptedSymbols(State state, std::set<Char>& symbols) const
{
	if (0 <= state && static_cast<size_t>(state) < transition_table.size())
	{
		const std::map<Char,Transition>& transitions = transition_table[state];
		for (typename std::map<Char,Transition>::const_iterator i = transitions.begin(); i != transitions.end(); ++i)
		{
			symbols.insert(i->first);
		}
	}
}

template<typename T, typename O> void Fsm<T, O>::getOutput(BitSequence<Token>& tokens) const
{
	//this->initOutputArray(tokens);

	for (State state = 0; state < static_cast<State>(this->transition_table.size()); ++state)
	{
		const std::map<Char,Transition>& transitions = transition_table[state];
		for (typename std::map<Char,Transition>::const_iterator i = transitions.begin(); i != transitions.end(); ++i)
			tokens |= i->second.output;
			//std::transform(tokens.begin(), tokens.end(), i->second.output.begin(), tokens.begin(), std::logical_or<bool>());
			//this->fillOutputArray(tokens, i->second.output);
			//tokens[
			//for (typename std::vector<bool>::const_iterator o = i->second.output.begin(); o != i->second.output.end(); ++o)
			//	tokens[
	}
}

/*template<typename T, typename O> O Fsm<T, O>::getToken(size_t index) const
{
	return static_cast<Token>(index) + this->getMinOutput();
}

template<typename T, typename O> size_t Fsm<T, O>::getIndex(Token token) const
{
	return static_cast<size_t>(token - this->getMinOutput());
}*/

/*template<typename T, typename O> O Fsm<T, O>::getMinOutput() const
{
	return this->tokenRange.empty() ? Token() : this->tokenRange.first;
}

template<typename T, typename O> O Fsm<T, O>::getMaxOutput() const
{
	return this->tokenRange.empty() ? Token() : this->tokenRange.last;
}

template<typename T, typename O> size_t Fsm<T, O>::getOutputSize() const
{
	return this->tokenRange.empty() ? 0 : this->tokenRange.last - this->tokenRange.first + 1;
}*/

template<typename T, typename O> typename Fsm<T, O>::State Fsm<T, O>::getNumberOfStates() const
{
	return static_cast<State>(this->transition_table.size());
}

template<typename T, typename O> void Fsm<T, O>::create(const Nfa<T, Token>& nfa)
{
	this->transition_table.clear();
	this->alphabet = nfa.input();
	std::set<Token> output = nfa.output();
	this->tokenRange = output.empty() ? Interval<Token>(1, 0) : Interval<Token>(*output.begin(), *output.rbegin());
	//this->emptyOutput.clear();
	//this->emptyOutput.resize(this->tokenRange.last - this->tokenRange.first + 1, false);

	std::set<Char> lbs = this->alphabet.lower_bound();
	
	std::vector<std::set<State> > states( 1 );
	states[0].insert( 0 );

	//if( nfa.final(0) ) final_states[0] = nfa.output(0);
	for(unsigned s = 0; s < states.size(); ++s )
	{
		std::map<T,std::pair<BitSequence<Token>,std::set<State> > > transitions;
		typename std::set<Char>::const_iterator lbi;
		for(lbi = lbs.begin(); lbi != lbs.end(); ++lbi)
		{
			std::set<State> fs = nfa.next(states[s], *lbi);
			if (!fs.empty())
			{
				//this->initOutputArray(transitions[*lbi].first);
				//this->fillOutputArray(transitions[*lbi].first, nfa.output(fs));
				transitions[*lbi].first = nfa.output(fs);
				transitions[*lbi].second = fs;
			}
		}

		transition_table.resize(s+1);

		typename std::map<T,std::pair<BitSequence<Token>,std::set<State> > >::const_iterator it;
		for(it = transitions.begin(); it != transitions.end(); ++it)
		{
			// find existing set with same states
			unsigned i;
			for(i = 0; i < states.size(); ++i)
				if(it->second.second == states[i])
					break;

			transition_table[s][it->first].nextState = i;
			transition_table[s][it->first].output = it->second.first;
			if(i == states.size()) states.push_back(it->second.second);
		}
	}
}

template<typename T, typename O> void Fsm<T, O>::reduce()
{
	// remove transitions without output and to states without outgoing transitions
	/*for (int changed = 1; 0 < changed; --changed)
		for (size_t i = 0; i < transition_table.size(); ++i)
		{
			std::map<Char, Transition> new_transitions;
			for (typename std::map<Char, Transition>::const_iterator itt = transition_table[i].begin(); itt != transition_table[i].end(); ++itt)
				if (!this->transition_table[itt->second.nextState].empty() || !itt->second.output.empty())
					new_transitions.insert(*itt);
				else changed = 2;
			transition_table[i].swap(new_transitions);
		}*/

	//std::map<State,State> curr_map;
	std::vector<State> curr_map(this->transition_table.size());
	std::map<std::vector<bool>,std::set<State> > final_output_map;
	//typename std::map<State,std::map<T,State> >::const_iterator itt;
	//for(itt = transition_table.begin(); itt != transition_table.end(); itt = transition_table.upper_bound(itt->first))
	//	if(!this->final(itt->first))
	//		curr_map[itt->first] = 0;

	for(size_t i = 0; i < transition_table.size(); ++i)
		curr_map[static_cast<State>(i)] = 0;

	State state_counter = 1, last_state_counter = 0;

	// p = part state; t = terminal; n = new part state
	std::map<std::pair<State,std::map<Char,Transition> >,State> ptpn;
	typename std::map<std::pair<State,std::map<Char,Transition> >,State>::const_iterator iptpn;
	//std::vector<std::map<Char,Transition> > ptp;
	//std::vector<State> ptp_newpart;
	std::vector<State> next_map(this->transition_table.size());

	//const State invalidState = static_cast<State>(transition_table.size());

	while( state_counter != last_state_counter )
	{
		ptpn.clear();
		//ptp.clear();
		//ptp.resize(transition_table.size());

		//ptp.assign(transition_table.size(), std::map<Char,Transition>());
		//ptp_newpart.assign(transition_table.size(), invalidState);

		last_state_counter = state_counter;
		state_counter = 0;

		//typename std::map<State,State>::const_iterator isp;

		// build up part-transition table into ptpn
		//for(isp = curr_map.begin(); isp != curr_map.end(); ++isp)
		for(State state = 0; state < static_cast<State>(curr_map.size()); ++state)
		{
			std::pair<std::pair<State,Transitions>,State> ptp;
			ptp.first.first = curr_map[state];
			ptp.second = state_counter;
			//State part = curr_map[state];
			// building up transition table for this part
			//std::map<Char,Transition> tp;
			typename std::map<Char,Transition>::const_iterator its;
			for(its = transition_table[state].begin(); its != transition_table[state].end(); ++its)
				ptp.first.second[its->first] = Transition(curr_map[its->second.nextState], its->second.output);

			// looking for part with same outgoing transitions
			std::pair<typename std::map<std::pair<State,Transitions>,State>::iterator, bool> insResult =
				ptpn.insert(ptp);
			//if( (iptpn = ptpn.find(ptp)) != ptpn.end() )
			if (!insResult.second)
				// create state-new-part mapping in next map for this state
				next_map[state] = insResult.first->second;
				//next_map[state] = iptpn->second;
			else
				// insert new part-transition pair and assign new part
				next_map[state] = state_counter++;
				//ptpn[ptp] = next_map[state] = state_counter++;

		}

		// swap state->part mapping with new partitions
		curr_map.swap(next_map);
	}

	TransitionTable new_transition_table(state_counter);
	for(iptpn = ptpn.begin(); iptpn != ptpn.end(); ++iptpn)
		new_transition_table[iptpn->first.first] = iptpn->first.second;
		//transition_table.insert( iptpn->first );

	//ptp.resize(state_counter);

	transition_table.swap(new_transition_table);
	//transition_table.swap(ptp);
}

template<typename T, typename O> void Fsm<T, O>::mergeDemux(const std::vector<Fsm<Char, Token> >& fsms)
{
	// todo: find way to make all alphabets equal
	this->alphabet = fsms.empty() ? Alphabet<Char>(): fsms[0].alphabet;
	// todo: handle different outputs
	this->tokenRange = Interval<Token>(1, 0);
	for (typename std::vector<Fsm<Char, Token> >::const_iterator iFsm = fsms.begin(); iFsm != fsms.end(); ++iFsm)
		if (this->tokenRange.empty()) this->tokenRange = iFsm->tokenRange;
		else
		{
			minimize(this->tokenRange.first, iFsm->tokenRange.first);
			maximize(this->tokenRange.last, iFsm->tokenRange.last);
		}

	//this->emptyOutput.reset();
	//this->emptyOutput.resize(this->tokenRange.last - this->tokenRange.first + 1, false);

	this->transition_table.clear();

	std::vector<std::map<State,State> > curr_map(fsms.size());

	State startStates = static_cast<State>(fsms.size());
	State state_counter = startStates;

	for(State f = 0; f < startStates; ++f)
		curr_map[f][static_cast<State>(0)] = f;

	for(State f = 0; f < startStates; ++f)
		for(State i = 1; i < fsms[f].getNumberOfStates(); ++i)
			curr_map[f][static_cast<State>(i)] = startStates;

	// p = part state; t = terminal; n = new part state
	std::map<std::pair<State,std::map<Char,Transition> >,State> ptpn;
	typename std::map<std::pair<State,std::map<Char,Transition> >,State>::const_iterator iptpn;

	//std::vector<bool> tmpOutput;

	bool partsChanged = true;
	do
	{
		ptpn.clear();
		state_counter = startStates;
		partsChanged = false;

		typename std::map<State,State>::const_iterator isp;

		// build up part-transition table into ptpn
		for(size_t f = 0; f < fsms.size(); ++f)
		{
			std::map<State,State> next_map;
			for(isp = curr_map[f].begin(); isp != curr_map[f].end(); ++isp)
			{
				State state = isp->first;
				State part = isp->second;
				// building up transition table for this part
				std::map<Char,Transition> tp;
				typename std::map<Char,Transition>::const_iterator its;
				for(its = fsms[f].transition_table[state].begin(); its != fsms[f].transition_table[state].end(); ++its)
					tp[its->first] = Transition(curr_map[f][its->second.nextState], its->second.output);
					//tp[its->first] = Transition(curr_map[f][its->second.nextState], transformOutput(its->second.output, fsms[f].getMinOutput() - this->getMinOutput(), tmpOutput));

				// looking for part with same outgoing transitions
				if( (iptpn = ptpn.find(std::make_pair(part,tp))) != ptpn.end() )
					// create state-new-part mapping in next map for this state
					next_map[state] = iptpn->second;
				else
				{
					// insert new part-transition pair and assign new part
					State newPart = state > 0 ? state_counter++ : static_cast<State>(f);
					ptpn[std::make_pair(part,tp)] = next_map[state] = newPart;
					partsChanged = partsChanged || newPart != part;
				}
			}
			// swap state->part mapping with new partitions
			curr_map[f].swap(next_map);
		}
	}
	while(partsChanged);

	this->transition_table.resize(state_counter);
	for(iptpn = ptpn.begin(); iptpn != ptpn.end(); ++iptpn)
		this->transition_table[iptpn->first.first] = iptpn->first.second;
}

template<typename T, typename O> void Fsm<T, O>::getStatePrefixes(std::map<State, BitSequence<Token> >& prefixes) const
{
	//prefixes.clear();
	//prefixes.resize(this->getNumberOfStates());
	//for (std::vector<std::vector<bool> >::iterator ip = prefixes.begin(); ip != prefixes.end(); ++ip)
	//	initOutputArray(*ip);
	//std::for_each(prefixes.begin(), prefixes.end(), std::bind1st(std::mem_fun<void,Fsm<T,O>,std::vector<bool>&>(&Fsm<T,O>::initOutputArray), *this));
	std::set<State> current; current.insert(0);
	while (!current.empty())
	{
		State state = *current.begin();
		current.erase(state);

		const BitSequence<Token>& currentPrefixes = prefixes[state];

		// traverse outgoing transitions
		for (typename std::map<Char, Transition>::const_iterator iNext = this->transition_table[state].begin(); iNext != this->transition_table[state].end(); ++iNext)
		{
			BitSequence<Token>& nextPrefixes = prefixes[iNext->second.nextState];
			BitSequence<Token> oldPrefixes = nextPrefixes;
			// add transition output and prefixes of source state to next states prefixes
			//set_union_add(set_union_add(nextPrefixes, iNext->second.output), currentPrefixes);
			//std::transform(nextPrefixes.begin(), nextPrefixes.end(), iNext->second.output.begin(), nextPrefixes.begin(), std::logical_or<bool>());
			//std::transform(nextPrefixes.begin(), nextPrefixes.end(), currentPrefixes.begin(), nextPrefixes.begin(), std::logical_or<bool>());
			(nextPrefixes |= iNext->second.output) |= currentPrefixes;
			// if next prefixes changed add to current list
			if (oldPrefixes != nextPrefixes) current.insert(iNext->second.nextState);
		}
	}
}

template<typename T, typename O> void Fsm<T, O>::getPrefixAssociations(std::map<Token, BitSequence<Token> >& prefixes) const
{
	std::map<State, BitSequence<Token> > statePrefixes;
	this->getStatePrefixes(statePrefixes);

	//prefixes.resize(this->tokenRange.last - this->tokenRange.first + 1);
	//for (size_t p = 0; p < prefixes.size(); ++p)
	//	this->initOutputArray(prefixes[p]);

	for (State state = 0; static_cast<size_t>(state) < statePrefixes.size(); ++state)
	{
		for (typename std::map<Char, Transition>::const_iterator iNext = this->transition_table[state].begin(); iNext != this->transition_table[state].end(); ++iNext)
			for (Token tok = iNext->second.output.begin(); tok < iNext->second.output.end(); ++tok)
				if (iNext->second.output.get(tok))
				{
					const BitSequence<Token>& currentPrefixes = statePrefixes[state];
					BitSequence<Token>& outputPrefixes = prefixes[tok];
					//std::transform(tokens.begin(), tokens.end(), i->second.output.begin(), tokens.begin(), std::logical_or<bool>());
					// iPrefix > iToken
					//std::transform(currentPrefixes.begin() + iToken + 1, currentPrefixes.end(), prefixes[iToken].begin() + iToken + 1, prefixes[iToken].begin() + iToken + 1, std::logical_or<bool>());
					for (Token tok2 = tok + 1; tok2 < currentPrefixes.end(); ++tok2)
						if (currentPrefixes.get(tok2))
							outputPrefixes.set(tok2);

					for (Token prefix = std::min(iNext->second.output.begin(), currentPrefixes.begin()); prefix < tok; ++prefix)
						if (!iNext->second.output.get(prefix) && currentPrefixes.get(prefix))
							outputPrefixes.set(prefix);

					// also insert current token to prefixes of current transitions smaller output
					for (Token tok2 = 0; tok2 < tok; ++tok2)
						if (iNext->second.output.get(tok2))
							prefixes[tok2].set(tok);
				}




			/*for (typename std::set<Token>::const_iterator iToken = iNext->second.output.begin(); iToken != iNext->second.output.end(); ++iToken)
			{
				for (typename std::set<Token>::const_iterator iPrefix = statePrefixes[state].begin(); iPrefix != statePrefixes[state].end(); ++iPrefix)
					// a prefix is overriden by token at greedy scanning
					if (*iPrefix > *iToken || (*iPrefix < *iToken && iNext->second.output.find(*iPrefix) == iNext->second.output.end()))
						prefixes[*iToken].insert(*iPrefix);

				// also insert current token to prefixes of current transitions smaller output
				for (typename std::set<Token>::const_iterator iToken2 = iNext->second.output.begin(); iToken2 != iToken; ++iToken2)
					prefixes[*iToken2].insert(*iToken);
			}*/

		/*typename std::map<State,std::set<Token> >::const_iterator iOutputs = this->final_states.find(iFinal->first);
		if (iOutputs != this->final_states.end())
			for (typename std::set<Token>::const_iterator iPrefix = iFinal->second.begin(); iPrefix != iFinal->second.end(); ++iPrefix)
				for (typename std::set<Token>::const_iterator iToken = iOutputs->second.begin(); iToken != iOutputs->second.end(); ++iToken)
					if (*iPrefix > *iToken || (*iPrefix < *iToken && iOutputs->second.find(*iPrefix) == iOutputs->second.end()))
						prefixes[*iToken].insert(*iPrefix);*/
	}
}

template<typename T, typename O> void Fsm<T, O>::getPotentialOutputs(std::map<State,BitSequence<Token> >& outputs) const
{
	outputs.clear();

	// create reverse graph and set direct outputs
	std::vector<std::set<State> > revtransitions(this->transition_table.size());
	for (State state = 0; state < static_cast<State>(this->transition_table.size()); ++state)
	{
		BitSequence<Token>& currOutputs = outputs[state];
		for (typename std::map<Char, Transition>::const_iterator iNext = this->transition_table[state].begin(); iNext != this->transition_table[state].end(); ++iNext)
		{
			// set direct outputs
			currOutputs |= iNext->second.output;
			//std::transform(currOutputs.begin(), currOutputs.end(), iNext->second.output.begin(), currOutputs.begin(), std::logical_or<bool>());
			//set_union_add(currOutputs, iNext->second.output);
			// set parent
			//revtransitions[iNext->second.nextState].insert(state);
			revtransitions[iNext->second.nextState].insert(state);
		}
	}

	std::set<State> toVisit;
	for (State state = 0; state < static_cast<State>(this->transition_table.size()); ++state)
		toVisit.insert(state);

	while (!toVisit.empty())
	{
		State state = *toVisit.begin();
		toVisit.erase(toVisit.begin());
		const BitSequence<Token>& currOutputs = outputs[state];
		const std::set<State>& parents = revtransitions[state];
		for (std::set<State>::const_iterator p = parents.begin(); p != parents.end(); ++p)
			if (*p != state)
			{
				BitSequence<Token>& parentsOutputs = outputs[*p];
				if (currOutputs != parentsOutputs)
				{
					parentsOutputs |= currOutputs;
					//std::transform(parentsOutputs.begin(), parentsOutputs.end(), currOutputs.begin(), parentsOutputs.begin(), std::logical_or<bool>());
					toVisit.insert(*p);
				}
				//size_t oldSize = parentsOutputs.size();
				//set_union_add(parentsOutputs, currOutputs);
				//if (oldSize < parentsOutputs.size()) toVisit.insert(*p);
			}
	}
		
	/*
	// set direct outputs
	for (State state = 0; state < static_cast<State>(this->transition_table.size()); ++state)
		for (typename std::map<Char, Transition>::const_iterator iNext = this->transition_table[state].begin(); iNext != this->transition_table[state].end(); ++iNext)
			set_union_add(outputs[state], iNext->second.output);

	

	bool change;
	do
	{
		change = false;
		for (State state = 0; state < static_cast<State>(this->transition_table.size()); ++state)
		{
			std::set<Token>& currentOutputs = outputs[state];
			for (typename std::map<Char, Transition>::const_iterator iNext = this->transition_table[state].begin(); iNext != this->transition_table[state].end(); ++iNext)
			{
				typename std::map<State,std::set<Token> >::iterator nextOutputs = outputs.find(iNext->second.nextState);
				if (nextOutputs != outputs.end())
				{
					size_t oldSize = currentOutputs.size();
					change = change || oldSize < set_union_add(currentOutputs, nextOutputs->second).size();
				}
			}
		}
	}
	while (change);
	*/
}

template<typename T, typename O> void Fsm<T, O>::stripTransitions(State state, const BitSequence<Token>& outputs)
{
	for (typename std::map<Char, Transition>::iterator iTrans = transition_table[state].begin(); iTrans != transition_table[state].end(); ++iTrans)
	{
		// erase all output at this transition not contained in outputs
		//set_intersect(iTrans->second.output, outputs);
		iTrans->second.output &= outputs;
		//std::transform(iTrans->second.output.begin(), iTrans->second.output.end(), outputs.begin(), iTrans->second.output.begin(), std::logical_and<bool>());

		/*typename std::set<Token>::iterator iOutput = iTrans->second.output.begin();
		while (iOutput != iTrans->second.output.end())
			if (outputs.find(*iOutput) == outputs.end())
			{
				typename std::set<Token>::iterator iErase = iOutput;
				++iOutput;
				iTrans->second.output.erase(iErase);
			}
			else ++iOutput;*/
	}
}

template<typename T, typename O> void Fsm<T, O>::stripTransition(Transition& transition, const BitSequence<Token>& outputs)
{
	// erase all output at this transition not contained in outputs
	transition.output &= outputs;
	//typename std::vector<bool>::iterator iOutput = transition.output.begin();
	//std::transform(transition.output.begin(), transition.output.end(), outputs.begin(), transition.output.begin(), std::logical_and<bool>());
	/*while (iOutput != transition.output.end())
		if (outputs.find(*iOutput) == outputs.end())
		{
			typename std::set<Token>::iterator iErase = iOutput;
			++iOutput;
			transition.output.erase(iErase);
		}
		else ++iOutput;*/
}

template<typename T, typename O> void Fsm<T, O>::disambiguate(const std::list<Token>& outputs)
{
//todo: check if transitions from first (original) state should be stripped

	// todo: make outputs.size() copies of all states
	// original is for unaccepted
	// copy n is for accepting <=outputs[n]
	//		or is it for accepting <=outputs[outputs.size()-n]?
	// strip unaccepted to smallest unaccepted

	// make copy of all states and transitions
	State nrOfOldStates = static_cast<State>(transition_table.size());
	// map from 
	std::map<Token, State> submachines;
	transition_table.resize(nrOfOldStates * (1 + outputs.size()));
	//std::set<Token> outputSet(outputs.begin(), outputs.end());
	//std::set<Token> strippedOutputs(outputSet);	
	BitSequence<Token> outputSet;
	for (typename std::list<Token>::const_iterator i = outputs.begin(); i != outputs.end(); ++i)
		outputSet.set(*i);
	//this->initOutputArray(outputSet);
	//this->fillOutputArray(outputSet, outputs);

	BitSequence<Token> strippedOutputs(outputSet);	

	// strip transitions with valid output
	for (State state = 0; state < nrOfOldStates; ++state)
		for (typename std::map<Char, Transition>::iterator iTrans = transition_table[state].begin(); iTrans != transition_table[state].end(); ++iTrans)
			if (iTrans->second.output && strippedOutputs)
				stripTransition(iTrans->second, strippedOutputs);

	typename std::list<Token>::const_reverse_iterator iOut = outputs.rbegin();
	State firstNew = 0;
	while(iOut != outputs.rend())
	{
		firstNew += nrOfOldStates;
		for (State state = 0; state < nrOfOldStates; ++state)
		{
			State copyState = state + firstNew;
			transition_table[copyState] = transition_table[state];
			for (typename std::map<Char, Transition>::iterator iTrans = transition_table[copyState].begin(); iTrans != transition_table[copyState].end(); ++iTrans)
				iTrans->second.nextState += firstNew;
			// strip copied transitions
			stripTransitions(copyState, strippedOutputs);
		}
		submachines[*iOut] = firstNew;
		//strippedOutputs.erase(*iOut);
		strippedOutputs.reset(*iOut);
		++iOut;
	}

	// change destination of old transitions with valid output to copied state.
	for (State state = 0; state < static_cast<State>(transition_table.size()); ++state)
	{
		// first of current submachine
		State subFirst = (state / nrOfOldStates) * nrOfOldStates;
		for (typename std::map<Char, Transition>::iterator iTrans = transition_table[state].begin(); iTrans != transition_table[state].end(); ++iTrans)
			for (Token iOutput = iTrans->second.output.begin(); iOutput < iTrans->second.output.end(); ++iOutput)
				if (outputSet.get(iOutput) && iTrans->second.output.get(iOutput))
			//for (typename std::vector<bool>::const_iterator iOutput = iTrans->second.output.begin(); iOutput != iTrans->second.output.end(); ++iOutput)
			//	if (outputSet.find(*iOutput) != outputSet.end())
				{
					typename std::map<Token, State>::const_iterator iPart = submachines.find(static_cast<Token>(iOutput)/* + this->tokenRange.first*/);
					// change destination to either state of current submachine or to submachine of higher priority output
					State firstNew = iPart == submachines.end() || subFirst > iPart->second ? subFirst : iPart->second;
					iTrans->second.nextState += firstNew - subFirst;
					break;
				}
				else
				{
					// TODO: 
				}
	}

	std::map<State,BitSequence<Token> > potOuts;
	this->getPotentialOutputs(potOuts);
	this->stripDeadEnds(potOuts);


	State endState = static_cast<State>(transition_table.size());
	transition_table.resize(endState + 1);

	/*for (State state = 0; state < nrOfOldStates; ++state)
		for (typename std::map<Char, Transition>::iterator iTrans = transition_table[state].begin(); iTrans != transition_table[state].end(); ++iTrans)
		{
			typename std::map<State,BitSequence<Token> >::const_iterator iPotOuts = potOuts.find(iTrans->second.nextState);
			//if (iPotOuts == potOuts.end() || !intersect(iPotOuts->second, outputSet))
			//if (iPotOuts == potOuts.end() || !outputsIntersect(iPotOuts->second, outputSet))
			if (iPotOuts == potOuts.end() || !(iPotOuts->second && outputSet))
				iTrans->second.nextState = endState;
		}*/

	this->stripUnreachableTransitions();
	this->reduce();
}

template<typename T, typename O> void Fsm<T, O>::strip(const BitSequence<Token>& outputs, bool strict)
{
	if (strict)
		for (State state = 0; state < static_cast<State>(transition_table.size()); ++state)
			stripTransitions(state, outputs);
	else
	{
		// todo: make outputs.size() copies of all states
		// original is for unaccepted
		// copy n is for accepting <=outputs[n]
		// strip unaccepted to smallest unaccepted

		// make copy of all states and transitions
		State nrOfOldStates = static_cast<State>(transition_table.size());
		transition_table.resize(nrOfOldStates * 2);
		for (State state = 0; state < nrOfOldStates; ++state)
		{
			State copyState = state + nrOfOldStates;
			transition_table[copyState] = transition_table[state];
			for (typename std::map<Char, Transition>::iterator iTrans = transition_table[copyState].begin(); iTrans != transition_table[copyState].end(); ++iTrans)
				iTrans->second.nextState += nrOfOldStates;
			// strip copied transitions
			stripTransitions(copyState, outputs);
		}

		// change destination of old transitions with valid output to copied state.
		for (State state = 0; state < nrOfOldStates; ++state)
		{
			for (typename std::map<Char, Transition>::iterator iTrans = transition_table[state].begin(); iTrans != transition_table[state].end(); ++iTrans)
				//for (typename std::set<Token>::const_iterator iOutput = iTrans->second.output.begin(); iOutput != iTrans->second.output.end(); ++iOutput)
				for (Token iOutput = iTrans->second.output.begin(); iOutput < iTrans->second.output.end(); ++iOutput)
					// if transition contains valid output
					//if (outputs.find(*iOutput) != outputs.end())
					if (outputs.get(iOutput) && iTrans->second.output.get(iOutput))
					{
						// change destination to copied state
						iTrans->second.nextState += nrOfOldStates;
						// strip transition to valid output
						iTrans->second.output &= outputs;
						//std::transform(iTrans->second.output.begin(), iTrans->second.output.end(), outputs.begin(), iTrans->second.output.begin(), std::logical_and<bool>());
						//set_intersect(iTrans->second.output, outputs);
						break;
					}
		}
	}
	//std::map<State,std::set<Token> > potOuts;
	std::map<State, BitSequence<Token> > potOuts;
	getPotentialOutputs(potOuts);
	this->stripDeadEnds(potOuts);
	this->stripUnreachableTransitions();
	this->reduce();
}

template<typename T, typename O> void Fsm<T, O>::stripUnreachableTransitions()
{
	std::set<State> reachable;
	// calculate reachable states
	std::queue<State> toCheck;
	toCheck.push(static_cast<State>(0));
	reachable.insert(static_cast<State>(0));
	while (!toCheck.empty())
	{
		State state = toCheck.front();
		toCheck.pop();
		
		for (typename std::map<Char, Transition>::iterator iTrans = transition_table[state].begin(); iTrans != transition_table[state].end(); ++iTrans)
			if (reachable.insert(iTrans->second.nextState).second)
				toCheck.push(iTrans->second.nextState);
	}

	// strip transitions of unreachable states
	for (State state = 0; state < static_cast<State>(this->transition_table.size()); ++state)
		if (reachable.find(state) == reachable.end())
			this->transition_table[state].clear();
}

template<typename T, typename O> void Fsm<T, O>::normalize()
{
	// map from old states to new states
	std::map<State, State> newStates;
	newStates[0] = 0;
	std::queue<State> toVisit;
	toVisit.push(0);
	State newState = 1;
	while(!toVisit.empty())
	{
		State oldState = toVisit.front();
		toVisit.pop();
		for (typename std::map<Char, Transition>::iterator iTrans = transition_table[oldState].begin(); iTrans != transition_table[oldState].end(); ++iTrans)
		{
			std::pair<typename std::map<State, State>::iterator, bool> iNewState = newStates.insert(std::make_pair(iTrans->second.nextState, newState));
			if (iNewState.second)
			{
				++newState;
				toVisit.push(iTrans->second.nextState);
			}
			iTrans->second.nextState = iNewState.first->second;
		}
	}

	TransitionTable newTransitions(transition_table.size());
	for (typename std::map<State, State>::const_iterator iState = newStates.begin(); iState != newStates.end(); ++iState)
		newTransitions[iState->second] = transition_table[iState->first];

	transition_table.swap(newTransitions);
}

template<typename T, typename O> void Fsm<T, O>::stripDeadEnds(const std::map<State,BitSequence<Token> >& outputs)
{
	for (State state = 0; state < static_cast<State>(transition_table.size()); ++state)
	{
		typename std::map<State,BitSequence<Token> >::const_iterator potOuts = outputs.find(state);
		//if (potOuts == outputs.end() || potOuts->second.empty())
		if (potOuts == outputs.end() || potOuts->second.empty())
			transition_table[state].clear();
		else
		{
			std::map<Char, Transition> new_transitions;
			for (typename std::map<Char, Transition>::const_iterator iTrans = transition_table[state].begin(); iTrans != transition_table[state].end(); ++iTrans)
				if (!iTrans->second.output.empty()
					|| ((potOuts = outputs.find(iTrans->second.nextState)) != outputs.end() && !potOuts->second.empty()))
				//if (!iTrans->second.output.empty() || ((potOuts = outputs.find(iTrans->second.nextState)) != outputs.end() && !potOuts->second.empty()))
					new_transitions.insert(*iTrans);
			new_transitions.swap(transition_table[state]);
		}
	}
}


template<typename T, typename O> void Fsm<T, O>::groupLinearPaths(State minState, int minLength)
{
	// find linear path states
	std::set<State> linearStates;
	for (State state = minState; state < static_cast<State>(transition_table.size()); ++state)
		if (transition_table[state].size() == 1)
			linearStates.insert(state);

	// find entries to linear paths
	std::set<State> seeds;
	for (State state = 0; state < static_cast<State>(transition_table.size()); ++state)
		// check source for non linear state or state with output
		if (	linearStates.find(state) == linearStates.end()
			||	!transition_table[state].begin()->second.output.empty())
			for (typename Transitions::const_iterator iTrans = transition_table[state].begin(); iTrans != transition_table[state].end(); ++iTrans)
				// if destination is a linear state it's a seed
				if (linearStates.find(iTrans->second.nextState) != linearStates.end())
					seeds.insert(iTrans->second.nextState);

	// fill paths
	State newStateCount = 0;
	std::map<State, std::list<State> > paths;
	std::set<State> pathStates;
	std::set<State> innerStates;
	for (typename std::set<State>::const_iterator iSeed = seeds.begin(); iSeed != seeds.end(); ++iSeed)
	{
		State state = *iSeed;
		std::list<State>& path = paths[state];
		std::set<State> inserted;
		while(	state != ERROR_STATE
			&&	transition_table[state].size() == 1
			&&	inserted.find(state) == inserted.end())
		{
			if (state != *iSeed) innerStates.insert(state);
			path.push_back(state);
			inserted.insert(state);
			if (!transition_table[state].begin()->second.output.empty())
				break;
			else
				state = transition_table[state].begin()->second.nextState;
		}
		// check for sufficient length
		if (path.empty() || static_cast<State>(path.size()) < minLength)
			paths.erase(*iSeed);
		else
		{
			pathStates.insert(path.begin(), path.end());
			newStateCount += static_cast<State>(path.size());
		}
	}
	// remove path B with entry state which is also part of larger path A where path B is suffix of.
	for (typename std::set<State>::const_iterator iState = innerStates.begin(); iState != innerStates.end(); ++iState)
	{
		typename std::map<State, std::list<State> >::iterator iPath = paths.find(*iState);
		if (iPath != paths.end())
		{
			// adjust newStateCount and erase path
			newStateCount -= static_cast<State>(iPath->second.size());
			paths.erase(iPath);
		}
	}
	
	// create mapping for non linear states
	// go through all old states and remove gaps remaining from path states
	std::map<State, State> newStates;
	State newState = 0;
	for (State state = 0; state < static_cast<State>(transition_table.size()); ++state)
		if (pathStates.find(state) == pathStates.end())
			newStates.insert(std::make_pair(state, newState++));

	// create new transitions and mapping for path states
	TransitionTable newTransitionTable(transition_table.size() - pathStates.size() + newStateCount);
	for (typename std::map<State, std::list<State> >::const_iterator iPath = paths.begin(); iPath != paths.end(); ++iPath)
	{
		typename std::list<State>::const_iterator iState = iPath->second.begin();
		// assign instead of insert to ensure beginning of path is mapped correctly
		// when beginning is part of another path whose states have already been mapped
		newStates[*iState] = newState;
		for (typename std::list<State>::const_iterator iNext = iState; ++iNext != iPath->second.end(); iState = iNext)
		{
			Transitions& newTransitions = newTransitionTable[newState] = transition_table[*iState];
			newTransitions.begin()->second.nextState = ++newState;
			newStates.insert(std::make_pair(*iNext, newState));
		}
		++newState;
	}

	assert(static_cast<State>(newTransitionTable.size()) == newState);
	assert(transition_table.size() == newStates.size());

	// create new transitions for exits of paths
	// todo: fix this, newStates-mapping fails at paths ending at the same state
	for (typename std::map<State, std::list<State> >::const_iterator iPath = paths.begin(); iPath != paths.end(); ++iPath)
	{
		State lastState = *iPath->second.rbegin();
		State newLastState = static_cast<State>(newStates.find(iPath->first)->second + iPath->second.size() - 1);
		Transitions& newTransitions = newTransitionTable[newLastState] = transition_table[lastState];
		newTransitions.begin()->second.nextState = newStates.find(newTransitions.begin()->second.nextState)->second;
	}

	// create new transition_table with new state mapping for nonLinearStates
	for (typename std::map<State, State>::const_iterator iNewState = newStates.begin(); iNewState != newStates.end(); ++iNewState)
		if (pathStates.find(iNewState->first) == pathStates.end())
		{
			Transitions& newTransitions = newTransitionTable[iNewState->second];
			// copy old transitions
			newTransitions = transition_table[iNewState->first];
			// change destinations
			for (typename Transitions::iterator iTrans = newTransitions.begin(); iTrans != newTransitions.end(); ++iTrans)
				iTrans->second.nextState = newStates.find(iTrans->second.nextState)->second;
		}

	

	transition_table.swap(newTransitionTable);
}

template<typename T, typename O> typename Fsm<T, O>::State Fsm<T, O>::getFirstLinearSequence(int minLength) const
{
	State result = 0;
	int length = 1;
	for (State state = 0; state < getNumberOfStates(); ++state)
		if (transition_table[state].size() != 1)
			result = state + 1;
		else if(transition_table[state].begin()->second.nextState == state + 1
			&&	transition_table[state].begin()->second.output.empty())
			++length;
		else
		{
			if (length < minLength) result = state + 1;
			length = 1;
		}
	return result;
}

template<typename T, typename O> typename Fsm<T, O>::State Fsm<T, O>::getNextLinearSequence(State startState, std::basic_string<Char>& input, BitSequence<Token>& output) const
{
	State state = startState;
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
	return state;
}

/*
template<typename T, typename O> void Fsm<T, O>::initOutputArray(std::vector<bool>& outputs) const
{
	outputs.resize(this->tokenRange.last - this->tokenRange.first + 1, false);
}

template<typename T, typename O> void Fsm<T, O>::fillOutputArray(std::vector<bool>& outputs, const std::set<Token>& tokens) const
{
	for (typename std::set<Token>::const_iterator io = tokens.begin(); io != tokens.end(); ++io)
		outputs[*io - this->tokenRange.first] = true;
}

template<typename T, typename O> void Fsm<T, O>::fillOutputArray(std::vector<bool>& outputs, const std::list<Token>& tokens) const
{
	for (typename std::list<Token>::const_iterator io = tokens.begin(); io != tokens.end(); ++io)
		outputs[*io - this->tokenRange.first] = true;
}

template<typename T, typename O> bool Fsm<T, O>::outputIsEmpty(const std::vector<bool>& outputs) const
{
	return outputs == std::vector<bool>(this->tokenRange.last - this->tokenRange.first + 1, false);
}

template<typename T, typename O> bool Fsm<T, O>::outputsIntersect(const std::vector<bool>& outs1, const std::vector<bool>& outs2) const
{
	for (std::vector<bool>::const_iterator i1 = outs1.begin(), i2 = outs2.begin(); i1 != outs1.end(); ++i1, ++i2)
		if (*i1 && *i2) return true;
	return false;
}

template<typename T, typename O> std::vector<bool>& Fsm<T, O>::transformOutput(const std::vector<bool>& output, size_t offset, std::vector<bool>& result) const
{
	result.resize(offset + output.size());
	std::fill_n(result.begin(), offset, false);
	std::copy(output.begin(), output.end(), result.begin() + offset);
	return result;
}*/

/*
using namespace std;

template<typename T, typename O> void Fsm<T, O>::printTable()
{	
	for(size_t i = 0; i < transition_table.size(); ++i)
	{
		typename std::map<T,State>::const_iterator t;
		for(t = transition_table[i].begin(); t != transition_table[i].end(); ++t)
			cout << "d(" << static_cast<Fsm<T, O>::Token>(i) << "," << t->first << ") = " << t->second << endl;
	}
	typename std::map<State,std::set<Token> >::const_iterator f;
	for(f = final_states.begin(); f != final_states.end(); ++f)
	{
		cout << "(" << f->first << ", ";
		std::set<Token>::const_iterator oi;
		for(oi = f->second.begin(); oi != f->second.end(); ++oi)
			cout << *oi << " ";
		cout << "); ";
	}
	cout << endl;
}*/

template class Fsm<char, char>;
template class Fsm<char, int>;
template class Fsm<int, int>;
