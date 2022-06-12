#pragma once
#ifndef MYDATATYPE_H
#define MYDATATYPE_H
//版本信息
struct VERSION
{
	char flag[3];
	int verson;
};
//点索引结构
struct PNT_STRU
{
	double x;
	double y;
	COLORREF color;
	int pattern;
	char isDel;
};
//线索引结构
struct  LIN_NDX_STRU
{
	char isDel;		
	COLORREF color;
	int pattern;	//线的类型
	long dotNum;	//线节点数
	long datOff;	//节点坐标储存位置
};
//节点的索引结构
struct D_DOT
{
	double x;
	double y;
};
#endif
