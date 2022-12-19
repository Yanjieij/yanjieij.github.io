#pragma once
#include<iostream>
#include<cstring>
#include<iomanip>
#include<queue>
#include<stack>
#define MAX_ 1000000007
using namespace std;
int vis[202],cpt;

struct Edge 
{
	int to, val, nxt;
	Edge()
	{
		to = val = nxt = 0;
	}
};
struct path
{
	int pre[202];
	int size;
	int dis;
	bool operator < (const path& a) const
	{
		return dis < a.dis;
	}
	bool operator > (const path& a) const
	{
		return dis > a.dis;
	}

}pth[20005];
void quick_sort_path(int l,int r)
{
	int mid = (l + r) >> 1;
	int i = 1; int j = r;
	do {
	//	cout << "i=" << i << ' ' << "j=" << j << endl;
		while (pth[i] < pth[mid]) ++i;
		while (pth[j] > pth[mid]) --j;
		if (i <= j)
		{
			swap(pth[i], pth[j]);
			++i;
			--j;
		}
	} while (i <= j);
	if (l < j) quick_sort_path(l, j);
	if (i < r) quick_sort_path(i, r);
}
struct Node 
{
	int id, dis, head;
	string name;
	Node()
	{
		id = -1;
		head = 0;
		dis = 0;
		name = "";
	}
	bool operator < (const Node& aa) const 
	{
		return aa.dis < dis;
	}
	Node(int a, int b)
	{
		id = a;
		dis = b;
		head = 0;
		name = "";
	}
};
struct status 
{
	int node, dis, priority;
	status():node(0), dis(0), priority(0){}
	status(int node, int dis,int dissum) :node(node), dis(dis), priority(dissum){}
	bool operator < (status b) const
	{
		return priority > b.priority;
	}
};
class AGraph
{
private:
	int MAXSIZEN = 1005;
	int MAXSIZEE = 50005;
	int sizeE, sizeN;
	bool edge_matrix[201][201] = { 0 };
	int dis_matrix[201][201] = { 0 };
	int shortestdis[205][205] = { 0 };
	void addedge(int x, int y, int v)
	{
		e[++sizeE].to = y;
		e[sizeE].val = v;
		e[sizeE].nxt = n[x].head;
		n[x].head = sizeE;
	}
public:
	Node* n;
	Edge* e;
	AGraph()
	{
		sizeE = sizeN = 0;
		n = new Node[MAXSIZEN]();
		e = new Edge[MAXSIZEE]();
	}
	~AGraph()
	{
		delete[] n;
		delete[] e;
	}
	void build_by_matrix_csv(string MC1[], string MC2[], int lines1, int lines2);
	void outputdis();
	void dijkstra(int s,int t);
	void floyd();
	void BFS_from_WuHan();
	int the_shortest_path_not_pass_one_city(int a, int b, int t);
	void paths_less_than_k_nodes(int a,int b,int k);
	void DFS_for_path(stack<int>& st, int now,int a,int b, int k, int dis);
	void the_kth_shortest_pth(int a, int b, int k);
};
int AGraph::the_shortest_path_not_pass_one_city(int a,int b,int t)
{
	dijkstra(a,t);
	cout << "从" << n[a].name << "到" << n[b].name <<
		"不经过"<<n[t].name<<"的最短路长度为:" << endl;
	cout << shortestdis[a][b] << endl;
	return shortestdis[a][b];
}
void AGraph::build_by_matrix_csv(string MC1[], string MC2[], int lines1, int lines2)
{
	string namee;
	for (int i = 1; i < MC1[0].size(); ++i)
	{
		if (MC1[0][i] == ',')
		{
			n[++sizeN].id = sizeN;
			n[sizeN].name = namee;
			namee.clear();
			continue;
		}
		namee.push_back(MC1[0][i]);
		if (i == MC1[0].size() - 1)
		{
			n[++sizeN].id = sizeN;
			n[sizeN].name = namee;
			namee.clear();
			break;
		}
	}
	int tdis = 0;
	int num = 0, tag = 0;
	for (int i = 1; i <= lines1; ++i)
	{
		num = 0;
		for (int j = 0; j < MC2[i].size(); ++j)
		{
			if (MC2[i][j] == ',' && j != MC2[i].size() - 1)
			{
				++num;
				if (num == i) continue;
				edge_matrix[i][num] =
					edge_matrix[num][i] = (int)(MC2[i][j + 1] - '0');
			}
		}
	}
	for (int i = 1; i <= sizeN; ++i)
	{
		for (int j = 1; j <= sizeN; ++j)
		{
			dis_matrix[i][j] = -1;
		}
	}
//	cout << "lines1=" << lines1 << endl << "lines2=" << lines2 << endl;
	for (int i = 1; i <= lines1; ++i)
	{
		num = 0;
		for (int j = 0; j < MC1[i].size(); ++j)
		{
			tdis = 0;
			if (MC1[i][j] == ',')
			{
				if (j == MC1[i].size() - 1) break;
				++num;
				if (i == num)
				{
					dis_matrix[i][num] = 0;
					continue;
				}
				tag = 0;
				while (j + 1 < MC1[i].size()
					&& MC1[i][j + 1] >= '0'
					&& MC1[i][j + 1] <= '9')
				{
					tag = 1;
					if (tdis != 0) tdis *= 10;
					tdis += (int)(MC1[i][j + 1] - '0');
					++j;
				}
				if (!tag) continue;
				if (edge_matrix[i][num] || edge_matrix[num][i])
				{
					//	cout << "i=" << n[i].name << ' ' << "num=" << n[num].name << ' ' << "tdis=" << tdis << endl;
					dis_matrix[i][num] = dis_matrix[num][i] = tdis;
					addedge(i, num, tdis);
					addedge(num, i, tdis);
				}
			}
		}
	}
	/*
	for (int i = 1; i <= sizeN; ++i)
	{
		for (int j = 1; j <= sizeN; ++j)
		{
			cout <<setw(5) << edge_matrix[i][j];
		}
		cout << endl;
	}
	cout << "====================================================" << endl;
	for (int i = 1; i <= sizeN; ++i)
	{
		for (int j = 1; j <= sizeN; ++j)
		{
			cout << setw(5) << dis_matrix[i][j];
		}
		cout << endl;
	}*/
}
void AGraph::outputdis()
{
	cout << setw(6) << "N";
	for (int i = 1; i <= sizeN; ++i)
	{
		if (n[i].name.size() > 4) cout << n[i].name;
		else cout << setw(6) << n[i].name;
	}
	cout << endl;
	for (int i = 1; i <= sizeN; ++i)
	{
		if (n[i].name.size() > 4)
		{
			cout << n[i].name;
		}
		else cout << setw(6) << n[i].name;
		for (int j = 1; j <= sizeN; ++j)
		{
			if (shortestdis[i][j] == MAX_) cout << setw(6) << "INF";
			else cout << setw(6)<< shortestdis[i][j];
		}
		cout << endl;
	}
}
void AGraph::dijkstra(int s,int t)   
{
	memset(vis, 0, sizeof(vis));
	vis[t] = 1;
	priority_queue<Node> q;
	q.push(Node(s, n[s].dis));
	for (int i = 1; i <= sizeN; ++i) shortestdis[s][i] = MAX_;
	shortestdis[s][s] = 0;
	while (!q.empty())
	{
		Node now = q.top();
		q.pop();
		if (vis[now.id]) continue;
		vis[now.id] = 1;
		for (int i = n[now.id].head; i; i = e[i].nxt)
		{
			int to = e[i].to;
			if (shortestdis[s][to] > shortestdis[s][now.id] + e[i].val)
			{
				shortestdis[s][to] = shortestdis[s][now.id] + e[i].val;
				if (!vis[to]) q.push(Node(to, shortestdis[s][to]));
			}
		}
	}
}
void AGraph::floyd()
{
	for (int i = 1; i <= sizeN; ++i)
	{
		for (int j = 1; j <= sizeN; ++j)
		{
			if (dis_matrix[i][j] != -1) shortestdis[i][j] = dis_matrix[i][j];
			else shortestdis[i][j] = MAX_;
		}
	}
	/*
	cout << "====================================================" << endl;
	for (int i = 1; i <= sizeN; ++i)
	{
		for (int j = 1; j <= sizeN; ++j)
		{
			if (shortestdis[i][j] == MAX_) cout << setw(5) << "0";
			else cout << setw(5) << shortestdis[i][j];
		}
		cout << endl;
	}*/
	for (int k = 1; k <= sizeN; ++k)
	{
		for (int i = 1; i <= sizeN; ++i)
		{
			for (int j = 1; j <= sizeN; ++j)
			{
				if (shortestdis[i][j] >
					shortestdis[i][k] + shortestdis[k][j])
				{
					shortestdis[i][j] =
						shortestdis[i][k] + shortestdis[k][j];

				}
			}
		}
	}
}
void AGraph::BFS_from_WuHan()
{
	queue<int> q;
	int vis[101] = { 0 };
	q.push(23);
	vis[23] = 1;
	int cnt = 0;
	while (!q.empty())
	{
		int nowid = q.front();
		q.pop();
		for (int i = n[nowid].head; i; i = e[i].nxt)
		{
			int to = e[i].to;
			if (vis[to]) continue;
			vis[to] = vis[nowid] + 1;
			if(vis[to]<=2) q.push(to);
		}
	}
	Node p[101];
	for (int i = 1; i <= sizeN; ++i)
	{
		if (i == 27 || i == 29 || i == 33 || i == 34) continue;
		if (!vis[i]) 
		{
			++cnt;
			p[cnt] = n[i];
		}
	}
	cout << "除港澳台海之外不能到达" << cnt << "个城市"<<endl<<"分别是:";
	for (int i = 1; i <= cnt; ++i) cout << p[i].name << "  ";
	cout << endl;
}
void AGraph::DFS_for_path(stack<int>& st, int now, int a, int b, int k, int dis)
{
	if (now == b)
	{
		stack<int> p;
		pth[++cpt].dis = dis;
		pth[cpt].size = (int)st.size();
	//	cout << cpt << endl;
		if (cpt == 1038)
		{
			cout << "asdas" << endl;
		}
		while (!st.empty())
		{
			p.push(st.top());
			st.pop();
		}
		for (int i = 1; i <= pth[cpt].size; ++i)
		{
			pth[cpt].pre[i] = p.top();
			st.push(p.top());
			p.pop();
		}
		return;
	}
	for (int i = n[now].head; i; i = e[i].nxt)
	{
		int to = e[i].to;
		if (vis[i]) continue;
		vis[i] =  1;
		//cout << st.size() << endl;
		if (st.size()<k)
		{
			st.push(to);
			DFS_for_path(st,to,a, b, k, dis + e[i].val);
			st.pop();
		}
		vis[i] = 0;
	}
}	
bool cmp(path& a, path& b)
{
	return a.dis < b.dis;
}
void AGraph::the_kth_shortest_pth(int a,int b,int k)
{
	dijkstra(b, 0);
	memset(vis, 0, sizeof(vis));
	priority_queue<status> q;
	if (shortestdis[b][a] == MAX_) 
	{
		cout << "两座城市不连通！" << endl;
		return ;
	}
	q.push(status(a, 0, shortestdis[b][a]));
	while (!q.empty())
	{
		status tmp = q.top();
		q.pop();
		vis[tmp.node]++;
		/*cout << "node=" << n[tmp.node].name << ' '
			<< "dis=" << tmp.dis << ' ' << vis[tmp.node] << endl;*/
		if (tmp.node == b && vis[tmp.node] == k)
		{
			cout << n[a].name << "到" << n[b].name <<
				"的第" << k << "短路长度为:" << tmp.dis << endl;
			return;
		}
		for (int i = n[tmp.node].head; i; i = e[i].nxt)
		{
			int to = e[i].to;
			if (vis[to] < k)
				q.push(status(to, tmp.dis + e[i].val, 
					shortestdis[b][to] + tmp.dis + e[i].val));
		}
	}
	return ;
}
void AGraph::paths_less_than_k_nodes(int a,int b,int k)
{
	ofstream outf("paths.txt");
	if (!outf)
	{
		cout << "打开文件失败!" << endl;
		return;
	}
 	cpt = 0;
	memset(pth, 0, sizeof(pth));
	memset(vis, 0, sizeof(vis));
	stack<int> st;
	st.push(a);
	vis[a] = 1;
	DFS_for_path(st, a, a, b, k+1, 0);
	quick_sort_path(1, cpt);
	//sort(pth + 1, pth + 1 + cpt, cmp);
	outf << "途径少于" << k << "个城市到达武汉有" << cpt << "种方案" << endl;
	outf << "分别是" << endl;
	for (int i = 1; i <= cpt; ++i)
	{
		for (int j = 1; j <= pth[i].size; ++j)
		{
			outf << n[pth[i].pre[j]].name;
			if (j != pth[i].size) outf << '-';
		}
		outf << "总长度:" << pth[i].dis << endl;
	}
}
/*
27 29 33 34
*/
