#pragma once
#ifndef WRITEORREAD_H
#define WRITEORREAD_H

#include"MyDataType.h"
//-------------------------------��--------------------------------//

//����д����ʱ�ļ�
void WritePntToFile(CFile* PntTmpF, int i, PNT_STRU point);

//����ʱ�ļ�����
void ReadTempFileToPnt(CFile* PntTmpF, int i, PNT_STRU& point);

//�򿪵��ļ�
void ReadPntPermanentFileToTemp(CFile* pntF, CFile* pntTmpF, int& nPnt, int& nLPnt);

//�޸ĵ�����
void UpdatePnt(CFile* pntTmpF, int i, PNT_STRU pnt);

//-------------------------------��--------------------------------//

//����д����ʱ�ļ�
void WriteLinNdxToFile(CFile* linTmpNdxF, int i, LIN_NDX_STRU line);

//���߽ڵ�д����ʱ�ļ�
void WriteLinDatToFile(CFile* linTmpDatF, long datOff, int i, D_DOT point);

//����ʱ�����ݶ�ȡ��
void ReadTempFileToLinDat(CFile* LinTmpDatF, long datOff, int i, D_DOT& pnt);

//����ʱ��������ȡ
void ReadTempFileToLinNdx(CFile* LinTmpNdxF, int i, LIN_NDX_STRU& LinNdx);

//��д�������ļ�
void WriteTempToLinPermanentFile(CFile * LinF, CFile * LinTmpDatF, CFile *LinTmpNdxF, VERSION LinVer, int nLin, int nLLin);

//�������ļ���ȡ
void ReadLinPermanentFileToTemp(CFile* LinF, CFile* LinTmpDatF,CFile* LinTmpNdxF, VERSION& LinVer, int& nLin, int& nLLin, long& TmpFLinDatOffset);

//����������
void UpdateLin( CFile * LinTmpNdxF, int nLin, LIN_NDX_STRU line);
void UpdateLin(CFile* LinTmpNdxF, CFile* LinTmpDatF, int LinNdx, double offset_x, double offset_y);

//����������
void AlterStartLin(CFile * LinTmpNdxF , long subdatOff, int nLine, int subNum);	//��һ����
void AlterEndLin(CFile * LinTmpNdxF, int nLine);								//�ڶ�����
//-------------------------------��--------------------------------//
void WriteRegNdxToFile(CFile* RegTmpNdxF, int i, REG_NDX_STRU Region);

void WriteRegDatToFile(CFile* RegTmpDatF, long datOff, int i, D_DOT point);

void ReadTempFileToRegNdx(CFile* RegTmpNdxF, int i, REG_NDX_STRU& RegNdx);

void ReadTempFileToRegDat(CFile* RegTmpDatF, long datOff, int i, D_DOT &Pnt);

void WriteTempToRegPermanentFile(CFile* RegF, CFile* RegTmpDatF, CFile* RegTmpNdxF, VERSION RegVer, int nReg, int nLReg);

void ReadRegPermanentFileToTemp(CFile* RegF, CFile* RegTmpDatF, CFile* RegTmpNdxF, VERSION& RegVer, int& nReg, int& nLReg, long& TmpFRegDatOffset);

//��������Ϣ
void UpdateReg(CFile* RegTmpNdxF, int nReg, REG_NDX_STRU Region);
void UpdateReg(CFile* RegTmpNdxF, CFile* RegTmpDatF, int RegNdx, double offset_x, double offset_y);
#endif