#pragma once
#include "CnComm.h"
#include "ComController.h"
#include "HexBinController.h"
#include "HexEdit.h"


// CHexBinWorkerDlg
class CHexBinWorkerDlg : public CDialogEx {

public:
	CHexBinWorkerDlg(CWnd* pParent = NULL);	

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
	//afx_msg void OnBnClickedBtnOpencom();
	afx_msg void OnBnClickedBtnFilereplication();
	DECLARE_MESSAGE_MAP()

public:

	// -User Interaction
	void showFilePath();
	
	void showHexEditText(bool needClear = true);
	void getHexEditText(string& hexText);

	void showBinEditText(bool needClear = false);
	void getBinEditText(BYTE *pDatas, int dataSize);


	// -Com
	void findAvailableCom();
	void showAvailableCom(const vector<CString> &aCom);
	

private:
	// -UI
	CFont _editFont;

	// -Controller
	HexBinController _hbController;
	ComController _comController;
	CHexEdit _hexEdit;
	
};
