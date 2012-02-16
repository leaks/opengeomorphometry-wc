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
// ProgressBarDialog.cpp : implementation file
//

#include "stdafx.h"
#include "landmap.h"
#include "ProgressBarDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgressBarDialog dialog


CProgressBarDialog::CProgressBarDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CProgressBarDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProgressBarDialog)
	m_UserMessage = _T("");
	m_CurrentProcess = _T("");
	m_OverallProgress = _T("");
	//}}AFX_DATA_INIT

	if(Create(IDD_PROGRESSBARDIALOG_DIALOG,pParent))
	{
		ShowWindow(SW_SHOW);
	}
}


void CProgressBarDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProgressBarDialog)
	DDX_Control(pDX, IDC_PROGRESS_BAR2, m_ProgressBar2);
	DDX_Control(pDX, IDC_PROGRESS_BAR1, m_ProgressBar1);
	DDX_Text(pDX, IDC_USER_MESSAGE, m_UserMessage);
	DDX_Text(pDX, IDC_CURRENT_PROCESS, m_CurrentProcess);
	DDX_Text(pDX, IDC_OVERALL_PROGRESS, m_OverallProgress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProgressBarDialog, CDialog)
	//{{AFX_MSG_MAP(CProgressBarDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProgressBarDialog message handlers

BOOL CProgressBarDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_ProgressBar1.SetRange(0,9);
	m_ProgressBar1.SetPos(0);
	m_ProgressBar1.SetStep(1);
	m_ProgressBar2.SetRange(0,12);	
	m_ProgressBar2.SetPos(0);	
	m_ProgressBar2.SetStep(1);
	m_CurrentProcess = "Current Process";
	m_OverallProgress = "Overall Progress";
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CProgressBarDialog::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
