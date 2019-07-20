#ifndef _STRING_H_
#define _STRING_H_

#define _CRT_SECURE_NO_WARNINGS	

#include <string>
#include <iostream>

/***************************************************************
//
// Дати визначення власного класу String обробки рядків символів типу char, 
// звертаючи особливу увагу на обґрунтованість вживання методів і утиліт.
// Мінімальний набір операцій включатиме:
//		копіювальне присвоєння = ; 
//		конкатенацію + ;
//		суміщене присвоєння += ; 
//		порівняння == , != , <, >, <= , >= ;  
//		селектор і модифікатор [];
//		зведення типу char*, string з бібліотеки STL.
// Зокрема передбачити можливість виконання змішаних операцій над рядками
// власного класу String, класу string з бібліотеки STL та С - рядками.
//
//	developed by Anastasia Vladimirska
//	on 2018/09/07
//	Version 2.5
//
****************************************************************/

class String
{
public:

	String();
	explicit String(const char);
	explicit String(const char* const);
	explicit String(const std::string&);

	String(const String&);

	~String();

	const char* c_str() const    { return _allocator; }
	operator std::string() const { return std::string(_allocator); }

	String& operator=(const String&);
	String& operator=(const char);
	String& operator=(const char* const);
	String& operator=(const std::string&);

	String& operator+=(const String&);
	String& operator+=(const char);
	String& operator+=(const char* const);
	String& operator+=(const std::string&);

	char& operator[](const size_t);
	const char operator[](const size_t) const;

	size_t length() const { return _len; }

	bool empty() const { return _len == 0; }

	void clear();

	class BadString;

private:

	size_t _len;

	char* _allocator;

};

std::ostream& operator<<(std::ostream&, const String&);

// Non-member functions, where both arguments are equal 

inline bool operator==(const String& a, const String& b)
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
inline bool operator!=(const String& a, const String& b)
{
	return !(a == b);
};
inline bool operator< (const String& a, const String& b)
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
inline bool operator<=(const String& a, const String& b)
{
	if (a == b)
		return true;
	return a < b;
};
inline bool operator> (const String& a, const String& b)
{
	return !(a <= b);
};
inline bool operator>=(const String & a, const String & b)
{
	return !(a < b);
}

inline const String operator+(const String & a, const String & b)
{
	String res(a);
	return res += b;
}

inline const String operator+(const String & s, const char c)
{
	String res(s);
	return res += c;
}

inline const String operator+(const char c, const String & s) //revert parameters
{
	String res(c);
	return res += s;
}

inline const String operator+(const String & s, const char * const ps)
{
	String res(s);
	return res += ps;
}

inline const String operator+(const char * const ps, const String & s) //revert parameters
{
	String res(ps);
	return res += s;
}

inline const String operator+(const String & s, const std::string & ss)
{
	String res(s);
	return res += ss;
}

inline const String operator+(const std::string & ss, const String & s) //revert parameters
{
	String res(ss);
	return res += s;
}

class String::BadString {

public:
	BadString(std::string reason = "", const size_t index = 0) : _reason(reason), _index(index) {}
	~BadString() {}

	void diagnose() const
	{
		std::cerr << _reason;
		if (_index != 0)
			std::cerr << ": " << _index;
		std::cerr << std::endl;
	}

private:
	const size_t _index;
	const std::string _reason;
};

#endif //_STRING_H_