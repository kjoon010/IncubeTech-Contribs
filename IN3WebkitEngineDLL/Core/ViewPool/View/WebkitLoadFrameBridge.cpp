#include "StdAfx.h"
#include "WebkitLoadFrameBridge.h"

#include "WebkitUIDelegate.h"
#include "IN3WebkitDefine.h"

#include "JSON.h"
#include "IN3WebLib.h"

#include "EPubBook.h"
#include "IN3WebkitView.h"

#include <afxwin.h>
#include <commctrl.h>
#include <commdlg.h>
#include <objbase.h>
#include <shlwapi.h>
#include <wininet.h>


HRESULT STDMETHODCALLTYPE CWebkitLoadFrameBridge::QueryInterface(REFIID riid, void** ppvObject)
{
	*ppvObject = 0;
	if (IsEqualGUID(riid, IID_IUnknown))
		*ppvObject = static_cast<IWebFrameLoadDelegate*>(this);
	else if (IsEqualGUID(riid, IID_IWebFrameLoadDelegate))
		*ppvObject = static_cast<IWebFrameLoadDelegate*>(this);
	else
		return E_NOINTERFACE;

	AddRef();
	return S_OK;
}

ULONG STDMETHODCALLTYPE CWebkitLoadFrameBridge::AddRef(void)
{
	return ++m_refCount;
}

ULONG STDMETHODCALLTYPE CWebkitLoadFrameBridge::Release(void)
{
	ASSERT(m_refCount > 0);
	ULONG newRef = --m_refCount;
	if (!newRef)
		delete(this);

	return newRef;
}

/**
 * @brief	��Ŷ ������ �並 �����.
 * @return	TRUE(success) / FALSE(fail)
 */
BOOL CWebkitLoadFrameBridge::Create()
{
	TRACEMSG(_T("CWebkitLoadFrameBridge::Create() START"));
	//
	CRect clientRect;
	m_pWebkitView->GetClientRect(&clientRect);

	HRESULT hr = WebKitCreateInstance(CLSID_WebView, 0, IID_IWebView, (void**)&m_pWebView);
	if (FAILED(hr)) return FALSE;
	//
	hr = m_pWebView->setFrameLoadDelegate(this);
	if (FAILED(hr)) return FALSE;
	//
	m_pWebkitView->m_pWebkitUIDelegate = new WebkitUIDelegate(m_pWebkitView);
	m_pWebkitView->m_pWebkitUIDelegate->AddRef();
	hr = m_pWebView->setUIDelegate(m_pWebkitView->m_pWebkitUIDelegate);
	if (FAILED(hr)) return FALSE;
	//
	HWND	hWnd = m_pWebkitView->GetSafeHwnd();
	hr = m_pWebView->setHostWindow((OLE_HANDLE)hWnd);
	if (FAILED(hr)) return FALSE;
	//
	GetClientRect(hWnd, &clientRect);
	hr = m_pWebView->initWithFrame(clientRect, 0, 0);
	if (FAILED(hr)) return FALSE;
	//
	CComPtr<IWebViewPrivate>	webViewPrivate;
	hr = m_pWebView->QueryInterface(IID_IWebViewPrivate, (void**)&webViewPrivate);
	if (FAILED(hr)) return FALSE;
	//
	//
	hr = webViewPrivate->viewWindow((OLE_HANDLE*) &(m_pWebkitView->m_ViewWindow));
	if (FAILED(hr)) return FALSE;
	//
	// �ݵ�� initWithFrame�Ŀ� ȣ���� ��
	CComPtr<IWebPreferences>	webPreferences;
	m_pWebView->preferences(&webPreferences);
	if ( webPreferences )
	{
		webPreferences->setFontSmoothing(FontSmoothingTypeWindows);
		webPreferences->setZoomsTextOnly(false);
		//	webPreferences->setUsesPageCache(FALSE);
	}
	//
	WebKitCookieStorageAcceptPolicy acceptPolicy = WebKitCookieStorageAcceptPolicyAlways;
	webPreferences->cookieStorageAcceptPolicy(&acceptPolicy);
	//
	TRACEMSG(_T("CWebkitLoadFrameBridge::Create() END"));

	return TRUE;
}

/**
 * @brief	Load url
 * @param	lpszURL	: url
 * @return	TRUE(success) / FALSE(fail)
 */
BOOL CWebkitLoadFrameBridge::LoadFromURL(LPCTSTR lpszURL)
{
	TRACEMSG(_T("CWebkitLoadFrameBridge::LoadFromURL() START"));
	//
	if ( m_pWebView )
	{
		// ��Ŷ�� ���� �������� ������ ���� Load �Ǿ� �ִ� �������� �����Ѵ�.
		IWebFrame* pWebFrame = NULL;
		HRESULT		hr = m_pWebView->mainFrame(&pWebFrame);
		if (pWebFrame)
		{
			pWebFrame->stopLoading();
			pWebFrame->Release();
		}

		// laod url
		BOOL	ok = loadURL(lpszURL);
		//
		TRACEMSG(_T("CWebkitLoadFrameBridge::LoadFromURL() END"));
		//
		return ok;
	}
	//
	TRACEMSG(_T("CWebkitLoadFrameBridge::LoadFromURL() END"));

	return FALSE;
}

/**
 * @brief	���� load �Ǿ� �ִ� ������ �ٽ� load �Ѵ�.
 * @return	TRUE(success) / FALSE(fail)
 */
BOOL CWebkitLoadFrameBridge::ReLoad()
{
	BOOL bRes = TRUE;
	HRESULT		hr = S_OK;
	if ( m_pWebView )
	{
		// ��Ŷ�� ���� �������� ������ ���� Load �Ǿ� �ִ� �������� �ٽ� �ε��Ѵ�.
		IWebFrame* pWebFrame = NULL;
		hr = m_pWebView->mainFrame(&pWebFrame);
		if (pWebFrame)
		{
			hr = pWebFrame->reload();
		}
	}
	return (hr == S_OK);
}




/********************************************************************************
 * UI Setting interface
 ********************************************************************************/
/**
 * @brief	���� Webkit View �� ���õ� ������ �ִ��� Ȯ�� �ϴ� �Լ�
 * @remarks	���õ� ������ �ؽ�Ʈ �� ��츸 Ȯ�� �����ϴٴ� ���� ����.
 * @reutrn	true(���õ� ���� ����)/false(���õ� ���� ����)
 */
BOOL CWebkitLoadFrameBridge::IsSelected()
{
	if(m_pWebView)
	{
		// ���õ� �ؽ�Ʈ�� �����´�.
		CComBSTR selected;
		m_pWebView->selectedText(&selected);

		// �ؽ�Ʈ�� �ִٸ� ���õ� ������ �ִ� ���� �����Ѵ�.
		if(selected.Length() > 0)
			return TRUE;
	}

	return FALSE;
}

/**
 * @brief	���õ� ������ ��ȯ�Ѵ�.
 * @return	���õ� ����
 */
CString CWebkitLoadFrameBridge::GetSelectedText()
{
	CString selectedText = _T("");
	//
	if(m_pWebView)
	{
		CComBSTR selected;
		m_pWebView->selectedText(&selected);
		selectedText = selected;
	}
	//
	return selectedText;
}

/**
 * @brief	scroll �� ���̰�/�Ⱥ��̰� �ϴ� �Լ�
 * @param	show	show(TRUE)/HIDE(FALSE)
 */
void CWebkitLoadFrameBridge::ShowHideScrollBar(BOOL show)
{
	if(m_pWebView)
	{
		IWebFrame* webFrame = NULL;
		HRESULT hr = m_pWebView->mainFrame(&webFrame);
		if (SUCCEEDED(hr) && webFrame)
		{
			webFrame->setAllowsScrolling(show);
			webFrame->Release();
		}
	}
}

/**
 * @brief fixed �������� laod �� �� �ʱ�ȭ �۾��� �Ѵ�. 
 * @remarks	fixed �������� viewport ���� �����ϴ� ������ �߰��Ǿ�� �Ѵ�. ���� body element �� width,height �� ���������� �Ǿ� ������ viewport �� ��õ� ������ �ٸ�.
 */
void CWebkitLoadFrameBridge::InitFixedPage()
{
	TRACEMSG(_T("CWebkitLoadFrameBridge::InitFixedPage() START"));
	//
	if(!m_pWebkitView){
		TRACEMSG_ERR(_T("m_pWebkitView is NULL!!"));
		return;
	}

	CEPubBook *pBook = (CEPubBook*)m_pWebkitView->GetParentFrame();
	if(!pBook){
		TRACEMSG_ERR(_T("pBook is NULL!!"));
		return;
	}

	if(!pBook->IsFixed()){
		TRACEMSG_ERR(_T("CWebkitLoadFrameBridge::InitFixedPage() works fixed layout content. The book isn't fixed content!!"));
		return;
	}

	// fixed �������� 1page = 1file �� �������� �Ѵ�.
	// ���� �� �������� 1, ���� �������� 1.
	m_pWebkitView->SetTotalPage(1);
	m_pWebkitView->SetCurrPage(1);

	// ȭ�� ũ�⿡ ���� fixed layout �������� �����ְ� �ϱ� ���ؼ� fixed �������� ũ�⸦ ���ؾ��Ѵ�.
	// ���� body element �� width, height �� ������ �����, �̴� viewport �� ��õ� ���� �޶� ȭ�� ǥ�ÿ� ������ �߻��Ѵ�.
	// viewport ���� ������ ����� �����ؾ� �Ѵ�.
	CComPtr<IDOMElement>				bodyElement = GetBodyElement();
	CComQIPtr<IDOMElementExtensions>	bodyElementExtensions = bodyElement;

#if 1
	if(bodyElementExtensions)
	{
#if 0
		int height = 0, width = 0;
		bodyElementExtensions->clientHeight(&height);
		bodyElementExtensions->clientWidth(&width);
		//
		pBook->SetFixedOriginalSize(width, height);
#else
		pBook->SetFixedOriginalSize(768, 1024);
#endif
	}
#else
	// width, height ���� ����.
	CComQIPtr<IDOMViewCSS>		domViewCSS;
	bodyElementExtensions->QueryInterface(IID_IDOMViewCSS, (void**)&domViewCSS);
	//m_pWebView->QueryInterface(IID_IDOMViewCSS, (void**)&domViewCSS);
	if(domViewCSS)
	{
		CComQIPtr<IDOMCSSStyleDeclaration>		domCSSStyleDeclaration;
		domViewCSS->getComputedStyle(bodyElement, NULL, &domCSSStyleDeclaration);
		if(domCSSStyleDeclaration)
		{
			int height = 0, width = 0;

			CComBSTR  propertyName, value;
			IDOMCSSValue *result;
			propertyName = _T("width");
			domCSSStyleDeclaration->getPropertyCSSValue(propertyName, &result);
			if(result){
				result->cssText(&value);
				width = _wtoi(value);
			}
			
			propertyName = _T("height");
			domCSSStyleDeclaration->getPropertyCSSValue(propertyName, &result);
			if(result){
				result->cssText(&value);
				width = _wtoi(value);
			}
			//
			if( width > 0 && height > 0)
				pBook->SetFixedOriginalSize(width, height);
		}
	}

#if 0
	// webkit api ���� �ȵǾ� ����
	CComQIPtr<IDOMElementCSSInlineStyle>	domElementCSSInlineStyle = bodyElement;
	CComQIPtr<IDOMCSSStyleDeclaration>		domCSSStyleDeclaration;
	if ( domElementCSSInlineStyle )
	{
		domElementCSSInlineStyle->style(&domCSSStyleDeclaration);
		if ( domCSSStyleDeclaration )
		{
			int height = 0, width = 0;

			CComBSTR  propertyName, value;
			IDOMCSSValue *result;
			propertyName = _T("width");
			domCSSStyleDeclaration->getPropertyCSSValue(propertyName, &result);
			if(result){
				result->cssText(&value);
				width = _wtoi(value);
			}
			
			propertyName = _T("height");
			domCSSStyleDeclaration->getPropertyCSSValue(propertyName, &result);
			if(result){
				result->cssText(&value);
				width = _wtoi(value);
			}
			//
			if( width > 0 && height > 0)
				pBook->SetFixedOriginalSize(width, height);
		}
	}
#endif
#endif

	// fixed �������� LOAD �Ϸ� �Ǿ����� ����
	m_pWebkitView->DocumentComplete();
	// ��ü ������ ���
	pBook->CalculateTotalPage();
	//
	TRACEMSG(_T("CWebkitLoadFrameBridge::InitFixedPage() END"));
}

/**
 * @brief reflow �������� laod �� �� �ʱ�ȭ �۾��� �Ѵ�. 
 * @param	fontSize	: ��Ʈ ������
 * @param	familyName	: ��Ʈ��
 * @param	back_clr	: ����
 * @param	fore_clr	: ��Ʈ��
 * @param	bKeepPage	: �ʱ�ȭ ���� ������ ���� ����, (true: ����, false: ���� ����)
 */
void CWebkitLoadFrameBridge::InitPage(CString fontSize, CString familyName, CString back_clr, CString fore_clr, bool bKeepPage /*= true*/)
{
	TRACEMSG(_T("CWebkitLoadFrameBridge::InitPage() START"));
	//
	CEPubBook *pBook = (CEPubBook*)m_pWebkitView->GetParentFrame();
	if(pBook)
	{
		// ��Ŷ extension ���� �ʱ�ȭ �۾��� ��û�Ѵ�.
		CString parm;
		parm.Format(L"{ \"FONT_SIZE\":\"%s\", \"FONT_FAMILY\":\"%s\", \"BACKGROUND_COLOR\":\"%s\", \"FOREGROUND_COLOR\":\"%s\", \"COLUMN_GAP\":\"%d\", \"COLUMN_COUNT\":\"%d\", \"KEEP_CURRENT_PAGE\":\"%d\" }",
			fontSize, familyName, back_clr, fore_clr, pBook->GetMargin(), pBook->GetPageDisplayMode(), bKeepPage);

		SendMessageToEngine(CMD_IN3_PAGE_INIT, parm);
	}
	//
	TRACEMSG(_T("CWebkitLoadFrameBridge::InitPage() END"));
}

/**
 * @brief	Ȯ��/��� ������ ��Ŷ�� �����Ѵ�.
 * @param	multiplier	: Ȯ��/��� ���� 1.0 = 100%
 * @return	S_OK(success)/S_FALSE(fail)
 */
HRESULT CWebkitLoadFrameBridge::SetMultiplier(float multiplier)
{
	if(multiplier < 0) return S_FALSE;

	HRESULT hr = S_FALSE;
	CComPtr<IWebPreferences> webPreferences;
	m_pWebView->preferences(&webPreferences);
	if(webPreferences)
	{
		// ��Ʈ �ε巴�� ó��.
		webPreferences->setFontSmoothing(FontSmoothingTypeWindows);
		// �ؽ�Ʈ �� ��� ������Ʈ�� ����
		webPreferences->setZoomsTextOnly(false);
	}
	//hr = m_pWebView->setTextSizeMultiplier(multipler);

	// Ȯ��/��� ���� ����
	CComPtr<IWebIBActions>	webIBActions;
	m_pWebView->QueryInterface(IID_IWebIBActions, (void**)&webIBActions);
	if ( webIBActions )
		hr = webIBActions->setPageSizeMultiplier(multiplier);

	return hr;
}



/********************************************************************************
 * Communication interface
 ********************************************************************************/
/**
 * @brief	�������� ���� ���� �ݹ� �޽���
 * @remarks	�޽����� �Ľ��Ͽ� command �� �޽����� �ؼ��Ѵ�
 * @param	msg	: �������κ��� ���� �޽��� ����
 * @return	S_OK(success)/S_FALSE(fail)
 */
HRESULT	CWebkitLoadFrameBridge::ReceiveMessage(CString msg)
{
	TRACEMSG(_T("CWebkitLoadFrameBridge::ReceiveMessage() START"));
	//
	// utf8 url decode
	HRESULT hr = S_OK;
	CString message = UrlDecode(msg);
	if(message.IsEmpty())return S_FALSE;

	JSONValue* value = JSON::Parse(message);
	if(value == NULL){
		TRACEMSG_ERR(_T("[CWebkitViewBridge::ReceiveMessage] recived data can't parsing!!"));
		return S_FALSE;
	}
	//
	// Retrieve the main object
	if (value->IsObject() == false){
		TRACEMSG_ERR(_T("[CWebkitViewBridge::ReceiveMessage] The root element is not an object!!"));
		return S_FALSE;
	}
	//
	CString cmdType(L"");
	CString cmdCode(L"");
	CString param(L"");
	//
	JSONObject root;
	root = value->AsObject();
	//
	// 1. Get command type
	JSONObject::iterator iter = root.find(CMD_IN3_TYPE);
	if(iter!= root.end() && root[CMD_IN3_TYPE]->IsString()){
		cmdType = root[CMD_IN3_TYPE]->Stringify().c_str();
		cmdType.Replace(L"\"",L"");
	}
	//
	// 2. Get command code
	iter = root.find(CMD_IN3_CODE);
	if(iter!= root.end() && root[CMD_IN3_CODE]->IsString()){
		cmdCode = root[CMD_IN3_CODE]->Stringify().c_str();
		cmdCode.Replace(L"\"",L"");
	}
	//
	// 3. Get param
	iter = root.find(L"PARAM");
	if(iter!= root.end() /*&& root[L"PARAM"]->IsString()*/){
		param = root[L"PARAM"]->Stringify().c_str();
		//param.Replace(L"\"",L"");
	}
	//
	if(cmdType.IsEmpty() || cmdCode.IsEmpty()) return S_FALSE;
	//
	// command type �� ����� ���� �޽��� ó���� ���� �ϸ� ������ ���� Webkitbridge���� ó��
	if(cmdType == CMD_TYPE_VIEWER)
	{
		if(m_pWebkitView)
			m_pWebkitView->ReceiveMessage(cmdCode, param);
	}
	else
	{
		ReceiveMessage(cmdCode, param);
	}
	//
	TRACEMSG(_T("CWebkitLoadFrameBridge::ReceiveMessage() END"));

	if(value)
		delete value;
	return hr;
}

/**
 * @brief	�������� ���� ���� �ݹ� �޽���
 * @param	command	: Ŀ�ǵ�
 * @param	param	: Ŀ�ǵ�� �Բ� ���ǵ� �޽���
 * @return	S_OK(success)/S_FALSE(fail)
 * @see		InitFixedPage, InitPage
 */
HRESULT CWebkitLoadFrameBridge::ReceiveMessage(CString command, CString param)
{
	TRACEMSG(_T("CWebkitLoadFrameBridge::ReceiveMessage() START"));
	//
	HRESULT hr = S_OK;
	// ���� �ε� �Ϸ�
	if(command == CMD_IN3_PAGE_LOAD)
	{
		if(!m_pWebkitView){
			TRACEMSG_ERR(_T("m_pWebkitView is NULL!!"));
			return NULL;
		}

		// ����� ȭ���� �����Ѵ�.
		m_pWebkitView->Invalidate();

		CEPubBook* pBook = (CEPubBook*)m_pWebkitView->GetParentFrame();
		if(!pBook){
			TRACEMSG_ERR(_T("pBook is NULL!!"));
			return NULL;
		}

		// fixed �� reflow �������� �����Ͽ� �ʱ�ȭ�� �����Ѵ�.
		if(pBook->IsFixed())
			InitFixedPage();
		else
		{
			CString strFontSize;
			strFontSize.Format(_T("%dpt"), pBook->GetFontSize());
			InitPage(strFontSize, pBook->GetFontFamily(), pBook->GetBackgroudColor(), pBook->GetFontColor());
		}
	}
	//
	TRACEMSG(_T("CWebkitLoadFrameBridge::ReceiveMessage() END"));

	return hr;
}

/**
 * @brief	������ �޽����� ������ �Լ�
 * @param	command	: Ŀ�ǵ�
 * @param	param	: Ŀ�ǵ�� �Բ� ���� ������
 * @return	S_OK(success)/S_FALSE(fail)
 */
HRESULT CWebkitLoadFrameBridge::SendMessageToEngine(CString command, CString param)
{
	TRACEMSG(_T("CWebkitLoadFrameBridge::SendMessageToEngine() START"));
	//
	if(!m_pWebView){
		TRACEMSG_ERR(_T("m_pWebView is NULL!!"));
		return S_FALSE;
	}
	//	
	CString msg;
	msg.Format(L"procCmd('%s', '%s', '%s');", CMD_TYPE_VIEWER, command, param);
	//
	BSTR res;
	CComBSTR msgBSTR(msg);
	HRESULT hr = m_pWebView->stringByEvaluatingJavaScriptFromString(msgBSTR, &res);
	//
	TRACEMSG(_T("CWebkitLoadFrameBridge::SendMessageToEngine() END"));

	return hr;
}





/********************************************************************************
 * Protected Method
 ********************************************************************************/
/**
 * @brief	��Ŷ frame laod ������ ȣ��Ǵ� �Լ�
 */
HRESULT CWebkitLoadFrameBridge::StartProvisionalLoadForFrame(IWebView* webView, IWebFrame *frame)
{
	TRACEMSG(_T("CWebkitLoadFrameBridge::StartProvisionalLoadForFrame() START"));
	//
	if(m_pWebkitView)
	{
		CString traceMsg;
		traceMsg.Format(_T("chapter index %d"), m_pWebkitView->GetChapterIndex());
		TRACEMSG_INFO(traceMsg);
	}
	//
	CComPtr<IWebDataSource>			dataSource;
	CComPtr<IWebMutableURLRequest>	request;
	CComBSTR						frameURL;

	HRESULT hr = S_OK;

	// load url ����
	hr = frame->provisionalDataSource(&dataSource);
	if (FAILED(hr) || !dataSource)
		return S_FALSE;

	hr = dataSource->request(&request);
	if (FAILED(hr) || !request)
		return S_FALSE;

	hr = request->mainDocumentURL(&frameURL);
	if (FAILED(hr))
		return S_FALSE;

	CString url = frameURL;
	if(!url.IsEmpty())
	{
		// load url �� 'appcall?' �� �ִٸ� ��Ŷ extension ���� ���� ���� �޽����� �����ϰ� �޽����� �Ľ��Ѵ�.
		int iStart = 0, iFind = 0;
		iFind = url.Find(L"appcall?",iStart);
		if(iFind >= 0)
		{
			int iStart = iFind + wcslen(L"appcall?");
			CString msg = url.Mid(iStart, url.GetLength() - iStart);
			/*
			HRESULT hres = ValidateRequestMessage(msg);
			if(hres == S_OK)
			{
			*/
			return ReceiveMessage(msg);
			//}
		}
	}
	//
	TRACEMSG(_T("CWebkitLoadFrameBridge::StartProvisionalLoadForFrame() END"));

	return S_OK;
}

/**
 * @brief	Webkit ���� ������ �Ϸ� �� ȣ���ϴ� �ݹ� �Լ�
 */
HRESULT CWebkitLoadFrameBridge::FinishLoadForFrame(IWebView* webView)
{
	TRACEMSG(_T("CWebkitLoadFrameBridge::FinishLoadForFrame() START"));
	//
	if(!m_pWebkitView){
		TRACEMSG_ERR(_T("m_pWebkitView is NULL!!"));
		return S_FALSE;
	}

	CEPubBook *pBook = (CEPubBook*)m_pWebkitView->GetParentFrame();
	if(!pBook){
		TRACEMSG_ERR(_T("pBook is NULL!!"));
		return S_FALSE;
	}

	// load �Ϸ��ϸ� �ϴ� ��ũ���� �Ⱥ��̰� ������.
	ShowHideScrollBar(FALSE);

	HRESULT hr = S_OK;
	if(m_pWebView)
	{
		// Load default resources
		// webit extensions �� load �Ѵ�.
		CString filePath = L"file:///";
		filePath.Append(GetApplicationPath());
		filePath.Replace(L"\\", L"/");

		// STEP 1) basic css resource
		CString strCSS(_T("var headID = document.getElementsByTagName('head')[0]; "));
		strCSS.Append(_T("var cssNode = document.createElement('link'); "));
		strCSS.Append(_T("cssNode.type = 'text/css'; cssNode.rel = 'stylesheet'; "));
		strCSS.Append(_T("cssNode.href = '"));
		strCSS.Append(filePath + IN3WebLib::GetPathLibIN3Css());
		strCSS.Append(_T("'; headID.appendChild(cssNode);"));

		// STEP 2) basic js resource
		CString strJS(_T("var g_in3_jquery_lib_src ='"));
		strJS.Append(filePath + IN3WebLib::GetPathLibIN3Jquery()); strJS.Append(_T("'; "));
		strJS.Append(_T("var g_in3_js_api_lib_src ='"));
		strJS.Append(filePath + IN3WebLib::GetPathLibIN3JS()); strJS.Append(_T("'; "));		
		strJS.Append(_T("var g_in3_memo_left_icon_src ='"));
		strJS.Append(filePath + IN3WebLib::GetPathMemoLeftIcon()); strJS.Append(_T("'; "));
		//strJS.Append(_T("memo_left.png'; "));
		strJS.Append(_T("var g_in3_memo_right_icon_src ='"));
		strJS.Append(filePath + IN3WebLib::GetPathMemoRightIcon()); strJS.Append(_T("'; "));
		//strJS.Append(_T("memo_right.png'; "));

		strJS.Append(_T("var newjs=document.createElement('script'); newjs.src='"));
		strJS.Append(filePath + IN3WebLib::GetPathLibIN3InitJS());	strJS.Append(_T("'; "));
		strJS.Append(_T("newjs.type='text/javascript';  "));
		strJS.Append(_T("var head=document.getElementsByTagName('head')[0]; head.appendChild(newjs);  "));

		// 20120406 - PC ���� �޸� ������ �Ⱥ��̴� ������ �־� �޸� �������� preload ��Ŵ.
		strJS.Append(_T("var in3LeftIconPreLoad = new Image(); "));
		//strJS.Append(_T("in3LeftIconPreLoad.src = 'memo_left.png'; "));
		strJS.Append(_T("in3LeftIconPreLoad.src = '")); strJS.Append(filePath + IN3WebLib::GetPathMemoLeftIcon()); strJS.Append(_T("'; "));
		strJS.Append(_T("var in3RightIconPreLoad = new Image(); "));
		//strJS.Append(_T("in3RightIconPreLoad.src = 'memo_right.png'; "));
		strJS.Append(_T("in3RightIconPreLoad.src = '")); strJS.Append(filePath + IN3WebLib::GetPathMemoRightIcon()); strJS.Append(_T("'; "));

		CString strAllJS = strCSS + L"\n" + strJS;
		CComBSTR allJS(strAllJS), res;
		hr = m_pWebView->stringByEvaluatingJavaScriptFromString(allJS, &res);
	}
	//
	CString traceMsg;
	traceMsg.Format(_T("javascript add reutrn %d, chapter index %d"), hr, m_pWebkitView->GetChapterIndex());
	TRACEMSG(traceMsg);
	//
	TRACEMSG(_T("CWebkitLoadFrameBridge::FinishLoadForFrame() END"));
	return hr;
}

HRESULT CWebkitLoadFrameBridge::FailProvisionalLoadWithError(IWebView* webView, IWebError *error, IWebFrame *frame)
{
	if ( m_pWebkitView )
	{
		if ( m_IsRun )
		{
			m_pWebkitView->EndModalLoop(0);
			m_IsRun= false;
		}

		CComBSTR	message;
		error->localizedDescription(&message);

		CComBSTR	url;
		error->failingURL(&url);

		m_pWebkitView->LoadError(url, message);
	}

	return S_OK;
}

HRESULT CWebkitLoadFrameBridge::FailLoadWithError(IWebView* webView, IWebError *error, IWebFrame *forFrame)
{
	if ( m_pWebkitView )
	{
		if ( m_IsRun )
		{
			m_pWebkitView->EndModalLoop(0);
			m_IsRun= false;
		}

		CComBSTR	message;
		error->localizedDescription(&message);

		CComBSTR	url;
		error->failingURL(&url);

		//m_pWebkitView->LoadError(url, message);
	}

	return S_OK;
}

/**
 * @brief	����� ���������� �ִ� ��ġ�� ��ȯ�Ѵ�.
 * @return	����� ���������� �ִ� ��ġ
 */
CString CWebkitLoadFrameBridge::GetApplicationPath()
{
	TCHAR    Temp[_MAX_PATH+1];
	VERIFY(::GetModuleFileName(AfxGetApp()->m_hInstance, Temp, _MAX_PATH));
	CString ProgramDir = Temp;

	int len = ProgramDir.ReverseFind('\\');
	if ( len > 1 )	ProgramDir = ProgramDir.Left( len + 1 );
	return ProgramDir;
}

/**
 * @brief	Load �� document �� body element �� ��ȯ�Ѵ�.
 * @return	body element
 */
CComPtr<IDOMElement> CWebkitLoadFrameBridge::GetBodyElement() const
{
	TRACEMSG(_T("CWebkitLoadFrameBridge::GetBodyElement() START"));
	//
	if(!m_pWebView){
		TRACEMSG_ERR(_T("m_pWebView is NULL!!!"));
		return NULL;
	}

	CComPtr<IDOMDocument>	domDocument;
	HRESULT	hr = m_pWebView->mainFrameDocument(&domDocument);

	CComQIPtr<IDOMHTMLDocument>	domHTMLDocument = domDocument;
	CComQIPtr<IDOMElement>		bodyElement;

	if ( domHTMLDocument )
	{
		CComPtr<IDOMHTMLElement>	htmlBodyElement;
		domHTMLDocument->body(&htmlBodyElement);
		bodyElement = htmlBodyElement;
	}
	else if ( domDocument )
	{
		CComPtr<IDOMNodeList>	nodeList;
		CComBSTR				bodyStr(L"body");

		domDocument->getElementsByTagName(bodyStr, &nodeList);
		if ( nodeList )
		{
			UINT		count = 0;
			nodeList->length(&count);
			if(count > 0)
			{
				CComPtr<IDOMNode> body;
				nodeList->item(0, &body);
				bodyElement = body;
			}
		}
	}
	//
	TRACEMSG(_T("CWebkitLoadFrameBridge::GetBodyElement() END"));
	return bodyElement;
}

/**
 * @brief	Load URL
 * @param	lpszURL	: Load �� URL
 * @return	TRUE(succes) / FALSE(fail)
 */
BOOL CWebkitLoadFrameBridge::loadURL(LPCTSTR lpszURL)
{
	if (!lpszURL && lpszURL[0]) return FALSE;
	if(!m_pWebView) return FALSE;
	//
	TRACEMSG(_T("CWebkitLoadFrameBridge::loadURL() START"));
	//
	CComBSTR	bstrURL(lpszURL);
	if (_wcsnicmp(bstrURL, L"http://", 7) != 0)
	{
		if (_wcsnicmp(lpszURL, L"file://", 7) == 0)
		{
			bstrURL = lpszURL + 7;
		}

		if ( (PathFileExists(bstrURL) || PathIsUNC(bstrURL))) 
		{
			TCHAR fileURL[INTERNET_MAX_URL_LENGTH];
			DWORD fileURLLength = sizeof(fileURL) / sizeof(fileURL[0]);

			if (SUCCEEDED(UrlCreateFromPath(bstrURL, fileURL, &fileURLLength, 0)))
				bstrURL= fileURL;
		}
		else
		{
			return FALSE;
		}
	}

	BSTR methodBStr = SysAllocString(TEXT("GET"));

	CComPtr<IWebFrame>				webFrame;
	HRESULT hr = m_pWebView->mainFrame(&webFrame);
	if (FAILED(hr)) return FALSE;

	CComPtr<IWebMutableURLRequest>	request;
	hr = WebKitCreateInstance(CLSID_WebMutableURLRequest, 0, IID_IWebMutableURLRequest, (void**)&request);
	if (FAILED(hr)) return FALSE;

	hr = request->initWithURL(bstrURL, WebURLRequestReloadIgnoringCacheData, 60);
	if (FAILED(hr)) return FALSE;

	if ( _wcsnicmp((BSTR)bstrURL, L"http://", 7) == 0 )
		hr = request->setHTTPMethod(methodBStr);

	hr = webFrame->loadRequest(request);
	if (FAILED(hr)) return FALSE;
	//
	TRACEMSG(_T("CWebkitLoadFrameBridge::loadURL() END"));

	return TRUE;
}