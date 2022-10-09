// ButtonDW.cpp : 实现文件
//

#include "stdafx.h"
#include "AutoUpdateClient.h"
#include "MyButton.h"
#include "Define.h"


// CMyButton

IMPLEMENT_DYNAMIC(CMyButton, CButton)

CMyButton::CMyButton()
{
	m_bTraceMouseEvent = FALSE;
	m_bMouseDown = FALSE;
}

CMyButton::~CMyButton()
{
}


BEGIN_MESSAGE_MAP(CMyButton, CButton)
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_MOUSEHOVER , OnMouseHover)
	ON_MESSAGE(WM_MOUSELEAVE , OnMouseLeave)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEACTIVATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

LRESULT CMyButton::OnMouseHover(WPARAM wParam , LPARAM lParam)
{
	if(m_bMouseDown == TRUE) return 1;

	m_bTraceMouseEvent = FALSE;
	
	CBitmap btnBitmap;
	btnBitmap.LoadBitmap(IDB_BITMAP1);

	CDC dcCom;
	dcCom.CreateCompatibleDC(GetDC());

	dcCom.SelectObject(&btnBitmap);

	CRect rectClient;
	GetClientRect(&rectClient);

	GetDC()->BitBlt(0 , 0 , 132 , 30 , &dcCom , 0 , 30 , SRCCOPY);

	return 1;
}

LRESULT CMyButton::OnMouseLeave(WPARAM wParam , LPARAM lParam)
{
	CBitmap btnBitmap;
	btnBitmap.LoadBitmap(IDB_BITMAP1);

	CDC dcCom;
	dcCom.CreateCompatibleDC(GetDC());

	dcCom.SelectObject(&btnBitmap);

	CRect rectClient;
	GetClientRect(&rectClient);

	GetDC()->BitBlt(0 , 0 , 132 , 30 , &dcCom , 0 , 0 , SRCCOPY);
	m_bTraceMouseEvent = FALSE;
	return 1;
}


////////////////必需重写
void CMyButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	/*UINT uStyle = DFCS_BUTTONPUSH;

	ASSERT(lpDrawItemStruct->CtlType == ODT_BUTTON);

	if(lpDrawItemStruct->itemState & ODS_SELECTED)
		uStyle |= DFCS_PUSHED;

	::DrawFrameControl(lpDrawItemStruct->hDC , &lpDrawItemStruct->rcItem , 
		DFC_BUTTON , uStyle);

	CString szText;
	GetWindowText(szText);

	COLORREF crOld = ::SetTextColor(lpDrawItemStruct->hDC , RGB(255 , 0 , 0 ));

	::DrawText(lpDrawItemStruct->hDC , szText  , szText.GetLength(),
		&lpDrawItemStruct->rcItem , DT_SINGLELINE|DT_VCENTER|DT_CENTER );

	::SetTextColor(lpDrawItemStruct->hDC , crOld);*/
}


BOOL CMyButton::OnEraseBkgnd(CDC* pDC)
{
	CBitmap btnBitmap;
	btnBitmap.LoadBitmap(IDB_BITMAP1);

	CDC dcCom;
	dcCom.CreateCompatibleDC(pDC);

	dcCom.SelectObject(&btnBitmap);

	CRect rectClient;
	GetClientRect(&rectClient);

	pDC->BitBlt(0 , 0 , 132 , 30 , &dcCom , 0 , 0 , SRCCOPY);

	return TRUE;
}

LRESULT CMyButton::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	return CButton::DefWindowProc(message, wParam, lParam);
}

void CMyButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if(m_bTraceMouseEvent == FALSE)
	{
		TRACKMOUSEEVENT hEventTrack;
		hEventTrack.cbSize = sizeof(TRACKMOUSEEVENT);
		hEventTrack.hwndTrack = m_hWnd;
		hEventTrack.dwFlags =  TME_LEAVE|TME_HOVER;
		hEventTrack.dwHoverTime = 10;
		_TrackMouseEvent(&hEventTrack);
		m_bTraceMouseEvent = TRUE;
	}

	CButton::OnMouseMove(nFlags, point);
}

int CMyButton::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	/*if(!m_bMouseDown)
	{
		m_bTraceMouseEvent = FALSE;
	
		CBitmap btnBitmap;
		btnBitmap.LoadBitmap(IDB_BITMAP1);

		CDC dcCom;
		dcCom.CreateCompatibleDC(GetDC());

		dcCom.SelectObject(&btnBitmap);

		CRect rectClient;
		GetClientRect(&rectClient);

		GetDC()->BitBlt(0 , 0 , 132 , 30 , &dcCom , 0 , 60 , SRCCOPY);
		
		m_bMouseDown = TRUE;
	}*/

	return 1;
}


void CMyButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bTraceMouseEvent = FALSE;

	CBitmap btnBitmap;
	btnBitmap.LoadBitmap(IDB_BITMAP1);

	CDC dcCom;
	dcCom.CreateCompatibleDC(GetDC());

	dcCom.SelectObject(&btnBitmap);

	CRect rectClient;
	GetClientRect(&rectClient);

	GetDC()->BitBlt(0 , 0 , 132 , 30 , &dcCom , 0 , 60 , SRCCOPY);

	m_bMouseDown = TRUE;

	CButton::OnLButtonDown(nFlags, point);
}


void CMyButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bTraceMouseEvent = FALSE;

	CBitmap btnBitmap;
	btnBitmap.LoadBitmap(IDB_BITMAP1);

	CDC dcCom;
	dcCom.CreateCompatibleDC(GetDC());

	dcCom.SelectObject(&btnBitmap);

	CRect rectClient;
	GetClientRect(&rectClient);

	GetDC()->BitBlt(0 , 0 , 132 , 30 , &dcCom , 0 , 0 , SRCCOPY);

	m_bMouseDown = TRUE;

	CButton::OnLButtonUp(nFlags, point);
}
