#pragma once
#include "CnComm.h"
#include "HexBinController.h"
#include "HexEdit.h"


// CHexBinWorkerDlg 对话框
class CHexBinWorkerDlg : public CDialogEx {

public:
	CHexBinWorkerDlg(CWnd* pParent = NULL);	

// 对话框数据
	enum { IDD = IDD_HEXBINWORKER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);


protected:
	HICON m_hIcon;
	virtual BOOL OnInitDialog();

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	// -Button
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedHexToBin();
	afx_msg void OnBnClickedBinToHex();
	afx_msg void OnBnClickedBtnOpencom();
	DECLARE_MESSAGE_MAP()

public:

	// -User Interaction
	void showFilePath();
	
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
