#pragma once

#include "IN3NavPointNode.h"
#include "IN3Annotation.h"

////////////////////////////////////////////////////////////////////////////////////////////
// IN3WebkitEngine
// ������ ���� �� ���� �ͽ��ټ��� �����Ǿ� �ִ� �ֻ��� Ŭ����
// UI ���� �������� �����Ͽ� ��û �Ǿ����� ������ IN3WebkitEngine �� ���Ͽ� ó���ȴ�.

class IN3WebkitEngine
{
public:
	IN3WebkitEngine(void);
	~IN3WebkitEngine(void);

	/*
	 * LoadLibraries
	 * Epub Open �ÿ� Load �Ǵ� ���̺귯������ ���� Load �Ѵ�.
	 * ���� Epub �� �� ������ ���� Library ���� Load �Ǿ� �ӵ� ������ ������ �ȴ�.
	 * ���� �̸� ���̺귯������ Load �� �θ� Epub Open �ÿ� �ҿ�Ǵ� �ӵ��� ���� �� �ִ�.
	 */
	BOOL LoadLibraries(void);
	
	/*
	 * Open
	 * ������ å�� ����.
	 * �Ķ���� pParentWnd		������ å ��ü�� ParentWnd ������
	 *			lpszPathName	���� å�� ���� ���
	 *			lpszDescPath	unzip �� ���� ���
	 * ���ϰ�	������ å ��ü
	 */
	CWnd* Open(CWnd *pParentWnd, LPCTSTR lpszFilePath, LPCTSTR lpszDescPath);

	/*
	 * Close
	 * å�� �ݴ´�.
	 * �������� ������ �ִ� å ����Ʈ���� ����
	 * �Ķ���� lpszFilePath	������ å�� ���� ���� ���
	 */
	void Close(LPCTSTR lpszFilePath);

	/*
	 * Close
	 * å�� �ݴ´�.
	 * �������� ������ �ִ� å ����Ʈ���� ����
	 * �Ķ���� pBook	������ å
	 */
	void Close(CWnd *pBook);

	/*
	 * å�� �ʿ��� �並 �����ϴ� �Լ�
	 * ������ ������ �並 �����Ѵ�.
	 * �Ķ���� pBook	�並 ������ å �ν��Ͻ� ������
	 */
	BOOL CreateView(CWnd* pBook);

	/*
	 * å�� �ʿ��� �並 �����ϴ� �Լ�
	 * ������ ������ �並 �����Ѵ�.
	 * �Ķ���� pBook	�並 ������ å �ν��Ͻ� ������
	 *			pRuntimeClass	�����ϰ��� �ϴ� ���� ��Ÿ�� Ŭ����
	 */
	BOOL CreateView(CWnd* pBook, CRuntimeClass* pRuntimeClass);

	/*
	 * ������ ũ�� ������� ������ ȭ�� ���̾ƿ��� ������ �ؾ� �ϴ� ��� ����Ѵ�.
	 * ���� �����ִ� ��� å�� Layout �� ������
	 */
	void ReLayout();

	/*
	 * ������ ũ�� ������� ������ ȭ�� ���̾ƿ��� ������ �ؾ� �ϴ� ��� ����Ѵ�.
	 * ������ å�� Layout �� ������
	 * �Ķ���� pBook ���̾ƿ��� �� ���� �� å�� �ν��Ͻ� ������
	 */
	void ReLayout(CWnd* pBook);

	/*
	 * ������ å�� �� ������ �Ѵ�.
	 * �Ķ���� pBook Reload �� å�� �ν��Ͻ�  ������
	 */
	void ReLoad(CWnd* pBook);

	/*
	 * ������ å�� view �� ���̰� ��.
	 * �Ķ���� lpszFileName å�� ���� ������ ���� ���
	 */
	void SetVisibleEpub(CString lpszFileName);

	/*
	 * ������ å�� view �� ���̰� ��.
	 * �Ķ���� pBook å�� �ν��Ͻ� ������
	 */
	void SetVisibleEpub(CWnd *pBook);

	/*
	 * ViewZoomIn
	 * Ȯ��
	 * �Ķ���� pBook å�� �ν��Ͻ� ������
	 */
	void ViewZoomIn(CWnd *pBook);

	/*
	 * ViewZoomOut
	 * ���
	 * �Ķ���� pBook å�� �ν��Ͻ� ������
	 */
	void ViewZoomOut(CWnd *pBook);

	/*
	 * ViewFit
	 * ����
	 * �Ķ���� pBook å�� �ν��Ͻ� ������
	 */
	void ViewFit(CWnd *pBook);





	/******************************************************************
	 * engine's feature	
	 ******************************************************************/
	/*
	 * GetEpubMainPath
	 * open �� epub ���ϵ��� ���� ���� ��θ� ��ȯ�Ѵ�
	 * ���ϰ�	open �� epub ���ϵ��� ���� ���� ���
	 */	
	const CString GetEpubMainPath(void) const;

	/*
	 * SetPageDisplayMode
	 * ������ ��带 �����Ѵ�. (�ܸ�/���)
	 * �Ķ���� pBook	������ ��带 ������ å
	 *			mode	1 : �ܸ�, 2 : ���
	 */
	void SetPageDisplayMode(CWnd *pBook, int mode);

	/*
	 * GetPageDisplayMode
	 * ���� ������ ��带 ��ȯ�Ѵ�.
	 * �Ķ���� pBook	������ ��带 ������ å
	 */
	const int GetPageDisplayMode(CWnd *pBook) const;

	/*
	 * ���� �Ʒ� ������ ������ ��ȣ ǥ�� ���θ� �����Ѵ�.
	 * �Ķ���� bDisplay ������ ��ȣ ǥ��
	 */
	void SetDisplayPageNumber(bool bDisplay);

	/*
	 * ������ å�� ���� �Ʒ� ������ ������ ��ȣ ǥ�� ���θ� �����Ѵ�.
	 * �Ķ���� pBook  å�� �ν��Ͻ�  ������
	 *			bDisplay ������ ��ȣ ǥ��
	 */
	void SetDisplayPageNumber(CWnd *pBook, bool bDisplay);

	/*
	 * ����Ʈ ��Ʈ�� ����� ��ü�� ���ؼ� ��ü�� �����Ѵ�.
	 * ���� �� ReLayout() �� �ؾ� ������ ����ȴ�.
	 * �Ķ���� pBook  å�� �ν��Ͻ�  ������
	 *			fontFamilyName ������ ��ü ��
	 */
	BOOL SetFontFamily(CWnd *pBook, CString fontFamilyName);

	/*
	 * ����ڰ� ������ ����Ʈ ��Ʈ�� ��ü�� �����´�.
	 * ���ϰ� ����Ʈ ��ü (���� �ε��ô� "")
	 */
	const CString GetFontFamily(CWnd *pBook) const;

	/*
	 * ���� ������ ��Ʈ�� ũ�⸦ �����Ѵ�.
	 * ���� �� ReLayout() �� �ؾ� ������ ����ȴ�.
	 * �Ķ���� pBook  å�� �ν��Ͻ�  ������
	 *			fontSize ��ü ũ��
	 */
	BOOL SetFontSize(CWnd *pBook, int fontSize);

	/*
	 * ����ڰ� ������ ��Ʈ�� ũ�⸦ �����´�.
	 * ���ϰ� ��ü ������ (���� �ε��ô� "0")
	 */
	const int GetFontSize(CWnd *pBook) const;

	/*
	 * ���� ������ ��Ʈ�� ���� �����Ѵ�.
	 * ���� �� ReLayout() �� �ؾ� ������ ����ȴ�.
	 * �Ķ���� pBook  å�� �ν��Ͻ�  ������
	 *			strColor ��Ʈ ����
	 */
	BOOL SetFontColor(CWnd *pBook, CString strColor);

	/*
	 * ����ڰ� ������ ��Ʈ�� ���� �����´�.
	 * ���ϰ� ��Ʈ�� ���� (���� �ε��ô� "")
	 */
	const CString GetFontColor(CWnd *pBook) const;

	/*
	 * ���� ������ �����Ѵ�.
	 * ���� �� ReLayout() �� �ؾ� ������ ����ȴ�.
	 * �Ķ���� pBook  å�� �ν��Ͻ�  ������
	 *			strColor ����
	 */
	BOOL SetBackgroudColor(CWnd *pBook, CString strColor);

	/*
	 * ����ڰ� ������ ������ �����´�.
	 * ���ϰ� ���� (���� �ε��ô� "")
	 */
	const CString GetBackgroudColor(CWnd *pBook) const;

	/*
	 * ���� ����� �ٷ� �����
	 * �Ķ���� pBook å�� �ν��Ͻ� ������
	 *			foregroundClr ��Ʈ��
	 *			backgroundClr ����
	 */
	BOOL SetColors(CWnd *pBook, CString foregroundClr, CString backgroundClr);




	/******************************************************************
	 * Get Book's Information
	 ******************************************************************/
	/*
	 * IsOpend
	 * ������ ��ο� �ִ� å�� ���� �ִ��� ���θ� Ȯ���ϴ� �Լ�
	 * �Ķ���� lpszFileName	���� å�� ���� ���
	 * ���ϰ�	Opened(TRUE)/Not Open(FALSE)
	 */
	const BOOL			IsOpend(LPCTSTR lpszFileName);

	/*
	 * ��� å�� ���������� load �Ǿ����� Ȯ���ϴ� �Լ�
	 * ���ϰ�	Loaded(TRUE)/Not Loaded(FALSE)
	 */
	const BOOL			IsLoaded();
	
	/*
	 * Ư�� å�� ���������� load �Ǿ����� Ȯ���ϴ� �Լ�
	 * �Ķ���� pBook	å�� �ν��Ͻ�
	 * ���ϰ�	Loaded(TRUE)/Not Loaded(FALSE)
	 */
	const BOOL			IsLoaded(CWnd *pBook) const;

	/*
	 * Ư�� å�� ���������� load �Ǿ����� Ȯ���ϴ� �Լ�
	 * �Ķ���� lpszFileName	���� å�� ���� ���
	 * ���ϰ�	Loaded(TRUE)/Not Loaded(FALSE)
	 */
	const BOOL			IsLoaded(LPCTSTR lpszFileName);

	/*
	 * ������ å�� fixed ���� reflow ������ ���� Ȯ�� �����ϴ� �Լ�
	 * �Ķ���� pBook	fixed/reflow �� Ȯ���ϰ��� �ϴ� å�� �ν��Ͻ� ������
	 * ���ϰ�	true(fixed)/false(reflow)
	 */
	const BOOL			IsFixed(CWnd *pBook) const;

	/*
	 * GetUUIDString
	 * å�� UUID �� �����´�
	 * �Ķ����	pBook		å�� �ν��Ͻ�
	 * ���ϰ� uuid
	 */
	const CString		GetUUIDString(CWnd *pBook) const;

	/*
	 * GetVersion
	 * å�� version �� �����´�
	 * �Ķ����	pBook		å�� �ν��Ͻ�
	 * ���ϰ� version
	 */
	const CString		GetVersion(CWnd *pBook) const;

	/*
	 * GetTitle
	 * å�� title �� �����´�
	 * �Ķ����	pBook		å�� �ν��Ͻ�
	 * ���ϰ� title
	 */
	const CString		GetTitle(CWnd *pBook) const;

	/*
	 * GetCreator
	 * å�� Creator �� �����´�
	 * �Ķ����	pBook		å�� �ν��Ͻ�
	 * ���ϰ� Creator
	 */
	const CString		GetCreator(CWnd *pBook) const;

	/*
	 * GetPublisher
	 * å�� Publisher �� �����´�
	 * �Ķ����	pBook		å�� �ν��Ͻ�
	 * ���ϰ� Publisher
	 */
	const CString		GetPublisher(CWnd *pBook) const;

	/*
	 * GetDesc
	 * å�� description �� �����´�
	 * �Ķ����	pBook		å�� �ν��Ͻ�
	 * ���ϰ� description
	 */
	const CString		GetDesc(CWnd *pBook) const;

	/*
	 * GetCoverHref
	 * å�� cover image path �� �����´�
	 * �Ķ����	pBook		å�� �ν��Ͻ�
	 * ���ϰ� cover image path
	 */
	const CString		GetCoverHref(CWnd *pBook) const;

	/*
	 * GetOriginalEpubPath
	 * å�� open �� ���޹޾ƴ� å�� ���� ���� ��θ� ��ȯ�Ѵ�.
	 * �Ķ����	pBook		å�� �ν��Ͻ�
	 * ���ϰ� ���� ���� ���
	 */
	const CString		GetOriginalEpubPath(CWnd *pBook) const;

	/*
	 * GetTableOfContent
	 * �������� ���� ������ �����´�.
	 * �Ķ����	pBook		å�� �ν��Ͻ�
	 * ���ϰ�	���� ���� (Ʈ������)
	 */
	IN3NavPointNode*	GetTableOfContent(CWnd *pBook);

	/*
	 * GetCurrentPage
	 * ���� �������� �����´�.
	 * �Ķ����	pBook		å�� �ν��Ͻ�
	 */
	int GetCurrentPage(CWnd *pBook);

	/*
	 * GetTotalPage
	 * �� �������� �����´�.
	 * �Ķ����	pBook		å�� �ν��Ͻ�
	 */
	int GetTotalPage(CWnd *pBook);

	/*
	 * GetCurrentChapter
	 * ���� é�͸� �����´�.
	 * �Ķ����	pBook		å�� �ν��Ͻ�
	 */
	int GetCurrentChapter(CWnd *pBook);

	/*
	 * GetTotalChapter
	 * �� é��(spine) ������ �����´�.
	 * �Ķ����	pBook		å�� �ν��Ͻ�
	 */
	int GetTotalChapter(CWnd *pBook);

	/*
	 * fixed layout ���� �� ��� ����. reflow content �� ������ false ����.
	 * ������ �������� ���� �������� �̹����� ĸ�� �Ѵ�.
	 * �Ķ����	pBook		å�� �ν��Ͻ�
	 *			page		ĸ���� ������ ��ȣ (1 ~ max)
	 *			width		�̹��� ���� ������
	 *			height		�̹��� ���� ������
	 *	  [out] bitmap		ĸ���� �̹��� 
	 * ���ϰ� true(success)/false(fail)
	 */
	bool PageCapture(CWnd *pBook, int page, int width, int height, CBitmap &bitmap);




	/********************************************************************************
	 * Interface for UI
	 ********************************************************************************/
	/*
	 * SetUIID
	 * UI�ܿ��� Epub display �����Ͽ� �����ϰ� �ִ� id
	 * (ex, tab control �� Epub �� ���̴� ��� tab's id )
	 * �Ķ���� id	UI �ܿ� �� �����ϴ� ���̵�
	 */
	void SetUIID(CWnd *pBook, int id);

	/*
	 * GetUIID
	 * UI �ܿ��� �����ϴ� id (ex tab control ����ϴ� ��� tab's id)
	 * �Ķ���� lpszFileName ���� epub ������ ���� ���
	 */
	const int  GetUIID(CString lpszFileName);

	/*
	 * GetUIID
	 * UI �ܿ��� �����ϴ� id (ex tab control ����ϴ� ��� tab's id)
	 * �Ķ���� pBook å �ν��Ͻ�
	 */
	const int  GetUIID(CWnd *pBook);



	/********************************************************************************
	 * page navigation
	 ********************************************************************************/
	/*
	 * MoveToFirstPage
	 * å�� ù��° �������� �̵�
	 * �Ķ���� pBook å �ν��Ͻ� ������
	 */
	void MoveToFirstPage(CWnd *pBook);

	/*
	 * MoveToLastPage
	 * å�� ������ �������� �̵�
	 * �Ķ���� pBook å �ν��Ͻ� ������
	 */
	void MoveToLastPage(CWnd *pBook);

	/*
	 * MoveToNextpage
	 * å�� ���� �������� �̵�
	 * �Ķ���� pBook å �ν��Ͻ� ������
	 */
	void MoveToNextPage(CWnd *pBook);

	/*
	 * MoveToPrevPage
	 * å�� ���� �������� �̵�
	 * �Ķ���� pBook å �ν��Ͻ� ������
	 */
	void MoveToPrevPage(CWnd *pBook);
	/*
	 * GotoPage
	 * ������ å�� ������ �������� �̵��Ѵ�.
	 * �Ķ���� pBook	�̵��� å
	 *			page	�̵��� ������, (å�� ��ü �������� �������� �Ѵ�.)
	 */
	void MoveToPage(CWnd *pBook, unsigned int page);

	/*
	 * GotoChapter
	 * ������ å�� ������ é�ͷ� �̵��Ѵ�.
	 * �Ķ���� pBook	�̵��� å
	 *			page	�̵��� é�� (��Ȯ���� �������� �ε����̴�.)
	 *			bDirectRight	���� ����. TRUE(Right) / FALSE(Left)
	 */
	void MoveToChapter(CWnd *pBook, unsigned int  chapter, BOOL bDirectRight = TRUE);

	/*
	 * MoveToAnchor
	 * å�� ���Ե� anchor �� �ִ� �������� �̵��Ѵ�.
	 * �Ķ���� pBook		�̵��� å
	 *			fileName	anchor �� �ִ� �����̸�
	 *			anchorID	�̵��� anchor id (= marker id)
	 */
	void MoveToAnchor(CWnd *pBook, CString fileName, CString anchorID);

	/*
	 * MoveToFindResult
	 * FindTextAll() �� ��� �� ������ �˻� ����� �ִ� �������� �̵��ϰ��� �Ҷ� ����Ѵ�.
	 * �Ķ���� pBook		�̵��� å
	 *			fileName	�˻� ����� �ִ� ���� ��
	 *			keyword		�˻� Ű����
	 *			seq			�̵��ϰ��� �ϴ� �˻� ����� ������ ��
	 */
	void MoveToFindResult(CWnd *pBook, CString fileName, CString keyword, int seq);



	/********************************************************************************
	 * annotation
	 ********************************************************************************/
	/*
	 * CreateTextHighlight
	 * ���̶���Ʈ �߰�
	 * �Ķ���� pBook	å �ν��Ͻ� ������
	 *			color	���̶���Ʈ Į�� ( ex. "rgba(x,x,x)" �� ���� ���� )
	 *			hasMemo  �޸� �߰��Ǿ� �ִ� ��� TRUE/ �޸� �߰� �ȵǾ� �ִ� ��� FALSE
	 * ���ϰ� TRUE(success)/FALSE(fail)
	 */
	BOOL CreateTextHighlight(CWnd *pBook, CString color, BOOL hasMemo = FALSE);
	
	/*
	 * CreateUnderline
	 * ������� �߰�
	 * �Ķ���� pBook	å �ν��Ͻ� ������
	 *			color	������� Į�� ( ex. "rgba(x,x,x)" �� ���� ���� )
	 *			hasMemo  �޸� �߰��Ǿ� �ִ� ��� TRUE/ �޸� �߰� �ȵǾ� �ִ� ��� FALSE
	 * ���ϰ� TRUE(success)/FALSE(fail)
	 */
	BOOL CreateUnderline(CWnd *pBook, CString color, BOOL hasMemo = FALSE);

	/*
	 * DeleteAnnotation
	 * annotation ����
	 * �Ķ���� pBook		å�� �ν��Ͻ� ������
	 *			fileName	annotation�� �߰��� ���ϸ�
	 *			markerID	annotation id
	 * ���ϰ� TRUE(success)/FALSE(fail)
	 */
	BOOL DeleteAnnotation(CWnd *pBook, CString fileName, CString markerID);

	/*
	 * ModifyAnnotation
	 * annotation ���� (���̶���Ʈ ������ ���� ����)
	 * �Ķ���� pBook	å�� �ν��Ͻ� ������
	 *			pAnnotation	����� ���� ������ ������ �ִ� IN3Annotatioin �ν��Ͻ� ������
	 * ���ϰ� TRUE(success)/FALSE(fail)
	 */
	BOOL ModifyAnnotation(CWnd *pBook, IN3Annotation *pAnnotation);

	/*
	 * InitAnnotationList
	 * å open �� å�� �߰��Ǿ� �ִ� annotation �� �ϰ� �����ϴ� �Լ�.
	 * å�� open �� ���Ŀ� �ش� �Լ��� ȣ���Ѵ�.
	 * �Ķ���� pBook  å�� �ν��Ͻ� ������
	 *          annotations �߰��� annotaton list
	 * ���ϰ� TRUE(success)/FALSE(fail)
	 */
	BOOL InitAnnotationList(CWnd *pBook, std::vector<IN3Annotation *> annotations);

	/*
	 * annoation ���� �޴��� ��� ��ġ�� ��ȸ�Ѵ�.
	 * �ش� �Լ� ȣ�� �� WM_ANNOTATION_POSITION �̺�Ʈ�� �߻��ϰ�, �Ķ���ͷ� annotation �� ��ġ�� ��ȯ�ȴ�.
	 * �Ķ���� pBook ��ȸ�� å�� �ν��Ͻ� ������
	 * ���ϰ� TRUE(success)/FALSE(fail)
	 */
	BOOL QueryAnnotationToolbarPosition(CWnd *pBook);




	/********************************************************************************
	 * bookmark
	 ********************************************************************************/
	/*
	 * SetBookmark
	 * �ϸ�ũ �߰�
	 * �Ķ���� pBook	å �ν��Ͻ� ������
	 * ���ϰ� TRUE(success)/FALSE(fail)
	 */
	BOOL SetBookmark(CWnd *pBook);

	/*
	 * ClearBookmark
	 * �ϸ�ũ ���� ���� �������� �������� �ϸ�ũ�� �����Ѵ�.
	 * �Ķ���� pBook	å �ν��Ͻ� ������
	 * ���ϰ� TRUE(success)/FALSE(fail)
	 */
	BOOL ClearBookmark(CWnd *pBook);

	/*
	 * DeleteBookmark
	 * �ϸ�ũ ����
	 * �Ķ���� pBook	å �ν��Ͻ� ������
	 *			pBookmark	������ �ϸ�ũ �ν��Ͻ� ������
	 * ���ϰ� TRUE(success)/FALSE(fail)
	 */
	BOOL DeleteBookmark(CWnd *pBook, IN3Bookmark *pBookmark);

	/*
	 * DeleteBookmarkList
	 * �ϸ�ũ ����
	 * �Ķ���� pBook	å �ν��Ͻ� ������
	 *			bookmarks	������ �ϸ�ũ�� ����Ʈ ������
	 * ���ϰ� TRUE(success)/FALSE(fail)
	 */
	BOOL DeleteBookmarkList(CWnd *pBook, std::vector<IN3Bookmark *> bookmarks);

	/*
	 * InitBookmarkList
	 * �ϸ�ũ �ϰ� ����
	 * å open �� å�� �߰��Ǿ� �ִ� �ϸ�ũ�� �ϰ� �����ϴ� �Լ�.
	 * å�� open �� ���Ŀ� �ش� �Լ��� ȣ���Ѵ�.
	 * �Ķ���� pBook	å �ν��Ͻ� ������
	 *			bookmarks �ϸ�ũ ����Ʈ
	 * ���ϰ� TRUE(success)/FALSE(fail)
	 */
	BOOL InitBookmarkList(CWnd *pBook, std::vector<IN3Bookmark *> bookmarks);




	/********************************************************************************
	 * Find Text
	 ********************************************************************************/
	/*
	 * FindTextALL
	 * �׽�Ʈ �˻� ��ü
	 * �Ķ���� pBook	å �ν��Ͻ� ������
	 *			keyword Ű����
	 */
	BOOL FindTextALL(CWnd *pBook, CString keyword);

	/*
	 * FindTextNext
	 * �׽�Ʈ �˻� ����
	 * �Ķ���� pBook	å �ν��Ͻ� ������
	 *			keyword Ű����
	 */
	BOOL FindTextNext(CWnd *pBook, CString keyword);

	/*
	 * FindTextPrev
	 * �׽�Ʈ �˻� ����
	 * �Ķ���� pBook	å �ν��Ͻ� ������
	 *			keyword Ű����
	 */
	BOOL FindTextPrev(CWnd *pBook, CString keyword);
};
