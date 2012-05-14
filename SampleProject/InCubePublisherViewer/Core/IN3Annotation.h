#pragma once


/**
  @class	IN3Annotation
  @brief	annotation�� ���̶���Ʈ/�޸�/������η� ���еȴ�.
 */
class AFX_EXT_CLASS IN3Annotation
{
public:
	// ������
	IN3Annotation(void);
	IN3Annotation(bool bMemo, int start, int end, CString markerID, CString fileName, CString selection, int page =0);
	IN3Annotation(bool bMemo, int start, int end, CString markerID, CString fileName, CString selection,
		CString bgColor, CString ulColor, int page =0);

	// �Ҹ���
	~IN3Annotation(void);

	// initialize
	// default ������ ��� �� init �Լ��� ���� ������ �Ѳ����� ������ �� �ִ�.
	void Init(bool bMemo, int start, int end, CString markerID, CString fileName, CString selection,
		CString bgColor, CString ulColor, int page =0);

	// setter
	void SetMemo(bool bMemo);
	void SetIndex(int start, int end);
	void SetStart(int start);
	void SetEnd(int end);
	void SetMarkerID(CString id);
	void SetFileName(CString fileName);
	void SetSelection(CString str);
	void SetBGColor(CString color);
	void SetULColor(CString color);
	void SetPage(int page);
	void SetAnnotationRect(int left, int top, int right, int bottom);

	// getter
	const bool HasMemo() const;
	const int GetStart() const;
	const int GetEnd() const;
	const CString GetMarkerID() const;
	const CString GetFileName() const;
	const CString GetSelection() const;
	const CString GetBGColor() const;
	const CString GetULColor() const;
	const int GetPage() const;
	const CRect GetAnnotationRect() const;
};



/**
  @class	IN3Bookmark
  @brief	�ϸ�ũ
 */
class AFX_EXT_CLASS IN3Bookmark
{
public:
	// ������
	IN3Bookmark(void);
	IN3Bookmark(int type, CString fileName, CString markerID, int index, int page = 0);

	// �Ҹ���
	~IN3Bookmark(void);

	// setter
	void SetMarkerIndex(int index);		
	void SetFileName(CString fileName);	
	void SetMarkerID(CString id);		
	void SetPage(int page);		

	// getter
	const int GetMarkerIndex() const;
	const CString GetFileName() const;
	const CString GetMarkerID() const;
	const int GetPage() const;
};
