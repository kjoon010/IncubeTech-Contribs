
// InCubePublisherViewer.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
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
	// ǥ�� ������ ���ʷ� �ϴ� ���� ����Դϴ�.
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// ǥ�� �μ� ���� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CInCubePublisherViewerApp ����

CInCubePublisherViewerApp::CInCubePublisherViewerApp()
{

	m_bHiColorIcons = TRUE;

	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}

// ������ CInCubePublisherViewerApp ��ü�Դϴ�.

CInCubePublisherViewerApp theApp;


// CInCubePublisherViewerApp �ʱ�ȭ

BOOL CInCubePublisherViewerApp::InitInstance()
{
	// ���� ���α׷� �Ŵ��佺Ʈ�� ComCtl32.dll ���� 6 �̻��� ����Ͽ� ���־� ��Ÿ����
	// ����ϵ��� �����ϴ� ���, Windows XP �󿡼� �ݵ�� InitCommonControlsEx()�� �ʿ��մϴ�. 
	// InitCommonControlsEx()�� ������� ������ â�� ���� �� �����ϴ�.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ���� ���α׷����� ����� ��� ���� ��Ʈ�� Ŭ������ �����ϵ���
	// �� �׸��� �����Ͻʽÿ�.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// OLE ���̺귯���� �ʱ�ȭ�մϴ�.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// ǥ�� �ʱ�ȭ
	// �̵� ����� ������� �ʰ� ���� ���� ������ ũ�⸦ ���̷���
	// �Ʒ����� �ʿ� ���� Ư�� �ʱ�ȭ
	// ��ƾ�� �����ؾ� �մϴ�.
	// �ش� ������ ����� ������Ʈ�� Ű�� �����Ͻʽÿ�.
	// TODO: �� ���ڿ��� ȸ�� �Ǵ� ������ �̸��� ����
	// ������ �������� �����ؾ� �մϴ�.
	SetRegistryKey(_T("���� ���� ���α׷� �����翡�� ������ ���� ���α׷�"));
	LoadStdProfileSettings(4);  // MRU�� �����Ͽ� ǥ�� INI ���� �ɼ��� �ε��մϴ�.

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// ���� ���α׷��� ���� ���ø��� ����մϴ�. ���� ���ø���
	//  ����, ������ â �� �� ������ ���� ������ �մϴ�.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CInCubePublisherViewerDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ������ â�Դϴ�.
		//RUNTIME_CLASS(CInCubePublisherViewerView));
		NULL);
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);



	// ǥ�� �� ���, DDE, ���� ���⿡ ���� ������� ���� �м��մϴ�.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// ����ٿ� ������ ����� ����ġ�մϴ�.
	// ���� ���α׷��� /RegServer, /Register, /Unregserver �Ǵ� /Unregister�� ���۵� ��� FALSE�� ��ȯ�մϴ�.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// â �ϳ��� �ʱ�ȭ�Ǿ����Ƿ� �̸� ǥ���ϰ� ������Ʈ�մϴ�.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// ���̻簡 ���� ��쿡�� DragAcceptFiles�� ȣ���մϴ�.
	//  SDI ���� ���α׷������� ProcessShellCommand �Ŀ� �̷��� ȣ���� �߻��ؾ� �մϴ�.
	return TRUE;
}



// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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

// ��ȭ ���ڸ� �����ϱ� ���� ���� ���α׷� ����Դϴ�.
void CInCubePublisherViewerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CInCubePublisherViewerApp ����� ���� �ε�/���� �޼���

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

// CInCubePublisherViewerApp �޽��� ó����




CDocument* CInCubePublisherViewerApp::OpenDocumentFile(LPCTSTR lpszFileName)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
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
			// ���� ����
		}

		// ReadingMode �� ��� �ٷ� Epub Open �� �����Ѵ�.
		if ( pMainFrame->GetViewMode() == CMainFrame::READING_MODE )
		{
			TRACE(_T("webkit open document START\r\n"));
			if(theApp.m_WebkitEngine.IsOpend(lpszFileName))
			{
				// @ note 
				// �ش� ������ �̵� ������� �ϳ�? 
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
				// Error Message?  (�� ���� ����)
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