/* ********************************************************************************
 *
 * Purpose:	MultiByte, UTF-16 Character Manager
 *
 * Version	: v1.0
 * Author	: IK HAN CHO
 * Copyright :(c) 2003 IKHAN CHO. All rights reserved.
 *
 * Modification History (most recent first):

 *			 1 DEC 2009		IK HAN CHO,  change Mac Carbon To CFString for Unicode / MultiByte Encoding
 *			 1 MAR 2007		IK HAN CHO,  support Quark QXStringRef
 *			 1 JUN 2007		IK HAN CHO,  support Linus
 *			 1 JUN 2002		IK HAN CHO,  Redesign for Mac
 *			 8 JAN 2002		IK HAN CHO,  Redesign for Windows
 *
 * Known problem:
 *
 * Note :
 * ******************************************************************************* */

#include "stdafx.h"

#if defined(WIN32) || defined(_WIN32_WCE)
#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif

#ifndef _CRT_NON_CONFORMING_SWPRINTFS
#define _CRT_NON_CONFORMING_SWPRINTFS
#endif
#endif	// WIN32

#include "TStringEx.h"

#include <assert.h>
#include <wchar.h>

#if defined(_WIN32_WCE)
#include <malloc.h>
#elif defined(WIN32)
#include <malloc.h>
#include <mbstring.h>
#else
#include <alloca.h>
#endif


/* Some fundamental constants */
#define UNI_REPLACEMENT_CHAR	(unsigned long)0x0000FFFD
#define UNI_MAX_BMP				(unsigned long)0x0000FFFF
#define UNI_MAX_UTF16			(unsigned long)0x0010FFFF
#define UNI_MAX_UTF32			(unsigned long)0x7FFFFFFF

#define UNI_MAX_LEGAL_UTF32		(unsigned long)0x0010FFFF


static const int halfShift  = 10; /* used for shifting by 10 bits */

static const unsigned long halfBase = 0x0010000UL;
static const unsigned long halfMask = 0x3FFUL;

#define UNI_SUR_HIGH_START  (unsigned long)0xD800
#define UNI_SUR_HIGH_END    (unsigned long)0xDBFF
#define UNI_SUR_LOW_START   (unsigned long)0xDC00
#define UNI_SUR_LOW_END     (unsigned long)0xDFFF


/* --------------------------------------------------------------------- */
//
//	UTF8
//

/*
 * Index into the table below with the first byte of a UTF-8 sequence to
 * get the number of trailing bytes that are supposed to follow it.
 * Note that *legal* UTF-8 values can't have 4 or 5-bytes. The table is
 * left as-is for anyone who may want to do such conversion, which was
 * allowed in earlier algorithms.
 */
static const char gTrailingBytesForUTF8[256] = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, 3,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5
};

/*
 * Magic values subtracted from a buffer value during UTF8 conversion.
 * This table contains as many values as there might be trailing bytes
 * in a UTF-8 sequence.
 */
static const unsigned long gOffsetsFromUTF8[6] = { 0x00000000UL, 0x00003080UL, 0x000E2080UL, 
					 0x03C82080UL, 0xFA082080UL, 0x82082080UL };

/*
 * Once the bits are split out into bytes of UTF-8, this is a mask OR-ed
 * into the first byte, depending on how many bytes follow.  There are
 * as many entries in this table as there are UTF-8 sequence types.
 * (I.e., one byte sequence, two byte... etc.). Remember that sequencs
 * for *legal* UTF-8 will be 4 or fewer bytes total.
 */
static const unsigned char gFirstByteMark[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };



/* ---------------------------------------------------------------------
	Legal UTF-8 sequences are:

	1st----	2nd----	3rd----	4th----	Codepoints---

	00-7F				            0000-  007F
	C2-DF	80-BF			        0080-  07FF
	E0      A0-BF	80-BF		    0800-  0FFF
	E1-EC   80-BF   80-BF           1000-  CFFF
	ED      80-9F   80-BF           D000-  D7FF
	EE-EF   80-BF   80-BF           E000-  FFFF
	F0	    90-BF	80-BF	80-BF	10000- 3FFFF
	F1-F3	80-BF	80-BF	80-BF	40000- FFFFF
	F4	    80-8F	80-BF	80-BF	100000-10FFFF

   --------------------------------------------------------------------- */

static bool IsLegalUTF8(const unsigned char *source, int length)
{
	unsigned char a;
	const unsigned char *srcptr = source + length;
	switch (length)
	{
		/* Everything else falls through when "true"... */
		case 4: if ((a = (*--srcptr)) < 0x80 || a > 0xBF) return false;
		case 3: if ((a = (*--srcptr)) < 0x80 || a > 0xBF) return false;
		case 2: if ((a = (*--srcptr)) > 0xBF) return false;
			switch (*source)
			{
			/* no fall-through in this inner switch */
			case 0xE0: if (a < 0xA0) return false; break;
			case 0xED: if (a > 0x9F) return false; break;	//+
			case 0xF0: if (a < 0x90) return false; break;
			case 0xF4: if (a > 0x8F) return false; break;
			default:   if (a < 0x80) return false;
			}
    	case 1: if (*source >= 0x80 && *source < 0xC2) return false;
		break;

		default: return false;
	}
	
	if (*source > 0xF4) return false;
	return true;
}


/*
 * Exported function to return whether a UTF-8 sequence is legal or not.
 * This is not used here; it's just exported.
 */
bool isLegalUTF8Sequence(const unsigned char *source, const unsigned char *sourceEnd)
{
    int length = gTrailingBytesForUTF8[*source]+1;
    if (source+length > sourceEnd)
	{
		return false;
    }
    return IsLegalUTF8(source, length);
}


void UTF16ToUTF8(const unicode_t* pUTF16, size_t nCount, TString<char> &outString)
{
//	ConversionResult result = ok;
	const unicode_t byteMask = 0xBF;
	const unicode_t byteMark = 0x80; 

	unicode_t* source = (unicode_t *)pUTF16;
	unicode_t* sourceEnd = source + nCount;
	unsigned long	bytesToWrite = 0;
	unsigned long	ch;
	unsigned char*	target;
	unsigned char	utf8Buff[20];

	while (source < sourceEnd)
	{
		ch = *source++;
		target = utf8Buff;

		/* If we have a surrogate pair, convert to unsigned long first. */
		if (ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_HIGH_END)
		{
			/* If the 16 bits following the high surrogate are in the source buffer... */
			if (source < sourceEnd)
			{
				unsigned long ch2 = *source;
				/* If it's a low surrogate, convert to unsigned long. */
				if (ch2 >= UNI_SUR_LOW_START && ch2 <= UNI_SUR_LOW_END)
				{
					ch = ((ch - UNI_SUR_HIGH_START) << halfShift)
					+ (ch2 - UNI_SUR_LOW_START) + halfBase;
					++source;
				}
				else // if (flags == strictConversion)
				{ // it's an unpaired high surrogate
				//	--source; // return to the illegal value itself
				//	result = sourceIllegal;
					break;
				}
			}
			else
			{ // We don't have the 16 bits following the high surrogate.
			//	--source; // return to the high surrogate
			//	result = sourceExhausted;
				break;
			}
		}
		else // if (flags == strictConversion)
		{
			/* UTF-16 surrogate values are illegal in UTF-32 */
			if (ch >= UNI_SUR_LOW_START && ch <= UNI_SUR_LOW_END)
			{
			//	--source; // return to the illegal value itself
			//	result = sourceIllegal;
			break;
			}
		}

		/* Figure out how many bytes the result will require */
		if (ch < (unsigned long)0x80)
		{
			bytesToWrite = 1;
		}
		else if (ch < (unsigned long)0x800)
		{		
			bytesToWrite = 2;
		}
		else if (ch < (unsigned long)0x10000) 
		{	
			bytesToWrite = 3;
		}
		else  if (ch <= UNI_MAX_LEGAL_UTF32) 
		{	
			bytesToWrite = 4;
		}
		else
		{
			bytesToWrite = 3;
			ch = UNI_REPLACEMENT_CHAR;
		}
		
		target += bytesToWrite;

		switch (bytesToWrite)
		{
			case 4:
				*--target = (unsigned char)((ch | byteMark) & byteMask);
				ch >>= 6;
			case 3:
				*--target = (unsigned char)((ch | byteMark) & byteMask);
				ch >>= 6;
			case 2:
				*--target = (unsigned char)((ch | byteMark) & byteMask);
				ch >>= 6;
			case 1:
				*--target =  (unsigned char)(ch | gFirstByteMark[bytesToWrite]);
		}
		
		outString.Append((char *)utf8Buff, bytesToWrite);
	}
}


void UTF8ToUTF16(const char* lpch, size_t nLength, TString<unicode_t> &outString)
{
//	ConversionResult result = conversionOK;
	unsigned char*	source = (unsigned char *)lpch;
	unsigned char*	sourceEnd = source + nLength;

	while (source < sourceEnd)
	{
		unsigned long  ch = 0;
		unsigned short extraBytesToRead = gTrailingBytesForUTF8[*source];

		if ((source + extraBytesToRead) >= sourceEnd)
		{
		//	result = sourceExhausted;
			break;
		}

		//Do this check whether lenient or strict
		if (!IsLegalUTF8(source, extraBytesToRead+1))
		{
		//	result = sourceIllegal;
			break;
		}

		/*
		 * The cases all fall through. See "Note A" below.
		 */

		switch (extraBytesToRead)
		{
			case 5: ch += *source++; ch <<= 6; /* remember, illegal UTF-8 */
			case 4: ch += *source++; ch <<= 6; /* remember, illegal UTF-8 */
			case 3:	ch += *source++; ch <<= 6;
			case 2:	ch += *source++; ch <<= 6;
			case 1:	ch += *source++; ch <<= 6;
			case 0:	ch += *source++;
		}

		ch -= gOffsetsFromUTF8[extraBytesToRead];

		if (ch <= UNI_MAX_BMP )	/* Target is a character <= 0xFFFF */
		{
			/* UTF-16 surrogate values are illegal in UTF-32 */
		#if 0
			if (ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_LOW_END)
			{
				/*
				if (flags == strictConversion)
				{
					source -= (extraBytesToRead+1); // return to the illegal value itself
					result = sourceIllegal;
					break;
				}
				else
				*/
				{
					outString.Append(UNI_REPLACEMENT_CHAR);
				}
			}
			else
			{
				outString.Append((unsigned short)ch);
			}
		#else
			outString.Append((unsigned short)ch);
		#endif
		}
		else if ( ch <= UNI_MAX_UTF16 )
		{
		/*
			if (target + 1 >= targetEnd)
			{
				source -= (extraBytesToRead+1); // Back up source pointer!
				result = targetExhausted;
				break;
			}
		*/
			ch -= halfBase;
		//	unsigned short hi = (unsigned short)(ch / 0x400 + UNI_SUR_HIGH_START);
		//	unsigned short low = (unsigned short)(ch % 0x400 + UNI_SUR_LOW_START);
			unsigned short hi = (unsigned short)((ch >> halfShift) + UNI_SUR_HIGH_START);
			unsigned short low = (unsigned short)((ch & halfMask) + UNI_SUR_LOW_START);

			outString.Append(hi);
			outString.Append(low);
		}
		else
		{
		/*
			if (flags == strictConversion)
			{
				result = sourceIllegal;
				source -= (extraBytesToRead+1); // return to the start
				break; // Bail out; shouldn't continue
			}
		*/
			/* i.e., ch > UNI_MAX_UTF32 */
			outString.Append(UNI_REPLACEMENT_CHAR);
		}
	}
}



#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif


#ifdef _X86_
#define DOUBLE_ARG  double	//_AFX_DOUBLE
#else
#define DOUBLE_ARG  double
#endif


#if defined(WIN32) || defined(_WIN32_WCE)
#define INT64_T		__int64
#else
#define	INT64_T		int64_t
#endif

#define FORCE_ANSI      0x10000
#define FORCE_UNICODE   0x20000
#define FORCE_INT64     0x40000

#if defined(WIN32) || defined(_WIN32_WCE)
#define WCHAR_ARG   wchar_t
#define CHAR_ARG    char
#elif defined(__APPLE_CPP__) || defined(__APPLE_CC__)
#define WCHAR_ARG   int
#define CHAR_ARG    int
#else
#define WCHAR_ARG   int
#define CHAR_ARG    int
#endif

#if defined(_WIN32_WCE)

#define MBCHARLEN(_pc)			( *(_pc) > 0x80 ? 2 : 1 )
#define MBSTRINC(_pc)			( *(_pc) > 0x80 ? (_pc)+2 : (_pc)+1 )
#define MBSTRLEN(_pc)			( ((_pc) != NULL) ? ::strlen(_pc) : 0 )	// ::lstrlenW(_pc)
#define MBSTRNCMP(a, b, n)		strncmp((const char *)(a), (const char *)(b), n)

#elif defined(WIN32)

#define MBCHARLEN(_pc)			_mbclen(_pc)
#define MBSTRINC(_pc)			_mbsinc(_pc)
#define MBSTRLEN(_pc)			::lstrlenA(_pc)
#define MBSTRNCMP(a, b, n)		_mbsnbcmp((const unsigned char *)(a), (const unsigned char *)(b), n)

#else

#define MBCHARLEN(_pc)			( *(_pc) > 0xA0 ? 2 : 1 )
#define MBSTRINC(_pc)			( *(_pc) > 0xA0 ? (_pc)+2 : (_pc)+1 )
#define MBSTRLEN(_pc)			( ((_pc) != NULL) ? ::strlen(_pc) : 0 )
#define MBSTRNCMP(a, b, n)		strncmp((const char *)(a), (const char *)(b), n)

#endif


/*
enum {
	size_of_wchar_t = sizeof(wchar_t)
};
 
#if defined(WIN32) || defined(_WIN32_WCE) || size_of_wchar_t == 2
#define WCSTRLEN(_pc)		wcslen(_pc)
#else
#define WCSTRLEN(_pc)		CH_LENGTH(_pc)
#endif
*/


#if defined(WIN32) || defined(_WIN32_WCE)
#define WCSTRLEN(_pc)		wcslen(_pc)
#elif defined(__APPLE_CPP__) || defined(__APPLE_CC__) || defined(__GNUC__)
#define WCSTRLEN(_pc)		CH_LENGTH(_pc)
#else
#define WCSTRLEN(_pc)		CH_LENGTH(_pc)
#endif

#define WCSNCMP(s1,s2,n)	wcsncmp(s1, s2, n)

#if defined(WIN32) || defined(_WIN32_WCE)
#define WTOI(a)				_wtoi((const wchar_t *)a)
#define WTOF(a)				_wtof((const wchar_t *)(a))
#elif defined(__APPLE_CPP__) || defined(__APPLE_CC__) || defined(__GNUC__)
inline int WTOI(const unicode_t *str)
{
	MBString	mbStr(str);
	
	return atoi(mbStr.GetString());
}

inline double WTOI(const unicode_t *str)
{
	MBString	mbStr(str);
	
	return atof(mbStr.GetString());
}

#else
#define WTOI(a)				wcstol(a, NULL, 10)
#endif


#define WCSTRINC(_pc)		((_pc)+1)	// _wcsinc(_pc)
#define WCCHARLEN(_pc)		(1)			// _tclen(_pc)



// ------------------------------------------------------------------------------------------------
//
// UniString
//
// ------------------------------------------------------------------------------------------------
UniString::UniString(void)
: TString<unicode_t>() 
{
}

UniString::UniString(const UniString &str)
: TString<unicode_t>(str)
{
}

// Create a string from unicode.
UniString::UniString(const unicode_t * lpsz)
: TString<unicode_t>(lpsz)
{
}

UniString::UniString (const unicode_t *buff, uint32_t nCount)
: TString<unicode_t>(buff, nCount)
{
}

UniString::UniString (uint32_t nCount, const unicode_t *buff)
: TString<unicode_t>(buff, nCount)
{
}

// Create a string from multibyte.
UniString::UniString(const TMBString &str)
: TString<unicode_t>()
{
	Append(str.GetString(), str.GetLength(), str.GetEncoding());
}

UniString::UniString(const char * lpsz)
: TString<unicode_t>()
{
	Append(lpsz, CH_LENGTH(lpsz));
}

UniString::UniString(const char * lpch, uint32_t nLength, encoding_t encoding/* = DEFAULT_CP*/)
: TString<unicode_t>()
{
	Append(lpch, nLength, encoding);
}

UniString::UniString(uint32_t nLength, const char * lpch, encoding_t encoding/* = DEFAULT_CP*/)
: TString<unicode_t>()
{
	Append(lpch, nLength, encoding);
}

UniString::UniString(const unsigned char * lpsz)
: TString<unicode_t>()
{
	Append((const char *)lpsz, CH_LENGTH((const char *)lpsz));
}


void UniString::Append(const char *lpch, uint32_t nLength, encoding_t encoding/* = DEFAULT_CP*/)
{
	if ( nLength == 0 || lpch == NULL ) return;

#if _UTF8_TEST_
	if ( codePage == TE_UTF8 )
	{
		UTF8ToUTF16(lpch, nLength, *this);
		return;
	}
#endif

#if defined(WIN32) || defined(_WIN32_WCE)
	
	int nSize = ::MultiByteToWideChar((UINT)encoding, 0, lpch, (int)nLength, NULL, 0);
	if ( nSize )
	{
		uint32_t	nOldCount = mCount;
		uint32_t	newCount = nOldCount + nSize;
		if ( newCount > mCapacity )
		{
			if ( !SetCapacity(newCount) )
				return;
		}

		MultiByteToWideChar((UINT)encoding, 0, lpch, (int)nLength, mBuff + nOldCount, nSize);
		mCount = newCount;
		mBuff[mCount] = 0;
	}
	
#elif defined(__APPLE_CPP__) || defined(__APPLE_CC__)
	
//	CFStringRef	cfStringRef = CFStringCreateWithBytesNoCopy(kCFAllocatorDefault, (const UInt8 *)lpch, nLength, (CFStringEncoding)encoding, false, NULL);
	CFStringRef	cfStringRef = CFStringCreateWithBytes(kCFAllocatorDefault, (const UInt8 *)lpch, nLength, (CFStringEncoding)encoding, false);
	if ( cfStringRef )
	{
		CFIndex		uniCharCount = CFStringGetLength(cfStringRef);
		uint32_t	nOldCount = mCount;
		uint32_t	newCount = nOldCount + uniCharCount;
		if ( newCount > mCapacity )
		{
			if ( !SetCapacity(newCount) ) return;
		}
		
		CFRange	range;
		range.location = 0;
		range.length = uniCharCount;
		CFStringGetCharacters(cfStringRef, range, (UniChar *)(mBuff + nOldCount));
		mCount = newCount;
		mBuff[mCount] = 0;
		
		CFRelease(cfStringRef);
	}
													  
/*
	TextEncoding		mFromEncoding = CreateTextEncoding(codePage, kTextEncodingDefaultVariant, kTextEncodingDefaultFormat);
//	TextEncoding		mToEncoding = CreateTextEncoding(kTextEncodingUnicodeV2_0, kTextEncodingDefaultVariant, kTextEncodingDefaultFormat);
//	TextEncoding		mToEncoding = CreateTextEncoding(kTextEncodingUnicodeV3_1, kTextEncodingDefaultVariant, kTextEncodingDefaultFormat);
//	TextEncoding		mToEncoding = CreateTextEncoding(kTextEncodingUnicodeDefault, kTextEncodingDefaultVariant, kTextEncodingDefaultFormat);
	TextEncoding		mToEncoding = CreateTextEncoding(kTextEncodingUnicodeV3_2, kTextEncodingDefaultVariant, kTextEncodingDefaultFormat);
	UnicodeMapping		theMapping;
	TextToUnicodeInfo	mTextToUnicodeInfo = NULL;
	OSStatus			mStatus;
	theMapping.unicodeEncoding = mToEncoding;
	theMapping.otherEncoding = mFromEncoding;
	theMapping.mappingVersion = kUnicodeUseLatestMapping;
	
	mStatus = CreateTextToUnicodeInfo(&theMapping, &mTextToUnicodeInfo);
	
	if ( mTextToUnicodeInfo == NULL ) return;

	const char		*charPtr = lpch;
	const char		*charEndPtr = lpch + nLength;
	long			charLen = nLength;
	ByteCount       oSourceRead;
	ByteCount		oOutputLen;	
	unicode_t*		buffPtr;
	uint32_t		totalUniCharCount = 0;
	uint32_t		capacity;
	
	while ( charPtr < charEndPtr )
	{
		capacity = totalUniCharCount + charLen;
		if ( capacity > mCapacity )
		{
			if ( !SetCapacity(capacity) ) break;
		}
		
		buffPtr = mBuff + totalUniCharCount;		
		oOutputLen = 0;
		oSourceRead = 0;

		mStatus = ConvertFromTextToUnicode(mTextToUnicodeInfo, charLen, (ConstLogicalAddress)charPtr,
										   0, 0, NULL, NULL, NULL, (charLen * 2), &oSourceRead, &oOutputLen, (UniCharArrayPtr)buffPtr);
		totalUniCharCount += oOutputLen / 2;
		charPtr += oSourceRead;
		charLen -= oSourceRead;
	}
	
	DisposeTextToUnicodeInfo( &mTextToUnicodeInfo );

	mCount = totalUniCharCount;
	mBuff[mCount] = 0;
*/
#else

	if ( encoding == TE_ANSI )
	{
		const char *s = lpch;
		const char *e = s + nLength;

		for ( ; s < e; s++ )
		{
			Append(*s);
		}
	}
	else if ( encoding == TE_KOREAN )
	{
		const unsigned char *s = (const unsigned char *)lpch;
		const unsigned char *e = s + nLength;

		for ( ; s < e; s++ )
		{
			if ( *s < 0x80 )
			{
				Append(*s);
			}
			else
			{
			//	unsigned short ksCode = *s;
			//	s++;
			//	ksCode = (ksCode << 8) | *s;
			//	unsigned short unicode = KsCode2Unicode(ksCode);
			//	Append(unicode);

			//	Append( KsCode2Unicode(((ksCode << 8) | *s)) );

				Append(*s);
			}
		}
	}
	else if ( encoding == TE_UTF8 )
	{
		UTF8ToUTF16(lpch, nLength, *this);
	}

#endif	
}


void UniString::FormatV(const char * lpszFormat, va_list argList)
{
	MBString	str;
	
	str.FormatV(lpszFormat, argList);
	Init(str.GetString(), str.GetCount());
}


void UniString::Format(const char * lpszFormat, ...)
{
	va_list argList;
	
	va_start(argList, lpszFormat);
	FormatV(lpszFormat, argList);
	va_end(argList);
}

void UniString::Format(const unicode_t * lpszFormat, ...)
{
	va_list argList;
	
	va_start(argList, lpszFormat);
	FormatV(lpszFormat, argList);
	va_end(argList);
}

void UniString::FormatV(const unicode_t * lpszFormat, va_list argList)
{
// gcc and macOS: 1 
// windows : 0
// sizeof(wchar_t) : 4   -> 1로 설정
// sizeof(wchar_t) : 2   -> 0로 설정
#if !defined(WIN32)	

	assert(lpszFormat != NULL);
	
	va_list argListSave = argList;
	
	// make a guess at the maximum length of the resulting string
//	int nMaxLen = 0;
	const unicode_t * pStart = lpszFormat;
	for (const unicode_t * lpsz = lpszFormat; *lpsz != '\0'; lpsz = WCSTRINC(lpsz))
	{
		if (*lpsz == '%' && *(WCSTRINC(lpsz)) == '%')
		{
			Append(pStart, (TSCount_t)(lpsz - pStart));
			lpsz = WCSTRINC(lpsz);
			pStart = lpsz;
			continue;
		}

		// handle '%' character, but watch out for '%%'
		if (*lpsz != '%' || *(lpsz = WCSTRINC(lpsz)) == '%')
		{
		//	nMaxLen += WCCHARLEN(lpsz);
			continue;
		}
		
		const unicode_t *pFormatStart = lpsz - 1;	// '%' character 嚥???뽰삂.
		Append(pStart, (TSCount_t)(pFormatStart - pStart));
		pStart = lpsz;
		
		int nItemLen = 0;
		
		// handle '%' character with format
		int nWidth = 0;
		for (; *lpsz != '\0'; lpsz = WCSTRINC(lpsz))
		{
			// check for valid flags
			if (*lpsz == '#')
				;	// nMaxLen += 2;   // for '0x'
			else if (*lpsz == '*')
				nWidth = va_arg(argList, int);
			else if (*lpsz == '-' || *lpsz == '+' || *lpsz == '0' || *lpsz == ' ')
				;
			else // hit non-flag character
				break;
		}
		// get width and skip it
		if (nWidth == 0)
		{
		#if defined(__APPLE_CPP__) || defined(__APPLE_CC__) || defined(__GNUC__)
			TString<char>	mbStr;
			for (; *lpsz != '\0' && iswdigit(*lpsz); lpsz = WCSTRINC(lpsz))
			{
				mbStr += *lpsz;
			}
			if ( !mbStr.IsEmpty() )
			{
				nWidth = atoi(mbStr.GetString());
			}
		#else		
			// width indicated by
			nWidth = WTOI(lpsz);
			for (; *lpsz != '\0' && iswdigit(*lpsz); lpsz = WCSTRINC(lpsz))
				;
		#endif
		}
		assert(nWidth >= 0);
		
		int nPrecision = 0;
		if (*lpsz == '.')
		{
			// skip past '.' separator (width.precision)
			lpsz = WCSTRINC(lpsz);
			
			// get precision and skip it
			if (*lpsz == '*')
			{
				nPrecision = va_arg(argList, int);
				lpsz = WCSTRINC(lpsz);
			}
			else
			{
			#if defined(__APPLE_CPP__) || defined(__APPLE_CC__) || defined(__GNUC__)
				TString<char>	mbStr;
				for (; *lpsz != '\0' && iswdigit(*lpsz); lpsz = WCSTRINC(lpsz))
				{
					mbStr += *lpsz;
				}

				if ( !mbStr.IsEmpty() )
				{
					nPrecision = atoi(mbStr.GetString());
				}
			#else
				nPrecision = WTOI(lpsz);
				for (; *lpsz != '\0' && iswdigit(*lpsz); lpsz = WCSTRINC(lpsz))
					;
			#endif
			}
			assert(nPrecision >= 0);
		}
		
		// should be on type modifier or specifier
		int nModifier = 0;
		if ( UniString("I64").CompareNI(0, lpsz, 3) == 0 ) // if (WCSNCMP(lpsz, WSTR("I64"), 3) == 0)
		{
			lpsz += 3;
			nModifier = FORCE_INT64;
		#if !defined(_X86_) && !defined(_ALPHA_)
			// __int64 is only available on X86 and ALPHA platforms
			assert(FALSE);
		#endif
		}
		else
		{
			switch (*lpsz)
			{
				// modifiers that affect size
				case 'h':
					nModifier = FORCE_ANSI;
					lpsz = WCSTRINC(lpsz);
					break;
				case 'l':
					nModifier = FORCE_UNICODE;
					lpsz = WCSTRINC(lpsz);
					break;
					
				// modifiers that do not affect size
				case 'F':
				case 'N':
				case 'L':
					lpsz = WCSTRINC(lpsz);
					break;
			}
		}
		
		// now should be on specifier
		switch (*lpsz | nModifier)
		{
			// single characters
			case 'c':
			case 'C':
				nItemLen = 2;
				if ( nWidth > 1 )
					Fill(' ', nWidth - 1);
				Append( va_arg(argList, WCHAR_ARG) );
				break;
			case 'c'|FORCE_ANSI:
			case 'C'|FORCE_ANSI:
				nItemLen = 2;
				if ( nWidth > 1 )
					Fill(' ', nWidth - 1);
				Append( va_arg(argList, CHAR_ARG) );
				break;
			case 'c'|FORCE_UNICODE:
			case 'C'|FORCE_UNICODE:
				nItemLen = 2;
				if ( nWidth > 1 )
					Fill(' ', nWidth - 1);
				Append( va_arg(argList, WCHAR_ARG) );
				break;
				
			// strings
			case 's':
			{
			#if 0
				const char * pstrNextArg = va_arg(argList, const char *);
				if (pstrNextArg == NULL)
				{
					if ( nWidth > 6 )
						Fill(' ', nWidth - 6);
					Append("(null)");
					nItemLen = 6; // "(null)"
				}
				else
				{
					nItemLen = MBSTRLEN(pstrNextArg);
					if ( nWidth > nItemLen )
						Fill(' ', nWidth - nItemLen);
					Append(pstrNextArg);
					nItemLen = max(1, nItemLen);
				}
			#else
				const unicode_t *pstrNextArg = va_arg(argList, const unicode_t *);
				if (pstrNextArg == NULL)
				{
					nItemLen = 6;  // "(null)"
					if ( nWidth > 6 )
						Fill(' ', nWidth - 6);
					Append("(null)");
				}
				else
				{
					nItemLen = WCSTRLEN(pstrNextArg);
					if ( nWidth > nItemLen )
						Fill(' ', nWidth - nItemLen);
					Append(pstrNextArg);
					nItemLen = max(1, nItemLen);
				}
			#endif
			}
				break;
				
			case 'S':
			{
			#if 0	// #ifndef _UNICODE
				const unicode_t * pstrNextArg = va_arg(argList, const unicode_t *);
				if (pstrNextArg == NULL)
				{
					if ( nWidth > 6 )
						Fill(' ', nWidth - 6);
					Append("(null)");
					nItemLen = 6;  // "(null)"
				}
				else
				{
					nItemLen = WCSTRLEN(pstrNextArg);
					if ( nWidth > nItemLen )
						Fill(' ', nWidth - nItemLen);
					Append(pstrNextArg);
					nItemLen = max(1, nItemLen);
				}
			#else
				const char * pstrNextArg = va_arg(argList, const char *);
				if (pstrNextArg == NULL)
				{
					if ( nWidth > 6 )
						Fill(' ', nWidth - 6);
					Append("(null)");
					nItemLen = 6; // "(null)"
				}
				else
				{
					nItemLen = MBSTRLEN(pstrNextArg);
					if ( nWidth > nItemLen )
						Fill(' ', nWidth - nItemLen);
					Append(pstrNextArg);
					nItemLen = max(1, nItemLen);
				}
			#endif
			}
				break;
				
			case 's'|FORCE_ANSI:
			case 'S'|FORCE_ANSI:
			{
				const char * pstrNextArg = va_arg(argList, const char *);
				if (pstrNextArg == NULL)
				{
					if ( nWidth > 6 )
						Fill(' ', nWidth - 6);
					Append("(null)");
					nItemLen = 6; // "(null)"
				}
				else
				{
					nItemLen = MBSTRLEN(pstrNextArg);
					if ( nWidth > nItemLen )
						Fill(' ', nWidth - nItemLen);
					Append(pstrNextArg);
					nItemLen = max(1, nItemLen);
				}
			}
				break;
				
			case 's'|FORCE_UNICODE:
			case 'S'|FORCE_UNICODE:
			{
				const unicode_t * pstrNextArg = va_arg(argList, const unicode_t *);
				if (pstrNextArg == NULL)
				{
					if ( nWidth > 6 )
						Fill(' ', nWidth - 6);
					Append("(null)");
					nItemLen = 6; // "(null)"
				}
				else
				{
					nItemLen = WCSTRLEN(pstrNextArg);
					if ( nWidth > nItemLen )
						Fill(' ', nWidth - nItemLen);
					Append(pstrNextArg);
					nItemLen = max(1, nItemLen);
				}
			}
				break;
		}
		 
		// adjust nItemLen for strings
		if (nItemLen != 0)
		{
		//	if (nPrecision != 0)
		//		nItemLen = min(nItemLen, nPrecision);
		//	nItemLen = max(nItemLen, nWidth);
		}
		else
		{
			MBString	fmStr(pFormatStart, TSCount_t(lpsz - pFormatStart + 1));
			MBString	str;
			
			switch (*lpsz)
			{
				// integers
				case 'd':
				case 'i':
				case 'u':
				case 'x':
				case 'X':
				case 'o':
					if (nModifier & FORCE_INT64)
						str.Format( fmStr.GetString(), va_arg(argList, INT64_T) );
					else
						str.Format( fmStr.GetString(), va_arg(argList, int) );
					Append(str);
					break;
					
				case 'e':
				case 'g':
				case 'G':
					str.Format( fmStr.GetString(), va_arg(argList, DOUBLE_ARG) );
					Append(str);
					break;
					
				case 'f':
					str.Format( fmStr.GetString(), va_arg(argList, double) );
					Append(str);
					break;
					
				case 'p':
					str.Format( fmStr.GetString(), va_arg(argList, void*) );
					Append(str);
					break;
					
				// no output
				case 'n':
					va_arg(argList, int*);
					break;
					
				default:
					assert(FALSE);  // unknown formatting option
			}
		}
		
		pStart = lpsz + 1;
	}
	
	va_end(argListSave);
	
#elif defined(__APPLE_CPP__) || defined(__APPLE_CC__)
	
	CFStringRef	formatStringRef;
	formatStringRef = CFStringCreateWithCharacters(kCFAllocatorDefault, lpszFormat, CH_LENGTH(lpszFormat));
	if ( formatStringRef )
	{
		CFStringRef cfStringRef;
		cfStringRef = CFStringCreateWithFormatAndArguments(kCFAllocatorDefault, NULL, formatStringRef, argList);
		if ( cfStringRef )
		{
			CFIndex		uniCharCount = CFStringGetLength(cfStringRef);
			uint32_t	nOldCount = mCount;
			uint32_t	newCount = nOldCount + uniCharCount;
			if ( newCount > mCapacity )
			{
				if ( !SetCapacity(newCount) ) return;
			}
			
			CFRange	range;
			range.location = 0;
			range.length = uniCharCount;
			CFStringGetCharacters(cfStringRef, range, (UniChar *)(mBuff + nOldCount));
			mCount = newCount;
			mBuff[mCount] = 0;
			
			CFRelease(cfStringRef);
		}
		
		CFRelease(formatStringRef);
	}
	
#else
	
//	ASSERT(AfxIsValidString(lpszFormat));
	assert(lpszFormat != NULL);

	va_list argListSave = argList;

	// make a guess at the maximum length of the resulting string
	int nMaxLen = 0;
	for (const unicode_t * lpsz = lpszFormat; *lpsz != '\0'; lpsz = WCSTRINC(lpsz))
	{
		// handle '%' character, but watch out for '%%'
		if (*lpsz != '%' || *(lpsz = WCSTRINC(lpsz)) == '%')
		{
			nMaxLen += WCCHARLEN(lpsz);
			continue;
		}

		int nItemLen = 0;

		// handle '%' character with format
		int nWidth = 0;
		for (; *lpsz != '\0'; lpsz = WCSTRINC(lpsz))
		{
			// check for valid flags
			if (*lpsz == '#')
				nMaxLen += 2;   // for '0x'
			else if (*lpsz == '*')
				nWidth = va_arg(argList, int);
			else if (*lpsz == '-' || *lpsz == '+' || *lpsz == '0' || *lpsz == ' ')
				;
			else // hit non-flag character
				break;
		}
		// get width and skip it
		if (nWidth == 0)
		{
			// width indicated by
			nWidth = WTOI(lpsz);
			for (; *lpsz != '\0' && iswdigit(*lpsz); lpsz = WCSTRINC(lpsz))
				;
		}
		assert(nWidth >= 0);

		int nPrecision = 0;
		if (*lpsz == '.')
		{
			// skip past '.' separator (width.precision)
			lpsz = WCSTRINC(lpsz);

			// get precision and skip it
			if (*lpsz == '*')
			{
				nPrecision = va_arg(argList, int);
				lpsz = WCSTRINC(lpsz);
			}
			else
			{
				nPrecision = WTOI(lpsz);
				for (; *lpsz != '\0' && iswdigit(*lpsz); lpsz = WCSTRINC(lpsz))
					;
			}
			assert(nPrecision >= 0);
		}

		// should be on type modifier or specifier
		int nModifier = 0;
		if (wcsncmp((const wchar_t*)lpsz, L"I64", 3) == 0)
		{
			lpsz += 3;
			nModifier = FORCE_INT64;
		#if !defined(_X86_) && !defined(_ALPHA_)
			// __int64 is only available on X86 and ALPHA platforms
			assert(FALSE);
		#endif
		}
		else
		{
			switch (*lpsz)
			{
			// modifiers that affect size
			case 'h':
				nModifier = FORCE_ANSI;
				lpsz = WCSTRINC(lpsz);
				break;
			case 'l':
				nModifier = FORCE_UNICODE;
				lpsz = WCSTRINC(lpsz);
				break;

			// modifiers that do not affect size
			case 'F':
			case 'N':
			case 'L':
				lpsz = WCSTRINC(lpsz);
				break;
			}
		}

		// now should be on specifier
		switch (*lpsz | nModifier)
		{
		// single characters
		case 'c':
		case 'C':
			nItemLen = 2;
			va_arg(argList, WCHAR_ARG);
			break;
		case 'c'|FORCE_ANSI:
		case 'C'|FORCE_ANSI:
			nItemLen = 2;
			va_arg(argList, CHAR_ARG);
			break;
		case 'c'|FORCE_UNICODE:
		case 'C'|FORCE_UNICODE:
			nItemLen = 2;
			va_arg(argList, WCHAR_ARG);
			break;

		// strings
		case 's':
			{
				const unicode_t *pstrNextArg = va_arg(argList, const unicode_t *);
				if (pstrNextArg == NULL)
				{
				   nItemLen = 6;  // "(null)"
				}
				else
				{
				   nItemLen = (int)WCSTRLEN(pstrNextArg);
				   nItemLen = max(1, nItemLen);
				}
			}
			break;

		case 'S':
			{
			#if 0	// #ifndef _UNICODE
				const unicode_t * pstrNextArg = va_arg(argList, const unicode_t *);
				if (pstrNextArg == NULL)
				{
				   nItemLen = 6;  // "(null)"
				}
				else
				{
				   nItemLen = WCSTRLEN(pstrNextArg);
				   nItemLen = max(1, nItemLen);
				}
			#else
				const char * pstrNextArg = va_arg(argList, const char *);
				if (pstrNextArg == NULL)
				{
				   nItemLen = 6; // "(null)"
				}
				else
				{
				   nItemLen = MBSTRLEN(pstrNextArg);
				   nItemLen = max(1, nItemLen);
				}
			#endif
			}
			break;

		case 's'|FORCE_ANSI:
		case 'S'|FORCE_ANSI:
			{
				const char * pstrNextArg = va_arg(argList, const char *);
				if (pstrNextArg == NULL)
				{
				   nItemLen = 6; // "(null)"
				}
				else
				{
				   nItemLen = MBSTRLEN(pstrNextArg);
				   nItemLen = max(1, nItemLen);
				}
			}
			break;

		case 's'|FORCE_UNICODE:
		case 'S'|FORCE_UNICODE:
			{
				const unicode_t * pstrNextArg = va_arg(argList, const unicode_t *);
				if (pstrNextArg == NULL)
				{
				   nItemLen = 6; // "(null)"
				}
				else
				{
				   nItemLen = (int)WCSTRLEN(pstrNextArg);
				   nItemLen = max(1, nItemLen);
				}
			}
			break;
		}

		// adjust nItemLen for strings
		if (nItemLen != 0)
		{
			if (nPrecision != 0)
				nItemLen = min(nItemLen, nPrecision);
			nItemLen = max(nItemLen, nWidth);
		}
		else
		{
			switch (*lpsz)
			{
			// integers
			case 'd':
			case 'i':
			case 'u':
			case 'x':
			case 'X':
			case 'o':
				if (nModifier & FORCE_INT64)
					va_arg(argList, INT64_T);
				else
					va_arg(argList, int);
				nItemLen = 32;
				nItemLen = max(nItemLen, nWidth+nPrecision);
				break;

			case 'e':
			case 'g':
			case 'G':
				va_arg(argList, DOUBLE_ARG);
				nItemLen = 128;
				nItemLen = max(nItemLen, nWidth+nPrecision);
				break;

			case 'f':
			#if defined(WIN32) || defined(_WIN32_WCE)
				__try
				{
					// 312 == strlen("-1+(309 zeroes).")
					// 309 zeroes == max precision of a double
					// 6 == adjustment in case precision is not specified,
					//   which means that the precision defaults to 6
					char *pszTemp = (char *)_alloca(max(nWidth, 312+nPrecision+6));

					double	f = va_arg(argList, double);
					sprintf( pszTemp, "%*.*f", nWidth, nPrecision+6, f );
					nItemLen = (int)strlen(pszTemp);
				}
				__except( GetExceptionCode() == STATUS_STACK_OVERFLOW )
				{
					// If the stack overflows, use this function to restore.
				#if _MSC_VER >= 1299 && !defined(_WIN32_WCE)
					int     errcode = _resetstkoflw();
				#endif
				}
			#else
				{
					// 312 == strlen("-1+(309 zeroes).")
					// 309 zeroes == max precision of a double
					// 6 == adjustment in case precision is not specified,
					//   which means that the precision defaults to 6
					char *pszTemp = (char *)alloca(max(nWidth, 312+nPrecision+6));
					
					double	f = va_arg(argList, double);
					sprintf( pszTemp, "%*.*f", nWidth, nPrecision+6, f );
					nItemLen = strlen(pszTemp);
				}
			#endif
				break;

			case 'p':
				va_arg(argList, void*);
				nItemLen = 32;
				nItemLen = max(nItemLen, nWidth+nPrecision);
				break;

			// no output
			case 'n':
				va_arg(argList, int*);
				break;

			default:
				assert(FALSE);  // unknown formatting option
			}
		}

		// adjust nMaxLen for output nItemLen
		nMaxLen += nItemLen;
	}

	if ( SetCount(nMaxLen) )
	{
	#if defined(WIN32) || defined(_WIN32_WCE)
		int	count = vswprintf(mBuff, lpszFormat, argListSave);
	#else
		int	count = vswprintf(mBuff, nMaxLen, lpszFormat, argListSave);
	#endif
		
		assert(count > 0 && count <= nMaxLen);
	//	VERIFY(count > 0 && count <= nMaxLen);

	//	mCount = (count > 0 ? count : 0);
		mCount = CH_LENGTH(mBuff);
		mBuff[mCount] = 0;
	}

	va_end(argListSave);	
	
#endif
}


int UniString::ToInt() const
{
	return mBuff ? WTOI(mBuff) : 0; 
}

double UniString::ToDouble() const
{ 
	return mBuff ? WTOF(mBuff) : 0;
}


void UniString::Composition()
{
	if ( mBuff == NULL || mCount == 0 ) return;
	
#if defined(__APPLE_CPP__) || defined(__APPLE_CC__) || defined(__GNUC__)
	TextEncoding	mFromEncoding = CreateTextEncoding(kTextEncodingUnicodeDefault, kTextEncodingDefaultVariant, kTextEncodingDefaultFormat);
	TextEncoding	mToEncoding = CreateTextEncoding(kTextEncodingUnicodeDefault, kUnicodeNormalizationFormC, kTextEncodingDefaultFormat);	// kUnicodeUTF16Format
	TECObjectRef	tecObjectRef = NULL;
	
	TECCreateConverter(&tecObjectRef, mFromEncoding, mToEncoding);
	if ( tecObjectRef == NULL ) return;
	
	UniString			temp;
	if ( temp.SetCapacity(mCount) )
	{
		ByteCount			actualInputLength = 0;
		ByteCount			actualOutputLength = 0;
		
		TECConvertText(tecObjectRef, (ConstTextPtr)mBuff, mCount * 2, &actualInputLength, (TextPtr)temp.mBuff, mCount * 2, &actualOutputLength);
		if ( actualOutputLength > 0)
		{
			temp.mCount = actualOutputLength / 2;
			temp.mBuff[actualOutputLength / 2] = 0;
			Attach(temp);
		}
	}
	
	TECDisposeConverter(tecObjectRef);
#endif
}


#ifdef _INC_COMUTIL

UniString::UniString( const VARIANT& varSrc ) : TString<unicode_t>()
{
   if (V_VT(&varSrc) == VT_BSTR) {
        *this = V_BSTR(&varSrc);
        return;
    }

	CComVariant varResult;
	HRESULT hr = ::VariantChangeType( &varResult, const_cast< VARIANT* >( &varSrc ), 0, VT_BSTR );
	if( FAILED( hr ) )
	{
		AtlThrow( hr );
	}

	*this = V_BSTR( &varResult );
}

const UniString& UniString::operator =(const VARIANT& varSrc)
{
	if (V_VT(&varSrc) == VT_BSTR) {
        *this = V_BSTR(&varSrc);
		return *this;
    }

	CComVariant varResult;
	HRESULT hr = ::VariantChangeType( &varResult, const_cast< VARIANT* >( &varSrc ), 0, VT_BSTR );
	if( FAILED( hr ) )
	{
		AtlThrow( hr );
	}

	*this = V_BSTR( &varResult );
	return *this;
}

#endif	// _INC_COMUTIL





// ------------------------------------------------------------------------------------------------
//
// TMBString
//
// ------------------------------------------------------------------------------------------------
TMBString::TMBString(encoding_t encoding/* = DEFAULT_CP*/)
: TString<char>(), mEncoding(encoding)
{ 
}

TMBString::TMBString(const TMBString &str)
: TString<char>(str), mEncoding(str.mEncoding)
{ 
}

// Create a string from multibyte.
TMBString::TMBString(const char * lpsz, encoding_t encoding)
: TString<char>(lpsz), mEncoding(encoding)
{
}

TMBString::TMBString(const unsigned char * lpsz, encoding_t encoding)
: TString<char>((const char *)lpsz), mEncoding(encoding)
{ 
}

TMBString::TMBString(uint32_t nLength, const char *lpch, encoding_t encoding/* = DEFAULT_CP*/)
: TString<char>(lpch, nLength), mEncoding(encoding) 
{
}

TMBString::// Create a string from unicode.
TMBString(const TString<unicode_t> &uniString, encoding_t encoding)
: TString<char>(), mEncoding(encoding)
{
	Append(uniString.GetString(), uniString.GetCount()); 
}

TMBString::TMBString(const unicode_t * lpsz, encoding_t encoding)
: TString<char>(), mEncoding(encoding)
{
	Append(lpsz, CH_LENGTH(lpsz));
}

TMBString::TMBString(uint32_t nCount, const unicode_t *lpch, encoding_t encoding/* = DEFAULT_CP*/)
: TString<char>(), mEncoding(encoding)
{
	Append(lpch, nCount);
}

/*
//	bool operator ==(const unicode_t * lpsz) const { return IsEqual(TMBString(lpsz, mEncoding)); }
//	bool operator ==(const TString<unicode_t> &uniString) const { return IsEqual(TMBString(uniString, mEncoding)); }
*/
bool TMBString::operator ==(const unicode_t * lpsz) const
{
	return UniString(*this) == lpsz;
//	return UniString(mBuff, mCount, mEncoding) == lpsz;
}

bool TMBString::operator ==(const TString<unicode_t> &uniString) const
{
	return UniString(*this) == uniString;
}

/*
const TMBString& TMBString::operator =(const TString<unicode_t> &uniString)
{
//	Init(uniString.GetString(), uniString.GetCount());
	Empty(); Append(uniString.GetString(), uniString.GetCount());
	return *this;
}
*/

const TMBString& TMBString::operator =(const TMBString &str)
{
	if ( this == &str ) return *this;

	Empty();
	if ( mEncoding == str.mEncoding )
	{
		Append(str.GetString(), str.GetCount());		
	}
	else
	{
		UniString	uniStr(str);
		Append(uniStr.GetString(), uniStr.GetCount());		
	}
	return *this;
}

/*
const TMBString& TMBString::operator +=(const TString<unicode_t> &uniString)
{
	Append(uniString.GetString(), uniString.GetCount());
	return *this;
}
*/

const TMBString& TMBString::operator +=(const TMBString &str)
{
	if ( this == &str ) return *this;
	
	if ( mEncoding == str.mEncoding )
	{
		Append(str.GetString(), str.GetCount());		
	}
	else
	{
		UniString	uniStr(str);
		Append(uniStr.GetString(), uniStr.GetCount());		
	}
	return *this;
}

/*
void TMBString::Init(const unicode_t * lpch, uint32_t nCount)
{
	Empty(); Append(lpch, nCount);
}
*/

void TMBString::Append(const unicode_t * lpch, uint32_t nCount)
{
#if _UTF8_TEST_
	if ( mEncoding == TE_UTF8 )
	{
		UTF16ToUTF8(lpch, nCount, *this);
		return;
	}
#endif
	
#if defined(WIN32) || defined(_WIN32_WCE)
	
	if ( nCount == 0 || lpch == NULL ) return;
	
	int nSize = ::WideCharToMultiByte(mEncoding, 0, lpch, (int)nCount, NULL, 0, NULL, NULL);
	if ( nSize )
	{
		uint32_t	nOldCount = mCount;
		uint32_t	newCount = nOldCount + nSize;
		if ( newCount > mCapacity )
		{
			if ( !SetCapacity(newCount) )
				return;
		}
		
		WideCharToMultiByte(mEncoding, 0, lpch, (int)nCount, mBuff + nOldCount, nSize, NULL, NULL);
		mCount = newCount;
		mBuff[mCount] = 0;
	}
	
#elif defined(__APPLE_CPP__) || defined(__APPLE_CC__)

//	CFStringRef		cfStringRef = CFStringCreateWithCharactersNoCopy(kCFAllocatorDefault,  (const UniChar *)lpch, nCount, NULL);
	CFStringRef		cfStringRef = CFStringCreateWithCharacters(kCFAllocatorDefault, (const UniChar *)lpch, nCount);
	if ( cfStringRef )
	{
		if ( mEncoding == TE_FS )
		{
			CFIndex		byteCount = CFStringGetMaximumSizeOfFileSystemRepresentation(cfStringRef);
			uint32_t	nOldCount = mCount;
			uint32_t	newCount = nOldCount + byteCount;
			if ( newCount > mCapacity )
			{
				if ( !SetCapacity(newCount) ) return;
			}			
			
			CFStringGetFileSystemRepresentation(cfStringRef, (char *)(mBuff + nOldCount), byteCount);
			mCount = nOldCount + byteCount;
			mBuff[mCount] = 0;
		}
		else
		{
			CFIndex		uniCharCount = CFStringGetLength(cfStringRef);
			CFRange		range;
			range.location = 0;
			range.length = uniCharCount;
			CFIndex		buffMaxLength = uniCharCount * 5;
			uint32_t	nOldCount = mCount;
			uint32_t	newCount = nOldCount + buffMaxLength;
			if ( newCount > mCapacity )
			{
				if ( !SetCapacity(newCount) ) return;
			}
			
			CFIndex charCount = 0;	// The number of characters converted.
			CFIndex mbCharCount = 0;
			charCount = CFStringGetBytes(cfStringRef, range, (CFStringEncoding)mEncoding, '?', false, (UInt8 *)(mBuff + nOldCount), buffMaxLength, &mbCharCount);
			mCount = nOldCount + mbCharCount;
			mBuff[mCount] = 0;
		}
		
		CFRelease(cfStringRef);
		/*
		 UInt8		buff[1024];
		 CFStringGetBytes(cfStringRef, range, mEncoding, '?', false, buff, 5, &mbCharCount);
		 if ( mbCharCount > 0 )
		 {
		 uint32_t	nOldCount = mCount;
		 uint32_t	newCount = nOldCount + mbCharCount;
		 if ( newCount > mCapacity )
		 {
		 if ( !SetCapacity(newCount) ) return;
		 }
		 
		 CFStringGetBytes(cfStringRef, range, mEncoding, '?', false, (UInt8 *)(mBuff + nOldCount), mbCharCount, &mbCharCount);
		 mCount = newCount;
		 mBuff[mCount] = 0;
		 }
		 */
	}
	
	/*
	 TextEncoding		mToEncoding = CreateTextEncoding(mEncoding, kTextEncodingDefaultVariant, kTextEncodingDefaultFormat);
	 //	TextEncoding		mFromEncoding = CreateTextEncoding(kTextEncodingUnicodeV2_0, kTextEncodingDefaultVariant, kTextEncodingDefaultFormat);
	 //	TextEncoding		mFromEncoding = CreateTextEncoding(kTextEncodingUnicodeV3_1, kTextEncodingDefaultVariant, kTextEncodingDefaultFormat);
	 //	TextEncoding		mFromEncoding = CreateTextEncoding(kTextEncodingUnicodeDefault, kTextEncodingDefaultVariant, kTextEncodingDefaultFormat);
	 TextEncoding		mFromEncoding = CreateTextEncoding(kTextEncodingUnicodeV3_2, kTextEncodingDefaultVariant, kTextEncodingDefaultFormat);
	 UnicodeMapping		theMapping;
	 UnicodeToTextInfo	mUnicodeToTextInfo = NULL;
	 OSStatus			mStatus;
	 theMapping.unicodeEncoding = mFromEncoding;
	 theMapping.otherEncoding = mToEncoding;
	 theMapping.mappingVersion = kUnicodeUseLatestMapping;
	 
	 mStatus = CreateUnicodeToTextInfo(&theMapping, &mUnicodeToTextInfo);
	 
	 if ( mUnicodeToTextInfo == NULL ) return;
	 
	 const unicode_t	*uniCharPtr = lpch;
	 const unicode_t	*uniCharEndPtr = lpch + nCount;
	 long			uniCharLen = nCount * 2;
	 ByteCount       oSourceRead;
	 ByteCount		oOutputLen;	
	 char*			buffPtr;
	 uint32_t		totalMBCharCount = 0;
	 uint32_t		capacity;
	 
	 while ( uniCharPtr < uniCharEndPtr )
	 {
	 capacity = totalMBCharCount + uniCharLen;
	 if ( capacity > mCapacity )
	 {
	 if ( !SetCapacity(capacity) ) break;
	 }
	 
	 buffPtr = mBuff + totalMBCharCount;		
	 oOutputLen = 0;
	 oSourceRead = 0;
	 
	 mStatus = ConvertFromUnicodeToText(mUnicodeToTextInfo, uniCharLen, (UniChar *)uniCharPtr,
	 0, 0, NULL, NULL, NULL, uniCharLen,
	 &oSourceRead, &oOutputLen, (LogicalAddress)buffPtr);
	 totalMBCharCount += oOutputLen;
	 uniCharPtr += (const unicode_t *)((char *)uniCharPtr + oSourceRead);
	 uniCharLen -= oSourceRead;
	 }
	 
	 DisposeUnicodeToTextInfo( &mUnicodeToTextInfo );
	 
	 mCount = totalMBCharCount;
	 mBuff[mCount] = 0;
	 */
#else
	
	if ( mEncoding == TE_ANSI )
	{
		const unicode_t *s = lpch;
		const unicode_t *e = s + nCount;
		
		for ( ; s < e; s++ )
		{
			Append(*s);
		}
	}
	else if ( mEncoding == TE_KOREAN )
	{
		const unicode_t *s = lpch;
		const unicode_t *e = s + nCount;
		
		for ( ; s < e; s++ )
		{
			if ( *s < 0x0080 )
			{
				Append(*s);
			}
			else
			{
			//	unsigned short ksCode = Unicode2ksCode(*s);
			//	Append((char)(ksCode >> 8));
			//	Append((char)ksCode);

				Append(*s);
			}
		}
	}
	else if ( mEncoding == TE_UTF8 )
	{
		UTF16ToUTF8(lpch, nCount, *this);
	}
	
#endif
}


void TMBString::FormatV(const char * lpszFormat, va_list argList)
{
	//	ASSERT(AfxIsValidString(lpszFormat));
	assert(lpszFormat != NULL);
	
	va_list argListSave = argList;
	
	// make a guess at the maximum length of the resulting string
	int nMaxLen = 0;
	for (const unsigned char * lpsz = (const unsigned char*)lpszFormat; *lpsz != '\0'; lpsz = MBSTRINC(lpsz))
	{
		// handle '%' character, but watch out for '%%'
		if (*lpsz != '%' || *(lpsz = MBSTRINC(lpsz)) == '%')
		{
			nMaxLen += (int)MBCHARLEN(lpsz);
			continue;
		}
		
		int nItemLen = 0;
		
		// handle '%' character with format
		int nWidth = 0;
		for (; *lpsz != '\0'; lpsz = MBSTRINC(lpsz))
		{
			// check for valid flags
			if (*lpsz == '#')
				nMaxLen += 2;   // for '0x'
			else if (*lpsz == '*')
				nWidth = va_arg(argList, int);
			else if (*lpsz == '-' || *lpsz == '+' || *lpsz == '0' || *lpsz == ' ')
				;
			else // hit non-flag character
				break;
		}
		// get width and skip it
		if (nWidth == 0)
		{
			// width indicated by
			nWidth = atoi((const char *)lpsz);
			for (; *lpsz != '\0' && isdigit(*lpsz); lpsz = MBSTRINC(lpsz))
				;
		}
		assert(nWidth >= 0);
		
		int nPrecision = 0;
		if (*lpsz == '.')
		{
			// skip past '.' separator (width.precision)
			lpsz = MBSTRINC(lpsz);
			
			// get precision and skip it
			if (*lpsz == '*')
			{
				nPrecision = va_arg(argList, int);
				lpsz = MBSTRINC(lpsz);
			}
			else
			{
				nPrecision = atoi((const char *)lpsz);
				for (; *lpsz != '\0' && isdigit(*lpsz); lpsz = MBSTRINC(lpsz))
					;
			}
			assert(nPrecision >= 0);
		}
		
		// should be on type modifier or specifier
		int nModifier = 0;
		if (MBSTRNCMP(lpsz, "I64", 3) == 0)
		{
			lpsz += 3;
			nModifier = FORCE_INT64;
		#if !defined(_X86_) && !defined(_ALPHA_)
			// __int64 is only available on X86 and ALPHA platforms
			assert(FALSE);
		#endif
		}
		else
		{
			switch (*lpsz)
			{
					// modifiers that affect size
				case 'h':
					nModifier = FORCE_ANSI;
					lpsz = MBSTRINC(lpsz);
					break;
				case 'l':
					nModifier = FORCE_UNICODE;
					lpsz = MBSTRINC(lpsz);
					break;
					
					// modifiers that do not affect size
				case 'F':
				case 'N':
				case 'L':
					lpsz = MBSTRINC(lpsz);
					break;
			}
		}
		
		// now should be on specifier
		switch (*lpsz | nModifier)
		{
				// single characters
			case 'c':
			case 'C':
				nItemLen = 2;
				va_arg(argList, CHAR_ARG);
				break;
			case 'c'|FORCE_ANSI:
			case 'C'|FORCE_ANSI:
				nItemLen = 2;
				va_arg(argList, CHAR_ARG);
				break;
			case 'c'|FORCE_UNICODE:
			case 'C'|FORCE_UNICODE:
				nItemLen = 2;
				va_arg(argList, WCHAR_ARG);
				break;
				
				// strings
			case 's':
			{
				const char * pstrNextArg = va_arg(argList, const char *);
				if (pstrNextArg == NULL)
				{
					nItemLen = 6;  // "(null)"
				}
				else
				{
					nItemLen = MBSTRLEN(pstrNextArg);
					nItemLen = max(1, nItemLen);
				}
			}
				break;
				
			case 'S':
			{
			#if 1	// #ifndef _UNICODE
				unicode_t * pstrNextArg = va_arg(argList, unicode_t *);
				if (pstrNextArg == NULL)
				{
					nItemLen = 6;  // "(null)"
				}
				else
				{
					nItemLen = (int)WCSTRLEN(pstrNextArg);
					nItemLen = max(1, nItemLen);
				}
			#else
				const char * pstrNextArg = va_arg(argList, const char *);
				if (pstrNextArg == NULL)
				{
					nItemLen = 6; // "(null)"
				}
				else
				{
					nItemLen = MBSTRLEN(pstrNextArg);
					nItemLen = max(1, nItemLen);
				}
			#endif
			}
				break;
				
			case 's'|FORCE_ANSI:
			case 'S'|FORCE_ANSI:
			{
				const char * pstrNextArg = va_arg(argList, const char *);
				if (pstrNextArg == NULL)
				{
					nItemLen = 6; // "(null)"
				}
				else
				{
					nItemLen = MBSTRLEN(pstrNextArg);
					nItemLen = max(1, nItemLen);
				}
			}
				break;
				
			case 's'|FORCE_UNICODE:
			case 'S'|FORCE_UNICODE:
			{
				unicode_t * pstrNextArg = va_arg(argList, unicode_t *);
				if (pstrNextArg == NULL)
				{
					nItemLen = 6; // "(null)"
				}
				else
				{
					nItemLen = (int)WCSTRLEN(pstrNextArg);
					nItemLen = max(1, nItemLen);
				}
			}
				break;
		}
		
		// adjust nItemLen for strings
		if (nItemLen != 0)
		{
			if (nPrecision != 0)
				nItemLen = min(nItemLen, nPrecision);
			nItemLen = max(nItemLen, nWidth);
		}
		else
		{
			switch (*lpsz)
			{
				// integers
				case 'd':
				case 'i':
				case 'u':
				case 'x':
				case 'X':
				case 'o':
					if (nModifier & FORCE_INT64)
						va_arg(argList, INT64_T);
					else
						va_arg(argList, int);
					nItemLen = 32;
					nItemLen = max(nItemLen, nWidth+nPrecision);
					break;
					
				case 'e':
				case 'g':
				case 'G':
					va_arg(argList, DOUBLE_ARG);
					nItemLen = 128;
					nItemLen = max(nItemLen, nWidth+nPrecision);
					break;
					
				case 'f':
			#if defined(WIN32) || defined(_WIN32_WCE)
				__try
				{
					// 312 == strlen("-1+(309 zeroes).")
					// 309 zeroes == max precision of a double
					// 6 == adjustment in case precision is not specified,
					//   which means that the precision defaults to 6
					char* pszTemp = (char *)_alloca(max(nWidth, 312+nPrecision+6));
					
					double f = va_arg(argList, double);
					sprintf( pszTemp, "%*.*f", nWidth, nPrecision+6, f );
					nItemLen = (int)strlen(pszTemp);
				}
				__except( GetExceptionCode() == STATUS_STACK_OVERFLOW )
				{
					// If the stack overflows, use this function to restore.
				#if _MSC_VER >= 1299 && !defined(_WIN32_WCE)
					int     errcode = _resetstkoflw();
				#endif
				}
			#else
				{
					// 312 == strlen("-1+(309 zeroes).")
					// 309 zeroes == max precision of a double
					// 6 == adjustment in case precision is not specified,
					//   which means that the precision defaults to 6
					char* pszTemp = (char *)alloca(max(nWidth, 312+nPrecision+6));
					
					double f = va_arg(argList, double);
					sprintf( pszTemp, "%*.*f", nWidth, nPrecision+6, f );
					nItemLen = strlen(pszTemp);
				}					
			#endif
					break;
					
				case 'p':
					va_arg(argList, void*);
					nItemLen = 32;
					nItemLen = max(nItemLen, nWidth+nPrecision);
					break;
					
					// no output
				case 'n':
					va_arg(argList, int*);
					break;
					
				default:
					assert(FALSE);  // unknown formatting option
			}
		}
		
		// adjust nMaxLen for output nItemLen
		nMaxLen += nItemLen;
	}
	
	if ( SetCapacity(nMaxLen) )
	{
		int	count = vsprintf(mBuff, lpszFormat, argListSave);
		assert(count > 0 && count <= nMaxLen);
	//	VERIFY(count > 0 && count <= nMaxLen);
		
		mCount = CH_LENGTH(mBuff);
		mBuff[mCount] = 0;
	//	string.ResetCount();
	}
	
	va_end(argListSave);
}

void TMBString::Format(const char * lpszFormat, ...)
{
	va_list argList;

	va_start(argList, lpszFormat);
	FormatV(lpszFormat, argList);
	va_end(argList);
}

void TMBString::FormatV(const unicode_t * lpszFormat, va_list argList)
{
	UniString	uniStr;
	uniStr.FormatV(lpszFormat, argList);
	Init(uniStr);
}

void TMBString::Format(const unicode_t * lpszFormat, ...)
{
	va_list argList;
	
	va_start(argList, lpszFormat);
	FormatV(lpszFormat, argList);
	va_end(argList);
}


#if defined(_INC_COMUTIL)

TMBString::TMBString( const VARIANT& varSrc ) : TString<char>()
{
	if (V_VT(&varSrc) == VT_BSTR) {
        *this = V_BSTR(&varSrc);
        return;
    }
	
	CComVariant varResult;
	HRESULT hr = ::VariantChangeType( &varResult, const_cast< VARIANT* >( &varSrc ), 0, VT_BSTR );
	if( FAILED( hr ) )
	{
		AtlThrow( hr );
	}
	
	*this = V_BSTR( &varResult );
}

const TMBString& TMBString::operator =(const VARIANT& varSrc)
{
	if (V_VT(&varSrc) == VT_BSTR) {
        *this = V_BSTR(&varSrc);
		return *this;
    }
	
	CComVariant varResult;
	HRESULT hr = ::VariantChangeType( &varResult, const_cast< VARIANT* >( &varSrc ), 0, VT_BSTR );
	if( FAILED( hr ) )
	{
		AtlThrow( hr );
	}
	
	*this = V_BSTR( &varResult );
	return *this;
}

#endif	// _INC_COMUTIL



