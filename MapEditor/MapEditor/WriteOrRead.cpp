#include"pch.h"
#include"WriteOrRead.h"
//����д����ʱ�ļ�
void WritePntToFile(CFile* PntTmpF, int i, PNT_STRU point)
{
	PntTmpF->Seek(i * sizeof(PNT_STRU), CFile::begin);
	PntTmpF->Write(&point, sizeof(PNT_STRU));
}

//����ʱ�ļ�����
void ReadTempFileToPnt(CFile* PntTmpF, int i, PNT_STRU& point)
{
	PntTmpF -> Seek(i * sizeof(PNT_STRU), CFile::begin);
	PntTmpF -> Read(&point, sizeof(PNT_STRU));
}

//��ȡ���ļ�
void ReadPntPermanentFileToTemp(CFile* pntF, CFile* pntTmpF, int& nPnt, int& nLPnt)
{
	PNT_STRU point;
	pntF->Seek(sizeof(VERSION), CFile::begin);	//�����ļ�ͷ
	pntF->Read(&nPnt, sizeof(int));				//ȷ�������
	pntF->Read(&nLPnt, sizeof(int));
	for (int i = 0; i < nPnt; ++i)
	{
		pntF->Read(&point, sizeof(PNT_STRU));		//��������
		pntTmpF->Write(&point, sizeof(PNT_STRU));	//д������
	}
}

//�޸ĵ�����
void UpdatePnt(CFile* pntTmpF, int i, PNT_STRU point)
{
	WritePntToFile(pntTmpF, i, point);
}
