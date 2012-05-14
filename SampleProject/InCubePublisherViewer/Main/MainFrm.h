
// MainFrm.h : CMainFrame 클래스의 인터페이스
//

#pragma once
#if 0
#include "FileView.h"
#include "ClassView.h"
#include "OutputWnd.h"
#include "PropertiesWnd.h"
#endif

#include "LibraryView.h"

#include "IN3Annotation.h"

class CMainFrame : public CFrameWndEx
{
	
protected: // serialization에서만 만들어집니다.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	enum VIEW_MODE { LIBRARY_MODE = 0, READING_MODE };

protected:  // 컨트롤 모음이 포함된 멤버입니다.
	CMFCMenuBar       m_wndMenuBar;
#if 0
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	CFileView         m_wndFileView;
	CClassView        m_wndClassView;
	COutputWnd        m_wndOutput;
	CPropertiesWnd    m_wndProperties;
#endif
	CLibraryView*		m_pWndLibraryView;

	long				m_ViewMode;

	CWnd*				m_pBook;		// 책의 인스턴스 포인터.

// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnExitSizeMove();
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);

public:
	long GetViewMode() { return m_ViewMode; };

	// interface for Library
	BOOL IsExistInLibrary(LPCTSTR lpszFilePath);


	//afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);




	/*************************************************************************
	 * Epub Engine interface
	 *************************************************************************/
protected:
	IN3Annotation*	m_pAnnotation;

public:
	void SetBook(CWnd *pBook);
	CWnd *GetBook();

	afx_msg LRESULT OnEpubLoaded(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBookmarkStatus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAnnotationAdded(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBookmarkAdded(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnContextMenu(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAnnotationMenu(WPARAM wParam, LPARAM lParam);
	
	afx_msg LRESULT OnAnnotationSelected(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMemoIconSelected(WPARAM wParam, LPARAM lParam);

	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnMoveFirstPage();
	afx_msg void OnMovePrevPage();
	afx_msg void OnMoveNextPage();
	afx_msg void OnMoveLastPage();
	afx_msg void OnAddBookmark();
	afx_msg void OnAddHighlight();
	afx_msg void OnAddMemo();	
	afx_msg void OnDeleteAnnotation();
	afx_msg void OnChangeAnnotationColorRed();
	afx_msg void OnChangeAnnotationColorGreen();
	afx_msg void OnChangeAnnotationColorBlue();
	afx_msg void OnSettingViewing();
	afx_msg void OnSingleDisplayMode();
	afx_msg void OnDualDisplayMode();
};


