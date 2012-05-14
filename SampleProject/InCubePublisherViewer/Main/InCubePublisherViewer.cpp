
// InCubePublisherViewer.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "InCubePublisherViewer.h"
#include "MainFrm.h"

#include "InCubePublisherViewerDoc.h"
#include "InCubePublisherViewerView.h"

#include "unzip.h"
#include "TStringEx.h"
#include "FileSystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CInCubePublisherViewerApp

BEGIN_MESSAGE_MAP(CInCubePublisherViewerApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CInCubePublisherViewerApp::OnAppAbout)
	// 표준 파일을 기초로 하는 문서 명령입니다.
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// 표준 인쇄 설정 명령입니다.
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CInCubePublisherViewerApp 생성

CInCubePublisherViewerApp::CInCubePublisherViewerApp()
{

	m_bHiColorIcons = TRUE;

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}

// 유일한 CInCubePublisherViewerApp 개체입니다.

CInCubePublisherViewerApp theApp;


// CInCubePublisherViewerApp 초기화

BOOL CInCubePublisherViewerApp::InitInstance()
{
	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다. 
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// OLE 라이브러리를 초기화합니다.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));
	LoadStdProfileSettings(4);  // MRU를 포함하여 표준 INI 파일 옵션을 로드합니다.

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 응용 프로그램의 문서 템플릿을 등록합니다. 문서 템플릿은
	//  문서, 프레임 창 및 뷰 사이의 연결 역할을 합니다.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CInCubePublisherViewerDoc),
		RUNTIME_CLASS(CMainFrame),       // 주 SDI 프레임 창입니다.
		//RUNTIME_CLASS(CInCubePublisherViewerView));
		NULL);
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);



	// 표준 셸 명령, DDE, 파일 열기에 대한 명령줄을 구문 분석합니다.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// 명령줄에 지정된 명령을 디스패치합니다.
	// 응용 프로그램이 /RegServer, /Register, /Unregserver 또는 /Unregister로 시작된 경우 FALSE를 반환합니다.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 창 하나만 초기화되었으므로 이를 표시하고 업데이트합니다.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// 접미사가 있을 경우에만 DragAcceptFiles를 호출합니다.
	//  SDI 응용 프로그램에서는 ProcessShellCommand 후에 이러한 호출이 발생해야 합니다.
	return TRUE;
}



// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// 대화 상자를 실행하기 위한 응용 프로그램 명령입니다.
void CInCubePublisherViewerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CInCubePublisherViewerApp 사용자 지정 로드/저장 메서드

void CInCubePublisherViewerApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CInCubePublisherViewerApp::LoadCustomState()
{
}

void CInCubePublisherViewerApp::SaveCustomState()
{
}

// CInCubePublisherViewerApp 메시지 처리기




CDocument* CInCubePublisherViewerApp::OpenDocumentFile(LPCTSTR lpszFileName)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CMainFrame *pMainFrame = (CMainFrame *)m_pMainWnd;
	if ( !pMainFrame ) return NULL;

	if(pMainFrame->GetBook() != NULL)
	{
		theApp.m_WebkitEngine.Close(pMainFrame->GetBook());
	}

	TCHAR	fname[_MAX_FNAME];
	TCHAR	ext[_MAX_EXT];

	//_wsplitpath(lpszFileName, NULL, NULL, fname, ext);	// warning C4996
	_wsplitpath_s(lpszFileName, NULL, 0, NULL, 0, fname, _MAX_FNAME, ext, _MAX_EXT);

	// file  extension check
	CString extStr = ext;
	extStr.MakeLower();
	if( extStr != _T(".epub") ) return NULL;

	CDocument* pDoc = NULL;
	pDoc = CWinAppEx::OpenDocumentFile(lpszFileName);

	if(pDoc)
	{		
		if(!pMainFrame->IsExistInLibrary(lpszFileName))
		{
			// @ note 
			// Add Content To Library
			// 구현 예정
		}

		// ReadingMode 인 경우 바로 Epub Open 을 실행한다.
		if ( pMainFrame->GetViewMode() == CMainFrame::READING_MODE )
		{
			TRACE(_T("webkit open document START\r\n"));
			if(theApp.m_WebkitEngine.IsOpend(lpszFileName))
			{
				// @ note 
				// 해당 탭으로 이동 시켜줘야 하나? 
				return pDoc;
			}
			
			// STEP 1) create epub folder
			CString unzipPath(_T(""));
			unzipPath.Append(theApp.m_WebkitEngine.GetEpubMainPath());
			unzipPath.Append(_T("\\"));
			unzipPath.Append(fname);
			CreateDirectory(unzipPath, NULL);

			// STEP 2) unzip
			unzipPath.Append(_T("\\"));
			if(!ExtractEPub(lpszFileName, unzipPath))
			{
				return NULL;
			}

			// STEP 3) load Epub
			CWnd *pBook = theApp.m_WebkitEngine.Open(pMainFrame, lpszFileName, unzipPath);
			if(!pBook)
			{
				// @ note
				// Error Message? 
				return NULL;
			}

			// STEP 4) create views
			if(!theApp.m_WebkitEngine.CreateView(pBook))
			{
				// @ note
				// Error Message?  (뷰 생성 실패)
				theApp.m_WebkitEngine.Close(pBook);
				return NULL;
			}
		
			// STEP 5) set book's instance pointer to UI
			pMainFrame->SetBook(pBook);
			theApp.ReloadWindowPlacement(pMainFrame);
		}
	}

	return pDoc;
}

BOOL CInCubePublisherViewerApp::ExtractEPub(LPCTSTR lpszPathName, LPCTSTR lpszDescPath)
{
	if ( lpszDescPath )
	{
		HZIP hZIP;
		hZIP = OpenZipFile( lpszPathName, NULL );
		if ( hZIP == NULL)
		{
		#if _DEBUG
			TRACE0("[error] OpenZip File\r\n");
		#endif /*_DEBUG*/
			return FALSE;
		}

		SetUnzipBaseDir(hZIP, lpszDescPath);

		UniString	rootPath(lpszDescPath);
		if ( !rootPath.IsEmpty() )
			rootPath.Append(PATH_SEPARATOR_CHAR);

		ZIPENTRY ze;
		GetZipItem(hZIP, -1, &ze);
		int numitems = ze.index;
		for (int i = 0; i < numitems; i++)
		{
			GetZipItem(hZIP, i, &ze);

			if ( _tcscmp(ze.name, _T("mimetype")) == 0 ) continue;	
			UnzipItemFile(hZIP, i, ze.name);
		}
	}
	return TRUE;
}