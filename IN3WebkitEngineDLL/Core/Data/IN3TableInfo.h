
#pragma once

#include <vector>


/**
  @class	CIN3TableInfo
  @brief	Epub 에서 추출된 표 데이터
			Epub 의 모든 표가 추출되는 것이 아니며 표 중 특정 클래스를 가지는 추출 가능한 표들을 추출한다.
			추출한 데이터를 EpubTableList 에 담아 UI 단으로 전달하여 UI 단에서 해당 표 목록을 사용자에게 보여준다.
 */
class AFX_EXT_CLASS CIN3TableInfo
{
public:
	// 생성자
	CIN3TableInfo(void);
	CIN3TableInfo(CString fileName, CString tableId, CString caption, int page = 0);

	// 소멸자
	~CIN3TableInfo(void);

	// setter
	void SetTableID(CString strID);
	void SetFileName(CString fileName);
	void SetCaption(CString caption);
	void SetPage(int page);

	// getter
	const CString GetTableID() const;
	const CString GetFileName() const;
	const CString GetCaption() const;
	const int GetPage() const;

protected:
	CString m_tableID; ///< 표 ID
	CString m_fileName; ///< 표가 있는 파일 명
	CString m_caption; ///< 표의 캡션
	int		m_page; ///< 표가 있는 페이지 값 (fixed layout Epub 에서만 유효한 값을 가진다)
};

typedef std::vector<CIN3TableInfo>	EpubTableList;