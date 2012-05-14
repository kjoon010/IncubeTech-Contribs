
// InCubePublisherViewer.h : InCubePublisherViewer 응용 프로그램에 대한 주 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"       // 주 기호입니다.
#include "IN3WebkitEngine.h"

// CInCubePublisherViewerApp:
// 이 클래스의 구현에 대해서는 InCubePublisherViewer.cpp을 참조하십시오.
//

class CInCubePublisherViewerApp : public CWinAppEx
{
public:
	CInCubePublisherViewerApp();


// 재정의입니다.
public:
	virtual BOOL InitInstance();

// 구현입니다.
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	IN3WebkitEngine		m_WebkitEngine;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);

	
	BOOL ExtractEPub(LPCTSTR lpszPathName, LPCTSTR lpszDescPath);

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CInCubePublisherViewerApp theApp;
