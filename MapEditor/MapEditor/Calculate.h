#pragma once
#ifndef CALCULATE_H
#define CALCULATE_H

#include "MyDataType.h"
//�������
double Distance(double x1, double y1, double x2, double y2);
//Ѱ�������
PNT_STRU FindPnt(CPoint mousePoint, int pntNum, CFile* pntTmpF, int& nPnt);
#endif