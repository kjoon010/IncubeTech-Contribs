#pragma once



// CLibraryView �� ���Դϴ�.

class CLibraryView : public CFormView
{
	DECLARE_DYNCREATE(CLibraryView)

public:
	CLibraryView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CLibraryView();

public:
	enum { IDD = IDD_LIBRARYVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:	
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext*  pContext = NULL);


	BOOL	IsExist(LPCTSTR lpszFilePath);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};


