#pragma once


/**
  @class	IN3BookInfo
  @brief	책 정보
 */
class AFX_EXT_CLASS IN3BookInfo
{
public:
	// 생성자
	IN3BookInfo(void);

	// 소멸자
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
	CString		m_title; ///< 책 타이틀
	CString		m_creator; ///< 저자
	CString		m_subject; ///< 주제
	CString		m_publisher; ///< 출판사
	CString		m_date; ///< 출판일
	CString		m_desc; ///< 설명
	CString		m_coverHref; ///< 커버 이미지 경로
};


/**
 * @class	IN3BookInfoEx
 * @brief	책 정보 확장
 */
class AFX_EXT_CLASS IN3BookInfoEx : public IN3BookInfo
{
public:
	// 생성자
	IN3BookInfoEx(void);

	// 소멸자
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
	CString		m_type; ///< 유형
	CString		m_format; ///< 문서형식
	CString		m_language; ///< 언어
	CString		m_source; ///< 출처
	CString		m_relation; ///< 관련자료
	CString		m_coverage; ///< 분야
	CString		m_rights; ///< 저작권
	CString		m_contributor; ///< 공헌자
};