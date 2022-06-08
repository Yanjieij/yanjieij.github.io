#pragma once
#ifndef WRITEORREAD_H
#define WRITEORREAD_H

#include"MyDataType.h"
//将点写入临时文件
void WritePntToFile(CFile* PntTmpF, int i, PNT_STRU point);

//从临时文件读点
void ReadTempFileToPnt(CFile* PntTmpF, int i, PNT_STRU& point);

//打开点文件
void ReadPntPermanentFileToTemp(CFile* pntF, CFile* pntTmpF, int& nPnt, int& nLPnt);

//修改点数据
void UpdatePnt(CFile* pntTmpF, int i, PNT_STRU pnt);
#endif