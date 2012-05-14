#include "StdAfx.h"
#include "BookInfo.h"
#include "IN3WebkitDefine.h"

/**
 * @brief	������
 */
CBookInfo::CBookInfo(void)
	:IN3BookInfoEx()
	,m_unzipRootPath(_T(""))
	,m_nNavType(NONE)
	,m_packageName(_T(""))
	,m_opsFilePath(_T(""))
	,m_tocFilePath(_T(""))
	,m_fixedWidth(DEFAULT_CONTENT_WIDTH)
	,m_fixedHeight(DEFAULT_CONTENT_HEIGHT)
{
}

/**
 * @brief	�Ҹ���
 */
CBookInfo::~CBookInfo(void)
{
}

/**
 * @brief	toc Ÿ�� ����
 * @remars	3.0 spec = nav, 2.0 spec = ncx
 * @param	type	: toc Ÿ��
 */
void CBookInfo::SetNavType(long type) 
{ 
	m_nNavType = type; 
};

/**
 * @brief	fixed layout �ɼ� ����
 * @remarks	samsung spec �� ������ fixed ���̾ƿ��� PORTRAIT(����), LANDSCAPE(����), BOTH(����/���� ��� ����) �������� �ٴ���.
 * @param	opt	: fixed layout �ɼ�
 */
void CBookInfo::SetFixedOption(long opt) 
{ 
	m_fixedOption = opt; 
};

/**
 * @brief	package Folder �̸��� �����Ѵ�.
 * @remarks	ex) OEBPS
 * @param	packageName	: package Folder �̸�
 */
void CBookInfo::SetPakcageName(CString packageName) 
{ 
	m_packageName = packageName; 
};

/**
 * @brief	ops ������ ��� ��θ� �����Ѵ�.
 * @remarks	$(root)\$(packageName)\content.opf
 *			ex) $(root)\OEBPS\content.opf
 * @param	href	: ops ������ ��� ���
 */
void CBookInfo::SetOPSFilePath(CString href) 
{ 
	m_opsFilePath = href; 
};

/**
 * @brief	toc ������ ��� ��θ� �����Ѵ�.
 * @remarks	$(root)\$(packageName)\toc.xhtml or $(root)\$(packageName)\toc.ncx
 *			ex) $(root)\OEBPS\toc.xhtml
 * @param	href	: toc ������ ��� ���
 */
void CBookInfo::SetTocFilePath(CString href) 
{ 
	m_tocFilePath = href; 
};

/**
 * @brief	epub �� unzip �� ���� ��� ����
 * @param	root	: epub �� unzip �� ���� ���
 */
void CBookInfo::SetUnzipRootPath(CString root) 
{ 
	m_unzipRootPath = root; 
};

/**
 * @brief	fixed �������� ���� ���� ����
 * @param	width	: fixed �������� ���� ����
 */
void CBookInfo::SetFixedWidth(int width) 
{ 
	m_fixedWidth = width; 
};

/**
 * @brief	fixed �������� ���� ���� ����
 * @param	height	: fixed �������� ���� ����
 */
void CBookInfo::SetFixedHeight(int height) 
{ 
	m_fixedHeight = height; 
};

#if SUPPORT_THUMBNAIL_PATH
	// thumbnail path �� �������� �����ϴ� ���, unzip �� epub ��� �ȿ� thumbnail ������ ����� �ش� ��θ� ��ȯ�Ѵ�.
	// �׷��� �������� ������ ��θ� �����Ͽ� ������ �ʿ䰡 �����Ƿ� �ش� �Լ��� �������ϵ��� ó����.
/**
 * @brief	����� ��� ����
 * @param	path	: ������ ���
 */
void CBookInfo::SetThumbnailPath(CString path) 
{ 
	m_epubThumnailPath = path; 
};
#endif /*SUPPROT_THUMBNAIL_PATH*/

/**
 * @brief	toc Ÿ���� ��ȯ�Ѵ�.
 * @remars	3.0 spec = nav, 2.0 spec = ncx
 * @return	toc Ÿ��
 */
const long	CBookInfo::GetNavType() const 
{ 
	return m_nNavType; 
};

/**
 * @brief	fixed layout �ɼ��� ��ȯ�Ѵ�.
 * @remarks	samsung spec �� ������ fixed ���̾ƿ��� PORTRAIT(����), LANDSCAPE(����), BOTH(����/���� ��� ����) �������� �ٴ���.
 * @return	fixed layout �ɼ�
 */
const int CBookInfo::GetFixedOption() const 
{ 
	return m_fixedOption; 
};

/**
 * @brief	package Folder �̸��� ��ȯ�Ѵ�.
 * @remarks	ex) OEBPS
 * @return	package Folder �̸�
 */
const CString CBookInfo::GetPackageName() const 
{ 
	return m_packageName; 
};

/**
 * @brief	ops ������ ��� ��θ� ��ȯ�Ѵ�.
 * @remarks	$(root)\$(packageName)\content.opf
 *			ex) $(root)\OEBPS\content.opf
 * @return	ops ������ ��� ���
 */
const CString CBookInfo::GetOPSFilePath() const 
{ 
	return m_opsFilePath; 
};

/**
 * @brief	toc ������ ��� ��θ� ��ȯ�Ѵ�.
 * @remarks	$(root)\$(packageName)\toc.xhtml or $(root)\$(packageName)\toc.ncx
 *			ex) $(root)\OEBPS\toc.xhtml
 * @return	toc ������ ��� ���
 */
const CString CBookInfo::GetTocFilePath() const 
{ 
	return m_tocFilePath; 
};

/**
 * @brief	epub �� unzip �� ���� ��� ��ȯ
 * @return	epub �� unzip �� ���� ���
 */
const CString CBookInfo::GetUnzipRootPath() const 
{ 
	return m_unzipRootPath; 
};

/**
 * @brief	fixed �������� ���� ���� ��ȯ
 * @return	fixed �������� ���� ����
 */
const int CBookInfo::GetFixedWidth() const 
{ 
	return m_fixedWidth; 
};

/**
 * @brief	fixed �������� ���� ���� ��ȯ
 * @return	fixed �������� ���� ����
 */
const int CBookInfo::GetFixedHeight() const 
{ 
	return m_fixedHeight; 
};

#if SUPPORT_THUMBNAIL_PATH
	// thumbnail path �� �������� �����ϴ� ���, unzip �� epub ��� �ȿ� thumbnail ������ ����� �ش� ��θ� ��ȯ�Ѵ�.
	// �׷��� �������� ������ ��θ� �����Ͽ� ������ �ʿ䰡 �����Ƿ� �ش� �Լ��� �������ϵ��� ó����.
/**
 * @brief	����� ��� ��ȯ
 * @return ������ ���
 */
const CString CBookInfo::GetThumbnailPath() const 
{ 
	return m_epubThumnailPath; 
};
#endif /*SUPPROT_THUMBNAIL_PATH*/


CBookInfo& CBookInfo::operator=(const IN3BookInfo &other)
{
	(IN3BookInfo)(*this) = other;

	return *this;
}

CBookInfo& CBookInfo::operator=(const IN3BookInfoEx &other)
{
	(IN3BookInfoEx)(*this) = other;

	return *this;
}

CBookInfo& CBookInfo::operator=(const CBookInfo &other)
{
	(IN3BookInfoEx)(*this) = (IN3BookInfoEx)other;

	m_nNavType = other.GetNavType();
	m_fixedOption = other.GetFixedOption();
	m_packageName = other.GetPackageName();
	m_opsFilePath = other.GetOPSFilePath();
	m_tocFilePath = other.GetTocFilePath();
	m_unzipRootPath = other.GetUnzipRootPath();
	m_fixedWidth = other.GetFixedWidth();
	m_fixedHeight = other.GetFixedHeight();

	return *this;
}
