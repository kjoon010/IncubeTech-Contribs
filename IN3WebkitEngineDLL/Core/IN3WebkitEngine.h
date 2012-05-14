#pragma once


#include <map>
#include <vector>

#include "IN3WebkitDefine.h"

#if SUPPORT_VIEW_POOL_MANAGER
#include "ViewPoolManager.h"
#endif /*SUPPORT_VIEW_POOL_MANAGER*/

#include "IN3NavPointNode.h"
#include "IN3Setting.h"
#include "IN3Annotation.h"
#include "IN3ImageInfo.h"
#include "IN3TableInfo.h"


////////////////////////////////////////////////////////////////////////////////////////////
// IN3WebkitEngine

/**
 * @class	IN3WebkitEngine
 * @brief	랜더러 엔진 및 엔진 익스텐션이 구현되어 있는 최상위 클래스
 * @remarks	UI 에서 랜더링과 관련하여 요청 되어지는 내용은 IN3WebkitEngine 를 통하여 처리된다.
 */
class AFX_EXT_CLASS IN3WebkitEngine
{
public:
	// 생성자
	IN3WebkitEngine(void);

	// 소멸자
	~IN3WebkitEngine(void);

	BOOL LoadLibraries(void);

	CWnd* Open(CWnd *pParentWnd, LPCTSTR lpszFilePath, LPCTSTR lpszDescPath);
	void Close(LPCTSTR lpszFilePath);
	void Close(CWnd *pBook);

	BOOL CreateView(CWnd* pBook);
	BOOL CreateView(CWnd* pBook, CRuntimeClass* pRuntimeClass);

	void ReLayout();
	void ReLayout(CWnd* pBook);

	void ReLoad(CWnd* pBook);

	void SetUIID(CWnd *pBook, int id);
	const int  GetUIID(CString lpszFileName);
	const int  GetUIID(CWnd *pBook);

	void SetVisibleEpub(CString lpszFileName);
	void SetVisibleEpub(CWnd *pBook);

	void ViewZoomIn(CWnd *pBook);
	void ViewZoomOut(CWnd *pBook);
	void ViewFit(CWnd *pBook);

	int GetCurrentPage(CWnd *pBook);
	int GetTotalPage(CWnd *pBook);
	int GetCurrentChapter(CWnd *pBook);
	int GetTotalChapter(CWnd *pBook);

	bool PageCapture(CWnd *pBook, int page, int width, int height, CBitmap &bitmap);


	// init 
	BOOL InitViewFeatures(CWnd *pBook, CString fontFamily, int fontSize, CString fontColor, CString bgColor);
	BOOL InitAnnotationList(CWnd *pBook, std::vector<IN3Annotation *> annotations);
	BOOL InitBookmarkList(CWnd *pBook, std::vector<IN3Bookmark *> bookmarks);


	// engine's feature
	const CString GetEpubMainPath(void) const;
	const int GetPageDisplayMode(CWnd *pBook) const;
	const int GetFontSize(CWnd *pBook) const;
	const CString GetFontFamily(CWnd *pBook) const;
	const CString GetFontColor(CWnd *pBook) const;
	const CString GetBackgroudColor(CWnd *pBook) const;

	void SetDisplayPageNumber(bool bDisplay);
	void SetDisplayPageNumber(CWnd *pBook, bool bDisplay);
	void SetPageDisplayMode(CWnd *pBook, int mode);
	BOOL SetFontFamily(CWnd *pBook, CString fontFamilyName);
	BOOL SetFontSize(CWnd *pBook, int fontSize);
	BOOL SetFontColor(CWnd *pBook, CString strColor);
	BOOL SetBackgroudColor(CWnd *pBook, CString strColor);
	BOOL SetColors(CWnd *pBook, CString foregroundClr, CString backgroundClr);


	// Get Book's Information
	const BOOL			IsOpend(LPCTSTR lpszFileName);
	const BOOL			IsLoaded();
	const BOOL			IsLoaded(LPCTSTR lpszFileName);
	const BOOL			IsLoaded(CWnd *pBook) const;
	const BOOL			IsFixed(CWnd *pBook) const;				// fixed 여부 확인하는 함수
	const CString		GetUUIDString(CWnd *pBook) const;
	const CString		GetVersion(CWnd *pBook) const;
	const CString		GetTitle(CWnd *pBook) const;
	const CString		GetCreator(CWnd *pBook) const;
	const CString		GetPublisher(CWnd *pBook) const;
	const CString		GetDesc(CWnd *pBook) const;
	const CString		GetCoverHref(CWnd *pBook) const;
#if SUPPORT_THUMBNAIL_PATH		
	// thumbnail path 를 엔진에서 제공하는 경우, unzip 된 epub 경로 안에 thumbnail 폴더를 만들어 해당 경로를 반환한다.
	// 그러나 엔진에서 섬네일 경로를 지정하여 제공할 필요가 없으므로 해당 함수는 미지원하도록 처리함.
	const CString		GetThumbNailPath(CWnd *pBook) const;
#endif
	const CString		GetOriginalEpubPath(CWnd *pBook) const;

	IN3NavPointNode*	GetTableOfContent(CWnd *pBook);
	BOOL QueryImageList(CWnd *pBook);
	BOOL QueryTableList(CWnd *pBook);



	// page navigation
	void MoveToFirstPage(CWnd *pBook);
	void MoveToLastPage(CWnd *pBook);
	void MoveToNextPage(CWnd *pBook);
	void MoveToPrevPage(CWnd *pBook);
	void MoveToPage(CWnd *pBook, unsigned int page);
	void MoveToChapter(CWnd *pBook, unsigned int  chapter, BOOL bDirectRight = TRUE);
	void MoveToAnchor(CWnd *pBook, CString fileName, CString anchorID);
	void MoveToFindResult(CWnd *pBook, CString fileName, CString keyword, int seq);		// FindTextALL() 사용하여 검색된 결과목록에서 임의 결과 페이지로 이동할 때 사용

	
	// annotation
	BOOL CreateTextHighlight(CWnd *pBook, CString color, BOOL hasMemo = FALSE);
	BOOL CreateUnderline(CWnd *pBook, CString color, BOOL hasMemo = FALSE);
	BOOL DeleteAnnotation(CWnd *pBook, CString fileName, CString markerID);
	BOOL ModifyAnnotation(CWnd *pBook, IN3Annotation *pAnnotation);
	BOOL QueryAnnotationToolbarPosition(CWnd *pBook);


	// bookmark
	BOOL SetBookmark(CWnd *pBook);
	BOOL ClearBookmark(CWnd *pBook);
	BOOL DeleteBookmark(CWnd *pBook, IN3Bookmark *pBookmark);
	BOOL DeleteBookmarkList(CWnd *pBook, std::vector<IN3Bookmark *> bookmarks);


	// Find Text
	BOOL FindTextALL(CWnd *pBook, CString keyword);
	BOOL FindTextNext(CWnd *pBook, CString keyword);
	BOOL FindTextPrev(CWnd *pBook, CString keyword);
	


protected:
	CString				m_epubMainPath; ///< unzip 경로

#if SUPPORT_VIEW_POOL_MANAGER
	// hmkim 20120418 - 현재 뷰에 대한 컨트롤을 하고 있지 않으므로 제거한다.
	CViewPoolManager	m_viewPoolManager; ///< 엔진에 열려있는 모든 뷰를 관리하는 매니져. 
#endif /*SUPPORT_VIEW_POOL_MANAGER*/

	typedef std::map<CString, CWnd *, std::less<CString>> EBookList; ///< 북 리스트 객체 타입 선언
	EBookList		m_bookList; ///< 북 리스트
};
