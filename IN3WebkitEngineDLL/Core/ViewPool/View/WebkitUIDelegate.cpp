#include "StdAfx.h"

#include "IN3WebkitDefine.h"
#include "WebkitUIDelegate.h"

/**
 * @brief	생성자
 */
WebkitUIDelegate::WebkitUIDelegate(IN3WebkitView *pWebkitView)
	: m_refCount(1)
	, m_pWebkitView(pWebkitView)
{
}

/**
 * @brief	소멸자
 */
WebkitUIDelegate::~WebkitUIDelegate(void)
{
}

HRESULT WebkitUIDelegate::QueryInterface(REFIID riid, void** ppvObject)
{
    *ppvObject = 0;
    if (IsEqualIID(riid, IID_IUnknown))
        *ppvObject = static_cast<IWebUIDelegate*>(this);
    else if (IsEqualIID(riid, IID_IWebUIDelegate))
        *ppvObject = static_cast<IWebUIDelegate*>(this);
    else
        return E_NOINTERFACE;

    AddRef();
    return S_OK;
}

ULONG WebkitUIDelegate::AddRef(void)
{
    return ++m_refCount;
}

ULONG WebkitUIDelegate::Release(void)
{
    ULONG newRef = --m_refCount;
    if (!newRef)
        delete this;

    return newRef;
}

HRESULT WebkitUIDelegate::webViewPrintingMarginRect(IWebView* view, RECT* rect)
{
    if (!view || !rect)
        return E_POINTER;

    IWebFrame* mainFrame = 0;
    if (FAILED(view->mainFrame(&mainFrame)))
        return E_FAIL;

    IWebFramePrivate* privateFrame = 0;
    if (FAILED(mainFrame->QueryInterface(&privateFrame))) {
        mainFrame->Release();
        return E_FAIL;
    }
    privateFrame->frameBounds(rect);

    return S_OK;
}

/**
 * @brief	javascript 내에서 alert() 으로 출력하는 메시지를 TRACE 를 사용하여 콘솔에 출력하는 인터페이스
 */
HRESULT STDMETHODCALLTYPE WebkitUIDelegate::runJavaScriptAlertPanelWithMessage( 
            /* [in] */ IWebView *sender,
            /* [in] */ BSTR message)
{
#ifdef _DEBUG
	HRESULT hr = S_OK;
	CString msg = message;
	CString traceMsg = msg.Left(200 < msg.GetLength() ? 200 : msg.GetLength());
	//TRACE(L"--------------- javascript alert -----------------\r\n");
	TRACEMSG_JAVA(traceMsg);
	//TRACE(L"--------------------------------------------------\r\n");
	return hr;
#else
	return E_NOTIMPL;
#endif /*_DEBUG*/
}
		
#if 0
void WebkitUIDelegate::SetPrintingPageInfo(HDC printDC, float fMarginCentimeter, float fHeaderCentimeter, float fFooterCentimeter)
{
	m_szPageInPixel.SetSize(
		GetDeviceCaps(printDC, PHYSICALWIDTH)  - 2 * GetDeviceCaps(printDC, PHYSICALOFFSETX),
        GetDeviceCaps(printDC, PHYSICALHEIGHT) - 2 * GetDeviceCaps(printDC, PHYSICALOFFSETY));

	float fPixelsPerInchX = GetDeviceCaps(printDC, LOGPIXELSX);
	float fPixelsPerInchY = GetDeviceCaps(printDC, LOGPIXELSY);

	float fHeaderInInch = fHeaderCentimeter / 2.54;
	float fFooterInInch = fFooterCentimeter / 2.54;

	m_nHeaderHeight = (int)(fHeaderInInch * fPixelsPerInchY + 0.5);
	m_nFooterHeight = (int)(fFooterInInch * fPixelsPerInchY + 0.5);

	float fMarginInInch = fMarginCentimeter / 2.54;

	m_nMarginLeftInPixel = m_nMarginRightInPixel = (int)(fMarginInInch * fPixelsPerInchX + 0.5);
	m_nMarginTopInPixel = m_nMarginBottomInPixel = (int)(fMarginInInch * fPixelsPerInchY + 0.5);

}
#endif