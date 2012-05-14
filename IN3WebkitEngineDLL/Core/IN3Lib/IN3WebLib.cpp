#include "StdAfx.h"
#include "IN3WebLib.h"


#define IsHexNum(c) ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))
CString Utf8ToStringT(LPSTR str)
{
    _ASSERT(str);
    USES_CONVERSION;
    WCHAR *buf;
    int length = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
    buf = new WCHAR[length+1];
    ZeroMemory(buf, (length+1) * sizeof(WCHAR));
    MultiByteToWideChar(CP_UTF8, 0, str, -1, buf, length);
    return (CString(W2T(buf)));
}

CString UrlDecode(LPCTSTR url)
{
    _ASSERT(url);
    USES_CONVERSION;
    LPSTR _url = T2A(const_cast<LPTSTR>(url));
    int i = 0;
    int length = (int)strlen(_url);
    CHAR *buf = new CHAR[length];
    ZeroMemory(buf, length);
    LPSTR p = buf;
    while(i < length)
    {
        if(i <= length -3 && _url[i] == '%' && IsHexNum(_url[i+1]) && IsHexNum(_url[i+2]))
        {
            sscanf(_url + i + 1, "%x", p++);
            i += 3;
        }
        else
        {
            *(p++) = _url[i++];
        }
    }
    return Utf8ToStringT(buf);
}

////////////////////////////////////////////////////////////////////////////////////////////
// IN3WebLib 입니다.

CString IN3WebLib::strLibIN3Jquery = _T("jquery-1.7.1.min.js");
CString IN3WebLib::strLibIN3InitJS = _T("in3.init.js");
CString IN3WebLib::strLibIN3Js = _T("in3.webkit.api.js");
CString IN3WebLib::strLibIN3Css = _T("IN3_EPUB_VIEWER.css");
CString IN3WebLib::strMemoIcon = _T("memoicon.png");
CString IN3WebLib::strMemoLeftIcon = _T("memo_left.png");
CString IN3WebLib::strMemoRightIcon = _T("memo_right.png");
//
CString IN3WebLib::strPathLibIN3 = _T("in3/in3_web_lib/");
//

/**
 * @brief	생성자
 */
IN3WebLib::IN3WebLib(void)
{
}

/**
 * @brief	소멸자
 */
IN3WebLib::~IN3WebLib(void)
{
}

CString IN3WebLib::GetPathLibIN3Jquery()
{
	CString path(_T(""));
	path.Append(strPathLibIN3);
	path.Append(strLibIN3Jquery);
	return path;
}

CString IN3WebLib::GetPathLibIN3InitJS()
{
	CString path(_T(""));
	path.Append(strPathLibIN3);
	path.Append(strLibIN3InitJS);
	return path;
}

CString IN3WebLib::GetPathLibIN3JS()
{
	CString path(_T(""));
	path.Append(strPathLibIN3);
	path.Append(strLibIN3Js);
	return path;
}

CString IN3WebLib::GetPathLibIN3Css()
{
	CString path(_T(""));
	path.Append(strPathLibIN3);
	path.Append(strLibIN3Css);
	return path;
}

CString IN3WebLib::GetPathMemoIcon()		// 현재 응용프로그램으로 부터의 상대 경로	
{
	CString path(_T(""));
	path.Append(strPathLibIN3);
	path.Append(strMemoIcon);
	return path;
}

CString IN3WebLib::GetPathMemoLeftIcon()
{
	CString path(_T(""));
	path.Append(strPathLibIN3);
	path.Append(strMemoLeftIcon);
	return path;
}

CString IN3WebLib::GetPathMemoRightIcon()
{
	CString path(_T(""));
	path.Append(strPathLibIN3);
	path.Append(strMemoRightIcon);
	return path;
}