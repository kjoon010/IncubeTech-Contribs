#include "StdAfx.h"
#include "BookInfo.h"
#include "IN3WebkitDefine.h"

/**
 * @brief	생성자
 */
CBookInfo::CBookInfo(void)
	:IN3BookInfoEx()
	,m_unzipRootPath(_T(""))
	,m_nNavType(NONE)
	,m_packageName(_T(""))
	,m_opsFilePath(_T(""))
	,m_tocFilePath(_T(""))
	,m_fixedWidth(DEFAULT_CONTENT_WIDTH)
	,m_fixedHeight(DEFAULT_CONTENT_HEIGHT)
{
}

/**
 * @brief	소멸자
 */
CBookInfo::~CBookInfo(void)
{
}

/**
 * @brief	toc 타입 설정
 * @remars	3.0 spec = nav, 2.0 spec = ncx
 * @param	type	: toc 타입
 */
void CBookInfo::SetNavType(long type) 
{ 
	m_nNavType = type; 
};

/**
 * @brief	fixed layout 옵션 설정
 * @remarks	samsung spec 에 따르면 fixed 레이아웃은 PORTRAIT(세로), LANDSCAPE(가로), BOTH(세로/가로 모두 지원) 형식으로 다뉜다.
 * @param	opt	: fixed layout 옵션
 */
void CBookInfo::SetFixedOption(long opt) 
{ 
	m_fixedOption = opt; 
};

/**
 * @brief	package Folder 이름을 설정한다.
 * @remarks	ex) OEBPS
 * @param	packageName	: package Folder 이름
 */
void CBookInfo::SetPakcageName(CString packageName) 
{ 
	m_packageName = packageName; 
};

/**
 * @brief	ops 파일의 상대 경로를 설정한다.
 * @remarks	$(root)\$(packageName)\content.opf
 *			ex) $(root)\OEBPS\content.opf
 * @param	href	: ops 파일의 상대 경로
 */
void CBookInfo::SetOPSFilePath(CString href) 
{ 
	m_opsFilePath = href; 
};

/**
 * @brief	toc 파일의 상대 경로를 설정한다.
 * @remarks	$(root)\$(packageName)\toc.xhtml or $(root)\$(packageName)\toc.ncx
 *			ex) $(root)\OEBPS\toc.xhtml
 * @param	href	: toc 파일의 상대 경로
 */
void CBookInfo::SetTocFilePath(CString href) 
{ 
	m_tocFilePath = href; 
};

/**
 * @brief	epub 이 unzip 된 절대 경로 설정
 * @param	root	: epub 이 unzip 된 절대 경로
 */
void CBookInfo::SetUnzipRootPath(CString root) 
{ 
	m_unzipRootPath = root; 
};

/**
 * @brief	fixed 컨텐츠의 가로 길이 설정
 * @param	width	: fixed 컨텐츠의 가로 길이
 */
void CBookInfo::SetFixedWidth(int width) 
{ 
	m_fixedWidth = width; 
};

/**
 * @brief	fixed 컨텐츠의 세로 길이 설정
 * @param	height	: fixed 컨텐츠의 세로 길이
 */
void CBookInfo::SetFixedHeight(int height) 
{ 
	m_fixedHeight = height; 
};

#if SUPPORT_THUMBNAIL_PATH
	// thumbnail path 를 엔진에서 제공하는 경우, unzip 된 epub 경로 안에 thumbnail 폴더를 만들어 해당 경로를 반환한다.
	// 그러나 엔진에서 섬네일 경로를 지정하여 제공할 필요가 없으므로 해당 함수는 미지원하도록 처리함.
/**
 * @brief	썸네일 경로 설정
 * @param	path	: 섬네일 경로
 */
void CBookInfo::SetThumbnailPath(CString path) 
{ 
	m_epubThumnailPath = path; 
};
#endif /*SUPPROT_THUMBNAIL_PATH*/

/**
 * @brief	toc 타입을 반환한다.
 * @remars	3.0 spec = nav, 2.0 spec = ncx
 * @return	toc 타입
 */
const long	CBookInfo::GetNavType() const 
{ 
	return m_nNavType; 
};

/**
 * @brief	fixed layout 옵션을 반환한다.
 * @remarks	samsung spec 에 따르면 fixed 레이아웃은 PORTRAIT(세로), LANDSCAPE(가로), BOTH(세로/가로 모두 지원) 형식으로 다뉜다.
 * @return	fixed layout 옵션
 */
const int CBookInfo::GetFixedOption() const 
{ 
	return m_fixedOption; 
};

/**
 * @brief	package Folder 이름을 반환한다.
 * @remarks	ex) OEBPS
 * @return	package Folder 이름
 */
const CString CBookInfo::GetPackageName() const 
{ 
	return m_packageName; 
};

/**
 * @brief	ops 파일의 상대 경로를 반환한다.
 * @remarks	$(root)\$(packageName)\content.opf
 *			ex) $(root)\OEBPS\content.opf
 * @return	ops 파일의 상대 경로
 */
const CString CBookInfo::GetOPSFilePath() const 
{ 
	return m_opsFilePath; 
};

/**
 * @brief	toc 파일의 상대 경로를 반환한다.
 * @remarks	$(root)\$(packageName)\toc.xhtml or $(root)\$(packageName)\toc.ncx
 *			ex) $(root)\OEBPS\toc.xhtml
 * @return	toc 파일의 상대 경로
 */
const CString CBookInfo::GetTocFilePath() const 
{ 
	return m_tocFilePath; 
};

/**
 * @brief	epub 이 unzip 된 절대 경로 반환
 * @return	epub 이 unzip 된 절대 경로
 */
const CString CBookInfo::GetUnzipRootPath() const 
{ 
	return m_unzipRootPath; 
};

/**
 * @brief	fixed 컨텐츠의 가로 길이 반환
 * @return	fixed 컨텐츠의 가로 길이
 */
const int CBookInfo::GetFixedWidth() const 
{ 
	return m_fixedWidth; 
};

/**
 * @brief	fixed 컨텐츠의 세로 길이 반환
 * @return	fixed 컨텐츠의 세로 길이
 */
const int CBookInfo::GetFixedHeight() const 
{ 
	return m_fixedHeight; 
};

#if SUPPORT_THUMBNAIL_PATH
	// thumbnail path 를 엔진에서 제공하는 경우, unzip 된 epub 경로 안에 thumbnail 폴더를 만들어 해당 경로를 반환한다.
	// 그러나 엔진에서 섬네일 경로를 지정하여 제공할 필요가 없으므로 해당 함수는 미지원하도록 처리함.
/**
 * @brief	썸네일 경로 반환
 * @return 섬네일 경로
 */
const CString CBookInfo::GetThumbnailPath() const 
{ 
	return m_epubThumnailPath; 
};
#endif /*SUPPROT_THUMBNAIL_PATH*/


CBookInfo& CBookInfo::operator=(const IN3BookInfo &other)
{
	(IN3BookInfo)(*this) = other;

	return *this;
}

CBookInfo& CBookInfo::operator=(const IN3BookInfoEx &other)
{
	(IN3BookInfoEx)(*this) = other;

	return *this;
}

CBookInfo& CBookInfo::operator=(const CBookInfo &other)
{
	(IN3BookInfoEx)(*this) = (IN3BookInfoEx)other;

	m_nNavType = other.GetNavType();
	m_fixedOption = other.GetFixedOption();
	m_packageName = other.GetPackageName();
	m_opsFilePath = other.GetOPSFilePath();
	m_tocFilePath = other.GetTocFilePath();
	m_unzipRootPath = other.GetUnzipRootPath();
	m_fixedWidth = other.GetFixedWidth();
	m_fixedHeight = other.GetFixedHeight();

	return *this;
}
