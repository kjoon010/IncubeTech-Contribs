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
 * ncx ���� ����Ǿ� �ִ� Unique ID �� �����´�.
 * �Ķ����	[out] uuidString	uuid
 * ���ϰ�	true(uuid is valid)/false(uuid is empty)
 */
bool NCXXML::GetUniqueID(TString<unicode_t> &uuidString)
{
	CXMLDOMElement docElem = GetDocumentElement();

	SelectionNamespaces(NAMESPACE_NCX_W);
	CXMLDOMElement elem = docElem.SelectSingleNode(USTR("//ncx:head/ncx:meta[@name='dtb:uid']"));		// <head>�� ���� ����� <meta> ��� ��, name='dtb:uid' �� ���� attirbute �� ���� ��� �˻�
	elem.GetAttribute(USTR("content"), uuidString);

	return !uuidString.IsEmpty();
}

/*
 * GetNavPointList
 * navPoint element List �� ��ȯ�Ѵ�.
 * ���ϰ�	navPoint element List
 */
CXMLDOMNodeList NCXXML::GetNavPointList()
{		
	CXMLDOMElement docElem = GetDocumentElement();

	SelectionNamespaces(NAMESPACE_NCX_W);
	return docElem.SelectNodes(USTR("//ncx:navMap/ncx:navPoint"));
}

/*
 * FindNavPoint
 * ������ src �� ��ġ�ϴ� �����͸� ���� navPoint �˻�
 * �Ķ����	[in] src	ã���� �ϴ� navPoint �� src
 * ���ϰ�	�˻��� navPoint
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
 * ������ src �� ��ġ�ϴ� navPoint element �� title �� �������� �Լ�
 * �Ķ����	[in] src	ã���� �ϴ� navPoint �� src
 *			[out] title	ã�� navPoint �� title
 * ���ϰ� true(has valied title) / false(title is empty)
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
 * opf ������ �������� ������ �����ϴ� �Լ�
 * ������ ������ ���� �������� �����Ƿ� ������ �ʴ´�.
 * �Ķ����	[in] opfXML				opf ������ �Ľ��� ������
 *			[in] lpszOPFFolderPath	opf ������ ���� ���
 */
void NCXXML::Init(OPFXML &opfXML, const unicode_t* lpzOPFFolderPath)
{
	UniString	uuidString;
	UniString	title;

	opfXML.GetUniqueID(uuidString);		// epub �� uuid ����
	opfXML.GetDCTitle(title);			// epub �� title ����

	Init1(title, uuidString);			// uuid, epub title �� �̿��Ͽ� ncx ����

	// xhtml ���� ����Ʈ�� �����´�.(spine List)
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

	// spineList �� ���� navPoint �� �����Ѵ�.
	CXMLDOMNodeList nodeList = opfXML.GetSpineItemRefs();
	long			count = nodeList.GetLength();
	for ( long i = 0; i < count; i++ )
	{
		nodeList[i].GetAttribute(USTR("linear"), linear);	// linear = no �� ���� ������� �ʴ� ���� �̹Ƿ� navPoint �� �߰����� �ʴ´�.
		if ( linear == USTR("no") ) continue;

		nodeList[i].GetAttribute(USTR("idref"), idRef);		
		opfXML.GetManifestItemHrefFromId(idRef, href);		// toc �� src �� opf ���Ͽ��� ����
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
 * ncx ������ ���� ��� ���� �����ϴ� �Լ��̴�.
 * ������ ncx �� ���� ����� ���� ���� �����Ƿ� ������ �ʴ´�.
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
 * toc �� �߰��Ѵ�. ������ ������ toc �� �߰��� ���� �����Ƿ� �ش� �Լ��� ������ �ʴ´�.
 * �Ķ����	[in] src		toc �� src
 *			[in] labeltext	toc �� title
 *			[in] playOrder	toc's playOrder
 * ���ϰ� �߰��� navPoint element
 */
CXMLDOMElement NCXXML::AddNavPoint(const unicode_t* src, const unicode_t* labeltext, long playOrder/* = 0 */)
{
	CXMLDOMElement docElem = GetDocumentElement();

	SelectionNamespaces(NAMESPACE_NCX_W);
	CXMLDOMNode navMap = docElem.SelectSingleNode(USTR("//ncx:navMap"));

	CXMLDOMElement	navPoint;
	UniString		str;

	// ù��° �Ķ������ src �� navPoint Element �� �˻�
	str.Format(USTR("ncx:navPoint[/ncx:content[@src='%s']]"), src);
	navPoint = navMap.SelectSingleNode(str);
	if ( !navPoint )
	{
		// �˻��� ������ ���ٸ�, ���ο� navPoint element �� �߰��Ѵ�.

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
		// �˻��� ������ �ִٸ� �˻��� navPoint Element �� title �� playOrder �� ���� ���޹��� �Ķ������ ������ �����Ѵ�.
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
