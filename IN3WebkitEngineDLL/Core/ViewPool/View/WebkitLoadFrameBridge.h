#pragma once


#include <WebKit/WebKit.h>
#include <WebKit/WebKitCOMAPI.h>

class IN3WebkitView;
////////////////////////////////////////////////////////////////////////////////////////////
// CWebkitLoadFrameBridge �Դϴ�.

/**
  @class	CWebkitLoadFrameBridge
  @brief	Webkit Renderer Engine �� UI CView ���̸� �����ϴ� �긴�� Ŭ����
  @remarks	FrameLoadDelegate �� ��ӹ޴´�
  @see		FrameLoadDelegate
 */
class CWebkitLoadFrameBridge : public IWebFrameLoadDelegate
{
public:
	 CWebkitLoadFrameBridge() : m_refCount(1), m_pWebkitView(NULL), m_IsRun(false) {}

	 
    // IUnknown
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject);
    virtual ULONG STDMETHODCALLTYPE AddRef(void);
    virtual ULONG STDMETHODCALLTYPE Release(void);

    // IWebFrameLoadDelegate
    virtual HRESULT STDMETHODCALLTYPE didStartProvisionalLoadForFrame( 
        /* [in] */ IWebView* webView,
        /* [in] */ IWebFrame* frame) { 
		//	return S_OK; 
			return StartProvisionalLoadForFrame(webView, frame);
	}
    
    virtual HRESULT STDMETHODCALLTYPE didReceiveServerRedirectForProvisionalLoadForFrame( 
        /* [in] */ IWebView *webView,
        /* [in] */ IWebFrame *frame) { return S_OK; }
    
    virtual HRESULT STDMETHODCALLTYPE didFailProvisionalLoadWithError( 
        /* [in] */ IWebView *webView,
        /* [in] */ IWebError *error,
        /* [in] */ IWebFrame *frame) { return FailProvisionalLoadWithError(webView, error, frame); }
    
    virtual HRESULT STDMETHODCALLTYPE didCommitLoadForFrame( 
        /* [in] */ IWebView *webView,
        /* [in] */ IWebFrame *frame) { /*return updateAddressBar(webView);*/ return S_OK; }
    
    virtual HRESULT STDMETHODCALLTYPE didReceiveTitle( 
        /* [in] */ IWebView *webView,
        /* [in] */ BSTR title,
        /* [in] */ IWebFrame *frame) { return S_OK; }
    
    virtual HRESULT STDMETHODCALLTYPE didChangeIcons(
        /* [in] */ IWebView *webView,
        /* [in] */ IWebFrame *frame) { return S_OK; }

    virtual HRESULT STDMETHODCALLTYPE didReceiveIcon( 
        /* [in] */ IWebView *webView,
        /* [in] */ OLE_HANDLE hBitmap,
        /* [in] */ IWebFrame *frame) { return S_OK; }
    
    virtual HRESULT STDMETHODCALLTYPE didFinishLoadForFrame( 
        /* [in] */ IWebView* webView,
        /* [in] */ IWebFrame* /*frame*/) { return FinishLoadForFrame(webView); }
    
    virtual HRESULT STDMETHODCALLTYPE didFailLoadWithError( 
        /* [in] */ IWebView *webView,
        /* [in] */ IWebError *error,
        /* [in] */ IWebFrame *forFrame) { return FailLoadWithError(webView, error, forFrame); }
    
    virtual HRESULT STDMETHODCALLTYPE didChangeLocationWithinPageForFrame( 
        /* [in] */ IWebView *webView,
        /* [in] */ IWebFrame *frame) { return S_OK; }

    virtual HRESULT STDMETHODCALLTYPE willPerformClientRedirectToURL( 
        /* [in] */ IWebView *webView,
        /* [in] */ BSTR url,
        /* [in] */ double delaySeconds,
        /* [in] */ DATE fireDate,
        /* [in] */ IWebFrame *frame) { return S_OK; }
    
    virtual HRESULT STDMETHODCALLTYPE didCancelClientRedirectForFrame( 
        /* [in] */ IWebView *webView,
        /* [in] */ IWebFrame *frame) { return S_OK; }
    
    virtual HRESULT STDMETHODCALLTYPE willCloseFrame( 
        /* [in] */ IWebView *webView,
        /* [in] */ IWebFrame *frame) { return S_OK; }
    
    virtual /* [local] */ HRESULT STDMETHODCALLTYPE windowScriptObjectAvailable( 
        /* [in] */ IWebView *webView,
        /* [in] */ JSContextRef context,
        /* [in] */ JSObjectRef windowScriptObject)  { return S_OK; }

    virtual /* [local] */ HRESULT STDMETHODCALLTYPE didClearWindowObject( 
        /* [in] */ IWebView *webView,
        /* [in] */ JSContextRef context,
        /* [in] */ JSObjectRef windowScriptObject,
        /* [in] */ IWebFrame *frame) { return S_OK; }


	// init
	BOOL Create();
	BOOL LoadFromURL(LPCTSTR lpszURL);
	BOOL ReLoad();


	// UI Setting interface
	BOOL IsSelected();
	CString GetSelectedText();
	void ShowHideScrollBar(BOOL show);

	void InitFixedPage();
	void InitPage(CString fontSize, CString familyName, CString back_clr, CString fore_clr, bool bKeepPage = true);

	HRESULT SetMultiplier(float multipler);


	// Communication interface
	HRESULT	ReceiveMessage(CString msg);
	HRESULT ReceiveMessage(CString command, CString param);
	HRESULT SendMessageToEngine(CString command, CString param);


protected:
	HRESULT StartProvisionalLoadForFrame(IWebView* webView, IWebFrame *frame);
	HRESULT FinishLoadForFrame(IWebView* webView);
	HRESULT FailProvisionalLoadWithError(IWebView* webView, IWebError *error, IWebFrame *forFrame);
	HRESULT FailLoadWithError(IWebView* webView, IWebError *error, IWebFrame *forFrame);

	static CString GetApplicationPath();
	CComPtr<IDOMElement> GetBodyElement() const;

	BOOL loadURL(LPCTSTR lpszURL);

protected:
    ULONG                   m_refCount;	///< reference count
	BOOL					m_IsRun; ///< frame loading ������ üũ�ϴ� �Լ�

	IN3WebkitView*			m_pWebkitView; ///< ���� ������
	CComPtr<IWebView>		m_pWebView;	///< webkit view

	friend IN3WebkitView;
};
