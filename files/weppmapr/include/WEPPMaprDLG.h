//////////////////////////////////////////////////////////////////////////////////////
//This file is part of open-geomorphometry.
//
//    open-geomorphometry is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    open-geomorphometry is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with open-geomorphometry.  If not, see <http://www.gnu.org/licenses/>.
//
//////////////////////////////////////////////////////////////////////////////////////
//
// WeppMaprDlg.h : header file
//

#if !defined(AFX_WEPPMAPRDLG_H__A294F476_6668_11D5_9AEC_0080C8EC880F__INCLUDED_)
#define AFX_WEPPMAPRDLG_H__A294F476_6668_11D5_9AEC_0080C8EC880F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define WM_USER_RECALC_IN_PROGRESS (WM_USER + 1)
/////////////////////////////////////////////////////////////////////////////
// CWeppMaprDlg dialog




class CWeppMaprDlg : public CDialog
{
// Construction
public:
	bool bStopProcess;
	CString sFullPathParam;
	CPtrArray* pPointerArray;
	~CWeppMaprDlg();
	//CWeppProcess* pWeppProcess;
	
	long nProcessPointer;

	CWeppMaprDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CWeppMaprDlg)
	enum { IDD = IDD_WEPPMAPR_DIALOG };
	CProgressCtrl	m_ProgressCtrl;
	CString	m_FileName;
	int		m_Size;
	int		m_Upslope;
	double	m_MissingData;
	CString	m_Message;
	CString	m_Directory;
	int		m_MaxSegLength;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWeppMaprDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

	

	HICON m_hIcon;
	// Generated message map functions
	//{{AFX_MSG(CWeppMaprDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg LRESULT OnRecalcInProgress1(WPARAM wParam, LPARAM lParam);
	afx_msg void OnButtonDemfile();
	afx_msg void OnButtonCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WEPPMAPRDLG_H__A294F476_6668_11D5_9AEC_0080C8EC880F__INCLUDED_)
