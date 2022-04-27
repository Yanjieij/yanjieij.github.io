#include "Cnum.h"
int main()
{
	int _a[5];
	int input = 0, cnt = 0;
	while (cin >> input)
	{
		_a[cnt] = input;
		cnt++;
		if (cin.get() == '\n') break;
	}
	Cnum test(_a, cnt);
	test.fun();
	test.print();
	return 0;
}