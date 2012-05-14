#include "StdAfx.h"
#include "ViewManager.h"
#include "IN3WebkitDefine.h"

#include "EPubBook.h"
#include "IN3WebkitView.h"

/**
 * @brief	������
 */
CViewManager::CViewManager(void)
	:m_nStartChapter(0)
	,m_nEndChapter(0)
	,m_nCurrChapter(0)
{
}

/**
 * @brief	�Ҹ���
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
 * @brief	å�� ������ ������ �並 �����
 * @param	pBook	: �並 ���� å�� �ν��Ͻ� ������
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

	// ������ �� ���� ������ �����´�. å�� ��� spine ���ϵ��� �信 �����ϱ� ���� spine ���� ����Ʈ ��ŭ�� �並 �����Ѵ�.
	int size = pEpub->GetSpineCount();
	for(int i=0; i<size; i++)
	{
		// ���� �������� ũ�⸦ �����´�.
		CRect clientRect;
		pEpub->GetClientRect(&clientRect);

		// �並 �����Ͽ� å�� ���δ�.
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

		// �䰡 ���������� �����Ǹ�
		if(pWebkitView)
		{
			// �丮��Ʈ�� �並 �߰��Ѵ�.
			m_viewList.push_back((void *)pWebkitView);
			//
			// spnie ������ load �Ѵ�.
			pWebkitView->SetChapter(i);
			CString url(pEpub->GetOPSFolderPath());
			url.Append(pEpub->GetSpineURL(i));
			url.Replace(_T("/"), _T("\\"));
			pWebkitView->LoadFromURL(url);
			//
			// ��� ���ϵ��� load �Ϸ�Ǳ� ������ ȭ�鿡 �Ⱥ��̰� ó��.
			pWebkitView->ShowWindow(SW_HIDE);
			pWebkitView->EnableWindow(FALSE);
		}
	}
	//
	// ��� ���� �ʱ�ȭ.
	// ��� ����� spine ���ϵ��� �ε��� ���� �����ϰ�, ����� ���� ��� �Ϸᰡ �ȵ� �����̱� ������ ���� �������� é�ʹ� -1 �� �����Ѵ�.
	m_nCurrChapter = -1;
	m_nStartChapter = 0;
	m_nEndChapter = size - 1;
	//
	TRACEMSG(_T("CViewManager::CreateView() END"));
	//
	return TRUE;
}

/**
 * @brief	å�� ������ ������ �並 �����
 * @param	pBook			: �並 ���� å�� �ν��Ͻ� ������
 * @param	pRuntimeClass	: ���� ��Ÿ�� Ŭ���� ������
 * @remarks	UI �ܿ��� ���� ��� �ɵ������� �ϱ����� UI ���� �˰��ִ� ������ �並 ������� �Ͽ�����, ��� ���� �� DLL ������ �߻��Ͽ� 
 *			����� ������� �ʴ´�. DLL ���� �並 �����ϰ� ���� ���� ������ UI �ܿ��� �̷�� ���� ������ �߻��Ѵٴµ�, �ذ� ����� �����غ��� �Ѵ�.
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

	// ������ �� ���� ������ �����´�. å�� ��� spine ���ϵ��� �信 �����ϱ� ���� spine ���� ����Ʈ ��ŭ�� �並 �����Ѵ�.
	int size = pEpub->GetSpineCount();
	for(int i=0; i<size; i++)
	{
		// ���� �������� ũ�⸦ �����´�.
		CRect clientRect;
		pEpub->GetClientRect(&clientRect);

		// �並 �����Ͽ� å�� ���δ�.
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

		// �䰡 ���������� �����Ǹ�
		if(pWebkitView)
		{
			// �丮��Ʈ�� �並 �߰��Ѵ�.
			m_viewList.push_back((void *)pWebkitView);
			//
			// spnie ������ load �Ѵ�.
			pWebkitView->SetChapter(i);
			CString url(pEpub->GetOPSFolderPath());
			url.Append(pEpub->GetSpineURL(i));
			url.Replace(_T("/"), _T("\\"));
			pWebkitView->LoadFromURL(url);
			//
			// ��� ���ϵ��� load �Ϸ�Ǳ� ������ ȭ�鿡 �Ⱥ��̰� ó��.
			pWebkitView->ShowWindow(SW_HIDE);
			pWebkitView->EnableWindow(FALSE);
		}
	}
	//
	// ��� ���� �ʱ�ȭ.
	// ��� ����� spine ���ϵ��� �ε��� ���� �����ϰ�, ����� ���� ��� �Ϸᰡ �ȵ� �����̱� ������ ���� �������� é�ʹ� -1 �� �����Ѵ�.
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
 * @brief	������� ���� ������ ��ȯ�Ѵ�.
 * @return	�� ����
 */
const int CViewManager::GetCount(void) const
{
	return m_viewList.size();
}

/**
 * @brief	���� Ȱ��ȭ�� é�� ������ �ε���(spine ������ �ε����� ����)
 * @return	Ȱ��ȭ�� é�� ������ index
 */
const int CViewManager::GetCurrChapter() const
{
	return m_nCurrChapter;
}

/**
 * @brief	���� Ȱ��ȭ�� é�� ������ �ε����� �����Ѵ�.(spine ������ �ε����� ����)
 * @parma	chapter	: Ȱ��ȭ�� é�� ������ index
 */
void CViewManager::SetCurrChapter(int chapter)
{
	m_nCurrChapter = chapter;
}

/**
 * @brief	���� Ȱ��ȭ�� �� ��ü�� �����ϴ� �Լ�
 * @return	���� Ȱ��ȭ�� �� ��ü
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
 * @brief	������ �� ��ü�� �����ϴ� �Լ�
 * @return	�� ��ü
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
 * @brief	å�� ��ü�������� ����Ͽ� ��ȯ�ϴ� �Լ�
 * @return	å�� ��ü ������
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
 * @brief	å�� Ư�� ������ �������� ��ȯ�ϴ� �Լ�
 * @param	index	������ ���� �ε���
 * @return	������ ������
 */
int CViewManager::GetTotalPage(int index)
{
	IN3WebkitView *pWebkitView = (IN3WebkitView *)m_viewList.at(index);
	return pWebkitView->GetTotalPage();
}