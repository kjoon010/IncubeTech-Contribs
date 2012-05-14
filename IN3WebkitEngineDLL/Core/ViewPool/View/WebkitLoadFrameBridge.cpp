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
 * @brief	웹킷 랜더링 뷰를 만든다.
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
	// 반드시 initWithFrame후에 호출할 것
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
		// 웹킷의 메인 프레임을 가져와 현재 Load 되어 있는 프레임을 해제한다.
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
 * @brief	현재 load 되어 있는 문서를 다시 load 한다.
 * @return	TRUE(success) / FALSE(fail)
 */
BOOL CWebkitLoadFrameBridge::ReLoad()
{
	BOOL bRes = TRUE;
	HRESULT		hr = S_OK;
	if ( m_pWebView )
	{
		// 웹킷의 메인 프레임을 가져와 현재 Load 되어 있는 프레임을 다시 로드한다.
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
 * @brief	현재 Webkit View 에 선택된 내용이 있는지 확인 하는 함수
 * @remarks	선택된 내용이 텍스트 일 경우만 확인 가능하다는 점에 주의.
 * @reutrn	true(선택된 내용 있음)/false(선택된 내용 없음)
 */
BOOL CWebkitLoadFrameBridge::IsSelected()
{
	if(m_pWebView)
	{
		// 선택된 텍스트를 가져온다.
		CComBSTR selected;
		m_pWebView->selectedText(&selected);

		// 텍스트가 있다면 선택된 내용이 있는 것을 생각한다.
		if(selected.Length() > 0)
			return TRUE;
	}

	return FALSE;
}

/**
 * @brief	선택된 내용을 반환한다.
 * @return	선택된 내용
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
 * @brief	scroll 을 보이게/안보이게 하는 함수
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
 * @brief fixed 컨텐츠를 laod 한 후 초기화 작업을 한다. 
 * @remarks	fixed 컨텐츠의 viewport 값을 추출하는 로직이 추가되어야 한다. 현재 body element 의 width,height 을 가져오도록 되어 있으나 viewport 에 명시된 값과는 다름.
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

	// fixed 컨텐츠는 1page = 1file 을 기준으로 한다.
	// 따라서 총 페이지는 1, 현재 페이지도 1.
	m_pWebkitView->SetTotalPage(1);
	m_pWebkitView->SetCurrPage(1);

	// 화면 크기에 맞춰 fixed layout 컨텐츠를 보여주게 하기 위해선 fixed 컨텐츠의 크기를 구해야한다.
	// 현재 body element 의 width, height 을 가져와 사용한, 이는 viewport 에 명시된 값과 달라 화명 표시에 문제가 발생한다.
	// viewport 값을 가져올 방법을 생각해야 한다.
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
	// width, height 값이 없음.
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
	// webkit api 구현 안되어 있음
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

	// fixed 컨텐츠가 LOAD 완료 되었음을 설정
	m_pWebkitView->DocumentComplete();
	// 전체 페이지 계산
	pBook->CalculateTotalPage();
	//
	TRACEMSG(_T("CWebkitLoadFrameBridge::InitFixedPage() END"));
}

/**
 * @brief reflow 컨텐츠를 laod 한 후 초기화 작업을 한다. 
 * @param	fontSize	: 폰트 사이즈
 * @param	familyName	: 폰트명
 * @param	back_clr	: 배경색
 * @param	fore_clr	: 폰트색
 * @param	bKeepPage	: 초기화 전후 페이지 유지 여부, (true: 유지, false: 유지 않함)
 */
void CWebkitLoadFrameBridge::InitPage(CString fontSize, CString familyName, CString back_clr, CString fore_clr, bool bKeepPage /*= true*/)
{
	TRACEMSG(_T("CWebkitLoadFrameBridge::InitPage() START"));
	//
	CEPubBook *pBook = (CEPubBook*)m_pWebkitView->GetParentFrame();
	if(pBook)
	{
		// 웹킷 extension 으로 초기화 작업을 요청한다.
		CString parm;
		parm.Format(L"{ \"FONT_SIZE\":\"%s\", \"FONT_FAMILY\":\"%s\", \"BACKGROUND_COLOR\":\"%s\", \"FOREGROUND_COLOR\":\"%s\", \"COLUMN_GAP\":\"%d\", \"COLUMN_COUNT\":\"%d\", \"KEEP_CURRENT_PAGE\":\"%d\" }",
			fontSize, familyName, back_clr, fore_clr, pBook->GetMargin(), pBook->GetPageDisplayMode(), bKeepPage);

		SendMessageToEngine(CMD_IN3_PAGE_INIT, parm);
	}
	//
	TRACEMSG(_T("CWebkitLoadFrameBridge::InitPage() END"));
}

/**
 * @brief	확대/축소 비율을 웹킷에 적용한다.
 * @param	multiplier	: 확대/축소 비율 1.0 = 100%
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
		// 폰트 부드럽게 처리.
		webPreferences->setFontSmoothing(FontSmoothingTypeWindows);
		// 텍스트 외 모든 엘리먼트에 적용
		webPreferences->setZoomsTextOnly(false);
	}
	//hr = m_pWebView->setTextSizeMultiplier(multipler);

	// 확대/축소 비율 적용
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
 * @brief	엔진으로 부터 받은 콜백 메시지
 * @remarks	메시지를 파싱하여 command 및 메시지를 해석한다
 * @param	msg	: 엔진으로부터 받은 메시지 전문
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
	// command type 이 뷰어인 경우는 메시지 처리를 뷰어에서 하며 엔진인 경우는 Webkitbridge에서 처리
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
 * @brief	엔진으로 부터 받은 콜백 메시지
 * @param	command	: 커맨드
 * @param	param	: 커맨드와 함께 정의된 메시지
 * @return	S_OK(success)/S_FALSE(fail)
 * @see		InitFixedPage, InitPage
 */
HRESULT CWebkitLoadFrameBridge::ReceiveMessage(CString command, CString param)
{
	TRACEMSG(_T("CWebkitLoadFrameBridge::ReceiveMessage() START"));
	//
	HRESULT hr = S_OK;
	// 파일 로드 완료
	if(command == CMD_IN3_PAGE_LOAD)
	{
		if(!m_pWebkitView){
			TRACEMSG_ERR(_T("m_pWebkitView is NULL!!"));
			return NULL;
		}

		// 뷰어의 화면을 갱신한다.
		m_pWebkitView->Invalidate();

		CEPubBook* pBook = (CEPubBook*)m_pWebkitView->GetParentFrame();
		if(!pBook){
			TRACEMSG_ERR(_T("pBook is NULL!!"));
			return NULL;
		}

		// fixed 와 reflow 컨텐츠를 구분하여 초기화를 진행한다.
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
 * @brief	엔진에 메시지를 보내는 함수
 * @param	command	: 커맨드
 * @param	param	: 커맨드와 함께 보낼 데이터
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
 * @brief	웹킷 frame laod 직전에 호출되는 함수
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

	// load url 추출
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
		// load url 에 'appcall?' 이 있다면 웹킷 extension 으로 부터 받은 메시지로 간주하고 메시지를 파싱한다.
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
 * @brief	Webkit 에서 랜더링 완료 후 호출하는 콜백 함수
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

	// load 완료하면 일단 스크로을 안보이게 가린다.
	ShowHideScrollBar(FALSE);

	HRESULT hr = S_OK;
	if(m_pWebView)
	{
		// Load default resources
		// webit extensions 을 load 한다.
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

		// 20120406 - PC 에서 메모 아이콘 안보이는 문제가 있어 메모 아이콘을 preload 시킴.
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
 * @brief	뷰어의 실행파일이 있는 위치를 반환한다.
 * @return	뷰어의 실행파일이 있는 위치
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
 * @brief	Load 된 document 의 body element 를 반환한다.
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
 * @param	lpszURL	: Load 할 URL
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