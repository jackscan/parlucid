#ifndef INTERVAL_HPP
#define INTERVAL_HPP

template<typename T> struct Interval
{
	explicit Interval(const T& s): first(s), last(s) {}
	Interval(const T& a, const T& b): first(a), last(b) {}
	bool operator< (const Interval& other) const { return this->last < other.first; }
	bool operator==(const Interval& other) const { return this->first == other.first && this->last == other.last; }
	bool operator!=(const Interval& other) const { return this->first != other.first || this->last != other.last; }
	bool empty() const { return *this < *this; }
	bool singleton() const { return this->first == this->last; }
	bool contains(const T& s) const { return !(s < this->first) && !(this->last < s); }
	bool contains(const Interval& other) const { return contains(other.first) && contains(other.last); }
	Interval intersection(const Interval& other) const { return Interval( (this->first<other.first)?other.first:this->first, (this->last<other.last)?this->last:other.last ); }

	T first, last;
};

#endif
