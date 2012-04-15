#include "../alphabet.hpp"

#include <limits>
#include <list>
#include <cassert>

template<typename T> Alphabet<T>::Alphabet(const T& symbol)
{
	singleton.insert(symbol);
}

template<typename T> Alphabet<T>::Alphabet(const Interval<T>& i)
{
	if (!i.empty()) interval.insert(i);
}

template<typename T> Alphabet<T>::Alphabet(const Alphabet<T>& a): singleton(a.singleton), interval(a.interval)
{
}

template<typename T> void Alphabet<T>::add(const T& symbol)
{
	if( interval.lower_bound(Interval<T>(symbol)) == interval.end() ||
		symbol < interval.lower_bound(Interval<T>(symbol))->first )
		singleton.insert( symbol );
}

template<typename T> void Alphabet<T>::add(const T& first, const T& last)
{
	Interval<T> i(first, last);

	if( i.empty() ) return;
	if( i.singleton() ){ add(first); return; }

	std::pair<typename std::set<Interval<T> >::iterator,typename std::set<Interval<T> >::iterator> irng;
	irng = interval.equal_range(i);
	if( irng.first != interval.end() && irng.first != irng.second )
	{
		if( irng.first->first < i.first ) i.first = irng.first->first;
		typename std::set<Interval<T> >::const_iterator inext, ilast = irng.first;
		for(inext = irng.first; inext != irng.second; ++inext) ilast = inext;
		if( i.last < ilast->last ) i.last = ilast->last;
		interval.erase(irng.first, irng.second);
	}

	singleton.erase(singleton.lower_bound(first), singleton.upper_bound(last));
	interval.insert(i);
}

template<typename T> void Alphabet<T>::add(const Alphabet<T>& a)
{
	typename std::set<Interval<T> >::const_iterator i;
	for(i = a.interval.begin(); i != a.interval.end(); ++i) add(i->first, i->last);
	typename std::set<T>::const_iterator s;
	for(s = a.singleton.begin(); s != a.singleton.end(); ++s) add(*s);
}

template<typename T> void Alphabet<T>::del(const T& symbol)
{
	singleton.erase(symbol);
	typename std::set<Interval<T> >::iterator iter = interval.lower_bound(Interval<T>(symbol, symbol));
	if( iter == interval.end() || symbol < iter->first ) return;
	Interval<T> i(*iter);
	interval.erase(iter);
	if(i.first < symbol) add(i.first, pred(symbol));
	if(symbol < i.last) add(succ(symbol), i.last);
}

template<typename T> void Alphabet<T>::del(const T& first, const T& last)
{
	singleton.erase(singleton.lower_bound(first), singleton.upper_bound(last));
	typename std::set<Interval<T> >::iterator ifirst = interval.lower_bound(Interval<T>(first));
	typename std::set<Interval<T> >::iterator ilast = interval.lower_bound(Interval<T>(last));
	typename std::set<Interval<T> >::iterator ibeyond = interval.upper_bound(Interval<T>(last));

	if( ifirst == interval.end() || last < ifirst->first) return;

	T i1_first = ifirst->first;
	T i2_last = ilast != interval.end() && ilast->first <= last ? ilast->last : last;

	interval.erase(ifirst, ibeyond);
	
	if(i1_first < first) add(i1_first, pred(first));	
	if(last < i2_last) add(succ(last), i2_last);
}

template<typename T> void Alphabet<T>::del(const Alphabet<T>& a)
{
	typename std::set<Interval<T> >::const_iterator i;
	for(i = a.interval.begin(); i != a.interval.end(); ++i) del(i->first, i->last);
	typename std::set<T>::const_iterator s;
	for(s = a.singleton.begin(); s != a.singleton.end(); ++s) del(*s);
}

template<typename T> void Alphabet<T>::clear()
{
	this->singleton.clear();
	this->interval.clear();
}

template<typename T> Alphabet<T> Alphabet<T>::intersection(const Alphabet<T>& a)
{
	Alphabet<T> isec;
	typename std::set<Interval<T> >::const_iterator i1, i2;
	for(i1 = interval.begin(); i1 != interval.end(); ++i1)
	{
		for(i2 = a.interval.lower_bound(*i1); i2 != a.interval.upper_bound(*i1); ++i2)
			isec.add(i1->intersection(*i2).first, i1->intersection(*i2).last);
	}
	typename std::set<T>::const_iterator s;
	for(s = singleton.begin(); s != singleton.end(); ++s)
		if( a.contains(*s) ) isec.add(*s);
	for(s = a.singleton.begin(); s != a.singleton.end(); ++s)
		if( contains(*s) ) isec.add(*s);

	return isec;
}

template<typename T> bool Alphabet<T>::contains(const T& symbol) const
{
	return
	singleton.count(symbol) > 0 ||
	(	interval.lower_bound(Interval<T>(symbol,symbol)) != interval.end() &&
		!(symbol < interval.lower_bound(Interval<T>(symbol,symbol))->first) );
}

template<typename T> bool Alphabet<T>::empty() const
{
	return singleton.empty() && interval.empty();
}

template<typename T> T Alphabet<T>::lower_bound(const T& symbol) const
{
	if( singleton.count(symbol) > 0 )
		return symbol;
	if( interval.lower_bound(Interval<T>(symbol)) == interval.end() )
		return succ(symbol);
	return interval.lower_bound(Interval<T>(symbol))->first;
}

template<typename T> T Alphabet<T>::upper_bound(const T& symbol) const
{
	if( singleton.count(symbol) > 0 )
		return symbol;
	if( interval.lower_bound(Interval<T>(symbol)) == interval.end() ||
		symbol < interval.lower_bound(Interval<T>(symbol))->first )
		return pred(symbol);
	return interval.lower_bound(Interval<T>(symbol))->last;
}

template<typename T> std::set<T> Alphabet<T>::lower_bound() const
{
	std::set<T> lbs = singleton;
	typename std::set<Interval<T> >::const_iterator i;
	for(i = interval.begin(); i != interval.end(); ++i)
		lbs.insert(i->first);

	return lbs;
}

template<typename T> T Alphabet<T>::min() const
{
	if(interval.empty())
	{
		if(singleton.empty()) return std::numeric_limits<char>::max();
		else return *singleton.begin();
	}
	else
	{
		const T& ifirst = interval.begin()->first;
		if(singleton.empty()) return ifirst;
		else return *singleton.begin() < ifirst ? *singleton.begin() : ifirst;
	}
}

template<typename T> void Alphabet<T>::addDisjunct(const T& symbol)
{
	//std::cerr << "addDisjunct(a) ... ";
	if( !(symbol < lower_bound(symbol)) )
		del(symbol);
	
	add(symbol);
	//std::cerr << "done" << std::endl;
}

template<typename T> void Alphabet<T>::addDisjunct(const T& first, const T& last)
{
	//std::cerr << "addDisjunct(a,b) ... ";
	Alphabet<T> isec(intersection(Alphabet(Interval<T>(first, last))));
	add(first, last);
	del(isec);
	add(isec);
	//std::cerr << "done" << std::endl;
}

template<typename T> void Alphabet<T>::addDisjunct(const Alphabet<T>& a)
{
	//std::cerr << "addDisjunct(Alphabet) ... ";
	Alphabet<T> isec(intersection(a));
	add(a);
	del(isec);
	add(isec);
	//std::cerr << "done" << std::endl;
}

template<typename T> const std::set<T>& Alphabet<T>::singletons() const
{
	return this->singleton;
}

template<typename T> const std::set<Interval<T> >& Alphabet<T>::intervals() const
{
	return this->interval;
}

template<typename T> void Alphabet<T>::merge(const std::set<T>& mergeset)
{
	// collect intervals and singletons containing members of mergeset
	std::set<Interval<T> > intervalset;
	for(typename std::set<T>::const_iterator iter = mergeset.begin(); iter != mergeset.end(); ++iter)
	{
		typename std::set<Interval<T> >::const_iterator ival = this->interval.lower_bound(Interval<T>(*iter));
		if (ival != this->interval.end() && ival->first <= *iter)
			intervalset.insert(*ival);
		typename std::set<T>::const_iterator iston = this->singleton.find(*iter);
		if (iston != this->singleton.end())
			intervalset.insert(Interval<T>(*iston));
	}

	// merge adjacent intervals and singletons
	if (1 < intervalset.size())
	{
		for(typename std::set<Interval<T> >::const_iterator next = intervalset.begin(), prev = next++; next != intervalset.end(); prev = next++)
			if (succ(prev->last) >= next->first)
			{
				Interval<T> newInterval(this->lower_bound(prev->first), this->upper_bound(next->last));
				this->del(newInterval.first, newInterval.last);
				this->interval.insert(newInterval);
			}
	}
}

template<typename T> void Alphabet<T>::merge()
{
	// collect intervals and singletons
	std::set<Interval<T> > intervalset = this->interval;
	for(typename std::set<T>::const_iterator iter = this->singleton.begin(); iter != this->singleton.end(); ++iter)
		intervalset.insert(Interval<T>(*iter));

	if (1 < intervalset.size())
	{
		for(typename std::set<Interval<T> >::const_iterator next = intervalset.begin(), prev = next++; next != intervalset.end(); prev = next++)
			if (succ(prev->last) >= next->first)
			{
				Interval<T> newInterval(this->lower_bound(prev->first), this->upper_bound(next->last));
				this->del(newInterval.first, newInterval.last);
				this->interval.insert(newInterval);
			}
	}
}

template<typename T> void Alphabet<T>::roundBoundaries(const std::set<Interval<T> >& spaces)
{
	for (typename std::set<Interval<T> >::const_iterator iter = spaces.begin(); iter != spaces.end(); ++iter)
	{
		const Interval<T>& space = *iter;
		if (!space.empty())
		{
			Interval<T> infSpace = space;
			// inflate space
			if (std::numeric_limits<T>::min() < infSpace.first) --infSpace.first;
			if (infSpace.last < std::numeric_limits<T>::max()) ++infSpace.last;

			if (!infSpace.singleton())
			{
				// find simplest boundary
				T mask = 0;
				for (T spaceXor = space.first ^ infSpace.last; 0 != (spaceXor = (spaceXor >> 1) & std::numeric_limits<T>::max());)
					mask = (mask << 1) | 1;

				T boundary = 0 == (mask & space.first) ? space.first : infSpace.last & ~mask;
				//T boundary = infSpace.last & ~mask;

				assert(space.first <= boundary);
				assert(boundary <= infSpace.last);

				std::list<Interval<T> > newIntervals;
				typename std::pair<typename std::set<Interval<T> >::iterator, typename std::set<Interval<T> >::iterator> range = interval.equal_range(infSpace);
				for (typename std::set<Interval<T> >::iterator iInterval = range.first; iInterval != range.second; ++iInterval)
					if (!space.contains(*iInterval))
					{
						if (iInterval->last <= space.last)
							newIntervals.push_back(Interval<T>(iInterval->first, pred(boundary)));
						else if (space.first <= iInterval->first)
							newIntervals.push_back(Interval<T>(boundary, iInterval->last));
						else
							newIntervals.push_back(*iInterval);
					}

				interval.erase(range.first, range.second);

				for (typename std::list<Interval<T> >::const_iterator iNew = newIntervals.begin(); iNew != newIntervals.end(); ++iNew)
					interval.insert(*iNew);
			}
		}
	}
}

template class Alphabet<unsigned char>;
template class Alphabet<char>;
template class Alphabet<unsigned short>;
template class Alphabet<short>;
template class Alphabet<int>;
