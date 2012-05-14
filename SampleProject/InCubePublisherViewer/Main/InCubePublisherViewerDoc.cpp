
// InCubePublisherViewerDoc.cpp : CInCubePublisherViewerDoc 클래스의 구현
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


// CInCubePublisherViewerDoc 생성/소멸

CInCubePublisherViewerDoc::CInCubePublisherViewerDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CInCubePublisherViewerDoc::~CInCubePublisherViewerDoc()
{
}

BOOL CInCubePublisherViewerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CInCubePublisherViewerDoc serialization

void CInCubePublisherViewerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}


// CInCubePublisherViewerDoc 진단

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


// CInCubePublisherViewerDoc 명령
