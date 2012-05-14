#include "StdAfx.h"
#include "IN3WebkitEngine.h"
#include "IN3WebkitDefine.h"

#include "EPubBook.h"

/**
 * @brief	생성자
 */

IN3WebkitEngine::IN3WebkitEngine(void)
{
	// unzip 경로 설정
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
 * @brief	소멸자
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
 * @brief	Epub Open 시에 Load 되는 라이브러리들을 임의 Load 한다.
 * @remarks	최초 Epub 을 열 때에는 많은 Library 들이 Load 되어 속도 저하의 원인이 된다.
 *			따라서 미리 라이브러리들을 Load 해 두면 Epub Open 시에 소요되는 속도를 줄일 수 있다.
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
 * @brief	책을 연다.
 * @param	pParentWnd		: 생성된 책 객체의 ParentWnd 포인터
 * @param	lpszPathName	: 원본 책의 절대 경로
 * @param	lpszDescPath	: unzip 된 절대 경로
 * @return	생성된 책 객체
 */
CWnd* IN3WebkitEngine::Open(CWnd *pParentWnd, LPCTSTR lpszFilePath, LPCTSTR lpszDescPath)
{
	TRACEMSG(_T("IN3WebkitEngine::Open() START"));
	//
#if !SUPPORT_VIEW_POOL_MANAGER
	// 동시에 열 수 있는 책의 개수를 제한
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
 * @brief	엔진에서 임의의 책을 제거한다. 
 * @remarks	엔진에서 가지고 있는 책 리스트에서 제거
 * @param	lpszFilePath	: 제거할 책의 원본 소스 경로
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
 * @brief	책을 닫는다.
 * @remarks	엔진에서 가지고 있는 책 리스트에서 제거
 * @param	pBook	: 제거할 책
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
 * @brief	책에 필요한 뷰를 생성하는 함수
 * @param	pBook	: 뷰를 생성할 책 인스턴스 포인터
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
			// 엔진에서
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
 * @brief	책에 필요한 뷰를 생성하는 함수
 * @remarks	지정된 형식의 뷰를 생성한다.
 * @param	pBook			: 뷰를 생성할 책 인스턴스 포인터
 * @param	pRuntimeClass	: 생성하고자 하는 뷰의 런타임 클래스
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
 * @brief	epub 의 레이아웃을 재정렬 한다.
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
 * @brief	epub 의 레이아웃을 재정렬 한다.
 * @param	pBook	: 레이아웃을 정리할 책의 인스턴스 포인터
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
 * @brief	epub 을 Reload 한다.
 * @param	pBook	: Reload 할 책의 인스턴스 포인터
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
 * @brief	UI단에서 Epub display 관련하여 관리하고 있는 id
 * @remarks	tab control 에 Epub 을 붙이는 경우 tab 의 id를 설정한다.
 * @param	pBook	: 책의 인스턴스 포인터
 * @param	id		: UI 단에 서 관리하는 아이디
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
 * @brief	UI단에서 각 epub 별로 고유하게 관리하고 있는 id값을 가져온다
 * @remarks	tab control 에 Epub 을 붙이는 경우 tab 의 id값과 같다.
 * @param	lpszFileName	: ui 단에서 관리하는 id를 가져올 책의 원본 경로
 * @return	UI 단에 서 관리하는 아이디
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
 * @brief	UI단에서 각 epub 별로 고유하게 관리하고 있는 id값을 가져온다
 * @remarks	tab control 에 Epub 을 붙이는 경우 tab 의 id값과 같다.
 * @param	pBook	: ui 단에서 관리하는 id를 가져올 책의 인스턴스 포인터
 * @return	UI 단에 서 관리하는 아이디
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
 * @brief	tab control 을 사용하지 않고, UI 단에서 직접 viewing 을 제어하는 경우, 임의의 책만 display 하고 나머지 책은 hide 시키기 위해 사용하는 api
 * @param	lpszFileName	: viewing 할 책의 원본 절대 경로
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
	// 모든 책들을 hide 시킨다.
	for(EBookList::iterator iter = m_bookList.begin(); iter != m_bookList.end(); iter++)
	{
		CEPubBook* pBook = (CEPubBook*)iter->second;
		if(pBook)
			pBook->ShowWindow(SW_HIDE);
	}

	// url 정보로 show 할 책을 찾는다
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
 * @brief	tab control 을 사용하지 않고, UI 단에서 직접 viewing 을 제어하는 경우, 임의의 책만 display 하고 나머지 책은 hide 시키기 위해 사용하는 api
 * @param	pBook	: display 할 책의 인스턴스 포인터
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

		// 모든 책들을 hide 시킨다.
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
 * @brief	fixed 컨텐츠의 뷰를 확대한다.
 * @param	pBook	: 확대할 책의 인스턴스 포인터
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
 * @brief	fixed 컨텐츠의 뷰를 축소한다.
 * @param	pBook	: 축소할 책의 인스턴스 포인터
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
 * @brief	fixed 컨텐츠의 뷰를 현재 윈도우의 크기에 맞춰 보여준다.
 * @param	pBook	: 책의 인스턴스 포인터
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
 * @brief	현재 보여지는 페이지 값을 반환한다.
 * @param	pBook	: 현재 보여지는 페이지 값을 가져올 책의 인스턴스 포인터
 * @return	현재 보여지는 페이지 값
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
 * @brief	책의 총 페이지 수를 반환한다.
 * @param	pBook	: 책의 총 페이지 수를 가져올 책의 인스턴스 포인터
 * @return	책의 총 페이지 수
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
 * @brief	현재보고 있는 챕터의 인덱스 값을 반환한다. (spine index 값과 일치함)
 * @param	pBook	: 챕터의 인덱스 값을 가져올 책의 인스턴스 포인터
 * @return	현재 보여지고 있는 spine 파일의 인덱스 값.	
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
 * @brief	책의 spine 수를 반환한다.
 * @param	pBook	: 책의 spine 수를 가져올 책의 인스턴스 포인터
 * @return	spine 개수
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
 * @brief	임의의 화면을 임의의 크기로 캡쳐한다.
 * @remarks fixed layout() 에서만 사용 가능
 * @param	pBook	: 화면을 임의의 크기로 캡쳐할 책의 인스턴스 포인터
 * @param	[in] page		: 캡쳐할 페이지
 * @param	[in] width		: 캡쳐 이미지의 가로 픽셀 값
 * @param	[in] height		: 캡쳐 이미지의 세로 픽셀 값
 * @param	[out] bitmap	: 캡쳐 이미지
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
 * @brief	페이지를 초기화할때 필요한 값들을 설정한다.
 * @remarks	폰트크기, 폰트, 폰트색, 배경색의 값을 설정한다.
 * @param	pBook		: 초기화 시 설정 값들을 적용할 책의 인스턴스 포인터
 * @param	fontFamily	: 폰트명
 * @param	fontSize	: 폰트 크기
 * @param	fontColor	: 폰트색
 * @param	bgColor	: 배경색
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
			// fixed 인 경우에는 적용하지 않는다.
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
 * @brief	annotation(하이라이트/언더라인/메모) 를 일괄적용한다.
 * @remarks	페이지 로드 후에 annotation 을 일괄 적용해야 한다.
 * @param	pBook		: annotation들을 적용할 책의 인스턴스 포인터
 * @param	annotations	: 적용해야 하는 annotation 리스트
 * @return	웹킷 extension 으로 annotation 일괄 적용을 요청한 경우 = TRUE, 그외= FALSE
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
 * @brief	북마크들을 일괄적용한다.
 * @param	pBook		: 북마크들을 적용할 책의 인스턴스 포인터
 * @param	bookmarks	: 일괄 적용할 bookmark 리스트
 * @return	웹킷 extension 으로 북마크 일괄 적용을 요청한 경우 = TRUE, 그외= FALSE
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
 * @brief	open 된 epub 파일들의 내부 저장 경로를 반환한다
 * @return	open 된 epub 파일들의 내부 저장 경로
 */	
const CString IN3WebkitEngine::GetEpubMainPath(void) const
{
	return m_epubMainPath;
}

/**
 * @brief	현재 페이지 모드(양면/단면 보기)를 반환한다.
 * @param	pBook	: 양면보기/단면보기를 확인할 책의 인스턴스 포인터
 * @return	페이지 모드 (1=단면, 2=양면)
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
 * @brief	사용자가 설정한 폰트 사이즈를 반환한다.
 * @param	pBook	: 사용자가 설정한 폰트 사이즈를 확인할 책의 인스턴스 포인터
 * @return	사용자가 설정한 폰트 사이즈
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
 * @brief	사용자가 설정한 폰트명을 반환한다.
 * @param	pBook	: 사용자가 설정한 폰트명을 확인할 책의 인스턴스 포인터
 * @return	사용자가 설정한 폰트명
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
 * @brief	사용자가 설정한 폰트색을 반환한다.
 * @param	pBook	: 사용자가 설정한 폰트색을 확인할 책의 인스턴스 포인터
 * @return	사용자가 설정한 폰트색
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
 * @brief	사용자가 설정한 배경색을 반환한다.
 * @param	pBook	: 사용자가 설정한 배경색을 확인할 책의 인스턴스 포인터
 * @return	사용자가 설정한 배경색
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
 * @brief	본문 아래영역에 현재 페이지 값을 표시할지 여부를 설정한다.
 * @param	pBook	: 현재 페이지 값을 표시를 설정할 책의 인스턴스 포인터
 * @param	bDisplay	: true(본문 밑에 페이지 번호 표시) / false(페이지 번호 표시 안함)
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
 * @brief	본문 아래영역에 현재 페이지 값을 표시할지 여부를 설정한다.
 * @param	bDisplay	: true(본문 밑에 페이지 번호 표시) / false(페이지 번호 표시 안함)
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
 * @brief	페이지 모드를 설정한다. (단면/양면)
 * @param	pBook	: 페이지 모드를 설정할 책의 인스턴스 포인터
 * @param	mode	: 1 = 단면, 2 = 양면
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
			// 책이 지정되어 있지 않으면 현재 열려있는 모든 책에 적용한다.
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
 * @brief	폰트를 설정한다.
 * @param	pBook			: 폰트를 변경할 책의 인스턴스 포인터
 * @param	fontFamilyName	: 변경할 폰트
 * @remarks 설정되는 폰트는 엔진 내부에서 가지고 있다가 ReLayout() 시 변경된 폰트를 적용하도록 한다.
 *			따라서 아래 함수는 단순히 엔진 내부에 폰트를 설정하는 기능만 한다.
 *			즉 변경 사항을 적용하기 위해선 사용자가 원하는 시점에 ReLayout() 함수를 호출하여 줘야 한다.
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
 * @brief	폰트 사이즈를 설정한다.
 * @param	pBook		: 폰트 크기를 변경할 책의 인스턴스 포인터
 * @param	fontSize	: 변경할 폰트 사이즈
 * @remarks 설정되는 폰트 사이즈는 엔진 내부에서 가지고 있다가 ReLayout() 시 변경된 폰트 사이즈를 적용하도록 한다.
 *			따라서 아래 함수는 단순히 엔진 내부에 폰트 사이즈를 설정하는 기능만 한다.
 *			즉 변경 사항을 적용하기 위해선 사용자가 원하는 시점에 ReLayout() 함수를 호출하여 줘야 한다.
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
 * @brief	폰트 색상을 설정한다.
 * @param	pBook		: 폰트 색상을 변경할 책의 인스턴스 포인터
 * @param	strColor	: 변경할 폰트 색상
 * @remarks 설정되는 폰트 색상은 엔진 내부에서 가지고 있다가 ReLayout() 시 변경된 폰트 색상을 적용하도록 한다.
 *			따라서 아래 함수는 단순히 엔진 내부에 폰트 색상을 설정하는 기능만 한다.
 *			즉 변경 사항을 적용하기 위해선 사용자가 원하는 시점에 ReLayout() 함수를 호출하여 줘야 한다.
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
 * @brief	배경색을 설정한다.
 * @param	pBook		: 배경색을 변경할 책의 인스턴스 포인터
 * @param	strColor	: 변경할 배경색
 * @remarks 설정되는 배경색은 엔진 내부에서 가지고 있다가 ReLayout() 시 변경된 배경색을 적용하도록 한다.
 *			따라서 아래 함수는 단순히 엔진 내부에 배경색을 설정하는 기능만 한다.
 *			즉 변경 사항을 적용하기 위해선 사용자가 원하는 시점에 ReLayout() 함수를 호출하여 줘야 한다.
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
 * @brief	폰트색, 배경색을 적용한다.
 * @remarks	폰트색, 배경색을 변경하는 것은 별도로 레이아웃을 재정리 할 필요는 없으므로 함수 호출 즉시 바로 반영한다.
 * @param	pBook			: 폰트색과 배경색을 변경할 책의 인스턴스 포인터
 * @param	foregroundClr	: 폰트색
 * @param	backgroundClr	: 배경색
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
 * @brief	Epub 이 open 되어 있는지 확인하는 함수
 * @param	lpszFileName	: 원본 책의 절대 경로
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
 * @brief	책이 정상적으로 Load 되었는지 학인하는 함수
 * @remarks	현재 엔진에 열려있는 모든 책을 검사한다.
 * @return	TRUE(로드 됨)/FALSE(로드 안됨)
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
 * @brief	책이 정상적으로 Load 되었는지 학인하는 함수
 * @param	lpszFileName	: 원본 책의 절대 경로
 * @return	TRUE(로드 됨)/FALSE(로드 안됨)
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
 * @brief	책이 정상적으로 Load 되었는지 학인하는 함수
 * @param	pBook	: Load 되었는지 확인할 책의 인스턴스 포인터
 * @return	TRUE(로드 됨)/FALSE(로드 안됨)
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
 * @brief	Epub 이 Fixed layout 인지 확인하는 함수
 * @param	pBook	: Fixed layout 인지 확인할 책의 인스턴스 포인터
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
 * @brief	책의 uuid 를 가져온다.
 * @param	pBook	: uuid 를 가져올 책의 인스턴스 포인터
 * @return	책의 uuid
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
 * @brief	책의 버전을 반환한다.
 * @param	pBook	: 책의 버전을 가져올 책의 인스턴스 포인터
 * @return	책의 버전 3 or 2
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
 * @brief	책의 표지를 가져온다.
 * @param	pBook	: 책의 표지를 가져올 책의 인스턴스 포인터
 * @return	책의 타이틀
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
 * @brief	저자명을 반환한다.
 * @param	pBook	: 저자명을 가져올 책의 인스턴스 포인터
 * @return	책의 저자명
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
 * @brief	책의 출판사를 반환한다.
 * @param	pBook	: 책의 출판사를 가져올 책의 인스턴스 포인터
 * @return	출판사명
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
 * @brief	책의 설명을 반환한다.
 * @param	pBook	: 책의 설명을 가져올 책의 인스턴스 포인터
 * @return	설명
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
 * @brief	책의 커버 이미지 경로를 반환한다.
 * @param	pBook	: 책의 커버 이미지 경로를 가져올 책의 인스턴스 포인터
 * @return	커버 이미지 경로
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
	// thumbnail path 를 엔진에서 제공하는 경우, unzip 된 epub 경로 안에 thumbnail 폴더를 만들어 해당 경로를 반환한다.
	// 그러나 엔진에서 섬네일 경로를 지정하여 제공할 필요가 없으므로 해당 함수는 미지원하도록 처리함.
/**
 * @breif	썸내일 경로
 * @param	pBook	: 썸내일 경로를 가져올 책의 인스턴스 포인터
 * @remarks epub 에 임의 경로를 설정하여 반환함
 * @return	썸네일 경로
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
 * @brief	epub 책의 원본 절대 경로를 반환한다.
 * @param	pBook	: epub 책의 원본 절대 경로를 가져올 책의 인스턴스 포인터
 * @return	epub 책의 원본 절대 경로
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
 * @brief	책의 목차를 반환한다.(트리구조)
 * @param	pBook	: 책의 목차를 가져올 책의 인스턴스 포인터
 * @remarks	책의 목차의 루트 데이터의 주소값을 반환한다.
 * @return	책의 목차
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
 * @brief	책에서 추출한 이미지의 리스트를 반환한다.
 * @param	pBook	: 이미지의 리스트를 가져올 책의 인스턴스 포인터
 * @remarks	책의 모든 이미지를 추출하는 것이 아니라, 인식 가능한 이미지만을 추출한다.
 *			extension 에 이미지 추출을 조회한다.
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
 * @brief	책에서 추출한 표의 리스트를 반환한다.
 * @param	pBook	: 표의 리스트를 가져올 책의 인스턴스 포인터
 * @remarks	책의 모든 표를 추출하는 것이 아니라, 인식 가능한 표만을 추출한다.
 *			extension 에 표 추출을 조회한다.
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
 * @brief	책의 첫 페이지로 이동한다.
 * @param	pBook	: 이동할 책의 인스턴스 포인터
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
 * @brief	책의 마지막 페이지로 이동한다.
 * @param	pBook	: 이동할 책의 인스턴스 포인터
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
 * @brief	책의 다음 페이지로 이동한다.
 * @param	pBook	: 이동할 책의 인스턴스 포인터
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
 * @brief	책의 이전 페이지로 이동한다.
 * @param	pBook	: 이동할 책의 인스턴스 포인터
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
 * @brief	임의의 책을 임의의 페이지로 이동한다.
 * @param	pBook	: 이동할 책
 * @param	page	: 이동할 페이지, (책의 전체 페이지를 기준으로 한다.)
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
 * @brief	임의의 책을 임의의 챕터로 이동한다.
 * @param	pBook			: 이동할 책
 * @param	chapter			: 이동할 챕터 (정확히는 스파인의 인덱스이다.)
 * @param	bDirectRight	: 진행 방향. TRUE(Right) / FALSE(Left)
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
 * @brief	책에 삽입된 anchor 가 있는 페이지로 이동한다.
 * @param	pBook		: 이동할 책
 * @param	fileName	: anchor 가 있는 파일이름
 * @param	anchorID	: 이동할 anchor id (= marker id)
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
 * @brief	FindTextALL() 을 하여 얻은 검색 결과 목록 중에서 임의 선택하여 페이지 이동하는 경우 사용한다.
 * @param	pBook		: 이동할 책
 * @param	fileName	: 이동하고자 하는 검색 결과가 있는 파일명
 * @param	keyword		: 검색 키워드
 * @param	seq			: 검색 결과 시퀀스 값
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
 * @brief	하이라이트 추가
 * @param	pBook	: 책 인스턴스 포인터
 * @param	color	: 하이라이트 칼라 ( ex. "rgba(x,x,x)" 와 같은 형태 )
 * @param	hasMemo	: 메모인지 아닌지에 대한 상태값 (true : 메모, false : 단순 하이라이트)
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
 * @brief	언더라인 추가
 * @param	pBook	: 책 인스턴스 포인터
 * @param	color	: 언더라인 칼라 ( ex. "rgba(x,x,x)" 와 같은 형태 )
 * @param	hasMemo	: 언더라인인지 아닌지에 대한 상태값 (true : 메모, false : 단순 언더라인)
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
 * @brief	annotation 삭제
 * @param	pBook		: 책의 인스턴스 포인터
 * @param	fileName	: annotation이 추가된 파일명
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
 * @brief	annotation(하이라이트/메모/언더라인) 을 수정한다.
 * @param	pBook		: 책의 인스턴스 포인터
 * @param	pAnnotation	: 수정할 annotaton 정보
 * @return	웹킷 extension 으로 annotation 수정을 요청한 경우 = TRUE, 그외= FALSE
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
 * @brief	annotation 메뉴를 띄워주기 위해 현재 사용자가 선택한 영역을 조회하는 함수
 * @param	pBook		: 책의 인스턴스 포인터
 * @return	웹킷 extension 으로 사용자가 선택한 영역을 조회를 요청한 경우 = TRUE, 그외= FALSE
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
 * @brief	현재 보이는 페이지에 북마크를 삽입한다.
 * @param	pBook		: 책의 인스턴스 포인터
 * @return	웹킷 extension 으로 북마크 추가를 요청한 경우 = TRUE, 그외= FALSE 
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
 * @brief	현재 보이는 쳅터의 북마크들을 삭제한다.
 * @param	pBook		: 책의 인스턴스 포인터
 * @return	웹킷 extension 으로 북마크 삭제를 요청한 경우 = TRUE, 그외= FALSE 
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
 * @brief	특정 북마크을 삭제한다.
 * @param	pBook		: 책의 인스턴스 포인터
 * @param	pBookmark	: 삭제할 북마크 정보
 * @return	웹킷 extension 으로 북마크 삭제를 요청한 경우 = TRUE, 그외= FALSE 
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
 * @brief	특정 북마크들을 삭제한다.
 * @param	pBook		: 책의 인스턴스 포인터
 * @param	bookmarks	: 삭제할 북마크 정보 리스트
 * @return	웹킷 extension 으로 북마크 삭제를 요청한 경우 = TRUE, 그외= FALSE 
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
 * @brief	본문 검색을 한다.
 * @remarks 검색 결과를 리스트로 전달받는다.
 * @param	pBook		: 책의 인스턴스 포인터
 * @param	keyword	: 검색 문자열
 * @return	웹킷 extension 으로 본문 검색을 요청한 경우 = TRUE, 그외= FALSE 
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
 * @brief	본문내용을 검색을 한다. 진행 방향 다음
 * @remarks 검색 시 결과 화면으로 바로 이동한다.
 * @param	pBook		: 책의 인스턴스 포인터
 * @param	keyword		: 검색 문자열
 * @return	웹킷 extension 으로 본문 검색을 요청한 경우 = TRUE, 그외= FALSE 
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
 * @brief	본문내용을 검색을 한다. 진행 방향 이전
 * @remarks 검색 시 결과 화면으로 바로 이동한다.
 * @param	pBook		: 책의 인스턴스 포인터
 * @param	keyword		: 검색 문자열
 * @return	웹킷 extension 으로 본문 검색을 요청한 경우 = TRUE, 그외= FALSE 
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