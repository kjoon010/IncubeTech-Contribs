#pragma once


extern CString Utf8ToStringT(LPSTR str);
extern CString UrlDecode(LPCTSTR url);

////////////////////////////////////////////////////////////////////////////////////////////
// IN3WebLib �Դϴ�.
// �������� ���� JS, CSS �� ��� �Ǵ� ���� ������� �����ϴ� Ŭ����

class IN3WebLib
{
public:
	// ������
	IN3WebLib(void);

	// �Ҹ���
	~IN3WebLib(void);

	static CString GetPathLibIN3Jquery();	// ���� �������α׷����� ������ ��� ���
	static CString GetPathLibIN3InitJS();	// ���� �������α׷����� ������ ��� ���
	static CString GetPathLibIN3JS();		// ���� �������α׷����� ������ ��� ���
	static CString GetPathLibIN3Css();		// ���� �������α׷����� ������ ��� ���
	static CString GetPathMemoIcon();		// ���� �������α׷����� ������ ��� ���
	static CString GetPathMemoLeftIcon();
	static CString GetPathMemoRightIcon();

protected:
	///*
	static CString strLibIN3Jquery;			// jquery file name
	static CString strLibIN3InitJS;			// javascript file name
	static CString strLibIN3Js;				// javascript file name
	static CString strLibIN3Css;			// css file name
	static CString strMemoIcon;
	static CString strMemoLeftIcon;
	static CString strMemoRightIcon;
	/*
	static CString strLibIN3JqueryFullTag;
	static CString strLibIN3FullTag;
	static CString strLibIN3CssFulllTag;
	//*/
	static CString strPathLibIN3;			// library path
	//*/
};