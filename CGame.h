#pragma once
#include<iostream>
#include<stdlib.h>
#include<iomanip>
#include "windows.h"

using namespace std;

class CGame
{
public:
	CGame(int difficulty);          //��ʼ��
	virtual ~CGame();
	void Crea_map();				//���붯̬�ڴ�
	void Print();                   //��ӡ��ǰͼ
	bool DoStep(char input);		//ִ�в���
	void FlgMine(int x, int y);     //�����
	void Open(int x, int y);        //�򿪸ý��
	bool Judge();                   //�ж��Ƿ����
	void Cal_mine(int x,int y);		//������Χ�����������
	void Open_around(int x,int y);  //��������Χ������

	int m_difficulty; int m_sum; int m_cnt_flg; int m_judge_sign;
private:
	int m_size;double m_mine_ratio;
	char** m_print_map; int** m_map;
};

