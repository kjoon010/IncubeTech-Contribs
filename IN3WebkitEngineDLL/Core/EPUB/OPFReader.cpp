#include "StdAfx.h"
#include "OPFReader.h"

#if defined(WIN32)
#define USTR(str)					L##str
#else
#define USTR(str)					str
#endif

#define WCSICMP(a,b)	_wcsicmp(a,b)


////////////////////////////////////////////////////////////////////////////////////////////
// ManifestItem

ManifestItem::ManifestItem() : CXMLDOMElement()
{
}

ManifestItem::ManifestItem(CXMLDOMNode &node) : CXMLDOMElement(node)
{
}

ManifestItem::ManifestItem(CXMLDOMElement &elem) : CXMLDOMElement(elem)
{
}

ManifestItem::~ManifestItem()
{
}


bool ManifestItem::GetId(TString<unicode_t> &id)
{
	GetAttribute(USTR("id"), id);
	return !id.IsEmpty();
}

bool ManifestItem::GetHref(TString<unicode_t> &href)
{
	GetAttribute(USTR("href"), href);
	return !href.IsEmpty();
}

bool ManifestItem::GetMediaType(TString<unicode_t> &mediaType)
{
	GetAttribute(USTR("media-type"), mediaType);
	return !mediaType.IsEmpty();
}

bool ManifestItem::GetFallback(TString<unicode_t> &fallback)
{
	GetAttribute(USTR("fallback"), fallback);
	return !fallback.IsEmpty();
}

bool ManifestItem::GetProperties(TString<unicode_t> &properties)
{
	GetAttribute(USTR("properties"), properties);
	return !properties.IsEmpty();
}

bool ManifestItem::GetMediaOverlay(TString<unicode_t> &mediaOverlay)
{
	GetAttribute(USTR("media-overlay"), mediaOverlay);
	return !mediaOverlay.IsEmpty();
}


////////////////////////////////////////////////////////////////////////////////////////////
// Meta

Meta::Meta() : CXMLDOMElement()
{
}

Meta::Meta(CXMLDOMNode &node) : CXMLDOMElement(node)
{
}

Meta::Meta(CXMLDOMElement &elem) : CXMLDOMElement(elem)
{
}

Meta::~Meta()
{
}

bool Meta::GetAbout(TString<unicode_t> &about)
{
	GetAttribute(USTR("about"), about);
	return !about.IsEmpty();
}

bool Meta::GetDatatype(TString<unicode_t> &datatype)
{
	GetAttribute(USTR("datatype"), datatype);
	return !datatype.IsEmpty();
}

bool Meta::GetProperty(TString<unicode_t> &oProperty)
{
	GetAttribute(USTR("property"), oProperty);
	return !oProperty.IsEmpty();
}

bool Meta::GetId(TString<unicode_t> &id)
{
	GetAttribute(USTR("id"), id);
	return !id.IsEmpty();
}

bool Meta::GetName(TString<unicode_t> &name)
{
	GetAttribute(USTR("name"), name);
	return !name.IsEmpty();
}

bool Meta::GetContent(TString<unicode_t> &content)
{
	GetAttribute(USTR("content"), content);
	return !content.IsEmpty();
}

bool Meta::GetRefines(TString<unicode_t> &refines)
{
	GetAttribute(USTR("refines"), refines);
	return !refines.IsEmpty();
}

bool Meta::GetScheme(TString<unicode_t> &scheme)
{
	GetAttribute(USTR("scheme"), scheme);
	return !scheme.IsEmpty();
}


////////////////////////////////////////////////////////////////////////////////////////////
// DCMeta

DCMeta::DCMeta() : CXMLDOMElement()
{
}

DCMeta::DCMeta(CXMLDOMNode &node) : CXMLDOMElement(node)
{
}

DCMeta::DCMeta(CXMLDOMElement &elem) : CXMLDOMElement(elem)
{
}

DCMeta::~DCMeta()
{
}

bool DCMeta::GetId(TString<unicode_t> &id)
{
	GetAttribute(USTR("id"), id);
	return !id.IsEmpty();
}

bool DCMeta::GetPrefer(TString<unicode_t> &prefer)
{
	GetAttribute(USTR("prefer"), prefer);
	return !prefer.IsEmpty();
}

bool DCMeta::role(TString<unicode_t> &role)
{
	GetAttribute(USTR("role"), role);
	return !role.IsEmpty();
}


////////////////////////////////////////////////////////////////////////////////////////////
// SpineItemRef

SpineItemRef::SpineItemRef() : CXMLDOMElement()
{
}

SpineItemRef::SpineItemRef(CXMLDOMNode &node) : CXMLDOMElement(node)
{
}

SpineItemRef::SpineItemRef(CXMLDOMElement &elem) : CXMLDOMElement(elem)
{
}

SpineItemRef::~SpineItemRef()
{
}

bool SpineItemRef::GetIdRef(TString<unicode_t> &idRef)
{
	GetAttribute(USTR("idref"), idRef);
	return !idRef.IsEmpty();
}

bool SpineItemRef::GetLinear(TString<unicode_t> &linear)
{
	GetAttribute(USTR("linear"), linear);
	return !linear.IsEmpty();
}

bool SpineItemRef::GetId(TString<unicode_t> &id)
{
	GetAttribute(USTR("id"), id);
	return !id.IsEmpty();
}

bool SpineItemRef::GetProperties(TString<unicode_t> &properties)
{
	GetAttribute(USTR("properties"), properties);
	return !properties.IsEmpty();
}


////////////////////////////////////////////////////////////////////////////////////////////
// OPFXML

OPFXML::OPFXML(void)
{
}

OPFXML::~OPFXML(void)
{
}


bool OPFXML::GetVersion(TString<unicode_t> &version)
{
	CXMLDOMElement docElem = GetDocumentElement();
	if ( !docElem ) return false;

	docElem.GetAttribute(USTR("version"), version);
	return !version.IsEmpty();
}

bool OPFXML::GetProfile(TString<unicode_t> &profile)
{
	CXMLDOMElement docElem = GetDocumentElement();
	if ( !docElem ) return false;

	docElem.GetAttribute(USTR("profile "), profile);
	return !profile.IsEmpty();
}

bool OPFXML::GetPrefix(TString<unicode_t> &prefix)
{
	CXMLDOMElement docElem = GetDocumentElement();
	if ( !docElem ) return false;

	docElem.GetAttribute(USTR("prefix"), prefix);
	return !prefix.IsEmpty();
}

bool OPFXML::GetUniqueID(TString<unicode_t> &uuidString)
{
	CXMLDOMElement docElem = GetDocumentElement();
	if ( !docElem ) return false;

	TString<unicode_t>	uniqueIdentifier;
	docElem.GetAttribute(USTR("unique-identifier"), uniqueIdentifier);

	CXMLDOMElement metadata = GetMetadata();

	UniString	str;
	str.Format(USTR("./dc:identifier[@id='%s']"), uniqueIdentifier.GetString());

	SelectionNamespaces(USTR("xmlns:opf=\"http://www.idpf.org/2007/opf\" xmlns:dc=\"http://purl.org/dc/elements/1.1/\""));

	CXMLDOMElement identifier = metadata.SelectSingleNode(str);
	identifier.GetText(uuidString);
//	identifier.GetAttribute(USTR("opf:scheme"), scheme);

	return !uuidString.IsEmpty();
}

bool OPFXML::SetUniqueID(const unicode_t *uuidString, const unicode_t *scheme)
{
	CXMLDOMElement docElem = GetDocumentElement();
	if ( !docElem ) return false;

	TString<unicode_t>	uniqueIdentifier;
	docElem.GetAttribute(USTR("unique-identifier"), uniqueIdentifier);

//	SelectionNamespaces(NAMESPACE_OPF_W);
	CXMLDOMElement metadata = GetMetadata();

	UniString	str;
	str.Format(USTR("./dc:identifier[@id='%s']"), uniqueIdentifier.GetString());

//	SelectionNamespaces(NAMESPACE_DC_W);
	SelectionNamespaces(USTR("xmlns:opf=\"http://www.idpf.org/2007/opf\" xmlns:dc=\"http://purl.org/dc/elements/1.1/\""));
	CXMLDOMElement identifier(metadata.SelectSingleNode(str) );
	if ( !identifier )
	{
		identifier = CreateElement(USTR("dc:identifier"), NAMESPACE_URI_DC_W);
		identifier.SetAttribute(USTR("id"), uniqueIdentifier.GetString());
		metadata.AppendChild(identifier);
	}
	identifier.SetText(uuidString);

	if ( scheme )
	{
	#if 0
		if ( identifier.GetAttributes().GetNamedItem(USTR("opf:scheme")).GetSpecified() )
			identifier.SetAttribute(USTR("opf:scheme"), scheme);
	#else
		metadata.SetAttribute(USTR("xmlns:opf"), NAMESPACE_URI_OPF_W);
		identifier.SetAttribute(USTR("opf:scheme"), scheme);
	#endif
	}

	return identifier.IsValid();
}

CXMLDOMElement OPFXML::GetMetadata() const
{
	CXMLDOMElement docElem = GetDocumentElement();

	SelectionNamespaces(NAMESPACE_OPF_W);
	CXMLDOMElement metadata = docElem.SelectSingleNode("//opf:metadata/opf:dc-metadata");
	if ( !metadata )
		metadata = docElem.SelectSingleNode("//opf:metadata");

	return metadata;
}

CXMLDOMElement OPFXML::GetDCMetadata(const unicode_t *tagName)
{
	SelectionNamespaces(USTR("xmlns:opf=\"http://www.idpf.org/2007/opf\" xmlns:dc=\"http://purl.org/dc/elements/1.1/\""));

	CXMLDOMElement docElem = GetDocumentElement();
	CXMLDOMElement metadata = docElem.SelectSingleNode(USTR("opf:metadata/opf:dc-metadata"));
	if ( !metadata )
		metadata = docElem.SelectSingleNode(USTR("//opf:metadata"));

	return metadata.SelectSingleNode(tagName);
}

CXMLDOMNodeList OPFXML::GetDCMetadataList(const unicode_t *tagName)
{
	SelectionNamespaces(USTR("xmlns:opf=\"http://www.idpf.org/2007/opf\" xmlns:dc=\"http://purl.org/dc/elements/1.1/\""));

	CXMLDOMElement docElem = GetDocumentElement();
	CXMLDOMElement metadata = docElem.SelectSingleNode(USTR("opf:metadata/opf:dc-metadata"));
	if ( !metadata )
		metadata = docElem.SelectSingleNode(USTR("//opf:metadata/"));
//	CXMLDOMElement	metadata = docElem.SelectSingleNode(USTR("//opf:metadata"));

	return metadata.SelectNodes(tagName);
}

bool OPFXML::GetDCIdentifier(TString<unicode_t> &identifier)
{
	GetDCMetadata(USTR("./dc:identifier")).GetText(identifier);
	return !identifier.IsEmpty();
}

bool OPFXML::GetDCLanguage(TString<unicode_t> &language)
{
	GetDCMetadata(USTR("./dc:language")).GetText(language);
	return !language.IsEmpty();
}

bool OPFXML::GetDCTitle(TString<unicode_t> &title)
{
#if 1
	GetDCMetadata(USTR("./dc:title")).GetText(title);
	return !title.IsEmpty();
#else	
	SelectionNamespaces(USTR("xmlns:opf=\"http://www.idpf.org/2007/opf\" xmlns:dc=\"http://purl.org/dc/elements/1.1/\""));

	CXMLDOMElement docElem = GetDocumentElement();
	CXMLDOMElement metadata = docElem.SelectSingleNode(USTR("opf:metadata/opf:dc-metadata"));
	if ( !metadata )
		metadata = docElem.SelectSingleNode(USTR("//opf:metadata/"));

	CXMLDOMNodeList titles = metadata.SelectNodes(USTR("./dc:title"));
	if (titles.GetCount() == 1 )
	{
		title = titles.GetItem(0).GetText();
		return !title.IsEmpty();
	}
	else
	{
		// @ note
		// 생각해보자...;;;
	}
#endif
}

Meta OPFXML::GetMetaByProperty(const unicode_t *propertyAttrValue)
{
	return GetMetaByAttr(USTR("property"), propertyAttrValue);
}

Meta OPFXML::GetMetaByAbout(const unicode_t *aboutAttrValue)
{
	return GetMetaByAttr(USTR("about"), aboutAttrValue);
}

Meta OPFXML::GetMetaByName(const unicode_t *nameValue)
{
	return GetMetaByAttr(USTR("name"), nameValue);
}

Meta OPFXML::GetMetaById(const unicode_t *idAttrValue)
{
	return GetMetaByAttr(USTR("id"), idAttrValue);
}

Meta OPFXML::GetMetaByAttr(const unicode_t *attrName, const unicode_t *attrValue)
{
	SelectionNamespaces(USTR("xmlns:opf=\"http://www.idpf.org/2007/opf\" xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:dcterms=\"http://purl.org/dc/elements/1.1/\""));

	CXMLDOMElement docElem = GetDocumentElement();
	CXMLDOMElement metadata = docElem.SelectSingleNode(USTR("//opf:metadata"));

	UniString	str;
	str.Format(USTR("./opf:meta[@%s='%s']"), attrName, attrValue);

	return metadata.SelectSingleNode(str);
}

ManifestItem OPFXML::GetManifestItemByAttr(const unicode_t *attrName, const unicode_t *attrValue)
{
	SelectionNamespaces(NAMESPACE_OPF_W);

	UniString	str;
	UniString	strAttrValue(attrValue);
	if ( !STRING_FOUND( strAttrValue.FindAt('\'') ) )
		str.Format(USTR("//opf:manifest/opf:item[@%s='%s']"), attrName, attrValue);
	else
		str.Format(USTR("//opf:manifest/opf:item[@%s=\"%s\"]"), attrName, attrValue);

	CXMLDOMElement docElem = GetDocumentElement();
	return docElem.SelectSingleNode(str);
}

ManifestItem OPFXML::GetManifestItemById(const unicode_t* idAttrValue)
{
	return GetManifestItemByAttr(USTR("id"), idAttrValue);
}

ManifestItem OPFXML::GetManifestItemByHref(const unicode_t* hrefAttrValue)
{
	return GetManifestItemByAttr(USTR("href"), hrefAttrValue);
}


ManifestItem OPFXML::GetManifestItemByProperties(const unicode_t* propertyAttrValue)
{
	return GetManifestItemByAttr(USTR("properties"), propertyAttrValue);
}

bool OPFXML::GetManifestItemIdFromHref(const unicode_t* href, TString<unicode_t> &id)
{
	return GetManifestItemByHref(href).GetId(id);
}

bool OPFXML::GetManifestItemHrefFromId(const unicode_t* id, TString<unicode_t> &href)
{
	return GetManifestItemById(id).GetHref(href);
}

CXMLDOMNodeList OPFXML::GetSpineItemRefs(bool all)
{
	SelectionNamespaces(NAMESPACE_OPF_W);
	CXMLDOMElement docElem = GetDocumentElement();

	if ( all )
		return docElem.SelectNodes(USTR("//opf:spine/opf:itemref"));

	return docElem.SelectNodes(USTR("//opf:spine/opf:itemref[not(contains(@linear,'no'))]"));
//	return docElem.SelectNodes(USTR("//opf:spine/opf:itemref[@linear != 'no']"));
//	return docElem.SelectNodes(USTR("//opf:spine/opf:itemref"));
}

bool OPFXML::GetNCXFileURL(TString<unicode_t> &oNCXFileURL)
{
	SelectionNamespaces(NAMESPACE_OPF_W);

	CXMLDOMElement docElem = GetDocumentElement();
	CXMLDOMElement spineElem = docElem.SelectSingleNode(USTR("//opf:spine"));

	UniString	tocValue;
	if ( spineElem.GetAttribute(USTR("toc"), tocValue) )
	{
		UniString	str;
		str.Format(USTR("//opf:manifest/opf:item[@id='%s']"), tocValue.GetString());
		CXMLDOMElement item = docElem.SelectSingleNode(str);

		item.GetAttribute(USTR("href"), oNCXFileURL);
	}
	else
	{
		oNCXFileURL.Empty();
	}

	return !oNCXFileURL.IsEmpty();
}

bool OPFXML::GetNAVFileURL(TString<unicode_t> &oNAVFileURL)
{
	ManifestItem item = GetManifestItemByProperties(USTR("nav"));
	return item.GetHref(oNAVFileURL);
}

OPFXML::NavType OPFXML::GetNavigationURL(TString<unicode_t> &oURL)
{
	if ( GetNAVFileURL(oURL) )
	{
		return NCX;
	}
	else if ( GetNCXFileURL(oURL) )
	{
		return NAV;
	}

	return NONE;
}

int OPFXML::GetCoverPageImage(TString<unicode_t> &href)
{
	CXMLDOMElement docElem = GetDocumentElement();

	SelectionNamespaces(NAMESPACE_OPF_W);
	CXMLDOMElement metadata = docElem.SelectSingleNode(USTR("//opf:metadata"));

	UniString	content;
	UniString	mediaType;
	if ( metadata.SelectSingleNode(USTR("opf:meta[@name='cover']")).GetAttribute(USTR("content"), content) )
	{
		UniString	str;
		str.Format(USTR("//opf:manifest/opf:item[@id='%s']"), content.GetString());
		CXMLDOMElement item = docElem.SelectSingleNode(str);

		item.GetAttribute(USTR("media-type"), mediaType);
		if ( mediaType.CompareN(0, USTR("image") , 5) != 0 ) 
		{
		/*
			LogMessage("잘못된 Cover Image 정보 : ");
			UniString	url;
			GetURL(url);
			LogMessageLn(url);
		*/
			href.Empty();
			return -1;
		}

		item.GetAttribute(USTR("href"), href);
	}
	else
	{
	/*
		LogMessage("Cover Image 정보 없음 : ");
		UniString	url;
		GetURL(url);
		LogMessageLn(url);
	*/
		href.Empty();
		return -2;
	}

	return 1;
}


int OPFXML::GetThumbnailImage(TString<unicode_t> &href)
{
	SelectionNamespaces(NAMESPACE_OPF_W);

	CXMLDOMElement docElem = GetDocumentElement();
	CXMLDOMElement metadata = docElem.SelectSingleNode(USTR("//opf:metadata"));

	UniString	content;
	UniString	mediaType;
	if ( metadata.SelectSingleNode(USTR("opf:meta[@name='thumbnail']")).GetAttribute(USTR("content"), content) )
	{
		UniString	str;
		str.Format(USTR("//opf:manifest/opf:item[@id='%s']"), content.GetString());
		CXMLDOMElement item = docElem.SelectSingleNode(str);

		item.GetAttribute(USTR("media-type"), mediaType);
		if ( mediaType.CompareN(0, USTR("image"), 5) != 0 ) 
		{
		/*
			LogMessage("잘못된 Cover Image 정보 : ");
			UniString	url;
			GetURL(url);
			LogMessageLn(url);
		*/
			href.Empty();
			return -1;
		}

		item.GetAttribute(USTR("href"), href);
	}
	else
	{
	/*
		LogMessage("Cover Image 정보 없음 : ");
		UniString	url;
		GetURL(url);
		LogMessageLn(url);
	*/
		href.Empty();
		return -2;
	}

	return 0;
}
