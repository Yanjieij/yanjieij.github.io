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

PNT_STRU FindDeletePnt(CPoint mousePoint, int PntNum, CFile* PntTmpF, int& nPnt)
{
	//寻找最近的删除的点
	PNT_STRU point;
	PNT_STRU tPnt = { tPnt.x = 0,tPnt.y = 0, tPnt.color = RGB(0,0,0), tPnt.pattern = 0,tPnt.isDel = 0 };
	double min = 10;
	for (int i = 0; i < PntNum; ++i)
	{
		ReadTempFileToPnt(PntTmpF, i, point);
		if (!point.isDel)
			continue;
		double dist = Distance(mousePoint.x, mousePoint.y, point.x, point.y);
		if (isSmall(min, dist))
		{
			min = dist;
			tPnt = point;
			nPnt = i;
		}
	}
	return tPnt;
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

//寻找删除的线
LIN_NDX_STRU FindDeleteLin(CFile* LinTmpNdxF, CFile* LinTmpDatF, CPoint mousePoint, int LinNum, int& nLinNdx)
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
		if (line.isDel)
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

//数据坐标转化为窗口坐标系
void PntDPtoVP(D_DOT& pt, double zoom, double offset_x, double offset_y)
{
	pt.x = pt.x - offset_x;
	pt.y = pt.y - offset_y;
	pt.x = zoom * pt.x;
	pt.y = zoom * pt.y;
}

//窗口坐标转化为数据坐标系
void PntVPtoDP(D_DOT& pt, double zoom, double offset_x, double offset_y)
{
	pt.x = pt.x / zoom;
	pt.y = pt.y / zoom;
	pt.x = pt.x + offset_x;
	pt.y = pt.y + offset_y;
}

//计算矩形中心
D_DOT GetCenter(RECT rect)
{
	D_DOT pt;
	pt.y = 0.5 * (rect.bottom + rect.top);
	pt.x = 0.5 * (rect.right + rect.left);
	return pt;
}

//计算放大倍数
void modulusZoom(RECT client, RECT rect, double& zoom)
{
	zoom = min(client.right / (double)(rect.right - rect.left), client.bottom / (double)(rect.bottom - rect.top));
}

//连接线
void AlterLindot(CFile* LinTmpDatF, LIN_NDX_STRU startLine, LIN_NDX_STRU endLine ,D_DOT firstPNT,D_DOT secondPNT, long allDataOff,int order1,int order2)
{
	D_DOT pt1, pt2, pt3, pt4, point;
	int ndot = 0;
	//分别从临时文件中读取两条线的端点数据
	ReadTempFileToLinDat(LinTmpDatF, startLine.datOff, 0, pt1);
	ReadTempFileToLinDat(LinTmpDatF, startLine.datOff, startLine.dotNum - 1, pt2);
	ReadTempFileToLinDat(LinTmpDatF, endLine.datOff, 0, pt3);
	ReadTempFileToLinDat(LinTmpDatF, endLine.datOff, endLine.dotNum - 1, pt4);
	if (order1 == 0 && order2 == 0)
	{
		//第一条线起点与第二条线起点连接
		for (int i = endLine.dotNum - 1; i >= 0; --i)
		{	//反向读取第二条线的节点并依次写入文件中
			ReadTempFileToLinDat(LinTmpDatF, endLine.datOff, i, point);
			WriteLinDatToFile(LinTmpDatF, allDataOff, ndot, point);
			ndot++;
		}
		for (int i = 0; i < startLine.dotNum; ++i)
		{	//正向读取第一条线的节点并依次写入文件中
			ReadTempFileToLinDat(LinTmpDatF, startLine.datOff, i, point);
			WriteLinDatToFile(LinTmpDatF, allDataOff, ndot, point);
			ndot++;
		}
	}
	else if (order1 == 0 && order2 == 1)
	{
		//第一条线起点与第二条线终点连接
		for (int i = 0; i < endLine.dotNum; ++i)
		{	//正向读取第二条线节点并依次写入文件中
			ReadTempFileToLinDat(LinTmpDatF, endLine.datOff, i, point);
			WriteLinDatToFile(LinTmpDatF, allDataOff, ndot, point);
			ndot++;
		}
		for (int i = 0; i < startLine.dotNum; ++i)
		{	//正向读取第一条线节点并依次写入文件中
			ReadTempFileToLinDat(LinTmpDatF, startLine.datOff, i, point);
			WriteLinDatToFile(LinTmpDatF, allDataOff, ndot, point);
			ndot++;
		}
	}
	else if (order1 == 1 && order2 == 0)
	{
		//第一条线终点与第二条线起点连接
		for (int i = 0; i < startLine.dotNum; ++i)
		{	//正向读取第一条线的节点
			ReadTempFileToLinDat(LinTmpDatF, startLine.datOff, i, point);
			WriteLinDatToFile(LinTmpDatF, allDataOff, ndot, point);
			ndot++;
		}
		for (int i = 0; i < endLine.dotNum; ++i)
		{	//正向读取第二条线的节点
			ReadTempFileToLinDat(LinTmpDatF, endLine.datOff, i, point);
			WriteLinDatToFile(LinTmpDatF, allDataOff, ndot, point);
			ndot++;
		}
	}
	else if (order1 == 1 && order2 == 1)
	{
		//第一条线终点与第二条线终点连接
		for (int i = 0; i < startLine.dotNum; ++i)
		{
			ReadTempFileToLinDat(LinTmpDatF, startLine.datOff, i, point);
			WriteLinDatToFile(LinTmpDatF, allDataOff, ndot, point);
			ndot++;
		}
		for (int i = endLine.dotNum - 1; i >= 0; --i)
		{
			ReadTempFileToLinDat(LinTmpDatF, endLine.datOff, i, point);
			WriteLinDatToFile(LinTmpDatF, allDataOff, ndot, point);
			ndot++;
		}
	}
	ndot = 0;
}

//判断点击区域是否在区内
BOOL PtinPolygon(CPoint p, D_DOT* ptPolygon, int nCount)
{
	int nCross = 0;
	for (int i = 0; i < nCount; i++)
	{
		D_DOT p1 = ptPolygon[i];
		D_DOT p2 = ptPolygon[(i + 1) % nCount];
		if (p1.y == p2.y)
			continue;
		if (p.y < min(p1.y, p2.y))
			continue;
		if (p.y >= max(p1.y, p2.y))
			continue;
		double x = (double)(p.y - p1.y) * (double)(p2.x - p1.x) / (double)(p2.y - p1.y) + p1.x;
		if (x > p.x)
			nCross++;
	}
	return (nCross % 2 == 1);
}

//寻找最近的区
REG_NDX_STRU FindReg(CFile* RegTmpNdxF, CFile* RegTmpDatF, CPoint mousePoint, int RegNum, int& nRegNdx)
{
	REG_NDX_STRU RegNdx,
		tRegNdx =
	{
		tRegNdx.isDel = 0,
		tRegNdx.color = RGB(0, 0, 0),
		tRegNdx.pattern = 0,
		tRegNdx.dotNum = 0,
		tRegNdx.datOff = 0
	};
	for (int i = 0; i < RegNum; i++)
	{
		ReadTempFileToRegNdx(RegTmpNdxF, i, RegNdx);
		if (RegNdx.isDel == 0)
		{
			D_DOT* pt = new D_DOT[RegNdx.dotNum];
			for (int j = 0; j < RegNdx.dotNum; j++)
				ReadTempFileToRegDat(RegTmpDatF, RegNdx.datOff, j, pt[j]);
			if (PtinPolygon(mousePoint, pt, RegNdx.dotNum))
			{
				tRegNdx = RegNdx;
				nRegNdx = i;
				delete[] pt;
				break;
			}
			else
				delete[]pt;
		}
	}
	return tRegNdx;
}

//寻找删除的区
REG_NDX_STRU FindDeleteReg(CFile* RegTmpNdxF, CFile* RegTmpDatF, CPoint mousePoint, int RegNum, int& nRegNdx)
{
	REG_NDX_STRU RegNdx,
	tRegNdx =
	{
		tRegNdx.isDel = 0,
		tRegNdx.color = RGB(0, 0, 0),
		tRegNdx.pattern = 0,
		tRegNdx.dotNum = 0,
		tRegNdx.datOff = 0
	};
	for (int i = 0; i < RegNum; i++)
	{
		ReadTempFileToRegNdx(RegTmpNdxF, i, RegNdx);
		if (RegNdx.isDel)
		{
			D_DOT* pt = new D_DOT[RegNdx.dotNum];
			for (int j = 0; j < RegNdx.dotNum; j++)
				ReadTempFileToRegDat(RegTmpDatF, RegNdx.datOff, j, pt[j]);
			if (PtinPolygon(mousePoint, pt, RegNdx.dotNum))
			{
				tRegNdx = RegNdx;
				nRegNdx = i;
				delete[] pt;
				break;
			}
			else
				delete[]pt;
		}
	}
	return tRegNdx;
}