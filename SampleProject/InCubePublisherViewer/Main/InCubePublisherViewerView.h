
// InCubePublisherViewerView.h : CInCubePublisherViewerView Ŭ������ �������̽�
//


#pragma once


class CInCubePublisherViewerView : public CView
{
protected: // serialization������ ��������ϴ�.
	CInCubePublisherViewerView();
	DECLARE_DYNCREATE(CInCubePublisherViewerView)

// Ư���Դϴ�.
public:
	CInCubePublisherViewerDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CInCubePublisherViewerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // InCubePublisherViewerView.cpp�� ����� ����
inline CInCubePublisherViewerDoc* CInCubePublisherViewerView::GetDocument() const
   { return reinterpret_cast<CInCubePublisherViewerDoc*>(m_pDocument); }
#endif

