
// HexBinWorkerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HexBinWorker.h"
#include "HexBinWorkerDlg.h"
#include "afxdialogex.h"

#include <iostream>
#include <fstream>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CHexBinWorkerDlg 对话框
CHexBinWorkerDlg::CHexBinWorkerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHexBinWorkerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
void CHexBinWorkerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}
BEGIN_MESSAGE_MAP(CHexBinWorkerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CHexBinWorkerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_SAVE, &CHexBinWorkerDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDC_HEX_TO_BIN, &CHexBinWorkerDlg::OnBnClickedHexToBin)
	ON_BN_CLICKED(IDC_BIN_TO_HEX, &CHexBinWorkerDlg::OnBnClickedBinToHex)
END_MESSAGE_MAP()


// CHexBinWorkerDlg 消息处理程序

BOOL CHexBinWorkerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
void CHexBinWorkerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}
// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CHexBinWorkerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}
//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CHexBinWorkerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CHexBinWorkerDlg::showTextField() {
	// clear
	GetDlgItem(IDC_HEXFILEFIELD)->SetWindowText(_T(""));
	GetDlgItem(IDC_BINFILEFIELD)->SetWindowText(_T(""));

	CString hexText, binText;
	_hbController.getEditFieldText(hexText, binText);
	GetDlgItem(IDC_HEXFILEFIELD)->SetWindowText(hexText);
	GetDlgItem(IDC_BINFILEFIELD)->SetWindowText(binText);
}

void CHexBinWorkerDlg::showFilePath() {
	// clear
	GetDlgItem(IDC_HEX_PATH)->SetWindowText(_T(""));
	GetDlgItem(IDC_BIN_PATH)->SetWindowText(_T(""));

	CString hexPath, binPath;
	_hbController.getFilePath(hexPath, binPath);

	GetDlgItem(IDC_HEX_PATH)->SetWindowText(hexPath);
	GetDlgItem(IDC_BIN_PATH)->SetWindowText(binPath);
}


void CHexBinWorkerDlg::OnBnClickedOk()
{
	// Open
	CFileDialog pHexFileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Inter Hex (*.hex)|*.hex|Bin File|*.bin||")); 
	CString filePathName; 

	if(pHexFileDlg.DoModal() == IDOK) {

		filePathName = pHexFileDlg.GetPathName(); 

		// Handle
		_hbController.init(filePathName);
		_hbController.parse(filePathName);

		// Show 
		showTextField();
		showFilePath();
	}
}


void CHexBinWorkerDlg::OnBnClickedHexToBin()
{
	// using hex file to write bin file
	_hbController.writeToBinFile();

	// read bin file
	_hbController.parseBin();

	showTextField();
}

void CHexBinWorkerDlg::OnBnClickedBinToHex()
{
	_hbController.writeToHexFile();
	_hbController.parseHex();

	//showFilePath();
	showTextField();
}

void CHexBinWorkerDlg::OnBnClickedSave()
{
	//_hexFile.writeToBinFile(); //TODO: add fileNAME
	//MessageBox(_T("saved"), NULL);

}






