/* ********************************************************************************
 *
 * Purpose:	implementation for the XMLDom class
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

#include "stdafx.h"
#include "XMLDom.h"

//#include <string.h>
//#include <stdio.h>
//#include <iostream>
//#include <rpc.h>


//--------------------------------------------------------------------
// CXMLDOMDocument2
//--------------------------------------------------------------------

CXMLDOMDocument2::CXMLDOMDocument2(bool doCteate/* = FALSE*/)
: TXMLDOMDocument<MSXML_IXMLDOMDocument2>(doCteate)
{
//	SetProperty("SelectionLanguage","XPath");
//	SetProperty("NewParser", VARIANT_TRUE);
}


CXMLDOMDocument2::CXMLDOMDocument2(MSXML_IXMLDOMDocument2 *lp, bool bIncrementRefCount /*= TRUE*/)
: TXMLDOMDocument<MSXML_IXMLDOMDocument2>(lp, bIncrementRefCount)
{}


CXMLDOMDocument2::CXMLDOMDocument2(const CXMLDOMDocument2 &o)
: TXMLDOMDocument<MSXML_IXMLDOMDocument2>(o)
{}


CXMLDOMDocument2::CXMLDOMDocument2(const CXMLDOMDocument &o)
: TXMLDOMDocument<MSXML_IXMLDOMDocument2>()
{
	_QueryInterface(o.p, &p);
//	AtlComPtrAssign((IUnknown**)&p, lp);
}

bool CXMLDOMDocument2::Create()
{
	if ( TXMLDOMDocument<MSXML_IXMLDOMDocument2>::Create() )	// CreateInstance(_T("Msxml2.DOMDocument.6.0")
	{
	//	HRCHECK( p->put_async(VARIANT_FALSE), "should never fail");
	//	HRCHECK( p->put_validateOnParse(VARIANT_FALSE), "should never fail");
	//	HRCHECK( p->put_resolveExternals(VARIANT_FALSE), "should never fail");
	//	HRCHECK( p->put_preserveWhiteSpace(VARIANT_TRUE), "" );

		if ( mVersion >= MSXML_VERSION_30 )
		{
			// Default : "XSLPattern" in MSXML 3.0; "XPath" for MSXML 4.0 and later
			SetProperty(L"SelectionLanguage", L"XPath");	// 3.0 and later
			// Default : false in 3.0 SP4 QFE and later, 4.0 SP2 QFE and later
			HRCHECK( p->setProperty(L"ProhibitDTD", _variant_t(false)), "ProhibitDTD fail" );
		//	HRCHECK( p->setProperty(_bstr_t(_T("ProhibitDTD")), _variant_t(false)), "ProhibitDTD fail" );
		//	SetProperty(L"ProhibitDTD", _variant_t(false));			// 5.0 and later, Default  : False
		}
	/*
		if ( mVersion >= MSXML_VERSION_40 )
		{
			SetProperty("NewParser", true);	// 4.0 and later, Default  : False
		}
	*/
		// DTD Validation 체크 안하기.
		if ( mVersion > MSXML_VERSION_40 )
		{
			HRCHECK( p->setProperty(L"ValidateOnParse", _variant_t(false)), "ValidateOnParse fail" );	// Default : TRUE
			HRCHECK( p->setProperty(L"ResolveExternals", _variant_t(false)), "ResolveExternals fail" );	// Default : TRUE

		//	SetProperty(L"ValidateOnParse", _variant_t(false));		// 5.0 and later, Default  : True
		//	SetProperty(L"ResolveExternals", _variant_t(false));	// 5.0 and later, Default  : True
		//	SetProperty("UseInlineSchema",false);					// 5.0 and later, Default  : True
		}
//		bool ok = GetValidateOnParse();

		return true;
	}

	return false;
}


CXMLDOMSchemaCollection CXMLDOMDocument2::GetNamespaces() const
{
	MSXML_IXMLDOMSchemaCollection* o = NULL;

	HRCHECK( p->get_namespaces(&o), "" );

	return CXMLDOMSchemaCollection(o, FALSE);
}


_variant_t CXMLDOMDocument2::GetSchemas() const
{
	VARIANT	var;
	V_VT(&var) = VT_EMPTY;

	HRCHECK( p->get_schemas(&var), "" );
	return _variant_t(var, false);
}


void CXMLDOMDocument2::SetRefSchemas(const _variant_t &rhs)
{
	HRCHECK( p->putref_schemas(rhs), "" );
}


CXMLDOMParseError CXMLDOMDocument2::Validate() const
{
	MSXML_IXMLDOMParseError* o = NULL;

	HRCHECK( p->validate(&o), "" );

	return CXMLDOMParseError(o, FALSE);
}


void CXMLDOMDocument2::SetProperty(LPCSTR name, const _variant_t &value) const
{
//	VARIANT	varvalue = value.Detach();
	_bstr_t		bstrname(name);
	HRCHECK( p->setProperty(bstrname, value), "setProperty failed" );
//	VariantClear(&varvalue);
}

void CXMLDOMDocument2::SetProperty(LPCWSTR name, const _variant_t &value) const
{
//	VARIANT	varvalue = value.Detach();
	HRCHECK( p->setProperty((unicode_t *)name, value), "setProperty failed" );
//	VariantClear(&varvalue);
}


_variant_t CXMLDOMDocument2::GetProperty(LPCSTR name) const
{
	_bstr_t		bstrname(name);
	VARIANT		var;
	V_VT(&var) = VT_EMPTY;

	HRCHECK( p->getProperty(bstrname, &var), "getProperty failed" );
	return _variant_t(var, false);
}
_variant_t CXMLDOMDocument2::GetProperty(LPCWSTR name) const
{
	VARIANT		var;
	V_VT(&var) = VT_EMPTY;

	HRCHECK( p->getProperty((unicode_t*)name, &var), "getProperty failed" );
	return _variant_t(var, false);
}


bool CXMLDOMDocument2::GetValidateOnParse() const 
{
	if ( mVersion > MSXML_VERSION_40 )
	{
		_variant_t	var;
		HRCHECK( p->getProperty(L"ValidateOnParse", &var), "" );
		return (bool)var;
	}

	return TXMLDOMDocument<MSXML_IXMLDOMDocument2>::GetValidateOnParse();
}

void CXMLDOMDocument2::SetValidateOnParse(bool rhs)
{
	SetProperty(L"ValidateOnParse", rhs);	// 5.0 and later, Default  : True
	TXMLDOMDocument<MSXML_IXMLDOMDocument2>::SetValidateOnParse(rhs);
}

bool CXMLDOMDocument2::GetResolveExternals() const
{
	if ( mVersion > MSXML_VERSION_40 )
	{
		_variant_t	var;
		HRCHECK( p->getProperty(L"ResolveExternals", &var), "" );
		return (bool)var;
	}

	return TXMLDOMDocument<MSXML_IXMLDOMDocument2>::GetResolveExternals();
}

void CXMLDOMDocument2::SetResolveExternals(bool rhs)
{
	SetProperty(L"ResolveExternals", rhs);	// 5.0 and later, Default  : True
	TXMLDOMDocument<MSXML_IXMLDOMDocument2>::SetResolveExternals(rhs);
}

bool CXMLDOMDocument2::GetProhibitDTD() const
{
	if ( mVersion > MSXML_VERSION_40 )
	{
		_variant_t	var;
		HRCHECK( p->getProperty(L"ProhibitDTD", &var), "" );
		return (bool)var;
	}

	return TXMLDOMDocument<MSXML_IXMLDOMDocument2>::GetResolveExternals();
}

void CXMLDOMDocument2::SetProhibitDTD(bool rhs)
{
	// 3.0 SP4 QFE and later, 4.0 SP2 QFE and later, Default  : false
	SetProperty(L"ProhibitDTD", rhs);
}


#if 0
bool CXMLDOMDocument2::Load(LPCSTR lpszXMLFile)
{
	bool ok = TXMLDOMDocument<MSXML_IXMLDOMDocument2>::LoadFile(lpszXMLFile);

	if (ok )
	{
		CXMLDOMSchemaCollection	schemaCollection = GetNamespaces();
		TString<unicode_t> url;
		TString<unicode_t> name;
		CXMLDOMNode	node;
		XMLCount_t	count = schemaCollection.GetLength();
		for ( XMLIndex_t i = 0; i < count; i++ )
		{
			schemaCollection.GetNamespaceURI(i, url);
			node = schemaCollection.Get(url);
			node.GetNodeName(name);
		}
	}

	return ok;
}

bool CXMLDOMDocument2::Load(LPCWSTR lpszXMLFile)
{
	bool ok = TXMLDOMDocument<MSXML_IXMLDOMDocument2>::LoadFile(lpszXMLFile);

	if (ok )
	{
		CXMLDOMSchemaCollection	schemaCollection = GetNamespaces();
		TString<unicode_t> url;
		TString<unicode_t> name;
		CXMLDOMNode	node;
		XMLCount_t	count = schemaCollection.GetLength();
		for ( XMLIndex_t i = 0; i < count; i++ )
		{
			schemaCollection.GetNamespaceURI(i, url);
			node = schemaCollection.Get(url);
			node.GetNodeName(name);
		}
	}

	return ok;
}
#endif


//--------------------------------------------------------------------
// class CXMLDOMNamedNodeMap
//--------------------------------------------------------------------

CXMLDOMNamedNodeMap::CXMLDOMNamedNodeMap()
:	CInterfaceCallingWrapper<MSXML_IXMLDOMNamedNodeMap>()
{
}


CXMLDOMNamedNodeMap::CXMLDOMNamedNodeMap(MSXML_IXMLDOMNamedNodeMap *lp, bool bIncrementRefCount /*= TRUE*/)
:	CInterfaceCallingWrapper<MSXML_IXMLDOMNamedNodeMap>(lp, bIncrementRefCount)
{
}


CXMLDOMNamedNodeMap::CXMLDOMNamedNodeMap(const CXMLDOMNamedNodeMap &o)
:	CInterfaceCallingWrapper<MSXML_IXMLDOMNamedNodeMap>(o)
{
}

CXMLDOMNode CXMLDOMNamedNodeMap::GetNamedItem(LPCSTR name) const
{
	MSXML_IXMLDOMNode* o = NULL;

	_bstr_t		bstrName(name);
	HRCHECK( p->getNamedItem(bstrName, &o), "" );

	return CXMLDOMNode(o, FALSE);
}

CXMLDOMNode CXMLDOMNamedNodeMap::GetNamedItem(LPCWSTR name) const
{
	MSXML_IXMLDOMNode* o = NULL;

	HRCHECK( p->getNamedItem((unicode_t*)name, &o), "" );

	return CXMLDOMNode(o, FALSE);
}

CXMLDOMNode CXMLDOMNamedNodeMap::SetNamedItem(MSXML_IXMLDOMNode* newItem)
{
	MSXML_IXMLDOMNode* o = NULL;

	HRCHECK( p->setNamedItem(newItem, &o), "" );

	return CXMLDOMNode(o, FALSE);
}


CXMLDOMNode CXMLDOMNamedNodeMap::RemoveNamedItem(LPCSTR name)
{
	MSXML_IXMLDOMNode* o = NULL;

	_bstr_t		bstrName(name);
	HRCHECK(  p->removeNamedItem(bstrName, &o), "" );

	return CXMLDOMNode(o, FALSE);
}

CXMLDOMNode CXMLDOMNamedNodeMap::RemoveNamedItem(LPCWSTR name)
{
	MSXML_IXMLDOMNode* o = NULL;

	HRCHECK(  p->removeNamedItem((unicode_t*)name, &o), "" );

	return CXMLDOMNode(o, FALSE);
}

CXMLDOMNode CXMLDOMNamedNodeMap::GetItem(XMLIndex_t index) const
{
	MSXML_IXMLDOMNode* o = NULL;

	HRCHECK( p->get_item(index, &o), "" );

	return CXMLDOMNode(o, FALSE);
}


XMLCount_t CXMLDOMNamedNodeMap::GetLength() const
{
	XMLCount_t o = 0;

	HRCHECK( p->get_length(&o), "" );

	return o;
}


CXMLDOMNode CXMLDOMNamedNodeMap::GetQualifiedItem(LPCSTR baseName, LPCSTR namespaceURI) const
{
	MSXML_IXMLDOMNode* o = NULL;

	_bstr_t		bstrBaseName(baseName);
	_bstr_t		bstrNameSpaceURI(namespaceURI);

	HRCHECK( p->getQualifiedItem(bstrBaseName, bstrNameSpaceURI, &o), "" );

	return CXMLDOMNode(o, FALSE);
}

CXMLDOMNode CXMLDOMNamedNodeMap::GetQualifiedItem(LPCWSTR baseName, LPCWSTR namespaceURI) const
{
	MSXML_IXMLDOMNode* o = NULL;

	HRCHECK( p->getQualifiedItem((unicode_t*)baseName, (unicode_t*)namespaceURI, &o), "" );

	return CXMLDOMNode(o, FALSE);
}

CXMLDOMNode CXMLDOMNamedNodeMap::RemoveQualifiedItem(LPCSTR baseName, LPCSTR namespaceURI)
{
	MSXML_IXMLDOMNode* o = NULL;

	_bstr_t		bstrBaseName(baseName);
	_bstr_t		bstrNameSpaceURI(namespaceURI);

	HRCHECK( p->removeQualifiedItem(bstrBaseName, bstrNameSpaceURI, &o), "" );

	return CXMLDOMNode(o, FALSE);
}

CXMLDOMNode CXMLDOMNamedNodeMap::RemoveQualifiedItem(LPCWSTR baseName, LPCWSTR namespaceURI)
{
	MSXML_IXMLDOMNode* o = NULL;

	HRCHECK( p->removeQualifiedItem((unicode_t*)baseName, (unicode_t*)namespaceURI, &o), "" );

	return CXMLDOMNode(o, FALSE);
}

CXMLDOMNode CXMLDOMNamedNodeMap::NextNode()
{
	MSXML_IXMLDOMNode* o = NULL;

	HRCHECK( p->nextNode(&o), "" );

	return CXMLDOMNode(o, FALSE);
}

IUnknown* CXMLDOMNamedNodeMap::GetNewEnum() const
{
	IUnknown* o = NULL;

	HRCHECK( p->get__newEnum(&o), "" );

	return o;
}

void CXMLDOMNamedNodeMap::SetAttribute(CXMLDOMDocument& doc, LPCSTR name, const _variant_t& value)
{
	CXMLDOMAttribute	newAttr = doc.CreateAttribute(name);
	if ( newAttr.IsValid() )
	{
		newAttr.SetNodeValue(value);
		SetNamedItem(newAttr);
	}
}

void CXMLDOMNamedNodeMap::SetAttribute(CXMLDOMDocument& doc, LPCWSTR name, const _variant_t& value)
{
	CXMLDOMAttribute	newAttr = doc.CreateAttribute(name);
	if ( newAttr.IsValid() )
	{
		newAttr.SetNodeValue(value);
		SetNamedItem(newAttr);
	}
}



//--------------------------------------------------------------------
// class CXMLDOMAttribute
//--------------------------------------------------------------------

CXMLDOMAttribute::CXMLDOMAttribute()
:	TXMLDOMNode<MSXML_IXMLDOMAttribute>()
{}


CXMLDOMAttribute::CXMLDOMAttribute(MSXML_IXMLDOMAttribute *lp, bool bIncrementRefCount /*= TRUE*/)
:	TXMLDOMNode<MSXML_IXMLDOMAttribute>(lp, bIncrementRefCount)
{}


CXMLDOMAttribute::CXMLDOMAttribute(const CXMLDOMAttribute &o)
:	TXMLDOMNode<MSXML_IXMLDOMAttribute>(o)
{}


CXMLDOMAttribute::CXMLDOMAttribute(const CXMLDOMNode &o)
: TXMLDOMNode<MSXML_IXMLDOMAttribute>()
{
	_QueryInterface(o.p, &p);
//	AtlComPtrAssign((IUnknown**)&p, lp);
}


#if SUPPORT_CSTRING
CString CXMLDOMAttribute::GetName() const
{
	BSTR	bstrO = NULL;

	HRCHECK( p->get_name(&bstrO), "" );

	return CString((LPCTSTR)_bstr_t(bstrO, false));
}

void CXMLDOMAttribute::GetName(CString &name) const
{
	BSTR	bstrO = NULL;

	HRCHECK( p->get_name(&bstrO), "" );

	name = (LPCTSTR)_bstr_t(bstrO, false);
}
#endif

#if SUPPORT_CSTRING
CString CXMLDOMAttribute::GetValue() const
{
	_variant_t	var;
	HRCHECK( p->get_value(&var), "" );

#if _MSC_VER < 1299 
	if ( var.vt == VT_BSTR )
		return CString(var.bstrVal);

	return CString();
#else
	return CString(var);
#endif
}

void CXMLDOMAttribute::GetValue(CString &value) const
{
	_variant_t	var;
	HRCHECK( p->get_value(&var), "" );

#if _MSC_VER < 1299 
	if ( var.vt == VT_BSTR )
		value = var.bstrVal;
	else
		value.Empty();
#else
	value = var;
#endif
}
#else
_variant_t CXMLDOMAttribute::GetValue() const
{
	VARIANT		var;
	V_VT(&var) = VT_EMPTY;

	HRCHECK( p->get_value(&var), "" );
	return _variant_t(var, false);
}
#endif

bool CXMLDOMAttribute::GetName(TString<unicode_t>& outString) const
{
	BSTR	bstrO = NULL;

	outString.Empty();
	RETURN_IF_FALSE( p->get_name(&bstrO) );

	if ( bstrO )
	{
		_bstr_t	bstr(bstrO, false);
		
		outString.Init((wchar_t*)bstr, bstr.length());
		return TRUE;
	}

	return FALSE;
}

bool CXMLDOMAttribute::GetValue(TString<unicode_t>& outString) const
{
	_variant_t	var;

	outString.Empty();
	RETURN_IF_FALSE( p->get_value(&var) );

	if ( var.vt == VT_BSTR )
	{
		outString  = var.bstrVal;

	//	_bstr_t	bstr(var);
	//	outString.Init((wchar_t*)bstr, bstr.length());
		return TRUE;
	}

	return FALSE;
}



void CXMLDOMAttribute::SetValue(LPCSTR value)
{
	_variant_t	var(value);

	HRCHECK( p->put_value(var), "" );

//	VariantClear(&varrhs);
}


void CXMLDOMAttribute::SetValue(LPCWSTR value)
{
	_variant_t	var(value);

	HRCHECK( p->put_value(var), "" );

//	VariantClear(&varrhs);
}

void CXMLDOMAttribute::SetValue(const _variant_t& rhs)
{
//	VARIANT	varrhs = rhs.Detach();

	HRCHECK( p->put_value(rhs), "" );

//	VariantClear(&varrhs);
}



//--------------------------------------------------------------------
// class CXMLDOMElement
//--------------------------------------------------------------------

CXMLDOMElement::CXMLDOMElement()
:	TXMLDOMNode<MSXML_IXMLDOMElement>()
{
}


CXMLDOMElement::CXMLDOMElement(MSXML_IXMLDOMElement *lp, bool bIncrementRefCount /*= TRUE*/)
:	TXMLDOMNode<MSXML_IXMLDOMElement>(lp, bIncrementRefCount)
{}


CXMLDOMElement::CXMLDOMElement(const CXMLDOMElement &o)
:	TXMLDOMNode<MSXML_IXMLDOMElement>(o)
{
}

CXMLDOMElement::CXMLDOMElement(const CXMLDOMNode &o)
: TXMLDOMNode<MSXML_IXMLDOMElement>()
{
	_QueryInterface(o.p, &p);
//	AtlComPtrAssign((IUnknown**)&p, lp);
}

CXMLDOMElement::CXMLDOMElement(const MSXML_IXMLDOMNode *lp)
: TXMLDOMNode<MSXML_IXMLDOMElement>()
{
	_QueryInterface((MSXML_IXMLDOMNode *)lp, &p);
}

#if SUPPORT_CSTRING
CString CXMLDOMElement::GetTagName() const
{
	BSTR	bstrO = NULL;

	HRCHECK( p->get_tagName(&bstrO), "" );

	return CString((LPCTSTR)_bstr_t(bstrO, false));
}
#endif

bool CXMLDOMElement::GetTagName(TString<unicode_t>& outString) const
{
	BSTR	bstrO = NULL;

	outString.Empty();
	RETURN_IF_FALSE( p->get_tagName(&bstrO) );

	if ( bstrO )
	{
		_bstr_t	bstr(bstrO, false);
		
		outString.Init((wchar_t*)bstr, bstr.length());
		return TRUE;
	}

	return FALSE;
}


#if SUPPORT_CSTRING

CString CXMLDOMElement::GetAttributeStr(LPCSTR name) const
{
	_variant_t	var;
	_bstr_t	bstrName(name);

	HRCHECK( p->getAttribute(bstrName, &var), "" );

#if _MSC_VER < 1299
	if ( var.vt == VT_BSTR )
		return CString(var.bstrVal);

	return CString();
#else
	return CString(var);
#endif
}

CString CXMLDOMElement::GetAttributeStr(LPCWSTR name) const
{
	_variant_t	var;

	HRCHECK( p->getAttribute((unicode_t*)name, &var), "" );

#if _MSC_VER < 1299
	if ( var.vt == VT_BSTR )
		return CString(var.bstrVal);

	return CString();
#else
	return CString(var);
#endif
}

#endif	// SUPPORT_CSTRING


/*
void CXMLDOMElement::GetAttribute(LPCTSTR name, CString &outValue) const
{
	_variant_t	var;

#if UNICODE_AWARE
	HRCHECK( p->getAttribute((unicode_t*)name, &var), "" );
#else
	_bstr_t	bstrName(name);
	HRCHECK( p->getAttribute(bstrName, &var), "" );
#endif

	if ( var.vt == VT_BSTR )
		outValue = var.bstrVal;
	else
		outValue.Empty();
}
*/

_variant_t CXMLDOMElement::GetAttribute(LPCSTR name) const
{
	return GetAttribute((unicode_t*)_bstr_t(name));
}

_variant_t CXMLDOMElement::GetAttribute(LPCWSTR name) const
{
	VARIANT		var;
	V_VT(&var) = VT_EMPTY;

	HRCHECK( p->getAttribute((unicode_t*)name, &var), "");
	return _variant_t(var, false);
}

bool CXMLDOMElement::GetAttribute(LPCSTR name, TString<unicode_t>& outString) const
{
	_variant_t	var;

	outString.Empty();

	_bstr_t	bstrName(name);
	RETURN_IF_FALSE( p->getAttribute(bstrName, &var) );

	if ( var.vt == VT_BSTR )
	{
		_bstr_t	bstr(var);
		outString.Init((wchar_t*)bstr, bstr.length());
		return TRUE;
	}

	return FALSE;
}

bool CXMLDOMElement::GetAttribute(LPCWSTR name, TString<unicode_t>& outString) const
{
	_variant_t	var;

	outString.Empty();

	RETURN_IF_FALSE( p->getAttribute((unicode_t*)name, &var) );

	if ( var.vt == VT_BSTR )
	{
		_bstr_t	bstr(var);
		outString.Init((wchar_t*)bstr, bstr.length());
		return TRUE;
	}

	return FALSE;
}


void CXMLDOMElement::SetAttribute(LPCSTR name, LPCSTR value)
{
	_bstr_t		name_bstr(name);
	_bstr_t		value_bstr(value);
	_variant_t	varValue(value_bstr);

	HRCHECK( p->setAttribute(name_bstr, varValue), "" );
//	SetAttribute(name, _variant_t(_bstr_t(value)));
}

void CXMLDOMElement::SetAttribute(LPCWSTR name, LPCWSTR value)
{
	_variant_t		varValue(value);
	HRCHECK( p->setAttribute((unicode_t*)name, varValue), "" );
}

void CXMLDOMElement::SetAttribute(LPCSTR name, const _variant_t& value)
{
	_bstr_t	bstr(name);
	SetAttribute((unicode_t*)bstr, value);
}

void CXMLDOMElement::SetAttribute(LPCWSTR name, const _variant_t& value)
{
	HRCHECK( p->setAttribute((unicode_t*)name, value), "" );
}

void CXMLDOMElement::SetAttributeNS(LPCWSTR namespaceUri, LPCWSTR qualifiedName, LPCWSTR value)
{
	CXMLDOMAttribute attrNode = GetOwnerDocument().CreateAttributeNS(namespaceUri, qualifiedName);
	attrNode.SetValue(value);
	SetAttributeNode(attrNode);
}

void CXMLDOMElement::RemoveAttribute(LPCSTR name)
{
	_bstr_t		bstrName(name);
	HRCHECK( p->removeAttribute(bstrName), "" );
}

void CXMLDOMElement::RemoveAttribute(LPCWSTR name)
{
	HRCHECK( p->removeAttribute((unicode_t*)name), "" );
}

CXMLDOMAttribute CXMLDOMElement::GetAttributeNode(LPCSTR name) const
{
	MSXML_IXMLDOMAttribute* o = NULL;
	HRCHECK( p->getAttributeNode(_bstr_t(name), &o), "");
	return CXMLDOMAttribute(o, FALSE);
}

CXMLDOMAttribute CXMLDOMElement::GetAttributeNode(LPCWSTR name) const
{
	MSXML_IXMLDOMAttribute* o = NULL;

	HRCHECK( p->getAttributeNode((unicode_t*)name, &o), "");

	return CXMLDOMAttribute(o, FALSE);
}

CXMLDOMAttribute CXMLDOMElement::SetAttributeNode(MSXML_IXMLDOMAttribute* domAttribute)
{
	MSXML_IXMLDOMAttribute* o = NULL;

	HRCHECK( p->setAttributeNode(domAttribute, &o), "" );

	return CXMLDOMAttribute(o, FALSE);
}


CXMLDOMAttribute CXMLDOMElement::RemoveAttributeNode(MSXML_IXMLDOMAttribute* domAttribute)
{
	MSXML_IXMLDOMAttribute* o = NULL;

	HRCHECK( p->removeAttributeNode(domAttribute, &o), "" );

	return CXMLDOMAttribute(o, FALSE);
}


CXMLDOMNodeList CXMLDOMElement::GetElementsByTagName(LPCSTR tagName) const
{
	MSXML_IXMLDOMNodeList* o = NULL;

	HRCHECK( p->getElementsByTagName(_bstr_t(tagName), &o), "" );

	return CXMLDOMNodeList(o, FALSE);
}

CXMLDOMNodeList CXMLDOMElement::GetElementsByTagName(LPCWSTR tagName) const
{
	MSXML_IXMLDOMNodeList* o = NULL;

	HRCHECK( p->getElementsByTagName((unicode_t*)tagName, &o), "" );

	return CXMLDOMNodeList(o, FALSE);
}


void CXMLDOMElement::Normalize()
{
	HRCHECK( p->normalize(), "" );
}

CXMLDOMElement CXMLDOMElement::GetFirstElementChild() const
{
	if ( !p ) return CXMLDOMElement();

	CXMLDOMNode node;
	CXMLDOMNode	next;
	p->get_firstChild(&node);
	while ( node )
	{
		MSXML_DOMNodeType	type = MSXML_NODE_INVALID;
		node->get_nodeType(&type);
		if ( type == MSXML_NODE_ELEMENT ) break;

        node->get_nextSibling(&next);
		node = next;
		next.Release();
	}

	return CXMLDOMElement(node);
/*
	CXMLDOMNode	node(GetFirstChild());
	while ( node.IsValid() && node.GetNodeType() != MSXML_NODE_ELEMENT )
	{
        node = node.GetFirstChild();
	}

	return CXMLDOMElement()
*/
}

CXMLDOMElement CXMLDOMElement::GetLastElementChild() const
{
	if ( !p ) return CXMLDOMElement();

	CXMLDOMNode	node;
	CXMLDOMNode	prev;
	p->get_lastChild(&node);
	while ( node )
	{
		MSXML_DOMNodeType	type = MSXML_NODE_INVALID;
		node->get_nodeType(&type);
		if ( type == MSXML_NODE_ELEMENT ) break;

		node->get_previousSibling(&prev);
		node = prev;
		prev.Release();
	}
	return CXMLDOMElement(node);
}

CXMLDOMElement CXMLDOMElement::GetPreviousElementSibling() const
{
	if ( !p ) return CXMLDOMElement();

	CXMLDOMNode	node;
	CXMLDOMNode	prev;
	p->get_previousSibling(&node);
	while ( node )
	{
		MSXML_DOMNodeType	type = MSXML_NODE_INVALID;
		node->get_nodeType(&type);
		if ( type == MSXML_NODE_ELEMENT ) break;

		node->get_previousSibling(&prev);
		node = prev;
		prev.Release();
	}
	return CXMLDOMElement(node);
}

CXMLDOMElement CXMLDOMElement::GetNextElementSibling() const
{
	if ( !p ) return CXMLDOMElement();

	CXMLDOMNode node;
	CXMLDOMNode	next;
	p->get_nextSibling(&node);
	while ( node )
	{
		MSXML_DOMNodeType	type = MSXML_NODE_INVALID;
		node->get_nodeType(&type);
		if ( type == MSXML_NODE_ELEMENT ) break;

		node->get_nextSibling(&next);
		node = next;
		next.Release();
	}

	return CXMLDOMElement(node);
}

XMLCount_t CXMLDOMElement::GetChildElementCount() const
{
	if ( !p ) return 0;

	XMLCount_t count = 0;
	CXMLDOMNode node;
	CXMLDOMNode	next;
	p->get_firstChild(&node);
	while (node)
	{
		MSXML_DOMNodeType	type = MSXML_NODE_INVALID;
		node->get_nodeType(&type);
		if ( type == MSXML_NODE_ELEMENT )
			count++;

		node->get_nextSibling(&next);
		node = next;
		next.Release();
	}

	return count;
}


void CXMLDOMElement::GetPath(TString<unicode_t> &oPath)
{
	TString<unicode_t> tag;

	GetTagName(tag);
	
	oPath = '/';
	oPath += tag;


	CXMLDOMElement parent(GetParentNode());
	while ( parent.IsValid() )
	{
		parent.GetTagName(tag);
	
		tag.InsertAt(0, '/');
		oPath.InsertAt(0, tag);

		parent = parent.GetParentNode();
	}
}



//--------------------------------------------------------------------
// class CXMLDOMDocumentType
//--------------------------------------------------------------------

CXMLDOMDocumentType::CXMLDOMDocumentType()
:	TXMLDOMNode<MSXML_IXMLDOMDocumentType>()
{}


CXMLDOMDocumentType::CXMLDOMDocumentType(MSXML_IXMLDOMDocumentType *lp, bool bIncrementRefCount /*= TRUE*/)
:	TXMLDOMNode<MSXML_IXMLDOMDocumentType>(lp, bIncrementRefCount)
{}


CXMLDOMDocumentType::CXMLDOMDocumentType(const CXMLDOMDocumentType &o)
:	TXMLDOMNode<MSXML_IXMLDOMDocumentType>(o)
{}


CXMLDOMDocumentType::CXMLDOMDocumentType(const CXMLDOMNode &o)
: TXMLDOMNode<MSXML_IXMLDOMDocumentType>()
{
	_QueryInterface(o.p, &p);
//	AtlComPtrAssign((IUnknown**)&p, lp);
}


#if SUPPORT_CSTRING
CString CXMLDOMDocumentType::GetName() const
{
	BSTR	bstrO = NULL;

	HRCHECK( p->get_name(&bstrO), "" );

	return CString((LPCTSTR)_bstr_t(bstrO, false));
}
#endif

bool CXMLDOMDocumentType::GetName(TString<unicode_t>& outString) const
{
	BSTR	bstrO = NULL;

	outString.Empty();
	RETURN_IF_FALSE( p->get_name(&bstrO) );

	if ( bstrO )
	{
		_bstr_t	bstr(bstrO, false);
		
		outString.Init((wchar_t*)bstr, bstr.length());
		return TRUE;
	}

	return FALSE;
}


CXMLDOMNamedNodeMap CXMLDOMDocumentType::GetEntities() const
{
	MSXML_IXMLDOMNamedNodeMap* o = NULL;

	HRCHECK( p->get_entities(&o), "" );

	return CXMLDOMNamedNodeMap(o, FALSE);
}


CXMLDOMNamedNodeMap CXMLDOMDocumentType::GetNotations() const
{
	MSXML_IXMLDOMNamedNodeMap* o = NULL;

	HRCHECK( p->get_notations(&o), "" );

	return CXMLDOMNamedNodeMap(o, FALSE);
}




//--------------------------------------------------------------------
// class CXMLDOMProcessingInstruction
//--------------------------------------------------------------------

CXMLDOMProcessingInstruction::CXMLDOMProcessingInstruction()
:	TXMLDOMNode<MSXML_IXMLDOMProcessingInstruction>()
{}


CXMLDOMProcessingInstruction::CXMLDOMProcessingInstruction(MSXML_IXMLDOMProcessingInstruction *lp, bool bIncrementRefCount /*= TRUE*/)
:	TXMLDOMNode<MSXML_IXMLDOMProcessingInstruction>(lp, bIncrementRefCount)
{}


CXMLDOMProcessingInstruction::CXMLDOMProcessingInstruction(const CXMLDOMProcessingInstruction &o)
:	TXMLDOMNode<MSXML_IXMLDOMProcessingInstruction>(o)
{}

CXMLDOMProcessingInstruction::CXMLDOMProcessingInstruction(const CXMLDOMNode &o)
: TXMLDOMNode<MSXML_IXMLDOMProcessingInstruction>()
{
	_QueryInterface(o.p, &p);
//	AtlComPtrAssign((IUnknown**)&p, lp);
}


#if SUPPORT_CSTRING
CString CXMLDOMProcessingInstruction::GetTarget() const
{
	BSTR	bstrO = NULL;

	HRCHECK( p->get_target(&bstrO), "" );

	return CString((LPCTSTR)_bstr_t(bstrO, false));
}
#endif

bool CXMLDOMProcessingInstruction::GetTarget(TString<unicode_t>& outString) const
{
	BSTR	bstrO = NULL;

	outString.Empty();
	RETURN_IF_FALSE( p->get_target(&bstrO) );

	if ( bstrO )
	{
		_bstr_t	bstr(bstrO, false);
		
		outString.Init((wchar_t*)bstr, bstr.length());
		return TRUE;
	}

	return FALSE;
}

#if SUPPORT_CSTRING
CString CXMLDOMProcessingInstruction::GetData() const
{
	BSTR	bstrO = NULL;

	HRCHECK( p->get_data(&bstrO), "" );

	return CString((LPCTSTR)_bstr_t(bstrO, false));
}
#endif

bool CXMLDOMProcessingInstruction::GetData(TString<unicode_t>& outString) const
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

void CXMLDOMProcessingInstruction::SetData(LPCSTR rhs)
{
	_bstr_t	bstr(rhs);
	HRCHECK( p->put_data(bstr), "" );
}

void CXMLDOMProcessingInstruction::SetData(LPCWSTR rhs)
{
	HRCHECK( p->put_data((unicode_t*)rhs), "" );
}


//--------------------------------------------------------------------
// class CXMLDOMNotation
//--------------------------------------------------------------------

CXMLDOMNotation::CXMLDOMNotation()
:	TXMLDOMNode<MSXML_IXMLDOMNotation>()
{}


CXMLDOMNotation::CXMLDOMNotation(MSXML_IXMLDOMNotation *lp, bool bIncrementRefCount /*= TRUE*/)
:	TXMLDOMNode<MSXML_IXMLDOMNotation>(lp, bIncrementRefCount)
{}


CXMLDOMNotation::CXMLDOMNotation(const CXMLDOMNotation &o)
:	TXMLDOMNode<MSXML_IXMLDOMNotation>(o)
{}


CXMLDOMNotation::CXMLDOMNotation(const CXMLDOMNode &o)
: TXMLDOMNode<MSXML_IXMLDOMNotation>()
{
	_QueryInterface(o.p, &p);
//	AtlComPtrAssign((IUnknown**)&p, lp);
}


_variant_t CXMLDOMNotation::GetPublicId() const
{
	VARIANT		var;
	V_VT(&var) = VT_EMPTY;

	HRCHECK( p->get_publicId(&var), "" );
	return _variant_t(var, false);
}


_variant_t CXMLDOMNotation::GetSystemId() const
{
	VARIANT		var;
	V_VT(&var) = VT_EMPTY;

	HRCHECK( p->get_systemId(&var), "" );
	return _variant_t(var, false);
}



//--------------------------------------------------------------------
// class CXMLDOMEntity
//--------------------------------------------------------------------

CXMLDOMEntity::CXMLDOMEntity()
:	TXMLDOMNode<MSXML_IXMLDOMEntity>()
{}


CXMLDOMEntity::CXMLDOMEntity(MSXML_IXMLDOMEntity *lp, bool bIncrementRefCount /*= TRUE*/)
:	TXMLDOMNode<MSXML_IXMLDOMEntity>(lp, bIncrementRefCount)
{}


CXMLDOMEntity::CXMLDOMEntity(const CXMLDOMEntity &o)
:	TXMLDOMNode<MSXML_IXMLDOMEntity>(o)
{}


CXMLDOMEntity::CXMLDOMEntity(const CXMLDOMNode &o)
: TXMLDOMNode<MSXML_IXMLDOMEntity>()
{
	_QueryInterface(o.p, &p);
//	AtlComPtrAssign((IUnknown**)&p, lp);
}


_variant_t CXMLDOMEntity::GetPublicId() const
{
	VARIANT		var;
	V_VT(&var) = VT_EMPTY;

	HRCHECK( p->get_publicId(&var), "" );
	return _variant_t(var, false);
}


_variant_t CXMLDOMEntity::GetSystemId() const
{
	VARIANT		var;
	V_VT(&var) = VT_EMPTY;

	HRCHECK( p->get_systemId(&var), "" );
	return _variant_t(var, false);
}

#if SUPPORT_CSTRING
CString CXMLDOMEntity::GetNotationName() const
{
	BSTR	bstrO = NULL;

	HRCHECK( p->get_notationName(&bstrO), "" );

	return CString((LPCTSTR)_bstr_t(bstrO, false));
}
#endif

bool CXMLDOMEntity::GetPublicId(TString<unicode_t>& outString) const
{
	_variant_t		var;
//	V_VT(&var) = VT_EMPTY;

	HRCHECK( p->get_publicId(&var), "" );

	if ( var.vt == VT_BSTR )
		outString  = var.bstrVal;
	else
		outString.Empty();

	return !outString.IsEmpty();
}

bool CXMLDOMEntity::GetSystemId(TString<unicode_t>& outString) const
{
	_variant_t		var;
//	VARIANT		var;
//	V_VT(&var) = VT_EMPTY;

	HRCHECK( p->get_systemId(&var), "" );

	if ( var.vt == VT_BSTR )
		outString  = var.bstrVal;
	else
		outString.Empty();

	return !outString.IsEmpty();
}

bool CXMLDOMEntity::GetNotationName(TString<unicode_t>& outString) const
{
	BSTR	bstrO = NULL;

	outString.Empty();
	RETURN_IF_FALSE( p->get_notationName(&bstrO) );

	if ( bstrO )
	{
		_bstr_t	bstr(bstrO, false);
		
		outString.Init((wchar_t*)bstr, bstr.length());
		return TRUE;
	}

	return FALSE;
}



//--------------------------------------------------------------------
// class CXMLDOMParseError
//--------------------------------------------------------------------

CXMLDOMParseError::CXMLDOMParseError(MSXML_IXMLDOMParseError *lp, bool bIncrementRefCount/* = TRUE*/)
	:CInterfaceCallingWrapper<MSXML_IXMLDOMParseError>(lp, bIncrementRefCount)
{
}


CXMLDOMParseError::CXMLDOMParseError(const CXMLDOMParseError &o)
:	CInterfaceCallingWrapper<MSXML_IXMLDOMParseError>(o)
{
}


long CXMLDOMParseError::GetErrorCode() const
{
	long o = 0;

	HRCHECK( p->get_errorCode(&o), "" );

	return o;
}

#if SUPPORT_CSTRING
CString CXMLDOMParseError::GetURL() const
{
	BSTR	bstrO = NULL;

	HRESULT	hr = p->get_url(&bstrO);

	return CString((LPCTSTR)_bstr_t(bstrO, false));
}
#endif

bool CXMLDOMParseError::GetURL(TString<unicode_t>& outString) const
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

#if SUPPORT_CSTRING
CString CXMLDOMParseError::GetReason(void) const
{
	BSTR	bstrO = NULL;

	HRCHECK( p->get_reason(&bstrO), "" );

	return CString( (LPCTSTR)_bstr_t(bstrO, false) );
}
#endif

bool CXMLDOMParseError::GetReason(TString<unicode_t>& outString) const
{
	BSTR	bstrO = NULL;

	outString.Empty();
	RETURN_IF_FALSE( p->get_reason(&bstrO) );

	if ( bstrO )
	{
		_bstr_t	bstr(bstrO, false);
		
		outString.Init((wchar_t*)bstr, bstr.length());
		return TRUE;
	}

	return FALSE;
}

#if SUPPORT_CSTRING
CString CXMLDOMParseError::GetSrcText() const
{
	BSTR	bstrO = NULL;

	HRCHECK( p->get_srcText(&bstrO), "" );

	return CString((LPCTSTR)_bstr_t(bstrO, false));
}
#endif

bool CXMLDOMParseError::GetSrcText(TString<unicode_t>& outString) const
{
	BSTR	bstrO = NULL;

	outString.Empty();
	RETURN_IF_FALSE( p->get_srcText(&bstrO) );

	if ( bstrO )
	{
		_bstr_t	bstr(bstrO, false);
		
		outString.Init((wchar_t*)bstr, bstr.length());
		return TRUE;
	}

	return FALSE;
}


long CXMLDOMParseError::GetLine() const
{
	long o = 0;

	HRCHECK( p->get_line(&o), "" );

	return o;
}


long CXMLDOMParseError::GetLinepos() const
{
	long o = 0;

	HRCHECK( p->get_linepos(&o), "" );

	return o;
}


long CXMLDOMParseError::GetFilepos() const
{
	long o = 0;

	HRCHECK( p->get_filepos(&o), "" );

	return o;
}



//--------------------------------------------------------------------
// class CXMLDOMSelection
//--------------------------------------------------------------------

CXMLDOMSelection::CXMLDOMSelection()
:	TXMLDOMNodeList<IXMLDOMSelection>()
{
}

CXMLDOMSelection::CXMLDOMSelection(const CXMLDOMSelection &o)
:	TXMLDOMNodeList<MSXML_IXMLDOMSelection>(o)
{
}

CXMLDOMSelection::CXMLDOMSelection(const CXMLDOMNodeList &o)
: TXMLDOMNodeList<MSXML_IXMLDOMSelection>()
{
	_QueryInterface(o.p, &p);
//	AtlComPtrAssign((IUnknown**)&p, lp);
}

CXMLDOMSelection::CXMLDOMSelection(IXMLDOMSelection *lp, bool bIncrementRefCount /*= TRUE*/)
:	TXMLDOMNodeList<IXMLDOMSelection>(lp, bIncrementRefCount)
{
}


#if SUPPORT_CSTRING
CString CXMLDOMSelection::GetExpr() const
{
	BSTR	bstrO = NULL;

	HRCHECK( p->get_expr(&bstrO), "" );

	return CString((LPCTSTR)_bstr_t(bstrO, false));
}
#endif

bool CXMLDOMSelection::GetExpr(TString<unicode_t>& outString) const
{
	BSTR	bstrO = NULL;

	outString.Empty();
	RETURN_IF_FALSE( p->get_expr(&bstrO) );

	if ( bstrO )
	{
		_bstr_t	bstr(bstrO, false);
		
		outString.Init((wchar_t*)bstr, bstr.length());
		return TRUE;
	}

	return FALSE;
}

void CXMLDOMSelection::SetExpr(LPCTSTR rhs)
{
#if UNICODE_AWARE
	HRCHECK( p->put_expr((unicode_t*)rhs), "" );
#else
	_bstr_t		bstr(rhs);
	HRCHECK( p->put_expr(bstr), "" );
#endif
}


CXMLDOMNode CXMLDOMSelection::GetContext() const
{
	MSXML_IXMLDOMNode* o = NULL;

	HRCHECK( p->get_context(&o), "" );

	return CXMLDOMNode(o, FALSE);
}


void CXMLDOMSelection::SetRefContext(MSXML_IXMLDOMNode* rhs)
{
	HRCHECK( p->putref_context(rhs), "" );
}


CXMLDOMNode CXMLDOMSelection::PeekNode() const
{
	MSXML_IXMLDOMNode* o = NULL;

	HRCHECK( p->peekNode(&o), "" );

	return CXMLDOMNode(o, FALSE);
}


CXMLDOMNode CXMLDOMSelection::Matches(MSXML_IXMLDOMNode* pNode) const
{
	MSXML_IXMLDOMNode* o = NULL;

	HRCHECK( p->matches(pNode, &o), "" );

	return CXMLDOMNode(o, FALSE);
}


CXMLDOMNode CXMLDOMSelection::RemoveNext()
{
	MSXML_IXMLDOMNode* o = NULL;

	HRCHECK( p->removeNext(&o), "" );

	return CXMLDOMNode(o, FALSE);
}


void CXMLDOMSelection::RemoveAll()
{
	HRCHECK( p->removeAll(), "" );
}


CXMLDOMSelection CXMLDOMSelection::Clone() const
{
	MSXML_IXMLDOMSelection* o = NULL;

	HRCHECK( p->clone(&o), "" );

	return CXMLDOMSelection(o, FALSE);
}


_variant_t CXMLDOMSelection::GetProperty(LPCTSTR name) const
{
	VARIANT		var;
	V_VT(&var) = VT_EMPTY;

#if UNICODE_AWARE
	HRCHECK( p->getProperty((unicode_t*)name, &var), "" );
#else
	_bstr_t	bstrName(name);
	HRCHECK( p->getProperty(bstrName, &var), "" );
#endif

	return _variant_t(var, false);
}


void CXMLDOMSelection::SetProperty(LPCTSTR name, const _variant_t &value)
{
#if UNICODE_AWARE
	HRCHECK( p->setProperty((unicode_t*)name, value), "" );
#else
	_bstr_t	bstrName(name);
//	VARIANT	varvalue = value.Detach();
	HRCHECK( p->setProperty(bstrName, value), "" );
#endif
//	VariantClear(&varvalue);
}


#ifndef _WIN32_WCE
//--------------------------------------------------------------------
// class CXMLDOMSchemaCollection2
//--------------------------------------------------------------------

CXMLDOMSchemaCollection2::CXMLDOMSchemaCollection2()
:	TXMLDOMSchemaCollection<MSXML_IXMLDOMSchemaCollection2>()
{}


CXMLDOMSchemaCollection2::CXMLDOMSchemaCollection2(IXMLDOMSchemaCollection2 *lp, bool bIncrementRefCount /*= TRUE*/)
:	TXMLDOMSchemaCollection<MSXML_IXMLDOMSchemaCollection2>(lp, bIncrementRefCount)
{}


CXMLDOMSchemaCollection2::CXMLDOMSchemaCollection2(const CXMLDOMSchemaCollection2 &o)
:	TXMLDOMSchemaCollection<MSXML_IXMLDOMSchemaCollection2>(o)
{}


void CXMLDOMSchemaCollection2::Validate() const
{
	HRCHECK( p->validate(), "" );
}


void CXMLDOMSchemaCollection2::SetValidateOnLoad(bool rhs)
{
	HRCHECK( p->put_validateOnLoad(rhs), "" );
}


bool CXMLDOMSchemaCollection2::GetValidateOnLoad() const
{
	VARIANT_BOOL	o = VARIANT_FALSE;

	HRCHECK( p->get_validateOnLoad(&o), "" );

	return o == VARIANT_TRUE;
}


ISchema* CXMLDOMSchemaCollection2::GetSchema(LPCTSTR namespaceURI) const
{
	ISchema* o = NULL;

#if UNICODE_AWARE
	HRCHECK( p->getSchema((unicode_t*)namespaceURI, &o), "" );
#else
	_bstr_t		bstrNamespaceURI(namespaceURI);
	HRCHECK( p->getSchema(bstrNamespaceURI, &o), "" );
#endif

	return o;
}


MSXML_ISchemaItem* CXMLDOMSchemaCollection2::GetDeclaration(MSXML_IXMLDOMNode* node) const
{
	ISchemaItem* o = NULL;

	HRCHECK( p->getDeclaration(node, &o), "" );

	return o;
}
	
#endif	// !_WIN32_WCE


//--------------------------------------------------------------------
// class CXMLDOMImplementation
//--------------------------------------------------------------------

bool CXMLDOMImplementation::HasFeature(LPCTSTR feature, LPCTSTR version) const
{
	VARIANT_BOOL hasFeature = VARIANT_FALSE;

#if UNICODE_AWARE
	HRCHECK( p->hasFeature((unicode_t*)feature, (unicode_t*)version, &hasFeature), "" );
#else
	_bstr_t		bstrFeature(feature);
	_bstr_t		bstrVersion(version);
	HRCHECK( p->hasFeature(bstrFeature, bstrVersion, &hasFeature), "" );
#endif

	return hasFeature == VARIANT_TRUE;
}


//--------------------------------------------------------------------
// class CXSLTemplate
//--------------------------------------------------------------------

CXSLTemplate::CXSLTemplate()
:	CInterfaceCallingWrapper<MSXML_IXSLTemplate>()
{}


CXSLTemplate::CXSLTemplate(MSXML_IXSLTemplate *lp, bool bIncrementRefCount /*= TRUE*/)
:	CInterfaceCallingWrapper<MSXML_IXSLTemplate>(lp, bIncrementRefCount)
{}


CXSLTemplate::CXSLTemplate(const CXSLTemplate &o)
:	CInterfaceCallingWrapper<MSXML_IXSLTemplate>(o)
{}


void CXSLTemplate::SetStylesheet(const MSXML_IXMLDOMNode* stylesheet)
{
	HRCHECK( p->putref_stylesheet(const_cast<MSXML_IXMLDOMNode*>(stylesheet)), "" );
}


CXMLDOMNode CXSLTemplate::GetStylesheet() const
{
	MSXML_IXMLDOMNode* o = NULL;

	HRCHECK( p->get_stylesheet(&o), "" );

	return CXMLDOMNode(o, FALSE);
}


CXSLProcessor CXSLTemplate::CreateProcessor()
{
	MSXML_IXSLProcessor* o = NULL;

	HRCHECK( p->createProcessor(&o), "" );

	return CXSLProcessor(o, FALSE);
}


//--------------------------------------------------------------------
// class CXSLProcessor
//--------------------------------------------------------------------

CXSLProcessor::CXSLProcessor()
:	CInterfaceCallingWrapper<MSXML_IXSLProcessor>()
{}


CXSLProcessor::CXSLProcessor(MSXML_IXSLProcessor *lp, bool bIncrementRefCount /*= TRUE*/)
:	CInterfaceCallingWrapper<MSXML_IXSLProcessor>(lp, bIncrementRefCount)
{}


CXSLProcessor::CXSLProcessor(const CXSLProcessor &o)
:	CInterfaceCallingWrapper<MSXML_IXSLProcessor>(o)
{}


void CXSLProcessor::SetInput(const _variant_t& rhs)
{
	HRCHECK( p->put_input(rhs), "" );
}


_variant_t CXSLProcessor::GetInput() const
{
	VARIANT		var;
	V_VT(&var) = VT_EMPTY;

	HRCHECK( p->get_input(&var), "" );
	return _variant_t(var, false);
}


CXSLTemplate CXSLProcessor::GetOwnerTemplate() const
{
	MSXML_IXSLTemplate* o = NULL;

	HRCHECK( p->get_ownerTemplate(&o), "" );

	return CXSLTemplate(o, FALSE);
}


void CXSLProcessor::SetStartMode(LPCTSTR mode, LPCTSTR namespaceURI)
{
#if UNICODE_AWARE
	HRCHECK( p->setStartMode((unicode_t*)mode, (unicode_t*)namespaceURI), "" );
#else
	_bstr_t		bstrMode(mode);
	_bstr_t		bstrNamespaceURI(namespaceURI);
	HRCHECK( p->setStartMode(bstrMode, bstrNamespaceURI), "" );
#endif
}

#if SUPPORT_CSTRING
CString CXSLProcessor::GetStartMode() const
{
	BSTR	bstrO = NULL;

	HRCHECK( p->get_startMode(&bstrO), "" );

	return CString((LPCTSTR)_bstr_t(bstrO, false));
}
#endif

bool CXSLProcessor::GetStartMode(TString<unicode_t>& outString) const
{
	BSTR	bstrO = NULL;

	outString.Empty();
	RETURN_IF_FALSE( p->get_startMode(&bstrO) );

	if ( bstrO )
	{
		_bstr_t	bstr(bstrO, false);
		
		outString.Init((wchar_t*)bstr, bstr.length());
		return TRUE;
	}

	return FALSE;
}

#if SUPPORT_CSTRING
CString CXSLProcessor::GetStartModeURI() const
{
	BSTR	bstrO = NULL;

	HRCHECK( p->get_startModeURI(&bstrO), "" );

	return CString((LPCTSTR)_bstr_t(bstrO, false));
}
#endif

bool CXSLProcessor::GetStartModeURI(TString<unicode_t>& outString) const
{
	BSTR	bstrO = NULL;

	outString.Empty();
	RETURN_IF_FALSE( p->get_startModeURI(&bstrO) );

	if ( bstrO )
	{
		_bstr_t	bstr(bstrO, false);
		
		outString.Init((wchar_t*)bstr, bstr.length());
		return TRUE;
	}

	return FALSE;
}

void CXSLProcessor::SetOutput(const _variant_t& rhs)
{
	HRCHECK( p->put_output(rhs), "" );
/*
	VARIANT	varrhs = rhs.Detach();
	HRCHECK( p->put_output(&bstrO), "" );
	VariantClear(&varrhs);
*/
}


_variant_t CXSLProcessor::GetOutput() const
{
	VARIANT		var;
	V_VT(&var) = VT_EMPTY;

	HRCHECK( p->get_output(&var), "" );
	return _variant_t(var, false);
}


bool CXSLProcessor::Transform()
{
	VARIANT_BOOL	o = VARIANT_FALSE;

	HRCHECK( p->transform(&o), "" );

	return o == VARIANT_TRUE;
}


void CXSLProcessor::Reset()
{
	HRCHECK( p->reset(), "" );
}


long CXSLProcessor::GetReadyState() const
{
	long o = 0;

	HRCHECK( p->get_readyState(&o), "" );

	return o;
}


void CXSLProcessor::AddParameter(LPCTSTR baseName, const _variant_t &parameter, LPCTSTR namespaceURI)
{
#if UNICODE_AWARE
	HRCHECK( p->addParameter((unicode_t*)baseName, parameter, (unicode_t*)namespaceURI), "" );
#else
	_bstr_t		bstrBaseName(baseName);
	_bstr_t		bstrNamespaceURI(namespaceURI);

	HRCHECK( p->addParameter(bstrBaseName, parameter, bstrNamespaceURI), "" );
#endif
}


void CXSLProcessor::AddObject(IDispatch* obj, LPCTSTR namespaceURI)
{
#if UNICODE_AWARE
	HRCHECK( p->addObject(obj, (unicode_t*)namespaceURI), "" );
#else
	_bstr_t		bstrNamespaceURI(namespaceURI);
	HRCHECK( p->addObject(obj, bstrNamespaceURI), "" );
#endif
}


CXMLDOMNode CXSLProcessor::GetStylesheet() const
{
	MSXML_IXMLDOMNode* o = NULL;

	HRCHECK( p->get_stylesheet(&o), "" );

	return CXMLDOMNode(o, FALSE);
}



//--------------------------------------------------------------------
// class CXMLHTTPRequest
//--------------------------------------------------------------------

CXMLHTTPRequest::CXMLHTTPRequest()
 : CInterfaceCallingWrapper<IXMLHTTPRequest>()
{}


CXMLHTTPRequest::CXMLHTTPRequest(IXMLHTTPRequest *lp, bool bIncrementRefCount /*= FALSE*/)
 : CInterfaceCallingWrapper<IXMLHTTPRequest>(lp, bIncrementRefCount)
{}


CXMLHTTPRequest::CXMLHTTPRequest(const CXMLHTTPRequest &o)
:	CInterfaceCallingWrapper<IXMLHTTPRequest>(o)
{}


void CXMLHTTPRequest::Open(LPCTSTR method, LPCTSTR url, const _variant_t &async, const _variant_t &user, const _variant_t &password)
{
#if UNICODE_AWARE
	HRCHECK( p->open((unicode_t*)method, (unicode_t*)url, async, user, password), "" );
#else
	_bstr_t	bstrMethod(method);
	_bstr_t	bstrUrl(url);

	HRCHECK( p->open(bstrMethod, bstrUrl, async, user, password), "" );
#endif
}

void CXMLHTTPRequest::Open(LPCTSTR method, LPCTSTR url, bool async, LPCTSTR user, LPCTSTR password)
{
	_variant_t	varAsync((async == TRUE) ? true : false);
	_variant_t	varUser(user);
	_variant_t	varPassword(password);

#if UNICODE_AWARE
	HRCHECK( p->open((unicode_t*)method, (unicode_t*)url, varAsync, varUser, varPassword), "" );
#else
	_bstr_t		bstrMethod(method);
	_bstr_t		bstrUrl(url);

	HRCHECK( p->open(bstrMethod, bstrUrl, varAsync, varUser, varPassword), "" );
#endif
}


void CXMLHTTPRequest::SetRequestHeader(LPCTSTR header, LPCTSTR value)
{
#if UNICODE_AWARE

	HRCHECK( p->setRequestHeader((unicode_t*)header, (unicode_t*)value), "" );

#else
	_bstr_t	bstrHeader(header);
	_bstr_t	bstrValue(value);

	HRCHECK( p->setRequestHeader(bstrHeader, bstrValue), "" );
#endif
}

#if SUPPORT_CSTRING
CString CXMLHTTPRequest::GetResponseHeader(LPCTSTR header) const
{
#if UNICODE_AWARE
	BSTR	bstrO = NULL;

	HRCHECK( p->getResponseHeader((unicode_t*)header, &bstrO), "" );

	return CString((LPCTSTR)_bstr_t(bstrO, false));
#else
	_bstr_t	bstrHeader(header);
	BSTR	bstrO = NULL;

	HRCHECK( p->getResponseHeader(bstrHeader, &bstrO), "" );

	return CString((LPCTSTR)_bstr_t(bstrO, false));
#endif
}
#endif


bool CXMLHTTPRequest::GetResponseHeader(LPCTSTR header, TString<unicode_t>& outString) const
{
#if UNICODE_AWARE
	BSTR	bstrO = NULL;

	outString.Empty();
	RETURN_IF_FALSE( p->getResponseHeader((unicode_t*)header, &bstrO) );
	if ( bstrO )
	{
		_bstr_t	bstr(bstrO, false);
		
		outString.Init((wchar_t*)bstr, bstr.length());
		return TRUE;
	}
	return FALSE;
#else
	_bstr_t	bstrHeader(header);
	BSTR	bstrO = NULL;

	outString.Empty();
	RETURN_IF_FALSE( p->getResponseHeader(bstrHeader, &bstrO) );
	if ( bstrO )
	{
		_bstr_t	bstr(bstrO, false);
	
		outString.Init((wchar_t*)bstr, bstr.length());
		return TRUE;
	}
	return FALSE;
#endif
}

#if SUPPORT_CSTRING
CString CXMLHTTPRequest::GetAllResponseHeaders() const
{
	BSTR	bstrO = NULL;

	HRCHECK( p->getAllResponseHeaders(&bstrO), "" );

	return CString((LPCTSTR)_bstr_t(bstrO, false));
}
#endif

bool CXMLHTTPRequest::GetAllResponseHeaders(TString<unicode_t>& outString) const
{
	BSTR	bstrO = NULL;

	outString.Empty();
	RETURN_IF_FALSE( p->getAllResponseHeaders(&bstrO) );

	if ( bstrO )
	{
		_bstr_t	bstr(bstrO, false);
		
		outString.Init((wchar_t*)bstr, bstr.length());
		return TRUE;
	}

	return FALSE;
}

void CXMLHTTPRequest::Send(_variant_t body)
{
	VARIANT	varBody = body.Detach();

	HRCHECK( p->send(varBody), "" );

	VariantClear(&varBody);
}


void CXMLHTTPRequest::Abort()
{
	HRCHECK( p->abort(), "" );
}


long CXMLHTTPRequest::GetStatus() const
{
	long o = 0;

	HRCHECK( p->get_status(&o), "" );

	return o;
}

#if SUPPORT_CSTRING
CString CXMLHTTPRequest::GetStatusText() const
{
	BSTR	bstrO = NULL;

	HRCHECK( p->get_statusText(&bstrO), "" );

	return CString((LPCTSTR)_bstr_t(bstrO, false));
}
#endif

bool CXMLHTTPRequest::GetStatusText(TString<unicode_t>& outString) const
{
	BSTR	bstrO = NULL;

	outString.Empty();
	RETURN_IF_FALSE( p->get_statusText(&bstrO) );

	if ( bstrO )
	{
		_bstr_t	bstr(bstrO, false);
		
		outString.Init((wchar_t*)bstr, bstr.length());
		return TRUE;
	}

	return FALSE;
}


IDispatch* CXMLHTTPRequest::GetResponseXML() const
{
	IDispatch* o = NULL;

	HRCHECK( p->get_responseXML(&o), "" );

	return o;
}

#if SUPPORT_CSTRING
CString CXMLHTTPRequest::GetResponseText() const
{
	BSTR	bstrO = NULL;

	HRCHECK( p->get_responseText(&bstrO), "" );

	return CString((LPCTSTR)_bstr_t(bstrO, false));
}
#endif


bool CXMLHTTPRequest::GetResponseText(TString<unicode_t>& outString) const
{
	BSTR	bstrO = NULL;

	outString.Empty();
	RETURN_IF_FALSE( p->get_responseText(&bstrO) );

	if ( bstrO )
	{
		_bstr_t	bstr(bstrO, false);
		
		outString.Init((wchar_t *)bstr, bstr.length());
		return TRUE;
	}

	return FALSE;
}


_variant_t CXMLHTTPRequest::GetResponseBody() const
{
	VARIANT		var;
	V_VT(&var) = VT_EMPTY;

	HRCHECK( p->get_responseBody(&var), "" );
	return _variant_t(var, false);
}


_variant_t CXMLHTTPRequest::GetResponseStream() const
{
	VARIANT		var;
	V_VT(&var) = VT_EMPTY;

	HRCHECK( p->get_responseStream(&var), "" );
	return _variant_t(var, false);
}


long CXMLHTTPRequest::GetReadyState() const
{
	long o = 0;

	HRCHECK( p->get_readyState(&o), "" );

	return o;
}


void CXMLHTTPRequest::SetOnReadyStateChange(IDispatch* rhs)
{
	HRCHECK( p->put_onreadystatechange(rhs), "" );
}

bool CXMLHTTPRequest::Create(LPCOLESTR lpszProgID/* = XML_HTTP_PROGID*/)
{
	if ( p != NULL ) return TRUE;

	CLSID	clsid;
	m_HResult = CLSIDFromProgID(lpszProgID, &clsid);
	if ( SUCCEEDED(m_HResult) )
	{
		m_HResult = CoCreateInstance(clsid);
		if ( SUCCEEDED(m_HResult) )
		{
			return TRUE;
		}
	}

	return FALSE;
}

bool CXMLHTTPRequest::Create(REFCLSID rclsid /*= CLS_XMLHTTP*/)
{
	if ( p != NULL ) return TRUE;

	RETURN_FLASE_IF_HRCALL_FAILED( CoCreateInstance(rclsid), "Create a new XMLHTTPRequest" );

	return TRUE;
}



//--------------------------------------------------------------------
// class CServerXMLHTTPRequest
//--------------------------------------------------------------------

CServerXMLHTTPRequest::CServerXMLHTTPRequest()
:	CInterfaceCallingWrapper<IServerXMLHTTPRequest>()
{}


CServerXMLHTTPRequest::CServerXMLHTTPRequest(IServerXMLHTTPRequest *lp, bool bIncrementRefCount /*= FALSE*/)
:	CInterfaceCallingWrapper<IServerXMLHTTPRequest>(lp, bIncrementRefCount)
{}


CServerXMLHTTPRequest::CServerXMLHTTPRequest(const CServerXMLHTTPRequest &o)
:	CInterfaceCallingWrapper<IServerXMLHTTPRequest>(o)
{}


void CServerXMLHTTPRequest::Open(LPCTSTR method, LPCTSTR url, _variant_t async, _variant_t user, _variant_t password)
{
#if UNICODE_AWARE
	VARIANT	varAsync = async.Detach();
	VARIANT	varUser = user.Detach();
	VARIANT	varPassword = password.Detach();

	HRCHECK( p->open((unicode_t*)method, (unicode_t*)url, varAsync, varUser, varPassword), "" );

	VariantClear(&varAsync);
	VariantClear(&varUser);
	VariantClear(&varPassword);
#else
	_bstr_t	bstrMethod(method);
	_bstr_t	bstrUrl(url);
	VARIANT	varAsync = async.Detach();
	VARIANT	varUser = user.Detach();
	VARIANT	varPassword = password.Detach();

	HRCHECK( p->open(bstrMethod, bstrUrl, varAsync, varUser, varPassword), "" );

	VariantClear(&varAsync);
	VariantClear(&varUser);
	VariantClear(&varPassword);
#endif
}


void CServerXMLHTTPRequest::SetRequestHeader(LPCTSTR header, LPCTSTR value)
{
#if UNICODE_AWARE

	HRCHECK( p->setRequestHeader((unicode_t*)header, (unicode_t*)value), "" );

#else
	_bstr_t	bstrHeader(header);
	_bstr_t	bstrValue(value);

	HRCHECK( p->setRequestHeader(bstrHeader, bstrValue), "" );

#endif
}

#if SUPPORT_CSTRING
CString CServerXMLHTTPRequest::GetResponseHeader(LPCTSTR header) const
{
#if UNICODE_AWARE
	BSTR	bstrO = NULL;

	HRCHECK( p->getResponseHeader((unicode_t*)header, &bstrO), "" );

	return CString((LPCTSTR)_bstr_t(bstrO, false));
#else

	_bstr_t	bstrHeader(header);
	BSTR	bstrO = NULL;

	HRCHECK( p->getResponseHeader(bstrHeader, &bstrO), "" );

	return CString((LPCTSTR)_bstr_t(bstrO, false));
#endif
}
#endif

bool CServerXMLHTTPRequest::GetResponseHeader(LPCTSTR header, TString<unicode_t>& outString) const
{
#if UNICODE_AWARE
	BSTR	bstrO = NULL;

	outString.Empty();
	RETURN_IF_FALSE( p->getResponseHeader((unicode_t*)header, &bstrO) );
	if ( bstrO )
	{
		_bstr_t	bstr(bstrO, false);
		
		outString.Init((wchar_t*)bstr, bstr.length());
		return TRUE;
	}
	return FALSE;
#else
	_bstr_t	bstrHeader(header);
	BSTR	bstrO = NULL;

	outString.Empty();
	RETURN_IF_FALSE( p->getResponseHeader(bstrHeader, &bstrO) );
	if ( bstrO )
	{
		_bstr_t	bstr(bstrO, false);
		
		outString.Init((wchar_t*)bstr, bstr.length());
		return TRUE;
	}
	return FALSE;
#endif
}

#if SUPPORT_CSTRING
CString CServerXMLHTTPRequest::GetAllResponseHeaders() const
{
	BSTR	bstrO = NULL;

	HRCHECK( p->getAllResponseHeaders(&bstrO), "" );

	return CString((LPCTSTR)_bstr_t(bstrO, false));
}
#endif

bool CServerXMLHTTPRequest::GetAllResponseHeaders(TString<unicode_t>& outString) const
{
	BSTR	bstrO = NULL;

	outString.Empty();
	RETURN_IF_FALSE( p->getAllResponseHeaders(&bstrO) );

	if ( bstrO )
	{
		_bstr_t	bstr(bstrO, false);
		
		outString.Init((wchar_t*)bstr, bstr.length());
		return TRUE;
	}

	return FALSE;
}


void CServerXMLHTTPRequest::Send(_variant_t body)
{
	VARIANT	varBody = body.Detach();

	HRCHECK( p->send(varBody), "" );

	VariantClear(&varBody);
}


void CServerXMLHTTPRequest::Abort()
{
	HRCHECK( p->abort(), "" );
}


long CServerXMLHTTPRequest::GetStatus() const
{
	long o = 0;

	HRCHECK( p->get_status(&o), "" );

	return o;
}

#if SUPPORT_CSTRING
CString CServerXMLHTTPRequest::GetStatusText() const
{
	BSTR	bstrO = NULL;

	HRCHECK( p->get_statusText(&bstrO), "" );

	return CString((LPCTSTR)_bstr_t(bstrO, false));
}
#endif

bool CServerXMLHTTPRequest::GetStatusText(TString<unicode_t>& outString) const
{
	BSTR	bstrO = NULL;

	outString.Empty();
	RETURN_IF_FALSE( p->get_statusText(&bstrO) );

	if ( bstrO )
	{
		_bstr_t	bstr(bstrO, false);
		
		outString.Init((wchar_t*)bstr, bstr.length());
		return TRUE;
	}

	return FALSE;
}


IDispatch* CServerXMLHTTPRequest::GetResponseXML() const
{
	IDispatch* o = NULL;

	HRCHECK( p->get_responseXML(&o), "" );

	return o;
}

#if SUPPORT_CSTRING
CString CServerXMLHTTPRequest::GetResponseText() const
{
	BSTR	bstrO = NULL;

	HRCHECK( p->get_responseText(&bstrO), "" );

	return CString((LPCTSTR)_bstr_t(bstrO, false));
}
#endif

bool CServerXMLHTTPRequest::GetResponseText(TString<unicode_t>& outString) const
{
	BSTR	bstrO = NULL;

	outString.Empty();
	RETURN_IF_FALSE( p->get_responseText(&bstrO) );

	if ( bstrO )
	{
		_bstr_t	bstr(bstrO, false);
		
		outString.Init((wchar_t*)bstr, bstr.length());
		return TRUE;
	}

	return FALSE;
}


_variant_t CServerXMLHTTPRequest::GetResponseBody() const
{
	VARIANT		var;
	V_VT(&var) = VT_EMPTY;

	HRCHECK( p->get_responseBody(&var), "" );
	return _variant_t(var, false);
}


_variant_t CServerXMLHTTPRequest::GetResponseStream() const
{
	VARIANT		var;
	V_VT(&var) = VT_EMPTY;

	HRCHECK( p->get_responseStream(&var), "" );
	return _variant_t(var, false);
}


long CServerXMLHTTPRequest::GetReadyState() const
{
	long o = 0;

	HRCHECK( p->get_readyState(&o), "" );

	return o;
}


void CServerXMLHTTPRequest::SetOnReadyStateChange(IDispatch* rhs)
{
	HRCHECK( p->put_onreadystatechange(rhs), "" );
}


void CServerXMLHTTPRequest::SetTimeouts(long resolveTimeout, long connectTimeout, long sendTimeout, long receiveTimeout)
{
	HRCHECK( p->setTimeouts(resolveTimeout, connectTimeout, sendTimeout, receiveTimeout), "" );
}


bool CServerXMLHTTPRequest::WaitForResponse(const _variant_t &timeoutInSeconds)
{
	short	o = 0;
	HRCHECK( p->waitForResponse(timeoutInSeconds, &o), "" );
	return o != 0;
}


_variant_t CServerXMLHTTPRequest::GetOption(SERVERXMLHTTP_OPTION option) const
{
	VARIANT		var;
	V_VT(&var) = VT_EMPTY;

	HRCHECK( p->getOption(option, &var), "" );
	return _variant_t(var, false);
}


void CServerXMLHTTPRequest::SetOption(SERVERXMLHTTP_OPTION option, const _variant_t &value)
{
	HRCHECK( p->setOption(option, value), "" );
}


bool CServerXMLHTTPRequest::Create(LPCOLESTR lpszProgID/* = SERVER_XMLHTTP_PROGID*/)
{
	if ( p != NULL ) return TRUE;

	CLSID	clsid;
	m_HResult = CLSIDFromProgID(lpszProgID, &clsid);
	if ( SUCCEEDED(m_HResult) )
	{
		m_HResult = CoCreateInstance(clsid);
		if ( SUCCEEDED(m_HResult) )
		{
			return TRUE;
		}
	}

	return FALSE;
}

bool CServerXMLHTTPRequest::Create(REFCLSID rclsid /*= CLS_SERVER_XMLHTTP*/)
{
	if ( p != NULL ) return TRUE;

	RETURN_FLASE_IF_HRCALL_FAILED( CoCreateInstance(rclsid), "Create a new ServerXMLHTTPRequest" );

	return TRUE;
}


// ============================================================================
//
// xerces XML Parser와 호환을 위한 API
//
// ============================================================================

void XPath::SelectionNamespaces(const unicode_t* namespaces)
{
	mXMLDOMDocument.SelectionNamespaces(namespaces);
}

void XPath::SelectionNamespaces(const char* namespaces)
{
	mXMLDOMDocument.SelectionNamespaces(namespaces);
}


void XPath::AddNamespaceBinding(const unicode_t* prefix, const unicode_t* uri)
{
	TString<unicode_t>	str(_bstr_t("xmlns:"));
	str += prefix;
	str += '=';
	str += '\'';
	str += uri;
	str += '\'';

	mXMLDOMDocument.SelectionNamespaces(str);
}

void XPath::AddNamespaceBinding(const char* prefix, const char* uri)
{
	AddNamespaceBinding((const unicode_t*)_bstr_t(prefix), (const unicode_t*)_bstr_t(uri));
}


CXMLDOMNode XPath::SelectSingleNode(MSXML_IXMLDOMNode *domNode, const unicode_t *queryString)
{
	if ( domNode == NULL ) return CXMLDOMNode();
	if ( queryString == NULL ) return CXMLDOMNode();

	MSXML_IXMLDOMNode* o = NULL;

	domNode->selectSingleNode((BSTR)queryString, &o);

	return CXMLDOMNode(o, FALSE);
}

CXMLDOMNode XPath::SelectSingleNode(MSXML_IXMLDOMNode *domNode, const char *queryString)
{
	return SelectSingleNode(domNode, (const unicode_t*)_bstr_t(queryString));
}


SelectionNode XPath::SelectNodes(MSXML_IXMLDOMNode *domNode,
						           const unicode_t *queryString)
{
	if ( domNode == NULL ) return SelectionNode();
	if ( queryString == NULL ) return SelectionNode();

	MSXML_IXMLDOMNodeList* o = NULL;

	domNode->selectNodes((BSTR)queryString, &o);

	return SelectionNode(o, FALSE);
}

SelectionNode XPath::SelectNodes(MSXML_IXMLDOMNode *domNode,
						    const char *queryString )
{
	return SelectNodes(domNode, (const unicode_t *)_bstr_t(queryString));
}



XMLCount_t XPath::SelectNodes(MSXML_IXMLDOMNode *domNode,
						    const unicode_t *queryString,
						    SelectionNode &selectNodes)
{
	selectNodes = SelectNodes(domNode, queryString);
	return selectNodes.GetLength();
}

XMLCount_t XPath::SelectNodes(MSXML_IXMLDOMNode *domNode,
						    const char *queryString,
						    SelectionNode &selectNodes)
{
	selectNodes = SelectNodes(domNode, queryString);
	return selectNodes.GetLength();
}


// ====================================================================================================== 

#if 0

//--------------------------------------------------------------------
// class CXMLHTTPClass
//--------------------------------------------------------------------

const CLSID CXMLHTTPClass::m_ClsId = {0xf6d90f16, 0x9c73, 0x11d3, {0xb3, 0x2e, 0x00, 0xc0, 0x4f, 0x99, 0x0b, 0xb4}};


CXMLHTTPRequest CXMLHTTPClass::CreateXMLHTTPRequest(LPUNKNOWN pUnkOuter /*= NULL*/, DWORD dwClsContext /*= CLSCTX_ALL*/)
{
	IXMLHTTPRequest *lp;
	HRESULT	hr = CoCreateInstance(m_ClsId, pUnkOuter, dwClsContext, __uuidof(IXMLHTTPRequest), (LPVOID*)&lp);
	if (hr != S_OK) AfxThrowComException(hr);
	return lp;
}




//--------------------------------------------------------------------
// class CXMLHTTP26Class
//--------------------------------------------------------------------

const CLSID CXMLHTTP26Class::m_ClsId = {0xf5078f1e, 0xc551, 0x11d3, {0x89, 0xb9, 0x00, 0x00, 0xf8, 0x1f, 0xe2, 0x21}};


CXMLHTTPRequest CXMLHTTP26Class::CreateXMLHTTPRequest(LPUNKNOWN pUnkOuter /*= NULL*/, DWORD dwClsContext /*= CLSCTX_ALL*/)
{
	IXMLHTTPRequest *lp;
	HRESULT	hr = CoCreateInstance(m_ClsId, pUnkOuter, dwClsContext, __uuidof(IXMLHTTPRequest), (LPVOID*)&lp);
	if (hr != S_OK) AfxThrowComException(hr);
	return lp;
}




//--------------------------------------------------------------------
// class CXMLHTTP30Class
//--------------------------------------------------------------------

const CLSID CXMLHTTP30Class::m_ClsId = {0xf5078f35, 0xc551, 0x11d3, {0x89, 0xb9, 0x00, 0x00, 0xf8, 0x1f, 0xe2, 0x21}};


CXMLHTTPRequest CXMLHTTP30Class::CreateXMLHTTPRequest(LPUNKNOWN pUnkOuter /*= NULL*/, DWORD dwClsContext /*= CLSCTX_ALL*/)
{
	IXMLHTTPRequest *lp;
	HRESULT	hr = CoCreateInstance(m_ClsId, pUnkOuter, dwClsContext, __uuidof(IXMLHTTPRequest), (LPVOID*)&lp);
	if (hr != S_OK) AfxThrowComException(hr);
	return lp;
}




//--------------------------------------------------------------------
// class CXMLHTTP40Class
//--------------------------------------------------------------------

//const CLSID CXMLHTTP40Class::m_ClsId = {0x88d969c5, 0xf192, 0x11d4, {0xa6, 0x5f, 0x00, 0x40, 0x96, 0x32, 0x51, 0xe5}};


CXMLHTTPRequest CXMLHTTP40Class::CreateXMLHTTPRequest(LPUNKNOWN pUnkOuter /*= NULL*/, DWORD dwClsContext /*= CLSCTX_ALL*/)
{
	IXMLHTTPRequest *lp = NULL;
//	HRESULT	hr = CoCreateInstance(m_ClsId, pUnkOuter, dwClsContext, __uuidof(IXMLHTTPRequest), (LPVOID*)&lp);
//	if (hr != S_OK) AfxThrowComException(hr);
	HRESULT	hr = CoCreateInstance(__uuidof(XMLHTTP40), pUnkOuter, dwClsContext, __uuidof(IXMLHTTPRequest), (LPVOID*)&lp);
	if (hr != S_OK) AfxThrowComException(hr);
	return lp;
}




//--------------------------------------------------------------------
// class CServerXMLHTTPClass
//--------------------------------------------------------------------

const CLSID CServerXMLHTTPClass::m_ClsId = {0xafba6b42, 0x5692, 0x48ea, {0x81, 0x41, 0xdc, 0x51, 0x7d, 0xcf, 0x0e, 0xf1}};


IServerXMLHTTPRequest2* CServerXMLHTTPClass::CreateServerXMLHTTPRequest2(LPUNKNOWN pUnkOuter /*= NULL*/, DWORD dwClsContext /*= CLSCTX_ALL*/)
{
	IServerXMLHTTPRequest2 *lp;
	HRESULT	hr = CoCreateInstance(m_ClsId, pUnkOuter, dwClsContext, __uuidof(IServerXMLHTTPRequest2), (LPVOID*)&lp);
	if (hr != S_OK) AfxThrowComException(hr);
	return lp;
}




//--------------------------------------------------------------------
// class CServerXMLHTTP30Class
//--------------------------------------------------------------------

const CLSID CServerXMLHTTP30Class::m_ClsId = {0xafb40ffd, 0xb609, 0x40a3, {0x98, 0x28, 0xf8, 0x8b, 0xbe, 0x11, 0xe4, 0xe3}};


IServerXMLHTTPRequest2* CServerXMLHTTP30Class::CreateServerXMLHTTPRequest2(LPUNKNOWN pUnkOuter /*= NULL*/, DWORD dwClsContext /*= CLSCTX_ALL*/)
{
	IServerXMLHTTPRequest2 *lp;
	HRESULT	hr = CoCreateInstance(m_ClsId, pUnkOuter, dwClsContext, __uuidof(IServerXMLHTTPRequest2), (LPVOID*)&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return lp;
}




//--------------------------------------------------------------------
// class CServerXMLHTTP40Class
//--------------------------------------------------------------------

const CLSID CServerXMLHTTP40Class::m_ClsId = {0x88d969c6, 0xf192, 0x11d4, {0xa6, 0x5f, 0x00, 0x40, 0x96, 0x32, 0x51, 0xe5}};


IServerXMLHTTPRequest2* CServerXMLHTTP40Class::CreateServerXMLHTTPRequest2(LPUNKNOWN pUnkOuter /*= NULL*/, DWORD dwClsContext /*= CLSCTX_ALL*/)
{
	IServerXMLHTTPRequest2 *lp;
	HRESULT	hr = CoCreateInstance(m_ClsId, pUnkOuter, dwClsContext, __uuidof(IServerXMLHTTPRequest2), (LPVOID*)&lp);
	if (hr != S_OK) AfxThrowComException(hr);
	return lp;
}

#endif


CXMLDOMElement MatchElement(CXMLDOMElement &contextNode, const unicode_t *elemPath,
							const unicode_t *attrName, const unicode_t *value)
{
	TString<unicode_t> str;

	str = elemPath;
	str += _XSTR("[@");
	str += attrName;
	str += _XSTR("='");
	str += value;
	str += _XSTR("']");

	return contextNode.SelectSingleNode(str);
}

CXMLDOMElement MatchElement(CXMLDOMElement &contextNode, const unicode_t *elemPath,
							const unicode_t *attrName, const unicode_t *value,
							const unicode_t *attrName1, const unicode_t *value1)
{
	TString<unicode_t> str;

	str = elemPath;
	str += _XSTR("[@");
	str += attrName;
	str += _XSTR("='");
	str += value;
	str += _XSTR("' and @");
	str += attrName1;
	str += _XSTR("='");
	str += value1;
	str += _XSTR("']");

	return contextNode.SelectSingleNode(str);
}



void GetXMLErrorMessage(const CXMLDOMDocument &xmlDomDoc, TString<unicode_t> &outMessage)
{
	CXMLDOMParseError error = xmlDomDoc.GetParseError();

//	long filePos = error.GetFilepos();
	long line = error.GetLine();
	long col = error.GetLinepos();

	unicode_t	buff[256];
#if _MSC_VER < 1299
	swprintf(buff, L"%d째 라인의 %d 위치에서 에러가 발생\r\n\r\n", line, col);
#else
	swprintf_s(buff, 256, L"%d째 라인의 %d 위치에서 에러가 발생\r\n\r\n", line, col);
#endif
	outMessage += buff;

	TString<unicode_t> str;

	outMessage += L"원인:\r\n";
	error.GetReason(str);
	outMessage += str;
	outMessage += L"\r\n";
	outMessage += L"\r\n";
	
	outMessage += L"Source:\r\n";
	error.GetSrcText(str);
	outMessage += str;	
}