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
// facetDlg.h : header file
//

#if !defined(AFX_FACETDLG_H__19CBF0FC_860E_4BE5_B341_0A494C8263B5__INCLUDED_)
#define AFX_FACETDLG_H__19CBF0FC_860E_4BE5_B341_0A494C8263B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "FacetProcess.h"
/////////////////////////////////////////////////////////////////////////////
// CFacetDlg dialog

#define WM_USER_RECALC_IN_PROGRESS (WM_USER + 1)

class CFacetDlg : public CDialog
{
// Construction
public:
	//UINT MyThreadProc( LPVOID pParam );
	//UINT LSM_FUZA_NEW_THREAD(LPVOID pParam);
	CFacetDlg(CWnd* pParent = NULL);	// standard constructor
	CString strDirectory;
	CString str3CharName;
// Dialog Data
	//{{AFX_DATA(CFacetDlg)
	enum { IDD = IDD_FACET_DIALOG };
	CButton	m_BUTON_ZONE;
	CButton	m_BUTTON_OK;
	CButton	m_BUTTON_FUZZY_FACETS;
	CButton	m_BUTTON_FUZZY_ATTRIBUTE_FILE;
	CProgressCtrl	m_PROGRESS_BAR2;
	CProgressCtrl	m_PROGRESS_BAR1;
	CButton	m_BUTTON_DEMFILE;
	CString	m_EDIT_DEM;
	CString	m_EDIT_FUZZY_ATTRIBUTE_FILE;
	CString	m_EDIT_FUZZY_FACETS;
	int		m_RADIO_1;
	CString	m_EDIT_RELZFILE;
	CString	m_EDIT_FORMFILE;
	CString	m_STATIC_CURRENTPROCESS;
	CString	m_EDIT_ZONEFILE;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFacetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CFacetDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnButtonDemfile();
	afx_msg void OnButtonFuzzyAttributesFile();
	afx_msg void OnButtonFuzzyFacets();
	afx_msg void OnButtonRelzfile();
	afx_msg void OnButtonFormfile();
	afx_msg void OnButtonZone();
	afx_msg LRESULT OnRecalcInProgress1(WPARAM wParam, LPARAM lParam);
	virtual void OnCancel();
	afx_msg void OnButtonClose();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
	CString sFullPathParam;
	CPtrArray* pPointerArray;
	bool bKillThread;
	CString sGeoFileName;
	void CUpdateData(BOOL bValue);
		~CFacetDlg();
	//void LSM_FUZA(CPtrArray* pArray);
	//bool ReadARuleFile(CPtrArray* pArray);
	//CFacetProcess* pFacetProcess;

	CFacetProcess* pFacetProcess;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FACETDLG_H__19CBF0FC_860E_4BE5_B341_0A494C8263B5__INCLUDED_)
