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
// DEMProcess.h: interface for the CDEMProcess class.
//
//////////////////////////////////////////////////////////////////////

#ifndef DEM_PROCESS_H
#define DEM_PROCESS_H

//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000

//#include "landmapDlg.h"
//#include "landmapDlg.h"
#include <math.h>
#include <malloc.h>
#include <time.h>
//#include "landmap.h"
#include <vector>
#include "landmapStructs.h"
#include "VOLDFile.h"

#define sqrt2 1.4142
#include "Utility/Sort.h"
//typedef CArray<double, double> CFloatArray;

class CDEMProcess  
{
public:
	double mv_pour;
	bool drains_2_mv;
	double dMissingValue;
	bool bIgor;
	void SortBottomUp2(int *piShedWGrid, double *pfDEMGrid, int *piUpSlpGrid, int** piSortedDEM, int nOldShed1, int nOldShed2, int nNewShed,int*  piHeapTbl, int** piTempSortedDEM, double* pfBottomUp);
	void SortBottomUp(int* piShedNo, double* pfDEMGrid, int* piUpSlpGrid, int* piSortedDem, int nOldShed, int nNewShed,  int* piHeapTbl, int* piTempSortedDEM, double* pfBottomUp);
	//void Lsm_PitR(double* pfDEMGrid,int* piDEMSort, LSMSTATVAR* pPitFile, int* piShedWGrid,int* piUpSlpGrid, int& iPitNo, short* piFlowGrid, int* piDrecGrid, int max_area, double max_depth, CLandmapDlg* dlgProgressBar, CWnd* pWndNotifyProgress, CString* sMessage);
	void Lsm_PitR(double* pfDEMGrid,int* piDEMSort, LSMSTATVAR* pPitFile, int* piShedWGrid,int* piUpSlpGrid, int& iPitNo, short* piFlowGrid, int* piDrecGrid, int max_area, double max_depth);
	//void LSM_PitR8(int* piShedGrid,LSMSTATVAR* PitFile, int& iPitNo,LSMSTATVAR** pFillFile,	int& nFillFileSize,	double* pfDEMGrid,int*  piShedWGrid,int* piUpSlpGrid, CVoldFile& mold, short* piFlowGrid, int* piDrecGrid,double* pfVol2FlGrid,double* pfMm2FlGrid,	LSMPONDSTATS * pPondStats, CLandmapDlg*	dlgProgressBar,	CWnd* pWndNotifyProgress, CString* sMessage);
	void LSM_PitR8(int* piShedGrid,LSMSTATVAR* PitFile, int& iPitNo,LSMSTATVAR** pFillFile,	int& nFillFileSize,	double* pfDEMGrid,int*  piShedWGrid,int* piUpSlpGrid, VoldFile& mold, short* piFlowGrid, int* piDrecGrid,double* pfVol2FlGrid,double* pfMm2FlGrid,	LSMPONDSTATS * pPondStats);
	void Reset_Dem(VoldFile& vold, short* piFlowGrid, int* piDrecGrid, int* pfUpSlpGrid, int* piShedWGrid, int* piShedGrid);
	void Fix_PitFile(LSMSTATVAR* ShedStat, double* pfDEMGrid, int* piShedNow, int iPitNo, int* piShedGrid);
	int new_shed;
	
	void ShedStat3(int& new_pitrec, int& nPointer, int* pShedNow, int* piDEMSort, double* pfDEMGrid, LSMPONDSTATS** pPondStats, int* piUpSlpGrid, double* pondcell_elev, double* pfShed, int* piShedOrd, int& nPondFileSize, double* pfVol2FlGrid, double* pfMm2FlGrid, int* pfPArea, short* piFlowGrid);
	void Remove2Pit(LSMPONDSTATS* pPondStats,int& nPointer, int* piShedOrd, int iPitNo,int* pDirGrid, short* pFlowGrid, int* pUpSlpGrid, int* pShedNow, int** piDEMSort, VoldFile& vold, double* pfBottomUp, double* pfDEMGrid, int& new_pitrec, bool& Ok_2_Remove, int*  piHeapTbl, int** piTempSortedDEM);

	void FindLast(LSMPONDSTATS*,int&, int, int* piShedOrd);

	//void LowPitRemoval( LSMSTATVAR* ShedStat, LSMPONDSTATS** pPondStats, double m_GridSize,  int iPitNo, double* pfDEMGrid, int* piDEMSort, int* piShedOrd, double* pfShed, double* pfBottomUp,	int* piShedWGrid, int* piUpSlpGrid, int* pDrecGrid, short* pFlowGrid,CVoldFile& vold, int& nPondFileSize, double* pfVol2FlGrid, double* pfMm2FlGrid, int* pfPArea, short* piFlowGrid,CWnd* pWndNotifyProgress, CString* sMessage);
	void LowPitRemoval( LSMSTATVAR* ShedStat, LSMPONDSTATS** pPondStats, double m_GridSize,  int iPitNo, double* pfDEMGrid, int* piDEMSort, int* piShedOrd, double* pfShed, double* pfBottomUp,	int* piShedWGrid, int* piUpSlpGrid, int* pDrecGrid, short* pFlowGrid, VoldFile& vold, int& nPondFileSize, double* pfVol2FlGrid, double* pfMm2FlGrid, int* pfPArea, short* piFlowGrid);
	CDEMProcess(int Row, int Column, double GridSize);
	virtual ~CDEMProcess();
	int		FlowDir(double *DEMGrid, short *FlowGrid);
	int		FixFlat(double *DEMGrid, short *FlowGrid);
	int		CalcDrec(short *FlowGrid, int *DrecGrid);
	int		CircleFlow(short *FlowGrid, int *DrecGrid);
	int		CalcWaterShed(double *DEMGrid, short *Flowgrid, int *DrecGrid, int *ShedGrid, int *iDEMSort);
	int		CalcUpSlopeArea(double *DEMGrid, short *FlowGrid, int *DrecGrid, int *UpSlpGrid, int *iDEMSort);
	void	LSMStat(double *DEMGrid, short *FlowGrid, int *ShedGrid, int *ShedWGrid, LSMSTATVAR *ShedStat, int *iDEMSort);
	void	LSMPitr(double *DEMGrid, short *FlowGrid, int *ShedGrid, int *ShedWGrid, LSMSTATVAR *ShedStat, LSMPROCPARM *LsmProcParm);
	void	ShedStat1(int PitLoc, int *iDEMSort, double *DEMGrid, short *FlowGrid, int *ShedGrid, int *ShedWGrid, LSMSTATVAR *ShedStat);
	void	ShedStat2(int PitLoc, int *iDEMSort, double *DEMGrid, short *FlowGrid, int *ShedGrid, int *ShedWGrid, LSMSTATVAR2 *ShedStat);
	bool	CheckIfOk(LSMSTATVAR *ShedStat, LSMPROCPARM *LsmProcParm, LSMSTATVAR2 *ShedStat2);
	int		RemovePit(int *UpSlpGrid, short *FlowGrid, int *DrecGrid, int *ShedGrid, int *ShedWGrid, LSMSTATVAR2 *ShedStat2, int *NewShed);
	
	//****************************************************************************************
	//IK
	void	Volume2Flood(int *iDEMSort, double *DEMGrid,LSMSTATVAR *ShedStat, 
							   	double *pfVol2FlGrid, double *pfMm2FlGrid, int *pfPArea, int *piShedWGrid, 
								int *piDEMSort,int *piDrecGrid, int iPitNo, short *piFlowGrid);
	//*******************************************************************************************
protected:
	void AddStat2(LSMSTATVAR* pPit, int& iPitNo);
	void ShedStat21(double* pfDEMGrid, int* piShedWGrid, int* UpSlpGrid, int& nDEMPointer, int* piDEMSort, LSMSTATVAR* pPit, int& iPitNo, double* pondcell_elev);
	void RemovePit1(double* pElev, short* pFlowGrid, int* pDrecGrid, int* pShedWGrid, double* pDEMGrid, int* pUpSlpGrid, int& nDEMPointer);
	bool CheckIfOk1(LSMSTATVAR* pPit, int& nPitPointer, int max_area, double max_depth, int iPitNo);
	void Final_Pits(LSMSTATVAR* PitFile, int& iPitNo, LSMSTATVAR** pFillFile, int& iFillFileSize, LSMPONDSTATS * pPondStats);
	void New_mm2Flood(int& nDEMPointer,int* piDEMSort, double* pfDEMGrid, int* piShedWGrid, double* pfVol2FlGrid,double* pfMm2FlGrid);
	void AddStat8(LSMSTATVAR** pFillFile, int& nFillFileSize,LSMSTATVAR* PitFile, int& iPitNo);
	void CalcStat8(double* pondcell_elev);
	int new_ups;
	void ShedStat8(int* piShedWGrid, double* pfDEMGrid, int* piDEMSort, int& nDEMPointer, int* UpSlpGrid,LSMSTATVAR** pFillFile, int& nFillFileSize, LSMSTATVAR *PitFile, int &iPitNo, double* Vol2Fld, double* Mm2Fl, double* pondcell_elev);
	int new_pitrec;
	double curr_mm2flood;
	void RemovePit8( VoldFile& mold, short* piFlowGrid, int* piDrecGrid, double* pfDEMGrid, int* piUpSlpGrid, int*  piShedWGrid, double* pfBottomUp, int** piDEMSort, int*  piHeapTbl, int** piTempSortedDEM);
	void Find_Low_Pit(LSMSTATVAR *PitFile, int& nPitPointer, int iPitNo);
	int pit2_end;
	int pit1_end;
	void Chek4Edge(LSMSTATVAR* PitFile, bool& ok_2_remove, int& nPitPointer, int iPitNo);
	void Find_LowVa(LSMSTATVAR* PitFile, int iPitNo, bool& ok_2_remove, int& nPitFilePointer);
	int no_pits;
	double max_va;
	int m_shednow;
	int m_upslope;
	int m_drec;
	int m_ddir;
	int m_seqno;
	double m_varatio;
	int centrec;
	double cent_z;
	int cent_x;
	int cent_y;
	int old_shed;
	int centre_row;
	int neigh_row;
	int centre_col;
	int neigh_col;
	double pit2_vol;
	double pit1_vol;
	int this_shed;
	int maxcols;
	int maxrows;
	int max_ponds;
	bool pit2_edge;
	double pit2_pour;
	int pit2_rec;
	double pit2_z;
	int pit1_rec;
	int lowest_pit;
	bool pit1_edge;
	double pit1_pour;
	int pit1_to;
	double pit1_z;
	void New_FlodVol(int* piDEMSort, int* piShedWGrid,double* pfDEMGrid, int* piUpSlpGrid, double* pfVol2FlGrid, double* pfMm2FlGrid, int* pfPArea, short* piFlowGrid);
	int pit2_no;
	int pit1_no;
	int new_shed_area;
	double shed_mm2fl;
	double pre_vol;
	int pit_rec;
	int pit_col;
	int pit_row;
	int shed_area;
	int max_shed;
	void AddStat3(LSMPONDSTATS ** pPondStats, int& nPondFileSize, double* pfShed, int* piShedOrd, int& nPointer);
	double cell_area;
	double pit_elev;
	//double pondcell_elev;
	double pit_vol;
	void CalcStat1(double* pondcell_elev);
	bool at_final;
	bool alldone;
	bool edge_pit;
	bool overspill;
	int pondcell_no;
	double out_elev;
	int out_row;
	int out_col;
	int out_rec;
	int out_shed;
	int pit_area;
	int in_rec;
	double in_elev;
	int in_row;
	int in_col;
	double pour_elev;

	void CheckNeig3(int nDEMPointer, int* piDEMSort, double* pfDEMGrid, int* pShedNow, int* zrec);
	void GetNeig1(int* zrec, int cent_y, int cent_x, double cent_z, int& numneigh);

	int		GetNewDir(int NeighRow, int NeighColumn, int CenRow, int CenColumn);

private:

	int		_iRow;
	int		_iColumn;
	double	_fGridSize;
	int		*_pFlatCell;
	int		_iNumFlatCell;

};

#endif // DEM_PROCESS_H
