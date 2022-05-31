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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//-----------------------点数据的全局变量
bool GPntFCreated = false;							//临时文件是否创建
CString GPntFName;									//永久文件名
CString GPntTmpFName = CString("tempPntF. dat");	//临时文件名
bool GPntChanged = false;							//是否更改
int GPntNum = 0;									//物理数
int GPntLNum = 0;									//逻辑数
CFile* GPntTmpF = new CFile();						//读取临时文件指针对象
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

	// TODO: 在此处为本机数据添加绘制代码
}

void CMapEditorView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
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
		MessageBox(L"File have been created.", L"Message", MB_OK);
		return;
	}
	//只有在用户按下确认时才会新建临时文件
	CCreateFileDlg dlg;
	if (dlg.DoModal() != IDOK)
		return;
	CString str;
	if (!GPntFCreated)
	{
		GPntTmpFName = dlg.m_add + CString("\\") + GPntTmpFName;
		if (GPntTmpF->Open(GPntTmpFName, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary))
		{
			GPntFCreated = true;
			str += "tempPntF.dat\n";
		}
		else
		{
			GPntTmpFName = CString("tempPntF.dat");
			TRACE(_T("File could not be opened n"));
		}
	}
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
			TRACE(_T("File could not be opened \n"));
		}
	}
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
			TRACE(_T("File could not be opened \n"));
		}
	}
	if (GPntFCreated && GLinFCreated && GRegFCreated)
	{
		str += "creat successful 1 ";
		MessageBox(str, L"message", MB_OK);
	}
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnFileOpenPoint()
{
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
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnFileSaveAsLine()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnFileSaveAsRegion()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnAppExit()
{
	// TODO: 在此添加命令处理程序代码
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
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnPointMove()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnPointDelete()
{
	// TODO: 在此添加命令处理程序代码
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
