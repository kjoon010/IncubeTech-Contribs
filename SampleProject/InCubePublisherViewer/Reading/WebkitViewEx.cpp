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


// CWebkitViewEx �׸����Դϴ�.

void CWebkitViewEx::OnDraw(CDC* pDC)
{
	IN3WebkitView::OnDraw(pDC);

	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
}


// CWebkitViewEx �޽��� ó�����Դϴ�.