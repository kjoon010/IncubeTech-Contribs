#include "StdAfx.h"
#include "IN3WebkitEngine.h"
#include "IN3WebkitDefine.h"

#include "EPubBook.h"

/**
 * @brief	������
 */

IN3WebkitEngine::IN3WebkitEngine(void)
{
	// unzip ��� ����
	TCHAR	localAppData[_MAX_PATH] = _T("");
	SHGetSpecialFolderPath(NULL, localAppData, CSIDL_LOCAL_APPDATA, TRUE);
	m_epubMainPath = localAppData;
	m_epubMainPath += _T("\\Incube");
	CreateDirectory(m_epubMainPath, NULL);
	//
	m_epubMainPath += _T("\\IPV_Epub");
	CreateDirectory(m_epubMainPath, NULL);
}

/**
 * @brief	�Ҹ���
 */
IN3WebkitEngine::~IN3WebkitEngine(void)
{
	TRACEMSG(_T("IN3WebkitEngine::~IN3WebkitEngine() START"));
	//
	if(m_bookList.size() > 0)
	{
		m_bookList.clear();
	}
	//
	TRACEMSG(_T("IN3WebkitEngine::~IN3WebkitEngine() END"));
}

/**
 * @brief	Epub Open �ÿ� Load �Ǵ� ���̺귯������ ���� Load �Ѵ�.
 * @remarks	���� Epub �� �� ������ ���� Library ���� Load �Ǿ� �ӵ� ������ ������ �ȴ�.
 *			���� �̸� ���̺귯������ Load �� �θ� Epub Open �ÿ� �ҿ�Ǵ� �ӵ��� ���� �� �ִ�.
 * @return	true(success)/false(fail)
 */
BOOL IN3WebkitEngine::LoadLibraries(void)
{
	TRACEMSG(_T("IN3WebkitEngine::LoadLibraries() START"));
	//
	BOOL bRes = TRUE;

	::LoadLibrary(L"TortoiseOverlays.dll");
	::LoadLibrary(L"TortoiseStub.dll");
	::LoadLibrary(L"TortoiseSVN.dll");

	::LoadLibrary(L"EhStorShell.dll");
	::LoadLibrary(L"sspicli.dll");
	::LoadLibrary(L"dhcpcsvc6.dll");
	::LoadLibrary(L"dhcpcsvc.dll");
	::LoadLibrary(L"mscms.dll");
	::LoadLibrary(L"userenv.dll");
	::LoadLibrary(L"cryptdll.dll");
	::LoadLibrary(L"igdumdx32.dll");
	::LoadLibrary(L"igdumd32.dll");
	::LoadLibrary(L"icm32.dll");

	::LoadLibrary(L"QTMovieWin.dll");
	::LoadLibrary(L"QuickTime.qts");
	::LoadLibrary(L"QTCF.dll");
	::LoadLibrary(L"dsound.dll");
	::LoadLibrary(L"MMDevAPI.dll");
	::LoadLibrary(L"wdmaud.drv");
	::LoadLibrary(L"ksuser.dll");
	::LoadLibrary(L"avrt.dll");
	::LoadLibrary(L"AudioSes.dll");
	::LoadLibrary(L"msacm32.drv");
	::LoadLibrary(L"msacm32.dll");
	::LoadLibrary(L"midimap.dll");

	::LoadLibrary(L"ddraw.dll");
	::LoadLibrary(L"dciman32.dll");
	::LoadLibrary(L"QuickTime3GPP.qtx");
	::LoadLibrary(L"CoreVideo.qtx");
	::LoadLibrary(L"QuickTime.qts");
	::LoadLibrary(L"QuickTime3GPPAuthoring.qtx");
	::LoadLibrary(L"QuickTimeAudioSupport.qtx");
	::LoadLibrary(L"CoreAudioToolbox.dll");
	::LoadLibrary(L"QuickTimeAuthoring.qtx");
	::LoadLibrary(L"QuickTimeCapture.qtx");
	::LoadLibrary(L"QuickTimeEffects.qtx");
	::LoadLibrary(L"QuickTimeEssentials.qtx");
	::LoadLibrary(L"QuickTimeH264.qtx");
	::LoadLibrary(L"QuickTimeImage.qtx");
	::LoadLibrary(L"QuickTimeInternetExtras.qtx");
	::LoadLibrary(L"QuickTimeMPEG.qtx");
	::LoadLibrary(L"QuickTimeMPEG4.qtx");
	::LoadLibrary(L"QuickTimeMPEG4Authoring.qtx");
	::LoadLibrary(L"QuickTimeMusic.qtx");
	::LoadLibrary(L"QuickTimeStreaming.qtx");
	::LoadLibrary(L"QuickTimeStreamingAuthoring.qtx");
	::LoadLibrary(L"QuickTimeStreamingExtras.qtx");
	::LoadLibrary(L"QuickTimeVR.qtx");
	::LoadLibrary(L"Plugins\npqtplugin5.dll");
	::LoadLibrary(L"QuickTimeWebHelper.qtx");
	//::LoadLibrary(L"QuickTimeVR.qtx");
	//::LoadLibrary(L"npqtplugin5.dll");

	::LoadLibrary(L"mlang.dll");
	::LoadLibrary(L"secur32.dll");
	::LoadLibrary(L"dnsapi.dll");
	::LoadLibrary(L"rasapi32.dll");
	::LoadLibrary(L"rasman.dll");
	::LoadLibrary(L"rtutils.dll");
	::LoadLibrary(L"SensApi.dll");
	::LoadLibrary(L"mswsock.dll");
	::LoadLibrary(L"zipfldr.dll");
	::LoadLibrary(L"ExplorerFrame.dll");
	::LoadLibrary(L"tiptsf.dll");
	::LoadLibrary(L"t2embed.dll");

	::LoadLibrary(L"cscui.dll");
	::LoadLibrary(L"cscdll.dll");
	::LoadLibrary(L"cscapi.dll");
	::LoadLibrary(L"ntshrui.dll");
	::LoadLibrary(L"srvcli.dll");
	::LoadLibrary(L"slc.dll");
		
	return bRes;
	//
	TRACEMSG(_T("IN3WebkitEngine::LoadLibraries() END"));
}

/*
 * @brief	å�� ����.
 * @param	pParentWnd		: ������ å ��ü�� ParentWnd ������
 * @param	lpszPathName	: ���� å�� ���� ���
 * @param	lpszDescPath	: unzip �� ���� ���
 * @return	������ å ��ü
 */
CWnd* IN3WebkitEngine::Open(CWnd *pParentWnd, LPCTSTR lpszFilePath, LPCTSTR lpszDescPath)
{
	TRACEMSG(_T("IN3WebkitEngine::Open() START"));
	//
#if !SUPPORT_VIEW_POOL_MANAGER
	// ���ÿ� �� �� �ִ� å�� ������ ����
	if(m_bookList.size() > MAX_EPUB)
	{
		TRACEMSG_ERR(_T("IN3WebkitEngine::Open() can't create CEPubBook!!."));
		TRACEMSG_ERR(_T("IN3WebkitEngine::Open() - Too may books opend. plase close any book."));
		return NULL;
	}
#endif /*!SUPPORT_VIEW_POOL_MANAGER*/

	CEPubBook*	pBook = new CEPubBook;
	if(!pBook)
	{
		TRACEMSG_ERR(_T("IN3WebkitEngine::Open() can't create CEPubBook!!"));
		return NULL;
	}
	//
	if(!pParentWnd)
	{
		TRACEMSG_ERR(_T("parent window pointer is NULL!! can't create book!!"));
		delete pBook;
		return NULL;
	}
	//
	CRect clientRect;
	::GetClientRect(pParentWnd->GetSafeHwnd(), &clientRect);
	if(!pBook->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, clientRect, pParentWnd))
	{
		TRACEMSG_ERR(_T("IN3WebkitEngine::Open() can't create CEPubBook!!"));
		delete pBook;
		return NULL;
	}

	// STEP 1) Get Book's information
	TRACEMSG(_T("STEP 1) Get Book's information"));
	if(!pBook->Open(lpszDescPath))
	{
		TRACEMSG_ERR(_T("CEPubBook::Open() return FALSE.."));
		delete pBook;
		return NULL;
	}
	pBook->SetOriginalEpubPath(lpszFilePath);

	// STEP 2) Read OPF File
	TRACEMSG(_T("STEP 2) Read OPF File"));
	if(!pBook->ReadOPSFile(pBook->GetOPSFilePath()))
	{
		TRACEMSG_ERR(_T("IN3WebkitEngine::Open() can't read OPF file!!"));
		delete pBook;
		return NULL;
	}

	// STEP 3) Get TOC info
	TRACEMSG(_T("STEP 3) Get TOC info"));
	if(!pBook->ReadTocFile())
	{
		TRACEMSG_ERR(_T("TOC File can't parse.."));
	}

	// STEP 4) Add book to book list
	TRACEMSG(_T("STEP 4) Add book to book list"));
	m_bookList.insert(EBookList::value_type(lpszFilePath, pBook));

	//
	TRACEMSG(_T("IN3WebkitEngine::Open() END"));
	//

	return pBook;
}

/**
 * @brief	�������� ������ å�� �����Ѵ�. 
 * @remarks	�������� ������ �ִ� å ����Ʈ���� ����
 * @param	lpszFilePath	: ������ å�� ���� �ҽ� ���
 */
void IN3WebkitEngine::Close(LPCTSTR lpszFilePath)
{
	TRACEMSG(_T("IN3WebkitEngine::Close() START"));
	//
	CString deleteBookPath(lpszFilePath);
	if(deleteBookPath.IsEmpty()) return;
	//
	if(m_bookList.size() <= 0) return;

	EBookList::iterator iter = m_bookList.find(deleteBookPath);
	if(iter == m_bookList.end()) return;
	
#if SUPPORT_VIEW_POOL_MANAGER
	CWnd *pBook = (CWnd *)iter->second;
	if(pBook)
		m_viewPoolManager.RemoveView(pBook);
#endif /*SUPPORT_VIEW_POOL_MANAGER*/

	m_bookList.erase(iter);
	//
	TRACEMSG(_T("IN3WebkitEngine::Close() END"));
}

/**
 * @brief	å�� �ݴ´�.
 * @remarks	�������� ������ �ִ� å ����Ʈ���� ����
 * @param	pBook	: ������ å
 */
void IN3WebkitEngine::Close(CWnd *pBook)
{
	if(!pBook) return;
	TRACEMSG(_T("IN3WebkitEngine::Close() START"));
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		CEPubBook* pEpub = (CEPubBook *)pBook;
		if(!pEpub) return;
		//
		if(m_bookList.size() <= 0) return;

		EBookList::iterator iter = m_bookList.find(pEpub->GetOriginalEpubPath());
		if(iter == m_bookList.end()) return;

		m_bookList.erase(iter);
		
	#if SUPPORT_VIEW_POOL_MANAGER
		m_viewPoolManager.RemoveView(pBook);
	#endif /*SUPPORT_VIEW_POOL_MANAGER*/

		if(pBook)
			delete pBook;
	}
	//
	TRACEMSG(_T("IN3WebkitEngine::Close() END"));
}

/**
 * @brief	å�� �ʿ��� �並 �����ϴ� �Լ�
 * @param	pBook	: �並 ������ å �ν��Ͻ� ������
 * @return	true(success)/false(fail)
 */
BOOL IN3WebkitEngine::CreateView(CWnd* pBook)
{
	if(!pBook) return FALSE;
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		TRACEMSG(_T("IN3WebkitEngine::CreateView() START"));
		//
		#if SUPPORT_VIEW_POOL_MANAGER	
			BOOL bRes = m_viewPoolManager.CreateView(pBook, NULL);
		#else
			// 
			// ��������
			BOOL bRes = FALSE;
			CEPubBook* pEpub = (CEPubBook *)pBook;
			if(pEpub)
				bRes = pEpub->CreateView(NULL);
		#endif
		//
		TRACEMSG(_T("IN3WebkitEngine::CreateView() END"));
		//
		return bRes;
	}
	return FALSE;
}

/**
 * @brief	å�� �ʿ��� �並 �����ϴ� �Լ�
 * @remarks	������ ������ �並 �����Ѵ�.
 * @param	pBook			: �並 ������ å �ν��Ͻ� ������
 * @param	pRuntimeClass	: �����ϰ��� �ϴ� ���� ��Ÿ�� Ŭ����
 * @return	true(success)/false(fail)
 */
BOOL IN3WebkitEngine::CreateView(CWnd* pBook, CRuntimeClass* pRuntimeClass)
{
	if(!pBook) return FALSE;
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		TRACEMSG(_T("IN3WebkitEngine::CreateView() START"));
		//
		#if SUPPORT_VIEW_POOL_MANAGER
			BOOL bRes = m_viewPoolManager.CreateView(pBook, pRuntimeClass);
		#else
			BOOL bRes = FALSE;
			CEPubBook* pEpub = (CEPubBook *)pBook;
			if(pEpub)
				bRes = pEpub->CreateView(pRuntimeClass);
		#endif
		//
		TRACEMSG(_T("IN3WebkitEngine::CreateView() END"));
		//
		return bRes;
	}
	return FALSE;
}

/**
 * @brief	epub �� ���̾ƿ��� ������ �Ѵ�.
 */
void IN3WebkitEngine::ReLayout()
{
	TRACEMSG(_T("IN3WebkitEngine::ReLayout() START"));
	//
	EBookList::iterator iter;
	for(iter = m_bookList.begin(); iter !=m_bookList.end(); iter++)
	{
		CEPubBook* pEpub = (CEPubBook*)iter->second;
		if(pEpub)
			pEpub->ReLayout();
	}
	//
	TRACEMSG(_T("IN3WebkitEngine::ReLayout() END"));
}

/**
 * @brief	epub �� ���̾ƿ��� ������ �Ѵ�.
 * @param	pBook	: ���̾ƿ��� ������ å�� �ν��Ͻ� ������
 */
void IN3WebkitEngine::ReLayout(CWnd* pBook)
{
	if(!pBook) return;
	//
	TRACEMSG(_T("IN3WebkitEngine::ReLayout() START"));
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		CEPubBook* pEpub = (CEPubBook *)pBook;
		if(pEpub)
			pEpub->ReLayout();
	}
	//
	TRACEMSG(_T("IN3WebkitEngine::ReLayout() END"));
}

/**
 * @brief	epub �� Reload �Ѵ�.
 * @param	pBook	: Reload �� å�� �ν��Ͻ� ������
 */
void IN3WebkitEngine::ReLoad(CWnd* pBook)
{
	if(!pBook) return;
	//
	TRACEMSG(_T("IN3WebkitEngine::ReLoad() START"));
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		CEPubBook* pEpub = (CEPubBook *)pBook;
		if(pEpub)
			pEpub->ReLoad();
	}
	//
	TRACEMSG(_T("IN3WebkitEngine::ReLoad() END"));
}

/**
 * @brief	UI�ܿ��� Epub display �����Ͽ� �����ϰ� �ִ� id
 * @remarks	tab control �� Epub �� ���̴� ��� tab �� id�� �����Ѵ�.
 * @param	pBook	: å�� �ν��Ͻ� ������
 * @param	id		: UI �ܿ� �� �����ϴ� ���̵�
 */
void IN3WebkitEngine::SetUIID(CWnd *pBook, int id)
{
	if(!pBook) return;
	//
	TRACEMSG(_T("IN3WebkitEngine::SetUIID() START"));
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		CEPubBook *pEpubBook = (CEPubBook *)pBook;
		if(pEpubBook) 
			pEpubBook->SetUIID(id);
	}
	//
	TRACEMSG(_T("IN3WebkitEngine::SetUIID() END"));
}

/**
 * @brief	UI�ܿ��� �� epub ���� �����ϰ� �����ϰ� �ִ� id���� �����´�
 * @remarks	tab control �� Epub �� ���̴� ��� tab �� id���� ����.
 * @param	lpszFileName	: ui �ܿ��� �����ϴ� id�� ������ å�� ���� ���
 * @return	UI �ܿ� �� �����ϴ� ���̵�
 */
const int  IN3WebkitEngine::GetUIID(CString lpszFileName)
{
	if(m_bookList.size() <= 0)
	{
		TRACEMSG(_T("IN3WebkitEngine::IsOpend() END"));
		return -1;
	}
	//
	EBookList::iterator iter = m_bookList.find(lpszFileName);
	if (iter == m_bookList.end())
	{
		TRACEMSG(_T("IN3WebkitEngine::IsOpend() END"));
		return -1;
	}

	CEPubBook* pBook = (CEPubBook*)iter->second;
	if (!pBook)
	{		
		TRACEMSG(_T("IN3WebkitEngine::IsOpend() END"));
		return -1;
	}
	return pBook->GetUIID();
}

/**
 * @brief	UI�ܿ��� �� epub ���� �����ϰ� �����ϰ� �ִ� id���� �����´�
 * @remarks	tab control �� Epub �� ���̴� ��� tab �� id���� ����.
 * @param	pBook	: ui �ܿ��� �����ϴ� id�� ������ å�� �ν��Ͻ� ������
 * @return	UI �ܿ� �� �����ϴ� ���̵�
 */
const int  IN3WebkitEngine::GetUIID(CWnd *pBook)
{
	if(!pBook) return -1;
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		CEPubBook *pEpubBook = (CEPubBook *)pBook;
		if(!pEpubBook) return -1;

		return pEpubBook->GetUIID();	
	}
	return -1;
}

/**
 * @brief	tab control �� ������� �ʰ�, UI �ܿ��� ���� viewing �� �����ϴ� ���, ������ å�� display �ϰ� ������ å�� hide ��Ű�� ���� ����ϴ� api
 * @param	lpszFileName	: viewing �� å�� ���� ���� ���
 */
void IN3WebkitEngine::SetVisibleEpub(CString lpszFileName)
{
	TRACEMSG(_T("IN3WebkitEngine::IsOpend() START"));
	//
	if(m_bookList.size() <= 0)
	{
		TRACEMSG(_T("IN3WebkitEngine::IsOpend() END"));
		return;
	}
	// ��� å���� hide ��Ų��.
	for(EBookList::iterator iter = m_bookList.begin(); iter != m_bookList.end(); iter++)
	{
		CEPubBook* pBook = (CEPubBook*)iter->second;
		if(pBook)
			pBook->ShowWindow(SW_HIDE);
	}

	// url ������ show �� å�� ã�´�
	EBookList::iterator iter = m_bookList.find(lpszFileName);
	if (iter == m_bookList.end())
	{
		TRACEMSG(_T("IN3WebkitEngine::IsOpend() END"));
		return;
	}

	// show
	CEPubBook* pBook = (CEPubBook*)iter->second;
	if (pBook)
	{		
		pBook->ShowWindow(SW_SHOW);
	}
}

/**
 * @brief	tab control �� ������� �ʰ�, UI �ܿ��� ���� viewing �� �����ϴ� ���, ������ å�� display �ϰ� ������ å�� hide ��Ű�� ���� ����ϴ� api
 * @param	pBook	: display �� å�� �ν��Ͻ� ������
 */
void IN3WebkitEngine::SetVisibleEpub(CWnd *pBook)
{
	if(!pBook) return;
	//
	TRACEMSG(_T("IN3WebkitEngine::IsOpend() START"));
	//
	if(!pBook) return;

	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		if(m_bookList.size() <= 0)
		{
			TRACEMSG(_T("IN3WebkitEngine::IsOpend() END"));
			return;
		}

		// ��� å���� hide ��Ų��.
		for(EBookList::iterator iter = m_bookList.begin(); iter != m_bookList.end(); iter++)
		{
			CEPubBook* pEpub = (CEPubBook*)iter->second;
			if(pEpub)
				pEpub->ShowWindow(SW_HIDE);
		}
		
		// show
		CEPubBook *pEpub = (CEPubBook *)pBook;
		if(pEpub)
			pEpub->ShowWindow(SW_SHOW);
	}
}

/**
 * @brief	fixed �������� �並 Ȯ���Ѵ�.
 * @param	pBook	: Ȯ���� å�� �ν��Ͻ� ������
 */
void IN3WebkitEngine::ViewZoomIn(CWnd *pBook)
{
	if(!pBook) return;
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		CEPubBook* pEpub = (CEPubBook *)pBook;
		if(pEpub)
			pEpub->ViewZoom(true);
	}
}

/**
 * @brief	fixed �������� �並 ����Ѵ�.
 * @param	pBook	: ����� å�� �ν��Ͻ� ������
 */
void IN3WebkitEngine::ViewZoomOut(CWnd *pBook)
{
	if(!pBook) return;
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		CEPubBook* pEpub = (CEPubBook *)pBook;
		if(pEpub)
			pEpub->ViewZoom(false);
	}
}

/**
 * @brief	fixed �������� �並 ���� �������� ũ�⿡ ���� �����ش�.
 * @param	pBook	: å�� �ν��Ͻ� ������
 */
void IN3WebkitEngine::ViewFit(CWnd *pBook)
{
	if(!pBook) return;
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		CEPubBook* pEpub = (CEPubBook *)pBook;
		if(pEpub)
			pEpub->ViewFit();
	}
}

/**
 * @brief	���� �������� ������ ���� ��ȯ�Ѵ�.
 * @param	pBook	: ���� �������� ������ ���� ������ å�� �ν��Ͻ� ������
 * @return	���� �������� ������ ��
 */
int IN3WebkitEngine::GetCurrentPage(CWnd *pBook)
{
	if(!pBook) return 0;
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		CEPubBook* pEpub = (CEPubBook *)pBook;
		if(pEpub) 
			return pEpub->GetCurrentPage();
	}
	return 0;
}

/**
 * @brief	å�� �� ������ ���� ��ȯ�Ѵ�.
 * @param	pBook	: å�� �� ������ ���� ������ å�� �ν��Ͻ� ������
 * @return	å�� �� ������ ��
 */
int IN3WebkitEngine::GetTotalPage(CWnd *pBook)
{
	if(!pBook) return 0;
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		CEPubBook* pEpub = (CEPubBook *)pBook;
		if(pEpub) 
			return pEpub->GetTotalPage();
	}
	return 0;
}

/**
 * @brief	���纸�� �ִ� é���� �ε��� ���� ��ȯ�Ѵ�. (spine index ���� ��ġ��)
 * @param	pBook	: é���� �ε��� ���� ������ å�� �ν��Ͻ� ������
 * @return	���� �������� �ִ� spine ������ �ε��� ��.	
 */
int IN3WebkitEngine::GetCurrentChapter(CWnd *pBook)
{
	if(!pBook) return -1;
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		CEPubBook* pEpub = (CEPubBook *)pBook;
		if(pEpub) 
			return pEpub->GetCurrentChapter();
	}
	return -1;
}

/**
 * @brief	å�� spine ���� ��ȯ�Ѵ�.
 * @param	pBook	: å�� spine ���� ������ å�� �ν��Ͻ� ������
 * @return	spine ����
 */
int IN3WebkitEngine::GetTotalChapter(CWnd *pBook)
{
	if(!pBook) return -1;
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		CEPubBook* pEpub = (CEPubBook *)pBook;
		if(pEpub) 
			return pEpub->GetSpineCount();
	}
	return -1;
}

/**
 * @brief	������ ȭ���� ������ ũ��� ĸ���Ѵ�.
 * @remarks fixed layout() ������ ��� ����
 * @param	pBook	: ȭ���� ������ ũ��� ĸ���� å�� �ν��Ͻ� ������
 * @param	[in] page		: ĸ���� ������
 * @param	[in] width		: ĸ�� �̹����� ���� �ȼ� ��
 * @param	[in] height		: ĸ�� �̹����� ���� �ȼ� ��
 * @param	[out] bitmap	: ĸ�� �̹���
 */
bool IN3WebkitEngine::PageCapture(CWnd *pBook, int page, int width, int height, CBitmap &bitmap)
{
	if(!pBook) return false;
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		CEPubBook* pEpub = (CEPubBook *)pBook;
		if(pEpub) 
			return pEpub->PageCapture(page, width, height, bitmap);
	}
	return false;
}





/******************************************************************
 * engine's feature	
 ******************************************************************/
/**
 * @brief	�������� �ʱ�ȭ�Ҷ� �ʿ��� ������ �����Ѵ�.
 * @remarks	��Ʈũ��, ��Ʈ, ��Ʈ��, ������ ���� �����Ѵ�.
 * @param	pBook		: �ʱ�ȭ �� ���� ������ ������ å�� �ν��Ͻ� ������
 * @param	fontFamily	: ��Ʈ��
 * @param	fontSize	: ��Ʈ ũ��
 * @param	fontColor	: ��Ʈ��
 * @param	bgColor	: ����
 * @return	true(success)/false(fail)
 */
BOOL IN3WebkitEngine::InitViewFeatures(CWnd *pBook, CString fontFamily, int fontSize, CString fontColor, CString bgColor)
{
	if(!pBook) return FALSE;
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		CEPubBook* pEpub = (CEPubBook *)pBook;
		if(pEpub)
		{
			// fixed �� ��쿡�� �������� �ʴ´�.
			if(pEpub->IsFixed()) return FALSE;

			pEpub->SetFontFamily(fontFamily);
			pEpub->SetFontSize(fontSize);
			pEpub->SetFontColor(fontColor);
			pEpub->SetBackgroudColor(bgColor);
			return TRUE;
		}
	}
	return FALSE;
}

/**
 * @brief	annotation(���̶���Ʈ/�������/�޸�) �� �ϰ������Ѵ�.
 * @remarks	������ �ε� �Ŀ� annotation �� �ϰ� �����ؾ� �Ѵ�.
 * @param	pBook		: annotation���� ������ å�� �ν��Ͻ� ������
 * @param	annotations	: �����ؾ� �ϴ� annotation ����Ʈ
 * @return	��Ŷ extension ���� annotation �ϰ� ������ ��û�� ��� = TRUE, �׿�= FALSE
 */
BOOL IN3WebkitEngine::InitAnnotationList(CWnd *pBook, std::vector<IN3Annotation*> annotations)
{
	if(!pBook) return FALSE;
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		TRACEMSG(_T("IN3WebkitEngine::InitAnnotation() START"));
		//
		CEPubBook *pEpubBook = (CEPubBook *)pBook;
		if(!pEpubBook){
			TRACEMSG_ERR(_T("CWnd *pBook is NULL.(first parameter)"));
			return FALSE;
		}
		//
		BOOL bRes = TRUE;
		bRes = pEpubBook->InitAnnotationList(annotations);
		TRACEMSG(_T("IN3WebkitEngine::InitAnnotation() END"));
		//
		return bRes;
	}
	return FALSE;
}

/**
 * @brief	�ϸ�ũ���� �ϰ������Ѵ�.
 * @param	pBook		: �ϸ�ũ���� ������ å�� �ν��Ͻ� ������
 * @param	bookmarks	: �ϰ� ������ bookmark ����Ʈ
 * @return	��Ŷ extension ���� �ϸ�ũ �ϰ� ������ ��û�� ��� = TRUE, �׿�= FALSE
 */
BOOL IN3WebkitEngine::InitBookmarkList(CWnd *pBook, std::vector<IN3Bookmark *> bookmarks)
{
	if(!pBook) return FALSE;
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		TRACEMSG(_T("IN3WebkitEngine::InitBookmark() START"));
		//
		CEPubBook *pEpubBook = (CEPubBook *)pBook;
		if(!pEpubBook){
			TRACEMSG_ERR(_T("CWnd *pBook is NULL.(first parameter)"));
			return FALSE;
		}
		//
		BOOL bRes = TRUE;
		bRes = pEpubBook->InitBookmarkList(bookmarks);
		//
		TRACEMSG(_T("IN3WebkitEngine::InitBookmark() END"));
		//
		return bRes;
	}
	return FALSE;
}





/******************************************************************
 * engine's feature	
 ******************************************************************/
/**
 * @brief	open �� epub ���ϵ��� ���� ���� ��θ� ��ȯ�Ѵ�
 * @return	open �� epub ���ϵ��� ���� ���� ���
 */	
const CString IN3WebkitEngine::GetEpubMainPath(void) const
{
	return m_epubMainPath;
}

/**
 * @brief	���� ������ ���(���/�ܸ� ����)�� ��ȯ�Ѵ�.
 * @param	pBook	: ��麸��/�ܸ麸�⸦ Ȯ���� å�� �ν��Ͻ� ������
 * @return	������ ��� (1=�ܸ�, 2=���)
 */
const int IN3WebkitEngine::GetPageDisplayMode(CWnd *pBook) const
{
	if(!pBook) return -1;
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		CEPubBook* pEpub = (CEPubBook *)pBook;
		if(pEpub)
			return pEpub->GetPageDisplayMode();

		return 1;
	}
	return -1;
}

/**
 * @brief	����ڰ� ������ ��Ʈ ����� ��ȯ�Ѵ�.
 * @param	pBook	: ����ڰ� ������ ��Ʈ ����� Ȯ���� å�� �ν��Ͻ� ������
 * @return	����ڰ� ������ ��Ʈ ������
 */
const int IN3WebkitEngine::GetFontSize(CWnd *pBook) const
{
	if(!pBook) return 0;
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		CEPubBook* pEpub = (CEPubBook *)pBook;
		if(pEpub)
		{
			return pEpub->GetFontSize();
		}
	}
	return 0;
}

/**
 * @brief	����ڰ� ������ ��Ʈ���� ��ȯ�Ѵ�.
 * @param	pBook	: ����ڰ� ������ ��Ʈ���� Ȯ���� å�� �ν��Ͻ� ������
 * @return	����ڰ� ������ ��Ʈ��
 */
const CString IN3WebkitEngine::GetFontFamily(CWnd *pBook) const
{
	if(!pBook) return CString(_T(""));
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		CEPubBook* pEpub = (CEPubBook *)pBook;
		if(pEpub)
		{
			return pEpub->GetFontFamily();
		}
	}
	return CString(_T(""));
}

/**
 * @brief	����ڰ� ������ ��Ʈ���� ��ȯ�Ѵ�.
 * @param	pBook	: ����ڰ� ������ ��Ʈ���� Ȯ���� å�� �ν��Ͻ� ������
 * @return	����ڰ� ������ ��Ʈ��
 */
const CString IN3WebkitEngine::GetFontColor(CWnd *pBook) const
{
	if(!pBook) return CString(_T(""));
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		CEPubBook* pEpub = (CEPubBook *)pBook;
		if(pEpub)
		{
			return pEpub->GetFontColor();
		}
	}
	return CString(_T(""));
}

/**
 * @brief	����ڰ� ������ ������ ��ȯ�Ѵ�.
 * @param	pBook	: ����ڰ� ������ ������ Ȯ���� å�� �ν��Ͻ� ������
 * @return	����ڰ� ������ ����
 */
const CString IN3WebkitEngine::GetBackgroudColor(CWnd *pBook) const
{	
	if(!pBook) return CString(_T(""));
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		CEPubBook* pEpub = (CEPubBook *)pBook;
		if(pEpub)
		{
			return pEpub->GetBackgroudColor();
		}
	}	
	return CString(_T(""));
}

/**
 * @brief	���� �Ʒ������� ���� ������ ���� ǥ������ ���θ� �����Ѵ�.
 * @param	pBook	: ���� ������ ���� ǥ�ø� ������ å�� �ν��Ͻ� ������
 * @param	bDisplay	: true(���� �ؿ� ������ ��ȣ ǥ��) / false(������ ��ȣ ǥ�� ����)
 */
void IN3WebkitEngine::SetDisplayPageNumber(CWnd *pBook, bool bDisplay)
{		
	if(!pBook) return;
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		CEPubBook* pEpub = (CEPubBook *)pBook;
		if(pEpub)
			pEpub->SetDisplayPageNumber(bDisplay);
	}
}

/**
 * @brief	���� �Ʒ������� ���� ������ ���� ǥ������ ���θ� �����Ѵ�.
 * @param	bDisplay	: true(���� �ؿ� ������ ��ȣ ǥ��) / false(������ ��ȣ ǥ�� ����)
 */
void IN3WebkitEngine::SetDisplayPageNumber(bool bDisplay)
{
	EBookList::iterator iter = m_bookList.begin();
	for(iter; iter != m_bookList.end(); iter++)
	{
		CEPubBook* pEpub = (CEPubBook *)iter->second;
		if(pEpub)
			pEpub->SetDisplayPageNumber(bDisplay);
	}
}

/**
 * @brief	������ ��带 �����Ѵ�. (�ܸ�/���)
 * @param	pBook	: ������ ��带 ������ å�� �ν��Ͻ� ������
 * @param	mode	: 1 = �ܸ�, 2 = ���
 */
void IN3WebkitEngine::SetPageDisplayMode(CWnd *pBook, int mode)
{	
	if(!pBook) return;
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		if(pBook)
		{
			CEPubBook* pEpub = (CEPubBook *)pBook;
			if(pEpub)
				pEpub->SetPageDisplayMode(mode);
		}
		else
		{
			// å�� �����Ǿ� ���� ������ ���� �����ִ� ��� å�� �����Ѵ�.
			EBookList::iterator iter = m_bookList.begin();
			for(iter; iter != m_bookList.end(); iter++)
			{
				CEPubBook* pEpub = (CEPubBook *)iter->second;
				if(pEpub && pEpub->IsFixed())
					pEpub->SetPageDisplayMode(mode);
			}
		}
	}
}

/**
 * @brief	��Ʈ�� �����Ѵ�.
 * @param	pBook			: ��Ʈ�� ������ å�� �ν��Ͻ� ������
 * @param	fontFamilyName	: ������ ��Ʈ
 * @remarks �����Ǵ� ��Ʈ�� ���� ���ο��� ������ �ִٰ� ReLayout() �� ����� ��Ʈ�� �����ϵ��� �Ѵ�.
 *			���� �Ʒ� �Լ��� �ܼ��� ���� ���ο� ��Ʈ�� �����ϴ� ��ɸ� �Ѵ�.
 *			�� ���� ������ �����ϱ� ���ؼ� ����ڰ� ���ϴ� ������ ReLayout() �Լ��� ȣ���Ͽ� ��� �Ѵ�.
 */
BOOL IN3WebkitEngine::SetFontFamily(CWnd *pBook, CString fontFamilyName)
{
	if(!pBook) return FALSE;
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		CEPubBook* pEpub = (CEPubBook *)pBook;
		if(pEpub)
		{
			if(pEpub->IsFixed()) return FALSE;

			pEpub->SetFontFamily(fontFamilyName);
			return TRUE;
		}
	}
	return FALSE;
}

/**
 * @brief	��Ʈ ����� �����Ѵ�.
 * @param	pBook		: ��Ʈ ũ�⸦ ������ å�� �ν��Ͻ� ������
 * @param	fontSize	: ������ ��Ʈ ������
 * @remarks �����Ǵ� ��Ʈ ������� ���� ���ο��� ������ �ִٰ� ReLayout() �� ����� ��Ʈ ����� �����ϵ��� �Ѵ�.
 *			���� �Ʒ� �Լ��� �ܼ��� ���� ���ο� ��Ʈ ����� �����ϴ� ��ɸ� �Ѵ�.
 *			�� ���� ������ �����ϱ� ���ؼ� ����ڰ� ���ϴ� ������ ReLayout() �Լ��� ȣ���Ͽ� ��� �Ѵ�.
 */
BOOL IN3WebkitEngine::SetFontSize(CWnd *pBook, int fontSize)
{
	if(!pBook) return FALSE;
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		CEPubBook* pEpub = (CEPubBook *)pBook;
		if(pEpub)
		{
			if(pEpub->IsFixed()) return FALSE;

			pEpub->SetFontSize(fontSize);
			return TRUE;
		}
	}
	return FALSE;
}

/**
 * @brief	��Ʈ ������ �����Ѵ�.
 * @param	pBook		: ��Ʈ ������ ������ å�� �ν��Ͻ� ������
 * @param	strColor	: ������ ��Ʈ ����
 * @remarks �����Ǵ� ��Ʈ ������ ���� ���ο��� ������ �ִٰ� ReLayout() �� ����� ��Ʈ ������ �����ϵ��� �Ѵ�.
 *			���� �Ʒ� �Լ��� �ܼ��� ���� ���ο� ��Ʈ ������ �����ϴ� ��ɸ� �Ѵ�.
 *			�� ���� ������ �����ϱ� ���ؼ� ����ڰ� ���ϴ� ������ ReLayout() �Լ��� ȣ���Ͽ� ��� �Ѵ�.
 */
BOOL IN3WebkitEngine::SetFontColor(CWnd *pBook, CString strColor)
{
	if(!pBook) return FALSE;
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		CEPubBook* pEpub = (CEPubBook *)pBook;
		if(pEpub)
		{
			if(pEpub->IsFixed()) return FALSE;

			pEpub->SetFontColor(strColor);
			return TRUE;
		}
	}
	return FALSE;
}

/**
 * @brief	������ �����Ѵ�.
 * @param	pBook		: ������ ������ å�� �ν��Ͻ� ������
 * @param	strColor	: ������ ����
 * @remarks �����Ǵ� ������ ���� ���ο��� ������ �ִٰ� ReLayout() �� ����� ������ �����ϵ��� �Ѵ�.
 *			���� �Ʒ� �Լ��� �ܼ��� ���� ���ο� ������ �����ϴ� ��ɸ� �Ѵ�.
 *			�� ���� ������ �����ϱ� ���ؼ� ����ڰ� ���ϴ� ������ ReLayout() �Լ��� ȣ���Ͽ� ��� �Ѵ�.
 */
BOOL IN3WebkitEngine::SetBackgroudColor(CWnd *pBook, CString strColor)
{
	if(!pBook) return FALSE;
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		CEPubBook* pEpub = (CEPubBook *)pBook;
		if(pEpub)
		{
			if(pEpub->IsFixed()) return FALSE;

			pEpub->SetBackgroudColor(strColor);
			return TRUE;
		}
	}
	return FALSE;
}

/**
 * @brief	��Ʈ��, ������ �����Ѵ�.
 * @remarks	��Ʈ��, ������ �����ϴ� ���� ������ ���̾ƿ��� ������ �� �ʿ�� �����Ƿ� �Լ� ȣ�� ��� �ٷ� �ݿ��Ѵ�.
 * @param	pBook			: ��Ʈ���� ������ ������ å�� �ν��Ͻ� ������
 * @param	foregroundClr	: ��Ʈ��
 * @param	backgroundClr	: ����
 * @return	true(success)/false(fail)
 */
BOOL IN3WebkitEngine::SetColors(CWnd *pBook, CString foregroundClr, CString backgroundClr)
{
	if(!pBook) return FALSE;
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		CEPubBook* pEpub = (CEPubBook *)pBook;
		if(pEpub)
		{
			if(pEpub->IsFixed()) return FALSE;

			pEpub->SetColors(foregroundClr, backgroundClr);
			return TRUE;
		}
	}
	return FALSE;
}



/********************************************************************************
 * Book's Information
 ********************************************************************************/
/**
 * @brief	Epub �� open �Ǿ� �ִ��� Ȯ���ϴ� �Լ�
 * @param	lpszFileName	: ���� å�� ���� ���
 * @return	already opend (TRUE)/ Not yet(FALSE)
 */
const BOOL IN3WebkitEngine::IsOpend(LPCTSTR lpszFileName)
{
	TRACEMSG(_T("IN3WebkitEngine::IsOpend() START"));
	//
	if(m_bookList.size() <= 0)
	{
		TRACEMSG(_T("IN3WebkitEngine::IsOpend() END"));
		return FALSE;
	}
	//
	EBookList::iterator iter = m_bookList.find(lpszFileName);
	if (iter == m_bookList.end())
	{
		TRACEMSG(_T("IN3WebkitEngine::IsOpend() END"));
		return FALSE;
	}

	CEPubBook* pBook = (CEPubBook*)iter->second;
	if (!pBook)
	{		
		TRACEMSG(_T("IN3WebkitEngine::IsOpend() END"));
		return FALSE;
	}
	//
	TRACEMSG(_T("IN3WebkitEngine::IsOpend() END"));
	return pBook->IsOpened();
}

/**
 * @brief	å�� ���������� Load �Ǿ����� �����ϴ� �Լ�
 * @remarks	���� ������ �����ִ� ��� å�� �˻��Ѵ�.
 * @return	TRUE(�ε� ��)/FALSE(�ε� �ȵ�)
 */
const BOOL IN3WebkitEngine::IsLoaded()
{
	BOOL bLoaded = TRUE;
	
	EBookList::iterator iter = m_bookList.begin();
	for(iter; iter != m_bookList.end(); iter++)
	{
		CEPubBook* pEpub = (CEPubBook *)iter->second;
		if(pEpub)
			bLoaded &= pEpub->IsLoaded();
	}

	return bLoaded;
}

/**
 * @brief	å�� ���������� Load �Ǿ����� �����ϴ� �Լ�
 * @param	lpszFileName	: ���� å�� ���� ���
 * @return	TRUE(�ε� ��)/FALSE(�ε� �ȵ�)
 */
const BOOL	IN3WebkitEngine::IsLoaded(LPCTSTR lpszFileName)
{
	TRACEMSG(_T("IN3WebkitEngine::IsOpend() START"));
	//
	if(m_bookList.size() <= 0)
	{
		TRACEMSG(_T("IN3WebkitEngine::IsOpend() END"));
		return FALSE;
	}
	//
	EBookList::iterator iter = m_bookList.find(lpszFileName);
	if (iter == m_bookList.end())
	{
		TRACEMSG(_T("IN3WebkitEngine::IsOpend() END"));
		return FALSE;
	}

	CEPubBook* pBook = (CEPubBook*)iter->second;
	if (!pBook)
	{		
		TRACEMSG(_T("IN3WebkitEngine::IsOpend() END"));
		return FALSE;
	}
	//
	TRACEMSG(_T("IN3WebkitEngine::IsOpend() END"));
	return pBook->IsLoaded();
}

/**
 * @brief	å�� ���������� Load �Ǿ����� �����ϴ� �Լ�
 * @param	pBook	: Load �Ǿ����� Ȯ���� å�� �ν��Ͻ� ������
 * @return	TRUE(�ε� ��)/FALSE(�ε� �ȵ�)
 */
const BOOL IN3WebkitEngine::IsLoaded(CWnd *pBook) const
{
	if(!pBook) return FALSE;
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{	
		CEPubBook* pEpub = (CEPubBook *)pBook;
		if(!pEpub) return FALSE;

		return pEpub->IsLoaded();
	}

	return FALSE;
}

/**
 * @brief	Epub �� Fixed layout ���� Ȯ���ϴ� �Լ�
 * @param	pBook	: Fixed layout ���� Ȯ���� å�� �ν��Ͻ� ������
 * @return	FixedLayout(TRUE)/ReflowLayout(FALSE)
 */
const BOOL IN3WebkitEngine::IsFixed(CWnd *pBook) const
{
	if(!pBook) return FALSE;
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		CEPubBook* pEpub = (CEPubBook *)pBook;
		if(!pEpub) return FALSE;

		return pEpub->IsFixed();
	}
	return FALSE;
}

/**
 * @brief	å�� uuid �� �����´�.
 * @param	pBook	: uuid �� ������ å�� �ν��Ͻ� ������
 * @return	å�� uuid
 */
const CString		IN3WebkitEngine::GetUUIDString(CWnd *pBook) const
{
	if(!pBook) return CString(_T(""));
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		CEPubBook* pEpub = (CEPubBook *)pBook;
		if(pEpub)
			return pEpub->GetUUIDString();
	}
	return CString(_T(""));
}

/**
 * @brief	å�� ������ ��ȯ�Ѵ�.
 * @param	pBook	: å�� ������ ������ å�� �ν��Ͻ� ������
 * @return	å�� ���� 3 or 2
 */
const CString		IN3WebkitEngine::GetVersion(CWnd *pBook) const
{
	if(!pBook) return CString(_T(""));
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		CEPubBook* pEpub = (CEPubBook *)pBook;
		if(pEpub)
			return pEpub->GetVersion();
	}
	return CString(_T(""));
}

/**
 * @brief	å�� ǥ���� �����´�.
 * @param	pBook	: å�� ǥ���� ������ å�� �ν��Ͻ� ������
 * @return	å�� Ÿ��Ʋ
 */
const CString		IN3WebkitEngine::GetTitle(CWnd *pBook) const
{
	if(!pBook) return CString(_T(""));
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		CEPubBook* pEpub = (CEPubBook *)pBook;
		if(pEpub) 
			return pEpub->GetTitle();
	}
	return CString(_T(""));
}

/**
 * @brief	���ڸ��� ��ȯ�Ѵ�.
 * @param	pBook	: ���ڸ��� ������ å�� �ν��Ͻ� ������
 * @return	å�� ���ڸ�
 */
const CString		IN3WebkitEngine::GetCreator(CWnd *pBook) const
{
	if(!pBook) return CString(_T(""));
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		CEPubBook* pEpub = (CEPubBook *)pBook;
		if(pEpub) 
			return pEpub->GetCreator();
	}
	return CString(_T(""));
}

/**
 * @brief	å�� ���ǻ縦 ��ȯ�Ѵ�.
 * @param	pBook	: å�� ���ǻ縦 ������ å�� �ν��Ͻ� ������
 * @return	���ǻ��
 */
const CString		IN3WebkitEngine::GetPublisher(CWnd *pBook) const
{
	if(!pBook) return CString(_T(""));
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		CEPubBook* pEpub = (CEPubBook *)pBook;
		if(pEpub) 
			return pEpub->GetPublisher();
	}
	return CString(_T(""));
}

/**
 * @brief	å�� ������ ��ȯ�Ѵ�.
 * @param	pBook	: å�� ������ ������ å�� �ν��Ͻ� ������
 * @return	����
 */
const CString		IN3WebkitEngine::GetDesc(CWnd *pBook) const
{
	if(!pBook) return CString(_T(""));
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		CEPubBook* pEpub = (CEPubBook *)pBook;
		if(pEpub) 
			return pEpub->GetDesc();
	}
	return CString(_T(""));
}

/**
 * @brief	å�� Ŀ�� �̹��� ��θ� ��ȯ�Ѵ�.
 * @param	pBook	: å�� Ŀ�� �̹��� ��θ� ������ å�� �ν��Ͻ� ������
 * @return	Ŀ�� �̹��� ���
 */
const CString		IN3WebkitEngine::GetCoverHref(CWnd *pBook) const
{
	if(!pBook) return CString(_T(""));
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		CEPubBook* pEpub = (CEPubBook *)pBook;
		if(pEpub) 
			return pEpub->GetCoverHref();
	}
	return CString(_T(""));
}

#if SUPPORT_THUMBNAIL_PATH
	// thumbnail path �� �������� �����ϴ� ���, unzip �� epub ��� �ȿ� thumbnail ������ ����� �ش� ��θ� ��ȯ�Ѵ�.
	// �׷��� �������� ������ ��θ� �����Ͽ� ������ �ʿ䰡 �����Ƿ� �ش� �Լ��� �������ϵ��� ó����.
/**
 * @breif	�泻�� ���
 * @param	pBook	: �泻�� ��θ� ������ å�� �ν��Ͻ� ������
 * @remarks epub �� ���� ��θ� �����Ͽ� ��ȯ��
 * @return	����� ���
 */
const CString		IN3WebkitEngine::GetThumbNailPath(CWnd *pBook) const
{
	if(!pBook) return CString(_T(""));
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		CEPubBook* pEpub = (CEPubBook *)pBook;
		if(pEpub) 
			return pEpub->GetThumbnailPath();
	}
	return CString(_T(""));
}
#endif /*SUPPORT_THUMBNAIL_PATH*/

/**
 * @brief	epub å�� ���� ���� ��θ� ��ȯ�Ѵ�.
 * @param	pBook	: epub å�� ���� ���� ��θ� ������ å�� �ν��Ͻ� ������
 * @return	epub å�� ���� ���� ���
 */
const CString IN3WebkitEngine::GetOriginalEpubPath(CWnd *pBook) const
{
	if(!pBook) return CString(_T(""));
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		CEPubBook* pEpub = (CEPubBook *)pBook;
		if(pEpub) 
			return pEpub->GetOriginalEpubPath();
	}
	return CString(_T(""));
}

/**
 * @brief	å�� ������ ��ȯ�Ѵ�.(Ʈ������)
 * @param	pBook	: å�� ������ ������ å�� �ν��Ͻ� ������
 * @remarks	å�� ������ ��Ʈ �������� �ּҰ��� ��ȯ�Ѵ�.
 * @return	å�� ����
 */
IN3NavPointNode*	IN3WebkitEngine::GetTableOfContent(CWnd *pBook)
{
	if(!pBook) return NULL;
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		CEPubBook* pEpub = (CEPubBook *)pBook;
		if(pEpub) 
			return pEpub->GetTableOfContent();
	}
	return NULL;
}

/**
 * @brief	å���� ������ �̹����� ����Ʈ�� ��ȯ�Ѵ�.
 * @param	pBook	: �̹����� ����Ʈ�� ������ å�� �ν��Ͻ� ������
 * @remarks	å�� ��� �̹����� �����ϴ� ���� �ƴ϶�, �ν� ������ �̹������� �����Ѵ�.
 *			extension �� �̹��� ������ ��ȸ�Ѵ�.
 * @return  true(success)/false(fail)
 */
BOOL IN3WebkitEngine::QueryImageList(CWnd *pBook)
{
	if(!pBook) return FALSE;
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		CEPubBook* pEpub = (CEPubBook *)pBook;
		if(pEpub) 
			return pEpub->QueryImageList();
	}
	return FALSE;
}

/**
 * @brief	å���� ������ ǥ�� ����Ʈ�� ��ȯ�Ѵ�.
 * @param	pBook	: ǥ�� ����Ʈ�� ������ å�� �ν��Ͻ� ������
 * @remarks	å�� ��� ǥ�� �����ϴ� ���� �ƴ϶�, �ν� ������ ǥ���� �����Ѵ�.
 *			extension �� ǥ ������ ��ȸ�Ѵ�.
 * @return  true(success)/false(fail)
 */
BOOL IN3WebkitEngine::QueryTableList(CWnd *pBook)
{
	if(!pBook) return FALSE;
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		CEPubBook* pEpub = (CEPubBook *)pBook;
		if(pEpub) 
			return pEpub->QueryTableList();
	}
	return FALSE;
}



/********************************************************************************
 * page navigation
 ********************************************************************************/
/**
 * @brief	å�� ù �������� �̵��Ѵ�.
 * @param	pBook	: �̵��� å�� �ν��Ͻ� ������
 */
void IN3WebkitEngine::MoveToFirstPage(CWnd *pBook)
{
	TRACEMSG(_T("IN3WebkitEngine::MoveToFirstPage() START"));
	//	
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		CEPubBook *pEpubBook = (CEPubBook *)pBook;
		if(pEpubBook)
			pEpubBook->MoveToPage(1);
	}
	//
	TRACEMSG(_T("IN3WebkitEngine::MoveToFirstPage() END"));
}

/**
 * @brief	å�� ������ �������� �̵��Ѵ�.
 * @param	pBook	: �̵��� å�� �ν��Ͻ� ������
 */
void IN3WebkitEngine::MoveToLastPage(CWnd *pBook)
{
	TRACEMSG(_T("IN3WebkitEngine::MoveToLastPage() START"));
	//	
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		CEPubBook *pEpubBook = (CEPubBook *)pBook;
		if(pEpubBook)
			pEpubBook->MoveToPage(pEpubBook->GetTotalPage());
	}
	//
	TRACEMSG(_T("IN3WebkitEngine::MoveToLastPage() END"));
}

/**
 * @brief	å�� ���� �������� �̵��Ѵ�.
 * @param	pBook	: �̵��� å�� �ν��Ͻ� ������
 */
void IN3WebkitEngine::MoveToNextPage(CWnd *pBook)
{
	TRACEMSG(_T("IN3WebkitEngine::MoveToNextPage() START"));
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		CEPubBook *pEpubBook = (CEPubBook *)pBook;
		if(pEpubBook)
			pEpubBook->MoveToNextPage();
	}
	//
	TRACEMSG(_T("IN3WebkitEngine::MoveToNextPage() END"));
}

/**
 * @brief	å�� ���� �������� �̵��Ѵ�.
 * @param	pBook	: �̵��� å�� �ν��Ͻ� ������
 */
void IN3WebkitEngine::MoveToPrevPage(CWnd *pBook)
{
	TRACEMSG(_T("IN3WebkitEngine::MoveToPrevPage() START"));
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		CEPubBook *pEpubBook = (CEPubBook *)pBook;
		if(pEpubBook)
			pEpubBook->MoveToPrevPage();
	}
	//
	TRACEMSG(_T("IN3WebkitEngine::MoveToPrevPage() END"));
}

/**
 * @brief	������ å�� ������ �������� �̵��Ѵ�.
 * @param	pBook	: �̵��� å
 * @param	page	: �̵��� ������, (å�� ��ü �������� �������� �Ѵ�.)
 */
void IN3WebkitEngine::MoveToPage(CWnd *pBook, unsigned int page)
{
	TRACEMSG(_T("IN3WebkitEngine::MoveToPage() START"));
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		CEPubBook *pEpubBook = (CEPubBook *)pBook;
		if(pEpubBook) 
			pEpubBook->MoveToPage(page);
	}
	//
	TRACEMSG(_T("IN3WebkitEngine::MoveToPage() END"));
}

/**
 * @brief	������ å�� ������ é�ͷ� �̵��Ѵ�.
 * @param	pBook			: �̵��� å
 * @param	chapter			: �̵��� é�� (��Ȯ���� �������� �ε����̴�.)
 * @param	bDirectRight	: ���� ����. TRUE(Right) / FALSE(Left)
 */
void IN3WebkitEngine::MoveToChapter(CWnd *pBook, unsigned int  chapter, BOOL bDirectRight /*= TRUE*/)
{
	TRACEMSG(_T("IN3WebkitEngine::MoveToPage() START"));
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		CEPubBook *pEpubBook = (CEPubBook *)pBook;
		if(pEpubBook) 
			pEpubBook->MoveToChapter(chapter, bDirectRight);
	}
	//
	TRACEMSG(_T("IN3WebkitEngine::MoveToPage() END"));
}

/**
 * @brief	å�� ���Ե� anchor �� �ִ� �������� �̵��Ѵ�.
 * @param	pBook		: �̵��� å
 * @param	fileName	: anchor �� �ִ� �����̸�
 * @param	anchorID	: �̵��� anchor id (= marker id)
 */
void IN3WebkitEngine::MoveToAnchor(CWnd *pBook, CString fileName, CString anchorID)
{
	TRACEMSG(_T("IN3WebkitEngine::MoveToAnchor() START"));
	//
	if(fileName.IsEmpty()) return;

	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		CEPubBook *pEpubBook = (CEPubBook *)pBook;
		if(pEpubBook)
			pEpubBook->MoveToAnchor(fileName, anchorID);
	}
	//
	TRACEMSG(_T("IN3WebkitEngine::MoveToAnchor() END"));
}

/**
 * @brief	FindTextALL() �� �Ͽ� ���� �˻� ��� ��� �߿��� ���� �����Ͽ� ������ �̵��ϴ� ��� ����Ѵ�.
 * @param	pBook		: �̵��� å
 * @param	fileName	: �̵��ϰ��� �ϴ� �˻� ����� �ִ� ���ϸ�
 * @param	keyword		: �˻� Ű����
 * @param	seq			: �˻� ��� ������ ��
 */
void IN3WebkitEngine::MoveToFindResult(CWnd *pBook, CString fileName, CString keyword, int seq)
{
	TRACEMSG(_T("IN3WebkitEngine::MoveToFindResult() START"));
	//
	if(fileName.IsEmpty()) return;

	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		CEPubBook *pEpubBook = (CEPubBook *)pBook;
		if(pEpubBook)
			pEpubBook->MoveToFindResult(fileName, keyword, seq);
	}
	//
	TRACEMSG(_T("IN3WebkitEngine::MoveToFindResult() END"));
}





/********************************************************************************
 * annotation
 ********************************************************************************/
/**
 * @brief	���̶���Ʈ �߰�
 * @param	pBook	: å �ν��Ͻ� ������
 * @param	color	: ���̶���Ʈ Į�� ( ex. "rgba(x,x,x)" �� ���� ���� )
 * @param	hasMemo	: �޸����� �ƴ����� ���� ���°� (true : �޸�, false : �ܼ� ���̶���Ʈ)
 * @return	TRUE(success)/FALSE(fail)
 */
BOOL IN3WebkitEngine::CreateTextHighlight(CWnd *pBook, CString color, BOOL hasMemo /*= FALSE*/)
{
	if(!pBook) return FALSE;
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		TRACEMSG(_T("IN3WebkitEngine::CreateTextHighlight() START"));
		//
		CEPubBook *pEpubBook = (CEPubBook *)pBook;
		if(!pEpubBook){
			TRACEMSG_ERR(_T("CWnd *pBook is NULL.(first parameter)"));
			return FALSE;
		}
		//
		BOOL bRes = TRUE;
		bRes = pEpubBook->CreateTextHighlight(color, hasMemo);
		//
		TRACEMSG(_T("IN3WebkitEngine::CreateTextHighlight() END"));
		//
		return bRes;
	}
	return FALSE;
}

/**
 * @brief	������� �߰�
 * @param	pBook	: å �ν��Ͻ� ������
 * @param	color	: ������� Į�� ( ex. "rgba(x,x,x)" �� ���� ���� )
 * @param	hasMemo	: ����������� �ƴ����� ���� ���°� (true : �޸�, false : �ܼ� �������)
 * @return	TRUE(success)/FALSE(fail)
 */
BOOL IN3WebkitEngine::CreateUnderline(CWnd *pBook, CString color, BOOL hasMemo /*= FALSE*/)
{
	if(!pBook) return FALSE;
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		TRACEMSG(_T("IN3WebkitEngine::CreateUnderline() START"));
		//
		CEPubBook *pEpubBook = (CEPubBook *)pBook;
		if(!pEpubBook){
			TRACEMSG_ERR(_T("CWnd *pBook is NULL.(first parameter)"));
			return FALSE;
		}
		//
		BOOL bRes = TRUE;
		bRes = pEpubBook->CreateUnderline(color, hasMemo);
		//
		TRACEMSG(_T("IN3WebkitEngine::CreateUnderline() END"));
		//
		return bRes;
	}
	return FALSE;
}

/**
 * @brief	annotation ����
 * @param	pBook		: å�� �ν��Ͻ� ������
 * @param	fileName	: annotation�� �߰��� ���ϸ�
 * @param	markerID	: annotation id
 * @return	TRUE(success)/FALSE(fail)
 */
BOOL IN3WebkitEngine::DeleteAnnotation(CWnd *pBook, CString fileName, CString markerID)
{
	if(!pBook) return FALSE;
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		TRACEMSG(_T("IN3WebkitEngine::DeleteTextHgihlight() START"));
		//
		CEPubBook *pEpubBook = (CEPubBook *)pBook;
		if(!pEpubBook){
			TRACEMSG_ERR(_T("CWnd *pBook is NULL.(first parameter)"));
			return FALSE;
		}
		//
		BOOL bRes = TRUE;
		bRes = pEpubBook->DeleteAnnotation(fileName, markerID);
		TRACEMSG(_T("IN3WebkitEngine::DeleteTextHgihlight() END"));
		//
		return bRes;
	}
	return FALSE;
}

/**
 * @brief	annotation(���̶���Ʈ/�޸�/�������) �� �����Ѵ�.
 * @param	pBook		: å�� �ν��Ͻ� ������
 * @param	pAnnotation	: ������ annotaton ����
 * @return	��Ŷ extension ���� annotation ������ ��û�� ��� = TRUE, �׿�= FALSE
 */
BOOL IN3WebkitEngine::ModifyAnnotation(CWnd *pBook, IN3Annotation *pAnnotation)
{
	if(!pBook) return FALSE;
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		TRACEMSG(_T("IN3WebkitEngine::ModifyTextHighlight() START"));
		//
		CEPubBook *pEpubBook = (CEPubBook *)pBook;
		if(!pEpubBook){
			TRACEMSG_ERR(_T("CWnd *pBook is NULL.(first parameter)"));
			return FALSE;
		}
		//
		BOOL bRes = TRUE;
		bRes = pEpubBook->ModifyAnnotation(pAnnotation);
		TRACEMSG(_T("IN3WebkitEngine::ModifyTextHighlight() END"));
		//
		return bRes;
	}
	return FALSE;
}

/**
 * @brief	annotation �޴��� ����ֱ� ���� ���� ����ڰ� ������ ������ ��ȸ�ϴ� �Լ�
 * @param	pBook		: å�� �ν��Ͻ� ������
 * @return	��Ŷ extension ���� ����ڰ� ������ ������ ��ȸ�� ��û�� ��� = TRUE, �׿�= FALSE
 */
BOOL IN3WebkitEngine::QueryAnnotationToolbarPosition(CWnd *pBook)
{
	if(!pBook) return FALSE;
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		TRACEMSG(_T("IN3WebkitEngine::QueryAnnotationToolbarPosition() START"));
		//
		CEPubBook *pEpubBook = (CEPubBook *)pBook;
		if(!pEpubBook){
			TRACEMSG_ERR(_T("CWnd *pBook is NULL.(first parameter)"));
			return FALSE;
		}
		//
		BOOL bRes = TRUE;
		bRes = pEpubBook->QueryAnnotationToolbarPosition();
		TRACEMSG(_T("IN3WebkitEngine::QueryAnnotationToolbarPosition() END"));
		//
		return bRes;
	}
	return FALSE;
}



/********************************************************************************
 * annotation
 ********************************************************************************/
/**
 * @brief	���� ���̴� �������� �ϸ�ũ�� �����Ѵ�.
 * @param	pBook		: å�� �ν��Ͻ� ������
 * @return	��Ŷ extension ���� �ϸ�ũ �߰��� ��û�� ��� = TRUE, �׿�= FALSE 
 */
BOOL IN3WebkitEngine::SetBookmark(CWnd *pBook)
{
	if(!pBook) return FALSE;
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		TRACEMSG(_T("IN3WebkitEngine::SetBookmark() START"));
		//
		CEPubBook *pEpubBook = (CEPubBook *)pBook;
		if(!pEpubBook){
			TRACEMSG_ERR(_T("CWnd *pBook is NULL.(first parameter)"));
			return FALSE;
		}
		//
		BOOL bRes = TRUE;
		bRes = pEpubBook->SetBookmark();
		//
		TRACEMSG(_T("IN3WebkitEngine::SetBookmark() END"));
		//
		return bRes;
	}
	return FALSE;
}

/**
 * @brief	���� ���̴� ������ �ϸ�ũ���� �����Ѵ�.
 * @param	pBook		: å�� �ν��Ͻ� ������
 * @return	��Ŷ extension ���� �ϸ�ũ ������ ��û�� ��� = TRUE, �׿�= FALSE 
 */
BOOL IN3WebkitEngine::ClearBookmark(CWnd *pBook)
{
	if(!pBook) return FALSE;
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		TRACEMSG(_T("IN3WebkitEngine::ClearBookmark() START"));
		//
		CEPubBook *pEpubBook = (CEPubBook *)pBook;
		if(!pEpubBook){
			TRACEMSG_ERR(_T("CWnd *pBook is NULL.(first parameter)"));
			return FALSE;
		}
		//
		BOOL bRes = TRUE;
		bRes = pEpubBook->ClearBookmark();
		//
		TRACEMSG(_T("IN3WebkitEngine::ClearBookmark() END"));
		//
		return bRes;
	}
	return FALSE;
}

/**
 * @brief	Ư�� �ϸ�ũ�� �����Ѵ�.
 * @param	pBook		: å�� �ν��Ͻ� ������
 * @param	pBookmark	: ������ �ϸ�ũ ����
 * @return	��Ŷ extension ���� �ϸ�ũ ������ ��û�� ��� = TRUE, �׿�= FALSE 
 */
BOOL IN3WebkitEngine::DeleteBookmark(CWnd *pBook, IN3Bookmark *pBookmark)
{
	if(!pBook) return FALSE;
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		TRACEMSG(_T("IN3WebkitEngine::DeleteBookmark() START"));
		//
		CEPubBook *pEpubBook = (CEPubBook *)pBook;
		if(!pEpubBook){
			TRACEMSG_ERR(_T("CWnd *pBook is NULL.(first parameter)"));
			return FALSE;
		}
		//
		BOOL bRes = TRUE;
		bRes = pEpubBook->DeleteBookmark(pBookmark);
		//
		TRACEMSG(_T("IN3WebkitEngine::DeleteBookmark() END"));
		//
		return bRes;
	}
	return FALSE;
}

/**
 * @brief	Ư�� �ϸ�ũ���� �����Ѵ�.
 * @param	pBook		: å�� �ν��Ͻ� ������
 * @param	bookmarks	: ������ �ϸ�ũ ���� ����Ʈ
 * @return	��Ŷ extension ���� �ϸ�ũ ������ ��û�� ��� = TRUE, �׿�= FALSE 
 */
BOOL IN3WebkitEngine::DeleteBookmarkList(CWnd *pBook, std::vector<IN3Bookmark *> bookmarks)
{
	if(!pBook) return FALSE;
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		TRACEMSG(_T("IN3WebkitEngine::DeleteBookmarkList() START"));
		//
		CEPubBook *pEpubBook = (CEPubBook *)pBook;
		if(!pEpubBook){
			TRACEMSG_ERR(_T("CWnd *pBook is NULL.(first parameter)"));
			return FALSE;
		}
		//
		BOOL bRes = TRUE;
		bRes = pEpubBook->DeleteBookmarkList(bookmarks);
		//
		TRACEMSG(_T("IN3WebkitEngine::DeleteBookmarkList() END"));
		//
		return bRes;
	}
	return FALSE;
}



/********************************************************************************
 * Find Text
 ********************************************************************************/
/**
 * @brief	���� �˻��� �Ѵ�.
 * @remarks �˻� ����� ����Ʈ�� ���޹޴´�.
 * @param	pBook		: å�� �ν��Ͻ� ������
 * @param	keyword	: �˻� ���ڿ�
 * @return	��Ŷ extension ���� ���� �˻��� ��û�� ��� = TRUE, �׿�= FALSE 
 */
BOOL IN3WebkitEngine::FindTextALL(CWnd *pBook, CString keyword)
{
	if(!pBook) return FALSE;
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		TRACEMSG(_T("IN3WebkitEngine::FindTextALL() START"));
		//
		CEPubBook *pEpubBook = (CEPubBook *)pBook;
		if(!pEpubBook){
			TRACEMSG_ERR(_T("CWnd *pBook is NULL.(first parameter)"));
			return FALSE;
		}
		//
		BOOL bRes = TRUE;
		bRes = pEpubBook->FindTextALL(keyword);
		//
		TRACEMSG(_T("IN3WebkitEngine::FindTextALL() END"));
		//
		return bRes;
	}
	return FALSE;
}

/**
 * @brief	���������� �˻��� �Ѵ�. ���� ���� ����
 * @remarks �˻� �� ��� ȭ������ �ٷ� �̵��Ѵ�.
 * @param	pBook		: å�� �ν��Ͻ� ������
 * @param	keyword		: �˻� ���ڿ�
 * @return	��Ŷ extension ���� ���� �˻��� ��û�� ��� = TRUE, �׿�= FALSE 
 */
BOOL IN3WebkitEngine::FindTextNext(CWnd *pBook, CString keyword)
{
	if(!pBook) return FALSE;
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		TRACEMSG(_T("IN3WebkitEngine::FindTextNext() START"));
		//
		CEPubBook *pEpubBook = (CEPubBook *)pBook;
		if(!pEpubBook){
			TRACEMSG_ERR(_T("CWnd *pBook is NULL.(first parameter)"));
			return FALSE;
		}
		//
		BOOL bRes = TRUE;
		bRes = pEpubBook->FindTextAndMove(keyword, true);
		//
		TRACEMSG(_T("IN3WebkitEngine::FindTextNext() END"));
		//
		return bRes;
	}
	return FALSE;
}

/**
 * @brief	���������� �˻��� �Ѵ�. ���� ���� ����
 * @remarks �˻� �� ��� ȭ������ �ٷ� �̵��Ѵ�.
 * @param	pBook		: å�� �ν��Ͻ� ������
 * @param	keyword		: �˻� ���ڿ�
 * @return	��Ŷ extension ���� ���� �˻��� ��û�� ��� = TRUE, �׿�= FALSE 
 */
BOOL IN3WebkitEngine::FindTextPrev(CWnd *pBook, CString keyword)
{
	if(!pBook) return FALSE;
	//
	if(pBook->IsKindOf(RUNTIME_CLASS(CEPubBook)))
	{
		TRACEMSG(_T("IN3WebkitEngine::FindTextPrev() START"));
		//
		CEPubBook *pEpubBook = (CEPubBook *)pBook;
		if(!pEpubBook){
			TRACEMSG_ERR(_T("CWnd *pBook is NULL.(first parameter)"));
			return FALSE;
		}
		//
		BOOL bRes = TRUE;
		bRes = pEpubBook->FindTextAndMove(keyword, false);
		//
		TRACEMSG(_T("IN3WebkitEngine::FindTextPrev() END"));
		//
		return bRes;
	}
	return FALSE;
}