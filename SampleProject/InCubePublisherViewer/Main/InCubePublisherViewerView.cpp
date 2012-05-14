
// InCubePublisherViewerView.cpp : CInCubePublisherViewerView Ŭ������ ����
//

#include "stdafx.h"
#include "InCubePublisherViewer.h"

#include "InCubePublisherViewerDoc.h"
#include "InCubePublisherViewerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CInCubePublisherViewerView

IMPLEMENT_DYNCREATE(CInCubePublisherViewerView, CView)

BEGIN_MESSAGE_MAP(CInCubePublisherViewerView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CInCubePublisherViewerView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CInCubePublisherViewerView ����/�Ҹ�

CInCubePublisherViewerView::CInCubePublisherViewerView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CInCubePublisherViewerView::~CInCubePublisherViewerView()
{
}

BOOL CInCubePublisherViewerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CInCubePublisherViewerView �׸���

void CInCubePublisherViewerView::OnDraw(CDC* /*pDC*/)
{
	CInCubePublisherViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CInCubePublisherViewerView �μ�


void CInCubePublisherViewerView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CInCubePublisherViewerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CInCubePublisherViewerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CInCubePublisherViewerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}

void CInCubePublisherViewerView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CInCubePublisherViewerView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CInCubePublisherViewerView ����

#ifdef _DEBUG
void CInCubePublisherViewerView::AssertValid() const
{
	CView::AssertValid();
}

void CInCubePublisherViewerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CInCubePublisherViewerDoc* CInCubePublisherViewerView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CInCubePublisherViewerDoc)));
	return (CInCubePublisherViewerDoc*)m_pDocument;
}
#endif //_DEBUG


// CInCubePublisherViewerView �޽��� ó����
