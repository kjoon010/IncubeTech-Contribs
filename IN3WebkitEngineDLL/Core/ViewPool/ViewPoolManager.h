#pragma once

#include <map>

// CViewPoolManager
/**
 * @class	CViewPoolManager
 * @brief	������ �������� ���� ������� ViewManager ���� �����ϴ� Ŭ����
 * @remarks	��ü ��������� ���� ������ �����ϸ�, å�� ���� �̵��Ҷ� ���� ViewManaer ���� ���� ������ ���� ������ ���̰Ŵ� �ø� �뵵�� ����Ϸ� �Ͽ�����,
 *			�ϴ� ����� å�� �ʿ��� ��� �並 ����� �����Ͽ� ������ ����� ������ �����ϴ� ����� ���� ������� �ʴ´�.
 *			���� �ش� ����� ���� �����ؾ� �Ѵ�.
 */
class CViewPoolManager
{
public:
	// ������
	CViewPoolManager(void);

	// �Ҹ���
	~CViewPoolManager(void);

	BOOL	CreateView(CWnd* pBook, CRuntimeClass* pRuntimeClass);
	BOOL	RemoveView(CWnd* pBook);

protected:
	int			m_nPoolCnt;	///< ���� ���ο� ������ ��ü ���� ����

	typedef std::map< CWnd*, void*, std::less<CWnd*>> ViewPool;
	ViewPool	m_pool; ///< �� �Ŵ�������Ʈ
};
