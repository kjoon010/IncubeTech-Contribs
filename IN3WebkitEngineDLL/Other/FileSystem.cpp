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
#include "stdafx.h"
#include "FileSystem.h"
#include <errno.h>

#if defined(WIN32)
#include <wchar.h>
//#include <sys/stat.h>
//#include <direct.h>
#elif defined(_WIN32_WCE)
#include <Winbase.h>
#include <wchar.h>
#else
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#endif

#if defined(__APPLE_CPP__) || defined(__APPLE_CC__)
//#include <copyfile.h>
#endif

#define NPSUSP(a)	NormalPathSeparator(UniString(a))
#define NPSUSC(s)	NormalPathSeparator(s)


UniString & NormalPathSeparator(UniString &unistring)
{
#if defined(WIN32) || defined(_WIN32_WCE)
	unistring.Replace('/', '\\');
#else
	unistring.Replace('\\', '/');
#endif

	return unistring;
}

FILE *FSOpenFile(const unicode_t *fileName, int mode)
{
	if ( fileName == NULL ) return NULL;
	if ( *fileName == 0 ) return NULL;

#if defined(WIN32) || defined(_WIN32_WCE)

	unicode_t	modeStr[4];
	int		pos = 0;
	
	if ( mode & FS_NEW_CREATE )
		modeStr[pos] = 'w';
	else if ( mode & FS_APPEND )
		modeStr[pos] = 'a';
	else
		modeStr[pos] = 'r';
	pos++;
	
	if ( mode & FS_BINARY )
	{
		modeStr[pos] = 'b';
		pos++;
	}
	else if ( mode & FS_TEXT )
	{
		modeStr[pos] = 't';
		pos++;
	}

	if ( mode & FS_READWRITE )
	{
		modeStr[pos] = '+';
		pos++;
	}
	modeStr[pos] = 0;
	
	return _wfopen(fileName, modeStr);
	
#else
	
	char	modeStr[4];
	int		pos = 0;
	
	if ( mode & FS_NEW_CREATE )
		modeStr[pos] = 'w';
	else if ( mode & FS_APPEND )
		modeStr[pos] = 'a';
	else
		modeStr[pos] = 'r';
	pos++;
	
	if ( mode & FS_BINARY )
	{
		modeStr[pos] = 'b';
		pos++;
	}
	else if ( mode & FS_TEXT )
	{
		modeStr[pos] = 't';
		pos++;
	}

	if ( mode & FS_READWRITE )
	{
		modeStr[pos] = '+';
		pos++;
	}
	modeStr[pos] = 0;
	
#if defined(__APPLE_CPP__) || defined(__APPLE_CC__)

	FSString	fsString(fileName);
	return fopen(fsString.GetString(), modeStr);	// FSOpenFile(fsString.GetString());
#else
	FSString	fsString(fileName);
	return fopen(fsString.GetString(), modeStr);	// FSOpenFile(utf8String.GetString());
#endif
	
#endif
}

FILE *FSOpenFile(const char *fileName, int mode)
{
#if defined(WIN32) || defined(_WIN32_WCE)

	if ( fileName == NULL ) return NULL;
	if ( *fileName == 0 ) return NULL;

	char	modeStr[4];
	int		pos = 0;

	if ( mode & FS_NEW_CREATE )
		modeStr[pos] = 'w';
	else if ( mode & FS_APPEND )
		modeStr[pos] = 'a';
	else
		modeStr[pos] = 'r';
	pos++;

	if ( mode & FS_BINARY )
	{
		modeStr[pos] = 'b';
		pos++;
	}
	else if ( mode & FS_TEXT )
	{
		modeStr[pos] = 't';
		pos++;
	}

	if ( mode & FS_READWRITE )
	{
		modeStr[pos] = '+';
		pos++;
	}
	modeStr[pos] = 0;

	return fopen(fileName, modeStr);
#elif defined(__APPLE_CPP__) || defined(__APPLE_CC__)
	UniString	uniString(fileName);
	return FSOpenFile(uniString);
#else
	UniString	uniString(fileName);
	return FSOpenFile(uniString);
#endif
}



FILE *FSOpenFile(const unicode_t *fileName)
{
	if ( fileName == NULL ) return NULL;
	if ( *fileName == 0 ) return NULL;

#if defined(WIN32) || defined(_WIN32_WCE)
	return _wfopen(fileName, L"rb+");
#elif defined(__APPLE_CPP__) || defined(__APPLE_CC__)
	FSString	fsString(fileName);
	return fopen(fsString.GetString(), "rb+");	// FSOpenFile(fsString.GetString());
#else
	FSString	fsString(fileName);
	return fopen(fsString.GetString(), "rb+");	// FSOpenFile(utf8String.GetString());
#endif
}

FILE *FSOpenFile(const char *fileName)
{
#if defined(WIN32) || defined(_WIN32_WCE)

	if ( fileName == NULL ) return NULL;
	if ( *fileName == 0 ) return NULL;

	return fopen(fileName, "rb+");

#elif defined(__APPLE_CPP__) || defined(__APPLE_CC__)
	UniString	uniString(fileName);
	return FSOpenFile(uniString);
#else
	UniString	uniString(fileName);
	return FSOpenFile(uniString);
#endif
}


FILE *FSSCreateFile(const unicode_t *fileName)
{
	if ( fileName == NULL ) return NULL;
	if ( *fileName == 0 ) return NULL;

#if defined(WIN32) || defined(_WIN32_WCE)
	return _wfopen(fileName, L"wb+");
#elif defined(__APPLE_CPP__) || defined(__APPLE_CC__)
	FSString	fsString(fileName);
	return fopen(fsString.GetString(), "wb+");	// FSSCreateFile(fsString.GetString());
#else
	FSString	fsString(fileName);
	return fopen(fsString.GetString(), "wb+");	// FSSCreateFile(fsString.GetString());
#endif
}

FILE *FSSCreateFile(const char *fileName)
{
#if defined(WIN32) || defined(_WIN32_WCE)

	if ( fileName == NULL ) return NULL;
	if ( *fileName == 0 ) return NULL;

	return fopen(fileName, "wb+");

#elif defined(__APPLE_CPP__) || defined(__APPLE_CC__)
	UniString	uniString(fileName);
	return FSSCreateFile(uniString);
#else
	UniString	uniString(fileName);
	return FSSCreateFile(uniString);
#endif
}


#if !defined(WIN32) && !defined(_WIN32_WCE)

bool _ExistFile(const char *fileName)
{
	struct stat	_Stat;
	int		result = stat( fileName, &_Stat );

	return result == 0;
/*
	if ( result != 0 )
	{
		if ( errno == ENOENT )
		{
			return false;
		}
	}

	return true;
*/
}

#endif

bool FSExistFile(const unicode_t *fileName)
{
	if ( fileName == NULL ) return false;
	if ( *fileName == 0 ) return false;

#if defined(_WIN32_WCE)

	return (GetFileAttributes(fileName) != INVALID_FILE_ATTRIBUTES);

#elif defined(WIN32)

//	return PathFileExistsW(fileName);
	return (GetFileAttributesW(fileName) != INVALID_FILE_ATTRIBUTES);
//	return _waccess(fileName, 0) == 0;

#elif defined(__APPLE_CPP__) || defined(__APPLE_CC__)
	FSString	utf8String(fileName);
	return _ExistFile(utf8String.GetString());
#else
	FSString	utf8String(fileName);
	return _ExistFile(utf8String.GetString());
#endif
}

bool FSExistFile(const char *fileName)
{
#if defined(_WIN32_WCE)
	return (GetFileAttributes(UniString(fileName)) != INVALID_FILE_ATTRIBUTES);
#elif defined(WIN32) 

	if ( fileName == NULL ) return false;
	if ( *fileName == 0 ) return false;

//	return PathFileExistsA(fileName);
	return (GetFileAttributesA(fileName) != INVALID_FILE_ATTRIBUTES);
//	return _access(fileName, 0) == 0;
#else

	UniString	uniString(fileName);
	return FSExistFile(uniString);

#endif
}


bool FSDeleteFile(const unicode_t *fileName)
{
#if defined(_WIN32_WCE)
	return DeleteFile(fileName) == TRUE;
#elif defined(WIN32)
	return DeleteFileW(fileName) == TRUE;
#elif defined(__APPLE_CPP__) || defined(__APPLE_CC__)
	FSString	fsString(fileName);
	return remove(fsString.GetString());;	// FSDeleteFile(fsString.GetString());
#else
	FSString	fsString(fileName);
	return remove(fsString.GetString());	// FSDeleteFile(fsString.GetString());
#endif
}

bool FSDeleteFile(const char *fileName)
{
#if defined(_WIN32_WCE)
	return DeleteFile(UniString(fileName)) == TRUE;
#elif defined(WIN32)
	return DeleteFileA(fileName) == TRUE;
#elif defined(__APPLE_CPP__) || defined(__APPLE_CC__)
	UniString	uniString(fileName);
	return FSDeleteFile(uniString);
#else
	UniString	uniString(fileName);
	return FSDeleteFile(uniString);
#endif
}


#if !defined(WIN32) && !defined(_WIN32_WCE)

bool _CopyFile(const unicode_t *srcFileName, const unicode_t *desFileName)
{
//	if ( strcmp(srcFileName, desFileName) == 0 ) return false;
	FILE *srcFile = FSOpenFile(srcFileName);
	if ( srcFile == NULL ) return false;

	FILE *desFile = FSSCreateFile(desFileName);
	if ( desFile == NULL )
	{
		fclose(srcFile);
		return false;
	}
	
	char buff[8092];
	long readSize = 0;
	while ( 1 )
	{
		readSize = fread(buff, 1, 8092, srcFile);
		if ( readSize <= 0 ) break;
		fwrite(buff, 1, readSize, desFile);
	}
	
	fclose(srcFile);
	fclose(desFile);
	
	return true;
}

#endif

bool FSCopyFile(const unicode_t *srcFileName, const unicode_t *desFileName, bool bFailIfExists/* = true*/)
{
#if defined(_WIN32_WCE)
	return 	CopyFile(srcFileName, desFileName, bFailIfExists) == TRUE;
#elif defined(WIN32)
	return 	CopyFileW(srcFileName, desFileName, bFailIfExists) == TRUE;
#elif defined(__APPLE_CPP__) || defined(__APPLE_CC__)
	FSString	srcString(srcFileName);
	FSString	desString(desFileName);
	
	if ( srcString == desString ) return false;
	
	if ( bFailIfExists )
	{
		if ( _ExistFile(desString.GetString()) ) return false;
	}
	
	return _CopyFile(srcFileName, desFileName);
	
//	return copyfile(srcString.GetString(), desString.GetString(), NULL, bFailIfExists ? COPYFILE_ALL | COPYFILE_EXCL : COPYFILE_ALL) == 0;
#else
	FSString	srcString(srcFileName);
	FSString	desString(desFileName);
	
	if ( srcString == desString ) return false;
	
	if ( bFailIfExists )
	{
		if ( _ExistFile(desString.GetString()) ) return false;
	}
	return copyfile(srcString.GetString(), desString.GetString(), NULL, bFailIfExists? COPYFILE_ALL | COPYFILE_EXCL : COPYFILE_ALL) == 0;
#endif
}

bool FSCopyFile(const char *srcFileName, const char *desFileName, bool bFailIfExists/* = true*/)
{
#if defined(_WIN32_WCE)
	return 	CopyFile(UniString(srcFileName), UniString(desFileName), bFailIfExists) == TRUE;
#elif defined(WIN32)
	return 	CopyFileA(srcFileName, desFileName, bFailIfExists) == TRUE;
#elif defined(__APPLE_CPP__) || defined(__APPLE_CC__)
	UniString	srcString(srcFileName);
	UniString	desString(desFileName);
	return FSCopyFile(srcString, desString);
#else
	UniString	srcString(srcFileName);
	UniString	desString(desFileName);
	return FSCopyFile(srcString, desString);
#endif
}


#if !defined(WIN32) && !defined(_WIN32_WCE)

bool _ExistDirectory(const char *dirName)
{
	struct stat	_Stat;

	_Stat.st_mode = 0;
	int		result = stat( dirName, &_Stat );

	return result == 0 && ((_Stat.st_mode & S_IFDIR) != 0);
/*
	if ( result != 0 )
	{
		if ( errno == ENOENT )
		{
			return false;
		}
	}

	return (_Stat.st_mode & S_IFDIR) != 0;
*/
}

#endif

bool FSExistDirectory(const unicode_t *dirName)
{
	if ( dirName == NULL ) return false;
	if ( *dirName == 0 ) return false;

#if defined(_WIN32_WCE)

	DWORD attr = GetFileAttributes(dirName);
	return ((attr != INVALID_FILE_ATTRIBUTES) && (attr & FILE_ATTRIBUTE_DIRECTORY));

#elif defined(WIN32)

	DWORD attr = GetFileAttributesW(dirName);
	return ((attr != INVALID_FILE_ATTRIBUTES) && (attr & FILE_ATTRIBUTE_DIRECTORY));

#elif defined(__APPLE_CPP__) || defined(__APPLE_CC__)
	FSString	uniString(dirName);
	return _ExistDirectory(uniString.GetString());
#else
	FSString	uniString(dirName);
	return _ExistDirectory(uniString.GetString());
#endif
}

bool FSExistDirectory(const char *dirName)
{
#if defined(_WIN32_WCE)
	return FSExistDirectory( UniString(dirName) );
#elif defined(WIN32) 

	if ( dirName == NULL ) return false;
	if ( *dirName == 0 ) return false;

	DWORD attr = GetFileAttributesA(dirName);
	return ((attr != INVALID_FILE_ATTRIBUTES) && (attr & FILE_ATTRIBUTE_DIRECTORY));
	
#else
	UniString	uniString(dirName);
	return FSExistDirectory(uniString);
#endif
}


bool FSCreateDirectory(const unicode_t *dirName)
{
	if ( dirName == NULL ) return false;
	if ( *dirName == 0 ) return false;

#if defined(_WIN32_WCE)
	return CreateDirectory(dirName, NULL) == TRUE;
#elif defined(WIN32) 
	return CreateDirectoryW(dirName, NULL) == TRUE;
//	return _wmkdir(dirName) == 0;
#elif defined(__APPLE_CPP__) || defined(__APPLE_CC__)
	FSString	fsString(dirName);
	return mkdir(fsString.GetString(), 0777) == 0;
#else
	FSString	fsString(dirName);
	return mkdir(fsString.GetString(), 0777) == 0;
#endif
}

bool FSCreateDirectory(const char *dirName)
{
#if defined(_WIN32_WCE)

	if ( dirName == NULL ) return false;
	if ( *dirName == 0 ) return false;

	return CreateDirectory(UniString(dirName), NULL) == TRUE;

#elif defined(WIN32)

	if ( dirName == NULL ) return false;
	if ( *dirName == 0 ) return false;

	return CreateDirectoryA(dirName, NULL) == TRUE;
//	return _mkdir(dirName) == 0;
#else
	UniString	uniString(dirName);
	return FSCreateDirectory(uniString);
#endif
}


#if !defined(WIN32) && !defined(_WIN32_WCE)

void _DeleteFolderAndFiles(const char *dirName)
{
	FSString	fsString;

	DIR	*dirp = NULL;
	dirp = opendir(dirName);
	if ( dirp == NULL ) return;
	
	struct dirent *dp;
	while ((dp = readdir(dirp)) != NULL)
	{
	//	printf("%s\n", dp->d_name);
		if ( dp->d_type == DT_REG )
		{
			fsString = dirName;
			fsString.Append(PATH_SEPARATOR_CHAR);
			fsString.Append( dp->d_name);
			
			remove(fsString.GetString());
		}
		else if (dp->d_type == DT_DIR )
		{
			if (strcmp(dp->d_name, ".") == 0 ) continue;
			if (strcmp(dp->d_name, "..") == 0 ) continue;
			
			fsString = dirName;
			fsString.Append(PATH_SEPARATOR_CHAR);
			fsString.Append( dp->d_name);
			
			_DeleteFolderAndFiles(fsString.GetString());
		}
	}
	closedir(dirp);

	rmdir(dirName);
}

#endif


void FSDeleteFolderAndFiles(const char *dirName)
{
#if defined(WIN32) || defined(_WIN32_WCE)
	UniString	uniString(dirName);
	FSDeleteFolderAndFiles(uniString);
#elif defined(__APPLE_CPP__) || defined(__APPLE_CC__)
	UniString	uniString(dirName);
	FSDeleteFolderAndFiles(uniString.GetString());
#else
	UniString	uniString(dirName);
	FSDeleteFolderAndFiles(uniString);
#endif
}

void FSDeleteFolderAndFiles(const unicode_t *dirName)
{
	if ( dirName == NULL ) return;
	if ( *dirName == 0 ) return;

#if defined(WIN32) || defined(_WIN32_WCE)

	UniString	findDir(dirName);

	if ( !findDir.EndWith(USTR("\\*.*")) )
	{
		if ( findDir.GetLastChar() == '\\' )
			findDir += L"*.*";
		else
			findDir += L"\\*.*";

		if ( findDir.GetCount() <= 6 ) return;
	}
	else
	{
		if ( findDir.GetCount() <= 6 ) return;
	}

	HANDLE hFind;

#if defined(_WIN32_WCE)
	WIN32_FIND_DATA findFileData;
	hFind = FindFirstFile(findDir.GetString(), &findFileData);
#else
	WIN32_FIND_DATAW findFileData;
	hFind = FindFirstFileW(findDir.GetString(), &findFileData);
#endif

	if (hFind != INVALID_HANDLE_VALUE) 
	{
		UniString	filePath;

		do
		{
			if ( wcscmp(findFileData.cFileName, L".") == 0 || wcscmp(findFileData.cFileName, L"..") == 0 )
				continue;

			filePath = dirName;
			filePath += L"\\";
			filePath += findFileData.cFileName;

			if ( findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				FSDeleteFolderAndFiles(filePath);
			}
			else
			{
				if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
					SetFileAttributesW(filePath.GetString(), FILE_ATTRIBUTE_NORMAL);

			#if defined(_WIN32_WCE)
				DeleteFile(filePath);
			#else
				DeleteFileW(filePath);
			#endif
			}
		}
	#if defined(_WIN32_WCE)
		while (FindNextFile(hFind, &findFileData) != 0);
	#else
		while (FindNextFileW(hFind, &findFileData) != 0);
	#endif

		FindClose(hFind);
	}

	RemoveDirectoryW(dirName);

#elif defined(__APPLE_CPP__) || defined(__APPLE_CC__)
	FSString	fsString(dirName);
	
	if ( fsString.GetCount() <= 1 ) return;

	_DeleteFolderAndFiles(fsString.GetString());
#else
	FSString	fsString(dirName);
	_DeleteFolderAndFiles(fsString.GetString());
#endif
}



bool FSGetApplPath(UniString &applPath, bool withPS/* = true*/)
{
#if defined(WIN32) || defined(_WIN32_WCE)

    wchar_t    Temp[_MAX_PATH+1] = L"";
#if defined(_WIN32_WCE)
	::GetModuleFileName(NULL, Temp, _MAX_PATH);
#else
	::GetModuleFileNameW(NULL, Temp, _MAX_PATH);
#endif

	applPath = Temp;

	STRIndex_t pos = applPath.FindRAt(PATH_SEPARATOR_CHAR);
	if ( STRING_FOUND(pos) )
		applPath.Truncate( withPS ? pos + 1 : pos);

	return true;

#else

	CFBundleRef	bundleRef = CFBundleGetMainBundle();
	if ( bundleRef == NULL ) return false;

	CFURLRef	urlRef = CFBundleCopyBundleURL(bundleRef);
	CFRelease(bundleRef);

	if ( urlRef )
	{
		CFStringRef	stringRef = CFURLCopyFileSystemPath(urlRef, kCFURLPOSIXPathStyle);
		CFRelease(urlRef);

		if ( stringRef )
		{
			applPath = stringRef;
			CFRelease(stringRef);
	
			STRIndex_t pos = applPath.FindRAt(PATH_SEPARATOR_CHAR);
			if ( STRING_FOUND(pos) )
			{
				if (!withPS)
					applPath.Truncate(pos);
			}
			else
			{
				if (withPS)
					applPath += PATH_SEPARATOR_CHAR;
			}

			return true;
		}
	}

	return false;

#endif

}

bool FSGetApplPath(MBString &applPath, bool withPS/* = false*/)
{
	UniString	uniString;
	if ( FSGetApplPath(uniString, withPS) )
	{
		applPath = uniString;
		return true;
	}
	return false;
}


#if !(defined(WIN32) || defined(_WIN32_WCE))

bool FSGetApplPath(FSString &applPath, bool withPS/* = true*/)
{
 #if defined(WIN32) || defined(_WIN32_WCE)

	char    Temp[_MAX_PATH+1];
	::GetModuleFileNameA(NULL, Temp, _MAX_PATH);

	applPath = Temp;

	STRIndex_t pos = applPath.FindRAt(PATH_SEPARATOR_CHAR);
	if ( STRING_FOUND(pos) )
		applPath.Truncate( withPS ? pos + 1 : pos );

	return true;

#else

	UniString uniString;
	if ( FSGetApplPath(uniString, withPS) )
	{
		applPath = uniString;
	}

	return false;

#endif
}

#endif


void FSGetSplitPath(const char *path, UniString *pDrive, UniString *pDir, UniString *fileTitle, UniString *exeName)
{
	UniString	pathStr(path);
	FSGetSplitPath(pathStr, pDrive, pDir, fileTitle, exeName);
}


void FSGetSplitPath(const unicode_t *path, UniString *pDrive, UniString *pDir, UniString *fileTitle, UniString *exeName)
{
#if defined(WIN32) || defined(_WIN32_WCE)

	wchar_t drive[_MAX_DRIVE] = {0x00};
	wchar_t dir[_MAX_DIR] = {0x00};
	wchar_t fname[_MAX_FNAME] = {0x00};				//
	wchar_t ext[_MAX_EXT] =  {0x00};

#if defined(_WIN32_WCE)
	_wsplitpath_s(path, drive, _MAX_DRIVE, dir, _MAX_DIR, fname, _MAX_FNAME, ext, _MAX_EXT);
#else
	_wsplitpath(path, drive, dir, fname, ext);
#endif

	if ( pDrive )
		*pDrive = drive;

	if ( pDir )
		*pDir = dir;

	if ( fileTitle )
		*fileTitle = fname;

	if ( exeName )
		*exeName = ext;

#else
	
	UniString	fsString(path);
	STRIndex_t	drivePos = 0;

#if defined(WIN32) || defined(_WIN32_WCE)
	drivePos = fsString.FindAt(':');
	if ( STRING_FOUND(drivePos) )
	{
		drivePos++;
		if ( pDrive )
			fsString.Left(drivePos, *pDrive);
	}
	else
	{
		drivePos = 0;
		if ( pDrive )
			pDrive->Empty();
	}
#else
	if ( pDrive )
		pDrive->Empty();
#endif

	STRIndex_t	exePos = fsString.FindRAt('.');
	if ( STRING_FOUND(exePos) )
	{
		if ( exeName )
			fsString.Right(fsString.GetCount() - exePos, *exeName);
	}
	else
	{
		if ( exeName )
			exeName->Empty();

		exePos = fsString.GetCount();
	}

	STRIndex_t	dirEnd = fsString.FindRAt(PATH_SEPARATOR_CHAR, exePos);
	if ( STRING_FOUND(dirEnd) )
	{
		dirEnd++;
		if ( fileTitle )
			fsString.GetString(dirEnd, exePos - dirEnd, *fileTitle);
	}
	else
	{
		if ( fileTitle )
			fsString.GetString(drivePos, exePos - drivePos, *fileTitle);

		if ( pDir )
			pDir->Empty();

		return;
	}

	if ( pDir )
	{
		STRIndex_t	dirStart = fsString.FindAt(PATH_SEPARATOR_CHAR, drivePos);
		if ( dirStart >= 0 && dirStart < dirEnd )
		{
			fsString.GetString(dirStart, dirEnd - dirStart, *pDir);
		}
		else
		{
			pDir->Empty();
		}
	}

#endif
}

#if !(defined(WIN32) || defined(_WIN32_WCE)) || !(defined(UNICODE) || defined(_UNICODE))

void FSGetSplitPath(const char *path, FSString *pDrive, FSString *pDir, FSString *fileTitle, FSString *exeName)
{
	FSString	fsString(path);
	STRIndex_t	drivePos = 0;

#if defined(WIN32) || defined(_WIN32_WCE)
	drivePos = fsString.FindAt(':');
	if ( STRING_FOUND(drivePos) )
	{
		drivePos++;
		if ( pDrive )
			fsString.Left(drivePos, *pDrive);
	}
	else
	{
		drivePos = 0;
		if ( pDrive )
			pDrive->Empty();
	}
#else
	if ( pDrive )
		pDrive->Empty();
#endif

	STRIndex_t	exePos = fsString.FindRAt('.');
	if ( STRING_FOUND(exePos) )
	{
		if ( exeName )
			fsString.Right(fsString.GetCount() - exePos, *exeName);
	}
	else
	{
		if ( exeName )
			exeName->Empty();
		
		exePos = fsString.GetCount();
	}
	
	STRIndex_t	dirEnd = fsString.FindRAt(PATH_SEPARATOR_CHAR, exePos);
	if ( STRING_FOUND(dirEnd) )
	{
		dirEnd++;
		if ( fileTitle )
			fsString.GetString(dirEnd, exePos - dirEnd, *fileTitle);
	}
	else
	{
		if ( fileTitle )
			fsString.GetString(0, exePos, *fileTitle);
		
		if ( pDir )
			pDir->Empty();
		
		return;
	}
	
	if ( pDir )
	{
		STRIndex_t	dirStart = fsString.FindAt(PATH_SEPARATOR_CHAR, drivePos);
		if ( dirStart >= 0 && dirStart < dirEnd )
		{
			fsString.GetString(dirStart, dirEnd - dirStart, *pDir);
		}
		else
		{
			pDir->Empty();
		}
	}
}

#endif


void FSAppendPath(UniString &string, const unicode_t *path)
{
	if ( string.GetLastChar() != PATH_SEPARATOR_CHAR )
		string += PATH_SEPARATOR_CHAR;
	string += path;
}
void FSAppendPath(MBString &string, const char *path)
{
	if ( string.GetLastChar() != PATH_SEPARATOR_CHAR )
		string += PATH_SEPARATOR_CHAR;
	string += path;
}


bool FSEnsureDirectory(const unicode_t* lpPathName)
{
	int nLen = (int)CH_LENGTH(lpPathName);

	UniString	path;
	for (int i = nLen - 1; i >= 0; i--)
	{
		if (lpPathName[i] == PATH_SEPARATOR_CHAR )
		{
			path.Init(i, lpPathName);

			if ( FSExistFile(path) ) break;

			if (!FSEnsureDirectory(path) )
			{
				return false;
			}
		}
	}

	return  ::FSCreateDirectory(lpPathName);
}


FSFileFind::FSFileFind()
{
#if defined(WIN32) || defined(_WIN32_WCE)
	mFind = INVALID_HANDLE_VALUE;
	memset(&mFindFileData, 0, sizeof(mFindFileData));
#endif
}

FSFileFind::~FSFileFind()
{
#if defined(WIN32) || defined(_WIN32_WCE)
	if ( mFind != INVALID_HANDLE_VALUE )
		FindClose(mFind);
#endif
}

void FSFileFind::Close()
{
#if defined(WIN32) || defined(_WIN32_WCE)
	if ( mFind != INVALID_HANDLE_VALUE )
	{
		FindClose(mFind);
		mFind = INVALID_HANDLE_VALUE;

		memset(&mFindFileData, 0, sizeof(mFindFileData));
	}
#endif
}

bool FSFileFind::FindFileInDir(const unicode_t *path)
{
	Close();

	UniString	findDir(path);

	if ( findDir.EndWith(USTR("\\*.*")) )
	{
		findDir.Left(findDir.GetCount() - 4, mFilePath);
	}
	else if ( findDir.GetLastChar() == '\\' )
	{
		findDir.Left(findDir.GetCount() - 1, mFilePath);
		findDir += L"*.*";
	}
	else
	{
		mFilePath = findDir;
		findDir += L"\\*.*";
	}

	if ( !FSExistDirectory(mFilePath) ) return false;

#if defined(WIN32) || defined(_WIN32_WCE)

/*
	if ( !findDir.EndWith(USTR("\\*.*")) )
	{
		if ( findDir.GetLastChar() == '\\' )
			findDir += L"*.*";
		else
			findDir += L"\\*.*";
	}
	findDir.Left(findDir.GetCount() - 4, mFilePath);
*/
	
#if defined(_WIN32_WCE)
	mFind = FindFirstFile(findDir.GetString(), &mFindFileData);
#else
	mFind = FindFirstFileW(findDir.GetString(), &mFindFileData);
#endif

	return (mFind != INVALID_HANDLE_VALUE);

#else


#endif

	return false;
}

bool FSFileFind::FindNextFile()
{
#if defined(WIN32) || defined(_WIN32_WCE)

	if (mFind == INVALID_HANDLE_VALUE)
			return false;

#if defined(_WIN32_WCE)
	return ::FindNextFile(mFind, &mFindFileData) = TRUE;
#else
	return ::FindNextFileW(mFind, &mFindFileData) == TRUE;
#endif

#else

#endif

	return false;
}


void FSFileFind::GetFilePath(UniString &path)
{
#if defined(WIN32) || defined(_WIN32_WCE)

	path = mFilePath;
	path += '\\';
	path += mFindFileData.cFileName;

#endif
}
void FSFileFind::GetFileName(UniString &fileName)
{
#if defined(WIN32) || defined(_WIN32_WCE)

	fileName = mFindFileData.cFileName;

#endif
}


bool FSFileFind::IsDirectory()
{
#if defined(WIN32) || defined(_WIN32_WCE)

//	path = mFilePath;
//	path += '\\';
//	path += mFindFileData.cFileName;
//	return FSExistDirectory(path);

	return (mFindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY;

#endif
}

bool FSFileFind::IsDot()
{
#if defined(WIN32) || defined(_WIN32_WCE)

	if ( wcscmp(mFindFileData.cFileName, L".") == 0 )
		return true;
	else if ( wcscmp(mFindFileData.cFileName, L".") == 0 )
		return true;
	
	return false;
#endif
}



/*
void FSFileFind::GetFileTitle(UniString &fileTitle)
{

}

*/
