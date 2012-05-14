#include "StdAfx.h"
#include "IN3TableInfo.h"

CIN3TableInfo::CIN3TableInfo(void)
	:m_page(0)
	,m_fileName(_T(""))
	,m_caption(_T(""))
{
}

CIN3TableInfo::CIN3TableInfo(CString fileName, CString tableId, CString caption, int page /*= 0*/)	
	:m_page(page)
	,m_fileName(fileName)
	,m_tableID(tableId)
	,m_caption(caption)
{
}

CIN3TableInfo::~CIN3TableInfo(void)
{
}

void CIN3TableInfo::SetTableID(CString strID)
{
	m_tableID = strID;
}

void CIN3TableInfo::SetFileName(CString fileName)
{
	m_fileName = fileName;
	m_fileName.Replace(_T("\\/"), _T("/"));
}

void CIN3TableInfo::SetCaption(CString caption)
{
	m_caption = caption;
}

void CIN3TableInfo::SetPage(int page)
{
	m_page = page;
}

const CString CIN3TableInfo::GetTableID() const
{
	return m_tableID;
}

const CString CIN3TableInfo::GetFileName() const
{
	return m_fileName;
}

const CString CIN3TableInfo::GetCaption() const
{
	return m_caption;
}

const int CIN3TableInfo::GetPage() const
{
	return m_page;
}
