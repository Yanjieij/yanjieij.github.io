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
//区索引结构
struct REG_NDX_STRU
{
	char isDel; 
	COLORREF color;
	int pattern; //图案
	long dotNum; //边界节点数
	long datOff; //边界节点数据储存位置
};
//用于存储缩放数据
struct zoomData
{
	CPoint GZoomLBDPnt;			//放大时左键的点
	CPoint GZoomMMPnt;			//放大前一状态
	double GZoomOffset_x;			//偏移向量
	double GZoomOffset_y;
	double GZoom;					//缩放系数
	int GZoomStyle;					//放大方式
};
#endif
