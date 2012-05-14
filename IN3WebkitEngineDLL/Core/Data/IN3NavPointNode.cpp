#include "StdAfx.h"
#include "IN3NavPointNode.h"

IN3NavPointNode::IN3NavPointNode(void)
	:m_title(_T(""))
	,m_url(_T(""))
	,m_page(0)
{
}

IN3NavPointNode::IN3NavPointNode(LPCTSTR title, LPCTSTR url)
	:m_title(title)
	,m_url(url)
	,m_page(0)
{
}
	
IN3NavPointNode::IN3NavPointNode(LPCTSTR title, LPCTSTR url, UINT page)
	:m_title(title)
	,m_url(url)
	,m_page(page)
{
}

IN3NavPointNode::~IN3NavPointNode(void)
{
}

void IN3NavPointNode::SetTitle(CString title) 
{ 
	m_title = title; 
}

void IN3NavPointNode::SetURL(CString url) 
{ 
	m_url = url; 
}

void IN3NavPointNode::SetPageNumber(UINT page)
{
	m_page = page;
}

long IN3NavPointNode::GetChildCount() const 
{ 
	return (long)m_ChildNavPoints.size(); 
}

const IN3NavPointNode& IN3NavPointNode::GetChild(long i) const 
{ 
	return m_ChildNavPoints[i]; 
}

IN3NavPointNode& IN3NavPointNode::GetChild(long i)       
{ 
	return m_ChildNavPoints[i]; 
}

const CString& IN3NavPointNode::GetTitle() const
{
	return m_title;
}

const CString& IN3NavPointNode::GetURL() const
{
	return m_url;
}

const UINT IN3NavPointNode::GetPageNumber() const
{
	return m_page;
}

void IN3NavPointNode::Append(const IN3NavPointNode &navPointNode)
{
	m_ChildNavPoints.push_back(navPointNode);
}

void IN3NavPointNode::Release()
{
	m_ChildNavPoints.clear();
}
