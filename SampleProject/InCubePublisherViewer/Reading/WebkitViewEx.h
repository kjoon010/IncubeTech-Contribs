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
	virtual void OnDraw(CDC* pDC);      // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	

protected:
	DECLARE_MESSAGE_MAP()
};
