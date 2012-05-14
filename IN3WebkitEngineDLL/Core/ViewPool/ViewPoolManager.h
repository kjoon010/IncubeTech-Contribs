#pragma once

#include <map>

// CViewPoolManager
/**
 * @class	CViewPoolManager
 * @brief	엔진의 관점에서 현재 만들어진 ViewManager 들을 관리하는 클래스
 * @remarks	전체 만들어지는 뷰의 개수를 관리하며, 책을 열고 이동할때 마다 ViewManaer 들이 가진 각각의 뷰의 개수를 줄이거니 늘릴 용도로 사용하려 하였으나,
 *			일단 현재는 책에 필요한 모든 뷰를 만들고 삭제하여 임의의 뷰들의 개수를 조정하는 기능이 없어 사용하지 않는다.
 *			또한 해당 기능은 추후 구현해야 한다.
 */
class CViewPoolManager
{
public:
	// 생성자
	CViewPoolManager(void);

	// 소멸자
	~CViewPoolManager(void);

	BOOL	CreateView(CWnd* pBook, CRuntimeClass* pRuntimeClass);
	BOOL	RemoveView(CWnd* pBook);

protected:
	int			m_nPoolCnt;	///< 엔진 내부에 생성된 전체 뷰의 개수

	typedef std::map< CWnd*, void*, std::less<CWnd*>> ViewPool;
	ViewPool	m_pool; ///< 뷰 매니저리스트
};
