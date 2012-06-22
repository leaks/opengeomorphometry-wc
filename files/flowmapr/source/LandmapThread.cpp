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
//		LandmapThread.cpp
//
//		Author: M Harrison mharrison@niagararesearch.org
//
/////////////////////////////////////////////////////////////////////////////////////

#include "LandmapThread.h"

/////////////////////////////////////////////////////////////////////////////////////
//
//		Constructors
//
/////////////////////////////////////////////////////////////////////////////////////
LandmapThread::LandmapThread()
{
	m_dMissDataValue = Settings::getSingleton()->getValue<double>("missing_data_value"); // 0.0;
	m_dPitDepth = Settings::getSingleton()->getValue<double>("pit_depth"); //0.15;
	m_lPitArea = Settings::getSingleton()->getValue<long>("pit_area"); // 10;
	m_dPitVolume = Settings::getSingleton()->getValue<double>("pit_volume"); // 0.0;
	m_bInvertedElev = Settings::getSingleton()->getValue<bool>("inverted_elev"); // true;
}

LandmapThread::~LandmapThread()
{

}

/////////////////////////////////////////////////////////////////////////////////////
//
//		Public Methods
//
/////////////////////////////////////////////////////////////////////////////////////

void LandmapThread::run(param_t* data, int startPos, int count)
{
	double		*pdElev = 0;
	CDEMProcess	*pDEMProcess;
	double		*pdTmpDEMGrid;
	double		*pdDEMGrid;
	double		*pdBottomUp;
	double		*pdShed;
	short		*pshFlowGrid;
	int			*piShedGrid;
	int			*piShedWGrid;
	int			*piUpSlpGrid;
	int			*piDrecGrid;
	int			*piDEMSort;
	int			*piShedOrd;
	
	int			iPitNo = 0;

	LSMSTATVAR *pLsmStatVar;
	LSMSTATVAR2 *pLsmStatVar2;

	//*********************************
	//IGK
	//3 Arrays used in calculation of volume to flood, mm to flood, and pArea
	double		*pdVol2FlGrid;
	double		*pdMm2FlGrid;
	int			*piPArea;
	//**********************************

	bool bValid = true;

	m_iWidth = Settings::getSingleton()->getValue<int>("input_file_width");
	m_iLength = count / m_iWidth;


	pDEMProcess = new CDEMProcess(m_iLength, m_iWidth, count);
	pDEMProcess->dMissingValue = m_dMissDataValue;

	if((pdTmpDEMGrid = new double[m_iWidth * m_iLength]) == NULL)
	{
		Logger::getSingleton()->log("Not enough memory for Tmp DEM grid.", LOG_OPTIONS::WARNING);
		return;
	}
	if((piDEMSort = new int[m_iWidth * m_iLength]) == NULL)
	{
		Logger::getSingleton()->log("Not enough memory for sort grid.", LOG_OPTIONS::WARNING);
		return;
	}

	int* pTempSort = new int[m_iWidth * m_iLength];
	int nMissingNum = 0;
	int nIndex = 0;
	int nIndex1 = 0;
	
	if((pdDEMGrid = new double[m_iWidth * m_iLength]) == NULL)
	{
		Logger::getSingleton()->log("Not enough memory for DEM grid.", LOG_OPTIONS::WARNING);
		return;
	}

	memcpy(pdDEMGrid, &data->input[startPos], count);
	
	for(int i = 0; i < (m_iWidth * m_iLength); i++)
	{
		if(pdDEMGrid[i] == m_dMissDataValue)
		{
			pTempSort[nMissingNum++] = i;
		}
		else
		{
			pdTmpDEMGrid[nIndex] = pdDEMGrid[i];
			piDEMSort[nIndex++] = i;
		}
	}

	QuickSort(pdTmpDEMGrid, (m_iWidth * m_iLength) - nMissingNum, piDEMSort, true); // sort only non-missing values
	delete[](pdTmpDEMGrid);
	nIndex = 0;

	for(int i = ((m_iWidth * m_iLength) - nMissingNum); i < (m_iWidth * m_iLength); i++)
	{
		piDEMSort[i] = pTempSort[nIndex++];
	}
	delete[](pTempSort);

	//*******************************************************************************
	// IGK
	if((pdVol2FlGrid = new double[m_iWidth * m_iLength]) == NULL)
	{
			Logger::getSingleton()->log("Not enough memory for volume to flood grid.", LOG_OPTIONS::WARNING);
		return;
	}
	if((pdMm2FlGrid = new double[m_iWidth * m_iLength]) == NULL)
	{
		Logger::getSingleton()->log("Not enough memory for mm to flood to flood grid.", LOG_OPTIONS::WARNING);
		return;
	}
	if((piPArea = new int[m_iWidth * m_iLength]) == NULL)
	{	
		Logger::getSingleton()->log("Not enough memory for pArea grid.", LOG_OPTIONS::WARNING);
		return;
	}

	//*******************************************************************************

	if((pshFlowGrid = new short[m_iWidth * m_iLength]) == NULL)
	{
		Logger::getSingleton()->log("Not enough memory for flow direction grid.", LOG_OPTIONS::WARNING);
		return;
	}

	Logger::getSingleton()->log("Calculating flow direction.", LOG_OPTIONS::INFO);
	pDEMProcess->FlowDir(pdDEMGrid, pshFlowGrid);
	
	Logger::getSingleton()->log("Running fixflat.", LOG_OPTIONS::INFO);
	pDEMProcess->FixFlat(pdDEMGrid, pshFlowGrid);

	if((piDrecGrid = new int [m_iWidth * m_iLength]) == NULL)
	{
		Logger::getSingleton()->log("Not enough memory for flow direction grid.", LOG_OPTIONS::WARNING);
		return;
	}

	Logger::getSingleton()->log("Running CalcDrec.", LOG_OPTIONS::INFO);
	pDEMProcess->CalcDrec(pshFlowGrid, piDrecGrid);


	Logger::getSingleton()->log("Running CircleFlow.", LOG_OPTIONS::INFO);
	pDEMProcess->CircleFlow(pshFlowGrid, piDrecGrid);

	if((piShedGrid = new int [m_iWidth * m_iLength]) == NULL)
	{
		Logger::getSingleton()->log("Not enough memory for watershed grid.", LOG_OPTIONS::WARNING);
		return;
	}

	Logger::getSingleton()->log("Calculating water sheds.", LOG_OPTIONS::INFO);
	pDEMProcess->CalcWaterShed(pdDEMGrid, pshFlowGrid, piDrecGrid, piShedGrid, piDEMSort);

	if((piUpSlpGrid = new int[m_iWidth * m_iLength]) == NULL)
	{
		Logger::getSingleton()->log("Not enough memory for upslope grid.", LOG_OPTIONS::WARNING);
		return;
	}

	Logger::getSingleton()->log("Calculating upslope area.", LOG_OPTIONS::INFO);
	pDEMProcess->CalcUpSlopeArea(pdDEMGrid, pshFlowGrid, piDrecGrid, piUpSlpGrid, piDEMSort);

	if((pdBottomUp = new double[m_iWidth * m_iLength]) == NULL)
	{
		Logger::getSingleton()->log("Not enough memory for bottom up index grid.", LOG_OPTIONS::WARNING);
		return;
	}

	if((piShedWGrid = new int[m_iWidth * m_iLength]) == NULL)
	{
		Logger::getSingleton()->log("Not enough memory for watershed grid.", LOG_OPTIONS::WARNING);
		return;
	}

	int *piSortedTemp = new int[m_iWidth * m_iLength]; //temp array to hold index of records with missing elev field.

	nIndex = 0;
	nMissingNum = 0;

	for(int i = 0; i < (m_iWidth * m_iLength); i++)
	{
		piShedWGrid[i] = piShedGrid[i];

		if(pshFlowGrid[i] == 5)
			iPitNo++;

		if(pdDEMGrid[i] != m_dMissDataValue) // make sure that value for elevation is not missing
		{
			piShedWGrid[i] = piShedGrid[i];
			pdBottomUp[nIndex] = (double)(((double)piShedWGrid[i] * 1000000000000000000.0) + (pdDEMGrid[i] * 1000000000000.0) + (1000000.0 - (double)piUpSlpGrid[i])); // replaced pow(10.0, 18.0) and pow(10.0, 12.0) with values
			piDEMSort[nIndex++] = i;
		}
		else
		{
			piSortedTemp[nMissingNum++] = i; // keep track of missing value records
		}
	}

	if((pLsmStatVar = new LSMSTATVAR[iPitNo]) == NULL)
	{
		Logger::getSingleton()->log("Not enough memory for LSM statistic 1.", LOG_OPTIONS::WARNING);
		return;
	}

	QuickSort(pdBottomUp, ((m_iWidth * m_iLength) - nMissingNum), piDEMSort); // sort only records that have valid values
	nIndex = 0;
	for(int i = ((m_iWidth * m_iLength) - nMissingNum); i < (m_iWidth * m_iLength); i++)
	{
		piDEMSort[i] = piSortedTemp[nIndex++];
	}

	delete[](piSortedTemp);

	int nPercentage = 10;
	iPitNo = 0;
	for(int i = 0; i < (m_iWidth * m_iLength); i++)
	{
		if(i == (int)(((m_iWidth * m_iLength) / 100)* nPercentage))
		{
			nPercentage+=10;
		}

		if(pshFlowGrid[piDEMSort[i]] == 5 && pdDEMGrid[piDEMSort[i]] != m_dMissDataValue)
		{
			pDEMProcess->ShedStat1(i, piDEMSort, pdDEMGrid, pshFlowGrid, piShedGrid, piShedWGrid, &pLsmStatVar[iPitNo]);

			if(pLsmStatVar[iPitNo].iShedArea > 0)
				pLsmStatVar[iPitNo].fVaratio = (double)((pLsmStatVar[iPitNo].fPitVol / pLsmStatVar[iPitNo].iShedArea) * 1000.0);
			else
				pLsmStatVar[iPitNo].fVaratio = 0.0;

			if(pLsmStatVar[iPitNo].fVaratio > 999999.99)
				pLsmStatVar[iPitNo].fVaratio = 999999.99;

			iPitNo++;
		}
	}

	if((piShedOrd = new int[m_iWidth * m_iLength]) == NULL)
	{
		Logger::getSingleton()->log("Not enough memory for watershed order index grid.", LOG_OPTIONS::WARNING);
		return;
	}

	if((pdShed = new double[m_iWidth * m_iLength]) == NULL)
	{
		Logger::getSingleton()->log("Not enough memory for watershed order grid.", LOG_OPTIONS::WARNING);
		return;
	}

	if((pLsmStatVar2 = new LSMSTATVAR2) == NULL)
	{
		Logger::getSingleton()->log("Not enough memory for LSM statistic 2.", LOG_OPTIONS::WARNING);
		return;
	}

	for(int i = 0; i < iPitNo; i++)
	{
		pdShed[i] = (double)(((20000.0 - (double)pLsmStatVar[i].fPitElev) * 10000000000) + (int)pLsmStatVar[i].fVaratio);
		piShedOrd[i] = i;
	}

	//*******************************************************************************
	//IGK
	//calculate volume to flood

	Logger::getSingleton()->log("Calculating VOlume2Flood.", LOG_OPTIONS::INFO);
	pDEMProcess->Volume2Flood(piDEMSort, pdDEMGrid, pLsmStatVar, pdVol2FlGrid, pdMm2FlGrid, piPArea, piShedWGrid, piDEMSort, piDrecGrid, iPitNo, pshFlowGrid);

	//*******************************************************************************

	QuickSort(pdShed, iPitNo, piShedOrd);

	int iPitRemoved = 0;

	for(int i = 0; i < iPitNo; i++)
	{
		pLsmStatVar[i].iDrainsTo = pLsmStatVar[i].iOutShed;
		pLsmStatVar[i].iNextPit = 0;
	}

	Logger::getSingleton()->log("Running Lsm_PitR.", LOG_OPTIONS::INFO);
	pDEMProcess->Lsm_PitR(pdDEMGrid, piDEMSort, pLsmStatVar, piShedWGrid, piUpSlpGrid, iPitNo, pshFlowGrid, piDrecGrid, m_lPitArea, m_dPitDepth);

	int nLargest = 0;

	for(int i = 0; i < (m_iWidth * m_iLength); i++)
	{
		piShedGrid[i] = piShedWGrid[i];
	}

	//*******************************************************************************
	//IGK
	//Declare variables to hold pond statistices data, represents pond table

	LSMPONDSTATS* pPondStatVar;
	if((pPondStatVar = new LSMPONDSTATS[1000]) == NULL)
	{
		Logger::getSingleton()->log("Not enough memory for pond statistics.", LOG_OPTIONS::WARNING);
		return;
	}

	int nPondFileSize = 0;

	LSMSTATVAR* pFillFile;
	int nFillFileSize = 0;

	if((pFillFile = new LSMSTATVAR[1000]) == NULL)
	{
		Logger::getSingleton()->log("Not enough memory for fill file.", LOG_OPTIONS::WARNING);
		return;
	}

	
	VoldFile vold("C++Vold.csv");
	VoldFile mold("C++Mold.csv");

	for(int i = 0; i < iPitNo; i++)
	{
		pLsmStatVar[i].fPreVol = 0.0;
	}

	for(int i = 0; i < (m_iWidth * m_iLength); i++)
	{
		piShedGrid[i] = piShedWGrid[i];
	}

	Logger::getSingleton()->log("Running LowPitRemoval.", LOG_OPTIONS::INFO);
	pDEMProcess->LowPitRemoval(pLsmStatVar, &pPondStatVar, (m_iWidth * m_iLength), iPitNo, pdDEMGrid, piDEMSort, piShedOrd, pdShed, pdBottomUp, piShedWGrid, piUpSlpGrid, piDrecGrid, pshFlowGrid, vold, nPondFileSize, pdVol2FlGrid, pdMm2FlGrid, piPArea, pshFlowGrid);

	delete(pdShed);
	delete(piShedOrd);

	Logger::getSingleton()->log("Running Fix_PitFile.", LOG_OPTIONS::INFO);
	pDEMProcess->Fix_PitFile(pLsmStatVar, pdDEMGrid, piShedWGrid, iPitNo, piShedGrid);

	Logger::getSingleton()->log("Running Reset_DEM.", LOG_OPTIONS::INFO);
	pDEMProcess->Reset_Dem(vold, pshFlowGrid, piDrecGrid, piUpSlpGrid, piShedWGrid, piShedGrid);

	Logger::getSingleton()->log("Running LSM_PitR8.", LOG_OPTIONS::INFO);
	pDEMProcess->LSM_PitR8(piShedGrid, pLsmStatVar, iPitNo, &pFillFile, nFillFileSize, pdDEMGrid, piShedWGrid, piUpSlpGrid, mold, pshFlowGrid, piDrecGrid, pdVol2FlGrid, pdMm2FlGrid, pPondStatVar);

	// copy out the data......
	
	double *row;
	Record rec;

	// Pit
	rec.fields = 28;
	for(int i = 0; i < iPitNo; i++)
	{
		row = new double[28];
		row[0] = pLsmStatVar[i].iThisShed;
		row[1] = 0;
		row[2] = (pLsmStatVar[i].bFinal ? 1 : 0);
		row[3] = pLsmStatVar[i].iEndPit;
		row[4] = pLsmStatVar[i].iShedArea;
		row[5] = pLsmStatVar[i].iPitRow;
		row[6] = pLsmStatVar[i].iPitCol;
		row[7] = pLsmStatVar[i].iPitRec;
		row[8] = pLsmStatVar[i].fPitElev;
		row[9] = pLsmStatVar[i].fPourElev;
		row[10] = pLsmStatVar[i].fPreVol;
		row[11] = pLsmStatVar[i].fPitVol;
		row[12] = pLsmStatVar[i].fVaratio;
		row[13] = pLsmStatVar[i].iPitArea;
		row[14] = pLsmStatVar[i].iDrainsTo;
		row[15] = pLsmStatVar[i].iNextPit;
		row[16] = pLsmStatVar[i].iBecomes;
		row[17] = (pLsmStatVar[i].bRemoved ? 1 : 0);
		row[18] = pLsmStatVar[i].iInRow;
		row[19] = pLsmStatVar[i].iInColumn;
		row[20] = pLsmStatVar[i].iInRec;
		row[21] = pLsmStatVar[i].fInElev;
		row[22] = pLsmStatVar[i].iOutRow;
		row[23] = pLsmStatVar[i].iOutColumn;
		row[24] = pLsmStatVar[i].iOutRec;
		row[25] = pLsmStatVar[i].fOutElev;
		row[26] = pLsmStatVar[i].iStage;
		row[27] = (pLsmStatVar[i].bVisited ? 1 : 0);

		rec.data = row;
		
		data->pit.push_back(rec);
	}

	delete[](pLsmStatVar);
	
	// Fill
	rec.fields = 28;
	for(int i = 0; i < nFillFileSize; i++)
	{
		row = new double[28];
		row[0] = pFillFile[i].iThisShed;
		row[1] = (pFillFile[i].bEdge ? 1 : 0);
		row[2] = (pFillFile[i].bFinal ? 1 : 0);
		row[3] = pFillFile[i].iEndPit;
		row[4] = pFillFile[i].iShedArea;
		row[5] = pFillFile[i].iPitRow;
		row[6] = pFillFile[i].iPitCol;
		row[7] = pFillFile[i].iPitRec;
		row[8] = pFillFile[i].fPitElev;
		row[9] = pFillFile[i].fPourElev;
		row[10] = pFillFile[i].fPreVol;
		row[11] = pFillFile[i].fPitVol;
		row[12] = pFillFile[i].fVaratio;
		row[13] = pFillFile[i].iPitArea;
		row[14] = pFillFile[i].iDrainsTo;
		row[15] = pFillFile[i].iNextPit;
		row[16] = pFillFile[i].iBecomes;
		row[17] = (pFillFile[i].bRemoved ? 1 : 0);
		row[18] = pFillFile[i].iInRow;
		row[19] = pFillFile[i].iInColumn;
		row[20] = pFillFile[i].iInRec;
		row[21] = pFillFile[i].fInElev;
		row[22] = pFillFile[i].iOutRow;
		row[23] = pFillFile[i].iOutColumn;
		row[24] = pFillFile[i].iOutRec;
		row[25] = pFillFile[i].fOutElev;
		row[26] = pFillFile[i].iStage;
		row[27] = (pFillFile[i].bVisited ? 1 : 0);

		rec.data = row;
		data->fill.push_back(rec);
	}

	delete[](pFillFile);


	//Pond
	rec.fields = 28;
	for(int i = 0; i < nPondFileSize; i++)
	{
		row = new double[28];
		row[0] = pPondStatVar[i].iThisShed;
		row[1] = (pPondStatVar[i].bEdge ? 1 : 0);
		row[2] = (pPondStatVar[i].bFinal ? 1: 0);
		row[3] = pPondStatVar[i].iEndPit;
		row[4] = pPondStatVar[i].iShedArea;
		row[5] = pPondStatVar[i].iPitRow;
		row[6] = pPondStatVar[i].iPitCol;
		row[7] = pPondStatVar[i].iPitRec;
		row[8] = pPondStatVar[i].fPitElev;
		row[9] = pPondStatVar[i].fPourElev;
		row[10] = pPondStatVar[i].fPreVol;
		row[11] = pPondStatVar[i].fPitVol;
		row[12] = pPondStatVar[i].fVaratio;
		row[13] = pPondStatVar[i].iPitArea;
		row[14] = pPondStatVar[i].iDrainsTo;
		row[15] = pPondStatVar[i].iNextPit;
		row[16] = pPondStatVar[i].iBecomes;
		row[17] = (pPondStatVar[i].bRemoved ? 1 : 0);
		row[18] = pPondStatVar[i].iInRow;
		row[19] = pPondStatVar[i].iInColumn;
		row[20] = pPondStatVar[i].iInRec;
		row[21] = pPondStatVar[i].fInElev;
		row[22] = pPondStatVar[i].iOutRow;
		row[23] = pPondStatVar[i].iOutColumn;
		row[24] = pPondStatVar[i].iOutRec;
		row[25] = pPondStatVar[i].fOutElev;
		row[26] = pPondStatVar[i].iStage;
		row[27] = (pPondStatVar[i].bVisited ? 1 : 0);

		rec.data = row;
		data->pond.push_back(rec);
	}

	delete[](pPondStatVar);

	//Vold
	rec.fields = 9;
	for(int i = 0; i < vold.GetSize(); i++)
	{
		row = new double[9];
		row[0] = vold.GetSeqNo(i);
		row[1] = vold.GetDdir(i);
		row[2] = vold.GetDrec(i);
		row[3] = vold.GetUpSlop(i);
		row[4] = vold.GetShedNow(i);
		row[5] = vold.GetStage(i);
		row[6] = vold.GetUrec(i);
		row[7] = vold.GetDs_Area(i);
		row[8] = vold.GetVaration(i);

		rec.data = row;
		data->vold.push_back(rec);
	}

	vold.RemoveAll();

	//mold
	rec.fields = 9;
	for(int i = 0; i < mold.GetSize(); i++)
	{
		row = new double[9];
		row[0] = mold.GetSeqNo(i);
		row[1] = mold.GetDdir(i);
		row[2] = mold.GetDrec(i);
		row[3] = mold.GetUpSlop(i);
		row[4] = mold.GetShedNow(i);
		row[5] = mold.GetStage(i);
		row[6] = mold.GetUrec(i);
		row[7] = mold.GetDs_Area(i);
		row[8] = mold.GetVaration(i);

		rec.data = row;
		data->vold.push_back(rec);
	}
	
	mold.RemoveAll();

	//DEM
	rec.fields = 14;
	for(int i = 0; i < (m_iWidth * m_iLength); i++)
	{
		row = new double[14];
		row[0] = i + 1;					// index
		row[1] = (i / m_iLength) + 1;	// row
		row[2] = (i % m_iLength) + 1;	// column
		row[3] = pdDEMGrid[i];
		row[4] = pshFlowGrid[i];
		row[5] = piDrecGrid[i];
		row[6] = piUpSlpGrid[i];
		row[7] = piShedGrid[i];
		row[8] = piShedWGrid[i];
		row[9] = (pdDEMGrid[i] == m_dMissDataValue ? 1 : 0);
		row[10] = 0;
		row[11] = pdVol2FlGrid[i];
		row[12] = pdMm2FlGrid[i];
		row[13] = piPArea[i];

		rec.data = row;
		data->dem.push_back(rec);
	}

	delete[](piDrecGrid);
	delete[](pshFlowGrid);
	delete[](piShedGrid);
	delete[](piShedWGrid);
	delete[](piUpSlpGrid);
	delete[](pLsmStatVar2);

	if(m_bInvertedElev)
	{
		double		*pdInvElev;
		short		*pshInvFlow;
		int			*piInvDrec;
		int			*piInvUpSlp;
		int			*piInvShedNo;
		int			*piInvShedNow;
		double		*pdInvVol2Fld;
		double		*pdInvMm2Fld;
		int			*piInvPArea;
		LSMSTATVAR	*pInvLsmStatVar;

		double	dHighest = 0;

		if((pdInvElev = new double[m_iWidth * m_iLength]) == NULL)
		{
			Logger::getSingleton()->log("Not enough memory to create inverted elevated array.", LOG_OPTIONS::WARNING);
			return;
		}

		if((pshInvFlow = new short[m_iWidth * m_iLength]) == NULL)
		{
			Logger::getSingleton()->log("Not enough memory to create inverted flow array.", LOG_OPTIONS::WARNING);
			return;
		}

		if((piInvDrec = new int[m_iWidth * m_iLength]) == NULL)
		{
			Logger::getSingleton()->log("Not enough memory to create inverted drainage array.", LOG_OPTIONS::WARNING);
			return;
		}

		if((piInvUpSlp = new int[m_iWidth * m_iLength]) == NULL)
		{
			Logger::getSingleton()->log("Not enough memory to create inverted UpSlope array.", LOG_OPTIONS::WARNING);
			return;
		}

		if((piInvShedNo = new int[m_iWidth * m_iLength]) == NULL)
		{
			Logger::getSingleton()->log("Not enough memory to create inverted ShedNo array.", LOG_OPTIONS::WARNING);
			return;
		}
		
		if((piInvShedNow = new int[m_iWidth * m_iLength]) == NULL)
		{
			Logger::getSingleton()->log("Not enough memory to create inverted ShedNoW array.", LOG_OPTIONS::WARNING);
			return;
		}

		if((pdInvVol2Fld = new double[m_iWidth * m_iLength]) == NULL)
		{
			Logger::getSingleton()->log("Not enough memory to create inverted volume to flood array.", LOG_OPTIONS::WARNING);
			return;
		}

		if((pdInvMm2Fld = new double[m_iWidth * m_iLength]) == NULL)
		{
			Logger::getSingleton()->log("Not enough memory to create inverted Mm to flood array.", LOG_OPTIONS::WARNING);
			return;
		}

		if((piInvPArea = new int[m_iWidth * m_iLength]) == NULL)
		{
			Logger::getSingleton()->log("Not enough memory to create inverted PArea array.", LOG_OPTIONS::WARNING);
			return;
		}

		for(int i = 0; i < (m_iWidth * m_iLength); i++)
		{
			if(dHighest < pdDEMGrid[i])
				dHighest = pdDEMGrid[i];
		}

		memcpy(pdInvElev, pdDEMGrid, (m_iWidth * m_iLength));
		for(int i = 0; i < (m_iWidth * m_iLength); i++)
		{
			if(pdInvElev[i] != m_dMissDataValue)
				pdInvElev[i] -= dHighest;
		}
		delete[](pdDEMGrid);

		Logger::getSingleton()->log("Running inverted flowdir.", LOG_OPTIONS::INFO);
		pDEMProcess->FlowDir(pdInvElev, pshInvFlow);

		Logger::getSingleton()->log("Running inverted fixflat.", LOG_OPTIONS::INFO);
		pDEMProcess->FixFlat(pdInvElev, pshInvFlow);

		Logger::getSingleton()->log("Running inverted CalcDrec.", LOG_OPTIONS::INFO);
		pDEMProcess->CalcDrec(pshInvFlow, piInvDrec);

		Logger::getSingleton()->log("Running inverted CircleFLow.", LOG_OPTIONS::INFO);
		pDEMProcess->CircleFlow(pshInvFlow, piInvDrec);

		if((pdTmpDEMGrid = new double[m_iWidth * m_iLength]) == NULL)
		{
			Logger::getSingleton()->log("Not enough memory for TmpDEMGrid.", LOG_OPTIONS::WARNING);
			return;
		}

		nMissingNum = 0;
		nIndex = 0;

		for(int i = 0; i < (m_iWidth * m_iLength); i++)
		{
			if(pdInvElev[i] != m_dMissDataValue)
			{
				pdTmpDEMGrid[nIndex] = pdInvElev[i];
				piDEMSort[nIndex++] = i;
			}
			else
				pTempSort[nMissingNum++] = i;
		}

		QuickSort(pdTmpDEMGrid, (m_iWidth * m_iLength) - nMissingNum, piDEMSort, true);
		delete[] pdTmpDEMGrid;

		nIndex = 0;
		for(int i = (m_iWidth * m_iLength) - nMissingNum; i < (m_iWidth * m_iLength); i++)
			piDEMSort[i] = pTempSort[nIndex++];

		Logger::getSingleton()->log("Running inverted CalcWaterShed", LOG_OPTIONS::INFO);
		pDEMProcess->CalcWaterShed(pdInvElev, pshInvFlow, piInvDrec, piInvShedNo, piDEMSort);

		Logger::getSingleton()->log("Running inverted CalcUpSlpArea.", LOG_OPTIONS::INFO);
		pDEMProcess->CalcUpSlopeArea(pdInvElev, pshInvFlow, piInvDrec, piInvUpSlp, piDEMSort);

		int nInvertedPitNo = 0;

		piSortedTemp = new int[m_iWidth * m_iLength];

		nIndex = 0;
		nMissingNum = 0;

		memcpy(piInvShedNow, piInvShedNo, (m_iWidth * m_iLength));
		for(int i = 0; i < (m_iWidth * m_iLength); i++)
		{
			if(pshInvFlow[i] == 5)
				nInvertedPitNo++;

			if(pdInvElev[i] != m_dMissDataValue)
			{
				pdBottomUp[nIndex] = (double)(((double)pshInvFlow[i] * 1000000000000000000) + ((double) pdInvElev[i] * 1000000000000) + 1000000.0 - (double)piInvUpSlp[i]);
				piDEMSort[nIndex++] = i;
			}
			else
				piSortedTemp[nMissingNum++] = i;
		}

		if((pInvLsmStatVar = new LSMSTATVAR[nInvertedPitNo]) == NULL)
		{
			Logger::getSingleton()->log("Not enough memory for LSM statistic 1.", LOG_OPTIONS::WARNING);
			return;
		}

		for(int i = 0; i < nInvertedPitNo; i++)
		{
			pInvLsmStatVar[i].bEdge = false;
			pInvLsmStatVar[i].bFinal = false;
			pInvLsmStatVar[i].bRemoved = false;
			pInvLsmStatVar[i].bVisited = false;
			pInvLsmStatVar[i].iNextPit = 0;
			pInvLsmStatVar[i].iBecomes = 0;
			pInvLsmStatVar[i].iStage = 0;
			pInvLsmStatVar[i].fPreVol = 0;
			pInvLsmStatVar[i].iDrainsTo = 0;
			pInvLsmStatVar[i].iEndPit = 0;
		}

		QuickSort(pdBottomUp, (m_iWidth * m_iLength) - nMissingNum, piDEMSort);
		delete[](pdBottomUp);

		nIndex = 0;
		for(int i = (m_iWidth * m_iLength) - nMissingNum; i < (m_iWidth * m_iLength); i++)
			piDEMSort[i] = piSortedTemp[nIndex++];
		
		delete[](piSortedTemp);

		nPercentage = 10;

		nInvertedPitNo = 0;
		for(int i = 0; i < (m_iWidth * m_iLength); i++)
		{
			if(i == (int)((m_iWidth * m_iLength)/100) * nPercentage)
				nPercentage += 10;

			if(pshInvFlow[piDEMSort[i]] == 5 && pdInvElev[piDEMSort[i]] != m_dMissDataValue)
			{
				pDEMProcess->ShedStat1(i, piDEMSort, pdInvElev, pshInvFlow, piInvShedNo, piInvShedNow, &pInvLsmStatVar[nInvertedPitNo]);

				if(pInvLsmStatVar[nInvertedPitNo].iShedArea > 0)
					pInvLsmStatVar[nInvertedPitNo].fVaratio = (double) (pInvLsmStatVar[nInvertedPitNo].fPitVol / pInvLsmStatVar[nInvertedPitNo].iShedArea * 1000.0);
				else
					pInvLsmStatVar[nInvertedPitNo].fVaratio = 0.0;

				if(pInvLsmStatVar[nInvertedPitNo].fVaratio > 999999.99)
					pInvLsmStatVar[nInvertedPitNo].fVaratio = 999999.99;
				nInvertedPitNo++;
			}
		}

		Logger::getSingleton()->log("Running inverted Lsm_PitR.", LOG_OPTIONS::INFO);

		pDEMProcess->Lsm_PitR(pdInvElev, piDEMSort, pInvLsmStatVar, piInvShedNow, piInvUpSlp, nInvertedPitNo, pshInvFlow, piInvDrec, m_lPitArea, m_dPitDepth);

		//IDEM
		rec.fields = 14;
		for(int i = 0; i < (m_iWidth * m_iLength); i++)
		{
			row = new double[14];
			row[0] = i + 1;					// index
			row[1] = (i / m_iLength) + 1;	// row
			row[2] = (i % m_iLength) + 1;	// column
			row[3] = pdInvElev[i];
			row[4] = pshInvFlow[i];
			row[5] = piInvDrec[i];
			row[6] = piInvUpSlp[i];
			row[7] = piInvShedNo[i];
			row[8] = piInvShedNow[i];
			row[9] = (pdInvElev[i] == m_dMissDataValue ? 1 : 0);
			row[10] = 0;
			row[11] = pdInvVol2Fld[i];
			row[12] = pdInvMm2Fld[i];
			row[13] = piInvPArea[i];

			rec.data = row;
			data->idem.push_back(rec);
		}

		// iPit
		rec.fields = 28;
		for(int i = 0; i < iPitNo; i++)
		{
			row = new double[28];
			row[0] = pInvLsmStatVar[i].iThisShed;
			row[1] = 0;
			row[2] = (pInvLsmStatVar[i].bFinal ? 1 : 0);
			row[3] = pInvLsmStatVar[i].iEndPit;
			row[4] = pInvLsmStatVar[i].iShedArea;
			row[5] = pInvLsmStatVar[i].iPitRow;
			row[6] = pInvLsmStatVar[i].iPitCol;
			row[7] = pInvLsmStatVar[i].iPitRec;
			row[8] = pInvLsmStatVar[i].fPitElev;
			row[9] = pInvLsmStatVar[i].fPourElev;
			row[10] = pInvLsmStatVar[i].fPreVol;
			row[11] = pInvLsmStatVar[i].fPitVol;
			row[12] = pInvLsmStatVar[i].fVaratio;
			row[13] = pInvLsmStatVar[i].iPitArea;
			row[14] = pInvLsmStatVar[i].iDrainsTo;
			row[15] = pInvLsmStatVar[i].iNextPit;
			row[16] = pInvLsmStatVar[i].iBecomes;
			row[17] = (pInvLsmStatVar[i].bRemoved ? 1 : 0);
			row[18] = pInvLsmStatVar[i].iInRow;
			row[19] = pInvLsmStatVar[i].iInColumn;
			row[20] = pInvLsmStatVar[i].iInRec;
			row[21] = pInvLsmStatVar[i].fInElev;
			row[22] = pInvLsmStatVar[i].iOutRow;
			row[23] = pInvLsmStatVar[i].iOutColumn;
			row[24] = pInvLsmStatVar[i].iOutRec;
			row[25] = pInvLsmStatVar[i].fOutElev;
			row[26] = pInvLsmStatVar[i].iStage;
			row[27] = (pInvLsmStatVar[i].bVisited ? 1 : 0);

			rec.data = row;
		
			data->ipit.push_back(rec);
		}

		delete[](piDEMSort);
		delete(pDEMProcess);
		delete[](piInvDrec);
		delete[](piInvPArea);
		delete[](piInvShedNo);
		delete[](piInvShedNow);
		delete[](piInvUpSlp);
		delete[](pdInvVol2Fld);
		delete[](pdInvMm2Fld);
		delete[](pshInvFlow);
		delete[](pdInvElev);

		Logger::getSingleton()->log("Calculation complete.", LOG_OPTIONS::INFO);
	}
}

/////////////////////////////////////////////////////////////////////////////////////
//
//		Property Methods
//
/////////////////////////////////////////////////////////////////////////////////////

//void LandmapThread::SetMissDataValue(double missDataValue)
//{
//	m_dMissDataValue = missDataValue;
//}
//
//void LandmapThread::SetPitDepth(double pitDepth)
//{
//	m_dPitDepth = pitDepth;
//}
//
//void LandmapThread::SetPitArea(long pitArea)
//{
//	m_lPitArea = pitArea;
//}
//
//void LandmapThread::SetPitVolume(double pitVolume)
//{
//	m_dPitVolume = pitVolume;
//}
//
//void LandmapThread::SetInvertedElev(bool invertedElev)
//{
//	m_bInvertedElev = invertedElev;
//}
//
//double LandmapThread::GetMissDataValue()
//{
//	return m_dMissDataValue;
//}
//
//double LandmapThread::GetPitDepth()
//{
//	return m_dPitDepth;
//}
//
//long LandmapThread::GetPitArea()
//{
//	return m_lPitArea;
//}
//
//double LandmapThread::GetPitVolume()
//{
//	return m_dPitVolume;
//}
//
//bool LandmapThread::GetInvertedElev()
//{
//	return m_bInvertedElev;
//}
