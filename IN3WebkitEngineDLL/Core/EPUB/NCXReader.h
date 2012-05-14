
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "XMLDom.h"
#include "OPFReader.h"
#include "TStringEx.h"

/*
 * Namespace 정의
 */
#define NAMESPACE_URI_NCX_W			USTR("http://www.daisy.org/z3986/2005/ncx/")
#define NAMESPACE_NCX_W				USTR("xmlns:ncx=\"http://www.daisy.org/z3986/2005/ncx/\"")


/**
  @class	NCXXML
  @brief	목차 파일 파서. epub 2.0 spec 준수
 */
class NCXXML : public CXMLDOMDocument2
{
public:
	// 생성자
	NCXXML();

	// 소멸자
	~NCXXML();

	// initialize
    void Init(OPFXML &opfXML, const unicode_t* lpzOPFFolderPath); ///< opf 파일을 기준으로 ncx 를 생성하는 함수, 뷰어에서는 사용하지 않는 함수이다.

	// get navPoint data
	bool GetUniqueID(TString<unicode_t> &uuidString); ///< get epub's unique ID
	CXMLDOMNodeList GetNavPointList(); ///< get navPoint element List

	bool GetTitleFromURL(const unicode_t* srcURL, TString<unicode_t> &title); ///< url 이 일치하는 navPoint element 에서 title 추출
	
	CXMLDOMElement FindNavPoint(const unicode_t* srcURL); ///< url 이 일치하는 navPoint element 를 검색


protected:
    void Init1(const unicode_t* docTitle, const unicode_t* uniqueID = NULL); ///< initialize
    CXMLDOMElement AddNavPoint(const unicode_t* src, const unicode_t* labeltext, long playOrder = 0); ///< add NavPoint

protected:
	long		mPlayOrder; ///< play order
};

