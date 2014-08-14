
// HexBinWorkerDlg.h : 头文件
#pragma once
#include "CnComm.h"
#include "HexBinController.h"
#include "HexEdit.h"


// CHexBinWorkerDlg 对话框
class CHexBinWorkerDlg : public CDialogEx
{
// 构造
public:
	CHexBinWorkerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_HEXBINWORKER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedHexToBin();
	afx_msg void OnBnClickedBinToHex();
	afx_msg void OnBnClickedBtnOpencom();
	DECLARE_MESSAGE_MAP()

public:

	void showTextField();
	void showFilePath();
	void getTextField(string& hexText, string& binText);

	// redo Edit Text 
	
	void showHexEditText();
	void getHexEditText(string& hexText);

	void showBinEditText();
	void getBinEditText(BYTE *pDatas, int dataSize);



	// to be rearraged..
	CnComm _hCom;
	void findAvailableCom();
	void showAvailableCom(const vector<CString> &aCom);
	

private:
	// -UI
	CFont _editFont;

	// -Controller
	HexBinController _hbController;
	CHexEdit _hexEdit;

	
};
