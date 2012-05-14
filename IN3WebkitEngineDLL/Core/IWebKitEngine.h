// Developer : Ho Chul Lee
// Date : 2012-03-10
// E-mail : calmtot@nate.com
// Subject : Webkit Engine Interface.

#ifndef _IWEBKITENGINE_H_
#define _IWEBKITENGINE_H_

#include "IN3NavPointNode.h"
#include "IN3ImageInfo.h"
#include "IN3Annotation.h"

interface IWebkitEngine
{
	virtual BOOL LoadLibraries(void) = 0;

	virtual CWnd* Open(CWnd *pParentWnd, LPCTSTR lpszFilePath, LPCTSTR lpszDescPath) = 0;
	virtual void Close(CWnd *pBook) = 0;

	virtual BOOL CreateView(CWnd* pBook) = 0;
	virtual BOOL CreateView(CWnd* pBook, CRuntimeClass* pRuntimeClass) = 0;

	virtual void ReLayout() = 0;
	virtual void ReLayout(CWnd* pBook) = 0;

	virtual void SetVisibleEpub(CString lpszFileName) = 0;


	// engine's feature
	virtual const CString GetEpubMainPath(void) const = 0;

	virtual void SetPageDisplayMode(CWnd *pBook, int mode) = 0;
	virtual const int GetPageDisplayMode(CWnd *pBook) const = 0;


	// Get Book's Information
	virtual const BOOL IsOpend(LPCTSTR lpszFileName) = 0;
	virtual IN3NavPointNode*	GetTableOfContent(CWnd *pBook) = 0;
	virtual const CString		GetUUIDString(CWnd *pBook) const = 0;
	virtual const CString		GetVersion(CWnd *pBook) const = 0;
	virtual const CString		GetTitle(CWnd *pBook) const = 0;
	virtual const CString		GetCreator(CWnd *pBook) const = 0;
	virtual const CString		GetPublisher(CWnd *pBook) const = 0;
	virtual const CString		GetDesc(CWnd *pBook) const = 0;
	virtual const CString		GetCoverHref(CWnd *pBook) const = 0;
	
	virtual int GetCurrentPage(CWnd *pBook) = 0;
	virtual int GetTotalPage(CWnd *pBook) = 0;
	virtual int GetCurrentChapter(CWnd *pBook) = 0;
	virtual int GetTotalChapter(CWnd *pBook) = 0;

	// Interface for UI
	virtual void SetUIID(CWnd *pBook, int id) = 0;
	virtual const int  GetUIID(CString lpszFileName) = 0;


	// page navigation
	virtual void MoveToNextPage(CWnd *pBook) = 0;
	virtual void MoveToPrevPage(CWnd *pBook) = 0;
	virtual void MoveToPage(CWnd *pBook, unsigned int page) = 0;
	virtual void MoveToChapter(CWnd *pBook, unsigned int  chapter, BOOL bDirectRight = TRUE) = 0;
	virtual void MoveToAnchor(CWnd *pBook, CString fileName, CString anchorID) = 0;


	// annotation
	virtual BOOL CreateTextHighlight(CWnd *pBook, CString color, BOOL hasMemo = FALSE) = 0;
	virtual BOOL CreateUnderline(CWnd *pBook, CString color, BOOL hasMemo = FALSE) = 0;
	virtual BOOL DeleteAnnotation(CWnd *pBook, CString fileName, CString markerID) = 0;
	virtual BOOL ModifyAnnotation(CWnd *pBook, IN3Annotation *pAnnotation) = 0;

	// bookmark
	virtual BOOL SetBookmark(CWnd *pBook) = 0;
	virtual BOOL ClearBookmark(CWnd *pBook) = 0;
	virtual BOOL DeleteBookmark(CWnd *pBook, IN3Bookmark *pBookmark) = 0;
	virtual BOOL DeleteBookmarkList(CWnd *pBook, std::vector<IN3Bookmark *> bookmarks) = 0;
	virtual BOOL InitBookmarkList(CWnd *pBook, std::vector<IN3Bookmark *> bookmarks) = 0;

};

#endif