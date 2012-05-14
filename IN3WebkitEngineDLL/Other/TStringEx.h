/* ********************************************************************************
 *
 * Purpose:	MultiByte, UTF-16 Character Manager
 *
 * Version	: v1.0
 * Author	: IK HAN CHO
 * Copyright :(c) 2003 IKHAN CHO. All rights reserved.
 *
 * Modification History (most recent first):

 *			 1 DEC 2009		IK HAN CHO,  change Mac Carbon To CFString API for Unicode / MultiByte Encoding
 *			 1 MAR 2007		IK HAN CHO,  support Quark QXStringRef
 *			 1 JUN 2007		IK HAN CHO,  support Linus
 *			 1 JUN 2002		IK HAN CHO,  Redesign for Mac
 *			 8 JAN 2002		IK HAN CHO,  Redesign for Windows
 *
 * Known problem:
 *
 * Note :
 * ******************************************************************************* */

#ifndef __T_STRING_EX__
#define __T_STRING_EX__


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "TString.h"
#include <assert.h>
#include <stdio.h>
#include <stdarg.h>


#if defined(WIN32) || defined(_WIN32_WCE)
#ifndef _AFXDLL
#include <Windows.h>
#endif
#include <comutil.h>
#include <atlcomcli.h>
#elif defined(__APPLE_CPP__) || defined(__APPLE_CC__)
//#include <CoreServices/CoreServices.h>
#include <CoreFoundation/CoreFoundation.h>
#else
#include <ctype.h>
#include <wctype.h>
#endif


#if defined(WIN32) || defined(_WIN32_WCE)

#define TE_ANSI				0
#define TE_KOREAN			949			// Korea
#define TE_UTF8				65001		// CP_UTF8
#define TE_FS				GetACP()	// File System Encoding
#define DEFAULT_CP			GetACP()	// TE_KOREAN

#elif defined(__APPLE_CPP__) || defined(__APPLE_CC__)

#define TE_ANSI				0
#define TE_KOREAN			3			// kCFStringEncodingMacKorean
#define TE_UTF8				0x08000100	// kCFStringEncodingUTF8
#define TE_FS				0xFF		// kCFStringEncodingMacHFS, File System Encoding
#define DEFAULT_CP			TE_KOREAN	// CFStringGetSystemEncoding()

#else

#define TE_ANSI				0
#define TE_KOREAN			1
#define TE_UTF8				2
#define TE_FS				TE_UTF8		// File System Encoding
#define DEFAULT_CP			TE_KOREAN	// 

#endif


#define _UTF8_TEST_					0


// Creates an unicode string from a constant compile-time string.
#if defined(WIN32) || defined(_WIN32_WCE)
#define USTR(str)					L##str
#else
#define USTR(str)					(const unicode_t *)UniString(str)
#endif

// Creates an unicode string from a multi-byte string.
#define UCSTR(str)					(const unicode_t *)UniString(str)


typedef unsigned long	encoding_t;

class UniString;
class TMBString;

class UTF8String;	// special of TMBString, UTF8 String
class MBString;		// special of TMBString


// ------------------------------------------------------------------------------------------------

// TMBString
//
// ------------------------------------------------------------------------------------------------

class TMBString : public TString<char> {
public:
	TMBString(encoding_t encoding = DEFAULT_CP);
	TMBString(const TMBString &str);

	// Create a string from multibyte.
	TMBString(const char *lpsz, encoding_t encoding = DEFAULT_CP);
	TMBString(const unsigned char *lpsz, encoding_t encoding = DEFAULT_CP);
	TMBString(uint32_t nLength, const char *lpch, encoding_t encoding = DEFAULT_CP);

	// Create a string from unicode.
	TMBString(const TString<unicode_t> &uniString, encoding_t encoding = DEFAULT_CP);
	TMBString(const unicode_t *lpsz, encoding_t encoding = DEFAULT_CP);
	TMBString(uint32_t nCount, const unicode_t *lpch, encoding_t encoding = DEFAULT_CP);	

	uint32_t GetLength() const { return GetCount(); }

//	using TString<char>::operator =;
	const TMBString& operator =(char ch) { Init(ch); return *this; }
	const TMBString& operator =(const char *lpsz) {
		Init(lpsz, CH_LENGTH(lpsz));
		return *this;
	}
	const TMBString& operator =(const TMBString &str);
	const TMBString& operator =(const unsigned char * lpsz) {
		Init((const char *)lpsz, CH_LENGTH((const char *)lpsz));
		return *this;
	}
	const TMBString& operator =(const unicode_t * lpsz) {
		Empty(); Append(lpsz, CH_LENGTH(lpsz));
		return *this;
	}
	const TMBString& operator =(const TString<unicode_t> &uniString) {
		Empty(); Append(uniString.GetString(), uniString.GetCount());
		return *this;		
	}

	using TString<char>::operator +=;
//	const TMBString& operator +=(char ch) { Append(ch); return *this; }
//	const TMBString& operator +=(const char *lpsz) { Append(lpsz, CH_LENGTH(lpsz)); return *this; }
	const TMBString& operator +=(const TMBString &str);
	const TMBString& operator +=(const unsigned char * lpsz) {
		Append((const char *)lpsz, CH_LENGTH((const char *)lpsz));
		return *this;
	}
	const TMBString& operator +=(const unicode_t * lpsz) {
		Append(lpsz, CH_LENGTH(lpsz));
		return *this;
	}
	const TMBString& operator +=(const TString<unicode_t> &uniString) {
		Append(uniString.GetString(), uniString.GetCount());
		return *this;
	}
	
	using TString<char>::operator ==;
	bool operator ==(const unicode_t * lpsz) const;
	bool operator ==(const TString<unicode_t> &uniString) const;
	
	using TString<char>::operator !=;
	bool operator !=(const unicode_t * lpsz) const { return !operator ==(lpsz); }
	bool operator !=(const TString<unicode_t> &uniString) const { return !operator ==(uniString); }
	
	using TString<char>::Init;
	void Init(const unicode_t * lpch, uint32_t nCount) {
		Empty(); Append(lpch, nCount);
	}
	void Init(uint32_t nCount, const unicode_t * lpch) {
		Empty(); Append(lpch, nCount);
	}

	void Init(const unicode_t * lpsz) {
		Empty(); Append(lpsz, CH_LENGTH(lpsz));
	}
	void Init(const TString<unicode_t> &uniString) {
		Empty(); Append(uniString.GetString(), uniString.GetCount());
	}
	
	using TString<char>::Append;
	void Append(const unicode_t * lpch, uint32_t nCount);
	void Append(uint32_t nCount, const unicode_t * lpch) { Append(lpch, nCount); }

	void Append(const unicode_t * lpsz) { Append(lpsz, CH_LENGTH(lpsz)); }
	void Append(const TString<unicode_t> &uniString) { Append(uniString.GetString(), uniString.GetCount()); }
	
	uint32_t GetCodePage() const { return mEncoding; }
	uint32_t GetEncoding() const { return mEncoding; }
	
	void Format(const char * lpszFormat, ...);
	void FormatV(const char * lpszFormat, va_list argList);
	void Format(const unicode_t * lpszFormat, ...);
	void FormatV(const unicode_t * lpszFormat, va_list argList);
	
	TMBString& operator << (long val) {
		TMBString	str;
		str.Format("%d", val);
		Append(str);
	//	Format("%d", val);
		return *this;
	}

	int ToInt() const { 
		return mBuff ? atoi(mBuff) : 0;
	}

	double ToDouble() const { 
		return mBuff ? atof(mBuff) : 0;
	}

#if defined(_INC_COMUTIL)
	TMBString(const _bstr_t& bstr) { Init((const char *)bstr); }
	TMBString(const VARIANT& varSrc);

	const TMBString& operator =(const _bstr_t& s) {
		Init((const char *)s); return *this;
	}
	const TMBString& operator =(const VARIANT& varSrc);
#endif
	
protected:
	unsigned long mEncoding;
};


// ------------------------------------------------------------------------------------------------
//
// MBString
//
// ------------------------------------------------------------------------------------------------

class MBString : public TMBString {
public:
	MBString() : TMBString(DEFAULT_CP) { }
	MBString(const MBString &str) : TMBString(str) { }
	
	// Create a string from multibyte.
	MBString(const char *lpsz) : TMBString(lpsz, DEFAULT_CP) { }
	MBString(const unsigned char *lpsz) : TMBString(lpsz, DEFAULT_CP) { }
	MBString(const char *lpch, uint32_t nLength) : TMBString(nLength, lpch, DEFAULT_CP) { }
	MBString(uint32_t nLength, const char *lpch) : TMBString(nLength, lpch, DEFAULT_CP) { }
	
	// Create a string from unicode.
	MBString(const TString<unicode_t> &uniString) : TMBString(uniString, DEFAULT_CP) { }
	MBString(const unicode_t *lpsz) : TMBString(lpsz, DEFAULT_CP) { }
	MBString(const unicode_t *lpch, uint32_t nCount) : TMBString(nCount, lpch, DEFAULT_CP) { }
	MBString(uint32_t nCount, const unicode_t *lpch) : TMBString(nCount, lpch, DEFAULT_CP) { }
};


// ------------------------------------------------------------------------------------------------
//
// UTF8String
//
// ------------------------------------------------------------------------------------------------

class UTF8String : public TMBString {
public:
	UTF8String() : TMBString(TE_UTF8) { }
	UTF8String(const UTF8String &str) : TMBString(str) { }
	
	// Create a string from multibyte.
	UTF8String(const char *lpsz) : TMBString(lpsz, TE_UTF8) { }
	UTF8String(const unsigned char *lpsz) : TMBString(lpsz, TE_UTF8) { }
	UTF8String(const char *lpch, uint32_t nLength) : TMBString(nLength, lpch, TE_UTF8) { }
	UTF8String(uint32_t nLength, const char *lpch) : TMBString(nLength, lpch, TE_UTF8) { }
	
	// Create a string from unicode.
	UTF8String(const TString<unicode_t> &uniString) : TMBString(uniString, TE_UTF8) { }
	UTF8String(const unicode_t *lpsz) : TMBString(lpsz, TE_UTF8) { }
	UTF8String(const unicode_t *lpch, uint32_t nCount) : TMBString(nCount, lpch, TE_UTF8) { }
	UTF8String(uint32_t nCount, const unicode_t *lpch) : TMBString(nCount, lpch, TE_UTF8) { }
};

class UTF8StringK : public TMBString {
public:
	UTF8StringK() : TMBString(TE_KOREAN) { }
	UTF8StringK(const UTF8String &str) : TMBString(str) { }

	// Create a string from multibyte.
	UTF8StringK(const char *lpsz) : TMBString(lpsz, TE_KOREAN) { }
	UTF8StringK(const unsigned char *lpsz) : TMBString(lpsz, TE_KOREAN) { }
	UTF8StringK(const char *lpch, uint32_t nLength) : TMBString(nLength, lpch, TE_KOREAN) { }

	// Create a string from unicode.
	UTF8StringK(const TString<unicode_t> &uniString) : TMBString(uniString, TE_KOREAN) { }
	UTF8StringK(const unicode_t *lpsz) : TMBString(lpsz, TE_KOREAN) { }
	UTF8StringK(const unicode_t *lpch, uint32_t nCount) : TMBString(nCount, lpch, TE_KOREAN) { }

};



// ------------------------------------------------------------------------------------------------
//
// UniString
//
// ------------------------------------------------------------------------------------------------

class UniString : public TString<unicode_t> {
public:
	UniString(void);
	UniString(const UniString &str);

	// Create a string from unicode.
	UniString(const unicode_t * lpsz);
	UniString (const unicode_t *buff, uint32_t nCount);
	UniString (uint32_t nCount, const unicode_t *buff);
	
	// Create a string from multibyte.
	UniString(const TMBString &str);
	UniString(const char * lpsz);
	UniString(const char * lpch, uint32_t nLength, encoding_t encoding = DEFAULT_CP);
	UniString(uint32_t nLength, const char * lpch, encoding_t encoding = DEFAULT_CP);
	UniString(const unsigned char * lpsz);
	
	using TString<unicode_t>::operator =;
	const UniString& operator =(const TMBString &str) {
		Empty(); Append(str.GetString(), str.GetLength(), str.GetEncoding());
		return *this;
	}
	const UniString& operator =(const char * lpsz) {
		Empty(); Append(lpsz, CH_LENGTH(lpsz));
		return *this;
	}
	const UniString& operator =(const unsigned char * lpsz) {
		Empty(); Append((const char *)lpsz, CH_LENGTH((const char *)lpsz));
		return *this;
	}

	using TString<unicode_t>::operator +=;
	const UniString& operator +=(const TMBString &str) {
		Append(str.GetString(), str.GetLength(), str.GetEncoding());
		return *this;
	}
	const UniString& operator +=(const char * lpsz) {
		Append(lpsz, CH_LENGTH(lpsz));
		return *this;
	}
	const UniString& operator +=(const unsigned char * lpsz) {
		Append((const char *)lpsz, CH_LENGTH((const char *)lpsz));
		return *this;
	}

	using TString<unicode_t>::operator ==;
	bool operator ==(const char * lpsz) const { return IsEqual(UniString(lpsz)); }
	bool operator ==(const TMBString &str) const { return IsEqual(UniString(str)); }

	using TString<unicode_t>::operator !=;
	bool operator !=(const char * lpsz) const { return !operator ==(lpsz); }
	bool operator !=(const TMBString &str) const { return !operator ==(str); }

	using TString<unicode_t>::Init;
	void Init(const char * lpch, uint32_t nLength, encoding_t encoding = DEFAULT_CP) {
		Empty(); Append(lpch, nLength, encoding);
	}
	void Init(uint32_t nLength, const char * lpch, encoding_t encoding = DEFAULT_CP) {
		Empty(); Append(lpch, nLength, encoding);
	}

	void Init(const char * lpsz) {
		Empty(); Append(lpsz, CH_LENGTH(lpsz));
	}
	void Init(const TMBString &str) {
		Empty(); Append(str, str.GetLength(), str.GetEncoding());
	}
	using TString<unicode_t>::Append;
	void Append(const char * lpch, uint32_t nLength, encoding_t encoding = DEFAULT_CP);
	void Append(uint32_t nLength, const char * lpch, encoding_t encoding = DEFAULT_CP) {
		Append(lpch, nLength, encoding);
	}

	void Append(const char * lpsz) {
		Append(lpsz, CH_LENGTH(lpsz));
	}
	void Append(const TMBString &str) {
		Append(str, str.GetLength(), str.GetEncoding());
	}
	
	void Format(const char * lpszFormat, ...);
	void FormatV(const char * lpszFormat, va_list argList);
	void Format(const unicode_t * lpszFormat, ...);
	void FormatV(const unicode_t * lpszFormat, va_list argList);
	
	
#if defined(WIN32)
	UniString& operator << (long val) {
		UniString	str;
		str.Format(L"%d", val);
		Append(str);
	//	Format(L"%d", val);
		return *this;}
#else
	UniString& operator << (long val) {
		UniString	str;
		str.Format("%d", val);
		Append(str);
	//	Format("%d", val);
		return *this;
	}
#endif

	int ToInt() const;
	
	double ToDouble() const;

	void Composition();

#ifdef _INC_COMUTIL
	UniString(const _bstr_t& s) { Init((const unicode_t *)s); }
	UniString(const VARIANT& varSrc);

	const UniString& operator =(const _bstr_t& s) {
		Init((const unicode_t *)s); return *this;
	}
	const UniString& operator =(const VARIANT& varSrc);
#endif
};


#endif	// __T_STRING_EX__
