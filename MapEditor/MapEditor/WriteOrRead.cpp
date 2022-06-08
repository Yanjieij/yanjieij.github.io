#include"pch.h"
#include"WriteOrRead.h"
//将点写入临时文件
void WritePntToFile(CFile* PntTmpF, int i, PNT_STRU point)
{
	PntTmpF->Seek(i * sizeof(PNT_STRU), CFile::begin);
	PntTmpF->Write(&point, sizeof(PNT_STRU));
}

//从临时文件读点
void ReadTempFileToPnt(CFile* PntTmpF, int i, PNT_STRU& point)
{
	PntTmpF -> Seek(i * sizeof(PNT_STRU), CFile::begin);
	PntTmpF -> Read(&point, sizeof(PNT_STRU));
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
