#pragma once

#include <WebKit/WebKit.h>

class IN3WebkitView;

////////////////////////////////////////////////////////////////////////////////////////////
// IN3WebResourceLoadDelegate

class WebkitResourceLoadDelegate : public IWebResourceLoadDelegate
{
public:
	WebkitResourceLoadDelegate(IN3WebkitView *pWebkitView);
	~WebkitResourceLoadDelegate(void);

    // IUnknown
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject);
    virtual ULONG STDMETHODCALLTYPE AddRef(void);
    virtual ULONG STDMETHODCALLTYPE Release(void);


	// IWebResourceLoadDelegate
    virtual HRESULT STDMETHODCALLTYPE identifierForInitialRequest( 
        /* [in] */ IWebView *webView,
        /* [in] */ IWebURLRequest *request,
        /* [in] */ IWebDataSource *dataSource,
        /* [in] */ unsigned long identifier) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE willSendRequest( 
        /* [in] */ IWebView *webView,
        /* [in] */ unsigned long identifier,
        /* [in] */ IWebURLRequest *request,
        /* [in] */ IWebURLResponse *redirectResponse,
        /* [in] */ IWebDataSource *dataSource,
        /* [retval][out] */ IWebURLRequest **newRequest) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE didReceiveAuthenticationChallenge( 
        /* [in] */ IWebView *webView,
        /* [in] */ unsigned long identifier,
        /* [in] */ IWebURLAuthenticationChallenge *challenge,
        /* [in] */ IWebDataSource *dataSource) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE didCancelAuthenticationChallenge( 
        /* [in] */ IWebView *webView,
        /* [in] */ unsigned long identifier,
        /* [in] */ IWebURLAuthenticationChallenge *challenge,
        /* [in] */ IWebDataSource *dataSource) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE didReceiveResponse( 
        /* [in] */ IWebView *webView,
        /* [in] */ unsigned long identifier,
        /* [in] */ IWebURLResponse *response,
        /* [in] */ IWebDataSource *dataSource) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE didReceiveContentLength( 
        /* [in] */ IWebView *webView,
        /* [in] */ unsigned long identifier,
        /* [in] */ UINT length,
        /* [in] */ IWebDataSource *dataSource) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE didFinishLoadingFromDataSource( 
        /* [in] */ IWebView *webView,
        /* [in] */ unsigned long identifier,
        /* [in] */ IWebDataSource *dataSource) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE didFailLoadingWithError( 
        /* [in] */ IWebView *webView,
        /* [in] */ unsigned long identifier,
        /* [in] */ IWebError *error,
        /* [in] */ IWebDataSource *dataSource) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE plugInFailedWithError( 
        /* [in] */ IWebView *webView,
        /* [in] */ IWebError *error,
        /* [in] */ IWebDataSource *dataSource) = 0;

private:
    int				m_refCount;
	IN3WebkitView*	m_pWebkitView;
};
