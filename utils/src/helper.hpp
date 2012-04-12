#ifndef HELPER_H
#define HELPER_H

#include <set>
#include <algorithm>

template<class T> std::set<T>& set_union_add(std::set<T>& dest, const std::set<T>& other)
{
	dest.insert(other.begin(), other.end());

	return dest;
}

template<class T> std::set<T>& set_intersect(std::set<T>& dest, const std::set<T>& other)
{
	typename std::set<T>::iterator iDest = dest.begin();
	while (iDest != dest.end())
		if (other.find(*iDest) == other.end())
		{
			typename std::set<T>::iterator iErase = iDest;
			++iDest;
			dest.erase(iErase);
		}
		else ++iDest;
	return dest;
}

template<class T> bool intersect(const T& set1, const T& set2)
{
	for (typename T::const_iterator i = set1.begin(); i != set1.end(); ++i)
		if (set2.find(*i) != set2.end())
			return true;
	return false;
}

template<class T> inline T& minimize(T& var, const T& other)
{
	return var < other ? var : var = other;
}

template<class T> inline T& maximize(T& var, const T& other)
{
	return var > other ? var : var = other;
}

template<class T> inline T gcd(T a, T b)
{
	if (a == 0) return b;
	while (b != 0)
		if (a > b) a -= b;
		else b -= a;
	return a;
}

#endif
