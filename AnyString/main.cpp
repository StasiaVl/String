#include <iostream>
using namespace std;

#include "AnyString.h"
#include "AnyString_char.h"
#include "AnyString_wchart.h"

int main()
{
	cout << "\n==================AnyString<int>==================\n" << endl;
	int * pn = new int[5];
	for (int i = 0; i < 5; ++i)
		pn[i] = i;
	AnyString<int> nStr1(pn, 5);
	cout << "String of ints: " << nStr1 << endl;
	AnyString<int> nStr2(nStr1);
	cout << "Its copy: " << nStr2;
	if (nStr1 == nStr2)
		cout << ". Copy is done well" << endl;
	for (size_t i = 0; i < nStr2.length(); ++i)
		nStr2[i] = nStr2[i] + 5;
	cout << "Changed: " << nStr2;
	if (nStr1 < nStr2)
		cout << " Now original is smaller" << endl;
	cout << "Adding strings: \n" << nStr2 + nStr1 << endl;
	nStr2 += nStr2;
	cout << nStr2 << endl;

	cout << "\n==================AnyString<char>=================\n" << endl;
	string s = "string";
	AnyString<char> cStr1(s);
	cout << "AnyString made of std::string: " << cStr1 << endl;
	AnyString<char> cStr2;
	if (cStr2.empty())
		cout << "Empty AnyString of chars" << endl;
	cStr2 = cStr1;
	for (size_t i = 0; i < cStr2.length(); ++i)
		cStr2.write(i, i + 40);
	AnyString<char> cStr3(cStr2.c_str());
	cout << "AnyString from char array: " << cStr3 << endl;
	cStr3 = s;
	cStr1.write(2, 'W');
	if (cStr1 != s)
	{
		cStr3 += s;
		cout << cStr1 + cStr3 << endl;
	}
	if (cStr1 > cStr3)
		cout << cStr1 << " is bigger then " << cStr3 << endl;
	

	cout << "\n==================AnyString<wchar_t>=================\n" << endl;
	AnyString<wchar_t> wStr1(L"Strange string");
	cout << "AnyString made of wstring: ";
	wcout << wStr1 << endl;
	AnyString<wchar_t> wStr2;
	if (wStr2.empty())
		cout << "Empty AnyString of wchar_t" << endl;
	wStr2 = wStr1.c_str();
	cout << "AnyString frow array of wchar_t: ";
	wcout << wStr2 << endl;
	AnyString<wchar_t> wStr3(wStr2);
	for (size_t i = 0; i < wStr3.length(); ++i)
		wStr3.write(i, i + 65);
	cout << "Changed copy: ";
	wcout << wStr3 << endl;
	wchar_t * w = L"abc";
	wStr3 = w + wStr3;
	cout << "Now it's array of wchars + AnyString<wchar_t> : ";
	wcout << wStr3 << endl;
	wStr1.write(1, 'W');
	if (w >= wStr1)
	{
		wcout << wStr1 << " is smaller then array" << endl;
	}

	return 0;
}