#pragma once

#include "IN3WebkitView.h"

class CWebkitViewEx :
	public IN3WebkitView
{
	DECLARE_DYNCREATE(CWebkitViewEx)

public:
	CWebkitViewEx(void);
	~CWebkitViewEx(void);

public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
	

protected:
	DECLARE_MESSAGE_MAP()
};
