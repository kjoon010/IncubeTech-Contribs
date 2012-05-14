#pragma once

 
class WebkitUIDelegate;
class CWebkitLoadFrameBridge;

/**
  @class	IN3WebkitView
  @brief	Epub 뷰
 */
class AFX_EXT_CLASS IN3WebkitView : public CView
{
	DECLARE_DYNCREATE(IN3WebkitView)

public:
	// 생성자
	IN3WebkitView(); 

	// 소멸자
	virtual ~IN3WebkitView();

public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	// initialize
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, 
		const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	BOOL	LoadFromURL(LPCTSTR lpszURL);
	BOOL	ReLoad();

	virtual void InitPage(CString fontSize, CString familyName, CString back_clr, CString fore_clr);
	virtual void InvalidateInfo();

	virtual void LoadError(BSTR url, BSTR errorMessage);
	virtual	void DocumentComplete();

	// interface
	bool PageCapture(int width, int height, CBitmap &bitmap);

	const float GetFixedMultiplier();
	const float GetFixedMultiplier(int width, int height);

	const int GetZoomStep() const;

	void SetZoomStep(int step);
	void SetZoomIn();
	void SetZoomOut();



	// Page Interface
	void MoveToPage(int page);

	void NextPage();
	void PrevPage();

	void SetChapter(int nChapter);
	void SetTotalPage(int page);
	void SetCurrPage(int page);;

	const CString GetURL() const;
	const int GetChapterIndex() const;
	const int GetTotalPage(void) const;
	const int GetCurrPage(void) const;


	// Communication interface
	BOOL SendMessage(CString command, CString param); ///< webkit extension 으로 메시지를 보내는 함수
	virtual HRESULT	ReceiveMessage(CString command, CString param);	 ///< webkit extension 으로부터 메시지를 받는 함수


protected:
	bool GetIntColorValue(CString strColor, int &red, int &green, int &blue);


public:
	/*
	 * LS_NONE : 아무상태 아님
	 * LS_LOADING : LOADING 중
	 * LS_LOAD_DONE : LOAD 완료 된 상태
	 * LS_ERROR : 에러
	 */
	enum { LS_NONE, LS_LOADING, LS_LODE_DONE, LS_ERROR };

	HWND		m_ViewWindow; ///< webkit 랜더링 영역의 윈도우 핸들
	int			m_nLoadState; ///<Load State 를 나타내는 변수

	WebkitUIDelegate*		m_pWebkitUIDelegate; ///< webkit UI delegater
	CWebkitLoadFrameBridge*	m_pWebkitViewBridge; ///< webkit load frame delegater

protected:
	CString		m_url; ///< 현재 로드된 파일의 url
	int			m_nCurrChapter; ///< 현재 로드된 파일의 챕터 인덱스
	int			m_nTotalPage; ///< 현재 로드된 파일의 총 페이지 수
	int			m_nCurrPage; ///< 현재 보여지는 페이지 번호(1~max)
	int			m_nZoomStep; ///< 확대/축소 단계 값. 0 인 경우는 화면 맞춤 ( -3 ~ 5 )
	float		m_fixedMultiplier; ///< 확대/축소 비율 (1.0 = 100%)


protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


