#include<fstream>
#include"Graph.h"

using namespace std;

int main()
{
	string* input_matrix_dist = new string[40];
	string* input_matrix_adj = new string[40];

	ifstream file_1,file_2;
	file_1.open("input_dist.csv", ios::in);
	file_2.open("input_adj.csv", ios::in);

	if (!file_1||!file_2)
	{
		cout << "文件打开失败！" << endl;
		return 0;
	}

	int line_1 = 0, line_2 = 0;

	while (!file_1.eof())
	{
		file_1 >> input_matrix_dist[line_1++];
	}
	while (!file_2.eof())
	{
		file_2 >> input_matrix_adj[line_2++];
	}

	myGraph Graph;

	Graph.created_by_csv(input_matrix_dist, input_matrix_adj, line_1, line_2);

	Graph.BFS_from_WuHan();

	for (int i = 1; i <= line_1; ++i) 
		Graph.Dijkstra(i);
	Graph.printMinDist();

	Graph.Floyd();
	Graph.printMinDist();

	Graph.printCityList();
	cout << "请输入出发地，目的地，需要绕过的城市" << endl;
	int from, to, avoid = 0;
	cin >> from >> to >> avoid;
	Graph.find_path_with_avoid(from, to, avoid);

	int limit = 10;
	cout << "请输入出发地，目的地，节点限制" << endl;
	cin >> from >> to >> limit;
	Graph.find_path_with_limit(from, to, limit);

	int num = 1;
	cout << "请输入出发地，目的地，第几长度" << endl;
	cin >> from >> to >> num;
	Graph.find_Nth_shortest_path(from, to, num);

	delete[] input_matrix_dist, input_matrix_adj;
	return 0;
}