#pragma once
#include <iostream>

using namespace std;

class CHexEdit : public CEdit {

public:
	CHexEdit();
	virtual ~CHexEdit();

	enum EDITMODE{	
		EDIT_NONE,
		EDIT_ASCII,
		EDIT_HIGH,
		EDIT_LOW
	};


public:
	LPBYTE		m_pData;			
	int			m_length;			
	int			m_topindex;			

	int			m_currentAddress;	
	EDITMODE	m_currentMode;		
	int			m_selStart;			
	int			m_selEnd;			

	int			m_bpr; // 卧槽。我猜是显示个数				
	int			m_lpp;				
	BOOL		m_bShowAddress;
	BOOL		m_bShowAscii;
	BOOL		m_bShowHex;
	BOOL		m_bAddressIsWide;
	
	BOOL		m_bNoAddressChange;	
	BOOL		m_bHalfPage;
	
	CFont		m_Font;
	int			m_lineHeight;
	int			m_nullWidth;
	BOOL		m_bUpdate;

	int			m_offHex,
				m_offAscii,
				m_offAddress;

	CPoint		m_editPos;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHexEdit)
public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL


public:
	void Clear();
	void AppendData(LPBYTE p, int addlen);
	void GetData(LPBYTE p, int &len);
	void SetData(LPBYTE p, int len);
	CSize GetSel(void);
	void SetSel(int s, int e);
	void SetBPR(int bpr);
	void SetOptions(BOOL a, BOOL h, BOOL c, BOOL w);
	

protected:
	void		ScrollIntoView(int p);
	void		RepositionCaret(int p);
	void		Move(int x, int y);
	inline BOOL IsSelected(void);
	void		UpdateScrollbars(void);
	void		CreateEditCaret(void);
	void		CreateAddressCaret(void);
	CPoint		CalcPos(int x, int y);
	void		SelInsert(int s, int l);
	void		SelDelete(int s, int e);
	inline void NormalizeSel(void);
	// modified methods
	void updateCharSize();


	//{{AFX_MSG(CHexEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point) { return; }
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point) { return; }
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point) { return; }
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point) { return; }
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	// del
	//afx_msg void OnContextMenu(CWnd*, CPoint point);
	//afx_msg void OnEditClear();
	//afx_msg void OnEditCut();
	//afx_msg void OnEditPaste();
	//afx_msg void OnEditSelectAll();
	//afx_msg void OnEditUndo();
	//afx_msg UINT OnGetDlgCode() { return DLGC_WANTALLKEYS; }
	
	//afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//afx_msg void OnSize(UINT nType, int cx, int cy);
	//afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);


	// TODO
	//afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnEditCopy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
