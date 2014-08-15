#include "stdafx.h"
#include "HexBinWorker.h"
#include "HexBinWorkerDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// MARK: About Dialog 
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

protected:
	DECLARE_MESSAGE_MAP()
};
CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD) {
}
void CAboutDlg::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
}
BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// MARK: CHexBinWorkerDlg 
CHexBinWorkerDlg::CHexBinWorkerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHexBinWorkerDlg::IDD, pParent), _hCom(false, 0) {
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
void CHexBinWorkerDlg::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HEXEDIT, _hexEdit);
}
BEGIN_MESSAGE_MAP(CHexBinWorkerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CHexBinWorkerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_SAVE, &CHexBinWorkerDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDC_HEX_TO_BIN, &CHexBinWorkerDlg::OnBnClickedHexToBin)
	ON_BN_CLICKED(IDC_BIN_TO_HEX, &CHexBinWorkerDlg::OnBnClickedBinToHex)
	ON_BN_CLICKED(IDC_BTN_OPENCOM, &CHexBinWorkerDlg::OnBnClickedBtnOpencom)
END_MESSAGE_MAP()


// CHexBinWorkerDlg Message Process
BOOL CHexBinWorkerDlg::OnInitDialog() {
	CDialogEx::OnInitDialog();

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
	findAvailableCom();

	
	_editFont.CreateFont(-12, 0,0,0,0,0,0,0,0,0,0,0,0, _T("Consolas"));
	GetDlgItem(IDC_HEXFILEFIELD)->SetFont(&_editFont);
	GetDlgItem(IDC_BINFILEFIELD)->SetFont(&_editFont);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
void CHexBinWorkerDlg::OnSysCommand(UINT nID, LPARAM lParam) {
	if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	} else {
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
//当用户拖动最小化窗口时系统调用此函数取得光标显示
HCURSOR CHexBinWorkerDlg::OnQueryDragIcon() {
	return static_cast<HCURSOR>(m_hIcon);
}


// MARK: -User Interaction
void CHexBinWorkerDlg::showFilePath() {
	// clear
	GetDlgItem(IDC_HEX_PATH)->SetWindowText(_T(""));
	GetDlgItem(IDC_BIN_PATH)->SetWindowText(_T(""));

	CString hexPath, binPath;
	_hbController.getFilePath(hexPath, binPath);

	GetDlgItem(IDC_HEX_PATH)->SetWindowText(hexPath);
	GetDlgItem(IDC_BIN_PATH)->SetWindowText(binPath);
}
void CHexBinWorkerDlg::showHexEditText() {
	
	GetDlgItem(IDC_HEXFILEFIELD)->SetWindowText(_T(""));  // clear

	CString hexText;
	_hbController.getHexText(hexText);
	GetDlgItem(IDC_HEXFILEFIELD)->SetWindowText(hexText);
}
void CHexBinWorkerDlg::showBinEditText() {
	BYTE* pDatas = NULL;
	int dataSize = 0;
	_hbController.getBinDatas(pDatas, dataSize);

	_hexEdit.SetData(pDatas, dataSize);
}
void CHexBinWorkerDlg::getHexEditText(string& hexText){
	CString hexTextCStr;
	GetDlgItem(IDC_HEXFILEFIELD)->GetWindowText(hexTextCStr);
	hexText = CT2A(hexTextCStr);
}
void CHexBinWorkerDlg::getBinEditText(BYTE *pDatas, int dataSize) {
	_hexEdit.GetData(pDatas, dataSize);
}

// MARK: -Button
void CHexBinWorkerDlg::OnBnClickedOk()
{
	// Open
	CString fileFormatter(_T("Inter Hex (*.hex)|*.hex|Bin File (*.bin)|*.bin||"));
	CFileDialog pHexFileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, fileFormatter); 

	if(pHexFileDlg.DoModal() == IDOK) {

		CString filePathName = pHexFileDlg.GetPathName(); 

		// Process
		_hbController.init(filePathName);
		_hbController.read(filePathName);

		// Show 
		showHexEditText();
		showBinEditText();

		showFilePath();
	}
}
void CHexBinWorkerDlg::OnBnClickedHexToBin() {
	_hbController.typeHexToBin();

	string hexText;
	getHexEditText(hexText);

	bool succeed = _hbController.parseHex(hexText);
	if (!succeed) {
		MessageBox(_T("Hex 文件无法解析到 Bin 文件，格式错误"));
	}

	showHexEditText();
	showBinEditText();
}
void CHexBinWorkerDlg::OnBnClickedBinToHex() {
	_hbController.typeBinToHex();

	BYTE* pDatas = NULL;
	int dataSize = 0;

	
	_hbController.getBinDatas(pDatas, dataSize); 
	_hexEdit.GetData(pDatas, dataSize);
	_hbController.parseBin(pDatas, dataSize); //TODO:
	
	showHexEditText();
	showBinEditText();
}
void CHexBinWorkerDlg::OnBnClickedSave() {
	// refresh hex data
	string hexText;
	getHexEditText(hexText);
	
	bool verifyOK = _hbController.parseHex(hexText);
	if (!verifyOK) {
		MessageBox(_T("检测到 Hex 文件格式存在错误，中止全部保存"));
		return;
	}

	// refreash bin data
	BYTE* pDatas = NULL;
	int dataSize = 0;
	_hbController.getBinDatas(pDatas, dataSize);
	getBinEditText(pDatas, dataSize);

	// save
	bool writeHexOK = _hbController.writeHex();
	bool writeBinOK = _hbController.writeBin();

	if (writeHexOK && writeBinOK) {
		MessageBox(_T("Intex Hex 和对应 Bin 文件保存成功"), NULL);
	} else {
		MessageBox(_T("保存失败"));
	}
}


// TODO: contents will need to be rearranged
void CHexBinWorkerDlg::findAvailableCom() {

	HANDLE hCom;
	// find com number [COM1-16]
	const int comSerialSize = 16;
	vector<CString> availableComSerial;
	
	for (int iComNumber = 1; iComNumber <= comSerialSize; iComNumber++) {
		CString comNumber;
		comNumber.Format(_T("COM%d"), iComNumber);

		hCom = CreateFile(comNumber, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
		if (hCom != INVALID_HANDLE_VALUE) {
			availableComSerial.push_back(comNumber);
		}

		CloseHandle(hCom);
	}


	showAvailableCom(availableComSerial);
}

void CHexBinWorkerDlg::showAvailableCom(const vector<CString> &aCom) {
	const int availableComSize = aCom.size();
	if (availableComSize == 0) {
		((CComboBox *)GetDlgItem(IDC_COMBO_COM))->EnableWindow(FALSE);
		MessageBox(_T("没有侦测到可用的串口"));
	}

	((CComboBox *)GetDlgItem(IDC_COMBO_COM))->EnableWindow(TRUE);
	for (int i=0; i<availableComSize; i++) {
		((CComboBox *)GetDlgItem(IDC_COMBO_COM))->AddString(aCom[i]);
	}

	int comCount = ((CComboBox *)GetDlgItem((IDC_COMBO_COM)))->GetCount();
	((CComboBox *)GetDlgItem(IDC_COMBO_COM))->SetCurSel(comCount - 1);
}

void CHexBinWorkerDlg::OnBnClickedBtnOpencom() {
	
	CString comSerialStr;
	int currentComSerial = ((CComboBox *)GetDlgItem(IDC_COMBO_COM))->GetCurSel();
	((CComboBox *)GetDlgItem(IDC_COMBO_COM))->GetLBText(currentComSerial, comSerialStr);

	CString comNumberCStr = comSerialStr.Mid(3);
	int comNumber = _ttoi(comNumberCStr);

	// open com: USB port
	bool isSuccess = _hCom.Open(comNumber);
	if (!isSuccess)	{
		CString messageCStr(_T("无法打开串口：COM") + comNumberCStr);
		MessageBox(messageCStr);
		return;
	}

	_hCom.SetState(115200, 8, EVENPARITY, ONESTOPBIT);
	
	
	//while (true)
	//{
		//char *revData = new char[3];
		BYTE *revData = new BYTE[15];	

		_hCom.Write("\x00\xFF", 2);
		//_hCom.Reads(revData, 16);
		_hCom.Read(revData, 15);
		//revData[2] = '\0';

		CString bufferCStr, bufferBlock;
		for (int i=0; i<15; i++) {
			bufferCStr.Format(_T("%02X"), revData[i]);
			bufferBlock += bufferCStr;
		}
		
		//CString revCStr;
		//revCStr.Format(_T("%02X"), revData);

		delete [] revData;

	
	//}
	
	_hCom.Close();
	
}
