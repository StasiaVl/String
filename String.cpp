#include "String.h"

String::String() : _allocator(new char[1]), _len(0)
{
	_allocator[0] = '\0';
	return;
}

String::String(const char c) : _allocator(new char[2]), _len(1)
{
	_allocator[0] = c;
	_allocator[1] = '\0';
	return;
}

String::String(const char * const ps)
{
	if (ps == 0)
		throw BadString("Attempted to use not defined pointer");
	_len = strlen(ps);
	_allocator = new char[_len + 1];
	strcpy(_allocator, ps);
	return;
}

String::String(const std::string & s) : _allocator(new char[s.length() + 1]), _len(s.length())
{
	strcpy(_allocator, s.c_str());
	return;
}

String::String(const String & s) : _len(s._len), _allocator(new char[_len + 1])
{
	strcpy(_allocator, s._allocator);
	return;
}

String::~String()
{
	delete[] _allocator;
	_allocator = 0;
	return;
}

String & String::operator=(const String & s)
{
	if (this != &s)
	{
		delete[] _allocator;
		_len = s._len;
		_allocator = new char[_len + 1];
		strcpy(_allocator, s._allocator);
	}
	return *this;
}

String & String::operator=(const char c)
{
	delete[] _allocator;
	_len = 1;
	_allocator = new char[2];
	_allocator[0] = c;
	_allocator[1] = '\0';
	return *this;
}

String & String::operator=(const char * const ps)
{
	if (ps == 0)
		throw BadString("Attempted to use not defined pointer");
	delete[] _allocator;
	for (_len = 0; ps[_len]; ++_len);
	_allocator = new char[_len + 1];
	strcpy(_allocator, ps);
	return *this;
}

String & String::operator=(const std::string & s)
{
	delete[] _allocator;
	_len = s.length();
	_allocator = new char[_len + 1];
	strcpy(_allocator, s.c_str());
	return *this;
}

String & String::operator+=(const String & s)
{
	size_t newLen = _len + s._len;
	char* newAllocator = new char[newLen + 1];
	strcpy(newAllocator, _allocator);
	delete[] _allocator;
	_allocator = newAllocator;
	strcpy(_allocator + _len, s._allocator);
	_len = newLen;
	return *this;
}

String & String::operator+=(const char c)
{
	++_len;
	char* newAllocator = new char[_len + 1];
	strcpy(newAllocator, _allocator);
	delete[] _allocator;
	_allocator = newAllocator;
	_allocator[_len - 1] = c;
	_allocator[_len] = '\0';
	return *this;
}

String & String::operator+=(const char * const ps)
{
	if (ps == 0)
		throw BadString("Attempted to use not defined pointer");
	size_t len = strlen(ps);
	char* newAllocator = new char[len + _len + 1];
	strcpy(newAllocator, _allocator);
	delete[] _allocator;
	_allocator = newAllocator;
	strcpy(_allocator + _len, ps);
	_len += len;
	return *this;
}

String & String::operator+=(const std::string & s)
{
	return *this += s.c_str();
}

char & String::operator[](const size_t i)
{
	if (0 <= i && i < _len)
		return _allocator[i];
	throw BadString("Index out of range", i);
}

const char String::operator[](const size_t i) const
{
	if (0 <= i && i < _len)
		return _allocator[i];
	throw BadString("Index out of range: ", i);
}

void String::clear()
{
	delete[] _allocator;
	_allocator = new char[1];
	_len = 0;
	_allocator[0] = '\0';
}

std::ostream & operator<<(std::ostream & os, const String & s)
{
	for (size_t i = 0; i < s.length(); ++i)
		os << s[i];
	return os;
}