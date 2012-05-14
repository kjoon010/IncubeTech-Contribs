#pragma once

#include <vector>


/**
  @class	IN3FindResult
  @brief	검색 결과에 대한 데이터
 */
class AFX_EXT_CLASS IN3FindResult
{
public:
	// 생성자
	IN3FindResult(void);
	IN3FindResult(int seq, int page, int startIdx, int endIdx);
	IN3FindResult(int seq, int page, int startIdx, int endIdx, CString sentence);
	IN3FindResult(int seq, int page, int startIdx, int endIdx, CString sentence, CString fileName);

	// 소멸자
	~IN3FindResult(void);

	// setter
	void SetSeq(int seq);
	void SetPage(int page);
	void SetStartIndex(int index);
	void SetEndIndex(int index);
	void SetSentence(CString sentence);
	void SetFileName(CString fileName);

	// getter
	const int GetSeq() const;
	const int GetPage() const;
	const int GetStartIndex() const;
	const int GetEndIndex() const;
	const CString GetSentence() const;
	const CString GetFileName() const;

protected:
	int		m_seq; ///< 시퀀스
	int		m_page; ///< 페이지 (fixed layout Epub 에서만 유효한 값을 가진다)
	int		m_startIdx; ///< 시작 인덱스
	int		m_endIdx; ///< 끝 인덱스

	CString m_sentence; ///< 검색 구문 포함된 전문 일부
	CString m_fileName; ///< 검색 결과가 있는 파일명
};

typedef std::vector<IN3FindResult>	FindResultList;