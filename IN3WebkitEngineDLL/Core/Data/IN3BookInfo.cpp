#include "StdAfx.h"
#include "IN3BookInfo.h"

/*****************************************************************************************
 * IN3BookInfo
 *****************************************************************************************/
/**
 * @brief	생성자
 */
IN3BookInfo::IN3BookInfo(void)
	:m_version(3.0)
	,m_bFixed(false)
	,m_srcRootPath(_T(""))
	,m_UUIDString(_T(""))
	,m_title(_T(""))
	,m_creator(_T(""))
	,m_subject(_T(""))
	,m_publisher(_T(""))
	,m_date(_T(""))
	,m_desc(_T(""))
	,m_coverHref(_T(""))
{
}

/**
 * @brief	소멸자
 */
IN3BookInfo::~IN3BookInfo(void)
{
}

/**
 * @brief	책의 버전 정보를 설정한다.	
 * @param	ver	: epub 버전
 */
void IN3BookInfo::SetVersion(float ver) 
{ 
	m_version = ver; 
};

/**
 * @brief	fixed 레이웃 정보를 설정한다.	
 * @param	bFixed	: fixed 컨텐츠 인지 (true: fixed, false: reflow)
 */
void IN3BookInfo::SetFixed(bool bFixed) 
{ 
	m_bFixed = bFixed; 
};

/** 
 * @brief	책의 원본 절대 경로를 설정
 * @param	root	: 책의 원본 절대 경로
 */
void IN3BookInfo::SetSrcRootPath(CString root) 
{ 
	m_srcRootPath = root; 
};

/**
 * @brief	책의 식별자(UUID) 를 설정
 * @param	uuid	: 책의 식별자
 */
void IN3BookInfo::SetUUIDString(CString uuid) 
{ 
	m_UUIDString = uuid; 
};

/**
 * @brief	책의 타이틀을 설정
 * @param	title	: 책 제목
 */
void IN3BookInfo::SetTitle(CString title) 
{ 
	m_title = title; 
};

/**
 * @brief	책의 저자를 설정
 * @param	creator	: 저자명
 */
void IN3BookInfo::SetCreator(CString creator) 
{ 
	m_creator = creator; 
};

/**
 * @brief	책의 주제를 설정한다.
 * @param	subject	: 주제
 */
void IN3BookInfo::SetSubject(CString subject)
{
	m_subject = subject;
}

/**
 * @brief	책의 출판사명을 설정한다.
 * @param	publisher	: 출판사명
 */
void IN3BookInfo::SetPublisher(CString publisher) 
{ 
	m_publisher = publisher; 
};

/**
 * @brief	출판일을 설정한다.
 * @param	date	: 출판일
 */
void IN3BookInfo::SetDate(CString date)
{
	m_date = date;
}

/**
 * @brief	책의 설명을 설정한다.
 * @param	desc	: 책의 설명
 */
void IN3BookInfo::SetDesc(CString desc) 
{ 
	m_desc = desc; 
};

/**
 * @brief	책의 커버 이미지 경로를 설정한다.
 * @param	href	: 책의 커버 이미지
 */
void IN3BookInfo::SetCoverHref(CString href) 
{ 
	m_coverHref = href; 
};

/**
 * @brief	책의 버전 정보를 반환한다.	
 * @return	epub 버전
 */
const float	IN3BookInfo::GetVersion() const 
{ 
	return m_version; 
};

/**
 * @brief	fixed 레이웃 정보를 반환한다.	
 * return	fixed 컨텐츠 인지 (true: fixed, false: reflow)
 */
const BOOL	IN3BookInfo::IsFixed() const 
{ 
	return m_bFixed; 
};

/** 
 * @brief	책의 원본 절대 경로를 반환
 * @reutrn	책의 원본 절대 경로
 */
const CString IN3BookInfo::GetSrcRootPath() const 
{ 
	return m_srcRootPath; 
};

/**
 * @brief	책의 식별자(UUID) 를 반환
 * @return	책의 식별자
 */
const CString IN3BookInfo::GetUUIDString() const 
{ 
	return m_UUIDString; 
};

/**
 * @brief	책의 타이틀을 반환
 * @return 책 제목
 */
const CString IN3BookInfo::GetTitle() const 
{ 
	return m_title; 
};

/**
 * @brief	책의 저자를 반환
 * @return	저자명
 */
const CString IN3BookInfo::GetCreator() const 
{ 
	return m_creator;
};

/**
 * @brief	책의 주제를 반환한다.
 * @return	주제
 */
const CString IN3BookInfo::GetSubject() const
{
	return m_subject;
}

/**
 * @brief	책의 출판사명을 반환한다.
 * @reutrn	출판사명
 */
const CString IN3BookInfo::GetPublisher() const 
{ 
	return m_publisher; 
};

/**
 * @brief	출판일을 반환한다.
 * @return	출판일
 */
const CString IN3BookInfo::GetDate() const
{
	return m_date;
}

/**
 * @brief	책의 설명을 반환한다.
 * @return	책의 설명
 */
const CString IN3BookInfo::GetDesc() const 
{ 
	return m_desc; 
};

/**
 * @brief	책의 커버 이미지 경로를 반환한다.
 * @return	책의 커버 이미지
 */
const CString IN3BookInfo::GetCoverHref() const 
{ 
	return m_coverHref; 
};

IN3BookInfo& IN3BookInfo::operator=(const IN3BookInfo &other)
{
	m_version = other.GetVersion();
	m_bFixed = other.IsFixed();
	m_srcRootPath = other.GetSrcRootPath();
	m_UUIDString = other.GetUUIDString();
	m_title = other.GetTitle();
	m_creator = other.GetCreator();
	m_subject = other.GetSubject();
	m_publisher = other.GetPublisher();
	m_date = other.GetDate();
	m_desc = other.GetDesc();
	m_coverHref = other.GetCoverHref();

	return *this;
}





/*****************************************************************************************
 * IN3BookInfoEx
 *****************************************************************************************/
/**
 * @brief	생성자
 */
IN3BookInfoEx::IN3BookInfoEx(void)
	:IN3BookInfo()
	,m_type(_T(""))
	,m_format(_T(""))
	,m_language(_T(""))
	,m_source(_T(""))
	,m_relation(_T(""))
	,m_coverage(_T(""))
	,m_rights(_T(""))
	,m_contributor(_T(""))
{
}

/**
 * @brief	소멸자
 */
IN3BookInfoEx::~IN3BookInfoEx(void)
{
}

/**
 * @brief	책의 유형을 설정한다.
 * @param	type	: 책 유형
 */
void IN3BookInfoEx::SetType(CString type)
{
	m_type = type;
}

/**
 * @brief	책의 문서 형식을 설정한다.
 * @param	format	: 문서 형식
 */
void IN3BookInfoEx::SetFormat(CString format)
{
	m_format = format;
}

/**
 * @brief	책의 언어를 설정한다.
 * @param	language	: 언어
 */
void IN3BookInfoEx::SetLanguage(CString language)
{
	m_language = language;
}

/**
 * @brief	책의 출처를 설정한다.
 * @param	source	: 출처
 */
void IN3BookInfoEx::SetSource(CString source)
{
	m_source = source;
}

/**
 * @brief	관련자료를 설정한다.
 * @param	relation	: 관련자료
 */
void IN3BookInfoEx::SetRelation(CString relation)
{
	m_relation = relation;
}

/**
 * @brief	분야를 설정한다.
 * @param	coverage	: 분야
 */
void IN3BookInfoEx::SetCoverage(CString coverage)
{
	m_coverage = coverage;
}

/**
 * @brief	저작권을 설정한다.
 * @param	rights	: 저작권
 */
void IN3BookInfoEx::SetRights(CString rights)
{
	m_rights = rights;
}

/**
 * @brief	공헌자를 설정한다.
 * @param	contributor	: 공헌자
 */
void IN3BookInfoEx::SetContributor(CString contributor)
{
	m_contributor = contributor;
}

/**
 * @brief	책의 유형을 반환한다.
 * @return	책 유형
 */
const CString IN3BookInfoEx::GetType() const
{
	return m_type;
}

/**
 * @brief	책의 문서 형식을 반환한다.
 * @return	문서 형식
 */
const CString IN3BookInfoEx::GetFormat() const
{
	return m_format;
}

/**
 * @brief	책의 언어를 반환한다.
 * @return	언어
 */
const CString IN3BookInfoEx::GetLanguage() const
{
	return m_language;
}

/**
 * @brief	책의 출처를 반환한다.
 * @return	출처
 */
const CString IN3BookInfoEx::GetSource() const
{
	return m_source;
}

/**
 * @brief	관련자료를 반환한다.
 * @return	관련자료
 */
const CString IN3BookInfoEx::GetRelation() const
{
	return m_relation;
}

/**
 * @brief	분야를 반환한다.
 * @return	분야
 */
const CString IN3BookInfoEx::GetCoverage() const
{
	return m_coverage;
}

/**
 * @brief	저작권을 반환한다.
 * @return	저작권
 */
const CString IN3BookInfoEx::GetRights() const
{
	return m_rights;
}

/**
 * @brief	공헌자를 반환한다.
 * @return	공헌자
 */
const CString IN3BookInfoEx::GetContributor() const
{
	return m_contributor;
}

IN3BookInfoEx& IN3BookInfoEx::operator=(const IN3BookInfo &other)
{
	(IN3BookInfo)(*this) = other;

	return *this;
}

IN3BookInfoEx& IN3BookInfoEx::operator=(const IN3BookInfoEx &other)
{	
	(IN3BookInfo)(*this) = (IN3BookInfo)other;
	/*
	m_version = other.GetVersion();
	m_bFixed = other.IsFixed();
	m_srcRootPath = other.GetSrcRootPath();
	m_UUIDString = other.GetUUIDString();
	m_title = other.GetTitle();
	m_creator = other.GetCreator();
	m_subject = other.GetSubject();
	m_publisher = other.GetPublisher();
	m_date = other.GetDate();
	m_desc = other.GetDesc();
	m_coverHref = other.GetCoverHref();
	*/

	m_type = other.GetType();
	m_format = other.GetFormat();
	m_language = other.GetLanguage();
	m_source = other.GetSource();
	m_relation = other.GetRelation();
	m_coverage = other.GetCoverage();
	m_rights = other.GetRights();
	m_contributor = other.GetContributor();

	return *this;
}