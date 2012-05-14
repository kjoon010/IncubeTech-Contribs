#include "StdAfx.h"
#include "IN3Setting.h"

/**
 * @brief	생성자
 */
IN3Setting::IN3Setting(void)
	:m_nPageMode(IN3_SIGNE_PAGE_MODE)
	,m_bDisplayPageNumber(false)
	,m_nFontSize(0)
	,m_margin(40)
	,m_lineHeight(1.6)
	,m_strFamily(_T(""))
	,m_strFontColor(_T(""))
	,m_strBgColor(_T(""))
{
}

/**
 * @brief	소멸자
 */
IN3Setting::~IN3Setting(void)
{
	annotations.clear();
	bookmarks.clear();
}

/**
 * @brief	본문 아래영역에 현재 페이지 값을 표시할지 여부를 설정한다.
 * @param	bDisplay	: true(본문 밑에 페이지 번호 표시) / false(페이지 번호 표시 안함)
 */
void IN3Setting::SetDisplayPageNumber(bool bDisplay)
{
	m_bDisplayPageNumber = bDisplay;
}

/**
 * @brief	페이지 모드를 설정한다. (단면/양면)
 * @param	mode	: 1 = 단면, 2 = 양면
 */
void IN3Setting::SetPageDisplayMode(int mode)
{
	m_nPageMode = mode;
}

/**
 * @brief	폰트 사이즈를 설정한다.
 * @param	size	: 변경할 폰트 사이즈
 */
void IN3Setting::SetFontSize(int size)
{
	m_nFontSize = size;
}

/**
 * @brief	COLUMN GAP을 설정한다.
 * @param	margin	: 변경할 COLUMN GAP
 */
void IN3Setting::SetMargin(int margin)
{
	m_margin = margin;
}

/**
 * @brief	line height을 설정한다.
 * @param	height	: 변경할 line height
 */
void IN3Setting::SetLineHeight(double height)
{
	m_lineHeight = height;
}

/**
 * @brief	폰트를 설정한다.
 * @param	family	: 변경할 폰트
 */
void IN3Setting::SetFontFamily(CString family)
{
	m_strFamily = family;
}

/**
 * @brief	폰트 색상을 설정한다.
 * @param	color	: 변경할 폰트 색상
 */
void IN3Setting::SetFontColor(CString color)
{
	m_strFontColor = color;
}

/**
 * @brief	배경색을 설정한다.
 * @param	color	: 변경할 배경색
 */
void IN3Setting::SetBgColor(CString color)
{
	m_strBgColor = color;
}

/**
 * @brief	본문 아래 영역에 페이지 번호를 표시하도록 설정되어 있는지 확인한다.
 * @return	true(본문 밑에 페이지 번호 표시) / false(페이지 번호 표시 안함)
 */
const bool IN3Setting::IsDisplayPageNumber() const
{
	return m_bDisplayPageNumber;
}

/**
 * @brief	현재 페이지 모드를 반환한다.
 * @return	페이지 모드 (1=단면, 2=양면)
 */
const int IN3Setting::GetPageDisplayMode() const
{
	return m_nPageMode;
}

/**
 * @brief	사용자가 설정한 폰트 사이즈를 반환한다.
 * @return	사용자가 설정한 폰트 사이즈
 */
const int IN3Setting::GetFontSize() const
{
	return m_nFontSize;
}

/**
 * @brief	사용자가 설정한 column gap 을 반환한다.
 * @return	사용자가 설정한 column gap
 */
const int IN3Setting::GetMargin() const
{
	return m_margin;
}

/**
 * @brief	사용자가 설정한 line height 을 반환한다.
 * @return	사용자가 설정한 line height
 */
const double IN3Setting::GetLineHeight() const
{
	return m_lineHeight;
}

/**
 * @brief	사용자가 설정한 폰트명 을 반환한다.
 * @return	사용자가 설정한 폰트명
 */
const CString IN3Setting::GetFontFamily() const
{
	return m_strFamily;
}

/**
 * @brief	사용자가 설정한 폰트색을 반환한다.
 * @return	사용자가 설정한 폰트색
 */
const CString IN3Setting::GetFontColor() const
{
	return m_strFontColor;
}

/**
 * @brief	사용자가 설정한 배경색을 반환한다.
 * @return	사용자가 설정한 배경색
 */
const CString IN3Setting::GetBgColor() const
{
	return m_strBgColor;
}

/**
 * @brief	annotation 을 파일변로 분류
 * @param	spineIndex	: annotation 이 있는 spine 인덱스
 * @param	pAnnotation : annotation 정보
 */ 
void IN3Setting::AddAnnotation(int spineIndex, IN3Annotation *pAnnotation)
{
	annotations.insert(std::pair<int, IN3Annotation *>(spineIndex, pAnnotation));
}

/**
 * @brief	북마크를 파일변로 분류
 * @param	spineIndex	: 북마크가 있는 spine 인덱스
 * @param	pBookmark : 북마크 정보
 */ 
void IN3Setting::AddBookmark(int spineIndex, IN3Bookmark *pBookmark)
{
	bookmarks.insert(std::pair<int, IN3Bookmark *>(spineIndex, pBookmark));
}

/**
 * @brief	annotation 일괄적용을 위해 annotation 리스트의 annotation 들을 스트링으로 변환한다.
 * @param	[in] spineIndex	: spine 파일의 index
 * @param	[out] param		: 엔진 extension 에 일괄 적용을 요청하기 위한 데이터
 */
void IN3Setting::GetInitAnnotationParam(int spineIndex, CString &param)
{
	param = _T("");
	//
	if(spineIndex < 0){
		return;
	}

	///*
	std::pair<std::multimap<int, IN3Annotation *>::iterator, std::multimap<int, IN3Annotation *>::iterator> iter_pair;
	iter_pair = annotations.equal_range(spineIndex);
	std::multimap<int, IN3Annotation *>::iterator iter;
	for( iter = iter_pair.first; iter != iter_pair.second; iter ++)
	{
		if(iter != iter_pair.first)
		{
			param.Append(_T(", "));
		}
		IN3Annotation *pAnnotation = iter->second;
		param.Append(pAnnotation->GetJSONString());
	}
	//*/
}

/**
 * @brief	북마크 일괄적용을 위해 북마크 리스트의 북마크 들을 스트링으로 변환한다.
 * @param	[in] spineIndex	: spine 파일의 index
 * @param	[out] param		: 엔진 extension 에 일괄 적용을 요청하기 위한 데이터
 */
void IN3Setting::GetInitBookmarkParam(int spineIndex, CString &param)
{
	param = _T("");
	//
	if(spineIndex < 0){
		return;
	}	
	
	std::pair<std::multimap<int, IN3Bookmark *>::iterator, std::multimap<int, IN3Bookmark *>::iterator> iter_pair;
	iter_pair = bookmarks.equal_range(spineIndex);
	std::multimap<int, IN3Bookmark *>::iterator iter;
	for( iter = iter_pair.first; iter != iter_pair.second; iter ++)
	{
		if(iter != iter_pair.first)
		{
			param.Append(_T(", "));
		}
		IN3Bookmark *pBookmark = iter->second;
		param.Append(pBookmark->GetJSONString());
	}
}

/**
 * @brief	북마크 삭제를 위해 파일별로 분류된 북마크 정보를 반환한다.
 * @param	[in] spineIndex	: spine 파일의 index
 * @param	[out] param		: 엔진 extension 에 북마크 삭제를 요청하기 위한 데이터
 */
void IN3Setting::GetDeleteBookmarkParam(int spineIndex, CString &param)
{
	param = _T("");
	//
	if(spineIndex < 0){
		return;
	}	
	
	std::pair<std::multimap<int, IN3Bookmark *>::iterator, std::multimap<int, IN3Bookmark *>::iterator> iter_pair;
	iter_pair = bookmarks.equal_range(spineIndex);
	std::multimap<int, IN3Bookmark *>::iterator iter;
	for( iter = iter_pair.first; iter != iter_pair.second; iter ++)
	{
		if(iter != iter_pair.first)
		{
			param.Append(_T(", "));
		}

		IN3Bookmark *pBookmark = iter->second;
		param.Append(_T("\""));
		param.Append(pBookmark->GetMarkerID());
		param.Append(_T("\""));
	}
}

/**
 * @brief	annotation 리스트 삭제
 */
void IN3Setting::ClearAnnotations()
{
	annotations.clear();
}

/**
 * @brief	북마크 리스트 삭제
 */
void IN3Setting::ClearBookmarks()
{
	bookmarks.clear();
}