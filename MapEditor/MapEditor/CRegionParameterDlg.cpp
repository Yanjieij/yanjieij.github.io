// CRegionParameterDlg.cpp: 实现文件
//

#include "pch.h"
#include "MapEditor.h"
#include "CRegionParameterDlg.h"
#include "afxdialogex.h"


// CRegionParameterDlg 对话框

IMPLEMENT_DYNAMIC(CRegionParameterDlg, CDialogEx)

CRegionParameterDlg::CRegionParameterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_REGION_PARAMETER, pParent)
{
	m_Pattern = 0;
}

CRegionParameterDlg::~CRegionParameterDlg()
{
}

void CRegionParameterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REGION_PATTERN, m_ComboBox);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON1, m_ColorButton);
}


BEGIN_MESSAGE_MAP(CRegionParameterDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_REGION_PATTERN, &CRegionParameterDlg::OnSelchangeRegionPattern)
END_MESSAGE_MAP()


// CRegionParameterDlg 消息处理程序


BOOL CRegionParameterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ComboBox.SetCurSel(m_Pattern);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CRegionParameterDlg::OnSelchangeRegionPattern()
{
	m_Pattern = m_ComboBox.GetCurSel();
}
