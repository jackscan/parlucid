#include "../nfa.hpp"
#include "../../../utils/src/helper.hpp"

#include <limits>

template<typename C, typename O> const std::set<typename Nfa<C, O>::State> Nfa<C, O>::emptyStateSet;

template<typename C, typename O> Nfa<C, O>::Nfa(): num_states(1)
{
	this->final_states.insert(std::make_pair(0, std::set<Token>()));
}

template<typename C, typename O> Nfa<C, O>::Nfa(const std::basic_string<Char>& word): num_states(static_cast<State>(word.length())+1)
{
	for(State i = 0; i < static_cast<State>(word.length()); ++i)
	{
		this->transition_table[i][word[i]].insert(i+1);
		this->alphabet.add(word[i]);
	}
	this->final_states.insert(std::make_pair(static_cast<State>(word.length()), std::set<Token>()));
}

template<typename C, typename O> Nfa<C, O>::Nfa(const Alphabet<Char>& alphabet): alphabet(alphabet), num_states(2)
{
	std::set<Char> lbs = this->alphabet.lower_bound();
	typename std::set<Char>::const_iterator lbi = lbs.begin();
	for(; lbi != lbs.end(); ++lbi) this->transition_table[0][*lbi].insert(1);
	this->final_states.insert(std::make_pair(1, std::set<Token>()));
}

template<typename C, typename O> Nfa<C, O> Nfa<C, O>::reverse() const
{
	Nfa<C, O> result;

	State finalState = this->num_states;
	result.num_states = this->num_states + 1;
	result.final_states[finalState] = this->output();
	result.alphabet = this->alphabet;

	// reverse transitions
	for (typename std::map<State,std::map<Char,std::set<State> > >::const_iterator it = transition_table.begin(); it != transition_table.end(); ++it)
	{
		// exchange starting state with final state.
		State src = it->first != 0 ? it->first : finalState;
		for (typename std::map<Char,std::set<State> >::const_iterator irow = it->second.begin(); irow != it->second.end(); ++irow)
			for (typename std::set<State>::const_iterator is = irow->second.begin(); is != irow->second.end(); ++is)
			{
				// exchange starting state with final state.
				State dst = *is != 0 ? *is : finalState;
				result.transition_table[dst][irow->first].insert(src);
				// if transitions leads to final state, add reverse of transition to start
				if (this->final(*is))
					result.transition_table[0][irow->first].insert(src);
			}
	}

	return result;
}

template<typename C, typename O> Nfa<C, O> Nfa<C, O>::operator*() const
{
	Nfa<C, O> result(*this);

	typename std::map<State,std::set<Token> >::const_iterator fsi = result.final_states.begin();
	typename std::map<Char,std::set<State> >::const_iterator ti;
	for(; fsi != result.final_states.end(); ++fsi)
	{
		for(ti = result.transition_table[0].begin(); ti != result.transition_table[0].end(); ++ti)
			result.transition_table[fsi->first].insert(*ti);

		result.add_output(0, fsi->second);
	}

	return result;
}

template<typename C, typename O> Nfa<C, O> Nfa<C, O>::operator|(const Nfa<C, O>& other) const
{
	Nfa<C, O> result;

	result.alphabet = this->alphabet;
	result.alphabet.addDisjunct(other.alphabet);

	std::set<Char> lbs = result.alphabet.lower_bound();
	typename std::set<Char>::const_iterator lbi;

	result.num_states = this->num_states + other.num_states - 1;
	State offset = this->num_states - 1;

	for(lbi = lbs.begin(); lbi != lbs.end(); ++lbi)
	{
		typename std::map<State,std::map<Char,std::set<State> > >::const_iterator row;
		for(row = this->transition_table.begin(); row != this->transition_table.end(); ++row)
			result.transition_table[row->first][*lbi] = this->next(row->first, *lbi);

		for(row = other.transition_table.begin(); row != other.transition_table.end(); ++row)
		{
			State s = row->first == 0 ? 0 : row->first + offset;
			std::set<State> fstates = other.next(row->first, *lbi);
			typename std::set<State>::const_iterator si;
			for(si = fstates.begin(); si != fstates.end(); ++si)
				result.transition_table[s][*lbi].insert(*si + offset);
		}
	}

	result.final_states = this->final_states;
	typename std::map<State,std::set<Token> >::const_iterator fsi;
	for(fsi = other.final_states.begin(); fsi != other.final_states.end(); ++fsi)
		if( fsi->first == 0 )
			result.add_output(0, fsi->second);
		else
			result.add_output(fsi->first + offset, fsi->second);

	//if( other.final(0) ) result.add_output(0, other.output(0));

	return result;
}

/*template<typename C, typename O> Nfa<C, O>& Nfa<C, O>::operator|=(const Nfa<C, O>& other)
{
	this->alphabet.addDisjunct(other.alphabet);

	std::set<Char> lbs = this->alphabet.lower_bound();
	std::set<Char>::const_iterator lbi;

	State offset = this->num_states - 1;
	this->num_states += other.num_states - 1;
}*/

template<typename C, typename O> Nfa<C, O> Nfa<C, O>::operator+(const Nfa<C, O>& other) const
{
	Nfa<C, O> result;

	result.alphabet = this->alphabet;
	result.alphabet.addDisjunct(other.alphabet);

	std::set<Char> lbs = result.alphabet.lower_bound();
	typename std::set<Char>::const_iterator lbi;

	result.num_states = this->num_states + other.num_states - 1;
	State offset = this->num_states - 1;

	for(lbi = lbs.begin(); lbi != lbs.end(); ++lbi)
	{
		typename std::map<State,std::map<Char,std::set<State> > >::const_iterator row;
		for(row = this->transition_table.begin(); row != this->transition_table.end(); ++row)
			result.transition_table[row->first][*lbi] = this->next(row->first, *lbi);

		for(row = other.transition_table.begin(); row != other.transition_table.end(); ++row)
		{
			std::set<State> fstates = other.next(row->first, *lbi);
			typename std::set<State>::const_iterator si;
			
			if( row->first == 0 )
			{
				typename std::map<State,std::set<Token> >::const_iterator fsi;
				for(fsi = this->final_states.begin(); fsi != this->final_states.end(); ++fsi)
					for(si = fstates.begin(); si != fstates.end(); ++si)
						result.transition_table[fsi->first][*lbi].insert(*si + offset);
			}
			else
			{
				for(si = fstates.begin(); si != fstates.end(); ++si)
					result.transition_table[row->first + offset][*lbi].insert(*si + offset);
			}			
		}
	}

	if( other.final(0) ) result.final_states = this->final_states;
	else result.final_states.clear();
	typename std::map<State,std::set<Token> >::const_iterator fsi;
	for(fsi = other.final_states.begin(); fsi != other.final_states.end(); ++fsi)
		result.add_output(fsi->first + offset, fsi->second);

	return result;
}

/*template<typename C, typename O> Nfa<C, O> Nfa<C, O>::operator~() const
{
	Nfa<C, O> result(*this);
	Alphabet<C> remain(Interval<C>(std::numeric_limits<C>::min(), std::numeric_limits<C>::max()));
	remain.del(result.alphabet);
	result.alphabet.add(remain);

	// add state accepting everything
	State accState = result.num_states++;
	std::set<State> accSet;
	accSet.insert(accState);
	result.transition_table[accState].clear();

	std::set<Char> lbs = result.alphabet.lower_bound();
	std::set<Char>::const_iterator lbi;

	// for every non-final state
	// insert new default transitions to accepting state
	// and starting transitions to first states
	std::map<State,std::map<Char,std::set<State> > >::const_iterator firstRow = this->transition_table.begin();
	if (firstRow != this->transition_table.end())
	{
		std::map<State,std::map<Char,std::set<State> > >::iterator row;
		for(row = result.transition_table.begin(); row != result.transition_table.end(); ++row)
			if(!this->final(row->first))
			{
				if (row->first != firstRow->first)
					for(std::map<Char,std::set<State> >::const_iterator ifr = firstRow->second.begin(); ifr != firstRow->second.end(); ++ifr)
						set_union_add(row->second[ifr->first], ifr->second);

				for(lbi = lbs.begin(); lbi != lbs.end(); ++lbi)
				{
					std::map<Char,std::set<State> >::iterator ins = row->second.insert(std::make_pair(*lbi, accSet)).first;
					if(ins->second.empty()) ins->second = accSet;
				}
			}
	}

	result.final_states.clear();

	result.final_states[accState] = this->output();

	return result;
}*/

template<typename C, typename O> Nfa<C, O> Nfa<C, O>::combine(const std::list<Nfa>& nfas)
{
	Nfa<C, O> result;
	result.final_states.clear();
	if(nfas.empty()) return result;

	typename std::list<Nfa>::const_iterator iNfa = nfas.begin();

	result.alphabet = iNfa->alphabet;
	for(++iNfa; iNfa != nfas.end(); ++iNfa)
		result.alphabet.addDisjunct(iNfa->alphabet);

	std::set<Char> lbs = result.alphabet.lower_bound();

	State offset = 0;

	for(iNfa = nfas.begin(); iNfa != nfas.end(); ++iNfa)
	{
		for(typename std::set<Char>::const_iterator lbi = lbs.begin(); lbi != lbs.end(); ++lbi)
		{
			typename std::map<State,std::map<Char,std::set<State> > >::const_iterator row;
			for(row = iNfa->transition_table.begin(); row != iNfa->transition_table.end(); ++row)
			{
				State s = row->first == 0 ? 0 : row->first + offset;
				const std::set<State>& fstates = iNfa->next(row->first, *lbi);
				typename std::set<State>::const_iterator si;
				for(si = fstates.begin(); si != fstates.end(); ++si)
					result.transition_table[s][*lbi].insert(*si + offset);
			}
		}

		typename std::map<State,std::set<Token> >::const_iterator fsi;
		for(fsi = iNfa->final_states.begin(); fsi != iNfa->final_states.end(); ++fsi)
			if( fsi->first == 0 ) result.add_output(0, fsi->second);
			else result.add_output(fsi->first + offset, fsi->second);

		offset += iNfa->num_states - 1;
	}

	result.num_states = offset + 1;

	return result;
}

template<typename C, typename O> const std::set<typename Nfa<C, O>::State>& Nfa<C, O>::next(typename Nfa<C, O>::State state, const Char& symbol) const
{
	Char c = alphabet.lower_bound(symbol);
	if( c > symbol ) return emptyStateSet;
	typename std::map<State,std::map<Char,std::set<State> > >::const_iterator irow = transition_table.find(state);
	if( irow == transition_table.end() ) return emptyStateSet;
	typename std::map<Char,std::set<State> >::const_iterator icol = irow->second.find(c);
	if( icol == irow->second.end() ) return emptyStateSet;
	return icol->second;
}

template<typename C, typename O> std::set<typename Nfa<C, O>::State> Nfa<C, O>::next(const std::set<State>& states, const Char& symbol) const
{
	std::set<State> result;
	typename std::set<State>::const_iterator si;
	for(si = states.begin(); si != states.end(); ++si)
	{
		std::set<State> fstates = this->next(*si, symbol);
		typename std::set<State>::const_iterator fsi;
		for(fsi = fstates.begin(); fsi != fstates.end(); ++fsi)
			result.insert(*fsi);
	}
	return result;
}

template<typename C, typename O> bool Nfa<C, O>::final(typename Nfa<C, O>::State state) const
{
	return this->final_states.count(state) > 0;
}

template<typename C, typename O> bool Nfa<C, O>::final(std::set<typename Nfa<C, O>::State> states) const
{
	typename std::set<State>::const_iterator si;
	for(si = states.begin(); si != states.end(); ++si)
		if( this->final(*si) ) return true;

	return false;
}

template<typename C, typename O> std::set<typename Nfa<C, O>::Token> Nfa<C, O>::output(typename Nfa<C, O>::State state) const
{
	typename std::map<State,std::set<Token> >::const_iterator s = this->final_states.find(state);
	if( s == this->final_states.end() ) return std::set<Token>();
	return s->second;
}

template<typename C, typename O> std::set<typename Nfa<C, O>::Token> Nfa<C, O>::output(std::set<typename Nfa<C, O>::State> states) const
{
	typename std::set<State>::const_iterator si;
	std::set<Token> result;
	for(si = states.begin(); si != states.end(); ++si)
		if( this->final(*si) )
		{
			std::set<Token> o = this->output(*si);
			typename std::set<Token>::const_iterator oi;
			for(oi = o.begin(); oi != o.end(); ++oi) result.insert(*oi);
		}

	return result;
}

template<typename C, typename O> std::set<typename Nfa<C, O>::Token> Nfa<C, O>::output() const
{
	std::set<Token> result;
	for(typename std::map<State,std::set<Token> >::const_iterator is = final_states.begin(); is != final_states.end(); ++is)
		for(typename std::set<Token>::const_iterator io = is->second.begin(); io != is->second.end(); ++io)
			result.insert(*io);
	return result;
}

template<typename C, typename O> void Nfa<C, O>::set_output(Token o)
{
	typename std::map<State,std::set<Token> >::iterator s = this->final_states.begin();
	for(; s != this->final_states.end(); ++s)
	{
		s->second.clear();
		s->second.insert(o);
	}
}

template<typename C, typename O> const Alphabet<typename Nfa<C, O>::Char>& Nfa<C, O>::input() const
{
	return this->alphabet;
}

template<typename C, typename O> void Nfa<C, O>::add_output(typename Nfa<C, O>::State state, const std::set<Token>& o)
{
	std::set<Token>& output = this->final_states[state];
	for(typename std::set<Token>::const_iterator oi = o.begin(); oi != o.end(); ++oi)
		output.insert(*oi);
}

template<typename C, typename O> bool Nfa<C, O>::empty() const
{
	return this->transition_table.empty();
}

template class Nfa<char, char>;
template class Nfa<char, int>;
template class Nfa<int, int>;
