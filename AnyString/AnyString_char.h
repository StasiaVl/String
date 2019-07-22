#pragma once
#include "AnyString.h"
// ========================= Specialisation for char =============================
template<>
class AnyString<char>
{
private:
	struct StringRep
	{
		char* _allocator;
		size_t _len;
		size_t _refCounter;
		StringRep(size_t size, const char* s) : _refCounter(1), _len(size), _allocator(new char[size + 1])
		{
			if (s == 0) throw BadString("Undefined pointer");
			if (_allocator == 0) throw BadString("No more space in the system");
			strcpy_s(_allocator, size + 1, s);
		}
		StringRep(size_t size, const AnyString<char>& s) : _refCounter(1), _len(size), _allocator(new char[size + 1])
		{
			if (_allocator == 0) throw BadString("No more space in the system");
			strcpy_s(_allocator, size + 1, s._rep->_allocator);
		}
		StringRep(size_t size, const string& s) : _refCounter(1), _len(size), _allocator(new char[size + 1])
		{
			if (_allocator == 0) throw BadString("No more space in the system");
			strcpy_s(_allocator, size + 1, s.c_str());
		}
		~StringRep() { delete[] _allocator; }
		StringRep* getOwnCopy()
		{
			if (_refCounter == 1) return this;
			_refCounter--;
			return new StringRep(_len, _allocator);
		}
		void assign(size_t size, const char* s)
		{
			if (s == 0) throw BadString("Undefined pointer");
			if (_len != size)
			{
				delete[] _allocator;
				_len = size;
				_allocator = new char[_len + 1];
			}
			strcpy_s(_allocator, size + 1, s);
		}

	private:
		StringRep(const StringRep&);
		StringRep& operator= (const StringRep&);
	};
	StringRep* _rep;
	class CharProxy
	{
		friend class AnyString<char>;
	private:
		AnyString<char> & _proxyship;
		size_t _index;
		CharProxy(AnyString<char>& s, size_t index) : _proxyship(s), _index(index) {};
	public:
		operator char() const { return _proxyship.read(_index); }
		CharProxy& operator=(char c){  _proxyship.write(_index, c);	return *this;}
	};

public:
	AnyString() : _rep(new StringRep(0, ""))
	{
		if (_rep == 0) throw BadString("No more space in the system");
	}
	AnyString(const string& s) : _rep(new StringRep(s.length(), s))
	{
		if (_rep == 0) throw BadString("No more space in the system");
	}
	AnyString(const char* ps)
	{
		if (ps == 0) throw BadString("Undefined pointer");
		_rep = new StringRep(strlen(ps), ps);
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

	const char* c_str() const { return _rep->_allocator; }
	const string toString() const { return string(_rep->_allocator); }

	AnyString& operator=(const AnyString& as)
	{
		as._rep->_refCounter++;
		assert((&as == this) ? (_rep->_refCounter > 1) : true);
		if (--_rep->_refCounter == 0)
			delete _rep;
		_rep = as._rep;
		return *this;
	}
	AnyString& operator=(const char* as)
	{
		if (as == 0) throw BadString("Undefined pointer");
		if (_rep->_refCounter == 1)
			_rep->assign(strlen(as), as);
		else
		{
			_rep->_refCounter--;
			_rep = new StringRep(strlen(as), as);
		}
		return *this;
	}
	AnyString& operator=(const string& s)
	{
		return *this = s.c_str();
	}

	char read(size_t i) const { return _rep->_allocator[i]; }
	void write(size_t i, char c)
	{
		_rep = _rep->getOwnCopy();
		_rep->_allocator[i] = c;
	}

	AnyString& operator+=(const AnyString& other)
	{
		_rep = _rep->getOwnCopy();
		size_t size = length() + other.length();
		string allc = toString() + other.toString();
		_rep->assign(size, allc.c_str());
		return *this;
	}
	AnyString& operator+=(const string& other)
	{
		_rep = _rep->getOwnCopy();
		size_t size = length() + other.length();
		string allc = toString() + other;
		_rep->assign(size, allc.c_str());
		return *this;
	}

	CharProxy operator[](size_t i)
	{
		if (i < 0 || _rep->_len <= i) throw BadString("Bad index", i);
		return CharProxy(*this, i);
	}
	const char operator[](size_t i) const
	{
		if (i < 0 || _rep->_len <= i) throw BadString("Bad index", i);
		return _rep->_allocator[i];
	}
	size_t length() const { return _rep->_len; }
	bool empty() const { return _rep->_len == 0; }
};

inline const AnyString<char> operator+(const string& a, const AnyString<char> & b)
{
	AnyString<char> res(a);
	return res += b;
}
inline const AnyString<char> operator+(const AnyString<char>& a, const string& b)
{
	AnyString<char> res(a);
	return res += b;
}
inline bool operator==(const string& a, const AnyString<char>& b)
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
}
inline bool operator==(const AnyString<char>& a, const string& b)
{
	return b == a;
}
inline bool operator!=(const string& a, const AnyString<char>& b)
{
	return !(a == b);
}
inline bool operator!=(const AnyString<char>& a, const string& b)
{
	return !(a == b);
}
inline bool operator< (const string& a, const AnyString<char>& b)
{
	AnyString<char> r(a);
	return r < b;
}
inline bool operator< (const AnyString<char>& a, const string& b)
{
	AnyString<char> r(b);
	return a < r;
}
inline bool operator<=(const string& a, const AnyString<char>& b)
{
	if (a == b)
		return true;
	return a < b;
}
inline bool operator<=(const AnyString<char>& a, const string& b)
{
	if (a == b)
		return true;
	return a < b;
}
inline bool operator> (const AnyString<char>& a, const string& b)
{
	return !(a <= b);
}
inline bool operator> (const string& a, const AnyString<char>& b)
{
	return !(a <= b);
}
inline bool operator>=(const string& a, const AnyString<char>& b)
{
	return !(a < b);
}
inline bool operator>=(const AnyString<char>& a, const string& b)
{
	return !(a < b);
}