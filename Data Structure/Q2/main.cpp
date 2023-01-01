#include<iostream>
#include<vector>

using namespace std;

bool ifArmed(int start, vector<int>* city, int* army)
{
	if (army[start])
		return 1;
	else
	{
		if (city[start].empty())
			return 0;
		else
		{
			int cnt = 0;
			for (int i = 0; i < city[start].size(); i++)
				cnt += ifArmed(city[start][i], city, army);
			if (cnt != city[start].size())
				return 0;
			else
				return 1;
		}
	}
}

bool armyCalcu(int start, const vector<int>* city, vector<int>* move, const int* army)
{
	if (army[start])
		for (int cnt = 0; cnt < army[start]; cnt++)
			(*move).push_back(start);
	if (city[start].empty())
		return 0;
	for (int i = 0; i < city[start].size(); i++)
		armyCalcu(city[start][i], city, move, army);
}

int main()
{
	int city_num = 0;
	cin >> city_num;

	int* city_set = new int[city_num] {-1};			//用并查集储存节点之间的关系
	vector<int>* city_list = new vector<int>[city_num];
	int* length_list = new int[city_num] {0};			//存储每个城市到上一城市的距离

	//建立并查集
	for (int i = 0; i < city_num - 1; i++)
	{
		int from, to, dist = 0;
		cin >> from >> to >> dist;
		city_set[to] = from;
		length_list[to] = dist;
		city_list[from].push_back(to);
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

	//每个副首都的军队数量
	vector<int>* move = new vector<int>[city_list[0].size()];
	for (int i = 0; i < city_list[0].size(); i++)
		armyCalcu(city_list[0][i], city_list, &(move[i]), army_list);

	//0代表不需要跨越首都，1代表需要
	bool type = 0;
	for (int i = 0; i < city_list[0].size(); i++)
	{
		if (move[i].empty())
		{
			type = 1;
		}
	}

	//算各个城市到首都的距离
	int* _length = new int[city_num] {0};
	for (int cnt = 0; cnt < city_num; cnt++)
	{
		int current = cnt;
		for (; current != -1; current = city_set[current])
			_length[cnt] += length_list[current];
	}

	vector<int> time;
	if (type == 1)
	{
		vector<int> major_from, major_to;
		for (int i = 0; i < city_list[0].size(); i++)
		{
			if (move[i].size() == 0)
				major_to.push_back(city_list[0][i]);
			else if (move[i].size() > 1)
				major_from.push_back(city_list[0][i]);
			else if (move[i].size() == 1)
			{
				int id = move[i][0];
				time.push_back(_length[id] - length_list[id]);
			}
		}
		//离首都最近的部队和最远的需要移动到的副首都匹配
		for (int i = 0; i < major_to.size(); i++)
		{
			//to
			int max_dist = 0;
			int __x = 0;
			int id_to;
			for (int x = 0; x < major_to.size(); x++)
			{
				if (_length[major_to[x]] > max_dist)
				{
					max_dist = _length[major_to[x]];
					__x = x;
				}
			}
			id_to = major_to[__x];
			major_to.erase(major_to.begin() + __x);
			army_list[id_to]++;
			//from
			int id_army = 0;
			int min_dist = 9999;
			int _x;
			for (int x = 0; x < major_from.size(); x++)
			{
				for (int y = 0; y < move[major_from[x]].size(); y++)
				{
					if (move[major_from[x]].size() == 1)
						continue;
					int id = move[major_from[x]][y];
					if (_length[id] < min_dist)
					{
						min_dist = _length[id];
						id_army = id;
						_x = x;
					}
				}
			}
			for (int i = 0; i < move[major_from[_x]].size(); i++)
				if (move[major_from[_x]][i] == min_dist)
				{
					army_list[move[major_from[_x]][i]]--;
					move[major_from[_x]].erase(move[major_from[_x]].begin() + i);
				}
			for (int i = 0; i < major_from.size(); i++)
				if (major_from[i] == id_army && move[major_from[i]].size() == 1)
					major_from.erase(major_from.begin() + i);

			time.push_back(length_list[id_to] + _length[id_army]);
		}
		//部分部队移走之后，剩余部队补充到位
		int _min_dist = 9999;
		int _x;
		for (int x = 0; x < major_from.size(); x++)
		{
			for (int y = 0; y < move[major_from[x]].size(); y++)
			{
				int id = move[major_from[x]][y];
				if (_length[id] < _min_dist)
				{
					_min_dist = _length[id];
					_x = x;
				}
			}
		}
		time.push_back(_min_dist - length_list[major_from[_x]]);
	}
	//不需要跨首都
	else
	{	//分布尝试，每次只让距离最近的走一步
		for (int i = 0; i < city_list[0].size(); i++)
		{
			int id = city_list[0][i];
			if (ifArmed(id, city_list, army_list))
				continue;
			int* army_time = new int[move[i].size()]{ 0 }, * current = new int[move[i].size()]{ 0 };
			for (int cnt = 0; cnt < move[i].size(); cnt++)
			{
				current[cnt] = move[i][cnt];
				//army_time[cnt] = length_list[move[i][cnt]];
			}
			int current_dist = 0;
			while(!ifArmed(id,city_list,army_list))
			{
				int min_cnt = 0;
				int min_dist = army_time[0] + length_list[current[0]];
				for (int cnt = 0; cnt < move[i].size(); cnt++)
				{
					if (army_time[cnt] + length_list[current[cnt]] < min_dist)
					{
						min_cnt = cnt;
					}
				}
				if (army_time[min_cnt] + length_list[current[min_cnt]] > current_dist)
					current_dist = army_time[min_cnt] + length_list[current[min_cnt]];
				army_time[min_cnt] += length_list[current[min_cnt]];
				army_list[current[min_cnt]]--;
				current[min_cnt] = city_set[current[min_cnt]];
				army_list[current[min_cnt]]++;
			}
			time.push_back(current_dist);
			delete[] army_time;
		}
	}

	int time_need = 0;
	for (int i = 0; i < time.size(); i++)
		if (time[i] > time_need)
			time_need = time[i];

	cout << '\n' << "需要的时间为：" << time_need << endl;
	
	delete[] city_set, army_list ,length_list, move, city_list, _length;

	return 0;
}