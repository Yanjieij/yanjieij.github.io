#pragma once
#ifndef WRITEORREAD_H
#define WRITEORREAD_H

#include"MyDataType.h"
//����д����ʱ�ļ�
void WritePntToFile(CFile* PntTmpF, int i, PNT_STRU point);

//����ʱ�ļ�����
void ReadTempFileToPnt(CFile* PntTmpF, int i, PNT_STRU& point);

//�򿪵��ļ�
void ReadPntPermanentFileToTemp(CFile* pntF, CFile* pntTmpF, int& nPnt, int& nLPnt);

//�޸ĵ�����
void UpdatePnt(CFile* pntTmpF, int i, PNT_STRU pnt);
#endif