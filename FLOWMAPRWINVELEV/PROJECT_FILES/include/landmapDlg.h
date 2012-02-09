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
// landmapDlg.h : header file
//

#if !defined(AFX_LANDMAPDLG_H__FF0EC226_6AA2_11D4_A966_0000B434B8D3__INCLUDED_)
#define AFX_LANDMAPDLG_H__FF0EC226_6AA2_11D4_A966_0000B434B8D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include<fstream>
#include "ProgressBarDialog.h"
#define WM_USER_RECALC_IN_PROGRESS (WM_USER + 1)


#include "VoldFile.h"
/////////////////////////////////////////////////////////////////////////////
// CLandmapDlg dialog


//*********************************
//IK replaced by newer StatVar
/*typedef struct StatVar
{
	double	fInElev;
	double	fMinElev;
	double	fOutElev;
	double	fPitVol;
	double	fPitElev;
	double	fPourElev;
	double	fStartElev;
	double	fVaratio;
	int		iDrainsTo;
	int		iInRec;
	int		iInRow;
	int		iInColumn;
	int		iOutRec;
	int		iOutRow;
	int		iOutColumn;
	int		iOutShed;
	int		iPitRow;
	int		iPitCol;
	int		iPitArea;
	int		iPitRec;
	int		iPondCell;
	int		iShedArea;
	int		iThisShed;
} LSMSTATVAR;
*/

typedef struct VoldFile
{
	int		iSeqNo;
	int		iDdir;
	int     iDrec;
	int     iUpslope;
	int     iShedNow;
	int     iStage;
	int     iUrec;
	int     iDs_Area;
	double   iVaration;
}LSMVOLDFILE;

typedef struct MoldFile
{
	int		iSeqNo;
	int		iDdir;
	int     iDrec;
	int     iUpslope;
	int     iShedNow;
	int     iStage;
	int     iUrec;
	int     iDs_Area;
	double   iVaration;
}LSMMOLDFILE;


typedef struct StatVar
{

	int		iThisShed;
	bool	bEdge;
	bool    bFinal;
	int     iEndPit;
	int		iShedArea;
	int		iPitRow;
	int		iPitCol;
	int		iPitRec;
	double   fPitElev;
	double	fPourElev;
	double   fPreVol;
	double	fPitVol;
	double	fVaratio;
	int		iPitArea;
	int		iDrainsTo;
	int		iOutShed;
	int     iNextPit;
	int     iBecomes;
	bool    bRemoved;
	int		iInRow;
	int		iInColumn;
	int		iInRec;
	double	fInElev;
	int		iOutRow;
	int		iOutColumn;
	int		iOutRec;
	double	fOutElev;
	int     iStage;
	bool    bVisited;
	int		iPondCell;
	double   fStartElev;
	double   fMinElev;
	bool bDR_2_MV;
	
} LSMSTATVAR;

typedef struct PondStats
{
	int		iThisShed;
	bool	bEdge;
	bool    bFinal;
	int     iEndPit;
	int		iShedArea;
	int		iPitRow;
	int		iPitCol;
	int		iPitRec;
	double   fPitElev;
	double	fPourElev;
	double   fPreVol;
	double	fPitVol;
	double	fVaratio;
	int		iPitArea;
	int     iDrainsTo;
	int		iOutShed;
	int     iNextPit;
	int     iBecomes;
	bool    bRemoved;
	int		iInRow;
	int		iInColumn;
	int		iInRec;
	double	fInElev;
	int		iOutRow;
	int		iOutColumn;
	int		iOutRec;
	double	fOutElev;
	int     iStage;
	bool    bVisited;
	
	
} LSMPONDSTATS;


typedef struct StatVar2
{
	int		iThisShed;
	double	fMinElev;
	double	fPourElev;
	double	fStartElev;
	double	fOutElev;
	double	fPitVol;
	int		iPitRow;
	int		iPitCol;
	int		iPitArea;
	int		iPitRec;
	double	fPitElev;
	int		iInRec;
	double	fInElev;
	int		iCenRow;
	int		iCenColumn;
	int		iOutRec;
	int		iNeighRow;
	int		iNeighColumn;
	int		iOutShed;
	int		iShedArea;
	int		iPondCell;
	int		iInRow;
	int		iInColumn;
	int		iOutRow;
	int		iOutColumn;
	double	fVaratio;
} LSMSTATVAR2;




typedef struct ProcParm
{
	double	fMaxVol;
	int		iMaxArea;
	double	fMaxDepth;
	double	fMaxRunOff;
} LSMPROCPARM;


class CLandmapDlg : public CDialog
{
// Construction
public:
	bool bKillThread;
	CString sFullCurrentPath;
	CLandmapDlg(CWnd* pParent = NULL);	// standard constructor
	double* ReadDbf(CString sTemp);
// Dialog Data
	//{{AFX_DATA(CLandmapDlg)
	enum { IDD = IDD_LANDMAP_DIALOG };
	CProgressCtrl	m_PROGRESS2;
	CProgressCtrl	m_PROGRESS1;
	CString	m_FileName;
	double	m_LLX;
	double	m_LLY;
	double	m_MissDataValue;
	double	m_PitDepth;
	long	m_PitArea;
	double	m_PitVolumn;
	int		m_Row;
	double	m_GridSize;
	int		m_Column;
	CString	m_WorkingDirectory;
	CString	m_STATIC_CURRENTPROCESS;
	BOOL	m_InvertedElev;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLandmapDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CLandmapDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnRecalcInProgress1(WPARAM wParam, LPARAM lParam);
	virtual void OnOK();
	afx_msg void OnButtonDemfile();
	virtual void OnCancel();
	afx_msg void OnBUTTONCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LANDMAPDLG_H__FF0EC226_6AA2_11D4_A966_0000B434B8D3__INCLUDED_)
