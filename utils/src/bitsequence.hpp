#ifndef BITSEQUENCE_HPP
#define BITSEQUENCE_HPP

#include <algorithm>
#include <set>
#include <ostream>
#include <limits>
#include <string>

template<typename T>
struct BitContainer
{
	T bits;
	static const int bitcount = std::numeric_limits<T>::digits;
};

template<typename T>
class BitSequence
{
public:
	BitSequence();
	BitSequence(T begin, T end);
	BitSequence(const BitSequence&);
	~BitSequence();

	bool empty() const;
	void clear();
	void reset();

	BitSequence& operator|=(const BitSequence& rhs);
	BitSequence& operator&=(const BitSequence& rhs);

	int compare(const BitSequence& rhs) const;

	bool operator<(const BitSequence& rhs) const;
	bool operator==(const BitSequence& rhs) const;
	bool operator!=(const BitSequence& rhs) const;
	bool operator&&(const BitSequence& rhs) const;
	BitSequence& operator=(const BitSequence& rhs);
	BitSequence& operator=(const std::set<T>& set);

	T begin() const;
	T end() const;

	bool get(T bit) const;
	void set(T bit);
	void reset(T bit);

	std::string toString(int bitsPerChar) const;

	//friend std::ostream& operator<<(std::ostream& os, const BitSequence& seq);

private:

	void resize(T begin, T end);

	static int getContainer(T bit);

	int contBegin;
	int contSize;

	typedef BitContainer<unsigned int> Container;
	Container *sequence;
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const BitSequence<T>& seq)
{
	os << "(" << seq.begin() << ", ";
	//for (int i = 0; i < seq.contSize; ++i)
	//	os << ", " << std::hex << seq.sequence[i].bits;
	for (T t = seq.begin(); t < seq.end(); ++t)
		 os << (seq.get(t) ? 1 : 0);
	os << ")";
	return os;
}

template<typename T>
struct BitSequenceCmp
{
	int operator()(const BitSequence<T>& lhs, const BitSequence<T>& rhs) const
	{
		/*T begin = std::min(lhs.begin(), rhs.begin());
		T end = std::max(lhs.end(), rhs.end());
		for (T t = begin; t < end; ++t)
			if (lhs.get(t) != rhs.get(t))
				return (lhs.get(t) ? 1 : 0) - (rhs.get(t) ? 1 : 0);
		return 0;*/
		return lhs.compare(rhs);
	}
};

template<typename T>
inline bool BitSequence<T>::operator<(const BitSequence& rhs) const
{
	BitSequenceCmp<T> cmp;
	return cmp(*this, rhs) < 0;
}

template<typename T>
inline bool BitSequence<T>::operator==(const BitSequence& rhs) const
{
	BitSequenceCmp<T> cmp;
	return cmp(*this, rhs) == 0;
}

template<typename T>
inline bool BitSequence<T>::operator!=(const BitSequence& rhs) const
{
	BitSequenceCmp<T> cmp;
	return cmp(*this, rhs) != 0;
}

template<typename T>
BitSequence<T>& BitSequence<T>::operator=(const std::set<T>& set)
{
	for (typename std::set<T>::const_iterator i = set.begin(); i != set.end(); ++i) this->set(*i);
	return *this;
}

template<typename T>
inline bool BitSequence<T>::operator&&(const BitSequence& rhs) const
{
	int begin = std::max(this->contBegin, rhs.contBegin) - this->contBegin;
	int end = std::min(this->contBegin + this->contSize, rhs.contBegin + rhs.contSize) - this->contBegin;
	int offset = rhs.contBegin - this->contBegin;

	for (int n = begin; n < end; ++n)
		if (0 != (this->sequence[n].bits & rhs.sequence[n + offset].bits))
			return true;

	/*T begin = std::max(this->begin(), rhs.begin());
	T end = std::min(this->end(), rhs.end());
	for (T t = begin; t < end; ++t)
		if (this->get(t) && rhs.get(t))
			return true;*/
	return false;
}

template<typename T>
inline bool BitSequence<T>::get(T bit) const
{
	int cont = getContainer(bit);
	int index = cont - this->contBegin;
	if (0 <= index && index < this->contSize)
	{
		int offset = (1 + cont) * Container::bitcount - bit - 1;
		return 0 != (this->sequence[index].bits & (1 << offset));
	}
	else return false;
}

template<typename T>
inline void BitSequence<T>::reset(T bit)
{
	int cont = getContainer(bit);
	int index = cont - this->contBegin;
	if (0 <= index && index < this->contSize)
	{
		int offset = (1 + cont) * Container::bitcount - bit - 1;
		this->sequence[index].bits &= ~(1 << offset);
	}
}

template<typename T>
inline bool BitSequence<T>::empty() const
{
	for (int n = 0; n < this->contSize ; ++n)
		if (this->sequence[n].bits != 0) return false;
	return true;
}

template<typename T>
inline void BitSequence<T>::clear()
{
	delete[] this->sequence;
	this->sequence = 0;
	this->contSize = 0;
}

template<typename T>
inline void BitSequence<T>::reset()
{
	for (int n = 0; n < this->contSize; ++n)
		this->sequence[n].bits = 0;
}

template<typename T>
inline T BitSequence<T>::begin() const
{
	return this->contBegin * Container::bitcount;
}

template<typename T>
inline T BitSequence<T>::end() const
{
	return (this->contBegin + this->contSize) * Container::bitcount;
}

template<typename T>
inline int BitSequence<T>::getContainer(T bit)
{
	return (bit < 0 ? (bit - Container::bitcount + 1) / Container::bitcount : bit / Container::bitcount);
}

#endif
