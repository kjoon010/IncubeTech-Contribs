
// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "InCubePublisherViewer.h"

#include "MainFrm.h"
#include "IN3WebkitDefine.h"
#include "IN3Annotation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_EXITSIZEMOVE()
	ON_WM_KEYDOWN()

	ON_MESSAGE(WM_LODEAD_EPUB, OnEpubLoaded)
	ON_MESSAGE(WM_BOOKMARK_STATUS, OnBookmarkStatus)
	ON_MESSAGE(WM_ANNOTATION_ADDED, OnAnnotationAdded)
	ON_MESSAGE(WM_BOOKMARK_ADDED, OnBookmarkAdded)
	ON_MESSAGE(WM_CONTEXTMENU_EPUB, OnContextMenu)
	ON_MESSAGE(WM_ANNOTATIONMENU_EPUB, OnAnnotationMenu)
	ON_MESSAGE(WM_ANNOTATION_SELECTED, OnAnnotationSelected)
	ON_MESSAGE(WM_MEMO_ICON_SELECTED, OnMemoIconSelected)
	ON_WM_SIZE()
	ON_COMMAND(ID_MOVE_FIRST_PAGE, &CMainFrame::OnMoveFirstPage)
	ON_COMMAND(ID_MOVE_PREV_PAGE, &CMainFrame::OnMovePrevPage)
	ON_COMMAND(ID_MOVE_NEXT_PAGE, &CMainFrame::OnMoveNextPage)
	ON_COMMAND(ID_MOVE_LAST_PAGE, &CMainFrame::OnMoveLastPage)
	ON_COMMAND(ID_ADD_BOOKMARK, &CMainFrame::OnAddBookmark)
	ON_COMMAND(ID_ADD_HIGHLIGHT, &CMainFrame::OnAddHighlight)
	ON_COMMAND(ID_ADD_MEMO, &CMainFrame::OnAddMemo)
	ON_COMMAND(ID_DELETE_ANNOTATION, &CMainFrame::OnDeleteAnnotation)
	ON_COMMAND(ID_CHANGE_ANNOTATION_COLOR_RED, &CMainFrame::OnChangeAnnotationColorRed)
	ON_COMMAND(ID_CHANGE_ANNOTATION_COLOR_GREEN, &CMainFrame::OnChangeAnnotationColorGreen)
	ON_COMMAND(ID_CHANGE_ANNOTATION_COLOR_BLUE, &CMainFrame::OnChangeAnnotationColorBlue)
	ON_COMMAND(ID_CONTEXT_SETTING, &CMainFrame::OnSettingViewing)
	ON_COMMAND(ID_SINGLE_DISPLAY_MODE, &CMainFrame::OnSingleDisplayMode)
	ON_COMMAND(ID_DUAL_DISPLAY_MODE, &CMainFrame::OnDualDisplayMode)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
	:m_pWndLibraryView(NULL)
	,m_ViewMode(READING_MODE)
	,m_pBook(NULL)
	,m_pAnnotation(NULL)
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2005);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// BOOL bNameValid;
	// 보관된 값에 따라 비주얼 관리자 및 스타일을 설정합니다.
	OnApplicationLook(theApp.m_nAppLook);

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("메뉴 모음을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// 메뉴 모음을 활성화해도 포커스가 이동하지 않게 합니다.
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

#if 0
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("도구 모음을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
#endif

	// 사용자 정의 도구 모음 작업을 허용합니다.
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

#if 0
	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
#endif

	// TODO: 도구 모음 및 메뉴 모음을 도킹할 수 없게 하려면 이 다섯 줄을 삭제하십시오.
	EnableDocking(CBRS_ALIGN_ANY);
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	
#if 0
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndToolBar);
#endif

	DockPane(&m_wndMenuBar);


	// Visual Studio 2005 스타일 도킹 창 동작을 활성화합니다.
	CDockingManager::SetDockingMode(DT_SMART);
	// Visual Studio 2005 스타일 도킹 창 자동 숨김 동작을 활성화합니다.
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// 메뉴 항목 이미지를 로드합니다(표준 도구 모음에 없음).
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

#if 0
	// 도킹 창을 만듭니다.
	if (!CreateDockingWindows())
	{
		TRACE0("도킹 창을 만들지 못했습니다.\n");
		return -1;
	}

	m_wndFileView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndClassView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndFileView);
	CDockablePane* pTabbedBar = NULL;
	m_wndClassView.AttachToTabWnd(&m_wndFileView, DM_SHOW, TRUE, &pTabbedBar);
	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOutput);
	m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndProperties);


	// 도구 모음 및 도킹 창 메뉴 바꾸기를 활성화합니다.
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// 빠른(<Alt> 키를 누른 채 끌기) 도구 모음 사용자 지정을 활성화합니다.
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// 사용자 정의 도구 모음 이미지를 로드합니다.
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			m_UserImages.SetImageSize(CSize(16, 16), FALSE);
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}
#endif

	// 메뉴 개인 설정을 활성화합니다(가장 최근에 사용한 명령).
	// TODO: 사용자의 기본 명령을 정의하여 각 풀다운 메뉴에 하나 이상의 기본 명령을 포함시킵니다.
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_MOVE_FIRST_PAGE);
	lstBasicCommands.AddTail(ID_MOVE_NEXT_PAGE);
	lstBasicCommands.AddTail(ID_MOVE_PREV_PAGE);
	lstBasicCommands.AddTail(ID_MOVE_LAST_PAGE);
	lstBasicCommands.AddTail(ID_ADD_BOOKMARK);

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);
	lstBasicCommands.AddTail(ID_SORTING_SORTALPHABETIC);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYTYPE);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYACCESS);
	lstBasicCommands.AddTail(ID_SORTING_GROUPBYTYPE);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
#if 0
	BOOL bNameValid;

	// 클래스 뷰를 만듭니다.
	CString strClassView;
	bNameValid = strClassView.LoadString(IDS_CLASS_VIEW);
	ASSERT(bNameValid);
	if (!m_wndClassView.Create(strClassView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CLASSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("클래스 뷰 창을 만들지 못했습니다.\n");
		return FALSE; // 만들지 못했습니다.
	}

	// 파일 뷰를 만듭니다.
	CString strFileView;
	bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
	ASSERT(bNameValid);
	if (!m_wndFileView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("파일 뷰 창을 만들지 못했습니다.\n");
		return FALSE; // 만들지 못했습니다.
	}

	// 출력 창을 만듭니다.
	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);
	if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("출력 창을 만들지 못했습니다.\n");
		return FALSE; // 만들지 못했습니다.
	}

	// 속성 창을 만듭니다.
	CString strPropertiesWnd;
	bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
	ASSERT(bNameValid);
	if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("속성 창을 만들지 못했습니다.\n");
		return FALSE; // 만들지 못했습니다.
	}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
#endif

	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
#if 0
	HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndFileView.SetIcon(hFileViewIcon, FALSE);

	HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndClassView.SetIcon(hClassViewIcon, FALSE);

	HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

	HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);
#endif
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* 메뉴를 검색합니다. */);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

void CMainFrame::OnExitSizeMove()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	theApp.m_WebkitEngine.ReLayout();

	//CFrameWndEx::OnExitSizeMove();
}


BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// 기본 클래스가 실제 작업을 수행합니다.

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// 모든 사용자 도구 모음에 사용자 지정 단추를 활성화합니다.
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(m_pWndLibraryView) delete m_pWndLibraryView;

	CRect rect;
	GetClientRect(rect);
	rect.top = 122;

	// Library View
	m_pWndLibraryView = new CLibraryView();
	m_pWndLibraryView->Create(NULL, NULL, WS_CHILD, rect, this, AFX_IDW_PANE_FIRST + 1, pContext);
	m_pWndLibraryView->ShowWindow(SW_HIDE);

	return CFrameWndEx::OnCreateClient(lpcs, pContext);
}

/*
 * IsExistInLibrary
 * 라이브러리에 특정 파일이 이미 추가 되어 있는지 확인하는 함수
 * 파라미터 lpszFilepath 확인하고자 하는 파일의 절대 경로
 * 리턴값 TRUE (exist) / FALSE (be nonexistent)
 */
BOOL CMainFrame::IsExistInLibrary(LPCTSTR lpszFilePath)
{
	if(m_pWndLibraryView)
	{
		return m_pWndLibraryView->IsExist(lpszFilePath);
	}

	return FALSE;
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN)
	{
       //SendMessage(WM_KEYDOWN,pMsg->wParam,pMsg->lParam);
	}

	return CFrameWndEx::PreTranslateMessage(pMsg);
}



/************************************************************
 * Epub Engine interface
 ************************************************************/
void CMainFrame::SetBook(CWnd *pBook)
{
	if(pBook)
	{
		m_pBook = pBook;
		//m_pBook->ShowWindow(SW_SHOW);
	}
}

CWnd *CMainFrame::GetBook()
{
	return m_pBook;
}

LRESULT CMainFrame::OnEpubLoaded(WPARAM wParam, LPARAM lParam)
{	
	HRESULT hr = S_OK;
	BOOL bInitialized = (BOOL)lParam;
	if(!bInitialized)
	{
		theApp.m_WebkitEngine.ReLayout((CWnd *)wParam);
		theApp.m_WebkitEngine.MoveToPage((CWnd *)wParam, 1);
	}

	return hr;
}

LRESULT CMainFrame::OnBookmarkStatus(WPARAM wParam, LPARAM lParam)
{
	// 페이지 이동, 북마크 추가/삭제 시 엔진으로 부터 호출되는 콜백 콜백 메시지 함수
	// 파라미터를 통해 현재 페이지에 북마크가 있는지 여부를 알려준다.
	// 북마크가 있을 경우, 북마크 표시
	HRESULT hr = S_OK;
	bool pHasBool = *((bool *)lParam);
	if(pHasBool)
	{
		// display bookmark
		TRACE(_T("this page has bookmark!!\r\n"));
	}

	return hr;
}
	
LRESULT CMainFrame::OnAnnotationAdded(WPARAM wParam, LPARAM lParam)
{
	// highlight, underline, memo 가 추가되면, 엔진으로 부터 호출되는 콜백 메시지 함수
	// lParam 은 IN3Annotation 객체
	HRESULT hr = S_OK;
	IN3Annotation *pAnnotation = (IN3Annotation *)lParam;
	if(pAnnotation)
	{
		// 뷰어 단에서 추가된 annotation을 관리한다. 
		if(pAnnotation->HasMemo())
		{
			// 메모가 추가된 경우
			TRACE("memo Added\r\n");
		}
		else
		{
			// 하이라이트가 추가된 경우
			TRACE("highlight Added\r\n");
		}
	}
	return hr;
}

LRESULT CMainFrame::OnBookmarkAdded(WPARAM wParam, LPARAM lParam)
{
	HRESULT hr = S_OK;
	IN3Bookmark *pBookmark = (IN3Bookmark *)lParam;
	if(pBookmark)
	{
		// 뷰어 단에서 추가된 북마크에 대해서 처리해야 할 내용 구현
		TRACE("bookmark added\r\n");
	}
	return hr;
}
	
LRESULT CMainFrame::OnContextMenu(WPARAM wParam, LPARAM lParam)
{
	// view 에서 마우스 오른쪽 버튼 클릭 시 UI 단으로 전달하는 callback
	//
	HRESULT hr = S_OK;
	POINT* pPt = (POINT *)lParam;

	// display context menu
	CMenu menu, *pPopup;
	if(!menu.LoadMenuW(IDR_EPUB_CONTEXT_MENU))
		return S_FALSE;
	
	pPopup = menu.GetSubMenu(0);
	if(pPopup == NULL)
		return S_FALSE;

	BOOL bRes = pPopup->TrackPopupMenu(TPM_LEFTALIGN /*| TPM_RETURNCMD*/, pPt->x, pPt->y, this);	
	pPopup->DestroyMenu();
	
	return hr;
}

LRESULT CMainFrame::OnAnnotationMenu(WPARAM wParam, LPARAM lParam)
{
	HRESULT hr = S_OK;
	POINT* pPt = (POINT *)lParam;

	// display context menu
	CMenu menu, *pPopup;
	if(!menu.LoadMenuW(IDR_EPUB_ANNOTATION_MENU))
		return S_FALSE;
	
	pPopup = menu.GetSubMenu(0);
	if(pPopup == NULL)
		return S_FALSE;

	BOOL bRes = pPopup->TrackPopupMenu(TPM_LEFTALIGN /*| TPM_RETURNCMD*/, pPt->x, pPt->y, this);	
	pPopup->DestroyMenu();
	
	return hr;
}
	
LRESULT CMainFrame::OnAnnotationSelected(WPARAM wParam, LPARAM lParam)
{
	HRESULT hr = S_OK;	
	IN3Annotation *pAnnotation = (IN3Annotation *)lParam;
	if(!pAnnotation) return S_FALSE;

	POINT pt;
	GetCursorPos(&pt);

	// display context menu
	CMenu menu, *pPopup;
	if(!menu.LoadMenuW(IDR_EPUB_ANNOTATION_MODIFY_MENU))
		return S_FALSE;
	
	pPopup = menu.GetSubMenu(0);
	if(pPopup == NULL)
		return S_FALSE;
	
	if(pAnnotation->GetBGColor() == _T(""))
	{
		// underline
		pPopup->DeleteMenu(1, MF_BYPOSITION);
	}

	LONG iSelect = pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RETURNCMD, pt.x, pt.y, this);
	if(iSelect == ID_DELETE_ANNOTATION)
	{
		theApp.m_WebkitEngine.DeleteAnnotation(m_pBook, pAnnotation->GetFileName(), pAnnotation->GetMarkerID());
	}
	else if (iSelect == ID_CHANGE_ANNOTATION_COLOR_RED)
	{
		pAnnotation->SetBGColor(_T("rgba(255, 0, 0, 0.6)"));
		pAnnotation->SetMemo(false);
		theApp.m_WebkitEngine.ModifyAnnotation(m_pBook, pAnnotation);
	}
	else if (iSelect == ID_CHANGE_ANNOTATION_COLOR_GREEN)
	{
		pAnnotation->SetBGColor(_T("rgba(0, 255, 0, 0.6)"));
		pAnnotation->SetMemo(false);
		theApp.m_WebkitEngine.ModifyAnnotation(m_pBook, pAnnotation);
	}
	else if (iSelect == ID_CHANGE_ANNOTATION_COLOR_BLUE)
	{
		pAnnotation->SetBGColor(_T("rgba(0, 0, 255, 0.6)"));
		pAnnotation->SetMemo(false);
		theApp.m_WebkitEngine.ModifyAnnotation(m_pBook, pAnnotation);
	}

	pPopup->DestroyMenu();
	return hr;
}

LRESULT CMainFrame::OnMemoIconSelected(WPARAM wParam, LPARAM lParam)
{
	// lparam 은 IN3Annotation 객체 포인터임.
	// 단, fileName, marker_id 값만 있음.
	HRESULT hr = S_OK;
	IN3Annotation *pAnnotation = (IN3Annotation *)lParam;
	if(!pAnnotation) return S_FALSE;

	// UI 구현
	// 뷰어단에서 관리하고 있는 annotaton list 에서 pAnnotation->GetMarkerID() 와 일치하는 어노테이션의
	// 메모를 화면에 표시해줌.
	//

	return hr;
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWndEx::OnSize(nType, cx, cy);
	
	// TODO: 여기에 메시지 처리기 코드를 추가합니다
	
	int nMenu = ::GetSystemMetrics(SM_CYMENU);
	if( nMenu > 0 )
		::MoveWindow(m_pBook->GetSafeHwnd(),  0 , nMenu , cx , cy - nMenu, FALSE);
	else
		::MoveWindow(m_pBook->GetSafeHwnd(),  0 , 0 , cx , cy, FALSE);
}

void CMainFrame::OnMoveFirstPage()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다
	if(m_pBook)
		theApp.m_WebkitEngine.MoveToFirstPage(m_pBook);
}

void CMainFrame::OnMovePrevPage()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다
	if(m_pBook)
		theApp.m_WebkitEngine.MoveToPrevPage(m_pBook);
}

void CMainFrame::OnMoveNextPage()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다
	if(m_pBook)
		theApp.m_WebkitEngine.MoveToNextPage(m_pBook);
}

void CMainFrame::OnMoveLastPage()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다
	if(m_pBook)
		theApp.m_WebkitEngine.MoveToLastPage(m_pBook);
}

void CMainFrame::OnAddBookmark()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다
	if(m_pBook)
		theApp.m_WebkitEngine.SetBookmark(m_pBook);
}

void CMainFrame::OnAddHighlight()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다
	if(m_pBook)
		theApp.m_WebkitEngine.CreateTextHighlight(m_pBook, _T("rgba(255, 0, 0, 0.6)"), FALSE);
}

void CMainFrame::OnAddMemo()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다
	if(m_pBook)
		theApp.m_WebkitEngine.CreateUnderline(m_pBook, _T(""), TRUE);
}

void CMainFrame::OnDeleteAnnotation()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다
}

void CMainFrame::OnChangeAnnotationColorRed()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다
}

void CMainFrame::OnChangeAnnotationColorGreen()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다
}

void CMainFrame::OnChangeAnnotationColorBlue()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다
}

void CMainFrame::OnSettingViewing()
{
	if(m_pBook)
	{
		CFontDialog fontDlg;
		if(fontDlg.DoModal() != IDOK) return;

		theApp.m_WebkitEngine.SetFontFamily(m_pBook, fontDlg.GetFaceName());
		theApp.m_WebkitEngine.SetFontSize(m_pBook, fontDlg.GetSize()/10);

		CString strColor;
		strColor.Format(_T("rgb(%d,%d,%d)"), GetRValue(fontDlg.GetColor()), GetGValue(fontDlg.GetColor()),GetBValue(fontDlg.GetColor()));
		theApp.m_WebkitEngine.SetFontColor(m_pBook, strColor);
		theApp.m_WebkitEngine.ReLayout(m_pBook);
	}
}

void CMainFrame::OnSingleDisplayMode()
{
	if(m_pBook)
	{
		theApp.m_WebkitEngine.SetPageDisplayMode(m_pBook, 1);
	}
}

void CMainFrame::OnDualDisplayMode()
{
	if(m_pBook)
	{
		theApp.m_WebkitEngine.SetPageDisplayMode(m_pBook, 2);
	}
}
