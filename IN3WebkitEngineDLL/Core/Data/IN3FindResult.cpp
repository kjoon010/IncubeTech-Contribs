#include "StdAfx.h"
#include "IN3FindResult.h"

IN3FindResult::IN3FindResult(void)
{
}

IN3FindResult::IN3FindResult(int seq, int page, int startIdx, int endIdx)
	:m_sentence(_T(""))
	,m_fileName(_T(""))
{
	m_seq = seq;
	m_page = page;
	m_startIdx = startIdx;
	m_endIdx = endIdx;
}

IN3FindResult::IN3FindResult(int seq, int page, int startIdx, int endIdx, CString sentence)
	:m_fileName(_T(""))
{
	m_seq = seq;
	m_page = page;
	m_startIdx = startIdx;
	m_endIdx = endIdx;
	m_sentence = sentence;
}

IN3FindResult::IN3FindResult(int seq, int page, int startIdx, int endIdx, CString sentence, CString fileName)
{
	m_seq = seq;
	m_page = page;
	m_startIdx = startIdx;
	m_endIdx = endIdx;
	m_sentence = sentence;
	m_fileName = fileName;
}

IN3FindResult::~IN3FindResult(void)
{
}

void IN3FindResult::SetSeq(int seq)
{
	m_seq = seq;
}

void IN3FindResult::SetPage(int page)
{
	m_page = page;
}

void IN3FindResult::SetStartIndex(int index)
{
	m_startIdx = index;
}

void IN3FindResult::SetEndIndex(int index)
{
	m_endIdx = index;
}

void IN3FindResult::SetSentence(CString sentence)
{
	m_sentence = sentence;
}

void IN3FindResult::SetFileName(CString fileName)
{
	m_fileName = fileName;
}

const int IN3FindResult::GetSeq() const
{
	return m_seq;
}

const int IN3FindResult::GetPage() const
{
	return m_page;
}

const int IN3FindResult::GetStartIndex() const
{
	return m_startIdx;
}

const int IN3FindResult::GetEndIndex() const
{
	return m_endIdx;
}

const CString IN3FindResult::GetSentence() const
{
	return m_sentence;
}

const CString IN3FindResult::GetFileName() const
{
	return m_fileName;
}