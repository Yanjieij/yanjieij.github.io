#pragma once
#ifndef MYDATATYPE_H
#define MYDATATYPE_H
struct PNT_STRU
{
	double x;
	double y;
	COLORREF color;
	int pattern;
	char isDel;
};

struct VERSION
{
	char flag[3];
	int verson;
};

struct  LIN_NDX_STRU
{
	char isDel;		
	COLORREF color;
	int pattern;	//�ߵ�����
	long dotNum;	//�߽ڵ���
	long datOff;	//�ڵ����괢��λ��
};

struct D_DOT
{
	double x;
	double y;
};
#endif
