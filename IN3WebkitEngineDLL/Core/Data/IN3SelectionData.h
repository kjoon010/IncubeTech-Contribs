#pragma once

/**
  @class	IN3SelectionData
  @brief	����ڰ� �巡���Ͽ� ������ ������
			���� ���ο��� �ʿ信 ���� ���ܿ��� ��밡���� �����͸� �����ϱ� ���� �뵵
 */
class AFX_EXT_CLASS IN3SelectionData
{
public:
	// ������
	IN3SelectionData(void);
	IN3SelectionData(POINT pt, CString selectedText);

	// �Ҹ���
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
