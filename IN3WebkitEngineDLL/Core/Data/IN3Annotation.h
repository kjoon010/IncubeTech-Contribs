#pragma once


/**
  @class	IN3Annotation
  @brief	annotation은 하이라이트/메모/언더라인로 구분된다.
 */
class AFX_EXT_CLASS IN3Annotation
{
public:
	// 생성자
	IN3Annotation(void);
	IN3Annotation(bool bMemo, int start, int end, CString markerID, CString fileName, CString selection, int page =0);
	IN3Annotation(bool bMemo, int start, int end, CString markerID, CString fileName, CString selection,
		CString bgColor, CString ulColor, int page =0);

	// 소멸자
	~IN3Annotation(void);

	// initialize
	void Init(bool bMemo, int start, int end, CString markerID, CString fileName, CString selection,
		CString bgColor, CString ulColor, int page =0);

	// setter
	void SetMemo(bool bMemo);
	void SetIndex(int start, int end);
	void SetStart(int start);
	void SetEnd(int end);
	void SetMarkerID(CString id);
	void SetFileName(CString fileName);
	void SetSelection(CString str);
	void SetBGColor(CString color);
	void SetULColor(CString color);
	void SetPage(int page);
	void SetAnnotationRect(int left, int top, int right, int bottom);

	// getter
	const bool HasMemo() const;
	const int GetStart() const;
	const int GetEnd() const;
	const CString GetMarkerID() const;
	const CString GetFileName() const;
	const CString GetSelection() const;
	const CString GetBGColor() const;
	const CString GetULColor() const;
	const int GetPage() const;
	const CRect GetAnnotationRect() const;

	CString	GetJSONString(); ///< 마커 정보를 json string 으로 변환하여 반환한다.			 


protected:
	bool		m_bMemo; ///< 메모가 있는지 여부
	int			m_nStart; ///< annotation 시작 위치
	int			m_nEnd; ///< annotation 끝 위치
	int			m_nPage; ///< 현재 fixed layout 컨텐츠의 경우에만 유효한 값을 가진다.

	CString		m_markerID; ///< 추가된 마커(annotation) ID
	CString		m_fileName; ///< annotation 이 추가된 파일 명(unzip 되어 있는 절대 경로)
	CString		m_selection; ///< annotation 이 추가되어 있는 문자열

	CString		m_bgColor; ///< 하이라이트 색상 ex) rgba(255, 100, 100, 0.6)
	CString		m_ulColor; ///< 언더라인 색상(현재는 지원하지 않음)

	CRect		m_rect; ///< annotation 삽입 위치
};



/**
  @class	IN3Bookmark
  @brief	북마크
 */
class AFX_EXT_CLASS IN3Bookmark
{
public:
	// 생성자
	IN3Bookmark(void);
	IN3Bookmark(int type, CString fileName, CString markerID, int index, int page = 0);

	// 소멸자
	~IN3Bookmark(void);

	// setter
	void SetMarkerIndex(int index);		
	void SetFileName(CString fileName);	
	void SetMarkerID(CString id);		
	void SetPage(int page);		

	// getter
	const int GetMarkerIndex() const;
	const CString GetFileName() const;
	const CString GetMarkerID() const;
	const int GetPage() const;

	CString GetJSONString(); //< 북마크 정보를 JSON String 으로 변환하여 반환한다.

protected:
	int			m_makerIndex; //< 북마크 삽입 위치
	int			m_nPage; //< 현재 fixed layout 컨텐츠의 경우에만 유효한 값을 가진다.

	CString		m_fileName; //< 북마크 삽입된 파일 명
	CString		m_markerID; //< 북마크 아이디
};
