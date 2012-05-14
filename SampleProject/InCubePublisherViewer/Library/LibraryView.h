#pragma once



// CLibraryView 폼 뷰입니다.

class CLibraryView : public CFormView
{
	DECLARE_DYNCREATE(CLibraryView)

public:
	CLibraryView();           // 동적 만들기에 사용되는 protected 생성자입니다.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};


