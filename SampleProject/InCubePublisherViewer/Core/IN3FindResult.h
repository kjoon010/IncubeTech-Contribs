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
};

typedef std::vector<IN3FindResult>	FindResultList;