#ifndef _IN3IMAGELIST_H_
#define _IN3IMAGELIST_H_

#pragma once

#include <vector>

/**
  @class	CIN3ImageInfo
  @brief	Epub ���� ����� �̹����� ������
			Epub �� ��� �̹����� ����Ǵ� ���� �ƴϸ� �̹��� �� Ư�� Ŭ������ ������ ���� ������ �̹������� �����Ѵ�.
			������ �����͸� EpubImageList �� ��� UI ������ �����Ͽ� UI �ܿ��� �ش� �̹��� ����� ����ڿ��� �����ش�.
 */
class AFX_EXT_CLASS CIN3ImageInfo
{
public:
	// ������
	CIN3ImageInfo(void);
	CIN3ImageInfo(CString filePath, CString imgId, CString imgPath, CString caption, int page = 0);

	// �Ҹ���
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
	CString m_imgID; ///< �̹��� ID
	CString m_filePath; ///< �̹����� �ִ� ���� ��
	CString m_ImagePath; ///< �̹��� ���(�����?)
	CString m_caption; ///< �̹��� ĸ��
	UINT m_pageNum; ///< �̹����� �ִ� ��ġ�� ������ �� (fixed layout Epub ������ ��ȿ�� ���� ������)
};

typedef std::vector<CIN3ImageInfo>	EpubImageList;

#endif