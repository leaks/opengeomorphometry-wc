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
// WeppProcess.h: interface for the CWeppProcess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WEPPPROCESS_H__4D39C433_5BFF_4644_BF0F_7673A42F0575__INCLUDED_)
#define AFX_WEPPPROCESS_H__4D39C433_5BFF_4644_BF0F_7673A42F0575__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "WeppMaprDlg.h"
#include "Sort.h"
#include <Time.h>
#include <Math.h>
#include<fstream>
#include "DbfFile.h"


class CWeppProcess  
{

public:
	CString m_Message;
public:
	CWeppProcess();
	virtual ~CWeppProcess();

public:
	void Find_UpSegs();
	int max_chans;
	void Remark_Chans(int* pSortedIndex);
	void Cut_Chan(int* piSortedIndex);
	void Merge_Chan(int* piSortedIndex);
	BOOL Mark_Chans(int* piSortedIndex);
public:
	bool* pbStopProcess;
	double m_MaxSegLength;
	
	int m_Size;
	double m_MissingData;
	int m_Upslope;
	CString m_FileName;
	CString m_Directory;
	void RunProcess(double m_MissingData1, CString m_Directory1, CString m_FileName1, 
		int m_Size1, int m_Upslope1,CWnd* pWndNotifyProgress, CProgressCtrl* pProgressBar,
		CString sFullPathParam,int m_MaxSegLength,CPtrArray *pArray);
	int nChanSize;
	void Chan_Stats();
	int nHillFileSize;
	int nProfSize;
	int nTempFileSize;
	void Hill_Stats(int* piHillNo, int* piDEMSort, int* piShedNo, short int* piShedSide);
	void WriteDbf2();
	//int* piHill_No;
	//int* piShedNo;
	//short int* piShedSide;
protected:
	double* pdDEMElev;
	float* pfZ2St;
	int* piN2St;
	float* pfL2St;
	void Wepp_Len();
	short int* piAspect;
	float* pfSlope;
	void Wepp_Form(int* piSortedIndex);
	void WriteDbf();
	int nStructureSize;
	void Build_Stru(int* piShedNo, int* piHillNo, short int* piShedSide);
	int nMax_Hills;
	void Renum_Segs(int* piShedNo);

	void Hill_Sheds(int* piSortedIndex, int* piShedNo, short int* piShedSide, int* piHillNo);
	void Add_Ddir();
	void Redo_Ddir();
	void Order_Segs(int* piSortedIndex);
	void Calc_Side(int& side_val, int& up_loc, int* Cell_no, int& down_loc, int nLoop);
	short int* piChan_Side;
	void FindSide(int* cent_y, int* cent_x, double* cent_z, int* cent_rec,
		int* piSortedIndex);
	void RecordSeg(int mstart_cell, int mstart_row, int mstart_col, double mstart_elev, 
		int mstart_type, bool mimpound, int seg_no, int mend_cell, int mend_row, 
		int mend_col, double mend_elev, int mend_type,int seg_len);
	short int* piSegNo;
	int nSegSize;
	
	void calc_segs(int* piSortedIndex);
	void Flow2Chan(int* piSortedIndex);
	void split_segs(int* piSortedIndex);
	int nCurrentRecord;
	void above_junct();
	int* piOutRec;
	int* piPitRec;
	void ReadFillDbf(int* nNumPits);
	void Mark_Pits(int* piSortedIndex);
	void CalcUpArea(int* piSortedIndex);
	int* piDEMShedNow;
	short int * piDEMDdir;
	int* piDEMDrec;
	int* piShedNow;
	short int * piSeedType;
	int * piChan_No;
	double* pdElev;
	short int* piDdir;
	int* piDrec;
	int* piUpSlope;
	BOOL ReadDbf();
	int m_Col;
	int m_Row;
	HICON m_hIcon;
	


	struct Structure
	{
		int Element_No;
		short int Ele_Type;
		int Left_Hill;
		int Right_Hill;
		int Top_Hill;
		int Left_Chan;
		int Right_Chan;
		int Top_Chan;
		int Left_Imp;
		int Right_Imp;
		int Top_Imp;
		char Comment [30];
	};
	Structure* pStructure;
	


	struct STemp
	{
		double Slope;
		int Aspect;
		int N2St;
		double Rel_n2St;

	};
	STemp* pSTemp;
	struct Prof
	{
		int Hill_No;
		double Distance;
		double Slope;
		double Aspect;
		double Rel_n2St;
		short int Ofe_Num;
		int Soil_Id;
		int Manage_Id;
	};
	Prof* pProf;


	struct ChanStat
	{
		int Chan_No;
		double Chan_Len;
		int Num_Points;
		double Mean_Slope;
		double Gen_Slope;
		int Aspect;
		char Profile[250];

	};
	ChanStat* pChanStat;

	struct Hill
	{
		int Hill_No;
		double Hill_Width;
		int Hill_Area;
		double Max_Len;
		double Wepp_Len;
		int Num_Points;
		int Aspect;
		char Profile [250];
	};
	Hill* pHill;

	struct Segs
	{
		int Initial_Id;
		int Final_Id;
		int Sort_Order;
		short int Start_Type;
		int Start_Cell;
		short int Start_Row;
		short int Start_Col;
		double Start_Elev;
		short Start_Ddir;
		short int End_Type;
		int End_Cell;
		short int End_Row;
		short int End_Col;
		double End_Elev;
		int Len_Cells;
		int Len_Meters;
		int Width;
		int Drains_Rec;
		int Down_Seg;
		int Left_Seg;
		int Right_Seg;
		int Center_Seg;
		int Left_Rec;
		int Right_Rec;
		int Center_Rec;
		int Left_Hill;
		int Right_Hill;
		int Top_Hill;
		int Left_Imp;
		int Right_Imp;
		int Top_Imp;
		short int Chan_Shape;
		double Flow2Crow;
		bool Impound;
	};
	Segs* pSegs;

};

#endif // !defined(AFX_WEPPPROCESS_H__4D39C433_5BFF_4644_BF0F_7673A42F0575__INCLUDED_)
