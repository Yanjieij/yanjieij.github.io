#include<iostream>
#include<fstream>
#include"Graph.h"
using namespace std;
string MC1[201],MC2[201];
AGraph G;
int main()
{
	ifstream infile1,infile2;
	infile1.open("MC1.csv", ios::in);
	infile2.open("MC2.csv", ios::in);
	if (!infile1||!infile2)
	{
		cout << "文件打开失败！" << endl;
		return 0;
	}
	int lines1 = 0, lines2 = 0;
	while (!infile1.eof())
	{
		infile1 >> MC1[lines1++];
	}
	while (!infile2.eof())
	{
		infile2 >> MC2[lines2++];
	}
	G.build_by_matrix_csv(MC1, MC2, lines1, lines2);
	G.floyd();
	G.outputdis();
	cout << "--------------------------------------------------------------------" << endl;
	for (int i = 1; i <= lines1; ++i) G.dijkstra(i,0);
	G.outputdis();
	G.the_shortest_path_not_pass_one_city(1, 23, 12);
	G.BFS_from_WuHan();
	G.paths_less_than_k_nodes(1,23,5);
	G.the_kth_shortest_pth(1, 23, 5);
	return 0;
}