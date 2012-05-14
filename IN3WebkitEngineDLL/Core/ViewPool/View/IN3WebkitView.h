#pragma once

 
class WebkitUIDelegate;
class CWebkitLoadFrameBridge;

/**
  @class	IN3WebkitView
  @brief	Epub ��
 */
class AFX_EXT_CLASS IN3WebkitView : public CView
{
	DECLARE_DYNCREATE(IN3WebkitView)

public:
	// ������
	IN3WebkitView(); 

	// �Ҹ���
	virtual ~IN3WebkitView();

public:
	virtual void OnDraw(CDC* pDC);      // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
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
	BOOL SendMessage(CString command, CString param); ///< webkit extension ���� �޽����� ������ �Լ�
	virtual HRESULT	ReceiveMessage(CString command, CString param);	 ///< webkit extension ���κ��� �޽����� �޴� �Լ�


protected:
	bool GetIntColorValue(CString strColor, int &red, int &green, int &blue);


public:
	/*
	 * LS_NONE : �ƹ����� �ƴ�
	 * LS_LOADING : LOADING ��
	 * LS_LOAD_DONE : LOAD �Ϸ� �� ����
	 * LS_ERROR : ����
	 */
	enum { LS_NONE, LS_LOADING, LS_LODE_DONE, LS_ERROR };

	HWND		m_ViewWindow; ///< webkit ������ ������ ������ �ڵ�
	int			m_nLoadState; ///<Load State �� ��Ÿ���� ����

	WebkitUIDelegate*		m_pWebkitUIDelegate; ///< webkit UI delegater
	CWebkitLoadFrameBridge*	m_pWebkitViewBridge; ///< webkit load frame delegater

protected:
	CString		m_url; ///< ���� �ε�� ������ url
	int			m_nCurrChapter; ///< ���� �ε�� ������ é�� �ε���
	int			m_nTotalPage; ///< ���� �ε�� ������ �� ������ ��
	int			m_nCurrPage; ///< ���� �������� ������ ��ȣ(1~max)
	int			m_nZoomStep; ///< Ȯ��/��� �ܰ� ��. 0 �� ���� ȭ�� ���� ( -3 ~ 5 )
	float		m_fixedMultiplier; ///< Ȯ��/��� ���� (1.0 = 100%)


protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


