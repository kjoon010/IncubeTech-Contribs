#include "StdAfx.h"
#include "IN3Setting.h"

/**
 * @brief	������
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
 * @brief	�Ҹ���
 */
IN3Setting::~IN3Setting(void)
{
	annotations.clear();
	bookmarks.clear();
}

/**
 * @brief	���� �Ʒ������� ���� ������ ���� ǥ������ ���θ� �����Ѵ�.
 * @param	bDisplay	: true(���� �ؿ� ������ ��ȣ ǥ��) / false(������ ��ȣ ǥ�� ����)
 */
void IN3Setting::SetDisplayPageNumber(bool bDisplay)
{
	m_bDisplayPageNumber = bDisplay;
}

/**
 * @brief	������ ��带 �����Ѵ�. (�ܸ�/���)
 * @param	mode	: 1 = �ܸ�, 2 = ���
 */
void IN3Setting::SetPageDisplayMode(int mode)
{
	m_nPageMode = mode;
}

/**
 * @brief	��Ʈ ����� �����Ѵ�.
 * @param	size	: ������ ��Ʈ ������
 */
void IN3Setting::SetFontSize(int size)
{
	m_nFontSize = size;
}

/**
 * @brief	COLUMN GAP�� �����Ѵ�.
 * @param	margin	: ������ COLUMN GAP
 */
void IN3Setting::SetMargin(int margin)
{
	m_margin = margin;
}

/**
 * @brief	line height�� �����Ѵ�.
 * @param	height	: ������ line height
 */
void IN3Setting::SetLineHeight(double height)
{
	m_lineHeight = height;
}

/**
 * @brief	��Ʈ�� �����Ѵ�.
 * @param	family	: ������ ��Ʈ
 */
void IN3Setting::SetFontFamily(CString family)
{
	m_strFamily = family;
}

/**
 * @brief	��Ʈ ������ �����Ѵ�.
 * @param	color	: ������ ��Ʈ ����
 */
void IN3Setting::SetFontColor(CString color)
{
	m_strFontColor = color;
}

/**
 * @brief	������ �����Ѵ�.
 * @param	color	: ������ ����
 */
void IN3Setting::SetBgColor(CString color)
{
	m_strBgColor = color;
}

/**
 * @brief	���� �Ʒ� ������ ������ ��ȣ�� ǥ���ϵ��� �����Ǿ� �ִ��� Ȯ���Ѵ�.
 * @return	true(���� �ؿ� ������ ��ȣ ǥ��) / false(������ ��ȣ ǥ�� ����)
 */
const bool IN3Setting::IsDisplayPageNumber() const
{
	return m_bDisplayPageNumber;
}

/**
 * @brief	���� ������ ��带 ��ȯ�Ѵ�.
 * @return	������ ��� (1=�ܸ�, 2=���)
 */
const int IN3Setting::GetPageDisplayMode() const
{
	return m_nPageMode;
}

/**
 * @brief	����ڰ� ������ ��Ʈ ����� ��ȯ�Ѵ�.
 * @return	����ڰ� ������ ��Ʈ ������
 */
const int IN3Setting::GetFontSize() const
{
	return m_nFontSize;
}

/**
 * @brief	����ڰ� ������ column gap �� ��ȯ�Ѵ�.
 * @return	����ڰ� ������ column gap
 */
const int IN3Setting::GetMargin() const
{
	return m_margin;
}

/**
 * @brief	����ڰ� ������ line height �� ��ȯ�Ѵ�.
 * @return	����ڰ� ������ line height
 */
const double IN3Setting::GetLineHeight() const
{
	return m_lineHeight;
}

/**
 * @brief	����ڰ� ������ ��Ʈ�� �� ��ȯ�Ѵ�.
 * @return	����ڰ� ������ ��Ʈ��
 */
const CString IN3Setting::GetFontFamily() const
{
	return m_strFamily;
}

/**
 * @brief	����ڰ� ������ ��Ʈ���� ��ȯ�Ѵ�.
 * @return	����ڰ� ������ ��Ʈ��
 */
const CString IN3Setting::GetFontColor() const
{
	return m_strFontColor;
}

/**
 * @brief	����ڰ� ������ ������ ��ȯ�Ѵ�.
 * @return	����ڰ� ������ ����
 */
const CString IN3Setting::GetBgColor() const
{
	return m_strBgColor;
}

/**
 * @brief	annotation �� ���Ϻ��� �з�
 * @param	spineIndex	: annotation �� �ִ� spine �ε���
 * @param	pAnnotation : annotation ����
 */ 
void IN3Setting::AddAnnotation(int spineIndex, IN3Annotation *pAnnotation)
{
	annotations.insert(std::pair<int, IN3Annotation *>(spineIndex, pAnnotation));
}

/**
 * @brief	�ϸ�ũ�� ���Ϻ��� �з�
 * @param	spineIndex	: �ϸ�ũ�� �ִ� spine �ε���
 * @param	pBookmark : �ϸ�ũ ����
 */ 
void IN3Setting::AddBookmark(int spineIndex, IN3Bookmark *pBookmark)
{
	bookmarks.insert(std::pair<int, IN3Bookmark *>(spineIndex, pBookmark));
}

/**
 * @brief	annotation �ϰ������� ���� annotation ����Ʈ�� annotation ���� ��Ʈ������ ��ȯ�Ѵ�.
 * @param	[in] spineIndex	: spine ������ index
 * @param	[out] param		: ���� extension �� �ϰ� ������ ��û�ϱ� ���� ������
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
 * @brief	�ϸ�ũ �ϰ������� ���� �ϸ�ũ ����Ʈ�� �ϸ�ũ ���� ��Ʈ������ ��ȯ�Ѵ�.
 * @param	[in] spineIndex	: spine ������ index
 * @param	[out] param		: ���� extension �� �ϰ� ������ ��û�ϱ� ���� ������
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
 * @brief	�ϸ�ũ ������ ���� ���Ϻ��� �з��� �ϸ�ũ ������ ��ȯ�Ѵ�.
 * @param	[in] spineIndex	: spine ������ index
 * @param	[out] param		: ���� extension �� �ϸ�ũ ������ ��û�ϱ� ���� ������
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
 * @brief	annotation ����Ʈ ����
 */
void IN3Setting::ClearAnnotations()
{
	annotations.clear();
}

/**
 * @brief	�ϸ�ũ ����Ʈ ����
 */
void IN3Setting::ClearBookmarks()
{
	bookmarks.clear();
}