
// InCubePublisherViewerDoc.cpp : CInCubePublisherViewerDoc Ŭ������ ����
//

#include "stdafx.h"
#include "InCubePublisherViewer.h"

#include "InCubePublisherViewerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CInCubePublisherViewerDoc

IMPLEMENT_DYNCREATE(CInCubePublisherViewerDoc, CDocument)

BEGIN_MESSAGE_MAP(CInCubePublisherViewerDoc, CDocument)
END_MESSAGE_MAP()


// CInCubePublisherViewerDoc ����/�Ҹ�

CInCubePublisherViewerDoc::CInCubePublisherViewerDoc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.

}

CInCubePublisherViewerDoc::~CInCubePublisherViewerDoc()
{
}

BOOL CInCubePublisherViewerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}




// CInCubePublisherViewerDoc serialization

void CInCubePublisherViewerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
	}
}


// CInCubePublisherViewerDoc ����

#ifdef _DEBUG
void CInCubePublisherViewerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CInCubePublisherViewerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CInCubePublisherViewerDoc ���
