#pragma once

#include <map>

#include "IN3Annotation.h"

/**
  @class	IN3Setting
  @brief	��� ���� ������ ������ Ŭ����
 */
class IN3Setting
{
public:
	// ������
	IN3Setting(void);

	// �Ҹ���
	~IN3Setting(void);

	/*
	 * IN3_SIGNE_PAGE_MODE : �ܸ� ���
	 * IN3_DUAL_PAGE_MODE : ��� ���
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
	bool	m_bDisplayPageNumber; ///< ������ ��ȣ ǥ�� ����
	int		m_nPageMode; ///< ������ ǥ�� ���. (���/ �ܸ�)
	int		m_nFontSize; ///< ��Ʈ ������
	int		m_margin; ///< �÷� ��
	double	m_lineHeight; ///< line height

	CString m_strFamily; ///< ��Ʈ��
	CString m_strFontColor; ///< ��Ʈ��
	CString m_strBgColor; ///< ����

	std::multimap<int, IN3Annotation *> annotations; ///< annotation ����Ʈ, annotation�� ���Ϻ��� �з� �� �ӽ÷� ���Ǵ� ����Ʈ
	std::multimap<int, IN3Bookmark *> bookmarks; ///< �ϸ�ũ ����Ʈ,  �ϸ�ũ�� ���Ϻ��� �з� �� �ӽ÷� ���Ǵ� ����Ʈ

};
