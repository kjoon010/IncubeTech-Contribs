#include "StdAfx.h"
#include "IN3Annotation.h"
#include "IN3WebkitDefine.h"


/*****************************************************************************************
 * IN3Annotation
 *****************************************************************************************/

// 持失切
IN3Annotation::IN3Annotation(void)
	:m_bMemo(false)
	,m_nStart(0)
	,m_nEnd(0)
	,m_nPage(0)
	,m_markerID(_T(""))
	,m_fileName(_T(""))
	,m_selection(_T(""))
	,m_bgColor(_T(""))
	,m_ulColor(_T(""))
{
}

IN3Annotation::IN3Annotation(bool bMemo, int start, int end, CString markerID, CString fileName, CString selection, int page /*=0*/)
	:m_bMemo(bMemo)
	,m_nStart(start)
	,m_nEnd(end)
	,m_markerID(markerID)
	,m_fileName(fileName)
	,m_selection(selection)
	,m_bgColor(_T(""))
	,m_ulColor(_T(""))
	,m_nPage(page)
{
}

IN3Annotation::IN3Annotation(bool bMemo, int start, int end, CString markerID, CString fileName, CString selection,
		CString bgColor, CString ulColor, int page /*=0*/)
	:m_bMemo(bMemo)
	,m_nStart(start)
	,m_nEnd(end)
	,m_markerID(markerID)
	,m_fileName(fileName)
	,m_selection(selection)
	,m_bgColor(bgColor)
	,m_ulColor(ulColor)
	,m_nPage(page)
{
}

// 社瑚切
IN3Annotation::~IN3Annotation(void)
{
}

void IN3Annotation::Init(bool bMemo, int start, int end, CString markerID, CString fileName, CString selection,
		CString bgColor, CString ulColor, int page /*=0*/)
{
	m_bMemo = bMemo;
	m_nStart = start;
	m_nEnd = end;
	m_markerID = markerID;
	m_fileName = fileName;
	m_selection = selection;
	m_bgColor = bgColor;
	m_ulColor = ulColor;
	m_nPage = page;
}

void IN3Annotation::SetMemo(bool bMemo)
{
	m_bMemo = bMemo;
}

void IN3Annotation::SetIndex(int start, int end)
{
	m_nStart = start;
	m_nEnd = end;
}

void IN3Annotation::SetStart(int start)
{
	m_nStart = start;
}

void IN3Annotation::SetEnd(int end)
{
	m_nEnd = end;
}

void IN3Annotation::SetMarkerID(CString id)
{
	m_markerID = id;
}

void IN3Annotation::SetFileName(CString fileName)
{
	m_fileName = fileName;
	m_fileName.Replace(_T("\\/"), _T("/"));
}

void IN3Annotation::SetSelection(CString str)
{
	m_selection = str;
}

void IN3Annotation::SetBGColor(CString color)
{
	if(!color.IsEmpty())
		m_ulColor.Empty();

	m_bgColor = color;
}

void IN3Annotation::SetULColor(CString color)
{
	if(!color.IsEmpty())
		m_bgColor.Empty();

	m_ulColor = color;
}

void IN3Annotation::SetAnnotationRect(int left, int top, int right, int bottom)
{
	m_rect.left = left;
	m_rect.top = top;
	m_rect.right = right;
	m_rect.bottom = bottom;
}

void IN3Annotation::SetPage(int page)
{
	m_nPage = page;
}

const bool IN3Annotation::HasMemo() const
{
	return m_bMemo;
}

const int IN3Annotation::GetStart() const
{
	return m_nStart;
}

const int IN3Annotation::GetEnd() const
{
	return m_nEnd;
}

const CString IN3Annotation::GetMarkerID() const
{
	return m_markerID;
}

const CString IN3Annotation::GetFileName() const
{
	return m_fileName;
}

const CString IN3Annotation::GetSelection() const
{
	return m_selection;
}

const CString IN3Annotation::GetBGColor() const
{
	return m_bgColor;
}

const CString IN3Annotation::GetULColor() const
{
	return m_ulColor;
}

const int IN3Annotation::GetPage() const
{
	return m_nPage;
}

const CRect IN3Annotation::GetAnnotationRect() const
{
	return m_rect;
}

CString	IN3Annotation::GetJSONString()
{
	if(m_nStart== 0 && m_nEnd == 0){
		TRACEMSG_ERR(_T("invalidate annotation's data. no annotation index information"));
		return CString(_T(""));
	}

	if(m_markerID.IsEmpty()){
		TRACEMSG_ERR(_T("invalidate annotation's data. no marker id information"));
		return CString(_T(""));
	}

	if(m_fileName.IsEmpty()){
		TRACEMSG_ERR(_T("invalidate annotation's data. no fileName information"));
		return CString(_T(""));
	}

	CString str;
	str.Format(_T("{\"MARKER_ID\":\"%s\", \"START_INDEX\":%d, \"END_INDEX\":%d, \"BG_COLOR\":\"%s\", \"HAS_MEMO\":%d}"), 
		m_markerID, m_nStart, m_nEnd, m_bgColor, m_bMemo);
	return str;
}




/*****************************************************************************************
 * IN3Bookmark
 *****************************************************************************************/
// 持失切
IN3Bookmark::IN3Bookmark(void)
	:m_makerIndex(0)
	,m_nPage(0)
	,m_fileName(_T(""))
	,m_markerID(_T(""))
{
}

IN3Bookmark::IN3Bookmark(int type, CString fileName, CString markerID, int index, int page /*=0*/)
	:m_makerIndex(index)
	,m_nPage(page)
	,m_fileName(fileName)
	,m_markerID(markerID)
{
}

// 社瑚切
IN3Bookmark::~IN3Bookmark(void)
{
}

// setter
void IN3Bookmark::SetMarkerIndex(int index)
{
	m_makerIndex = index;
}

void IN3Bookmark::SetFileName(CString fileName)
{
	m_fileName = fileName;
	m_fileName.Replace(_T("\\/"), _T("/"));
}

void IN3Bookmark::SetMarkerID(CString id)
{
	m_markerID = id;
}

void IN3Bookmark::SetPage(int page)
{
	m_nPage = page;
}

// getter
const int IN3Bookmark::GetMarkerIndex() const
{
	return m_makerIndex;
}

const CString IN3Bookmark::GetFileName() const
{
	return m_fileName;
}

const CString IN3Bookmark::GetMarkerID() const
{
	return m_markerID;
}

const int IN3Bookmark::GetPage() const
{
	return m_nPage;
}

CString IN3Bookmark::GetJSONString()
{
	if(m_makerIndex == 0){
		TRACEMSG_ERR(_T("invalidate bookmark's data. no annotation index information"));
		return CString(_T(""));
	}

	if(m_markerID.IsEmpty()){
		TRACEMSG_ERR(_T("invalidate bookmark's data. no marker id information"));
		return CString(_T(""));
	}

	CString str;
	str.Format(_T("{\"MARKER_ID\":\"%s\", \"MARKER_INDEX\":%d}"), m_markerID, m_makerIndex);
	return str;
}