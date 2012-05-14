/********************************************************************
*
* Copyright (C) 2005 Ikhan Cho
* 
* This file is free software; you can redistribute it and/or
* modify, but leave the headers intact and do not remove any 
* copyrights from the source.
*
* If you have further questions, suggestions or bug fixes, please
* let me know
*
*    
*
********************************************************************/

#ifndef __INTERFACE_WRAPPER_H__
#define __INTERFACE_WRAPPER_H__


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/*
#include <afxwin.h>		// MFC core and standard components
#include <afxext.h>     // MFC extensions
#include <afxdisp.h>    // MFC Automation classes
*/
//#include <windows.h>

#include <atlbase.h>
//#include <comutil.h>


#define FUNC_DECL	// extern "C"
#define FUNC_DECL_V	// extern
#define XML_CLASS	


#if defined(UNICODE) || defined(_UNICODE)
#define UNICODE_AWARE		1
#else
#define UNICODE_AWARE		0
#endif


// Macro that calls a COM method returning HRESULT value:

#if defined(DEBUG) || defined(_DEBUG)

FUNC_DECL void dprintf( const char* format, ...);

#define HRCALL(a, errmsg) \
m_HResult = (a); \
if (FAILED(m_HResult)) { \
    dprintf( "%s:%d  HRCALL Failed: %s\n  0x%.8x = %s\n", __FILE__, __LINE__, errmsg, m_HResult, #a ); \
    goto clean; \
} 

#define RETURN_FLASE_IF_HRCALL_FAILED(a, errmsg) \
m_HResult = (a); \
if (FAILED(m_HResult)) { \
    dprintf( "%s:%d  HRCALL Failed: %s\n  0x%.8x = %s\n", __FILE__, __LINE__, errmsg, m_HResult, #a ); \
    return FALSE; \
}

#define HRCHECK(a, errmsg) \
if ( p ) { \
    m_HResult = (a); \
    if (FAILED(m_HResult)) { \
        dprintf( "%s:%d  HRCHECK Failed: %s\n  0x%.8x = %s\n", __FILE__, __LINE__, errmsg, m_HResult, #a ); \
    } \
}

#define RETURN_IF_FALSE(a)	\
if ( p ) { \
    m_HResult = (a); \
    if (FAILED(m_HResult)) { \
        dprintf( "%s:%d HRESULT Failed: 0x%.8x = %s\n", __FILE__, __LINE__, m_HResult, #a ); \
		return FALSE;	\
	} \
} \
else {	\
	return FALSE;	\
}

#define HRCHECK_RETURN_BOOL(a)	\
if ( p ) { \
    m_HResult = (a); \
    if (FAILED(m_HResult)) { \
        dprintf( "%s:%d HRESULT Failed: 0x%.8x = %s\n", __FILE__, __LINE__, m_HResult, #a ); \
		return FALSE;	\
	} \
	return TRUE;	\
} \
else {	\
	return FALSE;	\
}


#define HRThrowComException(a)	\
if ( p ) { \
    m_HResult = (a); \
    if (FAILED(m_HResult)) { \
        dprintf( "%s:%d  HRCALL Failed: 0x%.8x = %s\n", __FILE__, __LINE__, m_HResult, #a ); \
		throw(m_HResult);	\
    } \
}

#define AfxThrowComException(hr)	\
	dprintf( "%s:%d  HRESULT Failed: 0x%.8x\n",  __FILE__, __LINE__, hr )
	
#else	// not ( defined(DEBUG) || defined(_DEBUG) )

#define HRCALL(a, errmsg) \
    m_HResult = (a); \
    if (FAILED(m_HResult)) goto clean

#define RETURN_FLASE_IF_HRCALL_FAILED(a, errmsg) \
    m_HResult = (a); \
    if (FAILED(m_HResult)) return FALSE
 
#define HRCHECK(a, errmsg)		if (p) m_HResult = (a)

#define RETURN_IF_FALSE(a)		\
if ( p ) { \
    m_HResult = (a); \
    if (FAILED(m_HResult)) return FALSE;	\
} \
else {	\
	return FALSE;	\
}

#define HRCHECK_RETURN_BOOL(a)	\
if ( p ) { \
    m_HResult = (a); \
    return SUCCEEDED(m_HResult); \
} \
else {	\
	return FALSE;	\
}


#define AfxThrowComException(hr)

#define HRThrowComException(a)	\
if ( p ) { \
	m_HResult = (a); \
    if (FAILED(m_HResult)) throw(m_HResult);	\
}

#endif	// DEBUG

#if 0

#include <ole2.h>
#include <olectl.h>

#define ATLINLINE	inline
#define ATLAPI_(x)	x

//#define __deref_out_opt	
//#define __in_opt	

#define ATLASSERT(x)	

ATLAPI_(IUnknown*) AtlComPtrAssign(IUnknown** pp, IUnknown* lp);
ATLAPI_(IUnknown*) AtlComQIPtrAssign(IUnknown** pp, IUnknown* lp, REFIID riid);

#ifndef _ATL_DLL

ATLINLINE ATLAPI_(IUnknown*) AtlComPtrAssign(__deref_out_opt IUnknown** pp, __in_opt IUnknown* lp)
{
	if (pp == NULL)
		return NULL;
		
	if (lp != NULL)
		lp->AddRef();
	if (*pp)
		(*pp)->Release();
	*pp = lp;
	return lp;
}

ATLINLINE ATLAPI_(IUnknown*) AtlComQIPtrAssign(__deref_out_opt IUnknown** pp, __in_opt IUnknown* lp, REFIID riid)
{
	if (pp == NULL)
		return NULL;

	IUnknown* pTemp = *pp;
	*pp = NULL;
	if (lp != NULL)
		lp->QueryInterface(riid, (void**)pp);
	if (pTemp)
		pTemp->Release();
	return *pp;
}

#endif // _ATL_DLL

/////////////////////////////////////////////////////////////////////////////
// COM Smart pointers

template <class T>
class _NoAddRefReleaseOnCComPtr : public T
{
	private:
		STDMETHOD_(ULONG, AddRef)()=0;
		STDMETHOD_(ULONG, Release)()=0;
};

inline __checkReturn HRESULT AtlSetChildSite(__inout IUnknown* punkChild, __in_opt IUnknown* punkParent)
{
	if (punkChild == NULL)
		return E_POINTER;

	HRESULT hr;
	IObjectWithSite* pChildSite = NULL;
	hr = punkChild->QueryInterface(__uuidof(IObjectWithSite), (void**)&pChildSite);
	if (SUCCEEDED(hr) && pChildSite != NULL)
	{
		hr = pChildSite->SetSite(punkParent);
		pChildSite->Release();
	}
	return hr;
}


//CComPtrBase provides the basis for all other smart pointers
//The other smartpointers add their own constructors and operators
template <class T>
class CComPtrBase
{
protected:
	CComPtrBase() throw()
	{
		p = NULL;
	}
	CComPtrBase(__in int nNull) throw()
	{
		ATLASSERT(nNull == 0);
		(void)nNull;
		p = NULL;
	}
	CComPtrBase(__in_opt T* lp) throw()
	{
		p = lp;
		if (p != NULL)
			p->AddRef();
	}
public:
	typedef T _PtrClass;
	~CComPtrBase() throw()
	{
		if (p)
			p->Release();
	}
	operator T*() const throw()
	{
		return p;
	}
	T& operator*() const
	{
		ATLENSURE(p!=NULL);
		return *p;
	}
	//The assert on operator& usually indicates a bug.  If this is really
	//what is needed, however, take the address of the p member explicitly.
	T** operator&() throw()
	{
		ATLASSERT(p==NULL);
		return &p;
	}
	_NoAddRefReleaseOnCComPtr<T>* operator->() const throw()
	{
		ATLASSERT(p!=NULL);
		return (_NoAddRefReleaseOnCComPtr<T>*)p;
	}
	bool operator!() const throw()
	{
		return (p == NULL);
	}
	bool operator<(__in_opt T* pT) const throw()
	{
		return p < pT;
	}
	bool operator!=(__in_opt T* pT) const
	{
		return !operator==(pT);
	}
	bool operator==(__in_opt T* pT) const throw()
	{
		return p == pT;
	}

	// Release the interface and set to NULL
	void Release() throw()
	{
		T* pTemp = p;
		if (pTemp)
		{
			p = NULL;
			pTemp->Release();
		}
	}
	// Compare two objects for equivalence
	bool IsEqualObject(__in_opt IUnknown* pOther) throw()
	{
		if (p == NULL && pOther == NULL)
			return true;	// They are both NULL objects

		if (p == NULL || pOther == NULL)
			return false;	// One is NULL the other is not

		CComPtr<IUnknown> punk1;
		CComPtr<IUnknown> punk2;
		p->QueryInterface(__uuidof(IUnknown), (void**)&punk1);
		pOther->QueryInterface(__uuidof(IUnknown), (void**)&punk2);
		return punk1 == punk2;
	}
	// Attach to an existing interface (does not AddRef)
	void Attach(__in_opt T* p2) throw()
	{
		if (p)
			p->Release();
		p = p2;
	}
	// Detach the interface (does not Release)
	T* Detach() throw()
	{
		T* pt = p;
		p = NULL;
		return pt;
	}
	__checkReturn HRESULT CopyTo(__deref_out_opt T** ppT) throw()
	{
		ATLASSERT(ppT != NULL);
		if (ppT == NULL)
			return E_POINTER;
		*ppT = p;
		if (p)
			p->AddRef();
		return S_OK;
	}
	__checkReturn HRESULT SetSite(__in_opt IUnknown* punkParent) throw()
	{
		return AtlSetChildSite(p, punkParent);
	}
	__checkReturn HRESULT Advise(__in IUnknown* pUnk, __in const IID& iid, __out LPDWORD pdw) throw()
	{
		return AtlAdvise(p, pUnk, iid, pdw);
	}
	__checkReturn HRESULT CoCreateInstance(__in REFCLSID rclsid, __in_opt LPUNKNOWN pUnkOuter = NULL, __in DWORD dwClsContext = CLSCTX_ALL) throw()
	{
		ATLASSERT(p == NULL);
		return ::CoCreateInstance(rclsid, pUnkOuter, dwClsContext, __uuidof(T), (void**)&p);
	}
	__checkReturn HRESULT CoCreateInstance(__in LPCOLESTR szProgID, __in_opt LPUNKNOWN pUnkOuter = NULL, __in DWORD dwClsContext = CLSCTX_ALL) throw()
	{
		CLSID clsid;
		HRESULT hr = CLSIDFromProgID(szProgID, &clsid);
		ATLASSERT(p == NULL);
		if (SUCCEEDED(hr))
			hr = ::CoCreateInstance(clsid, pUnkOuter, dwClsContext, __uuidof(T), (void**)&p);
		return hr;
	}
	template <class Q>
	__checkReturn HRESULT QueryInterface(__deref_out_opt Q** pp) const throw()
	{
		ATLASSERT(pp != NULL);
		return p->QueryInterface(__uuidof(Q), (void**)pp);
	}
	T* p;
};

template <class T>
class CComPtr : public CComPtrBase<T>
{
public:
	CComPtr() throw()
	{
	}
	CComPtr(int nNull) throw() :
		CComPtrBase<T>(nNull)
	{
	}
	CComPtr(T* lp) throw() :
		CComPtrBase<T>(lp)

	{
	}
	CComPtr(__in const CComPtr<T>& lp) throw() :
		CComPtrBase<T>(lp.p)
	{
	}
	T* operator=(__in_opt T* lp) throw()
	{
        if(*this!=lp)
        {
    		return static_cast<T*>(AtlComPtrAssign((IUnknown**)&p, lp));
        }
        return *this;
	}
	template <typename Q>
	T* operator=(__in const CComPtr<Q>& lp) throw()
	{
        if( !IsEqualObject(lp) )
        {
    		return static_cast<T*>(AtlComQIPtrAssign((IUnknown**)&p, lp, __uuidof(T)));
        }
        return *this;
	}
	T* operator=(__in const CComPtr<T>& lp) throw()
	{
        if(*this!=lp)
        {
    		return static_cast<T*>(AtlComPtrAssign((IUnknown**)&p, lp));
        }
        return *this;
	}
};

#endif


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template<class T>
class  XML_CLASS CInterfaceCallingWrapper : public CComPtr<T>
{
// construction/destruction
public:
	CInterfaceCallingWrapper():CComPtr<T>(), m_HResult(NO_ERROR) {}

	template <typename DerivedType>
	CInterfaceCallingWrapper<T>(const CInterfaceCallingWrapper<DerivedType> &o) : CComPtr<T>() {
	#ifdef _DEBUG
		// You'll get an error here if you try to set this to an underived class.
//		Type *test_whether_it_is_really_derived = (DerivedType *) 0;
	#endif
		T	*lp = NULL;
		if ( o.IsValid() )
		{
			o.QueryInterface(&lp);

		//	if (lp != NULL)
		//		lp->AddRef();
		}
		p = lp;
	}

	/**
	Attaches the interface of the given wrapper to this object and
	increments the reference count of the interface.
	*/
	CInterfaceCallingWrapper(const CInterfaceCallingWrapper<T> &iw) : CComPtr<T>(iw), m_HResult(NO_ERROR) {}
/*
	CInterfaceCallingWrapper(const CInterfaceCallingWrapper<T> &lp) : CComPtr<T>(lp), m_HResult(NO_ERROR)  { 
		p = lp;
		if (p != NULL)
			p->AddRef();
	}
*/

	/**
	Attaches the given interface to this object.

	@param bIncrementRefCount
		Specify TRUE, if the reference count of the given interface
		should be incremented and FALSE otherwise.
	*/

	CInterfaceCallingWrapper(T *lp, bool bIncrementRefCount = TRUE) : CComPtr<T>(), m_HResult(NO_ERROR)
	{
		p = lp;
		if ( p && bIncrementRefCount ) p->AddRef();
	}

	/**
	Decrements the interfaces reference count.
	*/
	virtual ~CInterfaceCallingWrapper() { }

// operations
public:

	/** 
	Attaches the specified pointer.

	Will fail if there is already an interface attached to this 
	wrapper. Call Detach() first.

	@param bIncrementRefCount
		Specify TRUE, if the reference count of the given interface
		should be incremented and FALSE otherwise.

	@return
		TRUE if the interface has been attached to the wrapper, FALSE
		if there is already an interface attached.
	*/
	bool Attach(T *lp, bool bIncrementRefCount = FALSE)
	{ 
		if (!lp)
			return FALSE;

		CComPtr<T>::Attach(lp);
		if ( bIncrementRefCount ) lp->AddRef();

		return TRUE;
	}


	/**
	Returns the member pointer and detaches it from this object by 
	setting it to NULL.

	@param bIncrementRefCount
		Specify TRUE, if the reference count of the given interface
		should be decremented and FALSE otherwise.
	*/
	T* Detach(bool bDecrementRefCount = FALSE)
	{
		if (bDecrementRefCount)
		{
			T* lp = p;
			if ( lp )
				lp->Release();
		}
		return CComPtr<T>::Detach();
		
		/*
		T* lp = CComPtr<T>::Detach();
		if (bDecrementRefCount)
		{
			if ( lp )
				lp->Release();
		}

		return lp;
		*/
	}

	/**
	Copies the internal pointer to a target pointer. The pointer's 
	reference count is incremented if successful. Returns NULL if the
	wrapped interface pointer is NULL.
	*/
	
	T* Copy()
	{
		T	*lp = NULL;
		if (CComPtr<T>::CopyTo(&lp) == S_OK)
			return lp;
		else
			return NULL;
	}
	

	/**
	Returns the wrapped interface pointer.
	*/
//	T* P() { return p; }

	/**
	Returns the wrapped interface pointer as const.
	*/
//	const T* P() const { return p; }

	// Return true if this refers to interface pointer; false if NULL.
	operator bool () const { return (p != NULL); }

	bool IsValid() const { return p != NULL; }
//	bool IsInvalid() const { return p == NULL; }
	HRESULT GetResult() const { return m_HResult; }

protected:
	mutable HRESULT	m_HResult;
};


/**
Base class for all the classes used to implement an interface.
Supports basic IUnknown features like internal reference counting.
*/
class XML_CLASS CInterfaceImplementationWrapper
{
// construction/destruction
public:
	CInterfaceImplementationWrapper();
	virtual ~CInterfaceImplementationWrapper();

// overridables
public:
	virtual IUnknown* GetInterface() = 0;
	virtual ULONG STDMETHODCALLTYPE AddRef();
	virtual ULONG STDMETHODCALLTYPE Release();
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppv);

//protected:
//	virtual void MethodPrologue();

// attributes
private:
	ULONG m_ulRef;
};


#define INIT_COM()	
#define DONE_COM()	

/*
extern void InitCOM();
extern void DoneCOM();
*/


#endif	// __INTERFACE_WRAPPER_H__
