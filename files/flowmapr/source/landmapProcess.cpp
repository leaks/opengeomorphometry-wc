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
//		landmapProcess.cpp
//
//		main application entry point for headless version.
//
/////////////////////////////////////////////////////////////////////////////////////

#include "landmapProcess.h"

/////////////////////////////////////////////////////////////////////////////////////
//
//		Constructors
//
/////////////////////////////////////////////////////////////////////////////////////
LandmapProcess::LandmapProcess()
{
	m_dX = 0.0;
	m_dY = 0.0;
	m_dMissDataValue = 0.0;
	m_dPitDepth = 0.15;
	m_lPitArea = 10;
	m_dPitVolume = 0.0;
	m_iRow = 0;
	m_dGridSize = 10;
	m_iColumn = 0;
	m_sWorkingDirectory = "";
	m_sFileName = "";
	m_bInvertedElev = true;
}

LandmapProcess::~LandmapProcess()
{

}

/////////////////////////////////////////////////////////////////////////////////////
//
//		Public Methods
//
/////////////////////////////////////////////////////////////////////////////////////

double* LandmapProcess::ReadDbf(std::string temp)
{
	int iCol = m_iColumn;
	char szBuffer[255];
	int iRecs = 0;
	int iRowLocation = -1;
	int iColLocation = -1;
	int iNumFields = 0;
	int iElevLocation = -1;
	int iDDirLocation = -1;
	int iDRecLocation = -1;
	int iUpSlpLocation = -1;
	int iShedNowLocation = -1;
	int iFirstRecordLoc = -1;
	int iRecordLength = -1;
	int * piFieldLengths = new int[35];
	FILE * pFile = fopen(temp.c_str(), "r");

	fseek(pFile, 4, SEEK_SET);
	fgets(szBuffer, 4, pFile);

	iRecs = (int)&szBuffer;

	fseek(pFile, 32, SEEK_SET);

	for( int i = 0; i < 100; i++)
	{
		fgets(szBuffer, 32, pFile);
		if(szBuffer[0] != 13)
		{
			if(stricmp(szBuffer, "elev") == 0)
			{
				iElevLocation = i;
				i = 100;
			}
			piFieldLengths[i] = szBuffer[16];
		}
		else
		{
			i = 100;
		}
	}

	fseek(pFile, 8, SEEK_SET);
	fgets(szBuffer, 2, pFile);

	iFirstRecordLoc = (int) &szBuffer;

	fseek(pFile, 10, SEEK_SET);
	fgets(szBuffer, 2, pFile);

	iRecordLength = (int) &szBuffer;

	fseek(pFile, (iFirstRecordLoc + (iRecs -1) * iRecordLength), SEEK_SET);
	fgets(szBuffer, iRecordLength, pFile);

	char szNumBuffer[8] = {'0','0','0','0','0','0','0','0'};

	//not sure if this is ment to be name iBytesOffSet
	int iBitesOfSet = 0;

	for(int i = 0; i < iColLocation; i++)
	{
		iBitesOfSet += piFieldLengths[i];
	}

	char* szP = new char[piFieldLengths[iColLocation]];
	for (int i = 0; i <piFieldLengths[iColLocation]; i++)
	{
		szP[i] = szBuffer[iBitesOfSet + 1 + i];
	}

	char szBuffTemp[8] = {'0','0','0','0','0','0','0','0'};

	for(int i = 0; i < 8; i++)
	{
		szBuffTemp[i] = szBuffer[i];
	}

	iCol = (int) atof(szP);
	iBitesOfSet = 0;
	for(int i = 0; i < piFieldLengths[iRowLocation];i++)
	{
		iBitesOfSet += piFieldLengths[i];
	}
	
	delete szP;
	szP = new char[piFieldLengths[iRowLocation]];

	for(int i = 0; i < piFieldLengths[iRowLocation]; i++)
	{
		szP[i] = szBuffer[iBitesOfSet + 1 + i];
	}

	for(int i = 0; i < 8; i++)
	{
		szBuffTemp[i] = '0';
	}

	for(int i = 0; i < 8; i++)
	{
		szBuffTemp[i] = szBuffer[i];
	}
	iCol = (int)atof(szP);
	delete szP;

	//Read fields Elev, Drec, DDir, UpSlp from dbf table
	double* pdTemp = new double[m_iRow * iCol];
	double* pdElev;
	int* piTemp1 = new int [m_iRow*iCol];
	short* psTemp = new short[m_iRow*iCol];
	int* piTemp2 = new int [m_iRow*iCol];

	iBitesOfSet = 0;
	int iUpSLpBitesOfSet = 0;
	int iDrecBitesOfSet = 0;
	int iDDirBitesOfSet = 0;
	int iShedNowBitesOfSet = 0;

	for(int i = 0; i < iElevLocation; i++)
	{
		iBitesOfSet += piFieldLengths[i];
	}

	for(int i = 0; i < iDRecLocation; i++)
	{
		iDrecBitesOfSet += piFieldLengths[i];
	}

	for(int i = 0; i <iDDirLocation; i++)
	{

	}

	return new double;
}

unsigned int LandmapProcess::RunProcess(void * param)
{
	double		*pdElev = 0;
	CDbfFile	*pDbfFile;
	CDbfFile	*pOutDbfFile;
	CDEMProcess	*pDEMProcess;
	double		*pdTmpDEMGrid;
	double		*pdDEMGrid;
	double		*pdBottomUp;
	short		*pshFlowGrid;
	int			*piShedGrid;
	int			*piShedWGrid;
	int			*piUpSlpGrid;
	int			*piDrecGrid;
	int			*piDEMSort;

	std::string	sTemp;

	double		*pdVol2FlGrid;
	double		*pdMm2FlGrid;
	int			*piPArea;

	if((pdDEMGrid = new double[m_iRow * m_iColumn]) == NULL)
		return 1;

	sTemp = m_sWorkingDirectory + m_sFileName + ".dbf";



	

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////
//
//		Property Methods
//
/////////////////////////////////////////////////////////////////////////////////////

void LandmapProcess::SetFullCurrentPath(std::string fullPath)
{
	m_sFullCurrentPath = fullPath;
}

void LandmapProcess::SetFileName(std::string fileName)
{
	m_sFileName = fileName;
}

void LandmapProcess::SetX(double x)
{
	m_dX = x;
}

void LandmapProcess::SetY(double y)
{
	m_dY = y;
}

void LandmapProcess::SetMissDataValue(double missDataValue)
{
	m_dMissDataValue = missDataValue;
}

void LandmapProcess::SetPitDepth(double pitDepth)
{
	m_dPitDepth = pitDepth;
}

void LandmapProcess::SetPitArea(long pitArea)
{
	m_lPitArea = pitArea;
}

void LandmapProcess::SetPitVolume(double pitVolume)
{
	m_dPitVolume = pitVolume;
}

void LandmapProcess::SetRow(int row)
{
	m_iRow = row;
}

void LandmapProcess::SetGridSize(double gridSize)
{
	m_dGridSize = gridSize;
}

void LandmapProcess::SetColumn(int column)
{
	m_iColumn = column;
}

void LandmapProcess::SetWorkingDir(std::string workingDir)
{
	m_sWorkingDirectory = workingDir;
}

void LandmapProcess::SetInvertedElev(bool invertedElev)
{
	m_bInvertedElev = invertedElev;
}

std::string LandmapProcess::GetFullCurrentPath()
{
	return m_sFullCurrentPath;
}

std::string LandmapProcess::GetFileName()
{
	return m_sFileName;
}

double LandmapProcess::GetX()
{
	return m_dX;
}

double LandmapProcess::GetY()
{
	return m_dY;
}

double LandmapProcess::GetMissDataValue()
{
	return m_dMissDataValue;
}

double LandmapProcess::GetPitDepth()
{
	return m_dPitDepth;
}

long LandmapProcess::GetPitArea()
{
	return m_lPitArea;
}

double LandmapProcess::GetPitVolume()
{
	return m_dPitVolume;
}

int LandmapProcess::GetRow()
{
	return m_iRow;
}

double LandmapProcess::GetGridSize()
{
	return m_dGridSize;
}

int LandmapProcess::GetColumn()
{
	return m_iColumn;
}

std::string LandmapProcess::GetWorkingDir()
{
	return m_sWorkingDirectory;
}

bool LandmapProcess::GetInvertedElev()
{
	return m_bInvertedElev;
}