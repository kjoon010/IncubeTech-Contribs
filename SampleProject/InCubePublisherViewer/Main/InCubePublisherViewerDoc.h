
// InCubePublisherViewerDoc.h : CInCubePublisherViewerDoc Ŭ������ �������̽�
//


#pragma once


class CInCubePublisherViewerDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CInCubePublisherViewerDoc();
	DECLARE_DYNCREATE(CInCubePublisherViewerDoc)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// �����Դϴ�.
public:
	virtual ~CInCubePublisherViewerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
};


