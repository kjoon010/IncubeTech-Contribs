// ReadingView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "InCubePublisherViewer.h"
#include "ReadingTabView.h"


// ReadingView

IMPLEMENT_DYNCREATE(CReadingTabView, CView)

CReadingTabView::CReadingTabView()
	:m_pWndReadingTab(NULL)
{

}

CReadingTabView::~CReadingTabView()
{
}

BEGIN_MESSAGE_MAP(CReadingTabView, CView)
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// ReadingView �׸����Դϴ�.

void CReadingTabView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
}


// ReadingView �����Դϴ�.

#ifdef _DEBUG
void CReadingTabView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CReadingTabView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// ReadingView �޽��� ó�����Դϴ�.

void CReadingTabView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	
	CRect rect;
    GetClientRect(&rect);
	if ( m_pWndReadingTab == NULL )
	{
		m_pWndReadingTab = new CMFCTabCtrlEx();
		if (!m_pWndReadingTab->Create(CMFCTabCtrl::STYLE_3D_VS2005, rect, this, -1, CMFCTabCtrl::LOCATION_TOP,FALSE))
		{
		#if _DEBUG
			TRACE0("���� ���� ������ ���߽��ϴ�.\r\n");
		#endif /*_DEBUG*/
			return;
		}
		CWnd *pWnd = m_pWndReadingTab->GetActiveWindow();
		m_pWndReadingTab->EnableActiveTabCloseButton();

		// Don't remove book instance when tab closed. 
		// Because webkitEngine delete the book instance.
		m_pWndReadingTab->AutoDestroyWindow(FALSE);	
	}
}


/*
 * add tab
 * �Ķ���� pWnd			�ǿ� ������ å ��ü
 *			lpszTabLabel	���� Ÿ��Ʋ
 */
int CReadingTabView::AddTab(CWnd* pWnd, LPCTSTR lpszTabLabel)
{
	if ( m_pWndReadingTab == NULL ) return -1;

	m_pWndReadingTab->AddTab(pWnd, lpszTabLabel, -1, FALSE);
	
	int index = 0;
	index = m_pWndReadingTab->GetTabsNum()-1;
	//
	m_pWndReadingTab->SetActiveTab(index);
	m_pWndReadingTab->ShowTab(index);

	return m_pWndReadingTab->GetTabID(index);	// An ID of the tab or -1 if iTab is out of range.
}

/*
 * Ư�� ���� �ݴ� �Լ�
 * �Ķ����	tabId	���� ���� id
 */
BOOL CReadingTabView::RemoveTab(int tabId)
{
	if ( m_pWndReadingTab == NULL ) return FALSE;
	//
	BOOL bRemove = false;
	int index = m_pWndReadingTab->GetTabByID(tabId);
	if ( index >= 0 )
	{
		int next = -1;
		if(index+1 < m_pWndReadingTab->GetTabsNum())
			next = index+1;
		else if( index-1 >= 0 )
			next = index-1;
		//
		if( next >= 0)
		{
			m_pWndReadingTab->SetActiveTab(next);
			m_pWndReadingTab->ShowTab(next);
		}

		bRemove = m_pWndReadingTab->RemoveTab(index);
	}
	return bRemove;
}

void CReadingTabView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�
	::MoveWindow(m_pWndReadingTab->GetSafeHwnd(),  0 , 0 , cx , cy, FALSE);
}

void CReadingTabView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch(nChar)
	{
	case 49:
		{
			int active = m_pWndReadingTab->GetActiveTab();	
			theApp.m_WebkitEngine.SetPageDisplayMode(m_pWndReadingTab->GetTabWnd(active), 1);
		}
		break;

	case 50:
		{
			int active = m_pWndReadingTab->GetActiveTab();	
			theApp.m_WebkitEngine.SetPageDisplayMode(m_pWndReadingTab->GetTabWnd(active), 2);
		}
		break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
