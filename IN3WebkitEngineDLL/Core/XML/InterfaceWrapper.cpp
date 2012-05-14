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

#include "stdafx.h"
#include "InterfaceWrapper.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>



#if defined(DEBUG) || defined(_DEBUG)

/*

void AfxThrowComException(HRESULT hr)
{
	dprintf( "%s:%d  HRCALL Failed: 0x%.8x\n",  __FILE__, __LINE__, hr );
}

*/
// Helper function that put output in debug window in Visual Studio 
// and stdout.
FUNC_DECL void dprintf( const char* format, ...)
{
#if 0

#if UNICODE_AWARE
	static char buf[8192];

	va_list args;
	va_start( args, format );
	vsprintf( buf, format, args );
	va_end( args);
	OutputDebugStringA(buf);
/*
	_bstr_t			bstr(format);
	CString str;
	va_list args;

    va_start( args, bstr );
  	str.FormatV(bstr, args);
	va_end( args);

    OutputDebugString(str);
*/
#else

	CString str;
	va_list args;

    va_start( args, format );
  	str.FormatV(format, args);
	va_end( args);

    OutputDebugString(str);

//	DebugBreak();
//	printf("%s", buf);

#endif	// UNICODE

#else

	static char buf[8192];

	va_list args;
	va_start( args, format );
	vsprintf( buf, format, args );
	va_end( args);
	OutputDebugStringA(buf);
//	printf("%s", buf);

#endif
}

#endif	// defined(DEBUG) || defined(_DEBUG)


//-------------------------------------------------------------------
// class CInterfaceImplementationWrapper
//-------------------------------------------------------------------

CInterfaceImplementationWrapper::CInterfaceImplementationWrapper()
: m_ulRef(0)
{
}


CInterfaceImplementationWrapper::~CInterfaceImplementationWrapper()
{
}


ULONG STDMETHODCALLTYPE CInterfaceImplementationWrapper::AddRef()
{
	return InterlockedIncrement((long*)&m_ulRef);
}


ULONG STDMETHODCALLTYPE CInterfaceImplementationWrapper::Release()
{
	ULONG	ulRef = InterlockedDecrement((long*)&m_ulRef);
	if (ulRef == 0)
		delete this;

	return ulRef;
}


HRESULT STDMETHODCALLTYPE CInterfaceImplementationWrapper::QueryInterface(REFIID riid, void **ppv)
{
	if (*ppv == NULL)
		return E_FAIL;

	if (riid == IID_IUnknown)
//		*ppv = dynamic_cast<IUnknown*>(this);
		*ppv = (void*)(this);
	else
		*ppv = NULL;

	if (*ppv == NULL)
		return E_NOINTERFACE;

	AddRef();
	return S_OK;
}

/*
void CInterfaceImplementationWrapper::MethodPrologue()
{
}
*/



/*

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
static ULONG	gComRef = 0;

void InitCOM()
{
	if ( gComRef == 0 )
	{
		HRESULT	result = ::CoInitialize(NULL);
		if ( (result != S_OK) && (result != S_FALSE) ) return;
	}
	
	gComRef++;	
}


void DoneCOM()
{
	if ( gComRef > 0 )
	{
		gComRef--;
		if ( gComRef == 0 )
		{
			::CoUninitialize();
		}
	}
}

*/