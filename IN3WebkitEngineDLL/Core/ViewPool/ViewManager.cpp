#include "StdAfx.h"
#include "ViewManager.h"
#include "IN3WebkitDefine.h"

#include "EPubBook.h"
#include "IN3WebkitView.h"

/**
 * @brief	생성자
 */
CViewManager::CViewManager(void)
	:m_nStartChapter(0)
	,m_nEndChapter(0)
	,m_nCurrChapter(0)
{
}

/**
 * @brief	소멸자
 */
CViewManager::~CViewManager(void)
{
	TRACEMSG(_T("CViewManager::~CViewManager() START"));
	//
	if(m_viewList.size() > 0)
		m_viewList.clear();
	//
	TRACEMSG(_T("CViewManager::~CViewManager() END"));
}

/**
 * @brief	책의 내용을 보여줄 뷰를 만든다
 * @param	pBook	: 뷰를 만들 책의 인스턴스 포인터
 * @return	true(success)/false(fail)
 */
BOOL CViewManager::CreateView(CWnd* pBook)
{
	TRACEMSG(_T("CViewManager::CreateView() START"));
	//
	CEPubBook* pEpub = (CEPubBook *)pBook;
	if(!pEpub){
		TRACEMSG_ERR(_T("CEPubBook is NULL!! can't create views"));
		return FALSE;
	}

	// 만들어야 할 뷰의 개수를 가져온다. 책의 모든 spine 파일들을 뷰에 연결하기 위해 spine 파일 리스트 만큼의 뷰를 생성한다.
	int size = pEpub->GetSpineCount();
	for(int i=0; i<size; i++)
	{
		// 현재 윈도우의 크기를 가져온다.
		CRect clientRect;
		pEpub->GetClientRect(&clientRect);

		// 뷰를 생성하여 책에 붙인다.
		IN3WebkitView* pWebkitView;
		CRuntimeClass* pRuntimeClass = RUNTIME_CLASS(IN3WebkitView);
		CObject *pObj = pRuntimeClass->CreateObject();
		if(pObj)
		{
			pWebkitView = (IN3WebkitView *)pObj;

			UINT viewID = AFX_IDW_PANE_FIRST + i + 1;
			CCreateContext context;
			if(!pWebkitView->Create(NULL, NULL, WS_CHILD, clientRect, pEpub, viewID, &context))
			{
				TRACEMSG_ERR(_T("can't create views. view's create() fail"));
				delete pObj;
				return FALSE;
			}
		}

		// 뷰가 정상적으로 생성되면
		if(pWebkitView)
		{
			// 뷰리스트에 뷰를 추가한다.
			m_viewList.push_back((void *)pWebkitView);
			//
			// spnie 파일을 load 한다.
			pWebkitView->SetChapter(i);
			CString url(pEpub->GetOPSFolderPath());
			url.Append(pEpub->GetSpineURL(i));
			url.Replace(_T("/"), _T("\\"));
			pWebkitView->LoadFromURL(url);
			//
			// 모든 파일들이 load 완료되기 전까진 화면에 안보이게 처리.
			pWebkitView->ShowWindow(SW_HIDE);
			pWebkitView->EnableWindow(FALSE);
		}
	}
	//
	// 멤버 변수 초기화.
	// 뷰와 연결된 spine 파일들의 인덱스 값을 설정하고, 현재는 아직 모두 완료가 안된 상태이기 때문에 현재 보여지는 챕터는 -1 로 설정한다.
	m_nCurrChapter = -1;
	m_nStartChapter = 0;
	m_nEndChapter = size - 1;
	//
	TRACEMSG(_T("CViewManager::CreateView() END"));
	//
	return TRUE;
}

/**
 * @brief	책의 내용을 보여줄 뷰를 만든다
 * @param	pBook			: 뷰를 만들 책의 인스턴스 포인터
 * @param	pRuntimeClass	: 뷰의 런타임 클래스 포인터
 * @remarks	UI 단에서 뷰의 제어를 능동적으로 하기위해 UI 에서 알고있는 형태의 뷰를 만드려고 하였으나, 뷰어 삭제 시 DLL 오류가 발생하여 
 *			현재는 사용하지 않는다. DLL 에서 뷰를 생성하고 실제 뷰의 삭제는 UI 단에서 이루어 지기 때문에 발생한다는데, 해결 방법을 생각해봐야 한다.
 * @return	true(success)/false(fail)
 */
BOOL CViewManager::CreateView(CWnd* pBook, CRuntimeClass* pRuntimeClass)
{
	TRACEMSG(_T("CViewManager::CreateView() START"));
	//
	CEPubBook* pEpub = (CEPubBook *)pBook;
	if(!pEpub){
		TRACEMSG_ERR(_T("CEPubBook is NULL!! can't create views"));
		return FALSE;
	}

	// 만들어야 할 뷰의 개수를 가져온다. 책의 모든 spine 파일들을 뷰에 연결하기 위해 spine 파일 리스트 만큼의 뷰를 생성한다.
	int size = pEpub->GetSpineCount();
	for(int i=0; i<size; i++)
	{
		// 현재 윈도우의 크기를 가져온다.
		CRect clientRect;
		pEpub->GetClientRect(&clientRect);

		// 뷰를 생성하여 책에 붙인다.
		IN3WebkitView* pWebkitView;
		CObject *pObj = pRuntimeClass->CreateObject();
		if(pObj)
		{
			CCreateContext context;
			UINT viewID = AFX_IDW_PANE_FIRST + i + 1;
			pWebkitView = (IN3WebkitView *)pObj;
			if(!pWebkitView->Create(NULL, NULL, WS_CHILD, clientRect, pEpub, viewID, &context))
			{
				TRACEMSG_ERR(_T("can't create views. view's create() fail"));
				delete pObj;
				return FALSE;
			}
		}

		// 뷰가 정상적으로 생성되면
		if(pWebkitView)
		{
			// 뷰리스트에 뷰를 추가한다.
			m_viewList.push_back((void *)pWebkitView);
			//
			// spnie 파일을 load 한다.
			pWebkitView->SetChapter(i);
			CString url(pEpub->GetOPSFolderPath());
			url.Append(pEpub->GetSpineURL(i));
			url.Replace(_T("/"), _T("\\"));
			pWebkitView->LoadFromURL(url);
			//
			// 모든 파일들이 load 완료되기 전까진 화면에 안보이게 처리.
			pWebkitView->ShowWindow(SW_HIDE);
			pWebkitView->EnableWindow(FALSE);
		}
	}
	//
	// 멤버 변수 초기화.
	// 뷰와 연결된 spine 파일들의 인덱스 값을 설정하고, 현재는 아직 모두 완료가 안된 상태이기 때문에 현재 보여지는 챕터는 -1 로 설정한다.
	m_nCurrChapter = -1;
	m_nStartChapter = 0;
	m_nEndChapter = size - 1;
	//
	TRACEMSG(_T("CViewManager::CreateView() END"));
	//
	return TRUE;
}




/********************************************************************************
 * view manager's information
 ********************************************************************************/
/**
 * @brief	만들어진 뷰의 개수를 반환한다.
 * @return	뷰 개수
 */
const int CViewManager::GetCount(void) const
{
	return m_viewList.size();
}

/**
 * @brief	현재 활성화된 챕터 파일의 인덱스(spine 파일의 인덱스와 동일)
 * @return	활성화된 챕터 파일의 index
 */
const int CViewManager::GetCurrChapter() const
{
	return m_nCurrChapter;
}

/**
 * @brief	현재 활성화된 챕터 파일의 인덱스를 설정한다.(spine 파일의 인덱스와 동일)
 * @parma	chapter	: 활성화된 챕터 파일의 index
 */
void CViewManager::SetCurrChapter(int chapter)
{
	m_nCurrChapter = chapter;
}

/**
 * @brief	현재 활성화된 뷰 객체를 리턴하는 함수
 * @return	현재 활성화된 뷰 객체
 */
void* CViewManager::GetCurrentView()
{
	int nCurr = m_nCurrChapter - m_nStartChapter;
	if(nCurr >= 0)
		return m_viewList.at(nCurr);
	else 
		return NULL;
}

/**
 * @brief	임의의 뷰 객체를 리턴하는 함수
 * @return	뷰 객체
 */
void* CViewManager::GetView(int index)
{
	if ( int(m_viewList.size()) <= index) return NULL;
	if(index < 0) return NULL;

	return m_viewList.at(index);
}






/********************************************************************************
 * page interface
 ********************************************************************************/
/*
 * @brief	책의 전체페이지를 계산하여 반환하는 함수
 * @return	책의 전체 페이지
 */
int CViewManager::GetTotalPage()
{
	int totalpage = 0;
	for(UINT i=0; i<m_viewList.size(); i++)
	{
		IN3WebkitView *pWebkitView = (IN3WebkitView *)m_viewList.at(i);
		totalpage += pWebkitView->GetTotalPage();
	}
	return totalpage;
}

/*
 * @brief	책의 특정 쳅터의 페이지를 반환하는 함수
 * @param	index	임의의 쳅터 인덱스
 * @return	쳅터의 페이지
 */
int CViewManager::GetTotalPage(int index)
{
	IN3WebkitView *pWebkitView = (IN3WebkitView *)m_viewList.at(index);
	return pWebkitView->GetTotalPage();
}