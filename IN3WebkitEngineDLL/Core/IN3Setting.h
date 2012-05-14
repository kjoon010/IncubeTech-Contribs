#pragma once

#include <map>

#include "IN3Annotation.h"

/**
  @class	IN3Setting
  @brief	뷰어 설정 값들을 가지는 클래스
 */
class IN3Setting
{
public:
	// 생성자
	IN3Setting(void);

	// 소멸자
	~IN3Setting(void);

	/*
	 * IN3_SIGNE_PAGE_MODE : 단면 모드
	 * IN3_DUAL_PAGE_MODE : 양면 모드
	 */
	enum { IN3_SIGNE_PAGE_MODE = 1, IN3_DUAL_PAGE_MODE };
	

	// setter
	void SetDisplayPageNumber(bool bDisplay);
	void SetPageDisplayMode(int mode);
	void SetFontSize(int size);
	void SetMargin(int margin);
	void SetLineHeight(double height);
	void SetFontFamily(CString family);
	void SetFontColor(CString color);
	void SetBgColor(CString color);

	// getter
	const bool IsDisplayPageNumber() const;
	const int GetPageDisplayMode() const;
	const int GetFontSize() const;
	const int GetMargin() const;
	const double GetLineHeight() const;
	const CString GetFontFamily() const;
	const CString GetFontColor() const;
	const CString GetBgColor() const;


	// interface
	void AddAnnotation(int spineIndex, IN3Annotation *pAnnotation);
	void AddBookmark(int spineIndex, IN3Bookmark *pBookmark);

	void GetInitAnnotationParam(int spineIndex, CString &param);
	void GetInitBookmarkParam(int spineIndex, CString &param);
	void GetDeleteBookmarkParam(int spineIndex, CString &param);

	void ClearAnnotations();
	void ClearBookmarks();

protected:
	bool	m_bDisplayPageNumber; ///< 페이지 번호 표시 여부
	int		m_nPageMode; ///< 페이지 표시 방법. (양면/ 단면)
	int		m_nFontSize; ///< 폰트 사이즈
	int		m_margin; ///< 컬럼 갭
	double	m_lineHeight; ///< line height

	CString m_strFamily; ///< 폰트명
	CString m_strFontColor; ///< 폰트색
	CString m_strBgColor; ///< 배경색

	std::multimap<int, IN3Annotation *> annotations; ///< annotation 리스트, annotation를 파일별로 분류 시 임시로 사용되는 리스트
	std::multimap<int, IN3Bookmark *> bookmarks; ///< 북마크 리스트,  북마크를 파일별로 분류 시 임시로 사용되는 리스트

};
