// ImageProgress.cpp : 实现文件
//

#include "stdafx.h"
#include "AutoUpdateClient.h"
#include "ImageProgress.h"


// CImageProgress

IMPLEMENT_DYNAMIC(CImageProgress, CProgressCtrl)

CImageProgress::CImageProgress()
{
	m_brTrans = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
}

CImageProgress::~CImageProgress()
{
}


BEGIN_MESSAGE_MAP(CImageProgress, CProgressCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_CREATE()
	ON_WM_NCCALCSIZE()
END_MESSAGE_MAP()

// CImageProgress 消息处理程序

//BOOL CImageProgress::OnEraseBkgnd(CDC* pDC)
//{
//	CRect rect;
//	GetClientRect(&rect);
//	pDC->FillRect(&rect , m_brTrans);
//	
//	return TRUE;
//}

void CImageProgress::OnPaint()
{
	PAINTSTRUCT ps;//声明一个绘画区域   
	CDC *pDC=BeginPaint(&ps);//把绘画区域选入设备上下文并开始   
	int AdrPos=GetPos();//获取进度条的进度位置   
	 
	CRect WindowRC;	
	GetWindowRect(&WindowRC);

	CRect ClientRC;   
	//ClientRC.left = 0;
	//ClientRC.top = 0;
	//ClientRC.right = WindowRC.right - WindowRC.left;
	//ClientRC.bottom = WindowRC.bottom - WindowRC.top;
	GetClientRect(&ClientRC);//获取客户区矩形   
	pDC->SetBkMode (TRANSPARENT);//设置背景模式为透明   
	int nMin,nMax;//声明进度条的最大最小限制   
	GetRange(nMin,nMax);//获取限制   
	double Every=(double)ClientRC.Width ()/(nMax-nMin);//获取单位刻度   
	int Now=Every*AdrPos;//当前长度   
	CRect LeftRC,RightRC;//声明进度条区域和空白区域   
	LeftRC=RightRC=ClientRC;
	LeftRC.right =Now;//进度条区域  
	RightRC.left =Now;//空白区域   

	CBitmap bmpProgress  , bmpBlank;
	bmpProgress.LoadBitmap(IDB_BITMAP2);
	bmpBlank.LoadBitmap(IDB_BITMAP4);
	CBrush brProgress(&bmpProgress);
	CBrush brBlank(&bmpBlank);

	pDC->SelectObject(m_brTrans);

	pDC->FillRect (LeftRC,&brProgress);	//画进度条区域  
	pDC->FillRect (RightRC,&brBlank);	//画空白区域   
	ReleaseDC(pDC);//销毁设备上下文   
	EndPaint(&ps);//结束绘画区域绘画
}
void CImageProgress::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	lpncsp->rgrc[0].left -= 1;
	lpncsp->rgrc[0].top -= 1;
	lpncsp->rgrc[0].right += 1;
	lpncsp->rgrc[0].bottom += 1;;

	CProgressCtrl::OnNcCalcSize(bCalcValidRects, lpncsp);
}
