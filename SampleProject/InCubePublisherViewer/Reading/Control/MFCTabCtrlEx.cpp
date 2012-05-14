// MFCTabCtrlEx.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "InCubePublisherViewer.h"
#include "MFCTabCtrlEx.h"

#include "IN3WebkitDefine.h"
#include "IN3Annotation.h"

// CMFCTabCtrlEx

IMPLEMENT_DYNAMIC(CMFCTabCtrlEx, CMFCTabCtrl)

CMFCTabCtrlEx::CMFCTabCtrlEx()
{

}

CMFCTabCtrlEx::~CMFCTabCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CMFCTabCtrlEx, CMFCTabCtrl)
	ON_MESSAGE(WM_CLOSE_EPUB, OnCloseEpub)
	ON_MESSAGE(WM_LODEAD_EPUB, OnEpubLoaded)
	ON_MESSAGE(WM_CONTEXTMENU_EPUB, OnContextMenu)
	ON_MESSAGE(WM_ANNOTATIONMENU_EPUB, OnAnnotationMenu)
	ON_MESSAGE(WM_ANNOTATION_SELECTED, OnAnnotationSelected)
	ON_MESSAGE(WM_MEMO_ICON_SELECTED, OnMemoIconSelected)
END_MESSAGE_MAP()



// CMFCTabCtrlEx 메시지 처리기입니다.
LRESULT CMFCTabCtrlEx::OnCloseEpub(WPARAM wParam, LPARAM lParam)
{
	TRACE(_T("CMFCTabCtrlEx::OnCloseEpub() START\r\n"));
	//
	HRESULT hr = S_OK;

	// Remove epub
	theApp.m_WebkitEngine.Close((CWnd *)wParam);

	// Remove tab
	int tabID = (int)lParam;
	int index = GetTabByID(tabID);
	if(index >= 0)
	{
		int next = -1;
		if(index + 1 < GetTabsNum())
			next = index + 1;
		else if( index - 1 >= 0 )
			next = index - 1;
		//
		if( next >= 0)
		{
			SetActiveTab(next);
			ShowTab(next);
		}

		if(!RemoveTab(index)) return S_FALSE;
	}
	//
	TRACE(_T("CMFCTabCtrlEx::OnCloseEpub() END\r\n"));
	return hr;
}

LRESULT CMFCTabCtrlEx::OnEpubLoaded(WPARAM wParam, LPARAM lParam)
{
	HRESULT hr = S_OK;
	BOOL bInitialized = (BOOL)lParam;
	if(!bInitialized)
	{
		// 최초 책이 Load 완료되면 화면 사이즈에 맞춰 레이아웃을 다시 정리해야한다.
		//theApp.m_WebkitEngine.SetPageDisplayMode((CWnd *)wParam, 1);
		theApp.m_WebkitEngine.ReLayout((CWnd *)wParam);
		theApp.m_WebkitEngine.MoveToPage((CWnd *)wParam, 0);
	}
	return hr;
}

LRESULT CMFCTabCtrlEx::OnContextMenu(WPARAM wParam, LPARAM lParam)
{
	TRACE(_T("CMFCTabCtrlEx::OnContextMenu() START\r\n"));
	HRESULT hr = S_OK;
	//
	// UI process
	//
	TRACE(_T("CMFCTabCtrlEx::OnContextMenu() END\r\n"));
	return hr;
}

LRESULT CMFCTabCtrlEx::OnAnnotationMenu(WPARAM wParam, LPARAM lParam)
{
	TRACE(_T("CMFCTabCtrlEx::OnAnnotationMenu() START\r\n"));
	HRESULT hr = S_OK;
	//
	// UI process
	//theApp.m_WebkitEngine.CreateTextHighlight((CWnd*)wParam, _T("rgba(255,0,0,0.2)"), TRUE);

	//
	TRACE(_T("CMFCTabCtrlEx::OnAnnotationMenu() END\r\n"));
	return hr;
}

LRESULT CMFCTabCtrlEx::OnAnnotationAdded(WPARAM wParam, LPARAM lParam)
{
	TRACE(_T("CMFCTabCtrlEx::OnAnnotationAdded() START\r\n"));
	HRESULT hr = S_OK;
	//
	// UI process
	//
	TRACE(_T("CMFCTabCtrlEx::OnAnnotationAdded() END\r\n"));
	return hr;
}

LRESULT CMFCTabCtrlEx::OnAnnotationSelected(WPARAM wParam, LPARAM lParam)
{
	TRACE(_T("CMFCTabCtrlEx::OnAnnotationSelected() START\r\n"));
	HRESULT hr = S_OK;
	/*
	// UI process
	IN3Annotation *pAnnotation = (IN3Annotation *)lParam;
	if(pAnnotation->GetBGColor() == _T(""))
		pAnnotation->SetBGColor(_T("rgba(0,255,0,0.3)"));
	else
		pAnnotation->SetBGColor(_T(""));
	//theApp.m_WebkitEngine.DeleteAnnotation((CWnd*)wParam, pAnnotation->GetFileName(), pAnnotation->GetMarkerID());
	theApp.m_WebkitEngine.ModifyAnnotation((CWnd*)wParam, pAnnotation);
	*/
	TRACE(_T("CMFCTabCtrlEx::OnAnnotationSelected() END\r\n"));
	return hr;
}

LRESULT CMFCTabCtrlEx::OnMemoIconSelected(WPARAM wParam, LPARAM lParam)
{
	TRACE(_T("CMFCTabCtrlEx::OnMemoIconSelected() START\r\n"));
	HRESULT hr = S_OK;
	//
	// UI process
	//
	TRACE(_T("CMFCTabCtrlEx::OnMemoIconSelected() END\r\n"));
	return hr;
}