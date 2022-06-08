#include"pch.h"
#include<math.h>
#include"paint.h"
//画点
void DrawPnt(CClientDC* dc, PNT_STRU point)
{
	CBrush brush(point.color);
	CPen pen(PS_SOLID, 1, point.color);
	CObject* oldObject;
	switch (point.pattern)
	{
		//点的形状：十字、圆、五角星
	case 0: 
	{
	oldObject = dc->SelectObject(&pen);
	dc->MoveTo((long) point.x - 4, (long)point.y);
	dc->LineTo ((long)point.x + 4, (long)point.y);
	dc->MoveTo((long) point.x, (long) point.y - 4 );
	dc->LineTo((long) point.x, (long)point.y + 4 );
	break;
	}
	case 1: 
	{
		oldObject = dc->SelectObject(&brush);
		dc->Ellipse((long)point.x - 2, (long)point.y - 2, (long)point.x + 2, (long)point.y + 2);
		break;
	}
	case 2:
	{
	oldObject = dc->SelectObject(&pen);
	//外部顶点，内部顶点
	POINT external_pt[5],interior_pt[5];
	//外部圆半径，内部圆半径
	double external_r = 10,interior_r = external_r/2;
	//顺时针求外部顶点
	external_pt[0].x = (long)point.x;
	external_pt[0].y = long(point.y - external_r );
	external_pt[1].x = long(point.x + (external_r* cos(18.0 * 3.14 / 180)));
	external_pt[1].y = long(point.y - (external_r * sin(18.0 * 3.14 / 180)));
	external_pt[2].x = long(point.x + (external_r * sin(36.0 * 3.14 / 180)));
	external_pt[2].y = long(point.y + (external_r * cos(36.0 * 3.14 / 180)));
	external_pt[3].x = long(point.x - (external_r * sin(36.0 * 3.14 / 180)));
	external_pt[3].y = long(external_pt[2].y);
	external_pt[4].x = long(point.x - (external_r * cos(18.0 * 3.14 / 180)));
	external_pt[4].y = long(external_pt[1].y);
	//顺时针求内部顶点
	interior_pt[0].x = long(point.x + (interior_r * cos(54.0 * 3.14 / 180)));
	interior_pt[0].y = long(point.y - (interior_r * sin(54.0 * 3.14 / 180)));
	interior_pt[1].x = long(point.x + (interior_r * sin(72.0 * 3.14 / 180)));
	interior_pt[1].y = long(point.y + (interior_r * cos(72.0 * 3.14 / 180)));
	interior_pt[2].x = long(point.x);
	interior_pt[2].y = long(point.y + interior_r);
	interior_pt[3].x = long(point.x - (interior_r * sin(72.0 * 3.14 / 180)));
	interior_pt[3].y = long(interior_pt[1].y);
	interior_pt[4].x = long(point.x - (interior_r * cos(54.0 * 3.14 / 180)));
	interior_pt[4].y = long(interior_pt[0].y);
	dc->MoveTo((long)external_pt[0].x, (long)external_pt[0].y);
	dc->LineTo((long)interior_pt[0].x, (long)interior_pt[0].y);
	for (int i = 1; i < 5; i++)
		{
			dc->LineTo((long)external_pt[i].x, (long)external_pt[i].y);
			dc->LineTo((long)interior_pt[i].x, (long)interior_pt[i].y);
		}
		dc->LineTo((long)external_pt[0].x, (long)external_pt[0].y);
	break;
	}
	dc->SelectObject(oldObject);
	}
}

//显示所有点
void ShowAllPnt(CClientDC* de, CFile* pntTmpF, int pntNum)
{
	PNT_STRU point;
	for (int i = 0; i < pntNum; ++i)
	{
		ReadTempFileToPnt(pntTmpF, i, point);	//读点
		if (point.isDel == 0)
			DrawPnt(de, point);					//绘制点
	}
}