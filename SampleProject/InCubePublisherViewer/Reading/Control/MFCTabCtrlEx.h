#pragma once


// CMFCTabCtrlEx

class CMFCTabCtrlEx : public CMFCTabCtrl
{
	DECLARE_DYNAMIC(CMFCTabCtrlEx)

public:
	CMFCTabCtrlEx();
	virtual ~CMFCTabCtrlEx();

protected:
	DECLARE_MESSAGE_MAP()
	
public:
	afx_msg LRESULT OnCloseEpub(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEpubLoaded(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnContextMenu(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAnnotationMenu(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAnnotationAdded(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAnnotationSelected(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMemoIconSelected(WPARAM wParam, LPARAM lParam);
};


