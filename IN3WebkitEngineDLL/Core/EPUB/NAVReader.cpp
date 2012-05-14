#include "stdafx.h"
#include "NAVReader.h"


/*
 * nav sample 
 *
	<nav epub:type="lot">
		<h2>List of tables, broken down into individual groups, one per major section of the publication content</h2>
		<ol>
			<li><span>Tables in Chapter 1</span>
				<ol>
					<li><a href="chap1.xhtml#table-1.1">Table 1.1</a>
					</li>
					<li><a href="chap1.xhtml#table-1.2">Table 1.2</a></li>
				</ol>
			</li>
	        
			<li><span>Tables in Chapter 2</span>
				<ol>
					<li><a href="chap2.xhtml#table-2.1">Table 2.1</a>
					</li>
					<li><a href="chap2.xhtml#table-2.2">Table 2.2</a></li>
					<li><a href="chap2.xhtml#table-2.3">Table 2.3</a></li>
				</ol>
			</li>
			...
			<li><span>Tables in Appendix</span>
				<ol>
					<li><a href="appendix.xhtml#table-a.1">Table A.1</a>
					</li>
					<li><a href="appendix.xhtml#table-a.2">Table B.2</a></li>
				</ol>
			</li>
		</ol>
	</nav>
 */


// ----------------------------------------------------------------------------
// NCXXML
// ----------------------------------------------------------------------------
NAVXML::NAVXML()
: CXMLDOMDocument2(false)
{
}

NAVXML::~NAVXML()
{
}

/*
 * document Element �� �������� nav ����Ʈ�� �����´�.
 * ���ϰ� nav ����Ʈ
 */
CXMLDOMNodeList NAVXML::GetNavList()
{		
	SelectionNamespaces(NAMESPACE_EPUB_W);
	CXMLDOMElement docElem = GetDocumentElement();
	return docElem.SelectNodes(USTR("//nav/ol/li"));
}

/*
 * ������ element �� �������� nav ����Ʈ�� �����´�.
 * ���ϰ� nav ����Ʈ
 */
CXMLDOMNodeList NAVXML::GetNavList(const CXMLDOMElement &liElement)
{		
	SelectionNamespaces(NAMESPACE_EPUB_W);
	return liElement.SelectNodes(USTR(".//ol/li"));
}

/*
 * ������ nav ��忡�� ���� Ÿ��Ʋ��, href �� �����Ѵ�.
 * �Ķ����	[in] liElement	Ÿ��Ʋ�� href �� �����ϰ��� �ϴ� nav element
 *			[out] href		������ href
 *			[out] title		������
 * ���ϰ� true(success) / false(fail)
 */
bool NAVXML::GetHrefAndTitleFromLI(const CXMLDOMElement &liElement, TString<wchar_t> &href, TString<wchar_t> &title)
{
	CXMLDOMElement aElem = liElement.SelectSingleNode(USTR("./a"));
	if ( aElem )
	{
		aElem.GetAttribute(USTR("href"), href);
		aElem.GetText(title);
		return true;
	}
	
	CXMLDOMElement spanElem = liElement.SelectSingleNode(USTR("./span"));
	if ( spanElem )
	{
		href.Empty();
		spanElem.GetText(title);
		return true;
	}

	href.Empty();
	title.Empty();
	return false;
}

/*
 * FindNav
 * ������ url �� �������� ��ġ�ϴ� �����͸� �˻��Ѵ�.
 * �Ķ����	src		ã���� �ϴ� nav �������� src
 * ���ϰ�	�˻��� nav element
 */
CXMLDOMElement NAVXML::FindNav(const wchar_t* src)
{
	SelectionNamespaces(NAMESPACE_EPUB_W);

	UniString		str;
	str.Format(USTR("//a[@href='%s']"), src);

	CXMLDOMElement docElem = GetDocumentElement();
	return docElem.SelectSingleNode(str);
}
