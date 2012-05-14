/* ********************************************************************************
 *
 * Purpose:	interface for the XMLDom class
 *
 * Version	: v1.0
 * Author	: IKHAN CHO
 * Copyright :(c) 2004 IKHAN CHO. All rights reserved.
 *
 * Modification History (most recent first):

 *			 7 DEC 2010		IKHAN CHO,  revision
 *			 8 JAN 2004		IKHAN CHO,  first design for Windows
 *
 * Known problem:
 *
 * Note :
 * ******************************************************************************* */

#ifndef __XML_DOM__
#define __XML_DOM__


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "InterfaceWrapper.h"
#include "TString.h"



#define MSXML_VERSION_UNKNOWN	0x0
#define MSXML_VERSION_10		0x10
#define MSXML_VERSION_26		0x26
#define MSXML_VERSION_30		0x30
#define MSXML_VERSION_40		0x40
#define MSXML_VERSION_50		0x50
#define MSXML_VERSION_60		0x60


#define XML_DOMDOCUMENT60_PROGID	L"Msxml2.DOMDocument.6.0"
#define XMLHTTP60_PROGID			L"Msxml2.XMLHTTP.6.0"
#define SERVER_XMLHTTP60_PROGID		L"Msxml2.ServerXMLHTTP.6.0"

#define XML_DOMDOCUMENT50_PROGID	L"Msxml2.DOMDocument.5.0"
#define XMLHTTP50_PROGID			L"Msxml2.XMLHTTP.5.0"
#define SERVER_XMLHTTP50_PROGID		L"Msxml2.ServerXMLHTTP.5.0"

#define XML_DOMDOCUMENT40_PROGID	L"Msxml2.DOMDocument.4.0"
#define XMLHTTP40_PROGID			L"Msxml2.XMLHTTP.4.0"
#define SERVER_XMLHTTP40_PROGID		L"Msxml2.ServerXMLHTTP.4.0"

#define XML_DOMDOCUMENT30_PROGID	L"Msxml2.DOMDocument.3.0"
#define XMLHTTP30_PROGID			L"Msxml2.XMLHTTP.3.0"
#define SERVER_XMLHTTP30_PROGID		L"Msxml2.ServerXMLHTTP.3.0"

#define XML_DOMDOCUMENT26_PROGID	L"Msxml2.DOMDocument.2.6"
#define XMLHTTP26_PROGID			L"Msxml2.XMLHTTP.2.6"
#define SERVER_XMLHTTP26_PROGID		L"Msxml2.ServerXMLHTTP.2.6"

#define XML_DOMDOCUMENT_PROGID		L"Msxml2.DOMDocument"
#define XMLHTTP_PROGID				L"Msxml2.XMLHTTP"
#define SERVER_XMLHTTP_PROGID		L"Msxml2.ServerXMLHTTP"




#ifdef _WIN32_WCE
#define MSXML_VERSION			MSXML_VERSION_30
#else
#define MSXML_VERSION			MSXML_VERSION_40
#endif



#define SUPPORT_CASTING_OP		0
#define ALT_CSTRING				0

#if ALT_CSTRING
#ifdef _WIN32_WCE
#include <afx.h>
#include <afxstr.h>
//#include <atlstr.h>
#else
#include <atlstr.h>
#endif
#endif

#ifdef __ATLSTR_H__
#define SUPPORT_CSTRING			1	// may 0 or 1
#else
#define SUPPORT_CSTRING			0	// must 0
#endif



#if 0

#if MSXML_VERSION == MSXML_VERSION_60
#import <msxml6.dll> raw_interfaces_only 
#elif MSXML_VERSION == MSXML_VERSION_50
#import <msxml5.dll> raw_interfaces_only 
#else
#import <msxml4.dll> raw_interfaces_only 
#endif
//#include <comutil.h>

using namespace MSXML2;

#define MSXML_IXMLDOMNode					MSXML2::IXMLDOMNode
#define	MSXML_IXMLDOMNodeList				MSXML2::IXMLDOMNodeList
#define MSXML_IXMLDOMDocumentFragment		MSXML2::IXMLDOMDocumentFragment
#define MSXML_IXMLDOMCharacterData			MSXML2::IXMLDOMCharacterData
#define MSXML_IXMLDOMComment				MSXML2::IXMLDOMComment
#define MSXML_IXMLDOMText					MSXML2::IXMLDOMText
#define MSXML_IXMLDOMCDATASection			MSXML2::IXMLDOMCDATASection
#define MSXML_IXMLDOMDocument				MSXML2::IXMLDOMDocument
#define MSXML_IXMLDOMDocument2				MSXML2::IXMLDOMDocument2
#define MSXML_IXMLDOMSchemaCollection		MSXML2::IXMLDOMSchemaCollection
#define MSXML_IXMLDOMSchemaCollection2		MSXML2::IXMLDOMSchemaCollection2
#define MSXML_IXMLDOMNamedNodeMap			MSXML2::IXMLDOMNamedNodeMap
#define MSXML_IXMLDOMAttribute				MSXML2::IXMLDOMAttribute
#define MSXML_IXMLDOMElement				MSXML2::IXMLDOMElement
#define MSXML_IXMLDOMDocumentType			MSXML2::IXMLDOMDocumentType
#define MSXML_IXMLDOMProcessingInstruction	MSXML2::IXMLDOMProcessingInstruction
#define MSXML_IXMLDOMEntityReference		MSXML2::IXMLDOMEntityReference
#define MSXML_IXMLDOMEntity					MSXML2::IXMLDOMEntity
#define MSXML_IXMLDOMParseError				MSXML2::IXMLDOMParseError
#define MSXML_IXMLDOMNotation				MSXML2::IXMLDOMNotation
#define MSXML_IXMLDOMImplementation			MSXML2::IXMLDOMImplementation
#define MSXML_IXSLProcessor					MSXML2::IXSLProcessor
#define MSXML_IXMLDOMSelection				MSXML2::IXMLDOMSelection
#define MSXML_ISchemaItem					MSXML2::ISchemaItem
#define MSXML_IXSLTemplate					MSXML2::IXSLTemplate

#define MSXML_DOMNodeType					MSXML2::DOMNodeType
#define MSXML_NODE_INVALID					MSXML2::NODE_INVALID
#define MSXML_NODE_ELEMENT					MSXML2::NODE_ELEMENT
#define MSXML_NODE_ATTRIBUTE				MSXML2::NODE_ATTRIBUTE
#define MSXML_NODE_TEXT						MSXML2::NODE_TEXT
#define MSXML_NODE_CDATA_SECTION			MSXML2::NODE_CDATA_SECTION
#define MSXML_NODE_ENTITY_REFERENCE			MSXML2::NODE_ENTITY_REFERENCE
#define MSXML_NODE_ENTITY					MSXML2::NODE_ENTITY
#define MSXML_NODE_PROCESSING_INSTRUCTION	MSXML2::NODE_PROCESSING_INSTRUCTION
#define MSXML_NODE_COMMENT					MSXML2::NODE_COMMENT
#define MSXML_NODE_DOCUMENT					MSXML2::NODE_DOCUMENT
#define MSXML_NODE_DOCUMENT_TYPE			MSXML2::NODE_DOCUMENT_TYPE
#define MSXML_NODE_DOCUMENT_FRAGMENT		MSXML2::NODE_DOCUMENT_FRAGMENT
#define MSXML_NODE_NOTATION					MSXML2::NODE_NOTATION

#else

//#define __USE_MSXML2_NAMESPACE__

#include <msxml2.h>
#include <comutil.h>

#ifdef _WIN32_WCE
//#include <msxml2.idl>
#else
#pragma comment (lib, "msxml2.lib")
#endif

#ifdef _DEBUG
#pragma comment (lib, "comsuppd.lib")
#pragma comment (lib, "comsuppwd.lib")
#else
#pragma comment (lib, "comsupp.lib")
#pragma comment (lib, "comsuppw.lib")
#endif


#define MSXML_IXMLDOMNode					IXMLDOMNode
#define	MSXML_IXMLDOMNodeList				IXMLDOMNodeList
#define MSXML_IXMLDOMDocumentFragment		IXMLDOMDocumentFragment
#define MSXML_IXMLDOMCharacterData			IXMLDOMCharacterData
#define MSXML_IXMLDOMComment				IXMLDOMComment
#define MSXML_IXMLDOMText					IXMLDOMText
#define MSXML_IXMLDOMCDATASection			IXMLDOMCDATASection
#define MSXML_IXMLDOMDocument				IXMLDOMDocument
#define MSXML_IXMLDOMDocument2				IXMLDOMDocument2
#define MSXML_IXMLDOMSchemaCollection		IXMLDOMSchemaCollection
#define MSXML_IXMLDOMSchemaCollection2		IXMLDOMSchemaCollection2
#define MSXML_IXMLDOMNamedNodeMap			IXMLDOMNamedNodeMap
#define MSXML_IXMLDOMAttribute				IXMLDOMAttribute
#define MSXML_IXMLDOMElement				IXMLDOMElement
#define MSXML_IXMLDOMDocumentType			IXMLDOMDocumentType
#define MSXML_IXMLDOMProcessingInstruction	IXMLDOMProcessingInstruction
#define MSXML_IXMLDOMEntityReference		IXMLDOMEntityReference
#define MSXML_IXMLDOMEntity					IXMLDOMEntity
#define MSXML_IXMLDOMParseError				IXMLDOMParseError
#define MSXML_IXMLDOMNotation				IXMLDOMNotation
#define MSXML_IXMLDOMImplementation			IXMLDOMImplementation
#define MSXML_IXSLProcessor					IXSLProcessor
#define MSXML_IXMLDOMSelection				IXMLDOMSelection
#define MSXML_ISchemaItem					ISchemaItem
#define MSXML_IXSLTemplate					IXSLTemplate

#define MSXML_DOMNodeType					DOMNodeType
#define MSXML_NODE_INVALID					NODE_INVALID
#define MSXML_NODE_ELEMENT					NODE_ELEMENT
#define MSXML_NODE_ATTRIBUTE				NODE_ATTRIBUTE
#define MSXML_NODE_TEXT						NODE_TEXT
#define MSXML_NODE_CDATA_SECTION			NODE_CDATA_SECTION
#define MSXML_NODE_ENTITY_REFERENCE			NODE_ENTITY_REFERENCE
#define MSXML_NODE_ENTITY					NODE_ENTITY
#define MSXML_NODE_PROCESSING_INSTRUCTION	NODE_PROCESSING_INSTRUCTION
#define MSXML_NODE_COMMENT					NODE_COMMENT
#define MSXML_NODE_DOCUMENT					NODE_DOCUMENT
#define MSXML_NODE_DOCUMENT_TYPE			NODE_DOCUMENT_TYPE
#define MSXML_NODE_DOCUMENT_FRAGMENT		NODE_DOCUMENT_FRAGMENT
#define MSXML_NODE_NOTATION					NODE_NOTATION

#endif	// #if 0/1


#define XML_DOMNodeType						MSXML_DOMNodeType
#define XML_NODE_INVALID					MSXML_NODE_INVALID
#define XML_NODE_ELEMENT					MSXML_NODE_ELEMENT
#define XML_NODE_ATTRIBUTE					MSXML_NODE_ATTRIBUTE
#define XML_NODE_TEXT						MSXML_NODE_TEXT
#define XML_NODE_CDATA_SECTION				MSXML_NODE_CDATA_SECTION
#define XML_NODE_ENTITY_REFERENCE			MSXML_NODE_ENTITY_REFERENCE
#define XML_NODE_ENTITY						MSXML_NODE_ENTITY
#define XML_NODE_PROCESSING_INSTRUCTION		MSXML_NODE_PROCESSING_INSTRUCTION
#define XML_NODE_COMMENT					MSXML_NODE_COMMENT
#define XML_NODE_DOCUMENT					MSXML_NODE_DOCUMENT
#define XML_NODE_DOCUMENT_TYPE				MSXML_NODE_DOCUMENT_TYPE
#define XML_NODE_DOCUMENT_FRAGMENT			MSXML_NODE_DOCUMENT_FRAGMENT
#define XML_NODE_NOTATION					MSXML_NODE_NOTATION



/*
 TXMLDOMNode
    |- TXMLDOMDocument
	|   |- CXMLDOMDocument2
	|- TXMLDOMCharacterData
	    |- TXMLDOMText
		     |- CXMLDOMCDATASection
		|- CXMLDOMComment
	|- CXMLDOMEntityReference
    |- CXMLDOMDocumentFragment
	|- CXMLDOMElement
	|- CXMLDOMAttribute
    |- CXMLDOMDocumentType
	|- CXMLDOMProcessingInstruction
	|- CXMLDOMNotation
	|- CXMLDOMEntity

 CXMLDOMImplementation

 TXMLDOMNodeList
    |- CXMLDOMSelection

 TXMLDOMSchemaCollection
    |- CXMLDOMSchemaCollection2
*/



#if MSXML_VERSION == MSXML_VERSION_60
#define XMLDOMDOCUMENT_ProgID		XML_DOMDOCUMENT60_PROGID
#define XMLHTTP_ProgID				XMLHTTP60_PROGID
#define SERVER_XMLHTTP_ProgID		SERVER_XMLHTTP60_PROGID
#elif MSXML_VERSION == MSXML_VERSION_50
#define XMLDOMDOCUMENT_ProgID		XML_DOMDOCUMENT50_PROGID
#define XMLHTTP_ProgID				XMLHTTP50_PROGID
#define SERVER_XMLHTTP_ProgID		SERVER_XMLHTTP50_PROGID
#elif MSXML_VERSION == MSXML_VERSION_40
#define XMLDOMDOCUMENT_ProgID		XML_DOMDOCUMENT40_PROGID
#define XMLHTTP_ProgID				XMLHTTP40_PROGID
#define SERVER_XMLHTTP_ProgID		SERVER_XMLHTTP40_PROGID
#elif MSXML_VERSION == MSXML_VERSION_30
#define XMLDOMDOCUMENT_ProgID		XML_DOMDOCUMENT30_PROGID
#define XMLHTTP_ProgID				XMLHTTP30_PROGID
#define SERVER_XMLHTTP_ProgID		SERVER_XMLHTTP30_PROGID
#elif MSXML_VERSION == MSXML_VERSION_26
#define XMLDOMDOCUMENT_ProgID		XML_DOMDOCUMENT26_PROGID
#define XMLHTTP_ProgID				XMLHTTP26_PROGID
#define SERVER_XMLHTTP_ProgID		SERVER_XMLHTTP26_PROGID
#else
#define XMLDOMDOCUMENT_ProgID		XML_DOMDOCUMENT_PROGID
#define XMLHTTP_ProgID				XMLHTTP_PROGID
#define SERVER_XMLHTTP_ProgID		SERVER_XMLHTTP_PROGID
#endif



#ifdef _WIN32_WCE

#define CLS_DOMDOCUMENT		__uuidof(DOMDocument)		// CLSID_DOMDocument
#define CLS_XMLHTTP			__uuidof(CLIENT_XMLHTTP)	// CLSID_XMLHTTP
#define CLS_SERVER_XMLHTTP	__uuidof(SERVER_XMLHTTP)	// CLSID_ServerXMLHTTP

#else

#if MSXML_VERSION == MSXML_VERSION_60
#define CLS_DOMDOCUMENT		__uuidof(DOMDocument60)		// CLSID_DOMDocument60
#define CLS_XMLHTTP			__uuidof(XMLHTTP60)			// CLSID_XMLHTTP60
#define CLS_SERVER_XMLHTTP	__uuidof(ServerXMLHTTP60)	// CLSID_ServerXMLHTTP60
#elif MSXML_VERSION == MSXML_VERSION_50
#define CLS_DOMDOCUMENT		__uuidof(DOMDocument50)		// CLSID_DOMDocument50
#define CLS_XMLHTTP			__uuidof(XMLHTTP50)			// CLSID_XMLHTTP50
#define CLS_SERVER_XMLHTTP	__uuidof(ServerXMLHTTP50)	// CLSID_ServerXMLHTTP60
#elif MSXML_VERSION == MSXML_VERSION_40
#define CLS_DOMDOCUMENT		__uuidof(DOMDocument40)		// CLSID_DOMDocument40,
#define CLS_XMLHTTP			__uuidof(XMLHTTP40)			// CLSID_XMLHTTP40
#define CLS_SERVER_XMLHTTP	__uuidof(ServerXMLHTTP40)	// CLSID_ServerXMLHTTP40
#elif MSXML_VERSION == MSXML_VERSION_30
#define CLS_DOMDOCUMENT		__uuidof(DOMDocument30)		// CLSID_DOMDocument30
#define CLS_XMLHTTP			__uuidof(XMLHTTP30)			// CLSID_XMLHTTP30
#define CLS_SERVER_XMLHTTP	__uuidof(ServerXMLHTTP30)	// CLSID_ServerXMLHTTP30
#elif MSXML_VERSION == MSXML_VERSION_26
#define CLS_DOMDOCUMENT		__uuidof(DOMDocument26)
#define CLS_XMLHTTP			__uuidof(XMLHTTP26)			// CLSID_XMLHTTP26
#define CLS_SERVER_XMLHTTP	__uuidof(ServerXMLHTTP)		// CLSID_ServerXMLHTTP
#else
#define CLS_DOMDOCUMENT		__uuidof(DOMDocument)		// CLSID_DOMDocument
#define CLS_XMLHTTP			__uuidof(XMLHTTP)			// CLSID_XMLHTTP
#define CLS_SERVER_XMLHTTP	__uuidof(ServerXMLHTTP)		// CLSID_ServerXMLHTTP
#endif

#endif	// _WIN32_WCE



#define _XSTR(str)					L##str


typedef long			XMLSize_t;
typedef XMLSize_t		XMLIndex_t;
typedef XMLSize_t		XMLCount_t;


template<class T> class XML_CLASS TXMLDOMNode;
template<class T> class XML_CLASS TXMLDOMNodeList;
template<class T> class XML_CLASS TXMLDOMCharacterData;
template<class T> class XML_CLASS TXMLDOMText;
template<class T> class XML_CLASS TXMLDOMSchemaCollection;
template<class T> class XML_CLASS TXMLDOMDocument;

typedef TXMLDOMNode<MSXML_IXMLDOMNode>						CXMLDOMNode;
typedef TXMLDOMNodeList<MSXML_IXMLDOMNodeList>				CXMLDOMNodeList;
typedef TXMLDOMNode<MSXML_IXMLDOMEntityReference>			CXMLDOMEntityReference;
typedef TXMLDOMNode<MSXML_IXMLDOMDocumentFragment>			CXMLDOMDocumentFragment;
typedef TXMLDOMCharacterData<MSXML_IXMLDOMCharacterData>	CXMLDOMCharacterData;
typedef TXMLDOMCharacterData<MSXML_IXMLDOMComment>			CXMLDOMComment;
typedef TXMLDOMText<MSXML_IXMLDOMText>						CXMLDOMText;
typedef TXMLDOMText<MSXML_IXMLDOMCDATASection>				CXMLDOMCDATASection;
typedef TXMLDOMDocument<MSXML_IXMLDOMDocument>				CXMLDOMDocument;

typedef TXMLDOMSchemaCollection<MSXML_IXMLDOMSchemaCollection> CXMLDOMSchemaCollection;


//class CXMLDOMNode;
class CXMLDOMNamedNodeMap;
class CXMLDOMElement;
class CXMLDOMAttribute;
class CXMLDOMDocumentType;
class CXMLDOMProcessingInstruction;
class CXMLDOMNotation;
class CXMLDOMEntity;
class CXMLDOMParseError;
class CXMLDOMSelection;
class CXMLDOMImplementation;
class CXMLDOMDocument2;
#ifndef _WIN32_WCE
class CXMLDOMSchemaCollection2;
#endif

class CXSLTemplate;
class CXSLProcessor;


class CXMLHTTPRequest;
class CServerXMLHTTPRequest;


//using namespace std;

/*
template <typename DerivedType, typename T>
inline void _QueryInterface(const CComPtr<DerivedType> &o, T* *p)
{
#ifdef _DEBUG
#endif
	if (p == NULL)
		return;

	T	*lp = NULL;
	if ( o.p )
	{
	//	o.QueryInterface(__uuidof(T), &lp);
		o.QueryInterface(&lp);

	//	if (lp != NULL)
	//		lp->AddRef();
	}
	*p = lp;
}
*/

template <typename T1, typename T2>
inline void _QueryInterface(T1 *p1, T2* *p2)
{
#ifdef _DEBUG
	assert(p2 != NULL);
#endif

	if (p2 == NULL)
		return;

#ifdef _DEBUG
	assert(*p2 == NULL);
#endif

	T2	*lp = NULL;
	if ( p1 )
	{
		p1->QueryInterface(&lp);
	//	if (lp != NULL)
	//		lp->AddRef();
	}
	*p2 = lp;
}


// -----------------------------------------------------------------------------
// class template TXMLDOMNode
// -----------------------------------------------------------------------------
template<class T>
class XML_CLASS TXMLDOMNode : public CInterfaceCallingWrapper<T>
{
// construction/destruction
public:
	TXMLDOMNode() : CInterfaceCallingWrapper<T>() {}

	/// brief Generalised constructor: construct a TXMLDOMNode from a derived type.
		template <typename DerivedType>
		TXMLDOMNode<T>(const TXMLDOMNode<DerivedType> &o) : CInterfaceCallingWrapper<T>() {
		#ifdef _DEBUG
			// You'll get an error here if you try to set this to an underived class.
	//		Type *test_whether_it_is_really_derived = (DerivedType *) 0;
		#endif
			_QueryInterface(o.p, &p);
		}

	TXMLDOMNode(const TXMLDOMNode<T> &o);
	TXMLDOMNode(T* lp, bool bIncrementRefCount = TRUE);
	virtual ~TXMLDOMNode() {}

// method wrappers

	/** name of the node */
#if SUPPORT_CSTRING
	CString GetNodeName() const;
	bool GetNodeName(CString& name)  const;
#else
	_bstr_t GetNodeName() const;
#endif
	bool GetNodeName(TString<unicode_t> &name) const;

	/** value stored in the node. the text associated with the node. */
#if SUPPORT_CSTRING
//	CString GetNodeValue() const;
//	bool GetNodeValue(CString& value) const;
#endif
	_variant_t GetNodeValueVar() const;
	bool GetNodeValue(TString<unicode_t> &value) const;

	/** value stored in the node */
//	void SetNodeValue(LPCSTR value);
//	void SetNodeValue(LPCWSTR value);
	void SetNodeValue(const _variant_t& rhs);

	/** the node's type */
	MSXML_DOMNodeType GetNodeType() const;

	/** parent of the node */
	CXMLDOMNode GetParentNode() const;

	/** the collection of the node's children */
	CXMLDOMNodeList GetChildNodes() const;

	long GetChildCount() const;

	/** first child of the node */
	CXMLDOMNode GetFirstChild() const;
	CXMLDOMNode GetFirstChildNode() const { return GetFirstChild(); }

	/** last child of the node */
	CXMLDOMNode GetLastChild() const;
	CXMLDOMNode GetLastChildNode() const { return GetLastChild(); }

	/** left sibling of the node */
	CXMLDOMNode GetPreviousSibling() const;

	/** right sibling of the node */
	CXMLDOMNode GetNextSibling() const;
	CXMLDOMNode GetNextSiblingNode() const { return GetNextSibling(); }

	/** the collection of the node's attributes */
	CXMLDOMNamedNodeMap GetAttributes() const;
	bool GetAttributes(MSXML_IXMLDOMNamedNodeMap* *attributeMap) const;
//	CXMLDOMNamedNodeMap& GetAttributes(CXMLDOMNamedNodeMap& attributeMap) const;
	MSXML_IXMLDOMNamedNodeMap* GetNamedNodeMap() const;

	/** insert a child node */
	CXMLDOMNode InsertBefore(MSXML_IXMLDOMNode* newChild, const _variant_t& refChild);
	CXMLDOMNode InsertBefore(MSXML_IXMLDOMNode* newChild, MSXML_IXMLDOMNode* refChild);

	/** replace a child node */
	CXMLDOMNode ReplaceChild(MSXML_IXMLDOMNode* newChild, MSXML_IXMLDOMNode* oldChild);

	/** remove a child node */
	CXMLDOMNode RemoveChild(MSXML_IXMLDOMNode* childNode);

	/** append a child node */
	CXMLDOMNode AppendChild(MSXML_IXMLDOMNode* newChild);

	bool HasChildNodes() const;

	/** document that contains the node */
	CXMLDOMDocument GetOwnerDocument() const;
	MSXML_IXMLDOMDocument* GetIXMLDOMDocument() const;

	CXMLDOMNode CloneNode(bool deep);

	/** the type of node in string form */
#if SUPPORT_CSTRING
	CString GetNodeTypeString() const;
#endif
	bool GetNodeTypeString(TString<unicode_t>& outString) const;

	/** text content of the node and subtree */
	/** Represents the text content of the node or 
	    the concatenated text representing the node and its descendants */
#if SUPPORT_CSTRING
	CString GetText() const;
	bool GetText(CString& text) const;
#else
	_bstr_t GetText() const;
#endif
	bool GetText(TString<unicode_t> &text) const;

	/** text content of the node and subtree */
	void SetText(LPCSTR rhs);
	void SetText(LPCWSTR rhs);

	/** indicates whether node is a default value */
	bool GetSpecified() const;

	/** pointer to the definition of the node in the DTD or schema */
	CXMLDOMNode GetDefinition() const;

	/** get the strongly typed value of the node */
	_variant_t GetNodeTypedValue() const;

	/** get the strongly typed value of the node */
	void SetNodeTypedValue(const _variant_t& rhs);

	/** the data type of the node */
	_variant_t GetDataType() const;

	/** the data type of the node */
	void SetDataType(LPCSTR rhs);
	void SetDataType(LPCWSTR rhs);

	/** return the XML source for the node and each of its descendants */
#if SUPPORT_CSTRING
	CString GetXML() const;
	bool GetXML(CString& outString) const;
#endif
	bool GetXML(TString<char>& outString) const;
	bool GetXML(TString<unsigned char>& outString) const;
	bool GetXML(TString<unicode_t>& outString) const;

	/** apply the stylesheet to the subtree */
#if SUPPORT_CSTRING
	CString TransformNode(MSXML_IXMLDOMNode* stylesheet);
#endif
	bool TransformNode(MSXML_IXMLDOMNode* stylesheet, TString<unicode_t>& outString);

	/** execute query on the subtree */
	CXMLDOMNodeList SelectNodes(LPCSTR queryString) const;
	CXMLDOMNodeList SelectNodes(LPCWSTR queryString) const;

	/** execute query on the subtree */
	CXMLDOMNode SelectSingleNode(LPCSTR queryString) const;
	CXMLDOMNode SelectSingleNode(LPCWSTR queryString) const;

	/** has sub-tree been completely parsed */
	bool GetParsed() const;

	/** the URI for the namespace applying to the node */
#if SUPPORT_CSTRING
	CString GetNamespaceURI() const;
#endif
	bool GetNamespaceURI(TString<unicode_t>& outString) const;

	/** the prefix for the namespace applying to the node */
#if SUPPORT_CSTRING
	CString GetPrefix() const;
#endif
	bool GetPrefix(TString<unicode_t>& outString) const;

	/** the base name of the node (nodename with the prefix stripped off) */
#if SUPPORT_CSTRING
	CString GetBaseName() const;
#endif
	bool GetBaseName(TString<unicode_t>& outString) const;

	/** apply the stylesheet to the subtree, returning the result through a document or a stream */
	void TransformNodeToObject(MSXML_IXMLDOMNode* stylesheet, const _variant_t& outputObject);

// Util
#if SUPPORT_CSTRING
	CString GetAttributeStr(LPCSTR name) const;
	CString GetAttributeStr(LPCWSTR name) const;
#endif

	_variant_t GetAttribute(LPCSTR name) const;
	_variant_t GetAttribute(LPCWSTR name) const;
	bool GetAttribute(LPCSTR name, TString<unicode_t>& outString) const;
	bool GetAttribute(LPCWSTR name, TString<unicode_t>& outString) const;

	bool SetAttribute(LPCSTR Name, LPCSTR value);
	bool SetAttribute(LPCWSTR Name, LPCWSTR value);
	bool SetAttribute(LPCSTR Name, const _variant_t &value);
	bool SetAttribute(LPCWSTR Name, const _variant_t &value);

	void SetAttributes(const CXMLDOMNode &node) {SetAttributes(node.GetAttributes()); }
	void SetAttributes(const CXMLDOMNamedNodeMap &nodeAttr);

	void RemoveChildNodes();	// RemoveChildren
	// 현재 자신의 노드를 지움.
	CXMLDOMNode RemoveMe();
	//현재 자신의 노드도 지우고 자신의 ParentNode의 childnode의 갯수가 0이면 Parentnode도 지운다..
	void RemoveMeEx();
//	void RemoveMeExtend(MSXML_IXMLDOMNode *Node);
	void AppendChild(CXMLDOMNodeList &nodeList);	// AppendChildren

	CXMLDOMNode GetPrevSiblingElement() const;
	CXMLDOMNode GetNextSiblingElement() const;

#if SUPPORT_CASTING_OP
	operator CXMLDOMNode() const
	{
		return CXMLDOMNode(p, TRUE);
	//	CXMLDOMNode	node(p, TRUE);
	//	node = (MSXML_IXMLDOMNode*)p;
	//	return node;
	}
#endif

// for Debug
	void Dump() const;
};

template<class T>
TXMLDOMNode<T>::TXMLDOMNode(T *lp, bool bIncrementRefCount /*= TRUE*/)
 : CInterfaceCallingWrapper<T>(lp, bIncrementRefCount)
{
}

template<class T>
TXMLDOMNode<T>::TXMLDOMNode(const TXMLDOMNode<T> &o)
 : CInterfaceCallingWrapper<T>(o)
{
}

#if SUPPORT_CSTRING
template<class T>
CString TXMLDOMNode<T>::GetNodeName() const
{
	BSTR	bstrO = NULL;

	HRCHECK( p->get_nodeName(&bstrO), "" );

	_bstr_t	bstr(bstrO, false);

	return CString((LPCTSTR)bstr);
}

template<class T>
bool TXMLDOMNode<T>::GetNodeName(CString& name) const
{
	BSTR	bstrO = NULL;

	name = "";
	RETURN_IF_FALSE( p->get_nodeName(&bstrO) );
	name = (LPCTSTR)_bstr_t(bstrO, false);
//	name = bstrO;

	return name.GetLength() > 0;
}
#else
template<class T>
_bstr_t TXMLDOMNode<T>::GetNodeName() const
{
	BSTR	bstrO = NULL;
	HRCHECK( p->get_nodeName(&bstrO), "" );
	return	_bstr_t(bstrO, false);
}
#endif	// SUPPORT_CSTRING


template<class T>
bool TXMLDOMNode<T>::GetNodeName(TString<unicode_t> &name) const
{
	BSTR	bstrO = NULL;

	name.Empty();
	RETURN_IF_FALSE( p->get_nodeName(&bstrO) );

	if ( bstrO )
	{
		name.Init((wchar_t *)bstrO, SysStringLen(bstrO));
		SysFreeString(bstrO);
	}

	return name.GetCount() > 0;
}

#if SUPPORT_CSTRING && 0

template<class T>
CString TXMLDOMNode<T>::GetNodeValue() const
{
	_variant_t	var;

	HRCHECK( p->get_nodeValue(&var), "" );

	if ( var.vt == VT_BSTR )
		return CString(var.bstrVal);

	return CString();
}

template<class T>
bool TXMLDOMNode<T>::GetNodeValue(CString& text) const
{
	_variant_t	var;

	HRCHECK( p->get_nodeValue(&var), "" );

	if ( var.vt == VT_BSTR )
		text = var.bstrVal;
	else
		text = _T("");

	return text.GetLength() > 0;
}

#else

template<class T>
_variant_t TXMLDOMNode<T>::GetNodeValueVar() const
{
#if 1
	VARIANT	var;
	V_VT(&var) = VT_EMPTY;
	HRCHECK( p->get_nodeValue(&var), "" );
	return _variant_t(var, false);
#else
	_variant_t	varO;
	HRCHECK( p->get_nodeValue(&varO), "" );
	return varO;
#endif
}

#endif	// SUPPORT_CSTRING

template<class T>
bool TXMLDOMNode<T>::GetNodeValue(TString<unicode_t> &text) const
{
	_variant_t	var;
	
	text.Empty();
	RETURN_IF_FALSE( p->get_nodeValue(&var) );

	if ( var.vt == VT_BSTR )
		text.Init((wchar_t *)var.bstrVal, SysStringLen(var.bstrVal));

	return text.GetCount() > 0;
}


/*
template<class T>
void TXMLDOMNode<T>::SetNodeValue(LPCSTR value)
{
	_variant_t	var(value);
	HRCHECK( p->put_nodeValue(var), "" );
}

template<class T>
void TXMLDOMNode<T>::SetNodeValue(LPCWSTR value)
{
	_variant_t	var(value);
	HRCHECK( p->put_nodeValue(var), "" );
}
*/

template<class T>
void TXMLDOMNode<T>::SetNodeValue(const _variant_t& rhs)
{
	HRCHECK( p->put_nodeValue(rhs), "" );
}

template<class T>
MSXML_DOMNodeType TXMLDOMNode<T>::GetNodeType() const
{
	MSXML_DOMNodeType o = (MSXML_DOMNodeType)MSXML_NODE_INVALID;

	HRCHECK( p->get_nodeType(&o), "" );

	return o;
}

template<class T>
CXMLDOMNode TXMLDOMNode<T>::GetParentNode() const
{
	MSXML_IXMLDOMNode* o = NULL;

	HRCHECK( p->get_parentNode(&o), "" );

	return CXMLDOMNode(o, FALSE);
}

template<class T>
CXMLDOMNodeList TXMLDOMNode<T>::GetChildNodes() const
{
	MSXML_IXMLDOMNodeList* o = NULL;

	HRCHECK( p->get_childNodes(&o), "" );

	return CXMLDOMNodeList(o, FALSE);
}

template<class T>
long TXMLDOMNode<T>::GetChildCount() const
{
	return GetChildNodes().GetCount();
/*
	MSXML_IXMLDOMNodeList* domNodeList = NULL;

	HRCHECK( p->get_childNodes(&domNodeList), "" );

	long count = 0;

	if ( domNodeList )
	{
		domNodeList->get_length(&count);
		domNodeList->Release();
	}

	return count;
*/
}


template<class T>
CXMLDOMNode TXMLDOMNode<T>::GetFirstChild() const
{
	MSXML_IXMLDOMNode* o = NULL;

	HRCHECK( p->get_firstChild(&o), "" );

	return CXMLDOMNode(o, FALSE);
}

template<class T>
CXMLDOMNode TXMLDOMNode<T>::GetLastChild() const
{
	MSXML_IXMLDOMNode* o = NULL;

	HRCHECK( p->get_lastChild(&o), "" );

	return CXMLDOMNode(o, FALSE);
}

template<class T>
CXMLDOMNode TXMLDOMNode<T>::GetPreviousSibling() const
{
	MSXML_IXMLDOMNode* o = NULL;

	HRCHECK( p->get_previousSibling(&o), "" );

	return CXMLDOMNode(o, FALSE);
}

template<class T>
CXMLDOMNode TXMLDOMNode<T>::GetNextSibling() const
{
	MSXML_IXMLDOMNode* o = NULL;

	HRCHECK( p->get_nextSibling(&o), "" );

	return CXMLDOMNode(o, FALSE);
}


template<class T>
MSXML_IXMLDOMNamedNodeMap* TXMLDOMNode<T>::GetNamedNodeMap() const
{
	MSXML_IXMLDOMNamedNodeMap* o = NULL;

	HRCHECK( p->get_attributes(&o), "" );

	return o;
}

template<class T>
bool TXMLDOMNode<T>::GetAttributes(MSXML_IXMLDOMNamedNodeMap* *attributeMap) const
{
	*attributeMap = NULL;
	HRCHECK_RETURN_BOOL( p->get_attributes(attributeMap) );
	return *attributeMap != NULL;
}

/*
template<class T>
CXMLDOMNamedNodeMap TXMLDOMNode<T>::GetAttributes() const
{
	MSXML_IXMLDOMNamedNodeMap* lp = NULL;

	HRCHECK( p->get_attributes(&lp), "" );

	return CXMLDOMNamedNodeMap(lp, FALSE);
}
*/

template<class T>
CXMLDOMNode TXMLDOMNode<T>::InsertBefore(MSXML_IXMLDOMNode* newChild, const _variant_t& refChild)
{
	MSXML_IXMLDOMNode* outNewChild = NULL;

	if ( newChild )
	{
		HRCHECK( p->insertBefore(newChild, refChild, &outNewChild), "" );
	}

	return CXMLDOMNode(outNewChild, FALSE);
}

template<class T>
CXMLDOMNode TXMLDOMNode<T>::InsertBefore(MSXML_IXMLDOMNode* newChild, MSXML_IXMLDOMNode* refChild)
{
	MSXML_IXMLDOMNode* outNewChild = NULL;

	if ( newChild )
	{
		_variant_t	var(refChild);
		HRCHECK( p->insertBefore(newChild, var, &outNewChild), "" );
	}

	return CXMLDOMNode(outNewChild, FALSE);
}

template<class T>
CXMLDOMNode TXMLDOMNode<T>::ReplaceChild(MSXML_IXMLDOMNode* newChild, MSXML_IXMLDOMNode* oldChild)
{
	MSXML_IXMLDOMNode* outOldChild = NULL;

	if ( newChild && oldChild )
	{
		HRCHECK( p->replaceChild(newChild, oldChild, &outOldChild), "" );
	}

	return CXMLDOMNode(outOldChild, FALSE);
}

template<class T>
CXMLDOMNode TXMLDOMNode<T>::RemoveChild(MSXML_IXMLDOMNode* childNode)
{
	MSXML_IXMLDOMNode* outOldChild = NULL;

	if ( childNode )
	{
		HRCHECK( p->removeChild(childNode, &outOldChild), "" );
	}

	return CXMLDOMNode(outOldChild, FALSE);
}

template<class T>
CXMLDOMNode TXMLDOMNode<T>::AppendChild(MSXML_IXMLDOMNode* newChild)
{
	MSXML_IXMLDOMNode* outNewChild = NULL;

	if ( newChild )
	{
		HRCHECK( p->appendChild(newChild, &outNewChild), "" );
	}

	return CXMLDOMNode(outNewChild, FALSE);
}

template<class T>
bool TXMLDOMNode<T>::HasChildNodes() const
{
	VARIANT_BOOL	status = VARIANT_FALSE;

	HRCHECK( p->hasChildNodes(&status), "" );

	return status == VARIANT_TRUE;
}

template<class T>
CXMLDOMDocument TXMLDOMNode<T>::GetOwnerDocument() const
{
	MSXML_IXMLDOMDocument* o = NULL;

	HRCHECK( p->get_ownerDocument(&o), "" );

	return CXMLDOMDocument(o, FALSE);
}

template<class T>
CXMLDOMNode TXMLDOMNode<T>::CloneNode(bool deep)
{
	MSXML_IXMLDOMNode* o = NULL;

	HRCHECK( p->cloneNode(deep ? VARIANT_TRUE : VARIANT_FALSE, &o), "" );

	return CXMLDOMNode(o, FALSE);
}

#if SUPPORT_CSTRING
template<class T>
CString TXMLDOMNode<T>::GetNodeTypeString() const
{
	BSTR	bstrO = NULL;

	HRCHECK(  p->get_nodeTypeString(&bstrO), "" );

	return CString((LPCTSTR)_bstr_t(bstrO, false));
}
#endif

template<class T>
bool TXMLDOMNode<T>::GetNodeTypeString(TString<unicode_t>& outString) const
{
	BSTR	bstrO = NULL;

	outString.Empty();
	RETURN_IF_FALSE( p->get_nodeTypeString(&bstrO) );

	if ( bstrO )
	{
		_bstr_t	bstr(bstrO, false);
		
		outString.Init((wchar_t*)bstr, bstr.length());
		return TRUE;
	}

	return FALSE;
}

#if SUPPORT_CSTRING
template<class T>
CString TXMLDOMNode<T>::GetText() const
{
	BSTR	bstrO = NULL;

	HRCHECK( p->get_text(&bstrO), "" );

	return CString((LPCTSTR)_bstr_t(bstrO, false));
}

template<class T>
bool TXMLDOMNode<T>::GetText(CString& text) const
{
	BSTR		bstrO = NULL;

	text = "";
  	RETURN_IF_FALSE( p->get_text(&bstrO) );
	text = (LPCTSTR)_bstr_t(bstrO, false);

	return text.GetLength() > 0;
}
#else
template<class T>
_bstr_t TXMLDOMNode<T>::GetText() const
{
	BSTR	bstrO = NULL;
	HRCHECK( p->get_text(&bstrO), "" );
	return _bstr_t(bstrO, false);
}
#endif	// SUPPORT_CSTRING

template<class T>
bool TXMLDOMNode<T>::GetText(TString<unicode_t> &text) const
{
	BSTR		bstrO = NULL;

 	text.Empty();
 	RETURN_IF_FALSE( p->get_text(&bstrO) );
	
	if ( bstrO )
	{
		text.Init((wchar_t *)bstrO, SysStringLen(bstrO));
		SysFreeString(bstrO);
	}

	return text.GetCount() > 0;
}


template<class T>
void TXMLDOMNode<T>::SetText(LPCSTR rhs)
{
	_bstr_t		bstrrhs(rhs);
	HRCHECK( p->put_text(bstrrhs), "" );
}


template<class T>
void TXMLDOMNode<T>::SetText(LPCWSTR rhs)
{
	HRCHECK( p->put_text((unicode_t*)rhs), "" );
}


template<class T>
bool TXMLDOMNode<T>::GetSpecified() const
{
	VARIANT_BOOL	status = VARIANT_FALSE;

	HRCHECK( p->get_specified(&status), "" );

	return status == VARIANT_TRUE;
}

template<class T>
CXMLDOMNode TXMLDOMNode<T>::GetDefinition() const
{
	MSXML_IXMLDOMNode* o = NULL;

	HRCHECK( p->get_definition(&o), "" );

	return CXMLDOMNode(o, FALSE);
}

template<class T>
_variant_t TXMLDOMNode<T>::GetNodeTypedValue() const
{
	VARIANT	var;
	V_VT(&var) = VT_EMPTY;

	HRCHECK( p->get_nodeTypedValue(&var), "" );
	return _variant_t(var, false);
}

template<class T>
void TXMLDOMNode<T>::SetNodeTypedValue(const _variant_t& rhs)
{
	HRCHECK( p->put_nodeTypedValue(rhs), "" );
}

template<class T>
_variant_t TXMLDOMNode<T>::GetDataType() const
{
	VARIANT	var;
	V_VT(&var) = VT_EMPTY;

	HRCHECK( p->get_dataType(&var), "" );
	return _variant_t(var, false);
}

template<class T>
void TXMLDOMNode<T>::SetDataType(LPCSTR rhs)
{
	_bstr_t		bstrrhs(rhs);
	HRCHECK( p->put_dataType(bstrrhs), "" );
}

template<class T>
void TXMLDOMNode<T>::SetDataType(LPCWSTR rhs)
{
	HRCHECK( p->put_dataType((unicode_t*)rhs), "" );
}

#if SUPPORT_CSTRING
template<class T>
CString TXMLDOMNode<T>::GetXML() const
{
	BSTR	bstrO = NULL;

	HRCHECK( p->get_xml(&bstrO), "" );

	return CString((LPCTSTR)_bstr_t(bstrO, false));
}
#endif

#if SUPPORT_CSTRING
template<class T>
CString TXMLDOMNode<T>::TransformNode(MSXML_IXMLDOMNode* stylesheet)
{
	BSTR	bstrO = NULL;

	HRCHECK( p->transformNode(stylesheet, &bstrO), "" );

	return CString((LPCTSTR)_bstr_t(bstrO, false));
}
#endif

template<class T>
bool TXMLDOMNode<T>::TransformNode(MSXML_IXMLDOMNode* stylesheet, TString<unicode_t>& outString)
{
	BSTR	bstrO = NULL;

	outString.Empty();

	RETURN_IF_FALSE( p->transformNode(stylesheet, &bstrO) );

	if ( bstrO )
	{
		_bstr_t	bstr(bstrO, false);
		
		outString.Init((wchar_t*)bstr, bstr.length());
		return TRUE;
	}

	return FALSE;
}

template<class T>
CXMLDOMNodeList TXMLDOMNode<T>::SelectNodes(LPCSTR queryString) const
{
	MSXML_IXMLDOMNodeList* o = NULL;

	_bstr_t		bstrQueryString(queryString);
	HRCHECK( p->selectNodes(bstrQueryString, &o), "");

	return CXMLDOMNodeList(o, FALSE);
}

template<class T>
CXMLDOMNodeList TXMLDOMNode<T>::SelectNodes(LPCWSTR queryString) const
{
	MSXML_IXMLDOMNodeList* o = NULL;

	HRCHECK( p->selectNodes((unicode_t*)queryString, &o), "");

	return CXMLDOMNodeList(o, FALSE);
}

template<class T>
CXMLDOMNode TXMLDOMNode<T>::SelectSingleNode(LPCSTR queryString) const 
{
	MSXML_IXMLDOMNode* o = NULL;

	_bstr_t		bstrQueryString(queryString);
	HRCHECK( p->selectSingleNode(bstrQueryString, &o), "");

	return CXMLDOMNode(o, FALSE);
}

template<class T>
CXMLDOMNode TXMLDOMNode<T>::SelectSingleNode(LPCWSTR queryString) const 
{
	MSXML_IXMLDOMNode* o = NULL;

	HRCHECK( p->selectSingleNode((unicode_t*)queryString, &o), "");

	return CXMLDOMNode(o, FALSE);
}

template<class T>
bool TXMLDOMNode<T>::GetParsed() const
{
	VARIANT_BOOL	status = VARIANT_FALSE;

	HRCHECK( p->get_parsed(&status), "" );

	return status == VARIANT_TRUE;
}

#if SUPPORT_CSTRING
template<class T>
CString TXMLDOMNode<T>::GetNamespaceURI() const
{
	BSTR	bstrO = NULL;

	HRCHECK( p->get_namespaceURI(&bstrO), "" );

	return CString((LPCTSTR)_bstr_t(bstrO, false));
}
#endif

template<class T>
bool TXMLDOMNode<T>::GetNamespaceURI(TString<unicode_t>& outString) const
{
	BSTR	bstrO = NULL;

	outString.Empty();
	RETURN_IF_FALSE( p->get_namespaceURI(&bstrO) );

	if ( bstrO )
	{
		_bstr_t	bstr(bstrO, false);
		
		outString.Init((wchar_t*)bstr, bstr.length());
		return TRUE;
	}

	return FALSE;
}

#if SUPPORT_CSTRING
template<class T>
CString TXMLDOMNode<T>::GetPrefix() const
{
	BSTR	bstrO = NULL;

	HRCHECK( p->get_prefix(&bstrO), "" );

	return CString((LPCTSTR)_bstr_t(bstrO, false));
}
#endif

template<class T>
bool TXMLDOMNode<T>::GetPrefix(TString<unicode_t>& outString) const
{
	BSTR	bstrO = NULL;

	outString.Empty();
	RETURN_IF_FALSE( p->get_prefix(&bstrO) );

	if ( bstrO )
	{
		_bstr_t	bstr(bstrO, false);
		
		outString.Init((wchar_t *)bstr, bstr.length());
		return TRUE;
	}

	return FALSE;
}

#if SUPPORT_CSTRING
template<class T>
CString TXMLDOMNode<T>::GetBaseName() const
{
	BSTR	bstrO = NULL;

	HRCHECK( p->get_baseName(&bstrO), "" );

	return CString((LPCTSTR)_bstr_t(bstrO, false));
}
#endif

template<class T>
bool TXMLDOMNode<T>::GetBaseName(TString<unicode_t>& outString) const
{
	BSTR	bstrO = NULL;

	outString.Empty();
	RETURN_IF_FALSE( p->get_baseName(&bstrO) );

	if ( bstrO )
	{
		_bstr_t	bstr(bstrO, false);
		
		outString.Init((wchar_t*)bstr, bstr.length());
		return TRUE;
	}

	return FALSE;
}

template<class T>
void TXMLDOMNode<T>::TransformNodeToObject(MSXML_IXMLDOMNode* stylesheet, const _variant_t& outputObject)
{
	HRCHECK( p->transformNodeToObject(stylesheet, outputObject), "" );
}

template<class T>
_variant_t TXMLDOMNode<T>::GetAttribute(LPCSTR Name) const
{
	return GetAttribute((unicode_t*)_bstr_t(Name));
}

template<class T>
_variant_t TXMLDOMNode<T>::GetAttribute(LPCWSTR Name) const
{
#if 0
	MSXML_DOMNodeType nodeType = GetNodeType();
	if ( nodeType == MSXML_NODE_ELEMENT )
	{
		CXMLDOMElement	elem(*this);

		return elem.GetVarAttribute(Name);
	}
#else
//	else // if ( nodeType == MSXML2::NODE_ATTRIBUTE )
	{
	//	var = GetNodeValue();
		CXMLDOMNamedNodeMap nodeAttr = GetAttributes();
		if ( nodeAttr.IsValid() )
		{
			return nodeAttr.GetNamedItem(Name).GetNodeValueVar();
		//	var = nodeAttr.GetAttribute(Name);
		}
	}
#endif

	return _variant_t();
}

#if SUPPORT_CSTRING
template<class T>
CString TXMLDOMNode<T>::GetAttributeStr(LPCSTR Name) const
{
	return GetAttributeStr((unicode_t*)_bstr_t(Name));
}

template<class T>
CString TXMLDOMNode<T>::GetAttributeStr(LPCWSTR Name) const
{
	_variant_t	 var;
#if 0
	MSXML_DOMNodeType nodeType = GetNodeType();
	if ( nodeType == MSXML_NODE_ELEMENT )
	{
		CXMLDOMElement	elem(*this);

		var = elem.GetVarAttribute(Name);
	}
#else
//	else // if ( nodeType == MSXML2::NODE_ATTRIBUTE )
	{
	//	var = GetNodeValue();
		CXMLDOMNamedNodeMap nodeAttr = GetAttributes();
		if ( nodeAttr.IsValid() )
		{
			var = nodeAttr.GetNamedItem(Name).GetNodeValueVar();
		//	var = nodeAttr.GetAttribute(Name);
		}
	}
#endif

#if _MSC_VER < 1299
	if ( var.vt == VT_BSTR )
		return CString(var.bstrVal);

	return CString();
#else
	return CString(var);
#endif
}
#endif	//SUPPORT_CSTRING

template<class T>
bool TXMLDOMNode<T>::GetAttribute(LPCSTR name, TString<unicode_t>& outString) const
{
	return GetAttribute((unicode_t*)_bstr_t(name), outString);
}

template<class T>
bool TXMLDOMNode<T>::GetAttribute(LPCWSTR name, TString<unicode_t>& outString) const
{
#if 0
	MSXML_DOMNodeType nodeType = GetNodeType();
	if ( nodeType == MSXML_NODE_ELEMENT )
	{
		CXMLDOMElement	elem(*this);

		return elem.GetAttribute(name, outString);
	}
	else // if ( nodeType == MSXML2::NODE_ATTRIBUTE )
#else
	{
		outString.Empty();
		CXMLDOMNamedNodeMap nodeAttr = GetAttributes();
		if ( nodeAttr.IsValid() )
		{
			nodeAttr.GetNamedItem(name).GetNodeValue(outString);
		}
	}
#endif

	return !outString.IsEmpty();
}


template<class T>
bool TXMLDOMNode<T>::SetAttribute(LPCSTR Name, LPCSTR value)
{
	return SetAttribute((LPCWSTR)_bstr_t(Name), _variant_t(value));
}

template<class T>
bool TXMLDOMNode<T>::SetAttribute(LPCWSTR Name, LPCWSTR value)
{
	return SetAttribute(Name, _variant_t(value));
}

template<class T>
bool TXMLDOMNode<T>::SetAttribute(LPCSTR Name, const _variant_t &value)
{
	return SetAttribute((LPCWSTR)_bstr_t(Name), value);
}

template<class T>
bool TXMLDOMNode<T>::SetAttribute(LPCWSTR name, const _variant_t &value)
{
#if 0
	MSXML_DOMNodeType nodeType = GetNodeType();
	if ( nodeType == MSXML_NODE_ELEMENT )
	{
		CXMLDOMElement elem(*this);
		elem.SetAttribute(name, value);
		return TRUE;
	}
#else
	CXMLDOMNamedNodeMap nodeAttr = GetAttributes();
	if ( nodeAttr.IsValid() )
	{
		CXMLDOMAttribute NewAttr = GetOwnerDocument().CreateAttribute(name);
		if ( NewAttr.IsValid() )
		{
			NewAttr.SetNodeValue(value);
			nodeAttr.SetNamedItem(NewAttr);

			return TRUE;
		}
	}
#endif
	return FALSE;
}

template<class T>
void TXMLDOMNode<T>::SetAttributes(const CXMLDOMNamedNodeMap &nodeAttr)
{
	XMLCount_t			attrCount = nodeAttr.GetLength();
	CXMLDOMNode			attrNode;
	TString<unicode_t>	name;
	TString<unicode_t>	value;

	for (XMLCount_t i = 0; i < attrCount; i++ )
	{
		attrNode = nodeAttr.GetItem(i);
		attrNode.GetNodeName(name);	//
		attrNode.GetNodeValue(value);	//

		SetAttribute(name, value);
	}
}



#if SUPPORT_CSTRING
template<class T>
bool TXMLDOMNode<T>::GetXML(CString& outString) const
{
	BSTR	bstrO = NULL;

	outString = "";
	RETURN_IF_FALSE( p->get_xml(&bstrO) );

	if ( bstrO )
	{
	//	outString = bstrO;
		outString = (LPCTSTR)(_bstr_t(bstrO, false));
		return TRUE;
	}

	return FALSE;
}
#endif

template<class T>
bool TXMLDOMNode<T>::GetXML(TString<char>& outString) const
{
	BSTR	bstrO = NULL;

	outString.Empty();
	RETURN_IF_FALSE( p->get_xml(&bstrO) );

	if ( bstrO )
	{
		_bstr_t	bstr(bstrO, false);

		int	nSize = ::WideCharToMultiByte(GetACP(), 0, bstr, bstr.length(), NULL, 0, NULL, NULL);
		if ( nSize )
		{
		//	ULONG	nOldCount = outString.GetCount();
			char* pChar = outString.SetCount(nSize);
			if ( pChar != NULL )
			{
				WideCharToMultiByte(GetACP(), 0, bstr, bstr.length(), pChar, nSize, NULL, NULL);
			}
		}

		return TRUE;
	}
	return FALSE;
}

template<class T>
bool TXMLDOMNode<T>::GetXML(TString<unsigned char>& outString) const
{
	BSTR	bstrO = NULL;

	outString.Empty();
	RETURN_IF_FALSE( p->get_xml(&bstrO) );

	if ( bstrO )
	{
		_bstr_t	bstr(bstrO, false);

		int	nSize = ::WideCharToMultiByte(CP_UTF8, 0, bstr, bstr.length(), NULL, 0, NULL, NULL);
		if ( nSize )
		{
		//	ULONG	nOldCount = outString.GetCount();
			unsigned char* pChar = outString.SetCount(nSize);
			if ( pChar != NULL )
			{
				WideCharToMultiByte(CP_UTF8, 0, bstr, bstr.length(), (char *)pChar, nSize, NULL, NULL);
			}
		}

		return TRUE;
	}
	return FALSE;
}

template<class T>
bool TXMLDOMNode<T>::GetXML(TString<unicode_t>& outString) const
{
	BSTR	bstrO = NULL;

	outString.Empty();
	RETURN_IF_FALSE( p->get_xml(&bstrO) );
	if ( bstrO )
	{
	#if 0
		outString.Init(bstrO, (ULONG)SysStringByteLen(bstrO)/2));
		SysFreeString(bstrO);
	#else
		_bstr_t	bstr(bstrO, false);		
		outString.Init((wchar_t*)bstr, bstr.length());
	#endif
		return TRUE;
	}

	return FALSE;
}


template<class T>
MSXML_IXMLDOMDocument* TXMLDOMNode<T>::GetIXMLDOMDocument() const
{
	MSXML_IXMLDOMDocument* o = NULL;

	HRCHECK( p->get_ownerDocument(&o), "" );

	return o;
}


template<class T>
void TXMLDOMNode<T>::RemoveChildNodes()
{
	CXMLDOMNodeList nodeList( GetChildNodes() );

	XMLCount_t count = nodeList.GetCount();
	for ( XMLIndex_t i = count - 1; i >= 0; i-- )
	{
		RemoveChild(nodeList[i]);
	}
}

template<class T>
CXMLDOMNode TXMLDOMNode<T>::RemoveMe()
{
	return GetParentNode().RemoveChild( p );
}

//현재 자신의 노드도 지우고 자신의 ParentNode의 childnode의 갯수가 0이면 Parentnode도 지운다..
template<class T>
void TXMLDOMNode<T>::RemoveMeEx()
{
	CXMLDOMNode parentNode(GetParentNode());
	parentNode.RemoveChild( p );

	if ( parentNode.GetChildCount() == 0 )
	{		
		parentNode.RemoveMe();
	}
}

#if 0
//현재 자신의 노드도 지우고 자신의 ParentNode의 childnode의 갯수가 0이면 Parentnode도 지운다..
template<class T>
void TXMLDOMNode<T>::RemoveMeExtend(MSXML_IXMLDOMNode *node)
{
	HRESULT hr;

	CComPtr<MSXML_IXMLDOMNode>	parentNode;
	hr = node->get_parentNode(&parentNode);
	if(!parentNode)	return;

	CComPtr<MSXML_IXMLDOMNode>	child;
	hr = parentNode->removeChild(Node, &child);

	BSTR	bstr = NULL;
	hr = parentNode->get_nodeName(&bstr);

#if 1
	_bstr_t	bstrSystem(_T("system"));
	_bstr_t	bstr_t(bstr, false);
	if( bstrSystem == bstr_t ) return;
#else
	_bstr_t	bstr_t(bstr, false);
	CString	nodeName = (LPCTSTR)bstr_t;
	if( nodeName == _T("system") )	return;
#endif

	CComPtr<MSXML_IXMLDOMNodeList> olist;
	hr = parentNode->get_childNodes(&olist);
	if ( olist )
	{
		long len = 0;
		hr = olist->get_length(&len);
		
		if (len==0)
			RemoveMeExtend(parentNode);
	}
}
#endif


template<class T>
void TXMLDOMNode<T>::AppendChild(CXMLDOMNodeList &nodeList)
{
	XMLCount_t count = nodeList.GetCount();
	for ( XMLIndex_t i = 0; i < count; i++ )
	{
		CXMLDOMNode	node(nodeList.GetItem(i));
		AppendChild(node);
	}
}


template<class T>
CXMLDOMNode TXMLDOMNode<T>::GetPrevSiblingElement() const
{
	CXMLDOMNode node  = GePrevSibling();

	while ( node.IsValid() )
	{
		if ( node.GetNodeType() == MSXML_NODE_ELEMENT )
			break;

		node  = node.GePrevSibling();
	}

	return node;
}


template<class T>
CXMLDOMNode TXMLDOMNode<T>::GetNextSiblingElement() const
{
	CXMLDOMNode node  = GetNextSibling();

	while ( node.IsValid() )
	{
		if ( node.GetNodeType() == MSXML_NODE_ELEMENT )
			break;

		node  = node.GetNextSibling();
	}

	return node;
}



template<class T>
void TXMLDOMNode<T>::Dump() const
{
	BSTR		bstr = NULL;

	HRCHECK( p->get_xml(&bstr), "" );
	if (bstr)
	{
		wprintf(L"%s", bstr);

//		_bstr_t		bstr(bstr, false);
//		char*		p = bstr;
//		dprintf(_T("%s\n"), p);

	//	CString	str((LPCTSTR)bstr);

//		std::cout << p;
		
//		dprintf("XML DOM loaded from stocks.xml:\n%S\n",bstr);

		SysFreeString(bstr);
	}
}



// -----------------------------------------------------------------------------
// TXMLDOMNodeList
// -----------------------------------------------------------------------------
template<class T>
class XML_CLASS TXMLDOMNodeList : public CInterfaceCallingWrapper<T>
{
// construction/destruction
public:
	TXMLDOMNodeList() : CInterfaceCallingWrapper<T>() {}
	TXMLDOMNodeList(const TXMLDOMNodeList<T>& o);
	TXMLDOMNodeList(T *lp, bool bIncrementRefCount = TRUE);
	virtual ~TXMLDOMNodeList();

// method wrappers
public:
	/** collection of nodes */
	CXMLDOMNode GetItem(XMLIndex_t index) const;
	MSXML_IXMLDOMNode* GetNodeItem(XMLIndex_t index) const;

	/** number of nodes in the collection */
	XMLCount_t GetLength() const;
	XMLCount_t GetCount() const { return GetLength(); } // Alias of GetLength

	/** get next node from iterator */
	CXMLDOMNode GetNextNode() const;
	MSXML_IXMLDOMNode* GetNextNodeItem() const;

	/** reset the position of iterator */
	void Reset();
	
	IUnknown* GetNewEnum() const;

	const CXMLDOMNode operator [](XMLIndex_t index) const { return GetItem(index); }
	CXMLDOMNode operator [](XMLIndex_t index) { return GetItem(index); }

#if SUPPORT_CSTRING
	CString GetItemName(XMLIndex_t index) const { return GetItem(index).GetNodeName(); }
	bool GetItemName(XMLIndex_t index, CString &name) const { return GetItem(index).GetNodeName(name); }
	CString GetItemValue(XMLIndex_t index) const { return GetItem(index).GetNodeValue(); }
	CString GetItemText(XMLIndex_t index) const { return GetItem(index).GetText(); }
	bool GetItemText(XMLIndex_t index, CString &text) const { return GetItem(index).GetText(text); }

#else
	_bstr_t GetItemName(XMLIndex_t index) const { return GetItem(index).GetNodeName(); }
	_bstr_t GetItemText(XMLIndex_t index) const { return GetItem(index).GetText(); }
#endif

	bool GetItemName(XMLIndex_t index, TString<unicode_t>& oName) const { return GetItem(index).GetNodeName(oName); }
	_variant_t GetItemValueVar(XMLIndex_t index) const { return GetItem(index).GetNodeValueVar(); }
	bool GetItemValue(XMLIndex_t index, TString<unicode_t>& oValue) const { return GetItem(index).GetNodeValue(oValue); }
	bool GetItemText(XMLIndex_t index, TString<unicode_t>& oText) const { return GetItem(index).GetText(oText); }
 
#if SUPPORT_CASTING_OP
	operator CXMLDOMNodeList() const
	{
		return CXMLDOMNodeList(p, TRUE);
	}
#endif
};

template<class T>
TXMLDOMNodeList<T>::TXMLDOMNodeList(T *lp, bool bIncrementRefCount /*= TRUE*/)
:	CInterfaceCallingWrapper<T>(lp, bIncrementRefCount)
{
}

template<class T>
TXMLDOMNodeList<T>::TXMLDOMNodeList(const TXMLDOMNodeList<T> &o)
:	CInterfaceCallingWrapper<T>(o)
{
}

template<class T>
TXMLDOMNodeList<T>::~TXMLDOMNodeList()
{
}

template<class T>
MSXML_IXMLDOMNode* TXMLDOMNodeList<T>::GetNodeItem(XMLIndex_t index) const
{
	MSXML_IXMLDOMNode* o = NULL;

	HRCHECK( p->get_item(index, &o), "" );

	return o;
}

template<class T>
CXMLDOMNode TXMLDOMNodeList<T>::GetItem(XMLIndex_t index) const
{
	MSXML_IXMLDOMNode* o = NULL;

	HRCHECK( p->get_item(index, &o), "" );

	return CXMLDOMNode(o, FALSE);
}

template<class T>
XMLCount_t TXMLDOMNodeList<T>::GetLength() const
{
	XMLCount_t o = 0;

	HRCHECK( p->get_length(&o), "" );

	return o;
}

template<class T>
MSXML_IXMLDOMNode* TXMLDOMNodeList<T>::GetNextNodeItem() const
{
	MSXML_IXMLDOMNode* o = NULL;

	HRCHECK( p->nextNode(&o), "" );

	return o;
}

template<class T>
CXMLDOMNode TXMLDOMNodeList<T>::GetNextNode() const
{
	MSXML_IXMLDOMNode* o = NULL;

	HRCHECK( p->nextNode(&o), "" );

	return CXMLDOMNode(o, FALSE);
}

template<class T>
void TXMLDOMNodeList<T>::Reset()
{
	HRCHECK( p->reset(), "" );
}

template<class T>
IUnknown* TXMLDOMNodeList<T>::GetNewEnum() const
{
	IUnknown* o = NULL;

	HRCHECK( p->get__newEnum(&o), "" );

	return o;
}


// -----------------------------------------------------------------------------
// CXMLDOMNamedNodeMap
// -----------------------------------------------------------------------------
class XML_CLASS CXMLDOMNamedNodeMap : public CInterfaceCallingWrapper<MSXML_IXMLDOMNamedNodeMap>
{
// construction/destruction
public:
	CXMLDOMNamedNodeMap();
	CXMLDOMNamedNodeMap(MSXML_IXMLDOMNamedNodeMap *lp, bool bIncrementRefCount = TRUE);
	CXMLDOMNamedNodeMap(const CXMLDOMNamedNodeMap &o);
	virtual ~CXMLDOMNamedNodeMap() {}

// method wrappers
public:
	/** lookup item by name */
	CXMLDOMNode GetNamedItem(LPCSTR name) const;
	CXMLDOMNode GetNamedItem(LPCWSTR name) const;

	const CXMLDOMNode operator [](LPCSTR name) const { return GetNamedItem(name); }
	CXMLDOMNode operator [](LPCWSTR name) { return GetNamedItem(name); }

	/** set item by name */
	CXMLDOMNode SetNamedItem(MSXML_IXMLDOMNode* newItem);

	/** remove item by name */
	CXMLDOMNode RemoveNamedItem(LPCSTR name);
	CXMLDOMNode RemoveNamedItem(LPCWSTR name);

	/** collection of nodes */
	CXMLDOMNode GetItem(XMLIndex_t index) const;

	const CXMLDOMNode operator [](XMLIndex_t index) const { return GetItem(index); }
	CXMLDOMNode operator [](XMLIndex_t index) { return GetItem(index); }

	/** number of nodes in the collection */
	XMLCount_t GetLength() const;

	/** lookup the item by name and namespace */
	CXMLDOMNode GetQualifiedItem(LPCSTR baseName, LPCSTR namespaceURI) const;
	CXMLDOMNode GetQualifiedItem(LPCWSTR baseName, LPCWSTR namespaceURI) const;

	/** remove the item by name and namespace */
	CXMLDOMNode RemoveQualifiedItem(LPCSTR baseName, LPCSTR namespaceURI);
	CXMLDOMNode RemoveQualifiedItem(LPCWSTR baseName, LPCWSTR namespaceURI);

	/** reset the position of iterator */
	void Reset();

	/** get next node from iterator */
	CXMLDOMNode NextNode();

	IUnknown* GetNewEnum() const;

	/** collection of nodes */
#if SUPPORT_CSTRING
	CString GetItemName(XMLIndex_t index) const;
	bool GetItemName(XMLIndex_t index, CString &oName) const;
#endif
	bool GetItemName(XMLIndex_t index, TString<unicode_t>& oName) const;

	bool GetItemValue(XMLIndex_t index, TString<unicode_t>& oText) const;
#if SUPPORT_CSTRING
	CString GetItemText(XMLIndex_t index) const;
	bool GetItemText(XMLIndex_t index, CString &oText) const;
#endif
	bool GetItemText(XMLIndex_t index, TString<unicode_t>& oText) const;

	/** collection of nodes */
#if SUPPORT_CSTRING
	CString GetItemName(LPCTSTR nodeName) const
		{ return GetNamedItem(nodeName).GetNodeName(); }
	bool GetItemName(LPCTSTR nodeName, CString &outName) const
		{ return GetNamedItem(nodeName).GetNodeName(outName); }
#endif

	bool GetNamedItemValue(LPCSTR nodeName, TString<unicode_t>& outName) const
		{ return GetNamedItem(nodeName).GetNodeValue(outName); }
	bool GetNamedItemValue(LPCWSTR nodeName, TString<unicode_t>& outName) const
		{ return GetNamedItem(nodeName).GetNodeValue(outName); }

	/** collection of nodes */
#if SUPPORT_CSTRING
	CString GetItemText(LPCTSTR nodeName) const
		{ return GetNamedItem(nodeName).GetText(); }
	bool GetItemText(LPCTSTR nodeName, CString &outText) const
		{ return GetNamedItem(nodeName).GetText(outText); }
#endif

	bool GetNamedItemText(LPCSTR nodeName, TString<unicode_t>& outText) const
		{ return GetNamedItem(nodeName).GetText(outText); }
	bool GetNamedItemText(LPCWSTR nodeName, TString<unicode_t>& outText) const
		{ return GetNamedItem(nodeName).GetText(outText); }

	bool GetAttribute(LPCSTR nodeName, TString<unicode_t>& outName) {
		return GetNamedItem(nodeName).GetNodeValue(outName);
	}

	bool GetAttribute(LPCWSTR nodeName, TString<unicode_t>& outName) {
		return GetNamedItem(nodeName).GetNodeValue(outName);
	}

	_variant_t GetAttribute(LPCSTR nodeName) {
		return GetNamedItem(nodeName).GetNodeValueVar();
	}

	_variant_t GetAttribute(LPCWSTR nodeName) {
		return GetNamedItem(nodeName).GetNodeValueVar();
	}

	void SetAttribute(CXMLDOMDocument& doc, LPCSTR name, const _variant_t &value);
	void SetAttribute(CXMLDOMDocument& doc, LPCWSTR name, const _variant_t &value);
};


#if SUPPORT_CSTRING
inline CString CXMLDOMNamedNodeMap::GetItemName(XMLIndex_t index) const
{
	return GetItem(index).GetNodeName();
}
#endif

#if SUPPORT_CSTRING
inline bool CXMLDOMNamedNodeMap::GetItemName(XMLIndex_t index, CString &oName) const
{
	return GetItem(index).GetNodeName(oName);
}
#endif

inline bool CXMLDOMNamedNodeMap::GetItemName(XMLIndex_t index, TString<unicode_t>& oName) const
{
	return GetItem(index).GetNodeName(oName);
}

inline bool CXMLDOMNamedNodeMap::GetItemValue(XMLIndex_t index, TString<unicode_t>& oName) const
{
	return GetItem(index).GetNodeValue(oName);
}

#if SUPPORT_CSTRING
inline CString CXMLDOMNamedNodeMap::GetItemText(XMLIndex_t index) const
{
	return GetItem(index).GetText();
}

inline bool CXMLDOMNamedNodeMap::GetItemText(XMLIndex_t index, CString &oText) const
{
	return GetItem(index).GetText(oText);
}
#endif

inline bool CXMLDOMNamedNodeMap::GetItemText(XMLIndex_t index, TString<unicode_t>& oText) const
{
	return GetItem(index).GetText(oText);
}

inline void CXMLDOMNamedNodeMap::Reset()
{
	HRCHECK( p->reset(), "" );
}



//
// TXMLDOMNode
// 
template<class T>
CXMLDOMNamedNodeMap TXMLDOMNode<T>::GetAttributes() const
{
	MSXML_IXMLDOMNamedNodeMap* o = NULL;

	HRCHECK( p->get_attributes(&o), "" );

	return CXMLDOMNamedNodeMap(o, FALSE);
}


// -----------------------------------------------------------------------------
// CXMLDOMAttribute : inherited by TXMLDOMNode
// -----------------------------------------------------------------------------
class XML_CLASS CXMLDOMAttribute : public TXMLDOMNode<MSXML_IXMLDOMAttribute>
{
// construction/destruction
public:
	CXMLDOMAttribute();
	CXMLDOMAttribute(MSXML_IXMLDOMAttribute *lp, bool bIncrementRefCount = TRUE);
	CXMLDOMAttribute(const CXMLDOMAttribute &o);
	CXMLDOMAttribute(const CXMLDOMNode &o);
	virtual ~CXMLDOMAttribute() {}

// method wrappers
public:

	/** get name of the attribute. Read-only. */
#if SUPPORT_CSTRING
	CString GetName() const;
	void GetName(CString &name) const;
#endif
	bool GetName(TString<unicode_t>& outString) const;

	/** get name of the attribute. Read-only. */
#if SUPPORT_CSTRING
	CString GetValue() const;
	void GetValue(CString &value) const;
#else
	_variant_t GetValue() const;
#endif
	/** string value of the attribute */
	bool GetValue(TString<unicode_t>& outString) const;

	/** string value of the attribute */
	void SetValue(LPCSTR value);
	void SetValue(LPCWSTR value);
	void SetValue(const _variant_t& rhs);
};



// -----------------------------------------------------------------------------
// CXMLDOMElement
// -----------------------------------------------------------------------------
class XML_CLASS CXMLDOMElement : public TXMLDOMNode<MSXML_IXMLDOMElement>
{
// construction/destruction
public:
	CXMLDOMElement();
	CXMLDOMElement(const CXMLDOMElement &o);
	CXMLDOMElement(MSXML_IXMLDOMElement *lp, bool bIncrementRefCount = TRUE);
	CXMLDOMElement(const CXMLDOMNode &o);
	CXMLDOMElement(const MSXML_IXMLDOMNode *lp);
	virtual ~CXMLDOMElement() {}

// method wrappers
public:
//
// methods provided by MSXML_IXMLDOMElement
//
	/** get the tagName of the element. Read-only. */
#if SUPPORT_CSTRING
	CString GetTagName() const;
#endif
	bool GetTagName(TString<unicode_t>& outString) const;

	/** look up the string value of an attribute by name */
#if SUPPORT_CSTRING
	CString GetAttributeStr(LPCSTR name) const;
	CString GetAttributeStr(LPCWSTR name) const;
#endif

	_variant_t GetAttribute(LPCSTR name) const;
	_variant_t GetAttribute(LPCWSTR name) const;

	bool GetAttribute(LPCSTR name, TString<unicode_t> &outValue) const;
	bool GetAttribute(LPCWSTR name, TString<unicode_t> &outValue) const;

	/** set the string value of an attribute by name */
	void SetAttribute(LPCSTR name, LPCSTR value);
	void SetAttribute(LPCWSTR name, LPCWSTR value);
	void SetAttribute(LPCSTR name, const _variant_t& value);
	void SetAttribute(LPCWSTR name, const _variant_t& value);

	void SetAttributeNS(LPCWSTR namespaceUri, LPCWSTR name, LPCWSTR value);

	/** remove an attribute by name */
	void RemoveAttribute(LPCSTR name);
	void RemoveAttribute(LPCWSTR name);

	/** look up the attribute node by name */
	CXMLDOMAttribute GetAttributeNode(LPCSTR name) const;
	CXMLDOMAttribute GetAttributeNode(LPCWSTR name) const;

	/** set the specified attribute on the element */
	CXMLDOMAttribute SetAttributeNode(MSXML_IXMLDOMAttribute* domAttribute);

	/** remove the specified attribute */
	CXMLDOMAttribute RemoveAttributeNode(MSXML_IXMLDOMAttribute* domAttribute);

	/** build a list of elements by name */
	CXMLDOMNodeList GetElementsByTagName(LPCSTR tagName) const;
	CXMLDOMNodeList GetElementsByTagName(LPCWSTR tagName) const;

	/** collapse all adjacent text nodes in sub-tree */
	void Normalize();

    //  DOMElementTraversal interface
	CXMLDOMElement   GetFirstElementChild() const;
	CXMLDOMElement   GetLastElementChild() const;
	CXMLDOMElement   GetPreviousElementSibling() const;
	CXMLDOMElement   GetNextElementSibling() const;
	XMLCount_t       GetChildElementCount() const;

	void GetPath(TString<unicode_t> &oPath);

#if 0
	using TXMLDOMNode<MSXML_IXMLDOMElement>::operator=;
	CXMLDOMElement& operator =(const CXMLDOMNode &node)
	{
		if ( node.IsValid() )
		{
			MSXML_IXMLDOMElement	*lp = NULL;
			node.QueryInterface(&lp);
			AtlComPtrAssign((IUnknown**)&p, lp);
		}

		return *this;
	}
#endif
};


// -----------------------------------------------------------------------------
// class template TXMLDOMCharacterData
// -----------------------------------------------------------------------------
template<class T>
class XML_CLASS TXMLDOMCharacterData : public TXMLDOMNode<T>
{
// construction/destruction
public:
	TXMLDOMCharacterData() : TXMLDOMNode<T>() {}
	/// brief Generalised constructor: construct a TXMLDOMCharacterData from a derived type.
	template <typename DerivedType>
		TXMLDOMCharacterData<T> (const TXMLDOMCharacterData<DerivedType> &o) : TXMLDOMNode<T>() {
		//	ComPtrAssign(0, &p);
			_QueryInterface(o.p, &p);
		}

	TXMLDOMCharacterData(const TXMLDOMCharacterData<T> &o):TXMLDOMNode<T>(o) {}
	TXMLDOMCharacterData(T *lp, bool bIncrementRefCount = TRUE)
		:TXMLDOMNode<T>(lp, bIncrementRefCount) {}
	virtual ~TXMLDOMCharacterData() {}

	/** value of the node */
#if SUPPORT_CSTRING
	CString GetData() const;
#endif
	bool GetData(TString<unicode_t>& outString) const;

	/** value of the node */
	void SetData(LPCSTR rhs);
	void SetData(LPCWSTR rhs);

	/** number of characters in value */
	XMLCount_t GetLength() const;

	/** retrieve substring of value */
#if SUPPORT_CSTRING
	CString SubstringData(XMLIndex_t offset, XMLCount_t count);
#endif
	bool SubstringData(XMLIndex_t offset, XMLCount_t count, TString<unicode_t>& outString);

	/** append string to value */
	void AppendData(LPCTSTR data);

	/** insert string into value */
	void InsertData(XMLIndex_t offset, LPCTSTR data);

	/** delete string within the value */
	void DeleteData(XMLIndex_t offset, XMLCount_t count);

	/** replace string within the value */
	void ReplaceData(XMLIndex_t offset, XMLCount_t count, LPCTSTR data);

#if SUPPORT_CASTING_OP
	operator CXMLDOMCharacterData() const
	{
		return CXMLDOMCharacterData(p, TRUE);;
	}
#endif
};

#if SUPPORT_CSTRING
template<class T>
CString TXMLDOMCharacterData<T>::GetData() const
{
	BSTR	bstrO = NULL;

	HRCHECK( p->get_data(&bstrO), "" );

	return CString((LPCTSTR)_bstr_t(bstrO, false));
}
#endif

template<class T>
bool TXMLDOMCharacterData<T>::GetData(TString<unicode_t>& outString) const
{
	BSTR	bstrO = NULL;

	outString.Empty();
	RETURN_IF_FALSE( p->get_data(&bstrO) );

	if ( bstrO )
	{
		_bstr_t	bstr(bstrO, false);
		
		outString.Init((wchar_t*)bstr, bstr.length());
		return TRUE;
	}

	return FALSE;
}

template<class T>
void TXMLDOMCharacterData<T>::SetData(LPCSTR rhs)
{
	_bstr_t	bstrData(rhs);
	HRCHECK( p->put_data(bstrData), "" );
}

template<class T>
void TXMLDOMCharacterData<T>::SetData(LPCWSTR rhs)
{
	HRCHECK( p->put_data((unicode_t*)rhs), "" );
}


template<class T>
XMLCount_t TXMLDOMCharacterData<T>::GetLength() const
{
	XMLCount_t o = 0;

	HRCHECK( p->get_length(&o), "" );

	return o;
}

#if SUPPORT_CSTRING
template<class T>
CString TXMLDOMCharacterData<T>::SubstringData(XMLIndex_t offset, long count)
{
	BSTR	bstrO = NULL;

	HRCHECK( p->substringData(offset, count, &bstrO), "" );

	return CString((LPCTSTR)_bstr_t(bstrO, false));
}
#endif

template<class T>
bool TXMLDOMCharacterData<T>::SubstringData(XMLIndex_t offset, XMLCount_t count, TString<unicode_t>& outString)
{
	BSTR	bstrO = NULL;

	outString.Empty();
	RETURN_IF_FALSE( p->substringData(offset, count, &bstrO) );

	if ( bstrO )
	{
		_bstr_t	bstr(bstrO, false);
		
		outString.Init((wchar_t*)bstr, bstr.length());
		return TRUE;
	}

	return FALSE;
}

template<class T>
void TXMLDOMCharacterData<T>::AppendData(LPCTSTR data)
{
#if UNICODE_AWARE
	HRCHECK( p->appendData((unicode_t*)data), "" );
#else
	_bstr_t	bstrData(data);
	HRCHECK( p->appendData(bstrData), "" );
#endif
}

template<class T>
void TXMLDOMCharacterData<T>::InsertData(XMLIndex_t offset, LPCTSTR data)
{
#if UNICODE_AWARE
	HRCHECK( p->insertData(offset, (unicode_t*)data), "" );
#else
	_bstr_t	bstrData(data);
	HRCHECK( p->insertData(offset, bstrData), "" );
#endif
}

template<class T>
void TXMLDOMCharacterData<T>::DeleteData(XMLIndex_t offset, XMLCount_t count)
{
	HRCHECK( p->deleteData(offset, count), "" );
}

template<class T>
void TXMLDOMCharacterData<T>::ReplaceData(XMLIndex_t offset, XMLCount_t count, LPCTSTR data)
{
#if UNICODE_AWARE
	HRCHECK( p->replaceData(offset, count, (unicode_t*)data), "" );
#else
	_bstr_t	bstrData(data);
	HRCHECK( p->replaceData(offset, count, bstrData), "" );
#endif
}




// -----------------------------------------------------------------------------
// class template TXMLDOMText
// -----------------------------------------------------------------------------
template<class T>
class XML_CLASS TXMLDOMText : public TXMLDOMCharacterData<T>
{
// construction/destruction
public:
	TXMLDOMText() : TXMLDOMCharacterData<T>() {}

	/// brief Generalised constructor: construct a TXMLDOMCharacterData from a derived type.
	template <typename DerivedType>
		TXMLDOMText<T> (const TXMLDOMText<DerivedType> &o) : TXMLDOMCharacterData<T>() {
			_QueryInterface(o.p, &p);
		}

	TXMLDOMText(const TXMLDOMNode<T> &o) : TXMLDOMCharacterData<T>() {
		_QueryInterface(o.p, &p);
	}

	TXMLDOMText(const TXMLDOMText<T> &o) : TXMLDOMCharacterData<T>(o) {}

	TXMLDOMText(T *lp, bool bIncrementRefCount = TRUE)
		:TXMLDOMCharacterData<T>(lp, bIncrementRefCount) {}
	virtual ~TXMLDOMText() {}

	// split the text node into two text nodes at the position specified
	TXMLDOMText<T> SplitText(XMLIndex_t offset);

#if SUPPORT_CASTING_OP
	operator CXMLDOMText() const
	{
		return CXMLDOMText(p, TRUE);
	}
#endif
};

template<class T>
TXMLDOMText<T> TXMLDOMText<T>::SplitText(XMLIndex_t offset)
{
	MSXML_IXMLDOMText* o = NULL;

	HRCHECK( p->splitText(offset, &o), "");

	return TXMLDOMText<o, FALSE>;
}



// -----------------------------------------------------------------------------
// CXMLDOMDocumentType
// -----------------------------------------------------------------------------
class XML_CLASS CXMLDOMDocumentType : public TXMLDOMNode<MSXML_IXMLDOMDocumentType>
{
// construction/destruction
public:
	CXMLDOMDocumentType();
	CXMLDOMDocumentType(MSXML_IXMLDOMDocumentType *lp, bool bIncrementRefCount = TRUE);
	CXMLDOMDocumentType(const CXMLDOMDocumentType &o);
	CXMLDOMDocumentType(const CXMLDOMNode &o);
	virtual ~CXMLDOMDocumentType() {}

	/** name of the document type (root of the tree) */
#if SUPPORT_CSTRING
	CString GetName() const;
#endif
	bool GetName(TString<unicode_t>& outString) const;

	/** a list of entities in the document */
	CXMLDOMNamedNodeMap GetEntities() const;

	/** a list of notations in the document */
	CXMLDOMNamedNodeMap GetNotations() const;
};



// -----------------------------------------------------------------------------
// CXMLDOMProcessingInstruction
// -----------------------------------------------------------------------------
class XML_CLASS CXMLDOMProcessingInstruction : public TXMLDOMNode<MSXML_IXMLDOMProcessingInstruction>
{
// construction/destruction
public:
	CXMLDOMProcessingInstruction();
	CXMLDOMProcessingInstruction(MSXML_IXMLDOMProcessingInstruction *lp, bool bIncrementRefCount = TRUE);
	CXMLDOMProcessingInstruction(const CXMLDOMProcessingInstruction &o);
	CXMLDOMProcessingInstruction(const CXMLDOMNode &o);
	virtual ~CXMLDOMProcessingInstruction() {}

// method wrappers
public:

	/** the target */
#if SUPPORT_CSTRING
	CString GetTarget() const;
#endif
	bool GetTarget(TString<unicode_t>& outString) const;

	/** the data */
#if SUPPORT_CSTRING
	CString GetData() const;
#endif
	bool GetData(TString<unicode_t>& outString) const;

	/** the data */
	void SetData(LPCSTR rhs);
	void SetData(LPCWSTR rhs);
};



// -----------------------------------------------------------------------------
// CXMLDOMNotation
// -----------------------------------------------------------------------------
class XML_CLASS CXMLDOMNotation : public TXMLDOMNode<MSXML_IXMLDOMNotation>
{
// construction/destruction
public:
	CXMLDOMNotation();
	CXMLDOMNotation(MSXML_IXMLDOMNotation *lp, bool bIncrementRefCount = TRUE);
	CXMLDOMNotation(const CXMLDOMNotation &o);
	CXMLDOMNotation(const CXMLDOMNode &o);
	virtual ~CXMLDOMNotation() {}

// method wrappers
public:
	/** the public ID */
	_variant_t GetPublicId() const;

	/** the system ID */
	_variant_t GetSystemId() const;
};



// -----------------------------------------------------------------------------
// CXMLDOMEntity
// -----------------------------------------------------------------------------
class XML_CLASS CXMLDOMEntity : public TXMLDOMNode<MSXML_IXMLDOMEntity>
{
// construction/destruction
public:
	CXMLDOMEntity();
	CXMLDOMEntity(MSXML_IXMLDOMEntity *lp, bool bIncrementRefCount = TRUE);
	CXMLDOMEntity(const CXMLDOMEntity &o);
	CXMLDOMEntity(const CXMLDOMNode &o);
	virtual ~CXMLDOMEntity() {}

// method wrappers
public:

	/** the public ID */
	_variant_t GetPublicId() const;

	/** the system ID */
	_variant_t GetSystemId() const;

	/** the name of the notation */
#if SUPPORT_CSTRING
	CString GetNotationName() const;
#endif
	bool GetPublicId(TString<unicode_t>& outString) const;
	bool GetSystemId(TString<unicode_t>& outString) const;
	bool GetNotationName(TString<unicode_t>& outString) const;
};



// -----------------------------------------------------------------------------
// CXMLDOMImplementation
// -----------------------------------------------------------------------------
class XML_CLASS CXMLDOMImplementation : public CInterfaceCallingWrapper<MSXML_IXMLDOMImplementation>
{
// construction/destruction
public:
	CXMLDOMImplementation() : CInterfaceCallingWrapper<MSXML_IXMLDOMImplementation>() {}
	CXMLDOMImplementation(MSXML_IXMLDOMImplementation* lp, bool bIncrementRefCount = TRUE)
		: CInterfaceCallingWrapper<MSXML_IXMLDOMImplementation>(lp, bIncrementRefCount) {}
	CXMLDOMImplementation(const CXMLDOMImplementation &o) 
		: CInterfaceCallingWrapper<MSXML_IXMLDOMImplementation>(o) {}
	virtual ~CXMLDOMImplementation() {}

	bool HasFeature(LPCTSTR feature, LPCTSTR version) const;
};




// -----------------------------------------------------------------------------
// CXMLDOMParseError
// -----------------------------------------------------------------------------
class XML_CLASS CXMLDOMParseError : public CInterfaceCallingWrapper<MSXML_IXMLDOMParseError>
{
// construction/destruction
public:
	CXMLDOMParseError():CInterfaceCallingWrapper<MSXML_IXMLDOMParseError>() {}
	CXMLDOMParseError(MSXML_IXMLDOMParseError *lp, bool bIncrementRefCount = TRUE);
	CXMLDOMParseError(const CXMLDOMParseError&o);
	virtual ~CXMLDOMParseError() {};

// method wrappers
public:
	/** the error code */
	long GetErrorCode() const;

	/** the URL of the XML document containing the error */
#if SUPPORT_CSTRING
	CString GetURL() const;
#endif
	bool GetURL(TString<unicode_t>& outString) const;

	/** the cause of the error */
#if SUPPORT_CSTRING
	CString GetReason(void) const;
#endif
	bool GetReason(TString<unicode_t>& outString) const;

	/** the data where the error occurred */
#if SUPPORT_CSTRING
	CString GetSrcText(void) const;
#endif
	bool GetSrcText(TString<unicode_t>& outString) const;

	/** the line number in the XML document where the error occurred */
	long GetLine() const;

	/** the character position in the line containing the error */
	long GetLinepos() const;

	/** the absolute file position in the XML document containing the error */
	long GetFilepos() const;
};



// -----------------------------------------------------------------------------
// CXMLDOMDocument
// -----------------------------------------------------------------------------
template<class T>
class XML_CLASS TXMLDOMDocument : public TXMLDOMNode<T>
{
public:
	explicit TXMLDOMDocument(bool doCteate = FALSE);
	TXMLDOMDocument(T *lp, bool bIncrementRefCount = TRUE);

	// brief Generalised constructor: construct a TXMLDOMDocument from a derived type.
	template <typename DerivedType>
		TXMLDOMDocument<T> (const TXMLDOMDocument<DerivedType> &o) : TXMLDOMNode<T>() {
			mVersion  = o.GetVersion();

			_QueryInterface(o.p, &p);
		}

	TXMLDOMDocument(const TXMLDOMDocument<T> &o);
	virtual ~TXMLDOMDocument();

	virtual bool	CreateDocument() { return CreateInstance(); }
	virtual bool	CreateDocument(const unicode_t *qualifiedName,
								   const unicode_t *namespaceURI = NULL,
								   MSXML_IXMLDOMDocumentType *doctype = NULL);
	virtual bool	CreateDocument(const char *qualifiedName,
								   const char *namespaceURI = NULL,
								   MSXML_IXMLDOMDocumentType *doctype = NULL);

	virtual bool Create();
//	bool CreateInstance(REFCLSID rclsid = CLS_DOMDOCUMENT);
	bool CreateInstance(LPCOLESTR lpszProgID = XMLDOMDOCUMENT_ProgID, unsigned short version = MSXML_VERSION);

	virtual bool Open(LPCSTR lpszXMLFile) {
		if ( !Create() ) return false;
		return LoadFile(lpszXMLFile);
	};

	virtual bool Open(LPCWSTR lpszXMLFile) {
		if ( !Create() ) return false;
		return LoadFile(lpszXMLFile);
	};

	virtual bool OpenString(LPCWSTR lpXML) {
		if ( !Create() ) return false;
		return LoadXML(lpXML);
	};

	/** load document from the specified XML File */
	bool LoadFile(LPCSTR lpszXMLFile) { return Load(lpszXMLFile); }
	bool LoadFile(LPCWSTR lpszXMLFile) { return Load(lpszXMLFile); }
	bool Load(LPCSTR lpszXMLFile);
	bool Load(LPCWSTR lpszXMLFile);
	/** load document from the specified XML source in Memory*/
	bool LoadXML(const char* lpXML);
	bool LoadXML(const unicode_t* lpXML);
	bool Load(const _variant_t& xmlSource);

	/** save document to the specified XML File */
	bool Save(LPCSTR pszXMLFilePath);
	bool Save(LPCWSTR pszXMLFilePath);
//	void SaveURL(LPCTSTR pszXMLFilePath) { Save(pszXMLFilePath); }

	TXMLDOMDocument<T> GetXMLDOMDocument() { return TXMLDOMDocument<T>(p); }
	MSXML_IXMLDOMDocument* GetIXMLDOMDocument() { return p; }

// method wrappers
public:
	/** node corresponding to the DOCTYPE */
	CXMLDOMDocumentType GetDoctype() const;

	/** info on this DOM implementation */
	CXMLDOMImplementation GetImplementation() const;

	/** the root of the tree */
	CXMLDOMElement GetDocumentElement() const;

	/** the root of the tree */
	void SetRefDocumentElement(MSXML_IXMLDOMElement* rhs);

	/** create an Element node */
	CXMLDOMElement CreateElement(LPCSTR tagName);
	CXMLDOMElement CreateElement(LPCWSTR tagName);

	CXMLDOMElement CreateElement(LPCSTR tagName, LPCSTR namespaceURI) {
		return CreateNode(MSXML_NODE_ELEMENT, tagName, namespaceURI);
	}
	CXMLDOMElement CreateElement(LPCWSTR tagName, LPCWSTR namespaceURI) {
		return CreateNode(MSXML_NODE_ELEMENT, tagName, namespaceURI);
	}

	CXMLDOMElement CreateElementNS(LPCWSTR namespaceURI,LPCWSTR qualifiedName) {
		return CreateNode(MSXML_NODE_ELEMENT, qualifiedName, namespaceURI);
	}

	CXMLDOMElement CreateElementNS(LPCSTR namespaceURI, LPCSTR qualifiedName) {
		return CreateNode(MSXML_NODE_ELEMENT, qualifiedName, namespaceURI);
	}

	/** create a DocumentFragment node */
	CXMLDOMDocumentFragment CreateDocumentFragment();

	/** create a text node */
	CXMLDOMText CreateTextNode(LPCSTR data);
	CXMLDOMText CreateTextNode(LPCWSTR data);

	/** create a comment node */
	CXMLDOMComment CreateComment(LPCSTR data);
	CXMLDOMComment CreateComment(LPCWSTR data);

	/** create a CDATA section node */
	CXMLDOMCDATASection CreateCDATASection(LPCSTR data);
	CXMLDOMCDATASection CreateCDATASection(LPCWSTR data);

	/** create a processing instruction node */
	CXMLDOMProcessingInstruction CreateProcessingInstruction(LPCSTR target, LPCSTR data);
	CXMLDOMProcessingInstruction CreateProcessingInstruction(LPCWSTR target, LPCWSTR data);

	/** create an attribute node */
	CXMLDOMAttribute CreateAttribute(LPCSTR name);
	CXMLDOMAttribute CreateAttribute(LPCWSTR name);

    CXMLDOMAttribute CreateAttributeNS(LPCWSTR namespaceURI, LPCWSTR qualifiedName) {
		return CreateNode(MSXML_NODE_ATTRIBUTE, qualifiedName, namespaceURI);
	}

    CXMLDOMAttribute CreateAttributeNS(LPCSTR namespaceURI, LPCSTR qualifiedName) {
		return CreateNode(MSXML_NODE_ATTRIBUTE, qualifiedName, namespaceURI);
	}

	/** create an entity reference node */
	CXMLDOMEntityReference CreateEntityReference(LPCSTR name);
	CXMLDOMEntityReference CreateEntityReference(LPCWSTR name);

	/** build a list of elements by name */
	CXMLDOMNodeList GetElementsByTagName(LPCSTR tagName) const;
	CXMLDOMNodeList GetElementsByTagName(LPCWSTR tagName) const;

	/** create a node of the specified node type and name */
	CXMLDOMNode CreateNode(const _variant_t& type, LPCSTR name, LPCSTR namespaceURI);
	CXMLDOMNode CreateNode(const _variant_t& type, LPCWSTR name, LPCWSTR namespaceURI);

	/** retrieve node from it's ID */
	CXMLDOMNode NodeFromID(LPCSTR idString) const;
	CXMLDOMNode NodeFromID(LPCWSTR idString) const;

	CXMLDOMElement GetElementById(LPCSTR elementId) const { return NodeFromID(elementId); }
	CXMLDOMElement GetElementById(LPCWSTR elementId) const { return NodeFromID(elementId); }

	/** get the state of the XML document */
	long GetReadyState() const;

	/** get the URL for the loaded XML document */
#if SUPPORT_CSTRING
	CString GetURL() const;
#endif
	bool GetURL(TString<unicode_t>& outString) const;

	/** flag for asynchronous download */
	bool GetAsync() const;

	/** flag for asynchronous download */
	void SetAsync(bool rhs);

	/** abort an asynchronous download */
	void Abort();

	/** save the document to a specified destination */
	bool Save(const _variant_t& destination);

	/** indicates whether the parser performs validation */
	bool GetValidateOnParse() const;

	/** indicates whether the parser performs validation */
	void SetValidateOnParse(bool rhs);

	/** indicates whether the parser resolves references to external DTD/Entities/Schema */
	bool GetResolveExternals() const;

	/** indicates whether the parser resolves references to external DTD/Entities/Schema */
	void SetResolveExternals(bool rhs);

	/** indicates whether the parser preserves whitespace */
	bool GetPreserveWhiteSpace() const;

	/** indicates whether the parser preserves whitespace */
	void SetPreserveWhiteSpace(bool rhs);

	/** register a readystatechange event handler */
	void SetOnReadyStateChange(const _variant_t& rhs);

	/** register an ondataavailable event handler */
	void SetOnDataAvailable(const _variant_t& rhs);

	/** register an ontransformnode event handler */
	void SetOnTransformNode(const _variant_t& rhs);

	// XMLDOMParseError
	/** get the last parser error */
	CXMLDOMParseError GetParseError()  const;

	unsigned short	GetVersion() const { return mVersion; }

#if SUPPORT_CASTING_OP
	operator CXMLDOMDocument() const
	{
		return CXMLDOMDocument(p, TRUE);
	}
#endif

protected:
	unsigned short	mVersion;
//	friend class CXMLDOMDocument2;
};


template<class T>
TXMLDOMDocument<T>::TXMLDOMDocument(bool doCteate/* = FALSE*/)
: TXMLDOMNode<T>()
{
	mVersion  = 0;
	INIT_COM();

	if ( doCteate )
	{
		Create();
	}
}


template<class T>
TXMLDOMDocument<T>::TXMLDOMDocument(T *lp, bool bIncrementRefCount /*= TRUE*/)
:	TXMLDOMNode<T>(lp, bIncrementRefCount)
{
	mVersion  = 0;
	INIT_COM();
}


template<class T>
TXMLDOMDocument<T>::TXMLDOMDocument(const TXMLDOMDocument<T> &o):TXMLDOMNode<T>(o)
{
	mVersion  = 0;
	INIT_COM();
}


template<class T>
TXMLDOMDocument<T>::~TXMLDOMDocument()
{
	DONE_COM();
}

template<class T>
bool TXMLDOMDocument<T>::CreateDocument(const unicode_t *qualifiedName,
										const unicode_t *namespaceURI/* = NULL*/,
										MSXML_IXMLDOMDocumentType *doctype/* = NULL*/)
{
	if ( CreateDocument() )
	{
		CXMLDOMElement elem = CreateElement(qualifiedName);
		if ( elem.IsValid() )
		{
		//	elem.SetAttribute(
			return AppendChild(elem).IsValid();
		}
	}

	return false;
}

template<class T>
bool TXMLDOMDocument<T>::CreateDocument(const char *qualifiedName,
										const char *namespaceURI/* = NULL*/,
										MSXML_IXMLDOMDocumentType *doctype/* = NULL*/)
{
	return  CreateDocument( (const unicode_t *)bstr_t(qualifiedName), 
							(const unicode_t *)bstr_t(namespaceURI),
							doctype);
}


template<class T>
bool TXMLDOMDocument<T>::Create()
{
//	if ( p != NULL ) return true;

	if ( CreateInstance() )
	{
		HRCHECK( p->put_async(VARIANT_FALSE), "should never fail");
		HRCHECK( p->put_validateOnParse(VARIANT_FALSE), "should never fail");
		HRCHECK( p->put_resolveExternals(VARIANT_FALSE), "should never fail");
		HRCHECK( p->put_preserveWhiteSpace(VARIANT_TRUE), "" );

	//	SetAsync(FALSE);
	//	SetValidateOnParse(FALSE);
	//	SetResolveExternals(FALSE);
	//	SetPreserveWhiteSpace(FALSE);

		return true;
	}

	return false;
}

#if 0
template<class T>
bool TXMLDOMDocument<T>::CreateInstance(REFCLSID rclsid /*= CLS_DOMDOCUMENT*/)
{
	if ( p != NULL ) return TRUE;

	RETURN_FLASE_IF_HRCALL_FAILED( CoCreateInstance(rclsid), "Create a new DOMDocument" );

	LPOLESTR	lpszProgID;
	ProgIDFromCLSID(rclsid, &lpszProgID);

	if ( _tcscmp(lpszProgID, XML_DOMDOCUMENT40_PROGID) == 0 )
		mVersion = MSXML_VERSION_40;
	else if ( _tcscmp(lpszProgID, XML_DOMDOCUMENT60_PROGID) == 0 )
		mVersion = MSXML_VERSION_60;
	else if ( _tcscmp(lpszProgID, XML_DOMDOCUMENT50_PROGID) == 0 )
		mVersion = MSXML_VERSION_50;
	else if ( _tcscmp(lpszProgID, XML_DOMDOCUMENT30_PROGID) == 0 )
		mVersion = MSXML_VERSION_30;
	else if ( _tcscmp(lpszProgID, XML_DOMDOCUMENT26_PROGID) == 0 )
		mVersion = MSXML_VERSION_26;
	else
		mVersion = version;

	return TRUE;
}
#endif


// Helper function to create a DOM instance:
template<class T>
bool TXMLDOMDocument<T>::CreateInstance(LPCOLESTR lpszProgID/* = XMLDOMDOCUMENT_ProgID*/, unsigned short version/* = MSXML_VERSION*/)
{
	if ( p != NULL ) return TRUE;

/*
	UUID	Uuid;
	RPC_STATUS status = ::UuidCreate(&Uuid);

	unsigned char *StringUuid = NULL;
	status = ::UuidToString(&Uuid, (unsigned char**)&StringUuid);
	if ( StringUuid )
		RpcStringFree(&StringUuid);
*/

//	HRESULT hr = ::CoCreateInstance(rclsid, NULL, CLSCTX_ALL, __uuidof(T), (void**)&p);
/*
	// IID_IXMLDOMDocument2 = __uuidof(IXMLDOMDocument2)
	// IID_IXMLDOMDocument = __uuidof(IXMLDOMDocument)
	m_HResult = ::CoCreateInstance(CLS_DOMDocument, NULL, CLSCTX_ALL, __uuidof(T), (void**)&p);
	if ( FAILED(m_HResult) ) 
	{
		dprintf( "%s:%d  HRCALL Failed: %s\n  0x%.8x = %s\n", __FILE__, __LINE__, "Create a new DOMDocument", m_HResult, "CoCreateInstance" );
		return FALSE;
	}
*/

	CLSID	clsid;

	if ( lpszProgID != NULL && *lpszProgID != 0)
	{
		m_HResult = CLSIDFromProgID(lpszProgID, &clsid);
		if ( SUCCEEDED(m_HResult) )
		{
			m_HResult = CoCreateInstance(clsid);
			if ( SUCCEEDED(m_HResult) )
			{
				if ( wcscmp(lpszProgID, XML_DOMDOCUMENT40_PROGID) == 0 )
					mVersion = MSXML_VERSION_40;
				else if ( wcscmp(lpszProgID, XML_DOMDOCUMENT60_PROGID) == 0 )
					mVersion = MSXML_VERSION_60;
				else if ( wcscmp(lpszProgID, XML_DOMDOCUMENT50_PROGID) == 0 )
					mVersion = MSXML_VERSION_50;
				else if ( wcscmp(lpszProgID, XML_DOMDOCUMENT30_PROGID) == 0 )
					mVersion = MSXML_VERSION_30;
				else if ( wcscmp(lpszProgID, XML_DOMDOCUMENT26_PROGID) == 0 )
					mVersion = MSXML_VERSION_26;
				else
					mVersion = version;

			//	mVersion = version;
				return TRUE;
			}
		}
	}

	m_HResult = CLSIDFromProgID(XML_DOMDOCUMENT40_PROGID, &clsid);
	if ( SUCCEEDED(m_HResult) )
	{
		m_HResult = CoCreateInstance(clsid);
		if ( SUCCEEDED(m_HResult) )
		{
			mVersion = MSXML_VERSION_40;
			return TRUE;
		}
	}

	m_HResult = CLSIDFromProgID(XML_DOMDOCUMENT60_PROGID, &clsid);
	if ( SUCCEEDED(m_HResult) )
	{
		m_HResult = CoCreateInstance(clsid);
		if ( SUCCEEDED(m_HResult) )
		{
			mVersion = MSXML_VERSION_60;
			return TRUE;
		}
	}

	m_HResult = CLSIDFromProgID(XML_DOMDOCUMENT50_PROGID, &clsid);
	if ( SUCCEEDED(m_HResult) )
	{
		m_HResult = CoCreateInstance(clsid);
		if ( SUCCEEDED(m_HResult) )
		{
			mVersion = MSXML_VERSION_50;
			return TRUE;
		}
	}

	m_HResult = CLSIDFromProgID(XML_DOMDOCUMENT30_PROGID, &clsid);
	if ( SUCCEEDED(m_HResult) )
	{
		m_HResult = CoCreateInstance(clsid);
		if ( SUCCEEDED(m_HResult) )
		{
			mVersion = MSXML_VERSION_30;
			return TRUE;
		}
	}

/*
	m_HResult = CLSIDFromProgID(_T("Msxml2.DOMDocument.2.6"), &clsid);
	if ( SUCCEEDED(m_HResult) )
	{
		m_HResult = CoCreateInstance(clsid);
		if ( SUCCEEDED(m_HResult) )
		{
			mVersion = MSXML_VERSION_26;
			return TRUE;
		}
	}
*/

/*
	m_HResult = CoCreateInstance(CLS_DOMDocument40);
	if ( FAILED(m_HResult) ) 
	{
		m_HResult = CoCreateInstance(CLS_DOMDocument);
		if ( FAILED(m_HResult) ) 
			return FALSE;

	}
*/

	RETURN_FLASE_IF_HRCALL_FAILED( CoCreateInstance(CLS_DOMDOCUMENT), "Create a new DOMDocument" );
//	RETURN_FLASE_IF_HRCALL_FAILED( CoCreateInstance(rclsid), "Create a new DOMDocument" );

	return TRUE;
}

template<class T>
bool TXMLDOMDocument<T>::Load(LPCSTR lpszXMLFile)
{
	VARIANT_BOOL	status = VARIANT_FALSE;
	_variant_t		varXml(lpszXMLFile);

	HRCHECK( p->load(varXml, &status), "" );

	return status == VARIANT_TRUE;
}

template<class T>
bool TXMLDOMDocument<T>::Load(LPCWSTR lpszXMLFile)
{
	VARIANT_BOOL	status = VARIANT_FALSE;
	_variant_t		varXml(lpszXMLFile);

	HRCHECK( p->load(varXml, &status), "" );

	return status == VARIANT_TRUE;
}


/*
template<class T>
bool TXMLDOMDocument<T>::Load(const CString& xmlFile)
{
	VARIANT_BOOL	status = VARIANT_FALSE;

#if 0
	_bstr_t			bstr(xmlFile);
	VARIANT			varXml;
	VariantInit(&varXml);

	V_VT(&varXml) = VT_BSTR;
	V_BSTR(&varXml) = bstr;
#else
	_variant_t		varXml(xmlFile);	
#endif

//	V_BSTR(&var) = xmlFile.AllocSysString();

	HRCHECK( p->load(varXml, &status), "" );

//	if (&var)
//	   VariantClear(&var);

	return status == VARIANT_TRUE;
}
*/

template<class T>
bool TXMLDOMDocument<T>::Load(const _variant_t& xmlSource)
{
//	VARIANT	varxmlSource = varXMLFile.Detach();
	VARIANT_BOOL	status = VARIANT_FALSE;

	HRCHECK( p->load(xmlSource, &status), "" );

//	VariantClear(&varxmlSource);
	return status == VARIANT_TRUE;
}


template<class T>
bool TXMLDOMDocument<T>::LoadXML(LPCSTR lpXML)
{
	VARIANT_BOOL	status = VARIANT_FALSE;

	_bstr_t			bstr(lpXML);
	HRCHECK( p->loadXML(bstr, &status), "" );

	return status == VARIANT_TRUE;
}


template<class T>
bool TXMLDOMDocument<T>::LoadXML(LPCWSTR lpXML)
{
	VARIANT_BOOL	status = VARIANT_FALSE;

	HRCHECK( p->loadXML((unicode_t*)lpXML, &status), "" );

	return status == VARIANT_TRUE;
}


template<class T>
bool TXMLDOMDocument<T>::Save(LPCSTR pszXMLFilePath)
{
//	VARIANT	vardestination = destination.Detach();
	_variant_t	destination(pszXMLFilePath);

	HRCHECK( p->save(destination), "" );

	return SUCCEEDED(m_HResult);

//	VariantClear(&vardestination);
}

template<class T>
bool TXMLDOMDocument<T>::Save(LPCWSTR pszXMLFilePath)
{
//	VARIANT	vardestination = destination.Detach();
	_variant_t	destination(pszXMLFilePath);

	HRCHECK( p->save(destination), "" );
	return SUCCEEDED(m_HResult);

//	VariantClear(&vardestination);
}

template<class T>
CXMLDOMParseError TXMLDOMDocument<T>::GetParseError() const
{
	MSXML_IXMLDOMParseError	*pXMLErr = NULL;

	HRCHECK( p->get_parseError(&pXMLErr), "" );

	return CXMLDOMParseError(pXMLErr, FALSE);
}


template<class T>
CXMLDOMDocumentType TXMLDOMDocument<T>::GetDoctype() const
{
	MSXML_IXMLDOMDocumentType* o = NULL;

	HRCHECK( p->get_doctype(&o), "" );

	return CXMLDOMDocumentType(o, FALSE);
}

template<class T>
CXMLDOMImplementation TXMLDOMDocument<T>::GetImplementation() const
{
	MSXML_IXMLDOMImplementation *o = NULL

	HRCHECK( p->get_implementation(&o), "" );

	return CXMLDOMImplementation(o, FALSE);
}


template<class T>
CXMLDOMElement TXMLDOMDocument<T>::GetDocumentElement() const
{
	MSXML_IXMLDOMElement* o = NULL;

	HRCHECK( p->get_documentElement(&o), "" );

	return CXMLDOMElement(o, FALSE);
}


template<class T>
void TXMLDOMDocument<T>::SetRefDocumentElement(MSXML_IXMLDOMElement* rhs)
{
	HRCHECK( p->putref_documentElement(rhs), "" );
}


template<class T>
CXMLDOMElement TXMLDOMDocument<T>::CreateElement(LPCSTR tagName)
{
	MSXML_IXMLDOMElement* o = NULL;

	_bstr_t		bstrTagName(tagName);
	HRCHECK( p->createElement(bstrTagName, &o), "" );

	return CXMLDOMElement(o, FALSE);
}

template<class T>
CXMLDOMElement TXMLDOMDocument<T>::CreateElement(LPCWSTR tagName)
{
	MSXML_IXMLDOMElement* o = NULL;

	HRCHECK( p->createElement((unicode_t*)tagName, &o), "" );

	return CXMLDOMElement(o, FALSE);
}


template<class T>
CXMLDOMText TXMLDOMDocument<T>::CreateTextNode(LPCSTR data)
{
	MSXML_IXMLDOMText* o = NULL;

	_bstr_t		bstrData(data);
	HRCHECK( p->createTextNode(bstrData, &o), "" );

	return CXMLDOMText(o, FALSE);
}

template<class T>
CXMLDOMText TXMLDOMDocument<T>::CreateTextNode(LPCWSTR data)
{
	MSXML_IXMLDOMText* o = NULL;

	HRCHECK( p->createTextNode((unicode_t*)data, &o), "" );

	return CXMLDOMText(o, FALSE);
}

template<class T>
CXMLDOMDocumentFragment TXMLDOMDocument<T>::CreateDocumentFragment()
{
	MSXML_IXMLDOMDocumentFragment* o = NULL;

	HRCHECK( p->createDocumentFragment(&o), "" );

	return CXMLDOMDocumentFragment(o, FALSE);
}


template<class T>
CXMLDOMComment TXMLDOMDocument<T>::CreateComment(LPCSTR data)
{
	MSXML_IXMLDOMComment* o = NULL;

	_bstr_t		bstrData(data);
	HRCHECK( p->createComment(bstrData, &o), "" );

	return CXMLDOMComment(o, FALSE);
}

template<class T>
CXMLDOMComment TXMLDOMDocument<T>::CreateComment(LPCWSTR data)
{
	MSXML_IXMLDOMComment* o = NULL;
	HRCHECK( p->createComment((unicode_t*)data, &o), "" );
	return CXMLDOMComment(o, FALSE);
}

template<class T>
CXMLDOMCDATASection TXMLDOMDocument<T>::CreateCDATASection(LPCSTR data)
{
	MSXML_IXMLDOMCDATASection* o = NULL;

	_bstr_t		bstrData(data);
	HRCHECK( p->createCDATASection(bstrData, &o), "" );

	return CXMLDOMCDATASection(o, FALSE);
}

template<class T>
CXMLDOMCDATASection TXMLDOMDocument<T>::CreateCDATASection(LPCWSTR data)
{
	MSXML_IXMLDOMCDATASection* o = NULL;

	HRCHECK( p->createCDATASection((unicode_t*)data, &o), "" );

	return CXMLDOMCDATASection(o, FALSE);
}


template<class T>
CXMLDOMProcessingInstruction TXMLDOMDocument<T>::CreateProcessingInstruction(LPCSTR target, LPCSTR data)
{
	MSXML_IXMLDOMProcessingInstruction* o = NULL;

	_bstr_t		bstrTarget(target);
	_bstr_t		bstrData(data);

	HRCHECK( p->createProcessingInstruction(bstrTarget, bstrData, &o), "" );

	return CXMLDOMProcessingInstruction(o, FALSE);
}

template<class T>
CXMLDOMProcessingInstruction TXMLDOMDocument<T>::CreateProcessingInstruction(LPCWSTR target, LPCWSTR data)
{
	MSXML_IXMLDOMProcessingInstruction* o = NULL;

	HRCHECK( p->createProcessingInstruction((unicode_t*)target, (unicode_t*)data, &o), "" );

	return CXMLDOMProcessingInstruction(o, FALSE);
}

template<class T>
CXMLDOMAttribute TXMLDOMDocument<T>::CreateAttribute(LPCSTR name)
{
	MSXML_IXMLDOMAttribute* o = NULL;

	_bstr_t		bstrName(name);
	HRCHECK( p->createAttribute(bstrName, &o), "" );

	return CXMLDOMAttribute(o, FALSE);
}

template<class T>
CXMLDOMAttribute TXMLDOMDocument<T>::CreateAttribute(LPCWSTR name)
{
	MSXML_IXMLDOMAttribute* o = NULL;
	HRCHECK( p->createAttribute((unicode_t*)name, &o), "" );
	return CXMLDOMAttribute(o, FALSE);
}

template<class T>
CXMLDOMEntityReference TXMLDOMDocument<T>::CreateEntityReference(LPCSTR name)
{
	MSXML_IXMLDOMEntityReference* o = NULL;

	_bstr_t		bstrName(name);
	HRCHECK( p->createEntityReference(bstrName, &o), "" );

	return CXMLDOMEntityReference(o, FALSE);
}

template<class T>
CXMLDOMEntityReference TXMLDOMDocument<T>::CreateEntityReference(LPCWSTR name)
{
	MSXML_IXMLDOMEntityReference* o = NULL;

	HRCHECK( p->createEntityReference((unicode_t*)name, &o), "" );

	return CXMLDOMEntityReference(o, FALSE);
}

template<class T>
CXMLDOMNodeList TXMLDOMDocument<T>::GetElementsByTagName(LPCSTR tagName) const
{
	MSXML_IXMLDOMNodeList* o = NULL;

	_bstr_t		bstrTagName(tagName);
	HRCHECK( p->getElementsByTagName(bstrTagName, &o), "" );

	return CXMLDOMNodeList(o, FALSE);
}

template<class T>
CXMLDOMNodeList TXMLDOMDocument<T>::GetElementsByTagName(LPCWSTR tagName) const
{
	MSXML_IXMLDOMNodeList* o = NULL;

	HRCHECK( p->getElementsByTagName((unicode_t*)tagName, &o), "" );

	return CXMLDOMNodeList(o, FALSE);
}

template<class T>
CXMLDOMNode TXMLDOMDocument<T>::CreateNode(const _variant_t& type, LPCSTR name, LPCSTR namespaceURI)
{
	MSXML_IXMLDOMNode* o = NULL;

	_bstr_t		bstrName(name);
	_bstr_t		bstrNamespaceURI(namespaceURI);

	HRCHECK( p->createNode(type, bstrName, bstrNamespaceURI, &o), "" );

	return CXMLDOMNode(o, FALSE);
}

template<class T>
CXMLDOMNode TXMLDOMDocument<T>::CreateNode(const _variant_t& type, LPCWSTR name, LPCWSTR namespaceURI)
{
	MSXML_IXMLDOMNode* o = NULL;

	HRCHECK( p->createNode(type, (unicode_t*)name, (unicode_t*)namespaceURI, &o), "" );

	return CXMLDOMNode(o, FALSE);
}


template<class T>
CXMLDOMNode TXMLDOMDocument<T>::NodeFromID(LPCSTR idString) const
{
	MSXML_IXMLDOMNode* o = NULL;

	_bstr_t		bstrIDString(idString);
	HRCHECK( p->nodeFromID(bstrIDString, &o), "" );

	return CXMLDOMNode(o, FALSE);
}

template<class T>
CXMLDOMNode TXMLDOMDocument<T>::NodeFromID(LPCWSTR idString) const
{
	MSXML_IXMLDOMNode* o = NULL;

	HRCHECK( p->nodeFromID((unicode_t*)idString, &o), "" );

	return CXMLDOMNode(o, FALSE);
}

template<class T>
long TXMLDOMDocument<T>::GetReadyState() const
{
	long o = 0;

	HRCHECK( p->get_readyState(&o), "" );

	return o;
}

#if SUPPORT_CSTRING
template<class T>
CString TXMLDOMDocument<T>::GetURL() const
{
	BSTR	bstrO = NULL;

	HRCHECK( p->get_url(&bstrO), "" );

	return CString((LPCTSTR)_bstr_t(bstrO, false));
}
#endif

template<class T>
bool TXMLDOMDocument<T>::GetURL(TString<unicode_t>& outString) const
{
	BSTR	bstrO = NULL;

	outString.Empty();
	RETURN_IF_FALSE( p->get_url(&bstrO) );

	if ( bstrO )
	{
		_bstr_t	bstr(bstrO, false);
		outString.Init((wchar_t*)bstr, bstr.length());
		return TRUE;
	}

	return FALSE;
}


template<class T>
bool TXMLDOMDocument<T>::GetAsync() const
{
	VARIANT_BOOL	status = VARIANT_FALSE;

	HRCHECK( p->get_async(&status), "" );

	return status == VARIANT_TRUE;
}


template<class T>
void TXMLDOMDocument<T>::SetAsync(bool rhs)
{
	HRCHECK( p->put_async(rhs ? VARIANT_TRUE : VARIANT_FALSE), "" );
}


template<class T>
void TXMLDOMDocument<T>::Abort()
{
	HRCHECK( p->abort(), "" );
}


template<class T>
bool TXMLDOMDocument<T>::Save(const _variant_t& destination)
{
//	VARIANT	vardestination = destination.Detach();

	HRCHECK( p->save(destination), "" );
	return SUCCEEDED(m_HResult);

//	VariantClear(&vardestination);
}


template<class T>
bool TXMLDOMDocument<T>::GetValidateOnParse() const
{
	VARIANT_BOOL	status = VARIANT_FALSE;

	HRCHECK( p->get_validateOnParse(&status), "" );

	return status == VARIANT_TRUE;
}


template<class T>
void TXMLDOMDocument<T>::SetValidateOnParse(bool rhs)
{
	HRCHECK( p->put_validateOnParse(rhs ? VARIANT_TRUE : VARIANT_FALSE), "" );
}


template<class T>
bool TXMLDOMDocument<T>::GetResolveExternals() const
{
	VARIANT_BOOL	status = VARIANT_FALSE;

	HRCHECK( p->get_resolveExternals(&status), "" );

	return status == VARIANT_TRUE;
}


template<class T>
void TXMLDOMDocument<T>::SetResolveExternals(bool rhs)
{
	HRCHECK( p->put_resolveExternals(rhs ? VARIANT_TRUE : VARIANT_FALSE), "" );

	
}


template<class T>
bool TXMLDOMDocument<T>::GetPreserveWhiteSpace() const
{
	VARIANT_BOOL	status = VARIANT_FALSE;

	HRCHECK( p->get_preserveWhiteSpace(&status), "" );

	return status == VARIANT_TRUE;
}


template<class T>
void TXMLDOMDocument<T>::SetPreserveWhiteSpace(bool rhs)
{
	HRCHECK( p->put_preserveWhiteSpace(rhs ? VARIANT_TRUE : VARIANT_FALSE), "" );
}


template<class T>
void TXMLDOMDocument<T>::SetOnReadyStateChange(const _variant_t& rhs)
{
//	VARIANT	varrhs = rhs.Detach();
	HRCHECK( p->put_onreadystatechange(rhs), "" );
//	VariantClear(&varrhs);
}


template<class T>
void TXMLDOMDocument<T>::SetOnDataAvailable(const _variant_t& rhs)
{
//	VARIANT	varrhs = rhs.Detach();
	HRCHECK( p->put_ondataavailable(rhs), "" );
//	VariantClear(&varrhs);
}


template<class T>
void TXMLDOMDocument<T>::SetOnTransformNode(const _variant_t& rhs)
{
//	VARIANT	varrhs = rhs.Detach();
	HRCHECK( p->put_ontransformnode(rhs), "" );
//	VariantClear(&varrhs);
}



// -----------------------------------------------------------------------------
// CXMLDOMDocument2
// -----------------------------------------------------------------------------
class XML_CLASS CXMLDOMDocument2 : public TXMLDOMDocument<MSXML_IXMLDOMDocument2>
{
// construction/destruction
public:
	explicit CXMLDOMDocument2(bool doCteate = FALSE);
	CXMLDOMDocument2(MSXML_IXMLDOMDocument2 *lp, bool bIncrementRefCount = TRUE);
	CXMLDOMDocument2(const CXMLDOMDocument2 &o);
	CXMLDOMDocument2(const CXMLDOMDocument &o);

	virtual bool Create();

#if 0
	virtual bool Open(LPCSTR lpszXMLFile) {
		if ( !Create() ) return false;
		return Load(lpszXMLFile);
	};

	virtual bool Open(LPCWSTR lpszXMLFile) {
		if ( !Create() ) return false;
		return Load(lpszXMLFile);
	};

	bool Load(LPCSTR lpszXMLFile);
	bool Load(LPCWSTR lpszXMLFile);
#endif

	/** A collection of all namespaces for this document */
	CXMLDOMSchemaCollection GetNamespaces() const;

	/** The associated schema cache */
	_variant_t GetSchemas() const;

	/** The associated schema cache */
	void SetRefSchemas(const _variant_t &rhs);

	/** perform runtime validation on the currently loaded XML document */
	CXMLDOMParseError Validate() const;

	/** set the value of the named property */
	void SetProperty(LPCSTR name, const _variant_t &value) const;
	void SetProperty(LPCWSTR name, const _variant_t &value) const;

	/** get the value of the named property */
	_variant_t GetProperty(LPCSTR name) const;
	_variant_t GetProperty(LPCWSTR name) const;

	bool GetValidateOnParse() const;
	void SetValidateOnParse(bool rhs);
	bool GetResolveExternals() const;
	void SetResolveExternals(bool rhs);
	bool GetProhibitDTD() const;
	void SetProhibitDTD(bool rhs);

	void SelectionNamespaces(LPCSTR namespaces)  const { SetProperty("SelectionNamespaces", _variant_t(namespaces)); }
	void SelectionNamespaces(LPCWSTR namespaces) const { SetProperty(L"SelectionNamespaces", _variant_t(namespaces)); }

	void NamespaceBinding(LPCSTR namespaces)  const { SetProperty("SelectionNamespaces", _variant_t(namespaces)); }
	void NamespaceBinding(LPCWSTR namespaces) const { SetProperty(L"SelectionNamespaces", _variant_t(namespaces)); } 

//	friend class CXMLDOMDocument;
};



// -----------------------------------------------------------------------------
// CXMLDOMSelection
// -----------------------------------------------------------------------------

class XML_CLASS CXMLDOMSelection : public TXMLDOMNodeList<MSXML_IXMLDOMSelection>
{
// construction/destruction
public:
	CXMLDOMSelection();
	CXMLDOMSelection(MSXML_IXMLDOMSelection *lp, bool bIncrementRefCount = TRUE);
	CXMLDOMSelection(const CXMLDOMSelection &o);
	CXMLDOMSelection(const CXMLDOMNodeList &o);
	virtual ~CXMLDOMSelection() { }


// method wrappers
public:
	/** selection expression */
#if SUPPORT_CSTRING
	CString GetExpr() const;
#endif
	bool GetExpr(TString<unicode_t>& outString) const;

	/** selection expression */
	void SetExpr(LPCTSTR rhs);

	/** nodes to apply selection expression to */
	CXMLDOMNode GetContext() const;

	/** nodes to apply selection expression to */
	void SetRefContext(MSXML_IXMLDOMNode* rhs);

	/** gets the next node without advancing the list position */
	CXMLDOMNode PeekNode() const;

	/** checks to see if the node matches the pattern */
	CXMLDOMNode Matches(MSXML_IXMLDOMNode* pNode) const;

	/** removes the next node */
	CXMLDOMNode RemoveNext();

	/** removes all the nodes that match the selection */
	void RemoveAll();

	/** clone this object with the same position and context */
	CXMLDOMSelection Clone() const;

	/** get the value of the named property */
	_variant_t GetProperty(LPCTSTR name) const;

	/** set the value of the named property */
	void SetProperty(LPCTSTR name, const _variant_t &value);
};

/*
// -----------------------------------------------------------------------------
// TSchemaItem
// -----------------------------------------------------------------------------
template<class T>
class XML_CLASS TSchemaItem : public CInterfaceCallingWrapper<T>
{
	TSchemaItem() : CInterfaceCallingWrapper<T>() { }

	// brief Generalised constructor: construct a TXMLDOMDocument from a derived type.
	template <typename DerivedType>
	TXMLDOMSchemaCollection<T> (const TSchemaItem<DerivedType> &o) : CInterfaceCallingWrapper<T>() {
		#ifdef _DEBUG
		#endif
			_QueryInterface(o.p, &p);
	}
	
	TSchemaItem(const TSchemaItem &o) : CInterfaceCallingWrapper<T>(o) { }
	TSchemaItem(T *lp, bool bIncrementRefCount = TRUE)  : CInterfaceCallingWrapper<T>(lp, bIncrementRefCount) {}
	virtual ~TXMLDOMSchemaCollection() {}

	bool GetName(TString<unicode_t> &name);
	bool GetNamespaceURI(TString<unicode_t> &namespaceURI);
	CSchema GetSchema();
	bool GetId(TString<unicode_t> &id);
	SOMITEMTYPE GetItemType(TString<unicode_t> &itemType);
	CVBSAXAttributes GetUnhandledAttributes();

};
*/


// -----------------------------------------------------------------------------
// TXMLDOMSchemaCollection
// -----------------------------------------------------------------------------

template<class T>
class XML_CLASS TXMLDOMSchemaCollection : public CInterfaceCallingWrapper<T>
{
// construction/destruction
public:
	TXMLDOMSchemaCollection();

	// brief Generalised constructor: construct a TXMLDOMDocument from a derived type.
	template <typename DerivedType>
		TXMLDOMSchemaCollection<T> (const TXMLDOMSchemaCollection<DerivedType> &o) : CInterfaceCallingWrapper<T>() {
		#ifdef _DEBUG
		#endif
			_QueryInterface(o.p, &p);
		}
	TXMLDOMSchemaCollection(const TXMLDOMSchemaCollection &o);
	TXMLDOMSchemaCollection(T *lp, bool bIncrementRefCount = TRUE);
	virtual ~TXMLDOMSchemaCollection() {}

// method wrappers
public:
	/** add a new schema */
	void Add(LPCTSTR namespaceURI, const _variant_t &var);

	/** lookup schema by namespaceURI */
	CXMLDOMNode Get(LPCSTR namespaceURI) const;
	CXMLDOMNode Get(LPCWSTR namespaceURI) const;

	/** remove schema by namespaceURI */
	void Remove(LPCTSTR namespaceURI);

	/** number of schemas in collection */
	XMLCount_t GetLength() const;

	/** Get namespaceURI for schema by index */
#if SUPPORT_CSTRING
	CString GetNamespaceURI(XMLIndex_t index) const;
#endif
	bool GetNamespaceURI(XMLIndex_t index, TString<unicode_t>& outString) const;

	/** copy & merge other collection into this one */
	void AddCollection(MSXML_IXMLDOMSchemaCollection* otherCollection);

	IUnknown* GetNewEnum() const;

#if SUPPORT_CASTING_OP
	operator CXMLDOMSchemaCollection() const
	{
		return CXMLDOMSchemaCollection(p, TRUE);
	}
#endif
};


template<class T>
TXMLDOMSchemaCollection<T>::TXMLDOMSchemaCollection()
:	CInterfaceCallingWrapper<T>()
{}


template<class T>
TXMLDOMSchemaCollection<T>::TXMLDOMSchemaCollection(T *lp, bool bIncrementRefCount /*= TRUE*/)
:	CInterfaceCallingWrapper<T>(lp, bIncrementRefCount)
{}


template<class T>
TXMLDOMSchemaCollection<T>::TXMLDOMSchemaCollection(const TXMLDOMSchemaCollection &o)
:	CInterfaceCallingWrapper<T>(o)
{}

template<class T>
void TXMLDOMSchemaCollection<T>::Add(LPCTSTR namespaceURI, const _variant_t &var)
{
	_bstr_t		bstrNamespaceURI(namespaceURI);

	HRCHECK( p->add(bstrNamespaceURI, var), "" );
}

template<class T>
CXMLDOMNode TXMLDOMSchemaCollection<T>::Get(LPCSTR namespaceURI) const
{
	MSXML_IXMLDOMNode* o = NULL;

	_bstr_t		bstrNamespaceURI(namespaceURI);
	HRCHECK( p->get(bstrNamespaceURI, &o), "" );

	return CXMLDOMNode(o, FALSE);
}

template<class T>
CXMLDOMNode TXMLDOMSchemaCollection<T>::Get(LPCWSTR namespaceURI) const
{
	MSXML_IXMLDOMNode* o = NULL;

	_bstr_t		bstrNamespaceURI(namespaceURI);
	HRCHECK( p->get(bstrNamespaceURI, &o), "" );

	return CXMLDOMNode(o, FALSE);
}

template<class T>
void TXMLDOMSchemaCollection<T>::Remove(LPCTSTR namespaceURI)
{
	_bstr_t		bstrNamespaceURI(namespaceURI);

	HRCHECK( p->remove(bstrNamespaceURI), "" );
}

template<class T>
XMLCount_t TXMLDOMSchemaCollection<T>::GetLength() const
{
	XMLCount_t o = 0;

	HRCHECK( p->get_length(&o), "" );

	return o;
}

#if SUPPORT_CSTRING
template<class T>
CString TXMLDOMSchemaCollection<T>::GetNamespaceURI(XMLIndex_t index) const
{
	BSTR	bstrO = NULL;

	HRCHECK( p->get_namespaceURI(index, &bstrO), "" );

	return CString((LPCTSTR)_bstr_t(bstrO, false));
}
#endif

template<class T>
bool TXMLDOMSchemaCollection<T>::GetNamespaceURI(XMLIndex_t index, TString<unicode_t>& outString) const
{
	BSTR	bstrO = NULL;

	outString.Empty();
	RETURN_IF_FALSE( p->get_namespaceURI(index, &bstrO) );

	if ( bstrO )
	{
		_bstr_t	bstr(bstrO, false);
		
		outString.Init((wchar_t*)bstr, bstr.length());
		return TRUE;
	}

	return FALSE;
}

template<class T>
void TXMLDOMSchemaCollection<T>::AddCollection(MSXML_IXMLDOMSchemaCollection* otherCollection)
{
	HRCHECK( p->addCollection(otherCollection), "" );
}

template<class T>
IUnknown* TXMLDOMSchemaCollection<T>::GetNewEnum() const
{
	IUnknown* o = NULL;

	HRCHECK( p->get__newEnum(&o), "" );

	return o;
}

#ifndef _WIN32_WCE
// -----------------------------------------------------------------------------
// CXMLDOMSchemaCollection2
// -----------------------------------------------------------------------------
class XML_CLASS CXMLDOMSchemaCollection2 : public TXMLDOMSchemaCollection<MSXML_IXMLDOMSchemaCollection2>
{
// construction/destruction
public:
	CXMLDOMSchemaCollection2();
	CXMLDOMSchemaCollection2(MSXML_IXMLDOMSchemaCollection2 *lp, bool bIncrementRefCount = TRUE);
	CXMLDOMSchemaCollection2(const CXMLDOMSchemaCollection2 &o);

// method wrappers
public:

	void Validate() const;

	void SetValidateOnLoad(bool rhs);

	bool GetValidateOnLoad() const;

	ISchema* GetSchema(LPCTSTR namespaceURI) const;

	MSXML_ISchemaItem* GetDeclaration(MSXML_IXMLDOMNode* node) const;
};

#endif	// !_WIN32_WCE

#if 0

// -----------------------------------------------------------------------------
// CSchemaItem
// -----------------------------------------------------------------------------
template<class T>
class XML_CLASS TSchemaItem : public CInterfaceCallingWrapper<MSXML_ISchemaItem>
{
// construction/destruction
public:
	TSchemaItem();
	TSchemaItem(MSXML_ISchemaItem *lp, bool bIncrementRefCount = TRUE);
	TSchemaItem(const TSchemaItem &schemaItem);

// method wrappers
public:
	/** Get Name */
	CString GetName() const;

	/** Get namespaceURI for schema by index */
	CString GetNamespaceURI() const;

	void GetSchema(CSchema &schema) const;

	CString GetID() const;

	SOMITEMTYPE GetItemType() const;

	IVBSAXAttributes* GetUnhandledAttributes() const;
	bool WriteAnnotation(IUnknown * annotationSink);
};


#endif



// -----------------------------------------------------------------------------
// CXSLTemplate
// -----------------------------------------------------------------------------
class XML_CLASS CXSLTemplate : public CInterfaceCallingWrapper<MSXML_IXSLTemplate>
{
// construction/destruction
public:
	CXSLTemplate();
	CXSLTemplate(MSXML_IXSLTemplate *lp, bool bIncrementRefCount = TRUE);
	CXSLTemplate(const CXSLTemplate &o);

// method wrappers
public:
	/** stylesheet to use with processors */
	void SetStylesheet(const MSXML_IXMLDOMNode* stylesheet);

	/** stylesheet to use with processors */
	CXMLDOMNode GetStylesheet() const;

	/** create a new processor object */
	CXSLProcessor CreateProcessor();
};



// -----------------------------------------------------------------------------
// CXSLProcessor
// -----------------------------------------------------------------------------
class XML_CLASS CXSLProcessor : public CInterfaceCallingWrapper<MSXML_IXSLProcessor>
{
// construction/destruction
public:
	CXSLProcessor();
	CXSLProcessor(MSXML_IXSLProcessor *lp, bool bIncrementRefCount = TRUE);
	CXSLProcessor(const CXSLProcessor &o);

// method wrappers
public:
	/** XML input tree to transform */
	void SetInput(const _variant_t &rhs);

	/** XML input tree to transform */
	_variant_t GetInput() const;

	/** template object used to create this processor object */
	CXSLTemplate GetOwnerTemplate() const;

	/** set XSL mode and it's namespace */
	void SetStartMode(LPCTSTR mode, LPCTSTR namespaceURI);

	/** starting XSL mode */
#if SUPPORT_CSTRING
	CString GetStartMode() const;
#endif
	bool GetStartMode(TString<unicode_t>& outString) const;

	/** namespace of starting XSL mode */
#if SUPPORT_CSTRING
	CString GetStartModeURI() const;
#endif
	bool GetStartModeURI(TString<unicode_t>& outString) const;

	/** custom stream object for transform output */
	void SetOutput(const _variant_t &rhs);

	/** custom stream object for transform output */
	_variant_t GetOutput() const;

	/** start/resume the XSL transformation process */
	bool Transform();

	/** reset state of processor and abort current transform */
	void Reset();

	/** current state of the processor */
	long GetReadyState() const;

	/** set <xsl:param> values */
	void AddParameter(LPCTSTR baseName, const _variant_t &parameter, LPCTSTR namespaceURI);

	/** pass object to stylesheet */
	void AddObject(IDispatch* obj, LPCTSTR namespaceURI);

	/** current stylesheet being used */
	CXMLDOMNode GetStylesheet() const;
};



//--------------------------------------------------------------------
// class CXMLHTTPRequest
//--------------------------------------------------------------------

class XML_CLASS CXMLHTTPRequest : public CInterfaceCallingWrapper<IXMLHTTPRequest>
{
// construction/destruction
public:
	CXMLHTTPRequest();
	CXMLHTTPRequest(IXMLHTTPRequest *lp, bool bIncrementRefCount = TRUE);
	CXMLHTTPRequest(const CXMLHTTPRequest &o);

// method wrappers
public:
	/** Open HTTP connection */
	void Open(LPCTSTR method, LPCTSTR url, const _variant_t &async, const _variant_t &user, const _variant_t &password);
	void Open(LPCTSTR method, LPCTSTR url, bool async, LPCTSTR user, LPCTSTR password);

	/** Add HTTP request header */
	void SetRequestHeader(LPCTSTR header, LPCTSTR value);

	/** Get HTTP response header */
#if SUPPORT_CSTRING
	CString GetResponseHeader(LPCTSTR header) const;
#endif
	bool GetResponseHeader(LPCTSTR header, TString<unicode_t>& outString) const;

	/** Get all HTTP response headers */
#if SUPPORT_CSTRING
	CString GetAllResponseHeaders() const;
#endif
	bool GetAllResponseHeaders(TString<unicode_t>& outString) const;

	/** Send HTTP request */
	void Send(_variant_t body);

	/** Abort HTTP request */
	void Abort();

	/** Get HTTP status code */
	long GetStatus() const;

	/** Get HTTP status text */
#if SUPPORT_CSTRING
	CString GetStatusText() const;
#endif
	bool GetStatusText(TString<unicode_t>& outString) const;

	/** Get response body */
	IDispatch* GetResponseXML() const;

	/** Get response body */
#if SUPPORT_CSTRING
	CString GetResponseText() const;
#endif
	bool GetResponseText(TString<unicode_t>& outString) const;

	/** Get response body */
	_variant_t GetResponseBody() const;

	/** Get response body */
	_variant_t GetResponseStream() const;

	/** Get ready state */
	long GetReadyState() const;

	/** Register a complete event handler */
	void SetOnReadyStateChange(IDispatch* rhs);

	bool Create(LPCOLESTR lpszProgID = XMLHTTP_ProgID);
	bool Create(REFCLSID rclsid = CLS_XMLHTTP);

};



//--------------------------------------------------------------------
// class CServerXMLHTTPRequest
//--------------------------------------------------------------------

class XML_CLASS CServerXMLHTTPRequest : public CInterfaceCallingWrapper<IServerXMLHTTPRequest>
{
// construction/destruction
public:
	CServerXMLHTTPRequest();
	CServerXMLHTTPRequest(IServerXMLHTTPRequest *lp, bool bIncrementRefCount = TRUE);
	CServerXMLHTTPRequest(const CServerXMLHTTPRequest &o);

// method wrappers
public:
	/** Open HTTP connection */
	void Open(LPCTSTR method, LPCTSTR url, _variant_t async, _variant_t user, _variant_t password);

	/** Add HTTP request header */
	void SetRequestHeader(LPCTSTR header, LPCTSTR value);

	/** Get HTTP response header */
#if SUPPORT_CSTRING
	CString GetResponseHeader(LPCTSTR header) const;
#endif
	bool GetResponseHeader(LPCTSTR header, TString<unicode_t>& outString) const;

	/** Get all HTTP response headers */
#if SUPPORT_CSTRING
	CString GetAllResponseHeaders() const;
#endif
	bool GetAllResponseHeaders(TString<unicode_t>& outString) const;

	/** Send HTTP request */
	void Send(_variant_t body);

	/** Abort HTTP request */
	void Abort();

	/** Get HTTP status code */
	long GetStatus() const;

	/** Get HTTP status text */
#if SUPPORT_CSTRING
	CString GetStatusText() const;
#endif
	bool GetStatusText(TString<unicode_t>& outString) const;

	/** Get response body */
	IDispatch* GetResponseXML() const;

	/** Get response body */
#if SUPPORT_CSTRING
	CString GetResponseText() const;
#endif
	bool GetResponseText(TString<unicode_t>& outString) const;

	/** Get response body */
	_variant_t GetResponseBody() const;

	/** Get response body */
	_variant_t GetResponseStream() const;

	/** Get ready state */
	long GetReadyState() const;

	/** Register a complete event handler */
	void SetOnReadyStateChange(IDispatch* rhs);

	/** Specify timeout settings (in milliseconds) */
	void SetTimeouts(long resolveTimeout, long connectTimeout, long sendTimeout, long receiveTimeout);

	/** Wait for asynchronous send to complete, with optional timeout (in seconds) */
	bool WaitForResponse(const _variant_t &timeoutInSeconds);

	/** Get an option value */
	_variant_t GetOption(SERVERXMLHTTP_OPTION option) const;

	/** Set an option value */
	void SetOption(SERVERXMLHTTP_OPTION option, const _variant_t &value);

	bool Create(LPCOLESTR lpszProgID = SERVER_XMLHTTP_ProgID);
	bool Create(REFCLSID rclsid = CLS_SERVER_XMLHTTP);

};



// ====================================================================================================== 

#if 0

class XML_CLASS CXMLHTTPClass
{
// static operations
public:
	static CXMLHTTPRequest CreateXMLHTTPRequest(LPUNKNOWN pUnkOuter = NULL, DWORD dwClsContext = CLSCTX_ALL);

// static attributes
public:
	static const CLSID m_ClsId;
};



class XML_CLASS CXMLHTTP26Class
{
// static operations
public:
	static CXMLHTTPRequest CreateXMLHTTPRequest(LPUNKNOWN pUnkOuter = NULL, DWORD dwClsContext = CLSCTX_ALL);

// static attributes
public:
	static const CLSID m_ClsId;
};



class XML_CLASS CXMLHTTP30Class
{
// static operations
public:
	static CXMLHTTPRequest CreateXMLHTTPRequest(LPUNKNOWN pUnkOuter = NULL, DWORD dwClsContext = CLSCTX_ALL);

// static attributes
public:
	static const CLSID m_ClsId;
};



class XML_CLASS CXMLHTTP40Class
{
// static operations
public:
	static CXMLHTTPRequest CreateXMLHTTPRequest(LPUNKNOWN pUnkOuter = NULL, DWORD dwClsContext = CLSCTX_ALL);

// static attributes
public:
	static const CLSID m_ClsId;
};



class XML_CLASS CServerXMLHTTPClass
{
// static operations
public:
	static IServerXMLHTTPRequest2* CreateServerXMLHTTPRequest2(LPUNKNOWN pUnkOuter = NULL, DWORD dwClsContext = CLSCTX_ALL);

// static attributes
public:
	static const CLSID m_ClsId;
};



class XML_CLASS CServerXMLHTTP30Class
{
// static operations
public:
	static IServerXMLHTTPRequest2* CreateServerXMLHTTPRequest2(LPUNKNOWN pUnkOuter = NULL, DWORD dwClsContext = CLSCTX_ALL);

// static attributes
public:
	static const CLSID m_ClsId;
};



class XML_CLASS CServerXMLHTTP40Class
{
// static operations
public:
	static IServerXMLHTTPRequest2* CreateServerXMLHTTPRequest2(LPUNKNOWN pUnkOuter = NULL, DWORD dwClsContext = CLSCTX_ALL);

// static attributes
public:
	static const CLSID m_ClsId;
};

#endif	// ===============================================================================


#if SUPPORT_CSTRING

#if _MSC_VER < 1299 
inline CString VarToCString(const _variant_t &var)
{
	if ( var.vt == VT_BSTR )
		return CString(var.bstrVal);

	return CString();
}
#else
#define VarToCString(a)	 
#endif

#endif	// SUPPORT_CSTRING


// ============================================================================
//
// xerces XML Parser와 호환을 위한 API
//
// ============================================================================

typedef CXMLDOMNodeList	SelectionNode;

// ----------------------------------------------------------------------------
// XPath
// ----------------------------------------------------------------------------
class XPath {
public:
	XPath(CXMLDOMDocument2 &domDoc) : mXMLDOMDocument(domDoc) {}
	XPath(CXMLDOMNode &domNode) { mXMLDOMDocument = domNode.GetOwnerDocument(); }
	~XPath() {}

	void SelectionNamespaces(const unicode_t* namespaces);
	void SelectionNamespaces(const char* namespaces);
	void AddNamespaceBinding(const unicode_t* prefix, const unicode_t* uri);
	void AddNamespaceBinding(const char* prefix, const char* uri);

	CXMLDOMNode SelectSingleNode(MSXML_IXMLDOMNode *domNode, const unicode_t *queryString);
	CXMLDOMNode SelectSingleNode(MSXML_IXMLDOMNode *domNode, const char *queryString);

	SelectionNode	SelectNodes(MSXML_IXMLDOMNode *domNode,
								const unicode_t *queryString);
	SelectionNode	SelectNodes(MSXML_IXMLDOMNode *domNode,
								const char *queryString );

	XMLCount_t	SelectNodes(MSXML_IXMLDOMNode *domNode,
						    const unicode_t *queryString,
						    SelectionNode &selectNodes);
	XMLCount_t	SelectNodes(MSXML_IXMLDOMNode *domNode,
						    const char *queryString,
						    SelectionNode &selectNodes);

	void Reset() { }

protected:
	CXMLDOMDocument2		mXMLDOMDocument;
//	TString<unicode_t>		mQueryString;
//	TString<unicode_t>		mSelectionNamespaces;
};


CXMLDOMElement MatchElement(CXMLDOMElement &contextNode, const unicode_t *elemPath,
							const unicode_t *attrName, const unicode_t *value);
CXMLDOMElement MatchElement(CXMLDOMElement &contextNode, const unicode_t *elemPath,
							const unicode_t *attrName, const unicode_t *value,
							const unicode_t *attrName1, const unicode_t *value1);



void GetXMLErrorMessage(const CXMLDOMDocument &xmlDomDoc, TString<unicode_t> &outMessage);



#endif	// __XML_DOM__


