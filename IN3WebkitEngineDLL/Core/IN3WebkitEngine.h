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
 * @brief	������ ���� �� ���� �ͽ��ټ��� �����Ǿ� �ִ� �ֻ��� Ŭ����
 * @remarks	UI ���� �������� �����Ͽ� ��û �Ǿ����� ������ IN3WebkitEngine �� ���Ͽ� ó���ȴ�.
 */
class AFX_EXT_CLASS IN3WebkitEngine
{
public:
	// ������
	IN3WebkitEngine(void);

	// �Ҹ���
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
	const BOOL			IsFixed(CWnd *pBook) const;				// fixed ���� Ȯ���ϴ� �Լ�
	const CString		GetUUIDString(CWnd *pBook) const;
	const CString		GetVersion(CWnd *pBook) const;
	const CString		GetTitle(CWnd *pBook) const;
	const CString		GetCreator(CWnd *pBook) const;
	const CString		GetPublisher(CWnd *pBook) const;
	const CString		GetDesc(CWnd *pBook) const;
	const CString		GetCoverHref(CWnd *pBook) const;
#if SUPPORT_THUMBNAIL_PATH		
	// thumbnail path �� �������� �����ϴ� ���, unzip �� epub ��� �ȿ� thumbnail ������ ����� �ش� ��θ� ��ȯ�Ѵ�.
	// �׷��� �������� ������ ��θ� �����Ͽ� ������ �ʿ䰡 �����Ƿ� �ش� �Լ��� �������ϵ��� ó����.
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
	void MoveToFindResult(CWnd *pBook, CString fileName, CString keyword, int seq);		// FindTextALL() ����Ͽ� �˻��� �����Ͽ��� ���� ��� �������� �̵��� �� ���

	
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
	CString				m_epubMainPath; ///< unzip ���

#if SUPPORT_VIEW_POOL_MANAGER
	// hmkim 20120418 - ���� �信 ���� ��Ʈ���� �ϰ� ���� �����Ƿ� �����Ѵ�.
	CViewPoolManager	m_viewPoolManager; ///< ������ �����ִ� ��� �並 �����ϴ� �Ŵ���. 
#endif /*SUPPORT_VIEW_POOL_MANAGER*/

	typedef std::map<CString, CWnd *, std::less<CString>> EBookList; ///< �� ����Ʈ ��ü Ÿ�� ����
	EBookList		m_bookList; ///< �� ����Ʈ
};
