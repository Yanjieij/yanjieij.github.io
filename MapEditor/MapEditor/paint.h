#pragma once
#ifndef PAINT_H
#define PAINT_H

#include"MyDataType.h"
#include"WriteOrRead.h"
//画点
void DrawPnt(CClientDC* dc, PNT_STRU point);

//显示所有点
void ShowAllPnt(CClientDC* de, CFile* pnt1lnpF, int pntNum);
#endif