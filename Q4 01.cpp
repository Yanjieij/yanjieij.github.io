#include"CGame.h"

CGame::CGame(int difficulty) :m_cnt_flg(0), m_difficulty(0), m_judge_sign(0)
{
retry1:
	switch (difficulty)
	{
	case 1:m_size = 8, m_mine_ratio = 0.15; break;
	case 2:m_size = 16, m_mine_ratio = 0.18; break;
	case 3:m_size = 32, m_mine_ratio = 0.18; break;
	default: cout << "��������û��������ȷ���Ѷ�ϵ�������������룺" << endl;
	cin >> difficulty;
	goto retry1;
	}

	Crea_map();

	m_sum = static_cast<int>(m_size * m_size * m_mine_ratio);

	for (int y = 0; y < m_size; y++)
	{
		for (int x = 0; x < m_size; x++)
		{
			m_map[x][y] = 0;
			m_print_map[x][y] = '#';
		}
	}

	srand((unsigned)time(NULL));
	for (int i = 1, x, y; i <= m_sum; i++)      //���ѡ��λ�������õ���
	{
		x = rand() % (m_size);
		y = rand() % (m_size);

		if (m_map[x][y] == 9)                   //ȷ�����ظ�����
		{
			i--;
		}
		else
		{
			m_map[x][y] = 9;
		}
	}

	for (int y = 0; y < m_size; y++)
	{
		for (int x = 0; x < m_size; x++)
		{
			if (m_map[x][y] != 9)
				Cal_mine(x, y);                  //����ȫ�����ʼ��
		}
	}
}

CGame::~CGame()
{
	for (int i = 0; i < m_size; i++)            //�ͷ�����Ķ�̬�ڴ�
	{
		delete m_map[i];
		delete m_print_map[i];
	}
	delete[]m_map;
	delete[]m_print_map;
}

void CGame::Crea_map()                         //m_map�������ĵ�ͼ��m_print_map�ǳ��ָ���ҵĵ�ͼ
{
	m_map = new int* [m_size];
	for (int i = 0; i < m_size; i++)
		m_map[i] = new int[m_size];

	m_print_map = new char* [m_size];
	for (int i = 0; i < m_size; i++)
		m_print_map[i] = new char[m_size];
}

void CGame::Cal_mine(int x, int y)           //������Χ�˸�����ĵ�������
{
	int cnt = 0;
	for (int m = -1; m <= 1; m++)
	{
		for (int n = -1; n <= 1; n++)
		{
			if ((x + m) >= 0 && (y + n) >= 0 && (x + m) <= m_size-1 && (y + n) <= m_size-1 && m_map[x + m][y + n] == 9)
				cnt++;
		}
	}
	m_map[x][y] = cnt;
}

bool CGame::DoStep(char input)
{
	int x, y;

	retry2:
	if (input == 'q')
		return 0;
	if (input == 'm')
	{
		cin >> x >> y;
		if (x <= m_size && y <= m_size)
			FlgMine(x - 1, y - 1);
		else
		{
			cout << "���������������������룡" << endl;
			cin >> input;
			goto retry2;
		}
	}
	if (input == 'k')
	{
		cin >> x >> y;
		if (x <= m_size && y <= m_size)
			Open_around(x, y);
		else
		{
			cout << "���������������������룡" << endl;
			cin >> input;
			goto retry2;
		}
	}
		return 1;
}

void CGame::Open(int x, int y)
{
	m_print_map[x][y] = '%';
}

void CGame::Open_around(int x, int y)               //�����򿪵�ʵ��
{
	if (m_map[x - 1][y - 1] == 0)
	{
		for (int m = -1; m <= 1; m++)
		{
			for (int n = -1; n <= 1; n++)
			{
				if (x + m - 1 >= 0 && x + m - 1 <= m_size - 1 && y + n - 1 >= 0 && y + n - 1 <= m_size - 1 && m_print_map[x + m - 1][y + n - 1] != '%')
				{
					Open(x + m - 1, y + n - 1);

					if (m_map[x + m - 1][y + n - 1] == 0)
						Open_around(x + m, y + n);
				}
			}
		}
	}
	else 
	{
		Open(x - 1, y - 1);
	}
}

void CGame::FlgMine(int x, int y)           
{
	if (m_print_map[x][y] == '#')
	{
		m_print_map[x][y] = '*';
		m_cnt_flg++;
	}
	else if (m_print_map[x][y] == '*')      
	{
		m_print_map[x][y] = '#';
		m_cnt_flg--;
	}
	else if (m_print_map[x][y] == '%')
	{
		cout << "��������Ѿ������ˣ�" << endl;
	}
}

void CGame::Print()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);     //��ָ�������������������ɫ
	cout << setw(3) << " ";
	for (int i = 0; i < m_size; i++)
		cout << setw(3) << i + 1;
	cout << endl;

	for (int y = 0; y < m_size; y++)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		cout << setw(3) << y + 1;
		for (int x = 0; x < m_size; x++)
		{
			if (m_print_map[x][y] == '#')
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
				cout << setw(3) << "��";
			}

			if (m_print_map[x][y] == '*')
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
				cout << setw(3) << '*';
			}

			if (m_print_map[x][y] == '%')
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
				cout << setw(3) << m_map[x][y];
			}

		}
		cout << endl;
	}
}

bool CGame::Judge()
{
	int right_flg = 0, right_open = 0;
	for (int y = 0; y < m_size; y++)
	{
		for (int x = 0; x < m_size; x++)
		{
			if (m_map[x][y] == 9 && m_print_map[x][y] == '$')
				right_flg++;

			if (m_map[x][y] != 9 && m_print_map[x][y] == '%')
				right_open++;

			if (m_print_map[x][y] == '%' && m_map[x][y] == 9)
			{
				m_judge_sign = 1;
				return 1;
			}

			if (m_cnt_flg == m_sum)
			{
				m_judge_sign = 2;
				return 1;
			}

			if (right_open == m_size * m_size - m_sum)
			{
				m_judge_sign = 3;
				return 1;
			}
		}
	}
	return 0;
}

int main()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY |FOREGROUND_BLUE);
	cout << "��ӭ����ɨ����Ϸ���ڱ���Ϸ����������ȡʤ��ʽ��" << endl;
	cout << "1�������а�ȫ����ȫ���򿪣�" << endl;
	cout << "2�������еĵ���ȫ����ǡ�" << endl;
	
	int dif = 0;
	cout << "�������Ѷȣ�1��2��3������Խ���Ѷ�Խ�ߣ�" << endl;
	cin >> dif;
	CGame playing(dif);

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
	cout << "��Ϸ��ʼ��!" << endl;

	char input;
	while (1)
	{
		playing.Print();
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		cout << "�ܹ���" << playing.m_sum << "�ŵ��ף��㻹ʣ��" << playing.m_sum - playing.m_cnt_flg << "����־" << endl << endl;

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY |FOREGROUND_GREEN);
		cout << "������ָ�����q�����˳���Ϸ��k����򿪸�����m�����ע������" << endl;
		cout << "(��������������ᣬ���������ᣬ�ÿո����������ѱ�������ȡ�����)" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		
		cin >> input;
		if (!playing.DoStep(input))
			break;
		if (playing.Judge())
			break;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	cout << "GAME OVER��" << endl;

	switch (playing.m_judge_sign)
	{
	case 1:cout << "���ź������ȵ��˵��ף���Ϸʧ�ܡ�" << endl; 
		   break;
	case 2:cout << "��ϲ������������еĵ��ף�" << endl;
		   playing.Print(); break;
	case 3:cout << "��ϲ������������û�е��׵�����" << endl;
		   playing.Print(); break;
	default: ;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	return 0;
}