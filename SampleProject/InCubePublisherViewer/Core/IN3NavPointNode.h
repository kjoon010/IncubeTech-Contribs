#pragma once

#include <vector>

/**
  @class	IN3NavPointNode
  @brief	Epub 의 TOC(목차) 데이터를 표현하는 클래스(트리구조)
 */
class AFX_EXT_CLASS IN3NavPointNode
{
public:
	// 생성자
	IN3NavPointNode(void);
	IN3NavPointNode(LPCTSTR title, LPCTSTR url);
	IN3NavPointNode(LPCTSTR title, LPCTSTR url, UINT page);

	// 소멸자
	~IN3NavPointNode(void);

	// getter
	const CString& GetTitle() const;
	const CString& GetURL() const;
	const UINT GetPageNumber() const;

	// get child node data
	long GetChildCount() const;
	const IN3NavPointNode& GetChild(long i) const;
	      IN3NavPointNode& GetChild(long i);
	
    // add, remove
	void Append(const IN3NavPointNode &navPointNode);
	void Release();
};