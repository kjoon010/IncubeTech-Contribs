#pragma once


// IN3WebkitView ���Դϴ�.

class IN3WebkitView : public CView
{
	DECLARE_DYNCREATE(IN3WebkitView)

public:
	IN3WebkitView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~IN3WebkitView();

public:
	virtual void OnDraw(CDC* pDC);      // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	virtual BOOL	PreTranslateMessage(MSG* pMsg);

	/*
	 * LoadError
	 * Load �� Error �� �߻��ϴ� ��� ȣ��ȴ�
	 * �Ķ����	url				Load �� url
	 *			errorMessage	���� �޽���
	 */
	virtual void	LoadError(BSTR url, BSTR errorMessage);

	/*
	 * DocumentComplete
	 * LoadURL �� �Ϸ� �Ǹ� ȣ��Ǵ� �Լ�
	 */
	virtual	void	DocumentComplete();

	/*
	 * LoadFromURL
	 * ������ URL �� Load �Ѵ�.
	 * �Ķ���� lpszURL url
	 */
	BOOL	LoadFromURL(LPCTSTR lpszURL);


	
	/******************************************************************
	 * Page Interface
	 ******************************************************************/
	/*
	 * GetURL
	 * Load �Ǵ� url �ּҸ� ��ȯ�Ѵ�.
	 * ���ϰ� Load �� url
	 */
	const CString GetURL() const;

	/*
	 * GetChapterIndex
	 * ��Ŷ �信�� Load �� é�� ������ spine �ε��� ���� ��ȯ�Ѵ�.
	 * ���ϰ� spine �ε��� ��
	 */
	const int GetChapterIndex() const;

	/*
	 * GetTotalPage
	 * ��Ŷ �信�� Load�� ���� ������ total page �� ��ȯ�Ѵ�.
	 * ���ϰ� ���� Load �� ������ total page
	 */
	const int GetTotalPage(void) const;

	/*
	 * GetCurrPage
	 * ���� �������� �ִ� �������� ��ȯ�Ѵ�.
	 * ���ϰ� ���� �������� ������
	 */
	const int GetCurrPage(void) const;

	/*
	 * MoveToPage
	 * ������ �������� �̵��Ѵ�.
	 * �Ķ���� �̵��� ������
	 */
	void MoveToPage(int page);





	/******************************************************************
	 * Communication interface
	 ******************************************************************/
	/*
	 * ReceiveMessage
	 * �������� ���� �ݹ� �޽��� �� ���� ó���ؾ� �ϴ� �޽����� ó���ϴ� �Լ�
	 * �Ķ����	command	Ŀ�ǵ�
	 *			param	�Ķ����
	 */
	virtual HRESULT	ReceiveMessage(CString command, CString param);


protected:
	DECLARE_MESSAGE_MAP()
};


