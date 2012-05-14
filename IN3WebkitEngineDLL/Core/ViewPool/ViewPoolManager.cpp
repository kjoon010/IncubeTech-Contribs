#include "StdAfx.h"
#include "ViewPoolManager.h"
#include "ViewManager.h"

#include "IN3WebkitDefine.h"
#include "EPubBook.h"

/**
 * @brief	생성자
 */
CViewPoolManager::CViewPoolManager(void)
	:m_nPoolCnt(0)
{
}

/**
 * @brief	소멸자
 */
CViewPoolManager::~CViewPoolManager(void)
{
	TRACEMSG(_T("CViewPoolManager::~CViewPoolManager() START"));
	//
	if(m_pool.size() > 0)
		m_pool.clear();
	//
	TRACEMSG(_T("CViewPoolManager::~CViewPoolManager() END"));
}

/**
 * @brief	책의 뷰를 만드는 함수
 * @param	pBook			: 뷰를 만들 책의 인스턴스 포인터
 * @param	pRuntimeClass	: 생성할 뷰의 런타임 클래스
 * @return	true(success)/false(fail)
 */
BOOL CViewPoolManager::CreateView(CWnd* pBook, CRuntimeClass* pRuntimeClass)
{
	TRACEMSG(_T("CViewPoolManager::CreateView() START"));
	//
	BOOL bRes = FALSE;
	CEPubBook	*pEpub = (CEPubBook *)pBook;
	if(pEpub){
		// 처음 open 할 때는 무조건 모든 spine 파일을 Read 한다.
		// STEP 1) Create
		CViewManager	*pViewMgr = new CViewManager;
		if(pViewMgr)
		{
			if(pRuntimeClass ==NULL)
				bRes = pViewMgr->CreateView(pBook);
			else
				bRes = pViewMgr->CreateView(pBook, pRuntimeClass);

			if(bRes)
			{
				pEpub->SetViewManager(pViewMgr);

				// STEP 2) Add view to pool
				m_nPoolCnt += pViewMgr->GetCount();
				m_pool.insert( ViewPool::value_type(pBook, pViewMgr));
			}
			else
			{
				TRACEMSG_ERR(_T("CViewManager failed to create views"));
				delete pViewMgr;
			}
		}
	}
	//
	TRACEMSG(_T("CViewPoolManager::CreateView() END"));
	//
	return bRes;
}

/**
 * @brief	임의의 책과 관련된 뷰를 View pool 에서 제거하는 함수
 * @param	pBook : 뷰를 제거할 책
 * @return	success(ture)/fail(false)
 */
BOOL	CViewPoolManager::RemoveView(CWnd* pBook)
{
	TRACEMSG(_T("CViewPoolManager::RemoveView() START"));
	//
	ViewPool::iterator iter = m_pool.find(pBook);
	if(iter == m_pool.end())
	{
		TRACEMSG_ERR(_T("CViewPoolManager::RemoveView() - The map m_pool dosen't have an element with a key of \"pWnd\""));
		return FALSE;
	}

	// 뷰 개수 업데이트
	CViewManager *pMgr = (CViewManager *)iter->second;
	if(pMgr)
	{
		if(pMgr->GetCount() > 0 )
			m_nPoolCnt -= pMgr->GetCount();
	}

	// 뷰 매니져 리스트에서 해당 책의 매니져 삭제
	m_pool.erase(iter);
	//
	TRACEMSG(_T("CViewPoolManager::RemoveView() END"));
	//
	return FALSE;
}