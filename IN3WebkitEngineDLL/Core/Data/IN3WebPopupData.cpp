#include "StdAfx.h"
#include "IN3WebPopupData.h"

IN3WebPopupData::IN3WebPopupData(void)
	:m_url(_T(""))
	,m_width(0)
	,m_height(0)
{
}

IN3WebPopupData::IN3WebPopupData(CString url, int width, int height)
{
	m_url = url;
	m_width = width;
	m_height = height;
}	

IN3WebPopupData::~IN3WebPopupData(void)
{
}

void IN3WebPopupData::SetURL(CString url)
{
	m_url = url;
}

void IN3WebPopupData::SetPopupSize(int width, int height)
{
	m_width = width;
	m_height = height;
}

void IN3WebPopupData::SetPopupWidth(int width)
{
	m_width = width;
}

void IN3WebPopupData::SetPopupHeight(int height)
{
	m_height = height;
}

const CString IN3WebPopupData::GetURL() const
{
	return m_url;
}

const int IN3WebPopupData::GetWidth() const
{
	return m_width;
}

const int IN3WebPopupData::GetHeight() const
{
	return m_height;
}