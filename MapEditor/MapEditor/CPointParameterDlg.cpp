// CPointParameterDlg.cpp: 实现文件
//

#include "pch.h"
#include "MapEditor.h"
#include "CPointParameterDlg.h"
#include "afxdialogex.h"


// CPointParameterDlg 对话框

IMPLEMENT_DYNAMIC(CPointParameterDlg, CDialogEx)

CPointParameterDlg::CPointParameterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_POINT_PARAMETER, pParent)
{
	m_Pattern = 0;
}

CPointParameterDlg::~CPointParameterDlg()
{
}

void CPointParameterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_POINT_PATTERN, m_ComboBox);
	DDX_Control(pDX, IDC_POINT_COLOR, m_ColorButton);
}


BEGIN_MESSAGE_MAP(CPointParameterDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_POINT_PATTERN, &CPointParameterDlg::OnSelchangePointPattern)
END_MESSAGE_MAP()


// CPointParameterDlg 消息处理程序


BOOL CPointParameterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ComboBox.SetCurSel(m_Pattern);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CPointParameterDlg::OnSelchangePointPattern()
{
	m_Pattern = m_ComboBox.GetCurSel();
}
