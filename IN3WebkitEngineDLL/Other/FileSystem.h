/* ********************************************************************************
 *
 * Purpose:	interface for the File System
 *
 * Version	: v1.0
 * Author	: IKHAN CHO
 * Copyright :(c) 2009 Incube Tech. All rights reserved.
 *
 * Modification History (most recent first):
 
 *			 1 DEC 2009		IKHAN CHO,  first design for Mac
 *			 1 JUN 2009		IKHAN CHO,  first design for Windows
 *
 * Known problem:
 *
 * Note :
 * ******************************************************************************* */

#ifndef __FILE_SYSTEM__
#define __FILE_SYSTEM__


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdio.h>
#include "TStringEx.h"


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


#if defined(WIN32)
#define PATH_SEPARATOR_CHAR			'\\'
#define PATH_SEPARATOR_STR_W 		L"\\"
#define PATH_SEPARATOR_STR_A		"\\"
#elif defined(__APPLE_CPP__) || defined(__APPLE_CC__)
#define PATH_SEPARATOR_CHAR		'/'
const unsigned short PATH_SEPARATOR_STR_W[2] =	{'/', 0};
#define PATH_SEPARATOR_STR_A					"/"
#else
#define PATH_SEPARATOR_CHAR		'/'
const wchar_t PATH_SEPARATOR_STR_W[2] =			{'/', 0};
#define PATH_SEPARATOR_STR_A					"/"
#endif



#if defined(WIN32) || defined(_WIN32_WCE)

#if defined(UNICODE) || defined(_UNICODE)
typedef UniString			FSString;
#else
typedef MBString			FSString;
#endif

#elif defined(__APPLE_CPP__) || defined(__APPLE_CC__)

//typedef TMBString<TE_FS>	FSString;
typedef UTF8String			FSString;

#else

typedef UTF8String			FSString;

#endif


#if 0

#define _O_RDONLY       0x0000  /* open for reading only */
#define _O_WRONLY       0x0001  /* open for writing only */
#define _O_RDWR         0x0002  /* open for reading and writing */
#define _O_APPEND       0x0008  /* writes done at eof */

#define _O_CREAT        0x0100  /* create and open file */
#define _O_TRUNC        0x0200  /* open and truncate */
#define _O_EXCL         0x0400  /* open only if file doesn't already exist */

/* O_TEXT files have <cr><lf> sequences translated to <lf> on read()'s,
** and <lf> sequences translated to <cr><lf> on write()'s
*/

#define _O_TEXT         0x4000  /* file mode is text (translated) */
#define _O_BINARY       0x8000  /* file mode is binary (untranslated) */
#define _O_WTEXT        0x10000 /* file mode is UTF16 (translated) */
#define _O_U16TEXT      0x20000 /* file mode is UTF16 no BOM (translated) */
#define _O_U8TEXT       0x40000 /* file mode is UTF8  no BOM (translated) */

#endif


typedef enum {

	// open mode, exclusive
	FS_EXIST_OPEN		= 0x0,
	FS_NEW_CREATE		= 0x01,
	FS_APPEND			= 0x02,
	FS_EXIST_APPEND		= FS_APPEND,

	// reading and writing 
//	FS_READ				= 0x10,
//	FS_WRITE			= 0x20,
	FS_READWRITE		= 0x30,

	// text or binary mode, exclusive
	FS_TEXT				= 0x40,
	FS_BINARY			= 0x80,
} FS_MODE;


FILE *FSOpenFile(const unicode_t *fileName, int mode);
FILE *FSOpenFile(const char *fileName, int mode);


FILE *FSOpenFile(const unicode_t *fileName);
FILE *FSOpenFile(const char *fileName);

FILE *FSSCreateFile(const unicode_t *fileName);
FILE *FSSCreateFile(const char *fileName);

bool FSExistFile(const unicode_t *fileName);
bool FSExistFile(const char *fileName);

bool FSDeleteFile(const unicode_t *fileName);
bool FSDeleteFile(const char *fileName);

bool FSCopyFile(const unicode_t *srcFileName, const unicode_t *desFileName, bool bFailIfExists = true);
bool FSCopyFile(const char *srcFileName, const char *desFileName, bool bFailIfExists = true);

bool FSCreateDirectory(const unicode_t *dirName);
bool FSCreateDirectory(const char *dirName);

void FSDeleteFolderAndFiles(const char *dir);
void FSDeleteFolderAndFiles(const unicode_t *dir);

bool FSExistDirectory(const unicode_t *dirName);
bool FSExistDirectory(const char *dirName);

void FSAppendPath(UniString &string, const unicode_t *path);
void FSAppendPath(MBString &string, const char *path);

#if !(defined(WIN32) || defined(_WIN32_WCE))
bool FSGetApplPath(FSString &applPath, bool withPS = false);
#endif

void FSGetSplitPath(const char *path, UniString *pDrive, UniString *pDir, UniString *fileTitle, UniString *exeName);
void FSGetSplitPath(const unicode_t *path, UniString *pDrive, UniString *pDir, UniString *fileTitle, UniString *exeName);
#if !(defined(WIN32) || defined(_WIN32_WCE)) || !(defined(UNICODE) || defined(_UNICODE))
void FSGetSplitPath(const char *path, FSString *pDrive, FSString *pDir, FSString *fileTitle, FSString *exeName);
#endif

bool FSEnsureDirectory(const unicode_t* lpPathName);

bool FSGetApplPath(UniString &applPath, bool withPS = false);
bool FSGetApplPath(MBString &applPath, bool withPS = false);


//#ifdef _XP_XTPLATE8_H_

#if defined(WIN32) || defined(_WIN32_WCE)
#define XT_GetPaltformPathFromXFileInfoRef(a,b)		XTGetUniPathFromXFileInfoRef(a,b)
#else
#define XT_GetPaltformPathFromXFileInfoRef(a,b)		XTGetPOSIXPathFromXFileInfoRef(a,b)
#endif

//#endif	// _XP_XTPLATE8_H_


class FSFileFind
{
public:
	FSFileFind();
	~FSFileFind();

	bool FindFileInDir(const unicode_t *path);
	bool FindNextFile();
	void Close();

	void GetFileName(UniString &fileName);
	void GetFilePath(UniString &path);
//	void GetFileTitle(UniString &fileTitle);

	bool IsDirectory();
	bool IsDot();

protected:
#if defined(_WIN32_WCE)
	HANDLE				mFind;
	WIN32_FIND_DATA		mFindFileData;
#elif defined(WIN32)
	HANDLE				mFind;
	WIN32_FIND_DATAW	mFindFileData;
	UniString			mFilePath;
#endif

};

#endif	// __FILE_SYSTEM__

