#include <iostream>
#include <cstdio>
using namespace std;


int f(int x, int y)
{
	return 2 * x + y;
}

extern int ext;
void Func();

void Show(int x, int y)
{
	char str[50];

	sprintf(str, "f(%d, %d) = %d", x, y, f(y, x));
	cout << str << endl;
}


	const char* MONTH[] = {
		"1","2","3","4","5"
	};

	const char* GetMonth(int month)
	{
		if (1 <= month && month <= 5)
		{
			return MONTH[month - 1];
		}
		return 0;
	};

	void ShowAddresses(char* pointer)
	{
		cout << "pointer : " << (size_t)pointer << endl;
		for (int i = 0; i < 10; ++i)
		{
			cout << "&pointer [" << i << " ]  :" << (size_t)&pointer[i] << endl;
		}
		cout << sizeof pointer / sizeof pointer[0] << endl;


}

	int Compare(int a, int b)
	{
		return a < b ? a : a > b ? b : 1;
	}

int main()
{

	Func();
	ext = 11;
	Func();

	const char* str;
	str = "abcd";
	


	/*int month;
	const char* newMonth = "March";
	MONTH[2] = newMonth;
	int a;
	int& r = a;
	r = 5;
	cout << "a = " << a << endl;
	cout << "r = " << r << endl;
	cout << "r = " << &r << endl;
	int b = 1;

	r = b;
	cout << "a = " << a << endl;
	cout << "r = " << r << endl;
	cout << "r = " << &r << endl;
	const char* str = "Hello";
	cout << str[0] << endl;

	cout << "ŒŽ" << flush;
	cin >> month;
	
	const char* name = GetMonth(month);
	if (name == 0)
	{
		cout << "–³Œø" << endl;
	}
	else
	{
		cout << name << endl;
	}*/
}