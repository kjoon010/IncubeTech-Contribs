
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "XMLDom.h"
#include "OPFReader.h"
#include "TStringEx.h"

/*
 * Namespace ����
 */
#define NAMESPACE_URI_NCX_W			USTR("http://www.daisy.org/z3986/2005/ncx/")
#define NAMESPACE_NCX_W				USTR("xmlns:ncx=\"http://www.daisy.org/z3986/2005/ncx/\"")


/**
  @class	NCXXML
  @brief	���� ���� �ļ�. epub 2.0 spec �ؼ�
 */
class NCXXML : public CXMLDOMDocument2
{
public:
	// ������
	NCXXML();

	// �Ҹ���
	~NCXXML();

	// initialize
    void Init(OPFXML &opfXML, const unicode_t* lpzOPFFolderPath); ///< opf ������ �������� ncx �� �����ϴ� �Լ�, ������ ������� �ʴ� �Լ��̴�.

	// get navPoint data
	bool GetUniqueID(TString<unicode_t> &uuidString); ///< get epub's unique ID
	CXMLDOMNodeList GetNavPointList(); ///< get navPoint element List

	bool GetTitleFromURL(const unicode_t* srcURL, TString<unicode_t> &title); ///< url �� ��ġ�ϴ� navPoint element ���� title ����
	
	CXMLDOMElement FindNavPoint(const unicode_t* srcURL); ///< url �� ��ġ�ϴ� navPoint element �� �˻�


protected:
    void Init1(const unicode_t* docTitle, const unicode_t* uniqueID = NULL); ///< initialize
    CXMLDOMElement AddNavPoint(const unicode_t* src, const unicode_t* labeltext, long playOrder = 0); ///< add NavPoint

protected:
	long		mPlayOrder; ///< play order
};

