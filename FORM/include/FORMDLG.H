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
// formDlg.h : header file
//

#if !defined(AFX_FORMDLG_H__AF637B96_C8A9_4C18_8812_8A4FB6E34102__INCLUDED_)
#define AFX_FORMDLG_H__AF637B96_C8A9_4C18_8812_8A4FB6E34102__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "FormProcess.h"
/////////////////////////////////////////////////////////////////////////////
// CFormDlg dialog

#define WM_USER_RECALC_IN_PROGRESS (WM_USER + 1)

class CFormDlg : public CDialog
{
// Construction
public:
	CString sFullPathParam;
	bool bKillThread;
	CPtrArray* pPointerArray;
	CFormProcess* pFormProcess;
	CString str3CharName;
	CString strDirectory;
	CFormDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFormDlg)
	enum { IDD = IDD_FORM_DIALOG };
	CProgressCtrl	m_OVERALL_PROGRESS;
	CProgressCtrl	m_CURRENT_PROGRESS;
	CString	m_EDIT_DEM;
	double		m_EDIT_DSAREA;
	int		m_EDIT_SIZE;
	double		m_EDIT_UPSLOPEAREA;
	int		m_RADIO1;
	CString	m_CURRENT_STATUS;
	CString	m_OVERALL_STATUS;
	double	m_MissingValue;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CFormDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonDem();
	virtual void OnOK();
	afx_msg void OnUpdateStatus(WPARAM wParam, LPARAM lParam);
	virtual void OnCancel();
	afx_msg LRESULT OnRecalcInProgress1(WPARAM wParam, LPARAM lParam);
	afx_msg void OnButtonClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMDLG_H__AF637B96_C8A9_4C18_8812_8A4FB6E34102__INCLUDED_)
