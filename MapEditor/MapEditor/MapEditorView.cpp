// MapEditorView.cpp: CMapEditorView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MapEditor.h"
#endif

#include "MapEditorDoc.h"
#include "MapEditorView.h"
#include"CCreateFileDlg.h"
#include"paint.h"
#include"WriteOrRead.h"
#include"Calculate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//-----------------------信息的全局变量
VERSION GPntVer = { 'P','N','T',10 };
//-----------------------点数据的全局变量
bool GPntFCreated = false;							//临时文件是否创建
CString GPntFName;									//永久文件名
CString GPntTmpFName = CString("tempPntF. dat");	//临时文件名
bool GPntChanged = false;							//是否更改
int GPntNum = 0;									//物理数
int GPntLNum = 0;									//逻辑数
CFile* GPntTmpF = new CFile();						//读取临时文件指针对象
int GPntNdx = -1;									//找到的点位于文件中的位置
PNT_STRU GTPnt;										//临时点，储存找到的点数据
//-----------------------数据的全局变量
bool GLinFCreated = false;							//临时文件是否创建
CString GLinFName;									//永久文件名
CString GLinTmpNdxFName = CString("tempLinF. ndx");	//临时索引文件名
CString GLinTmpDatFName = CString("tempLinF. dat");	//临时数据文件名
bool GLinChanged = false;							//是否更改
int GLinNum = 0;									//物理数
int GLinLNum = 0;									//逻辑数
CFile* GLinTmpDatF = new CFile();					//读取临时数据文件指针对象
CFile* GLinTmpNdxF = new CFile();					//读取临时索引文件指针对象
//-----------------------区数据的全局变量
bool GRegFCreated = false;							//临时文件是否创建
CString GRegFName;									//永久文件名
CString GRegTmpNdxFName = CString("tempRegF. ndx"); //临时索引文件名
CString GRegTmpDatFName = CString("tempRegF. dat"); //临时数据文件名
bool GRegChanged = false; 							//是否更改
int GRegNum = 0;									//物理数
int GRegLNum = 0;									//逻辑数
CFile* GRegTmpDatF = new CFile();					//读取临时数据文件指针对象
CFile* GRegTmpNdxF = new CFile();					//读取临时索引文件指针对象
//-----------------------与操作相关
enum Action {
	Noaction,
	OPERSTATE_INPUT_PNT,
	OPERSTATE_DELETE_PNT,
	OPERSTATE_MOVE_PNT,
	OPERSTATE_INPUT_LIN,
	OPERSTATE_DELETE_LIN,
	OPERSTATE_MOVE_LIN,
	OPERSTATE_INPUT_REG,
	OPERSTATE_DELETE_REG,
	OPERSTATE_MOVE_REG,
	OPERSTATE_ZOOM_IN,
	OPERSTATE_ZOOM_OUT,
	OPERSTATE_WINDOW_MOVE,
	OPERSTATE_LIN_DELETE_PNT,
	OPERSTATE_LIN_ADD_PNT,
	OPERSTATE_LINK_LIN,
	OPERSTATE_MODIFY_POINT_PARAMETER,
	OPERSTATE_MODIFY_LINE_PARAMETER,
	OPERSTATE_MODIFY_REGION_PARAMETER,
	OPERSTATE_UNDELETE_PNT,
	OPERSTATE_UNDELETE_LIN,
	OPERSTATE_UNDELETE_REG
};//枚举操作状态
Action GCurOperState;//操作参数
//--------------------------默认点的结构
PNT_STRU GPnt = { 0,0,RGB(0,0,0),0,0 };//默认点参数
																			
// CMapEditorView

IMPLEMENT_DYNCREATE(CMapEditorView, CView)

BEGIN_MESSAGE_MAP(CMapEditorView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_WINDOW_ZOOM_IN, &CMapEditorView::OnWindowZoomIn)
	ON_COMMAND(ID_FILE_NEW, &CMapEditorView::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN_POINT, &CMapEditorView::OnFileOpenPoint)
	ON_COMMAND(ID_FILE_OPEN_LINE, &CMapEditorView::OnFileOpenLine)
	ON_COMMAND(ID_FILE_OPEN_REGION, &CMapEditorView::OnFileOpenRegion)
	ON_COMMAND(ID_FILE_SAVE_POINT, &CMapEditorView::OnFileSavePoint)
	ON_COMMAND(ID_FILE_SAVE_LINE, &CMapEditorView::OnFileSaveLine)
	ON_COMMAND(ID_FILE_SAVE_REGION, &CMapEditorView::OnFileSaveRegion)
	ON_COMMAND(ID_FILE_SAVE_AS_POINT, &CMapEditorView::OnFileSaveAsPoint)
	ON_COMMAND(ID_FILE_SAVE_AS_LINE, &CMapEditorView::OnFileSaveAsLine)
	ON_COMMAND(ID_FILE_SAVE_AS_REGION, &CMapEditorView::OnFileSaveAsRegion)
	ON_COMMAND(ID_APP_EXIT, &CMapEditorView::OnAppExit)
	ON_COMMAND(ID_WINDOW_ZOOM_OUT, &CMapEditorView::OnWindowZoomOut)
	ON_COMMAND(ID_WINDOW_MOVE, &CMapEditorView::OnWindowMove)
	ON_COMMAND(ID_WINDOW_RESET, &CMapEditorView::OnWindowReset)
	ON_COMMAND(ID_WINDOW_SHOW_POINT, &CMapEditorView::OnWindowShowPoint)
	ON_COMMAND(ID_WINDOW_SHOW_LINE, &CMapEditorView::OnWindowShowLine)
	ON_COMMAND(ID_WINDOW_SHOW_REGION, &CMapEditorView::OnWindowShowRegion)
	ON_COMMAND(ID_LINE_CREATE, &CMapEditorView::OnLineCreate)
	ON_COMMAND(ID_LINE_MOVE, &CMapEditorView::OnLineMove)
	ON_COMMAND(ID_LINE_DELETED, &CMapEditorView::OnLineDeleted)
	ON_COMMAND(ID_LINE_SHOW_DELETED, &CMapEditorView::OnLineShowDeleted)
	ON_COMMAND(ID_LINE_UNDETELE, &CMapEditorView::OnLineUndetele)
	ON_COMMAND(ID_LINE_DELETE_DOT, &CMapEditorView::OnLineDeleteDot)
	ON_COMMAND(ID_LINE_ADD_DOT, &CMapEditorView::OnLineAddDot)
	ON_COMMAND(ID_LINE_MODIFY_PARAMETER, &CMapEditorView::OnLineModifyParameter)
	ON_COMMAND(ID_LINE_SET_DEPARAMETER, &CMapEditorView::OnLineSetDeparameter)
	ON_COMMAND(ID_LINE_LINK, &CMapEditorView::OnLineLink)
	ON_COMMAND(ID_POINT_CREATE, &CMapEditorView::OnPointCreate)
	ON_COMMAND(ID_POINT_MOVE, &CMapEditorView::OnPointMove)
	ON_COMMAND(ID_POINT_DELETE, &CMapEditorView::OnPointDelete)
	ON_COMMAND(ID_POINT_SHOW_DELETED, &CMapEditorView::OnPointShowDeleted)
	ON_COMMAND(ID_POINT_UNDELETE, &CMapEditorView::OnPointUndelete)
	ON_COMMAND(ID_POINT_MODIFY_PARAMETER, &CMapEditorView::OnPointModifyParameter)
	ON_COMMAND(ID_POINT_SET_DEFPARAMETER, &CMapEditorView::OnPointSetDefparameter)
	ON_COMMAND(ID_REGION_CREATE, &CMapEditorView::OnRegionCreate)
	ON_COMMAND(ID_REGION_MOVE, &CMapEditorView::OnRegionMove)
	ON_COMMAND(ID_REGION_DELETE, &CMapEditorView::OnRegionDelete)
	ON_COMMAND(ID_REGION_SHOW_DELETED, &CMapEditorView::OnRegionShowDeleted)
	ON_COMMAND(ID_REGION_UNDELETE, &CMapEditorView::OnRegionUndelete)
	ON_COMMAND(ID_REGION_MODIFY_PARAMETER, &CMapEditorView::OnRegionModifyParameter)
	ON_COMMAND(ID_REGION_SET_DEFPARAMETER, &CMapEditorView::OnRegionSetDefparameter)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CMapEditorView 构造/析构

CMapEditorView::CMapEditorView() noexcept
{
	// TODO: 在此处添加构造代码

}

CMapEditorView::~CMapEditorView()
{
}

BOOL CMapEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMapEditorView 绘图

void CMapEditorView::OnDraw(CDC* /*pDC*/)
{
	CMapEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	//重新绘图
	CRect myrect;
	GetClientRect(&myrect);
	CClientDC dc(this);
	dc.FillSolidRect(0, 0, myrect.Width(), myrect.Height(), dc.GetBkColor());
	dc.SetROP2(R2_NOTXORPEN);
	ShowAllPnt(&dc, GPntTmpF, GPntNum);
	ReleaseDC(&dc);
	// TODO: 在此处为本机数据添加绘制代码
}

void CMapEditorView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	//ClientToScreen(&point);
	//OnContextMenu(this, point);
}

void CMapEditorView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
	#ifndef SHARED_HANDLERS
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
	#endif
}


// CMapEditorView 诊断

#ifdef _DEBUG
void CMapEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CMapEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMapEditorDoc* CMapEditorView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMapEditorDoc)));
	return (CMapEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CMapEditorView 消息处理程序

void CMapEditorView::OnFileNew()
{
	//防止重复创建
	if (GPntFCreated && GLinFCreated && GRegFCreated)
	{
		MessageBox(L"您已经新建过文件了！", L"提示", MB_OK);
		return;
	}
	
	CCreateFileDlg dlg;//对话框
	if (dlg.DoModal() != IDOK)	//只有在用户按下确认时才会新建临时文件
		return;
	CString str;
	if (!GPntFCreated)	//点临时文件不存在则新建
	{
		GPntTmpFName = dlg.m_add + CString("\\") + GPntTmpFName;	//临时点数据文件名
		if (GPntTmpF->Open(GPntTmpFName, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary))
		{
			GPntFCreated = true;	//标志为成功
			str += "tempPntF.dat\n";
		}
		else
		{
			GPntTmpFName = CString("tempPntF.dat");
			TRACE(_T("文件无法打开！\n"));
		}
	}
	//线
	if (!GLinFCreated)	
	{
		GLinTmpNdxFName = dlg.m_add + CString("\\") + GLinTmpNdxFName;
		GLinTmpDatFName = dlg.m_add + CString("\\") + GLinTmpDatFName;
		if (GLinTmpNdxF->Open(GLinTmpNdxFName, CFile::modeCreate
			| CFile::modeReadWrite | CFile::typeBinary) && GLinTmpDatF->Open(GLinTmpDatFName,
				CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary))
		{
			GLinFCreated = true;
			str += "tempLinF .dat tempLinF .ndx\n";
		}
		else
		{
			GLinTmpDatFName = CString("tempLinF.dat");
			GLinTmpNdxFName = CString("tempLinF .ndx");
			TRACE(_T("文件无法打开！\n"));
		}
	}
	//区
	if (!GRegFCreated)
	{
		GRegTmpNdxFName = dlg.m_add + CString("\\") + GRegTmpNdxFName;
		GRegTmpDatFName = dlg.m_add + CString("\\") + GRegTmpDatFName;
		if (GRegTmpNdxF->Open(GRegTmpNdxFName, CFile::modeCreate
			| CFile::modeReadWrite | CFile::typeBinary) && GRegTmpDatF->Open(GRegTmpDatFName,
				CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary))
		{
			GRegFCreated = true;
			str += "tempRegF. ndx tempRegF. dat \n";
		}
		else
		{
			GRegTmpNdxFName = CString("tempRegF.ndx");
			GRegTmpDatFName = CString("tempRegF. dat");
			TRACE(_T("文件无法打开！ \n"));
		}
	}
	if (GPntFCreated && GLinFCreated && GRegFCreated)
	{
		str += "creat successful 1 ";
		MessageBox(str, L"提示", MB_OK);
	}
	// TODO: 在此添加命令处理程序代码
}

void CMapEditorView::OnFileOpenPoint()
{
	CFileDialog dlg(true);
	dlg.m_ofn.lpstrFilter = L"pnt\0*.pnt";
	//如果临时文件数据已改变，提示是否保存
	if (GPntChanged)
	{
		if (IDYES == AfxMessageBox(L"当前文件还没有保存，要先保存吗？", MB_YESNO, MB_ICONQUESTION))
			OnFileSavePoint();
	}
	//让用户指定打开的文件
	if (dlg.DoModal() == IDOK)
	{
		GPntFName = dlg.m_ofn.lpstrFile;
		CFile* pntF = new CFile();
		if (!pntF->Open(GPntFName, CFile::modeRead | CFile::typeBinary))
		{
			TRACE(_T("文件无法打开！ \n"));
			return;
		}
		int nPos = GPntFName.ReverseFind(_T('\\'));
		CString floderAdd = GPntFName.Left(nPos);
		//如果临时文件已创建，定位到开头；否则创建临时文件
		if (GPntTmpFName != "tempPntF. dat")
			GPntTmpF->SeekToBegin();
		else
		{
			GPntTmpFName = floderAdd + CString("\\") + GPntTmpFName;
			if (!GPntTmpF->Open(GPntTmpFName, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary))
			{
				GPntTmpFName = CString("tempPntF.dat");
				TRACE(_T("文件无法打开！ \n"));
			}
			else
				GPntFCreated = true;
		}
		ReadPntPermanentFileToTemp(pntF, GPntTmpF, GPntNum, GPntLNum);
		pntF->Close();
		delete pntF;
		CString windowText = dlg.GetFileName() + "-MapEditor";
		GetParent()->SetWindowTextW(windowText);
		this->InvalidateRect(NULL); //让MFC重绘窗口
	}
	GCurOperState = Noaction;
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnFileOpenLine()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnFileOpenRegion()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnFileSavePoint()
{
	GCurOperState = Noaction;
	//如果还没有打开文件
	if (GPntFCreated == false)
	{
		MessageBox(L"还没有建立文件！", L"提示", MB_OK);
		return;
	}
	CFile* pntF = new CFile();
	//如果文件名不为空，则删除原来的文件，不然让用户进行输入
	if (GPntFName.IsEmpty() == false)
		CFile::Remove(GPntFName);
	else
	{
		LPCTSTR lpszFilters;
		lpszFilters = _T("点, (* .pnt) |* .pnt ||");
		CFileDialog dlg(false, _T("pnt"), NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, lpszFilters);
		int nPos = GPntTmpFName.ReverseFind(_T('\\'));
		CString folderAdd = GPntTmpFName.Left(nPos);
		dlg.m_ofn.lpstrInitialDir = folderAdd;
		if (dlg.DoModal() == IDOK)
			GPntFName = dlg.GetPathName();
		else
			return;
	}
	PNT_STRU tempPnt;
	//重新创建永久文件，并写入版本信息和点数
	if (pntF->Open(GPntFName, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
	{
		pntF->Write(&GPntVer, sizeof(VERSION));
		pntF->Write(&GPntNum, sizeof(int));
		pntF->Write(&GPntLNum, sizeof(int));
	}
	else
		TRACE(_T("错误，文件无法打开！\n"));
	//将点临时文件信息写入永久文件
	for (int i = 0; i < GPntNum; i++)\
	{
		ReadTempFileToPnt(GPntTmpF, i, tempPnt);
		pntF->Write(&tempPnt, sizeof(PNT_STRU));
	}
	pntF->Close();
	delete pntF;
	//修改数据变化标志，改变主窗口主题
	GPntChanged = false;
	int nPos = GPntFName.ReverseFind(_T('\\') ) ;
	CString windowText = GPntFName.Right(GPntFName.GetLength() - nPos - 1) + "-MapEditor";
	GetParent()->SetWindowTextW(windowText);
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnFileSaveLine()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnFileSaveRegion()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnFileSaveAsPoint()
{
	//暂存文件名
	CString tempFName = GPntFName;
	GPntFName = CString("");
	OnFileSavePoint();
	//若另存失败，还原文件名
	if (GPntFName == "")
		GPntFName = tempFName;
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnFileSaveAsLine()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnFileSaveAsRegion()
{
	
}

//退出程序
void CMapEditorView::OnAppExit()
{
	//如果点数据已改变，提示保存
	if (GPntChanged == true)
	{
		if (IDYES == AfxMessageBox(L"文件还没有保存，要先保存再退出吗？", MB_YESNO, MB_ICONQUESTION))
			OnFileSavePoint();
	}
	//销毁窗口
	GetParent()->DestroyWindow();
}

void CMapEditorView::OnWindowZoomIn()
{
	// TODO: 在此添加命令处理程序代码
}

void CMapEditorView::OnWindowZoomOut()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnWindowMove()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnWindowReset()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnWindowShowPoint()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnWindowShowLine()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnWindowShowRegion()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnPointCreate()
{
	if (GPntFCreated)
		GCurOperState = OPERSTATE_INPUT_PNT;
	else
		MessageBox(L"还没有建立文件！", L"提示", MB_OK);
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnPointMove()
{
	if (GPntFCreated)
		GCurOperState = OPERSTATE_MOVE_PNT;
	else
		MessageBox(L"还没有创建文件！", L"提示", MB_OK);
}


void CMapEditorView::OnPointDelete()
{
	if (GPntFCreated)
		GCurOperState = OPERSTATE_DELETE_PNT;	//将状态改为删除
	else
		MessageBox(L"还没有建立文件！", L"提示", MB_OK);
}


void CMapEditorView::OnPointShowDeleted()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnPointUndelete()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnPointModifyParameter()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnPointSetDefparameter()
{
	// TODO: 在此添加命令处理程序代码
}

void CMapEditorView::OnLineCreate()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnLineMove()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnLineDeleted()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnLineShowDeleted()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnLineUndetele()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnLineDeleteDot()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnLineAddDot()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnLineLink()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnLineModifyParameter()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnLineSetDeparameter()
{
	// TODO: 在此添加命令处理程序代码
}

void CMapEditorView::OnRegionCreate()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnRegionMove()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnRegionDelete()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnRegionShowDeleted()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnRegionUndelete()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnRegionModifyParameter()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnRegionSetDefparameter()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CClientDC dc(this);
	dc.SetROP2(R2_NOTXORPEN);
	if (GPntFCreated)
	{
		switch (GCurOperState)
		{
			//绘制点
		case OPERSTATE_INPUT_PNT:
			PNT_STRU pnt;
			memcpy_s(&pnt, sizeof(PNT_STRU), &GPnt, sizeof(PNT_STRU));
			pnt.x = point.x;
			pnt.y = point.y; 
			WritePntToFile(GPntTmpF, GPntNum, pnt); //写入临时文件

			DrawPnt(&dc, pnt);
			//点数加一
			GPntNum++;
			GPntLNum++;
			GPntChanged = true;
			break;
		case OPERSTATE_DELETE_PNT:
			FindPnt(point, GPntNum, GPntTmpF, GPntNdx); 
			//如果找到了最近点
			if (GPntNdx != -1)
			{
				PNT_STRU pnt;
				ReadTempFileToPnt(GPntTmpF, GPntNdx, pnt);	//读点
				pnt.isDel = 1;								//标记删除
				UpdatePnt(GPntTmpF, GPntNdx, pnt);			//更新数据
				DrawPnt(&dc, pnt);							//重绘以清除
				GPntNdx = -1;
				GPntChanged = true;
				GPntLNum--;
			}
			break;
			//移动点
		case OPERSTATE_MOVE_PNT:
			if (GPntNdx != -1)
			{
				PNT_STRU pnt;
				ReadTempFileToPnt(GPntTmpF, GPntNdx, pnt); 
				pnt.x = point.x;
				pnt.y = point.y; 
				UpdatePnt(GPntTmpF, GPntNdx, pnt);		 //更新数据
				GPntNdx = -1;
				GPntChanged = true; 
			}
			break;
		default:
			break;
		}
	}
	CView::OnLButtonUp(nFlags, point);
}


void CMapEditorView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (GPntFCreated)
		switch (GCurOperState)
		{
		case OPERSTATE_MOVE_PNT:	//如果需要移动
			GTPnt = FindPnt(point, GPntNum, GPntTmpF, GPntNdx);
			break;
		default:
			break;
		}
	CView::OnLButtonDown(nFlags, point);
}


void CMapEditorView::OnDestroy()
{
	//销毁点
	if (GPntFCreated)
	{
		if (GPntTmpF->m_hFile != CFile::hFileNull)
		{
			GPntTmpF->Close();
			GPntTmpF->Remove(GPntTmpFName);
		}
	}
	delete GPntTmpF;
	//销毁线
	if (GLinFCreated)
	{
		if (GLinTmpDatF->m_hFile != CFile::hFileNull)
		{
			GLinTmpDatF->Close();
			GLinTmpDatF->Remove(GLinTmpDatFName);
		}
		if (GLinTmpNdxF->m_hFile != CFile::hFileNull)
		{
			GLinTmpNdxF->Close();
			GLinTmpNdxF->Remove(GLinTmpNdxFName);
		}
	}
	delete GLinTmpDatF;
	delete GLinTmpNdxF;
	//销毁区
	if (GRegFCreated)
	{
		if (GRegTmpDatF->m_hFile != CFile::hFileNull)
		{
			GRegTmpDatF->Close();
			GRegTmpDatF->Remove(GRegTmpDatFName);
		}
		if (GRegTmpNdxF->m_hFile != CFile::hFileNull)
		{
			GRegTmpNdxF->Close();
			GRegTmpNdxF->Remove(GRegTmpNdxFName);
		}
	}
	delete GRegTmpDatF;
	delete GRegTmpNdxF;

	CView::OnDestroy();
}


void CMapEditorView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (GPntFCreated)
	{
		switch (GCurOperState)
		{
		case OPERSTATE_MOVE_PNT:
			if (GPntNdx != -1)
			{
				CClientDC dc(this);
				dc.SetROP2(R2_NOTXORPEN);//进行异或模式画点
				DrawPnt(&dc, GTPnt);	//在原位置画，即清除原来的点
				GTPnt.x = point.x;
				GTPnt.y = point.y;
				DrawPnt(&dc, GTPnt);	//在新位置画点
			}
			break;
		default:
			break;
		}
	}


	CView::OnMouseMove(nFlags, point);
}
