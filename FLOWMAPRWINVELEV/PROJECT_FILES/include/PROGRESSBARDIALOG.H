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
#if !defined(AFX_PROGRESSBARDIALOG_H__85FA41F3_1247_11D5_B00A_0080C8EC880F__INCLUDED_)
#define AFX_PROGRESSBARDIALOG_H__85FA41F3_1247_11D5_B00A_0080C8EC880F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProgressBarDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProgressBarDialog dialog

class CProgressBarDialog : public CDialog
{
// Construction
public:
	CProgressBarDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProgressBarDialog)
	enum { IDD = IDD_PROGRESSBARDIALOG_DIALOG };
	CProgressCtrl	m_ProgressBar2;
	CProgressCtrl	m_ProgressBar1;
	CString	m_UserMessage;
	CString	m_CurrentProcess;
	CString	m_OverallProgress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgressBarDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProgressBarDialog)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGRESSBARDIALOG_H__85FA41F3_1247_11D5_B00A_0080C8EC880F__INCLUDED_)
