
// InCubePublisherViewer.h : InCubePublisherViewer ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.
#include "IN3WebkitEngine.h"

// CInCubePublisherViewerApp:
// �� Ŭ������ ������ ���ؼ��� InCubePublisherViewer.cpp�� �����Ͻʽÿ�.
//

class CInCubePublisherViewerApp : public CWinAppEx
{
public:
	CInCubePublisherViewerApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.
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
