
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "XMLDom.h"
#include "TStringEx.h"

/*
 * Namespace ����
 */
#define NAMESPACE_URI_EPUB_W			USTR("http://www.idpf.org/2007/ops ")
#define NAMESPACE_EPUB_W				USTR("xmlns:epub=\"http://www.idpf.org/2007/ops \"")


/**
  @class	NAVXML
  @brief	���� ���� �ļ�. epub 3.0 spec �ؼ�
 */
class NAVXML : public CXMLDOMDocument2
{
public:
	// ������
	NAVXML();

	// �Ҹ���
	~NAVXML();

	CXMLDOMNodeList GetNavList(); ///< nav ����Ʈ�� �����´�
	CXMLDOMNodeList GetNavList(const CXMLDOMElement &liElement); ///< Ư�� ��带 �������� nav ����Ʈ�� �����´�.	

	static bool GetHrefAndTitleFromLI(const CXMLDOMElement &liElement, TString<wchar_t> &href, TString<wchar_t> &title); ///< Ư�� element �� toc title �� toc href ������ �����´�.

	CXMLDOMElement FindNav(const wchar_t* srcURL); ///< url�� ��ġ�ϴ� nav �� �˻�

protected:
	long		mPlayOrder;		///< play order
};

