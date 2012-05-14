#pragma once

 
/**
  @class	IN3WebPopupData
  @brief	Epub 내에 정의된 web 링크 관련 데이터이다.
			참고) 케리스의 LTI(게시판 연동) 용으로 정의되었으며, 추후 필요할 경우 확장하여 사용
 */
class AFX_EXT_CLASS IN3WebPopupData
{
public:
	// 생성자
	IN3WebPopupData(void);
	IN3WebPopupData(CString url, int width, int height);

	// 소멸자
	~IN3WebPopupData(void);

	// setter
	void SetURL(CString url);
	void SetPopupSize(int width, int height);
	void SetPopupWidth(int width);
	void SetPopupHeight(int height);

	// getter
	const CString GetURL() const;
	const int GetWidth() const;
	const int GetHeight() const;
	
protected:
	CString m_url; ///< 연동할 링크의 url
	int m_width; ///< 링크 팝업의 width
	int m_height; ///< 핑크 팝업의 height
};
