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
//		landmapStructs.h
//
//		Various structures for use in Landmap
//
/////////////////////////////////////////////////////////////////////////////////////

#ifndef LANDMAP_STRUCTURES_H
#define LANDMAP_STRUCTURES_H

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

#endif //LANDMAP_STRUCTURES_H