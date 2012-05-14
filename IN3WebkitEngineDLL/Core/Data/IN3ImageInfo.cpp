#include "StdAfx.h"
#include "IN3ImageInfo.h"

CIN3ImageInfo::CIN3ImageInfo(void)
	: m_pageNum(0)
	,m_filePath(_T(""))
	,m_ImagePath(_T(""))
	,m_caption(_T(""))
{
}

CIN3ImageInfo::CIN3ImageInfo(CString filePath, CString imgId, CString imgPath, CString caption, int page /*= 0*/)
	: m_pageNum(page)
	,m_filePath(filePath)
	,m_imgID(imgId)
	,m_ImagePath(imgPath)
	,m_caption(caption)
{
}

CIN3ImageInfo::~CIN3ImageInfo(void)
{
}

const CString CIN3ImageInfo::GetImageID()
{
	return m_imgID;
}

const CString CIN3ImageInfo::GetFilePath()
{
	return m_filePath;
}

const CString CIN3ImageInfo::GetImagePath()
{
	return m_ImagePath;
}

const CString CIN3ImageInfo::GetCaption()
{
	return m_caption;
}

const UINT CIN3ImageInfo::GetPageNumber()
{
	return m_pageNum;
}

void CIN3ImageInfo::SetImgID(const CString strId)
{
	m_imgID = strId;
}

void CIN3ImageInfo::SetFilePath( const CString strFilePath )
{
	m_filePath = strFilePath;
	m_filePath.Replace(_T("\\/"), _T("/"));
}

void CIN3ImageInfo::SetImagePath(const CString strImagePath)
{
	m_ImagePath = strImagePath;
}

void CIN3ImageInfo::SetCaption( const CString strCaption )
{
	m_caption = strCaption;
}

void CIN3ImageInfo::SetPageNumber( const UINT pageNum )
{
	m_pageNum = pageNum;
}
