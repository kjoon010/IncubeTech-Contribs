#pragma once


extern CString Utf8ToStringT(LPSTR str);
extern CString UrlDecode(LPCTSTR url);

////////////////////////////////////////////////////////////////////////////////////////////
// IN3WebLib 입니다.
// 랜더링에 사용될 JS, CSS 의 경로 또는 파일 내용들을 관리하는 클래스

class IN3WebLib
{
public:
	// 생성자
	IN3WebLib(void);

	// 소멸자
	~IN3WebLib(void);

	static CString GetPathLibIN3Jquery();	// 현재 응용프로그램으로 부터의 상대 경로
	static CString GetPathLibIN3InitJS();	// 현재 응용프로그램으로 부터의 상대 경로
	static CString GetPathLibIN3JS();		// 현재 응용프로그램으로 부터의 상대 경로
	static CString GetPathLibIN3Css();		// 현재 응용프로그램으로 부터의 상대 경로
	static CString GetPathMemoIcon();		// 현재 응용프로그램으로 부터의 상대 경로
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