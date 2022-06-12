#pragma once
#ifndef PAINT_H
#define PAINT_H

#include"MyDataType.h"
#include"WriteOrRead.h"

//--------------------------------点部分--------------------------------------//
//画点
void DrawPnt(CClientDC* dc, PNT_STRU point);

//显示所有点
void ShowAllPnt(CClientDC* de, CFile* pnt1lnpF, int pntNum);
//--------------------------------线部分--------------------------------------//
//构造线段
void DrawSeg(CClientDC* de, LIN_NDX_STRU line, POINT pointl, POINT point2);

//显示所有线
void ShowAllLin(CClientDC* de, CFile* LinTmpNdxF, CFile* LinTmpDatF, int LinNum);
#endif