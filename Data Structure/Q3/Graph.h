#pragma once
#include<iostream>
#include<cstring>
#include<iomanip>
#include<queue>
#include<stack>

#define NaN 10000
#define MAX_NODE_NUM 40

using namespace std;

int visit[MAX_NODE_NUM * MAX_NODE_NUM] = { 0 };
int cnt;
int status_dest;
int min_dist[MAX_NODE_NUM][MAX_NODE_NUM] = { 0 };

struct Edge 
{
	int to, dist, next;
};

struct Node
{
	int id, dist, headNode_id;
	string name;

	Node()
	{
		id = -1;
		headNode_id = 0;
		dist = 0;
		name = "";
	}
	Node(int _id, int _dist)
	{
		id = _id;
		dist = _dist;
		headNode_id = 0;
		name = "";
	}

	bool operator < (const Node& aa) const
	{
		return aa.dist < dist;
	}
};

struct path
{
	int pre[MAX_NODE_NUM];
	int size;
	int dist;
	bool operator < (const path& a) const
	{
		return dist < a.dist;
	}
	bool operator > (const path& a) const
	{
		return dist > a.dist;
	}

}path_dfs[MAX_NODE_NUM * MAX_NODE_NUM * 40];

struct mid_Node
{
	int node, dist;
	mid_Node() :node(0), dist(0) { ; }
	mid_Node(int node, int dis) :node(node), dist(dis) { ; }
	bool operator < (mid_Node b) const
	{
		return dist + min_dist[status_dest][node] > b.dist + min_dist[status_dest][b.node];
	}
};

class myGraph
{
private:
	int cnt_edge, cnt_node;
	Node* node_arr;
	Edge* edge_arr;
	bool edge_matrix[MAX_NODE_NUM][MAX_NODE_NUM] = { 0 };
	int dist_matrix[MAX_NODE_NUM][MAX_NODE_NUM] = { 0 };
public:
	myGraph()
	{
		cnt_edge = cnt_node = 0;
		node_arr = new Node[MAX_NODE_NUM]();
		edge_arr = new Edge[MAX_NODE_NUM * MAX_NODE_NUM]();
	}
	~myGraph()
	{
		delete[] node_arr;
		delete[] edge_arr;
	}
	void addEdge(int from, int to, int dist);
	void printCityList();
	void created_by_csv(string matrix_1[], string matrix_2[], int lines1, int lines2);
	void printMinDist();
	void Dijkstra(int from,int avoid);
	void Floyd();
	void BFS_from_WuHan();
	int find_path_with_avoid(int from, int to, int avoid);
	void find_path_with_limit(int a,int b,int k);
	void DFS(stack<int>& st, int now,int b, int limit, int dis);
	void find_Nth_shortest_path(int a, int b, int k);
};

void myGraph::addEdge(int from, int to, int dist)
{
	edge_arr[++cnt_edge].to = to;
	edge_arr[cnt_edge].dist = dist;
	edge_arr[cnt_edge].next = node_arr[from].headNode_id;
	node_arr[from].headNode_id = cnt_edge;
}

void myGraph::printCityList()
{
	cout << "城市列表为：" << endl;
	for (int i = 1; i < cnt_node; i++)
		if (node_arr[i].name.size() > 6)
			cout << node_arr[i].name << "\t" << node_arr[i].id << endl;
		else
			cout << node_arr[i].name << "\t\t" << node_arr[i].id << endl;
}

void myGraph::created_by_csv(string matrix_1[], string matrix_2[], int line_1, int line_2)
{
	//读取第一行
	string _name;
	for (int i = 1; i < matrix_1[0].size(); ++i)
	{
		if (matrix_1[0][i] == ',')
		{
			node_arr[++cnt_node].id = cnt_node;
			node_arr[cnt_node].name = _name;
			_name.clear();
			continue;
		}
		_name.push_back(matrix_1[0][i]);
		if (i == matrix_1[0].size() - 1)
		{
			node_arr[++cnt_node].id = cnt_node;
			node_arr[cnt_node].name = _name;
			_name.clear();
			break;
		}
	}

	int num = 0;

	//读取邻接表格
	for (int i = 1; i <= line_1; ++i)
	{
		num = 0;
		for (int j = 0; j < matrix_2[i].size(); ++j)
		{
			if (matrix_2[i][j] == ',' && j != matrix_2[i].size() - 1)
			{
				++num;
				if (num == i) continue;
				edge_matrix[i][num] = (int)(matrix_2[i][j + 1] - '0');
				edge_matrix[num][i] = (int)(matrix_2[i][j + 1] - '0');
			}
		}
	}

	//初始化距离矩阵
	for (int i = 1; i <= cnt_node; ++i)
	{
		for (int j = 1; j <= cnt_node; ++j)
		{
			dist_matrix[i][j] = -1;
		}
	}

	int flag = 0;
	int _dist = 0;

	//读取距离表格
	for (int i = 1; i <= line_1; ++i)
	{
		num = 0;
		for (int j = 0; j < matrix_1[i].size(); ++j)
		{
			_dist = 0;
			if (matrix_1[i][j] == ',')
			{
				if (j == matrix_1[i].size() - 1) 
					break;
				++num;
				if (i == num)
				{
					dist_matrix[i][num] = 0;
					continue;
				}
				flag = 0;
				//读取距离
				while (j + 1 < matrix_1[i].size() && matrix_1[i][j + 1] >= '0' && matrix_1[i][j + 1] <= '9')
				{
					flag = 1;
					if (_dist != 0) _dist *= 10;
					_dist += (int)(matrix_1[i][j + 1] - '0');
					++j;
				}
				if (!flag) continue;
				if (edge_matrix[i][num] || edge_matrix[num][i])
				{
					dist_matrix[i][num] = dist_matrix[num][i] = _dist;
					addEdge(i, num, _dist);
					addEdge(num, i, _dist);
				}
			}
		}
	}
}

void myGraph::printMinDist()
{
	cout << setw(6) << "N";
	for (int i = 1; i <= cnt_node; ++i)
	{
		if (node_arr[i].name.size() > 4) cout << node_arr[i].name;
		else cout << setw(6) << node_arr[i].name;
	}
	cout << endl;
	for (int i = 1; i <= cnt_node; ++i)
	{
		if (node_arr[i].name.size() > 4)
		{
			cout << node_arr[i].name;
		}
		else cout << setw(6) << node_arr[i].name;
		for (int j = 1; j <= cnt_node; ++j)
		{
			if (min_dist[i][j] == NaN) cout << setw(6) << "INF";
			else cout << setw(6)<< min_dist[i][j];
		}
		cout << endl;
	}
}

void myGraph::Dijkstra(int start, int node_to_avoid = 0)
{
	for (int i = 0; i < MAX_NODE_NUM; i++)
		visit[i] = 0;

	visit[node_to_avoid] = 1;

	priority_queue<Node> que;
	que.push(Node(start, node_arr[start].dist));

	for (int i = 1; i <= cnt_node; ++i) 
		min_dist[start][i] = NaN;
	min_dist[start][start] = 0;

	while (!que.empty())
	{
		Node current = que.top();
		que.pop();
		if (visit[current.id]) 
			continue;
		visit[current.id] = 1;
		for (int i = node_arr[current.id].headNode_id; i; i = edge_arr[i].next)
		{
			int to = edge_arr[i].to;
			if (min_dist[start][to] > min_dist[start][current.id] + edge_arr[i].dist)
			{
				min_dist[start][to] = min_dist[start][current.id] + edge_arr[i].dist;
				if (!visit[to]) 
					que.push(Node(to, min_dist[start][to]));
			}
		}
	}
}

void myGraph::Floyd()
{
	for (int i = 1; i <= cnt_node; ++i)
	{
		for (int j = 1; j <= cnt_node; ++j)
		{
			if (dist_matrix[i][j] != -1) 
				min_dist[i][j] = dist_matrix[i][j];
			else 
				min_dist[i][j] = NaN;
		}
	}
	//取cnt为中间节点
	for (int cnt = 1; cnt <= cnt_node; ++cnt)
	{
		for (int i = 1; i <= cnt_node; ++i)
		{
			for (int j = 1; j <= cnt_node; ++j)
			{
				if (min_dist[i][j] > min_dist[i][cnt] + min_dist[cnt][j])
				{
					min_dist[i][j] = min_dist[i][cnt] + min_dist[cnt][j];
				}
			}
		}
	}
}

int myGraph::find_path_with_avoid(int from, int to, int avoid)
{
	Dijkstra(from, avoid);
	cout << "从" << node_arr[from].name << "到" << node_arr[to].name << "不经过" << node_arr[avoid].name << "的最短路长度为:" << endl;
	cout << min_dist[from][to] << endl;
	return min_dist[from][to];
}

void myGraph::BFS_from_WuHan()
{
	queue<int> q;
	int dist[MAX_NODE_NUM] = { 0 };

	q.push(23);
	dist[23] = 1;

	int cnt = 0;
	while (!q.empty())
	{
		int current = q.front();
		q.pop();
		for (int i = node_arr[current].headNode_id; i; i = edge_arr[i].next)
		{
			int to = edge_arr[i].to;
			if (dist[to])
			{
				if (dist[to] > dist[current] + 1)
					dist[to] = dist[current] + 1;
				continue;
			}
			dist[to] = dist[current] + 1;
			q.push(to);
		}
	}
	Node p[MAX_NODE_NUM];
	for (int i = 1; i <= cnt_node; ++i)
	{
		if (i == 27 || i == 29 || i == 33 || i == 34) 
			continue;
		if (dist[i] > 4)
		{
			++cnt;
			p[cnt] = node_arr[i];
		}
	}
	cout << "除港澳台海之外不能到达以外，其余不能到达的有" << cnt << "个城市" << endl;
	cout << endl;
}

void myGraph::DFS(stack<int>& s, int current, int dest, int limit, int dis)
{
	if (current == dest)
	{
		stack<int> p;
		path_dfs[++cnt].dist = dis;
		path_dfs[cnt].size = (int)s.size();

		while (!s.empty())
		{
			p.push(s.top());
			s.pop();
		}
		for (int i = 1; i <= path_dfs[cnt].size; ++i)
		{
			path_dfs[cnt].pre[i] = p.top();
			s.push(p.top());
			p.pop();
		}
		return;
	}

	for (int i = node_arr[current].headNode_id; i; i = edge_arr[i].next)
	{
		if (visit[i])
			continue;
		visit[i] = 1;
		int to = edge_arr[i].to;
		if (s.size() < limit)
		{
			s.push(to);
			DFS(s, to, dest, limit, dis + edge_arr[i].dist);
			s.pop();
		}
		visit[i] = 0;
	}
}

void qsort_path(int l, int r)
{
	int mid = (l + r) >> 1;
	int i = 1; int j = r;
	do {
		while (path_dfs[i] < path_dfs[mid])
			++i;
		while (path_dfs[j] > path_dfs[mid])
			--j;
		if (i <= j)
		{
			swap(path_dfs[i], path_dfs[j]);
			++i;
			--j;
		}
	} while (i <= j);
	if (l < j) qsort_path(l, j);
	if (i < r) qsort_path(i, r);
}


void myGraph::find_path_with_limit(int from, int to, int limit)
{
	ofstream writeFile("_paths.txt");
	if (!writeFile)
	{
		cout << "打开文件失败!" << endl;
		return;
	}

 	cnt = 0;
	memset(path_dfs, 0, sizeof(path_dfs));
	memset(visit, 0, sizeof(visit));

	stack<int> Stack;
	Stack.push(from);
	visit[from] = 1;

	DFS(Stack, from, to, limit, 0);

	qsort_path(1, cnt);

	writeFile << "从" << node_arr[from].name << "途径少于" << limit << "个城市到达" << node_arr[to].name << "有" << cnt << "种方案" << endl;
	writeFile << "分别是" << endl;
	for (int i = 1; i <= cnt; ++i)
	{
		for (int j = 1; j <= path_dfs[i].size; ++j)
		{
			writeFile << node_arr[path_dfs[i].pre[j]].name;
			if (j != path_dfs[i].size) 
				writeFile << '-';
		}
		writeFile << "总长度:" << path_dfs[i].dist << endl;
	}
}

void myGraph::find_Nth_shortest_path(int from, int to, int N)
{
	//Dijkstra(to);

	memset(visit, 0, sizeof(visit));

	priority_queue<mid_Node> q;

	if (min_dist[to][from] == NaN)
	{
		cout << "两座城市不连通" << endl;
		return;
	}

	status_dest = to;
	q.push(mid_Node(from, 0));

	while (!q.empty())
	{
		mid_Node temp = q.top();
		q.pop();
		visit[temp.node]++;
		if (temp.node == to && visit[temp.node] == N)
		{
			cout << node_arr[from].name << "到" << node_arr[to].name << "的第" << N << "短路长度为:" << temp.dist << endl;
			return;
		}
		for (int i = node_arr[temp.node].headNode_id; i; i = edge_arr[i].next)
		{
			int to = edge_arr[i].to;
			if (visit[to] < N)
				q.push(mid_Node(to, temp.dist + edge_arr[i].dist));
		}
	}
	return;
}
