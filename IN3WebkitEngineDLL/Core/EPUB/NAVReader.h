
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "XMLDom.h"
#include "TStringEx.h"

/*
 * Namespace 정의
 */
#define NAMESPACE_URI_EPUB_W			USTR("http://www.idpf.org/2007/ops ")
#define NAMESPACE_EPUB_W				USTR("xmlns:epub=\"http://www.idpf.org/2007/ops \"")


/**
  @class	NAVXML
  @brief	목차 파일 파서. epub 3.0 spec 준수
 */
class NAVXML : public CXMLDOMDocument2
{
public:
	// 생성자
	NAVXML();

	// 소멸자
	~NAVXML();

	CXMLDOMNodeList GetNavList(); ///< nav 리스트를 가져온다
	CXMLDOMNodeList GetNavList(const CXMLDOMElement &liElement); ///< 특정 노드를 기준으로 nav 리스트를 가져온다.	

	static bool GetHrefAndTitleFromLI(const CXMLDOMElement &liElement, TString<wchar_t> &href, TString<wchar_t> &title); ///< 특정 element 의 toc title 과 toc href 정보를 가져온다.

	CXMLDOMElement FindNav(const wchar_t* srcURL); ///< url이 일치하는 nav 를 검색

protected:
	long		mPlayOrder;		///< play order
};

