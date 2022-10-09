// AniStatic.cpp : implementation file
//

#include "stdafx.h"
#include "AutoUpdateClient.h"
#include "AniStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAniStatic

CAniStatic::CAniStatic()
{
	m_strWindowText = _T("");
	m_strPaintText = _T("");

	m_bDrawBoard = false;
	m_TextColor = RGB(0x80 , 0xFF , 0x80);

	LOGFONT logFont;
	m_Font.CreatePointFont(12,_T("宋体"), NULL);
	m_Font.GetLogFont(&logFont);
	logFont.lfCharSet = GB2312_CHARSET;
	logFont.lfHeight = 12;
	//logFont.lfWidth = 6;
	logFont.lfWeight = FW_BOLD;
	m_Font.CreateFontIndirect(&logFont);

	SetFont(&m_Font);
}

CAniStatic::~CAniStatic()
{
}


BEGIN_MESSAGE_MAP(CAniStatic, CStatic)
	//{{AFX_MSG_MAP(CAniStatic)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAniStatic message handlers

void CAniStatic::SetWindowText(LPCTSTR lpszString)
{
	m_strWindowText = (CString)lpszString;  
	CRect rct;  
	GetWindowRect(&rct);  
	GetParent()->ScreenToClient(&rct);  
	GetParent()->InvalidateRect(&rct); //使区域无效 重绘防止出现文本重叠 

}

void CAniStatic::DrawBoard(CDC* dc,CRect rct)
{
	CPen pen;  
	pen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));  
	CPen  *oldPen;  
	oldPen = dc->SelectObject(&pen);  
	dc->MoveTo(rct.left, rct.top);  
	dc->LineTo(rct.right, rct.top);  
	dc->LineTo(rct.right, rct.bottom);  
	dc->LineTo(rct.left, rct.bottom);  
	dc->LineTo(rct.left, rct.top);  
	dc->SelectObject(oldPen);  
}

void CAniStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rct;  
	GetWindowRect(&rct);  
	ScreenToClient(&rct);  

	CPaintDC *pDC = &dc;  
	pDC->SetBkMode(TRANSPARENT);  
	pDC->SetBkColor(TRANSPARENT);  

	/*内存绘图*/  
	CDC memDC;  
	memDC.CreateCompatibleDC(&dc);  
	CBitmap m_MemMap;  
	m_MemMap.CreateCompatibleBitmap(pDC, rct.Width(), rct.Height()); 
	memDC.SelectObject(&m_MemMap); 
	memDC.SelectObject(&m_Font);
	memDC.FillSolidRect(rct, RGB(255, 0, 255));  
	memDC.SetBkMode(TRANSPARENT);  
	memDC.SetBkColor(TRANSPARENT);  

	if (m_strPaintText != m_strWindowText)  
	{  
		//m_Font.CreatePointFont(12, _T("宋体"));  
		memDC.SetTextColor(m_TextColor);  
		memDC.DrawText(m_strWindowText, rct, DT_VCENTER | DT_VCENTER | DT_SINGLELINE);  
		if (m_bDrawBoard)  DrawBoard(&memDC,rct);  
		m_strPaintText = m_strWindowText;  
	}  

	else  
	{  
		memDC.SetTextColor(m_TextColor);  
		memDC.DrawText(m_strWindowText, rct, DT_VCENTER | DT_VCENTER | DT_SINGLELINE);  
		if (m_bDrawBoard)  DrawBoard(&memDC,rct);  
	}  

	/*复制到界面dc*/  
	pDC->TransparentBlt(rct.left, rct.top, rct.Width(), rct.Height(), &memDC, 0, 0, rct.Width(), rct.Height(), RGB(255, 0, 255)); // SRCCOPY);  

	/*清理内存*/  
	m_MemMap.DeleteObject();  
	memDC.DeleteDC();  

	// 不为绘图消息调用 CStatic::OnPaint()
}

HBRUSH CAniStatic::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CStatic::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(255, 0, 0));

	return (HBRUSH)GetStockObject(NULL_BRUSH) ;


	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

LRESULT CAniStatic::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类

	/*if(WM_SETTEXT == message)
	{
		CRect rect;
		GetWindowRect(&rect);
		CWnd *pParentWnd = GetParent();
		if(pParentWnd)
		{
			pParentWnd->ScreenToClient(&rect);
			pParentWnd->InvalidateRect(&rect);
		}

	}*/

	return CStatic::DefWindowProc(message, wParam, lParam);
}

BOOL CAniStatic::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetBkColor(TRANSPARENT);
	return TRUE;

	return CStatic::OnEraseBkgnd(pDC);
}
