#pragma once
#include<iostream>
#include<stdlib.h>
#include<iomanip>
#include "windows.h"

using namespace std;

class CGame
{
public:
	CGame(int difficulty);          //初始化
	virtual ~CGame();
	void Crea_map();				//申请动态内存
	void Print();                   //打印当前图
	bool DoStep(char input);		//执行步骤
	void FlgMine(int x, int y);     //标记雷
	void Open(int x, int y);        //打开该结点
	bool Judge();                   //判断是否结束
	void Cal_mine(int x,int y);		//计算周围区域地雷数量
	void Open_around(int x,int y);  //连带打开周围的区域

	int m_difficulty; int m_sum; int m_cnt_flg; int m_judge_sign;
private:
	int m_size;double m_mine_ratio;
	char** m_print_map; int** m_map;
};

