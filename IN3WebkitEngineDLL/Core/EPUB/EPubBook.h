#pragma once

#include "BookInfo.h"

#include "IN3NavPointNode.h"
#include "TStringEx.h"
#include "XMLDom.h"

#include "IN3WebkitDefine.h"
#include "ViewManager.h"
#include "IN3Setting.h"
#include "IN3Annotation.h"
#include "IN3ImageInfo.h"
#include "IN3TableInfo.h"
#include "IN3FindResult.h"

#include <vector>

/**
 * @class	CEPubBook
 * @brief	Epub 책
 */
class CEPubBook : public CFrameWnd
{
	DECLARE_DYNCREATE(CEPubBook)

public:
	// 생성자
	CEPubBook();           

	// 소멸자
	virtual ~CEPubBook();
	
	// Interface for UI
	virtual BOOL Open(LPCTSTR lpszPathName);

#if !SUPPORT_VIEW_POOL_MANAGER
	BOOL	CreateView(CRuntimeClass* pRuntimeClass = NULL);
#endif

	void SetUIID(int id);
	const int GetUIID(void) const;

	virtual void ReLoad();
	virtual void ReLayout();
	virtual void ShowContextMenu(POINT pt);
	virtual void ShowAnnotationMenu(POINT pt, CString selectedText);

	void OpenPopupWeb(CString param); ///< web popup 을 띄우도록 UI 단으로 이벤트를 발생

	void ViewZoom(bool zoomin);
	void ViewFit();

	bool PageCapture(unsigned int page, int width, int height, CBitmap &bitmap);

	int GetCurrentPage();
	int GetTotalPage();
	int GetCurrentChapter();

	BOOL CancelSelection();
	BOOL CancelSelection(int chapter);


	// setting feature
	void SetDisplayPageNumber(bool bDisplay);
	void SetPageDisplayMode(int mode);
	void SetFontSize(int size);
	void SetMargin(int margin);
	void SetLineHeight(double height);
	void SetFontFamily(CString family);
	void SetFontColor(CString color);
	void SetBackgroudColor(CString color);
	void SetColors(CString foregroundClr, CString backgroundClr);

	const bool IsDisplayPageNumber();
	const int GetPageDisplayMode() const;
	const int GetFontSize() const;
	const int GetMargin() const;
	const double GetLineHeight() const;
	const CString GetFontFamily() const;
	const CString GetFontColor() const;
	const CString GetBackgroudColor() const;



	// Book's Information
	void GetBookInfo
	void SetOriginalEpubPath(LPCTSTR lpszOriginalPath);
	void SetFixedOriginalSize(int width, int height);
	void SetViewManager(CViewManager *pMgr);
	void SetCurrChapter(int chapter);

	const BOOL IsOpened(void) const;
	const BOOL IsLoaded(void) const;
	const BOOL IsFixed(void) const;

	const CString GetUUIDString(void) const;
	const CString GetVersion(void) const;
	const CString GetTitle(void) const;
	const CString GetCreator(void) const;
	const CString GetPublisher(void) const;
	const CString GetDesc(void) const;
	const CString GetCoverHref(void) const;
	
	const int GetFixedWidth() const;
	const int GetFixedHeight() const;

	const int GetSpineCount(void) const;

	const CString GetSpineURL(int idx) const;
	const CString GetOriginalEpubPath(void) const;
	const CString GetOPSFilePath(void); ///< Epub 파일의 content.opf 의 절대경로를 반환한다.
	const CString GetOPSFolderPath(void) const; ///< epub 책의 unzip 된 경로에 있는 패키지 폴더(OEBPS)의 절대 경로를 반환한다.
	
#if SUPPORT_THUMBNAIL_PATH
	// thumbnail path 를 엔진에서 제공하는 경우, unzip 된 epub 경로 안에 thumbnail 폴더를 만들어 해당 경로를 반환한다.
	// 그러나 엔진에서 섬네일 경로를 지정하여 제공할 필요가 없으므로 해당 함수는 미지원하도록 처리함.
	const CString		GetThumbnailPath(void);
#endif /*SUPPORT_THUMBNAIL_PATH*/

	IN3NavPointNode*	GetTableOfContent(void);
	BOOL QueryImageList(void);
	BOOL QueryTableList(void);



	// page navigation
	void CalculateTotalPage(BOOL bWaitGathering = TRUE);

	void MoveToChapter(unsigned int chapter, BOOL bDirectRight = TRUE);
	void MoveToChatperAndPage(unsigned int chapter, int page);
	void MoveToAnchor(CString fileName, CString anchorID);
	void MoveToPage(unsigned int page);
	void MoveToNextPage();
	void MoveToPrevPage();
	void MoveToFindResult(CString fileName, CString keyword, int seq);

	void OnMoveToAnchor(int page);
	void OnPageMoved();



	// annotation
	BOOL CreateTextHighlight(CString color, BOOL hasMemo = FALSE);
	BOOL CreateUnderline(CString color, BOOL hasMemo = FALSE);
	BOOL DeleteAnnotation(CString fileName, CString markerID);
	BOOL ModifyAnnotation(IN3Annotation *pAnnotation);
	BOOL InitAnnotationList(std::vector<IN3Annotation*> annotations);
	BOOL QueryAnnotationToolbarPosition();

	void OnAnnotationAdded(CString param, int index = 0);
	void OnAnnotationSelected(CString param);
	void OnMemoIconSelected(CString param);
	void OnAnnotationDeleted(CString param);
	void OnAnnotationModified(CString param);
	void OnQueryAnnotationPos(CString param);



	// bookmark
	BOOL SetBookmark();
	BOOL ClearBookmark();
	BOOL DeleteBookmark(IN3Bookmark *pBookmark);
	BOOL DeleteBookmarkList(std::vector<IN3Bookmark*> bookmarks);
	BOOL InitBookmarkList(std::vector<IN3Bookmark*> bookmarks);
	BOOL QueryBookmarkStatus();
	BOOL QueryBookmarkStatus(int chapter);

	void OnBookmarkAdded(CString param, int index = 0);
	void OnBookmarkCleared(CString param);
	void OnBookmarkDeleted(CString param);
	void OnEventBookmarkStatus(CString param, int fixedpage);

	

	// Find Text
	BOOL FindTextALL(CString keyword);
	BOOL FindTextAndMove(CString keyword, bool direction);
	BOOL FindTextAndMove(CString keyword, bool direction, int chapter);
	void OnFindText(bool bExistNext);
	void OnFindTextInNextChapter(CString keyword, int chapter);
	void OnFindTextInPrevChapter(CString keyword, int chapter);
	void OnFindTextAll(CString param);

	FindResultList* GetFindTextAllResult();
	int GetFindTextAllResultCount();


	// interface
	BOOL ReadOPSFile(LPCTSTR lpszPathName);
	BOOL ReadTocFile();
	BOOL ParseParam(CString param, CString key, void *resValue);
	BOOL ParseParamToString(CString param, CString key, CString &resValue);
	BOOL ParseFindTextParam(CString param, bool &bMatches, bool &bForward, int &page, CString &keyword);

	
protected:
	bool GetPackageFileName(LPCTSTR lpszFilePath, CString &oOPSFolderName, CString &oOPSFileName);
	bool GetFixedFeature(LPCTSTR lpszFilePath);
	bool GetAppleFeature(LPCTSTR lpszFilePath);
	bool GetSamsungFeature(LPCTSTR lpszFilePath);

#if SUPPORT_DELETE_UNZIPDIR
	bool DeleteFolderAndFiles(LPCTSTR lpszFilePath);
#endif

	int	 GetSpineIndex(CString fileName);

	void CopyMemoIcons();

	void AppendNavPointOfNCX(IN3NavPointNode &navPointNode, CXMLDOMNodeList &navPoints);
	void AppendNavPointOfNAV(IN3NavPointNode &navPointNode, CXMLDOMNodeList &navPoints);

	void DocumentLoadedComplate();

	BOOL ChangeView(void *pPrev, void *pNext);
	BOOL ChapgeViewFixedDual(void* pPrev, void *pNext);

	BOOL ReLayoutReflow();
	BOOL ReLayoutFixedSingle();
	BOOL ReLayoutFixedDual();

	BOOL ParseAnntationParam(CString param, void *pAnnotation);
	BOOL ParseBookmarkParam(CString param, void *pBookmark);


protected:
	int						m_uiID; ///< ui단에서 사용하는 각 epub 에 대한 id 값. ex)tab control 을 사용하는 경우는 tab의 id를 m_uiID로 설정한다.
	BOOL					m_bOpened; ///< 책이 오픈되어있는지에 대한 정보
	BOOL					m_bInitialized; ///< 책이 사용자가 읽을 수 있도록 초기화(epub 파싱 완료. 레이아웃 적용)되었는지에 대한 정보
	CBookInfo*				m_pBookInfo; ///< 책 정보의 포인터 변수
	IN3Setting*				m_pSetting; ///< 책의 뷰잉과 관련된 설정 값들의 포인터
	CViewManager*			m_pViewManager; ///< 책에 포함된 뷰어들을 관리하는 매니저

	std::vector<CString>	m_spineList; ///< spine 리스트
	IN3NavPointNode			m_tocTree; ///< 목차의 루트 데이터

	FindResultList*			m_pResultList; ///< 검색 결과 리스트

	unsigned int			m_nGatheringInit; ///< spine 초기화 변수(하나의 spine 파일이 초기화 될때마다 값이 증가한다.)
	int						m_nTotalPage; ///< 책의 총 페이지 수


protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnClose();
};


