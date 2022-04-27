#include <iostream>
using namespace std;

class CNumber
{
public:
	CNumber();
	int isPrime(int x); //判断是否为素数
	void fund();		//寻找30以内的所有符合条件的三元组
	void print();		//输出
private:
	int m_datd[50][4];
	int m_count_data;
};

CNumber::CNumber() : m_count_data(0)
{
	for (int x = 0; x < 50; x++)
		for (int y = 0; y < 4; y++)
			m_datd[x][y] = 0;
}

int CNumber::isPrime(int x)
{
	if (x == 4)
		return 0;

	for (int i = 2; i < x / 2; i++)
	{
		if (x % i == 0)
			return 0;
	}
	return 1;
}

void CNumber::fund() //寻找符合条件的三元组遵循两个原则:一是三者不相等；二是x<y<z，以保证不重复
{
	for (int x = 2; x < 30; x++)
	{
		if (!isPrime(x))
			continue;
		for (int y = x; y < 30; y++)
		{
			if (!isPrime(y) || x == y)
				continue;
			for (int z = y; z < 30; z++)
			{
				if (!isPrime(z) || x == z || y == z)
					continue;
				if (isPrime(x + y + z))
				{
					m_datd[m_count_data][0] = x + y + z;
					m_datd[m_count_data][1] = x;
					m_datd[m_count_data][2] = y;
					m_datd[m_count_data][3] = z;
					m_count_data++;
				}
			}
		}
	}
}

void CNumber::print()
{
	for (int cnt = 0; cnt < m_count_data; cnt++)
	{
		cout << '(' << m_datd[cnt][1] << ',' << m_datd[cnt][2] << ',' << m_datd[cnt][3] << "):" << m_datd[cnt][0] << endl;
	}
}

int main()
{
	CNumber demo;
	demo.fund();
	demo.print();
	return 0;
}