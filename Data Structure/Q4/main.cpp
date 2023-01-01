#include <io.h>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include "cppjieba/Jieba.hpp"

using namespace std;

const char* const DICT_PATH = "dict/jieba.dict.utf8";
const char* const HMM_PATH = "dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "dict/user.dict.utf8";
const char* const IDF_PATH = "dict/idf.utf8";
const char* const STOP_WORD_PATH = "dict/stop_words.utf8";

struct Word
{
	string word = "";
	int id[3][20][100] = {0};	//第几个文档 第几行 [0]表示这个单词在一行中 出现几次 后面就是每一次的位置
}word_list[10000];
int word_num;


/*
	path: 指定目录
	files: 保存结果
	fileType: 指定的文件格式，如 .jpg
*/

// 使用字符分割
void refresh_word_list(const vector<string>& str_list, Word wlist[], int Tid, int Rid)
{
	if (str_list.empty())
		return;

	int len = 0;

	for (int cnt = 0; cnt < str_list.size(); cnt++)
	{
		bool flag = 0;
		string t = str_list[cnt];
		for (int i = 0; i < word_num; i++)
		{
			if (t == wlist[i].word)
			{
				wlist[i].id[Tid][Rid][0]++;
				wlist[i].id[Tid][Rid][wlist[i].id[Tid][Rid][0]] = len;
				flag = 1;
				break;
			}
		}
		if (!flag)
		{
			wlist[word_num].word = t;
			wlist[word_num].id[Tid][Rid][0]++;
			wlist[word_num].id[Tid][Rid][wlist[word_num].id[Tid][Rid][0]] = len;	//i文档第Rid行 该单词最后一个字母的后一个位置
			word_num++;
		}
		len++;
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
		do 
		{	// 保存文件的全路径
			files.push_back(p.assign(path).append("\\").append(fileinfo.name));
		} while (_findnext(hFile, &fileinfo) == 0); 
		_findclose(hFile);
	}
}

int main(int argc, char** argv)
{
	cppjieba::Jieba jieba
		(DICT_PATH,
		HMM_PATH,
		USER_DICT_PATH,
		IDF_PATH,
		STOP_WORD_PATH);


	vector<string> files;

	getAllFiles("D:\\CODE\\Data Structure\\Q4", files, ".txt");

	for (int i = 0; i < files.size(); ++i) //访问每一个文件读取
	{
		ifstream ifs;
		string str;

		ifs.open(files[i], ios::in);
		if (!ifs)
		{
			cout << "文件打开失败" << endl;
			exit(0);
		}

		int len = 0;
		vector<string> words;
		while (getline(ifs,str))
		{
			jieba.Cut(str, words, false);
			refresh_word_list(words, word_list, i, len);
			words.clear();
			len++;
		}
		ifs.close();
	}

	string aim_word, str;
	cout << "请输入你要查询的单词：" << endl;
	cin >> aim_word;

	cout << "word:" << aim_word << "出现的位置为：" << endl;
	for (int i = 0; i < word_num; i++)
	{
		if (word_list[i].word == aim_word)
		{
			for (int Tid = 0; Tid < 2; Tid++)
			{
				for (int row = 0; row < 20; row++)
				{
					for (int j = 1; j <= word_list[i].id[Tid][row][0]; j++)
					{
						cout << "文档：" << Tid + 1 << "  行：" << row + 1 << "  位置：" << word_list[i].id[Tid][row][j] + 1 << endl;
						ifstream ifs;
						ifs.open(files[Tid], ios::in);
						for (int cnt = 0; cnt < row + 1; cnt++)
						{
							str.clear();
							getline(ifs, str);
						}
						cout << '\t' << str << endl;
						ifs.close();
					}
				}
			}
		}
	}

	return 0;
}