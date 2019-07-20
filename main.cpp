#include <iostream>
using namespace std;
#include "String.h"

int main()
{
	cout << "\n================Creating and copying===============\n" << endl;
	char * cString = "Chars!";
	String empty = String();
	String fromCstr(cString);
	string stlString = "A standart string!";
	String fromStlString(stlString);
	String copy(fromCstr);
	String charry('a');
	cout << fromCstr << endl;
	cout << empty << endl;
	cout << fromStlString << endl;
	cout << copy << endl;
	cout << charry << endl;
	cout << "\n===================Assigning===================\n" << endl;
	char * anotherArray = "I was made from chars";
	fromStlString = anotherArray;
	cout << "String = char * " << fromStlString << endl;
	fromStlString = 'b';
	cout << "String = char " << fromStlString << endl;
	string normalString = copy;
	cout << "Normal string = String " << normalString << endl;
	const char * chars = copy.c_str();
	cout << "Normal array = String " << endl;
	for (size_t i = 0; i < copy.length(); ++i)
	{
		cout << chars[i] << ' ';
	}
	cout << endl;

	if (empty.empty())
		cout << "Empty String!" << endl;
	empty = string("This is a string");
	cout << "String = string " << empty << endl;
	cout << "\tclearing... ";
	empty.clear();
	if (empty.empty())
		cout << "Empty again!" << endl;
	if (empty < fromStlString)
		cout << "Less" << endl;
	else cout << "Bigger or equal" << endl;
	cout << "\n==========Conctatinating and checking if equal==========\n" << endl;
	cout << charry + fromStlString << endl;
	charry += chars;
	cout << charry << endl;
	charry += normalString;
	cout << charry << endl;
	if (charry > String(normalString))
		cout << "Our is bigger!" << endl;
	if (fromCstr != fromStlString)
		cout << fromCstr << " and " << fromStlString << " are not equal" << endl;
	else cout << fromCstr << " and " << fromStlString << " are equal" << endl;
	cout << "\n===================Taking elements===================\n" << endl;
	try
	{
		for (size_t i = 0; i < 100; ++i)
		{
			cout << "String[" << i << "] = " << fromStlString[i] << endl;
		}
	}
	catch (String::BadString bs)
	{
		cerr << "ERROR!" << endl;
		bs.diagnose();
	}
	return 0;
}