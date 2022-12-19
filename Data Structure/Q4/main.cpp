#include <io.h>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
using namespace std;
struct Word
{
	string  word = "";
	int id[20][10][10] = {0};//第几个文档 第几行 [0]表示这个单词在一行中 出现几次 后面就是每一次的位置
}w[10000];
int num;
/*
path: 指定目录
files: 保存结果
fileType: 指定的文件格式，如 .jpg
*/

// 使用字符分割
void Stringsplit(const string& str, const char split, Word w[],int Tid,int Rid)
{
	if (str == "")		return;
	int len = 0;
	//在字符串末尾也加入分隔符，方便截取最后一段
	string strs = str + split;
	size_t pos = strs.find(split);
	len += pos;
	// 若找不到内容则字符串搜索函数返回 npos
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
			w[num].id[Tid][Rid][w[num].id[Tid][Rid][0]] = len;//i文档第Rid行 该单词最后一个字母的后一个位置
			num++;
		}
		//去掉已分割的字符串,在剩下的字符串中进行分割
		strs = strs.substr(pos + 1, strs.size());
		pos = strs.find(split);
		len = len + pos + 1;
	}
}
void getAllFiles(string path, vector<string>& files, string fileType)//读路径名
{
	// 文件句柄
	long long hFile = 0;
	// 文件信息
	struct _finddata_t fileinfo;

	string p;

	if ((hFile = _findfirst(p.assign(path).append("\\*" + fileType).c_str(), &fileinfo)) != -1) {
		do {
			// 保存文件的全路径
			files.push_back(p.assign(path).append("\\").append(fileinfo.name));

		} while (_findnext(hFile, &fileinfo) == 0);  //寻找下一个，成功返回0，否则-1

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
							cout << "word:" << s << "出现的位置为："<<"文档："<<Tid<<"  行："<<row<<"  位置："
								<< w[i].id[Tid][row][j] - s.size() << endl;
					}
					
		}
	}

}
int main(int argc, char** argv)
{
	vector<string> temp;
	getAllFiles("D:\\tets01", temp, ".txt");
	for (int i = 0; i < temp.size(); ++i)//访问每一个文件读取
	{
		ifstream ifs;
		string file;
		ifs.open(temp[i], ios::in);
		if (!ifs)
		{
				cout << "文件打开失败" << endl;
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
	cout << "请输入你要查询的单词：" << endl;
	cin >> word;
	find(word);
	return 0;
}