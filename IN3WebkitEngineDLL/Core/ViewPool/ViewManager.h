#pragma once

#include <vector>

/**
  @class	CViewManager
  @brief	epub 의 뷰들을 관리하는 매니져 
  @remarks	뷰 리스트를 가지고 있다.
 */
class CViewManager
{
public:
	// 생성자
	CViewManager(void);

	// 소멸자
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
	IN3WebkitViewList	m_viewList;	///< 뷰 리스트

	int m_nStartChapter; ///< view list 에 있는 첫번째 챕터 (spine 인덱스 기준)
	int m_nEndChapter; ///< view list 에 있는 마지막 챕터 (spine 인덱스 기준)
	int	m_nCurrChapter; ///< 현재 view 에 보이고 있는 spine 인덱스
};
