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
// GridReadWriteUtilityDlg.h : header file
//

#if !defined(AFX_GRIDREADWRITEUTILITYDLG_H__6B7CA205_1809_4463_9B61_63E5C28B357F__INCLUDED_)
#define AFX_GRIDREADWRITEUTILITYDLG_H__6B7CA205_1809_4463_9B61_63E5C28B357F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CGridReadWriteUtilityDlg dialog

class CGridReadWriteUtilityDlg : public CDialog
{
// Construction
public:
	void InableGridFields();
	void PopulateFields();
	CString sOutputFileName;
	CString sByteOrder;
	double NoDataValue;
	int nRows;
	int nCols;
	CString sInputFileName;
	CGridReadWriteUtilityDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CGridReadWriteUtilityDlg)
	enum { IDD = IDD_GRIDREADWRITEUTILITY_DIALOG };
	CComboBox	m_FIELDTYPE;
	CComboBox	m_ComboBox;
	CProgressCtrl	m_PROGRESS_BAR;
	int		m_RADIO_BUTTON;
	CString	m_EDIT_INPUT;
	CString	m_EDIT_OUTPUT;
	int		m_NumCols;
	double	m_MissingData;
	int		m_NumRows;
	double	m_X;
	double	m_Y;
	short	m_CellSize;
	int		m_FIELDWIDTH;
	int		m_FIELDPRECISION;
	CString	m_FIELDNAME;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridReadWriteUtilityDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CGridReadWriteUtilityDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnButtonInput();
	afx_msg void OnButtonOutput();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnChangeEditRows();
	afx_msg void OnChangeEditColumns();
	afx_msg void OnChangeEditX();
	afx_msg void OnChangeEditY();
	afx_msg void OnChangeEditCellsize();
	afx_msg void OnChangeEditMissingdata();
	afx_msg void OnButtonLoadheader();
	afx_msg void OnChangeEditFieldWidth();
	afx_msg void OnEditchangeComboFieldtype();
	afx_msg void OnChangeEditPrecision();
	afx_msg void OnChangeEditFieldname();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDREADWRITEUTILITYDLG_H__6B7CA205_1809_4463_9B61_63E5C28B357F__INCLUDED_)
