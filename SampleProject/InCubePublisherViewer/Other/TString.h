/* ********************************************************************************
 *
 * Purpose:	UTF(7, 8, 16, 32) Character Manager
 *
 * Version	: v1.0
 * Author	: IK HAN CHO
 * Copyright :(c) 2003 IKHAN CHO. All rights reserved.
 *
 * Modification History (most recent first):

 *			 1 JUN 2002		IK HAN CHO,  Redesign for Mac
 *			 8 JAN 2002		IK HAN CHO,  Redesign for Windows
 *
 * Known problem:
 *
 * Note :
 * ******************************************************************************* */

#ifndef __T_STRING__
#define __T_STRING__


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//#include <ctype.h>
#include <string.h>
#include <stdlib.h>
//#include <wchar.h>
#include <limits.h>

#if !defined(WIN32) && !defined(_WIN32_WCE)
#include <stdint.h>
#endif


/////////////////////////////////////////////////////////////////////////////
// template class TString
#ifndef CLASS_DECL
#if defined(WIN32) || defined(_WIN32_WCE)
	#ifdef _AFXEXT	// _AFXDLL 
	#define CLASS_DECL	__declspec(dllexport)	// AFX_EXT_CLASS	
	#else
	#define CLASS_DECL
	#endif
#else
	 #define CLASS_DECL
#endif
#endif	// CLASS_DECL


#ifndef FALSE
#define FALSE   0
#endif

#ifndef TRUE
#define TRUE    1
#endif

#ifndef NULL
#define NULL    0
#endif

#ifndef STRING_INT_MAX
#define STRING_INT_MAX			0xFFFFFFFFUL		// SIZE_MAX
#endif

#define STRING_MAX_BUFF_LENGTH		STRING_INT_MAX
#define STRING_MAX_COUNT			STRING_INT_MAX
//#define STRING_MAX_INDEX			(STRING_INT_MAX-1)
#define STRING_INVALID_INDEX		STRING_INT_MAX

#define VALID_STRING_INDEX(idx)		((idx) != STRING_INVALID_INDEX)
#define STRING_FOUND(idx)			VALID_STRING_INDEX(idx)


/*
#if !defined(_W64)
#if !defined(__midl) && (defined(_X86_) || defined(_M_IX86)) && _MSC_VER >= 1300
#define _W64 __w64
#else
#define _W64
#endif
#endif

#ifdef _WIN64 
typedef   signed __int64	sint_t;
typedef unsigned __int64	uint_t;
#else
typedef _W64   signed int	sint_t;
typedef _W64 unsigned int	uint_t;
#endif
*/


#ifndef _UNICODE_T_DEFINED

#if defined(WIN32) || defined(_WIN32_WCE)
#define _UNICODE_T_DEFINED

#ifdef _NATIVE_WCHAR_T_DEFINED
typedef wchar_t			unicode_t;
#else
typedef unsigned short	unicode_t;
#endif

#elif defined(__APPLE_CPP__) || defined(__APPLE_CC__)
#define _UNICODE_T_DEFINED
typedef unsigned short	unicode_t;
#else
#define _UNICODE_T_DEFINED
typedef unsigned short	unicode_t;
#endif

#endif	// _UNICODE_T_DEFINED

typedef unicode_t			utf16_t;
typedef unsigned long		utf32_t;



#ifndef __uint32_t_defined
#ifndef _UINT32_T
#define _UINT32_T
typedef unsigned long		uint32_t;
//typedef size_t			uint32_t;
#endif
#endif

typedef uint32_t		STRCount_t;
typedef uint32_t		STRIndex_t;


/*
#if STRING_INT_MAX == 0xffffffffU
typedef unsigned long		TSCount_t;
typedef unsigned long		TSIndex_t;
#else
typedef UInt64				TSCount_t;
typedef UInt64				TSIndex_t;
#endif
*/


template<class TYPE>
inline uint32_t CH_LENGTH(const TYPE* pch)
{
	if ( pch == NULL ) return 0;

	const TYPE* plz = pch;
	while ( *plz )
	{
		plz++;
	}

	return (uint32_t)(plz - pch);
}

inline uint32_t CH_LENGTH(const char* pch) {
	if ( pch == NULL ) return 0;
	return (uint32_t)strlen(pch);
}

inline uint32_t CH_LENGTH(const wchar_t* pch) {
	if ( pch == NULL ) return 0;
	return (uint32_t)wcslen(pch);
}


template <class T>
class CLASS_DECL TString {
public:
	// constructor
	// Create an empty string.
	TString(void);
	// Create a string from TString.
	TString(const TString<T> &str);
	// Create a string from T Array.
	TString (const T *lpzstr);
	TString (const T *buff, uint32_t count);

	// Destructor.
	virtual ~TString(void);

	// Accessor
	// Returns the number of characters 
	uint32_t GetCount(void) const { return mCount; }
	uint32_t GetStringSize(void) const { return mCount * sizeof(T); }

	T *GetString(void) {
		static T	t = 0;
		return mBuff ? mBuff : &t; 
	}

	const T *GetString(void) const {
		static const T	t = 0;
		return mBuff ? (const T*)mBuff : &t; 
	}

	void GetString(uint32_t at, uint32_t count, TString<T> &string) const;

	const T *GetBuffer(void) const {
		return mBuff; 
	}

	const T GetLastChar() const { return mCount ? mBuff[mCount-1] : 0; }

//	bool HasString() const { return (mCount > 0 && mBuff != NULL); }
	bool IsEmpty() const { return mCount == 0; }
	void Empty() { mCount = 0; if ( mBuff ) *mBuff = 0; }

	operator const T*() const { return (const T*)GetString(); }	// { return (const T*)mBuff; }
	operator T*() { return (T*)GetString(); } // { return mBuff; }

	// Impletation
	// Get <i>th character.
	T GetAt(uint32_t at) const { return (at < mCount) ? mBuff[at] : 0; }
	// Change <i>th character.
	void SetAt(uint32_t at, T c);

	void Left(uint32_t count, TString<T> &string) const { GetString(0, count, string); }
	void Right(uint32_t count, TString<T> &string) const;
	void Mid(TString<T> &string, uint32_t at, uint32_t count=STRING_MAX_COUNT) const;

	void TrimLeft();
	void TrimRight();
	void MakeUpper();
	void MakeLower();

	// Clear string to zero length.
	void RemoveAll(void);

	void Init(T ch);
	void Init(const T *buff, uint32_t count);
	void Init(uint32_t count, const T *buff) { Init(buff, count); }
	void Init(const TString<T> &string) { Init(string.GetString(), string.GetCount());}
	void Init(const T *buff) { Init(buff, CH_LENGTH(buff)); }

	// Append a character or string.
	void Append(const TString<T> &string) { Append(string.GetString(), string.GetCount());}
	void Append(const T *buff) { Append(buff, CH_LENGTH(buff)); }
	void Append(const T *buff, uint32_t count);
	void Append(uint32_t count, const T *buff) { Append(buff, count); }
	void Append(T ch);

	// Insert a character or string.
	void InsertAt(uint32_t at, T c) { InsertAt(at, &c, 1);}
	void InsertAt(uint32_t at, const TString<T> &string)
		{ InsertAt(at, string.GetString(), string.GetCount());}
	void InsertAt(uint32_t at, const T *buff) { InsertAt(at, buff, CH_LENGTH(buff)); }
	void InsertAt(uint32_t at, const T *buff, uint32_t count);

    // Delete a character or range of characters.
	void DeleteAt(uint32_t at, uint32_t count = 1);
	void RemoveAt(STRIndex_t at, STRCount_t count = 1) { DeleteAt(at, count); }
	void Truncate(STRIndex_t at) { if ( at < mCount ) DeleteAt(at, mCount - at); }

	uint32_t FindAt(const TString<T> &string) const 
		{ return FindAt(0, string.GetString(), string.GetCount()); }
	uint32_t FindAt(const T* pCh, uint32_t count) const { return FindAt(0, pCh, count); } 
	uint32_t FindAt(const T* pCh) const { return FindAt(0, pCh, CH_LENGTH(pCh)); } 
	uint32_t FindAt(const T ch) const;

	uint32_t FindAt(uint32_t at, const T ch) const { return FindAt(at, &ch, 1); } 
	uint32_t FindAt(uint32_t at, const T* pCh) const { return FindAt(at, pCh, CH_LENGTH(pCh)); } 
	uint32_t FindAt(uint32_t at, const T* pCh, uint32_t count) const;
	uint32_t FindAt(uint32_t startAt, uint32_t endAt, const T* pCh, uint32_t count) const;

	uint32_t FindRAt(const TString<T> &string) const 
		{ return FindRAt(0, string.GetString(), string.GetCount()); }
	uint32_t FindRAt(const T* pCh, uint32_t count) const { return FindRAt(0, pCh, count); } 
	uint32_t FindRAt(const T* pCh) const { return FindRAt(0, pCh, CH_LENGTH(pCh)); } 
	uint32_t FindRAt(const T ch) const;

	uint32_t FindRAt(uint32_t at, const T ch) const { return FindARt(at, &ch, 1); } 
	uint32_t FindRAt(uint32_t at, const T* pCh) const { return FindRAt(at, pCh, CH_LENGTH(pCh)); } 
	uint32_t FindRAt(uint32_t at, const T* pCh, uint32_t count) const;

	uint32_t FindOneOf(const TString<T> &string) const 
		{ return FindOneOf(0, string.GetString(), string.GetCount()); }
	uint32_t FindOneOf(const T* buff) const { return FindOneOf(0, buff, CH_LENGTH(buff)); }
	uint32_t FindOneOf(const T* buff, uint32_t count) const { return FindOneOf(0, buff, count); }
	uint32_t FindOneOf(uint32_t at, const T* buff, uint32_t count) const;

	uint32_t FindCount(const T* buff) const { return FindCount(0, mCount, buff, CH_LENGTH(buff)); }
	uint32_t FindCount(const T* buff, uint32_t count) const { return FindCount(0, mCount, buff, count); }
	uint32_t FindCount(uint32_t startAt, uint32_t endAt, const T* buff) const {
		return FindCount(startAt, endAt, buff, CH_LENGTH(buff));
	}
	uint32_t FindCount(uint32_t startAt, uint32_t endAt, const T* buff, uint32_t count) const;

	void ReplaceAt(uint32_t at, uint32_t count, const TString<T> &string)
		{ ReplaceAt(at, count, string.GetString(), string.GetCount());}
 	void ReplaceAt(uint32_t at, uint32_t count, const T charCode)
 		{ ReplaceAt(at, count, &charCode, 1);}
	void ReplaceAt(uint32_t at, uint32_t count, const T *buff)
		{ ReplaceAt(at, count, buff, CH_LENGTH(buff));}
 	void ReplaceAt(uint32_t at, uint32_t count, const T *buff, uint32_t charCount);

	void Replace(T ch, T newCh);
	void Replace(const TString<T> &findString, const TString<T> &replaceString)
		{ Replace(findString.GetString(), findString.GetCount(), replaceString.GetString(), replaceString.GetCount()); }
	void Replace(T ch, const TString<T> &replaceString) {
		Replace(&ch, 1, replaceString.GetString(), replaceString.GetCount());
	}
	// replace occurrences of substring pOldCh with pNewCh;
	// empty pNewCh removes instances of pOldCh
	void Replace(const T* pOldCh, const T* pNewCh) {
		Replace(pOldCh, CH_LENGTH(pOldCh), pNewCh, CH_LENGTH(pNewCh));
	}
	void Replace(const T* pOldCh, uint32_t count, const T* pNewCh, uint32_t newCount);

	void Delete(T ch) {
		Replace(&ch, 1, NULL, 0);
	}

	bool IsEqual(const TString<T> &string) const { return IsEqual(string.GetString(), string.GetCount()); }
	bool IsEqual(const T *buff) const { return IsEqual(buff, CH_LENGTH(buff)); }
	bool IsEqual(const T *buff, uint32_t charCount) const;

	// Zero if the strings are identical, < 0 if this TString object is less than string,
	// or > 0 if this TString object is greater than string.
	short Compare(const TString<T> &string) const
		{ return Compare(0, string.GetString(), string.GetCount()); }
	short Compare(const T *buff, uint32_t count) const { return Compare(0, buff, count); }
	short Compare(const T *buff) const { return Compare(0, buff, CH_LENGTH(buff)); }

	short Compare(uint32_t at, const T *buff) const { return Compare(at, buff, CH_LENGTH(buff)); }
	short Compare(uint32_t at, const TString<T> &string) const
		{ return Compare(at, string.GetString(), string.GetCount()); }
	short Compare(uint32_t at, const T *buff, uint32_t count) const;

	short Compare(uint32_t at, uint32_t length, const T *buff) const {
		return Compare(at, length, buff, CH_LENGTH(buff)); 
	}
	short Compare(uint32_t at, uint32_t srcCount, const T *buff, uint32_t count) const;
	short CompareN(uint32_t at, const T *buff, uint32_t count) const {
		return Compare(at, count, buff, count);
	}


	// Zero if the strings are identical, < 0 if this TString object is less than buff,
	// or > 0 if this TString object is greater than buff.
	short CompareNoCase(const TString<T> &string) const
		{ return CompareNoCase(0, string.GetString(), string.GetCount()); }
	short CompareNoCase(const T *buff, uint32_t count) const
		{ return CompareNoCase(0, buff, count); }
	short CompareNoCase(const T *buff) const
		{ return CompareNoCase(0, buff, CH_LENGTH(buff)); }

	short CompareNoCase(uint32_t at, const TString<T> &string) const
		{ return CompareNoCase(at, string.GetString(), string.GetCount()); }
	short CompareNoCase(uint32_t at, const T *buff) const { return CompareNoCase(at, buff, CH_LENGTH(buff)); }
	short CompareNoCase(uint32_t at, const T *buff, uint32_t count) const;
	short CompareNoCase(uint32_t at, uint32_t srcCount, const T *buff, uint32_t count) const;
	short CompareNI(uint32_t at, const T *buff, uint32_t count) const {
		return CompareNoCase(at, count, buff, count);
	}
	
	bool StartWith(const T *pCh, bool ignoreCase = false) const { return StartWith(CH_LENGTH(pCh), pCh, ignoreCase); }
	bool StartWith(STRCount_t count, const T *pCh, bool ignoreCase = false) const;
	
	bool EndWith(const T *pCh, bool ignoreCase = false) const { return EndWith(CH_LENGTH(pCh), pCh, ignoreCase); }
	bool EndWith(STRCount_t count, const T *pCh, bool ignoreCase = false) const;

	bool Contains(const T *pCh) const { return FindAt(CH_LENGTH(pCh), pCh) != STRING_INVALID_INDEX; }
	bool Contains(STRCount_t count, const T *pCh) const { return FindAt(count, pCh) != STRING_INVALID_INDEX; }


	void Fill(T ch, uint32_t count);

	// Assign
	const TString<T>& operator =(const T *buff) { Init(buff, CH_LENGTH(buff)); return *this; }
	const TString<T>& operator =(T ch) { Init(ch); return *this; }
	const TString<T>& operator =(const TString<T> &string)
		{ Init(string.GetString(), string.GetCount()); return *this;  }

	// concatenate a single character
	const TString<T>& operator +=(T ch) { Append(ch); return *this; }
	// concatenate from another TString<T>
	const TString<T>& operator +=(const TString<T> &string) { Append(string.GetString(), string.GetCount()); return *this; }
	const TString<T>& operator +=(const T *buff) { Append(buff, CH_LENGTH(buff)); return *this; }

	bool operator ==(const TString<T> &string) const { return IsEqual(string.GetString(), string.GetCount()); }
	bool operator ==(const T *buff) const { return IsEqual(buff, CH_LENGTH(buff)); }
	bool operator ==(const T ch) const { return IsEqual(&ch, 1); }

	bool operator !=(const TString<T> &string) const { return !IsEqual(string.GetString(), string.GetCount()); }
	bool operator !=(const T *buff) const { return !IsEqual(buff, CH_LENGTH(buff)); }
	bool operator !=(const T ch) const { return !sIsEqual(&ch, 1); }

	bool operator <(const TString<T> &string) const {
		return Compare(0, string.GetString(), string.GetCount()) < 0;
	}
	bool operator <(const T *buff) const { return Compare(0, buff, CH_LENGTH(buff)) < 0; }
	bool operator <(const T ch) const { return Compare(0, &ch, 1) < 0; }

	bool operator <=(const TString<T> &string) const {
		return Compare(0, string.GetString(), string.GetCount()) <= 0;
	}
	bool operator <=(const T *buff) const { return Compare(0, buff, CH_LENGTH(buff)) <= 0; }
	bool operator <=(const T ch) const { return Compare(0, &ch, 1) <= 0; }

	bool operator >(const TString<T> &string) const {
		return Compare(0, string.GetString(), string.GetCount()) > 0;
	}
	bool operator >(const T *buff) const { return Compare(0, buff, CH_LENGTH(buff)) > 0; }
	bool operator >(const T ch) const { return Compare(0, &ch, 1) > 0; }

	bool operator >=(const TString<T> &string) const {
		return Compare(0, string.GetString(), string.GetCount()) >= 0;
	}
	bool operator >=(const T *buff) const { return Compare(0, buff, CH_LENGTH(buff)) >= 0; }
	bool operator >=(const T ch) const { return Compare(0, &ch, 1) >= 0; }


	friend TString<T> operator +(const TString<T>& string1, const TString<T>& string2)
	{
		TString<T>	string(string1);
		string.Append(string2);
		return string;
	}

	friend TString<T> operator +(const TString<T>& string, T ch)
	{
		TString<T>	str(string);
		str.Append(ch);
		return str;
	}

	friend TString<T> operator +(T ch, const TString<T>& string)
	{
		TString<T>	str(&ch, 1);
		str.Append(string);
		return str;
	}

	friend TString<T> operator +(const T *pch, const TString<T>& string)
	{
		TString<T>	str(pch);
		str.Append(string);
		return str;
	}

	friend TString<T> operator +(const TString<T>& string, const T *pch)
	{
		TString<T>	str(string);
		str.Append(pch);
		return str;
	}

	T* SetCount(uint32_t count);

	void ResetCount() {
		mCount = CH_LENGTH(mBuff);
		if ( mBuff )
			mBuff[mCount] = 0;
	}

/*
	void ResetCount(uint32_t count) {
		if ( count < mCapacity )
		{
			mCount = count;
			mBuff[mCount] = 0;
		}
	}
*/
	bool SetCapacity(uint32_t capacity);
	uint32_t GetCapacity() const { return mCapacity; }
	uint32_t GetCapacitySize(void) const { return mCapacity * sizeof(T); }

	void FreeExtra(void);

	// Accessor
	uint32_t GetMaxCount(void) const { return (STRING_MAX_BUFF_LENGTH / sizeof(T) - 1); }

	void Attach(const T *buff, uint32_t count, uint32_t capacity);
	void Attach(TString<T> &string);
	T*   Detach(void);

	//	void Move(uint32_t srcAt, uint32_t desAt);

protected:
	T			*mBuff;
	uint32_t	mCount;
	uint32_t	mCapacity;
};


/*
  num   : Number of elements
  _type : Data Type of each element
*/

#define TMemAlloc(num, _type)			(_type *)malloc((num)*sizeof(_type))
#define TMemFree(buff)					free(buff)
#define TMemSetSize(p, num, _type)		(_type *)realloc(p, (num)*sizeof(_type))

#define TMemoryCopy(src, des, len)		memcpy((des), (src), (len))
#define TMemorySafeCopy(src, des, len)	memmove((des), (src), (len))

// MemoryCompare() returns a zero if all n characters pointed to by m1 and m2 are equal.
// MemoryCompare() returns a negative value if the first non-matching character pointed to by m1
// is less than the character pointed to by m2.
// MemoryCompare() returns a positive value if the first non-matching character pointed to by m1
// is greater than the character pointed to by m2.
#define TMemoryCompare(m1, m2, size)		memcmp(m1, m2, size)

#define CopyChar(src, des, count)		memcpy((des), (src), (count)*sizeof(T))
#define SafeCopyChar(src, des, count)	memmove((des), (src), (count)*sizeof(T))
#define CompareChar(m1, m2, count)		memcmp((m1), (m2), (count)*sizeof(T))


static inline uint32_t CalcCapacity(uint32_t charCount)
{
	if ( charCount > 256 )
		return 2 * charCount;

	return 256;
/*
	uint32_t delta;

	delta = charCount < 256 ? 63 : 1023;
//	delta = charCount < 256 ? 7 : 255;

	return (charCount + 1 + delta) & ~delta;
*/
}


template <class T>
TString<T>::TString(void):mBuff(NULL), mCount(0), mCapacity(0)
{
//	Init(NULL, count);
}

template <class T>
TString<T>::TString(const T *lpzstr):mBuff(NULL), mCount(0), mCapacity(0)
{
	Init(lpzstr);
}


template <class T>
TString<T>::TString(const T *buff, uint32_t count):mBuff(NULL), mCount(0), mCapacity(0)
{
	Init(buff, count);
}

template <class T>
TString<T>::TString(const TString &str):mBuff(NULL), mCount(0), mCapacity(0)
{
	Init(str.GetString(), str.GetCount());
}

template <class T>
TString<T>::~TString(void)
{
	if ( mBuff )
		TMemFree(mBuff);
}

template <class T>
void TString<T>::RemoveAll(void)
{
	if ( mBuff )
	{
		TMemFree(mBuff);
		mBuff = NULL;

		mCount = 0;
		mCapacity = 0;
	}
}

template <class T>
inline void TString<T>::Init(T ch)
{
//	RemoveAll();
	Init(&ch, 1);
}


template <class T>
void TString<T>::Init(const T *ptr, uint32_t count)
{
	if ( ptr == mBuff ) return;
#if 0
	Empty();
	Append(ptr, count);
#else
/*
	T	*pCh = SetCount(count);
	if ( count )
	{
		CopyChar(ptr, pCh, count);
	}
*/
	if ( ptr != NULL && count != 0 )
	{
		if ( count > mCapacity )
		{
	 		if ( !SetCapacity(count+1) ) return;
		}

		CopyChar(ptr, mBuff, count);
		mCount = count;
		mBuff[mCount] = 0;
	}
	else
	{
		mCount = 0;
		if ( mBuff )
			*mBuff = 0;
	}

#endif
}


template <class T>
inline void TString<T>::SetAt(uint32_t at, T c)
{
	if ( at < mCount )
		mBuff[at] = c;
}

template <class T>
void TString<T>::GetString(uint32_t at, uint32_t count, TString<T> &string) const
{
	if ( at >= mCount )
	{
		string.Empty();
		return;
	}

	uint32_t	realCount = mCount - at;
	if ( count > realCount )
		count = realCount;

	string.Init(mBuff + at, count);

/*
	uint32_t	realCount = mCount - at;

	if ( count > realCount )
		count = realCount;

	string.Append(mBuff+at, count);
*/
}

template <class T>
void TString<T>::Append(T ch)
{
	if ( mCount >= GetMaxCount() ) return;

	if ( mCount >= mCapacity )
	{
	 	if ( !SetCapacity(mCount+1) ) return;
	}

	mBuff[mCount++] = ch;
	mBuff[mCount] = 0;
}


template <class T>
void TString<T>::Append(const T *buff, uint32_t count)
{
	if ( buff == NULL || count == 0 ) return;
	if ( count > (GetMaxCount() - mCount) ) return;

	uint32_t	newCount = mCount + count;
	if ( newCount > mCapacity )
	{
	 	if ( !SetCapacity(newCount) ) return;
	}

	CopyChar(buff, mBuff + mCount, count);
	mCount = newCount;
	mBuff[mCount] = 0;

}

template <class T>
void TString<T>::InsertAt(uint32_t at, const T *buff, uint32_t count)
{
	if ( buff == NULL || count == 0 ) return;
	if ( count > (GetMaxCount() - mCount) ) return;

	if ( count > (mCapacity - mCount) )
	{
	//	uint32_t	remaind = GetMaxCount() - mCount;
	//	if ( count > remaind )
	//		count = remaind;

	 	if ( !SetCapacity(mCount + count) ) return;
	}

	if ( at < mCount )
	{
		T *srcPtr = mBuff + at;
		SafeCopyChar(srcPtr, srcPtr + count, mCount - at);
	}
	else
	{
		at = mCount;
	}

	CopyChar(buff, mBuff + at, count);

	mCount += count;
	mBuff[mCount] = 0;
}

template <class T>
void TString<T>::ReplaceAt(uint32_t at, uint32_t count, const T *buff, uint32_t charCount)
{
	if ( buff == NULL || charCount == 0 ) return;

	if ( (mBuff == NULL || count == 0) /*|| at >= mCount */)
	{
		InsertAt(at, buff, charCount);
		return;
	}

	if ( charCount > count )
	{
		if ( (charCount - count) > (GetMaxCount() - mCount) ) return;
	}

	if ( at > mCount )
		at = mCount;

	if ( count > (mCount - at) )
		count = mCount - at;

	if ( charCount != count )
	{
		uint32_t	fromAt = at + count;
		uint32_t	toAt = at + charCount;

		if ( charCount > count )
		{
			if ( (charCount - count) > (GetMaxCount()-mCount) ) return;

			uint32_t	newCount = mCount + (charCount - count);
			if ( !SetCapacity(newCount) ) return;

			if ( fromAt < mCount )
				SafeCopyChar(mBuff+fromAt, mBuff+toAt, mCount - fromAt);

			CopyChar(buff, mBuff + at, charCount);
			mCount = newCount;
		}
		else
		{
			if ( fromAt < mCount )
				SafeCopyChar(mBuff+fromAt, mBuff+toAt, mCount - fromAt);
			CopyChar(buff, mBuff + at, charCount);

			mCount -= (count - charCount);
			SetCapacity(mCount);
		}
	}
	else
	{
		CopyChar(buff, mBuff + at, charCount);
	}

	mBuff[mCount] = 0;
}


template <class T>
void TString<T>::Replace(T ch, T newCh)
{
	T *pStart = mBuff;
	T *pEnd = mBuff + mCount;
	for ( ; pStart < pEnd; pStart++ )
	{
		if ( *pStart == ch )
			*pStart = newCh;
	}
/*
	for ( uint32_t i = 0; i < mCount; i++ )
	{
		if ( mBuff[i] == ch )
			mBuff[i] = newCh;
	}
*/
}


template <class T>
void TString<T>::Replace(const T* pOldCh, uint32_t count, const T* pNewCh, uint32_t newCount)
{
	if ( pOldCh == NULL || count == 0 ) return;

	uint32_t	at = 0;

	while ( at < mCount )
	{
		at = FindAt(at, pOldCh, count);
		if ( at == STRING_INVALID_INDEX ) break;
	
		if ( newCount && pNewCh )
		{
			ReplaceAt(at, count, pNewCh, newCount);
			at += newCount;
		}
		else
		{
			DeleteAt(at, count);
		//	at++;
		}
	}
}

/*
template <class T>
uint32_t TString<T>::FindAt(const T* pCh, uint32_t count)
{
	if ( pCh == NULL || count == 0 ) return STRING_INVALID_INDEX;
	if ( at >= mCount ) return STRING_INVALID_INDEX;
	if ( count > (mCount - at) ) return STRING_INVALID_INDEX;

	uint32_t	endAt = mCount - count;

	uint32_t	at = 0;
	while ( at <= endAt )
	{
		T		*pBuff = mBuff + at;
		bool	equal = TRUE;
		
		for ( uint32_t i = 0; i < count; i++ )
		{
			if ( pCh[i] != pBuff[i] )
			{
				equal = FALSE;
				break;
			}
		}

		if ( equal )
			return at;
		
		at++;
	}

	return STRING_INVALID_INDEX;
}
*/

template <class T>
uint32_t TString<T>::FindAt(uint32_t at, const T* pCh, uint32_t count) const
{
	if ( pCh == NULL || count == 0 ) return STRING_INVALID_INDEX;
	if ( at >= mCount ) return STRING_INVALID_INDEX;
	if ( count > (mCount - at) ) return STRING_INVALID_INDEX;

	uint32_t	endAt = mCount - count;
	while ( at <= endAt )
	{
		T		*pBuff = mBuff + at;
		bool	equal = TRUE;
		
		for ( uint32_t i = 0; i < count; i++ )
		{
			if ( pCh[i] != pBuff[i] )
			{
				equal = FALSE;
				break;
			}
		}

		if ( equal )
			return at;
		
		at++;
	}

	return STRING_INVALID_INDEX;
}

template <class T>
uint32_t TString<T>::FindAt(uint32_t startAt, uint32_t endAt, const T* pCh, uint32_t count) const
{
	if ( pCh == NULL || count == 0 ) return STRING_INVALID_INDEX;
	if ( startAt >= mCount ) return STRING_INVALID_INDEX;
	if ( endAt >= mCount ) endAt == mCount;
	if ( startAt >= endAt ) STRING_INVALID_INDEX;
	if ( count > (endAt - startAt) ) return STRING_INVALID_INDEX;

	//
	while ( startAt <= endAt )
	{
		T		*pBuff = mBuff + startAt;
		bool	equal = TRUE;
		
		for ( uint32_t i = 0; i < count; i++ )
		{
			if ( pCh[i] != pBuff[i] )
			{
				equal = FALSE;
				break;
			}
		}

		if ( equal )
			return startAt;
		
		startAt++;
	}

	return STRING_INVALID_INDEX;
}


template <class T>
uint32_t TString<T>::FindAt(T ch) const
{
/*
	T *pStart = mBuff;
	T *pEnd = mBuff + mCount;
	for ( ; pStart < pEnd; pStart++ )
	{
		if ( *pStart == ch )
			return pStart - mBuff;
	}
*/
	for ( uint32_t at = 0; at < mCount; at++ )
	{
		if ( mBuff[at] == ch )
			return at;
	}

	return STRING_INVALID_INDEX;

}


template <class T>
uint32_t TString<T>::FindRAt(uint32_t at, const T* pCh, uint32_t count) const
{
	if ( pCh == NULL || count == 0 ) return STRING_INVALID_INDEX;
	if ( at >= mCount ) return STRING_INVALID_INDEX;
	if ( count > (mCount - at) ) return STRING_INVALID_INDEX;

	uint32_t	endAt = mCount - count;
	while ( at <= endAt )
	{
		T		*pBuff = mBuff + endAt;
		bool	equal = TRUE;
		
		for ( uint32_t i = 0; i < count; i++ )
		{
			if ( pCh[i] != pBuff[i] )
			{
				equal = FALSE;
				break;
			}
		}

		if ( equal )
			return endAt;
		
		endAt--;
	}

	return STRING_INVALID_INDEX;
}

template <class T>
uint32_t TString<T>::FindRAt(T ch) const
{
	for ( uint32_t at = mCount; at > 0; at-- )
	{
		if ( mBuff[at-1] == ch )
			return at - 1;
	}

	return STRING_INVALID_INDEX;

}


template <class T>
uint32_t TString<T>::FindCount(uint32_t startAt, uint32_t endAt, const T* pCh, uint32_t count) const
{
	if ( pCh == NULL || count == 0 ) return 0;
	if ( startAt >= endAt ) return 0;
	if ( startAt >= mCount ) return 0;

	if ( endAt > mCount )
		endAt = mCount;

	if ( (endAt - startAt) < count ) return 0;
	if ( endAt <= count ) return 0;

	endAt -= count;
	uint32_t	findCount = 0;
	while ( startAt <= endAt )
	{
		T		*pBuff = mBuff + startAt;
		bool	equal = TRUE;
		
		for ( uint32_t i = 0; i < count; i++ )
		{
			if ( pCh[i] != pBuff[i] )
			{
				equal = FALSE;
				break;
			}
		}

		if ( equal )
		{
			findCount++;
			startAt += count;
		}
		else
		{
			startAt++;
		}
	}

	return findCount;
}


template <class T>
uint32_t TString<T>::FindOneOf(uint32_t at, const T* buff, uint32_t count) const
{
	if ( buff == NULL || count == 0 ) return STRING_INVALID_INDEX;
	if ( at >= mCount ) return STRING_INVALID_INDEX;

	while ( at < mCount )
	{
		for ( uint32_t i = 0; i < count; i++ )
		{
			if ( mBuff[at] == buff[i] )
				return at;
		}

		at++;
	}

	return STRING_INVALID_INDEX;
}

template <class T>
void TString<T>::DeleteAt(uint32_t at, uint32_t count)
{
	if ( mBuff == NULL || count == 0 ) return;
	if ( at >= mCount ) return;

	if ( count < (mCount - at) )
	{
		uint32_t endOffset = at + count;
		SafeCopyChar(mBuff + endOffset, mBuff + at, mCount - endOffset);
		mCount -= count;
	}
	else
	{
		mCount = at;
	}

#if 0
	if ( mCapacity > (mCount * 2) )
		SetCapacity(mCount * 2);
#else
	SetCapacity(mCount);
#endif

	mBuff[mCount] = 0;
}


template <class T>
void TString<T>::Right(uint32_t count, TString<T> &string) const
{
	if ( count > mCount )	
		count = mCount;

	string.Init(mBuff + (mCount - count), count);
}


template <class T>
void TString<T>::Mid(TString<T> &string, uint32_t at, uint32_t count/*=STRING_MAX_COUNT*/) const
{
	if ( at >= mCount )
	{
		string.Empty();
		return;
	}

	uint32_t	realCount = mCount - at;
	if ( count > realCount )
		count = realCount;

	string.Init(mBuff + at, count);
}

template <class T>
void TString<T>::TrimLeft()
{
	if ( mBuff == NULL ) return;

	T	*begin = mBuff;
	T	*end = mBuff + mCount;

	for ( ; begin < end; begin++ )
	{
		if ( *begin > 0x20 )
			break;
	}

	if ( (uint32_t)(begin - mBuff) > 0 )
		DeleteAt(0, (uint32_t)(begin - mBuff));

}

template <class T>
void TString<T>::TrimRight()
{
	if ( mBuff == NULL ) return;

	T	*pEnd = mBuff + mCount;
	while ( pEnd > mBuff )
	{
		if ( *(pEnd - 1) > 0x20 )
		{
			break;
		}
		pEnd--;
	}

	mCount = (uint32_t)(pEnd - mBuff);

	*pEnd = 0;
}


template <class T>
void TString<T>::MakeUpper()
{
	T	*end = mBuff + mCount;

	for ( T	*begin = mBuff; begin < end; begin++ )
	{
		if ( *begin >= 'a' && *begin <= 'z' )
			*begin -= 0x20;
	}
}

template <class T>
void TString<T>::MakeLower()
{
	T	*end = mBuff + mCount;

	for ( T	*begin = mBuff; begin < end; begin++ )
	{
		if ( *begin >= 'A' && *begin <= 'Z' )
			*begin += 0x20;
	}

}

template <class T>
bool TString<T>::IsEqual(const T *buff, uint32_t charCount) const
{
	if ( mCount != charCount ) return false;
	if ( mBuff == buff ) return true;
	if ( mBuff == NULL || buff == NULL ) return false;

	T	*pBuff = mBuff;
	T	*pBuffEnd = pBuff + mCount;
	for ( ; pBuff < pBuffEnd; pBuff++, buff++ )
	{
		if ( (*pBuff) != (*buff) )
			return false;
	}
/*
	for ( uint32_t i = 0; i < mCount; i++ )
	{
		if ( mBuff[i] != buff[i] )
			return false;
	}
*/
	return true;
}

#if 0
template <class T>
short TString<T>::Compare(const T *buff, uint32_t count) const
{
	int		result = Compare(0, buff, count);

	if ( result == 0 )
	{
		if ( count < mCount )
			return -1;
		else if ( count > mCount )
			return 1;
	}

	return 0;

/*
	uint32_t	cmpCount;
	int		result;

	cmpCount = (mCount < count) ? mCount: count;

	result = memcmp(buff, mBuff, cmpCount*sizeof(T));
	if ( result == 0 )
	{
		if ( count < mCount )
			result = -1;
		else if ( count > mCount )
			result = 1;
	}
	return result;
*/
}
#endif


template <class T>
short TString<T>::Compare(uint32_t at, const T *buff, uint32_t count) const
{
	if ( buff == NULL )
	{
		return mBuff ? 1 : 0;
	}

	if ( mBuff == NULL )
	{
		return buff ? -1 : 0;
	}

	uint32_t	length = mCount - at;
	uint32_t	cmpCount = ( length < count ) ? length : count;

	T	*pBuff = mBuff + at;
	T	*pBuffEnd = pBuff + cmpCount;
	for ( ; pBuff < pBuffEnd; pBuff++, buff++ )
	{
		if ( (*pBuff) < (*buff) )
			return ( -1 );
		else if ( (*pBuff) > (*buff) )
			return ( 1 );
	}
/*
	for ( uint32_t i = 0; i < cmpCount; i++ )
	{
		if ( pBuff[i] < buff[i] )
			return ( -1 );
		else if ( pBuff[i] > buff[i] )
			return ( 1 );
	}
*/
	if ( length < count )
		return -1;
	else if ( length > count )
		return 1;

	return 0;
}

template <class T>
short TString<T>::Compare(uint32_t at, uint32_t srcCount, const T *buff, uint32_t count) const
{
	if ( buff == NULL )
	{
		return mBuff ? 1 : 0;
	}

	if ( mBuff == NULL )
	{
		return buff ? -1 : 0;
	}

	uint32_t	len = mCount - at;
	if ( srcCount > len )
		srcCount = len;

	uint32_t	cmpCount = ( srcCount < count ) ? srcCount : count;

	T	*pBuff = mBuff + at;
	T	*pBuffEnd = pBuff + cmpCount;
	for ( ; pBuff < pBuffEnd; pBuff++, buff++ )
	{
		if ( (*pBuff) < (*buff) )
			return ( -1 );
		else if ( (*pBuff) > (*buff) )
			return ( 1 );
	}
/*
	for ( uint32_t i = 0; i < cmpCount; i++ )
	{
		if ( pBuff[i] < buff[i] )
			return ( -1 );
		else if ( pBuff[i] > buff[i] )
			return ( 1 );
	}
*/
	if ( srcCount < count )
		return -1;
	else if ( srcCount > count )
		return 1;

	return 0;
}

template <class T>
short TString<T>::CompareNoCase(uint32_t at, const T *buff, uint32_t count) const
{
	if ( buff == NULL )
	{
		return mBuff ? 1 : 0;
	}

	if ( mBuff == NULL )
	{
		return buff ? -1 : 0;
	}

	uint32_t	length = mCount - at;
	uint32_t	cmpCount = ( length < count ) ? length : count;

	T	*pBuff = mBuff + at;
	T	ch1;
	T	ch2;

	for ( uint32_t i = 0; i < cmpCount; i++ )
	{	
		ch1 = pBuff[i];
		ch2 = buff[i];

		if ( ch1 >= 'A' && ch1 <= 'Z' )
			ch1 += 0x20;

		if ( ch2 >= 'A' && ch2 <= 'Z' )
			ch2 += 0x20;

		if ( ch1 < ch2 )
			return ( -1 );
		else if ( ch1 > ch2 )
			return ( 1 );
	}

	if ( length < count )
		return -1;
	else if ( length > count )
		return 1;

	return 0;
}


template <class T>
short TString<T>::CompareNoCase(uint32_t at, uint32_t srcCount, const T *buff, uint32_t count) const
{
	if ( buff == NULL )
	{
		return mBuff ? 1 : 0;
	}

	if ( mBuff == NULL )
	{
		return buff ? -1 : 0;
	}

	uint32_t	len = mCount - at;
	if ( srcCount > len )
		srcCount = len;

	uint32_t	cmpCount = ( srcCount < count ) ? srcCount : count;

	T	*pBuff = mBuff + at;
	T	ch1;
	T	ch2;
	for ( uint32_t i = 0; i < cmpCount; i++ )
	{	
		ch1 = pBuff[i];
		ch2 = buff[i];

		if ( ch1 >= 'A' && ch1 <= 'Z' )
			ch1 += 0x20;

		if ( ch2 >= 'A' && ch2 <= 'Z' )
			ch2 += 0x20;

		if ( ch1 < ch2 )
			return ( -1 );
		else if ( ch1 > ch2 )
			return ( 1 );
	}

	if ( srcCount < count )
		return -1;
	else if ( srcCount > count )
		return 1;

	return 0;
}

template <class T>
void TString<T>::Fill(T ch, uint32_t count)
{
	for ( uint32_t i = 0; i < count; i++ )
	{	
		Append(ch);
	}
}

/*
template <class T>
short TString<T>::CompareNoCase(uint32_t at, const T *buff, uint32_t count) const
{
	uint32_t	length = mCount - at;
	uint32_t	cmpCount = ( length < count ) ? length : count;

	TString<T> src;
	GetString(at, cmpCount, src);
	src.MakeLower();

	TString<T> des(buff, count);
	des.MakeLower();

	return src.Compare(des);
}
*/

/*
template <class T>
T* TString<T>::SetCount(uint32_t newCount)
{
	if ( SetCapacity(newCount) )
	{
		mCount = newCount;
		mBuff[mCount] = 0;

		return mBuff;
	}

	return NULL;
}
*/


template <class T>
T* TString<T>::SetCount(uint32_t newCount)
{
	if ( newCount > mCapacity )
	{
		T*	pBuffer = TMemSetSize(mBuff, newCount+1, T);
		if ( pBuffer == NULL )
		{
			if ( newCount < mCount )
			{
				mCount = newCount;
				if ( mBuff )
					mBuff[newCount] = 0;

				return mBuff;
			}

			return NULL;
		}

		mBuff = pBuffer;
		mCount = newCount;
		mCapacity = newCount;
		mBuff[mCount] = 0;
	}
	else
	{
		mCount = newCount;
		if ( mBuff )
			mBuff[mCount] = 0;
	}

	return mBuff;
}


template <class T>
bool TString<T>::SetCapacity(uint32_t capacity)
{
	uint32_t	newCapacity = CalcCapacity(capacity);
//	if ( newCapacity > GetMaxCount() ) return FALSE;

	if ( newCapacity != mCapacity )
	{
		T*	pBuffer = TMemSetSize(mBuff, (newCapacity+1), T);
		if ( pBuffer == NULL ) return FALSE;

		mBuff = pBuffer;
		mCapacity = newCapacity;

		if ( newCapacity < mCount )
			mCount = newCapacity;

		mBuff[mCapacity] = 0;
	}

	return mBuff != NULL;
}

template <class T>
void TString<T>::FreeExtra(void)
{
	if ( mBuff && mCount < mCapacity )
	{
		SetCount(mCount);
	}
}

/*
template <class T>
void TString<T>::Move(uint32_t srcAt, uint32_t desAt)
{
	if ( srcAt == desAt ) return;
	if ( srcAt > desAt )
	{
		T temp = mBuff[srcAt];
		SafeCopyChar(mBuff + desAt, mBuff + desAt+1, srcAt - desAt);
		mBuff[desAt] = temp;

	}
	else
	{
		T temp = mBuff[srcAt];
		SafeCopyChar(mBuff + srcAt+1, mBuff + srcAt, desAt - srcAt);
		mBuff[desAt] = temp;

	}
}
*/

template <class T>
void TString<T>::Attach(const T *buff, uint32_t count, uint32_t capacity)
{
	if ( mBuff )
		TMemFree(mBuff);

	mBuff = (T *)buff;
	mCount =  count;
	mCapacity = capacity;
}

template <class T>
void TString<T>::Attach(TString<T> &string)
{
	if ( mBuff )
		TMemFree(mBuff);

	mCount =  string.GetCount();
	mCapacity = string.GetCapacity();
	mBuff = string.Detach();
}

template <class T>
T* TString<T>::Detach()
{
	T *p = mBuff;

	mBuff = NULL;
	mCount =  0;
	mCapacity = 0;

	return p;
}


template <class T>
bool TString<T>::StartWith(STRCount_t count, const T *pCh, bool ignoreCase/*=false*/) const
{
//	if ( count > mCount ) return false;

	if ( ignoreCase )
	{
		return CompareNoCase(count, pCh, 0, count) == 0;
	}
	
	return Compare(count, pCh, 0, count) == 0;
}

template <class T>
bool TString<T>::EndWith(STRCount_t count, const T *pCh, bool ignoreCase/*=false*/) const
{
	if ( count > mCount ) return false;
	
	if ( ignoreCase )
	{
		return CompareNoCase(count, pCh, mCount - count) == 0;
	}
	
	return Compare(count, pCh, mCount - count) == 0;
}


template<class T>
bool operator == (const T* s1, const TString<T> &s2)
{
	return s2 == s1;
}

template<class T>
bool operator != (const T* s1, const TString<T> &s2)
{
	return s2 != s1;
}

template<class T>
bool operator < (const T* s1, const TString<T> &s2)
{
	return s2.Compare(0, s1, CH_LENGTH(s1)) > 0;
}

template<class T>
bool operator <= (const T* s1, const TString<T> &s2)
{
	return s2.Compare(0, s1, CH_LENGTH(s1)) >= 0;
}

template<class T>
bool operator > (const T* s1, const TString<T> &s2)
{
	return s2.Compare(0, s1, CH_LENGTH(s1)) < 0;
}

template<class T>
bool operator >= (const T* s1, const TString<T> &s2)
{
	return s2.Compare(0, s1, CH_LENGTH(s1)) <= 0;
}



#endif	/* __T_STRING__ */

