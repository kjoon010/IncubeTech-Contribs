#include "StdAfx.h"
#include "WebkitResourceLoadDelegate.h"

#include "IN3WebkitDefine.h"

WebkitResourceLoadDelegate::WebkitResourceLoadDelegate(IN3WebkitView *pWebkitView)
	: m_refCount(1)
	, m_pWebkitView(pWebkitView)
{
}

WebkitResourceLoadDelegate::~WebkitResourceLoadDelegate(void)
{
}

HRESULT WebkitResourceLoadDelegate::QueryInterface(REFIID riid, void** ppvObject)
{
    *ppvObject = 0;
    if (IsEqualIID(riid, IID_IUnknown))
        *ppvObject = static_cast<IWebResourceLoadDelegate*>(this);
    else if (IsEqualIID(riid, IID_IWebResourceLoadDelegate))
        *ppvObject = static_cast<IWebResourceLoadDelegate*>(this);
    else
        return E_NOINTERFACE;

    AddRef();
    return S_OK;
}

ULONG WebkitResourceLoadDelegate::AddRef(void)
{
    return ++m_refCount;
}

ULONG WebkitResourceLoadDelegate::Release(void)
{
    ULONG newRef = --m_refCount;
    if (!newRef)
        delete this;

    return newRef;
}
