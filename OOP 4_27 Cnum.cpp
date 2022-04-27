#include <iostream>
#include <vector>
using namespace std;

class Cnum
{
private:
	int a[5];
	int m_cnt;

public:
	Cnum(int t[], int n);
	void sort(int t[], int n);
	int convert(int n);
	void fun();
	void print();
};

Cnum::Cnum(int t[], int n)
{
	m_cnt = n;

	for (int i = 0; i < n; i++)
	{
		a[i] = t[i];
	}
	for (int i = n; i < 5; i++)
	{
		a[i] = 0;
	}
}

void Cnum::sort(int t[], int n)
{
	for (int x = 0; x < m_cnt; x++)
		for (int y = x; y < m_cnt; y++)
		{
			if (t[x] > t[y])
				swap(t[x], t[y]);
		}
}

int Cnum::convert(int n)
{
	//这里用了vector来保存每一位数
	vector<int> _val;
	int _data = a[n];
	do
	{
		_val.push_back(_data % 10);
		_data /= 10;
	} while (_data);

restart:  //一旦调用了erase函数，vector尺寸就会减一，此时x、y值也有可能需要改变，所以使用goto来重新执行，以防出现溢出
	for (int x = _val.size() - 1; x >= 0; x--) //因为输入的时候是从个位开始的，所以用x--从大到小开始循环
	{
		for (int y = x - 1; y >= 0; y--)
		{
			if (_val[x] == _val[y])
			{
				_val.erase(_val.begin() + y);
				goto restart;
			}
		}
	}

	int _result = 0;
	for (int x = _val.size() - 1; x >= 0; x--)
	{
		_result = _result * 10 + _val[x];
	}

	return _result;
}

void Cnum::fun()
{
	for (int i = 0; i < m_cnt; i++)
		a[i] = convert(i);

	sort(a, m_cnt);
}

void Cnum::print()
{
	for (int i = 0; i < m_cnt; i++)
		cout << a[i] << ' ';
}

int main()
{
	int _a[5];
	int input = 0, cnt = 0;
	while (cin >> input)
	{
		_a[cnt] = input;
		cnt++;
		if (cin.get() == '\n')
			break;
	}
	Cnum test(_a, cnt);
	test.fun();
	test.print();
	return 0;
}
