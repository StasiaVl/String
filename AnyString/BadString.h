#ifndef _BADSTRING_H_
#define _BADSTRING_H_

#include <iostream>
#include <string>
using namespace std;

class BadString
{
private:
	string _reason;
	size_t _index;

public:
	BadString(string reason = "", size_t index = 0) : _reason(reason), _index(index) {}
	~BadString() {};

	void show() const
	{
		cerr << _reason << " at index " << _index << endl;
	}
};
#endif // _BADSTRING_H_