#include"pch.h"
#include"WriteOrRead.h"
//--------------------------------点部分--------------------------------------//

//将点写入临时文件
void WritePntToFile(CFile* PntTmpF, int i, PNT_STRU point)
{
	PntTmpF->Seek(i * sizeof(PNT_STRU), CFile::begin);
	PntTmpF->Write(&point, sizeof(PNT_STRU));
}

//从临时文件读点
void ReadTempFileToPnt(CFile* PntTmpF, int i, PNT_STRU& point)
{
	PntTmpF->Seek(i * sizeof(PNT_STRU), CFile::begin);
	PntTmpF->Read(&point, sizeof(PNT_STRU));
}

//读取点文件
void ReadPntPermanentFileToTemp(CFile* pntF, CFile* pntTmpF, int& nPnt, int& nLPnt)
{
	PNT_STRU point;
	pntF->Seek(sizeof(VERSION), CFile::begin);	//跳过文件头
	pntF->Read(&nPnt, sizeof(int));				//确定点个数
	pntF->Read(&nLPnt, sizeof(int));
	for (int i = 0; i < nPnt; ++i)
	{
		pntF->Read(&point, sizeof(PNT_STRU));		//读出数据
		pntTmpF->Write(&point, sizeof(PNT_STRU));	//写入数据
	}
}

//修改点数据
void UpdatePnt(CFile* pntTmpF, int i, PNT_STRU point)
{
	WritePntToFile(pntTmpF, i, point);
}

//--------------------------------线部分---------------------------------------//

//将第i条线的引索写入临时文件
void WriteLinNdxToFile(CFile* LinTmpNdxF, int i, LIN_NDX_STRU line)
{
	LinTmpNdxF->Seek(i * sizeof(LIN_NDX_STRU), CFile::begin);
	LinTmpNdxF->Write(&line, sizeof(LIN_NDX_STRU));
}

//将线的节点数据写入临时文件
void WriteLinDatToFile(CFile* LinTmpDatF, long datOff, int i, D_DOT point)
{
	LinTmpDatF->Seek(datOff + i * sizeof(D_DOT), CFile::begin);
	LinTmpDatF->Write(&point, sizeof(D_DOT));
}

//从临时线数据文件读取点数据
void ReadTempFileToLinDat(CFile* LinTmpDatF, long datOff, int i, D_DOT& pnt)
{
	LinTmpDatF->Seek(datOff + i * sizeof(D_DOT), CFile::begin);
	LinTmpDatF->Read(&pnt, sizeof(D_DOT));
}

//从临时索引文件读取线索引
void ReadTempFileToLinNdx(CFile* LinTmpNdxF, int i, LIN_NDX_STRU& LinNdx)
{
	LinTmpNdxF->Seek(i * sizeof(LIN_NDX_STRU), CFile::begin);
	LinTmpNdxF->Read(&LinNdx, sizeof(LIN_NDX_STRU));
}

//写入永久文件
void WriteTempToLinPermanentFile(CFile* LinF, CFile* LinTmpDatF, CFile* LinTmpNdxF, VERSION LinVer, int nLin, int nLLin)
{
	LIN_NDX_STRU TempLinNdx;
	D_DOT Pnt;
	long LinNdxOffset = sizeof(VERSION) + sizeof(int) * 2;
	long LinDatOffset = LinNdxOffset + sizeof(LIN_NDX_STRU) * nLin;
	LinF->Write(&LinVer, sizeof(VERSION));//版本信息
	LinF->Write(&nLin, sizeof(int));	  //物理信息
	LinF->Write(&nLLin, sizeof(int));	  //逻辑信息
	for (int i = 0; i < nLin; i++)
	{
		//读取线索引
		ReadTempFileToLinNdx(LinTmpNdxF, i, TempLinNdx);
		LinF->Seek(LinDatOffset, CFile::begin);
		for (int j = 0; j < TempLinNdx.dotNum; j++)
		{
			ReadTempFileToLinDat(LinTmpDatF,TempLinNdx.datOff,j,Pnt);
			LinF->Write(&Pnt, sizeof(D_DOT));
		}
		LinF->Seek(LinNdxOffset, CFile::begin);
		TempLinNdx.datOff = LinDatOffset;
		//写入永久文件
		LinF->Write(&TempLinNdx, sizeof(LIN_NDX_STRU));
		LinNdxOffset += sizeof(LIN_NDX_STRU);
		LinDatOffset += (sizeof(D_DOT) * TempLinNdx.dotNum);
	}
}

//从永久文件读取
void ReadLinPermanentFileToTemp(CFile* LinF, CFile* LinTmpDatF, CFile* LinTmpNdxF, VERSION& LinVer, int& nLin, int& nLLin, long& TmpFLinDatOffset)
{
	LinF->Seek(0, CFile::begin);
	LinF->Read(&LinVer, sizeof(VERSION));
	LinF->Read(&nLin, sizeof(int)); 
	LinF->Read(&nLLin, sizeof(int)); 
	long LinNdxOffset = sizeof(VERSION) + sizeof(int) * 2;
	long LinDatOffset = LinNdxOffset + sizeof(LIN_NDX_STRU) * nLin;
	TmpFLinDatOffset = 0;
	LIN_NDX_STRU TempLinNdx;
	D_DOT Pnt;
	for (int i = 0; i < nLin; i++)
	{
		LinF->Seek(LinNdxOffset, CFile::begin);
		LinF->Read(&TempLinNdx, sizeof(LIN_NDX_STRU));
		LinF->Seek(TempLinNdx.datOff, CFile::begin);
		for (int j = 0; j < TempLinNdx.dotNum; j++)
		{
			LinF->Read(&Pnt, sizeof(D_DOT));
			LinTmpDatF->Write(&Pnt, sizeof(D_DOT));
		}
		TempLinNdx.datOff = TmpFLinDatOffset;
		LinTmpNdxF->Write(&TempLinNdx, sizeof(LIN_NDX_STRU));
		TmpFLinDatOffset += (sizeof(D_DOT) * TempLinNdx.dotNum);
		LinDatOffset += (sizeof(D_DOT) * TempLinNdx.dotNum);
		LinNdxOffset += sizeof(LIN_NDX_STRU);
	}
}

//更新线数据
void UpdateLin(CFile *LinTmpNdxF, int nLin, LIN_NDX_STRU line)
{
	WriteLinNdxToFile(LinTmpNdxF,nLin,line);
}
void UpdateLin(CFile* LinTmpNdxF, CFile* LinTmpDatF, int LinNdx, double offset_x, double offset_y)
{
	LIN_NDX_STRU tLin;
	D_DOT pt;
	ReadTempFileToLinNdx(LinTmpNdxF, LinNdx, tLin);
	for (int i = 0; i < tLin.dotNum; i++)
	{
		LinTmpDatF->Seek(tLin.datOff + i * sizeof(D_DOT), CFile::begin);
		LinTmpDatF->Read(&pt, sizeof(D_DOT));
		pt.x = pt.x + offset_x;
		pt.y = pt.y + offset_y;
		LinTmpDatF->Seek(tLin.datOff + i * sizeof(D_DOT), CFile::begin);
		LinTmpDatF->Write(&pt, sizeof(D_DOT));
	}
}



