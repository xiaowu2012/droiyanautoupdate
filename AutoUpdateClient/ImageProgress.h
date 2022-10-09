#pragma once


// CImageProgress

class CImageProgress : public CProgressCtrl
{
	DECLARE_DYNAMIC(CImageProgress)

public:
	CImageProgress();
	virtual ~CImageProgress();

protected:
	DECLARE_MESSAGE_MAP()
public:
	//afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();

protected:
	CBrush	 *m_brTrans;
public:
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
};


