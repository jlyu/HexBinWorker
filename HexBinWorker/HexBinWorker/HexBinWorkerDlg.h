
// HexBinWorkerDlg.h : 头文件
#pragma once
#include "HexBinController.h"
#include "CnComm.h"


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
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedHexToBin();
	afx_msg void OnBnClickedBinToHex();

	void showTextField();
	void showFilePath();
	void getTextField(string& hexText, string& binText);

	// to be settled..
	CnComm _hCom;

	void findAvailableCom();
	void showAvailableCom(const vector<CString> &aCom);
	afx_msg void OnBnClickedBtnOpencom();

private:
	HexBinController _hbController;
public:
	
};
