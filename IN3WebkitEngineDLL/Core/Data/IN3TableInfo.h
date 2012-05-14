
#pragma once

#include <vector>


/**
  @class	CIN3TableInfo
  @brief	Epub ���� ����� ǥ ������
			Epub �� ��� ǥ�� ����Ǵ� ���� �ƴϸ� ǥ �� Ư�� Ŭ������ ������ ���� ������ ǥ���� �����Ѵ�.
			������ �����͸� EpubTableList �� ��� UI ������ �����Ͽ� UI �ܿ��� �ش� ǥ ����� ����ڿ��� �����ش�.
 */
class AFX_EXT_CLASS CIN3TableInfo
{
public:
	// ������
	CIN3TableInfo(void);
	CIN3TableInfo(CString fileName, CString tableId, CString caption, int page = 0);

	// �Ҹ���
	~CIN3TableInfo(void);

	// setter
	void SetTableID(CString strID);
	void SetFileName(CString fileName);
	void SetCaption(CString caption);
	void SetPage(int page);

	// getter
	const CString GetTableID() const;
	const CString GetFileName() const;
	const CString GetCaption() const;
	const int GetPage() const;

protected:
	CString m_tableID; ///< ǥ ID
	CString m_fileName; ///< ǥ�� �ִ� ���� ��
	CString m_caption; ///< ǥ�� ĸ��
	int		m_page; ///< ǥ�� �ִ� ������ �� (fixed layout Epub ������ ��ȿ�� ���� ������)
};

typedef std::vector<CIN3TableInfo>	EpubTableList;