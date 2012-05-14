#pragma once


// IN3WebkitView 뷰입니다.

class IN3WebkitView : public CView
{
	DECLARE_DYNCREATE(IN3WebkitView)

public:
	IN3WebkitView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~IN3WebkitView();

public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	virtual BOOL	PreTranslateMessage(MSG* pMsg);

	/*
	 * LoadError
	 * Load 시 Error 가 발생하는 경우 호출된다
	 * 파라미터	url				Load 할 url
	 *			errorMessage	에러 메시지
	 */
	virtual void	LoadError(BSTR url, BSTR errorMessage);

	/*
	 * DocumentComplete
	 * LoadURL 이 완료 되면 호출되는 함수
	 */
	virtual	void	DocumentComplete();

	/*
	 * LoadFromURL
	 * 임의의 URL 을 Load 한다.
	 * 파라미터 lpszURL url
	 */
	BOOL	LoadFromURL(LPCTSTR lpszURL);


	
	/******************************************************************
	 * Page Interface
	 ******************************************************************/
	/*
	 * GetURL
	 * Load 되는 url 주소를 반환한다.
	 * 리턴값 Load 된 url
	 */
	const CString GetURL() const;

	/*
	 * GetChapterIndex
	 * 웹킷 뷰에서 Load 한 챕터 파일의 spine 인덱스 값을 반환한다.
	 * 리턴값 spine 인덱스 값
	 */
	const int GetChapterIndex() const;

	/*
	 * GetTotalPage
	 * 웹킷 뷰에서 Load된 현재 파일의 total page 를 반환한다.
	 * 리턴값 현재 Load 된 파일의 total page
	 */
	const int GetTotalPage(void) const;

	/*
	 * GetCurrPage
	 * 현재 보여지고 있는 페이지를 반환한다.
	 * 리턴값 현재 보여지는 페이지
	 */
	const int GetCurrPage(void) const;

	/*
	 * MoveToPage
	 * 임의의 페이지로 이동한다.
	 * 파라미터 이동할 페이지
	 */
	void MoveToPage(int page);





	/******************************************************************
	 * Communication interface
	 ******************************************************************/
	/*
	 * ReceiveMessage
	 * 엔진으로 받은 콜백 메시지 중 뷰어에서 처리해야 하는 메시지를 처리하는 함수
	 * 파라미터	command	커맨드
	 *			param	파라미터
	 */
	virtual HRESULT	ReceiveMessage(CString command, CString param);


protected:
	DECLARE_MESSAGE_MAP()
};


