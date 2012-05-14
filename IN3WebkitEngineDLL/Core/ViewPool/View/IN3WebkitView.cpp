// IN3WebkitView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "IN3WebkitView.h"

#include "IN3WebkitDefine.h"
#include "EPubBook.h"

#include "WebkitUIDelegate.h"
#include "WebkitLoadFrameBridge.h"

#include "atlimage.h"

#include <math.h>

// IN3WebkitView

IMPLEMENT_DYNCREATE(IN3WebkitView, CView)

/**
 * @brief	������
 */
IN3WebkitView::IN3WebkitView()
	:m_url(_T(""))
	,m_nCurrChapter(0)
	,m_nTotalPage(0)
	,m_nCurrPage(0)
	,m_nZoomStep(0)
	,m_fixedMultiplier(1.0)
	,m_nLoadState(LS_NONE)
	,m_pWebkitUIDelegate(NULL)
	,m_pWebkitViewBridge(NULL)
{

}

/**
 * @brief	�Ҹ���
 */
IN3WebkitView::~IN3WebkitView()
{
	TRACEMSG(_T("IN3WebkitView::~IN3WebkitView() START"));
	//
	if(m_pWebkitUIDelegate)
		m_pWebkitUIDelegate->Release();

	if ( m_pWebkitViewBridge )
		m_pWebkitViewBridge->Release();
	//
	TRACEMSG(_T("IN3WebkitView::~IN3WebkitView() END"));
}

BEGIN_MESSAGE_MAP(IN3WebkitView, CView)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CWebkitView �׸����Դϴ�.
/**
 * @brief	WebkitView �� OnDraw �Լ�
 */
void IN3WebkitView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.

	CEPubBook* pBook = (CEPubBook*)GetParentFrame();
	if(!pBook) return;

	CRect rect;
	GetClientRect(&rect);

	/*
	 * ���� open �� epub �� reflow �������̸�, ���ܿ��� ������ ������ �ִٸ� �ش� ������ �信 ����
	 */
	if(pBook && !pBook->IsFixed() && !pBook->GetBackgroudColor().IsEmpty())
	{
		int red = 0, green = 0, blue = 0;
		COLORREF backtroundClr = 0;
		CString strBackgroundClr = pBook->GetBackgroudColor();
		if(GetIntColorValue(strBackgroundClr, red, green, blue))
		{
			CBrush cBrush,*oldBrush;
			cBrush.CreateSolidBrush(RGB(red, green, blue));
			oldBrush = pDC->SelectObject(&cBrush);
			//
			pDC->FillRect(rect, &cBrush);
			pDC->SelectObject(oldBrush);
			cBrush.DeleteObject();
		}
	}

	/*
	 * ���� ������ ��ȣ ǥ��
	 * ��� �ܿ��� ������ ǥ�ø� �ϵ��� ������ ���. pBook->IsDisplayPageNumber() �� ���ϰ�(bool) �� Ȯ���� �� �ִ�.
	 */
	if(pBook && pBook->IsDisplayPageNumber())
	{
		int totalPage = pBook->GetTotalPage();		// ��ü ������
		int currPage = pBook->GetCurrentPage();		// ���� ������
		
		// reflow �������� ���/�ܸ� ���⿡ ���� ����, �� ������ ���� �޶���.
		// pBook->GetPageDisplayMode() �� 1(�ܸ�) or 2(���)
		if(!pBook->IsFixed())
		{
			totalPage = (int)(ceil((double)totalPage / (double)pBook->GetPageDisplayMode()));
			currPage = (int)(ceil((double)currPage /(double)pBook->GetPageDisplayMode()));
		}

		// ��ü ������ ���� ���� ������ ���� ��ȿ�ϸ� ȭ�鿡 ������ ������ ǥ���Ѵ�.
		if((currPage >= 0) && (totalPage > 0))
		{
			CRect rectPage(rect.left, (rect.bottom - (int)((float)((float)RECT_PAGEINFO_HEIGHT*0.6))), rect.right, rect.bottom);
			CString strPage;

			// fixed �� ���� �������� ���� ���� �ε��� + 1 �� ����. ���� fixed �� 1page = 1spine file �� �����Ͽ���.
			if(pBook->IsFixed())
				strPage.Format(_T("%d / %d"), m_nCurrChapter + 1, totalPage);
			else
				strPage.Format(_T("%d / %d"), currPage, totalPage);

		
			// 2-page display mode and last chater's location is left
			// ���ʿ� ������ ��ȣ ǥ��.
			if((pBook->IsFixed()) && (pBook->GetPageDisplayMode() == 2) && (m_nCurrChapter == pBook->GetSpineCount()-1) && (m_nCurrChapter %2 == 0))
			{
				rectPage.right /= 2;
			}

			COLORREF oldColor = pDC->SetTextColor(RGB(74, 74, 74));
			pDC->DrawText(strPage, strPage.GetLength(), rectPage, DT_CENTER|DT_VCENTER);
			pDC->SetTextColor(oldColor);
		}
	}
}


// CWebkitView �����Դϴ�.

#ifdef _DEBUG
void IN3WebkitView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void IN3WebkitView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// IN3WebkitView �޽��� ó�����Դϴ�.
/**
 * @brief	IN3WebkitView �� Create �Լ�
 */
BOOL IN3WebkitView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	TRACEMSG(_T("IN3WebkitView::Create() START"));
	//
	// create the view window itself
	if ( !CView::Create(lpszClassName, lpszWindowName,
		dwStyle, rect, pParentWnd, nID, pContext))
	{		
		TRACEMSG_ERR(_T("CView::Create() is failed!!"));
		return FALSE;
	}

	// create webkitViewBridge
	BOOL bRes = FALSE;
	m_pWebkitViewBridge = new CWebkitLoadFrameBridge();
	m_pWebkitViewBridge->m_pWebkitView = this;
	bRes = m_pWebkitViewBridge->Create();

	if(bRes)
		::RevokeDragDrop(m_ViewWindow);

	TRACEMSG(_T("IN3WebkitView::Create() END"));

	return bRes;
}

/**
 * @brief	IN3WebkitView �� �޽��� ó����
 */
BOOL IN3WebkitView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	
	/*
	 * 2012.04.12.ADD.hmkim - ICP 1.0 viewer �ҽ� ����.
	 * view ȭ�鿡�� Drag ��, ȭ�� ������ �Ѿ ���� drag �ϴ� ���,
	 * �Ʒ��� ���� ���� ó��.
	 * �þ� ����ó���� ���� �ʴ� ���, ȭ���� ��� ������ �̵��ϸ鼭 �������� ����Ǵ� ������ �߻���.
	 */
	if (pMsg->message == WM_MOUSEMOVE)
	{
		CPoint	pt = pMsg->pt;
		CRect	rect;
		GetWindowRect(rect);
		if(!rect.PtInRect(pt) && DragDetect(pt))
		{
			return TRUE;
		}
	}

	if(pMsg->message == WM_KEYDOWN)
	{
		/*
		 * �¿� ����Ű�� ���� ������ �̵�
		 */
		if(pMsg->wParam == VK_RIGHT)
		{
			NextPage();
		}
		
		if(pMsg->wParam == VK_LEFT)
		{
			PrevPage();
		}
	}

	/*
	 * ���콺 ������ ��ư Ŭ�� �� ���ؽ�Ʈ �޴� �̺�Ʈ�� UI ������ ����
	 */
	if(pMsg->message == WM_RBUTTONUP)
	{
		///*
		CEPubBook* pBook = (CEPubBook*)this->GetParentFrame();
		if(pBook)
		{
			pBook->SetCurrChapter(m_nCurrChapter);
			pBook->SetActiveView(this);
			pBook->ShowContextMenu(pMsg->pt);
		}
		return TRUE;
		//*/
	}

	if(pMsg->message == WM_LBUTTONUP)
	{
		if(m_pWebkitViewBridge)
		{
			/*
			 * ���콺 ������ �巡�� �� ���õ� ���� ������ �ִٸ� annotation menu �޽����� UI ������ ����
			 */
			if(m_pWebkitViewBridge->IsSelected())
			{
				// annotation Menu
				CEPubBook* pBook = (CEPubBook*)this->GetParentFrame();
				if(pBook)
				{
					pBook->SetCurrChapter(m_nCurrChapter);
					pBook->SetActiveView(this);
					pBook->ShowAnnotationMenu(pMsg->pt, m_pWebkitViewBridge->GetSelectedText());
				}
			}
		}
	}

#if (_WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400)	
	/*
	 * ���콺 �� �̺�Ʈ ����
	 */
	if(pMsg->message == WM_MOUSEWHEEL)
	{
		return TRUE;
	}
#endif

	return CView::PreTranslateMessage(pMsg);
}

/**
 * @brief	������ ������ Loading �Ѵ�.
 * @param	lpszURL	: load �� URL
 * @return	true(success)/false(fail)
 */
BOOL	IN3WebkitView::LoadFromURL(LPCTSTR lpszURL)
{
	TRACEMSG(_T("IN3WebkitView::LoadFromURL() START"));
	//
	// ���� �ε� ���̸� ����ó���Ѵ�.
	if(m_nLoadState == LS_LOADING){
		TRACEMSG_ERR(_T("m_nLoadState == LS_LOADING"));
		return FALSE;
	}
	if(!m_pWebkitViewBridge){
		TRACEMSG_ERR(_T("m_pWebkitViewBridge is NULL !!!"));
		return FALSE;
	}
	//
	// load
	m_url = lpszURL;	
	m_nLoadState = LS_LOADING;
	if ( !m_pWebkitViewBridge->LoadFromURL(lpszURL) )
	{
		m_nLoadState = LS_NONE;
		return FALSE;
	}

	TRACEMSG(_T("IN3WebkitView::LoadFromURL() END"));
	//
	return TRUE;
}

/**
 * @brief	Relaod �Ѵ�
 * @return	true(success)/false(fail)
 */
BOOL	IN3WebkitView::ReLoad()
{
	//
	if(m_nLoadState == LS_LOADING){
		TRACEMSG_ERR(_T("m_nLoadState == LS_LOADING"));
		return FALSE;
	}
	//
	if(!m_pWebkitViewBridge){
		TRACEMSG_ERR(_T("m_pWebkitViewBridge is NULL !!!"));
		return FALSE;
	}

	return m_pWebkitViewBridge->ReLoad();
}

/**
 * @brief	�������� �ʱ�ȭ �Ѵ�.
 * @remarks	��Ʈũ��, ��Ʈ, ��Ʈ��, ���� ���� �����Ͽ� ������ ��� �� ���̾ƿ��� �����Ѵ�.
 * @param	fontSize	: ��Ʈ ũ��
 * @param	familyName	: ��Ʈ��
 * @param	back_clr	: ����
 * @praram	fore_clr	: ��Ʈ��
 */
void IN3WebkitView::InitPage(CString fontSize, CString familyName, CString back_clr, CString fore_clr)
{
	if(m_pWebkitViewBridge)
		m_pWebkitViewBridge->InitPage(fontSize, familyName, back_clr, fore_clr);
}

/**
 * @brief	ȭ�� �Ʒ� ������ ǥ�õǴ� ������ ������ ������Ʈ �Ѵ�.
 */
void IN3WebkitView::InvalidateInfo()
{
	if(m_nLoadState != LS_LODE_DONE) return; 

	// ��ȣ ǥ�õǴ� ���� ���
	CRect rect;
	GetClientRect(&rect);
	CRect rectPage(rect.left, rect.bottom - RECT_PAGEINFO_HEIGHT, rect.right, rect.bottom);

	// ������ ������ ȭ�鿡 ������Ʈ
	InvalidateRect(&rectPage, TRUE);
}

/**
 * @brief	���� �߻� �� ȣ��Ǵ� �Լ�
 * @remarks	���� ���ٸ� ó���� �ϰ� ���� ����.
 */
void	IN3WebkitView::LoadError(BSTR url, BSTR errorMessage)
{
	// @ note
	// display error message ?
}

/**
 * @brief ������ �ε尡 �Ϸ� �������� �����Ѵ�.
 * @remarks ������ �ε� �޷� �� ��Ŷ extension ���κ��� ȣ��ȴ�.
 */
void	IN3WebkitView::DocumentComplete()
{
	m_nLoadState = LS_LODE_DONE;
}

/**
 * @brief ���� ���̴� ��Ŷ ������ ������ ĸ���Ѵ�.
 * @param	[in] width	: ĸ�� �̹����� width
 * @param	[in] height	: ĸ�� �̹����� height
 * @param	[out] bitmap	: ĸ�� �̹���
 * @return	true(success)/false(fail)
 */
bool IN3WebkitView::PageCapture(int width, int height, CBitmap &bitmap)
{
	// ������ �ε尡 �Ϸ�� ���¿����� é�� ����
	if(m_nLoadState != LS_LODE_DONE) return false;
	
	CEPubBook* pBook = (CEPubBook*)this->GetParentFrame();
	if(!pBook) return false;

	bool bRes = true;
	// ������ ������ �ڵ��� �����´�. 
	HDC hdc = ::GetDC(m_ViewWindow);
	if(hdc)
	{
		// ������ ������ �ڵ�� ���� CDC �����͸� ������.
		CDC *pDC = CDC::FromHandle(hdc);
		if(pDC)
		{
			// ���� �信 �׷��� ������ ������ ũ�⸦ �����´�.
			int realWidth = (int)(pBook->GetFixedWidth()*m_fixedMultiplier);
			int realHeight = (int)(pBook->GetFixedHeight()*m_fixedMultiplier);

			// �޸� DC ����
			CDC memDC;
			memDC.CreateCompatibleDC(pDC);

			// ������ �޸� DC �� bitmap �� �����Ѵ�.
			bitmap.CreateCompatibleBitmap(pDC, width, height);
			CBitmap *pOldBitmap = memDC.SelectObject(&bitmap);

		#if 0
			memDC.SetStretchBltMode(COLORONCOLOR);  // set iMode.
			memDC.StretchBlt(0, 0, width, height, pDC, 0, 0, realWidth, realHeight, SRCCOPY);
		#else

			// ȭ�鿡 ���̴� �並 �״�� ������ �޸� DC ����
			CDC orgMemDC;
			orgMemDC.CreateCompatibleDC(pDC);
			
			// �信 ���̴� ũ�� �״�� ������ ��Ʈ�� �̹��� ���� �� �޸� DC �� �����Ѵ�.
			CBitmap orgBitmap;
			orgBitmap.CreateCompatibleBitmap(pDC, realWidth, realHeight);
			CBitmap *pOrgOldBitmap = orgMemDC.SelectObject(&orgBitmap);

			// ȭ�� ����
			typedef BOOL (WINAPI *tPrintWindow)( HWND, HDC,UINT);

			tPrintWindow pPrintWindow = 0;
			HINSTANCE handle = ::LoadLibrary(_T("User32.dll"));
			if ( handle == 0 ) 
				return FALSE;

			pPrintWindow = (tPrintWindow)::GetProcAddress(handle, "PrintWindow");    
			if ( pPrintWindow ) 
				bRes = pPrintWindow(m_ViewWindow, orgMemDC, 0);

			// ������ ȭ���� ���� �����ϰ��� �ϴ� ũ��� resize �Ͽ� �� ���� �Ѵ�.
			memDC.SetStretchBltMode(COLORONCOLOR);  // set iMode.
			memDC.StretchBlt(0, 0, width, height, &orgMemDC, 0, 0, realWidth, realHeight, SRCCOPY);

			//CImage image;
			//image.Attach(orgBitmap);
			//image.StretchBlt(memDC.GetSafeHdc(), 0, 0, width, height, SRCCOPY);

			// �޸� DC ����
			orgMemDC.SelectObject(pOrgOldBitmap);
			orgMemDC.DeleteDC();
			orgBitmap.DeleteObject();
		#endif

			// �޸� DC ����
			memDC.SelectObject(pOldBitmap);
			memDC.DeleteDC();
			ReleaseDC(pDC);

			return bRes;
		}
	}
	return false;
}

/**
 * @brief	���� �������� ȭ�� ũ�⿡ ���� ������ ������ ��ȯ�Ѵ�.
 * @return	������ resizeing ����
 */
const float IN3WebkitView::GetFixedMultiplier()
{
	return m_fixedMultiplier;
}

/**
 * @brief	���� �������� ������ ũ�⿡ ���� ������ ����ϰ�, ���� ������ �����Ѵ�.
 * @remarks	PC�� ����� ũ��� ������ �ƴϹǷ� fixed layout ���� ������ �������� ���� �������� ������ ȭ�� ũ�⿡ ���� �ٽ� resizing �� �ʿ䰡 �ִ�.
 *			���� ��Ŷ���� �����ϴ� SetMultiplier() �� ����Ѵ�.
 *			fixed ������ ������ ���Ǹ� ��ȿ�� ���� ������.
 * @param	width	: ������ ����� ���� ���� (ȭ�鿡 ���� ���� ����)
 * @param	height	; ������ ����� ���� ũ�� (ȭ�鿡 ���� ���� ����)
 * @return	�������� ����
 */
const float IN3WebkitView::GetFixedMultiplier(int width, int height)
{
	CEPubBook* pBook = (CEPubBook*)this->GetParentFrame();
	if(!pBook) return m_fixedMultiplier;
	//
	// ���� ���θ� ������ ����Ѵ�.
	// ���� �������� ���� ũ��� ȭ�鿡 ������ �ϴ� ���� ũ�� ������ ������ ����Ѵ�.
	// ���� ȭ�鿡 ���̴� ũ��� �������� ���� ���� ���� ��� RECT_CHAPTER_HEGIHT(���� ǥ�� ����) + RECT_PAGEINFO_HEIGHT(������ ǥ�� ����) �� ������ �����̴�.
	m_fixedMultiplier = (float)(height - (RECT_CHAPTER_HEGIHT + RECT_PAGEINFO_HEIGHT))/(float)pBook->GetFixedHeight();

	// ���� ������ ���ο� �����Ͽ��� ��, �������� ũ�⺸�� �� ���� ���� ���̰� Ŭ ���,
	// ���� ũ�⺸�� �� �۰� �Ǿ�� �ϹǷ�, ���θ� ������ �ٽ� ������ �� ����Ѵ�.
	if(pBook->GetFixedWidth() * m_fixedMultiplier > width)
	{
		m_fixedMultiplier = (float)width/(float)pBook->GetFixedWidth();
	}
	
	// ȭ�鿡 ���� ���� ������, ����ڰ� ���� ������ ������ ���� ���� ��Ŷ ������ �信 �����Ѵ�.
	// ����ڰ� ���� ������ ������ (+/-)10% �� �����ȴ�.
	float fMultiplier = m_fixedMultiplier + (float)m_nZoomStep/(float)10.0;
	//
	if(m_pWebkitViewBridge)
		m_pWebkitViewBridge->SetMultiplier(fMultiplier);

	return fMultiplier;
}

/**
 * @brief	����ڰ� ���� ������ ������ ��ȯ�Ѵ�.
 * @remarks fixed ������ ������ ���Ǹ� ��ȿ�� ���� ������.
 * @return	����ڰ� ���� ������ ���� �ܰ�
 */
const int IN3WebkitView::GetZoomStep() const
{
	return m_nZoomStep;
}

/**
 * @brief	����ڰ� ���� �����ϴ� ������ �����Ѵ�.
 * @remarks	����ڰ� ���� �����ϴ� �ܰ�� -3 ~ +5 �ܰ���� ������, 0 �� ���� ���� ȭ�鿡 ���� ������ ���Ѵ�.
 * @param	step	: ����ڰ� ���� ������ ���� �ܰ�
 */
void IN3WebkitView::SetZoomStep(int step)
{
	m_nZoomStep = step;
}

/**
 * @brief	����ڰ� ���� �����ϴ� ������ 1�ܰ� Ȯ�� �Ѵ�.
 * @remarks	����ڰ� �����ϴ� �ܰ�� �ִ� +5�ܰ���� �����Ѵ�. ��, �ִ� 5�ܰ�� ȭ�鿡 ���� ������ +50%
 */
void IN3WebkitView::SetZoomIn()
{
	// ������ �ε� �Ϸ� �� ���� �����ϴ�.
	if(m_nLoadState != LS_LODE_DONE) return; 

	if(m_nZoomStep < 5) m_nZoomStep++;
}

/**
 * @brief	����ڰ� ���� �����ϴ� ������ 1�ܰ� ��� �Ѵ�.
 * @remarks	����ڰ� �����ϴ� �ܰ�� �ִ� -3�ܰ���� �����Ѵ�. ��, �ִ� 5�ܰ�� ȭ�鿡 ���� ������ -30%
 *			��, ����� ������ ���� �������� 0% ������ ���� �������� �ʴ´�.
 */
void IN3WebkitView::SetZoomOut()
{
	// ������ �ε� �Ϸ� �� ���� �����ϴ�.
	if(m_nLoadState != LS_LODE_DONE) return; 

	// ����� ������ ���� �������� 0% �̻��� ��쿡�� �����Ѵ�.
	if(m_fixedMultiplier + ((float)m_nZoomStep/10.0) > 0)
	{
		if(m_nZoomStep > -3) m_nZoomStep--;
	}
}




/********************************************************************************
 * Page Interface
 ********************************************************************************/
/**
 * @brief	������ �������� �̵��ϴ� �Լ�
 * @param	page	: �̵��� ������ ( ���� Load �� ������ ������ ���� )
 */
void IN3WebkitView::MoveToPage(int page)
{
	// LOAD �Ϸ� ���¿����� ������ �̵� ����
	if(m_nLoadState != LS_LODE_DONE) return; 

	TRACEMSG(_T("IN3WebkitView::MoveToPage() START"));
	//
	// �̵��� �������� ù������ ���� �۰� ���� �������� ù������ ���� ũ�� �̵��� �������� ù�������� �̵��ϵ��� ����
	if(page <= 0){		
		if(m_nCurrPage > 1)
			page = 1;
		else
		{
			TRACEMSG_ERR(_T("page <= 0 . so can't move page"));
			return;
		}
	}

	// �̵��� �������� �� ������ ���� ũ�� ���� ���̴� �������� ������ �������� �ƴ϶�� ������ �������� �̵��ϵ��� ����
	if(page > m_nTotalPage){	
		if(m_nCurrPage < m_nTotalPage)
			page = m_nTotalPage;
		else
		{
			TRACEMSG_ERR(_T("page >= m_nTotalPage . so can't move page"));
			return;
		}
	}

	// ������ �̵�
	if(m_pWebkitViewBridge){
		CString parm;
		parm.Format(_T("%d"), page);
		HRESULT hr = m_pWebkitViewBridge->SendMessageToEngine(CMD_IN3_PAGE_MOVE, parm);
	}

	TRACEMSG(_T("IN3WebkitView::MoveToPage() END"));
}

/**
 * @brief	���� �������� �̵��ϴ� �Լ�
 */
void IN3WebkitView::NextPage()
{
	if(m_nLoadState != LS_LODE_DONE) return; 

	CEPubBook* pBook = (CEPubBook*)this->GetParentFrame();
	if(!pBook) return;

	// fixed �������� reflow �������� �����Ͽ� �������� �̵��Ѵ�.
	if(pBook->IsFixed())
	{
		// fixed ��麸���� ���� +2, �ܸ��� ���� +1 ��ŭ ���͸� �̵��Ѵ�.
		if(pBook->GetPageDisplayMode() == 2)
		{
			int next = m_nCurrChapter + 2;
			if( next >= pBook->GetSpineCount())
				next = pBook->GetSpineCount()-1;
			pBook->MoveToChapter(next);
		}
		else
			pBook->MoveToChapter(m_nCurrChapter+1);
	}
	else
	{
		// reflow :: ���� �������� ������ �������� ���� �̵�, �ƴϸ� ������ �̵�
		if(m_nCurrPage + pBook->GetPageDisplayMode() <= m_nTotalPage)
		{
			MoveToPage(m_nCurrPage + pBook->GetPageDisplayMode());
		}
		else
			pBook->MoveToChapter(m_nCurrChapter+1);
	}
}

/**
 * @brief	���� �������� �̵��ϴ� �Լ�
 */
void IN3WebkitView::PrevPage()
{
	if(m_nLoadState != LS_LODE_DONE) return; 

	CEPubBook* pBook = (CEPubBook*)this->GetParentFrame();
	if(!pBook) return;

	// fixed �������� reflow �������� �����Ͽ� �������� �̵��Ѵ�.
	if(pBook->IsFixed())
	{
		// fixed ��麸���� ���� -2, �ܸ��� ���� -1 ��ŭ ���͸� �̵��Ѵ�.
		if(pBook->GetPageDisplayMode() == 2)
		{
			int prev = m_nCurrChapter - 2;
			if( prev < 0)
				prev = 0;
			pBook->MoveToChapter(prev, FALSE);
		}
		else
			pBook->MoveToChapter(m_nCurrChapter-1, FALSE);
	}
	else
	{
		// reflow :: ���� �������� ù �������� ���� �̵�, �ƴϸ� ������ �̵�
		if(m_nCurrPage - pBook->GetPageDisplayMode() > 0)
		{
			MoveToPage(m_nCurrPage - pBook->GetPageDisplayMode());
		}
		else
			pBook->MoveToChapter(m_nCurrChapter-1, FALSE);
	}
}

void IN3WebkitView::SetChapter(int nChapter)
{
	m_nCurrChapter = nChapter;
}

void IN3WebkitView::SetTotalPage(int page)
{
	m_nTotalPage = page;
}

/**
 * @brief		���� �������� ���������� �����Ѵ�.
 * @remarks		��Ŷ extension ���� move page �� ���, �̵��� ������ ������ ���� �� ��������� �Ѵ�.
 *				MoveToAnchor() �� extension���� ������ �̵��� �Ѵ�.
 */
void IN3WebkitView::SetCurrPage(int page)
{
	m_nCurrPage = page;
}

/**
 * @brief	���� Load �� url �� ��ȯ�Ѵ�.
 * @return	Load �� url
 */
const CString IN3WebkitView::GetURL() const
{
	return m_url;
}

/**
 * @brief	���� �������� ������ ���° é���ΰ��� ��ȯ�Ѵ�.
 * @return	é�� �ε��� (spine �ε����� �����ϴ�)
 */
const int IN3WebkitView::GetChapterIndex() const
{
	return m_nCurrChapter;
}

/**
 * @brief	���� �������� ������ �� ������ ���� ��ȯ�Ѵ�.
 * @return	���� ������ �� ������ ��
 */
const int IN3WebkitView::GetTotalPage(void) const
{
	return m_nTotalPage;
}

/**
 * @brief	���� �������� �ִ� ������ ���� ��ȯ�Ѵ�.(���� ������ �� ������ ���� �������� �Ѵ�.)
 * @return	���� �������� ������ ��ȣ
 */
const int IN3WebkitView::GetCurrPage(void) const
{
	return m_nCurrPage;
}


/********************************************************************************
 * Communication interface
 ********************************************************************************/
/**
 * @brief	��Ŷ extension ���� �޽����� ������.
 * @param	command	: ��Ŷ extension ���� ��û�ϴ� �׼� ��ɾ�
 * @param	param	: ��Ŷ extension �� �׼� ���࿡ �ʿ��� ������
 * @return	���������� ��Ŷ extension ���� �޽����� ���� ��� true, �׿� false
 */
BOOL IN3WebkitView::SendMessage(CString command, CString param)
{
	if(!m_pWebkitViewBridge) return FALSE;
	
	HRESULT hr = m_pWebkitViewBridge->SendMessageToEngine(command, param);
	return (hr == S_OK);
}

/**
 * @brief	�������� ���� �ݹ� �޽��� �� ���� ó���ؾ� �ϴ� �޽����� ó���ϴ� �Լ�
 * @param	command	: Ŀ�ǵ�
 * @param	param	: �Ķ����
 * @return	S_OK(success)/S_FALSE(fail)
 */
HRESULT	IN3WebkitView::ReceiveMessage(CString command, CString param)
{
	TRACEMSG(_T("IN3WebkitView::ReceiveMessage() START"));
	//
	CEPubBook* pBook = (CEPubBook*)this->GetParentFrame();	
	if(!pBook){
		TRACEMSG_ERR(_T("pBook is NULL!!"));
		return S_FALSE;
	}
	
	HRESULT hr = S_OK;
	// ������ �ε� �Ϸ�
	if( command == CMD_IN3_COMPLETE_FILE_INIT )	
	{
		// fixed �� ���� ���ٸ� ó���� ���� �ʴ´�.
		if(pBook->IsFixed()) return S_OK;

		// ȭ�鿡 ��ũ���� ���ش�.(ȭ�鿡 ���� ���̾ƿ��� �����ϱ� ������ �ݵ�� FALSE ó���Ѵ�.)
		if(m_pWebkitViewBridge) m_pWebkitViewBridge->ShowHideScrollBar(FALSE);

		int page = 0;
		// ��ü ������ ���� �����Ѵ�.
		if(pBook->ParseParam(param, _T("FILE_PAGE_COUNT"), &page))
		{
			CString traceMsg;
			traceMsg.Format(_T("ch: %d, totalpage : %d"), m_nCurrChapter, page);
			TRACEMSG_INFO(traceMsg);

			// ��ü������ ���� ������Ʈ �Ѵ�.
			if(m_nTotalPage != page) m_nTotalPage = page;
			if(m_nCurrPage >= m_nTotalPage) m_nCurrPage = page - 1;

			// LS_LOADING �����̸�, LS_LODE_DONE ���·� ������Ʈ �Ѵ�.
			// å�� ��ü �������� �����Ѵ�.
			if(m_nLoadState == LS_LOADING){
				DocumentComplete();
				pBook->CalculateTotalPage();
			}
			else
				pBook->CalculateTotalPage(FALSE);

			// ���� ������ ���� ������Ʈ �Ѵ�.
			if(pBook->ParseParam(param, _T("CURRENT_PAGE"), &page))
			{
				if((m_nTotalPage >= page) && (page > 0))
				{
					m_nCurrPage = page;
				}
			}
			
			// ȭ���� ������Ʈ �Ѵ�.
			Invalidate();
		}
	}

	// ������ �̵� �Ϸ�
	if( command == CMD_IN3_PAGE_MOVE )
	{
		if(param.IsEmpty()) return S_FALSE;

		int page = 0;
		// �̵��� �������� �����Ͽ� ��ȿ�� ���� ���, ���� �������� ������Ʈ �Ѵ�.
		if(pBook->ParseParam(param, _T("CURRENT_PAGE"), &page))
		{
			if((m_nTotalPage >= page) && (page > 0))
			{
				m_nCurrPage = page;
				InvalidateInfo();
				// 
				// ����� �������� UI �ܿ� �˷��ش�.
				pBook->OnPageMoved();
				// ����� �������� �ϸ�ũ�� ���ԵǾ� �ִ��� Ȯ���Ѵ�.
				pBook->QueryBookmarkStatus();
			}
		}
	}

	// ������ ������Ʈ(anchor)�� �̵� �Ϸ�
	if( command == CMD_IN3_MOVE_TO_OBJECT )
	{
		if(param.IsEmpty()) return S_FALSE;

		int page = 0;
		// �̵��� �������� �����Ͽ� ��ȿ�� ���� ���, ���� �������� ������Ʈ �Ѵ�.
		if(pBook->ParseParam(param, _T("CURRENT_PAGE"), &page))
		{
			pBook->OnMoveToAnchor(page);
			InvalidateInfo();
			// 
			// ����� �������� UI �ܿ� �˷��ش�.
			pBook->OnPageMoved();
			// ����� �������� �ϸ�ũ�� ���ԵǾ� �ִ��� Ȯ���Ѵ�.
			pBook->QueryBookmarkStatus();
		}
	}

	// ��Ʈ��, ���� ���� �Ϸ�
	if( command == CMD_IN3_CHANGE_COLORS )
	{
		// ȭ�� ������Ʈ
		Invalidate();
	}

	// �����ڷ�. �� ����
	if( command == CMD_IN3_OPEN_POPUP_WEB )
	{
		if(!param.IsEmpty())
		{
			// url �ּҸ� �״�� JSON �Ľ��� ��� ������ �߻��Ͽ� �Ľ� ������ ���·� �����Ѵ�.
			param.Replace(_T("\\/"), _T("/"));
			param.Replace(_T("\"{"), _T("{"));
			param.Replace(_T("}\""), _T("}"));
			param.Replace(_T("\\\""), _T("\""));
			// 
			pBook->OpenPopupWeb(param);
		}
	}


	/***
	 * Annotaiton create/modify/delete
	 ***/
	// annotation(highlight/underline/memo) �߰� �Ϸ�
	if( command == CMD_IN3_MARK_ANNOTATION )
	{
		if(!param.IsEmpty()){
			// fixed �������� ���� annotation�� �߰��� ������ ���� ���� �����Ѵ�.
			// fixed �� 1page = 1spine file �̹Ƿ� m_nCurrChapter(0~max) ���� �̿��Ͽ� �����Ѵ�.
			// m_nCurrChapter ���� 0~max-1 �̰� page �� 1~max �� ���ʿ� ��������.
			if(pBook->IsFixed())
				pBook->OnAnnotationAdded(param, m_nCurrChapter+1);
			else
				pBook->OnAnnotationAdded(param);
		}
	}

	// annotation �� ����ڰ� ������ ���, (ex ȭ�鿡�� ���̶���Ʈ, ��������� Ŭ��)
	if( command == CMD_IN3_EVENT_ANNOTATION )
	{
		if(!param.IsEmpty())
		{
			// ���õ� annotation ������ UI �ܿ� ������.
			pBook->SetCurrChapter(m_nCurrChapter);
			pBook->SetActiveView(this);
			pBook->OnAnnotationSelected(param);
		}
	}

	// �޸� �������� ����ڰ� ������ ���
	if( command == CMD_IN3_EVENT_MEMO_ICON )
	{
		if(!param.IsEmpty())
		{
			// ���õ� annotation ������ UI �ܿ� ������.
			pBook->SetCurrChapter(m_nCurrChapter);
			pBook->SetActiveView(this);
			pBook->OnMemoIconSelected(param);
		}
	}

	// annotation ���� �Ϸ�
	if( command == CMD_IN3_DELETE_ANNOTATION )
	{
		if(!param.IsEmpty())
			pBook->OnAnnotationDeleted(param);
	}

	// annotation ���� �Ϸ�
	if( command == CMD_IN3_MODIFY_ANNOTATION )
	{
		if(!param.IsEmpty())
			pBook->OnAnnotationModified(param);
	}

	// annotation �ϰ� ���� �Ϸ�
	if( command == CMD_IN3_INIT_ANNOTATION_LIST )
	{
		if(!param.IsEmpty())
			pBook->CancelSelection(m_nCurrChapter);
	}

	// annotation �޴��� ����ϱ� ���� ��ġ�� ��ȸ
	if( command == CMD_IN3_QUERY_ANNOTATION_POS )
	{
		if(!param.IsEmpty())
			pBook->OnQueryAnnotationPos(param);
	}

	
	/***
	 * Bookmark
	 ***/
	// �ϸ�ũ �߰� �Ϸ�
	if( command == CMD_IN3_SET_BOOKMARK )
	{
		if(!param.IsEmpty()){
			// fixed �������� ���� annotation�� �߰��� ������ ���� ���� �����Ѵ�.
			// fixed �� 1page = 1spine file �̹Ƿ� m_nCurrChapter(0~max) ���� �̿��Ͽ� �����Ѵ�.
			// m_nCurrChapter ���� 0~max-1 �̰� page �� 1~max �� ���ʿ� ��������.
			if(pBook->IsFixed())
				pBook->OnBookmarkAdded(param, m_nCurrChapter+1);
			else
				pBook->OnBookmarkAdded(param);
		}
		//
		pBook->QueryBookmarkStatus();
	}

	// �ϸ�ũ ���� �Ϸ�
	if( command == WM_BOOKMARK_DELETED )
	{
		if(!param.IsEmpty())
			pBook->OnBookmarkDeleted(param);
		//
		pBook->QueryBookmarkStatus();
	}

	// �ϸ�ũ ���� �Ϸ�
	if( command == WM_BOOKMARK_CLEARED )
	{
		if(!param.IsEmpty())
			pBook->OnBookmarkCleared(param);
		//
		pBook->QueryBookmarkStatus();
	}

	// �ϸ�ũ�� ���ԵǾ� �ִ��� üũ �Ϸ�
	if( command == CMD_IN3_EVENT_BOOKMARK_STATUS )
	{
		if(pBook->IsFixed())
		{
			if(!param.IsEmpty())
				pBook->OnEventBookmarkStatus(param, m_nCurrChapter+1);
		}
		else
		{
			if(!param.IsEmpty())
				pBook->OnEventBookmarkStatus(param, -1);
		}
	}

	/*
	 * FInd Text
	 */
	// ���� ���� �˻� �Ϸ�
	if(command == CMD_IN3_FIND_TEXT)
	{
		if(param.IsEmpty()) return S_FALSE;

		int page, forward;
		CString keyword;
		bool bMatches;
		// �����˻� ��� �ִ��� Ȯ��(bMatches), �˻� ����� �ִ� ������(page), �˻� Ű����(keyword), �˻� ����(forward) ����
		pBook->ParseParam(param, _T("MATCHES"), &bMatches);
		pBook->ParseParam(param, _T("CURRENT_PAGE"), &page);
		pBook->ParseParam(param, _T("KEYWORD"), &keyword);
		pBook->ParseParam(param, _T("FORWARD"), &forward);
		//if(pBook->ParseFindTextParam(param, bMatches, bForward, page, keyword))
		//{
			// ���� �˻� ����� �ִ��� UI �ܿ� �˷��ش�.
			pBook->OnFindText(bMatches);

			// �˻� ����� �ִ� �������� ���� �������� ������Ʈ �Ѵ�.
			if((m_nTotalPage >= page) && (page > 0))
			{
				m_nCurrPage = page;
				InvalidateInfo();
				//
				pBook->QueryBookmarkStatus();
			}

			// ���� �˻� ����� �ִٸ�, �ϴ� ���� �˻��� ��� �������� �̵�
			// ���� �˻� ����� ���ٸ�, �˻� ���⿡ ���� ����, ���� é�Ϳ��� ��˻�
			if(bMatches)
			{
				// ��Ŷ extension ���� �̵����༭ ���� �̵������� �ʾƵ� �ǳ�, ���̾ƿ��� Ʋ�����ų� ȭ���� ������ ���� ������ 
				// �ذ��ϱ� ���� ��������� �ٽ� �ѹ� ��� �������� �̵��ϵ��� �Ѵ�.
				if(pBook->GetCurrentChapter() != m_nCurrChapter)
				{
					// fixed ���̾ƿ� ����� ��쿡 ���ʿ� ��� �˻� ����� �ִٸ� ���� é�Ϳ��� ������ é�ͷ� �̵� ��
					// ������ �˻� ����� ��� ǥ�ð� �Ǵ� ������ �߻��Ѵ�.
					// ����, �ش� ��쿡�� ���� é�Ϳ� �ִ� �˻� ��� ���� ǥ�ø� �������ش�.
					if(pBook->IsFixed() && (pBook->GetPageDisplayMode() == 2))
					{
						pBook->CancelSelection();
					}
					pBook->MoveToChatperAndPage(m_nCurrChapter, page);		
				}
			}
			else
			{
				if(forward == 1)
					pBook->OnFindTextInNextChapter(keyword, m_nCurrChapter+1);
				else
					pBook->OnFindTextInPrevChapter(keyword, m_nCurrChapter-1);
			}
		//}
	}

	// ���� ���� �˻�(all)
	if(command == CMD_IN3_FIND_TEXT_ALL)
	{
		// �˻� ����� �ִٸ� UI ������ �����Ѵ�.
		if(!param.IsEmpty())
		{
			pBook->OnFindTextAll(param);
		}
	}
	//
	TRACEMSG(_T("IN3WebkitView::ReceiveMessage() END"));
	return hr;
}

/**
 * @brief	IN3WebkitView �� OnSize() �Լ�
 * @remarks	ȭ�� ����� ���� ��Ŷ ������ ������ �����Ͽ� ȭ�鿡 ǥ���Ѵ�.
 */
void IN3WebkitView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CEPubBook* pBook = (CEPubBook*)GetParentFrame();
	if(pBook)
	{
		// fixed �������� reflow  �������� ȭ�鿡 ǥ���ϴ� ����� �ٸ���.
		// fixed �� ���� �������� ũ�Ⱑ ������ �־�, ���� ��� ũ�⿡ ���� �������� ũ�⸦ �����Ͽ� ������� �Ѵ�.
		if(pBook->IsFixed())
		{
			// fixed �������� ��� �����ϴ� ��� ȭ���� ������ ���� ���ʿ� �����ʿ� ���� �ٸ� �並 �����ְ� �ȴ�.
			// �̶� ������ �䰡 ���ʿ� ���� �Ǹ� ������ ���� ũ��� ��� ũ���� ���� �ƴ� �ܸ麸��� �����ϰ� ��ü�� ����ؾ� �Ѵ�.
			// �ٸ� ��� ���������� �ݸ� ����ϰ� �Ǹ�, ������ ������ �� ������ �ƴ� ������ �ʴ� �ٸ� �䰡 ���� �� �ִ�.
			// ����, ��麸���� ��� ������ �䰡 ���ʿ� ���� �Ǵ� ���� �� ���� ��츦 �и��Ͽ� ó���Ѵ�.
			
			// ��麸�� �鼭, ������ ���̰�, ȭ���� ���ʿ� �������� ���
			if((pBook->GetPageDisplayMode() == 2) && (m_nCurrChapter == pBook->GetSpineCount()-1) && (m_nCurrChapter %2 == 0))
			{
				// ���� ũ�Ⱑ ����� ũ��� �����ϹǷ�, ���� ������ �� ������ ���� ����ũ���� ������ ����Ͽ� ȭ�� Ȯ��/��� ������ ���Ѵ�.
				// ��, ���� ���� ũ��� ���ũ��� �����ϳ� �ٸ� é�͵��� �䰡 ����� ũ���� ���� ����ϹǷ�, ������ ������ ���̰� �ϱ� ���� �� ����ũ���� ���� ����Ѵ�.
				float multiplier = GetFixedMultiplier((cx/2), cy);
				if(multiplier > 0.0)
				{
					// ���� ������ ���� ȭ�鿡 ������ �ϴ� ������ ������ ũ�⸦ ���Ѵ�.
					bool bScroll = false;
					int height = int(pBook->GetFixedHeight() * multiplier);
					int width = int(pBook->GetFixedWidth() * multiplier);

					// ������ ������ x,y ��ǥ�� ���Ѵ�.
					// �̶� �������� ���̴� ������ ���� ����� ��ġ�ϵ��� ����Ѵ�.
					int startX = (cx/2) - width;
					int startY = RECT_CHAPTER_HEGIHT;
					int posY = int( ((cy - (RECT_CHAPTER_HEGIHT + RECT_PAGEINFO_HEIGHT)) - (pBook->GetFixedHeight() * multiplier))/2 );
					startY += posY;

					// �������� �������� ���� ����, ����ڰ� ���� Ȯ��/��� �Ͽ� ������ �� ������ ������ ū ���
					// ��ũ���� ������� �Ѵ�.
					if(height > (cy - (RECT_CHAPTER_HEGIHT + RECT_PAGEINFO_HEIGHT))){
						startY = RECT_CHAPTER_HEGIHT;
						height = cy - (RECT_CHAPTER_HEGIHT + RECT_PAGEINFO_HEIGHT);
						bScroll |= true;
					}
					if(width > (cx/2)){
						startX = 0;
						width = cx/2;
						bScroll |= true;
					}

					if(m_pWebkitViewBridge)
						m_pWebkitViewBridge->ShowHideScrollBar(bScroll);

					::MoveWindow(m_ViewWindow, startX, startY, width, height, FALSE);

					return;
				}
			}
			else
			{
				// ���� ũ�⿡ ���� �������� ������ �ϴ� ������ ���Ѵ�.
				bool bScroll = false;
				float multiplier = GetFixedMultiplier(cx, cy);
				if(multiplier > 0.0)
				{
					// ���� ������ ���� ȭ�鿡 ������ �ϴ� ������ ������ ũ�⸦ ���Ѵ�.
					int height = int( pBook->GetFixedHeight() * multiplier );
					int width = int( pBook->GetFixedWidth() * multiplier );
					//
					// ������ ������ x,y ��ǥ�� ���Ѵ�.
					// �̶� �������� ���̴� ������ ���� ����� ��ġ�ϵ��� ����Ѵ�.
					int startX = (cx - width)/2;
					int startY = RECT_CHAPTER_HEGIHT;
					int posY = int( ((cy - (RECT_CHAPTER_HEGIHT + RECT_PAGEINFO_HEIGHT)) - (pBook->GetFixedHeight() * multiplier))/2 );
					startY += posY;

					// �������� �������� ���� ����, ����ڰ� ���� Ȯ��/��� �Ͽ� ������ �� ������ ������ ū ���
					// ��ũ���� ������� �Ѵ�.
					if(height > (cy - (RECT_CHAPTER_HEGIHT + RECT_PAGEINFO_HEIGHT))){
						startY = RECT_CHAPTER_HEGIHT;
						height = cy - (RECT_CHAPTER_HEGIHT + RECT_PAGEINFO_HEIGHT);
						bScroll |= true;
					}
					if(width > cx){
						startX = 0;
						width = cx;
						bScroll |= true;
					}

					if(m_pWebkitViewBridge)
						m_pWebkitViewBridge->ShowHideScrollBar(bScroll);

					// ���� ��� ������ �並 �߾� ���� �� ����� �� ����� �������� �������� ���̰� �ϱ� ���ؼ�, 
					// ���� ��ġ�� ���� x ��ǥ�� �ٸ��� ����Ѵ�.
					if(pBook->GetPageDisplayMode() == 2)
					{
						if(m_nCurrChapter % 2 == 0)
							startX = cx-width;
						else
							startX = 0;
						::MoveWindow(m_ViewWindow, startX, startY, width, height, FALSE);
					}
					else
					{
						::MoveWindow(m_ViewWindow, startX, startY, width, height, FALSE); 
						return;
					}
				}
			}
		}
		else
		{
			// reflow content
			::MoveWindow(m_ViewWindow, RECT_REFLOW_SIDE_GAP, RECT_CHAPTER_HEGIHT, 
				cx - (RECT_REFLOW_SIDE_GAP*2), cy - (RECT_CHAPTER_HEGIHT + RECT_PAGEINFO_HEIGHT), FALSE);
		}
	}
	else
	{
		::MoveWindow(m_ViewWindow, 0, 0, cx, cy, FALSE);
	}
}

/**
 * @brief	Į���� ������ ��Ʈ������ ���� Į���� �����ϴ� �Լ�
 * @remarks	'rgb(x, x, x)' �� ��Ʈ������ �� Į���� ���� integer ������ �����Ѵ�.
 * @param	[in] strColor	: ������ Į���� ������ ��Ʈ��
 * @param	[out] red		: ������(0~255)
 * @param	[out] green		: �ʷϻ�(0~255)
 * @param	[out] blue		: �Ķ���(0~255)
 * @return	true(success)/false(fail)
 */
bool IN3WebkitView::GetIntColorValue(CString strColor, int &red, int &green, int &blue)
{
	if(strColor.IsEmpty()) return false;
	//
	// '#0xffffff' �� 'rbg(x,x,x)' �� ��츦 �и��Ͽ� �����Ѵ�.
	int index = strColor.Find(_T("#"));
	if(index >= 0)
	{
		if(strColor.GetLength() == 7)
		{
			_stscanf_s(strColor, _T("#%02x%02x%02x"), &red, &green, &blue);
			return true;
		}
	}
	else
	{
		int start = strColor.Find(_T("(")), comma = 0;
		if(start >= 0)
		{
			comma = strColor.Find(_T(","), start);
			if(comma > start)
			{
				CString value = strColor.Mid(start+1, comma - start - 1);
				red = _wtoi(value);
			}

			start = comma + 1;
			comma = strColor.Find(_T(","), start);
			if(comma > start)
			{
				CString value = strColor.Mid(start, comma - start);
				green = _wtoi(value);
			}

			start = comma + 1;
			comma = strColor.Find(_T(","), start);
			if(comma > start)
			{
				CString value = strColor.Mid(start, comma - start);
				blue = _wtoi(value);
			}
			else
			{
				comma = strColor.Find(_T(")"), start);
				if(comma > start)
				{
					CString value = strColor.Mid(start, comma - start);
					blue = _wtoi(value);
				}
			}
			return true;
		}
	}
	//
	return false;
}