// EPubBook.cpp : 구현 파일입니다.
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
	// 검색 결과 리스트가 있다면 삭제한다.
	if(m_pResultList)
	{
		if(m_pResultList->size() > 0)
			m_pResultList->clear();
		delete m_pResultList;
	}

	// 목차 정보 삭제
	m_tocTree.Release();

	// spine 리스트 삭제
	if (m_spineList.size() > 0)
		m_spineList.clear();

	// 뷰 매니져 삭제
	if(m_pViewManager)
		delete m_pViewManager;

	// 책정보도 삭제한다. unzip 된 폴더도 같이 삭제
	if(m_pBookInfo)
	{
	#if SUPPORT_DELETE_UNZIPDIR
		CString unzipPath = m_pBookInfo->GetUnzipRootPath();
		if(!unzipPath.IsEmpty())
			DeleteFolderAndFiles(m_pBookInfo->GetUnzipRootPath());
	#endif /*SUPPORT_DELETE_UNZIPDIR*/
		delete m_pBookInfo;
	}
	
	// 뷰잉 관련 설정 내용도 삭제한다.
	if(m_pSetting)
		delete m_pSetting;
	//	
	TRACEMSG(_T("CEPubBook::~CEPubBook() END"));
}


BEGIN_MESSAGE_MAP(CEPubBook, CFrameWnd)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CEPubBook 메시지 처리기입니다.

/********************************************************************************
 * UI interface
 ********************************************************************************/
/**
 * @brief	epub 파일을 연다.
 * @remarks	epub 파일 unzip 된 경로를 설정하고, opf 파일을 찾아 파싱한다.
 * @param lpszPathName	: epub 파일이 unzip 된 경로(절대경로)
 * @return	success(TRUE)/fail(FALSE)
 */
BOOL CEPubBook::Open(LPCTSTR lpszPathName)
{
	if(m_bOpened) return TRUE;
	//
	TRACEMSG(_T("CEPubBook::Open() START"));
	//
	// 책 open 플래그 설정
	m_bOpened = TRUE;
	//
	// 책 정보 변수를 생성한다.
	m_pBookInfo = new CBookInfo;
	if(!m_pBookInfo)
	{
		TRACEMSG_ERR(_T("CEPubBook::Open() :: can't create CBookInfo!!"));
		return FALSE;
	}
	//
	// epub 파일이 unzip 된 경로 설정
	m_pBookInfo->SetUnzipRootPath(lpszPathName);
	//
	// STEP 1) OPF 파일 및 파일이 있는 경로를 "META-INF\\container.xml 파일을 파싱하여 가져온다.
	// "META-INF\\container.xml" 파일은 고정된 이름으로 변경되지 아니한다.
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
	// thumbnail path 를 엔진에서 제공하는 경우, unzip 된 epub 경로 안에 thumbnail 폴더를 만들어 해당 경로를 반환한다.
	// 그러나 엔진에서 섬네일 경로를 지정하여 제공할 필요가 없으므로 해당 함수는 미지원하도록 처리함.
		// optional : if viewer support thumbnail slide bar, than engine save thumbnail for page in this path
		CString thumbnailPath(m_pBookInfo->GetUnzipRootPath());
		thumbnailPath.Append(opfFolderName);
		thumbnailPath.Append(_T("\\thumbnail\\"));
		m_pBookInfo->SetThumbnailPath(thumbnailPath);
	#endif /*SUPPORT_THUMBNAIL_PATH*/
	}
	//
	// STEP 2) Fixed 컨텐츠 인지 확인한다.
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
 * @brief	책에 필요한 뷰들을 생성한다.
 * @param	만들 뷰의 런타임 클래스. NULL 인 경우 디폴트 뷰로 생성한다. (런타임 클래스 지정 시, 뷰를 제거할 때 문제가 있어 현재 지정하지는 않고 있다.)
 * @remarks 엔진에서 뷰 pool 매니저를 제공하지 않을 경우, 이 함수를 사용해서 책 객체에서 필요한 뷰를 직접 생성한다.
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
 * @brief		epub 삭제 시 발생하는 이벤트. 
 * @remarks	tab control 에서 임의의 tab 을 삭제하는 경우 발생한다. 
 * 			이벤트가 발생하여 OnClose() 함수 호출 시에 실제 Epub 을 삭제하지는 않는다.
 * 			UI 단으로 epub 삭제를 위해 WM_CLOSE_EPUB 이젠트를 발생시킨다.
 * 			UI 단에서 WebkitEngine API 인 Close() 함수를 통해 실제 epub 을 삭제해야 한다.
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
 * @brief	UI단에서 각 epub 별로 고유하게 관리하고 있는 id값
 * @remarks	tab control 에 Epub 을 붙이는 경우 tab 의 id를 설정한다.
 * @param	id	: UI 단에 서 관리하는 아이디
 */
void CEPubBook::SetUIID(int id)
{
	m_uiID = id;
}

/**
 * @brief	UI단에서 각 epub 별로 고유하게 관리하고 있는 id값을 가져온다
 * @remarks	tab control 에 Epub 을 붙이는 경우 tab 의 id값과 같다.
 * @return	UI 단에 서 관리하는 아이디
 */
const int CEPubBook::GetUIID(void) const
{
	return m_uiID;
}

/**
 * @brief	epub 을 Reload 한다.
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
 * @brief	epub 의 레이아웃을 재정렬 한다.
 */
void CEPubBook::ReLayout()
{
	if(!m_pSetting){
		TRACEMSG_ERR(_T("m_pSetting is NULL"));
		return;
	}
	//
	m_bInitialized = TRUE;

	// fixed layout 컨텐츠와 reflow 컨텐츠의 레이아웃은 다르게 적용한다.
	if(IsFixed())
	{
		// fixed layout 인 경우에도, 양면보기와 단면 보기에 따라 레이아웃을 다르게 적용한다.
		if(m_pSetting->GetPageDisplayMode() == 1)
			ReLayoutFixedSingle();
		else
			ReLayoutFixedDual();
	}
	else
		ReLayoutReflow();
}

/**
 * @brief	컨텍스트 메뉴를 띄우도록 UI 단으로 이벤트를 발생시킨다.
 * @param	pt	: 마우스 클릭 위치
 */
void CEPubBook::ShowContextMenu(POINT pt)
{
	CWnd* pParentWnd = GetParent();
	if(pParentWnd)
		pParentWnd->SendMessage(WM_CONTEXTMENU_EPUB, (WPARAM)this, (LPARAM)&pt);
}

/**
 * @brief	annotation 메뉴를 띄우도록 UI 단으로 이벤트를 발생시킨다.
 * @param	pt				: 마우스 클릭 위치
 * @param	selectedText	: 사용자가 선택한 텍스트
 */
void CEPubBook::ShowAnnotationMenu(POINT pt, CString selectedText)
{
	// 마우스 위치 및 선택한 텍스트 정보를 기준으로 UI 단으로 넘겨주기 위한 데이터를 생성한다.
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
 * @brief	web popup 을 띄우도록 UI 단으로 이벤트를 발생시킨다.
 * @remarks	사용자의 이벤트에 의해 엔진으로 부터 호출되어 지는 함수이다.
 * @param	param	: 엔진으로 부터 전달받은 연동할 web popup 데이터
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
 * @brief	fixed 컨텐츠인 경우 뷰를 확대/축소한다.
 * @param	zoomin	: 확대/축소에 대한 플래그. TRUE: 확대, FALSE: 축소
 */
void CEPubBook::ViewZoom(bool zoomin)
{
	if(m_pViewManager == NULL) return;
	if(!IsFixed()) return;

	// 현재 보여지는 뷰를 확대/축소 한다.
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
 * @brief	fixed 컨텐츠의 뷰를 현재 윈도우의 크기에 맞춰 보여준다.
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
 * @brief	임의의 화면을 임의의 크기로 캡쳐한다.
 * @remarks fixed layout() 에서만 사용 가능
 * @param	[in] page		: 캡쳐할 페이지
 * @param	[in] width		: 캡쳐 이미지의 가로 픽셀 값
 * @param	[in] height		: 캡쳐 이미지의 세로 픽셀 값
 * @param	[out] bitmap	: 캡쳐 이미지
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
			// 화면 캡쳐는 뷰의 로드가 완료된 이후부터 가능하다.
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
 * @brief	현재 보여지는 페이지 값을 반환한다.
 * @return	현재 보여지는 페이지 값
 */
int CEPubBook::GetCurrentPage()
{
	if(m_pViewManager)
	{
		int currPage = 0;
		int currChapter = m_pViewManager->GetCurrChapter();
		// 처음부터 현재 보여지는 챕터 전까지의 페이지를 더함
		for(int i=0; i< currChapter; i++)
		{
			IN3WebkitView* pView = (IN3WebkitView *)m_pViewManager->GetView(i);
			if(pView)
				currPage += pView->GetTotalPage();
		}
		
		// 현재 보여지는 쳅터에서 몇번째 페이지를 보고있는지 확인하여 페이지 값을 더함
		IN3WebkitView* pView = (IN3WebkitView *)m_pViewManager->GetView(currChapter);
		if(pView)
			currPage += pView->GetCurrPage();

		return currPage;
	}

	return 0;
}

/**
 * @brief	책의 총 페이지 수를 반환한다.
 * @return	책의 총 페이지 수
 */
int CEPubBook::GetTotalPage()
{
	if(m_pViewManager)
		return m_pViewManager->GetTotalPage();

	return 0;
}

/**
 * @brief	현재보고 있는 챕터의 인덱스 값을 반환한다. (spine index 값과 일치함)
 * @return	현재 보여지고 있는 spine 파일의 인덱스 값.	
 */
int CEPubBook::GetCurrentChapter()
{
	if(m_pViewManager)
		return m_pViewManager->GetCurrChapter();

	return -1;
}

/**
 * @biref	본문에 사용자가 임의 선택한 영역을 해제하는 함수
 * @return	웹킷 extension 으로 선택 영역 해제를 요청한 경우 = TRUE, 그외= FALSE 
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
 * @biref	임의의 본문에 사용자가 임의 선택한 영역을 해제하는 함수
 * @param	chater	: 선택 영역을 해제할 챕터 인덱스(spine 파일의 인덱스)
 * @return	웹킷 extension 으로 선택 영역 해제를 요청한 경우 = TRUE, 그외= FALSE 
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
 * @brief	본문 아래영역에 현재 페이지 값을 표시할지 여부를 설정한다.
 * @param	bDisplay	: true(본문 밑에 페이지 번호 표시) / false(페이지 번호 표시 안함)
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
 * @brief	페이지 모드를 설정한다. (단면/양면)
 * @param	mode	: 1 = 단면, 2 = 양면
 */
void CEPubBook::SetPageDisplayMode(int mode)
{
	if(!m_pSetting){
		TRACEMSG_ERR(_T("m_pSetting is NULL"));
		return ;
	}
	//
	// 페이지 모드를 설정한다.
	m_pSetting->SetPageDisplayMode(mode);
	// 변경된 모드에 맞게 화면 재구성
	ReLayout();			
	// 변경된 페이지에 북마크가 삽입되어 있는지 확인
	QueryBookmarkStatus();					
}

/**
 * @brief	폰트 사이즈를 설정한다.
 * @param	size	: 변경할 폰트 사이즈
 * @remarks 설정되는 폰트 사이즈는 엔진 내부에서 가지고 있다가 ReLayout() 시 변경된 폰트 사이즈를 적용하도록 한다.
 *			따라서 아래 함수는 단순히 엔진 내부에 폰트 사이즈를 설정하는 기능만 한다.
 *			즉 변경 사항을 적용하기 위해선 사용자가 원하는 시점에 ReLayout() 함수를 호출하여 줘야 한다.
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
 * @brief	COLUMN GAP을 설정한다.
 * @param	margin	: 변경할 COLUMN GAP
 * @remarks 설정되는 COLUMN GAP은 엔진 내부에서 가지고 있다가 ReLayout() 시 적용하도록 한다.
 *			따라서 아래 함수는 단순히 엔진 내부에 COLUMN GAP을 설정하는 기능만 한다.
 *			즉 변경 사항을 적용하기 위해선 사용자가 원하는 시점에 ReLayout() 함수를 호출하여 줘야 한다.
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
 * @brief	line height을 설정한다.
 * @param	height	: 변경할 line height
 * @remarks 설정되는 line height은 엔진 내부에서 가지고 있다가 ReLayout() 시 적용하도록 한다.
 *			따라서 아래 함수는 단순히 엔진 내부에 line height을 설정하는 기능만 한다.
 *			즉 변경 사항을 적용하기 위해선 사용자가 원하는 시점에 ReLayout() 함수를 호출하여 줘야 한다.
 *			(현재 사용되고 있지 않음)
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
 * @brief	폰트를 설정한다.
 * @param	family	: 변경할 폰트
 * @remarks 설정되는 폰트는 엔진 내부에서 가지고 있다가 ReLayout() 시 변경된 폰트를 적용하도록 한다.
 *			따라서 아래 함수는 단순히 엔진 내부에 폰트를 설정하는 기능만 한다.
 *			즉 변경 사항을 적용하기 위해선 사용자가 원하는 시점에 ReLayout() 함수를 호출하여 줘야 한다.
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
 * @brief	폰트 색상을 설정한다.
 * @param	color	: 변경할 폰트 색상
 * @remarks 설정되는 폰트 색상은 엔진 내부에서 가지고 있다가 ReLayout() 시 변경된 폰트 색상을 적용하도록 한다.
 *			따라서 아래 함수는 단순히 엔진 내부에 폰트 색상을 설정하는 기능만 한다.
 *			즉 변경 사항을 적용하기 위해선 사용자가 원하는 시점에 ReLayout() 함수를 호출하여 줘야 한다.
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
 * @brief	배경색을 설정한다.
 * @param	size	: 변경할 배경색
 * @remarks 설정되는 배경색은 엔진 내부에서 가지고 있다가 ReLayout() 시 변경된 배경색을 적용하도록 한다.
 *			따라서 아래 함수는 단순히 엔진 내부에 배경색을 설정하는 기능만 한다.
 *			즉 변경 사항을 적용하기 위해선 사용자가 원하는 시점에 ReLayout() 함수를 호출하여 줘야 한다.
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
 * @biref	폰트색, 배경색을 적용한다.
 * @param	foregroundClr	: 폰트색
 * @param	backgroundClr	: 배경색
 * @remarks	폰트색, 배경색을 변경하는 것은 별도로 레이아웃을 재정리 할 필요는 없으므로 함수 호출 즉시 바로 반영한다.
 */
void CEPubBook::SetColors(CString foregroundClr, CString backgroundClr)
{
	if(!m_pSetting){
		TRACEMSG_ERR(_T("m_pSetting is NULL"));
		return;
	}

	// 폰트색, 배경색 설정
	m_pSetting->SetFontColor(foregroundClr);
	m_pSetting->SetBgColor(backgroundClr);


	// 책에 포함되어 있는 모든 뷰에 폰트색, 배경색 적용
	for(unsigned int i=0; i< m_spineList.size(); i++)
	{
		IN3WebkitView *pView = (IN3WebkitView *)m_pViewManager->GetView(i);
		if(pView)
		{
			// 뷰의 로딩이 완료된 이후에 적용 가능하다.
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
 * @brief	본문 아래 영역에 페이지 번호를 표시하도록 설정되어 있는지 확인한다.
 * @return	true(본문 밑에 페이지 번호 표시) / false(페이지 번호 표시 안함)
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
 * @brief	현재 페이지 모드를 반환한다.
 * @return	페이지 모드 (1=단면, 2=양면)
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
 * @brief	사용자가 설정한 폰트 사이즈를 반환한다.
 * @return	사용자가 설정한 폰트 사이즈
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
 * @brief	사용자가 설정한 column gap 을 반환한다.
 * @return	사용자가 설정한 column gap
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
 * @brief	사용자가 설정한 line height 을 반환한다.
 * @return	사용자가 설정한 line height
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
 * @brief	사용자가 설정한 폰트명 을 반환한다.
 * @return	사용자가 설정한 폰트명
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
 * @brief	사용자가 설정한 폰트색을 반환한다.
 * @return	사용자가 설정한 폰트색
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
 * @brief	사용자가 설정한 배경색을 반환한다.
 * @return	사용자가 설정한 배경색
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
 * @brief	epub 책의 원본 절대 경로를 설정한다.
 * @param	lpszOriginalPath	: epub 책의 원본 절대 경로
 */
void CEPubBook::SetOriginalEpubPath(LPCTSTR lpszOriginalPath)
{
	if(m_pBookInfo)
		m_pBookInfo->SetSrcRootPath(lpszOriginalPath);
}

/**
 * @brief	fixed layout 의 width, height 사이즈를 설정한다.
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
 * @brief	epub 책의 뷰들을 관리하는 매니터를 설정한다.
 * @param	pMgr	: 뷰를 관리하는 매니저의 포인터
 */
void CEPubBook::SetViewManager(CViewManager *pMgr)
{
	m_pViewManager = pMgr;
}

/**
 * @brief	Epub 이 open 되어 있는지 확인하는 함수
 * @return	already opend (TRUE)/ Not yet(FALSE)
 */
const BOOL CEPubBook::IsOpened(void) const
{
	return m_bOpened;
}

/**
 * @ 활성화 되어 있는 챕터(spine file) 의 정보를 업데이트 한다.
 * @param	chapter	: 활성화 되는 챕터 값
 */
void CEPubBook::SetCurrChapter(int chapter)
{
	if(m_pViewManager)
		m_pViewManager->SetCurrChapter(chapter);
}

/**
 * @brief	책이 정상적으로 Load 되었는지 학인하는 함수
 * @return	TRUE(로드 됨)/FALSE(로드 안됨)
 */
const BOOL CEPubBook::IsLoaded(void) const
{
	return m_bInitialized;
}

/**
 * @brief	Epub 이 Fixed layout 인지 확인하는 함수
 * @return	FixedLayout(TRUE)/ReflowLayout(FALSE)
 */
const BOOL CEPubBook::IsFixed(void) const
{
	if(m_pBookInfo)
		return m_pBookInfo->IsFixed();

	return FALSE;
}

/**
 * @brief	책의 uuid 를 가져온다.
 * @return	책의 uuid
 */
const CString CEPubBook::GetUUIDString(void) const
{
	if(m_pBookInfo)
		return m_pBookInfo->GetUUIDString();

	return CString(_T(""));
}

/**
 * @brief	책의 버전을 반환한다.
 * @return	책의 버전 3 or 2
 */
const CString CEPubBook::GetVersion(void) const
{
	CString strVer(_T(""));
	if(m_pBookInfo)
		strVer.Format(_T("%.0f"), m_pBookInfo->GetVersion());

	return strVer;
}

/**
 * @brief	책의 표지를 가져온다.
 * @return	책의 타이틀
 */
const CString CEPubBook::GetTitle(void) const
{
	if(m_pBookInfo)
		return m_pBookInfo->GetTitle();

	return CString(_T(""));
}

/**
 * @brief	저자명을 반환한다.
 * @return	책의 저자명
 */
const CString CEPubBook::GetCreator(void) const
{	
	if(m_pBookInfo)
		return m_pBookInfo->GetCreator();

	return CString(_T(""));
}

/**
 * @brief	책의 출판사를 반환한다.
 * @return	출판사명
 */
const CString CEPubBook::GetPublisher(void) const
{
	if(m_pBookInfo)
		return m_pBookInfo->GetPublisher();

	return CString(_T(""));
}

/**
 * @brief	책의 설명을 반환한다.
 * @return	설명
 */
const CString CEPubBook::GetDesc(void) const
{
	if(m_pBookInfo)
		return m_pBookInfo->GetDesc();

	return CString(_T(""));
}

/**
 * @brief	책의 커버 이미지 경로를 반환한다.
 * @return	커버 이미지 경로
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
 * @brief	fixed layout 컨텐츠의 width 를 반환한다.
 * @return	fixed 컨텐츠의 width 값
 */
const int CEPubBook::GetFixedWidth() const
{
	return m_pBookInfo->GetFixedWidth();
}

/**
 * @brief	fixed layout 컨텐츠의 height 를 반환한다.
 * @return	fixed 컨텐츠의 height 값
 */
const int CEPubBook::GetFixedHeight() const
{
	return m_pBookInfo->GetFixedHeight();
}

/**
 * @brief	책의 spine 수를 반환한다.
 * @return	spine 개수
 */
const int CEPubBook::GetSpineCount() const
{
	return m_spineList.size();
}

/**
 * @brief	임의의 spine 파일의 url을 반환한다.
 * @param	idx	: url을 찾고자 하는 spine 파일의 인덱스
 * @return	spine 파일의 url
 */
const CString CEPubBook::GetSpineURL(int idx) const
{
	return m_spineList.at(idx);
}

/**
 * @brief	epub 책의 원본 절대 경로를 반환한다.
 * @return	epub 책의 원본 절대 경로
 */
const CString CEPubBook::GetOriginalEpubPath(void) const
{
	if(m_pBookInfo)
		return m_pBookInfo->GetSrcRootPath();
	
	return CString(_T(""));
}

/**
 * @brief	Epub 파일의 content.opf 의 절대경로를 반환한다.
 * @return	content.opf 파일의 절대 경로
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
 * @brief	epub 책의 unzip 된 경로에 있는 패키지 폴더(OEBPS)의 절대 경로를 반환한다.
 * @return	패키지 폴더(OEBPS) 의 절대 경로
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
	// thumbnail path 를 엔진에서 제공하는 경우, unzip 된 epub 경로 안에 thumbnail 폴더를 만들어 해당 경로를 반환한다.
	// 그러나 엔진에서 섬네일 경로를 지정하여 제공할 필요가 없으므로 해당 함수는 미지원하도록 처리함.
/**
 * @breif	썸내일 경로
 * @remarks epub 에 임의 경로를 설정하여 반환함
 * @return	썸네일 경로
 */
const CString CEPubBook::GetThumbnailPath(void)
{
	if(!m_pBookInfo) return CString(_T(""));

	CString thumbnailPath(m_pBookInfo->GetThumbnailPath()); 

	return thumbnailPath;
}
#endif;

/**
 * @brief	책의 목차를 반환한다.(트리구조)
 * @remarks	책의 목차의 루트 데이터의 주소값을 반환한다.
 * @return	책의 목차
 */
IN3NavPointNode*	CEPubBook::GetTableOfContent(void)
{
	return &m_tocTree;
}

/**
 * @brief	책에서 추출한 이미지의 리스트를 반환한다.
 * @remarks	책의 모든 이미지를 추출하는 것이 아니라, 인식 가능한 이미지만을 추출한다.
 *			extension 에 이미지 추출을 조회한다.
 * @return  true(success)/false(fail)
 */
BOOL CEPubBook::QueryImageList(void)
{
	// @ note
	// 구현 요
	return TRUE;
}

/**
 * @brief	책에서 추출한 표의 리스트를 반환한다.
 * @remarks	책의 모든 표를 추출하는 것이 아니라, 인식 가능한 표만을 추출한다.
 *			extension 에 표 추출을 조회한다.
 * @return  true(success)/false(fail)
 */
BOOL CEPubBook::QueryTableList(void)
{
	// @ note
	// 구현 요
	return TRUE;
}




/********************************************************************************
 * page navigation
 ********************************************************************************/
/**
 * @brief	책의 전체 페이지를 계선하는 함수
 * @param	bWaitCathering	: 전체 페이지를 계산할 때, 책의 모든 spine 이 init 되길 기다릴지 판단하는 값. (true : 모두 init 되길 기다림, false : 바로 총 페이지 계산)
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
 * @brief	임의의 챕터로 이동하기
 * 파라미터	chapter			이동할 쳅터 (spine index)
 *			bDirectRight	챕터 이동 방향. TRUE(Right) /FALSE(Left)
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
	// 이동하고자 하는 챕터가 현지 보여지는 챕터라면 별도의 처리 없이 바로 리턴.
	if(chapter == m_pViewManager->GetCurrChapter()){
		return;
	}
	//
	// 이동하고자 하는 챕터의 뷰를 가져온다.
	IN3WebkitView *pNext = (IN3WebkitView *)m_pViewManager->GetView(chapter);
	if(!pNext) return;
	//
	// 현재 활성화되어 있는 뷰를 가져온다.
	IN3WebkitView *pCurr = (IN3WebkitView *)this->GetActiveView();
	//
	// fixed 양면보기와 그 외 페이지의 챕터 이동을 구분하여 처리한다.
	if(IsFixed() && m_pSetting->GetPageDisplayMode() == 2)
	{
		// fixed 컨텐츠이고, 양면보기인 경우 챕터 이동
		if(ChapgeViewFixedDual(pCurr, pNext))
		{
			m_pViewManager->SetCurrChapter(chapter);
			pNext->SetFocus();
			this->SetActiveView(pNext);
		}
	}
	else
	{
		// 챕터 이동
		if(ChangeView(pCurr, pNext))
		{
			m_pViewManager->SetCurrChapter(chapter);
			// 
			// 이동하는 방향에 따라 챕터의 첫 페이지가 보일지, 마지막 페이지가 보일지 결정한다.
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
 * @brief	임의의 챕터로 이동하기
 * @param	chapter		: 이동할 쳅터 (spine index)
 *			page		: 이동할 페이지 (한 spine 파일 내에서의 페이지임. 책의 전체 페이지값은 아님)
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
	// 이동할 챕터의 뷰 가져오기
	IN3WebkitView *pNext = (IN3WebkitView *)m_pViewManager->GetView(chapter);
	if(!pNext) return;
	//
	// 현재 활성화 되어 있는 챕터의 뷰 가져오기
	IN3WebkitView *pCurr = (IN3WebkitView *)this->GetActiveView();
	//
	// fixed 양면보기와 그 외 페이지의 챕터 이동을 구분하여 처리한다.
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
 * @brief	책에 삽입된 anchor 가 있는 페이지로 이동한다.
 * 파라미터 pBook		이동할 책
 *			fileName	anchor 가 있는 파일이름
 *			anchorID	이동할 anchor id
 */
void CEPubBook::MoveToAnchor(CString fileName, CString anchorID)
{
	TRACEMSG(_T("CEPubBook::MoveToAnchor() START"));
	//
	if(!m_pViewManager){
		TRACEMSG_ERR(_T("m_pViewManager is NULL!!"));
		return;
	}

	// anchor 가 삽입되어 있는 파일의 spine 인덱스를 가져온다.
	int index = GetSpineIndex(fileName);

	// anchor 가 삽입되어 있는 뷰를 가져온다.
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
 * @brief	임의의 페이지로 이동하는 함수
 * @param	page	: 이동할 페이지(전체 페이지 기준)
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
	// 이동할 페이지가 몇번째 쳅터파일인지 확인한다. (몇번째 spine 파일인지 확인한다.)
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
 * @brief 다음 페이지로 이동한다.
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
 * @brief 이전 페이지로 이동한다.
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
 * @brief 검색 결과 리스트 중 임의의 결과가 있는 페이지로 이동한다.
 * @param	fileName	: 검색 결과가 있는 파일명
 * @param	keyword		: 검색 문자열
 * @param	seq			: 검색 결과의 시퀀스 값
 */
void CEPubBook::MoveToFindResult(CString fileName, CString keyword, int seq)
{
	if(!m_pViewManager){
		TRACEMSG_ERR(_T("m_pViewManager is NULL!!"));
		return;
	}

	// 검색 결과가 있는 파일명의 spine 인덱스를 가져온다.
	// spine 인덱스를 이용해 관련된 뷰를 가져온다.
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
 * @brief	Anchor 이동 후 웹킷 extension 으로부터 호출되는 callback 함수
 * @remarks	웹킷 extension 에서 페이지 이동까지 하므로, 별다른 페이지 이동 없이 이동된 페이지 정보만 업데이트 한다.
 * @param	page	: 이동된 페이지 값
 */
void CEPubBook::OnMoveToAnchor(int page)
{		
	IN3WebkitView *pView = (IN3WebkitView *)m_pViewManager->GetCurrentView();
	if(pView)
	{
		// Webkit Extentions Engine 에서 이동한 페이지 정보를 뷰에 설정한다.
		// 페이지 이동은 하지 않고 단순한 정보만 update
		pView->SetCurrPage(page);
	}
}

/**
 * @brief	페이지 이동 후 웹킷 extension 으로부터 호출되는 callback 함수
 * @remarks	UI단에 WM_PAGE_MOVED 메시지를 보낸다,
 */
void CEPubBook::OnPageMoved()
{
	CWnd* pParentWnd = GetParent();
	if(pParentWnd)
	{
		// 현재 이동된 페이지 값을 가져와 UI 단으로 메시지를 보낸다.
		int currPage = GetCurrentPage();
		pParentWnd->SendMessage(WM_PAGE_MOVED, (WPARAM)this, (LPARAM)currPage);
	}
}




/********************************************************************************
 * annotation
 ********************************************************************************/
/**
 * @brief	하이라이트를 추가하는 함수
 * @param	color	: 하이라이트 색상
 * @param	hasMemo	: 메모가 추가되어 있는지 여부
 * @return	웹킷 extension 으로 하이라이트 추가를 요청한 경우 = TRUE, 그외= FALSE
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
 * @brief	언더라인을 추가하는 함수
 * @param	color	: 언더라인 색상(현재는 사용되지 않으나 추후 업데이트 될 수 도 있음)
 * @param	hasMemo	: 메모가 추가되어 있는지 여부
 * @return	웹킷 extension 으로 언더라인 추가를 요청한 경우 = TRUE, 그외= FALSE
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
		// BG_COLOR 값이 "" 이면 언더라인으로 처리하고 있음.
		CString param;
		param.Format(_T("{\"BG_COLOR\":\"\", \"UL_COLOR\":\"%s\", \"HAS_MEMO\":%d}"), color, hasMemo);
		bRes =  pView->SendMessage(CMD_IN3_MARK_ANNOTATION, param);
	}
	//
	TRACEMSG(_T("CEPubBook::CreateUnderline() END"));

	return bRes;
}

/**
 * @brief	annotation(하이라이트/메모/언더라인) 을 삭제한다.
 * @param	fileName	: 삭제할 annotaton 이 있는 파일명
 * @param	markerID	: 삭제할 annotation 의 id
 * @return	웹킷 extension 으로 annotation 삭제를 요청한 경우 = TRUE, 그외= FALSE
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
 * @brief	annotation(하이라이트/메모/언더라인) 을 수정한다.
 * @param	pAnnotation	: 수정할 annotaton 정보
 * @return	웹킷 extension 으로 annotation 수정을 요청한 경우 = TRUE, 그외= FALSE
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
 * @brief	annotation(하이라이트/메모/언더라인) 정보를 일괄적용한다.
 * @param	annotations	: 일괄 적용할 annotation 리스트
 * @return	웹킷 extension 으로 annotation 일괄 적용을 요청한 경우 = TRUE, 그외= FALSE
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
	// setting 변수의 annotation 리스트 초기화
	m_pSetting->ClearAnnotations();
	//
	// 일괄 적용할 annotation 들을 실제 annotation 이 삽입되어 있는 파일을 기준으로 분리하여 setting 변수에 임시 저장한다.
	for(i; i< annotations.size(); i++)
	{
		IN3Annotation *pAnnotation = annotations[i];
		int index = GetSpineIndex(pAnnotation->GetFileName());
		if(index < 0) continue;

		m_pSetting->AddAnnotation(index, pAnnotation);
	}

	// 파일별로 분류된 annotation 을 실제 파일에 적용한다.
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

	// 임시 저장하였던 annotation 리스트를 setting 변수에서 삭제한다.
	m_pSetting->ClearAnnotations();
	return bRes;
}

/**
 * @brief	annotation 메뉴를 띄워주기 위해 현재 사용자가 선택한 영역을 조회하는 함수
 * @return	웹킷 extension 으로 사용자가 선택한 영역을 조회를 요청한 경우 = TRUE, 그외= FALSE
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
 * @brief	annotation 추가 후 웹킷 extension 으로부터 호출되는 callback 함수
 * @param	param	: extension 으로 부터 전달받은 JSON 데이터
 * @param	index	: annotation 이 추가된 페이지 (fixed layout 에서만 유효한 값을 가짐)
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
 * @brief	사용자가 annotation 을 클릭할 때, 웹킷 extension 으로 부터 호출되는 callback 함수
 * @param	param	: extension 으로 부터 전달받은 JSON 데이터
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
 * @brief	사용자가 메모 아이콘을 클릭할 때, 웹킷 extension 으로 부터 호출되는 callback 함수
 * @param	param	: extension 으로 부터 전달받은 JSON 데이터
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
 * @brief	annotation 삭제 후 웹킷 extension 으로부터 호출되는 callback 함수
 * @param	param	: extension 으로 부터 전달받은 JSON 데이터
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
 * @brief	annotation 수정 후 웹킷 extension 으로부터 호출되는 callback 함수
 * @param	param	: extension 으로 부터 전달받은 JSON 데이터
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
 * @brief	QueryAnnotationToolbarPosition() 함수에 대한 callback 함수
 * @param	param	: extension 으로 부터 전달받은 JSON 데이터
 */
void CEPubBook::OnQueryAnnotationPos(CString param)
{
	if(param.IsEmpty()) return;
	//
	BOOL bRes = TRUE;
	// 파라미터를 JOSN 으로 파싱한다.
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
	// "RESULT_CODE" 의 값이 0 이 아니면 에러로 간주한다.
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
 * @brief	현재 보이는 페이지에 북마크를 삽입한다.
 * @return	웹킷 extension 으로 북마크 추가를 요청한 경우 = TRUE, 그외= FALSE 
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
 * @brief	현재 보이는 쳅터의 북마크들을 삭제한다.
 * @return	웹킷 extension 으로 북마크 삭제를 요청한 경우 = TRUE, 그외= FALSE 
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
 * @brief	특정 북마크을 삭제한다.
 * @param	pBookmark	: 삭제할 북마크 정보
 * @return	웹킷 extension 으로 북마크 삭제를 요청한 경우 = TRUE, 그외= FALSE 
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
 * @brief	특정 북마크들을 삭제한다.
 * @param	bookmarks	: 삭제할 북마크 정보 리스트
 * @return	웹킷 extension 으로 북마크 삭제를 요청한 경우 = TRUE, 그외= FALSE 
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
	// setting 변수의 북마크 리스트를 초기화
	m_pSetting->ClearBookmarks();
	//
	// 삭제할 북마크 들을 실제 북마크가 삽입되어 있는 파일을 기준으로 분리하여 setting 변수에 임시 저장한다.
	for(i; i< bookmarks.size(); i++)
	{
		IN3Bookmark *pBookmark = bookmarks[i];
		int index = GetSpineIndex(pBookmark->GetFileName());
		if(index < 0) continue;

		m_pSetting->AddBookmark(index, pBookmark);
	}

	// 파일별로 분류된 북마크들을 삭제한다.
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
 * @brief	북마크들을 일괄적용한다.
 * @param	bookmarks	: 일괄 적용할 bookmark 리스트
 * @return	웹킷 extension 으로 북마크 일괄 적용을 요청한 경우 = TRUE, 그외= FALSE
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
	// setting 변수의 북마크 리스트를 초기화
	m_pSetting->ClearBookmarks();
	//
	// 삭제할 북마크 들을 실제 북마크가 삽입되어 있는 파일을 기준으로 분리하여 setting 변수에 임시 저장한다.
	for(i; i< bookmarks.size(); i++)
	{
		IN3Bookmark *pBookmark = bookmarks[i];
		int index = GetSpineIndex(pBookmark->GetFileName());
		if(index < 0) continue;

		m_pSetting->AddBookmark(index, pBookmark);
	}

	// 파일별로 분류된 북마크들을 적용한다.
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
 * @brief	현재 보이는 뷰에 북마크가 삽입되어 있는 지 확인한다.
 * @return	웹킷 extension 으로 북마크가 삽입되어 있는지 확인요청을 한 경우 = TRUE, 그외= FALSE 
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
 * @brief	임의의 챕터에 북마크가 삽입되어 있는 지 확인한다. (fixed 양면보기에서 사용함)
 * @return	웹킷 extension 으로 북마크가 삽입되어 있는지 확인요청을 한 경우 = TRUE, 그외= FALSE 
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
 * @brief	북마크 추가 후 웹킷 extension 으로부터 호출되는 callback 함수
 * @param	param	: extension 으로 부터 전달받은 JSON 데이터
 * @param	index	: 북마크 이 추가된 페이지 (fixed layout 에서만 유효한 값을 가짐)
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
 * @brief	북마크 삭제 후 웹킷 extension 으로부터 호출되는 callback 함수
 * @remarks	ClearBookmark() 함수에 대한 callback 함수
 * @param	param	: extension 으로 부터 전달받은 JSON 데이터
 * @see		ParseParam, ClearBookmark
 */
void CEPubBook::OnBookmarkCleared(CString param)
{
	if(param.IsEmpty()) return;
	//
	JSONArray ary;
	// 삭제된 북마크 ID 리스트를 추출한다.
	if(ParseParam(param, _T("MARKER_ID_LIST"), &ary))
	{
		std::vector<CString> *pMarkerIdList = new std::vector<CString>();
		if(!pMarkerIdList) return;

		// UI 단으로 전달할 삭제된 북마크 리스트를 생성한다.
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
 * @brief	북마크 삭제 후 웹킷 extension 으로부터 호출되는 callback 함수
 * @remarks	DeleteBookmark() 함수에 대한 callback 함수
 * @param	param	: extension 으로 부터 전달받은 JSON 데이터
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
 * @brief	북마크가 있는지 조회하면 extension 으로부터 호출되는 callback 함수
 * @remarks	QueryBookmarkStatus() 함수에 대한 callback 함수
 * @param	param	: extension 으로 부터 전달받은 JSON 데이터
 * @see		ParseParam, QueryBookmarkStatus
 */
void CEPubBook::OnEventBookmarkStatus(CString param, int fixedpage)
{
	if(param.IsEmpty()) return;

	bool bHasBookmark;
	// 파라미터를 파싱하여 북마크가 포함되어 있는지 가져온다.
	if(ParseParam(param, _T("HAS_BOOKMARK"), &bHasBookmark))
	{
		// fixed layout 양면보기이 경우는 왼쪽 뷰와 오른쪽 뷰를 모두 검사하여 양쪽 중 하나라도 북마크가 있을 경우,
		// UI 단으로 북마크가 있다고 메시지를 보내준다.
		if((fixedpage > 0) && IsFixed() && GetPageDisplayMode() == 2)
		{
			// 마지막 왼쪽 페이지인 경우는 오른쪽 페이지가 없으므로 왼쪽 페이지만 확인.
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
				// 왼쪽 뷰에 북마크가 있는지 없는지 확인 된 경우 flag 변수의 마지막 비트를 1로 설정한다.
				// 오른쪽 뷰에 북마크가 있는지 없는지 확인 된 경우 flag 변수의 마지막 바로 위 비트를 1로 설정한다.
				// 북마크가 있을 경우 flag 변수의 끝에서 세번째 비트를 1로 설정한다.
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

				// 왼쪽과 오른쪽 뷰 모두 확인이 되면 flag 의 마지막 비트와 끝에서 두번째 비트가 1로 설정된다.
				// 양쪽 뷰 모두에서 북마크가 확인이 되어, 북마크가 있다고 판단되면 UI 단으로 북마크가 있음을 알린다.
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
 * @brief	본문 검색을 한다.
 * @remarks 검색 결과를 리스트로 전달받는다.
 * @param	keyword	: 검색 문자열
 * @return	웹킷 extension 으로 본문 검색을 요청한 경우 = TRUE, 그외= FALSE 
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
 * @brief	현재 보여지는 페이지의 본문내용을 검색을 한다.
 * @remarks 검색 시 결과 화면으로 바로 이동한다.
 * @param	keyword		: 검색 문자열
 * @param	direction	: 이동 방향(true : forward, false : backward)
 * @return	웹킷 extension 으로 본문 검색을 요청한 경우 = TRUE, 그외= FALSE 
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
 * @brief	임의 페이지의 본문내용을 검색을 한다.
 * @remarks 검색 시 결과 화면으로 바로 이동한다.
 * @param	keyword		: 검색 문자열
 * @param	direction	: 이동 방향(true : forward, false : backward)
 * @param	chapter		: 검색할 챕터 파일(spine 파일)
 * @return	웹킷 extension 으로 본문 검색을 요청한 경우 = TRUE, 그외= FALSE 
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
 * @brief	본문 검색 후 webkit extension 으로 부터 호출되는 callback 함수
 * @param	bExistNext	: 다음 검색 결과가 있는지 확인 할 수 있는 플래그 (true : 다음 검색 결과 있음, false : 다음 검색 결과 없음)	
 */
void CEPubBook::OnFindText(bool bExistNext)
{
	// send information to UI
	CWnd* pParentWnd = GetParent();
	if(pParentWnd)
		pParentWnd->SendMessage(WM_FIND_TEXT, (WPARAM)this, (LPARAM)&bExistNext);
}

/**
 * @brief	계속해서 다음 검색을 하는 함수
 * @remarks	다음 검색 결과가 없을 경우, 현재 보이는 챕터의 다음 챕터로 이동하여 검색을 계속 할 때 호출한다.
 *			검색 결과를 받은 후 검색을 계속할지 판단하며, 그외에 별도로 호출하지 않는다.
 * @param	keyword	: 검색 문자열
 * @param	chapter	: 계속 해서 검색 할 챕터 인덱스(spine 인덱스)
 */
void CEPubBook::OnFindTextInNextChapter(CString keyword, int chapter)
{
	if(keyword.IsEmpty()) return;
	if(!m_pViewManager) return;
	if(chapter > (int)m_spineList.size()) return;

	FindTextAndMove(keyword, true, chapter);
}

/**
 * @brief	계속해서 이전 검색을 하는 함수
 * @remarks	이전 검색 결과가 없을 경우, 현재 보이는 챕터의 이전 챕터로 이동하여 검색을 계속 할 때 호출한다.
 *			검색 결과를 받은 후 검색을 계속할지 판단하며, 그외에 별도로 호출하지 않는다.
 * @param	keyword	: 검색 문자열
 * @param	chapter	: 계속 해서 검색 할 챕터 인덱스(spine 인덱스)
 */
void CEPubBook::OnFindTextInPrevChapter(CString keyword, int chapter)
{
	if(keyword.IsEmpty()) return;
	if(!m_pViewManager) return;
	if(chapter < 0) return;

	FindTextAndMove(keyword, false, chapter);
}

/**
 * @brief 본문 내용 전체 검색을 한 이후 웹킷 extension 으로 부터 호출되는 callback 함수. 검색 결과를 파라미터로 받는다.
 * @param	param	: 본문 내용 검색 결과
 */
void CEPubBook::OnFindTextAll(CString param)
{
	if(param.IsEmpty()) return;
	if(m_pResultList == NULL) m_pResultList = new FindResultList;

	// 파라미터 JSON 으로 파싱
	JSONValue* pValue = JSON::Parse(param);
	if(pValue == NULL) return;
	if(pValue->IsObject() == false) return;

	JSONObject root;
	root = pValue->AsObject();
	// "RESULT_CODE" 가 0 이 아니면 에러
	JSONObject::iterator iter = root.find(_T("RESULT_CODE"));
	if(iter != root.end() && root[_T("RESULT_CODE")]->IsNumber()){
		if(root[_T("RESULT_CODE")]->AsNumber() != 0.0)
			return;
	}

	JSONArray ary;
	CString fileName = _T("");
	// 본문을 검색한 파일명 추출
	iter = root.find(_T("FILE_NAME"));
	if(iter != root.end() && root[_T("FILE_NAME")]->IsString()){
		fileName = root[_T("FILE_NAME")]->Stringify().c_str();
	}
	
	// 검색 결과 리스트를 추출한다.
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
 * @brief	검색 결과를 반환하는 함수
 * @remarks	본문 전체 검색 후, 모든 결과를 한꺼번에 받을 때 사용한다.
 * @return	검색 결과 리스트
 */
FindResultList* CEPubBook::GetFindTextAllResult()
{
	return m_pResultList;
}

/**
 * @brief	검색 결과의 개수를 반환하는 함수
 * @return	검색 결과 리스트 개수
 */
int CEPubBook::GetFindTextAllResultCount()
{
	return m_pResultList->size();
}







/********************************************************************************
 * interface
 ********************************************************************************/
/**
 * @brief		OPF file 을 읽는다. 
 * @remarks		OPF 에 기술된 책의 정보 및 TOC 파일의 상대경로 등의 정보를 해석한다.
 * @param		lpszPathName	: OPF file 절대 경로
 * @return		success(TRUE)/fail(FALSE)
 */
BOOL CEPubBook::ReadOPSFile(LPCTSTR lpszPathName)
{
	TRACEMSG(_T("CEPubBook::ReadOPSFile() START"));
	//
	BOOL bRes = TRUE;
	OPFXML	opfXML;
	// opf 파일을 파싱한다.
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
		// 엔진에서 epub 2.0 을 지원하는 경우, 그 이상의 버전에서는 책을 open 하지 않는다.
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
 * @brief	toc 파일을 파싱한다.
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
	
	// 목차의 최상위 root 엘리먼트는 책의 타이틀 정보로 만든다.
	m_tocTree.SetTitle(m_pBookInfo->GetTitle());

	// epub 버전에 따라 목차를 파싱하는 방법이 달라진다. EPub 2.0 => ncx,  EPub 3.0 =>nav
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
 * @brief 엔진으로부터 전달받은 event 들의 parameter 를 해석한다.
 * @param	[in] param		: 엔진으로 부터 받은 데이터.(파싱해야 할 데이터)
 *			[in] key		: 파싱하여 얻을 데이터의 키 값
 *			[out] resValue	: 파싱하여 얻은 데이터의 포인터
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
 * @brief 엔진으로부터 전달받은 event 들의 parameter 를 해석하여 원하는 값을 스트링으로 전달한다.
 * @param	[in] param		: 엔진으로 부터 받은 데이터.(파싱해야 할 데이터)
 *			[in] key		: 파싱하여 얻을 데이터의 키 값
 *			[out] resValue	: 파싱하여 얻은 데이터
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
 * @brief 엔진으로부터 전달받은 검색 결과 event 의 parameter 를 해석한다.
 * @param	[in] param		: 엔진으로 부터 받은 데이터.(파싱해야 할 데이터)
 *			[out] bMatches	: 다음 검색 결과가 있는 지를 나타내는 데이터
 *			[out] bForward	: 검생 방향을 나다내는 데이터 (true : next, false : prev)
 *			[out] page		: 검색 결과가 있는 페이지
 *			[out] keyword	: 검색 키워드
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
  @remarks	OPS 폴더명과 OPS 파일의 경로를 가져온다.
  @param	[in] lpszFilePath		"container.xml" 의 절대 경로
  @param	[out] oOPSFolderName	OPS 폴더명
  @param	[out] oOPSFileName		OPS 파일의 경로
  @return	TRUE(success) / FALSE (fail)
*/
bool CEPubBook::GetPackageFileName(LPCTSTR lpszFilePath, CString &oOPSFolderName, CString &oOPSFileName)
{
	TRACEMSG(_T("CEPubBook::GetPackageFileName() START"));
	//
	// container.xml 파일을 파싱한다.
	UniString uniOPSFolderName, uniOPSFileName;
	CXMLDOMDocument2	xmlDom;
	bool ok = xmlDom.Open(lpszFilePath);
	if(!ok)
	{
		TRACEMSG_ERR(_T("can't open \"container.xml\"!!"));
		return FALSE;
	}
	//
	// opf 파일의 경로를 가져온다.
	UniString opfFilePath;
	xmlDom.SelectionNamespaces("xmlns:container='urn:oasis:names:tc:opendocument:xmlns:container'");
	CXMLDOMElement elem = xmlDom.SelectSingleNode(L"//container:rootfile[@media-type='application/oebps-package+xml']");
	ok = elem.GetAttribute(L"full-path", opfFilePath);
	if ( opfFilePath.IsEmpty() ){
		TRACEMSG_ERR(_T("opf file path is empty!!"));
		return FALSE;
	}
	//
	// opf 파일에서 '/' 을 기준으로 ops 폴더(패키지 폴더) 명과 opf 파일 명을 구분한다.
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
 * @brief	컨텐츠가 fixed layout 으로 구성되었는지 확인한다.
 * @remarks	현재는 apple spec, samsung spec 만을 검사한다. 추후 IDPF spec 을 반영예정.
 * @param	lpszFilePath	: Epub 이 unzip 된 절대경로
 * @return	TRUE(fixed) / FALSE(reflow)
 */
bool CEPubBook::GetFixedFeature(LPCTSTR lpszFilePath)
{
	// apple fixed 규칙을 확인한다.
	bool bFixed = GetAppleFeature(lpszFilePath);
	if(!bFixed)
	{
		// apple fixed 가 아닌 경우, 삼성의 fixed 규칙을 확인한다.
		bFixed = GetSamsungFeature(lpszFilePath);
	}
	return bFixed;
}


/**
 * @brief	Apple 에서 정의한 fixed layout 규칙을 확인한다.
 * @remarks	"com.apple.ibooks.display-options.xml" 파일이 있어야 하며, 그 안에 fixed true/false 의 정보가 있음.
 * @param	lpszFilePath	: Epub 이 unzip 된 절대경로
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
 * @brief	samsung 에서 정의한 fixed layout 규칙을 확인한다.
 * @remarks	"samsung.opm.property.xml" 파일이 있어야 한다. 해당 파일에는 Portraint/Landscape/Both 의 정보가 기재되어 있다.
 * @param	lpszFilePath	: Epub 이 unzip 된 절대경로
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
 * @brief	unzip 된 경로 이하 내용을 모두 삭제한다.
 * @param	lpszFilePath	: epub 이 unzip 된 경로
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
 * @brief	특정 파일의 spine 인덱스를 가져온다.
 * @param	fileName	: spine의 인덱스를 확인할 파일명
 * @return	spine 인덱스 값(0~max)
 * @bug		파일의 끝의 #sefe 등으로 끝나는 경우에 대한 처리가 되어 있지 않음.
 */
int	 CEPubBook::GetSpineIndex(CString fileName)
{
	if(fileName.IsEmpty()) return -1;

	CString strName;
	fileName.Replace(_T("/"), _T("\\"));

	// 파라미터로 넘어온 파일명이 절대경로일지, 상대경로일지 알 수 없으므로, 일단, 패키지폴더(OEBPS) 를 기준으로 상대경로로 파일명을 변경한다.
	// OPF 파일이 없다면 뒤에서부터 "\\" 구분자로 나누어 파일명만을 추출한다.
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

	// spine 리스트에서 파일명이 일치하는 spine 인덱스를 찾는다.
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
 * @brief	메모 아이콘들을 Package 폴더 $(unzipPath)\OEPBS\ 의 경로로 복사한다.
 * @remarks	그냥 써도 되나 각 xhtml 에서 메모 추가시 url 을 상대 경로로 지정하기 위해서. 현재 사용 안됨.
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
 * @brief	Epub 3.0 spec 을 준수하여 목차 정보를 생성한다.
 * @param	[in] navPointNode	: parent 목차 노드
 * @param	[in] navPoints	: toc 파일로부터 파싱한 목차 데이터
 */
void CEPubBook::AppendNavPointOfNCX(IN3NavPointNode &navPointNode, CXMLDOMNodeList &navPoints)
{
	// 파싱된 데이터로 부터 목차 리스트의 개수를 가져온다.
	long count = navPoints.GetCount();
	//
	// 목차 개수 만큼 목차 데이터를 생성하여 첫번째 파라미터로 넘어온 부모 목차의 자식 목차로 추가한다.
	for(long i = 0; i<count; i++)
	{
		CXMLDOMNode child;
		child = navPoints.GetItem(i);

		// 목차명, 목차 경로를 추출
		UniString	uniTitle;
		UniString	uniURL;
		child.SelectSingleNode(USTR("./ncx:navLabel/ncx:text")).GetText(uniTitle);
		child.SelectSingleNode(USTR("./ncx:content")).GetAttribute(USTR("src"), uniURL);

		CString title(uniTitle);
		CString url(uniURL);
		// 
		// fixed layout 컨텐츠의 경우는 목차의 페이지 정보도 설정한다. fixed 에서는 1파일==1페이지 이므로 스파인 인덱스가 페이지 값과 같다.
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
 * @brief	Epub 2.0 spec 을 준수하여 목차 정보를 생성한다.
 * @param	[in] navPointNode	: parent 목차 노드
 * @param	[in] navPoints	: toc 파일로부터 파싱한 목차 데이터
 */
void CEPubBook::AppendNavPointOfNAV(IN3NavPointNode &navPointNode, CXMLDOMNodeList &navPoints)
{	
	long count = navPoints.GetCount();
	for(long i = 0; i<count; i++)
	{
		CXMLDOMNode child;
		child = navPoints.GetItem(i);

		// 목차명, 목차 경로를 추출
		UniString	uniTitle;
		UniString	uniURL;
		NAVXML::GetHrefAndTitleFromLI(child, uniURL, uniTitle);

		CString title(uniTitle);
		CString url(uniURL);
		// 
		// fixed layout 컨텐츠의 경우는 목차의 페이지 정보도 설정한다. fixed 에서는 1파일==1페이지 이므로 스파인 인덱스가 페이지 값과 같다.
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
 * @brief	parent window 에 epub file 이 정상적으로 Load 됬음을 알리는 함수
 */
void CEPubBook::DocumentLoadedComplate()
{
	TRACEMSG(_T("CEPubBook::DocumentLoadedComplate() START"));
	//
	// 모든 document 가 load 되었을 때 
	// 현재 활성화 된 spine(chapter) 파일이 없다면, 디폴트 spine 파일을 활성화 시킨다. ( default spine(chaptr) index : 0 )
	// 별도의 설정이 없을 경우 default spine index = 0
	if(m_pViewManager->GetCurrChapter() < 0)
	{
		MoveToChapter(0);
	}
	//
	// send massage to UI
	CWnd* pParentWnd = GetParent();
	if(pParentWnd)
		pParentWnd->SendMessage(WM_LODEAD_EPUB, (WPARAM)this, (LPARAM)m_bInitialized);	// UI 단으로 보내는 Load complete message
	//
	TRACEMSG(_T("CEPubBook::DocumentLoadedComplate() END"));
}

/**
 * @brief	view change 하는 함수
 * @param	pPrev	: 이전 뷰(현재 보고있는 뷰)
 * @parma	pNext	: 변경할 뷰(앞으로 보이게 될 뷰)
 * @return	true(success)/false(fail)
 */
BOOL CEPubBook::ChangeView(void *pPrev, void *pNext)
{
	if(!pNext) return FALSE;

	// 이전 뷰를 안보이게 하며, inactive 하게 설정한다.
	IN3WebkitView *pPrevView = (IN3WebkitView *)pPrev;
	if(pPrevView){
		pPrevView->SetDlgCtrlID(AFX_IDW_PANE_FIRST + pPrevView->GetChapterIndex() +1);
		pPrevView->ShowWindow(SW_HIDE);
		pPrevView->EnableWindow(FALSE);
	}

	// 변경할 뷰를 보이게 하며, 최상위 윈도우로 올리고, active window 로 설정한다.
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
 * @brief	view change 하는 함수 (fixed 컨텐츠의 양면 보기인 경우는 현재 보이는 뷰 2개와 앞으로 보일 뷰 2개를 변경해야 한다.)
 * @remarks	fixed 양면보기의 경우는 앞으로 보일 양쪽 페이지에 모두 북마크가 있는지 확인해야 하므로 해당 함수에서 북마크가 있는지 확인한다.
 * @param	pPrev	: 이전 뷰(현재 보고있는 뷰)
 * @parma	pNext	: 변경할 뷰(앞으로 보이게 될 뷰)
 * @return	true(success)/false(fail)
 */
BOOL CEPubBook::ChapgeViewFixedDual(void* pPrev, void *pNext)
{
	if(!pNext) return FALSE;

	// 현재 보이는 뷰를 안보이게 설정한다.
	IN3WebkitView *pLeft, *pRight;
	int chapter = 1, leftChapter =0, rightChapter = 1;
	if(pPrev)
	{
		// 현재 보이는 왼쪽 뷰와 오른쪽 뷰의 인덱스를 가져온다
		chapter = ((IN3WebkitView *)pPrev)->GetChapterIndex();
		leftChapter = chapter % 2 == 0 ? chapter : chapter - 1;
		rightChapter = leftChapter+1;

		// 현재 보이는 왼쪽 뷰를 안보이게 처리
		pLeft = (IN3WebkitView *)m_pViewManager->GetView(leftChapter);
		if(pLeft)
		{
			pLeft->SetDlgCtrlID(AFX_IDW_PANE_FIRST + leftChapter + 1);
			pLeft->ShowWindow(SW_HIDE);
			pLeft->EnableWindow(FALSE);
		}
		
		// 현재 보이는 오른쪽 뷰를 안보이게 처리
		pRight = (IN3WebkitView *)m_pViewManager->GetView(rightChapter);
		if(pRight)
		{
			pRight->SetDlgCtrlID(AFX_IDW_PANE_FIRST + rightChapter + 1);
			pRight->ShowWindow(SW_HIDE);
			pRight->EnableWindow(FALSE);
		}
	}

	// 앞으로 보여야 하는 왼쪽 뷰와 오른쪽 뷰의 인덱스를 가져온다
	chapter = ((IN3WebkitView *)pNext)->GetChapterIndex();
	leftChapter = chapter % 2 == 0 ? chapter : chapter - 1;
	rightChapter = leftChapter+1;

	// 앞으로 보이게 될 왼쪽 뷰를 보이게 처리
	pLeft = (IN3WebkitView *)m_pViewManager->GetView(leftChapter);
	if(pLeft)
	{
		pLeft->EnableWindow(TRUE);
		pLeft->SetDlgCtrlID(AFX_IDW_PANE_FIRST);
		pLeft->BringWindowToTop();
		pLeft->ShowWindow(SW_SHOW);
		pLeft->Invalidate();
		//
		// 새로 이동할 페이지에 북마크 정보를 조회한다.
		QueryBookmarkStatus(leftChapter);		
	}

	// 앞으로 보이게 될 오른쪽 뷰를 보이게 처리
	pRight = (IN3WebkitView *)m_pViewManager->GetView(rightChapter);
	if(pRight)
	{
		pRight->EnableWindow(TRUE);
		pRight->SetDlgCtrlID(AFX_IDW_PANE_FIRST);
		pRight->BringWindowToTop();
		pRight->ShowWindow(SW_SHOW);
		pRight->Invalidate();
		//
		// 새로 이동할 페이지에 북마크 정보를 조회한다.
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
 * @brief	Reflow 의 Layout 을 재정리한다.
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
			// 뷰에 WM_SIZE 메시지를 보내 윈도우의 크기를 정리한다.
			::SendMessage(pWebView->GetSafeHwnd(), WM_SIZE, NULL, lParam);

			// 각 페이지들을 init 한다.
			CString strFontSize;
			strFontSize.Format(_T("%dpt"), m_pSetting->GetFontSize());
			pWebView->InitPage(strFontSize, m_pSetting->GetFontFamily(), m_pSetting->GetBgColor(), m_pSetting->GetFontColor());
		}
	}

	return TRUE;
}

/**
 * @brief	fixed layout 컨텐츠의 단면보기 Layout 을 재정리한다.
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
			// 뷰의 위치를 단면보기 위치로 변경
			pView->MoveWindow(0, 0, clientRect.Width(), clientRect.Height());
			//			
			// 뷰에 WM_SIZE 메시지를 보내 윈도우의 크기를 정리한다.
			DWORD lParam = 0L;
			lParam = ((WORD)clientRect.Height()<<16) | (WORD)clientRect.Width();
			::SendMessage(pView->GetSafeHwnd(), WM_SIZE, NULL, lParam);
			
			// 뷰를 잠시 안보이게 처리
			pView->SetDlgCtrlID(AFX_IDW_PANE_FIRST + i + 1);
			pView->ShowWindow(SW_HIDE);
			pView->EnableWindow(FALSE);
		}
	}

	// 현재 보여야 하는 뷰를 다시 보이도록 설정
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
 * @brief	fixed layout 컨텐츠의 양면보기 Layout 을 재정리한다.
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
	// 가장 마지막에 보이는 뷰가 왼쪽에 표시되는 경우, 뷰의 크기를 반으로 줄이지 말고 단면보기처럼 전체 화면을 사용한다.
	// 다른 뷰 처럼 절반만을 사용하면, 오른쪽 화면에 의도하지 않은 다른 뷰들이 보이기 때문.
	int lastLeftPage = ((int)(m_spineList.size()) -1)%2 == 0? ((int)(m_spineList.size())-1) : -1;
	//
	// 왼쪽, 오른쪽에 뷰를 배치한다. 단, 가장 마지막 뷰가 왼쪽 화면에 올 때는 별도로 처리한다.
	int max = lastLeftPage >=0 ? (int)(m_spineList.size()) -1 : (int)(m_spineList.size());
	for(int i = 0; i<max ; i++)
	{
		IN3WebkitView	*pView = (IN3WebkitView *)m_pViewManager->GetView(i);
		if(!pView) continue;

		DWORD lParam = 0L;
		lParam = ((WORD)clientRect.Height()<<16) | (WORD)(clientRect.Width() / 2);
		::SendMessage(pView->GetSafeHwnd(), WM_SIZE, NULL, lParam);

		// 왼쪽에 오는 화면은 윈도우의 x좌표를 0, 오른쪽에 화면은 정 가운데에서 부터 시작하도록 설정한다.
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

	// 현재 active 되어 보여지는 화면과 함께 보여야 하는 뷰를 화면에 보이도록 처리한다.
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
 * @brief 엔진으로부터 전달받은 annotation event 들의 parameter 를 해석하여 annotation instance를 만든다.
 * @param	[in] param			: 엔진으로 부터 받은 데이터.(파싱해야 할 데이터)
 *			[out] annotation	: 파싱하여 얻은 annotation 정보의 포인터
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
 * @brief 엔진으로부터 전달받은 bookmark event 들의 parameter 를 해석하여 bookmark instance를 만든다.
 * @param	[in] param		: 엔진으로 부터 받은 데이터.(파싱해야 할 데이터)
 *			[out] pBookmark	: 파싱하여 얻은 bookmark 정보의 포인터
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
