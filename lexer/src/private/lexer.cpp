#include "../lexer.hpp"

template class Lexer<char>;

template<typename T, class F>
void Lexer<T,F>::init(const Fsm& fsm)
{
	this->fsm = fsm;
}

template<typename T, class F>
void Lexer<T,F>::reset()
{
	this->state = 0;
	this->count = 0;
}

template<typename T, class F>
bool Lexer<T,F>::error() const
{
	return this->fsm.error(this->state);
}

template<typename T, class F>
std::streamsize Lexer<T,F>::gcount() const
{
	return this->count;
}

template<typename T, class F>
bool Lexer<T,F>::read(std::basic_istream<T>& is, int& token_type)
{
	//TODO:
	/*this->reset();
	typename std::basic_istream<T>::pos_type istr_pos = is.tellg(), start_pos = istr_pos;
	typename Fsm::State accept_state = this->state;
	std::streamsize cnt = 0;

	// initialize with empty set
	typename Fsm::State errorState = Fsm::ERROR_STATE;
	const BitSequence<int>* result = &this->fsm.next(errorState, static_cast<T>(0));

	while(is.good())
	{
		T ch;
		is.get(ch);
		if(is.fail()) break;
		const BitSequence<int>& outs = this->fsm.next(this->state, ch);
		if(this->fsm.error(this->state))
		{
			is.unget();
			break;
		}
		cnt++;
		if(!outs.empty())
		{
			this->count = cnt;
			result = &outs;
			accept_state = this->state;
			istr_pos = is.tellg();
		}
	}

	is.seekg(istr_pos);

	if (!result->empty()) token_type = *result->begin();
	return !result->empty();*/
	return false;
}

template<typename T, class F>
const std::set<int>& Lexer<T,F>::read(std::basic_istream<T>& is)
{
	/*this->reset();
	typename std::basic_istream<T>::pos_type istr_pos = is.tellg(), start_pos = istr_pos;
	typename Fsm::State accept_state = this->state;
	std::streamsize cnt = 0;

	// initialize with empty set
	typename Fsm::State errorState = Fsm::ERROR_STATE;
	const std::set<int>* result = &this->fsm.next(errorState, static_cast<T>(0));

	while(is.good())
	{
		T ch;
		is.get(ch);
		if(is.fail()) break;
		const std::set<int>& outs = this->fsm.next(this->state, ch);
		if(this->fsm.error(this->state))
		{
			is.unget();
			break;
		}
		cnt++;
		if(!outs.empty())
		{
			this->count = cnt;
			result = &outs;
			accept_state = this->state;
			istr_pos = is.tellg();
		}
	}

	is.seekg(istr_pos);

	return *result;*/
	static std::set<int> empty;
	return empty;
}
