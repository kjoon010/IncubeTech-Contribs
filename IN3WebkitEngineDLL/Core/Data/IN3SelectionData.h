#pragma once

/**
  @class	IN3SelectionData
  @brief	사용자가 드래그하여 선택한 데이터
			엔진 내부에서 필요에 의해 뷰어단에서 사용가능한 데이터를 제공하기 위한 용도
 */
class AFX_EXT_CLASS IN3SelectionData
{
public:
	// 생성자
	IN3SelectionData(void);
	IN3SelectionData(POINT pt, CString selectedText);

	// 소멸자
	~IN3SelectionData(void);

	// setter
	void SetSelectPoint(POINT pt);
	void SetSelectedText(CString selected);

	// getter
	const POINT GetSelectPoint() const;
	const CString GetSelectedText() const;

protected:
	POINT	m_pt; ///< mouse position
	CString m_selectedText; ///< selected text
};
