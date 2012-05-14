#include "StdAfx.h"
#include "ViewPoolManager.h"
#include "ViewManager.h"

#include "IN3WebkitDefine.h"
#include "EPubBook.h"

/**
 * @brief	������
 */
CViewPoolManager::CViewPoolManager(void)
	:m_nPoolCnt(0)
{
}

/**
 * @brief	�Ҹ���
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
 * @brief	å�� �並 ����� �Լ�
 * @param	pBook			: �並 ���� å�� �ν��Ͻ� ������
 * @param	pRuntimeClass	: ������ ���� ��Ÿ�� Ŭ����
 * @return	true(success)/false(fail)
 */
BOOL CViewPoolManager::CreateView(CWnd* pBook, CRuntimeClass* pRuntimeClass)
{
	TRACEMSG(_T("CViewPoolManager::CreateView() START"));
	//
	BOOL bRes = FALSE;
	CEPubBook	*pEpub = (CEPubBook *)pBook;
	if(pEpub){
		// ó�� open �� ���� ������ ��� spine ������ Read �Ѵ�.
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
 * @brief	������ å�� ���õ� �並 View pool ���� �����ϴ� �Լ�
 * @param	pBook : �並 ������ å
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

	// �� ���� ������Ʈ
	CViewManager *pMgr = (CViewManager *)iter->second;
	if(pMgr)
	{
		if(pMgr->GetCount() > 0 )
			m_nPoolCnt -= pMgr->GetCount();
	}

	// �� �Ŵ��� ����Ʈ���� �ش� å�� �Ŵ��� ����
	m_pool.erase(iter);
	//
	TRACEMSG(_T("CViewPoolManager::RemoveView() END"));
	//
	return FALSE;
}