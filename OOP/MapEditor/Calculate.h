#pragma once
#ifndef CALCULATE_H
#define CALCULATE_H

#include "MyDataType.h"

//�������
double Distance(double x1, double y1, double x2, double y2);

//Ѱ�������
PNT_STRU FindPnt(CPoint mousePoint, int pntNum, CFile* pntTmpF, int& nPnt);

//����ɾ����
PNT_STRU FindDeletePnt(CPoint mousePoint, int PntNum, CFile* Pnt1lnpF, int& nPnt);

//D_DOT��POINT�໥ת��
void PntToDot(D_DOT* dot, POINT* pnt, int num);
void PntToDot(D_DOT& dot, POINT pnt);
void DotToPnt(POINT* pnt, D_DOT* dot, int num);
void DotToPnt(POINT& pnt, D_DOT dot);

//�㵽�ߵľ������
double DisPntToSeg(D_DOT ptl, D_DOT pt2, D_DOT pt);

//Ѱ���������
LIN_NDX_STRU FindLin(CFile* LinTmpNdxF, CFile* LinTmpDatF, CPoint mousePoint, int LinNum, int& nLinNdx);

//Ѱ��ɾ������
LIN_NDX_STRU FindDeleteLin(CFile* LinTmpNdxF, CFile* LinTmpDatF, CPoint mousePoint, int LinNum, int& nLinNdx);

//��������ת��Ϊ��������ϵ
void PntDPtoVP(D_DOT& pt, double zoom, double offset_x, double offset_y);

//��������ת��Ϊ��������ϵ
void PntVPtoDP(D_DOT& pt, double zoom, double offset_x, double offset_y);

//�����������
D_DOT GetCenter(RECT rect);

//����Ŵ���
void modulusZoom(RECT client, RECT rect, double& zoom);

//�ı��ߵĵ�����
void AlterLindot(CFile* LinTmpDatF, LIN_NDX_STRU startLine, LIN_NDX_STRU endLine, D_DOT firstPNT, D_DOT secondPNT, long allDataOff, int order1, int order2);

//�жϵ��λ���Ƿ�������
BOOL PtinPolygon(CPoint p, D_DOT* ptPolygon, int nCount);

//Ѱ���������
REG_NDX_STRU FindReg(CFile* RegTmpNdxF, CFile* RegTmpDatF, CPoint mousePoint, int RegNum, int& nRegNdx);

//Ѱ��ɾ������
REG_NDX_STRU FindDeleteReg(CFile* RegTmpNdxF, CFile* RegTmpDatF, CPoint mousePoint, int RegNum, int& nRegNdx);
#endif