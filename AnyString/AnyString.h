#ifndef _ANYSTRING_H_
#define _ANYSTRING_H_
#include <cassert>
#include <iostream>
#include <string>
using namespace std;
#include "BadString.h"
/*
Відкладене копіювання
Застосувати техніку маніпуляторів для визначення рядків,
складених із об'єктів довільного типу (символів char,
розширеного набору символів w_char, розфарбованих символів,
форматованих символів тощо), придатних для їх для відкладеного копіювання.

by Anastasia Vladimirska
on 2018.10.27
Version 2.0
*/

template<typename T>
class AnyString
{
private:
	struct StringRep;
	StringRep* _rep;
	class CharProxy;
public:
	AnyString(const T* s, const size_t length)
	{
		if (s == 0)	throw BadString("Undefined pointer");
		_rep = new StringRep(length, s);
		if (_rep == 0) throw BadString("No more space in the system");
	}
	AnyString(const AnyString& other)
	{
		other._rep->_refCounter++;
		_rep = other._rep;
	}
	~AnyString()
	{
		if (--_rep->_refCounter == 0)
			delete _rep;
	}

	AnyString& operator=(const AnyString& other)
	{
		other._rep->_refCounter++;
		assert((&other == this) ? (_rep->_refCounter > 1) : true);
		if (--_rep->_refCounter == 0)
			delete _rep;
		_rep = other._rep;
		return *this;
	}
	const T& read(size_t i) const { return _rep->_allocator[i]; }
	void write(size_t i, const T& chars)
	{
		_rep = _rep->getOwnCopy();
		_rep->_allocator[i] = chars;
	}

	AnyString& operator+=(const AnyString& other)
	{
		_rep = _rep->getOwnCopy();
		size_t size = length() + other.length();
		T * allc = new T[size];
		copyAllocator(length(), _rep->_allocator, allc);
		copyAllocator(other.length(), other.look(), allc, length());
		_rep->assign(size, allc);
		return *this;
	}

	CharProxy operator[](size_t i)
	{
		if (i < 0 || _rep->_len <= i) throw BadString("Bad index", i);
		return CharProxy(*this, i);
	}
	const T& operator[](size_t i) const
	{
		if (i < 0 || _rep->_len <= i) throw BadString("Bad index", i);
		return _rep->_allocator[i];
	}
	const T* look() const { return _rep->_allocator; }
	size_t length() const { return _rep->_len; }
	bool empty() const { return _rep->_len == 0;}
};
///=================StringRep=====================
template <typename T>
struct AnyString<T>::StringRep 
{
	T* _allocator;
	size_t _len;
	size_t _refCounter;
	StringRep(size_t size, const T* s) : _refCounter(1), _len(size), _allocator(new T[size])
	{
		if (s == 0) throw BadString("Undefined pointer");
		if (_allocator == 0) throw BadString("No more space in the system");
		copyAllocator(size, s, _allocator);
	}
	~StringRep() { delete[] _allocator; }
	StringRep* getOwnCopy()
	{
		if (_refCounter == 1) return this;
		--_refCounter;
		return new StringRep(_len, _allocator);
	}
	void assign(size_t size, const T* s)
	{
		if (s == 0) throw BadString("Undefined pointer");
		if (_len != size)
		{
			delete[] _allocator;
			_len = size;
			_allocator = new T[_len];
		}
		copyAllocator(size, s, _allocator);
	}
private:
	StringRep(const StringRep&);
	StringRep& operator= (const StringRep&);
};
///================CharProxy===========================
template <typename T>
class AnyString<T>::CharProxy
{
	friend class AnyString<T>;
private:
	AnyString<T>& _proxyship;
	size_t _index;
	CharProxy(AnyString<T>& str, size_t index) : _proxyship(str), _index(index) {}
public:
	operator T() const { return _proxyship.read(_index); }
	CharProxy& operator=(T c){  _proxyship.write(_index, c); return *this;	}
};
///===============Other utilities=====================
template <typename T>
void copyAllocator(size_t length, const T* from, T* into, size_t begin = 0)
{
	if (from == 0 || into == 0)
		throw BadString("Undefined pointer");

	for (size_t i = begin, j = 0; i < length+begin; ++i, ++j)
	{
		into[i] = from[j];
	}
}
template <typename T>
inline const AnyString<T> operator+(const AnyString<T> & a, const AnyString<T> & b)
{
	AnyString<T> res(a);
	return res += b;
}
template <typename T>
inline bool operator==(const AnyString<T>& a, const AnyString<T>& b)
{
	bool equal = a.length() == b.length();
	if (equal)
	{
		for (size_t i = 0; i < a.length(); ++i)
		{
			equal = a[i] == b[i];
			if (!equal) break;
		}
	}
	return equal;
};
template <typename T>
inline bool operator!=(const AnyString<T>& a, const AnyString<T>& b)
{
	return !(a == b);
}
template <typename T>
inline bool operator< (const AnyString<T>& a, const AnyString<T>& b)
{
	bool res = true;
	size_t i = 0;
	while (res && i < a.length() && i < b.length())
	{
		res = res && a[i] == b[i];
		++i;
	}
	if (res)
		return a.length() < b.length();
	--i;
	return a[i] < b[i];
};
template <typename T>
inline bool operator<=(const AnyString<T>& a, const AnyString<T>& b)
{
	if (a == b)
		return true;
	return a < b;
};
template <typename T>
inline bool operator> (const AnyString<T>& a, const AnyString<T>& b)
{
	return !(a <= b);
};
template <typename T>
inline bool operator>=(const AnyString<T> & a, const AnyString<T> & b)
{
	return !(a < b);
}
template <typename T>
ostream& operator<<(ostream& os, const AnyString<T>& as)
{
	for (size_t i = 0; i < as.length(); ++i)
	{
		os << static_cast<T>(as[i]);
	}
	return os;
}

#endif