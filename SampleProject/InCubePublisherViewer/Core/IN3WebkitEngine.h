#pragma once

#include "IN3NavPointNode.h"
#include "IN3Annotation.h"

////////////////////////////////////////////////////////////////////////////////////////////
// IN3WebkitEngine
// 랜더러 엔진 및 엔진 익스텐션이 구현되어 있는 최상위 클래스
// UI 에서 랜더링과 관련하여 요청 되어지는 내용은 IN3WebkitEngine 를 통하여 처리된다.

class IN3WebkitEngine
{
public:
	IN3WebkitEngine(void);
	~IN3WebkitEngine(void);

	/*
	 * LoadLibraries
	 * Epub Open 시에 Load 되는 라이브러리들을 임의 Load 한다.
	 * 최초 Epub 을 열 때에는 많은 Library 들이 Load 되어 속도 저하의 원인이 된다.
	 * 따라서 미리 라이브러리들을 Load 해 두면 Epub Open 시에 소요되는 속도를 줄일 수 있다.
	 */
	BOOL LoadLibraries(void);
	
	/*
	 * Open
	 * 임의의 책을 연다.
	 * 파라미터 pParentWnd		생성된 책 객체의 ParentWnd 포인터
	 *			lpszPathName	원본 책의 절대 경로
	 *			lpszDescPath	unzip 된 절대 경로
	 * 리턴값	생성된 책 객체
	 */
	CWnd* Open(CWnd *pParentWnd, LPCTSTR lpszFilePath, LPCTSTR lpszDescPath);

	/*
	 * Close
	 * 책을 닫는다.
	 * 엔진에서 가지고 있는 책 리스트에서 제거
	 * 파라미터 lpszFilePath	제거할 책의 원본 절대 경로
	 */
	void Close(LPCTSTR lpszFilePath);

	/*
	 * Close
	 * 책을 닫는다.
	 * 엔진에서 가지고 있는 책 리스트에서 제거
	 * 파라미터 pBook	제거할 책
	 */
	void Close(CWnd *pBook);

	/*
	 * 책에 필요한 뷰를 생성하는 함수
	 * 지정된 형식의 뷰를 생성한다.
	 * 파라미터 pBook	뷰를 생성할 책 인스턴스 포인터
	 */
	BOOL CreateView(CWnd* pBook);

	/*
	 * 책에 필요한 뷰를 생성하는 함수
	 * 지정된 형식의 뷰를 생성한다.
	 * 파라미터 pBook	뷰를 생성할 책 인스턴스 포인터
	 *			pRuntimeClass	생성하고자 하는 뷰의 런타임 클래스
	 */
	BOOL CreateView(CWnd* pBook, CRuntimeClass* pRuntimeClass);

	/*
	 * 윈도우 크기 변경등의 이유로 화면 레이아웃을 재정비 해야 하는 경우 사용한다.
	 * 현재 열려있는 모든 책의 Layout 을 재정리
	 */
	void ReLayout();

	/*
	 * 윈도우 크기 변경등의 이유로 화면 레이아웃을 재정비 해야 하는 경우 사용한다.
	 * 임의의 책의 Layout 을 재정리
	 * 파라미터 pBook 레이아웃을 재 정리 할 책의 인스턴스 포인터
	 */
	void ReLayout(CWnd* pBook);

	/*
	 * 임의의 책을 재 랜더링 한다.
	 * 파라미터 pBook Reload 할 책의 인스턴스  포인터
	 */
	void ReLoad(CWnd* pBook);

	/*
	 * 임의의 책의 view 만 보이게 함.
	 * 파라미터 lpszFileName 책의 원본 파일의 절대 경로
	 */
	void SetVisibleEpub(CString lpszFileName);

	/*
	 * 임의의 책의 view 만 보이게 함.
	 * 파라미터 pBook 책의 인스턴스 포인터
	 */
	void SetVisibleEpub(CWnd *pBook);

	/*
	 * ViewZoomIn
	 * 확대
	 * 파라미터 pBook 책의 인스턴스 포인터
	 */
	void ViewZoomIn(CWnd *pBook);

	/*
	 * ViewZoomOut
	 * 축소
	 * 파라미터 pBook 책의 인스턴스 포인터
	 */
	void ViewZoomOut(CWnd *pBook);

	/*
	 * ViewFit
	 * 맞춤
	 * 파라미터 pBook 책의 인스턴스 포인터
	 */
	void ViewFit(CWnd *pBook);





	/******************************************************************
	 * engine's feature	
	 ******************************************************************/
	/*
	 * GetEpubMainPath
	 * open 된 epub 파일들의 내부 저장 경로를 반환한다
	 * 리턴값	open 된 epub 파일들의 내부 저장 경로
	 */	
	const CString GetEpubMainPath(void) const;

	/*
	 * SetPageDisplayMode
	 * 페이지 모드를 설정한다. (단면/양면)
	 * 파라미터 pBook	페이지 모드를 설정할 책
	 *			mode	1 : 단면, 2 : 양면
	 */
	void SetPageDisplayMode(CWnd *pBook, int mode);

	/*
	 * GetPageDisplayMode
	 * 현재 페이지 모드를 반환한다.
	 * 파라미터 pBook	페이지 모드를 가져올 책
	 */
	const int GetPageDisplayMode(CWnd *pBook) const;

	/*
	 * 본문 아래 영역에 페이지 번호 표시 여부를 결정한다.
	 * 파라미터 bDisplay 페이지 번호 표시
	 */
	void SetDisplayPageNumber(bool bDisplay);

	/*
	 * 임의의 책의 본문 아래 영역에 페이지 번호 표시 여부를 결정한다.
	 * 파라미터 pBook  책의 인스턴스  포인터
	 *			bDisplay 페이지 번호 표시
	 */
	void SetDisplayPageNumber(CWnd *pBook, bool bDisplay);

	/*
	 * 디폴트 폰트가 적용된 서체에 한해서 서체를 변경한다.
	 * 변경 후 ReLayout() 을 해야 본문에 적용된다.
	 * 파라미터 pBook  책의 인스턴스  포인터
	 *			fontFamilyName 변경할 서체 명
	 */
	BOOL SetFontFamily(CWnd *pBook, CString fontFamilyName);

	/*
	 * 사용자가 지정한 디폴트 폰트의 서체를 가져온다.
	 * 리턴값 디폴트 서체 (최초 로딩시는 "")
	 */
	const CString GetFontFamily(CWnd *pBook) const;

	/*
	 * 현재 설정된 폰트의 크기를 변경한다.
	 * 변경 후 ReLayout() 을 해야 본문에 적용된다.
	 * 파라미터 pBook  책의 인스턴스  포인터
	 *			fontSize 서체 크기
	 */
	BOOL SetFontSize(CWnd *pBook, int fontSize);

	/*
	 * 사용자가 지정한 폰트의 크기를 가져온다.
	 * 리턴값 서체 사이즈 (최초 로딩시는 "0")
	 */
	const int GetFontSize(CWnd *pBook) const;

	/*
	 * 현재 설정된 폰트의 색상를 변경한다.
	 * 변경 후 ReLayout() 을 해야 본문에 적용된다.
	 * 파라미터 pBook  책의 인스턴스  포인터
	 *			strColor 폰트 색상
	 */
	BOOL SetFontColor(CWnd *pBook, CString strColor);

	/*
	 * 사용자가 지정한 폰트의 색상를 가져온다.
	 * 리턴값 폰트의 색상 (최초 로딩시는 "")
	 */
	const CString GetFontColor(CWnd *pBook) const;

	/*
	 * 현재 배경색을 변경한다.
	 * 변경 후 ReLayout() 을 해야 본문에 적용된다.
	 * 파라미터 pBook  책의 인스턴스  포인터
	 *			strColor 배경색
	 */
	BOOL SetBackgroudColor(CWnd *pBook, CString strColor);

	/*
	 * 사용자가 지정한 배경색을 가져온다.
	 * 리턴값 배경색 (최초 로딩시는 "")
	 */
	const CString GetBackgroudColor(CWnd *pBook) const;

	/*
	 * 색상 변경시 바로 적용됨
	 * 파라미터 pBook 책의 인스턴스 포인터
	 *			foregroundClr 폰트색
	 *			backgroundClr 배경색
	 */
	BOOL SetColors(CWnd *pBook, CString foregroundClr, CString backgroundClr);




	/******************************************************************
	 * Get Book's Information
	 ******************************************************************/
	/*
	 * IsOpend
	 * 임의의 경로에 있는 책이 열려 있는지 여부를 확인하는 함수
	 * 파라미터 lpszFileName	원본 책의 절대 경로
	 * 리턴값	Opened(TRUE)/Not Open(FALSE)
	 */
	const BOOL			IsOpend(LPCTSTR lpszFileName);

	/*
	 * 모든 책이 정상적으로 load 되었는지 확인하는 함수
	 * 리턴값	Loaded(TRUE)/Not Loaded(FALSE)
	 */
	const BOOL			IsLoaded();
	
	/*
	 * 특정 책이 정상적으로 load 되었는지 확인하는 함수
	 * 파라미터 pBook	책의 인스턴스
	 * 리턴값	Loaded(TRUE)/Not Loaded(FALSE)
	 */
	const BOOL			IsLoaded(CWnd *pBook) const;

	/*
	 * 특정 책이 정상적으로 load 되었는지 확인하는 함수
	 * 파라미터 lpszFileName	원본 책의 절대 경로
	 * 리턴값	Loaded(TRUE)/Not Loaded(FALSE)
	 */
	const BOOL			IsLoaded(LPCTSTR lpszFileName);

	/*
	 * 임의의 책이 fixed 인지 reflow 컨텐츠 인지 확인 가능하는 함수
	 * 파라미터 pBook	fixed/reflow 를 확인하고자 하는 책의 인스턴스 포인터
	 * 리턴값	true(fixed)/false(reflow)
	 */
	const BOOL			IsFixed(CWnd *pBook) const;

	/*
	 * GetUUIDString
	 * 책의 UUID 를 가져온다
	 * 파라미터	pBook		책의 인스턴스
	 * 리턴값 uuid
	 */
	const CString		GetUUIDString(CWnd *pBook) const;

	/*
	 * GetVersion
	 * 책의 version 을 가져온다
	 * 파라미터	pBook		책의 인스턴스
	 * 리턴값 version
	 */
	const CString		GetVersion(CWnd *pBook) const;

	/*
	 * GetTitle
	 * 책의 title 을 가져온다
	 * 파라미터	pBook		책의 인스턴스
	 * 리턴값 title
	 */
	const CString		GetTitle(CWnd *pBook) const;

	/*
	 * GetCreator
	 * 책의 Creator 을 가져온다
	 * 파라미터	pBook		책의 인스턴스
	 * 리턴값 Creator
	 */
	const CString		GetCreator(CWnd *pBook) const;

	/*
	 * GetPublisher
	 * 책의 Publisher 을 가져온다
	 * 파라미터	pBook		책의 인스턴스
	 * 리턴값 Publisher
	 */
	const CString		GetPublisher(CWnd *pBook) const;

	/*
	 * GetDesc
	 * 책의 description 을 가져온다
	 * 파라미터	pBook		책의 인스턴스
	 * 리턴값 description
	 */
	const CString		GetDesc(CWnd *pBook) const;

	/*
	 * GetCoverHref
	 * 책의 cover image path 을 가져온다
	 * 파라미터	pBook		책의 인스턴스
	 * 리턴값 cover image path
	 */
	const CString		GetCoverHref(CWnd *pBook) const;

	/*
	 * GetOriginalEpubPath
	 * 책의 open 시 전달받아던 책의 원본 절대 경로를 반환한다.
	 * 파라미터	pBook		책의 인스턴스
	 * 리턴값 원본 절대 경로
	 */
	const CString		GetOriginalEpubPath(CWnd *pBook) const;

	/*
	 * GetTableOfContent
	 * 컨텐츠의 목차 정보를 가져온다.
	 * 파라미터	pBook		책의 인스턴스
	 * 리턴값	목차 정보 (트리구조)
	 */
	IN3NavPointNode*	GetTableOfContent(CWnd *pBook);

	/*
	 * GetCurrentPage
	 * 현재 페이지를 가져온다.
	 * 파라미터	pBook		책의 인스턴스
	 */
	int GetCurrentPage(CWnd *pBook);

	/*
	 * GetTotalPage
	 * 총 페이지를 가져온다.
	 * 파라미터	pBook		책의 인스턴스
	 */
	int GetTotalPage(CWnd *pBook);

	/*
	 * GetCurrentChapter
	 * 현재 챕터를 가져온다.
	 * 파라미터	pBook		책의 인스턴스
	 */
	int GetCurrentChapter(CWnd *pBook);

	/*
	 * GetTotalChapter
	 * 총 챕터(spine) 개수를 가져온다.
	 * 파라미터	pBook		책의 인스턴스
	 */
	int GetTotalChapter(CWnd *pBook);

	/*
	 * fixed layout 에서 만 사용 가능. reflow content 는 무조건 false 리턴.
	 * 임의의 페이지를 임의 사이즈의 이미지로 캡쳐 한다.
	 * 파라미터	pBook		책의 인스턴스
	 *			page		캡쳐할 페이지 번호 (1 ~ max)
	 *			width		이미지 가로 사이즈
	 *			height		이미지 세로 사이즈
	 *	  [out] bitmap		캡쳐한 이미지 
	 * 리턴값 true(success)/false(fail)
	 */
	bool PageCapture(CWnd *pBook, int page, int width, int height, CBitmap &bitmap);




	/********************************************************************************
	 * Interface for UI
	 ********************************************************************************/
	/*
	 * SetUIID
	 * UI단에서 Epub display 관련하여 관리하고 있는 id
	 * (ex, tab control 에 Epub 을 붙이는 경우 tab's id )
	 * 파라미터 id	UI 단에 서 관리하는 아이디
	 */
	void SetUIID(CWnd *pBook, int id);

	/*
	 * GetUIID
	 * UI 단에서 관리하는 id (ex tab control 사용하는 경우 tab's id)
	 * 파라미터 lpszFileName 원본 epub 파일의 절대 경로
	 */
	const int  GetUIID(CString lpszFileName);

	/*
	 * GetUIID
	 * UI 단에서 관리하는 id (ex tab control 사용하는 경우 tab's id)
	 * 파라미터 pBook 책 인스턴스
	 */
	const int  GetUIID(CWnd *pBook);



	/********************************************************************************
	 * page navigation
	 ********************************************************************************/
	/*
	 * MoveToFirstPage
	 * 책의 첫번째 페이지로 이동
	 * 파라미터 pBook 책 인스턴스 포인터
	 */
	void MoveToFirstPage(CWnd *pBook);

	/*
	 * MoveToLastPage
	 * 책의 마지막 페이지로 이동
	 * 파라미터 pBook 책 인스턴스 포인터
	 */
	void MoveToLastPage(CWnd *pBook);

	/*
	 * MoveToNextpage
	 * 책의 다음 페이지로 이동
	 * 파라미터 pBook 책 인스턴스 포인터
	 */
	void MoveToNextPage(CWnd *pBook);

	/*
	 * MoveToPrevPage
	 * 책의 이전 페이지로 이동
	 * 파라미터 pBook 책 인스턴스 포인터
	 */
	void MoveToPrevPage(CWnd *pBook);
	/*
	 * GotoPage
	 * 임의의 책을 임의의 페이지로 이동한다.
	 * 파라미터 pBook	이동할 책
	 *			page	이동할 페이지, (책의 전체 페이지를 기준으로 한다.)
	 */
	void MoveToPage(CWnd *pBook, unsigned int page);

	/*
	 * GotoChapter
	 * 임의의 책을 임의의 챕터로 이동한다.
	 * 파라미터 pBook	이동할 책
	 *			page	이동할 챕터 (정확히는 스파인의 인덱스이다.)
	 *			bDirectRight	진행 방향. TRUE(Right) / FALSE(Left)
	 */
	void MoveToChapter(CWnd *pBook, unsigned int  chapter, BOOL bDirectRight = TRUE);

	/*
	 * MoveToAnchor
	 * 책에 삽입된 anchor 가 있는 페이지로 이동한다.
	 * 파라미터 pBook		이동할 책
	 *			fileName	anchor 가 있는 파일이름
	 *			anchorID	이동할 anchor id (= marker id)
	 */
	void MoveToAnchor(CWnd *pBook, CString fileName, CString anchorID);

	/*
	 * MoveToFindResult
	 * FindTextAll() 한 결과 중 임의의 검색 결과가 있는 페이지로 이동하고자 할때 사용한다.
	 * 파라미터 pBook		이동할 책
	 *			fileName	검색 결과가 있는 파일 명
	 *			keyword		검색 키워드
	 *			seq			이동하고자 하는 검색 결과의 시퀀스 값
	 */
	void MoveToFindResult(CWnd *pBook, CString fileName, CString keyword, int seq);



	/********************************************************************************
	 * annotation
	 ********************************************************************************/
	/*
	 * CreateTextHighlight
	 * 하이라이트 추가
	 * 파라미터 pBook	책 인스턴스 포인터
	 *			color	하이라이트 칼라 ( ex. "rgba(x,x,x)" 와 같은 형태 )
	 *			hasMemo  메모가 추가되어 있는 경우 TRUE/ 메모 추가 안되어 있는 경우 FALSE
	 * 리턴값 TRUE(success)/FALSE(fail)
	 */
	BOOL CreateTextHighlight(CWnd *pBook, CString color, BOOL hasMemo = FALSE);
	
	/*
	 * CreateUnderline
	 * 언더라인 추가
	 * 파라미터 pBook	책 인스턴스 포인터
	 *			color	언더라인 칼라 ( ex. "rgba(x,x,x)" 와 같은 형태 )
	 *			hasMemo  메모가 추가되어 있는 경우 TRUE/ 메모 추가 안되어 있는 경우 FALSE
	 * 리턴값 TRUE(success)/FALSE(fail)
	 */
	BOOL CreateUnderline(CWnd *pBook, CString color, BOOL hasMemo = FALSE);

	/*
	 * DeleteAnnotation
	 * annotation 삭제
	 * 파라미터 pBook		책의 인스턴스 포인터
	 *			fileName	annotation이 추가된 파일명
	 *			markerID	annotation id
	 * 리턴값 TRUE(success)/FALSE(fail)
	 */
	BOOL DeleteAnnotation(CWnd *pBook, CString fileName, CString markerID);

	/*
	 * ModifyAnnotation
	 * annotation 수정 (하이라이트 색깔을 수정 가능)
	 * 파라미터 pBook	책의 인스턴스 포인터
	 *			pAnnotation	변경된 색깔 정보를 가지고 있는 IN3Annotatioin 인스턴스 포인터
	 * 리턴값 TRUE(success)/FALSE(fail)
	 */
	BOOL ModifyAnnotation(CWnd *pBook, IN3Annotation *pAnnotation);

	/*
	 * InitAnnotationList
	 * 책 open 후 책에 추가되어 있는 annotation 을 일괄 적용하는 함수.
	 * 책이 open 된 직후에 해당 함수를 호출한다.
	 * 파라미터 pBook  책의 인스턴스 포인터
	 *          annotations 추가된 annotaton list
	 * 리턴값 TRUE(success)/FALSE(fail)
	 */
	BOOL InitAnnotationList(CWnd *pBook, std::vector<IN3Annotation *> annotations);

	/*
	 * annoation 관련 메뉴를 띄울 위치를 조회한다.
	 * 해당 함수 호출 시 WM_ANNOTATION_POSITION 이벤트가 발생하고, 파라미터로 annotation 의 위치가 반환된다.
	 * 파라미터 pBook 조회할 책의 인스턴스 포인터
	 * 리턴값 TRUE(success)/FALSE(fail)
	 */
	BOOL QueryAnnotationToolbarPosition(CWnd *pBook);




	/********************************************************************************
	 * bookmark
	 ********************************************************************************/
	/*
	 * SetBookmark
	 * 북마크 추가
	 * 파라미터 pBook	책 인스턴스 포인터
	 * 리턴값 TRUE(success)/FALSE(fail)
	 */
	BOOL SetBookmark(CWnd *pBook);

	/*
	 * ClearBookmark
	 * 북마크 삭제 현재 보여지는 페이지의 북마크를 삭제한다.
	 * 파라미터 pBook	책 인스턴스 포인터
	 * 리턴값 TRUE(success)/FALSE(fail)
	 */
	BOOL ClearBookmark(CWnd *pBook);

	/*
	 * DeleteBookmark
	 * 북마크 삭제
	 * 파라미터 pBook	책 인스턴스 포인터
	 *			pBookmark	삭제할 북마크 인스턴스 포인터
	 * 리턴값 TRUE(success)/FALSE(fail)
	 */
	BOOL DeleteBookmark(CWnd *pBook, IN3Bookmark *pBookmark);

	/*
	 * DeleteBookmarkList
	 * 북마크 삭제
	 * 파라미터 pBook	책 인스턴스 포인터
	 *			bookmarks	삭제할 북마크의 리스트 포인터
	 * 리턴값 TRUE(success)/FALSE(fail)
	 */
	BOOL DeleteBookmarkList(CWnd *pBook, std::vector<IN3Bookmark *> bookmarks);

	/*
	 * InitBookmarkList
	 * 북마크 일괄 적용
	 * 책 open 후 책에 추가되어 있는 북마크를 일괄 적용하는 함수.
	 * 책이 open 된 직후에 해당 함수를 호출한다.
	 * 파라미터 pBook	책 인스턴스 포인터
	 *			bookmarks 북마크 리스트
	 * 리턴값 TRUE(success)/FALSE(fail)
	 */
	BOOL InitBookmarkList(CWnd *pBook, std::vector<IN3Bookmark *> bookmarks);




	/********************************************************************************
	 * Find Text
	 ********************************************************************************/
	/*
	 * FindTextALL
	 * 테스트 검색 전체
	 * 파라미터 pBook	책 인스턴스 포인터
	 *			keyword 키워드
	 */
	BOOL FindTextALL(CWnd *pBook, CString keyword);

	/*
	 * FindTextNext
	 * 테스트 검색 다음
	 * 파라미터 pBook	책 인스턴스 포인터
	 *			keyword 키워드
	 */
	BOOL FindTextNext(CWnd *pBook, CString keyword);

	/*
	 * FindTextPrev
	 * 테스트 검색 이전
	 * 파라미터 pBook	책 인스턴스 포인터
	 *			keyword 키워드
	 */
	BOOL FindTextPrev(CWnd *pBook, CString keyword);
};
