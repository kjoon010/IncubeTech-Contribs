#include "StdAfx.h"
#include "IN3SelectionData.h"

IN3SelectionData::IN3SelectionData(void)
{
}
	
IN3SelectionData::IN3SelectionData(POINT pt, CString selectedText)
{
	m_pt = pt;
	m_selectedText = selectedText;
}

IN3SelectionData::~IN3SelectionData(void)
{
}

void IN3SelectionData::SetSelectPoint(POINT pt)
{
	m_pt = pt;
}

void IN3SelectionData::SetSelectedText(CString selected)
{
	m_selectedText = selected;
}

const POINT IN3SelectionData::GetSelectPoint() const
{
	return m_pt;
}

const CString IN3SelectionData::GetSelectedText() const
{
	return m_selectedText;
}
