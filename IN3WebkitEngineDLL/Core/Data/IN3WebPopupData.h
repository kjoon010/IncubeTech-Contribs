#pragma once

 
/**
  @class	IN3WebPopupData
  @brief	Epub ���� ���ǵ� web ��ũ ���� �������̴�.
			����) �ɸ����� LTI(�Խ��� ����) ������ ���ǵǾ�����, ���� �ʿ��� ��� Ȯ���Ͽ� ���
 */
class AFX_EXT_CLASS IN3WebPopupData
{
public:
	// ������
	IN3WebPopupData(void);
	IN3WebPopupData(CString url, int width, int height);

	// �Ҹ���
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
	CString m_url; ///< ������ ��ũ�� url
	int m_width; ///< ��ũ �˾��� width
	int m_height; ///< ��ũ �˾��� height
};
