#include "StdAfx.h"
#include "WebkitViewEx.h"

IMPLEMENT_DYNCREATE(CWebkitViewEx, IN3WebkitView)

CWebkitViewEx::CWebkitViewEx(void)
{
}

CWebkitViewEx::~CWebkitViewEx(void)
{
}


BEGIN_MESSAGE_MAP(CWebkitViewEx, IN3WebkitView)
END_MESSAGE_MAP()


// CWebkitViewEx 그리기입니다.

void CWebkitViewEx::OnDraw(CDC* pDC)
{
	IN3WebkitView::OnDraw(pDC);

	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CWebkitViewEx 메시지 처리기입니다.