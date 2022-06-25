#pragma once
#ifndef PAINT_H
#define PAINT_H

#include"MyDataType.h"
#include"WriteOrRead.h"

//--------------------------------点部分--------------------------------------//
//画点
void DrawPnt(CClientDC* dc, PNT_STRU point);

//显示所有点
void ShowAllPnt(CClientDC* de, CFile* PntTmpF, int PntNum, double zoomOffset_x, double zoomOffset_y, double zoom, char isDel);

//--------------------------------线部分--------------------------------------//
//构造线段
void DrawSeg(CClientDC* de, LIN_NDX_STRU line, POINT pointl, POINT point2);

//显示所有线
void ShowAllLin(CClientDC* de, CFile* LinTmpNdxF, CFile* LinTmpDatF, int LinNum, double zoomOffset_x, double zoomOffset_y, double zoom, char isDel);

//--------------------------------区部分----------------------------------------//
//展示所有区
void ShowAllReg(CClientDC* dc, CFile* RegTmpNdxF, CFile* RegTmpDatF, int RegNum, double zoomOffset_x, double zoomOffset_y, double zoom, char isDel);

//画区
void DrawReg(CClientDC* de, REG_NDX_STRU Region, POINT* pt, long nPnt);
#endif