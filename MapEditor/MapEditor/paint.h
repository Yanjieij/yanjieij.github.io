#pragma once
#ifndef PAINT_H
#define PAINT_H

#include"MyDataType.h"
#include"WriteOrRead.h"

//--------------------------------�㲿��--------------------------------------//
//����
void DrawPnt(CClientDC* dc, PNT_STRU point);

//��ʾ���е�
void ShowAllPnt(CClientDC* de, CFile* PntTmpF, int PntNum, double zoomOffset_x, double zoomOffset_y, double zoom, char isDel);

//--------------------------------�߲���--------------------------------------//
//�����߶�
void DrawSeg(CClientDC* de, LIN_NDX_STRU line, POINT pointl, POINT point2);

//��ʾ������
void ShowAllLin(CClientDC* de, CFile* LinTmpNdxF, CFile* LinTmpDatF, int LinNum, double zoomOffset_x, double zoomOffset_y, double zoom, char isDel);

//--------------------------------������----------------------------------------//
//չʾ������
void ShowAllReg(CClientDC* dc, CFile* RegTmpNdxF, CFile* RegTmpDatF, int RegNum, double zoomOffset_x, double zoomOffset_y, double zoom, char isDel);

//����
void DrawReg(CClientDC* de, REG_NDX_STRU Region, POINT* pt, long nPnt);
#endif