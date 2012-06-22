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
//#include "stdafx.h"
#include "VoldFile.h"



VoldFile::VoldFile(std::string sNewFileName)
{
	nArraySize = 0;
	nNumberOfRecords = 0;
	pFile = 0;
	sFileName = sNewFileName;
}

VoldFile::~VoldFile()
{
	/*
	ofstream VoldFile(sFileName);
	VoldFile<<"SEQNO,"<<"DDIR,"<<"DREC,"<<"UPSLOPE,"<<"SHEDNOW,"<<"STAGE,"<<"UREC,"<<"DS_AREA,"<<"VARATIO"<<endl;
	for(int nLoop=0;nLoop<nNumberOfRecords;nLoop++)
	{
		VoldFile<<pFile[nLoop].iSeqNo <<","<<
					pFile[nLoop].iDdir<<","<<
					pFile[nLoop].iDrec<<","<<
					pFile[nLoop].iUpslope<<","<<
					pFile[nLoop].iShedNow<<","<<
					pFile[nLoop].iStage<<","<<
					pFile[nLoop].iUrec<<","<<
					pFile[nLoop].iDs_Area<<","<<
					pFile[nLoop].iVaration<<endl;

	}

	VoldFile.close();
	*/
	delete pFile;
	nNumberOfRecords = 0;
}

void VoldFile::AddNew(int iSeqNoNew ,int iDdirNew, int iDrecNew, int iUpslopeNew ,
					   int iShedNowNew , int iStageNew, int iUrecNew , int iDs_AreaNew,
					   double iVarationNew)
{

	if(nArraySize== nNumberOfRecords)
	{
		int nOriginalSize =0;
		VoldFile_t* pTemp;
		pTemp = new VoldFile_t[nArraySize+15000];
		nOriginalSize = nArraySize;
		nArraySize = nArraySize + 15000;
		for(int nLoop=0;nLoop<nOriginalSize;nLoop++)
		{
			pTemp[nLoop] = pFile[nLoop];

		}
		delete pFile;
		pFile = pTemp;
	}
	pFile[nNumberOfRecords].iDdir = iDdirNew;
	pFile[nNumberOfRecords].iDrec = iDrecNew;
	pFile[nNumberOfRecords].iDs_Area  = iDs_AreaNew;
	pFile[nNumberOfRecords].iSeqNo  = iSeqNoNew;
	pFile[nNumberOfRecords].iShedNow = iShedNowNew;
	pFile[nNumberOfRecords].iStage = iStageNew;
	pFile[nNumberOfRecords].iUpslope = iUpslopeNew;
	pFile[nNumberOfRecords].iUrec = iUrecNew;
	pFile[nNumberOfRecords].iVaration = iVarationNew;
	nNumberOfRecords++;

}


int VoldFile::GetDdir(int nIndex)
{
	return pFile[nIndex].iDdir ;

}

int VoldFile::GetDrec(int nIndex)
{
	return pFile[nIndex].iDrec;
}


int VoldFile::GetDs_Area(int nIndex)
{
	return pFile[nIndex].iDs_Area;
}

int VoldFile::GetSeqNo(int nIndex)
{
	return pFile[nIndex].iSeqNo;
}


int VoldFile::GetShedNow(int nIndex)
{
	return pFile[nIndex].iShedNow;
}

int VoldFile::GetStage(int nIndex)
{
	return pFile[nIndex].iStage;
}

int VoldFile::GetUpSlop(int nIndex)
{
	return pFile[nIndex].iUpslope;
}

int VoldFile::GetUrec(int nIndex)
{
	return pFile[nIndex].iUrec;
}

double VoldFile::GetVaration(int nIndex)
{
	return pFile[nIndex].iVaration;
}


void VoldFile::RemoveAt(int nIndex)
{
	if(nNumberOfRecords>nIndex)
	{
		for(int nLoop = nIndex;nLoop<nNumberOfRecords -1;nLoop++)
		{
			pFile[nLoop] = pFile[nLoop+1];
		}
		nNumberOfRecords --;
		
	}
}

int VoldFile::GetSize()
{
	return nNumberOfRecords;
}

void VoldFile::RemoveAll()
{

	delete pFile;
	pFile = 0;
	nNumberOfRecords = 0;

}
