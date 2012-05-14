// IN3WebkitView.cpp : 구현 파일입니다.
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
 * @brief	생성자
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
 * @brief	소멸자
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


// CWebkitView 그리기입니다.
/**
 * @brief	WebkitView 의 OnDraw 함수
 */
void IN3WebkitView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.

	CEPubBook* pBook = (CEPubBook*)GetParentFrame();
	if(!pBook) return;

	CRect rect;
	GetClientRect(&rect);

	/*
	 * 현재 open 된 epub 이 reflow 컨텐츠이며, 뷰어단에서 설정한 배경색이 있다면 해당 배경색을 뷰에 적용
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
	 * 현재 페이지 번호 표시
	 * 뷰어 단에서 페이지 표시를 하도록 설정한 경우. pBook->IsDisplayPageNumber() 의 리턴값(bool) 로 확인할 수 있다.
	 */
	if(pBook && pBook->IsDisplayPageNumber())
	{
		int totalPage = pBook->GetTotalPage();		// 전체 페이지
		int currPage = pBook->GetCurrentPage();		// 현재 페이지
		
		// reflow 컨텐츠는 양면/단면 보기에 따라 현재, 총 페이지 수가 달라짐.
		// pBook->GetPageDisplayMode() 는 1(단면) or 2(양면)
		if(!pBook->IsFixed())
		{
			totalPage = (int)(ceil((double)totalPage / (double)pBook->GetPageDisplayMode()));
			currPage = (int)(ceil((double)currPage /(double)pBook->GetPageDisplayMode()));
		}

		// 전체 페이지 수와 현재 페이지 값이 유효하면 화면에 페이지 정보를 표시한다.
		if((currPage >= 0) && (totalPage > 0))
		{
			CRect rectPage(rect.left, (rect.bottom - (int)((float)((float)RECT_PAGEINFO_HEIGHT*0.6))), rect.right, rect.bottom);
			CString strPage;

			// fixed 의 현재 페이지는 현재 쳅터 인덱스 + 1 과 동일. 현재 fixed 는 1page = 1spine file 로 정의하였음.
			if(pBook->IsFixed())
				strPage.Format(_T("%d / %d"), m_nCurrChapter + 1, totalPage);
			else
				strPage.Format(_T("%d / %d"), currPage, totalPage);

		
			// 2-page display mode and last chater's location is left
			// 왼쪽에 페이지 번호 표시.
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


// CWebkitView 진단입니다.

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


// IN3WebkitView 메시지 처리기입니다.
/**
 * @brief	IN3WebkitView 의 Create 함수
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
 * @brief	IN3WebkitView 의 메시지 처리기
 */
BOOL IN3WebkitView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	
	/*
	 * 2012.04.12.ADD.hmkim - ICP 1.0 viewer 소스 참고.
	 * view 화면에서 Drag 시, 화면 영역을 넘어서 까지 drag 하는 경우,
	 * 아래와 같이 리턴 처리.
	 * 먄약 리턴처리를 하지 않는 경우, 화면이 계속 옆으로 이동하면서 페이지가 변경되는 현상이 발생함.
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
		 * 좌우 방향키에 따른 페이지 이동
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
	 * 마우스 오른쪽 버튼 클릭 시 컨텍스트 메뉴 이벤트를 UI 단으로 전달
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
			 * 마우스 오른쪽 드래그 시 선택된 본문 내용이 있다면 annotation menu 메시지를 UI 단으로 전달
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
	 * 마우스 휠 이벤트 막음
	 */
	if(pMsg->message == WM_MOUSEWHEEL)
	{
		return TRUE;
	}
#endif

	return CView::PreTranslateMessage(pMsg);
}

/**
 * @brief	임의의 파일을 Loading 한다.
 * @param	lpszURL	: load 할 URL
 * @return	true(success)/false(fail)
 */
BOOL	IN3WebkitView::LoadFromURL(LPCTSTR lpszURL)
{
	TRACEMSG(_T("IN3WebkitView::LoadFromURL() START"));
	//
	// 현재 로딩 중이면 에러처리한다.
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
 * @brief	Relaod 한다
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
 * @brief	페이지를 초기화 한다.
 * @remarks	폰트크기, 폰트, 폰트색, 배경색 등을 적용하여 페이지 계산 및 레이아웃을 정리한다.
 * @param	fontSize	: 폰트 크기
 * @param	familyName	: 폰트명
 * @param	back_clr	: 배경색
 * @praram	fore_clr	: 폰트색
 */
void IN3WebkitView::InitPage(CString fontSize, CString familyName, CString back_clr, CString fore_clr)
{
	if(m_pWebkitViewBridge)
		m_pWebkitViewBridge->InitPage(fontSize, familyName, back_clr, fore_clr);
}

/**
 * @brief	화면 아래 영역에 표시되는 페이지 정보를 업데이트 한다.
 */
void IN3WebkitView::InvalidateInfo()
{
	if(m_nLoadState != LS_LODE_DONE) return; 

	// 번호 표시되는 영역 계산
	CRect rect;
	GetClientRect(&rect);
	CRect rectPage(rect.left, rect.bottom - RECT_PAGEINFO_HEIGHT, rect.right, rect.bottom);

	// 페이지 정보를 화면에 업데이트
	InvalidateRect(&rectPage, TRUE);
}

/**
 * @brief	에러 발생 시 호출되는 함수
 * @remarks	현재 별다른 처리를 하고 있지 않음.
 */
void	IN3WebkitView::LoadError(BSTR url, BSTR errorMessage)
{
	// @ note
	// display error message ?
}

/**
 * @brief 페이지 로드가 완료 상태임을 설정한다.
 * @remarks 페이지 로드 왼료 후 웻킷 extension 으로부터 호출된다.
 */
void	IN3WebkitView::DocumentComplete()
{
	m_nLoadState = LS_LODE_DONE;
}

/**
 * @brief 현재 보이는 웹킷 랜더링 영역을 캡쳐한다.
 * @param	[in] width	: 캡쳐 이미지의 width
 * @param	[in] height	: 캡쳐 이미지의 height
 * @param	[out] bitmap	: 캡쳐 이미지
 * @return	true(success)/false(fail)
 */
bool IN3WebkitView::PageCapture(int width, int height, CBitmap &bitmap)
{
	// 페이지 로드가 완료된 상태에서만 챕쳐 가능
	if(m_nLoadState != LS_LODE_DONE) return false;
	
	CEPubBook* pBook = (CEPubBook*)this->GetParentFrame();
	if(!pBook) return false;

	bool bRes = true;
	// 랜더링 영역의 핸들을 가져온다. 
	HDC hdc = ::GetDC(m_ViewWindow);
	if(hdc)
	{
		// 랜더링 영역의 핸들로 부터 CDC 포인터를 가져옴.
		CDC *pDC = CDC::FromHandle(hdc);
		if(pDC)
		{
			// 실제 뷰에 그려진 랜더링 영역의 크기를 가져온다.
			int realWidth = (int)(pBook->GetFixedWidth()*m_fixedMultiplier);
			int realHeight = (int)(pBook->GetFixedHeight()*m_fixedMultiplier);

			// 메모리 DC 생성
			CDC memDC;
			memDC.CreateCompatibleDC(pDC);

			// 생성한 메모리 DC 와 bitmap 을 연결한다.
			bitmap.CreateCompatibleBitmap(pDC, width, height);
			CBitmap *pOldBitmap = memDC.SelectObject(&bitmap);

		#if 0
			memDC.SetStretchBltMode(COLORONCOLOR);  // set iMode.
			memDC.StretchBlt(0, 0, width, height, pDC, 0, 0, realWidth, realHeight, SRCCOPY);
		#else

			// 화면에 보이는 뷰를 그대로 복사할 메모리 DC 생성
			CDC orgMemDC;
			orgMemDC.CreateCompatibleDC(pDC);
			
			// 뷰에 보이는 크기 그대로 복사할 비트맵 이미지 생성 후 메모리 DC 와 연결한다.
			CBitmap orgBitmap;
			orgBitmap.CreateCompatibleBitmap(pDC, realWidth, realHeight);
			CBitmap *pOrgOldBitmap = orgMemDC.SelectObject(&orgBitmap);

			// 화면 복사
			typedef BOOL (WINAPI *tPrintWindow)( HWND, HDC,UINT);

			tPrintWindow pPrintWindow = 0;
			HINSTANCE handle = ::LoadLibrary(_T("User32.dll"));
			if ( handle == 0 ) 
				return FALSE;

			pPrintWindow = (tPrintWindow)::GetProcAddress(handle, "PrintWindow");    
			if ( pPrintWindow ) 
				bRes = pPrintWindow(m_ViewWindow, orgMemDC, 0);

			// 복사한 화면을 실제 적용하고자 하는 크기로 resize 하여 재 복사 한다.
			memDC.SetStretchBltMode(COLORONCOLOR);  // set iMode.
			memDC.StretchBlt(0, 0, width, height, &orgMemDC, 0, 0, realWidth, realHeight, SRCCOPY);

			//CImage image;
			//image.Attach(orgBitmap);
			//image.StretchBlt(memDC.GetSafeHdc(), 0, 0, width, height, SRCCOPY);

			// 메모리 DC 해제
			orgMemDC.SelectObject(pOrgOldBitmap);
			orgMemDC.DeleteDC();
			orgBitmap.DeleteObject();
		#endif

			// 메모리 DC 해제
			memDC.SelectObject(pOldBitmap);
			memDC.DeleteDC();
			ReleaseDC(pDC);

			return bRes;
		}
	}
	return false;
}

/**
 * @brief	실제 컨텐츠을 화면 크기에 맞춘 임의의 비율을 반환한다.
 * @return	컨텐츠 resizeing 비율
 */
const float IN3WebkitView::GetFixedMultiplier()
{
	return m_fixedMultiplier;
}

/**
 * @brief	실제 컨텐츠를 임의의 크기에 맞춘 비율을 계산하고, 계산된 비율을 적용한다.
 * @remarks	PC용 뷰어의 크기는 고정이 아니므로 fixed layout 으로 지정된 컨텐츠는 실제 컨텐츠의 비율을 화면 크기에 맞춰 다시 resizing 할 필요가 있다.
 *			실제 웹킷에서 지원하는 SetMultiplier() 를 사용한다.
 *			fixed 컨텐츠 에서만 사용되며 유효한 값을 가진다.
 * @param	width	: 비율을 계산할 가로 길이 (화면에 보일 가로 길이)
 * @param	height	; 비율을 계산할 세로 크기 (화면에 보일 세로 길이)
 * @return	재조정된 비율
 */
const float IN3WebkitView::GetFixedMultiplier(int width, int height)
{
	CEPubBook* pBook = (CEPubBook*)this->GetParentFrame();
	if(!pBook) return m_fixedMultiplier;
	//
	// 먼저 세로를 기준을 계산한다.
	// 실제 컨텐츠의 세로 크기와 화면에 보여야 하는 세로 크기 사이의 비율을 계산한다.
	// 실제 화면에 보이는 크기는 윈도우의 세로 길이 에서 상단 RECT_CHAPTER_HEGIHT(쳅터 표시 영역) + RECT_PAGEINFO_HEIGHT(페이지 표시 영역) 을 제외한 영역이다.
	m_fixedMultiplier = (float)(height - (RECT_CHAPTER_HEGIHT + RECT_PAGEINFO_HEIGHT))/(float)pBook->GetFixedHeight();

	// 계산된 비율을 가로에 적용하였을 때, 윈도우의 크기보다 재 계산된 가로 길이가 클 경우,
	// 계산된 크기보다 더 작게 되어야 하므로, 가로를 기준을 다시 비율을 재 계산한다.
	if(pBook->GetFixedWidth() * m_fixedMultiplier > width)
	{
		m_fixedMultiplier = (float)width/(float)pBook->GetFixedWidth();
	}
	
	// 화면에 맞춰 계산된 비율과, 사용자가 임의 선택한 비율을 합쳐 실제 웹킷 랜더링 뷰에 적용한다.
	// 사용자가 임의 선택한 비율은 (+/-)10% 식 조절된다.
	float fMultiplier = m_fixedMultiplier + (float)m_nZoomStep/(float)10.0;
	//
	if(m_pWebkitViewBridge)
		m_pWebkitViewBridge->SetMultiplier(fMultiplier);

	return fMultiplier;
}

/**
 * @brief	사용자가 임의 조절한 비율을 반환한다.
 * @remarks fixed 컨텐츠 에서만 사용되며 유효한 값을 가진다.
 * @return	사용자가 임의 조절한 비율 단계
 */
const int IN3WebkitView::GetZoomStep() const
{
	return m_nZoomStep;
}

/**
 * @brief	사용자가 임의 조절하는 비율을 설정한다.
 * @remarks	사용자가 임의 조절하는 단계는 -3 ~ +5 단계까지 있으며, 0 의 값일 경우는 화면에 맞춘 비율을 뜻한다.
 * @param	step	: 사용자가 임의 조절한 비율 단계
 */
void IN3WebkitView::SetZoomStep(int step)
{
	m_nZoomStep = step;
}

/**
 * @brief	사용자가 임의 조절하는 비율을 1단계 확대 한다.
 * @remarks	사용자가 조절하는 단계는 최대 +5단계까지 지원한다. 즉, 최대 5단계시 화면에 맞춘 비율에 +50%
 */
void IN3WebkitView::SetZoomIn()
{
	// 페이지 로드 완료 후 적용 가능하다.
	if(m_nLoadState != LS_LODE_DONE) return; 

	if(m_nZoomStep < 5) m_nZoomStep++;
}

/**
 * @brief	사용자가 임의 조절하는 비율을 1단계 축소 한다.
 * @remarks	사용자가 조절하는 단계는 최대 -3단계까지 지원한다. 즉, 최대 5단계시 화면에 맞춘 비율에 -30%
 *			단, 적용된 비율이 실제 컨텐츠의 0% 이하일 경우는 적용하지 않는다.
 */
void IN3WebkitView::SetZoomOut()
{
	// 페이지 로드 완료 후 적용 가능하다.
	if(m_nLoadState != LS_LODE_DONE) return; 

	// 적용된 비율이 실제 컨텐츠의 0% 이상인 경우에만 적용한다.
	if(m_fixedMultiplier + ((float)m_nZoomStep/10.0) > 0)
	{
		if(m_nZoomStep > -3) m_nZoomStep--;
	}
}




/********************************************************************************
 * Page Interface
 ********************************************************************************/
/**
 * @brief	임의의 페이지로 이동하는 함수
 * @param	page	: 이동할 페이지 ( 현재 Load 된 쳅터의 페이지 기준 )
 */
void IN3WebkitView::MoveToPage(int page)
{
	// LOAD 완료 상태에서만 페이지 이동 가능
	if(m_nLoadState != LS_LODE_DONE) return; 

	TRACEMSG(_T("IN3WebkitView::MoveToPage() START"));
	//
	// 이동할 페이지가 첫페이지 보다 작고 현재 페이지가 첫페이지 보다 크면 이동할 페이지를 첫페이지로 이동하도록 설정
	if(page <= 0){		
		if(m_nCurrPage > 1)
			page = 1;
		else
		{
			TRACEMSG_ERR(_T("page <= 0 . so can't move page"));
			return;
		}
	}

	// 이동할 페이지가 총 페이지 보다 크고 현재 보이는 페이지가 마지막 페이지가 아니라면 마지막 페이지로 이동하도록 설정
	if(page > m_nTotalPage){	
		if(m_nCurrPage < m_nTotalPage)
			page = m_nTotalPage;
		else
		{
			TRACEMSG_ERR(_T("page >= m_nTotalPage . so can't move page"));
			return;
		}
	}

	// 페이지 이동
	if(m_pWebkitViewBridge){
		CString parm;
		parm.Format(_T("%d"), page);
		HRESULT hr = m_pWebkitViewBridge->SendMessageToEngine(CMD_IN3_PAGE_MOVE, parm);
	}

	TRACEMSG(_T("IN3WebkitView::MoveToPage() END"));
}

/**
 * @brief	다음 페이지로 이동하는 함수
 */
void IN3WebkitView::NextPage()
{
	if(m_nLoadState != LS_LODE_DONE) return; 

	CEPubBook* pBook = (CEPubBook*)this->GetParentFrame();
	if(!pBook) return;

	// fixed 컨텐츠라 reflow 컨텐츠를 구분하여 페이지를 이동한다.
	if(pBook->IsFixed())
	{
		// fixed 양면보기인 경우는 +2, 단면인 경우는 +1 만큼 쳅터를 이동한다.
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
		// reflow :: 현재 페이지가 마지막 페이지면 쳅터 이동, 아니면 페이지 이동
		if(m_nCurrPage + pBook->GetPageDisplayMode() <= m_nTotalPage)
		{
			MoveToPage(m_nCurrPage + pBook->GetPageDisplayMode());
		}
		else
			pBook->MoveToChapter(m_nCurrChapter+1);
	}
}

/**
 * @brief	이전 페이지로 이동하는 함수
 */
void IN3WebkitView::PrevPage()
{
	if(m_nLoadState != LS_LODE_DONE) return; 

	CEPubBook* pBook = (CEPubBook*)this->GetParentFrame();
	if(!pBook) return;

	// fixed 컨텐츠라 reflow 컨텐츠를 구분하여 페이지를 이동한다.
	if(pBook->IsFixed())
	{
		// fixed 양면보기인 경우는 -2, 단면인 경우는 -1 만큼 쳅터를 이동한다.
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
		// reflow :: 현재 페이지가 첫 페이지면 쳅터 이동, 아니면 페이지 이동
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
 * @brief		현재 보여지는 페이지값을 설정한다.
 * @remarks		웹킷 extension 에서 move page 한 경우, 이동한 페이지 정보는 따로 뷰어에 설정해줘야 한다.
 *				MoveToAnchor() 는 extension에서 페이지 이동을 한다.
 */
void IN3WebkitView::SetCurrPage(int page)
{
	m_nCurrPage = page;
}

/**
 * @brief	현재 Load 된 url 을 반환한다.
 * @return	Load 된 url
 */
const CString IN3WebkitView::GetURL() const
{
	return m_url;
}

/**
 * @brief	현재 보여지는 파일이 몇번째 챕터인가를 반환한다.
 * @return	챕터 인덱스 (spine 인덱스와 동일하다)
 */
const int IN3WebkitView::GetChapterIndex() const
{
	return m_nCurrChapter;
}

/**
 * @brief	현재 보여지는 파일의 총 페이지 수를 반환한다.
 * @return	현재 파일의 총 페이지 수
 */
const int IN3WebkitView::GetTotalPage(void) const
{
	return m_nTotalPage;
}

/**
 * @brief	현재 보여지고 있는 페이지 수를 반환한다.(현재 파일의 총 페이지 수를 기준으로 한다.)
 * @return	현재 보여지는 페이지 번호
 */
const int IN3WebkitView::GetCurrPage(void) const
{
	return m_nCurrPage;
}


/********************************************************************************
 * Communication interface
 ********************************************************************************/
/**
 * @brief	웹킷 extension 으로 메시지를 보낸다.
 * @param	command	: 웹킷 extension 으로 요청하는 액션 명령어
 * @param	param	: 웹킷 extension 이 액션 수행에 필요한 데이터
 * @return	정상적으로 웹킷 extension 으로 메시지를 보낸 경우 true, 그외 false
 */
BOOL IN3WebkitView::SendMessage(CString command, CString param)
{
	if(!m_pWebkitViewBridge) return FALSE;
	
	HRESULT hr = m_pWebkitViewBridge->SendMessageToEngine(command, param);
	return (hr == S_OK);
}

/**
 * @brief	엔진으로 받은 콜백 메시지 중 뷰어에서 처리해야 하는 메시지를 처리하는 함수
 * @param	command	: 커맨드
 * @param	param	: 파라미터
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
	// 파일이 로드 완료
	if( command == CMD_IN3_COMPLETE_FILE_INIT )	
	{
		// fixed 인 경우는 별다른 처리를 하지 않는다.
		if(pBook->IsFixed()) return S_OK;

		// 화면에 스크롤을 없앤다.(화면에 맞춰 레이아웃을 정리하기 때문에 반드시 FALSE 처리한다.)
		if(m_pWebkitViewBridge) m_pWebkitViewBridge->ShowHideScrollBar(FALSE);

		int page = 0;
		// 전체 페이지 수를 추출한다.
		if(pBook->ParseParam(param, _T("FILE_PAGE_COUNT"), &page))
		{
			CString traceMsg;
			traceMsg.Format(_T("ch: %d, totalpage : %d"), m_nCurrChapter, page);
			TRACEMSG_INFO(traceMsg);

			// 전체페이지 값을 업데이트 한다.
			if(m_nTotalPage != page) m_nTotalPage = page;
			if(m_nCurrPage >= m_nTotalPage) m_nCurrPage = page - 1;

			// LS_LOADING 상태이면, LS_LODE_DONE 상태로 업데이트 한다.
			// 책의 전체 페이지를 재계산한다.
			if(m_nLoadState == LS_LOADING){
				DocumentComplete();
				pBook->CalculateTotalPage();
			}
			else
				pBook->CalculateTotalPage(FALSE);

			// 현재 페이지 값을 업데이트 한다.
			if(pBook->ParseParam(param, _T("CURRENT_PAGE"), &page))
			{
				if((m_nTotalPage >= page) && (page > 0))
				{
					m_nCurrPage = page;
				}
			}
			
			// 화면을 업데이트 한다.
			Invalidate();
		}
	}

	// 페이지 이동 완료
	if( command == CMD_IN3_PAGE_MOVE )
	{
		if(param.IsEmpty()) return S_FALSE;

		int page = 0;
		// 이동된 페이지를 추출하여 휴효한 값일 경우, 현재 페이지를 업데이트 한다.
		if(pBook->ParseParam(param, _T("CURRENT_PAGE"), &page))
		{
			if((m_nTotalPage >= page) && (page > 0))
			{
				m_nCurrPage = page;
				InvalidateInfo();
				// 
				// 변경된 페이지를 UI 단에 알려준다.
				pBook->OnPageMoved();
				// 변경된 페이지에 북마크가 삽입되어 있는지 확인한다.
				pBook->QueryBookmarkStatus();
			}
		}
	}

	// 임의의 오브젝트(anchor)로 이동 완료
	if( command == CMD_IN3_MOVE_TO_OBJECT )
	{
		if(param.IsEmpty()) return S_FALSE;

		int page = 0;
		// 이동된 페이지를 추출하여 휴효한 값일 경우, 현재 페이지를 업데이트 한다.
		if(pBook->ParseParam(param, _T("CURRENT_PAGE"), &page))
		{
			pBook->OnMoveToAnchor(page);
			InvalidateInfo();
			// 
			// 변경된 페이지를 UI 단에 알려준다.
			pBook->OnPageMoved();
			// 변경된 페이지에 북마크가 삽입되어 있는지 확인한다.
			pBook->QueryBookmarkStatus();
		}
	}

	// 폰트색, 배경색 변경 완료
	if( command == CMD_IN3_CHANGE_COLORS )
	{
		// 화면 업데이트
		Invalidate();
	}

	// 보조자료. 웹 연동
	if( command == CMD_IN3_OPEN_POPUP_WEB )
	{
		if(!param.IsEmpty())
		{
			// url 주소를 그대로 JSON 파싱할 경우 에러가 발생하여 파싱 가능한 상태로 변경한다.
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
	// annotation(highlight/underline/memo) 추가 완료
	if( command == CMD_IN3_MARK_ANNOTATION )
	{
		if(!param.IsEmpty()){
			// fixed 컨텐츠인 경우는 annotation이 추가된 페이지 값을 같이 전달한다.
			// fixed 는 1page = 1spine file 이므로 m_nCurrChapter(0~max) 값을 이용하여 전달한다.
			// m_nCurrChapter 값은 0~max-1 이고 page 는 1~max 로 사용됨에 주의하자.
			if(pBook->IsFixed())
				pBook->OnAnnotationAdded(param, m_nCurrChapter+1);
			else
				pBook->OnAnnotationAdded(param);
		}
	}

	// annotation 을 사용자가 선택한 경우, (ex 화면에서 하이라이트, 언더라인을 클릭)
	if( command == CMD_IN3_EVENT_ANNOTATION )
	{
		if(!param.IsEmpty())
		{
			// 선택된 annotation 정보를 UI 단에 전달함.
			pBook->SetCurrChapter(m_nCurrChapter);
			pBook->SetActiveView(this);
			pBook->OnAnnotationSelected(param);
		}
	}

	// 메모 아이콘을 사용자가 선택한 경우
	if( command == CMD_IN3_EVENT_MEMO_ICON )
	{
		if(!param.IsEmpty())
		{
			// 선택된 annotation 정보를 UI 단에 전달함.
			pBook->SetCurrChapter(m_nCurrChapter);
			pBook->SetActiveView(this);
			pBook->OnMemoIconSelected(param);
		}
	}

	// annotation 삭제 완료
	if( command == CMD_IN3_DELETE_ANNOTATION )
	{
		if(!param.IsEmpty())
			pBook->OnAnnotationDeleted(param);
	}

	// annotation 수정 완료
	if( command == CMD_IN3_MODIFY_ANNOTATION )
	{
		if(!param.IsEmpty())
			pBook->OnAnnotationModified(param);
	}

	// annotation 일괄 적용 완료
	if( command == CMD_IN3_INIT_ANNOTATION_LIST )
	{
		if(!param.IsEmpty())
			pBook->CancelSelection(m_nCurrChapter);
	}

	// annotation 메뉴를 출력하기 위해 위치를 조회
	if( command == CMD_IN3_QUERY_ANNOTATION_POS )
	{
		if(!param.IsEmpty())
			pBook->OnQueryAnnotationPos(param);
	}

	
	/***
	 * Bookmark
	 ***/
	// 북마크 추가 완료
	if( command == CMD_IN3_SET_BOOKMARK )
	{
		if(!param.IsEmpty()){
			// fixed 컨텐츠인 경우는 annotation이 추가된 페이지 값을 같이 전달한다.
			// fixed 는 1page = 1spine file 이므로 m_nCurrChapter(0~max) 값을 이용하여 전달한다.
			// m_nCurrChapter 값은 0~max-1 이고 page 는 1~max 로 사용됨에 주의하자.
			if(pBook->IsFixed())
				pBook->OnBookmarkAdded(param, m_nCurrChapter+1);
			else
				pBook->OnBookmarkAdded(param);
		}
		//
		pBook->QueryBookmarkStatus();
	}

	// 북마크 삭제 완료
	if( command == WM_BOOKMARK_DELETED )
	{
		if(!param.IsEmpty())
			pBook->OnBookmarkDeleted(param);
		//
		pBook->QueryBookmarkStatus();
	}

	// 북마크 삭제 완료
	if( command == WM_BOOKMARK_CLEARED )
	{
		if(!param.IsEmpty())
			pBook->OnBookmarkCleared(param);
		//
		pBook->QueryBookmarkStatus();
	}

	// 북마크가 삽입되어 있는지 체크 완료
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
	// 본문 내용 검색 완료
	if(command == CMD_IN3_FIND_TEXT)
	{
		if(param.IsEmpty()) return S_FALSE;

		int page, forward;
		CString keyword;
		bool bMatches;
		// 다음검색 결과 있는지 확인(bMatches), 검색 결과가 있는 페이지(page), 검색 키워드(keyword), 검색 방향(forward) 추출
		pBook->ParseParam(param, _T("MATCHES"), &bMatches);
		pBook->ParseParam(param, _T("CURRENT_PAGE"), &page);
		pBook->ParseParam(param, _T("KEYWORD"), &keyword);
		pBook->ParseParam(param, _T("FORWARD"), &forward);
		//if(pBook->ParseFindTextParam(param, bMatches, bForward, page, keyword))
		//{
			// 다음 검색 결과가 있는지 UI 단에 알려준다.
			pBook->OnFindText(bMatches);

			// 검색 결과가 있는 페이지를 현재 페이지로 업데이트 한다.
			if((m_nTotalPage >= page) && (page > 0))
			{
				m_nCurrPage = page;
				InvalidateInfo();
				//
				pBook->QueryBookmarkStatus();
			}

			// 다음 검색 결과가 있다면, 일단 지금 검색된 결과 페이지로 이동
			// 다음 검색 결과가 없다면, 검색 방향에 따라 이전, 다음 챕터에서 재검색
			if(bMatches)
			{
				// 웹킷 extension 에서 이동해줘서 따로 이동해주지 않아도 되나, 레이아웃이 틀어지거나 화면의 깜빡임 등의 문제를 
				// 해결하기 위해 명시적으로 다시 한번 결과 페이지로 이동하도록 한다.
				if(pBook->GetCurrentChapter() != m_nCurrChapter)
				{
					// fixed 레이아웃 양면인 경우에 양쪽에 모두 검색 결과가 있다면 왼쪽 챕터에서 오른쪽 챕터로 이동 시
					// 양쪽의 검색 결과에 모두 표시가 되는 문제가 발생한다.
					// 따라서, 해당 경우에는 이전 챕터에 있는 검색 결과 선택 표시를 해제해준다.
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

	// 본문 내용 검색(all)
	if(command == CMD_IN3_FIND_TEXT_ALL)
	{
		// 검색 결과가 있다면 UI 단으로 전달한다.
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
 * @brief	IN3WebkitView 의 OnSize() 함수
 * @remarks	화면 사이즈에 맞춰 웹킷 랜더링 영역을 수정하여 화면에 표시한다.
 */
void IN3WebkitView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEPubBook* pBook = (CEPubBook*)GetParentFrame();
	if(pBook)
	{
		// fixed 컨텐츠와 reflow  컨텐츠는 화면에 표시하는 방법이 다르다.
		// fixed 의 경우는 컨텐츠의 크기가 정해져 있어, 실제 뷰어 크기에 맞춰 컨텐츠의 크기를 수정하여 보여줘야 한다.
		if(pBook->IsFixed())
		{
			// fixed 컨텐츠를 양면 보기하는 경우 화면을 반으로 갈라 왼쪽와 오른쪽에 각각 다른 뷰를 보여주게 된다.
			// 이때 마지막 뷰가 왼쪽에 오게 되면 마지막 뷰의 크기는 뷰어 크기의 반이 아닌 단면보기와 동일하게 전체를 사용해야 한다.
			// 다른 뷰와 마찬가지로 반만 사용하게 되면, 오른쪽 영역이 빈 공간이 아닌 원하지 않는 다른 뷰가 보일 수 있다.
			// 따라서, 양면보기의 경우 마지막 뷰가 왼쪽에 오게 되는 경우와 그 외의 경우를 분리하여 처리한다.
			
			// 양면보기 면서, 마지막 뷰이고, 화면의 왼쪽에 보여지는 경우
			if((pBook->GetPageDisplayMode() == 2) && (m_nCurrChapter == pBook->GetSpineCount()-1) && (m_nCurrChapter %2 == 0))
			{
				// 뷰의 크기가 뷰어의 크기와 동일하므로, 실제 랜더링 할 영역은 뷰의 가로크기의 반으로 계산하여 화면 확대/축소 비율을 구한다.
				// 즉, 실제 뷰의 크기는 뷰어크기와 동일하나 다른 챕터들의 뷰가 뷰어의 크기의 반을 사용하므로, 동일한 비율로 보이게 하기 위해 뷰 가로크기의 반을 사용한다.
				float multiplier = GetFixedMultiplier((cx/2), cy);
				if(multiplier > 0.0)
				{
					// 계산된 비율로 실제 화면에 보여야 하는 랜더링 영역의 크기를 구한다.
					bool bScroll = false;
					int height = int(pBook->GetFixedHeight() * multiplier);
					int width = int(pBook->GetFixedWidth() * multiplier);

					// 랜더링 영역의 x,y 좌표를 구한다.
					// 이때 컨텐츠가 보이는 영역은 뷰의 가운데에 위치하도록 계산한다.
					int startX = (cx/2) - width;
					int startY = RECT_CHAPTER_HEGIHT;
					int posY = int( ((cy - (RECT_CHAPTER_HEGIHT + RECT_PAGEINFO_HEIGHT)) - (pBook->GetFixedHeight() * multiplier))/2 );
					startY += posY;

					// 컨텐츠가 보여지는 영역 보다, 사용자가 임의 확대/축소 하여 재조정 된 랜더링 영역이 큰 경우
					// 스크롤을 보여줘야 한다.
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
				// 뷰의 크기에 맞춰 컨텐츠가 보여야 하는 비율을 구한다.
				bool bScroll = false;
				float multiplier = GetFixedMultiplier(cx, cy);
				if(multiplier > 0.0)
				{
					// 계산된 비율로 실제 화면에 보여야 하는 랜더링 영역의 크기를 구한다.
					int height = int( pBook->GetFixedHeight() * multiplier );
					int width = int( pBook->GetFixedWidth() * multiplier );
					//
					// 랜더링 영역의 x,y 좌표를 구한다.
					// 이때 컨텐츠가 보이는 영역은 뷰의 가운데에 위치하도록 계산한다.
					int startX = (cx - width)/2;
					int startY = RECT_CHAPTER_HEGIHT;
					int posY = int( ((cy - (RECT_CHAPTER_HEGIHT + RECT_PAGEINFO_HEIGHT)) - (pBook->GetFixedHeight() * multiplier))/2 );
					startY += posY;

					// 컨텐츠가 보여지는 영역 보다, 사용자가 임의 확대/축소 하여 재조정 된 랜더링 영역이 큰 경우
					// 스크롤을 보여줘야 한다.
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

					// 왼쪽 뷰와 오른쪽 뷰를 중앙 정렬 즉 뷰어의 정 가운데를 기준으로 양쪽으로 보이게 하기 위해서, 
					// 뷰의 위치에 따라 x 좌표를 다르게 계산한다.
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
 * @brief	칼라값을 가지는 스트링에서 실제 칼라값을 추출하는 함수
 * @remarks	'rgb(x, x, x)' 의 스트링에서 각 칼라의 값을 integer 형으로 추출한다.
 * @param	[in] strColor	: 추출할 칼라값을 가지는 스트링
 * @param	[out] red		: 빨간색(0~255)
 * @param	[out] green		: 초록색(0~255)
 * @param	[out] blue		: 파란색(0~255)
 * @return	true(success)/false(fail)
 */
bool IN3WebkitView::GetIntColorValue(CString strColor, int &red, int &green, int &blue)
{
	if(strColor.IsEmpty()) return false;
	//
	// '#0xffffff' 와 'rbg(x,x,x)' 의 경우를 분리하여 추출한다.
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