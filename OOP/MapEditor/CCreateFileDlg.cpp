// CCreateFileDlg.cpp: 实现文件
//

#include "pch.h"
#include "MapEditor.h"
#include "CCreateFileDlg.h"
#include "afxdialogex.h"


// CCreateFileDlg 对话框

IMPLEMENT_DYNAMIC(CCreateFileDlg, CDialogEx)
//路径的显示
CCreateFileDlg::CCreateFileDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CREATE_FILE, pParent)
	, m_add(_T(""))
{
	HMODULE module = GetModuleHandle(0);
	char* pFileName = new char[MAX_PATH];
	GetModuleFileName(module, LPWSTR(pFileName), MAX_PATH);
	m_add.Format(_T("%s"), pFileName);
	int nPos = m_add.ReverseFind(_T('\\'));
	if (nPos < 0)
	{
		m_add = "";
	}
	else
	{
		m_add = m_add.Left(nPos);	//得到.exe文件地址
	}
	delete[] pFileName;
}

CCreateFileDlg::~CCreateFileDlg()
{
}

void CCreateFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CREATE_FILE_ADDRESS, m_add);
}


BEGIN_MESSAGE_MAP(CCreateFileDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CREATE_FILE_CHANGE_ADDRESS_BTN, &CCreateFileDlg::OnBnClickedCreateFileChangeAddressBtn)
END_MESSAGE_MAP()


// CCreateFileDlg 消息处理程序

//路径选择的实现
void CCreateFileDlg::OnBnClickedCreateFileChangeAddressBtn()
{
	BROWSEINFO bInfo;
	ZeroMemory(&bInfo, sizeof(bInfo));
	bInfo.hwndOwner = GetSafeHwnd();
	bInfo.lpszTitle = _T("请选择临时文件存放路径");
	bInfo.ulFlags = BIF_RETURNONLYFSDIRS;
	LPITEMIDLIST lpDlist;	//保存返回信息
	lpDlist = SHBrowseForFolder(&bInfo);	//显示选择对话框
	if (lpDlist != NULL)
	{
		TCHAR chPath[MAX_PATH];					//储存路径的字符串
		SHGetPathFromIDList(lpDlist, chPath);	//把项目表示列表转化为字符串
		m_add = chPath;							//把TChar转化为CString
		UpdateData(FALSE);						//更新变量值到控件
	}
	// TODO: 在此添加控件通知处理程序代码
}
