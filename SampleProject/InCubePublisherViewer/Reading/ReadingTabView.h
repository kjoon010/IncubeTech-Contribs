#pragma once


// ReadingView ���Դϴ�.

#include "MFCTabCtrlEx.h"

class CReadingTabView : public CView
{
	DECLARE_DYNCREATE(CReadingTabView)

public:
	CReadingTabView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CReadingTabView();

public:
	virtual void OnDraw(CDC* pDC);      // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	virtual void OnInitialUpdate();

	// add tab
	int AddTab(CWnd* pWnd, LPCTSTR lpszTabLabel);

	// remove tab
	BOOL RemoveTab(int tabId);

	
	CWnd* GetTabWindow() { return m_pWndReadingTab; };

protected:
	CMFCTabCtrlEx*		m_pWndReadingTab;

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};


