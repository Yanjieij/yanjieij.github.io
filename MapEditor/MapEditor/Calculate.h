#pragma once
#ifndef CALCULATE_H
#define CALCULATE_H

#include "MyDataType.h"
//计算距离
double Distance(double x1, double y1, double x2, double y2);

//寻找最近点
PNT_STRU FindPnt(CPoint mousePoint, int pntNum, CFile* pntTmpF, int& nPnt);

//D_DOT和POINT相互转化
void PntToDot(D_DOT* dot, POINT* pnt, int num);
void PntToDot(D_DOT& dot, POINT pnt);
void DotToPnt(POINT* pnt, D_DOT* dot, int num);
void DotToPnt(POINT& pnt, D_DOT dot);

//点到线的距离计算
double DisPntToSeg(D_DOT ptl, D_DOT pt2, D_DOT pt);

//寻找最近的线
LIN_NDX_STRU FindLin(CFile* LinTmpNdxF, CFile* LinTmpDatF, CPoint mousePoint, int LinNum, int& nLinNdx);
#endif