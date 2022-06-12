#include "pch.h"
#include "Calculate.h"
#include "WriteOrRead.h"
#include<math.h>

double Distance(double x1, double y1, double x2, double y2)
{
	//算距离
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

//PNT和D_DOT转化的一系列函数
void PntToDot(D_DOT* dot, POINT* pnt, int num)
{
	for (int i = 0; i < num; ++i)
	{
		dot[i].x = pnt[i].x;
		dot[i].y = pnt[i].y;
	}
}
void PntToDot(D_DOT& dot, POINT pnt)
{
	dot.x = pnt.x;
	dot.y = pnt.y;
}
void DotToPnt(POINT* pnt, D_DOT* dot, int num)
{
	for (int i = 0; i < num; ++i)
	{
		pnt[i].x = (long)dot[i].x;
		pnt[i].y = (long)dot[i].y;
	}
}
void DotToPnt(POINT& pnt, D_DOT dot)
{
	pnt.x = (long)dot.x;
	pnt.y = (long)dot.y;
}

//计算点到线的距离
double DisPntToSeg(D_DOT pt1, D_DOT pt2, D_DOT pt)
{
	double a = (pt2.x - pt1.x) * (pt.x - pt1.x) + (pt2.y - pt1.y) * (pt.y - pt1.y);
	double b = (pt1.x - pt2.x) * (pt.x - pt2.x) + (pt1.y - pt2.y) * (pt.y - pt2.y);
	if (a * b > 1e-10)
	{
		double area;
		double hight;
		double s = (Distance(pt1.x, pt1.y, pt2.x, pt2.y)
			+ Distance(pt.x, pt.y, pt1.x, pt1.y)
			+ Distance(pt.x, pt.y, pt2.x, pt2.y))/2;
		area = sqrt(s * (s - Distance(pt.x, pt.y, pt1.x, pt1.y))
			* (s - Distance(pt.x, pt.y, pt2.x, pt2.y))
			* (s - Distance(pt1.x, pt1.y, pt2.x, pt2.y)));
		hight = 2 * area / Distance(pt1.x, pt1.y, pt2.x, pt2.y);
		return hight;
	}
	else
	{
		return (Distance(pt1.x, pt1.y, pt.x, pt.y) > Distance(pt.x, pt.y, pt2.x, pt2.y))
			? Distance(pt.x, pt.y, pt2.x, pt2.y)
			: Distance(pt.x, pt.y, pt1.x, pt1.y);
	}
}

//寻找最近的线
LIN_NDX_STRU FindLin(CFile* LinTmpNdxF, CFile * LinTmpDatF, CPoint mousePoint,int LinNum, int& nLinNdx)
{
	double min = 10;
	LIN_NDX_STRU tLine =
	{
		tLine.isDel = 0,
		tLine.color = RGB(0,0,0),
		tLine.pattern = 0,
		tLine.dotNum = 0,
		tLine.datOff = 0
	};
	LIN_NDX_STRU line;
	D_DOT pt1, pt2, mpt;
	CFile tempLinDatF;
	for (int i = 0; i < LinNum; ++i)
	{
		ReadTempFileToLinNdx(LinTmpNdxF, i, line);
		if (line.isDel == 0)
			for (int j = 0; j < line.dotNum - 1; ++j)
			{
				ReadTempFileToLinDat(LinTmpDatF, line.datOff, j, pt1);
				ReadTempFileToLinDat(LinTmpDatF, line.datOff, j + 1, pt2);
				mpt.x = mousePoint.x;
				mpt.y = mousePoint.y;
				if (isSmall(min, DisPntToSeg(pt1, pt2, mpt)))
				{
					nLinNdx = i;
					min = DisPntToSeg(pt1, pt2, mpt);
					tLine = line;
				}
			}
	}
	return tLine;
}