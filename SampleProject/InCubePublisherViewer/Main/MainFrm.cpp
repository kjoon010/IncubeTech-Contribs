
// MainFrm.cpp : CMainFrame Ŭ������ ����
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
	ID_SEPARATOR,           // ���� �� ǥ�ñ�
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/�Ҹ�

CMainFrame::CMainFrame()
	:m_pWndLibraryView(NULL)
	,m_ViewMode(READING_MODE)
	,m_pBook(NULL)
	,m_pAnnotation(NULL)
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
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
	// ������ ���� ���� ���־� ������ �� ��Ÿ���� �����մϴ�.
	OnApplicationLook(theApp.m_nAppLook);

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("�޴� ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// �޴� ������ Ȱ��ȭ�ص� ��Ŀ���� �̵����� �ʰ� �մϴ�.
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

#if 0
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("���� ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
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

	// ����� ���� ���� ���� �۾��� ����մϴ�.
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

#if 0
	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("���� ǥ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
#endif

	// TODO: ���� ���� �� �޴� ������ ��ŷ�� �� ���� �Ϸ��� �� �ټ� ���� �����Ͻʽÿ�.
	EnableDocking(CBRS_ALIGN_ANY);
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	
#if 0
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndToolBar);
#endif

	DockPane(&m_wndMenuBar);


	// Visual Studio 2005 ��Ÿ�� ��ŷ â ������ Ȱ��ȭ�մϴ�.
	CDockingManager::SetDockingMode(DT_SMART);
	// Visual Studio 2005 ��Ÿ�� ��ŷ â �ڵ� ���� ������ Ȱ��ȭ�մϴ�.
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// �޴� �׸� �̹����� �ε��մϴ�(ǥ�� ���� ������ ����).
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

#if 0
	// ��ŷ â�� ����ϴ�.
	if (!CreateDockingWindows())
	{
		TRACE0("��ŷ â�� ������ ���߽��ϴ�.\n");
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


	// ���� ���� �� ��ŷ â �޴� �ٲٱ⸦ Ȱ��ȭ�մϴ�.
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// ����(<Alt> Ű�� ���� ä ����) ���� ���� ����� ������ Ȱ��ȭ�մϴ�.
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// ����� ���� ���� ���� �̹����� �ε��մϴ�.
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			m_UserImages.SetImageSize(CSize(16, 16), FALSE);
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}
#endif

	// �޴� ���� ������ Ȱ��ȭ�մϴ�(���� �ֱٿ� ����� ���).
	// TODO: ������� �⺻ ����� �����Ͽ� �� Ǯ�ٿ� �޴��� �ϳ� �̻��� �⺻ ����� ���Խ�ŵ�ϴ�.
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
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
#if 0
	BOOL bNameValid;

	// Ŭ���� �並 ����ϴ�.
	CString strClassView;
	bNameValid = strClassView.LoadString(IDS_CLASS_VIEW);
	ASSERT(bNameValid);
	if (!m_wndClassView.Create(strClassView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CLASSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Ŭ���� �� â�� ������ ���߽��ϴ�.\n");
		return FALSE; // ������ ���߽��ϴ�.
	}

	// ���� �並 ����ϴ�.
	CString strFileView;
	bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
	ASSERT(bNameValid);
	if (!m_wndFileView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("���� �� â�� ������ ���߽��ϴ�.\n");
		return FALSE; // ������ ���߽��ϴ�.
	}

	// ��� â�� ����ϴ�.
	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);
	if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("��� â�� ������ ���߽��ϴ�.\n");
		return FALSE; // ������ ���߽��ϴ�.
	}

	// �Ӽ� â�� ����ϴ�.
	CString strPropertiesWnd;
	bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
	ASSERT(bNameValid);
	if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("�Ӽ� â�� ������ ���߽��ϴ�.\n");
		return FALSE; // ������ ���߽��ϴ�.
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

// CMainFrame ����

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


// CMainFrame �޽��� ó����

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* �޴��� �˻��մϴ�. */);
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	theApp.m_WebkitEngine.ReLayout();

	//CFrameWndEx::OnExitSizeMove();
}


BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// �⺻ Ŭ������ ���� �۾��� �����մϴ�.

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// ��� ����� ���� ������ ����� ���� ���߸� Ȱ��ȭ�մϴ�.
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
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
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
 * ���̺귯���� Ư�� ������ �̹� �߰� �Ǿ� �ִ��� Ȯ���ϴ� �Լ�
 * �Ķ���� lpszFilepath Ȯ���ϰ��� �ϴ� ������ ���� ���
 * ���ϰ� TRUE (exist) / FALSE (be nonexistent)
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
	// ������ �̵�, �ϸ�ũ �߰�/���� �� �������� ���� ȣ��Ǵ� �ݹ� �ݹ� �޽��� �Լ�
	// �Ķ���͸� ���� ���� �������� �ϸ�ũ�� �ִ��� ���θ� �˷��ش�.
	// �ϸ�ũ�� ���� ���, �ϸ�ũ ǥ��
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
	// highlight, underline, memo �� �߰��Ǹ�, �������� ���� ȣ��Ǵ� �ݹ� �޽��� �Լ�
	// lParam �� IN3Annotation ��ü
	HRESULT hr = S_OK;
	IN3Annotation *pAnnotation = (IN3Annotation *)lParam;
	if(pAnnotation)
	{
		// ��� �ܿ��� �߰��� annotation�� �����Ѵ�. 
		if(pAnnotation->HasMemo())
		{
			// �޸� �߰��� ���
			TRACE("memo Added\r\n");
		}
		else
		{
			// ���̶���Ʈ�� �߰��� ���
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
		// ��� �ܿ��� �߰��� �ϸ�ũ�� ���ؼ� ó���ؾ� �� ���� ����
		TRACE("bookmark added\r\n");
	}
	return hr;
}
	
LRESULT CMainFrame::OnContextMenu(WPARAM wParam, LPARAM lParam)
{
	// view ���� ���콺 ������ ��ư Ŭ�� �� UI ������ �����ϴ� callback
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
	// lparam �� IN3Annotation ��ü ��������.
	// ��, fileName, marker_id ���� ����.
	HRESULT hr = S_OK;
	IN3Annotation *pAnnotation = (IN3Annotation *)lParam;
	if(!pAnnotation) return S_FALSE;

	// UI ����
	// ���ܿ��� �����ϰ� �ִ� annotaton list ���� pAnnotation->GetMarkerID() �� ��ġ�ϴ� ������̼���
	// �޸� ȭ�鿡 ǥ������.
	//

	return hr;
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWndEx::OnSize(nType, cx, cy);
	
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�
	
	int nMenu = ::GetSystemMetrics(SM_CYMENU);
	if( nMenu > 0 )
		::MoveWindow(m_pBook->GetSafeHwnd(),  0 , nMenu , cx , cy - nMenu, FALSE);
	else
		::MoveWindow(m_pBook->GetSafeHwnd(),  0 , 0 , cx , cy, FALSE);
}

void CMainFrame::OnMoveFirstPage()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�
	if(m_pBook)
		theApp.m_WebkitEngine.MoveToFirstPage(m_pBook);
}

void CMainFrame::OnMovePrevPage()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�
	if(m_pBook)
		theApp.m_WebkitEngine.MoveToPrevPage(m_pBook);
}

void CMainFrame::OnMoveNextPage()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�
	if(m_pBook)
		theApp.m_WebkitEngine.MoveToNextPage(m_pBook);
}

void CMainFrame::OnMoveLastPage()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�
	if(m_pBook)
		theApp.m_WebkitEngine.MoveToLastPage(m_pBook);
}

void CMainFrame::OnAddBookmark()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�
	if(m_pBook)
		theApp.m_WebkitEngine.SetBookmark(m_pBook);
}

void CMainFrame::OnAddHighlight()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�
	if(m_pBook)
		theApp.m_WebkitEngine.CreateTextHighlight(m_pBook, _T("rgba(255, 0, 0, 0.6)"), FALSE);
}

void CMainFrame::OnAddMemo()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�
	if(m_pBook)
		theApp.m_WebkitEngine.CreateUnderline(m_pBook, _T(""), TRUE);
}

void CMainFrame::OnDeleteAnnotation()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�
}

void CMainFrame::OnChangeAnnotationColorRed()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�
}

void CMainFrame::OnChangeAnnotationColorGreen()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�
}

void CMainFrame::OnChangeAnnotationColorBlue()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�
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
