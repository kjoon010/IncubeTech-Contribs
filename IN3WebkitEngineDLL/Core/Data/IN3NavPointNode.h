#pragma once

#include <vector>

/**
  @class	IN3NavPointNode
  @brief	Epub �� TOC(����) �����͸� ǥ���ϴ� Ŭ����(Ʈ������)
 */
class AFX_EXT_CLASS IN3NavPointNode
{
public:
	// ������
	IN3NavPointNode(void);
	IN3NavPointNode(LPCTSTR title, LPCTSTR url);
	IN3NavPointNode(LPCTSTR title, LPCTSTR url, UINT page);

	// �Ҹ���
	~IN3NavPointNode(void);

	// setter
	void SetTitle(CString title);
	void SetURL(CString url);
	void SetPageNumber(UINT page);

	// getter
	const CString& GetTitle() const;
	const CString& GetURL() const;
	const UINT GetPageNumber() const;

	// get child node data
	long GetChildCount() const;
	const IN3NavPointNode& GetChild(long i) const;
	      IN3NavPointNode& GetChild(long i);
	
    // add, remove
	void Append(const IN3NavPointNode &navPointNode);
	void Release();

protected:
	CString		m_title; ///< ���� Ÿ��Ʋ
	CString		m_url; ///<///< ������ URL
	UINT		m_page; ///< ������ ������ (fixed layout Epub ������ ��ȿ�� ���� ������)

	typedef std::vector<IN3NavPointNode>	IN3NavPointNodeList;
	IN3NavPointNodeList	m_ChildNavPoints; ///< �ڽ� ����
};