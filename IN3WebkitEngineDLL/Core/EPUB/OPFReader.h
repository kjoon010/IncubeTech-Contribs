#pragma once

#include "XMLDom.h"
#include "TStringEx.h"

/*
 구조 :
 package
	- metadata
	    - dc
		- meta
	- manifest
		- item
	- spine
		- itemref
*/



/*
 * Namespace 정의
 */
#define NAMESPACE_URI_OPF_W			USTR("http://www.idpf.org/2007/opf")
#define NAMESPACE_OPF_W				USTR("xmlns:opf=\"http://www.idpf.org/2007/opf\"")

#define NAMESPACE_URI_DC_W			USTR("http://purl.org/dc/elements/1.1/")
#define NAMESPACE_DC_W				USTR("xmlns:dc=\"http://purl.org/dc/elements/1.1/\"")


////////////////////////////////////////////////////////////////////////////////////////////
// ManifestItem

class ManifestItem : public CXMLDOMElement
{
public:
	ManifestItem();
	ManifestItem(CXMLDOMNode &node);
	ManifestItem(CXMLDOMElement &elem);
	~ManifestItem();

	bool GetId(TString<unicode_t> &id);
	bool GetHref(TString<unicode_t> &href);
	bool GetMediaType(TString<unicode_t> &mediaType);
	bool GetFallback(TString<unicode_t> &fallback);
	bool GetProperties(TString<unicode_t> &properties);
	bool GetMediaOverlay(TString<unicode_t> &mediaOverlay);
};


////////////////////////////////////////////////////////////////////////////////////////////
// Meta

class Meta : public CXMLDOMElement
{
public:
	Meta();
	Meta(CXMLDOMNode &node);
	Meta(CXMLDOMElement &elem);
	~Meta();
	
	bool GetAbout(TString<unicode_t> &about);
	bool GetDatatype(TString<unicode_t> &datatype);
	bool GetProperty(TString<unicode_t> &oProperty);
	bool GetId(TString<unicode_t> &id);

	// epub 2.0
	bool GetName(TString<unicode_t> &name);
	bool GetContent(TString<unicode_t> &content);

	// epub 3.0
	bool GetRefines(TString<unicode_t> &refines);
	bool GetScheme(TString<unicode_t> &scheme);
};


////////////////////////////////////////////////////////////////////////////////////////////
// DCMeta

class DCMeta : public CXMLDOMElement
{
public:
	DCMeta();
	DCMeta(CXMLDOMNode &node);
	DCMeta(CXMLDOMElement &elem);
	~DCMeta();

	bool GetId(TString<unicode_t> &id);
	bool GetPrefer(TString<unicode_t> &prefer);	
	bool role(TString<unicode_t> &role);
};


////////////////////////////////////////////////////////////////////////////////////////////
// SpineItemRef

class SpineItemRef : public CXMLDOMElement
{
public:
	SpineItemRef();
	SpineItemRef(CXMLDOMNode &node);
	SpineItemRef(CXMLDOMElement &elem);
	~SpineItemRef();

	bool GetIdRef(TString<unicode_t> &idRef);
	bool GetLinear(TString<unicode_t> &linear);
	bool GetId(TString<unicode_t> &id);
	bool GetProperties(TString<unicode_t> &properties);
};


////////////////////////////////////////////////////////////////////////////////////////////
// OPFXML

class OPFXML : public CXMLDOMDocument2
{
public:
	OPFXML(void);
	~OPFXML(void);

	// from package attribute
	bool GetVersion(TString<unicode_t> &version);
	bool GetProfile(TString<unicode_t> &profile);
	bool GetPrefix(TString<unicode_t> &prefix);

	// package Unique Identifier value
	bool GetUniqueID(TString<unicode_t> &uuidString);
	bool SetUniqueID(const unicode_t *uuidString, const unicode_t *scheme);
	
	// metadata
	CXMLDOMElement GetMetadata() const;

	// metadata/dc
	CXMLDOMElement GetDCMetadata(const unicode_t *tagName);
	CXMLDOMNodeList GetDCMetadataList(const unicode_t *tagName);
	bool GetDCIdentifier(TString<unicode_t> &identifier);
	bool GetDCLanguage(TString<unicode_t> &language);
	bool GetDCTitle(TString<unicode_t> &title);

	// metadata/meta
	Meta GetMetaByProperty(const unicode_t *propertyAttrValue);
	Meta GetMetaByAbout(const unicode_t *aboutAttrValue);
	Meta GetMetaById(const unicode_t *idAttrValue);
	Meta GetMetaByName(const unicode_t *nameAttrValue);
	Meta GetMetaByAttr(const unicode_t *attrName, const unicode_t *attrValue);

	// Manifest/item
	ManifestItem GetManifestItemById(const unicode_t* idAttrValue);
	ManifestItem GetManifestItemByHref(const unicode_t* hrefAttrValue);
	ManifestItem GetManifestItemByProperties(const unicode_t* propertyAttrValue);
	ManifestItem GetManifestItemByAttr(const unicode_t *attrName, const unicode_t *attrValue);

	bool GetManifestItemHrefFromId(const unicode_t* id, TString<unicode_t> &href);
	bool GetManifestItemIdFromHref(const unicode_t* href, TString<unicode_t> &id);

	// spine
	CXMLDOMNodeList GetSpineItemRefs(bool all = false);

	// etc
	typedef enum NavType { NONE, NCX, NAV };
	bool GetNCXFileURL(TString<unicode_t> &oNCXFileURL);
	bool GetNAVFileURL(TString<unicode_t> &oNAVFileURL);
	NavType GetNavigationURL(TString<unicode_t> &oURL);

	int GetCoverPageImage(TString<unicode_t> &href);
	int GetThumbnailImage(TString<unicode_t> &href);

};
