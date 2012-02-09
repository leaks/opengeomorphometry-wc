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
// FormProcess.h: interface for the FormProcess class.
//
//////////////////////////////////////////////////////////////////////

#include "FormDlg.h"
#if !defined(AFX_FORMPROCESS_H__245B0193_BA98_4F39_89C2_1C6C6EF7CC9B__INCLUDED_)
#define AFX_FORMPROCESS_H__245B0193_BA98_4F39_89C2_1C6C6EF7CC9B__INCLUDED_

#include "DbfFile.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFormProcess  
{
public:
	bool* bKillThread;
	CWnd* pWndNotifyProgress;
	void calc_len3();
	void calcrelief3(int* pSortedIndex);
	void go_peak3(int* pSortedIndex);
	void go_ridge3(int* pSortedIndex);
	void go_pit3();
	int* go_stream3();
	void CALC_RELZ3();
	void PROCESS3(CPtrArray* pArray);
	void PROCESS2(CPtrArray* pArray);
	void go_stream2();
	void CALC_RELZ2();
	void calcrelief1();
	void up2peak1();
	CDbfFile IDemFile;
	void go_ridge1();
	void go_pit1();
	void go_stream1(int &numup, int &numdn);
	void CALC_RELZ1();
	CDbfFile RelzFile;
	void calc_quinn(int numneigh, double cent_z, double cent_qa, int* zrec, double* deltax, double* pElev, 
		short int* pDDir, int* pDrec, int centrec, double* l,double* pQArea, double* pQweti);
	
	void qfindneig(int cent_x, int cent_y, double cent_z, double cent_qa, int& numneigh, int* zrec, double* l, 
		double* deltax, double orthogonal, double diagonal , int maxcols, int maxrows, double l1, double l2);
	void CALC_WETI(int nNum);
	void pad_edges(double* pSlope, short int* pAspect, double* pProf, double* pPlan,
		int no_cols, int no_rows);
	double max_runoff;
	double max_vol;
	double max_area;
	double max_depth;
	double missing_value;
	void eyton_form(double* row1z, double* row2z, double* row3z, int this_col,
		double* pSlope, short int* pAspect, double* pProf, double* pPlan, int nIndex);
	CDbfFile DEMFile;
	void CALC_FORM();
	CDbfFile FormFile;
	CString strDirectory;
	CString str3CharName;
	HWND phWnd;
	double dDSArea;
	double dUpslopeArea;
	double dGridSize;
	CString sDEMFileName;
	void SetVariables(CPtrArray*);
	CProgressCtrl* pProgressBar2;
	CProgressCtrl* pProgressBar1;
	bool PROCESS1(CPtrArray* pArray);
	CFormProcess();
	virtual ~CFormProcess();

};

#endif // !defined(AFX_FORMPROCESS_H__245B0193_BA98_4F39_89C2_1C6C6EF7CC9B__INCLUDED_)
