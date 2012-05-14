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
 * @brief	Epub å
 */
class CEPubBook : public CFrameWnd
{
	DECLARE_DYNCREATE(CEPubBook)

public:
	// ������
	CEPubBook();           

	// �Ҹ���
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

	void OpenPopupWeb(CString param); ///< web popup �� ��쵵�� UI ������ �̺�Ʈ�� �߻�

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
	const CString GetOPSFilePath(void); ///< Epub ������ content.opf �� �����θ� ��ȯ�Ѵ�.
	const CString GetOPSFolderPath(void) const; ///< epub å�� unzip �� ��ο� �ִ� ��Ű�� ����(OEBPS)�� ���� ��θ� ��ȯ�Ѵ�.
	
#if SUPPORT_THUMBNAIL_PATH
	// thumbnail path �� �������� �����ϴ� ���, unzip �� epub ��� �ȿ� thumbnail ������ ����� �ش� ��θ� ��ȯ�Ѵ�.
	// �׷��� �������� ������ ��θ� �����Ͽ� ������ �ʿ䰡 �����Ƿ� �ش� �Լ��� �������ϵ��� ó����.
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
	int						m_uiID; ///< ui�ܿ��� ����ϴ� �� epub �� ���� id ��. ex)tab control �� ����ϴ� ���� tab�� id�� m_uiID�� �����Ѵ�.
	BOOL					m_bOpened; ///< å�� ���µǾ��ִ����� ���� ����
	BOOL					m_bInitialized; ///< å�� ����ڰ� ���� �� �ֵ��� �ʱ�ȭ(epub �Ľ� �Ϸ�. ���̾ƿ� ����)�Ǿ������� ���� ����
	CBookInfo*				m_pBookInfo; ///< å ������ ������ ����
	IN3Setting*				m_pSetting; ///< å�� ���װ� ���õ� ���� ������ ������
	CViewManager*			m_pViewManager; ///< å�� ���Ե� ������ �����ϴ� �Ŵ���

	std::vector<CString>	m_spineList; ///< spine ����Ʈ
	IN3NavPointNode			m_tocTree; ///< ������ ��Ʈ ������

	FindResultList*			m_pResultList; ///< �˻� ��� ����Ʈ

	unsigned int			m_nGatheringInit; ///< spine �ʱ�ȭ ����(�ϳ��� spine ������ �ʱ�ȭ �ɶ����� ���� �����Ѵ�.)
	int						m_nTotalPage; ///< å�� �� ������ ��


protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnClose();
};


