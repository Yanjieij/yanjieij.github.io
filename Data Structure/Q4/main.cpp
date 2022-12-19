#include <io.h>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
using namespace std;
struct Word
{
	string  word = "";
	int id[20][10][10] = {0};//�ڼ����ĵ� �ڼ��� [0]��ʾ���������һ���� ���ּ��� �������ÿһ�ε�λ��
}w[10000];
int num;
/*
path: ָ��Ŀ¼
files: ������
fileType: ָ�����ļ���ʽ���� .jpg
*/

// ʹ���ַ��ָ�
void Stringsplit(const string& str, const char split, Word w[],int Tid,int Rid)
{
	if (str == "")		return;
	int len = 0;
	//���ַ���ĩβҲ����ָ����������ȡ���һ��
	string strs = str + split;
	size_t pos = strs.find(split);
	len += pos;
	// ���Ҳ����������ַ��������������� npos
	while (pos != strs.npos)
	{
		
		bool flag = 0;
		Word temp;
		string t= strs.substr(0, pos);
		for (int i = 0; i < num; i++)
		{
			if (t == w[i].word)
			{
				w[i].id[Tid][Rid][0]++;
				w[i].id[Tid][Rid][w[i].id[Tid][Rid][0]] = len;
				flag = 1;
				break;
			}
		}
		if (!flag)
		{
			w[num].word = t;
			w[num].id[Tid][Rid][0]++;
			w[num].id[Tid][Rid][w[num].id[Tid][Rid][0]] = len;//i�ĵ���Rid�� �õ������һ����ĸ�ĺ�һ��λ��
			num++;
		}
		//ȥ���ѷָ���ַ���,��ʣ�µ��ַ����н��зָ�
		strs = strs.substr(pos + 1, strs.size());
		pos = strs.find(split);
		len = len + pos + 1;
	}
}
void getAllFiles(string path, vector<string>& files, string fileType)//��·����
{
	// �ļ����
	long long hFile = 0;
	// �ļ���Ϣ
	struct _finddata_t fileinfo;

	string p;

	if ((hFile = _findfirst(p.assign(path).append("\\*" + fileType).c_str(), &fileinfo)) != -1) {
		do {
			// �����ļ���ȫ·��
			files.push_back(p.assign(path).append("\\").append(fileinfo.name));

		} while (_findnext(hFile, &fileinfo) == 0);  //Ѱ����һ�����ɹ�����0������-1

		_findclose(hFile);
	}
}
void find(string s)
{
	for (int i = 0; i < num; i++)
	{
		if (w[i].word == s)
		{
			for(int Tid=0;Tid<10;Tid++)
				for(int row=0;row<10;row++)
					for (int j = 1; j <= w[i].id[Tid][row][0]; j++)
					{
							cout << "word:" << s << "���ֵ�λ��Ϊ��"<<"�ĵ���"<<Tid<<"  �У�"<<row<<"  λ�ã�"
								<< w[i].id[Tid][row][j] - s.size() << endl;
					}
					
		}
	}

}
int main(int argc, char** argv)
{
	vector<string> temp;
	getAllFiles("D:\\tets01", temp, ".txt");
	for (int i = 0; i < temp.size(); ++i)//����ÿһ���ļ���ȡ
	{
		ifstream ifs;
		string file;
		ifs.open(temp[i], ios::in);
		if (!ifs)
		{
				cout << "�ļ���ʧ��" << endl;
				exit(0);
			
		}
		int len = 0;
		while (getline(ifs,file))
		{
			Stringsplit(file, ' ', w, i, len);
			len++;
		}
		ifs.close();
	}
	string word;
	cout << "��������Ҫ��ѯ�ĵ��ʣ�" << endl;
	cin >> word;
	find(word);
	return 0;
}