#ifndef _IN3IMAGELIST_H_
#define _IN3IMAGELIST_H_

#pragma once

#include <vector>

/**
  @class	CIN3ImageInfo
  @brief	Epub 에서 추출된 이미지의 데이터
			Epub 의 모든 이미지가 추출되는 것이 아니며 이미지 중 특정 클래스를 가지는 추출 가능한 이미지들을 추출한다.
			추출한 데이터를 EpubImageList 에 담아 UI 단으로 전달하여 UI 단에서 해당 이미지 목록을 사용자에게 보여준다.
 */
class AFX_EXT_CLASS CIN3ImageInfo
{
public:
	// 생성자
	CIN3ImageInfo(void);
	CIN3ImageInfo(CString filePath, CString imgId, CString imgPath, CString caption, int page = 0);

	// 소멸자
	virtual ~CIN3ImageInfo(void);

	// getter
	const CString GetImageID();
	const CString GetFilePath();
	const CString GetImagePath();
	const CString GetCaption();
	const UINT GetPageNumber();

	// setter
	void SetImgID(const CString strId);
	void SetFilePath(const CString strFilePath);
	void SetImagePath(const CString strImagePath);
	void SetCaption(const CString strCaption);
	void SetPageNumber(const UINT pageNum);


private:
	CString m_imgID; ///< 이미지 ID
	CString m_filePath; ///< 이미지가 있는 파일 명
	CString m_ImagePath; ///< 이미지 경로(썸네일?)
	CString m_caption; ///< 이미지 캡션
	UINT m_pageNum; ///< 이미지가 있는 위치의 페이지 값 (fixed layout Epub 에서만 유효한 값을 가진다)
};

typedef std::vector<CIN3ImageInfo>	EpubImageList;

#endif