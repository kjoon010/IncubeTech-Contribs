
// InCubePublisherViewerView.cpp : CInCubePublisherViewerView 클래스의 구현
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
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CInCubePublisherViewerView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CInCubePublisherViewerView 생성/소멸

CInCubePublisherViewerView::CInCubePublisherViewerView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CInCubePublisherViewerView::~CInCubePublisherViewerView()
{
}

BOOL CInCubePublisherViewerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CInCubePublisherViewerView 그리기

void CInCubePublisherViewerView::OnDraw(CDC* /*pDC*/)
{
	CInCubePublisherViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CInCubePublisherViewerView 인쇄


void CInCubePublisherViewerView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CInCubePublisherViewerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CInCubePublisherViewerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CInCubePublisherViewerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
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


// CInCubePublisherViewerView 진단

#ifdef _DEBUG
void CInCubePublisherViewerView::AssertValid() const
{
	CView::AssertValid();
}

void CInCubePublisherViewerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CInCubePublisherViewerDoc* CInCubePublisherViewerView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CInCubePublisherViewerDoc)));
	return (CInCubePublisherViewerDoc*)m_pDocument;
}
#endif //_DEBUG


// CInCubePublisherViewerView 메시지 처리기
