#include "StdAfx.h"
#include "IN3BookInfo.h"

/*****************************************************************************************
 * IN3BookInfo
 *****************************************************************************************/
/**
 * @brief	������
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
 * @brief	�Ҹ���
 */
IN3BookInfo::~IN3BookInfo(void)
{
}

/**
 * @brief	å�� ���� ������ �����Ѵ�.	
 * @param	ver	: epub ����
 */
void IN3BookInfo::SetVersion(float ver) 
{ 
	m_version = ver; 
};

/**
 * @brief	fixed ���̿� ������ �����Ѵ�.	
 * @param	bFixed	: fixed ������ ���� (true: fixed, false: reflow)
 */
void IN3BookInfo::SetFixed(bool bFixed) 
{ 
	m_bFixed = bFixed; 
};

/** 
 * @brief	å�� ���� ���� ��θ� ����
 * @param	root	: å�� ���� ���� ���
 */
void IN3BookInfo::SetSrcRootPath(CString root) 
{ 
	m_srcRootPath = root; 
};

/**
 * @brief	å�� �ĺ���(UUID) �� ����
 * @param	uuid	: å�� �ĺ���
 */
void IN3BookInfo::SetUUIDString(CString uuid) 
{ 
	m_UUIDString = uuid; 
};

/**
 * @brief	å�� Ÿ��Ʋ�� ����
 * @param	title	: å ����
 */
void IN3BookInfo::SetTitle(CString title) 
{ 
	m_title = title; 
};

/**
 * @brief	å�� ���ڸ� ����
 * @param	creator	: ���ڸ�
 */
void IN3BookInfo::SetCreator(CString creator) 
{ 
	m_creator = creator; 
};

/**
 * @brief	å�� ������ �����Ѵ�.
 * @param	subject	: ����
 */
void IN3BookInfo::SetSubject(CString subject)
{
	m_subject = subject;
}

/**
 * @brief	å�� ���ǻ���� �����Ѵ�.
 * @param	publisher	: ���ǻ��
 */
void IN3BookInfo::SetPublisher(CString publisher) 
{ 
	m_publisher = publisher; 
};

/**
 * @brief	�������� �����Ѵ�.
 * @param	date	: ������
 */
void IN3BookInfo::SetDate(CString date)
{
	m_date = date;
}

/**
 * @brief	å�� ������ �����Ѵ�.
 * @param	desc	: å�� ����
 */
void IN3BookInfo::SetDesc(CString desc) 
{ 
	m_desc = desc; 
};

/**
 * @brief	å�� Ŀ�� �̹��� ��θ� �����Ѵ�.
 * @param	href	: å�� Ŀ�� �̹���
 */
void IN3BookInfo::SetCoverHref(CString href) 
{ 
	m_coverHref = href; 
};

/**
 * @brief	å�� ���� ������ ��ȯ�Ѵ�.	
 * @return	epub ����
 */
const float	IN3BookInfo::GetVersion() const 
{ 
	return m_version; 
};

/**
 * @brief	fixed ���̿� ������ ��ȯ�Ѵ�.	
 * return	fixed ������ ���� (true: fixed, false: reflow)
 */
const BOOL	IN3BookInfo::IsFixed() const 
{ 
	return m_bFixed; 
};

/** 
 * @brief	å�� ���� ���� ��θ� ��ȯ
 * @reutrn	å�� ���� ���� ���
 */
const CString IN3BookInfo::GetSrcRootPath() const 
{ 
	return m_srcRootPath; 
};

/**
 * @brief	å�� �ĺ���(UUID) �� ��ȯ
 * @return	å�� �ĺ���
 */
const CString IN3BookInfo::GetUUIDString() const 
{ 
	return m_UUIDString; 
};

/**
 * @brief	å�� Ÿ��Ʋ�� ��ȯ
 * @return å ����
 */
const CString IN3BookInfo::GetTitle() const 
{ 
	return m_title; 
};

/**
 * @brief	å�� ���ڸ� ��ȯ
 * @return	���ڸ�
 */
const CString IN3BookInfo::GetCreator() const 
{ 
	return m_creator;
};

/**
 * @brief	å�� ������ ��ȯ�Ѵ�.
 * @return	����
 */
const CString IN3BookInfo::GetSubject() const
{
	return m_subject;
}

/**
 * @brief	å�� ���ǻ���� ��ȯ�Ѵ�.
 * @reutrn	���ǻ��
 */
const CString IN3BookInfo::GetPublisher() const 
{ 
	return m_publisher; 
};

/**
 * @brief	�������� ��ȯ�Ѵ�.
 * @return	������
 */
const CString IN3BookInfo::GetDate() const
{
	return m_date;
}

/**
 * @brief	å�� ������ ��ȯ�Ѵ�.
 * @return	å�� ����
 */
const CString IN3BookInfo::GetDesc() const 
{ 
	return m_desc; 
};

/**
 * @brief	å�� Ŀ�� �̹��� ��θ� ��ȯ�Ѵ�.
 * @return	å�� Ŀ�� �̹���
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
 * @brief	������
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
 * @brief	�Ҹ���
 */
IN3BookInfoEx::~IN3BookInfoEx(void)
{
}

/**
 * @brief	å�� ������ �����Ѵ�.
 * @param	type	: å ����
 */
void IN3BookInfoEx::SetType(CString type)
{
	m_type = type;
}

/**
 * @brief	å�� ���� ������ �����Ѵ�.
 * @param	format	: ���� ����
 */
void IN3BookInfoEx::SetFormat(CString format)
{
	m_format = format;
}

/**
 * @brief	å�� �� �����Ѵ�.
 * @param	language	: ���
 */
void IN3BookInfoEx::SetLanguage(CString language)
{
	m_language = language;
}

/**
 * @brief	å�� ��ó�� �����Ѵ�.
 * @param	source	: ��ó
 */
void IN3BookInfoEx::SetSource(CString source)
{
	m_source = source;
}

/**
 * @brief	�����ڷḦ �����Ѵ�.
 * @param	relation	: �����ڷ�
 */
void IN3BookInfoEx::SetRelation(CString relation)
{
	m_relation = relation;
}

/**
 * @brief	�о߸� �����Ѵ�.
 * @param	coverage	: �о�
 */
void IN3BookInfoEx::SetCoverage(CString coverage)
{
	m_coverage = coverage;
}

/**
 * @brief	���۱��� �����Ѵ�.
 * @param	rights	: ���۱�
 */
void IN3BookInfoEx::SetRights(CString rights)
{
	m_rights = rights;
}

/**
 * @brief	�����ڸ� �����Ѵ�.
 * @param	contributor	: ������
 */
void IN3BookInfoEx::SetContributor(CString contributor)
{
	m_contributor = contributor;
}

/**
 * @brief	å�� ������ ��ȯ�Ѵ�.
 * @return	å ����
 */
const CString IN3BookInfoEx::GetType() const
{
	return m_type;
}

/**
 * @brief	å�� ���� ������ ��ȯ�Ѵ�.
 * @return	���� ����
 */
const CString IN3BookInfoEx::GetFormat() const
{
	return m_format;
}

/**
 * @brief	å�� �� ��ȯ�Ѵ�.
 * @return	���
 */
const CString IN3BookInfoEx::GetLanguage() const
{
	return m_language;
}

/**
 * @brief	å�� ��ó�� ��ȯ�Ѵ�.
 * @return	��ó
 */
const CString IN3BookInfoEx::GetSource() const
{
	return m_source;
}

/**
 * @brief	�����ڷḦ ��ȯ�Ѵ�.
 * @return	�����ڷ�
 */
const CString IN3BookInfoEx::GetRelation() const
{
	return m_relation;
}

/**
 * @brief	�о߸� ��ȯ�Ѵ�.
 * @return	�о�
 */
const CString IN3BookInfoEx::GetCoverage() const
{
	return m_coverage;
}

/**
 * @brief	���۱��� ��ȯ�Ѵ�.
 * @return	���۱�
 */
const CString IN3BookInfoEx::GetRights() const
{
	return m_rights;
}

/**
 * @brief	�����ڸ� ��ȯ�Ѵ�.
 * @return	������
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