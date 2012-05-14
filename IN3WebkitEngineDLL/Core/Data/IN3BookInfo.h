#pragma once


/**
  @class	IN3BookInfo
  @brief	å ����
 */
class AFX_EXT_CLASS IN3BookInfo
{
public:
	// ������
	IN3BookInfo(void);

	// �Ҹ���
	~IN3BookInfo(void);

	// setter
	void SetVersion(float ver);
	void SetFixed(bool bFixed);
	void SetSrcRootPath(CString root);
	void SetUUIDString(CString uuid);
	void SetTitle(CString title);
	void SetCreator(CString creator);
	void SetSubject(CString subject);
	void SetPublisher(CString publisher);
	void SetDate(CString date);
	void SetDesc(CString desc);
	void SetCoverHref(CString href);


	// getter
	const float	GetVersion() const;
	const BOOL	IsFixed() const;
	const CString GetSrcRootPath() const;
	const CString GetUUIDString() const;
	const CString GetTitle() const;
	const CString GetCreator() const;
	const CString GetSubject() const;
	const CString GetPublisher() const;
	const CString GetDate() const;
	const CString GetDesc() const;
	const CString GetCoverHref() const;

	
	// overloading
	IN3BookInfo& operator=(const IN3BookInfo &other); ///< assignment operator


protected:	
	float		m_version; ///< Epub version
	BOOL		m_bFixed; ///< is fixed content?
	CString		m_srcRootPath; ///< source file path
	CString		m_UUIDString; ///< Epub UUID
	CString		m_title; ///< å Ÿ��Ʋ
	CString		m_creator; ///< ����
	CString		m_subject; ///< ����
	CString		m_publisher; ///< ���ǻ�
	CString		m_date; ///< ������
	CString		m_desc; ///< ����
	CString		m_coverHref; ///< Ŀ�� �̹��� ���
};


/**
 * @class	IN3BookInfoEx
 * @brief	å ���� Ȯ��
 */
class AFX_EXT_CLASS IN3BookInfoEx : public IN3BookInfo
{
public:
	// ������
	IN3BookInfoEx(void);

	// �Ҹ���
	~IN3BookInfoEx(void);

	// getter
	void SetType(CString type);
	void SetFormat(CString format);
	void SetLanguage(CString language);
	void SetSource(CString source);
	void SetRelation(CString relation);
	void SetCoverage(CString coverage);
	void SetRights(CString rights);
	void SetContributor(CString contributor);

	// setter
	const CString GetType() const;
	const CString GetFormat() const;
	const CString GetLanguage() const;
	const CString GetSource() const;
	const CString GetRelation() const;
	const CString GetCoverage() const;
	const CString GetRights() const;
	const CString GetContributor() const;

	
	// overloading
	IN3BookInfoEx& operator=(const IN3BookInfo &other); ///< assignment operator
	IN3BookInfoEx& operator=(const IN3BookInfoEx &other); ///< assignment operator


protected:
	CString		m_type; ///< ����
	CString		m_format; ///< ��������
	CString		m_language; ///< ���
	CString		m_source; ///< ��ó
	CString		m_relation; ///< �����ڷ�
	CString		m_coverage; ///< �о�
	CString		m_rights; ///< ���۱�
	CString		m_contributor; ///< ������
};