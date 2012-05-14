// EPubBook.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "EPubBook.h"

#include "IN3WebkitDefine.h"
#include "IN3WebkitView.h"
#include "IN3Annotation.h"
#include "IN3WebPopupData.h"
#include "IN3FindResult.h"
#include "IN3SelectionData.h"
#include "IN3WebLib.h"

#include "OPFReader.h"
#include "NCXReader.h"
#include "NAVReader.h"

#include "JSON.h"


// CEPubBook

IMPLEMENT_DYNCREATE(CEPubBook, CFrameWnd)

CEPubBook::CEPubBook()
	:m_pBookInfo(NULL)
	,m_pViewManager(NULL)
	,m_pSetting(NULL)
	,m_bOpened(FALSE)
	,m_bInitialized(FALSE)
	,m_uiID(-1)
	,m_nGatheringInit(0)
	,m_nTotalPage(0)
	,m_pResultList(NULL)
{
}

CEPubBook::~CEPubBook()
{
	TRACEMSG(_T("CEPubBook::~CEPubBook() START"));
	//	
	// �˻� ��� ����Ʈ�� �ִٸ� �����Ѵ�.
	if(m_pResultList)
	{
		if(m_pResultList->size() > 0)
			m_pResultList->clear();
		delete m_pResultList;
	}

	// ���� ���� ����
	m_tocTree.Release();

	// spine ����Ʈ ����
	if (m_spineList.size() > 0)
		m_spineList.clear();

	// �� �Ŵ��� ����
	if(m_pViewManager)
		delete m_pViewManager;

	// å������ �����Ѵ�. unzip �� ������ ���� ����
	if(m_pBookInfo)
	{
	#if SUPPORT_DELETE_UNZIPDIR
		CString unzipPath = m_pBookInfo->GetUnzipRootPath();
		if(!unzipPath.IsEmpty())
			DeleteFolderAndFiles(m_pBookInfo->GetUnzipRootPath());
	#endif /*SUPPORT_DELETE_UNZIPDIR*/
		delete m_pBookInfo;
	}
	
	// ���� ���� ���� ���뵵 �����Ѵ�.
	if(m_pSetting)
		delete m_pSetting;
	//	
	TRACEMSG(_T("CEPubBook::~CEPubBook() END"));
}


BEGIN_MESSAGE_MAP(CEPubBook, CFrameWnd)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CEPubBook �޽��� ó�����Դϴ�.

/********************************************************************************
 * UI interface
 ********************************************************************************/
/**
 * @brief	epub ������ ����.
 * @remarks	epub ���� unzip �� ��θ� �����ϰ�, opf ������ ã�� �Ľ��Ѵ�.
 * @param lpszPathName	: epub ������ unzip �� ���(������)
 * @return	success(TRUE)/fail(FALSE)
 */
BOOL CEPubBook::Open(LPCTSTR lpszPathName)
{
	if(m_bOpened) return TRUE;
	//
	TRACEMSG(_T("CEPubBook::Open() START"));
	//
	// å open �÷��� ����
	m_bOpened = TRUE;
	//
	// å ���� ������ �����Ѵ�.
	m_pBookInfo = new CBookInfo;
	if(!m_pBookInfo)
	{
		TRACEMSG_ERR(_T("CEPubBook::Open() :: can't create CBookInfo!!"));
		return FALSE;
	}
	//
	// epub ������ unzip �� ��� ����
	m_pBookInfo->SetUnzipRootPath(lpszPathName);
	//
	// STEP 1) OPF ���� �� ������ �ִ� ��θ� "META-INF\\container.xml ������ �Ľ��Ͽ� �����´�.
	// "META-INF\\container.xml" ������ ������ �̸����� ������� �ƴ��Ѵ�.
	CString containerPath(lpszPathName);
	containerPath.Append(_T("META-INF\\container.xml"));
	containerPath.Replace(_T("/"), _T("\\"));
	//
	CString opfFilePath;
	CString packageName;
	if(GetPackageFileName(containerPath, packageName, opfFilePath))
	{
		m_pBookInfo->SetPakcageName(packageName);
		m_pBookInfo->SetOPSFilePath(opfFilePath);
		
	#if SUPPORT_THUMBNAIL_PATH
	// thumbnail path �� �������� �����ϴ� ���, unzip �� epub ��� �ȿ� thumbnail ������ ����� �ش� ��θ� ��ȯ�Ѵ�.
	// �׷��� �������� ������ ��θ� �����Ͽ� ������ �ʿ䰡 �����Ƿ� �ش� �Լ��� �������ϵ��� ó����.
		// optional : if viewer support thumbnail slide bar, than engine save thumbnail for page in this path
		CString thumbnailPath(m_pBookInfo->GetUnzipRootPath());
		thumbnailPath.Append(opfFolderName);
		thumbnailPath.Append(_T("\\thumbnail\\"));
		m_pBookInfo->SetThumbnailPath(thumbnailPath);
	#endif /*SUPPORT_THUMBNAIL_PATH*/
	}
	//
	// STEP 2) Fixed ������ ���� Ȯ���Ѵ�.
	m_pBookInfo->SetFixed(GetFixedFeature(lpszPathName));
	//
	if(!m_pSetting)
		m_pSetting = new IN3Setting;
	//
	TRACEMSG(_T("CEPubBook::Open() END"));
	//
	return TRUE;
}

#if !SUPPORT_VIEW_POOL_MANAGER
/**
 * @brief	å�� �ʿ��� ����� �����Ѵ�.
 * @param	���� ���� ��Ÿ�� Ŭ����. NULL �� ��� ����Ʈ ��� �����Ѵ�. (��Ÿ�� Ŭ���� ���� ��, �並 ������ �� ������ �־� ���� ���������� �ʰ� �ִ�.)
 * @remarks �������� �� pool �Ŵ����� �������� ���� ���, �� �Լ��� ����ؼ� å ��ü���� �ʿ��� �並 ���� �����Ѵ�.
 * @return	true(success)/false(fail)
 */
BOOL CEPubBook::CreateView(CRuntimeClass* pRuntimeClass /*= NULL*/)
{
	TRACEMSG(_T("CEPubBook::CreateView() START"));
	BOOL bRes = FALSE;
	CViewManager	*pViewMgr = new CViewManager;
	if(pViewMgr)
	{
		if(pRuntimeClass ==NULL)
			bRes = pViewMgr->CreateView(this);
		else
			bRes = pViewMgr->CreateView(this, pRuntimeClass);

		if(bRes)
		{
			m_pViewManager = pViewMgr;
			//SetViewManager(pViewMgr);
		}
		else
		{
			TRACEMSG_ERR(_T("CViewManager failed to create views"));
			delete pViewMgr;
		}
	}
	TRACEMSG(_T("CEPubBook::CreateView() END"));
	return bRes;
}
#endif /*!SUPPORT_VIEW_POOL_MANAGER*/

/**
 * @brief		epub ���� �� �߻��ϴ� �̺�Ʈ. 
 * @remarks	tab control ���� ������ tab �� �����ϴ� ��� �߻��Ѵ�. 
 * 			�̺�Ʈ�� �߻��Ͽ� OnClose() �Լ� ȣ�� �ÿ� ���� Epub �� ���������� �ʴ´�.
 * 			UI ������ epub ������ ���� WM_CLOSE_EPUB ����Ʈ�� �߻���Ų��.
 * 			UI �ܿ��� WebkitEngine API �� Close() �Լ��� ���� ���� epub �� �����ؾ� �Ѵ�.
 */
void CEPubBook::OnClose()
{
	TRACEMSG(_T("CEPubBook::OnClose() START"));
	//
	CWnd* pParentWnd = GetParent();
	if(pParentWnd)
		pParentWnd->SendMessage(WM_CLOSE_EPUB, (WPARAM)this, m_uiID);
	//
	TRACEMSG(_T("CEPubBook::OnClose() END"));
}

/**
 * @brief	UI�ܿ��� �� epub ���� �����ϰ� �����ϰ� �ִ� id��
 * @remarks	tab control �� Epub �� ���̴� ��� tab �� id�� �����Ѵ�.
 * @param	id	: UI �ܿ� �� �����ϴ� ���̵�
 */
void CEPubBook::SetUIID(int id)
{
	m_uiID = id;
}

/**
 * @brief	UI�ܿ��� �� epub ���� �����ϰ� �����ϰ� �ִ� id���� �����´�
 * @remarks	tab control �� Epub �� ���̴� ��� tab �� id���� ����.
 * @return	UI �ܿ� �� �����ϴ� ���̵�
 */
const int CEPubBook::GetUIID(void) const
{
	return m_uiID;
}

/**
 * @brief	epub �� Reload �Ѵ�.
 */
void CEPubBook::ReLoad()
{
	if(!m_pViewManager) return;

	for(UINT i = 0; i< m_spineList.size(); i++)
	{
		IN3WebkitView *pView = (IN3WebkitView *)m_pViewManager->GetView(i);
		pView->ReLoad();
	}
}

/**
 * @brief	epub �� ���̾ƿ��� ������ �Ѵ�.
 */
void CEPubBook::ReLayout()
{
	if(!m_pSetting){
		TRACEMSG_ERR(_T("m_pSetting is NULL"));
		return;
	}
	//
	m_bInitialized = TRUE;

	// fixed layout �������� reflow �������� ���̾ƿ��� �ٸ��� �����Ѵ�.
	if(IsFixed())
	{
		// fixed layout �� ��쿡��, ��麸��� �ܸ� ���⿡ ���� ���̾ƿ��� �ٸ��� �����Ѵ�.
		if(m_pSetting->GetPageDisplayMode() == 1)
			ReLayoutFixedSingle();
		else
			ReLayoutFixedDual();
	}
	else
		ReLayoutReflow();
}

/**
 * @brief	���ؽ�Ʈ �޴��� ��쵵�� UI ������ �̺�Ʈ�� �߻���Ų��.
 * @param	pt	: ���콺 Ŭ�� ��ġ
 */
void CEPubBook::ShowContextMenu(POINT pt)
{
	CWnd* pParentWnd = GetParent();
	if(pParentWnd)
		pParentWnd->SendMessage(WM_CONTEXTMENU_EPUB, (WPARAM)this, (LPARAM)&pt);
}

/**
 * @brief	annotation �޴��� ��쵵�� UI ������ �̺�Ʈ�� �߻���Ų��.
 * @param	pt				: ���콺 Ŭ�� ��ġ
 * @param	selectedText	: ����ڰ� ������ �ؽ�Ʈ
 */
void CEPubBook::ShowAnnotationMenu(POINT pt, CString selectedText)
{
	// ���콺 ��ġ �� ������ �ؽ�Ʈ ������ �������� UI ������ �Ѱ��ֱ� ���� �����͸� �����Ѵ�.
	IN3SelectionData *pSelection = new IN3SelectionData(pt, selectedText);
	if(pSelection)
	{
		CWnd* pParentWnd = GetParent();
		if(pParentWnd)
			pParentWnd->SendMessage(WM_ANNOTATIONMENU_EPUB, (WPARAM)this, (LPARAM)pSelection);

		delete pSelection;
	}
}

/**
 * @brief	web popup �� ��쵵�� UI ������ �̺�Ʈ�� �߻���Ų��.
 * @remarks	������� �̺�Ʈ�� ���� �������� ���� ȣ��Ǿ� ���� �Լ��̴�.
 * @param	param	: �������� ���� ���޹��� ������ web popup ������
 */
void CEPubBook::OpenPopupWeb(CString param)
{
	if(param.IsEmpty()) return;
	//
	BOOL bRes = TRUE;
	JSONValue* value = JSON::Parse(param);
	if(value == NULL)
	{
		TRACEMSG_ERR(_T("[CEPubBook::OpenPopupWeb] recived data can't parsing!!"));
		return;
	}

	if(value->IsObject() == false)
	{
		TRACEMSG_ERR(_T("[CEPubBook::OpenPopupWeb] The root element is not an object!!"));
		return;
	}

	JSONObject root;
	root = value->AsObject();
	JSONObject::iterator iter = root.find(_T("RESULT_CODE"));
	if(iter != root.end() && root[_T("RESULT_CODE")]->IsNumber()){
		if(root[_T("RESULT_CODE")]->AsNumber() != 0.0)
			return;
		/*CString result = root[_T("RESUOT_CODE")]->Stringify().c_str();
		result.Replace(_T("\\"), _T(""));
		if(result != _T("0"))
		{
			return FALSE;
		}*/
	}

	CString url;
	int width, height;

	// url
	iter = root.find(_T("open_url"));
	if(iter!= root.end()){
		url = root[_T("open_url")]->Stringify().c_str();
		url.Replace(L"&amp;", L"&");
	}
	//
	// popup width size
	iter = root.find(_T("popup_width"));
	if(iter!= root.end()){
		width = int(root[_T("popup_width")]->AsNumber());
	}
	//
	// popup height size
	iter = root.find(_T("popup_height"));
	if(iter!= root.end()){
		height = int(root[_T("popup_height")]->AsNumber());
	}
	//
	IN3WebPopupData *pPopup = new IN3WebPopupData(url, width, height);
	if(pPopup)
	{
		CWnd* pParentWnd = GetParent();
		if(pParentWnd)
			pParentWnd->SendMessage(WM_OPEN_WEB_POPUP, (WPARAM)this, (LPARAM)pPopup);

		delete pPopup;
	}

	if(value)
		delete value;
}

/**
 * @brief	fixed �������� ��� �並 Ȯ��/����Ѵ�.
 * @param	zoomin	: Ȯ��/��ҿ� ���� �÷���. TRUE: Ȯ��, FALSE: ���
 */
void CEPubBook::ViewZoom(bool zoomin)
{
	if(m_pViewManager == NULL) return;
	if(!IsFixed()) return;

	// ���� �������� �並 Ȯ��/��� �Ѵ�.
	IN3WebkitView* pView = (IN3WebkitView *)m_pViewManager->GetCurrentView();
	if(pView)
	{
		if(zoomin)
			pView->SetZoomIn();
		else
			pView->SetZoomOut();
	}
	//
	ReLayout();
}

/**
 * @brief	fixed �������� �並 ���� �������� ũ�⿡ ���� �����ش�.
 */
void CEPubBook::ViewFit()
{
	if(m_pViewManager == NULL) return;
	if(!IsFixed()) return;

	IN3WebkitView* pView = (IN3WebkitView *)m_pViewManager->GetCurrentView();
	if(pView)
	{
		pView->SetZoomStep(0);
	}
	//
	ReLayout();
}

/**
 * @brief	������ ȭ���� ������ ũ��� ĸ���Ѵ�.
 * @remarks fixed layout() ������ ��� ����
 * @param	[in] page		: ĸ���� ������
 * @param	[in] width		: ĸ�� �̹����� ���� �ȼ� ��
 * @param	[in] height		: ĸ�� �̹����� ���� �ȼ� ��
 * @param	[out] bitmap	: ĸ�� �̹���
 */
bool CEPubBook::PageCapture(unsigned int page, int width, int height, CBitmap &bitmap)
{
	if(!IsFixed()) return false;
	if(!m_pViewManager) return false;
	if(!m_pBookInfo) return false;
	//
	if(page > 0 && page <= m_spineList.size())
	{
		IN3WebkitView *pView = (IN3WebkitView *)m_pViewManager->GetView(page-1);
		if(pView)
		{			
			// ȭ�� ĸ�Ĵ� ���� �ε尡 �Ϸ�� ���ĺ��� �����ϴ�.
			if(pView->m_nLoadState == IN3WebkitView::LS_LODE_DONE)
			{
			#if 1
				IN3WebkitView *pCurrView = (IN3WebkitView *)m_pViewManager->GetCurrentView();
				if(pCurrView != pView)
				{
					int activePage = m_pViewManager->GetCurrChapter();
					MoveToChapter(page-1);
					//ChangeView(pCurrView, pView);
					bool bRes = pView->PageCapture(width, height, bitmap);
					//ChangeView(pView, pCurrView);
					MoveToChapter(activePage);
					return bRes;
				}
				else
			#endif
					return pView->PageCapture(width, height, bitmap);
			}
		}
	}

	return false;
}

/**
 * @brief	���� �������� ������ ���� ��ȯ�Ѵ�.
 * @return	���� �������� ������ ��
 */
int CEPubBook::GetCurrentPage()
{
	if(m_pViewManager)
	{
		int currPage = 0;
		int currChapter = m_pViewManager->GetCurrChapter();
		// ó������ ���� �������� é�� �������� �������� ����
		for(int i=0; i< currChapter; i++)
		{
			IN3WebkitView* pView = (IN3WebkitView *)m_pViewManager->GetView(i);
			if(pView)
				currPage += pView->GetTotalPage();
		}
		
		// ���� �������� ���Ϳ��� ���° �������� �����ִ��� Ȯ���Ͽ� ������ ���� ����
		IN3WebkitView* pView = (IN3WebkitView *)m_pViewManager->GetView(currChapter);
		if(pView)
			currPage += pView->GetCurrPage();

		return currPage;
	}

	return 0;
}

/**
 * @brief	å�� �� ������ ���� ��ȯ�Ѵ�.
 * @return	å�� �� ������ ��
 */
int CEPubBook::GetTotalPage()
{
	if(m_pViewManager)
		return m_pViewManager->GetTotalPage();

	return 0;
}

/**
 * @brief	���纸�� �ִ� é���� �ε��� ���� ��ȯ�Ѵ�. (spine index ���� ��ġ��)
 * @return	���� �������� �ִ� spine ������ �ε��� ��.	
 */
int CEPubBook::GetCurrentChapter()
{
	if(m_pViewManager)
		return m_pViewManager->GetCurrChapter();

	return -1;
}

/**
 * @biref	������ ����ڰ� ���� ������ ������ �����ϴ� �Լ�
 * @return	��Ŷ extension ���� ���� ���� ������ ��û�� ��� = TRUE, �׿�= FALSE 
 */
BOOL CEPubBook::CancelSelection()
{
	if(!m_pViewManager) return FALSE;

	IN3WebkitView *pView = (IN3WebkitView *)m_pViewManager->GetCurrentView();
	if(pView)
	{
		pView->SendMessage(CMD_IN3_CANCEL_SELECTION, NULL);
	}
	return TRUE;
}

/**
 * @biref	������ ������ ����ڰ� ���� ������ ������ �����ϴ� �Լ�
 * @param	chater	: ���� ������ ������ é�� �ε���(spine ������ �ε���)
 * @return	��Ŷ extension ���� ���� ���� ������ ��û�� ��� = TRUE, �׿�= FALSE 
 */
BOOL CEPubBook::CancelSelection(int chapter)
{
	if(!m_pViewManager) return FALSE;

	IN3WebkitView *pView = (IN3WebkitView *)m_pViewManager->GetView(chapter);
	if(pView)
	{
		pView->SendMessage(CMD_IN3_CANCEL_SELECTION, NULL);
	}
	return TRUE;
}





/********************************************************************************
 * setting feature
 ********************************************************************************/
/**
 * @brief	���� �Ʒ������� ���� ������ ���� ǥ������ ���θ� �����Ѵ�.
 * @param	bDisplay	: true(���� �ؿ� ������ ��ȣ ǥ��) / false(������ ��ȣ ǥ�� ����)
 */
void CEPubBook::SetDisplayPageNumber(bool bDisplay)
{
	if(!m_pSetting){
		TRACEMSG_ERR(_T("m_pSetting is NULL"));
		return ;
	}

	m_pSetting->SetDisplayPageNumber(bDisplay);
}

/**
 * @brief	������ ��带 �����Ѵ�. (�ܸ�/���)
 * @param	mode	: 1 = �ܸ�, 2 = ���
 */
void CEPubBook::SetPageDisplayMode(int mode)
{
	if(!m_pSetting){
		TRACEMSG_ERR(_T("m_pSetting is NULL"));
		return ;
	}
	//
	// ������ ��带 �����Ѵ�.
	m_pSetting->SetPageDisplayMode(mode);
	// ����� ��忡 �°� ȭ�� �籸��
	ReLayout();			
	// ����� �������� �ϸ�ũ�� ���ԵǾ� �ִ��� Ȯ��
	QueryBookmarkStatus();					
}

/**
 * @brief	��Ʈ ����� �����Ѵ�.
 * @param	size	: ������ ��Ʈ ������
 * @remarks �����Ǵ� ��Ʈ ������� ���� ���ο��� ������ �ִٰ� ReLayout() �� ����� ��Ʈ ����� �����ϵ��� �Ѵ�.
 *			���� �Ʒ� �Լ��� �ܼ��� ���� ���ο� ��Ʈ ����� �����ϴ� ��ɸ� �Ѵ�.
 *			�� ���� ������ �����ϱ� ���ؼ� ����ڰ� ���ϴ� ������ ReLayout() �Լ��� ȣ���Ͽ� ��� �Ѵ�.
 */
void CEPubBook::SetFontSize(int size)
{
	if(!m_pSetting){
		TRACEMSG_ERR(_T("m_pSetting is NULL"));
		return ;
	}
	//
	m_pSetting->SetFontSize(size);
}

/**
 * @brief	COLUMN GAP�� �����Ѵ�.
 * @param	margin	: ������ COLUMN GAP
 * @remarks �����Ǵ� COLUMN GAP�� ���� ���ο��� ������ �ִٰ� ReLayout() �� �����ϵ��� �Ѵ�.
 *			���� �Ʒ� �Լ��� �ܼ��� ���� ���ο� COLUMN GAP�� �����ϴ� ��ɸ� �Ѵ�.
 *			�� ���� ������ �����ϱ� ���ؼ� ����ڰ� ���ϴ� ������ ReLayout() �Լ��� ȣ���Ͽ� ��� �Ѵ�.
 */
void CEPubBook::SetMargin(int margin)
{
	if(!m_pSetting){
		TRACEMSG_ERR(_T("m_pSetting is NULL"));
		return ;
	}
	//
	m_pSetting->SetMargin(margin);
}

/**
 * @brief	line height�� �����Ѵ�.
 * @param	height	: ������ line height
 * @remarks �����Ǵ� line height�� ���� ���ο��� ������ �ִٰ� ReLayout() �� �����ϵ��� �Ѵ�.
 *			���� �Ʒ� �Լ��� �ܼ��� ���� ���ο� line height�� �����ϴ� ��ɸ� �Ѵ�.
 *			�� ���� ������ �����ϱ� ���ؼ� ����ڰ� ���ϴ� ������ ReLayout() �Լ��� ȣ���Ͽ� ��� �Ѵ�.
 *			(���� ���ǰ� ���� ����)
 */
void CEPubBook::SetLineHeight(double height)
{
	if(!m_pSetting){
		TRACEMSG_ERR(_T("m_pSetting is NULL"));
		return ;
	}
	//
	m_pSetting->SetLineHeight(height);
}

/**
 * @brief	��Ʈ�� �����Ѵ�.
 * @param	family	: ������ ��Ʈ
 * @remarks �����Ǵ� ��Ʈ�� ���� ���ο��� ������ �ִٰ� ReLayout() �� ����� ��Ʈ�� �����ϵ��� �Ѵ�.
 *			���� �Ʒ� �Լ��� �ܼ��� ���� ���ο� ��Ʈ�� �����ϴ� ��ɸ� �Ѵ�.
 *			�� ���� ������ �����ϱ� ���ؼ� ����ڰ� ���ϴ� ������ ReLayout() �Լ��� ȣ���Ͽ� ��� �Ѵ�.
 */
void CEPubBook::SetFontFamily(CString family)
{
	if(!m_pSetting){
		TRACEMSG_ERR(_T("m_pSetting is NULL"));
		return ;
	}
	//
	m_pSetting->SetFontFamily(family);
}

/**
 * @brief	��Ʈ ������ �����Ѵ�.
 * @param	color	: ������ ��Ʈ ����
 * @remarks �����Ǵ� ��Ʈ ������ ���� ���ο��� ������ �ִٰ� ReLayout() �� ����� ��Ʈ ������ �����ϵ��� �Ѵ�.
 *			���� �Ʒ� �Լ��� �ܼ��� ���� ���ο� ��Ʈ ������ �����ϴ� ��ɸ� �Ѵ�.
 *			�� ���� ������ �����ϱ� ���ؼ� ����ڰ� ���ϴ� ������ ReLayout() �Լ��� ȣ���Ͽ� ��� �Ѵ�.
 */
void CEPubBook::SetFontColor(CString color)
{
	if(!m_pSetting){
		TRACEMSG_ERR(_T("m_pSetting is NULL"));
		return ;
	}
	//
	m_pSetting->SetFontColor(color);
}

/**
 * @brief	������ �����Ѵ�.
 * @param	size	: ������ ����
 * @remarks �����Ǵ� ������ ���� ���ο��� ������ �ִٰ� ReLayout() �� ����� ������ �����ϵ��� �Ѵ�.
 *			���� �Ʒ� �Լ��� �ܼ��� ���� ���ο� ������ �����ϴ� ��ɸ� �Ѵ�.
 *			�� ���� ������ �����ϱ� ���ؼ� ����ڰ� ���ϴ� ������ ReLayout() �Լ��� ȣ���Ͽ� ��� �Ѵ�.
 */
void CEPubBook::SetBackgroudColor(CString color)
{
	if(!m_pSetting){
		TRACEMSG_ERR(_T("m_pSetting is NULL"));
		return;
	}
	//
	m_pSetting->SetBgColor(color);
}

/**
 * @biref	��Ʈ��, ������ �����Ѵ�.
 * @param	foregroundClr	: ��Ʈ��
 * @param	backgroundClr	: ����
 * @remarks	��Ʈ��, ������ �����ϴ� ���� ������ ���̾ƿ��� ������ �� �ʿ�� �����Ƿ� �Լ� ȣ�� ��� �ٷ� �ݿ��Ѵ�.
 */
void CEPubBook::SetColors(CString foregroundClr, CString backgroundClr)
{
	if(!m_pSetting){
		TRACEMSG_ERR(_T("m_pSetting is NULL"));
		return;
	}

	// ��Ʈ��, ���� ����
	m_pSetting->SetFontColor(foregroundClr);
	m_pSetting->SetBgColor(backgroundClr);


	// å�� ���ԵǾ� �ִ� ��� �信 ��Ʈ��, ���� ����
	for(unsigned int i=0; i< m_spineList.size(); i++)
	{
		IN3WebkitView *pView = (IN3WebkitView *)m_pViewManager->GetView(i);
		if(pView)
		{
			// ���� �ε��� �Ϸ�� ���Ŀ� ���� �����ϴ�.
			if(pView->m_nLoadState == IN3WebkitView::LS_LODE_DONE)
			{
				CString param;
				param.Format(_T("{\"FOREGROUND_COLOR\":\"%s\", \"BACKGROUND_COLOR\":\"%s\"}"), foregroundClr, backgroundClr);
				pView->SendMessage(CMD_IN3_CHANGE_COLORS, param);
			}
		}
	}
}

/**
 * @brief	���� �Ʒ� ������ ������ ��ȣ�� ǥ���ϵ��� �����Ǿ� �ִ��� Ȯ���Ѵ�.
 * @return	true(���� �ؿ� ������ ��ȣ ǥ��) / false(������ ��ȣ ǥ�� ����)
 */
const bool CEPubBook::IsDisplayPageNumber()
{
	if(!m_pSetting){
		TRACEMSG_ERR(_T("m_pSetting is NULL"));
		return false;
	}

	return m_pSetting->IsDisplayPageNumber();
}

/*
 * @brief	���� ������ ��带 ��ȯ�Ѵ�.
 * @return	������ ��� (1=�ܸ�, 2=���)
 */
const int CEPubBook::GetPageDisplayMode() const
{
	if(!m_pSetting){
		TRACEMSG_ERR(_T("m_pSetting is NULL"));
		return -1;
	}
	//
	return m_pSetting->GetPageDisplayMode();
}

/**
 * @brief	����ڰ� ������ ��Ʈ ����� ��ȯ�Ѵ�.
 * @return	����ڰ� ������ ��Ʈ ������
 */
const int CEPubBook::GetFontSize() const
{
	if(!m_pSetting){
		TRACEMSG_ERR(_T("m_pSetting is NULL"));
		return -1;
	}
	//
	return m_pSetting->GetFontSize();
}

/**
 * @brief	����ڰ� ������ column gap �� ��ȯ�Ѵ�.
 * @return	����ڰ� ������ column gap
 */
const int CEPubBook::GetMargin() const
{
	if(!m_pSetting){
		TRACEMSG_ERR(_T("m_pSetting is NULL"));
		return -1;
	}
	//
	return m_pSetting->GetMargin();
}

/**
 * @brief	����ڰ� ������ line height �� ��ȯ�Ѵ�.
 * @return	����ڰ� ������ line height
 */
const double CEPubBook::GetLineHeight() const
{
	if(!m_pSetting){
		TRACEMSG_ERR(_T("m_pSetting is NULL"));
		return 0.0;
	}
	//
	return m_pSetting->GetLineHeight();
}

/**
 * @brief	����ڰ� ������ ��Ʈ�� �� ��ȯ�Ѵ�.
 * @return	����ڰ� ������ ��Ʈ��
 */
const CString CEPubBook::GetFontFamily() const
{
	if(!m_pSetting){
		TRACEMSG_ERR(_T("m_pSetting is NULL"));
		return CString(_T(""));
	}
	//
	return m_pSetting->GetFontFamily();
}

/**
 * @brief	����ڰ� ������ ��Ʈ���� ��ȯ�Ѵ�.
 * @return	����ڰ� ������ ��Ʈ��
 */
const CString CEPubBook::GetFontColor() const
{
	if(!m_pSetting){
		TRACEMSG_ERR(_T("m_pSetting is NULL"));
		return CString(_T(""));
	}
	//
	return m_pSetting->GetFontColor();
}

/**
 * @brief	����ڰ� ������ ������ ��ȯ�Ѵ�.
 * @return	����ڰ� ������ ����
 */
const CString CEPubBook::GetBackgroudColor() const
{
	if(!m_pSetting){
		TRACEMSG_ERR(_T("m_pSetting is NULL"));
		return CString(_T(""));
	}
	//
	return m_pSetting->GetBgColor();
}







/********************************************************************************
 * Book's Information
 ********************************************************************************/
/**
 * @brief	epub å�� ���� ���� ��θ� �����Ѵ�.
 * @param	lpszOriginalPath	: epub å�� ���� ���� ���
 */
void CEPubBook::SetOriginalEpubPath(LPCTSTR lpszOriginalPath)
{
	if(m_pBookInfo)
		m_pBookInfo->SetSrcRootPath(lpszOriginalPath);
}

/**
 * @brief	fixed layout �� width, height ����� �����Ѵ�.
 * @param	width	: width of fixed contents
 * @param	height	: height of fixed contents
 */
void CEPubBook::SetFixedOriginalSize(int width, int height)
{
	if(width < 0 || height < 0) return;
	
	if(m_pBookInfo)
	{
		m_pBookInfo->SetFixedWidth(width);
		m_pBookInfo->SetFixedHeight(height);
	}
}

/**
 * @brief	epub å�� ����� �����ϴ� �Ŵ��͸� �����Ѵ�.
 * @param	pMgr	: �並 �����ϴ� �Ŵ����� ������
 */
void CEPubBook::SetViewManager(CViewManager *pMgr)
{
	m_pViewManager = pMgr;
}

/**
 * @brief	Epub �� open �Ǿ� �ִ��� Ȯ���ϴ� �Լ�
 * @return	already opend (TRUE)/ Not yet(FALSE)
 */
const BOOL CEPubBook::IsOpened(void) const
{
	return m_bOpened;
}

/**
 * @ Ȱ��ȭ �Ǿ� �ִ� é��(spine file) �� ������ ������Ʈ �Ѵ�.
 * @param	chapter	: Ȱ��ȭ �Ǵ� é�� ��
 */
void CEPubBook::SetCurrChapter(int chapter)
{
	if(m_pViewManager)
		m_pViewManager->SetCurrChapter(chapter);
}

/**
 * @brief	å�� ���������� Load �Ǿ����� �����ϴ� �Լ�
 * @return	TRUE(�ε� ��)/FALSE(�ε� �ȵ�)
 */
const BOOL CEPubBook::IsLoaded(void) const
{
	return m_bInitialized;
}

/**
 * @brief	Epub �� Fixed layout ���� Ȯ���ϴ� �Լ�
 * @return	FixedLayout(TRUE)/ReflowLayout(FALSE)
 */
const BOOL CEPubBook::IsFixed(void) const
{
	if(m_pBookInfo)
		return m_pBookInfo->IsFixed();

	return FALSE;
}

/**
 * @brief	å�� uuid �� �����´�.
 * @return	å�� uuid
 */
const CString CEPubBook::GetUUIDString(void) const
{
	if(m_pBookInfo)
		return m_pBookInfo->GetUUIDString();

	return CString(_T(""));
}

/**
 * @brief	å�� ������ ��ȯ�Ѵ�.
 * @return	å�� ���� 3 or 2
 */
const CString CEPubBook::GetVersion(void) const
{
	CString strVer(_T(""));
	if(m_pBookInfo)
		strVer.Format(_T("%.0f"), m_pBookInfo->GetVersion());

	return strVer;
}

/**
 * @brief	å�� ǥ���� �����´�.
 * @return	å�� Ÿ��Ʋ
 */
const CString CEPubBook::GetTitle(void) const
{
	if(m_pBookInfo)
		return m_pBookInfo->GetTitle();

	return CString(_T(""));
}

/**
 * @brief	���ڸ��� ��ȯ�Ѵ�.
 * @return	å�� ���ڸ�
 */
const CString CEPubBook::GetCreator(void) const
{	
	if(m_pBookInfo)
		return m_pBookInfo->GetCreator();

	return CString(_T(""));
}

/**
 * @brief	å�� ���ǻ縦 ��ȯ�Ѵ�.
 * @return	���ǻ��
 */
const CString CEPubBook::GetPublisher(void) const
{
	if(m_pBookInfo)
		return m_pBookInfo->GetPublisher();

	return CString(_T(""));
}

/**
 * @brief	å�� ������ ��ȯ�Ѵ�.
 * @return	����
 */
const CString CEPubBook::GetDesc(void) const
{
	if(m_pBookInfo)
		return m_pBookInfo->GetDesc();

	return CString(_T(""));
}

/**
 * @brief	å�� Ŀ�� �̹��� ��θ� ��ȯ�Ѵ�.
 * @return	Ŀ�� �̹��� ���
 */
const CString CEPubBook::GetCoverHref(void) const
{
	if(m_pBookInfo)
	{
		if(!m_pBookInfo->GetCoverHref().IsEmpty())
		{
			CString coverPath(GetOPSFolderPath());
			if(coverPath.Right(1) != '\\')
			{
				coverPath.Append(_T("\\"));
			}
			coverPath.Append(m_pBookInfo->GetCoverHref());
			coverPath.Replace(_T("/"), _T("\\"));
			return coverPath;
		}
	}

	return CString(_T(""));
}

/**
 * @brief	fixed layout �������� width �� ��ȯ�Ѵ�.
 * @return	fixed �������� width ��
 */
const int CEPubBook::GetFixedWidth() const
{
	return m_pBookInfo->GetFixedWidth();
}

/**
 * @brief	fixed layout �������� height �� ��ȯ�Ѵ�.
 * @return	fixed �������� height ��
 */
const int CEPubBook::GetFixedHeight() const
{
	return m_pBookInfo->GetFixedHeight();
}

/**
 * @brief	å�� spine ���� ��ȯ�Ѵ�.
 * @return	spine ����
 */
const int CEPubBook::GetSpineCount() const
{
	return m_spineList.size();
}

/**
 * @brief	������ spine ������ url�� ��ȯ�Ѵ�.
 * @param	idx	: url�� ã���� �ϴ� spine ������ �ε���
 * @return	spine ������ url
 */
const CString CEPubBook::GetSpineURL(int idx) const
{
	return m_spineList.at(idx);
}

/**
 * @brief	epub å�� ���� ���� ��θ� ��ȯ�Ѵ�.
 * @return	epub å�� ���� ���� ���
 */
const CString CEPubBook::GetOriginalEpubPath(void) const
{
	if(m_pBookInfo)
		return m_pBookInfo->GetSrcRootPath();
	
	return CString(_T(""));
}

/**
 * @brief	Epub ������ content.opf �� �����θ� ��ȯ�Ѵ�.
 * @return	content.opf ������ ���� ���
 */
const CString CEPubBook::GetOPSFilePath(void)
{
	if(!m_pBookInfo) return CString(_T(""));

	CString opsFilePath(m_pBookInfo->GetUnzipRootPath());
	opsFilePath.Append(m_pBookInfo->GetPackageName());
	opsFilePath.Append(_T("\\"));
	opsFilePath.Append(m_pBookInfo->GetOPSFilePath());

	return opsFilePath;
}

/**
 * @brief	epub å�� unzip �� ��ο� �ִ� ��Ű�� ����(OEBPS)�� ���� ��θ� ��ȯ�Ѵ�.
 * @return	��Ű�� ����(OEBPS) �� ���� ���
 */
const CString CEPubBook::GetOPSFolderPath(void) const
{
	if(!m_pBookInfo) return CString(_T(""));

	CString url(m_pBookInfo->GetUnzipRootPath());
	url.Append(m_pBookInfo->GetPackageName());
	url.Append(_T("\\"));
	return url;
}

#if SUPPORT_THUMBNAIL_PATH
	// thumbnail path �� �������� �����ϴ� ���, unzip �� epub ��� �ȿ� thumbnail ������ ����� �ش� ��θ� ��ȯ�Ѵ�.
	// �׷��� �������� ������ ��θ� �����Ͽ� ������ �ʿ䰡 �����Ƿ� �ش� �Լ��� �������ϵ��� ó����.
/**
 * @breif	�泻�� ���
 * @remarks epub �� ���� ��θ� �����Ͽ� ��ȯ��
 * @return	����� ���
 */
const CString CEPubBook::GetThumbnailPath(void)
{
	if(!m_pBookInfo) return CString(_T(""));

	CString thumbnailPath(m_pBookInfo->GetThumbnailPath()); 

	return thumbnailPath;
}
#endif;

/**
 * @brief	å�� ������ ��ȯ�Ѵ�.(Ʈ������)
 * @remarks	å�� ������ ��Ʈ �������� �ּҰ��� ��ȯ�Ѵ�.
 * @return	å�� ����
 */
IN3NavPointNode*	CEPubBook::GetTableOfContent(void)
{
	return &m_tocTree;
}

/**
 * @brief	å���� ������ �̹����� ����Ʈ�� ��ȯ�Ѵ�.
 * @remarks	å�� ��� �̹����� �����ϴ� ���� �ƴ϶�, �ν� ������ �̹������� �����Ѵ�.
 *			extension �� �̹��� ������ ��ȸ�Ѵ�.
 * @return  true(success)/false(fail)
 */
BOOL CEPubBook::QueryImageList(void)
{
	// @ note
	// ���� ��
	return TRUE;
}

/**
 * @brief	å���� ������ ǥ�� ����Ʈ�� ��ȯ�Ѵ�.
 * @remarks	å�� ��� ǥ�� �����ϴ� ���� �ƴ϶�, �ν� ������ ǥ���� �����Ѵ�.
 *			extension �� ǥ ������ ��ȸ�Ѵ�.
 * @return  true(success)/false(fail)
 */
BOOL CEPubBook::QueryTableList(void)
{
	// @ note
	// ���� ��
	return TRUE;
}




/********************************************************************************
 * page navigation
 ********************************************************************************/
/**
 * @brief	å�� ��ü �������� �輱�ϴ� �Լ�
 * @param	bWaitCathering	: ��ü �������� ����� ��, å�� ��� spine �� init �Ǳ� ��ٸ��� �Ǵ��ϴ� ��. (true : ��� init �Ǳ� ��ٸ�, false : �ٷ� �� ������ ���)
 */
void CEPubBook::CalculateTotalPage(BOOL bWaitGathering /*= TRUE*/)
{
	if(bWaitGathering)
	{
		++m_nGatheringInit;
		if(m_nGatheringInit < m_spineList.size()) return;
	}
	
	if(m_pViewManager)
		m_nTotalPage = m_pViewManager->GetTotalPage();

	// send message to parent wnd ( document init complate )
	DocumentLoadedComplate();
}

/**
 * @brief	������ é�ͷ� �̵��ϱ�
 * �Ķ����	chapter			�̵��� ���� (spine index)
 *			bDirectRight	é�� �̵� ����. TRUE(Right) /FALSE(Left)
 */
void CEPubBook::MoveToChapter(unsigned int chapter, BOOL bDirectRight /*= TRUE*/)
{
	TRACEMSG(_T("CEPubBook::MoveChapter() START"));
	//
	if(!m_pSetting){
		TRACEMSG_ERR(_T("m_pSetting is NULL"));
		return;
	}
	if(!m_pViewManager){
		TRACEMSG_ERR(_T("m_pViewManager is NULL"));
		return;
	}

	if(chapter < 0){
		TRACEMSG_ERR(_T("chapter < 0. so can't move chapter"));
		return;
	}

	if(chapter >= m_spineList.size()){
		TRACEMSG_ERR(_T("chapter > max spine count. so can't move chapter"));
		return;
	}
	//
	// �̵��ϰ��� �ϴ� é�Ͱ� ���� �������� é�Ͷ�� ������ ó�� ���� �ٷ� ����.
	if(chapter == m_pViewManager->GetCurrChapter()){
		return;
	}
	//
	// �̵��ϰ��� �ϴ� é���� �並 �����´�.
	IN3WebkitView *pNext = (IN3WebkitView *)m_pViewManager->GetView(chapter);
	if(!pNext) return;
	//
	// ���� Ȱ��ȭ�Ǿ� �ִ� �並 �����´�.
	IN3WebkitView *pCurr = (IN3WebkitView *)this->GetActiveView();
	//
	// fixed ��麸��� �� �� �������� é�� �̵��� �����Ͽ� ó���Ѵ�.
	if(IsFixed() && m_pSetting->GetPageDisplayMode() == 2)
	{
		// fixed �������̰�, ��麸���� ��� é�� �̵�
		if(ChapgeViewFixedDual(pCurr, pNext))
		{
			m_pViewManager->SetCurrChapter(chapter);
			pNext->SetFocus();
			this->SetActiveView(pNext);
		}
	}
	else
	{
		// é�� �̵�
		if(ChangeView(pCurr, pNext))
		{
			m_pViewManager->SetCurrChapter(chapter);
			// 
			// �̵��ϴ� ���⿡ ���� é���� ù �������� ������, ������ �������� ������ �����Ѵ�.
			if(bDirectRight /*&& (pNext->GetCurrPage() > 0)*/)
			{
				pNext->MoveToPage(1);
			}

			if(!bDirectRight)
			{
				pNext->MoveToPage(pNext->GetTotalPage());
			}
		}
	}
	//
	TRACEMSG(_T("CEPubBook::MoveChapter() END"));
}

/**
 * @brief	������ é�ͷ� �̵��ϱ�
 * @param	chapter		: �̵��� ���� (spine index)
 *			page		: �̵��� ������ (�� spine ���� �������� ��������. å�� ��ü ���������� �ƴ�)
 */
void CEPubBook::MoveToChatperAndPage(unsigned int chapter, int page)
{
	TRACEMSG(_T("CEPubBook::MoveChapter() START"));
	//
	if(!m_pViewManager){
		TRACEMSG_ERR(_T("m_pViewManager is NULL"));
		return;
	}
	if(!m_pSetting){
		TRACEMSG_ERR(_T("m_pSetting is NULL"));
		return;
	}

	if(chapter < 0){
		TRACEMSG_ERR(_T("chapter < 0. so can't move chapter"));
		return;
	}

	if(chapter >= m_spineList.size()){
		TRACEMSG_ERR(_T("chapter > max spine count. so can't move chapter"));
		return;
	}
	//
	// �̵��� é���� �� ��������
	IN3WebkitView *pNext = (IN3WebkitView *)m_pViewManager->GetView(chapter);
	if(!pNext) return;
	//
	// ���� Ȱ��ȭ �Ǿ� �ִ� é���� �� ��������
	IN3WebkitView *pCurr = (IN3WebkitView *)this->GetActiveView();
	//
	// fixed ��麸��� �� �� �������� é�� �̵��� �����Ͽ� ó���Ѵ�.
	if(IsFixed() && m_pSetting->GetPageDisplayMode() == 2)
	{
		if(ChapgeViewFixedDual(pCurr, pNext))
		{
			m_pViewManager->SetCurrChapter(chapter);
			pNext->SetFocus();
			this->SetActiveView(pNext);
		}
	}
	else
	{
		IN3WebkitView *pCurr = (IN3WebkitView *)this->GetActiveView();
		if(ChangeView(pCurr, pNext))
		{
			m_pViewManager->SetCurrChapter(chapter);
			if(pNext->GetCurrPage() != page)
				pNext->MoveToPage(page);
		}
	}		
	//
	TRACEMSG(_T("CEPubBook::MoveChapter() END"));
}

/**
 * @brief	å�� ���Ե� anchor �� �ִ� �������� �̵��Ѵ�.
 * �Ķ���� pBook		�̵��� å
 *			fileName	anchor �� �ִ� �����̸�
 *			anchorID	�̵��� anchor id
 */
void CEPubBook::MoveToAnchor(CString fileName, CString anchorID)
{
	TRACEMSG(_T("CEPubBook::MoveToAnchor() START"));
	//
	if(!m_pViewManager){
		TRACEMSG_ERR(_T("m_pViewManager is NULL!!"));
		return;
	}

	// anchor �� ���ԵǾ� �ִ� ������ spine �ε����� �����´�.
	int index = GetSpineIndex(fileName);

	// anchor �� ���ԵǾ� �ִ� �並 �����´�.
	IN3WebkitView *pView = (IN3WebkitView *)m_pViewManager->GetView(index);
	if(pView)
	{
		// move chapter
		MoveToChapter(index);
		pView->SendMessage(CMD_IN3_MOVE_TO_OBJECT, anchorID);
	}
	//	
	TRACEMSG(_T("CEPubBook::MoveToAnchor() END"));
}

/**
 * @brief	������ �������� �̵��ϴ� �Լ�
 * @param	page	: �̵��� ������(��ü ������ ����)
 */
void CEPubBook::MoveToPage(unsigned int page)
{
	TRACEMSG(_T("CEPubBook::MoveToPage() START"));
	//
	if(!m_pViewManager){
		TRACEMSG_ERR(_T("m_pViewManager is NULL!!"));
		return;
	}

	UINT chapter = 0;
	int movePage = page;
	// �̵��� �������� ���° ������������ Ȯ���Ѵ�. (���° spine �������� Ȯ���Ѵ�.)
	for(chapter; chapter < m_spineList.size(); chapter++)
	{
		int tempPage = m_pViewManager->GetTotalPage(chapter);
		if(movePage - tempPage <= 0)
			break;
		else
			movePage -= tempPage;
	}

	// move to chpater and page;
	if(m_pViewManager->GetCurrChapter() != chapter)
		MoveToChatperAndPage(chapter, (int)movePage);
	else
	{
		IN3WebkitView	*pCurrent = (IN3WebkitView *)m_pViewManager->GetCurrentView();
		if(pCurrent)
			pCurrent->MoveToPage(movePage);
	}
	//
	TRACEMSG(_T("CEPubBook::MoveToPage() END"));
}

/**
 * @brief ���� �������� �̵��Ѵ�.
 */
void CEPubBook::MoveToNextPage()
{
	if(!m_pViewManager){
		TRACEMSG_ERR(_T("m_pViewManager is NULL!!"));
		return;
	}

	IN3WebkitView* pView = (IN3WebkitView *)m_pViewManager->GetCurrentView();
	if(pView)
	{
		pView->NextPage();
	}
}

/**
 * @brief ���� �������� �̵��Ѵ�.
 */
void CEPubBook::MoveToPrevPage()
{
	if(!m_pViewManager){
		TRACEMSG_ERR(_T("m_pViewManager is NULL!!"));
		return;
	}

	IN3WebkitView* pView = (IN3WebkitView *)m_pViewManager->GetCurrentView();
	if(pView)
	{
		pView->PrevPage();
	}
}

/**
 * @brief �˻� ��� ����Ʈ �� ������ ����� �ִ� �������� �̵��Ѵ�.
 * @param	fileName	: �˻� ����� �ִ� ���ϸ�
 * @param	keyword		: �˻� ���ڿ�
 * @param	seq			: �˻� ����� ������ ��
 */
void CEPubBook::MoveToFindResult(CString fileName, CString keyword, int seq)
{
	if(!m_pViewManager){
		TRACEMSG_ERR(_T("m_pViewManager is NULL!!"));
		return;
	}

	// �˻� ����� �ִ� ���ϸ��� spine �ε����� �����´�.
	// spine �ε����� �̿��� ���õ� �並 �����´�.
	int index = GetSpineIndex(fileName);
	IN3WebkitView *pView = (IN3WebkitView *)m_pViewManager->GetView(index);
	if(pView)
	{
		MoveToChapter(index);
		//
		CString param;
		param.Format(_T("{\"KEYWORD\":\"%s\", \"SEQUENCE\":%d}"), keyword, seq);
		pView->SendMessage(CMD_IN3_MOVE_TO_FIND_RESULT, param);
	}
}

/**
 * @brief	Anchor �̵� �� ��Ŷ extension ���κ��� ȣ��Ǵ� callback �Լ�
 * @remarks	��Ŷ extension ���� ������ �̵����� �ϹǷ�, ���ٸ� ������ �̵� ���� �̵��� ������ ������ ������Ʈ �Ѵ�.
 * @param	page	: �̵��� ������ ��
 */
void CEPubBook::OnMoveToAnchor(int page)
{		
	IN3WebkitView *pView = (IN3WebkitView *)m_pViewManager->GetCurrentView();
	if(pView)
	{
		// Webkit Extentions Engine ���� �̵��� ������ ������ �信 �����Ѵ�.
		// ������ �̵��� ���� �ʰ� �ܼ��� ������ update
		pView->SetCurrPage(page);
	}
}

/**
 * @brief	������ �̵� �� ��Ŷ extension ���κ��� ȣ��Ǵ� callback �Լ�
 * @remarks	UI�ܿ� WM_PAGE_MOVED �޽����� ������,
 */
void CEPubBook::OnPageMoved()
{
	CWnd* pParentWnd = GetParent();
	if(pParentWnd)
	{
		// ���� �̵��� ������ ���� ������ UI ������ �޽����� ������.
		int currPage = GetCurrentPage();
		pParentWnd->SendMessage(WM_PAGE_MOVED, (WPARAM)this, (LPARAM)currPage);
	}
}




/********************************************************************************
 * annotation
 ********************************************************************************/
/**
 * @brief	���̶���Ʈ�� �߰��ϴ� �Լ�
 * @param	color	: ���̶���Ʈ ����
 * @param	hasMemo	: �޸� �߰��Ǿ� �ִ��� ����
 * @return	��Ŷ extension ���� ���̶���Ʈ �߰��� ��û�� ��� = TRUE, �׿�= FALSE
 */
BOOL CEPubBook::CreateTextHighlight(CString color, BOOL hasMemo /*= FALSE*/)
{
	TRACEMSG(_T("CEPubBook::CreateTextHighlight() START"));
	//
	if(!m_pViewManager){
		TRACEMSG_ERR(_T("m_pViewManager is NULL!!"));
		return FALSE;
	}

	BOOL bRes = FALSE;
	IN3WebkitView *pView = (IN3WebkitView *)m_pViewManager->GetCurrentView();
	if(pView)
	{
		CString param;
		param.Format(_T("{\"BG_COLOR\":\"%s\", \"HAS_MEMO\":%d}"), color, hasMemo);
		bRes =  pView->SendMessage(CMD_IN3_MARK_ANNOTATION, param);
	}
	//
	TRACEMSG(_T("CEPubBook::CreateTextHighlight() END"));

	return bRes;
}

/**
 * @brief	��������� �߰��ϴ� �Լ�
 * @param	color	: ������� ����(����� ������ ������ ���� ������Ʈ �� �� �� ����)
 * @param	hasMemo	: �޸� �߰��Ǿ� �ִ��� ����
 * @return	��Ŷ extension ���� ������� �߰��� ��û�� ��� = TRUE, �׿�= FALSE
 */
BOOL CEPubBook::CreateUnderline(CString color, BOOL hasMemo /*= FALSE*/)
{
	TRACEMSG(_T("CEPubBook::CreateUnderline() START"));
	//
	if(!m_pViewManager){
		TRACEMSG_ERR(_T("m_pViewManager is NULL!!"));
		return FALSE;
	}

	BOOL bRes = FALSE;
	IN3WebkitView *pView = (IN3WebkitView *)m_pViewManager->GetCurrentView();
	if(pView)
	{
		// BG_COLOR ���� "" �̸� ����������� ó���ϰ� ����.
		CString param;
		param.Format(_T("{\"BG_COLOR\":\"\", \"UL_COLOR\":\"%s\", \"HAS_MEMO\":%d}"), color, hasMemo);
		bRes =  pView->SendMessage(CMD_IN3_MARK_ANNOTATION, param);
	}
	//
	TRACEMSG(_T("CEPubBook::CreateUnderline() END"));

	return bRes;
}

/**
 * @brief	annotation(���̶���Ʈ/�޸�/�������) �� �����Ѵ�.
 * @param	fileName	: ������ annotaton �� �ִ� ���ϸ�
 * @param	markerID	: ������ annotation �� id
 * @return	��Ŷ extension ���� annotation ������ ��û�� ��� = TRUE, �׿�= FALSE
 */
BOOL CEPubBook::DeleteAnnotation(CString fileName, CString markerID)
{
	TRACEMSG(_T("CEPubBook::DeleteAnnotation() START"));
	//
	if(!m_pViewManager){
		TRACEMSG_ERR(_T("m_pViewManager is NULL!!"));
		return FALSE;
	}

	BOOL bRes = FALSE;
	IN3WebkitView *pView = (IN3WebkitView *)m_pViewManager->GetCurrentView();
	if(pView)
	{
		CString param;
		param.Format(_T("{\"FILE_NAME\":\"%s\", \"MARKER_ID\":\"%s\"}"), fileName, markerID);
		bRes =  pView->SendMessage(CMD_IN3_DELETE_ANNOTATION, param);
	}
	//
	TRACEMSG(_T("CEPubBook::DeleteAnnotation() END"));

	return bRes;
}

/**
 * @brief	annotation(���̶���Ʈ/�޸�/�������) �� �����Ѵ�.
 * @param	pAnnotation	: ������ annotaton ����
 * @return	��Ŷ extension ���� annotation ������ ��û�� ��� = TRUE, �׿�= FALSE
 */
BOOL CEPubBook::ModifyAnnotation(IN3Annotation *pAnnotation)
{
	if(pAnnotation == NULL) return FALSE;
	//
	if(!m_pViewManager){
		TRACEMSG_ERR(_T("m_pViewManager is NULL!!"));
		return FALSE;
	}

	if(!pAnnotation) return FALSE;

	BOOL bRes = FALSE;
	IN3WebkitView *pView = (IN3WebkitView *)m_pViewManager->GetCurrentView();
	if(pView)
	{
		CString param;
		param.Format(_T("{\"HAS_MEMO\":%d, \"FILE_NAME\":\"%s\", \"MARKER_ID\":\"%s\", \"UL_COLOR\":\"%s\", \"BG_COLOR\":\"%s\"}"), 
			pAnnotation->HasMemo(), pAnnotation->GetFileName(), pAnnotation->GetMarkerID(), pAnnotation->GetULColor(), pAnnotation->GetBGColor());
		bRes =  pView->SendMessage(CMD_IN3_MODIFY_ANNOTATION, param);
	}

	return bRes;
}

/**
 * @brief	annotation(���̶���Ʈ/�޸�/�������) ������ �ϰ������Ѵ�.
 * @param	annotations	: �ϰ� ������ annotation ����Ʈ
 * @return	��Ŷ extension ���� annotation �ϰ� ������ ��û�� ��� = TRUE, �׿�= FALSE
 */
BOOL CEPubBook::InitAnnotationList(std::vector<IN3Annotation*> annotations)
{
	if(!m_pViewManager) return FALSE;
	if(annotations.size() == 0) return TRUE;;

	BOOL bRes = TRUE;
	if(!m_pSetting){
		TRACEMSG_ERR(_T("m_pSetting is NULL"));
		return FALSE;
	}
	
	UINT i=0;
	// setting ������ annotation ����Ʈ �ʱ�ȭ
	m_pSetting->ClearAnnotations();
	//
	// �ϰ� ������ annotation ���� ���� annotation �� ���ԵǾ� �ִ� ������ �������� �и��Ͽ� setting ������ �ӽ� �����Ѵ�.
	for(i; i< annotations.size(); i++)
	{
		IN3Annotation *pAnnotation = annotations[i];
		int index = GetSpineIndex(pAnnotation->GetFileName());
		if(index < 0) continue;

		m_pSetting->AddAnnotation(index, pAnnotation);
	}

	// ���Ϻ��� �з��� annotation �� ���� ���Ͽ� �����Ѵ�.
	for(i=0; i< m_spineList.size(); i++)
	{
		IN3WebkitView *pView = (IN3WebkitView *)m_pViewManager->GetView(i);
		if(pView)
		{
			if(pView->m_nLoadState == IN3WebkitView::LS_LODE_DONE)
			{
				CString strAry;
				m_pSetting->GetInitAnnotationParam(i, strAry);
				if(strAry.IsEmpty()) continue;

				CString param;
				param.Format(_T("{\"ANNOTATION_LIST\":[%s]}"), strAry);
				pView->SendMessage(CMD_IN3_INIT_ANNOTATION_LIST, param);
			}
		}
	}

	// �ӽ� �����Ͽ��� annotation ����Ʈ�� setting �������� �����Ѵ�.
	m_pSetting->ClearAnnotations();
	return bRes;
}

/**
 * @brief	annotation �޴��� ����ֱ� ���� ���� ����ڰ� ������ ������ ��ȸ�ϴ� �Լ�
 * @return	��Ŷ extension ���� ����ڰ� ������ ������ ��ȸ�� ��û�� ��� = TRUE, �׿�= FALSE
 */
BOOL CEPubBook::QueryAnnotationToolbarPosition()
{
	if(!m_pViewManager){
		TRACEMSG_ERR(_T("m_pViewManager is NULL!!"));
		return FALSE;
	}

	BOOL bRes = FALSE;
	IN3WebkitView *pView = (IN3WebkitView *)m_pViewManager->GetCurrentView();
	if(pView)
	{
		bRes =  pView->SendMessage(CMD_IN3_QUERY_ANNOTATION_POS, NULL);
	}

	return bRes;
}

/**
 * @brief	annotation �߰� �� ��Ŷ extension ���κ��� ȣ��Ǵ� callback �Լ�
 * @param	param	: extension ���� ���� ���޹��� JSON ������
 * @param	index	: annotation �� �߰��� ������ (fixed layout ������ ��ȿ�� ���� ����)
 * @see		ParseAnntationParam
 */
void CEPubBook::OnAnnotationAdded(CString param, int index /*= 0*/)
{
	if(param.IsEmpty()) return;
	//
	IN3Annotation *pAnnotation = new IN3Annotation;
	if(ParseAnntationParam(param, pAnnotation))
	{
		pAnnotation->SetPage(index);
		// send information to UI
		CWnd* pParentWnd = GetParent();
		if(pParentWnd)
			pParentWnd->SendMessage(WM_ANNOTATION_ADDED, (WPARAM)this, (LPARAM)pAnnotation);
		//
		CancelSelection();
	}

	if(pAnnotation)
		delete pAnnotation;
}

/**
 * @brief	����ڰ� annotation �� Ŭ���� ��, ��Ŷ extension ���� ���� ȣ��Ǵ� callback �Լ�
 * @param	param	: extension ���� ���� ���޹��� JSON ������
 * @see		ParseAnntationParam
 */
void CEPubBook::OnAnnotationSelected(CString param)
{
	if(param.IsEmpty()) return;
	//
	IN3Annotation *pAnnotation = new IN3Annotation;
	if(ParseAnntationParam(param, pAnnotation))
	{
		// send information to UI
		CWnd* pParentWnd = GetParent();
		if(pParentWnd)
			pParentWnd->SendMessage(WM_ANNOTATION_SELECTED, (WPARAM)this, (LPARAM)pAnnotation);
	}

	if(pAnnotation)
		delete pAnnotation;
}

/**
 * @brief	����ڰ� �޸� �������� Ŭ���� ��, ��Ŷ extension ���� ���� ȣ��Ǵ� callback �Լ�
 * @param	param	: extension ���� ���� ���޹��� JSON ������
 * @see		ParseAnntationParam
 */
void CEPubBook::OnMemoIconSelected(CString param)
{
	if(param.IsEmpty()) return;
	//
	IN3Annotation *pAnnotation = new IN3Annotation;
	if(ParseAnntationParam(param, pAnnotation))
	{
		// send information to UI
		CWnd* pParentWnd = GetParent();
		if(pParentWnd)
			pParentWnd->SendMessage(WM_MEMO_ICON_SELECTED, (WPARAM)this, (LPARAM)pAnnotation);
	}

	if(pAnnotation)
		delete pAnnotation;
}

/**
 * @brief	annotation ���� �� ��Ŷ extension ���κ��� ȣ��Ǵ� callback �Լ�
 * @param	param	: extension ���� ���� ���޹��� JSON ������
 * @see		ParseAnntationParam
 */
void CEPubBook::OnAnnotationDeleted(CString param)
{
	if(param.IsEmpty()) return;
	//
	IN3Annotation *pAnnotation = new IN3Annotation;
	if(ParseAnntationParam(param, pAnnotation))
	{
		// send information to UI
		CWnd* pParentWnd = GetParent();
		if(pParentWnd)
			pParentWnd->SendMessage(WM_ANNOTATION_DELETED, (WPARAM)this, (LPARAM)pAnnotation);
	}

	if(pAnnotation)
		delete pAnnotation;
}

/**
 * @brief	annotation ���� �� ��Ŷ extension ���κ��� ȣ��Ǵ� callback �Լ�
 * @param	param	: extension ���� ���� ���޹��� JSON ������
 * @see		ParseAnntationParam
 */
void CEPubBook::OnAnnotationModified(CString param)
{
	if(param.IsEmpty()) return;
	//
	IN3Annotation *pAnnotation = new IN3Annotation;
	if(ParseAnntationParam(param, pAnnotation))
	{
		// send information to UI
		CWnd* pParentWnd = GetParent();
		if(pParentWnd)
			pParentWnd->SendMessage(WM_ANNOTATION_MODIFIED, (WPARAM)this, (LPARAM)pAnnotation);
	}

	if(pAnnotation)
		delete pAnnotation;
}

/**
 * @brief	QueryAnnotationToolbarPosition() �Լ��� ���� callback �Լ�
 * @param	param	: extension ���� ���� ���޹��� JSON ������
 */
void CEPubBook::OnQueryAnnotationPos(CString param)
{
	if(param.IsEmpty()) return;
	//
	BOOL bRes = TRUE;
	// �Ķ���͸� JOSN ���� �Ľ��Ѵ�.
	JSONValue* value = JSON::Parse(param);
	if(value == NULL)
	{
		TRACEMSG_ERR(_T("[CEPubBook::OnQueryAnnotationPos] recived data can't parsing!!"));
		return;
	}

	if(value->IsObject() == false)
	{
		TRACEMSG_ERR(_T("[CEPubBook::OnQueryAnnotationPos] The root element is not an object!!"));
		return;
	}

	JSONObject root;
	root = value->AsObject();
	// "RESULT_CODE" �� ���� 0 �� �ƴϸ� ������ �����Ѵ�.
	JSONObject::iterator iter = root.find(_T("RESULT_CODE"));
	if(iter != root.end() && root[_T("RESULT_CODE")]->IsNumber()){
		if(root[_T("RESULT_CODE")]->AsNumber() != 0.0)
			return;
		/*CString result = root[_T("RESUOT_CODE")]->Stringify().c_str();
		result.Replace(_T("\\"), _T(""));
		if(result != _T("0"))
		{
			return FALSE;
		}*/
	}

	BOOL bShow = FALSE;
	CRect rect;

	//
	iter = root.find(_T("SHOW_HIDE"));
	if(iter!= root.end() /*&& root[key.GetString()]->IsString()*/){
		bShow = root[_T("SHOW_HIDE")]->AsBool();
	}
	//
	iter = root.find(_T("SELECTION_LEFT"));
	if(iter!= root.end() /*&& root[key.GetString()]->IsString()*/){
		rect.left = (int)root[_T("SELECTION_LEFT")]->AsNumber();
	}
	//
	iter = root.find(_T("SELECTION_RIGHT"));
	if(iter!= root.end() /*&& root[key.GetString()]->IsString()*/){
		rect.right = (int)root[_T("SELECTION_RIGHT")]->AsNumber();
	}
	//
	iter = root.find(_T("SELECTION_TOP"));
	if(iter!= root.end() /*&& root[key.GetString()]->IsString()*/){
		rect.top = (int)root[_T("SELECTION_TOP")]->AsNumber();
	}
	//
	iter = root.find(_T("SELECTION_BOTTOM"));
	if(iter!= root.end() /*&& root[key.GetString()]->IsString()*/){
		rect.bottom = (int)root[_T("SELECTION_BOTTOM")]->AsNumber();
	}
	// send information to UI
	CWnd* pParentWnd = GetParent();
	if(pParentWnd)
		pParentWnd->SendMessage(WM_ANNOTATION_POSITION, (WPARAM)this, (LPARAM)&rect);

	if(value)
		delete value;
}




/********************************************************************************
 * bookmark
 ********************************************************************************/
/**
 * @brief	���� ���̴� �������� �ϸ�ũ�� �����Ѵ�.
 * @return	��Ŷ extension ���� �ϸ�ũ �߰��� ��û�� ��� = TRUE, �׿�= FALSE 
 */
BOOL CEPubBook::SetBookmark()
{
	TRACEMSG(_T("CEPubBook::SetBookmark() START"));
	//
	if(!m_pViewManager){
		TRACEMSG_ERR(_T("m_pViewManager is NULL!!"));
		return FALSE;
	}

	BOOL bRes = FALSE;
	IN3WebkitView *pView = (IN3WebkitView *)m_pViewManager->GetCurrentView();
	if(pView)
	{
		bRes =  pView->SendMessage(CMD_IN3_SET_BOOKMARK, NULL);
	}
	//
	TRACEMSG(_T("CEPubBook::SetBookmark() END"));

	return bRes;
}

/**
 * @brief	���� ���̴� ������ �ϸ�ũ���� �����Ѵ�.
 * @return	��Ŷ extension ���� �ϸ�ũ ������ ��û�� ��� = TRUE, �׿�= FALSE 
 */
BOOL CEPubBook::ClearBookmark()
{
	TRACEMSG(_T("CEPubBook::ClearBookmark() START"));
	//
	if(!m_pViewManager){
		TRACEMSG_ERR(_T("m_pViewManager is NULL!!"));
		return FALSE;
	}
	BOOL bRes = FALSE;
	IN3WebkitView *pView = (IN3WebkitView *)m_pViewManager->GetCurrentView();
	if(pView)
	{
		bRes =  pView->SendMessage(CMD_IN3_CLEAR_BOOKMARK, NULL);
	}
	//
	TRACEMSG(_T("CEPubBook::ClearBookmark() END"));

	return bRes;
}

/**
 * @brief	Ư�� �ϸ�ũ�� �����Ѵ�.
 * @param	pBookmark	: ������ �ϸ�ũ ����
 * @return	��Ŷ extension ���� �ϸ�ũ ������ ��û�� ��� = TRUE, �׿�= FALSE 
 */
BOOL CEPubBook::DeleteBookmark(IN3Bookmark *pBookmark)
{
	TRACEMSG(_T("CEPubBook::DeleteBookmark() START"));
	//
	if(!m_pViewManager){
		TRACEMSG_ERR(_T("m_pViewManager is NULL!!"));
		return FALSE;
	}

	BOOL bRes = FALSE;
	int index = GetSpineIndex(pBookmark->GetFileName());
	IN3WebkitView *pView = (IN3WebkitView *)m_pViewManager->GetView(index);
	if(pView)
	{
		CString param;
		param.Format(_T("{\"FILE_NAME\":\"%s\", \"BOOKMARK_LIST\":[\"%s\"]}"), pBookmark->GetFileName(), pBookmark->GetMarkerID());
		bRes =  pView->SendMessage(CMD_IN3_DELETE_BOOKMARK, param);
	}
	//
	TRACEMSG(_T("CEPubBook::DeleteBookmark() END"));

	return bRes;
}

/**
 * @brief	Ư�� �ϸ�ũ���� �����Ѵ�.
 * @param	bookmarks	: ������ �ϸ�ũ ���� ����Ʈ
 * @return	��Ŷ extension ���� �ϸ�ũ ������ ��û�� ��� = TRUE, �׿�= FALSE 
 */
BOOL CEPubBook::DeleteBookmarkList(std::vector<IN3Bookmark*> bookmarks)
{
	if(!m_pSetting){
		TRACEMSG_ERR(_T("m_pSetting is NULL"));
		return FALSE;
	}
	if(!m_pViewManager) return FALSE;
	//
	BOOL bRes = TRUE;
	
	UINT i=0;
	// setting ������ �ϸ�ũ ����Ʈ�� �ʱ�ȭ
	m_pSetting->ClearBookmarks();
	//
	// ������ �ϸ�ũ ���� ���� �ϸ�ũ�� ���ԵǾ� �ִ� ������ �������� �и��Ͽ� setting ������ �ӽ� �����Ѵ�.
	for(i; i< bookmarks.size(); i++)
	{
		IN3Bookmark *pBookmark = bookmarks[i];
		int index = GetSpineIndex(pBookmark->GetFileName());
		if(index < 0) continue;

		m_pSetting->AddBookmark(index, pBookmark);
	}

	// ���Ϻ��� �з��� �ϸ�ũ���� �����Ѵ�.
	for(i=0; i< m_spineList.size(); i++)
	{
		IN3WebkitView *pView = (IN3WebkitView *)m_pViewManager->GetView(i);
		if(pView)
		{
			if(pView->m_nLoadState == IN3WebkitView::LS_LODE_DONE)
			{
				CString strAry;
				m_pSetting->GetDeleteBookmarkParam(i, strAry);
				if(strAry.IsEmpty()) continue;

				CString param;
				param.Format(_T("{\"BOOKMARK_LIST\":[%s]}"), strAry);
				pView->SendMessage(CMD_IN3_INIT_BOOKMARK_LIST, param);
			}
		}
	}
	return bRes;
}

/**
 * @brief	�ϸ�ũ���� �ϰ������Ѵ�.
 * @param	bookmarks	: �ϰ� ������ bookmark ����Ʈ
 * @return	��Ŷ extension ���� �ϸ�ũ �ϰ� ������ ��û�� ��� = TRUE, �׿�= FALSE
 */
BOOL CEPubBook::InitBookmarkList(std::vector<IN3Bookmark*> bookmarks)
{
	if(!m_pSetting){
		TRACEMSG_ERR(_T("m_pSetting is NULL"));
		return FALSE;
	}
	if(!m_pViewManager) return FALSE;
	//
	BOOL bRes = TRUE;
	
	UINT i=0;
	// setting ������ �ϸ�ũ ����Ʈ�� �ʱ�ȭ
	m_pSetting->ClearBookmarks();
	//
	// ������ �ϸ�ũ ���� ���� �ϸ�ũ�� ���ԵǾ� �ִ� ������ �������� �и��Ͽ� setting ������ �ӽ� �����Ѵ�.
	for(i; i< bookmarks.size(); i++)
	{
		IN3Bookmark *pBookmark = bookmarks[i];
		int index = GetSpineIndex(pBookmark->GetFileName());
		if(index < 0) continue;

		m_pSetting->AddBookmark(index, pBookmark);
	}

	// ���Ϻ��� �з��� �ϸ�ũ���� �����Ѵ�.
	for(i=0; i< m_spineList.size(); i++)
	{
		IN3WebkitView *pView = (IN3WebkitView *)m_pViewManager->GetView(i);
		if(pView)
		{
			if(pView->m_nLoadState == IN3WebkitView::LS_LODE_DONE)
			{
				CString strAry;
				m_pSetting->GetInitBookmarkParam(i, strAry);
				if(strAry.IsEmpty()) continue;

				CString param;
				param.Format(_T("{\"BOOKMARK_LIST\":[%s]}"), strAry);
				pView->SendMessage(CMD_IN3_INIT_BOOKMARK_LIST, param);
			}
		}
	}
	return bRes;
}

/**
 * @brief	���� ���̴� �信 �ϸ�ũ�� ���ԵǾ� �ִ� �� Ȯ���Ѵ�.
 * @return	��Ŷ extension ���� �ϸ�ũ�� ���ԵǾ� �ִ��� Ȯ�ο�û�� �� ��� = TRUE, �׿�= FALSE 
 */
BOOL CEPubBook::QueryBookmarkStatus()
{
	if(!m_pViewManager){
		TRACEMSG_ERR(_T("m_pViewManager is NULL!!"));
		return FALSE;
	}

	BOOL bRes = FALSE;
	IN3WebkitView *pView = (IN3WebkitView *)m_pViewManager->GetCurrentView();
	if(pView)
	{
		bRes =  pView->SendMessage(CMD_IN3_QUERY_BOOKMARK_STATUS, NULL);
	}

	return bRes;
}

/**
 * @brief	������ é�Ϳ� �ϸ�ũ�� ���ԵǾ� �ִ� �� Ȯ���Ѵ�. (fixed ��麸�⿡�� �����)
 * @return	��Ŷ extension ���� �ϸ�ũ�� ���ԵǾ� �ִ��� Ȯ�ο�û�� �� ��� = TRUE, �׿�= FALSE 
 */
BOOL CEPubBook::QueryBookmarkStatus(int chapter)
{
	if(!m_pViewManager){
		TRACEMSG_ERR(_T("m_pViewManager is NULL!!"));
		return FALSE;
	}

	BOOL bRes = FALSE;
	IN3WebkitView *pView = (IN3WebkitView *)m_pViewManager->GetView(chapter);
	if(pView)
	{
		bRes =  pView->SendMessage(CMD_IN3_QUERY_BOOKMARK_STATUS, NULL);
	}

	return bRes;
}

/**
 * @brief	�ϸ�ũ �߰� �� ��Ŷ extension ���κ��� ȣ��Ǵ� callback �Լ�
 * @param	param	: extension ���� ���� ���޹��� JSON ������
 * @param	index	: �ϸ�ũ �� �߰��� ������ (fixed layout ������ ��ȿ�� ���� ����)
 * @see		ParseBookmarkParam
 */
void CEPubBook::OnBookmarkAdded(CString param, int index /*= 0*/)
{
	if(param.IsEmpty()) return;
	//
	IN3Bookmark *pBookmark = new IN3Bookmark;
	if(ParseBookmarkParam(param, pBookmark))
	{
		pBookmark->SetPage(index);
		// send information to UI
		CWnd* pParentWnd = GetParent();
		if(pParentWnd)
			pParentWnd->SendMessage(WM_BOOKMARK_ADDED, (WPARAM)this, (LPARAM)pBookmark);
	}

	if(pBookmark)
		delete pBookmark;
}

/**
 * @brief	�ϸ�ũ ���� �� ��Ŷ extension ���κ��� ȣ��Ǵ� callback �Լ�
 * @remarks	ClearBookmark() �Լ��� ���� callback �Լ�
 * @param	param	: extension ���� ���� ���޹��� JSON ������
 * @see		ParseParam, ClearBookmark
 */
void CEPubBook::OnBookmarkCleared(CString param)
{
	if(param.IsEmpty()) return;
	//
	JSONArray ary;
	// ������ �ϸ�ũ ID ����Ʈ�� �����Ѵ�.
	if(ParseParam(param, _T("MARKER_ID_LIST"), &ary))
	{
		std::vector<CString> *pMarkerIdList = new std::vector<CString>();
		if(!pMarkerIdList) return;

		// UI ������ ������ ������ �ϸ�ũ ����Ʈ�� �����Ѵ�.
		for(unsigned int i=0; i < ary.size(); i++)
		{
			JSONValue* pValue = ary[i];
			CString strId = pValue->Stringify().c_str();
			pMarkerIdList->push_back(strId);
		}

		if(pMarkerIdList->size() > 0)
		{
			// send information to UI
			CWnd* pParentWnd = GetParent();
			if(pParentWnd)
				pParentWnd->SendMessage(WM_BOOKMARK_CLEARED, (WPARAM)this, (LPARAM)pMarkerIdList);
		}

		if(pMarkerIdList){
			pMarkerIdList->clear();
			delete pMarkerIdList;
		}
	}	
}

/**
 * @brief	�ϸ�ũ ���� �� ��Ŷ extension ���κ��� ȣ��Ǵ� callback �Լ�
 * @remarks	DeleteBookmark() �Լ��� ���� callback �Լ�
 * @param	param	: extension ���� ���� ���޹��� JSON ������
 * @see		ParseParam, DeleteBookmark
 */
void CEPubBook::OnBookmarkDeleted(CString param)
{
	if(param.IsEmpty()) return;
	//
	JSONArray ary;
	if(ParseParam(param, _T("MARKER_ID_LIST"), &ary))
	{
		std::vector<CString> *pMarkerIdList = new std::vector<CString>();
		if(!pMarkerIdList) return;

		for(unsigned int i=0; i < ary.size(); i++)
		{
			JSONValue* pValue = ary[i];
			CString strId = pValue->Stringify().c_str();
			pMarkerIdList->push_back(strId);
		}

		if(pMarkerIdList->size() > 0)
		{
			// send information to UI
			CWnd* pParentWnd = GetParent();
			if(pParentWnd)
				pParentWnd->SendMessage(WM_BOOKMARK_DELETED, (WPARAM)this, (LPARAM)pMarkerIdList);
		}

		if(pMarkerIdList){
			pMarkerIdList->clear();
			delete pMarkerIdList;
		}
	}	
}

/**
 * @brief	�ϸ�ũ�� �ִ��� ��ȸ�ϸ� extension ���κ��� ȣ��Ǵ� callback �Լ�
 * @remarks	QueryBookmarkStatus() �Լ��� ���� callback �Լ�
 * @param	param	: extension ���� ���� ���޹��� JSON ������
 * @see		ParseParam, QueryBookmarkStatus
 */
void CEPubBook::OnEventBookmarkStatus(CString param, int fixedpage)
{
	if(param.IsEmpty()) return;

	bool bHasBookmark;
	// �Ķ���͸� �Ľ��Ͽ� �ϸ�ũ�� ���ԵǾ� �ִ��� �����´�.
	if(ParseParam(param, _T("HAS_BOOKMARK"), &bHasBookmark))
	{
		// fixed layout ��麸���� ���� ���� ��� ������ �並 ��� �˻��Ͽ� ���� �� �ϳ��� �ϸ�ũ�� ���� ���,
		// UI ������ �ϸ�ũ�� �ִٰ� �޽����� �����ش�.
		if((fixedpage > 0) && IsFixed() && GetPageDisplayMode() == 2)
		{
			// ������ ���� �������� ���� ������ �������� �����Ƿ� ���� �������� Ȯ��.
			if((fixedpage == (int)m_spineList.size()) && (fixedpage %2 == 1))
			{
				// send information to UI
				CWnd* pParentWnd = GetParent();
				if(pParentWnd)
					pParentWnd->SendMessage(WM_BOOKMARK_STATUS, (WPARAM)this, (LPARAM)&bHasBookmark);
			}
			else
			{
				static unsigned char flag = 0x00;
				// ���� �信 �ϸ�ũ�� �ִ��� ������ Ȯ�� �� ��� flag ������ ������ ��Ʈ�� 1�� �����Ѵ�.
				// ������ �信 �ϸ�ũ�� �ִ��� ������ Ȯ�� �� ��� flag ������ ������ �ٷ� �� ��Ʈ�� 1�� �����Ѵ�.
				// �ϸ�ũ�� ���� ��� flag ������ ������ ����° ��Ʈ�� 1�� �����Ѵ�.
				if(fixedpage %2 == 0)
				{
					flag |= 0x01;
					if(bHasBookmark) flag |= 0x04;
				}
				else
				{
					flag |= 0x02;
					if(bHasBookmark) flag |= 0x04;
				}

				// ���ʰ� ������ �� ��� Ȯ���� �Ǹ� flag �� ������ ��Ʈ�� ������ �ι�° ��Ʈ�� 1�� �����ȴ�.
				// ���� �� ��ο��� �ϸ�ũ�� Ȯ���� �Ǿ�, �ϸ�ũ�� �ִٰ� �ǴܵǸ� UI ������ �ϸ�ũ�� ������ �˸���.
				if((flag & 0x03) == 0x03)
				{
					// send information to UI
					bool bBookmark = ((flag & 0x04) == 0x04);
					flag = 0x00;
					CWnd* pParentWnd = GetParent();
					if(pParentWnd)
						pParentWnd->SendMessage(WM_BOOKMARK_STATUS, (WPARAM)this, (LPARAM)&bBookmark);
				}
			}
		}
		else
		{
			// send information to UI
			CWnd* pParentWnd = GetParent();
			if(pParentWnd)
				pParentWnd->SendMessage(WM_BOOKMARK_STATUS, (WPARAM)this, (LPARAM)&bHasBookmark);
		}
	}
}





/********************************************************************************
 * Find Text
 ********************************************************************************/
/**
 * @brief	���� �˻��� �Ѵ�.
 * @remarks �˻� ����� ����Ʈ�� ���޹޴´�.
 * @param	keyword	: �˻� ���ڿ�
 * @return	��Ŷ extension ���� ���� �˻��� ��û�� ��� = TRUE, �׿�= FALSE 
 * @see		OnFindTextAll
 */
BOOL CEPubBook::FindTextALL(CString keyword)
{
	if(keyword.IsEmpty()) return FALSE;
	if(!m_pViewManager) return FALSE;

	if(m_pResultList && m_pResultList->size())
		m_pResultList->clear();

	for(unsigned int i=0; i< m_spineList.size(); i++)
	{
		IN3WebkitView *pView = (IN3WebkitView *)m_pViewManager->GetView(i);
		if(pView)
		{
			if(pView->m_nLoadState == IN3WebkitView::LS_LODE_DONE)
			{
				CString param;
				param.Format(_T("{\"KEYWORD\":\"%s\"}"), keyword);
				pView->SendMessage(CMD_IN3_FIND_TEXT_ALL, param);
			}
		}
	}
	return TRUE;
}

/**
 * @brief	���� �������� �������� ���������� �˻��� �Ѵ�.
 * @remarks �˻� �� ��� ȭ������ �ٷ� �̵��Ѵ�.
 * @param	keyword		: �˻� ���ڿ�
 * @param	direction	: �̵� ����(true : forward, false : backward)
 * @return	��Ŷ extension ���� ���� �˻��� ��û�� ��� = TRUE, �׿�= FALSE 
 */
BOOL CEPubBook::FindTextAndMove(CString keyword, bool direction)
{
	if(keyword.IsEmpty()) return FALSE;
	if(!m_pViewManager) return FALSE;

	IN3WebkitView *pView = (IN3WebkitView *)m_pViewManager->GetCurrentView();
	if(pView)
	{
		if(pView->m_nLoadState == IN3WebkitView::LS_LODE_DONE)
		{
			CString param;
			param.Format(_T("{\"KEYWORD\":\"%s\", \"FORWARD\":%d}"), keyword, direction);
			pView->SendMessage(CMD_IN3_FIND_TEXT, param);
		}
	}
	return TRUE;
}

/**
 * @brief	���� �������� ���������� �˻��� �Ѵ�.
 * @remarks �˻� �� ��� ȭ������ �ٷ� �̵��Ѵ�.
 * @param	keyword		: �˻� ���ڿ�
 * @param	direction	: �̵� ����(true : forward, false : backward)
 * @param	chapter		: �˻��� é�� ����(spine ����)
 * @return	��Ŷ extension ���� ���� �˻��� ��û�� ��� = TRUE, �׿�= FALSE 
 */
BOOL CEPubBook::FindTextAndMove(CString keyword, bool direction, int chapter)
{
	if(keyword.IsEmpty()) return FALSE;
	if(!m_pViewManager) return FALSE;

	IN3WebkitView *pView = (IN3WebkitView *)m_pViewManager->GetView(chapter);
	if(pView)
	{
		if(pView->m_nLoadState == IN3WebkitView::LS_LODE_DONE)
		{
			CString param;
			param.Format(_T("{\"KEYWORD\":\"%s\", \"FORWARD\":%d}"), keyword, direction);
			pView->SendMessage(CMD_IN3_FIND_TEXT, param);
		}
	}
	return TRUE;
}

/**
 * @brief	���� �˻� �� webkit extension ���� ���� ȣ��Ǵ� callback �Լ�
 * @param	bExistNext	: ���� �˻� ����� �ִ��� Ȯ�� �� �� �ִ� �÷��� (true : ���� �˻� ��� ����, false : ���� �˻� ��� ����)	
 */
void CEPubBook::OnFindText(bool bExistNext)
{
	// send information to UI
	CWnd* pParentWnd = GetParent();
	if(pParentWnd)
		pParentWnd->SendMessage(WM_FIND_TEXT, (WPARAM)this, (LPARAM)&bExistNext);
}

/**
 * @brief	����ؼ� ���� �˻��� �ϴ� �Լ�
 * @remarks	���� �˻� ����� ���� ���, ���� ���̴� é���� ���� é�ͷ� �̵��Ͽ� �˻��� ��� �� �� ȣ���Ѵ�.
 *			�˻� ����� ���� �� �˻��� ������� �Ǵ��ϸ�, �׿ܿ� ������ ȣ������ �ʴ´�.
 * @param	keyword	: �˻� ���ڿ�
 * @param	chapter	: ��� �ؼ� �˻� �� é�� �ε���(spine �ε���)
 */
void CEPubBook::OnFindTextInNextChapter(CString keyword, int chapter)
{
	if(keyword.IsEmpty()) return;
	if(!m_pViewManager) return;
	if(chapter > (int)m_spineList.size()) return;

	FindTextAndMove(keyword, true, chapter);
}

/**
 * @brief	����ؼ� ���� �˻��� �ϴ� �Լ�
 * @remarks	���� �˻� ����� ���� ���, ���� ���̴� é���� ���� é�ͷ� �̵��Ͽ� �˻��� ��� �� �� ȣ���Ѵ�.
 *			�˻� ����� ���� �� �˻��� ������� �Ǵ��ϸ�, �׿ܿ� ������ ȣ������ �ʴ´�.
 * @param	keyword	: �˻� ���ڿ�
 * @param	chapter	: ��� �ؼ� �˻� �� é�� �ε���(spine �ε���)
 */
void CEPubBook::OnFindTextInPrevChapter(CString keyword, int chapter)
{
	if(keyword.IsEmpty()) return;
	if(!m_pViewManager) return;
	if(chapter < 0) return;

	FindTextAndMove(keyword, false, chapter);
}

/**
 * @brief ���� ���� ��ü �˻��� �� ���� ��Ŷ extension ���� ���� ȣ��Ǵ� callback �Լ�. �˻� ����� �Ķ���ͷ� �޴´�.
 * @param	param	: ���� ���� �˻� ���
 */
void CEPubBook::OnFindTextAll(CString param)
{
	if(param.IsEmpty()) return;
	if(m_pResultList == NULL) m_pResultList = new FindResultList;

	// �Ķ���� JSON ���� �Ľ�
	JSONValue* pValue = JSON::Parse(param);
	if(pValue == NULL) return;
	if(pValue->IsObject() == false) return;

	JSONObject root;
	root = pValue->AsObject();
	// "RESULT_CODE" �� 0 �� �ƴϸ� ����
	JSONObject::iterator iter = root.find(_T("RESULT_CODE"));
	if(iter != root.end() && root[_T("RESULT_CODE")]->IsNumber()){
		if(root[_T("RESULT_CODE")]->AsNumber() != 0.0)
			return;
	}

	JSONArray ary;
	CString fileName = _T("");
	// ������ �˻��� ���ϸ� ����
	iter = root.find(_T("FILE_NAME"));
	if(iter != root.end() && root[_T("FILE_NAME")]->IsString()){
		fileName = root[_T("FILE_NAME")]->Stringify().c_str();
	}
	
	// �˻� ��� ����Ʈ�� �����Ѵ�.
	FindResultList resultList;
	iter = root.find(_T("FIND_RESULT"));
	if(iter != root.end() && root[_T("FIND_RESULT")]->IsArray()){
		ary = root[_T("FIND_RESULT")]->AsArray();

		for(unsigned int i = 0; i<ary.size(); i++)
		{
			JSONValue* pResValue = ary.at(i);

			JSONObject resRoot;
			resRoot = pResValue->AsObject();

			IN3FindResult findResult;
			findResult.SetFileName(fileName);

			// seqence
			JSONObject::iterator it = resRoot.find(_T("SEQ"));
			if(it != resRoot.end() && resRoot[_T("SEQ")]->IsNumber()){
				int seq = int(resRoot[_T("SEQ")]->AsNumber());
				findResult.SetSeq(seq);
			}
			// page
			it = resRoot.find(_T("PAGE"));
			if(it!= resRoot.end() && resRoot[_T("PAGE")]->IsNumber()){
				int page = int(resRoot[_T("PAGE")]->AsNumber());
				findResult.SetPage(page);
			}
			// start index
			it = resRoot.find(_T("START_INDEX"));
			if(it!= resRoot.end() && resRoot[_T("START_INDEX")]->IsNumber()){
				int startindex = int(resRoot[_T("START_INDEX")]->AsNumber());
				findResult.SetStartIndex(startindex);
			}
			// end index
			it = resRoot.find(_T("END_INDEX"));
			if(it!= resRoot.end() && resRoot[_T("END_INDEX")]->IsNumber()){
				int endindex = int(resRoot[_T("END_INDEX")]->AsNumber());
				findResult.SetEndIndex(endindex);
			}
			// sentence
			it = resRoot.find(_T("SENTENCE"));
			if(it!= resRoot.end() && resRoot[_T("SENTENCE")]->IsString()){
				CString sentence = resRoot[_T("SENTENCE")]->Stringify().c_str();
				findResult.SetSentence(sentence);
			}
			resultList.push_back(findResult);
			m_pResultList->push_back(findResult);
		}
	}

	CWnd* pParentWnd = GetParent();
	if(pParentWnd)
		pParentWnd->SendMessage(WM_FIND_TEXT_ALL, (WPARAM)this, (LPARAM)&resultList);
}

/**
 * @brief	�˻� ����� ��ȯ�ϴ� �Լ�
 * @remarks	���� ��ü �˻� ��, ��� ����� �Ѳ����� ���� �� ����Ѵ�.
 * @return	�˻� ��� ����Ʈ
 */
FindResultList* CEPubBook::GetFindTextAllResult()
{
	return m_pResultList;
}

/**
 * @brief	�˻� ����� ������ ��ȯ�ϴ� �Լ�
 * @return	�˻� ��� ����Ʈ ����
 */
int CEPubBook::GetFindTextAllResultCount()
{
	return m_pResultList->size();
}







/********************************************************************************
 * interface
 ********************************************************************************/
/**
 * @brief		OPF file �� �д´�. 
 * @remarks		OPF �� ����� å�� ���� �� TOC ������ ����� ���� ������ �ؼ��Ѵ�.
 * @param		lpszPathName	: OPF file ���� ���
 * @return		success(TRUE)/fail(FALSE)
 */
BOOL CEPubBook::ReadOPSFile(LPCTSTR lpszPathName)
{
	TRACEMSG(_T("CEPubBook::ReadOPSFile() START"));
	//
	BOOL bRes = TRUE;
	OPFXML	opfXML;
	// opf ������ �Ľ��Ѵ�.
	if(!opfXML.Open(lpszPathName))
	{
		TRACEMSG_ERR(_T("EPubBook::ReadOPSFile() can't open opf file for parsing!!"));
		return FALSE;
	}
	//
	UniString uniStr;
	// 1. Get Epub Version
	if (opfXML.GetVersion(uniStr)){
		CString version(uniStr);
		float fVersion = (float)_wtof(version);

	#if (SUPPORT_EPUB_VERSION == EPUB_VER_2)
		// �������� epub 2.0 �� �����ϴ� ���, �� �̻��� ���������� å�� open ���� �ʴ´�.
		if(fVersion == 3.0) return FALSE;
	#endif

		m_pBookInfo->SetVersion(fVersion);
	}

	// 2. Get Epub UUID
	if (opfXML.GetUniqueID(uniStr)){
		CString uuid(uniStr);
		m_pBookInfo->SetUUIDString(uuid);
	}

	// 3. Get Epub Title
	if (opfXML.GetDCTitle(uniStr)){
		CString title(uniStr);
		m_pBookInfo->SetTitle(title);
	}

	CString str;
	CXMLDOMElement metadata = opfXML.GetMetadata();

	// 4. Get Epub Creator (== Author)
	metadata.SelectSingleNode(L"dc:creator").GetText(uniStr);
	str = uniStr;
	m_pBookInfo->SetCreator(str);

	// 5. Get Epub Publisher
	metadata.SelectSingleNode(L"dc:publisher").GetText(uniStr);
	str = uniStr;
	m_pBookInfo->SetPublisher(str);

	// 6. Get Epub description
	metadata.SelectSingleNode(L"dc:description").GetText(uniStr);
	str = uniStr;
	m_pBookInfo->SetDesc(str);

	// 7. Get Nav Type & nav href
	m_pBookInfo->SetNavType(opfXML.GetNavigationURL(uniStr));
	str = uniStr;
	m_pBookInfo->SetTocFilePath(str);
	
	// 8. Get cover href
	if (opfXML.GetCoverPageImage(uniStr))
	{
		CString coverHref(uniStr);
		m_pBookInfo->SetCoverHref(coverHref);
	}

	// 9. Get spine List
	CXMLDOMNodeList spines = opfXML.GetSpineItemRefs(false);
	for(int i=0; i< spines.GetCount(); i++)
	{
		CXMLDOMNode	child = spines.GetItem(i);

		UniString id;
		UniString href;
		child.GetAttribute(L"idref", id);
		ManifestItem item =  opfXML.GetManifestItemById(id);
		item.GetHref(href);

		if( !href.IsEmpty() )
		{
			CString str(href);
			m_spineList.push_back(str);
		}
	}
	//
	TRACEMSG(_T("CEPubBook::ReadOPSFile() END"));
	//
	return bRes;
}

/**
 * @brief	toc ������ �Ľ��Ѵ�.
 * @return	true(success)/false(fail)
 */
BOOL CEPubBook::ReadTocFile()
{
	TRACEMSG(_T("CEPubBook::ReadTocFile() START"));
	//
	if(!m_pBookInfo) return FALSE;
	//
	BOOL bRes = TRUE;
	CString strNavPath(GetOPSFolderPath());
	strNavPath.Append(m_pBookInfo->GetTocFilePath());
	strNavPath.Replace(_T("/"), _T("\\"));
	
	// ������ �ֻ��� root ������Ʈ�� å�� Ÿ��Ʋ ������ �����.
	m_tocTree.SetTitle(m_pBookInfo->GetTitle());

	// epub ������ ���� ������ �Ľ��ϴ� ����� �޶�����. EPub 2.0 => ncx,  EPub 3.0 =>nav
	if(m_pBookInfo->GetVersion() == 3.0)
	{
		NAVXML navReader;
		if(!navReader.Open(strNavPath))
		{
			TRACEMSG_ERR(_T("CEPubBook::ReadNav() can't open toc file for parsing!!"));
			return FALSE;
		}
		//
		AppendNavPointOfNAV(m_tocTree, navReader.GetNavList());
	}
	else
	{
		NCXXML ncxReader;
		if(!ncxReader.Open(strNavPath))
		{
			TRACEMSG_ERR(_T("CEPubBook::ReadNav() can't open toc file for parsing!!"));
			return FALSE;
		}
		//
		AppendNavPointOfNCX(m_tocTree, ncxReader.GetNavPointList());
	}
	//
	TRACEMSG(_T("CEPubBook::ReadTocFile() END"));
	//
	return bRes;
}

/**
 * @brief �������κ��� ���޹��� event ���� parameter �� �ؼ��Ѵ�.
 * @param	[in] param		: �������� ���� ���� ������.(�Ľ��ؾ� �� ������)
 *			[in] key		: �Ľ��Ͽ� ���� �������� Ű ��
 *			[out] resValue	: �Ľ��Ͽ� ���� �������� ������
 * @return TRUE(success) / FALSE(fail)
 */
BOOL CEPubBook::ParseParam(CString param, CString key, void *resValue)
{
	if(param.IsEmpty()) return FALSE;
	//
	BOOL bRes = TRUE;
	JSONValue* value = JSON::Parse(param);
	if(value == NULL)
	{
		TRACEMSG_ERR(_T("[CEPubBook::ParseParam] recived data can't parsing!!"));
		return FALSE;
	}

	if(value->IsObject() == false)
	{
		TRACEMSG_ERR(_T("[CEPubBook::ParseParam] The root element is not an object!!"));
		return FALSE;
	}

	JSONObject root;
	root = value->AsObject();
	JSONObject::iterator iter = root.find(_T("RESULT_CODE"));
	if(iter != root.end() && root[_T("RESULT_CODE")]->IsNumber()){
		if(root[_T("RESULT_CODE")]->AsNumber() != 0.0)
			return FALSE;
		/*CString result = root[_T("RESUOT_CODE")]->Stringify().c_str();
		result.Replace(_T("\\"), _T(""));
		if(result != _T("0"))
		{
			return FALSE;
		}*/
	}

	//
	iter = root.find(key.GetString());
	if(iter!= root.end())
	{
		if(root[key.GetString()]->IsString()){
			((CString *)resValue)->Format(_T("%s"), root[key.GetString()]->Stringify().c_str());
			((CString *)resValue)->Replace(L"\"",L"");
		}
		else if(root[key.GetString()]->IsNumber()){
			*((int *)resValue) = int(root[key.GetString()]->AsNumber());
		}
		else if(root[key.GetString()]->IsBool()){
			*((bool *)resValue) = root[key.GetString()]->AsBool();
		}
		else if(root[key.GetString()]->IsArray()){
			*((JSONArray *)resValue) = root[key.GetString()]->AsArray();
		}
	}

	if(value)
		delete value;
	return TRUE;
}

/**
 * @brief �������κ��� ���޹��� event ���� parameter �� �ؼ��Ͽ� ���ϴ� ���� ��Ʈ������ �����Ѵ�.
 * @param	[in] param		: �������� ���� ���� ������.(�Ľ��ؾ� �� ������)
 *			[in] key		: �Ľ��Ͽ� ���� �������� Ű ��
 *			[out] resValue	: �Ľ��Ͽ� ���� ������
 * @return TRUE(success) / FALSE(fail)
 */
BOOL CEPubBook::ParseParamToString(CString param, CString key, CString &resValue)
{
	if(param.IsEmpty()) return FALSE;
	//
	BOOL bRes = TRUE;
	JSONValue* value = JSON::Parse(param);
	if(value == NULL)
	{
		TRACEMSG_ERR(_T("[CEPubBook::ParseParamToString] recived data can't parsing!!"));
		return FALSE;
	}

	if(value->IsObject() == false)
	{
		TRACEMSG_ERR(_T("[CEPubBook::ParseParamToString] The root element is not an object!!"));
		return FALSE;
	}

	JSONObject root;
	root = value->AsObject();
	JSONObject::iterator iter = root.find(_T("RESULT_CODE"));
	if(iter != root.end() && root[_T("RESULT_CODE")]->IsNumber()){
		if(root[_T("RESULT_CODE")]->AsNumber() != 0.0)
			return FALSE;
		/*CString result = root[_T("RESUOT_CODE")]->Stringify().c_str();
		result.Replace(_T("\\"), _T(""));
		if(result != _T("0"))
		{
			return FALSE;
		}*/
	}

	//
	iter = root.find(key.GetString());
	if(iter!= root.end())
	{		
		resValue = root[key.GetString()]->Stringify().c_str();
	}

	if(value)
		delete value;
	return TRUE;
}

/**
 * @brief �������κ��� ���޹��� �˻� ��� event �� parameter �� �ؼ��Ѵ�.
 * @param	[in] param		: �������� ���� ���� ������.(�Ľ��ؾ� �� ������)
 *			[out] bMatches	: ���� �˻� ����� �ִ� ���� ��Ÿ���� ������
 *			[out] bForward	: �˻� ������ ���ٳ��� ������ (true : next, false : prev)
 *			[out] page		: �˻� ����� �ִ� ������
 *			[out] keyword	: �˻� Ű����
 * @return TRUE(success) / FALSE(fail)
 */
BOOL CEPubBook::ParseFindTextParam(CString param, bool &bMatches, bool &bForward, int &page, CString &keyword)
{
	if(param.IsEmpty()) return FALSE;
	//
	BOOL bRes = TRUE;
	JSONValue* value = JSON::Parse(param);
	if(value == NULL)
	{
		TRACEMSG_ERR(_T("[CEPubBook::ParseFindTextParam] recived data can't parsing!!"));
		return FALSE;
	}

	if(value->IsObject() == false)
	{
		TRACEMSG_ERR(_T("[CEPubBook::ParseFindTextParam] The root element is not an object!!"));
		return FALSE;
	}

	JSONObject root;
	root = value->AsObject();
	JSONObject::iterator iter = root.find(_T("RESULT_CODE"));
	if(iter != root.end() && root[_T("RESULT_CODE")]->IsString()){
		CString result = root[_T("RESUOT_CODE")]->Stringify().c_str();
		result.Replace(_T("\\"), _T(""));
		if(result != _T("0"))
		{
			return FALSE;
		}
	}
	// matches
	iter = root.find(_T("MATCHES"));
	if(iter!= root.end() && root[_T("MATCHES")]->IsBool()){
		bMatches = root[_T("MATCHES")]->AsBool();
	}
	// direction
	iter = root.find(_T("FORWARD"));
	if(iter!= root.end() && root[_T("FORWARD")]->IsBool()){
		bForward = root[_T("FORWARD")]->AsBool();
	}
	// page
	iter = root.find(_T("CURRENT_PAGE"));
	if(iter!= root.end() && root[_T("CURRENT_PAGE")]->IsNumber()){
		page = int(root[_T("CURRENT_PAGE")]->AsNumber());
	}
	// keyword
	iter = root.find(_T("KEYWORD"));
	if(iter!= root.end() && root[_T("KEYWORD")]->IsString()){
		keyword = root[_T("KEYWORD")]->Stringify().c_str();
	}

	if(value)
		delete value;
	return TRUE;
}







/********************************************************************************
 * Protected Method
 ********************************************************************************/
/**
  @brief	GetPackageFileName member function
  @remarks	OPS ������� OPS ������ ��θ� �����´�.
  @param	[in] lpszFilePath		"container.xml" �� ���� ���
  @param	[out] oOPSFolderName	OPS ������
  @param	[out] oOPSFileName		OPS ������ ���
  @return	TRUE(success) / FALSE (fail)
*/
bool CEPubBook::GetPackageFileName(LPCTSTR lpszFilePath, CString &oOPSFolderName, CString &oOPSFileName)
{
	TRACEMSG(_T("CEPubBook::GetPackageFileName() START"));
	//
	// container.xml ������ �Ľ��Ѵ�.
	UniString uniOPSFolderName, uniOPSFileName;
	CXMLDOMDocument2	xmlDom;
	bool ok = xmlDom.Open(lpszFilePath);
	if(!ok)
	{
		TRACEMSG_ERR(_T("can't open \"container.xml\"!!"));
		return FALSE;
	}
	//
	// opf ������ ��θ� �����´�.
	UniString opfFilePath;
	xmlDom.SelectionNamespaces("xmlns:container='urn:oasis:names:tc:opendocument:xmlns:container'");
	CXMLDOMElement elem = xmlDom.SelectSingleNode(L"//container:rootfile[@media-type='application/oebps-package+xml']");
	ok = elem.GetAttribute(L"full-path", opfFilePath);
	if ( opfFilePath.IsEmpty() ){
		TRACEMSG_ERR(_T("opf file path is empty!!"));
		return FALSE;
	}
	//
	// opf ���Ͽ��� '/' �� �������� ops ����(��Ű�� ����) ��� opf ���� ���� �����Ѵ�.
	STRIndex_t at = opfFilePath.FindRAt('/');
	if ( STRING_FOUND(at) )
	{
		opfFilePath.GetString(0, at, uniOPSFolderName);

		at++;
		opfFilePath.GetString(at, uniOPSFolderName.GetCount() - at, uniOPSFileName);
	}
	else
	{
		uniOPSFolderName.Empty();
		uniOPSFileName = opfFilePath;
	}
	//
	oOPSFolderName = uniOPSFolderName;
	oOPSFileName = uniOPSFileName;
	//
	TRACEMSG(_T("CEPubBook::GetPackageFileName() END"));
	//
	return ok;	
}

/**
 * @brief	�������� fixed layout ���� �����Ǿ����� Ȯ���Ѵ�.
 * @remarks	����� apple spec, samsung spec ���� �˻��Ѵ�. ���� IDPF spec �� �ݿ�����.
 * @param	lpszFilePath	: Epub �� unzip �� ������
 * @return	TRUE(fixed) / FALSE(reflow)
 */
bool CEPubBook::GetFixedFeature(LPCTSTR lpszFilePath)
{
	// apple fixed ��Ģ�� Ȯ���Ѵ�.
	bool bFixed = GetAppleFeature(lpszFilePath);
	if(!bFixed)
	{
		// apple fixed �� �ƴ� ���, �Ｚ�� fixed ��Ģ�� Ȯ���Ѵ�.
		bFixed = GetSamsungFeature(lpszFilePath);
	}
	return bFixed;
}


/**
 * @brief	Apple ���� ������ fixed layout ��Ģ�� Ȯ���Ѵ�.
 * @remarks	"com.apple.ibooks.display-options.xml" ������ �־�� �ϸ�, �� �ȿ� fixed true/false �� ������ ����.
 * @param	lpszFilePath	: Epub �� unzip �� ������
 * @return	TRUE(fixed) / FALSE(reflow)
 */
bool CEPubBook::GetAppleFeature(LPCTSTR lpszFilePath)
{
	CString filePath(lpszFilePath);
	filePath.Append(_T("META-INF\\com.apple.ibooks.display-options.xml"));
	filePath.Replace(_T("/"), _T("\\"));

	CXMLDOMDocument2	xmlDom;
	bool ok = xmlDom.Open(filePath);
	if ( !ok )
	{
		TRACEMSG_ERR(_T("can't open \"com.apple.ibooks.display-options.xml\"!!"));
		return false;
	}

	CXMLDOMElement elem = xmlDom.SelectSingleNode(_T("//display_options/platform/option[@name='fixed-layout']"));
	if(elem.IsValid())
	{
		CString fixedString = elem.GetText();
		if ( fixedString.IsEmpty() )
		{
			TRACEMSG_ERR(_T("fixed-layout's value empty!!"));
			return false;
		}
		return ((fixedString.Compare(_T("true")) == 0) || (fixedString.Compare(_T("TRUE")) == 0));
	}
	else
		return false;
}

/**
 * @brief	samsung ���� ������ fixed layout ��Ģ�� Ȯ���Ѵ�.
 * @remarks	"samsung.opm.property.xml" ������ �־�� �Ѵ�. �ش� ���Ͽ��� Portraint/Landscape/Both �� ������ ����Ǿ� �ִ�.
 * @param	lpszFilePath	: Epub �� unzip �� ������
 * @return	TRUE(fixed) / FALSE(reflow)
 */
bool CEPubBook::GetSamsungFeature(LPCTSTR lpszFilePath)
{
	CString filePath(lpszFilePath);
	filePath.Append(_T("META-INF\\samsung.opm.property.xml"));
	filePath.Replace(_T("/"), _T("\\"));

	CXMLDOMDocument2	xmlDom;
	bool ok = xmlDom.Open(filePath);
	if ( !ok )
	{
		TRACEMSG_ERR(_T("can't open \"samsung.opm.property.xml\"!!"));
		return false;
	}

	CXMLDOMElement elem = xmlDom.SelectSingleNode(_T("//property_options/option[@value='orientation-lock']"));
	if(elem.IsValid())
	{
		CString fixedString = elem.GetText();
		if ( fixedString.IsEmpty() )
		{
			TRACEMSG_ERR(_T("fixed-layout's value empty!!"));
			return false;
		}

		if(fixedString == _T("Portrait-only"))
		{
			if(m_pBookInfo)
				m_pBookInfo->SetFixedOption(CBookInfo::FIXED_PORTRAIT);
		}
		else if (fixedString == _T("Landscape-only"))
		{
			if(m_pBookInfo)
				m_pBookInfo->SetFixedOption(CBookInfo::FIXED_LANDSCAPE);
		}
		else
		{
			if(m_pBookInfo)
				m_pBookInfo->SetFixedOption(CBookInfo::FIXED_BOTH);
		}

		return true;
	}
	else
		return false;
}

#if SUPPORT_DELETE_UNZIPDIR
/**
 * @brief	unzip �� ��� ���� ������ ��� �����Ѵ�.
 * @param	lpszFilePath	: epub �� unzip �� ���
 * @return	TRUE(success) / FALSE(fail)
 */
bool CEPubBook::DeleteFolderAndFiles(LPCTSTR lpszFilePath)
{
	if(lpszFilePath == NULL || *lpszFilePath == 0) return FALSE;

	CString unzipPath(lpszFilePath);
	if(unzipPath.Right(1) != _T("\\"))
		unzipPath += _T("\\");
	unzipPath += _T("*.*");

	WIN32_FIND_DATA findFileData;
	HANDLE hFind;
	hFind = FindFirstFile(unzipPath, &findFileData);
	if (hFind != INVALID_HANDLE_VALUE) 
	{
		CString	filePath;

		do
		{
			if ( _tcscmp(findFileData.cFileName, _T(".")) == 0 || _tcscmp(findFileData.cFileName, _T("..")) == 0 )
				continue;

			filePath = lpszFilePath;
			filePath += _T("\\");
			filePath += findFileData.cFileName;

			if ( findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				DeleteFolderAndFiles(filePath);
			}
			else
			{
				DeleteFile(filePath);
			}
		}
		while (FindNextFile(hFind, &findFileData) != 0);

		FindClose(hFind);
	}

	RemoveDirectory(lpszFilePath);

	return TRUE;
}
#endif /*SUPPORT_DELETE_UNZIPDIR*/

/**
 * @brief	Ư�� ������ spine �ε����� �����´�.
 * @param	fileName	: spine�� �ε����� Ȯ���� ���ϸ�
 * @return	spine �ε��� ��(0~max)
 * @bug		������ ���� #sefe ������ ������ ��쿡 ���� ó���� �Ǿ� ���� ����.
 */
int	 CEPubBook::GetSpineIndex(CString fileName)
{
	if(fileName.IsEmpty()) return -1;

	CString strName;
	fileName.Replace(_T("/"), _T("\\"));

	// �Ķ���ͷ� �Ѿ�� ���ϸ��� ����������, ��������� �� �� �����Ƿ�, �ϴ�, ��Ű������(OEBPS) �� �������� ����η� ���ϸ��� �����Ѵ�.
	// OPF ������ ���ٸ� �ڿ������� "\\" �����ڷ� ������ ���ϸ��� �����Ѵ�.
	int index = fileName.Find(m_pBookInfo->GetPackageName());
	if(index >= 0){
		index += m_pBookInfo->GetPackageName().GetLength();
		strName = fileName.Right(fileName.GetLength() - index - 1);
	}
	else
	{
		index = fileName.ReverseFind('\\');
		if(index >= 0){
			strName = fileName.Right(fileName.GetLength() - index - 1);
		}
	}

	if(strName.IsEmpty()) return -1;

	// spine ����Ʈ���� ���ϸ��� ��ġ�ϴ� spine �ε����� ã�´�.
	for(UINT i=0; i< m_spineList.size(); i++)
	{
		CString spineURL = m_spineList.at(i);	
		spineURL.Replace(_T("/"), _T("\\"));	
		if( spineURL.Find(strName) >= 0){
			return i;
		}
	}

	return -1;
}

/**
 * @brief	�޸� �����ܵ��� Package ���� $(unzipPath)\OEPBS\ �� ��η� �����Ѵ�.
 * @remarks	�׳� �ᵵ �ǳ� �� xhtml ���� �޸� �߰��� url �� ��� ��η� �����ϱ� ���ؼ�. ���� ��� �ȵ�.
 */
void CEPubBook::CopyMemoIcons()
{
	TCHAR    Temp[_MAX_PATH+1];
	VERIFY(::GetModuleFileName(AfxGetApp()->m_hInstance, Temp, _MAX_PATH));
	CString ProgramDir = Temp;

	int len = ProgramDir.ReverseFind('\\');
	if ( len > 1 )	ProgramDir = ProgramDir.Left( len + 1 );

	CString descPath = GetOPSFolderPath();

	CString descFile(descPath);
	descFile.Append(_T("memo_left.png"));
	CString srcFile(ProgramDir);
	srcFile.Append(IN3WebLib::GetPathMemoLeftIcon());
	srcFile.Replace(_T("/"), _T("\\"));

	CopyFile(srcFile, descFile, true);

	descFile = descPath;
	descFile.Append(_T("memo_right.png"));
	srcFile = ProgramDir;
	srcFile.Append(IN3WebLib::GetPathMemoRightIcon());
	srcFile.Replace(_T("/"), _T("\\"));

	CopyFile(srcFile, descFile, true);
}

/**
 * @brief	Epub 3.0 spec �� �ؼ��Ͽ� ���� ������ �����Ѵ�.
 * @param	[in] navPointNode	: parent ���� ���
 * @param	[in] navPoints	: toc ���Ϸκ��� �Ľ��� ���� ������
 */
void CEPubBook::AppendNavPointOfNCX(IN3NavPointNode &navPointNode, CXMLDOMNodeList &navPoints)
{
	// �Ľ̵� �����ͷ� ���� ���� ����Ʈ�� ������ �����´�.
	long count = navPoints.GetCount();
	//
	// ���� ���� ��ŭ ���� �����͸� �����Ͽ� ù��° �Ķ���ͷ� �Ѿ�� �θ� ������ �ڽ� ������ �߰��Ѵ�.
	for(long i = 0; i<count; i++)
	{
		CXMLDOMNode child;
		child = navPoints.GetItem(i);

		// ������, ���� ��θ� ����
		UniString	uniTitle;
		UniString	uniURL;
		child.SelectSingleNode(USTR("./ncx:navLabel/ncx:text")).GetText(uniTitle);
		child.SelectSingleNode(USTR("./ncx:content")).GetAttribute(USTR("src"), uniURL);

		CString title(uniTitle);
		CString url(uniURL);
		// 
		// fixed layout �������� ���� ������ ������ ������ �����Ѵ�. fixed ������ 1����==1������ �̹Ƿ� ������ �ε����� ������ ���� ����.
		if(IsFixed())
		{
			// fixed contents
			UINT index = 0;
			index =	GetSpineIndex(url) + 1;
			if(index >= m_spineList.size()) index = 0;
			navPointNode.Append(IN3NavPointNode(title, url, index));
		}
		else
		{
			// reflow contents
			navPointNode.Append(IN3NavPointNode(title, url));
		}

		AppendNavPointOfNCX(navPointNode.GetChild(i), child.SelectNodes(USTR("./ncx:navPoint")));
	}
}

/**
 * @brief	Epub 2.0 spec �� �ؼ��Ͽ� ���� ������ �����Ѵ�.
 * @param	[in] navPointNode	: parent ���� ���
 * @param	[in] navPoints	: toc ���Ϸκ��� �Ľ��� ���� ������
 */
void CEPubBook::AppendNavPointOfNAV(IN3NavPointNode &navPointNode, CXMLDOMNodeList &navPoints)
{	
	long count = navPoints.GetCount();
	for(long i = 0; i<count; i++)
	{
		CXMLDOMNode child;
		child = navPoints.GetItem(i);

		// ������, ���� ��θ� ����
		UniString	uniTitle;
		UniString	uniURL;
		NAVXML::GetHrefAndTitleFromLI(child, uniURL, uniTitle);

		CString title(uniTitle);
		CString url(uniURL);
		// 
		// fixed layout �������� ���� ������ ������ ������ �����Ѵ�. fixed ������ 1����==1������ �̹Ƿ� ������ �ε����� ������ ���� ����.
		if(IsFixed())
		{
			// fixed contents
			UINT index = 0;
			index =	GetSpineIndex(url) + 1;
			if(index >= m_spineList.size()) index = 0;
			navPointNode.Append(IN3NavPointNode(title, url, index));
		}
		else
		{
			// reflow contents
			navPointNode.Append(IN3NavPointNode(title, url));
		}

		AppendNavPointOfNAV(navPointNode.GetChild(i), child.SelectNodes(USTR("./ol/li")));
	}
}

/**
 * @brief	parent window �� epub file �� ���������� Load ������ �˸��� �Լ�
 */
void CEPubBook::DocumentLoadedComplate()
{
	TRACEMSG(_T("CEPubBook::DocumentLoadedComplate() START"));
	//
	// ��� document �� load �Ǿ��� �� 
	// ���� Ȱ��ȭ �� spine(chapter) ������ ���ٸ�, ����Ʈ spine ������ Ȱ��ȭ ��Ų��. ( default spine(chaptr) index : 0 )
	// ������ ������ ���� ��� default spine index = 0
	if(m_pViewManager->GetCurrChapter() < 0)
	{
		MoveToChapter(0);
	}
	//
	// send massage to UI
	CWnd* pParentWnd = GetParent();
	if(pParentWnd)
		pParentWnd->SendMessage(WM_LODEAD_EPUB, (WPARAM)this, (LPARAM)m_bInitialized);	// UI ������ ������ Load complete message
	//
	TRACEMSG(_T("CEPubBook::DocumentLoadedComplate() END"));
}

/**
 * @brief	view change �ϴ� �Լ�
 * @param	pPrev	: ���� ��(���� �����ִ� ��)
 * @parma	pNext	: ������ ��(������ ���̰� �� ��)
 * @return	true(success)/false(fail)
 */
BOOL CEPubBook::ChangeView(void *pPrev, void *pNext)
{
	if(!pNext) return FALSE;

	// ���� �並 �Ⱥ��̰� �ϸ�, inactive �ϰ� �����Ѵ�.
	IN3WebkitView *pPrevView = (IN3WebkitView *)pPrev;
	if(pPrevView){
		pPrevView->SetDlgCtrlID(AFX_IDW_PANE_FIRST + pPrevView->GetChapterIndex() +1);
		pPrevView->ShowWindow(SW_HIDE);
		pPrevView->EnableWindow(FALSE);
	}

	// ������ �並 ���̰� �ϸ�, �ֻ��� ������� �ø���, active window �� �����Ѵ�.
	IN3WebkitView *pNextView = (IN3WebkitView *)pNext;
	if(pNextView)
	{
		//pNextView->ReLoad();
		pNextView->EnableWindow(TRUE);
		pNextView->SetDlgCtrlID(AFX_IDW_PANE_FIRST);
		pNextView->BringWindowToTop();
		pNextView->ShowWindow(SW_SHOW);
		pNextView->SetFocus();
		pNextView->Invalidate();

		this->SetActiveView(pNextView);
		this->RecalcLayout();
	}
	return TRUE;
}

/**
 * @brief	view change �ϴ� �Լ� (fixed �������� ��� ������ ���� ���� ���̴� �� 2���� ������ ���� �� 2���� �����ؾ� �Ѵ�.)
 * @remarks	fixed ��麸���� ���� ������ ���� ���� �������� ��� �ϸ�ũ�� �ִ��� Ȯ���ؾ� �ϹǷ� �ش� �Լ����� �ϸ�ũ�� �ִ��� Ȯ���Ѵ�.
 * @param	pPrev	: ���� ��(���� �����ִ� ��)
 * @parma	pNext	: ������ ��(������ ���̰� �� ��)
 * @return	true(success)/false(fail)
 */
BOOL CEPubBook::ChapgeViewFixedDual(void* pPrev, void *pNext)
{
	if(!pNext) return FALSE;

	// ���� ���̴� �並 �Ⱥ��̰� �����Ѵ�.
	IN3WebkitView *pLeft, *pRight;
	int chapter = 1, leftChapter =0, rightChapter = 1;
	if(pPrev)
	{
		// ���� ���̴� ���� ��� ������ ���� �ε����� �����´�
		chapter = ((IN3WebkitView *)pPrev)->GetChapterIndex();
		leftChapter = chapter % 2 == 0 ? chapter : chapter - 1;
		rightChapter = leftChapter+1;

		// ���� ���̴� ���� �並 �Ⱥ��̰� ó��
		pLeft = (IN3WebkitView *)m_pViewManager->GetView(leftChapter);
		if(pLeft)
		{
			pLeft->SetDlgCtrlID(AFX_IDW_PANE_FIRST + leftChapter + 1);
			pLeft->ShowWindow(SW_HIDE);
			pLeft->EnableWindow(FALSE);
		}
		
		// ���� ���̴� ������ �並 �Ⱥ��̰� ó��
		pRight = (IN3WebkitView *)m_pViewManager->GetView(rightChapter);
		if(pRight)
		{
			pRight->SetDlgCtrlID(AFX_IDW_PANE_FIRST + rightChapter + 1);
			pRight->ShowWindow(SW_HIDE);
			pRight->EnableWindow(FALSE);
		}
	}

	// ������ ������ �ϴ� ���� ��� ������ ���� �ε����� �����´�
	chapter = ((IN3WebkitView *)pNext)->GetChapterIndex();
	leftChapter = chapter % 2 == 0 ? chapter : chapter - 1;
	rightChapter = leftChapter+1;

	// ������ ���̰� �� ���� �並 ���̰� ó��
	pLeft = (IN3WebkitView *)m_pViewManager->GetView(leftChapter);
	if(pLeft)
	{
		pLeft->EnableWindow(TRUE);
		pLeft->SetDlgCtrlID(AFX_IDW_PANE_FIRST);
		pLeft->BringWindowToTop();
		pLeft->ShowWindow(SW_SHOW);
		pLeft->Invalidate();
		//
		// ���� �̵��� �������� �ϸ�ũ ������ ��ȸ�Ѵ�.
		QueryBookmarkStatus(leftChapter);		
	}

	// ������ ���̰� �� ������ �並 ���̰� ó��
	pRight = (IN3WebkitView *)m_pViewManager->GetView(rightChapter);
	if(pRight)
	{
		pRight->EnableWindow(TRUE);
		pRight->SetDlgCtrlID(AFX_IDW_PANE_FIRST);
		pRight->BringWindowToTop();
		pRight->ShowWindow(SW_SHOW);
		pRight->Invalidate();
		//
		// ���� �̵��� �������� �ϸ�ũ ������ ��ȸ�Ѵ�.
		QueryBookmarkStatus(rightChapter);		
	}
	//
	///*
	if(chapter == leftChapter)
	{
		//pRight->EnableWindow(FALSE);
		this->SetActiveView(pLeft);
	}
	else
	{
		//pLeft->EnableWindow(FALSE);
		this->SetActiveView(pRight);
	}
	//*/

	return TRUE;
}

/**
 * @brief	Reflow �� Layout �� �������Ѵ�.
 * @return	true(success)/false(fail)
 */
BOOL CEPubBook::ReLayoutReflow()
{
	if(IsFixed()) return FALSE;
	if(!m_pSetting) return FALSE;
	if(!m_pViewManager) return FALSE;
	//
	// get epub view's size
	CRect clientRect;
	GetClientRect(&clientRect);

	DWORD lParam = 0L;
	lParam = ((WORD)clientRect.Height()<<16) | (WORD)clientRect.Width();

	// relayout
	int count = m_pViewManager->GetCount();
	for(int i=0; i<count; i++)
	{
		IN3WebkitView *pWebView = (IN3WebkitView *)m_pViewManager->GetView(i);
		if(pWebView)
		{
			// �信 WM_SIZE �޽����� ���� �������� ũ�⸦ �����Ѵ�.
			::SendMessage(pWebView->GetSafeHwnd(), WM_SIZE, NULL, lParam);

			// �� ���������� init �Ѵ�.
			CString strFontSize;
			strFontSize.Format(_T("%dpt"), m_pSetting->GetFontSize());
			pWebView->InitPage(strFontSize, m_pSetting->GetFontFamily(), m_pSetting->GetBgColor(), m_pSetting->GetFontColor());
		}
	}

	return TRUE;
}

/**
 * @brief	fixed layout �������� �ܸ麸�� Layout �� �������Ѵ�.
 * @return	true(success)/false(fail)
 */
BOOL CEPubBook::ReLayoutFixedSingle()
{
	if(!IsFixed()) return FALSE;
	if(!m_pViewManager) return FALSE;

	// get epub view's size
	CRect clientRect;
	GetClientRect(&clientRect);

	DWORD lParam = 0L;
	lParam = ((WORD)clientRect.Height()<<16) | (WORD)clientRect.Width();

	for(UINT i=0; i<m_spineList.size(); i++)
	{
		IN3WebkitView	*pView = (IN3WebkitView *)m_pViewManager->GetView(i);
		if(pView)
		{
			// ���� ��ġ�� �ܸ麸�� ��ġ�� ����
			pView->MoveWindow(0, 0, clientRect.Width(), clientRect.Height());
			//			
			// �信 WM_SIZE �޽����� ���� �������� ũ�⸦ �����Ѵ�.
			DWORD lParam = 0L;
			lParam = ((WORD)clientRect.Height()<<16) | (WORD)clientRect.Width();
			::SendMessage(pView->GetSafeHwnd(), WM_SIZE, NULL, lParam);
			
			// �並 ��� �Ⱥ��̰� ó��
			pView->SetDlgCtrlID(AFX_IDW_PANE_FIRST + i + 1);
			pView->ShowWindow(SW_HIDE);
			pView->EnableWindow(FALSE);
		}
	}

	// ���� ������ �ϴ� �並 �ٽ� ���̵��� ����
	IN3WebkitView *pActiveView = (IN3WebkitView *)m_pViewManager->GetCurrentView();
	if(pActiveView)
	{
		pActiveView->EnableWindow(TRUE);
		pActiveView->SetDlgCtrlID(AFX_IDW_PANE_FIRST);
		pActiveView->BringWindowToTop();
		pActiveView->ShowWindow(SW_SHOW);
		pActiveView->SetFocus();
		pActiveView->Invalidate();
		
		this->SetActiveView(pActiveView);
		this->RecalcLayout();
	}
	return TRUE;
}

/**
 * @brief	fixed layout �������� ��麸�� Layout �� �������Ѵ�.
 * @return	true(success)/false(fail)
 */
BOOL CEPubBook::ReLayoutFixedDual()
{
	if(!IsFixed()) return FALSE;
	if(!m_pViewManager) return FALSE;
	
	// get epub view's size
	CRect clientRect;
	GetClientRect(&clientRect);

	int x = 0, y = 0;
	int width = clientRect.Width() / 2;
	int height = clientRect.Height();
	//
	// ���� �������� ���̴� �䰡 ���ʿ� ǥ�õǴ� ���, ���� ũ�⸦ ������ ������ ���� �ܸ麸��ó�� ��ü ȭ���� ����Ѵ�.
	// �ٸ� �� ó�� ���ݸ��� ����ϸ�, ������ ȭ�鿡 �ǵ����� ���� �ٸ� ����� ���̱� ����.
	int lastLeftPage = ((int)(m_spineList.size()) -1)%2 == 0? ((int)(m_spineList.size())-1) : -1;
	//
	// ����, �����ʿ� �並 ��ġ�Ѵ�. ��, ���� ������ �䰡 ���� ȭ�鿡 �� ���� ������ ó���Ѵ�.
	int max = lastLeftPage >=0 ? (int)(m_spineList.size()) -1 : (int)(m_spineList.size());
	for(int i = 0; i<max ; i++)
	{
		IN3WebkitView	*pView = (IN3WebkitView *)m_pViewManager->GetView(i);
		if(!pView) continue;

		DWORD lParam = 0L;
		lParam = ((WORD)clientRect.Height()<<16) | (WORD)(clientRect.Width() / 2);
		::SendMessage(pView->GetSafeHwnd(), WM_SIZE, NULL, lParam);

		// ���ʿ� ���� ȭ���� �������� x��ǥ�� 0, �����ʿ� ȭ���� �� ������� ���� �����ϵ��� �����Ѵ�.
		if(i%2 == 0) x = 0;
		else x = (clientRect.Width() / 2);

		pView->MoveWindow(x, y, width, height);
	}

	if(lastLeftPage > 0)
	{
		IN3WebkitView	*pLastView = (IN3WebkitView *)m_pViewManager->GetView(lastLeftPage);
		pLastView->MoveWindow(0, 0, clientRect.Width(), clientRect.Height());

		DWORD lParam = 0L;
		lParam = ((WORD)clientRect.Height()<<16) | (WORD)clientRect.Width();
		//
		::SendMessage(pLastView->GetSafeHwnd(), WM_SIZE, NULL, lParam);
	}

	// ���� active �Ǿ� �������� ȭ��� �Բ� ������ �ϴ� �並 ȭ�鿡 ���̵��� ó���Ѵ�.
	int nChapter = m_pViewManager->GetCurrChapter();
	if(nChapter >= 0)
	{
		IN3WebkitView *pOther;
		if(nChapter%2 == 0) pOther = (IN3WebkitView *)m_pViewManager->GetView(nChapter + 1);
		else pOther = (IN3WebkitView *)m_pViewManager->GetView(nChapter - 1);
		if(pOther){
			pOther->EnableWindow(TRUE);
			pOther->SetDlgCtrlID(AFX_IDW_PANE_FIRST);
			pOther->BringWindowToTop();
			pOther->ShowWindow(SW_SHOW);
			pOther->Invalidate();
			pOther->InvalidateInfo();
			//pOther->EnableWindow(FALSE);
		}
	}

	IN3WebkitView *pActiveView = (IN3WebkitView *)this->GetActiveView();
	if(pActiveView)
	{
		pActiveView->EnableWindow(TRUE);
		pActiveView->SetDlgCtrlID(AFX_IDW_PANE_FIRST);
		pActiveView->BringWindowToTop();
		pActiveView->ShowWindow(SW_SHOW);
		pActiveView->SetFocus();
		pActiveView->Invalidate();

		this->SetActiveView(pActiveView);
	}
	return TRUE;
}

/**
 * @brief �������κ��� ���޹��� annotation event ���� parameter �� �ؼ��Ͽ� annotation instance�� �����.
 * @param	[in] param			: �������� ���� ���� ������.(�Ľ��ؾ� �� ������)
 *			[out] annotation	: �Ľ��Ͽ� ���� annotation ������ ������
 * @return TRUE(success) / FALSE(fail)
 */
BOOL CEPubBook::ParseAnntationParam(CString param, void *pAnnotation)
{
	if(param.IsEmpty()) return FALSE;
	//
	BOOL bRes = TRUE;
	JSONValue* value = JSON::Parse(param);
	if(value == NULL)
	{
		TRACEMSG_ERR(_T("[CEPubBook::ParseAnntationParam] recived data can't parsing!!"));
		return FALSE;
	}

	if(value->IsObject() == false)
	{
		TRACEMSG_ERR(_T("[CEPubBook::ParseAnntationParam] The root element is not an object!!"));
		return FALSE;
	}

	JSONObject root;
	root = value->AsObject();
	JSONObject::iterator iter = root.find(_T("RESULT_CODE"));
	if(iter != root.end() && root[_T("RESULT_CODE")]->IsString()){
		CString result = root[_T("RESUOT_CODE")]->Stringify().c_str();
		result.Replace(_T("\\"), _T(""));
		if(result != _T("0"))
		{
			// Error
			/*
			iter = root.find(_T("ERROR_MSG"));
			if(iter!= root.end() && root[_T("ERROR_MSG")]->IsString()){
				CString errMsg = root[_T("ERROR_MSG")]->Stringify().c_str();
				errMsg.Replace(L"\"",L"");
				// @ note
				// display errMsg? 
			}
			*/
			return FALSE;
		}
	}
	// common feature
	IN3Annotation *pAnno = (IN3Annotation *)pAnnotation;	
	//
	// marker id
	iter = root.find(_T("MARKER_ID"));
	if(iter!= root.end() && root[_T("MARKER_ID")]->IsString()){
		CString markerID = root[_T("MARKER_ID")]->Stringify().c_str();
		markerID.Replace(L"\"",L"");
		pAnno->SetMarkerID(markerID);
	}
	// fileName
	iter = root.find(_T("FILE_NAME"));
	if(iter!= root.end() && root[_T("FILE_NAME")]->IsString()){
		CString fileName = root[_T("FILE_NAME")]->Stringify().c_str();
		fileName.Replace(L"\"",L"");
		pAnno->SetFileName(fileName);
	}
	// selection text
	iter = root.find(_T("SELECTION_TEXT"));
	if(iter!= root.end() && root[_T("SELECTION_TEXT")]->IsString()){
		CString selection = root[_T("SELECTION_TEXT")]->Stringify().c_str();
		selection.Replace(L"\"",L"");
		pAnno->SetSelection(selection);
	}
	// background color
	iter = root.find(_T("BG_COLOR"));
	if(iter!= root.end() && root[_T("BG_COLOR")]->IsString()){
		CString bgColor = root[_T("BG_COLOR")]->Stringify().c_str();
		bgColor.Replace(L"\"",L"");
		pAnno->SetBGColor(bgColor);
	}
	// underline color
	iter = root.find(_T("UL_COLOR"));
	if(iter!= root.end() && root[_T("UL_COLOR")]->IsString()){
		CString ulColor = root[_T("UL_COLOR")]->Stringify().c_str();
		ulColor.Replace(L"\"",L"");
		pAnno->SetULColor(ulColor);
	}
	// start index
	iter = root.find(_T("START_INDEX"));
	if(iter!= root.end() && root[_T("START_INDEX")]->IsNumber()){
		int startIndex = int(root[_T("START_INDEX")]->AsNumber());
		pAnno->SetStart(startIndex);
	}
	// end index
	iter = root.find(_T("END_INDEX"));
	if(iter!= root.end() && root[_T("END_INDEX")]->IsNumber()){
		int endIndex = int(root[_T("END_INDEX")]->AsNumber());
		pAnno->SetEnd(endIndex);
	}
	// has memo
	iter = root.find(_T("HAS_MEMO"));
	if(iter!= root.end() && root[_T("HAS_MEMO")]->IsNumber()){
		bool hasMemo = (root[_T("HAS_MEMO")]->AsNumber() > 0.0);
		pAnno->SetMemo(hasMemo);
	}
	// rect
	int left = 0, top = 0, right = 0, bottom = 0;
	iter = root.find(_T("SELECTION_LEFT"));
	if(iter != root.end() && root[_T("SELECTION_LEFT")]->IsNumber()){
		left = int(root[_T("SELECTION_LEFT")]->AsNumber());
	}
	iter = root.find(_T("SELECTION_TOP"));
	if(iter != root.end() && root[_T("SELECTION_TOP")]->IsNumber()){
		top = int(root[_T("SELECTION_TOP")]->AsNumber());
	}
	iter = root.find(_T("SELECTION_RIGHT"));
	if(iter != root.end() && root[_T("SELECTION_RIGHT")]->IsNumber()){
		right = int(root[_T("SELECTION_RIGHT")]->AsNumber());
	}
	iter = root.find(_T("SELECTION_BOTTOM"));
	if(iter != root.end() && root[_T("SELECTION_BOTTOM")]->IsNumber()){
		bottom = int(root[_T("SELECTION_BOTTOM")]->AsNumber());
	}
	pAnno->SetAnnotationRect(left, top, right, bottom);
	//
	if(value)
		delete value;
	return bRes;
}

/**
 * @brief �������κ��� ���޹��� bookmark event ���� parameter �� �ؼ��Ͽ� bookmark instance�� �����.
 * @param	[in] param		: �������� ���� ���� ������.(�Ľ��ؾ� �� ������)
 *			[out] pBookmark	: �Ľ��Ͽ� ���� bookmark ������ ������
 * @return TRUE(success) / FALSE(fail)
 */
BOOL CEPubBook::ParseBookmarkParam(CString param, void *pBookmark)
{
	if(param.IsEmpty()) return FALSE;
	//
	BOOL bRes = TRUE;
	JSONValue* value = JSON::Parse(param);
	if(value == NULL)
	{
		TRACEMSG_ERR(_T("[CEPubBook::ParseBookmarkParam] recived data can't parsing!!"));
		return FALSE;
	}

	if(value->IsObject() == false)
	{
		TRACEMSG_ERR(_T("[CEPubBook::ParseBookmarkParam] The root element is not an object!!"));
		return FALSE;
	}

	JSONObject root;
	root = value->AsObject();
	JSONObject::iterator iter = root.find(_T("RESULT_CODE"));
	if(iter != root.end() && root[_T("RESULT_CODE")]->IsString()){
		CString result = root[_T("RESUOT_CODE")]->Stringify().c_str();
		result.Replace(_T("\\"), _T(""));
		if(result != _T("0"))
		{
			return FALSE;
		}
	}
	// common feature
	IN3Bookmark *pMark = (IN3Bookmark *)pBookmark;	
	//
	// marker id
	iter = root.find(_T("MARKER_ID"));
	if(iter!= root.end() && root[_T("MARKER_ID")]->IsString()){
		CString markerID = root[_T("MARKER_ID")]->Stringify().c_str();
		markerID.Replace(L"\"",L"");
		pMark->SetMarkerID(markerID);
	}
	// file name
	iter = root.find(_T("FILE_NAME"));
	if(iter!= root.end() && root[_T("FILE_NAME")]->IsString()){
		CString fileName = root[_T("FILE_NAME")]->Stringify().c_str();
		fileName.Replace(L"\"",L"");
		pMark->SetFileName(fileName);
	}
	// marker index
	iter = root.find(_T("MARKER_INDEX"));
	if(iter!= root.end() && root[_T("MARKER_INDEX")]->IsNumber()){
		int makerIndex = int(root[_T("MARKER_INDEX")]->AsNumber());
		pMark->SetMarkerIndex(makerIndex);
	}

	if(value)
		delete value;
	return TRUE;
}
