#pragma once
#ifndef WRITEORREAD_H
#define WRITEORREAD_H

#include"MyDataType.h"
//-------------------------------点--------------------------------//

//将点写入临时文件
void WritePntToFile(CFile* PntTmpF, int i, PNT_STRU point);

//从临时文件读点
void ReadTempFileToPnt(CFile* PntTmpF, int i, PNT_STRU& point);

//打开点文件
void ReadPntPermanentFileToTemp(CFile* pntF, CFile* pntTmpF, int& nPnt, int& nLPnt);

//修改点数据
void UpdatePnt(CFile* pntTmpF, int i, PNT_STRU pnt);

//-------------------------------线--------------------------------//

//将线写入临时文件
void WriteLinNdxToFile(CFile* linTmpNdxF, int i, LIN_NDX_STRU line);

//将线节点写入临时文件
void WriteLinDatToFile(CFile* linTmpDatF, long datOff, int i, D_DOT point);

//从临时线数据读取点
void ReadTempFileToLinDat(CFile* LinTmpDatF, long datOff, int i, D_DOT& pnt);

//从临时点索引读取
void ReadTempFileToLinNdx(CFile* LinTmpNdxF, int i, LIN_NDX_STRU& LinNdx);

//线写入永久文件
void WriteTempToLinPermanentFile(CFile * LinF, CFile * LinTmpDatF, CFile *LinTmpNdxF, VERSION LinVer, int nLin, int nLLin);

//从永久文件读取
void ReadLinPermanentFileToTemp(CFile* LinF, CFile* LinTmpDatF,CFile* LinTmpNdxF, VERSION& LinVer, int& nLin, int& nLLin, long& TmpFLinDatOffset);

//更新线数据
void UpdateLin( CFile * LinTmpNdxF, int nLin, LIN_NDX_STRU line);
void UpdateLin(CFile* LinTmpNdxF, CFile* LinTmpDatF, int LinNdx, double offset_x, double offset_y);

//连接两条线
void AlterStartLin(CFile * LinTmpNdxF , long subdatOff, int nLine, int subNum);	//第一条线
void AlterEndLin(CFile * LinTmpNdxF, int nLine);								//第二条线
//-------------------------------区--------------------------------//
void WriteRegNdxToFile(CFile* RegTmpNdxF, int i, REG_NDX_STRU Region);

void WriteRegDatToFile(CFile* RegTmpDatF, long datOff, int i, D_DOT point);

void ReadTempFileToRegNdx(CFile* RegTmpNdxF, int i, REG_NDX_STRU& RegNdx);

void ReadTempFileToRegDat(CFile* RegTmpDatF, long datOff, int i, D_DOT &Pnt);

void WriteTempToRegPermanentFile(CFile* RegF, CFile* RegTmpDatF, CFile* RegTmpNdxF, VERSION RegVer, int nReg, int nLReg);

void ReadRegPermanentFileToTemp(CFile* RegF, CFile* RegTmpDatF, CFile* RegTmpNdxF, VERSION& RegVer, int& nReg, int& nLReg, long& TmpFRegDatOffset);

//更新区信息
void UpdateReg(CFile* RegTmpNdxF, int nReg, REG_NDX_STRU Region);
void UpdateReg(CFile* RegTmpNdxF, CFile* RegTmpDatF, int RegNdx, double offset_x, double offset_y);
#endif