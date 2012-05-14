// LibraryView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "InCubePublisherViewer.h"
#include "LibraryView.h"


// CLibraryView

IMPLEMENT_DYNCREATE(CLibraryView, CFormView)

CLibraryView::CLibraryView()
	: CFormView(CLibraryView::IDD)
{

}

CLibraryView::~CLibraryView()
{
}

void CLibraryView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLibraryView, CFormView)
END_MESSAGE_MAP()


// CLibraryView 진단입니다.

#ifdef _DEBUG
void CLibraryView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CLibraryView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CLibraryView 메시지 처리기입니다.
BOOL CLibraryView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext*  pContext/* = NULL*/)
{
	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

BOOL CLibraryView::IsExist(LPCTSTR lpszFilePath)
{
	// @ note
	// 구현 필요
	return TRUE;
}