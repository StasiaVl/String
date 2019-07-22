#pragma once
#include "AnyString.h"
// ========================= Specialisation for wchar_t ================================
template<>
class AnyString<wchar_t>
{
private:
	struct StringRep
	{
		wchar_t* _allocator;
		size_t _len;
		size_t _refCounter;
		StringRep(size_t size, const wchar_t* s) : _refCounter(1), _len(size), _allocator(new wchar_t[size + 1])
		{
			if (s == 0) throw BadString("Undefined pointer");
			if (_allocator == 0) throw BadString("No more space in the system");
			wcscpy_s(_allocator, size + 1, s);
		}
		StringRep(size_t size, const AnyString<wchar_t>& s) : _refCounter(1), _len(size), _allocator(new wchar_t[size + 1])
		{
			if (_allocator == 0) throw BadString("No more space in the system");
			wcscpy_s(_allocator, size + 1, s._rep->_allocator);

		}
		StringRep(size_t size, const wstring& s) : _refCounter(1), _len(size), _allocator(new wchar_t[size + 1])
		{
			if (_allocator == 0) throw BadString("No more space in the system");
			wcscpy_s(_allocator, size + 1, s.c_str());
		}
		~StringRep() { delete[] _allocator; }
		StringRep* getOwnCopy()
		{
			if (_refCounter == 1) return this;
			_refCounter--;
			return new StringRep(_len, _allocator);
		}
		void assign(size_t size, const wchar_t* s)
		{
			if (s == 0) throw BadString("Undefined pointer");
			if (_len != size)
			{
				delete[] _allocator;
				_len = size;
				_allocator = new wchar_t[_len + 1];
			}
			wcscpy_s(_allocator, size + 1, s);
		}

	private:
		StringRep(const StringRep&);
		StringRep& operator= (const StringRep&);
	};
	StringRep* _rep;
	class CharProxy
	{
		friend class AnyString<wchar_t>;
	private:
		AnyString<wchar_t> & _proxyship;
		size_t _index;
		CharProxy(AnyString<wchar_t>& s, size_t index) : _proxyship(s), _index(index) {};
	public:
		operator wchar_t() const { return _proxyship.read(_index); }
		CharProxy& operator=(wchar_t c){  _proxyship.write(_index, c); return *this;}
	};

public:
	AnyString() : _rep(new StringRep(0, L""))
	{
		if (_rep == 0) throw BadString("No more space in the system");
	}
	AnyString(const wstring& s) : _rep(new StringRep(s.length(), s))
	{
		if (_rep == 0) throw BadString("No more space in the system");
	}
	AnyString(const wchar_t* ps)
	{
		if (ps == 0) throw BadString("Undefined pointer");
		_rep = new StringRep(wcslen(ps), ps);
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

	const wchar_t* c_str() const { return _rep->_allocator; }
	const wstring toWString() const { return wstring(_rep->_allocator); }

	AnyString& operator=(const AnyString& as)
	{
		as._rep->_refCounter++;
		assert((&as == this) ? (_rep->_refCounter > 1) : true);
		if (--_rep->_refCounter == 0) 
			delete _rep;
		_rep = as._rep;
		return *this;
	}
	AnyString& operator=(const wchar_t* as)
	{
		if (as == 0) throw BadString("Undefined pointer");
		if (_rep->_refCounter == 1)
			_rep->assign(wcslen(as), as);
		else
		{
			_rep->_refCounter--;
			_rep = new StringRep(wcslen(as), as);
		}

		return *this;
	}
	AnyString& operator=(const wstring& s)
	{
		return *this = s.c_str();
	}

	wchar_t read(size_t i) const { return _rep->_allocator[i]; }
	void write(size_t i, wchar_t c)
	{
		_rep = _rep->getOwnCopy();
		_rep->_allocator[i] = c;
	}

	AnyString& operator+=(const AnyString& other)
	{
		_rep = _rep->getOwnCopy();
		size_t size = length() + other.length();
		wstring allc = toWString() + other.toWString();
		_rep->assign(size, allc.c_str());
		return *this;
	}
	AnyString& operator+=(const wstring& other)
	{
		_rep = _rep->getOwnCopy();
		size_t size = length() + other.length();
		wstring allc = toWString() + other;
		_rep->assign(size, allc.c_str());
		return *this;
	}
	CharProxy operator[](size_t i)
	{
		if (i < 0 || _rep->_len <= i) throw BadString("Bad index", i);
		return CharProxy(*this, i);
	}
	wchar_t operator[](size_t i) const
	{
		if (i < 0 || _rep->_len <= i) throw BadString("Bad index", i);
		return _rep->_allocator[i];
	}
	size_t length() const { return _rep->_len; }
	bool empty() const { return _rep->_len == 0; }
};
inline const AnyString<wchar_t> operator+(const wstring& a, const AnyString<wchar_t> & b)
{
	AnyString<wchar_t> res(a);
	return res += b;
}
inline const AnyString<wchar_t> operator+(const AnyString<wchar_t>& a, const wstring& b)
{
	AnyString<wchar_t> res(a);
	return res += b;
}
inline bool operator==(const wstring& a, const AnyString<wchar_t>& b)
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
inline bool operator==(const AnyString<wchar_t>& a, const wstring& b)
{
	return b == a;
}
inline bool operator!=(const wstring& a, const AnyString<wchar_t>& b)
{
	return !(a == b);
}
inline bool operator!=(const AnyString<wchar_t>& a, const wstring& b)
{
	return !(a == b);
}
inline bool operator< (const wstring& a, const AnyString<wchar_t>& b)
{
	AnyString<wchar_t> r(a);
	return r < b;
}
inline bool operator< (const AnyString<wchar_t>& a, const wstring& b)
{
	AnyString<wchar_t> r(b);
	return a < r;
}
inline bool operator<=(const wstring& a, const AnyString<wchar_t>& b)
{
	if (a == b)
		return true;
	return a < b;
}
inline bool operator<=(const AnyString<wchar_t>& a, const wstring& b)
{
	if (a == b)
		return true;
	return a < b;
}
inline bool operator> (const AnyString<wchar_t>& a, const wstring& b)
{
	return !(a <= b);
}
inline bool operator> (const wstring& a, const AnyString<wchar_t>& b)
{
	return !(a <= b);
}
inline bool operator>=(const wstring& a, const AnyString<wchar_t>& b)
{
	return !(a < b);
}
inline bool operator>=(const AnyString<wchar_t>& a, const wstring& b)
{
	return !(a < b);
}
wostream& operator<<(wostream& os, const AnyString<wchar_t>& asw)
{
	for (size_t i = 0; i < asw.length(); ++i)
	{
		os << static_cast<wchar_t>(asw[i]);
	}
	return os;
}