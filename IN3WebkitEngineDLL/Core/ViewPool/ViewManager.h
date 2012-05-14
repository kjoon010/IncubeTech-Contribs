#pragma once

#include <vector>

/**
  @class	CViewManager
  @brief	epub �� ����� �����ϴ� �Ŵ��� 
  @remarks	�� ����Ʈ�� ������ �ִ�.
 */
class CViewManager
{
public:
	// ������
	CViewManager(void);

	// �Ҹ���
	~CViewManager(void);

	// createView
	BOOL CreateView(CWnd* pBook);
	BOOL CreateView(CWnd* pBook, CRuntimeClass* pRuntimeClass);


	// view's information
	const int GetCount(void) const;
	const int GetCurrChapter() const;
	void SetCurrChapter(int chapter);

	void* GetCurrentView();
	void* GetView(int index);


	// page interface
	int GetTotalPage();
	int GetTotalPage(int index);

protected:	
	typedef std::vector<void *> IN3WebkitViewList;
	IN3WebkitViewList	m_viewList;	///< �� ����Ʈ

	int m_nStartChapter; ///< view list �� �ִ� ù��° é�� (spine �ε��� ����)
	int m_nEndChapter; ///< view list �� �ִ� ������ é�� (spine �ε��� ����)
	int	m_nCurrChapter; ///< ���� view �� ���̰� �ִ� spine �ε���
};
