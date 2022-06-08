#include "pch.h"
#include "Calculate.h"
#include "WriteOrRead.h"
#include<math.h>

double Distance(double x1, double y1, double x2, double y2)
{
	return sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
}

bool isSmall(double a, double b)
{
	return ((a < b) ? false : true);
}

PNT_STRU FindPnt(CPoint mousePoint, int pntNum, CFile* pntTmpF, int& nPnt)
{
	//寻找最近的点
	PNT_STRU point, tPnt = { 0,0,RGB(0,0,0),0,0 };
	double min = 20;
	double dst;
	for (int i = 0; i < pntNum; i++)
	{
		ReadTempFileToPnt(pntTmpF, i, point);
		if (point.isDel)
			continue;
		dst = Distance(mousePoint.x, mousePoint.y, point.x, point.y);
		if (dst <= min)
		{
			min = dst;
			tPnt = point;
			nPnt = i;
		}
	}
	return(tPnt);
}