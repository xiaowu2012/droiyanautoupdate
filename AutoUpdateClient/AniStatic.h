#if !defined(AFX_ANISTATIC_H__BBFFE504_4E01_48F6_B672_C6097AA63987__INCLUDED_)
#define AFX_ANISTATIC_H__BBFFE504_4E01_48F6_B672_C6097AA63987__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AniStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAniStatic window

class CAniStatic : public CStatic
{
// Construction
public:
	CAniStatic();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAniStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAniStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAniStatic)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
protected:
	bool		m_bDrawBoard;
	CFont		m_Font;
	COLORREF	m_TextColor;
	CString		m_strWindowText;
	CString		m_strPaintText;
	
	void		DrawBoard(CDC* dc,CRect rct);  

	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
	void SetWindowText(LPCTSTR lpszString); 
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANISTATIC_H__BBFFE504_4E01_48F6_B672_C6097AA63987__INCLUDED_)
