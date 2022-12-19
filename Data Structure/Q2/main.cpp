#include<iostream>
#include<vector>

using namespace std;

struct Node								
{
	int cityID = -1;
	Node* lcity = nullptr, * rcity = nullptr;
	int ldist = -1 , rdist = -1;
};

//寻找start子树中除了partner以外的叶节点，存入results
bool searchOtherLeaves(Node* start, int partner, vector<int>* results)
{
	if (!start->lcity && !start->rcity)
	{
		if (start->cityID != partner)
		{
			(*results).push_back(start->cityID);
			return 1;
		}
		else
			return 0;
	}
	if (start->lcity)
		searchOtherLeaves(start->lcity, partner, results);
	if (start->rcity)
		searchOtherLeaves(start->rcity, partner, results);
	return 1;
}

//连接两城市，存储距离
bool linkCity(Node* from, Node* to, int dist)
{
	if (from->lcity == nullptr)
	{
		from->lcity = to;
		from->ldist = dist;
	}
	else
	{
		if (from->rcity != nullptr)
			return 0;
		from->rcity = to;
		from->rdist = dist;
	}
	return 1;
}

int main()
{
	int city_num = 0;
	cin >> city_num;

	Node** city_list = new Node *[city_num] {nullptr};	//指向各个节点的指针
	int* route_set = new int[city_num] {-1};			//用并查集储存节点之间的关系
	int* length_list = new int[city_num] {-1};			//存储每个城市到上一城市的距离
	
	//建立树
	for (int i = 0; i < city_num - 1; i++)
	{
		int from, to, dist = 0;
		cin >> from >> to >> dist;
		if (!city_list[from])
		{
			city_list[from] = new Node;
			city_list[from]->cityID = from;
		}
		if (!city_list[to])
		{
			city_list[to] = new Node;
			city_list[to]->cityID = to;
		}
		route_set[to] = from;
		length_list[to] = dist;
		linkCity(city_list[from], city_list[to],dist);
	}

	//找到所有边境城市
	vector<int> border;
	for (int _cnt = 0; _cnt < city_num; _cnt++)
	{
		if (!city_list[_cnt]->lcity && !city_list[_cnt]->rcity)
			border.push_back(_cnt);
	}
	
	//只有军队不够才会导致无法控制疫情
	int army_num;
	cin >> army_num;
	if (army_num < 2 && city_num != 1)
		return -1;

	int* army_list = new int[city_num] {0};
	for (int i = 0; i < army_num; i++)
	{
		int id;
		cin >> id;
		army_list[id]++;
	}

	//每个边境城市到首都路线上的军队数量
	int* army_route = new int[city_num] {-1};

	//统计后，找到路线上没有军队的边境城市
	vector<int> border_to_arm, border_matched;
	for (int i = 0; i < border.size(); i++)
	{
		int _id = border[i];
		int current = _id;
		while (current != -1)
		{
			army_route[_id] += army_list[current];
			current = route_set[current];
		}
		if (!army_route[_id])
			border_to_arm.push_back(_id);
	}

	//对每个需要军队的边境城市进行军队调整
	int time_need = 0;
	for (int i = 0; i < border_to_arm.size(); i++)
	{
		int _id = border_to_arm[i];
		int current = _id;

		//对当前边境城市，寻找最近的有军队的路线，其对应子树的根为current
		while (current != -1)
		{
			current = route_set[current];
			searchOtherLeaves(city_list[current], _id, &border_matched);
			if (!border_matched.empty())
			{
				for (int x = 0; x < border_matched.size(); x++)
				{
					int __id = border_matched[x];
					if (army_route[__id] == 0)
					{
						border_matched.erase(border.begin() + x);
						x--;
					}
				}
				break;
			}
		}

		//在找到的路线中，对每条都计算距current最近军队移动到current需要的时间，取最小值
		int bestLength = 0, _current = 0;
		for (int cnt = 0, last_modify_city = -1; cnt < border_matched.size(); cnt++)
		{
			int length = 0, ex_armed_city = -1;
			_current = border_matched[cnt];
			int current_arm_city = _current;
			while (route_set[_current] != current)
			{
				if (army_list[_current] != 0)
				{
					length = 0;
					ex_armed_city = _current;
				}
				length += length_list[_current];
				_current = route_set[_current];
			}
			if (army_list[_current] != 0)
			{
				length = 0;
				ex_armed_city = _current;
			}
			length += length_list[_current];

			if (cnt == 0 || length < bestLength)
			{
				bestLength = length;
				army_list[current_arm_city]--;
				//把上个路径的军队数恢复
				if (last_modify_city != -1)
					army_list[last_modify_city]++;
				last_modify_city = current_arm_city;
			}
		}

		//如果军队需要跨过首都，那这是特殊情况，需要额外增加一段距离
		if (current == 0)
		{
			int city_to_arm = ((city_list[0]->lcity->cityID == _current) ? city_list[0]->rcity->cityID : city_list[0]->lcity->cityID);
			bestLength += length_list[city_to_arm];
			army_list[city_to_arm]++;
		}
		else
		{
			army_list[current]++;
		}

		//在所有调整中取最长时间
		if (bestLength > time_need)
		{
			time_need = bestLength;
		}

		//显然，一次调整有可能解决多个城市的问题，我们重新计算各边境是否需要军队
		border_to_arm.clear();
		for (int z = 0; z < border.size(); z++)
		{
			int _id = border[z];
			int __current = _id;
			army_route[_id] = 0;
			while (__current != -1)
			{
				army_route[_id] += army_list[__current];
				__current = route_set[__current];
			}
			if (!army_route[_id])
			{
				border_to_arm.push_back(_id);
				i--;
			}
		}
		//问题已经解决，退出循环
		if (border_to_arm.empty())
			break;
		border_matched.clear();
	}

	cout << '\n' << "需要的时间为：" << time_need << endl;
	cout << "现在的军队分布为：" << endl;
	for (int i = 0; i < city_num; i++)
		cout << i << " city has " << army_list[i] << " troops" << endl;

	delete[] route_set, army_list, army_route ,length_list;
	for (int i = 0; i < city_num; i++)
		delete city_list[i];
	delete[] city_list;

	return 0;
}