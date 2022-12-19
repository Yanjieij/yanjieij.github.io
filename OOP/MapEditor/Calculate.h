#pragma once
#ifndef CALCULATE_H
#define CALCULATE_H

#include "MyDataType.h"

//计算距离
double Distance(double x1, double y1, double x2, double y2);

//寻找最近点
PNT_STRU FindPnt(CPoint mousePoint, int pntNum, CFile* pntTmpF, int& nPnt);

//查找删除点
PNT_STRU FindDeletePnt(CPoint mousePoint, int PntNum, CFile* Pnt1lnpF, int& nPnt);

//D_DOT和POINT相互转化
void PntToDot(D_DOT* dot, POINT* pnt, int num);
void PntToDot(D_DOT& dot, POINT pnt);
void DotToPnt(POINT* pnt, D_DOT* dot, int num);
void DotToPnt(POINT& pnt, D_DOT dot);

//点到线的距离计算
double DisPntToSeg(D_DOT ptl, D_DOT pt2, D_DOT pt);

//寻找最近的线
LIN_NDX_STRU FindLin(CFile* LinTmpNdxF, CFile* LinTmpDatF, CPoint mousePoint, int LinNum, int& nLinNdx);

//寻找删除的线
LIN_NDX_STRU FindDeleteLin(CFile* LinTmpNdxF, CFile* LinTmpDatF, CPoint mousePoint, int LinNum, int& nLinNdx);

//数据坐标转化为窗口坐标系
void PntDPtoVP(D_DOT& pt, double zoom, double offset_x, double offset_y);

//窗口坐标转化为数据坐标系
void PntVPtoDP(D_DOT& pt, double zoom, double offset_x, double offset_y);

//计算矩形中心
D_DOT GetCenter(RECT rect);

//计算放大倍数
void modulusZoom(RECT client, RECT rect, double& zoom);

//改变线的点数据
void AlterLindot(CFile* LinTmpDatF, LIN_NDX_STRU startLine, LIN_NDX_STRU endLine, D_DOT firstPNT, D_DOT secondPNT, long allDataOff, int order1, int order2);

//判断点击位置是否在区内
BOOL PtinPolygon(CPoint p, D_DOT* ptPolygon, int nCount);

//寻找最近的区
REG_NDX_STRU FindReg(CFile* RegTmpNdxF, CFile* RegTmpDatF, CPoint mousePoint, int RegNum, int& nRegNdx);

//寻找删除的区
REG_NDX_STRU FindDeleteReg(CFile* RegTmpNdxF, CFile* RegTmpDatF, CPoint mousePoint, int RegNum, int& nRegNdx);
#endif