#pragma once
#ifndef PAINT_H
#define PAINT_H

#include"MyDataType.h"
#include"WriteOrRead.h"

//--------------------------------�㲿��--------------------------------------//
//����
void DrawPnt(CClientDC* dc, PNT_STRU point);

//��ʾ���е�
void ShowAllPnt(CClientDC* de, CFile* pnt1lnpF, int pntNum);
//--------------------------------�߲���--------------------------------------//
//�����߶�
void DrawSeg(CClientDC* de, LIN_NDX_STRU line, POINT pointl, POINT point2);

//��ʾ������
void ShowAllLin(CClientDC* de, CFile* LinTmpNdxF, CFile* LinTmpDatF, int LinNum);
#endif