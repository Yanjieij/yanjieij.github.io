// CLineParameterDlg.cpp: 实现文件
//

#include "pch.h"
#include "MapEditor.h"
#include "CLineParameterDlg.h"
#include "afxdialogex.h"


// CLineParameterDlg 对话框

IMPLEMENT_DYNAMIC(CLineParameterDlg, CDialogEx)

CLineParameterDlg::CLineParameterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LINE_PARAMETER, pParent)
{
	m_Pattern = 0;
}

CLineParameterDlg::~CLineParameterDlg()
{
}

void CLineParameterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LINE_PATTERN, m_ComboBox);
	DDX_Control(pDX, IDC_LINE_COLOR, m_ColorButton);
}


BEGIN_MESSAGE_MAP(CLineParameterDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_LINE_PATTERN, &CLineParameterDlg::OnSelchangeLinePattern)
END_MESSAGE_MAP()


// CLineParameterDlg 消息处理程序


BOOL CLineParameterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ComboBox.SetCurSel(m_Pattern);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CLineParameterDlg::OnSelchangeLinePattern()
{
	m_Pattern = m_ComboBox.GetCurSel();
}
