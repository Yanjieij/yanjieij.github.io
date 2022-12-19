#include"pch.h"
#include"WriteOrRead.h"
//----------------------------------------------�㲿��------------------------------------------------------//

//����д����ʱ�ļ�
void WritePntToFile(CFile* PntTmpF, int i, PNT_STRU point)
{
	PntTmpF->Seek(i * sizeof(PNT_STRU), CFile::begin);
	PntTmpF->Write(&point, sizeof(PNT_STRU));
}

//����ʱ�ļ�����
void ReadTempFileToPnt(CFile* PntTmpF, int i, PNT_STRU& point)
{
	PntTmpF->Seek(i * sizeof(PNT_STRU), CFile::begin);
	PntTmpF->Read(&point, sizeof(PNT_STRU));
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

//------------------------------------------------�߲���--------------------------------------------------------//

//����i���ߵ�����д����ʱ�ļ�
void WriteLinNdxToFile(CFile* LinTmpNdxF, int i, LIN_NDX_STRU line)
{
	LinTmpNdxF->Seek(i * sizeof(LIN_NDX_STRU), CFile::begin);
	LinTmpNdxF->Write(&line, sizeof(LIN_NDX_STRU));
}

//���ߵĽڵ�����д����ʱ�ļ�
void WriteLinDatToFile(CFile* LinTmpDatF, long datOff, int i, D_DOT point)
{
	LinTmpDatF->Seek(datOff + i * sizeof(D_DOT), CFile::begin);
	LinTmpDatF->Write(&point, sizeof(D_DOT));
}

//����ʱ�������ļ���ȡ������
void ReadTempFileToLinDat(CFile* LinTmpDatF, long datOff, int i, D_DOT& pnt)
{
	LinTmpDatF->Seek(datOff + i * sizeof(D_DOT), CFile::begin);
	LinTmpDatF->Read(&pnt, sizeof(D_DOT));
}

//����ʱ�����ļ���ȡ������
void ReadTempFileToLinNdx(CFile* LinTmpNdxF, int i, LIN_NDX_STRU& LinNdx)
{
	LinTmpNdxF->Seek(i * sizeof(LIN_NDX_STRU), CFile::begin);
	LinTmpNdxF->Read(&LinNdx, sizeof(LIN_NDX_STRU));
}

//д�������ļ�
void WriteTempToLinPermanentFile(CFile* LinF, CFile* LinTmpDatF, CFile* LinTmpNdxF, VERSION LinVer, int nLin, int nLLin)
{
	LIN_NDX_STRU TempLinNdx;
	D_DOT Pnt;
	long LinNdxOffset = sizeof(VERSION) + sizeof(int) * 2;
	long LinDatOffset = LinNdxOffset + sizeof(LIN_NDX_STRU) * nLin;
	LinF->Write(&LinVer, sizeof(VERSION));//�汾��Ϣ
	LinF->Write(&nLin, sizeof(int));	  //������Ϣ
	LinF->Write(&nLLin, sizeof(int));	  //�߼���Ϣ
	for (int i = 0; i < nLin; i++)
	{
		//��ȡ������
		ReadTempFileToLinNdx(LinTmpNdxF, i, TempLinNdx);
		LinF->Seek(LinDatOffset, CFile::begin);
		for (int j = 0; j < TempLinNdx.dotNum; j++)
		{
			ReadTempFileToLinDat(LinTmpDatF,TempLinNdx.datOff,j,Pnt);
			LinF->Write(&Pnt, sizeof(D_DOT));
		}
		LinF->Seek(LinNdxOffset, CFile::begin);
		TempLinNdx.datOff = LinDatOffset;
		//д�������ļ�
		LinF->Write(&TempLinNdx, sizeof(LIN_NDX_STRU));
		LinNdxOffset += sizeof(LIN_NDX_STRU);
		LinDatOffset += (sizeof(D_DOT) * TempLinNdx.dotNum);
	}
}

//�������ļ���ȡ
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

//����������
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

//����������
void AlterStartLin(CFile* LinTmpNdxF, long subdatOff, int nLine, int subNum)
{
	LIN_NDX_STRU LinNdx;
	LinTmpNdxF->Seek(nLine * sizeof(LIN_NDX_STRU), CFile::begin);
	LinTmpNdxF->Read(&LinNdx, sizeof(LIN_NDX_STRU));
	LinNdx.datOff = subdatOff;	//���µ�����
	LinNdx.dotNum = subNum;		//���µ���Ŀ
	LinTmpNdxF->Seek(nLine * sizeof(LIN_NDX_STRU), CFile::begin);
	LinTmpNdxF->Write(&LinNdx, sizeof(LIN_NDX_STRU));
}
void AlterEndLin(CFile* LinTmpNdxF, int nLine)
{
	LIN_NDX_STRU linNdx;
	LinTmpNdxF->Seek(nLine * sizeof(LIN_NDX_STRU), CFile::begin);
	LinTmpNdxF->Read(&linNdx, sizeof(LIN_NDX_STRU));
	linNdx.dotNum = 0;
	linNdx.isDel = 0;
	LinTmpNdxF->Seek(nLine * sizeof(LIN_NDX_STRU), CFile::begin);
	LinTmpNdxF->Write(&linNdx, sizeof(LIN_NDX_STRU));
}

//----------------------------------------------������-------------------------------------------------------//
void WriteRegNdxToFile(CFile* RegTmpNdxF, int i, REG_NDX_STRU Region)
{
	RegTmpNdxF->Seek(i * sizeof(REG_NDX_STRU), CFile::begin);
	RegTmpNdxF->Write(&Region, sizeof(REG_NDX_STRU));
}

void WriteRegDatToFile(CFile* RegTmpDatF, long datOff, int i, D_DOT point)
{
	RegTmpDatF->Seek(datOff + i * sizeof(D_DOT), CFile::begin);
	RegTmpDatF->Write(&point, sizeof(D_DOT));
}

void ReadTempFileToRegNdx(CFile* RegTmpNdxF, int i, REG_NDX_STRU& RegNdx)
{
	RegTmpNdxF->Seek(i * sizeof(REG_NDX_STRU), CFile::begin);
	RegTmpNdxF->Read(&RegNdx, sizeof(REG_NDX_STRU));
}

void ReadTempFileToRegDat(CFile* RegTmpDatF, long datOff, int i, D_DOT& Pnt)
{
	RegTmpDatF->Seek(datOff + i * sizeof(D_DOT), CFile::begin);
	RegTmpDatF->Read(&Pnt, sizeof(D_DOT));
}

void WriteTempToRegPermanentFile(CFile* RegF, CFile* RegTmpDatF, CFile* RegTmpNdxF, VERSION RegVer, int nReg, int nLReg)
{
	REG_NDX_STRU TempRegNdx;
	D_DOT Pnt;
	long RegNdxOffset = sizeof(VERSION) + sizeof(int) * 2;
	long RegDatOffset = RegNdxOffset + sizeof(REG_NDX_STRU) * nReg;
	RegF->Write(&RegVer, sizeof(VERSION));//д��汾��Ϣ
	RegF->Write(&nReg, sizeof(int));//д��������
	RegF->Write(&nLReg, sizeof(int));//д���߼���
	for (int i = 0; i < nReg; i++)
	{
		ReadTempFileToRegNdx(RegTmpNdxF, i, TempRegNdx);
		RegF->Seek(RegDatOffset, CFile::begin);
		for (int j = 0; j < TempRegNdx.dotNum; j++)
		{
			ReadTempFileToRegDat(RegTmpDatF, TempRegNdx.datOff, j, Pnt);
			RegF->Write(&Pnt, sizeof(D_DOT));
		}
		RegF->Seek(RegNdxOffset, CFile::begin);
		TempRegNdx.datOff = RegDatOffset;
		RegF->Write(&TempRegNdx, sizeof(REG_NDX_STRU));
		RegNdxOffset += sizeof(REG_NDX_STRU);
		RegDatOffset += (sizeof(D_DOT) * TempRegNdx.dotNum);
	}
}


void ReadRegPermanentFileToTemp(CFile* RegF, CFile* RegTmpDatF, CFile* RegTmpNdxF, VERSION& RegVer, int& nReg, int& nLReg, long& TmpFRegDatOffset)
{
	RegF->Seek(0, CFile::begin);
	RegF->Read(&RegVer, sizeof(VERSION));
	RegF->Read(&nReg, sizeof(int));//���������
	RegF->Read(&nLReg, sizeof(int)); //���߼�����
	long RegNdxOffset = sizeof(VERSION) + sizeof(int) * 2;
	long RegDatOffset = RegNdxOffset + sizeof(REG_NDX_STRU) * nReg;
	TmpFRegDatOffset = 0;
	REG_NDX_STRU TempRegNdx;
	D_DOT Pnt;
	for (int i = 0; i < nReg; i++)
	{
		RegF->Seek(RegNdxOffset, CFile::begin);
		RegF->Read(&TempRegNdx, sizeof(REG_NDX_STRU));
		RegF->Seek(TempRegNdx.datOff, CFile::begin);
		for (int j = 0; j < TempRegNdx.dotNum; j++)
		{
			RegF->Read(&Pnt, sizeof(D_DOT));
			RegTmpDatF->Write(&Pnt, sizeof(D_DOT));
		}
		TempRegNdx.datOff = TmpFRegDatOffset;
		RegTmpNdxF->Write(&TempRegNdx, sizeof(REG_NDX_STRU));
		TmpFRegDatOffset += (sizeof(D_DOT) * TempRegNdx.dotNum);
		RegDatOffset += (sizeof(D_DOT) * TempRegNdx.dotNum);
		RegNdxOffset += sizeof(REG_NDX_STRU);
	}
}

//��������Ϣ
void UpdateReg(CFile* RegTmpNdxF, int nReg, REG_NDX_STRU Region)
{
	WriteRegNdxToFile(RegTmpNdxF, nReg, Region);
}
void UpdateReg(CFile* RegTmpNdxF, CFile* RegTmpDatF, int RegNdx, double offset_x, double offset_y)
{
	REG_NDX_STRU tReg;
	D_DOT pt;
	ReadTempFileToRegNdx(RegTmpNdxF, RegNdx, tReg);
	for (int i = 0; i < tReg.dotNum; i++)
	{
		RegTmpDatF->Seek(tReg.datOff + i * sizeof(D_DOT), CFile::begin);
		RegTmpDatF->Read(&pt, sizeof(D_DOT));
		pt.x = pt.x + offset_x;
		pt.y = pt.y + offset_y;
		RegTmpDatF->Seek(tReg.datOff + i * sizeof(D_DOT), CFile::begin);
		RegTmpDatF->Write(&pt, sizeof(D_DOT));
	}
}

