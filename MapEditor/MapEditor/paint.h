#pragma once
#ifndef PAINT_H
#define PAINT_H

#include"MyDataType.h"
#include"WriteOrRead.h"
//����
void DrawPnt(CClientDC* dc, PNT_STRU point);

//��ʾ���е�
void ShowAllPnt(CClientDC* de, CFile* pnt1lnpF, int pntNum);
#endif