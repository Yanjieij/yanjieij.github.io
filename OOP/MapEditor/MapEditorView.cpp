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
#include"CPointParameterDlg.h"
#include"CLineParameterDlg.h"
#include"CRegionParameterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//-----------------------信息的全局变量
VERSION GPntVer = { 'P','N','T',10 };
VERSION GLinVer = { 'L','I','N',10 };
VERSION GRegVer = { 'R','E','G',10 };
//-----------------------点数据的全局变量
bool GPntFCreated = false;							//临时文件是否创建
CString GPntFName;									//永久文件名
CString GPntTmpFName = CString("tempPntF.dat");		//临时文件名
bool GPntChanged = false;							//是否更改
int GPntNum = 0;									//物理数
int GPntLNum = 0;									//逻辑数
PNT_STRU GPnt = { 0,0,RGB(0,0,0),0,0 };				//默认点参数
CFile* GPntTmpF = new CFile();						//读取临时文件指针对象
int GPntNdx = -1;									//找到的点位于文件中的位置
PNT_STRU GTPnt;										//临时点，储存找到的点数据
CString getPntNum()
{
	CString output = _T("点：");
	CString first, second;
	first.Format(_T("%d"), GPntLNum);
	second.Format(_T("%d"), GPntNum);
	return output + first + '/' + second;
}
CPoint GDeleteLBDPnt(-1, -1);			
CPoint GDeleteMMPnt(-1, -1);			
CPoint GModifyLBDPnt(-1, -1);			
CPoint GModifyMMPnt(-1, -1);			
//-----------------------线数据的全局变量
bool GLinFCreated = false;							//临时文件是否创建
CString GLinFName;									//永久文件名
CString GLinTmpNdxFName = CString("tempLinF.ndx");	//临时索引文件名
CString GLinTmpDatFName = CString("tempLinF.dat");	//临时数据文件名
bool GLinChanged = false;							//是否更改
int GLinNum = 0;									//物理数
int GLinLNum = 0;									//逻辑数
LIN_NDX_STRU GLin = { GLin.isDel = 0, GLin.color = RGB(0, 0, 0), GLin.pattern = 0,GLin.dotNum = 0, GLin.datOff = 0 };
													//默认线参数
LIN_NDX_STRU GTLin;									//线
POINT GLPnt = { GLPnt.x = -1, GLPnt.y = -1 };		//线段起点
CPoint GMPnt(-1, -1);								//鼠标上一状态的点
CFile* GLinTmpDatF = new CFile();					//读取临时数据文件指针对象
CFile* GLinTmpNdxF = new CFile();					//读取临时索引文件指针对象
int GLinNdx = -1;									//找到线位于文件中的位置
CPoint GLinLBDPnt(-1, -1);							//记录鼠标左键按下的位置
CPoint GLinMMPnt(-1, -1);							//记录鼠标移动的上一状态，用于擦除
long GLinMMOffsetX = 0;								//X轴偏移量
long GLinMMOffsetY = 0;								//Y轴偏移量
LIN_NDX_STRU GLinMMTmpNdx;							//记录选中的线的索引
int order = -1;										//延长线时选中线的端点
LIN_NDX_STRU GLinToAdd = GLin;						//需要延长的线
int GnPick = -1;									//延长线的序列
POINT GLPntToAdd = { GLPnt.x = -1, GLPnt.y = -1 };	//延长线段起点
CString getLinNum()
{
	CString output = _T("线：");
	CString first, second;
	first.Format(_T("%d"), GLinLNum);
	second.Format(_T("%d"), GLinNum);
	return output + first + '/' + second;
}
//-----------------------区数据的全局变量
bool GRegFCreated = false;							//临时文件是否创建
CString GRegFName;									//永久文件名
CString GRegTmpNdxFName = CString("tempRegF.ndx");  //临时索引文件名
CString GRegTmpDatFName = CString("tempRegF.dat");  //临时数据文件名
bool GRegChanged = false; 							//是否更改
int GRegNum = 0;									//物理数
int GRegLNum = 0;									//逻辑数
CFile* GRegTmpDatF = new CFile();					//读取临时数据文件指针对象
CFile* GRegTmpNdxF = new CFile();					//读取临时索引文件指针对象
CPoint GRegCreateMMPnt(-1, -1);						//鼠标移动前一状态点
CPoint GRegCreateStartPnt(-1, -1);					//造区的起点
													//索引结构
REG_NDX_STRU GReg = { GReg.isDel = 0,GReg.color = RGB(0,0,0),GReg.pattern = 0,GReg.dotNum = 0,GReg.datOff = 0 };
REG_NDX_STRU GTReg;
int GRegNdx = -1;									//区位于文件中的位置
REG_NDX_STRU GRegMMTmpNdx;							//选中区的索引
CPoint GRegLBDPnt(-1, -1);							//左键按下的位置
CPoint GRegMMPnt(-1, -1);							//记录上一状态，进行擦除
long GRegMMOffsetX = 0;								//x轴偏移量
long GRegMMOffsetY = 0;								//y轴偏移量
CString getRegNum()
{
	CString output = _T("区：");
	CString first, second;
	first.Format(_T("%d"), GRegLNum);
	second.Format(_T("%d"), GRegNum);
	return output + first + '/' + second;
}

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
	OPERSTATE_ZOOM,
	OPERSTATE_WINDOW_MOVE,
	OPERSTATE_LIN_DELETE_PNT,
	OPERSTATE_LIN_ADD_PNT,
	OPERSTATE_LINK_LIN,
	OPERSTATE_MODIFY_POINT_PARAMETER,
	OPERSTATE_MODIFY_LINE_PARAMETER,
	OPERSTATE_MODIFY_REGION_PARAMETER,
	OPERSTATE_UNDELETE_PNT,
	OPERSTATE_UNDELETE_LIN,
	OPERSTATE_UNDELETE_REG,
	OPERSTATE_LIN_DELETE_DOT,
	OPERSTATE_LIN_ADD_DOT,
	OPERSTATE_DELETE_MANY_POINT,
	OPERSTATE_MODIFY_MANY_POINT
};//枚举操作状态
Action GCurOperState;//操作参数
//----------------------------用于放大缩小---------------------------//
zoomData zoom = { (-1,-1),(-1,-1),0,0,1.0,0 };
CPoint GZoomLBDPnt(-1, -1);			//放大时左键的点
CPoint GZoomMMPnt(-1, -1);			//放大前一状态
double GZoomOffset_x = 0;			//偏移向量
double GZoomOffset_y = 0;	
double GZoom = 1.0;					//缩放系数
int GZoomStyle = 0;					//放大方式
//---------------------------窗口移动----------------------------//
CPoint GWinMoveLBDPnt(-1, -1);		//移动时左键按下的点
CPoint GWinMoveMMPnt(-1, -1);		//鼠标移动前状态
									//外包矩形的顶点坐标
double GMaxX = 0;
double GMaxY = 0;
double GMinX = 0;
double GMinY = 0;
//----------------------------连接线-----------------------------//
LIN_NDX_STRU GStartLin = GLin;		//选中的第一条线
int GnStart = -1;
LIN_NDX_STRU GEndLin = GLin;		//选中的第二条线
int GnEnd = -1;
int GnLine = 0;
D_DOT firstPoint = { 0,0 };			//选择的第一个点
D_DOT secondPoint = { 0,0 };		//选择的第二个点
int order1 = -1;					//所选择点的顺序，0代表在开头，1代表在结尾
int order2 = -1;
int isPick = 0;

//---------------------------窗口显示---------------------------//
enum State{ SHOWSTATE_UNDEL, SHOWSTATE_DEL };
State GCurShowState = SHOWSTATE_UNDEL;		//默认显示非删除状态
bool GShowPnt = true;
bool GShowLin = true;
bool GShowReg = true; 

// CMapEditorView

IMPLEMENT_DYNCREATE(CMapEditorView, CView)

BEGIN_MESSAGE_MAP(CMapEditorView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
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
	ON_COMMAND(ID_WINDOW_ZOOM, &CMapEditorView::OnWindowZoom)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_SHOW_POINT, &CMapEditorView::OnUpdateWindowShowPoint)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_SHOW_LINE, &CMapEditorView::OnUpdateWindowShowLine)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_SHOW_REGION, &CMapEditorView::OnUpdateWindowShowRegion)
	ON_UPDATE_COMMAND_UI(ID_POINT_SHOW_DELETED, &CMapEditorView::OnUpdatePointShowDeleted)
	ON_UPDATE_COMMAND_UI(ID_LINE_SHOW_DELETED, &CMapEditorView::OnUpdateLineShowDeleted)
	ON_UPDATE_COMMAND_UI(ID_REGION_SHOW_DELETED, &CMapEditorView::OnUpdateRegionShowDeleted)
	ON_COMMAND(ID_DELETE_MANY_POINT, &CMapEditorView::OnDeleteManyPoint)
	ON_COMMAND(ID_MODIFY_MANY_POINT_PARAMETER, &CMapEditorView::OnModifyManyPointParameter)
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

	if (GShowPnt)
		ShowAllPnt(&dc, GPntTmpF, GPntNum, GZoomOffset_x, GZoomOffset_y, GZoom, GCurShowState);
	if (GShowLin)
		ShowAllLin(&dc, GLinTmpNdxF, GLinTmpDatF, GLinNum, GZoomOffset_x, GZoomOffset_y, GZoom, GCurShowState);
	if (GShowReg)
		ShowAllReg(&dc, GRegTmpNdxF, GRegTmpDatF, GRegNum, GZoomOffset_x, GZoomOffset_y, GZoom, GCurShowState);
	ReleaseDC(&dc);
}

void CMapEditorView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	//-------------------------------线--------------------------------//
	CClientDC dc(this);
	dc.SetROP2(R2_NOTXORPEN);
	if (GLinFCreated)
	{
		switch (GCurOperState)
		{
		case OPERSTATE_INPUT_LIN:
			if (GTLin.dotNum > 1)
			{
				WriteLinNdxToFile(GLinTmpNdxF, GLinNum, GTLin);//写入临时索引文件
				GLinNum++;
				GLinLNum++;
				DrawSeg(&dc, GTLin, GLPnt, point);
				GLin.datOff += (GTLin.dotNum * sizeof(D_DOT));
				memset(&GTLin, 0, sizeof(LIN_NDX_STRU));
				GMPnt.SetPoint(-1, -1);
				GLPnt.x = -1;
				GLPnt.y = -1;
			}
			else if (GTLin.dotNum == 1)
			{
				DrawSeg(&dc, GTLin, GLPnt, point);
				memset(&GTLin, 0, sizeof(LIN_NDX_STRU));
				GMPnt.SetPoint(-1, -1);
				GLPnt.x = -1;
				GLPnt.y = -1;
			}
			break;

		case OPERSTATE_LIN_ADD_DOT:
			switch (isPick)
			{
				case 1:
				{
					DrawSeg(&dc, GLinToAdd, GLPntToAdd, point);
					memset(&GLinToAdd, 0, sizeof(LIN_NDX_STRU));
					GMPnt.SetPoint(-1, -1);
					GLPnt.x = -1;
					GLPnt.y = -1;
					break;
				}
				case 2:
				{
					DrawSeg(&dc, GLinToAdd, GLPntToAdd, point);
					GLin.datOff = (GLinToAdd.dotNum * sizeof(D_DOT));
					memset(&GLinToAdd, 0, sizeof(LIN_NDX_STRU));
					GMPnt.SetPoint(-1, -1);
					GLPntToAdd.x = -1;	//起点
					GLPntToAdd.y = -1;
					memset(&GTLin, 0, sizeof(LIN_NDX_STRU));
					GLPnt.x = -1;
					GLPnt.y = -1;

					isPick = 0;
					GnPick = -1;
					this->Invalidate();
					break;
				}
			}
		}

		//------------------------缩放-----------------------------//
		if (GPntFCreated || GLinFCreated || GRegFCreated)
		{
			RECT client;
			double zoom = 1.0;
			switch (GCurOperState)
			{
			case OPERSTATE_ZOOM:	//缩小
			{
				GetClientRect(&client);
				double x0 = point.x - (client.right / 2.0) - (client.right / 8.0);
				double y0 = point.y - (client.bottom / 2.0) - (client.bottom / 8.0);
				GZoomOffset_x += (x0 / GZoom);
				GZoomOffset_y += (y0 / GZoom);
				GZoom *= 3 / 4.0;
				this->Invalidate();
				break;
			}
			default:
				break;
			}
		}

		//--------------------------造区-----------------------------//
		if (GRegFCreated)
			switch (GCurOperState)
			{
			case OPERSTATE_INPUT_REG:
				if (GTReg.dotNum > 2)
				{
					WriteRegNdxToFile(GRegTmpNdxF, GRegNum, GTReg);
					++GRegNum;
					++GRegLNum;
					POINT* pt = new POINT[3];
					D_DOT dot;
					ReadTempFileToRegDat(GRegTmpDatF, GTReg.datOff, 0, dot);
					PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);
					DotToPnt(pt[0], dot);
					ReadTempFileToRegDat(GRegTmpDatF, GTReg.datOff, GTReg.dotNum - 1, dot);
					PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);
					DotToPnt(pt[1], dot);
					pt[2] = point;
					DrawReg(&dc, GTReg, pt, 3);
					delete[] pt;
					GReg.datOff += (GTReg.dotNum * sizeof(D_DOT));
					memset(&GTReg, 0, sizeof(REG_NDX_STRU));
					GRegCreateMMPnt = CPoint(-1, -1);
					GRegCreateStartPnt = CPoint(-1, -1);
				}
				else if (GTReg.dotNum == 2)
				{
					POINT* pt = new POINT[3];
					D_DOT dot;
					ReadTempFileToRegDat(GRegTmpDatF, GTReg.datOff, 0, dot);
					PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);
					DotToPnt(pt[0], dot);
					ReadTempFileToRegDat(GRegTmpDatF, GTReg.datOff,
						GTReg.dotNum - 1, dot);
					PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);
					DotToPnt(pt[1], dot);
					pt[2] = point;
					DrawReg(&dc, GTReg, pt, 3);
					delete[] pt;
					memset(&GTReg, 0, sizeof(REG_NDX_STRU));
					GRegCreateMMPnt = CPoint(-1, -1);
					GRegCreateStartPnt = CPoint(-1, -1);
				}
				else if (GTReg.dotNum == 1)
				{
					LIN_NDX_STRU tln = { tln.pattern = GTReg.pattern, tln.color = GTReg.color };
					DrawSeg(&dc, tln, GRegCreateStartPnt, GRegCreateMMPnt);
					memset(&GTReg, 0, sizeof(REG_NDX_STRU));
					GRegCreateMMPnt = CPoint(-1, -1);
					GRegCreateStartPnt = CPoint(-1, -1);
				}
				break;
			default:
				break;
			}
	}
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
	str += "已成功创建下列临时文件：\n";
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
			str += "tempLinF.dat tempLinF.ndx\n";
		}
		else
		{
			GLinTmpDatFName = CString("tempLinF.dat");
			GLinTmpNdxFName = CString("tempLinF.ndx");
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
			str += "tempRegF.ndx tempRegF.dat \n";
		}
		else
		{
			GRegTmpNdxFName = CString("tempRegF.ndx");
			GRegTmpDatFName = CString("tempRegF.dat");
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
		if (GPntTmpFName != "tempPntF.dat")
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
}


void CMapEditorView::OnFileOpenLine()
{
	CFileDialog dlg(true);
	dlg.m_ofn.lpstrFilter = L"lin\0*.lin";
	if (GLinChanged == true)
	{
		if (IDYES == AfxMessageBox(L"文件还没有保存，要先保存再继续吗 ", MB_YESNO, MB_ICONQUESTION))
			OnFileSaveLine();
	}
	if (dlg.DoModal() == IDOK)
	{
		GLinFCreated = false;
		int IsCreate = 0;
		GLinFName = dlg.m_ofn.lpstrFile;//永久文件
		CFile* LinF = new CFile();
		if (!LinF->Open(GLinFName, CFile::modeRead | CFile::typeBinary))
		{
			TRACE(_T("文件无法打开 \n"));
			return;
		}
		int nPos = GLinFName.ReverseFind(_T('\\'));
		CString floderAdd = GLinFName.Left(nPos);
		if (GLinTmpDatFName != "tempLinF.dat")
		{
			GLinTmpDatF->SeekToBegin();
			++IsCreate;
		}
		else
		{
			GLinTmpDatFName = floderAdd + CString("\\") + GLinTmpDatFName;
			if (!GLinTmpDatF -> Open(GLinTmpDatFName, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary))
			{
				GLinTmpDatFName = CString("tempLinF.dat");
				TRACE(_T("文件无法打开 \n"));
			}
			else
				++IsCreate;
		}
		if (GLinTmpNdxFName != "tempLinF.ndx")
		{
			GLinTmpNdxF->SeekToBegin();
			++IsCreate;
		}
		else
		{
			GLinTmpNdxFName = floderAdd + CString("\\") + GLinTmpNdxFName;
			if (!GLinTmpNdxF -> Open(GLinTmpNdxFName, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary))
			{
				GLinTmpNdxFName = CString("tempLinF. ndx");
				TRACE(_T("文件无法打开 \n"));
			}
			else
				++IsCreate;
		}
		if (IsCreate == 2)
			GLinFCreated = true;
		ReadLinPermanentFileToTemp(LinF, GLinTmpDatF, GLinTmpNdxF, GLinVer, GLinNum, GLinLNum, GLin.datOff);//读入临时文件
		LinF->Close();
		delete LinF;
		CString windowText = dlg.GetFileName() + "-MapEditor";
		GetParent()->SetWindowTextW(windowText);
		this->InvalidateRect(NULL);	//重绘窗口，进行显示
		GCurOperState = Noaction;
	}
}


void CMapEditorView::OnFileOpenRegion()
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog dlg(true);
	dlg.m_ofn.lpstrFilter = L"reg\0* .reg";
	if (GRegChanged == true)
	{
		if (IDYES == AfxMessageBox(L"文件还没有保存，要先保存吗 ", MB_YESNO, MB_ICONQUESTION))
		{
			OnFileSaveRegion();
		}
	}
	if (dlg.DoModal() == IDOK)
	{
		GRegFCreated = false;
		int IsCreate = 0;
		GRegFName = dlg.m_ofn.lpstrFile;
		CFile* RegF = new CFile();
		if (!RegF->Open(GRegFName, CFile::modeRead | CFile::typeBinary))
		{
			TRACE(_T("无法打开文件 \n"));
			return;
		}
		int nPos = GRegFName.ReverseFind(_T('\\'));
		CString floderAdd = GRegFName.Left(nPos);
		if (GRegTmpDatFName != "tempRegF.dat")
		{
			GRegTmpDatF->SeekToBegin();
			++IsCreate;
		}
		else
		{
			GRegTmpDatFName = floderAdd + CString("\\") + GRegTmpDatFName;
			if (!GRegTmpDatF->Open(GRegTmpDatFName, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary))
			{
				GRegTmpDatFName = CString("tempRegF.dat");
				TRACE(_T("无法打开文件 \n"));
			}
			else
				++IsCreate;
		}
		if (GRegTmpNdxFName != "tempRegF.ndx")
		{
			GRegTmpNdxF->SeekToBegin();
			++IsCreate;
		}
		else
		{
			GRegTmpNdxFName = floderAdd + CString("\\") + GRegTmpNdxFName;
			if (!GRegTmpNdxF->Open(GRegTmpNdxFName, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary))
			{
				GRegTmpNdxFName = CString("tempRegF.ndx");
				TRACE(_T("无法打开文件 \n"));
			}
			else
				++IsCreate;
		}
		if (2 == IsCreate)
			GRegFCreated = true;
		ReadRegPermanentFileToTemp(RegF, GRegTmpDatF, GRegTmpNdxF, GRegVer, GRegNum, GRegLNum, GReg.datOff);
		RegF->Close();
		delete RegF;
		CString windowText = dlg.GetFileName() + "-MapEditor";
		GetParent()->SetWindowTextW(windowText);
		this->InvalidateRect(NULL); //重绘窗口显示
		GCurOperState = Noaction;
	}
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
}


void CMapEditorView::OnFileSaveLine()
{
	if (GLinFCreated)
	{
		CFile* LinF = new CFile();
		if (GLinFName.IsEmpty())
		{
			LPCTSTR lpszFilters;
			lpszFilters = _T("线 （* .lin) |* .lin||");
			CFileDialog dlg(false, _T("lin"), NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, lpszFilters); //保存线对话框
			int nPos = GLinTmpDatFName.ReverseFind(_T('\\'));
			CString folderAdd = GLinTmpDatFName.Left(nPos);
			dlg.m_ofn.lpstrInitialDir = folderAdd;
			if (dlg.DoModal() == IDOK)
				GLinFName = dlg.GetPathName(); //线文件名称
			else
				return;
		}
		else
		{
			LinF->Remove(GLinFName);
		}
		if (!LinF->Open(GLinFName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
		{
			TRACE(_T("无法打开文件 \n"));
			return;
		}
		//写入永久文件
		WriteTempToLinPermanentFile(LinF, GLinTmpDatF, GLinTmpNdxF, GLinVer, GLinNum, GLinLNum);
		LinF->Close();
		delete LinF;
		//线数据无变更
		GLinChanged = false;
		int nPos = GLinFName.ReverseFind(_T('\\') ) ;
			CString windowText = GLinFName.Right(GLinFName.GetLength() - nPos - 1) + " -MapEditor";
		GetParent()->SetWindowTextW(windowText);
	}
	else
	{
		MessageBox(L"还没有创建文件", L"提示", MB_OK);
	}
	GCurOperState = Noaction;
}


void CMapEditorView::OnFileSaveRegion()
{
	// TODO: 在此添加命令处理程序代码
	if (GRegFCreated)
	{ //已经存在临时文件
		CFile* RegF = new CFile();
		if (GRegFName.IsEmpty())
		{
			LPCTSTR lpszFilters;
			lpszFilters = _T("区(*.reg)|*.reg||");
			CFileDialog dlg(false, _T("reg"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, lpszFilters);//保存区的对话框
			int nPos = GRegTmpDatFName.ReverseFind(_T('\\'));
			CString folderAdd = GRegTmpDatFName.Left(nPos);
			dlg.m_ofn.lpstrInitialDir = folderAdd;
			if (dlg.DoModal() == IDOK)
			{
				GRegFName = dlg.GetPathName(); //区文件的名称
			}
			else
			{
				return;
			}
		}
		else
		{
			RegF->Remove(GRegFName);
		}
		if (!RegF->Open(GRegFName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
		{
			TRACE(_T("无法打开文件 \n"));
			return;
		}
		WriteTempToRegPermanentFile(RegF, GRegTmpDatF, GRegTmpNdxF, GRegVer, GRegNum, GRegLNum);//将区的索引和点数据写入永久文件
		RegF->Close();
		delete RegF;
		GRegChanged = false;//线数据无变更
		int nPos = GRegFName.ReverseFind(_T('\\'));
		CString windowText = GRegFName.Right(GRegFName.GetLength() - nPos - 1) + " -MapEditor";
		GetParent()->SetWindowTextW(windowText);
	}
	else
	{
		MessageBox(L"还没有创建文件！", L"提示", MB_OK);
	}
	GCurOperState = Noaction;
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
}


void CMapEditorView::OnFileSaveAsLine()
{
	//暂存文件名
	CString tempFName = GLinFName;
	GLinFName = CString("");
	OnFileSaveLine();
	//若另存失败，还原文件名
	if (GLinFName == "")
		GLinFName = tempFName;
}


void CMapEditorView::OnFileSaveAsRegion()
{
	CString tempFName = GRegFName; //保留原点永久文件名
	GRegFName = CString("");
	OnFileSaveRegion(); 
	if (GRegFName == "") //若另存失败 ，则还原原永久文件名
		GRegFName = tempFName;
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

//放大/缩小
void CMapEditorView::OnWindowZoom()
{
	if (GPntFCreated || GLinFCreated || GRegFCreated)
		GCurOperState = OPERSTATE_ZOOM;
	else
		MessageBox(L"还没有创建临时文件！", L"提示", MB_OK);
}


void CMapEditorView::OnWindowMove()
{
	if (GPntFCreated || GLinFCreated || GRegFCreated)
		GCurOperState = OPERSTATE_WINDOW_MOVE;
	else
		MessageBox(L"还没有创建临时文件！", L"提示", MB_OK);
}


void CMapEditorView::OnWindowReset()
{
	GCurShowState = SHOWSTATE_UNDEL;
	this->Invalidate();
	GShowPnt = true;
	GShowReg = true;
	GShowLin = true;

	//重置偏移量和放大倍数
	GZoomOffset_x = 0;
	GZoomOffset_y = 0;
	GZoom = 1.0;
	//遍历点、线、区的点数据
	D_DOT tempPt;
	PNT_STRU tempPnt;
	LIN_NDX_STRU tempLin;
	REG_NDX_STRU tempReg;
	bool isInit = false;
	if (GPntLNum == 0 && GLinLNum == 0 && GRegLNum == 0)
		return;
	if (isInit == false & &GPntLNum > 0)
	{
		for (int i = 0; i < GPntNum; ++i)
		{
			ReadTempFileToPnt(GPntTmpF, i, tempPnt);
			if (tempPnt.isDel)
				continue;
			else
			{
				GMaxX = tempPnt.x;
				GMinX = tempPnt.x;
				GMaxY = tempPnt.y;
				GMinY = tempPnt.y;
				isInit = true;
				break;
			}
		}
	}
	if (isInit == false && GLinLNum > 0)
	{
		for (int i = 0; i < GLinNum; ++i)
		{
			ReadTempFileToLinNdx(GLinTmpNdxF, i, tempLin);
			if (tempLin.isDel)
				continue;
			else
			{
				for (int j = 0; j < tempLin.dotNum; ++j)
					ReadTempFileToLinDat(GLinTmpDatF, tempLin.datOff, j, tempPt);
				GMaxX = tempPt.x;
				GMinX = tempPt.x;
				GMaxY = tempPt.y;
				GMinY = tempPt.y;
				isInit = true;
				break;
			}
		}
	}
	if (isInit == false && GRegLNum > 0)
	{
		for (int i = 0; i < GRegNum; ++i)
		{
			ReadTempFileToRegNdx(GRegTmpNdxF, i, tempReg);
			if (tempReg.isDel)
				continue;
			else
			{
				for (int j = 0; j < tempReg.dotNum; ++j)
				{
					ReadTempFileToRegDat(GRegTmpDatF, tempReg.datOff, j, tempPt);
					GMaxX; tempPt.x;
					GMinX; tempPt.x;
					GMaxY; tempPt.y;
					GMinY; tempPt.y;
					isInit == true;
					break;
				}
			}
		}
	}
	if (isInit == false)
	{
		this->Invalidate();
		return;
	}
	if (GPntFCreated)
	{
		for (int i=0; i < GPntNum; ++i)
		{
			ReadTempFileToPnt(GPntTmpF, i, tempPnt);
			if (tempPnt.isDel)
				continue;
			else
			{
				if (tempPnt.x > GMaxX)
					GMaxX; tempPnt.x;
				if (tempPnt.y > GMaxY)
					GMaxY; tempPnt.y;
				if (tempPnt.x < GMinX)
					GMinX; tempPnt.x;
				if (tempPnt.y < GMinY)
					GMinY; tempPnt.y;
			}
		}
	}
	if (GLinFCreated)
	{
		for (int i = 0; i < GLinNum; ++i)
		{
			ReadTempFileToLinNdx(GLinTmpNdxF, i, tempLin);
			if (tempLin.isDel)
				continue;
			else
			{
				for (int j = 0; j < tempLin.dotNum; ++j)
				{
					ReadTempFileToLinDat(GLinTmpDatF, tempLin.datOff, j, tempPt);
					if (tempPt.x > GMaxX)
						GMaxX = tempPt.x;
					if (tempPt.y > GMaxY)
						GMaxY = tempPt.y;
					if (tempPt.x < GMinX)
						GMinX = tempPt.x;
					if (tempPt.y < GMinY)
						GMinY = tempPt.y;
				}
			}
		}
	}
	if (GRegFCreated)
	{
		for (int i = 0; i < GRegLNum; ++i)
		{
			ReadTempFileToRegNdx(GRegTmpNdxF, i, tempReg);
			if (tempReg.isDel)
				continue;
			else
			{
				for (int j = 0; j < tempReg.dotNum; ++j)
				{
					ReadTempFileToRegDat(GRegTmpDatF, tempReg.datOff, j, tempPt);
					if (tempPt.x > GMaxX)
						GMaxX = tempPt.x;
					if (tempPt.y > GMaxY)
						GMaxY = tempPt.y;
					if (tempPt.x < GMinX)
						GMinX = tempPt.x;
				}
			}
		}
	}
	GMaxX += 20;
	GMinX -= 20;
	GMaxY += 20;
	GMinY -= 20;
	RECT rect, client;
	if (tempPt.y < GMinY)
	GMinY = tempPt.y;
	double zoom;
	GetClientRect(&client);
	rect.right = (long)GMaxX;
	rect.left = (long)GMinX;
	rect.bottom = (long)GMaxY;
	rect.top = (long)GMinY;
	modulusZoom(client, rect, zoom);
	GMaxX += 20 / zoom;
	GMinX -= 20 / zoom;
	GMaxY += 20 / zoom;
	GMinY -= 20 / zoom;
	rect.right = (long)GMaxX;
	rect.left = (long)GMinX;
	rect.bottom = (long)GMaxY;
	rect.top = (long)GMinY;
	modulusZoom(client, rect, zoom);
	double x0 = GetCenter(rect).x - (client.right / 2.0) + (client.right * (zoom - 1) / (2.0 * zoom));
	double y0 = GetCenter(rect).y - (client.bottom / 2.0) + (client.bottom * (zoom - 1) / (2.0 * zoom));
	GZoomOffset_x += (x0 / GZoom);
	GZoomOffset_y += (y0 / GZoom);
	GZoom *= zoom;
	GCurOperState = Noaction;
	this->Invalidate();
}


void CMapEditorView::OnWindowShowPoint()
{
	if (GCurShowState == SHOWSTATE_DEL)
	{
		GShowPnt = false;
		GShowLin = false;
		GShowReg = false;
	}
	GCurShowState = SHOWSTATE_UNDEL;
	//若已经显示点，则关闭
	if (GShowPnt == true)
		GShowPnt = false;
	else
		GShowPnt = true;
	this->InvalidateRect(NULL);
}

void CMapEditorView::OnUpdateWindowShowPoint(CCmdUI* pCmdUI)
{
	if (GCurShowState == SHOWSTATE_UNDEL && GShowPnt == true)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CMapEditorView::OnUpdatePointShowDeleted(CCmdUI* pCmdUI)
{
	if (GCurShowState == SHOWSTATE_DEL && GShowPnt == true)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CMapEditorView::OnWindowShowLine()
{
	if (GCurShowState == SHOWSTATE_DEL)
	{
		GShowPnt = false;
		GShowLin = false;
		GShowReg = false;
	}
	GCurShowState = SHOWSTATE_UNDEL;
	//若已经显示点，则关闭
	if (GShowLin == true)
		GShowLin = false;
	else
		GShowLin = true;
	this->InvalidateRect(NULL);
}


void CMapEditorView::OnUpdateWindowShowLine(CCmdUI* pCmdUI)
{
	if (GCurShowState == SHOWSTATE_UNDEL && GShowLin == true)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}


void CMapEditorView::OnUpdateLineShowDeleted(CCmdUI* pCmdUI)
{
	if (GCurShowState == SHOWSTATE_DEL && GShowLin == true)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}


void CMapEditorView::OnWindowShowRegion()
{
	if (GCurShowState == SHOWSTATE_DEL)
	{
		GShowPnt = false;
		GShowLin = false;
		GShowReg = false;
	}
	GCurShowState = SHOWSTATE_UNDEL;
	//若已经显示点，则关闭
	if (GShowReg == true)
		GShowReg = false;
	else
		GShowReg = true;
	this->InvalidateRect(NULL);
}


void CMapEditorView::OnUpdateWindowShowRegion(CCmdUI* pCmdUI)
{
	if (GCurShowState == SHOWSTATE_UNDEL && GShowReg == true)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CMapEditorView::OnUpdateRegionShowDeleted(CCmdUI* pCmdUI)
{
	if (GCurShowState == SHOWSTATE_DEL && GShowReg == true)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CMapEditorView::OnPointCreate()
{
	if (GPntFCreated)
		GCurOperState = OPERSTATE_INPUT_PNT;
	else
		MessageBox(L"还没有创建文件！", L"提示", MB_OK);

	GCurShowState = SHOWSTATE_UNDEL; 
	this->Invalidate();
	GShowPnt = true;
	GShowReg = true;
	GShowLin = true;
}


void CMapEditorView::OnPointMove()
{
	if (GPntFCreated)
		GCurOperState = OPERSTATE_MOVE_PNT;
	else
		MessageBox(L"还没有创建文件！", L"提示", MB_OK);

	GCurShowState = SHOWSTATE_UNDEL;
	this->Invalidate();
	GShowPnt = true;
	GShowReg = true;
	GShowLin = true;
}


void CMapEditorView::OnPointDelete()
{
	if (GPntFCreated)
		GCurOperState = OPERSTATE_DELETE_PNT;	//将状态改为删除
	else
		MessageBox(L"还没有创建文件！", L"提示", MB_OK);

	GCurShowState = SHOWSTATE_UNDEL;
	this->Invalidate();
	GShowPnt = true;
	GShowReg = true;
	GShowLin = true;
}

void CMapEditorView::OnDeleteManyPoint()
{
	if (GPntFCreated)
		GCurOperState = OPERSTATE_DELETE_MANY_POINT;	//将状态改为删除
	else
		MessageBox(L"还没有创建文件！", L"提示", MB_OK);

	GCurShowState = SHOWSTATE_UNDEL;
	this->Invalidate();
	GShowPnt = true;
	GShowReg = true;
	GShowLin = true;
}

void CMapEditorView::OnPointShowDeleted()
{
	//不是显示删除，则打开
	if (GCurShowState != SHOWSTATE_DEL)
	{
		GCurShowState = SHOWSTATE_DEL;
		GShowPnt = true;
		GShowLin = false;
		GShowReg = false;
	}
	//是显示删除但是显示的不是点，调整
	else if (GCurShowState == SHOWSTATE_DEL && GShowPnt != true)
	{
		GShowPnt = true;
		GShowLin = false;
		GShowReg = false;
	}
	//显示删除，改为正常显示
	else
	{
		GCurShowState = SHOWSTATE_UNDEL;
		GShowPnt = true;
		GShowLin = true;
		GShowReg = true;
	}
	this->InvalidateRect(NULL);
}


void CMapEditorView::OnPointUndelete()
{
	if (GPntFCreated)
	{
		GCurOperState = OPERSTATE_UNDELETE_PNT;
		GCurShowState = SHOWSTATE_DEL;
		this->Invalidate();
		GShowPnt = true;
		GShowLin = false;
		GShowReg = false;
	}
	else
		MessageBox(L"还没有创建文件 ", L"提示", MB_OK);
}


void CMapEditorView::OnPointModifyParameter()
{
	if (GPntFCreated)
	{
		GCurOperState = OPERSTATE_MODIFY_POINT_PARAMETER;
		GCurShowState = SHOWSTATE_UNDEL;
		GShowPnt = true;
		GShowLin = false;
		GShowReg = false;
		this->Invalidate();
	}
	else
		MessageBox(L"还没有创建文件！ ", L"提示", MB_OK);
}


void CMapEditorView::OnPointSetDefparameter()
{
	CPointParameterDlg dlg;
	dlg.m_Pattern = GPnt.pattern;			//点型
	dlg.m_ColorButton.SetColor(GPnt.color); //颜色
	if (IDOK == dlg.DoModal())
	{
		GPnt.pattern = dlg.m_Pattern;
		COLORREF tempColor = dlg.m_ColorButton.GetColor();
		memcpy_s(&GPnt.color, sizeof(COLORREF), &tempColor, sizeof(COLORREF));
	}
}


void CMapEditorView::OnModifyManyPointParameter()
{

	if (GPntFCreated)
	{
		GCurOperState = OPERSTATE_MODIFY_MANY_POINT;
		GCurShowState = SHOWSTATE_UNDEL;
		GShowPnt = true;
		GShowLin = false;
		GShowReg = false;
		this->Invalidate();
	}
	else
		MessageBox(L"还没有创建文件！ ", L"提示", MB_OK);
}


void CMapEditorView::OnLineCreate()
{
	if (GLinFCreated)
		GCurOperState = OPERSTATE_INPUT_LIN;
	else
		MessageBox(L"还没有创建临时文件！", L"提示", MB_OK);

	GCurShowState = SHOWSTATE_UNDEL;
	this->Invalidate();
	GShowPnt = true;
	GShowReg = true;
	GShowLin = true;
}


void CMapEditorView::OnLineMove()
{
	if (GLinFCreated)
		GCurOperState = OPERSTATE_MOVE_LIN;
	else
		MessageBox(L"还没有创建临时文件！", L"提示", MB_OK);

	GCurShowState = SHOWSTATE_UNDEL;
	this->Invalidate();
	GShowPnt = true;
	GShowReg = true;
	GShowLin = true;
}


void CMapEditorView::OnLineDeleted()
{
	if (GLinFCreated)
		GCurOperState = OPERSTATE_DELETE_LIN;
	else
		MessageBox(L"还没有创建临时文件！", L"提示", MB_OK);

	GCurShowState = SHOWSTATE_UNDEL;
	this->Invalidate();
	GShowPnt = true;
	GShowReg = true;
	GShowLin = true;
}


void CMapEditorView::OnLineShowDeleted()
{
	//不是显示删除，则打开
	if (GCurShowState != SHOWSTATE_DEL)
	{
		GCurShowState = SHOWSTATE_DEL;
		GShowPnt = false;
		GShowLin = true;
		GShowReg = false;
	}
	//是显示删除但是显示的不是线，调整
	else if (GCurShowState == SHOWSTATE_DEL && GShowLin != true)
	{
		GShowPnt = false;
		GShowLin = true;
		GShowReg = false;
	}
	//显示删除，改为正常显示
	else
	{
		GCurShowState = SHOWSTATE_UNDEL;
		GShowPnt = true;
		GShowLin = true;
		GShowReg = true;
	}
	this->InvalidateRect(NULL);
}


void CMapEditorView::OnLineUndetele()
{
	if (GLinFCreated)
	{
		GCurOperState = OPERSTATE_UNDELETE_LIN;
		GCurShowState = SHOWSTATE_DEL;
		this->Invalidate();
		GShowPnt = false;
		GShowLin = true;
		GShowReg = false;
	}
	else
		MessageBox(L"还没有创建文件 ", L"提示", MB_OK);
}


void CMapEditorView::OnLineDeleteDot()
{
	if (GLinFCreated)
		GCurOperState = OPERSTATE_LIN_DELETE_DOT;
	else
		MessageBox(L"还没有创建临时文件！", L"提示", MB_OK);
}


void CMapEditorView::OnLineAddDot()
{
	if (GLinFCreated)
		GCurOperState = OPERSTATE_LIN_ADD_DOT;
	else
		MessageBox(L"还没有创建临时文件！", L"提示", MB_OK);
}


void CMapEditorView::OnLineLink()
{
	if (GLinFCreated)
		GCurOperState = OPERSTATE_LINK_LIN;
	else
		MessageBox(L"还没有创建临时文件！", L"提示", MB_OK);
}


void CMapEditorView::OnLineModifyParameter()
{
	if (GLinFCreated)
	{
		GCurOperState = OPERSTATE_MODIFY_LINE_PARAMETER;
		GCurShowState = SHOWSTATE_UNDEL;
		GShowPnt = false;
		GShowLin = true;
		GShowReg = false;
		this->Invalidate();
	}
	else
		MessageBox(L"还没有创建文件！ ", L"提示", MB_OK);
}


void CMapEditorView::OnLineSetDeparameter()
{
	CLineParameterDlg dlg;
	dlg.m_Pattern = GLin.pattern;			
	dlg.m_ColorButton.SetColor(GLin.color); 
	if (IDOK == dlg.DoModal())
	{
		GLin.pattern = dlg.m_Pattern;
		COLORREF tempColor = dlg.m_ColorButton.GetColor();
		memcpy_s(&GLin.color, sizeof(COLORREF), &tempColor, sizeof(COLORREF));
	}
}

void CMapEditorView::OnRegionCreate()
{
	if (GRegFCreated)
		GCurOperState = OPERSTATE_INPUT_REG; 
	else
		MessageBox(L"还没有创建临时文件 ", L"提示", MB_OK);

	GCurShowState = SHOWSTATE_UNDEL;
	this->Invalidate();
	GShowPnt = true;
	GShowReg = true;
	GShowLin = true;
}


void CMapEditorView::OnRegionMove()
{
	if (GRegFCreated)
		GCurOperState = OPERSTATE_MOVE_REG; 
	else
		MessageBox(L"还没有创建临时文件 ", L"提示", MB_OK);

	GCurShowState = SHOWSTATE_UNDEL;
	this->Invalidate();
	GShowPnt = true;
	GShowReg = true;
	GShowLin = true;
}


void CMapEditorView::OnRegionDelete()
{
	if(GRegFCreated)
		GCurOperState = OPERSTATE_DELETE_REG; 
	else
		MessageBox(L"还没有创建临时文件! ", L"提示", MB_OK);

	GCurShowState = SHOWSTATE_UNDEL;
	this->Invalidate();
	GShowPnt = true;
	GShowReg = true;
	GShowLin = true;
}


void CMapEditorView::OnRegionShowDeleted()
{
	//不是显示删除，则打开
	if (GCurShowState != SHOWSTATE_DEL)
	{
		GCurShowState = SHOWSTATE_DEL;
		GShowPnt = false;
		GShowLin = false;
		GShowReg = true;
	}
	//是显示删除但是显示的不是区，调整
	else if (GCurShowState == SHOWSTATE_DEL && GShowReg != true)
	{
		GShowPnt = false;
		GShowLin = false;
		GShowReg = true;
	}
	//显示删除，改为正常显示
	else
	{
		GCurShowState = SHOWSTATE_UNDEL;
		GShowPnt = true;
		GShowLin = true;
		GShowReg = true;
	}
	this->InvalidateRect(NULL);
}


void CMapEditorView::OnRegionUndelete()
{
	if (GRegFCreated)
	{
		GCurOperState = OPERSTATE_UNDELETE_REG;
		GCurShowState = SHOWSTATE_DEL;
		this->Invalidate();
		GShowPnt = false;
		GShowLin = false;
		GShowReg = true;
	}
	else
		MessageBox(L"还没有创建文件 ", L"提示", MB_OK);
}


void CMapEditorView::OnRegionModifyParameter()
{
	if (GRegFCreated)
	{
		GCurOperState = OPERSTATE_MODIFY_REGION_PARAMETER;
		GCurShowState = SHOWSTATE_UNDEL;
		GShowPnt = false;
		GShowLin = false;
		GShowReg = true;
		this->Invalidate();
	}
	else
		MessageBox(L"还没有创建文件！ ", L"提示", MB_OK);
}


void CMapEditorView::OnRegionSetDefparameter()
{
	CRegionParameterDlg dlg;
	dlg.m_Pattern = GReg.pattern;
	dlg.m_ColorButton.SetColor(GReg.color);
	if (IDOK == dlg.DoModal())
	{
		GReg.pattern = dlg.m_Pattern;
		COLORREF tempColor = dlg.m_ColorButton.GetColor();
		memcpy_s(&GReg.color, sizeof(COLORREF), &tempColor, sizeof(COLORREF));
	}
}


void CMapEditorView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CClientDC dc(this);
	dc.SetROP2(R2_NOTXORPEN);
	D_DOT dot;
	//--------------------------------------点-----------------------------------------//
	if (GPntFCreated)
	{
		switch (GCurOperState)
		{
		//绘制点
		case OPERSTATE_INPUT_PNT:
			PNT_STRU Pnt;
			memcpy_s(&Pnt, sizeof(PNT_STRU), &GPnt, sizeof(PNT_STRU));
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);	//坐标系转化
			Pnt.x = dot.x;
			Pnt.y = dot.y;
			WritePntToFile(GPntTmpF, GPntNum, Pnt); 
			PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);	//坐标系转化
			Pnt.x = dot.x;
			Pnt.y = dot.y;
			DrawPnt(&dc, Pnt); //画点
			GPntNum++;
			GPntLNum++; 
			GPntChanged = true; 
			break;

		//删除点
		case OPERSTATE_DELETE_PNT:
			//坐标系转化
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);
			DotToPnt(point, dot);
			FindPnt(point, GPntNum, GPntTmpF, GPntNdx); 
			//如果找到了最近点
			if (GPntNdx != -1)
			{
				PNT_STRU pnt;
				ReadTempFileToPnt(GPntTmpF, GPntNdx, pnt);	//读点
				pnt.isDel = 1;								//标记删除
				UpdatePnt(GPntTmpF, GPntNdx, pnt); 
				dot.x = pnt.x;
				dot.y = pnt.y;
				PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y); 
				pnt.x = dot.x;
				pnt.y = dot.y;
				DrawPnt(&dc, pnt);							//重绘以清除
				GPntNdx = -1;
				GPntChanged = true; 
				GPntLNum--; 
			}
			break;

		case OPERSTATE_DELETE_MANY_POINT:
		{
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);
			DotToPnt(point, dot);
			double minX = min(point.x, GDeleteLBDPnt.x);
			double minY = min(point.y, GDeleteLBDPnt.y);
			double maxX = max(point.x, GDeleteLBDPnt.x);
			double maxY = max(point.y, GDeleteLBDPnt.y);
			PNT_STRU _point;
			for (int i = 0; i < GPntNum; i++)
			{
				ReadTempFileToPnt(GPntTmpF, i, _point);
				if (_point.isDel == 1)
					continue;
				if (_point.x<maxX && _point.x>minX && _point.y > minY && _point.y < maxY)
				{
					_point.isDel = 1;								//标记删除
					UpdatePnt(GPntTmpF, i, _point);
					dot.x = _point.x;
					dot.y = _point.y;
					PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);
					_point.x = dot.x;
					_point.y = dot.y;
					DrawPnt(&dc, _point);							//重绘以清除
					GPntNdx = -1;
					GPntChanged = true;
					GPntLNum--;
				}
			}
			GDeleteLBDPnt = CPoint(-1, -1);
			GDeleteMMPnt = CPoint(-1, -1);
			this->Invalidate();
			break;
		}

		case OPERSTATE_MODIFY_MANY_POINT:
		{
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);
			DotToPnt(point, dot);
			double minX = min(point.x, GModifyLBDPnt.x);
			double minY = min(point.y, GModifyLBDPnt.y);
			double maxX = max(point.x, GModifyLBDPnt.x);
			double maxY = max(point.y, GModifyLBDPnt.y);

			PNT_STRU tempPoint = GPnt;
			CPointParameterDlg dlg;
			dlg.m_ColorButton.SetColor(tempPoint.color);
			dlg.m_Pattern = tempPoint.pattern;
			if (IDOK == dlg.DoModal())
			{
				COLORREF tempColor = dlg.m_ColorButton.GetColor();
				for (int i = 0; i < GPntNum; i++)
				{
					ReadTempFileToPnt(GPntTmpF, i, tempPoint);
					if (tempPoint.isDel == 1)
						continue;
					if (tempPoint.x<maxX && tempPoint.x>minX && tempPoint.y > minY && tempPoint.y < maxY)
					{
						memcpy_s(&tempPoint.color, sizeof(COLORREF), &tempColor, sizeof(COLORREF));
						tempPoint.pattern = dlg.m_Pattern;
						GPntTmpF->Seek(i * sizeof(PNT_STRU), CFile::begin);
						GPntTmpF->Write(&tempPoint, sizeof(PNT_STRU));
					}
				}
			}

			GModifyLBDPnt = CPoint(-1, -1);
			GModifyMMPnt = CPoint(-1, -1);
			this->Invalidate();
			break;
		}

		//移动点
		case OPERSTATE_MOVE_PNT:
		{
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
		}

		//恢复点
		case OPERSTATE_UNDELETE_PNT : 
		{
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);
			DotToPnt(point, dot);
			FindDeletePnt(point, GPntNum, GPntTmpF, GPntNdx); //找到最近的删除点
			if (GPntNdx != -1)
			{
				PNT_STRU pnt;
				ReadTempFileToPnt(GPntTmpF, GPntNdx, pnt);
				pnt.isDel = 0;
				UpdatePnt(GPntTmpF, GPntNdx, pnt);
				dot.x = pnt.x;
				dot.y = pnt.y;
				PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);
				pnt.x = dot.x;
				pnt.y = dot.y;
				DrawPnt(&dc, pnt);
				GPntChanged = true;
				GPntNdx = -1;
				GPntLNum++;
			}
			break;
		}

		//修改点参数
		case OPERSTATE_MODIFY_POINT_PARAMETER: 
				PntToDot(dot, point);
				PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);
				DotToPnt(point, dot);
				PNT_STRU tempPoint;
				memcpy_s(&tempPoint, sizeof(PNT_STRU), &FindPnt(point, GPntNum, GPntTmpF, GPntNdx), sizeof(PNT_STRU));
				if ( GPntNdx != -1 )
				{
				CPointParameterDlg dlg;
				dlg.m_ColorButton.SetColor (tempPoint.color);
				dlg.m_Pattern = tempPoint.pattern;
				if (IDOK == dlg.DoModal())
				{
					COLORREF tempColor = dlg.m_ColorButton.GetColor( );
					memcpy_s(&tempPoint.color, sizeof(COLORREF) ,&tempColor ,sizeof(COLORREF));
					tempPoint.pattern = dlg.m_Pattern;
					GPntTmpF->Seek(GPntNdx * sizeof(PNT_STRU) , CFile::begin);
					GPntTmpF->Write(&tempPoint, sizeof(PNT_STRU));
				}
				this->Invalidate();
				GPntChanged = true;
				GPntNdx = -1;
				}
				break;

		default:
			break;
		}
	}

	//-------------------------------------线----------------------------------------//
	if (GLinFCreated)
	{ 
		D_DOT dot;
		switch (GCurOperState)
		{
		//绘制线
		case OPERSTATE_INPUT_LIN:
			if (GTLin.dotNum == 0)
				memcpy_s(&GTLin, sizeof(LIN_NDX_STRU), &GLin, sizeof(LIN_NDX_STRU));
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y); 
			WriteLinDatToFile(GLinTmpDatF, GLin .datOff, GTLin .dotNum, dot);
			GTLin.dotNum++;
			PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y); 
			GLPnt.x = (long)dot.x;	//起点
			GLPnt.y = (long)dot.y;
			GLinChanged = true; 
			break;

		//删除线
		case OPERSTATE_DELETE_LIN: 
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y); 
			DotToPnt(point, dot);
			FindLin(GLinTmpNdxF, GLinTmpDatF, point, GLinNum, GLinNdx); 
			if (GLinNdx != -1)
			{
				GLinLNum--;
				GLinChanged = true;
				LIN_NDX_STRU TmpLinNdx;
				D_DOT dot1, dot2;
				POINT pnt1, pnt2;
				ReadTempFileToLinNdx(GLinTmpNdxF, GLinNdx, TmpLinNdx);
				TmpLinNdx.isDel = 1;
				UpdateLin(GLinTmpNdxF, GLinNdx, TmpLinNdx);
				for (int i = 0; i < TmpLinNdx.dotNum - 1; ++i)
				{
					ReadTempFileToLinDat(GLinTmpDatF, TmpLinNdx.datOff, i, dot1);
					ReadTempFileToLinDat(GLinTmpDatF, TmpLinNdx.datOff, i + 1, dot2);
					PntDPtoVP(dot1, GZoom, GZoomOffset_x, GZoomOffset_y);
					PntDPtoVP(dot2, GZoom, GZoomOffset_x, GZoomOffset_y);
					DotToPnt(pnt1, dot1);
					DotToPnt(pnt2, dot2);
					DrawSeg(&dc, TmpLinNdx, pnt1, pnt2);
				}
				GLinNdx = -1;
			}
			break;

			//移动线
		case OPERSTATE_MOVE_LIN:
			if (GLinNdx!= -1)
				if (GLinLBDPnt.x != -1 && GLinLBDPnt.y != -1)
				{
					D_DOT dot1, dot2;
					PntToDot(dot1, point);
					PntToDot(dot2, GLinLBDPnt);
					PntDPtoVP(dot1, GZoom, GZoomOffset_x, GZoomOffset_y);
					PntDPtoVP(dot2, GZoom, GZoomOffset_x, GZoomOffset_y);
					double offset_x = dot1.x - dot2.x;
					double offset_y = dot1.y - dot2.y;
					UpdateLin(GLinTmpNdxF, GLinTmpDatF, GLinNdx, offset_x, offset_y);
					GLinNdx = -1;
					GLinMMOffsetX = 0;
					GLinMMOffsetY = 0;
					GLinChanged = true;
				}
			break;

		//连接线
		case OPERSTATE_LINK_LIN : 
			if (GnLine < 2)
			{
				LIN_NDX_STRU line;
				D_DOT dot;
				PntToDot(dot, point);
				PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);
				DotToPnt(point, dot);
				line = FindLin(GLinTmpNdxF, GLinTmpDatF, point, GLinNum, GLinNdx);
				if (GLinNdx != -1)
				{
					GnLine++;
					if (GnLine == 1)
					{
						GStartLin = line;
						GnStart = GLinNdx;
					}
					else if (GnLine == 2)
					{
						if (GnStart != GLinNdx)
						{
							GEndLin = line;
							GnEnd = GLinNdx;
						}
						else
						GnLine--;
					}
				}
				if (GnLine != 0)
				{
					D_DOT pt1;
					D_DOT pt2;
					if (GnLine == 1)
					{	//选中第一个端点
						ReadTempFileToLinDat(GLinTmpDatF, GStartLin.datOff, 0, pt1);
						ReadTempFileToLinDat(GLinTmpDatF, GStartLin.datOff, GStartLin.dotNum - 1, pt2);
						if (Distance(point.x, point.y, pt1.x, pt1.y) < Distance(point.x, point.y, pt2.x, pt2.y))
						{
							PntDPtoVP(pt1, GZoom, GZoomOffset_x, GZoomOffset_y);
							dc.Ellipse((long)pt1.x - 2, (long)pt1.y - 2, (long)pt1.x + 2, (long)pt1.y + 2);
							firstPoint = pt1;
							order1 = 0;
						}
						else
						{
							PntDPtoVP(pt2, GZoom, GZoomOffset_x, GZoomOffset_y);
							dc.Ellipse((long)pt2.x - 2, (long)pt2.y - 2, (long)pt2.x + 2, (long)pt2.y + 2);
							firstPoint = pt2;
							order1 = 1;
						}
					}
					else if (GnLine == 2)
					{	//选中第二个端点
						ReadTempFileToLinDat(GLinTmpDatF, GEndLin.datOff, 0, pt1);
						ReadTempFileToLinDat(GLinTmpDatF, GEndLin.datOff, GEndLin.dotNum - 1, pt2);
						if (Distance(point.x, point.y, pt1.x, pt1.y) < Distance(point.x, point.y, pt2.x, pt2.y))
						{
							PntDPtoVP(pt1, GZoom, GZoomOffset_x, GZoomOffset_y);
							secondPoint = pt1;
							order2 = 0;
						}
						else
						{
							PntDPtoVP(pt2, GZoom, GZoomOffset_x, GZoomOffset_y);
							secondPoint = pt2;
							order2 = 1;
						}
						//开始连接
						AlterLindot(GLinTmpDatF, GStartLin, GEndLin, firstPoint, secondPoint, GLin.datOff, order1, order2);
						AlterStartLin(GLinTmpNdxF, GLin.datOff, GnStart, GStartLin.dotNum + GEndLin.dotNum);
						AlterEndLin(GLinTmpNdxF, GnEnd);

						GLin.datOff += (GStartLin.dotNum + GEndLin.dotNum) * sizeof(D_DOT);
						order1 = -1;
						order2 = -1;
						GnLine = 0;
						GLinNum--;
						GLinLNum--;
						GLinChanged = true;
						GnStart = -1;
						GnEnd = -1;
						GLinNdx = -1;
						GStartLin = GLin;
						GEndLin = GLin;
						firstPoint = { 0,0 };
						secondPoint = { 0,0 };
						this->Invalidate();
					}
				}
			}
			break;

		//恢复线
		case OPERSTATE_UNDELETE_LIN:
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);
			DotToPnt(point, dot);
			FindDeleteLin(GLinTmpNdxF, GLinTmpDatF, point, GLinNum, GLinNdx);
			if (GLinNdx != -1)
			{
				GLinChanged = true;
				LIN_NDX_STRU TmpLinNdx;
				D_DOT dot1, dot2;
				POINT pnt1, pnt2;
				ReadTempFileToLinNdx(GLinTmpNdxF, GLinNdx, TmpLinNdx);
				TmpLinNdx.isDel = 0;
				UpdateLin(GLinTmpNdxF, GLinNdx, TmpLinNdx);
				for (int i = 0; i < TmpLinNdx.dotNum - 1; ++i)
				{
					ReadTempFileToLinDat(GLinTmpDatF, TmpLinNdx.datOff, i, dot1);
					ReadTempFileToLinDat(GLinTmpDatF, TmpLinNdx.datOff, i + 1, dot2);
					PntDPtoVP(dot1, GZoom, GZoomOffset_x, GZoomOffset_y);
					PntDPtoVP(dot2, GZoom, GZoomOffset_x, GZoomOffset_y);
					DotToPnt(pnt1, dot1);
					DotToPnt(pnt2, dot2);
					DrawSeg(&dc, TmpLinNdx, pnt1, pnt2);
				}
				GLinNdx = -1;
				GLinLNum++;
			}
			break;

		//修改线参数
		case OPERSTATE_MODIFY_LINE_PARAMETER:
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);
			DotToPnt(point, dot);
			LIN_NDX_STRU tempLine;
			memcpy_s(&tempLine, sizeof(LIN_NDX_STRU), &FindLin(GLinTmpNdxF, GLinTmpDatF, point, GLinNum, GLinNdx), sizeof(LIN_NDX_STRU));
			if (GLinNdx != -1)
			{
				CLineParameterDlg dlg;
				dlg.m_ColorButton.SetColor(tempLine.color);
				dlg.m_Pattern = tempLine.pattern;
				if (IDOK == dlg.DoModal())
				{
					COLORREF tempColor = dlg.m_ColorButton.GetColor();
					memcpy_s(&tempLine.color, sizeof(COLORREF), &tempColor, sizeof(COLORREF));
					tempLine.pattern = dlg.m_Pattern;
					GLinTmpNdxF->Seek(GLinNdx * sizeof(LIN_NDX_STRU), CFile::begin);
					GLinTmpNdxF->Write(&tempLine, sizeof(LIN_NDX_STRU));
				}
				this->Invalidate();
				GLinChanged = true;
				GLinNdx = -1;
			}
			break;

		//延长线
		case OPERSTATE_LIN_ADD_DOT:
			switch(isPick)
			{
			case 0:
				{
					PntToDot(dot, point);
					PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);
					DotToPnt(point, dot);
					GLinToAdd = FindLin(GLinTmpNdxF, GLinTmpDatF, point, GLinNum, GLinNdx);
					GnPick = GLinNdx;
					D_DOT pt1;
					D_DOT pt2;
					if (GLinNdx != -1)
						isPick = 1;
					//选中端点
					ReadTempFileToLinDat(GLinTmpDatF, GLinToAdd.datOff, 0, pt1);
					ReadTempFileToLinDat(GLinTmpDatF, GLinToAdd.datOff, GLinToAdd.dotNum - 1, pt2);
					if (Distance(point.x, point.y, pt1.x, pt1.y) < Distance(point.x, point.y, pt2.x, pt2.y))
					{
						PntDPtoVP(pt1, GZoom, GZoomOffset_x, GZoomOffset_y);
						dc.Ellipse((long)pt1.x - 2, (long)pt1.y - 2, (long)pt1.x + 2, (long)pt1.y + 2);
						order1 = 0;
						GLPntToAdd.x = (long)pt1.x;
						GLPntToAdd.y = (long)pt1.y;
					}
					else
					{
						PntDPtoVP(pt2, GZoom, GZoomOffset_x, GZoomOffset_y);
						dc.Ellipse((long)pt2.x - 2, (long)pt2.y - 2, (long)pt2.x + 2, (long)pt2.y + 2);
						order1 = 1;
						GLPntToAdd.x = (long)pt2.x;
						GLPntToAdd.y = (long)pt2.y;
					}
				}
				break;

			case 1:
			{
				PntToDot(dot, point);
				PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);
				PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);

				D_DOT _point;
				int ndot = 0;
				if (order == 0)
				{
					for (int i = GLinToAdd.dotNum - 1; i >= 0; --i)
					{
						ReadTempFileToLinDat(GLinTmpDatF, GLinToAdd.datOff, i, _point);
						WriteLinDatToFile(GLinTmpDatF, GLinToAdd.datOff, ndot, _point);
						ndot++;
					}
				}
				else if (order == 1)
				{
					for (int i = 0; i < GLinToAdd.dotNum; ++i)
					{
						ReadTempFileToLinDat(GLinTmpDatF, GLinToAdd.datOff, i, _point);
						WriteLinDatToFile(GLinTmpDatF, GLinToAdd.datOff, ndot, _point);
						ndot++;
					}
				}
				GLPntToAdd.x = point.x;
				GLPntToAdd.y = point.y;
				isPick = 2;

				this->Invalidate();
			}

			case 2:
			{
				LIN_NDX_STRU LinNdx;
				GLinTmpNdxF->Seek(GnPick * sizeof(LIN_NDX_STRU), CFile::begin);
				GLinTmpNdxF->Read(&LinNdx, sizeof(LIN_NDX_STRU));
				LinNdx.datOff = GnPick;						//更新点索引
				LinNdx.dotNum = GLinToAdd.dotNum + 1;		//更新点书目
				GLinTmpNdxF->Seek(GnPick * sizeof(LIN_NDX_STRU), CFile::begin);
				GLinTmpNdxF->Write(&LinNdx, sizeof(LIN_NDX_STRU));

				PntToDot(dot, point);
				PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);
				WriteLinDatToFile(GLinTmpDatF, GLinToAdd.datOff, GLinToAdd.dotNum, dot);
				GLinToAdd.dotNum++;
				PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);
				GLPntToAdd.x = (long)dot.x;	//起点
				GLPntToAdd.y = (long)dot.y;
				GLinChanged = true;
				break;
			}

			default:
				break;
			}
			break;
		default:
			break;
		}
	}

	//---------------------------------------缩放-------------------------------------------//
	if (GPntFCreated || GLinFCreated || GRegFCreated)
	{
		RECT client, rect;
		double zoom = 1.0;
		switch (GCurOperState)
		{
			case OPERSTATE_ZOOM : //放大
				GetClientRect(&client);
				//0是单击放大，1是拉框放大
				if (abs(GZoomLBDPnt.x - GZoomMMPnt.x) <= 10 && abs(GZoomLBDPnt.y-GZoomMMPnt.y) <= 10)
					GZoomStyle = 0;		
				else
					GZoomStyle = 1; 

				if (GZoomStyle == 0)	//单击
				{
					double x0 = point.x - (client.right / 2.0) + (client.right / 8.0);
					double y0 = point.y - (client.bottom / 2.0) + (client.bottom / 8.0);
					GZoomOffset_x += (x0 / GZoom);
					GZoomOffset_y += (y0 / GZoom);
					GZoom *= 4 / 3.0;
				}
				else					//拉框
				{
					rect.right = max(point.x, GZoomLBDPnt.x);
					rect.left = min(point.x, GZoomLBDPnt.x);
					rect.bottom = max(point.y, GZoomLBDPnt.y);
					rect.top = min(point.y, GZoomLBDPnt.y);
					modulusZoom(client, rect, zoom);
					double x0 = GetCenter(rect).x - (client.right / 2.0) + (client.right *(zoom - 1) / (2.0 * zoom));
					double y0 = GetCenter(rect).y - (client.bottom / 2.0) + (client.bottom * (zoom - 1) / (2.0 * zoom));
					GZoomOffset_x += (x0 / GZoom); 
					GZoomOffset_y += (y0 / GZoom);
					GZoom *= zoom;
					GZoomStyle = 0;
				}
				GZoomLBDPnt = CPoint(-1, -1);
				GZoomMMPnt = CPoint(-1, -1);
				this->Invalidate();
				break;

			case OPERSTATE_WINDOW_MOVE : 
				//复位
				GWinMoveLBDPnt = CPoint(-1, -1);
				GWinMoveMMPnt = CPoint(-1, -1); 
				break;
			default:
				break;
		}
	}

	//----------------------------------------区----------------------------------------//
	if (GRegFCreated)
	{
		D_DOT dot;
		switch (GCurOperState)
		{
			case OPERSTATE_INPUT_REG :
				if (GTReg.dotNum == 0)
					memcpy_s(&GTReg, sizeof(REG_NDX_STRU), &GReg, sizeof(REG_NDX_STRU));
				if (GRegCreateStartPnt.x == -1 && GRegCreateStartPnt.y == -1)
					GRegCreateStartPnt = point;
				if (GRegCreateMMPnt.x == -1 && GRegCreateMMPnt.y == -1)
					GRegCreateMMPnt = point;
				PntToDot(dot, point);
				PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);
				WriteRegDatToFile(GRegTmpDatF, GReg.datOff, GTReg.dotNum, dot);
				++GTReg.dotNum;
				if (GTReg.dotNum == 2)
					this->Invalidate();	//区节点小于三个取消造区
				GRegChanged = true;
				break;

			case OPERSTATE_DELETE_REG :
				PntToDot(dot, point);
				PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y); 
				DotToPnt(point, dot);
				FindReg(GRegTmpNdxF, GRegTmpDatF, point, GRegNum, GRegNdx);
				if(GRegNdx != -1)
				{ 
					GRegLNum--;
					GRegChanged = true;
					REG_NDX_STRU TmpRegNdx;
					ReadTempFileToRegNdx(GRegTmpNdxF, GRegNdx, TmpRegNdx); 
					TmpRegNdx.isDel = 1;
					UpdateReg(GRegTmpNdxF, GRegNdx, TmpRegNdx); //更新数据
					D_DOT* dot = new D_DOT[TmpRegNdx.dotNum];
					GRegTmpDatF->Seek(TmpRegNdx.datOff, CFile::begin);
					GRegTmpDatF->Read(dot, TmpRegNdx.dotNum * sizeof(D_DOT));
					for (int i = 0; i < TmpRegNdx.dotNum; ++i)
					{
						PntDPtoVP(dot[i], GZoom, GZoomOffset_x, GZoomOffset_y);
					}
					POINT* pnt = new POINT[TmpRegNdx.dotNum];
					DotToPnt(pnt, dot, TmpRegNdx.dotNum);
					DrawReg(&dc, TmpRegNdx, pnt, TmpRegNdx.dotNum);	//重绘以擦除
					delete[] pnt;
					delete[] dot;
					GRegNdx = -1;
				}
				break;

			case OPERSTATE_MOVE_REG:
				if (GRegNdx != -1)
				{
					if (GRegLBDPnt.x != -1 && GRegLBDPnt.y != -1)
					{
						D_DOT dot1, dot2;
						PntToDot(dot1, point);
						PntVPtoDP(dot1, GZoom, GZoomOffset_x, GZoomOffset_y);
						PntToDot(dot2, GRegLBDPnt);
						PntVPtoDP(dot2, GZoom, GZoomOffset_x, GZoomOffset_y);
						double offset_x = dot1.x - dot2.x;
						double offset_y = dot1.y - dot2.y;
						UpdateReg(GRegTmpNdxF, GRegTmpDatF, GRegNdx, offset_x, offset_y);
						GRegNdx = -1;
						GRegMMOffsetX = 0;
						GRegMMOffsetY = 0;
						GRegChanged = true;
					}
				}
				break;

			case OPERSTATE_UNDELETE_REG:
				PntToDot(dot, point);
				PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);
				DotToPnt(point, dot);
				FindDeleteReg(GRegTmpNdxF, GRegTmpDatF, point, GRegNum, GRegNdx);
				if (GRegNdx != -1)
				{
					GRegChanged = true;
					REG_NDX_STRU TmpRegNdx;
					ReadTempFileToRegNdx(GRegTmpNdxF, GRegNdx, TmpRegNdx);
					TmpRegNdx.isDel = 0;
					UpdateReg(GRegTmpNdxF, GRegNdx, TmpRegNdx); //更新数据
					D_DOT* dot = new D_DOT[TmpRegNdx.dotNum];
					GRegTmpDatF->Seek(TmpRegNdx.datOff, CFile::begin);
					GRegTmpDatF->Read(dot, TmpRegNdx.dotNum * sizeof(D_DOT));
					for (int i = 0; i < TmpRegNdx.dotNum; ++i)
					{
						PntDPtoVP(dot[i], GZoom, GZoomOffset_x, GZoomOffset_y);
					}
					POINT* pnt = new POINT[TmpRegNdx.dotNum];
					DotToPnt(pnt, dot, TmpRegNdx.dotNum);
					DrawReg(&dc, TmpRegNdx, pnt, TmpRegNdx.dotNum);	//重绘以擦除
					delete[] pnt;
					delete[] dot;
					GRegNdx = -1;
					GRegLNum++;
				}
				break;

			//修改区参数
			case OPERSTATE_MODIFY_REGION_PARAMETER:
				PntToDot(dot, point);
				PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);
				DotToPnt(point, dot);
				REG_NDX_STRU tempRegion;
				memcpy_s(&tempRegion, sizeof(REG_NDX_STRU), &FindReg(GRegTmpNdxF, GRegTmpDatF, point, GRegNum, GRegNdx), sizeof(REG_NDX_STRU));
				if (GRegNdx != -1)
				{
					CRegionParameterDlg dlg;
					dlg.m_ColorButton.SetColor(tempRegion.color);
					dlg.m_Pattern = tempRegion.pattern;
					if (IDOK == dlg.DoModal())
					{
						COLORREF tempColor = dlg.m_ColorButton.GetColor();
						memcpy_s(&tempRegion.color, sizeof(COLORREF), &tempColor, sizeof(COLORREF));
						tempRegion.pattern = dlg.m_Pattern;
						GRegTmpNdxF->Seek(GRegNdx * sizeof(REG_NDX_STRU), CFile::begin);
						GRegTmpNdxF->Write(&tempRegion, sizeof(REG_NDX_STRU));
					}
					this->Invalidate();
					GRegChanged = true;
					GRegNdx = -1;
				}
				break;
		}
	}
	CView::OnLButtonUp(nFlags, point);
}


void CMapEditorView::OnLButtonDown(UINT nFlags, CPoint point)
{
	//-------------------------------点--------------------------------//
	if (GPntFCreated)
		switch (GCurOperState)
		{
		case OPERSTATE_MOVE_PNT:	//如果需要移动
			GTPnt = FindPnt(point, GPntNum, GPntTmpF, GPntNdx);
			break;
		case OPERSTATE_DELETE_MANY_POINT:
			GDeleteLBDPnt = point;
			GDeleteMMPnt = point;
			break;
		case OPERSTATE_MODIFY_MANY_POINT:
			GModifyLBDPnt = point;
			GModifyMMPnt = point;
			break;
		default:
			break;
		}
	//-------------------------------线--------------------------------//
	if (GLinFCreated)
	{
		switch (GCurOperState)
		{
		case OPERSTATE_MOVE_LIN: 
			D_DOT dot;
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);
			DotToPnt(point, dot);
			GLinMMTmpNdx = FindLin(GLinTmpNdxF, GLinTmpDatF, point, GLinNum, GLinNdx);
			GLinMMOffsetX = 0;
			GLinMMOffsetY = 0;
			PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);
			DotToPnt(point, dot);
			GLinLBDPnt = point;
			GLinMMPnt = point;
			break;
		default:
			break;
		}
	}
	//------------------------------缩放和移动------------------------------//
	if (GPntFCreated || GLinFCreated || GRegFCreated)
	{
		switch (GCurOperState)
		{
		case OPERSTATE_ZOOM: //放大
			GZoomLBDPnt = point;
			GZoomMMPnt = point;
			break;
		case OPERSTATE_WINDOW_MOVE:
			GWinMoveLBDPnt = point;
			GWinMoveMMPnt = point;
			break;
		default:
			break;
		}
	}

	//-------------------------------区-----------------------------//
	if (GRegFCreated)
	{
		switch (GCurOperState)
		{
		case OPERSTATE_MOVE_REG :
				GRegLBDPnt = point;
				GRegMMPnt = point;
				D_DOT dot;
				PntToDot(dot, point);
				PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);
				DotToPnt(point, dot);
				GRegMMTmpNdx = FindReg(GRegTmpNdxF, GRegTmpDatF, point, GRegNum, GRegNdx); 
				GRegMMOffsetX = 0;
				GRegMMOffsetY = 0;
				break;
		}
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
	//--------------------------------点----------------------------------//
	if (GPntFCreated)
	{
		switch (GCurOperState)
		{
		case OPERSTATE_MOVE_PNT:
		{
			if (GPntNdx != -1)
			{
				CClientDC dc(this);
				dc.SetROP2(R2_NOTXORPEN);
				DrawPnt(&dc, GTPnt);
				GTPnt.x = point.x;
				GTPnt.y = point.y;
				DrawPnt(&dc, GTPnt);
			}
			break;
		}
		case OPERSTATE_DELETE_MANY_POINT:
		{
			if (GDeleteMMPnt.x != -1 && GDeleteMMPnt.y != -1)
			{
				CClientDC dc(this);
				CPen pen(PS_DOT, 1, RGB(0, 0, 0));
				CPen* oldPen = dc.SelectObject(&pen);
				dc.SetROP2(R2_NOTXORPEN);
				dc.Rectangle(GDeleteLBDPnt.x, GDeleteLBDPnt.y, GDeleteMMPnt.x, GDeleteMMPnt.y);
				dc.Rectangle(GDeleteLBDPnt.x, GDeleteLBDPnt.y, point.x, point.y);
				GDeleteMMPnt = point;
				dc.SelectObject(oldPen);
			}
			break;
		}

		case OPERSTATE_MODIFY_MANY_POINT:
		{
			if (GModifyMMPnt.x != -1 && GModifyMMPnt.y != -1)
			{
				CClientDC dc(this);
				CPen pen(PS_DOT, 1, RGB(0, 0, 0));
				CPen* oldPen = dc.SelectObject(&pen);
				dc.SetROP2(R2_NOTXORPEN);
				dc.Rectangle(GModifyLBDPnt.x, GModifyLBDPnt.y, GModifyMMPnt.x, GModifyMMPnt.y);
				dc.Rectangle(GModifyLBDPnt.x, GModifyLBDPnt.y, point.x, point.y);
				GModifyMMPnt = point;
				dc.SelectObject(oldPen);
			}
			break;
		}

		default:
			break;
		}
	}
	//---------------------------------线----------------------------------//
	if (GLinFCreated)
	{
		switch (GCurOperState)
		{
		case OPERSTATE_INPUT_LIN:
			if (GTLin.dotNum > 0)
			{
				CClientDC dc(this);
				dc.SetROP2(R2_NOTXORPEN);
				if (GMPnt.x != -1 && GMPnt.y != -1)
					DrawSeg(&dc, GTLin, GLPnt, GMPnt);
				//保存上一状态
				GMPnt.x = point.x;
				GMPnt.y = point.y;
				POINT mpoint = { mpoint.x = point.x,mpoint.y = point.y };
				DrawSeg(&dc, GTLin, GLPnt, mpoint);//连接
			}
			break;

		case OPERSTATE_MOVE_LIN:
			if (GLinNdx != -1)
			{
				CClientDC dc(this);
				dc.SetROP2(R2_NOTXORPEN);
				D_DOT dot1, dot2;
				POINT pnt1, pnt2;
				//擦除原来的线
				for (int i = 0; i < GLinMMTmpNdx.dotNum - 1; i++)
				{
					ReadTempFileToLinDat(GLinTmpDatF, GLinMMTmpNdx.datOff, i, dot1);
					ReadTempFileToLinDat(GLinTmpDatF, GLinMMTmpNdx.datOff, i + 1, dot2);
					//坐标系转化
					PntDPtoVP(dot1, GZoom, GZoomOffset_x, GZoomOffset_y);
					PntDPtoVP(dot2, GZoom, GZoomOffset_x, GZoomOffset_y);
					DotToPnt(pnt1, dot1);
					DotToPnt(pnt2, dot2);
					pnt1.x += GLinMMOffsetX;
					pnt1.y += GLinMMOffsetY;
					pnt2.x += GLinMMOffsetX;
					pnt2.y += GLinMMOffsetY;
					DrawSeg(&dc, GLinMMTmpNdx, pnt1, pnt2);
				}
				//计算偏移量
				GLinMMOffsetX = GLinMMOffsetX + point.x - GLinMMPnt.x;
				GLinMMOffsetY = GLinMMOffsetY + point.y - GLinMMPnt.y;
				//在新的位置绘制新的线段
				for (int i = 0; i < GLinMMTmpNdx.dotNum - 1; i++)
				{
					ReadTempFileToLinDat(GLinTmpDatF, GLinMMTmpNdx.datOff, i, dot1);
					ReadTempFileToLinDat(GLinTmpDatF, GLinMMTmpNdx.datOff, i + 1, dot2);
					PntDPtoVP(dot1, GZoom, GZoomOffset_x, GZoomOffset_y);
					PntDPtoVP(dot2, GZoom, GZoomOffset_x, GZoomOffset_y);
					DotToPnt(pnt1, dot1);
					DotToPnt(pnt2, dot2);
					pnt1.x += GLinMMOffsetX;
					pnt1.y += GLinMMOffsetY;
					pnt2.x += GLinMMOffsetX;
					pnt2.y += GLinMMOffsetY;
					DrawSeg(&dc, GLinMMTmpNdx, pnt1, pnt2);
				}
				GLinMMPnt = point;
			}
			break;

		case OPERSTATE_LIN_ADD_DOT:
			if(isPick!=0)
			{
				CClientDC dc(this);
				dc.SetROP2(R2_NOTXORPEN);
				if (GMPnt.x != -1 && GMPnt.y != -1)
					DrawSeg(&dc, GLinToAdd, GLPntToAdd, GMPnt);
				//保存上一状态
				GMPnt.x = point.x;
				GMPnt.y = point.y;
				POINT mpoint = { mpoint.x = point.x,mpoint.y = point.y };
				DrawSeg(&dc, GLinToAdd, GLPntToAdd, mpoint);//连接
			}
			break;
		default:
			break;
		}
	}

	//-------------------------------------缩放和移动--------------------------------------------//

	if (GPntFCreated || GLinFCreated || GRegFCreated)
	{
		CClientDC dc(this);
		CPen pen(PS_DOT, 1, RGB(0, 0, 0));
		CPen* oldPen = dc.SelectObject(&pen);
		switch (GCurOperState)
		{
		case OPERSTATE_ZOOM: //放大
			if (GZoomMMPnt.x != -1 && GZoomMMPnt.y != -1)
			{
				dc.SetROP2(R2_NOTXORPEN);
				dc.Rectangle(GZoomLBDPnt.x, GZoomLBDPnt.y, GZoomMMPnt.x, GZoomMMPnt.y);
				dc.Rectangle(GZoomLBDPnt.x, GZoomLBDPnt.y, point.x, point.y);
				GZoomMMPnt = point;
				dc.SelectObject(oldPen);
			}
			break;
		case OPERSTATE_WINDOW_MOVE :
			if (GWinMoveMMPnt.x != -1 && GWinMoveMMPnt.y != -1)
			{
				CPoint offset(0, 0);
				offset.x = point.x - GWinMoveLBDPnt.x;
				offset.y = point.y - GWinMoveLBDPnt.y;
				GZoomOffset_x -= offset.x / GZoom;
				GZoomOffset_y -= offset.y / GZoom;
				GWinMoveLBDPnt = point;
				this->Invalidate();
			}
			break;
		default:
			break;
		}
	}

	//-------------------------------------区-------------------------------------//
	if (GRegFCreated)
		switch (GCurOperState)
		{
		case OPERSTATE_INPUT_REG:
		{
			if (GRegCreateMMPnt.x != -1 && GRegCreateMMPnt.y != -1)
			{
				CClientDC dc(this);
				dc.SetROP2(R2_NOTXORPEN);
				LIN_NDX_STRU tln = { tln.pattern = GTReg.pattern, tln.color = GTReg.color };
				if (GTReg.dotNum == 1)
				{
					DrawSeg(&dc, tln, GRegCreateStartPnt, GRegCreateMMPnt);
					DrawSeg(&dc, tln, GRegCreateStartPnt, point);
				}
				else
				{

					D_DOT* dot = new D_DOT[GTReg.dotNum];
					for (int i = 0; i < GTReg.dotNum; ++i)
					{
						ReadTempFileToRegDat(GRegTmpDatF, GTReg.datOff, i, dot[i]);
						PntDPtoVP(dot[i], GZoom, GZoomOffset_x, GZoomOffset_y);
					}
					POINT* pnt = new POINT[GTReg.dotNum + 1];
					DotToPnt(pnt, dot, GTReg.dotNum);
					pnt[GTReg.dotNum] = GRegCreateMMPnt;
					DrawReg(&dc, GTReg, pnt, GTReg.dotNum + 1);
					pnt[GTReg.dotNum] = point;
					DrawReg(&dc, GTReg, pnt, GTReg.dotNum + 1);
					delete[] dot;
					delete[] pnt;
				}
				GRegCreateMMPnt = point;
			}
			break;
		}
		case OPERSTATE_MOVE_REG:
			if (GRegNdx != -1)
			{
				CClientDC dc(this);
				dc.SetROP2(R2_NOTXORPEN);
				D_DOT* dot = new D_DOT[GRegMMTmpNdx.dotNum];
				for (int i = 0; i < GRegMMTmpNdx.dotNum; i++)
				{
					ReadTempFileToRegDat(GRegTmpDatF, GRegMMTmpNdx.datOff, i, dot[i]);
					PntDPtoVP(dot[i], GZoom, GZoomOffset_x, GZoomOffset_y);
					dot[i].x += GRegMMOffsetX;
					dot[i].y += GRegMMOffsetY;
				}
				POINT* pnt = new POINT[GRegMMTmpNdx.dotNum];
				DotToPnt(pnt, dot, GRegMMTmpNdx.dotNum);
				DrawReg(&dc, GRegMMTmpNdx, pnt, GRegMMTmpNdx.dotNum);
				GRegMMOffsetX = GRegMMOffsetX + point.x - GRegMMPnt.x;
				GRegMMOffsetY = GRegMMOffsetY + point.y - GRegMMPnt.y;
				for (int i = 0; i < GRegMMTmpNdx.dotNum; i++)
				{
					ReadTempFileToRegDat(GRegTmpDatF, GRegMMTmpNdx.datOff, i, dot[i]);
					PntDPtoVP(dot[i], GZoom, GZoomOffset_x, GZoomOffset_y);
					dot[i].x += GRegMMOffsetX;
					dot[i].y += GRegMMOffsetY;
				}
				DotToPnt(pnt, dot, GRegMMTmpNdx.dotNum);
				DrawReg(&dc, GRegMMTmpNdx, pnt, GRegMMTmpNdx.dotNum);
				delete[] dot;
				delete[] pnt;
				GRegMMPnt = point;
			}
			break;
		default:
			break;
		}
	CView::OnMouseMove(nFlags, point);
}

