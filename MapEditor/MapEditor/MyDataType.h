#pragma once
#ifndef MYDATATYPE_H
#define MYDATATYPE_H
//�汾��Ϣ
struct VERSION
{
	char flag[3];
	int verson;
};
//�������ṹ
struct PNT_STRU
{
	double x;
	double y;
	COLORREF color;
	int pattern;
	char isDel;
};
//�������ṹ
struct  LIN_NDX_STRU
{
	char isDel;		
	COLORREF color;
	int pattern;	//�ߵ�����
	long dotNum;	//�߽ڵ���
	long datOff;	//�ڵ����괢��λ��
};
//�ڵ�������ṹ
struct D_DOT
{
	double x;
	double y;
};
//�������ṹ
struct REG_NDX_STRU
{
	char isDel; 
	COLORREF color;
	int pattern; //ͼ��
	long dotNum; //�߽�ڵ���
	long datOff; //�߽�ڵ����ݴ���λ��
};
//���ڴ洢��������
struct zoomData
{
	CPoint GZoomLBDPnt;			//�Ŵ�ʱ����ĵ�
	CPoint GZoomMMPnt;			//�Ŵ�ǰһ״̬
	double GZoomOffset_x;			//ƫ������
	double GZoomOffset_y;
	double GZoom;					//����ϵ��
	int GZoomStyle;					//�Ŵ�ʽ
};
#endif
