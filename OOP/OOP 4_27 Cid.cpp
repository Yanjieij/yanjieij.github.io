#include <iostream>
using namespace std;
class Cid
{
private:
	char m_s[19];
	char m_oris[15];
	char m_x[11];
	int m_w[17];

public:
	Cid(char *str);
	void fun();
	void print();
};

Cid::Cid(char *str)
{
	int _w[17] = {7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2};
	for (int i = 0; i < 17; i++)
		m_w[i] = _w[i];

	char _x[11] = {'1', '0', 'X', '9', '8', '7', '6', '5', '4', '3', '2'};
	for (int i = 0; i < 11; i++)
		m_x[i] = _x[i];

	for (int i = 0; i < 15; i++)
		m_s[i] = str[i];
	for (int i = 15; i < 19; i++)
		m_s[i] = ' ';

	//深拷贝s，保存原身份证
	for (int i = 0; i < 15; i++)
		m_oris[i] = m_s[i];
}

void Cid::fun()
{
	//扩展为17位
	m_s[6] = '1';
	m_s[7] = '9';
	for (int i = 8; i < 17; i++)
		m_s[i] = m_oris[i - 2];
	//计算权值
	int _val = 0;
	for (int i = 0; i < 17; i++)
		_val += (int)(m_s[i] - 48) * m_w[i];
	//填充第18位
	int value = _val % 11;
	m_s[17] = m_x[value];
}

void Cid::print()
{
	// cout << "原身份证号：" << oris << endl;
	// cout << "扩展后的身份证号：" << s << endl;
	cout << "原身份证号：";
	for (int i = 0; i < 15; i++)
		cout << m_oris[i];
	cout << endl;

	cout << "扩展后的身份证号：";
	for (int i = 0; i < 19; i++)
		cout << m_s[i];
	cout << endl;
}

int main()
{
	char id[16] = {' '};
	cin >> id;
	Cid test(id);
	test.fun();
	test.print();
	return 0;
}
