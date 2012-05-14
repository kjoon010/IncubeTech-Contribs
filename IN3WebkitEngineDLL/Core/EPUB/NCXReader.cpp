#include "stdafx.h"
#include "NCXReader.h"
//#include "EpubFontEnDec.h"


/*
 * ncx example
 *
	 <ncx xmlns="http://www.daisy.org/z3986/2005/ncx/" version="2005-1" xml:lang="en-US">
		<head>
			<meta content="org-example-5059463624137734586" name="dtb:uid"/>
		</head>
	    
		<docTitle>
			<text>Selections from "Great Pictures, As Seen and Described by Famous Writers"</text>
		</docTitle>
	    
		<docAuthor>
			<text>Esther Singleton</text>
		</docAuthor>
	    
		<navMap>
			<navPoint class="h1" id="ch1">
				<navLabel>
					<text>Chapter 1</text>
				</navLabel>
				<content src="content.html#ch_1"/>
				<navPoint class="h2" id="ch_1_1">
					<navLabel>
						<text>Chapter 1.1</text>
					</navLabel>
					<content src="content.html#ch_1_1"/>
				</navPoint>
			</navPoint>
			<navPoint class="h1" id="ncx-2">
				<navLabel>
					<text>Chapter 2</text>
				</navLabel>
				<content src="content.html#ch_2"/>            
			</navPoint>
		</navMap>
	</ncx>
 */


// ----------------------------------------------------------------------------
// NCXXML
// ----------------------------------------------------------------------------

NCXXML::NCXXML()
: CXMLDOMDocument2(false), mPlayOrder(0)
{
}

NCXXML::~NCXXML()
{
}

/*
 * GetUniqueID
 * ncx 내에 기술되어 있는 Unique ID 를 가져온다.
 * 파라미터	[out] uuidString	uuid
 * 리턴값	true(uuid is valid)/false(uuid is empty)
 */
bool NCXXML::GetUniqueID(TString<unicode_t> &uuidString)
{
	CXMLDOMElement docElem = GetDocumentElement();

	SelectionNamespaces(NAMESPACE_NCX_W);
	CXMLDOMElement elem = docElem.SelectSingleNode(USTR("//ncx:head/ncx:meta[@name='dtb:uid']"));		// <head>의 서브 노드인 <meta> 노드 중, name='dtb:uid' 와 같은 attirbute 를 같는 노드 검색
	elem.GetAttribute(USTR("content"), uuidString);

	return !uuidString.IsEmpty();
}

/*
 * GetNavPointList
 * navPoint element List 를 반환한다.
 * 리턴값	navPoint element List
 */
CXMLDOMNodeList NCXXML::GetNavPointList()
{		
	CXMLDOMElement docElem = GetDocumentElement();

	SelectionNamespaces(NAMESPACE_NCX_W);
	return docElem.SelectNodes(USTR("//ncx:navMap/ncx:navPoint"));
}

/*
 * FindNavPoint
 * 임의의 src 와 일치하는 데이터를 가진 navPoint 검색
 * 파라미터	[in] src	찾고자 하는 navPoint 의 src
 * 리턴값	검색된 navPoint
 */
CXMLDOMElement NCXXML::FindNavPoint(const unicode_t* src)
{
	CXMLDOMElement docElem = GetDocumentElement();

	SelectionNamespaces(NAMESPACE_NCX_W);

	UniString		str;
	str.Format(USTR("//ncx:navPoint[ncx:content[@src='%s']]"), src);

	return docElem.SelectSingleNode(str);
}

/*
 * GetTitleFromURL
 * 임의의 src 와 일치하는 navPoint element 의 title 를 가져오는 함수
 * 파라미터	[in] src	찾고자 하는 navPoint 의 src
 *			[out] title	찾은 navPoint 의 title
 * 리턴값 true(has valied title) / false(title is empty)
 */
bool NCXXML::GetTitleFromURL(const unicode_t* srcURL, TString<unicode_t> &title)
{
	CXMLDOMElement docElem = GetDocumentElement();

	SelectionNamespaces(NAMESPACE_NCX_W);

	UniString		str;
//	str.Format(USTR("//ncx:navPoint[ncx:content[contains(@src,'%s')]]"), srcURL);
	str.Format(USTR("//ncx:navPoint[ncx:content[starts-with(@src,'%s')]]"), srcURL);

	CXMLDOMElement elem = docElem.SelectSingleNode(str);

	if ( elem.IsValid() )
	{
		return elem.SelectSingleNode(USTR("ncx:navLabel/ncx:text")).GetText(title);
	}

	title.Empty();
	return FALSE;
}

/*
 * Init
 * opf 파일을 기준으로 목차를 생성하는 함수
 * 뷰어에서는 목차를 따로 생성하지 않으므로 사용되지 않는다.
 * 파라미터	[in] opfXML				opf 파일을 파싱한 데이터
 *			[in] lpszOPFFolderPath	opf 파일의 절대 경로
 */
void NCXXML::Init(OPFXML &opfXML, const unicode_t* lpzOPFFolderPath)
{
	UniString	uuidString;
	UniString	title;

	opfXML.GetUniqueID(uuidString);		// epub 의 uuid 추출
	opfXML.GetDCTitle(title);			// epub 의 title 추출

	Init1(title, uuidString);			// uuid, epub title 을 이용하여 ncx 생성

	// xhtml 파일 리스트를 가져온다.(spine List)
	opfXML.SelectionNamespaces(USTR("xmlns:opf=\"http://www.idpf.org/2007/opf\""));
	CXMLDOMNodeList xhtmItemList = 
		opfXML.GetDocumentElement().SelectNodes(USTR("//opf:manifest/opf:item[@media-type='application/xhtml+xml']"));
		
	UniString		opfFolderPath(lpzOPFFolderPath);
	if ( opfFolderPath.GetLastChar() != '\\' )
		opfFolderPath += USTR("\\");

	UniString		opfFilePath;
	UniString		linear;
	UniString		idRef;
	UniString		href;

	// spineList 를 토대로 navPoint 를 생성한다.
	CXMLDOMNodeList nodeList = opfXML.GetSpineItemRefs();
	long			count = nodeList.GetLength();
	for ( long i = 0; i < count; i++ )
	{
		nodeList[i].GetAttribute(USTR("linear"), linear);	// linear = no 인 경우는 출력하지 않는 파일 이므로 navPoint 에 추가하지 않는다.
		if ( linear == USTR("no") ) continue;

		nodeList[i].GetAttribute(USTR("idref"), idRef);		
		opfXML.GetManifestItemHrefFromId(idRef, href);		// toc 의 src 를 opf 파일에서 추출
		href.Replace('/', '\\');

		bool	ok = false;
		if ( lpzOPFFolderPath )
		{
			opfFilePath = opfFolderPath;
			opfFilePath += href;

			CXMLDOMDocument2	opfXML;
			ok = opfXML.Open(opfFilePath);
			opfXML.SelectSingleNode(USTR("html/head/title")).GetText(title);
		}

		if ( ok && title.IsEmpty() )
		{
			WCHAR fname[_MAX_FNAME];
			//_wsplitpath( href.GetString(), NULL, NULL, fname, NULL ); // C4996
			_wsplitpath_s(href.GetString(), NULL, 0, NULL, 0, fname, _MAX_FNAME, NULL, 0);
			title = fname;
		}

		AddNavPoint(href, title);
		break;
	}
}



/********************************************************************************
 * Protected Method
 ********************************************************************************/
/*
 * Initialize
 * ncx 파일이 없는 경우 새로 생성하는 함수이다.
 * 뷰어에서는 ncx 를 직접 만들어 쓰는 일이 없으므로 사용되지 않는다.
 */
void NCXXML::Init1(const unicode_t* docTitleText, const unicode_t* uniqueID)
{
	if ( IsValid() ) return;

	Create();

	CXMLDOMProcessingInstruction pi;
	pi = CreateProcessingInstruction(USTR("xml"), USTR("version='1.0' encoding='UTF-8'"));
	AppendChild(pi);

	SelectionNamespaces(NAMESPACE_NCX_W);

	CXMLDOMElement ncx = CreateElement(USTR("ncx"), NAMESPACE_URI_NCX_W);
	ncx.SetAttribute(USTR("xmlns"), NAMESPACE_URI_NCX_W);
//	ncx.SetAttribute(L"xmlns:ncx", NAMESPACE_URI_NCX_W);
	ncx.SetAttribute(USTR("version"), USTR("2005-1"));
	AppendChild(ncx);

	CXMLDOMElement head = CreateElement(USTR("head"), NAMESPACE_URI_NCX_W);
	head.SetAttribute(USTR("xmlns:ncx"), NAMESPACE_URI_NCX_W);
	ncx.AppendChild(head);

	CXMLDOMElement meta;
	meta = CreateElement(USTR("meta"), NAMESPACE_URI_NCX_W);
	meta.SetAttribute(USTR("name"), USTR("dtb:uid"));
	if ( uniqueID )
	{
		meta.SetAttribute(USTR("content"), uniqueID);
	}
/*
	else
	{
		TString<char>	uuidString;

		GenerateUUID(uuidString);
		meta.SetAttribute("content", uuidString.GetString());
	}
*/
	head.AppendChild(meta);

	meta = CreateElement(USTR("meta"), NAMESPACE_URI_NCX_W);
	meta.SetAttribute(USTR("name"), USTR("dtb:depth"));
	meta.SetAttribute(USTR("content"), USTR("1"));
	head.AppendChild(meta);

	meta = CreateElement(USTR("meta"), NAMESPACE_URI_NCX_W);
	meta.SetAttribute(USTR("name"), USTR("dtb:totalPageCount"));
	meta.SetAttribute(USTR("content"), USTR("0"));
	head.AppendChild(meta);

	meta = CreateElement(USTR("meta"), NAMESPACE_URI_NCX_W);
	meta.SetAttribute(USTR("name"), USTR("dtb:maxPageNumber"));
	meta.SetAttribute(USTR("content"), USTR("0"));
	head.AppendChild(meta);

	CXMLDOMElement docTitle = CreateElement(USTR("docTitle"), NAMESPACE_URI_NCX_W);
	ncx.AppendChild(docTitle);
	if ( docTitleText )
	{
		CXMLDOMElement text = CreateElement(USTR("text"), NAMESPACE_URI_NCX_W);
		text.SetText(docTitleText);
		docTitle.AppendChild(text);
	}

	// navMap
	CXMLDOMElement navMap = CreateElement(USTR("navMap"), NAMESPACE_URI_NCX_W);
	navMap.SetAttribute(USTR("xmlns:ncx"), NAMESPACE_URI_NCX_W);
	ncx.AppendChild(navMap);
}

/*
 * AddNavPoint
 * toc 를 추가한다. 뷰어에서는 별도로 toc 를 추가할 일이 없으므로 해당 함수는 사용되지 않는다.
 * 파라미터	[in] src		toc 의 src
 *			[in] labeltext	toc 의 title
 *			[in] playOrder	toc's playOrder
 * 리턴값 추가된 navPoint element
 */
CXMLDOMElement NCXXML::AddNavPoint(const unicode_t* src, const unicode_t* labeltext, long playOrder/* = 0 */)
{
	CXMLDOMElement docElem = GetDocumentElement();

	SelectionNamespaces(NAMESPACE_NCX_W);
	CXMLDOMNode navMap = docElem.SelectSingleNode(USTR("//ncx:navMap"));

	CXMLDOMElement	navPoint;
	UniString		str;

	// 첫번째 파라미터인 src 로 navPoint Element 를 검색
	str.Format(USTR("ncx:navPoint[/ncx:content[@src='%s']]"), src);
	navPoint = navMap.SelectSingleNode(str);
	if ( !navPoint )
	{
		// 검색된 내용이 없다면, 새로운 navPoint element 를 추가한다.

		navPoint = CreateElement(USTR("navPoint"), NAMESPACE_URI_NCX_W);

		mPlayOrder++;
		str.Format(USTR("navpoint-%d"), mPlayOrder);
		navPoint.SetAttribute(USTR("id"), str);

		str.Format(USTR("%d"), mPlayOrder);
		navPoint.SetAttribute(USTR("playOrder"), str);
		navMap.AppendChild(navPoint);

		CXMLDOMElement navLabel = CreateElement(USTR("navLabel"), NAMESPACE_URI_NCX_W);
		navPoint.AppendChild(navLabel);

		CXMLDOMElement text = CreateElement(USTR("text"), NAMESPACE_URI_NCX_W);
		text.SetText(labeltext);
		navLabel.AppendChild(text);

		CXMLDOMElement content = CreateElement(USTR("content"), NAMESPACE_URI_NCX_W);
		content.SetAttribute(USTR("src"), src);
		navPoint.AppendChild(content);
	}
	else 
	{
		// 검색된 내용이 있다면 검색된 navPoint Element 의 title 과 playOrder 를 새로 전달받은 파라미터의 값으로 변경한다.
		if ( labeltext )
		{
			CXMLDOMElement text = navPoint.SelectSingleNode(USTR("ncx:navLabel/ncx:text"));
			text.SetText(labeltext);
		}

		if ( playOrder > 0 )
		{
			str.Format(USTR("navpoint-%d"), playOrder);
			navPoint.SetAttribute(USTR("id"), str);

			str.Format(USTR("%d"), playOrder);
			navPoint.SetAttribute(USTR("playOrder"), str);

		}
	}

	return navPoint;
}


/*
CXMLDOMElement NCXXML::GetNavPointByPlayOrder(int playOrder)
{
	CXMLDOMElement docElem = GetDocumentElement();

	SelectionNamespaces(NAMESPACE_NCX_W);

	UniString		str;
	str.Format(L"//ncx:navMap/ncx:navPoint[@playOrder='%d']", playOrder);

	return docElem.SelectSingleNode(str);
}

CXMLDOMElement NCXXML::GetNavPointByID(const unicode_t* id)
{
	CXMLDOMElement docElem = GetDocumentElement();

	SelectionNamespaces(NAMESPACE_NCX_W);

	UniString		str;
	str.Format(L"//ncx:navMap/ncx:navPoint[@id='%s']", id);

	return docElem.SelectSingleNode(str);
}
*/
