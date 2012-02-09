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
// WeppProcess.cpp: implementation of the CWeppProcess class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WeppMapr.h"
#include "WeppProcess.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWeppProcess::CWeppProcess()
{
	pSegs = 0;
	piPitRec = 0;
	piUpSlope = 0;
	piShedNow = 0;
	piSegNo = 0;
	piSeedType = 0;
	piOutRec = 0;
	piDrec = 0;
	piDEMShedNow = 0;
	piDEMDrec = 0;
	piDEMDdir = 0;
	nStructureSize = 0;
	pStructure = 0;
	piN2St = 0;
	pfL2St = 0;
	pfZ2St = 0;
	piChan_Side = 0;
	piChan_No = 0;
	piDdir = 0;
	pSegs = 0;
	pStructure = 0;
	nHillFileSize = 0;
	nSegSize=0;
	nProfSize=0;
	nTempFileSize=0;
	pProf = 0;
	pHill = 0;
	pChanStat = 0;
	nChanSize =0;
}

CWeppProcess::~CWeppProcess()
{

}

void CWeppProcess::RunProcess(double m_MissingData1, CString m_Directory1, CString m_FileName1,
							  int m_Size1, int m_Upslope1,CWnd* pWndNotifyProgress, 
							  CProgressCtrl* pProgressBar, CString sFullPathParam,
							  int m_MaxSegLength1,CPtrArray *pArray)
{
	
	
	pbStopProcess =(bool*) pArray->GetAt(10);
	m_MaxSegLength = m_MaxSegLength1;
	pProgressBar->SetRange(0,26);
	pProgressBar->SetStep(1);
	pProgressBar->SetPos(0);


	m_MissingData = m_MissingData1;
	m_Directory = m_Directory1;
	m_FileName = m_FileName1;
	m_Size  = m_Size1;
	m_Upslope = m_Upslope1;
	
	//int* piWeppShed;
	int* piShedNo;
	short int* piShedSide;
	int* piHillNo;
	int* piDEMSort;
	double* pdSortedDEM;
	int nLoop=0;
	int i=0;
	int* pTempSort = 0;
	int nIndex = 0;
	int nMissingNum =0;
	//m_ProgressCtrl.SetRange(0,10);
	//m_ProgressCtrl.SetStep(1);
	//////UpdateData(false);
	

	
	//Read dbf file with Elev, Drec, Ddir, and Upslope fields

	//////UpdateData(false);
	m_Message = "Reading dem file...";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long)& m_Message);

	ReadDbf();
	
	if(*pbStopProcess == true)
	{
		AfxEndThread(0);
	}

	pProgressBar->StepIt();


	
	//Sort based on elevation
	piDEMSort = new int[m_Row*m_Col];
	
	if(piDEMSort==NULL) 
	{
		AfxMessageBox("Not enough memory to create sorted index arra.");
		exit(0);

	}
	

	pdSortedDEM = new double[m_Row*m_Col];
	
	if(pdSortedDEM==NULL) 
	{
		AfxMessageBox("Not enough memory to create sorted array.");
		exit(0);

	}

	pTempSort = new int[m_Row*m_Col/2];
	
	if(pTempSort==NULL)
	{
		AfxMessageBox("Not enough memory to created temp array.");
		exit(0);
	}
	for (i = 0; i < m_Row * m_Col; i++)
	{
		if(pdElev[i]!=m_MissingData)//if value is not missing
		{
			pdSortedDEM[nIndex] = 10000000 - pdElev[i];
			piDEMSort[nIndex] = i;
			nIndex++;

		}
		else
		{
			pTempSort[nMissingNum] = i;
			nMissingNum++; //keep count of missing values

		}
	}

	




	QuickSort(pdSortedDEM, m_Row * m_Col-nMissingNum, piDEMSort,true); //sort only non missing values
	
	if(*pbStopProcess == true)
	{
		AfxEndThread(0);
	}



	nIndex = 0;
	for(i=m_Row*m_Col-nMissingNum;i<m_Row*m_Col;i++) //make the rest of pointers which are
		// pointing to the missing values, valid. 
		
	{
		piDEMSort[i] = pTempSort[nIndex];
		nIndex++;

	}
	
	
	//Removed June 10,2003
	/*
	bool bChanged = true;
	int nTemp=0;
	while(bChanged)
	{
		bChanged = false;
		for(nLoop=0;nLoop<m_Row*m_Col-1;nLoop++)
		{
			if(pdElev[piDEMSort[nLoop]]==pdElev[piDEMSort[nLoop+1]])
			{
				if(piDEMSort[nLoop]>piDEMSort[nLoop+1])
				{
					nTemp = piDEMSort[nLoop];
					piDEMSort[nLoop] = piDEMSort[nLoop+1];
					piDEMSort[nLoop+1]  = nTemp;
					bChanged = true;
				}
			}
		}
	}

	*/
	//make a backup of the original drec;
	if(*pbStopProcess == true)
	{
		AfxEndThread(0);
	}
	piDEMDrec = new int[m_Row*m_Col];
	for(nLoop=0;nLoop<m_Row*m_Col;nLoop++)
	{
		piDEMDrec[nLoop] = piDrec[nLoop];
	}


	piDEMDdir = new short int[m_Row*m_Col];

	for(nLoop=0;nLoop<m_Row*m_Col;nLoop++)
	{
		piDEMDdir[nLoop] = piDdir[nLoop];
	}


	piDEMShedNow = new int[m_Row*m_Col];
	for(nLoop=0;nLoop<m_Row*m_Col;nLoop++)
	{
		piDEMShedNow[nLoop] = piShedNow[nLoop];
	}

	//char full[_MAX_PATH];
	//_fullpath( full, "WeppMapr.exe", _MAX_PATH );
	//CString sPath = full;
	//int nLoc = sPath.Find("WeppMapr.exe");
	//sPath = sPath.Left(nLoc);

	
	std::ofstream fParamFile(sFullPathParam);

	fParamFile<<"Param.txt"<<'\n';

	fParamFile<<m_Directory <<'\n';
	
	fParamFile<<m_FileName<<'\n';
	
	fParamFile<<m_MissingData<<'\n';
	
	fParamFile<<m_Size <<'\n';
	
	fParamFile<<m_Upslope<<'\n';
	fParamFile<<m_MaxSegLength<<'\n';
	fParamFile.close();
	
	m_Message = "Running Mark_Chans...";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long)& m_Message);

	Mark_Chans(piDEMSort);
	pProgressBar->StepIt();
	
	//pdDEMElev = new double[m_Row*m_Col];
	//for(nLoop=0;nLoop<m_Row*m_Col;nLoop++)
	//{
	//	pdDEMElev[nLoop] = pdElev[nLoop];
	//}

	if(*pbStopProcess == true)
	{
		AfxEndThread(0);
	}
	m_Message = "Running Cut_Chans...";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long)& m_Message);
	Cut_Chan(piDEMSort);
	if(*pbStopProcess == true)
	{
		AfxEndThread(0);
	}
	pProgressBar->StepIt();	
	m_Message = "Running Merge_Chans...";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long)& m_Message);
	Merge_Chan(piDEMSort);
	if(*pbStopProcess == true)
	{
		AfxEndThread(0);
	}
	pProgressBar->StepIt();
	m_Message = "Running CalcUpArea...";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long)& m_Message);	
	CalcUpArea(piDEMSort);
	if(*pbStopProcess == true)
	{
		AfxEndThread(0);
	}
	pProgressBar->StepIt();	
	m_Message = "Running Remark_Chans...";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long)& m_Message);
	Remark_Chans(piDEMSort);
	if(*pbStopProcess == true)
	{
		AfxEndThread(0);
	}
	pProgressBar->StepIt();
	FILE* pFillFile = fopen(m_Directory + m_FileName + "fill.dbf", "r");
	
	if(pFillFile!=NULL)
	{
		fclose(pFillFile);
	
		Mark_Pits(piDEMSort);
	}
	if(*pbStopProcess == true)
	{
		AfxEndThread(0);
	}
	
	split_segs(piDEMSort);
	nIndex = 0;
	nMissingNum = 0;
	for (i = 0; i < m_Row * m_Col; i++)
	{
		if(pdElev[i]!=m_MissingData)//if value is not missing
		{
			pdSortedDEM[nIndex] = 10000000 - pdElev[i];
			piDEMSort[nIndex] = i;
			nIndex++;

		}
		else
		{
			pTempSort[nMissingNum] = i;
			nMissingNum++; //keep count of missing values

		}
	}

	QuickSort(pdSortedDEM, m_Row * m_Col-nMissingNum, piDEMSort,true); //sort only non missing values
	if(*pbStopProcess == true)
	{
		AfxEndThread(0);
	}
	nIndex = 0;
	for(i=m_Row*m_Col-nMissingNum;i<m_Row*m_Col;i++) //populated the rest of the sorted array
		
	{
		piDEMSort[i] = pTempSort[nIndex];
		nIndex++;

	}
	

	//Removed June 10,2003
	/*
	bChanged = true;
	while(bChanged)
	{
		bChanged = false;
		for(nLoop=0;nLoop<m_Row*m_Col-1;nLoop++)
		{
			if(pdElev[piDEMSort[nLoop]]!=m_MissingData)
			{
				if(pdElev[piDEMSort[nLoop]]==pdElev[piDEMSort[nLoop+1]])
				{
					if(piDEMSort[nLoop]>piDEMSort[nLoop+1])
					{
						nTemp = piDEMSort[nLoop];
						piDEMSort[nLoop] = piDEMSort[nLoop+1];
						piDEMSort[nLoop+1]  = nTemp;
						bChanged = true;
					}
				}
			}
		}
	}
	*/
	if(*pbStopProcess == true)
	{
		AfxEndThread(0);
	}
	m_Message = "Running Flow2Chan...";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long)& m_Message);
	Flow2Chan(piDEMSort);
	pProgressBar->StepIt();
	if(*pbStopProcess == true)
	{
		AfxEndThread(0);
	}

	m_Message = "Running Calc_Segs...";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long)& m_Message);	
	calc_segs(piDEMSort);
	pProgressBar->StepIt();

	if(*pbStopProcess == true)
	{
		AfxEndThread(0);
	}
	m_Message = "Running Order_Chans...";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long)& m_Message);
	Order_Segs(piDEMSort);
	pProgressBar->StepIt();
	if(*pbStopProcess == true)
	{
		AfxEndThread(0);
	}
	m_Message = "Running Redp_Ddir...";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long)& m_Message);
	Redo_Ddir();
	pProgressBar->StepIt();
	if(*pbStopProcess == true)
	{
		AfxEndThread(0);
	}
	m_Message = "Running Add_Ddir...";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long)& m_Message);
	Add_Ddir();
	pProgressBar->StepIt();

		if(*pbStopProcess == true)
	{
		AfxEndThread(0);
	}

	m_Message = "Running Find_UpSegs...";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long)& m_Message);
	Find_UpSegs();
	pProgressBar->StepIt();
		if(*pbStopProcess == true)
	{
		AfxEndThread(0);
	}
	//delete pdDEMElev;
	delete pTempSort;
	piShedNo = new int [m_Row* m_Col];
	piShedSide = new short int[m_Row*m_Col];
	piHillNo = new int[m_Row*m_Col];
	if(*pbStopProcess == true)
	{
		AfxEndThread(0);
	}
	m_Message = "Running Hill_Sheds...";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long)& m_Message);
	Hill_Sheds(piDEMSort, piShedNo, piShedSide, piHillNo);
	pProgressBar->StepIt();	

	if(*pbStopProcess == true)
	{
		AfxEndThread(0);
	}
	m_Message = "Running Renum_Segs...";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long)& m_Message);
	Renum_Segs(piShedNo);
	pProgressBar->StepIt();
	
	if(*pbStopProcess == true)
	{
		AfxEndThread(0);
	}
	m_Message = "Running Build_Stru...";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long)& m_Message);
	Build_Stru(piShedNo,piHillNo,piShedSide);
	pProgressBar->StepIt();
	if(*pbStopProcess == true)
	{
		AfxEndThread(0);
	}
	
	m_Message = "Running Wepp_Form...";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long)& m_Message);	
	Wepp_Form(piDEMSort);
	pProgressBar->StepIt();
	if(*pbStopProcess == true)
	{
		AfxEndThread(0);
	}
	m_Message = "Running Wepp_Len...";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long)& m_Message);
	Wepp_Len();
	pProgressBar->StepIt();
		if(*pbStopProcess == true)
	{
		AfxEndThread(0);
	}
	nMissingNum = 0;
	nIndex = 0;

	//pdDEMElev = new double[m_Row*m_Col];
	pTempSort = new int[m_Row*m_Col];

	for (i = 0; i < m_Row * m_Col; i++)
	{
		if(pdElev[i]!=m_MissingData)//if value is not missing
		{
			pdSortedDEM[nIndex] = piHillNo[i];
			piDEMSort[nIndex] = i;
			nIndex++;

		}
		else
		{
			pTempSort[nMissingNum] = i;
			nMissingNum++; //keep count of missing values

		}
	}

	QuickSort(pdSortedDEM, m_Row * m_Col-nMissingNum, piDEMSort,true); //sort only non missing values
	if(*pbStopProcess == true)
	{
		AfxEndThread(0);
	}
	delete pdSortedDEM;
	nIndex = 0;
	for(i=m_Row*m_Col-nMissingNum;i<m_Row*m_Col;i++) //populated the rest of the sorted array
		
	{
		piDEMSort[i] = pTempSort[nIndex];
		nIndex++;

	}
	delete pTempSort;
	if(*pbStopProcess == true)
	{
		AfxEndThread(0);
	}

	m_Message = "Running Hill_Stats...";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long)& m_Message);
	Hill_Stats(piHillNo, piDEMSort, piShedNo, piShedSide);
	pProgressBar->StepIt();

	if(*pbStopProcess == true)
	{
		AfxEndThread(0);
	}

	delete piDEMSort;

	m_Message = "Running Chan_Stats...";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long)& m_Message);
	Chan_Stats();
	pProgressBar->StepIt();

	if(*pbStopProcess == true)
	{
		AfxEndThread(0);
	}
	piDEMSort = new int[m_Row*m_Col];
	pTempSort = new int[m_Row*m_Col];
	pdSortedDEM = new double [m_Row*m_Col];

	//pTempSort = new int[m_Row*m_Col/2];
	
	for (i = 0; i < m_Row * m_Col; i++)
	{
		if(pdElev[i]!=m_MissingData)//if value is not missing
		{
			pdSortedDEM[nIndex] = 10000000 - pdElev[i];
			piDEMSort[nIndex] = i;
			nIndex++;
		}
		else
		{
			pTempSort[nMissingNum] = i;
			nMissingNum++; //keep count of missing values
		}
	}

	QuickSort(pdSortedDEM, m_Row * m_Col-nMissingNum, piDEMSort,true); //sort only non missing values
	if(*pbStopProcess == true)
	{
		AfxEndThread(0);
	}
	nIndex = 0;
	for(i=m_Row*m_Col-nMissingNum;i<m_Row*m_Col;i++) //make the rest of pointers which are
		// pointing to the missing values, valid. 
		
	{
		piDEMSort[i] = pTempSort[nIndex];
		nIndex++;

	}
	delete pTempSort;
	delete pdSortedDEM;

	m_Message = "Running CalcUpArea...";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long)& m_Message);
	CalcUpArea(piDEMSort);
	pProgressBar->StepIt();

	if(*pbStopProcess == true)
	{
		AfxEndThread(0);
	}

	delete piDEMSort;







	//m_ProgressCtrl.StepIt();
	//////UpdateData(false);
	CString strDbfName;
	CDbfFile* pOutDbfFile;
	char* chBuffer = new char[255];
	double dValue = 0;



//*****************************************

	//////UpdateData(false);


	if(*pbStopProcess == true)
	{
		AfxEndThread(0);
	}

	m_Message = "Saving " + m_Directory + m_FileName + "wepp.dbf"  + " file...";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long)& m_Message);

	CFile myFile(m_Directory + m_FileName + "wepp.dbf",CFile::modeCreate|CFile::modeWrite );
	char* szBuffer = new char[2555];

	//Type of file
	//3 represents fox pro.
	szBuffer[0]  = 3;
	myFile.Seek(0,CFile::begin);
	myFile.Write(szBuffer,1);
	
	//Last Updated(YYMMDD)
	//This can be changed to be dinamic very easy, just get todays date.
	myFile.Seek(1,CFile::begin);
	szBuffer[0] = 01;
	szBuffer[1] = 7;
	szBuffer[2] = 11;
	myFile.Write(szBuffer,3);

	CStringArray strArray;

	strArray.Add("SeqNo");
	strArray.Add("Row");
	strArray.Add("Col");
	strArray.Add("Elev");
	strArray.Add("Ddir");
	strArray.Add("Drec");
	strArray.Add("UpSlope");
	strArray.Add("SeedType");
	strArray.Add("Wepp_Shed");
	strArray.Add("Shed_No");
	strArray.Add("Shed_Side");

	strArray.Add("Hill_No");
	strArray.Add("Chan_No");
	strArray.Add("Chan_Side");
	strArray.Add("Segment_No");
	strArray.Add("Slope");
	strArray.Add("Aspect");
	strArray.Add("L2St");
	strArray.Add("N2St");
	strArray.Add("Z2St");


	int nColNum = 20;
	int nFldSize [20];
	nFldSize[0]=13;
	nFldSize[1]=4;
	nFldSize[2]=4;
	nFldSize[3]=11;
	nFldSize[4]=2;
	nFldSize[5]=13;
	nFldSize[6]=13;
	nFldSize[7]=2;
	nFldSize[8]=13;
	nFldSize[9]=13;
	nFldSize[10]=2;
	nFldSize[11]=13;

	nFldSize[12]=13;
	nFldSize[13]=2;
	nFldSize[14]=13;
	nFldSize[15]=9;
	nFldSize[16]=4;
	nFldSize[17]=7;
	nFldSize[18]=6;
	nFldSize[19]=9;





	CStringArray strFldType ;
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	

	//Number of records in the file.
	myFile.Seek(4,CFile::begin);
	int nNumberOfRecords = m_Row*m_Col;
	memcpy(szBuffer,&nNumberOfRecords,4);
	int nNum = 0;
	
	myFile.Write(szBuffer,4);


	//location of the first record.
	//this will be 32 bytes plus (number of fields * 32 bytes, because field subrecord is 32 bytes each) 
	//plus 1.

	nNum = 32+(32*nColNum)+1;
	memcpy(szBuffer,&nNum,2);
	myFile.Seek(8,CFile::begin);
	myFile.Write(szBuffer,2);
	

	if(*pbStopProcess == true)
	{
		AfxEndThread(0);
	}
	//lenght of the record. This is lenght sum of all the fields.
	int nRecordLen = 0;
	for(nLoop=0;nLoop<nColNum;nLoop++)
	{
		nRecordLen = nRecordLen + nFldSize[nLoop];
	}
	nRecordLen = nRecordLen + 1;
	memcpy(szBuffer,&nRecordLen,2);
	myFile.Seek(10,CFile::begin);
	myFile.Write(szBuffer,2);
	
	//Code page mark. Not sure what it is used for, but if not set, fox pro will ask for it
	myFile.Seek(29,CFile::begin);
	szBuffer[0] = 3;
	myFile.Write(szBuffer,1);
	
	//field subrecord
	//Description of each field in the table(type, lenght, decimals, etc.)
	myFile.Seek(32,CFile::begin);



	int j = 0;
	int nDisplacement = 0;
	int* nDec = new int [nColNum];
	
	for(nLoop=0;nLoop<nColNum;nLoop++)
	{
		nDec[nLoop]=0;
	}
	nDec[3] = 4;
	nDec[15] = 3;
	nDec[17] = 1;
	nDec[19] = 3;
	
	int nLenght = 0;
	//write column headers
	for(nLoop=0;nLoop<nColNum;nLoop++)
	{

		//Column Name
		myFile.Seek(32+32*nLoop,CFile::begin);
		strcpy(szBuffer,((CString)strArray.GetAt(nLoop)).GetBuffer(8));
		nLenght = strlen(szBuffer);
		for (int k= nLenght;k<10;k++)
		{
			szBuffer[k] = 0;
		}
		myFile.Write(szBuffer,10);
		//Data type
		myFile.Seek(32 + 32*nLoop + 11,CFile::begin);
		strcpy(szBuffer,((CString)strFldType.GetAt(nLoop)).GetBuffer(1));
		myFile.Write(szBuffer,1);
		
		nDisplacement = 0;
		for(j=0;j<nLoop;j++)
		{
			nDisplacement = nDisplacement + nFldSize[j];
		}
		//Field displacement in the record.
		memcpy(szBuffer,&nDisplacement,4);
		myFile.Seek(32 + 32*nLoop + 12 ,CFile::begin);

		myFile.Write(szBuffer,4);
		//Lenght of the field(don't forget the first delete byte flag.
		myFile.Seek(32 + 32*nLoop + 16,CFile::begin);
		memcpy(szBuffer,&nFldSize[nLoop],1);
		myFile.Write(szBuffer,1);
	
		myFile.Seek(32 + 32*nLoop + 17,CFile::begin);
		//Number of decimal places.
		memcpy(szBuffer,&nDec[nLoop],1);
		myFile.Write(szBuffer,1);

	}
	//Header Record Terminator value 13(0x0D)
	myFile.Seek(32+32*nLoop,CFile::begin);

	szBuffer[0] = 13;
	myFile.Write(szBuffer,1);

	//szBuffer[0] = 0;

	//myFile.Write(szBuffer,1);
	

	nIndex = 0;

	char* pszTemp = new char [255];

	int nLoop3 = 0;
	int nStart = 0;
	int nFinish = 0;
	int nCounter = 0;
	


	if(*pbStopProcess == true)
	{
		AfxEndThread(0);
	}


	for(j=0;j<m_Row*m_Col;j++)
	{

		szBuffer[0]=32;
		//strcpy(szBuffer," ");
		
		nCounter = 0;
		//SeqNo
		_itoa(j+1,pszTemp,10);

		for(nLoop=0;nLoop<255;nLoop++)
		{
			if(pszTemp[nLoop]==0)
			{
				nLenght = nLoop;
				nLoop = 255;
			}
		}

		nStart = 1;
		nLoop3=0;
		nFinish = nFldSize[0];
		for (nLoop = nStart;nLoop<nFinish+1;nLoop++)
		{
			if(nFinish - nLoop >= nLenght)
			{
				szBuffer[nLoop] = ' ';
			}
			else
			{
				szBuffer[nLoop] = pszTemp[nLoop3];
				nLoop3++;
			}
		}
		nFinish = nFinish + 1;
		//Row
		_itoa((j)/m_Col + 1,pszTemp,10);

		for(nLoop=0;nLoop<255;nLoop++)
		{
			if(pszTemp[nLoop]==0)
			{
				nLenght = nLoop;
				nLoop = 255;
			}
		}


		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for (nLoop = nStart;nLoop<nFinish;nLoop++)
		{
			if(nFinish - nLoop > nLenght)
			{
				szBuffer[nLoop] = ' ';
			}
			else
			{
				szBuffer[nLoop] = pszTemp[nLoop3];
				nLoop3++;
			}
		}
		
		
		//Col
		_itoa((j)%m_Col + 1,pszTemp,10);

		for(nLoop=0;nLoop<255;nLoop++)
		{
			if(pszTemp[nLoop]==0)
			{
				nLenght = nLoop;
				nLoop = 255;
			}
		}

		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for (nLoop = nStart;nLoop<nFinish;nLoop++)
		{
			if(nFinish - nLoop > nLenght)
			{
				szBuffer[nLoop] = ' ';
			}
			else
			{
				szBuffer[nLoop] = pszTemp[nLoop3];
				nLoop3++;
			}
		}

		
		//Elev

		sprintf(pszTemp,"%.4f",pdElev[j]);
		for(nLoop=0;nLoop<255;nLoop++)
		{
			if(pszTemp[nLoop]==0)
			{
				nLenght = nLoop;
				nLoop = 255;
			}
		}

		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for (nLoop = nStart;nLoop<nFinish;nLoop++)
		{
			if(nFinish - nLoop > nLenght)
			{
				szBuffer[nLoop] = ' ';
			}
			else
			{
				szBuffer[nLoop] = pszTemp[nLoop3];
				nLoop3++;
			}
		}

		
		//Ddir
		_itoa(piDdir[j],pszTemp,10);
		for(nLoop=0;nLoop<255;nLoop++)
		{
			if(pszTemp[nLoop]==0)
			{
				nLenght = nLoop;
				nLoop = 255;
			}
		}

		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for (nLoop = nStart;nLoop<nFinish;nLoop++)
		{
			if(nFinish - nLoop > nLenght)
			{
				szBuffer[nLoop] = ' ';
			}
			else
			{
				szBuffer[nLoop] = pszTemp[nLoop3];
				nLoop3++;
			}
		}


		//Drec
		_itoa(piDrec[j] ,pszTemp,10);
		for(nLoop=0;nLoop<255;nLoop++)
		{
			if(pszTemp[nLoop]==0)
			{
				nLenght = nLoop;
				nLoop = 255;
			}
		}

		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for (nLoop = nStart;nLoop<nFinish;nLoop++)
		{
			if(nFinish - nLoop > nLenght)
			{
				szBuffer[nLoop] = ' ';
			}
			else
			{
				szBuffer[nLoop] = pszTemp[nLoop3];
				nLoop3++;
			}
		}



		//UpSlope	
		_itoa(piUpSlope[j] ,pszTemp,10);
		for(nLoop=0;nLoop<255;nLoop++)
		{
			if(pszTemp[nLoop]==0)
			{
				nLenght = nLoop;
				nLoop = 255;
			}
		}

		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for (nLoop = nStart;nLoop<nFinish;nLoop++)
		{
			if(nFinish - nLoop > nLenght)
			{
				szBuffer[nLoop] = ' ';
			}
			else
			{
				szBuffer[nLoop] = pszTemp[nLoop3];
				nLoop3++;
			}
		}

		//SeedType
		_itoa(piSeedType[j] ,pszTemp,10);
		for(nLoop=0;nLoop<255;nLoop++)
		{
			if(pszTemp[nLoop]==0)
			{
				nLenght = nLoop;
				nLoop = 255;
			}
		}

		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for (nLoop = nStart;nLoop<nFinish;nLoop++)
		{
			if(nFinish - nLoop > nLenght)
			{
				szBuffer[nLoop] = ' ';
			}
			else
			{
				szBuffer[nLoop] = pszTemp[nLoop3];
				nLoop3++;
			}
		}

		//Wepp_Shed
		_itoa(0 ,pszTemp,10);
		nLenght = strlen(pszTemp);

		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for (nLoop = nStart;nLoop<nFinish;nLoop++)
		{
			if(nFinish - nLoop > nLenght)
			{
				szBuffer[nLoop] = ' ';
			}
			else
			{
				szBuffer[nLoop] = pszTemp[nLoop3];
				nLoop3++;
			}
		}


		//Shed_No
		_itoa(piShedNo[j],pszTemp,10);
		nLenght = strlen(pszTemp);

		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for (nLoop = nStart;nLoop<nFinish;nLoop++)
		{
			if(nFinish - nLoop > nLenght)
			{
				szBuffer[nLoop] = ' ';
			}
			else
			{
				szBuffer[nLoop] = pszTemp[nLoop3];
				nLoop3++;
			}
		}


		//Shed_Side	
		_itoa(piShedSide[j]  ,pszTemp,10);
		nLenght = strlen(pszTemp);

		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for (nLoop = nStart;nLoop<nFinish;nLoop++)
		{
			if(nFinish - nLoop > nLenght)
			{
				szBuffer[nLoop] = ' ';
			}
			else
			{
				szBuffer[nLoop] = pszTemp[nLoop3];
				nLoop3++;
			}
		}
		

		//Hill_No
		_itoa(piHillNo[j]  ,pszTemp,10);
		nLenght = strlen(pszTemp);

		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for (nLoop = nStart;nLoop<nFinish;nLoop++)
		{
			if(nFinish - nLoop > nLenght)
			{
				szBuffer[nLoop] = ' ';
			}
			else
			{
				szBuffer[nLoop] = pszTemp[nLoop3];
				nLoop3++;
			}
		}

		//Chan_No
		_itoa(piChan_No[j]  ,pszTemp,10);
		nLenght = strlen(pszTemp);

		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for (nLoop = nStart;nLoop<nFinish;nLoop++)
		{
			if(nFinish - nLoop > nLenght)
			{
				szBuffer[nLoop] = ' ';
			}
			else
			{
				szBuffer[nLoop] = pszTemp[nLoop3];
				nLoop3++;
			}
		}



		//Chan_Side
		_itoa(piChan_Side[j]  ,pszTemp,10);
		nLenght = strlen(pszTemp);

		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for (nLoop = nStart;nLoop<nFinish;nLoop++)
		{
			if(nFinish - nLoop > nLenght)
			{
				szBuffer[nLoop] = ' ';
			}
			else
			{
				szBuffer[nLoop] = pszTemp[nLoop3];
				nLoop3++;
			}
		}

		//Segment
		_itoa(piSegNo[j]  ,pszTemp,10);
		nLenght = strlen(pszTemp);

		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for (nLoop = nStart;nLoop<nFinish;nLoop++)
		{
			if(nFinish - nLoop > nLenght)
			{
				szBuffer[nLoop] = ' ';
			}
			else
			{
				szBuffer[nLoop] = pszTemp[nLoop3];
				nLoop3++;
			}
		}

		//Slope
		sprintf(pszTemp,"%.3f",pfSlope[j]);
		nLenght = strlen(pszTemp);

		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for (nLoop = nStart;nLoop<nFinish;nLoop++)
		{
			if(nFinish - nLoop > nLenght)
			{
				szBuffer[nLoop] = ' ';
			}
			else
			{
				szBuffer[nLoop] = pszTemp[nLoop3];
				nLoop3++;
			}
		}

		//Aspect
		_itoa(piAspect[j] ,pszTemp,10);
		nLenght = strlen(pszTemp);

		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for (nLoop = nStart;nLoop<nFinish;nLoop++)
		{
			if(nFinish - nLoop > nLenght)
			{
				szBuffer[nLoop] = ' ';
			}
			else
			{
				szBuffer[nLoop] = pszTemp[nLoop3];
				nLoop3++;
			}
		}

		//L2St
		sprintf(pszTemp, "%.1f",pfL2St[j]);
		nLenght = strlen(pszTemp);

		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for (nLoop = nStart;nLoop<nFinish;nLoop++)
		{
			if(nFinish - nLoop > nLenght)
			{
				szBuffer[nLoop] = ' ';
			}
			else
			{
				szBuffer[nLoop] = pszTemp[nLoop3];
				nLoop3++;
			}
		}

		//N2St
		//_gcvt(piN2St[j],5,pszTemp);
		sprintf(pszTemp, "%.0f",piN2St[j]);
		nLenght = strlen(pszTemp);

		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for (nLoop = nStart;nLoop<nFinish;nLoop++)
		{
			if(nFinish - nLoop > nLenght)
			{
				szBuffer[nLoop] = ' ';
			}
			else
			{
				szBuffer[nLoop] = pszTemp[nLoop3];
				nLoop3++;
			}
		}


		//Z2St
		sprintf(pszTemp, "%.3f",pfZ2St[j]);
		nLenght = strlen(pszTemp);

		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for (nLoop = nStart;nLoop<nFinish;nLoop++)
		{
			if(nFinish - nLoop > nLenght)
			{
				szBuffer[nLoop] = ' ';
			}
			else
			{
				szBuffer[nLoop] = pszTemp[nLoop3];
				nLoop3++;
			}
		}

		//szBuffer[nLoop] = 0;
		myFile.Write(szBuffer,nRecordLen );
	}
	szBuffer[0]=26;
	myFile.Write(szBuffer,1);
	myFile.Close();


//*******************************************



	pProgressBar->StepIt();




	if(*pbStopProcess == true)
	{
		AfxEndThread(0);
	}












/*
	clock_t start, finish;   
	double  duration;


	start =clock();


	m_Message= "Saving weppout.dbf...";



	

	strDbfName = m_FileName + "weppout";
	////UpdateData(false);
	

	pOutDbfFile->SetDBFColumn("SeqNo",12,0);
	pOutDbfFile->SetDBFColumn("Row",12,0);
	pOutDbfFile->SetDBFColumn("Col",12,0);
	pOutDbfFile->SetDBFColumn("Elev",12,4);
	pOutDbfFile->SetDBFColumn("Ddir",2,0);
	pOutDbfFile->SetDBFColumn("Drec",12,0);
	pOutDbfFile->SetDBFColumn("UpSlope",12,0);
	pOutDbfFile->SetDBFColumn("SeedType",1,0);
	pOutDbfFile->SetDBFColumn("Wepp_Shed",1,0);
	pOutDbfFile->SetDBFColumn("Shed_No",12,0);
	pOutDbfFile->SetDBFColumn("Shed_Side",1,0);
	pOutDbfFile->SetDBFColumn("Hill_No",12,0);
	pOutDbfFile->SetDBFColumn("Chan_No",12,0);
	pOutDbfFile->SetDBFColumn("Chan_Side",12,0);
	pOutDbfFile->SetDBFColumn("Segment_No",12,0);
	pOutDbfFile->SetDBFColumn("Slope",8,3);
	pOutDbfFile->SetDBFColumn("Aspect",3,0);
	pOutDbfFile->SetDBFColumn("L2ST",6,1);
	pOutDbfFile->SetDBFColumn("N2ST",5,0);
	pOutDbfFile->SetDBFColumn("Z2ST",8,3);
	


	pOutDbfFile->Initialize();

	
	for(nLoop=0;nLoop<m_Row*m_Col;nLoop++)
	{
		sprintf(chBuffer,"%i",nLoop+1);
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,0,(void*) & dValue);


		sprintf(chBuffer,"%i",nLoop/m_Col+1);
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,1,(void*) & dValue);
		
		sprintf(chBuffer,"%i",nLoop%m_Col+1);
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,2,(void*) & dValue);

		sprintf(chBuffer,"%.4f",pdElev[nLoop]);
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,3,(void*) & dValue);

		sprintf(chBuffer,"%i",piDdir[nLoop]);
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,4,(void*) & dValue);

		sprintf(chBuffer,"%i",piDrec[nLoop]);
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,5,(void*) & dValue);

		sprintf(chBuffer,"%i",piUpSlope[nLoop]);
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,6,(void*) & dValue);		

		sprintf(chBuffer,"%i",piSeedType[nLoop]);
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,7,(void*) & dValue);
		
		
		sprintf(chBuffer,"%i",piShedNo[nLoop]);
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,8,(void*) & chBuffer);

		sprintf(chBuffer,"%i",piShedNo[nLoop]);
		dValue = (int)atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,9,(void*) & dValue);

		sprintf(chBuffer,"%i",piShedSide[nLoop]);
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,10,(void*) & dValue);

		sprintf(chBuffer,"%i",piHillNo[nLoop]);
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,11,(void*) & dValue);
		
		
		sprintf(chBuffer,"%i",piChan_No[nLoop]);
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,12,(void*) & dValue);
		
		
		sprintf(chBuffer,"%i",piChan_Side[nLoop]);
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,13,(void*) & dValue);


		sprintf(chBuffer,"%i", piSegNo[nLoop]);
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,14,(void*)&dValue);

		
		sprintf(chBuffer,"%.3f",pfSlope[nLoop]);
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,15,(void*)&dValue);
		
		sprintf(chBuffer,"%i",piAspect[nLoop]);
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,16,(void*)&dValue);
		
		
		
		sprintf(chBuffer,"%.1f",pfL2St[nLoop]);
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,17,(void*)&dValue);


		sprintf(chBuffer,"%i",piN2St[nLoop]);
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,18,(void*)&dValue);

		sprintf(chBuffer,"%.3f",pfZ2St[nLoop]);
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,19,(void*)&dValue);
		
		
		
		
	}

	pOutDbfFile->Close();
	delete pOutDbfFile;
	finish = clock();

	duration = (double)(finish - start)/CLOCKS_PER_SEC;
	CString s;
	s.Format("%.2f seconds" ,duration);
	AfxMessageBox(s);


  */
	

	////UpdateData(false);

	strDbfName = m_Directory + m_FileName + "segs";
	m_Message = "Saving " + strDbfName  + " file...";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long)& m_Message);
	pOutDbfFile = new CDbfFile(strDbfName, OVERWRITE);

	pOutDbfFile->SetDBFColumn("Initial_Id",12,0);
	pOutDbfFile->SetDBFColumn("Final_Id",12,0);
	pOutDbfFile->SetDBFColumn("Sort_Order",12,0);
	pOutDbfFile->SetDBFColumn("Start_Type",1,0);
	pOutDbfFile->SetDBFColumn("Start_Cell",12,0);
	pOutDbfFile->SetDBFColumn("Start_Row",6,0);
	pOutDbfFile->SetDBFColumn("Start_Col",6,0);
	pOutDbfFile->SetDBFColumn("Start_Elev",10,4);
	pOutDbfFile->SetDBFColumn("Start_Ddir",1,0);
	pOutDbfFile->SetDBFColumn("End_Type",1,0);
	pOutDbfFile->SetDBFColumn("End_Cell",12,0);
	pOutDbfFile->SetDBFColumn("End_Row",6,0);
	pOutDbfFile->SetDBFColumn("End_Col",6,0);
	pOutDbfFile->SetDBFColumn("End_Elev",10,4);
	pOutDbfFile->SetDBFColumn("Len_Cells",12,0);
	pOutDbfFile->SetDBFColumn("Len_Meters",6,0);
	pOutDbfFile->SetDBFColumn("Width_m",12,0);
	pOutDbfFile->SetDBFColumn("Drain_Rec",12,0);
	pOutDbfFile->SetDBFColumn("Down_Seg",12,0);
	pOutDbfFile->SetDBFColumn("Left_Seg",12,0);
	pOutDbfFile->SetDBFColumn("Right_Seg",12,0);
	pOutDbfFile->SetDBFColumn("Center_Seg",12,0);
	pOutDbfFile->SetDBFColumn("Left_Rec",12,0);
	pOutDbfFile->SetDBFColumn("Right_Rec",12,0);
	pOutDbfFile->SetDBFColumn("Center_Rec",12,0);
	pOutDbfFile->SetDBFColumn("Left_Hill",12,0);
	pOutDbfFile->SetDBFColumn("Right_Hill",12,0);
	pOutDbfFile->SetDBFColumn("Top_Hill",12,0);
	pOutDbfFile->SetDBFColumn("Left_Imp",12,0);
	pOutDbfFile->SetDBFColumn("Right_Imp",12,0);
	pOutDbfFile->SetDBFColumn("Top_Imp",12,0);
	pOutDbfFile->SetDBFColumn("Chan_Shape",1,0);
	pOutDbfFile->SetDBFColumn("Flow2Crow",5,3);
	pOutDbfFile->SetDBFColumnLogical("Impound",1,0);
	
	
	pOutDbfFile->Initialize();


	for(nLoop=0;nLoop<nSegSize;nLoop++)
	{
		sprintf(chBuffer,"%i",pSegs[nLoop].Initial_Id);
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,0,(void*) & dValue);
		sprintf(chBuffer,"%i",pSegs[nLoop].Final_Id );
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,1,(void*) & dValue);
		sprintf(chBuffer,"%i",pSegs[nLoop].Sort_Order );
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,2,(void*) & dValue);
		sprintf(chBuffer,"%i",pSegs[nLoop].Start_Type);
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,3,(void*) & dValue);

		sprintf(chBuffer,"%i",pSegs[nLoop].Start_Cell);
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,4,(void*) & dValue);
		sprintf(chBuffer,"%i",pSegs[nLoop].Start_Row );
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,5,(void*) & dValue);
		sprintf(chBuffer,"%i",pSegs[nLoop].Start_Col);
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,6,(void*) & dValue);
		sprintf(chBuffer,"%.4f",pSegs[nLoop].Start_Elev);
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,7,(void*) & dValue);
		sprintf(chBuffer,"%i",pSegs[nLoop].Start_Ddir);
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,8,(void*) & dValue);
		sprintf(chBuffer,"%i",pSegs[nLoop].End_Type );
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,9,(void*) & dValue);
		sprintf(chBuffer,"%i",pSegs[nLoop].End_Cell);
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,10,(void*) & dValue);
		sprintf(chBuffer,"%i",pSegs[nLoop].End_Row);
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,11,(void*) & dValue);
		sprintf(chBuffer,"%i",pSegs[nLoop].End_Col );
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,12,(void*) & dValue);
		sprintf(chBuffer,"%.4f",pSegs[nLoop].End_Elev);
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,13,(void*) & dValue);


		sprintf(chBuffer,"%i",pSegs[nLoop].Len_Cells);
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,14,(void*) & dValue);
		sprintf(chBuffer,"%i",pSegs[nLoop].Len_Meters );
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,15,(void*) & dValue);
		sprintf(chBuffer,"%i",pSegs[nLoop].Width);
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,16,(void*) & dValue);
		sprintf(chBuffer,"%i",pSegs[nLoop].Drains_Rec);
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,17,(void*) & dValue);
		sprintf(chBuffer,"%i",pSegs[nLoop].Down_Seg);
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,18,(void*) & dValue);


		sprintf(chBuffer,"%i",pSegs[nLoop].Left_Seg);
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,19,(void*) & dValue);
		sprintf(chBuffer,"%i",pSegs[nLoop].Right_Seg );
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,20,(void*) & dValue);
		sprintf(chBuffer,"%i",pSegs[nLoop].Center_Seg);
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,21,(void*) & dValue);
		sprintf(chBuffer,"%i",pSegs[nLoop].Left_Rec);
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,22,(void*) & dValue);
		sprintf(chBuffer,"%i",pSegs[nLoop].Right_Rec);
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,23,(void*) & dValue);
		sprintf(chBuffer,"%i",pSegs[nLoop].Center_Rec);
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,24,(void*) & dValue);
		sprintf(chBuffer,"%i",pSegs[nLoop].Left_Hill);
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,25,(void*) & dValue);
		sprintf(chBuffer,"%i",pSegs[nLoop].Right_Hill);
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,26,(void*) & dValue);

		sprintf(chBuffer,"%i",pSegs[nLoop].Top_Hill);
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,27,(void*) & dValue);
		sprintf(chBuffer,"%i",pSegs[nLoop].Left_Imp);
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,28,(void*) & dValue);
		sprintf(chBuffer,"%i",pSegs[nLoop].Right_Imp);
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,29,(void*) & dValue);
		sprintf(chBuffer,"%i",pSegs[nLoop].Top_Imp);
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,30,(void*) & dValue);

	
		sprintf(chBuffer,"%i",pSegs[nLoop].Chan_Shape);
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,31,(void*) & dValue);




		
		sprintf(chBuffer,"%3f",pSegs[nLoop].Flow2Crow);
		dValue = atof(chBuffer);
		pOutDbfFile->WriteRecord(nLoop,32,(void*) & dValue);
		
		if(pSegs[nLoop].Impound ==true)
		{
			sprintf(chBuffer,"%s","T");
		}
		else
		{
			sprintf(chBuffer,"%s","F");
		}
		
		pOutDbfFile->WriteRecord(nLoop,33,chBuffer);


	}
	pOutDbfFile->Close();

	delete pOutDbfFile;
	char* pCodePage = new char[1];
	*pCodePage = 30;
	CFile* pFile = new CFile(strDbfName + ".dbf",CFile::modeWrite);
	pFile->Seek(29,CFile::begin);
	pFile->Write(pCodePage,1);
	pFile->Close();
	delete pFile;
	delete pCodePage;
	pProgressBar->StepIt();








//Below is code created by Igor Kezhis to write dbf file.
//Original code was writing dbf file with 500000 records in over a minute.
//This code does it in 25 seconds.

	
	if(*pbStopProcess == true)
	{
		AfxEndThread(0);
	}


	m_Message = "Saving" + m_Directory + m_FileName + "struc.dbf" + " file...";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long)& m_Message);
	////UpdateData(false);
	myFile.Open(m_Directory + m_FileName + "struc.dbf",CFile::modeCreate|CFile::modeWrite );
	//char* szBuffer = new char[2555];

	//Type of file
	//3 represents fox pro.
	szBuffer[0]  = 3;
	myFile.Seek(0,CFile::begin);
	myFile.Write(szBuffer,1);
	
	//Last Updated(YYMMDD)
	//This can be changed to be dinamic very easy, just get todays date.
	myFile.Seek(1,CFile::begin);
	szBuffer[0] = 01;
	szBuffer[1] = 7;
	szBuffer[2] = 11;
	myFile.Write(szBuffer,3);
	
	//Number of records in the file.
	myFile.Seek(4,CFile::begin);
	nNumberOfRecords = 0;
	memcpy(szBuffer,&nStructureSize,4);
	nNum = 0;
	
	myFile.Write(szBuffer,4);

	nNum = 32+(32*12)+1;
	memcpy(szBuffer,&nNum,2);

	//location of the first record.
	//this will be 32 bytes plus (number of fields * 32 bytes, because field subrecord is 32 bytes each) 
	//plus 1.
	myFile.Seek(8,CFile::begin);
	//_itoa(nNum,szBuffer,10);
	myFile.Write(szBuffer,2);
	
	nNum = 13*10+31+2 +1;
	memcpy(szBuffer,&nNum,2);

	//lenght of the record. This is lenght sum of all the fields.
	myFile.Seek(10,CFile::begin);
	myFile.Write(szBuffer,2);
	
	//Code page mark. Not sure what it is used for, but if not set, fox pro will ask for it
	myFile.Seek(29,CFile::begin);
	szBuffer[0] = 3;
	myFile.Write(szBuffer,1);
	
	//field subrecord
	//Description of each field in the table(type, lenght, decimals, etc.)
	myFile.Seek(32,CFile::begin);

	strArray.RemoveAll();
	strArray.Add("Element_No");
	strArray.Add("Ele_Type");
	strArray.Add("Left_Hill");
	strArray.Add("Right_Hill");
	strArray.Add("Top_Hill");
	strArray.Add("Left_Chan");
	strArray.Add("Right_Chan");
	strArray.Add("Top_Chan");
	strArray.Add("Left_Imp");
	strArray.Add("Right_Imp");
	strArray.Add("Top_Imp");
	strArray.Add("Comments");

	//int nFldSize [13];
	nFldSize[0]=13;
	nFldSize[1]=2;
	nFldSize[2]=13;
	nFldSize[3]=13;
	nFldSize[4]=13;
	nFldSize[5]=13;
	nFldSize[6]=13;
	nFldSize[7]=13;
	nFldSize[8]=13;
	nFldSize[9]=13;
	nFldSize[10]=13;
	nFldSize[11]=31;


	strFldType.RemoveAll();
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("C");

	j = 0;
	nDisplacement = 0;
	int nDecNum = 0;
	//write column headers
	for(nLoop=0;nLoop<12;nLoop++)
	{
		//Column Name
		myFile.Seek(32+32*nLoop,CFile::begin);
		strcpy(szBuffer,((CString)strArray.GetAt(nLoop)).GetBuffer(8));
		nLenght = strlen(szBuffer);
		for (int k= nLenght;k<10;k++)
		{
			szBuffer[k] = 0;
		}
		myFile.Write(szBuffer,10);
		//Data type
		myFile.Seek(32 + 32*nLoop + 11,CFile::begin);
		strcpy(szBuffer,((CString)strFldType.GetAt(nLoop)).GetBuffer(1));
		myFile.Write(szBuffer,1);
		
		nDisplacement = 0;
		for(j=0;j<nLoop;j++)
		{
			nDisplacement = nDisplacement + nFldSize[j];
		}
		//Field displacement in the record.
		memcpy(szBuffer,&nDisplacement,4);
		myFile.Seek(32 + 32*nLoop + 12 ,CFile::begin);

		myFile.Write(szBuffer,4);
		//Lenght of the field(don't forget the first delete byte flag.
		myFile.Seek(32 + 32*nLoop + 16,CFile::begin);
		memcpy(szBuffer,&nFldSize[nLoop],1);
		myFile.Write(szBuffer,1);
	
		myFile.Seek(32 + 32*nLoop + 17,CFile::begin);
		//Number of decimal places.
		memcpy(szBuffer,&nDecNum,1);
		myFile.Write(szBuffer,1);

	}
	//Header Record Terminator value 13(0x0D)
	myFile.Seek(32+32*nLoop,CFile::begin);

	szBuffer[0] = 13;
	myFile.Write(szBuffer,1);

	//szBuffer[0] = 0;

	//myFile.Write(szBuffer,1);
	

	nIndex = 0;

	//pszTemp = new char [255];
	nLenght = 0;
	nLoop3 = 0;
	nStart = 0;
	nFinish = 0;
	nCounter = 0;





	for(j=0;j<nStructureSize;j++)
	{
		szBuffer[0]=32;
		//strcpy(szBuffer," ");

		//myFile.Seek(1,CFile::current);
		nCounter = 0;
		_itoa(pStructure[j].Element_No,pszTemp,10);

		for(nLoop=0;nLoop<255;nLoop++)
		{
			if(pszTemp[nLoop]==0)
			{
				nLenght = nLoop;
				nLoop = 255;
			}
		}

		nStart = 1;
		nLoop3=0;
		nFinish = nFldSize[0];
		for (nLoop = nStart;nLoop<nFinish+1;nLoop++)
		{
			if(nFinish - nLoop >= nLenght)
			{
				szBuffer[nLoop] = ' ';
			}
			else
			{
				szBuffer[nLoop] = pszTemp[nLoop3];
				nLoop3++;
			}
		}
		nFinish = nFinish + 1;
		_itoa(pStructure[j].Ele_Type,pszTemp,10);

		for(nLoop=0;nLoop<255;nLoop++)
		{
			if(pszTemp[nLoop]==0)
			{
				nLenght = nLoop;
				nLoop = 255;
			}
		}

		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for (nLoop = nStart;nLoop<nFinish;nLoop++)
		{
			if(nFinish - nLoop > nLenght)
			{
				szBuffer[nLoop] = ' ';
			}
			else
			{
				szBuffer[nLoop] = pszTemp[nLoop3];
				nLoop3++;
			}
		}	
		_itoa(pStructure[j].Left_Hill,pszTemp,10);

		for(nLoop=0;nLoop<255;nLoop++)
		{
			if(pszTemp[nLoop]==0)
			{
				nLenght = nLoop;
				nLoop = 255;
			}
		}

		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for (nLoop = nStart;nLoop<nFinish;nLoop++)
		{
			if(nFinish - nLoop > nLenght)
			{
				szBuffer[nLoop] = ' ';
			}
			else
			{
				szBuffer[nLoop] = pszTemp[nLoop3];
				nLoop3++;
			}
		}

		_itoa(pStructure[j].Right_Hill,pszTemp,10);
		for(nLoop=0;nLoop<255;nLoop++)
		{
			if(pszTemp[nLoop]==0)
			{
				nLenght = nLoop;
				nLoop = 255;
			}
		}



		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for (nLoop = nStart;nLoop<nFinish;nLoop++)
		{
			if(nFinish - nLoop > nLenght)
			{
				szBuffer[nLoop] = ' ';
			}
			else
			{
				szBuffer[nLoop] = pszTemp[nLoop3];
				nLoop3++;
			}
		}


		_itoa(pStructure[j].Top_Hill,pszTemp,10);
		for(nLoop=0;nLoop<255;nLoop++)
		{
			if(pszTemp[nLoop]==0)
			{
				nLenght = nLoop;
				nLoop = 255;
			}
		}

		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for (nLoop = nStart;nLoop<nFinish;nLoop++)
		{
			if(nFinish - nLoop > nLenght)
			{
				szBuffer[nLoop] = ' ';
			}
			else
			{
				szBuffer[nLoop] = pszTemp[nLoop3];
				nLoop3++;
			}
		}


		_itoa(pStructure[j].Left_Chan ,pszTemp,10);
		for(nLoop=0;nLoop<255;nLoop++)
		{
			if(pszTemp[nLoop]==0)
			{
				nLenght = nLoop;
				nLoop = 255;
			}
		}

		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for (nLoop = nStart;nLoop<nFinish;nLoop++)
		{
			if(nFinish - nLoop > nLenght)
			{
				szBuffer[nLoop] = ' ';
			}
			else
			{
				szBuffer[nLoop] = pszTemp[nLoop3];
				nLoop3++;
			}
		}


			
		_itoa(pStructure[j].Right_Chan ,pszTemp,10);
		for(nLoop=0;nLoop<255;nLoop++)
		{
			if(pszTemp[nLoop]==0)
			{
				nLenght = nLoop;
				nLoop = 255;
			}
		}

		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for (nLoop = nStart;nLoop<nFinish;nLoop++)
		{
			if(nFinish - nLoop > nLenght)
			{
				szBuffer[nLoop] = ' ';
			}
			else
			{
				szBuffer[nLoop] = pszTemp[nLoop3];
				nLoop3++;
			}
		}

		_itoa(pStructure[j].Top_Chan ,pszTemp,10);
		for(nLoop=0;nLoop<255;nLoop++)
		{
			if(pszTemp[nLoop]==0)
			{
				nLenght = nLoop;
				nLoop = 255;
			}
		}

		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for (nLoop = nStart;nLoop<nFinish;nLoop++)
		{
			if(nFinish - nLoop > nLenght)
			{
				szBuffer[nLoop] = ' ';
			}
			else
			{
				szBuffer[nLoop] = pszTemp[nLoop3];
				nLoop3++;
			}
		}
		_itoa(pStructure[j].Left_Imp ,pszTemp,10);
		nLenght = strlen(pszTemp);

		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for (nLoop = nStart;nLoop<nFinish;nLoop++)
		{
			if(nFinish - nLoop > nLenght)
			{
				szBuffer[nLoop] = ' ';
			}
			else
			{
				szBuffer[nLoop] = pszTemp[nLoop3];
				nLoop3++;
			}
		}


		_itoa(pStructure[j].Right_Imp  ,pszTemp,10);
		nLenght = strlen(pszTemp);

		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for (nLoop = nStart;nLoop<nFinish;nLoop++)
		{
			if(nFinish - nLoop > nLenght)
			{
				szBuffer[nLoop] = ' ';
			}
			else
			{
				szBuffer[nLoop] = pszTemp[nLoop3];
				nLoop3++;
			}
		}

			
		_itoa(pStructure[j].Top_Imp  ,pszTemp,10);
		nLenght = strlen(pszTemp);

		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for (nLoop = nStart;nLoop<nFinish;nLoop++)
		{
			if(nFinish - nLoop > nLenght)
			{
				szBuffer[nLoop] = ' ';
			}
			else
			{
				szBuffer[nLoop] = pszTemp[nLoop3];
				nLoop3++;
			}
		}
		if(*pbStopProcess == true)
		{
			AfxEndThread(0);
		}

		strcpy(pszTemp,pStructure[j].Comment);
		nLenght = strlen(pszTemp);

		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for (nLoop = nLenght;nLoop< nFldSize[nCounter];nLoop++)
		{
			pszTemp[nLoop] = ' ';
		}
		for (nLoop = nStart;nLoop<nFinish;nLoop++)
		{
			szBuffer[nLoop] = pszTemp[nLoop3];
			nLoop3++;
		}
		nLenght = strlen(szBuffer);
		//szBuffer[nLoop] = 0;

		myFile.Write(szBuffer,31+ 13*10+2+1);


	}
	szBuffer[0]=26;
	myFile.Write(szBuffer,1);
	myFile.Close();
	pProgressBar->StepIt();

	

//*********Prof
	m_Message = "Saving " +  m_Directory + m_FileName + "prof.dbf file...";

	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long)& m_Message);

	////UpdateData(false);
	myFile.Open(m_Directory + m_FileName + "prof.dbf",CFile::modeCreate|CFile::modeWrite );
	//char* szBuffer = new char[2555];

	//Type of file
	//3 represents fox pro.
	char szbuff[255];
	szbuff[0] = 20;



	szBuffer[0]  = 3;
	myFile.Seek(0,CFile::begin);
	myFile.Write(szBuffer,1);
	
	//Last Updated(YYMMDD)
	//This can be changed to be dinamic very easy, just get todays date.
	myFile.Seek(1,CFile::begin);
	szBuffer[0] = 01;
	szBuffer[1] = 7;
	szBuffer[2] = 11;
	myFile.Write(szBuffer,3);

	strArray.RemoveAll();

	strArray.Add("Hill_No");
	strArray.Add("Distance");
	strArray.Add("Slope");
	strArray.Add("Aspect");
	strArray.Add("Rel_n2st");
	strArray.Add("Ofe_num");
	strArray.Add("Soil_id");
	strArray.Add("Manage_id");



	//int nColNum = 20;
	//int nFldSize [20];
	nFldSize[0]=13;
	nFldSize[1]=7;
	nFldSize[2]=12;
	nFldSize[3]=4;
	nFldSize[4]=7;
	nFldSize[5]=3;
	nFldSize[6]=9;
	nFldSize[7]=9;





	strFldType.RemoveAll();
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");


	

	//Number of records in the file.
	myFile.Seek(4,CFile::begin);
	//int nNumberOfRecords = m_Row*m_Col;
	memcpy(szBuffer,&nProfSize,4);
	nNum = 0;
	
	myFile.Write(szBuffer,4);


	//location of the first record.
	//this will be 32 bytes plus (number of fields * 32 bytes, because field subrecord is 32 bytes each) 
	//plus 1.

	nNum = 32+(32*8)+1;
	memcpy(szBuffer,&nNum,2);
	myFile.Seek(8,CFile::begin);
	myFile.Write(szBuffer,2);
	


	//lenght of the record. This is lenght sum of all the fields.
	nRecordLen = 0;
	for(nLoop=0;nLoop<8;nLoop++)
	{
		nRecordLen = nRecordLen + nFldSize[nLoop];
	}
	nRecordLen++;
	memcpy(szBuffer,&nRecordLen,2);
	myFile.Seek(10,CFile::begin);
	myFile.Write(szBuffer,2);
	
	//Code page mark. Not sure what it is used for, but if not set, fox pro will ask for it
	myFile.Seek(29,CFile::begin);
	szBuffer[0] = 3;
	myFile.Write(szBuffer,1);
	
	//field subrecord
	//Description of each field in the table(type, lenght, decimals, etc.)
	myFile.Seek(32,CFile::begin);



	j = 0;
	nDisplacement = 0;
	//int* nDec = new int [nColNum];
	
	for(nLoop=0;nLoop<8;nLoop++)
	{
		nDec[nLoop]=0;
	}
	nDec[0] = 0;
	nDec[1] = 2;
	nDec[2] = 3;
	nDec[3] = 0;
	nDec[4] = 2;
	nDec[5] = 0;
	nDec[6] = 0;
	nDec[7] = 0;



	//write column headers
	for(nLoop=0;nLoop<8;nLoop++)
	{

		//Column Name
		myFile.Seek(32+32*nLoop,CFile::begin);
		strcpy(szBuffer,((CString)strArray.GetAt(nLoop)).GetBuffer(8));
		myFile.Write(szBuffer,10);
		//Data type
		myFile.Seek(32 + 32*nLoop + 11,CFile::begin);
		strcpy(szBuffer,((CString)strFldType.GetAt(nLoop)).GetBuffer(1));
		myFile.Write(szBuffer,1);
		
		nDisplacement = 0;
		for(j=0;j<nLoop;j++)
		{
			nDisplacement = nDisplacement + nFldSize[j];
		}
		//Field displacement in the record.
		memcpy(szBuffer,&nDisplacement,4);
		myFile.Seek(32 + 32*nLoop + 12 ,CFile::begin);

		myFile.Write(szBuffer,4);
		//Lenght of the field(don't forget the first delete byte flag.
		myFile.Seek(32 + 32*nLoop + 16,CFile::begin);
		memcpy(szBuffer,&nFldSize[nLoop],1);
		myFile.Write(szBuffer,1);
	
		myFile.Seek(32 + 32*nLoop + 17,CFile::begin);
		//Number of decimal places.
		memcpy(szBuffer,&nDec[nLoop],1);
		myFile.Write(szBuffer,1);

	}
	//Header Record Terminator value 13(0x0D)
	myFile.Seek(32+32*nLoop,CFile::begin);

	szBuffer[0] = 13;
	myFile.Write(szBuffer,1);

	szBuffer[0] = 0;

	myFile.Write(szBuffer,1);
	

	nIndex = 0;

	//pszTemp = new char [255];
	nLenght = 0;
	nLoop3 = 0;
	nStart = 0;
	nFinish = 0;
	nCounter = 0;
	


	if(*pbStopProcess == true)
	{
		AfxEndThread(0);
	}


	for(j=0;j<nProfSize;j++)
	{

		szBuffer[0]='\0';

		
		nCounter = 0;
		//HillNo
		_itoa(pProf[j].Hill_No,pszTemp,10);

		for(nLoop=0;nLoop<255;nLoop++)
		{
			if(pszTemp[nLoop]==0)
			{
				nLenght = nLoop;
				nLoop = 255;
			}
		}
		for(nLoop=nLenght;nLoop<nFldSize[0];nLoop++)
		{
			pszTemp[nLoop] = ' ';
		}
		nStart = 0;
		nLoop3=0;
		nFinish = nFldSize[0];
		for(nLoop=nStart;nLoop<nFinish;nLoop++)
		{
			szBuffer[nLoop] = pszTemp[nLoop3];
			nLoop3++;
		}
		
		//Distance
		sprintf(pszTemp, "%.2f",pProf[j].Distance);

		for(nLoop=0;nLoop<255;nLoop++)
		{
			if(pszTemp[nLoop]==0)
			{
				nLenght = nLoop;
				nLoop = 255;
			}
		}
		for(nLoop=nLenght;nLoop<nFldSize[1];nLoop++)
		{
			pszTemp[nLoop]=' ';
		}

		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for(nLoop=nStart;nLoop<nFinish;nLoop++)
		{
			szBuffer[nLoop] = pszTemp[nLoop3];
			nLoop3++;
		}
		
		
		//Slope
		sprintf(pszTemp, "%.3f",pProf[j].Slope);

		for(nLoop=0;nLoop<255;nLoop++)
		{
			if(pszTemp[nLoop]==0)
			{
				nLenght = nLoop;
				nLoop = 255;
			}
		}
		for(nLoop=nLenght;nLoop<nFldSize[2];nLoop++)
		{
			pszTemp[nLoop] = ' ';
		}
		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for(nLoop=nStart;nLoop<nFinish;nLoop++)
		{
			szBuffer[nLoop] = pszTemp[nLoop3];
			nLoop3++;
		}

		
		//aspect

		_itoa(int(pProf[j].Aspect+0.5),pszTemp,10);
		for(nLoop=0;nLoop<255;nLoop++)
		{
			if(pszTemp[nLoop]==0)
			{
				nLenght = nLoop;
				nLoop = 255;
			}
		}
		for(nLoop=nLenght;nLoop<nFldSize[3];nLoop++)
		{
			pszTemp[nLoop] = ' ';
		}
		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for(nLoop=nStart;nLoop<nFinish;nLoop++)
		{
			szBuffer[nLoop] = pszTemp[nLoop3];
			nLoop3++;
		}

		
		//rel_n2st
		sprintf(pszTemp, "%.2f",pProf[j].Rel_n2St);
		for(nLoop=0;nLoop<255;nLoop++)
		{
			if(pszTemp[nLoop]==0)
			{
				nLenght = nLoop;
				nLoop = 255;
			}
		}
		for(nLoop=nLenght;nLoop<nFldSize[4];nLoop++)
		{
			pszTemp[nLoop]= ' ';
		}
		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for(nLoop=nStart;nLoop<nFinish;nLoop++)
		{
			szBuffer[nLoop] = pszTemp[nLoop3];
			nLoop3++;
		}


		//OFE_NUM
		//_itoa(pProf[j].Ofe_Num ,pszTemp,10);
		strcpy(pszTemp,"");
		for(nLoop=0;nLoop<255;nLoop++)
		{
			if(pszTemp[nLoop]==0)
			{
				nLenght = nLoop;
				nLoop = 255;
			}
		}
		for(nLoop=nLenght;nLoop<nFldSize[5];nLoop++)
		{
			pszTemp[nLoop] = ' ';
		}
		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for(nLoop=nStart;nLoop<nFinish;nLoop++)
		{
			szBuffer[nLoop] = pszTemp[nLoop3];
			nLoop3++;
		}



		//SOIL_ID 	
		//_itoa(pProf[j].Soil_Id ,pszTemp,10);
		strcpy(pszTemp,"");
		for(nLoop=0;nLoop<255;nLoop++)
		{
			if(pszTemp[nLoop]==0)
			{
				nLenght = nLoop;
				nLoop = 255;
			}
		}
		for(nLoop=nLenght;nLoop<nFldSize[6];nLoop++)
		{
			pszTemp[nLoop] = ' ';
		}
		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for(nLoop=nStart;nLoop<nFinish;nLoop++)
		{
			szBuffer[nLoop] = pszTemp[nLoop3];
			nLoop3++;
		}	

		//MANAGE_ID 
		//_itoa(pProf[j].Manage_Id ,pszTemp,10);
		strcpy(pszTemp,"");
		for(nLoop=0;nLoop<255;nLoop++)
		{
			if(pszTemp[nLoop]==0)
			{
				nLenght = nLoop;
				nLoop = 255;
			}
		}
		for(nLoop=nLenght;nLoop<nFldSize[7];nLoop++)
		{
			pszTemp[nLoop] = ' ';
		}
		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for(nLoop=nStart;nLoop<nFinish;nLoop++)
		{
			szBuffer[nLoop] = pszTemp[nLoop3];
			nLoop3++;
		}
		


		szBuffer[nLoop] = 0;

		//myFile.Write(szbuff,1);



		myFile.Write(szBuffer,nRecordLen);
	}
	szBuffer[0]=26;
	myFile.Write(szBuffer,1);
	myFile.Close();
	//**********************************************************
	pProgressBar->StepIt();


// saving hill.dbf
	//*******************************
	m_Message = "Saving " +  m_Directory + m_FileName + "hill.dbf file...";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long)& m_Message);

	if(*pbStopProcess == true)
	{
		AfxEndThread(0);
	}
	////UpdateData(false);
	myFile.Open(m_Directory + m_FileName + "hill.dbf",CFile::modeCreate|CFile::modeWrite );
	//char* szBuffer = new char[2555];

	//Type of file
	//3 represents fox pro.
	szBuffer[0]  = 3;
	myFile.Seek(0,CFile::begin);
	myFile.Write(szBuffer,1);
	
	//Last Updated(YYMMDD)
	//This can be changed to be dinamic very easy, just get todays date.
	myFile.Seek(1,CFile::begin);
	szBuffer[0] = 01;
	szBuffer[1] = 7;
	szBuffer[2] = 11;
	myFile.Write(szBuffer,3);

	strArray.RemoveAll();

	strArray.Add("Hill_No");
	strArray.Add("Hill_Width");
	strArray.Add("Hill_Area");
	strArray.Add("Max_Len");
	strArray.Add("Wepp_Len");
	strArray.Add("Num_Points");
	strArray.Add("Aspect");
	strArray.Add("Profile");




	//int nColNum = 20;
	//int nFldSize [20];
	nFldSize[0]=13;
	nFldSize[1]=13;
	nFldSize[2]=10;
	nFldSize[3]=13;
	nFldSize[4]=13;
	nFldSize[5]=4;
	nFldSize[6]=6;
	nFldSize[7]=251;





	strFldType.RemoveAll();
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("C");
	


	

	//Number of records in the file.
	myFile.Seek(4,CFile::begin);
	//int nNumberOfRecords = m_Row*m_Col;
	memcpy(szBuffer,&nHillFileSize,4);
	nNum = 0;
	
	myFile.Write(szBuffer,4);


	//location of the first record.
	//this will be 32 bytes plus (number of fields * 32 bytes, because field subrecord is 32 bytes each) 
	//plus 1.

	nNum = 32+(32*8)+1;
	memcpy(szBuffer,&nNum,2);
	myFile.Seek(8,CFile::begin);
	myFile.Write(szBuffer,2);
	


	//lenght of the record. This is lenght sum of all the fields.
	nRecordLen = 0;
	for(nLoop=0;nLoop<8;nLoop++)
	{
		nRecordLen = nRecordLen + nFldSize[nLoop];
	}

	nRecordLen++;
	memcpy(szBuffer,&nRecordLen,2);
	myFile.Seek(10,CFile::begin);
	myFile.Write(szBuffer,2);
	
	//Code page mark. Not sure what it is used for, but if not set, fox pro will ask for it
	myFile.Seek(29,CFile::begin);
	szBuffer[0] = 3;
	myFile.Write(szBuffer,1);
	
	//field subrecord
	//Description of each field in the table(type, lenght, decimals, etc.)
	myFile.Seek(32,CFile::begin);



	j = 0;
	nDisplacement = 0;
	//int* nDec = new int [nColNum];
	
	for(nLoop=0;nLoop<8;nLoop++)
	{
		nDec[nLoop]=0;
	}
	nDec[0] = 0;
	nDec[1] = 1;
	nDec[2] = 0;
	nDec[3] = 1;
	nDec[4] = 1;
	nDec[5] = 0;
	nDec[6] = 0;
	nDec[7] = 0;



	//write column headers
	for(nLoop=0;nLoop<8;nLoop++)
	{

		//Column Name
		myFile.Seek(32+32*nLoop,CFile::begin);
		strcpy(szBuffer,((CString)strArray.GetAt(nLoop)).GetBuffer(8));
		myFile.Write(szBuffer,10);
		//Data type
		myFile.Seek(32 + 32*nLoop + 11,CFile::begin);
		strcpy(szBuffer,((CString)strFldType.GetAt(nLoop)).GetBuffer(1));
		myFile.Write(szBuffer,1);
		
		nDisplacement = 0;
		for(j=0;j<nLoop;j++)
		{
			nDisplacement = nDisplacement + nFldSize[j];
		}
		//Field displacement in the record.
		memcpy(szBuffer,&nDisplacement,4);
		myFile.Seek(32 + 32*nLoop + 12 ,CFile::begin);

		myFile.Write(szBuffer,4);
		//Lenght of the field(don't forget the first delete byte flag.
		myFile.Seek(32 + 32*nLoop + 16,CFile::begin);
		memcpy(szBuffer,&nFldSize[nLoop],1);
		myFile.Write(szBuffer,1);
	
		myFile.Seek(32 + 32*nLoop + 17,CFile::begin);
		//Number of decimal places.
		memcpy(szBuffer,&nDec[nLoop],1);
		myFile.Write(szBuffer,1);

	}
	//Header Record Terminator value 13(0x0D)
	myFile.Seek(32+32*nLoop,CFile::begin);

	szBuffer[0] = 13;
	myFile.Write(szBuffer,1);

	szBuffer[0] = 0;

	myFile.Write(szBuffer,1);
	

	nIndex = 0;

	//pszTemp = new char [255];
	nLenght = 0;
	nLoop3 = 0;
	nStart = 0;
	nFinish = 0;
	nCounter = 0;
	
	if(*pbStopProcess == true)
	{
		AfxEndThread(0);
	}




	for(j=0;j<nHillFileSize;j++)
	{

		szBuffer[0]='\0';

		
		nCounter = 0;
		//HillNo
		_itoa(pHill[j].Hill_No,pszTemp,10);

		for(nLoop=0;nLoop<255;nLoop++)
		{
			if(pszTemp[nLoop]==0)
			{
				nLenght = nLoop;
				nLoop = 255;
			}
		}
		for(nLoop=nLenght;nLoop<nFldSize[0];nLoop++)
		{
			pszTemp[nLoop] = ' ';
		}
		nStart = 0;
		nLoop3=0;
		nFinish = nFldSize[0];
		for(nLoop=nStart;nLoop<nFinish;nLoop++)
		{
			szBuffer[nLoop] = pszTemp[nLoop3];
			nLoop3++;
		}
		
		//Hill_Width
		sprintf(pszTemp, "%.1f",pHill[j].Hill_Width);

		for(nLoop=0;nLoop<255;nLoop++)
		{
			if(pszTemp[nLoop]==0)
			{
				nLenght = nLoop;
				nLoop = 255;
			}
		}
		for(nLoop=nLenght;nLoop<nFldSize[1];nLoop++)
		{
			pszTemp[nLoop]=' ';
		}

		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for(nLoop=nStart;nLoop<nFinish;nLoop++)
		{
			szBuffer[nLoop] = pszTemp[nLoop3];
			nLoop3++;
		}
		
		
		//Hill_Area 
		sprintf(pszTemp, "%i",pHill[j].Hill_Area);

		for(nLoop=0;nLoop<255;nLoop++)
		{
			if(pszTemp[nLoop]==0)
			{
				nLenght = nLoop;
				nLoop = 255;
			}
		}
		for(nLoop=nLenght;nLoop<nFldSize[2];nLoop++)
		{
			pszTemp[nLoop] = ' ';
		}
		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for(nLoop=nStart;nLoop<nFinish;nLoop++)
		{
			szBuffer[nLoop] = pszTemp[nLoop3];
			nLoop3++;
		}

		
		//MaxLen

		sprintf(pszTemp,"%.1f", pHill[j].Max_Len);
		for(nLoop=0;nLoop<255;nLoop++)
		{
			if(pszTemp[nLoop]==0)
			{
				nLenght = nLoop;
				nLoop = 255;
			}
		}
		for(nLoop=nLenght;nLoop<nFldSize[3];nLoop++)
		{
			pszTemp[nLoop] = ' ';
		}
		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for(nLoop=nStart;nLoop<nFinish;nLoop++)
		{
			szBuffer[nLoop] = pszTemp[nLoop3];
			nLoop3++;
		}

		
		//wepp_len
		sprintf(pszTemp, "%.1f",pHill[j].Wepp_Len);
		for(nLoop=0;nLoop<255;nLoop++)
		{
			if(pszTemp[nLoop]==0)
			{
				nLenght = nLoop;
				nLoop = 255;
			}
		}
		for(nLoop=nLenght;nLoop<nFldSize[4];nLoop++)
		{
			pszTemp[nLoop]= ' ';
		}
		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for(nLoop=nStart;nLoop<nFinish;nLoop++)
		{
			szBuffer[nLoop] = pszTemp[nLoop3];
			nLoop3++;
		}


		//num_points
		_itoa(pHill[j].Num_Points ,pszTemp,10);
		for(nLoop=0;nLoop<255;nLoop++)
		{
			if(pszTemp[nLoop]==0)
			{
				nLenght = nLoop;
				nLoop = 255;
			}
		}
		for(nLoop=nLenght;nLoop<nFldSize[5];nLoop++)
		{
			pszTemp[nLoop] = ' ';
		}
		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for(nLoop=nStart;nLoop<nFinish;nLoop++)
		{
			szBuffer[nLoop] = pszTemp[nLoop3];
			nLoop3++;
		}



		//Aspect 	
		_itoa(pHill[j].Aspect ,pszTemp,10);
		for(nLoop=0;nLoop<255;nLoop++)
		{
			if(pszTemp[nLoop]==0)
			{
				nLenght = nLoop;
				nLoop = 255;
			}
		}
		for(nLoop=nLenght;nLoop<nFldSize[6];nLoop++)
		{
			pszTemp[nLoop] = ' ';
		}
		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for(nLoop=nStart;nLoop<nFinish;nLoop++)
		{
			szBuffer[nLoop] = pszTemp[nLoop3];
			nLoop3++;
		}	

		//PROFILE  
		strncpy(pszTemp, pHill[j].Profile,250);
		nLenght = strlen(pszTemp);
		//for(nLoop=0;nLoop<255;nLoop++)
		//{
		//	if(pszTemp[nLoop]==0)
		//	{
		//		nLenght = nLoop;
		//		nLoop = 255;
		//	}
		//}
		for(nLoop=nLenght;nLoop<nFldSize[7];nLoop++)
		{
			pszTemp[nLoop] = ' ';
		}
		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for(nLoop=nStart;nLoop<nFinish;nLoop++)
		{
			szBuffer[nLoop] = pszTemp[nLoop3];
			nLoop3++;
		}
		


		szBuffer[nLoop] = 0;
		myFile.Write(szBuffer,nRecordLen);
	}
	szBuffer[0]=26;
	myFile.Write(szBuffer,1);
	myFile.Close();
	//*******************************
	pProgressBar->StepIt();


	if(*pbStopProcess == true)
	{
		AfxEndThread(0);
	}

// saving hill.dbf
	//******************************
	m_Message = "Saving " + m_Directory + m_FileName + "chan.dbf file..";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long)& m_Message);


	////UpdateData(false);
	myFile.Open(m_Directory + m_FileName + "chan.dbf",CFile::modeCreate|CFile::modeWrite );
	//char* szBuffer = new char[2555];

	//Type of file
	//3 represents fox pro.
	szBuffer[0]  = 3;
	myFile.Seek(0,CFile::begin);
	myFile.Write(szBuffer,1);
	
	//Last Updated(YYMMDD)
	//This can be changed to be dinamic very easy, just get todays date.
	myFile.Seek(1,CFile::begin);
	szBuffer[0] = 01;
	szBuffer[1] = 7;
	szBuffer[2] = 11;
	myFile.Write(szBuffer,3);

	strArray.RemoveAll();

	strArray.Add("Chan_No");
	strArray.Add("Chan_Len");
	strArray.Add("Num_Points");
	strArray.Add("Mean_Slope");
	strArray.Add("Gen_Slope");
	strArray.Add("Aspect");
	strArray.Add("Profile");




	//int nColNum = 20;
	//int nFldSize [20];
	nFldSize[0]=13;
	nFldSize[1]=13;
	nFldSize[2]=5;
	nFldSize[3]=13;
	nFldSize[4]=13;
	nFldSize[5]=7;
	nFldSize[6]=251;





	strFldType.RemoveAll();
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("C");
	


	

	//Number of records in the file.
	myFile.Seek(4,CFile::begin);
	//int nNumberOfRecords = m_Row*m_Col;
	memcpy(szBuffer,&nChanSize,4);
	nNum = 0;
	
	myFile.Write(szBuffer,4);


	//location of the first record.
	//this will be 32 bytes plus (number of fields * 32 bytes, because field subrecord is 32 bytes each) 
	//plus 1.

	nNum = 32+(32*7)+1;
	memcpy(szBuffer,&nNum,2);
	myFile.Seek(8,CFile::begin);
	myFile.Write(szBuffer,2);
	


	//lenght of the record. This is lenght sum of all the fields.
	nRecordLen = 0;
	for(nLoop=0;nLoop<7;nLoop++)
	{
		nRecordLen = nRecordLen + nFldSize[nLoop];
	}
	nRecordLen++;
	memcpy(szBuffer,&nRecordLen,2);
	myFile.Seek(10,CFile::begin);
	myFile.Write(szBuffer,2);
	
	//Code page mark. Not sure what it is used for, but if not set, fox pro will ask for it
	myFile.Seek(29,CFile::begin);
	szBuffer[0] = 3;
	myFile.Write(szBuffer,1);
	
	//field subrecord
	//Description of each field in the table(type, lenght, decimals, etc.)
	myFile.Seek(32,CFile::begin);



	j = 0;
	nDisplacement = 0;
	//int* nDec = new int [nColNum];
	
	for(nLoop=0;nLoop<8;nLoop++)
	{
		nDec[nLoop]=0;
	}
	nDec[0] = 0;
	nDec[1] = 1;
	nDec[2] = 0;
	nDec[3] = 4;
	nDec[4] = 4;
	nDec[5] = 0;
	nDec[6] = 0;
	nDec[7] = 0;



	//write column headers
	for(nLoop=0;nLoop<7;nLoop++)
	{

		//Column Name
		myFile.Seek(32+32*nLoop,CFile::begin);
		strcpy(szBuffer,((CString)strArray.GetAt(nLoop)).GetBuffer(8));
		myFile.Write(szBuffer,10);
		//Data type
		myFile.Seek(32 + 32*nLoop + 11,CFile::begin);
		strcpy(szBuffer,((CString)strFldType.GetAt(nLoop)).GetBuffer(1));
		myFile.Write(szBuffer,1);
		
		nDisplacement = 0;
		for(j=0;j<nLoop;j++)
		{
			nDisplacement = nDisplacement + nFldSize[j];
		}
		//Field displacement in the record.
		memcpy(szBuffer,&nDisplacement,4);
		myFile.Seek(32 + 32*nLoop + 12 ,CFile::begin);

		myFile.Write(szBuffer,4);
		//Lenght of the field(don't forget the first delete byte flag.
		myFile.Seek(32 + 32*nLoop + 16,CFile::begin);
		memcpy(szBuffer,&nFldSize[nLoop],1);
		myFile.Write(szBuffer,1);
	
		myFile.Seek(32 + 32*nLoop + 17,CFile::begin);
		//Number of decimal places.
		memcpy(szBuffer,&nDec[nLoop],1);
		myFile.Write(szBuffer,1);

	}
	//Header Record Terminator value 13(0x0D)
	myFile.Seek(32+32*nLoop,CFile::begin);

	szBuffer[0] = 13;
	myFile.Write(szBuffer,1);

	szBuffer[0] = 0;

	myFile.Write(szBuffer,1);
	

	nIndex = 0;

	//pszTemp = new char [255];
	nLenght = 0;
	nLoop3 = 0;
	nStart = 0;
	nFinish = 0;
	nCounter = 0;
	





	for(j=0;j<nChanSize;j++)
	{

		szBuffer[0]='\0';

		
		nCounter = 0;
		//Chan_No
		_itoa(pChanStat[j].Chan_No,pszTemp,10);

		for(nLoop=0;nLoop<255;nLoop++)
		{
			if(pszTemp[nLoop]==0)
			{
				nLenght = nLoop;
				nLoop = 255;
			}
		}
		for(nLoop=nLenght;nLoop<nFldSize[0];nLoop++)
		{
			pszTemp[nLoop] = ' ';
		}
		nStart = 0;
		nLoop3=0;
		nFinish = nFldSize[0];
		for(nLoop=nStart;nLoop<nFinish;nLoop++)
		{
			szBuffer[nLoop] = pszTemp[nLoop3];
			nLoop3++;
		}
		
		//Chan_Len
		sprintf(pszTemp, "%.1f",pChanStat[j].Chan_Len);

		for(nLoop=0;nLoop<255;nLoop++)
		{
			if(pszTemp[nLoop]==0)
			{
				nLenght = nLoop;
				nLoop = 255;
			}
		}
		for(nLoop=nLenght;nLoop<nFldSize[1];nLoop++)
		{
			pszTemp[nLoop]=' ';
		}

		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for(nLoop=nStart;nLoop<nFinish;nLoop++)
		{
			szBuffer[nLoop] = pszTemp[nLoop3];
			nLoop3++;
		}
		
		
		//Num_Points 
		sprintf(pszTemp, "%i",pChanStat[j].Num_Points);

		for(nLoop=0;nLoop<255;nLoop++)
		{
			if(pszTemp[nLoop]==0)
			{
				nLenght = nLoop;
				nLoop = 255;
			}
		}
		for(nLoop=nLenght;nLoop<nFldSize[2];nLoop++)
		{
			pszTemp[nLoop] = ' ';
		}
		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for(nLoop=nStart;nLoop<nFinish;nLoop++)
		{
			szBuffer[nLoop] = pszTemp[nLoop3];
			nLoop3++;
		}

		
		//MeanSlope

		sprintf(pszTemp,"%8.4f", pChanStat[j].Mean_Slope);
		for(nLoop=0;nLoop<255;nLoop++)
		{
			if(pszTemp[nLoop]==0)
			{
				nLenght = nLoop;
				nLoop = 255;
			}
		}
		for(nLoop=nLenght;nLoop<nFldSize[3];nLoop++)
		{
			pszTemp[nLoop] = ' ';
		}
		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for(nLoop=nStart;nLoop<nFinish;nLoop++)
		{
			szBuffer[nLoop] = pszTemp[nLoop3];
			nLoop3++;
		}

		
		//Gen_Slope
		sprintf(pszTemp, "%8.4f",pChanStat[j].Gen_Slope);
		for(nLoop=0;nLoop<255;nLoop++)
		{
			if(pszTemp[nLoop]==0)
			{
				nLenght = nLoop;
				nLoop = 255;
			}
		}
		for(nLoop=nLenght;nLoop<nFldSize[4];nLoop++)
		{
			pszTemp[nLoop]= ' ';
		}
		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for(nLoop=nStart;nLoop<nFinish;nLoop++)
		{
			szBuffer[nLoop] = pszTemp[nLoop3];
			nLoop3++;
		}



		//Aspect 	
		_itoa(pChanStat[j].Aspect ,pszTemp,10);
		for(nLoop=0;nLoop<255;nLoop++)
		{
			if(pszTemp[nLoop]==0)
			{
				nLenght = nLoop;
				nLoop = 255;
			}
		}
		for(nLoop=nLenght;nLoop<nFldSize[6];nLoop++)
		{
			pszTemp[nLoop] = ' ';
		}
		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for(nLoop=nStart;nLoop<nFinish;nLoop++)
		{
			szBuffer[nLoop] = pszTemp[nLoop3];
			nLoop3++;
		}	

		//PROFILE  
		strncpy(pszTemp, pChanStat[j].Profile,250);
		nLenght = strlen(pszTemp);
		for(nLoop=nLenght;nLoop<nFldSize[7];nLoop++)
		{
			pszTemp[nLoop] = ' ';
		}
		nLoop3=0;
		nStart = nFinish;
		nFinish = nStart + nFldSize[nCounter +1];
		nCounter ++;
		for(nLoop=nStart;nLoop<nFinish;nLoop++)
		{
			szBuffer[nLoop] = pszTemp[nLoop3];
			nLoop3++;
		}
		


		szBuffer[nLoop] = 0;
		myFile.Write(szBuffer,nRecordLen);
	}
	szBuffer[0]=26;
	myFile.Write(szBuffer,1);
	myFile.Close();
	pProgressBar->StepIt();
	//*******************************
	delete pChanStat;
	//delete pdDEMElev;
	//delete pOutDbfFile;
	delete chBuffer;
	delete piChan_Side;

	
	delete pdElev;
	delete piDrec;
	delete piDdir;
	delete piUpSlope;
	delete piChan_No;
	delete piSeedType;
	delete piShedNow;
	delete piDEMShedNow;
	delete piDEMDdir;
	delete piDEMDrec;
	delete pSegs;
	delete piSegNo;
	delete pfZ2St;
	delete pfL2St;
	delete piN2St;
	delete piShedNo;
	delete pszTemp;
	delete szBuffer;
	delete piAspect;
	delete pfSlope;
	delete pStructure;
	delete piShedSide;
	delete piHillNo;
	delete nDec;
	delete pHill;
	delete pProf;
	delete pSTemp;
	//AfxMessageBox("Done");
	m_Message = "Calculation Complete!";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long)& m_Message);
}


BOOL CWeppProcess::ReadDbf()
{

	BOOL bValid = TRUE;

	char szBuffer [255];	

	CFile myFile(m_Directory + m_FileName + "DEM.dbf", CFile::modeRead );

	myFile.Seek(4,CFile::begin);
	myFile.Read(szBuffer,4);

	int* pShortInt = new int;
	*pShortInt = *(int*) &szBuffer;
	

	myFile.Seek(32,CFile::begin);
	int* nRowLocation = new int;
	*nRowLocation = -1;
	int* nColLocation = new int;
	*nColLocation = -1;
	int* nNumFields = new int;
	* nNumFields = 0;
	int* nElevLocation = new int;
	*nElevLocation = -1;
	int* nDdirLocation = new int;
	*nDdirLocation = -1;
	int* nDrecLocation = new int;
	*nDrecLocation = -1;
	int* nUpSlpLocation = new int;
	*nUpSlpLocation = -1;
	int* pnShedNowLocation = new int;
	*pnShedNowLocation = -1;
	

	int nFieldLength[35];

	for(int nLoop = 0;nLoop<100;nLoop++)
	{
		myFile.Read(szBuffer,32);
		if(szBuffer[0] !=13)
		{
			if(_stricmp(szBuffer,"row") == 0)
			{
				*nRowLocation = nLoop;
			}
			if(_stricmp(szBuffer,"col")==0)
			{
				*nColLocation = nLoop;
			}
			if(_stricmp(szBuffer,"elev")==0)
			{
				*nElevLocation = nLoop;
			}
			if(_stricmp(szBuffer,"Drec")==0)
			{
				*nDrecLocation = nLoop;
			}
			if(_stricmp(szBuffer,"Ddir")==0)
			{
				*nDdirLocation = nLoop;
			}
			if(_stricmp(szBuffer,"Upslope")==0)
			{
				*nUpSlpLocation = nLoop;
			}
			if(_stricmp(szBuffer,"ShedNow")==0)
			{
				*pnShedNowLocation = nLoop;
			}

			nFieldLength[nLoop] = szBuffer[16];
		}
		else
		{
			nLoop = 100;
		}
	}
	
	

	myFile.Seek(8,CFile::begin);
	myFile.Read(szBuffer,2);

	short int* piFirstRecordLoc = new short int;
	*piFirstRecordLoc = *(short int*)&szBuffer ;
	
	myFile.Seek(10,CFile::begin);
	myFile.Read(szBuffer,2);

	short int* piRecordLength = new short int;
	*piRecordLength = *(short int*) &szBuffer;

	
	myFile.Seek(*piFirstRecordLoc + 
		(*pShortInt -1) * (*piRecordLength),CFile::begin);
	myFile.Read(szBuffer,*piRecordLength);
	
	char szNumBuffer[8] = {'0','0','0','0','0','0','0','0'};
	
	int nBitesOfSet = 0;

	for(int nLoop=0;nLoop<*nColLocation;nLoop++)
	{
		nBitesOfSet = nBitesOfSet + nFieldLength[nLoop];
	}
	int nIndex =0;
	char* szP = new char[nFieldLength[*nColLocation]];
	for(int nLoop=0;nLoop<nFieldLength[*nColLocation];nLoop++)
	{

		szP[nLoop] = szBuffer[nBitesOfSet +1 +nLoop];
			
		

	}
	
	char szBuffTemp[8] = {'0','0','0','0','0','0','0','0'};

	for(int nLoop = 0;nLoop<8;nLoop++)
	{
		szBuffTemp[nLoop] = szNumBuffer[nLoop];

	}
	m_Col = (int)atof(szP);

	
	nIndex = 0;
	nBitesOfSet =0;
	for(int nLoop=0;nLoop<*nRowLocation;nLoop++)
	{
		nBitesOfSet = nBitesOfSet + nFieldLength[nLoop];
	}
	delete szP;
	szP = new char [nFieldLength[*nRowLocation]];
	for(int nLoop=0;nLoop<nFieldLength[*nRowLocation];nLoop++)
	{

		szP[nLoop] = szBuffer[nBitesOfSet +1 +nLoop];
			


	}
	
	for(int nLoop=0;nLoop<8;nLoop++)
	{
		szBuffTemp[nLoop] = '0';
	}

	for(int nLoop = 0;nLoop<8;nLoop++)
	{
		szBuffTemp[nLoop] = szNumBuffer[nLoop];

	}
	m_Row = (int)atof(szP);
	delete szP;




	//Read fields Elev, Drec, DDir, UpSLp from dbf table
	//***********************************************************
	double* pTemp = new double [m_Row*m_Col];
	int* pTemp1 = new int [m_Row*m_Col];
	short* pTemp2 = new short int [m_Row*m_Col];
	int* pTemp3= new int [m_Row*m_Col];
	piShedNow = new int[m_Row*m_Col];

	nBitesOfSet =0;
	int nUpSlpBitesOfSet = 0;
	int nDrecBitesOfSet = 0;
	int nDdirBitesOfSet= 0;
	int nShedNowBitesOfSet = 0;

	for(int nLoop=0;nLoop<*nElevLocation;nLoop++)
	{
		nBitesOfSet = nBitesOfSet + nFieldLength[nLoop];
	}
	for(int nLoop=0;nLoop<*nDrecLocation;nLoop++)
	{
		nDrecBitesOfSet = nDrecBitesOfSet + nFieldLength[nLoop];
	}	
	for(int nLoop=0;nLoop<*nDdirLocation;nLoop++)
	{
		nDdirBitesOfSet = nDdirBitesOfSet + nFieldLength[nLoop];
	}	
	for(int nLoop=0;nLoop<*nUpSlpLocation;nLoop++)
	{
		nUpSlpBitesOfSet = nUpSlpBitesOfSet + nFieldLength[nLoop];
	}		
	for(int nLoop=0;nLoop<*pnShedNowLocation;nLoop++)
	{
		nShedNowBitesOfSet = nShedNowBitesOfSet + nFieldLength[nLoop];
	}

	
	myFile.Seek(*piFirstRecordLoc,CFile::begin);
	
	char* szElevBuffer = new char [255];
	char* szDrecBuffer = new char [255];
	char* szDdirBuffer = new char [255];
	char* szUpSlpBuffer = new char [255];
	
	char* szShedNowBuffer = new char[255];

	for(int nLoop=0;nLoop<m_Row*m_Col;nLoop++)
	{

		myFile.Read(szBuffer,*piRecordLength);

		for(nIndex=0;nIndex<nFieldLength[*nElevLocation];nIndex++)
		{
			szElevBuffer[nIndex] = szBuffer[nBitesOfSet +1 +nIndex];

		}
		szElevBuffer[nIndex] = 0;
		pTemp[nLoop] = atof(szElevBuffer);

		for(nIndex=0;nIndex<nFieldLength[*nDrecLocation];nIndex++)
		{
			szDrecBuffer[nIndex] = szBuffer[nDrecBitesOfSet +1 +nIndex];

		}
		szDrecBuffer[nIndex] = 0;
		pTemp1[nLoop] = atoi(szDrecBuffer);

		for(nIndex=0;nIndex<nFieldLength[*nDdirLocation];nIndex++)
		{
			szDdirBuffer[nIndex] = szBuffer[nDdirBitesOfSet +1 +nIndex];

		}
		
		szDdirBuffer[nIndex] = 0;
		pTemp2[nLoop] = atoi(szDdirBuffer);

		for(nIndex=0;nIndex<nFieldLength[*nUpSlpLocation];nIndex++)
		{
			szUpSlpBuffer[nIndex] = szBuffer[nUpSlpBitesOfSet +1 +nIndex];

		}
		szUpSlpBuffer[nIndex] = 0;
		pTemp3[nLoop] = atoi(szUpSlpBuffer);

		for(nIndex=0;nIndex<nFieldLength[*pnShedNowLocation];nIndex++)
		{
			szShedNowBuffer[nIndex] = szBuffer[nShedNowBitesOfSet +1 +nIndex];

		}
		szShedNowBuffer[nIndex] = 0;
		piShedNow[nLoop] = atoi(szShedNowBuffer);

	}
	

	pdElev = pTemp;
	piDrec = pTemp1;
	piDdir = pTemp2;


	piUpSlope = pTemp3;


	myFile.Close();
	
	piSeedType = new short int[m_Row*m_Col];

	if(piSeedType ==NULL) 
	{
		AfxMessageBox("Not enough memory for seed type array.");
		exit(0);
	}

	piChan_No = new int [m_Row*m_Col];

	if(piChan_No ==NULL)
	{
		AfxMessageBox("Not enough memory for Chan_No array.");
		exit(0);
	}


	delete nUpSlpLocation;
	delete nDrecLocation;
	delete nDdirLocation;
	delete szUpSlpBuffer;
	delete szDdirBuffer;
	delete szDrecBuffer;
	delete piRecordLength;
	delete nRowLocation;
	delete nColLocation;
	delete pShortInt;
	delete piFirstRecordLoc;
	delete nElevLocation;
	delete nNumFields;
	delete szElevBuffer;
	delete szShedNowBuffer;
	delete pnShedNowLocation;
	return bValid;
}

BOOL CWeppProcess::Mark_Chans(int* pi)
{
	int Ups_Diff = 0;
	int chan_len = 0;
	int last_ups=0;

	BOOL bSuccess = true;;

	bool bEOF = false;
	bool go_dn = false;
	int nLoop = 0;
	int start_rec = 0;
	bool next2channel = false;
	int numneigh = 0;
	int threshold = m_Upslope;
	int nRecordPointer = 0;

	nCurrentRecord = 0;



	int cent_y =0;
	int cent_x = 0;
	double cent_z = 0;
	int cent_rec = 0;
	int zrec[8] ;
	



	int nIndex = m_Col*m_Row;

	
	for(nLoop=0;nLoop<m_Row*m_Col;nLoop++)
	{
		piSeedType[nLoop] = 0;
		piChan_No[nLoop] = 0;
	}
	

	int curr_chan = 0;


	m_Message = "Running procedure mark_chan...";
	//m_ProgressCtrl.StepIt();

	////UpdateData(false);
	int nTemp =0;

	while(bEOF ==false)
	{		

		//check if next record has the same elev. and points to current record. 
		//if so, it should be first in order, not second
		
		if( nIndex -2 >nRecordPointer)//make sure we not on the last record
		{		
			if(pdElev[pi[nRecordPointer]] == pdElev[pi[nRecordPointer+1]])//if elevation the same
			{
				if(piDrec[pi[nRecordPointer+1]]-1 == pi[nRecordPointer]) //if next record points into
					//the current record, then we flip them.
				{
					nTemp = pi[nRecordPointer];
					pi[nRecordPointer] = pi[nRecordPointer+1];
					pi[nRecordPointer+1] = nTemp;

	
				}
			}
		}

		
		start_rec = nRecordPointer;

		go_dn = false;
		nCurrentRecord = pi[nRecordPointer];

		if(piUpSlope[nCurrentRecord]>threshold)
		{
			next2channel = false;
			//Get Neigh function
			//**************************************************
			cent_y = nCurrentRecord / m_Col;
			cent_x = nCurrentRecord % m_Col;
			cent_z = pdElev[nCurrentRecord];
			cent_rec = nCurrentRecord;
			numneigh = 0;
			if(cent_x >0 && cent_y >0)
			{
				
				zrec[numneigh] = cent_rec - (m_Col + 1);
				numneigh = numneigh + 1;
			}
			if(cent_y >0)
			{
				zrec[numneigh] = cent_rec - m_Col;
				numneigh = numneigh + 1;
			}
			if(cent_y >0 && cent_x <m_Col -1)
			{
				zrec[numneigh] = cent_rec - (m_Col -1);
				numneigh = numneigh + 1;
			}
			if(cent_x >0)
			{
				zrec[numneigh] = cent_rec -1;
				numneigh = numneigh + 1;
			}
			if(cent_x <m_Col-1)
			{
				zrec[numneigh] = cent_rec + 1;
				numneigh = numneigh + 1;				
			}

			if(cent_x >0 && cent_y <(m_Row-1))
			{
				zrec[numneigh] = cent_rec + (m_Col -1);
				numneigh = numneigh + 1;
			}
			if(cent_y <m_Row -1)
			{
				zrec[numneigh] = cent_rec + m_Col;
				numneigh = numneigh + 1;
			}
			if(cent_x <m_Col-1 && cent_y <(m_Row-1))
			{
				zrec[numneigh] = cent_rec  + m_Col +1;
				numneigh = numneigh + 1;
			}
			

			for(nLoop = 0;nLoop<numneigh;nLoop++)
			{
				if(pdElev[zrec[nLoop]]!=m_MissingData)
				{
					if(piChan_No[zrec[nLoop]]>0)
					{
						next2channel = true;
						nLoop = numneigh;
					}
				}
			}



			//***************************************************
			//Check4Chan();
			if (next2channel ==false)
			{
				go_dn = true;
				Ups_Diff = 0;
				chan_len = 1;
				curr_chan = curr_chan + 1;
			}

		}
		else
		{
			go_dn = false;
		}

		
		while((piDdir[nCurrentRecord]!=5) &&  (go_dn == true))
		{

			
			if( piChan_No[nCurrentRecord]<1)
			{
	
				if(chan_len == 1)
				{



					piSeedType[nCurrentRecord] = 1;
					piChan_No[nCurrentRecord] = curr_chan;

				}
				else
				{
					if(Ups_Diff>threshold)
					{




						piSeedType[nCurrentRecord] = 2;
						piChan_No[nCurrentRecord] = curr_chan;
					}
					else
					{
						if(piDdir[nCurrentRecord] ==5)
						{




							piSeedType[nCurrentRecord] = 5;
							piChan_No[nCurrentRecord] = curr_chan;
						}
						else
						{
							piChan_No[nCurrentRecord] = curr_chan;
						}
					}
				}


				last_ups = piUpSlope[nCurrentRecord];
				
				nCurrentRecord = piDrec[nCurrentRecord] -1;
				
				Ups_Diff = piUpSlope[nCurrentRecord] - last_ups;
				chan_len = chan_len + 1;
			}
			else
			{
				go_dn = false;

			}

			

		}

		if(piDdir[nCurrentRecord]==5)
		{
			piChan_No[nCurrentRecord] = curr_chan;
			piSeedType[nCurrentRecord] = 5;
		}
		nRecordPointer = start_rec;
		nRecordPointer ++;
		
		if(nRecordPointer>=nIndex)
		{
			bEOF = true;
		}
		else
		{
			nCurrentRecord = pi[nRecordPointer];
		}

	}
	
	//delete pi;
	return bSuccess;
}






void CWeppProcess::Merge_Chan(int *pi)
{
	int nLoop = 0;
	
	bool bExit = false;
	int cent_y =0;
	int cent_x = 0;
	double cent_z = 0;
	int cent_rec = 0;
	int zrec[8] ;
	int cent_chan =0;
	int cent_ddir = 0;
	int cent_shed =0;
	int cent_drec=0;
	double cent_elev=0;
	int numneigh = 0;
	nCurrentRecord = 0;
	int nSortedRecordPointer = m_Row*m_Col-1;

	double dElev = 0;


	
	int nLastRec = 0;
	m_Message = "Running Merge_Chan...";
	//m_ProgressCtrl.StepIt();
	////UpdateData(false);
	while(bExit ==false)
	{

		nLastRec = nSortedRecordPointer;
		nCurrentRecord = pi[nSortedRecordPointer];

		if(piChan_No[nCurrentRecord]>0)
		{

			cent_elev = pdElev[nCurrentRecord];
			cent_chan = piChan_No[nCurrentRecord];
			cent_ddir = piDEMDdir[nCurrentRecord];
			cent_shed = piDEMShedNow[nCurrentRecord];
			cent_drec = piDrec[nCurrentRecord];

			cent_y = nCurrentRecord / m_Col;
			cent_x = nCurrentRecord % m_Col;
			cent_z = pdElev[nCurrentRecord];
			cent_rec = nCurrentRecord;
			numneigh = 0;
			if(cent_x >0 && cent_y >0)
			{
				
				zrec[numneigh] = cent_rec - (m_Col + 1);
				numneigh = numneigh + 1;
			}
			if(cent_y >0)
			{
				zrec[numneigh] = cent_rec - m_Col;
				numneigh = numneigh + 1;
			}
			if(cent_y >0 && cent_x <m_Col -1)
			{
				zrec[numneigh] = cent_rec - (m_Col -1);
				numneigh = numneigh + 1;
			}
			if(cent_x >0)
			{
				zrec[numneigh] = cent_rec -1;
				numneigh = numneigh + 1;
			}
			if(cent_x <m_Col-1)
			{
				zrec[numneigh] = cent_rec + 1;
				numneigh = numneigh + 1;				
			}

			if(cent_x >0 && cent_y <(m_Row-1))
			{
				zrec[numneigh] = cent_rec + (m_Col -1);
				numneigh = numneigh + 1;
			}
			if(cent_y <m_Row -1)
			{
				zrec[numneigh] = cent_rec + m_Col;
				numneigh = numneigh + 1;
			}
			if(cent_x <m_Col-1 && cent_y <(m_Row-1))
			{
				zrec[numneigh] = cent_rec  + m_Col +1;
				numneigh = numneigh + 1;
			}
		
			for(int nNeighbor=0;nNeighbor<numneigh;nNeighbor++)
			{
				nCurrentRecord  = zrec[nNeighbor];
				if(pdElev[nCurrentRecord]!=m_MissingData)
				{
					if(piDEMShedNow[nCurrentRecord] ==cent_shed)
					{
						if(piChan_No[nCurrentRecord]>0 && piChan_No[nCurrentRecord]!=cent_chan)
						{
							if(piDrec[nCurrentRecord]-1!=cent_rec && cent_drec-1 != nCurrentRecord)
							{
								if(pdElev[nCurrentRecord]>=cent_elev)
								{
									if(piDEMDdir[nCurrentRecord]==cent_ddir)
									{
										piDrec[nCurrentRecord] = cent_rec+1;
										piChan_No[nCurrentRecord] = 0;
										piSeedType[nCurrentRecord] = 0;
									}
								}
							}
						}
					}
				}

			}

			for(int nNeighbor=0;nNeighbor<numneigh;nNeighbor++)
			{
				nCurrentRecord = zrec[nNeighbor];
				if ((pdElev[nCurrentRecord]!=m_MissingData) && (piChan_No[nCurrentRecord] ==0))
				{
					if(piDEMDrec[nCurrentRecord]-1==piDrec[nCurrentRecord]-1)
					{
						if(piDEMShedNow[nCurrentRecord]==cent_shed)
						{
							if((piDrec[nCurrentRecord]-1) !=cent_rec && cent_drec -1 !=nCurrentRecord)
							{
								if(pdElev[nCurrentRecord]>=cent_elev )
								{
									piDrec[nCurrentRecord] = cent_rec +1;
								}
							}
						}
					}

				}
			}

		
		}



		nSortedRecordPointer = nLastRec -1;
		if(nSortedRecordPointer<0)
		{
			bExit = true;
		}

	}

	//delete pi;
}

void CWeppProcess::Cut_Chan(int *piSortedIndex)
{

	int nLoop = 0;
	int path_len =0;
	double start_elev = 0;
	int start_rec  = 0;
	double end_elev = 0;
	int end_rec = 0;
	double elev_diff = 0.0;
	double delta_z = 0;
	int last_start = 0;
	nCurrentRecord  = 0;
	int nSortedRecordPointer = 0;
	int nLast_Chan = 0;
	
	bool bExit = false;
	double new_elev =0;
	bool bNext = false;
	
	m_Message = "Running procedure Cut_Chan...";
	//m_ProgressCtrl.StepIt();
	////UpdateData(false);


	int nCount =0;
	int nCount2 = 0;
	for(nLoop =0;nLoop<m_Row*m_Col;nLoop++)
	{

		if(piSeedType[piSortedIndex[nLoop]]==1)
		{
			nCurrentRecord = piSortedIndex[nLoop];
			nSortedRecordPointer = nLoop;
			nLast_Chan = nCurrentRecord;
			start_elev = pdElev[nCurrentRecord];
			start_rec = nCurrentRecord;
			
			bNext = false;
			while(piDdir[nCurrentRecord]!=5 && bNext ==false)
			{
				nCurrentRecord = piDrec[nCurrentRecord]-1;
				if(piSeedType[nCurrentRecord]==1)
				{
					piSeedType[nCurrentRecord] = 0;
				}
				if(pdElev[nCurrentRecord]<=start_elev)
				{
					start_elev = pdElev[nCurrentRecord];
					start_rec = nCurrentRecord;
				}
				else
				{
					path_len = 0;
					bExit = false;
		
					while(pdElev[nCurrentRecord]>start_elev && bExit ==false)
					{
						path_len = path_len + 1;
						if(piDdir[nCurrentRecord]!=5)
						{
							nCurrentRecord = piDrec[nCurrentRecord]-1;
							if(piSeedType[nCurrentRecord]==1)
							{
								piSeedType[nCurrentRecord] = 0;
							}
						}
						else
						{
							bExit = true;
						}
					}


					end_elev = pdElev[nCurrentRecord];
					end_rec = nCurrentRecord;
					elev_diff = start_elev - end_elev;
					delta_z = elev_diff/path_len;

					nCurrentRecord = start_rec;

					for(int n=1;n<path_len+1;n++)
					{
						nCurrentRecord = piDrec[nCurrentRecord]-1;
						pdElev[nCurrentRecord] = start_elev - (n*delta_z);
					}
					nCurrentRecord = end_rec;
				}
			}
			nCurrentRecord = nLast_Chan;
			bNext = true;
		}
	}
}

void CWeppProcess::CalcUpArea(int *piSortedIndex)
{
	nCurrentRecord=0;
	int nSortedRecordPointer =0;
	int nLoop =0;
	int start_rec = 0;
	int new_ups = 0;
	bool go_dn = false;
	

	m_Message = "Running New_Ups..";
	//m_ProgressCtrl.StepIt();
	////UpdateData(false);

	for(nLoop=0;nLoop<m_Row*m_Col;nLoop++)
	{
		piUpSlope[nLoop] = 1;
	}



	for(nLoop=0;nLoop<m_Row*m_Col;nLoop++)
	{
		nCurrentRecord = nLoop;

		nSortedRecordPointer = nLoop;

		if(pdElev[nLoop]!=m_MissingData)
		{
			start_rec = nCurrentRecord;
			new_ups = 0;
			if(piUpSlope[nCurrentRecord]==1)
			{
				go_dn = true;
			}
			else
			{
				go_dn = false;
			}

			while(piDdir[nCurrentRecord]!=5 && go_dn ==true)
			{
				nCurrentRecord = piDrec[nCurrentRecord]-1;
				new_ups = new_ups + 1;
				if(piUpSlope[nCurrentRecord]==1)
				{
					piUpSlope[nCurrentRecord] = piUpSlope[nCurrentRecord] + new_ups;
				}
				else
				{
					piUpSlope[nCurrentRecord] = piUpSlope[nCurrentRecord] + new_ups;
					go_dn = false;
				}
			}
			while(piDdir[nCurrentRecord]!=5 && new_ups>0 && piDdir[nCurrentRecord]!=0)
			{
				nCurrentRecord = piDrec[nCurrentRecord]-1;
				piUpSlope[nCurrentRecord] = piUpSlope[nCurrentRecord] + new_ups;
			}

			nCurrentRecord = start_rec;

		}
		else
		{
			piUpSlope[nCurrentRecord] = 0;
		}

	}
}

void CWeppProcess::Remark_Chans(int *piSortedIndex)
{
	nCurrentRecord =0;
	int nSortedRecordPointer = 0;
	int nLoop = 0;
	
	int curr_chan = 0;

	bool go_dn = false;
	int bExit = false;

	int Ups_Diff = 0;
	int chan_len = 0;
	
	max_chans = 0;
	int curr_cent  =0;
	
	for(nLoop=0;nLoop<m_Row*m_Col;nLoop++)
	{
		piChan_No[nLoop] = 0;
		if(piSeedType[nLoop] ==2)
		{
			piSeedType[nLoop] = 0;
		}
	}


	m_Message = "Running Remark_Chans .....";
	//m_ProgressCtrl.StepIt();
	////UpdateData(false);
	
	for(nLoop=0;nLoop<m_Row*m_Col;nLoop++)
	{
		nCurrentRecord = piSortedIndex[nLoop];

		if(piSeedType[nCurrentRecord] ==1)
		{
			if(piUpSlope[nCurrentRecord] >m_Upslope)
			{
				nSortedRecordPointer = nLoop;
				Ups_Diff = 0;
				chan_len = 1;
				curr_chan++;
				bExit = false;
				
				
				while(bExit ==false && piDdir[nCurrentRecord]!=5)
				{
					if(piChan_No[nCurrentRecord]<1)
					{
						if(chan_len==1)
						{
							piSeedType[nCurrentRecord] = 1;
							piChan_No[nCurrentRecord]  = curr_chan;
						}
						else
						{
							if(piDdir[nCurrentRecord] ==5)
							{
								piSeedType[nCurrentRecord] = 5;
								piChan_No[nCurrentRecord] = curr_chan;
							}
							else
							{
								piChan_No[nCurrentRecord] = curr_chan;
							}
							
						}
						nCurrentRecord= piDrec[nCurrentRecord]-1;

						chan_len = chan_len + 1;
					}
					else
					{
						piSeedType[nCurrentRecord] = 2;
						bExit = true;

					}

				}


			}
			else
			{
				piSeedType[nCurrentRecord] = 0;
			}



		}
		if(piDdir[nCurrentRecord] ==5)
		{
			piChan_No [nCurrentRecord]= curr_chan;
			piSeedType [nCurrentRecord]= 5;
		}

	}

	max_chans  = curr_chan;
	int Numchans = 0;
	int numneigh = 0;
	int zrec[8];
	int cent_y =0;
	int cent_x = 0;
	double cent_z = 0;
	int cent_rec = 0;
	int above_rec[9];
	double min_elev = 0;
	int lowest_neigh = 0;

	int cent_ups = 0;
	for(nLoop=0;nLoop<m_Row*m_Col;nLoop++)
	{
		nCurrentRecord = piSortedIndex[nLoop];
		if(piSeedType[nCurrentRecord]==2)
		{
			curr_cent = nCurrentRecord;
			Numchans = 0;

			numneigh = 0;
			cent_ups = 0;


			cent_y = nCurrentRecord / m_Col;
			cent_x = nCurrentRecord % m_Col;
			cent_z = pdElev[nCurrentRecord];
			cent_rec = nCurrentRecord;
			numneigh = 0;
			if(cent_x >0 && cent_y >0)
			{
				
				zrec[numneigh] = cent_rec - (m_Col + 1);
				numneigh = numneigh + 1;
			}
			if(cent_y >0)
			{
				zrec[numneigh] = cent_rec - m_Col;
				numneigh = numneigh + 1;
			}
			if(cent_y >0 && cent_x <m_Col -1)
			{
				zrec[numneigh] = cent_rec - (m_Col -1);
				numneigh = numneigh + 1;
			}
			if(cent_x >0)
			{
				zrec[numneigh] = cent_rec -1;
				numneigh = numneigh + 1;
			}
			if(cent_x <m_Col-1)
			{
				zrec[numneigh] = cent_rec + 1;
				numneigh = numneigh + 1;				
			}

			if(cent_x >0 && cent_y <(m_Row-1))
			{
				zrec[numneigh] = cent_rec + (m_Col -1);
				numneigh = numneigh + 1;
			}
			if(cent_y <m_Row -1)
			{
				zrec[numneigh] = cent_rec + m_Col;
				numneigh = numneigh + 1;
			}
			if(cent_x <m_Col-1 && cent_y <(m_Row-1))
			{
				zrec[numneigh] = cent_rec  + m_Col +1;
				numneigh = numneigh + 1;
			}


			for (int nIndex=0;nIndex<numneigh;nIndex++)
			{
				nCurrentRecord = zrec[nIndex];
				if (pdElev[nCurrentRecord]!=m_MissingData)
				{
					if (((piDrec[nCurrentRecord]-1)==cent_rec) && 
						(piChan_No[nCurrentRecord] >0))
					{
						Numchans = Numchans + 1;
						above_rec[Numchans-1] = nCurrentRecord;
					}
				}
			}
			if(Numchans>1)
			{
				for(int nIndex=0;nIndex<Numchans;nIndex++)
				{
					nCurrentRecord = above_rec[nIndex];
					min_elev = pdElev[nCurrentRecord];
					cent_rec = nCurrentRecord;
					cent_ups = piUpSlope[nCurrentRecord];
					lowest_neigh = cent_rec;

					cent_y = nCurrentRecord / m_Col;
					cent_x = nCurrentRecord % m_Col;
					cent_z = pdElev[nCurrentRecord];
					//cent_rec = nCurrentRecord;
					numneigh = 0;
					if(cent_x >0 && cent_y >0)
					{
						
						zrec[numneigh] = cent_rec - (m_Col + 1);
						numneigh = numneigh + 1;
					}
					if(cent_y >0)
					{
						zrec[numneigh] = cent_rec - m_Col;
						numneigh = numneigh + 1;
					}
					if(cent_y >0 && cent_x <m_Col -1)
					{
						zrec[numneigh] = cent_rec - (m_Col -1);
						numneigh = numneigh + 1;
					}
					if(cent_x >0)
					{
						zrec[numneigh] = cent_rec -1;
						numneigh = numneigh + 1;
					}
					if(cent_x <m_Col-1)
					{
						zrec[numneigh] = cent_rec + 1;
						numneigh = numneigh + 1;				
					}

					if(cent_x >0 && cent_y <(m_Row-1))
					{
						zrec[numneigh] = cent_rec + (m_Col -1);
						numneigh = numneigh + 1;
					}
					if(cent_y <m_Row -1)
					{
						zrec[numneigh] = cent_rec + m_Col;
						numneigh = numneigh + 1;
					}
					if(cent_x <m_Col-1 && cent_y <(m_Row-1))
					{
						zrec[numneigh] = cent_rec  + m_Col +1;
						numneigh = numneigh + 1;
					}
					for(int nIndex2=0;nIndex2<numneigh;nIndex2++)
					{
						nCurrentRecord = zrec[nIndex2];
						if(piChan_No[nCurrentRecord]>0)
						{
							if(pdElev[nCurrentRecord]<=min_elev && 
								piUpSlope[nCurrentRecord]>cent_ups)
							{
								min_elev = pdElev[nCurrentRecord];
								lowest_neigh = zrec[nIndex2];
							}
						}
					}

					if(lowest_neigh !=cent_rec)
					{
						piDrec[cent_rec] = lowest_neigh + 1;

						piSeedType[lowest_neigh] = 2;
					}
				}
			}
		}
	}
	
	curr_chan = 0;
	int cent_chan = 0;
	int cent_shed = 0;
	int max_upslope = 0;
	int max_up = 0;
	for(nLoop=0;nLoop<m_Row*m_Col;nLoop++)
	{
		
		nCurrentRecord = piSortedIndex[nLoop];
		if(piSeedType[nCurrentRecord] ==2)
		{
			cent_y = nCurrentRecord / m_Col;
			cent_x = nCurrentRecord % m_Col;
			cent_z = pdElev[nCurrentRecord];
			cent_rec = nCurrentRecord;
			curr_cent = nCurrentRecord;
			cent_chan = piChan_No[nCurrentRecord];
			cent_shed = piShedNow[nCurrentRecord];
			numneigh = 0;
			if(cent_x >0 && cent_y >0)
			{
				
				zrec[numneigh] = cent_rec - (m_Col + 1);
				numneigh = numneigh + 1;
			}
			if(cent_y >0)
			{
				zrec[numneigh] = cent_rec - m_Col;
				numneigh = numneigh + 1;
			}
			if(cent_y >0 && cent_x <m_Col -1)
			{
				zrec[numneigh] = cent_rec - (m_Col -1);
				numneigh = numneigh + 1;
			}
			if(cent_x >0)
			{
				zrec[numneigh] = cent_rec -1;
				numneigh = numneigh + 1;
			}
			if(cent_x <m_Col-1)
			{
				zrec[numneigh] = cent_rec + 1;
				numneigh = numneigh + 1;				
			}

			if(cent_x >0 && cent_y <(m_Row-1))
			{
				zrec[numneigh] = cent_rec + (m_Col -1);
				numneigh = numneigh + 1;
			}
			if(cent_y <m_Row -1)
			{
				zrec[numneigh] = cent_rec + m_Col;
				numneigh = numneigh + 1;
			}
			if(cent_x <m_Col-1 && cent_y <(m_Row-1))
			{
				zrec[numneigh] = cent_rec  + m_Col +1;
				numneigh = numneigh + 1;
			}
			

			
			Numchans = 0;
			max_upslope = 0;
			
			for(int nIndex = 0;nIndex<numneigh;nIndex++)
			{
				nCurrentRecord = zrec[nIndex];
				if(pdElev[nCurrentRecord]!=m_MissingData)
				{
					if((piDrec[nCurrentRecord]-1)==cent_rec && piChan_No[nCurrentRecord]>0)
					{
						Numchans = Numchans + 1;
						above_rec[Numchans-1] = nCurrentRecord;
						if(piUpSlope[nCurrentRecord]>max_upslope)
						{
							max_upslope = piUpSlope[nCurrentRecord];
							max_up = Numchans;
						}
					}
				}
			}

			if(Numchans>1)
			{
				for(int nIndex=0;nIndex<Numchans;nIndex++)
				{
					nCurrentRecord = above_rec[nIndex];
					if (nIndex+1!=max_up)
					{
						if (piSeedType[nCurrentRecord] ==0)
						{
							piSeedType[nCurrentRecord] = 3;
							

						}
					}
					else
					{
						if(piSeedType[nCurrentRecord]==0)
						{
							piSeedType[nCurrentRecord] = 7;

							cent_rec = nCurrentRecord;
							cent_y = nCurrentRecord / m_Col;
							cent_x = nCurrentRecord % m_Col;
							cent_z = pdElev[nCurrentRecord];

							numneigh = 0;
							if(cent_x >0 && cent_y >0)
							{
								
								zrec[numneigh] = cent_rec - (m_Col + 1);
								numneigh = numneigh + 1;
							}
							if(cent_y >0)
							{
								zrec[numneigh] = cent_rec - m_Col;
								numneigh = numneigh + 1;
							}
							if(cent_y >0 && cent_x <m_Col -1)
							{
								zrec[numneigh] = cent_rec - (m_Col -1);
								numneigh = numneigh + 1;
							}
							if(cent_x >0)
							{
								zrec[numneigh] = cent_rec -1;
								numneigh = numneigh + 1;
							}
							if(cent_x <m_Col-1)
							{
								zrec[numneigh] = cent_rec + 1;
								numneigh = numneigh + 1;				
							}

							if(cent_x >0 && cent_y <(m_Row-1))
							{
								zrec[numneigh] = cent_rec + (m_Col -1);
								numneigh = numneigh + 1;
							}
							if(cent_y <m_Row -1)
							{
								zrec[numneigh] = cent_rec + m_Col;
								numneigh = numneigh + 1;
							}
							if(cent_x <m_Col-1 && cent_y <(m_Row-1))
							{
								zrec[numneigh] = cent_rec  + m_Col +1;
								numneigh = numneigh + 1;
							}

							for(int nLoop2=0;nLoop2<numneigh;nLoop2++)
							{
								nCurrentRecord = zrec[nLoop2];
								if(pdElev[nCurrentRecord]!=m_MissingData)
								{
									if(piChan_No[nCurrentRecord]==0)
									{
										if (pdElev[nCurrentRecord]>cent_z)
										{
											piDrec[nCurrentRecord]=cent_rec+1;
										}
									}
								}

							}
							
							nCurrentRecord = cent_rec;


						}
					}
				}
			}
		}

		
	}


}


void CWeppProcess::ReadFillDbf(int* nNumPits)
{
	char szBuffer [2555];
	

	CFile myFile(m_Directory  + m_FileName + "fill.dbf",CFile::modeReadWrite);
	myFile.Seek(4,CFile::begin);
	myFile.Read(szBuffer,4);//Number of records in the file offset 4 bites

	int* pShortInt= new int ;
	*pShortInt = *(int*) &szBuffer;
	int nPitLocation = -1;
	int nOutLocation =- 1;
	int nFieldLength[100];
	int nNumFields = 0;
	myFile.Seek(32,CFile::begin);

	for(int nLoop=0;nLoop<100;nLoop++)
	{
		myFile.Read(szBuffer,32);
		if(szBuffer[0] !=13)
		{
			if(_stricmp(szBuffer,"pitrec") == 0)
			{
				nPitLocation = nLoop;
			}
			if(_stricmp(szBuffer,"outrec")==0)
			{
				nOutLocation =nLoop;
			}

			nFieldLength[nLoop] = szBuffer[16];
		}
		else
		{
			if(nPitLocation ==-1 || nOutLocation ==-1)
			{
				AfxMessageBox("Could not find some of the fields.");
				exit(0);
			}
			nNumFields = nLoop;
			nLoop = 100;
		}
	}


	myFile.Seek(8,CFile::begin);
	myFile.Read(szBuffer,2);

	short int* piFirstRecordLoc = new short int;
	*piFirstRecordLoc = *(short int*)&szBuffer ;
	
	int nBitesOfSet = 0;
	int nOutRecOfSet = 0;
	for(int nLoop=0;nLoop<nPitLocation;nLoop++)
	{
		nBitesOfSet = nBitesOfSet + nFieldLength[nLoop];
	}
	for(int nLoop=0;nLoop<nOutLocation;nLoop++)
	{
		nOutRecOfSet = nOutRecOfSet + nFieldLength[nLoop];
	}

	myFile.Seek(*piFirstRecordLoc,CFile::begin);
	char szPitRecBuffer[2555] ;
	myFile.Seek(10,CFile::begin);
	myFile.Read(szBuffer,2);

	short int* piRecordLength = new short int;
	*piRecordLength = *(short int*) &szBuffer;
	myFile.Seek(*piFirstRecordLoc,CFile::begin);
	int nIndex = 0;


	piPitRec = new int [*pShortInt] ;
	piOutRec = new int[*pShortInt];

	for(int nLoop = 0;nLoop<*pShortInt;nLoop++)
	{
		myFile.Read(szBuffer,*piRecordLength);

		for(nIndex=0;nIndex<nFieldLength[nPitLocation];nIndex++)
		{
			szPitRecBuffer[nIndex] = szBuffer[nBitesOfSet +1 +nIndex];

		}
		piPitRec[nLoop] = (int)atof(szPitRecBuffer);

		for(nIndex=0;nIndex<nFieldLength[nOutLocation];nIndex++)
		{
			szPitRecBuffer[nIndex] = szBuffer[nOutRecOfSet +1 +nIndex];

		}
		piOutRec[nLoop] = (int) atof(szPitRecBuffer);
	}

	*nNumPits = *pShortInt;
	delete piRecordLength;
	delete piFirstRecordLoc;
	delete pShortInt;
	myFile.Close();
}


void CWeppProcess::Mark_Pits(int *piSortedIndex)
{


	int nNumPits = 0;
	int curr_chan = 0;

	int curr_shed = 0;
	int curr_pitrec = 0;
	m_Message = "Running Mark_Pits...";
	////UpdateData(false);

	ReadFillDbf(&nNumPits);

	int nLoop=0;
	int nIndex = 0;
	bool bRemoved = false;
	//search and remove all but the last record for a pit at a given location
	for(nLoop=nNumPits-1;nLoop>0;nLoop--)
	{
		if(bRemoved ==true)
		{
			nLoop = nNumPits -1;
		}
		curr_shed = nLoop;
		curr_pitrec = piPitRec[nLoop];
		bRemoved = false;
		for(nIndex=nLoop-1;nIndex>0;nIndex--)
		{
			if(piPitRec[nIndex] ==curr_pitrec)
			{
				for(int j=nIndex;j<nNumPits-1;j++)
				{
					piPitRec[j] = piPitRec[j+1];
					piOutRec[j] = piOutRec[j+1];


				}
				nNumPits--;
				bRemoved = true;
				 
			}
		}


	}
	int max_up = 0;
	
	curr_chan = max_chans;
	int curr_pour = 0;
	nCurrentRecord = 0;
	int last_up = 0;
	for(nLoop=0;nLoop<nNumPits;nLoop++)
	{
		curr_pitrec = piPitRec[nLoop]-1;
		curr_pour = piOutRec[nLoop]-1;
		nCurrentRecord = curr_pitrec;

		piSeedType[nCurrentRecord] = 5;
		if(piDrec[nCurrentRecord]-1 !=nCurrentRecord && piChan_No[nCurrentRecord]==0)
		{
			curr_chan = curr_chan + 1;
			piChan_No[nCurrentRecord] = curr_chan ;
			nCurrentRecord = piDrec[nCurrentRecord]-1;

			piSeedType[nCurrentRecord]=8;
			while(piChan_No[nCurrentRecord]==0)
			{
				piChan_No[nCurrentRecord] = curr_chan;
				last_up = nCurrentRecord;
				nCurrentRecord = piDrec[nCurrentRecord]-1;
				if(piChan_No[nCurrentRecord]>0 ||piSeedType[nCurrentRecord]==5 ||
					piSeedType[nCurrentRecord]==6)
				{
					if(piSeedType[nCurrentRecord]==0)
					{
						piSeedType[nCurrentRecord] = 2;

						above_junct();
					
					}
					else
					{
						if(piSeedType[nCurrentRecord]==1)
						{
							piSeedType[nCurrentRecord] = 0;
						}
						else
						{
							if(piSeedType[nCurrentRecord]==5 || 
								piSeedType[nCurrentRecord]==6)
							{
								piChan_No[nCurrentRecord] = curr_chan;

							}
							else
							{
								if(piSeedType[nCurrentRecord]==2)
								{
									above_junct();
								}
								else
								{
									if(piSeedType[nCurrentRecord]==3)
									{

									}
									else
									{
										if(piSeedType[nCurrentRecord]==7)
										{

										}
									}
								}
							}
						}
					}
				}
			}
		}
		else
		{
			if(piChan_No[nCurrentRecord]==0)
			{
				curr_chan = curr_chan + 1;
				nCurrentRecord = curr_pour;
				if(piChan_No[nCurrentRecord]==0)
				{
					piSeedType[nCurrentRecord]=1;
				}
				
				while(piChan_No[nCurrentRecord]==0)
				{
					piChan_No[nCurrentRecord]= curr_chan;
					last_up = nCurrentRecord;
					nCurrentRecord = piDrec[nCurrentRecord]-1;

					if(piChan_No[nCurrentRecord]>0 || piSeedType[nCurrentRecord]==5 ||
						piSeedType[nCurrentRecord]==6)
					{
						if(piSeedType[nCurrentRecord]==0)
						{
							piSeedType[nCurrentRecord]=2;
							above_junct();
						}
						else
						{
							if(piSeedType[nCurrentRecord]==1)
							{
								piSeedType[nCurrentRecord] = 0;

							}
							else
							{
								if(piSeedType[nCurrentRecord]==5 || piSeedType[nCurrentRecord]==6)
								{
									piChan_No[nCurrentRecord] = curr_chan;
								}
								else
								{
									if(piSeedType[nCurrentRecord]==2)
									{
										above_junct();
									}
									else
									{
										if(piSeedType[nCurrentRecord]==3)
										{

										}
										else
										{
											if(piSeedType[nCurrentRecord]==7)
											{
											}
										}
									}
								}
							}
						}
					}
				}
			}
			else
			{
				
				if(piDrec[nCurrentRecord]-1!=nCurrentRecord)
				{
					nCurrentRecord = piDrec[nCurrentRecord]-1;
					piSeedType[nCurrentRecord]= 8;
				}
			}
		}
	}
	delete piPitRec;
	delete piOutRec;
}



void CWeppProcess::above_junct()
{



	int last_up=0;
	int cent_y = 0;
	int cent_x = 0;
	int cent_rec = 0;
	double cent_z = 0;
	int numneigh = 0;
	int Numchans = 0;
	int max_upslope = 0;
	int zrec[8];
	int above_rec[9] ;
	int max_up  = 0;

	cent_y = nCurrentRecord / m_Col;
	cent_x = nCurrentRecord % m_Col;
	cent_z = pdElev[nCurrentRecord];
	cent_rec = nCurrentRecord;
	numneigh = 0;
	if(cent_x >0 && cent_y >0)
	{
		
		zrec[numneigh] = cent_rec - (m_Col + 1);
		numneigh = numneigh + 1;
	}
	if(cent_y >0)
	{
		zrec[numneigh] = cent_rec - m_Col;
		numneigh = numneigh + 1;
	}
	if(cent_y >0 && cent_x <m_Col -1)
	{
		zrec[numneigh] = cent_rec - (m_Col -1);
		numneigh = numneigh + 1;
	}
	if(cent_x >0)
	{
		zrec[numneigh] = cent_rec -1;
		numneigh = numneigh + 1;
	}
	if(cent_x <m_Col-1)
	{
		zrec[numneigh] = cent_rec + 1;
		numneigh = numneigh + 1;				
	}

	if(cent_x >0 && cent_y <(m_Row-1))
	{
		zrec[numneigh] = cent_rec + (m_Col -1);
		numneigh = numneigh + 1;
	}
	if(cent_y <m_Row -1)
	{
		zrec[numneigh] = cent_rec + m_Col;
		numneigh = numneigh + 1;
	}
	if(cent_x <m_Col-1 && cent_y <(m_Row-1))
	{
		zrec[numneigh] = cent_rec  + m_Col +1;
		numneigh = numneigh + 1;
	}



	Numchans = 0;
	max_upslope = 0;
	
	for(int nIndex = 0;nIndex<numneigh;nIndex++)
	{
		nCurrentRecord = zrec[nIndex];
		if(pdElev[nCurrentRecord]!=m_MissingData)
		{
			if((piDrec[nCurrentRecord]-1)==cent_rec && piChan_No[nCurrentRecord]>0)
			{
				Numchans = Numchans + 1;
				above_rec[Numchans-1] = nCurrentRecord;
				if(piUpSlope[nCurrentRecord]>max_upslope)
				{
					max_upslope = piUpSlope[nCurrentRecord];
					max_up = Numchans;
				}
			}
		}
	}

	if(Numchans>1)
	{
		for(int nIndex=0;nIndex<Numchans;nIndex++)
		{
			nCurrentRecord = above_rec[nIndex];
			if (nIndex+1!=max_up)
			{
				if (piSeedType[nCurrentRecord] ==0)
				{
					piSeedType[nCurrentRecord] = 3;
					

				}
			}
			else
			{
				if(piSeedType[nCurrentRecord]==0)
				{
					piSeedType[nCurrentRecord] = 7;

					cent_rec = nCurrentRecord;
					cent_y = nCurrentRecord / m_Col;
					cent_x = nCurrentRecord % m_Col;
					cent_z = pdElev[nCurrentRecord];

					numneigh = 0;
					if(cent_x >0 && cent_y >0)
					{
						
						zrec[numneigh] = cent_rec - (m_Col + 1);
						numneigh = numneigh + 1;
					}
					if(cent_y >0)
					{
						zrec[numneigh] = cent_rec - m_Col;
						numneigh = numneigh + 1;
					}
					if(cent_y >0 && cent_x <m_Col -1)
					{
						zrec[numneigh] = cent_rec - (m_Col -1);
						numneigh = numneigh + 1;
					}
					if(cent_x >0)
					{
						zrec[numneigh] = cent_rec -1;
						numneigh = numneigh + 1;
					}
					if(cent_x <m_Col-1)
					{
						zrec[numneigh] = cent_rec + 1;
						numneigh = numneigh + 1;				
					}

					if(cent_x >0 && cent_y <(m_Row-1))
					{
						zrec[numneigh] = cent_rec + (m_Col -1);
						numneigh = numneigh + 1;
					}
					if(cent_y <m_Row -1)
					{
						zrec[numneigh] = cent_rec + m_Col;
						numneigh = numneigh + 1;
					}
					if(cent_x <m_Col-1 && cent_y <(m_Row-1))
					{
						zrec[numneigh] = cent_rec  + m_Col +1;
						numneigh = numneigh + 1;
					}

					for(int nLoop2=0;nLoop2<numneigh;nLoop2++)
					{
						nCurrentRecord = zrec[nLoop2];

						if(pdElev[nCurrentRecord]!=m_MissingData)
						{
							if(piChan_No[nCurrentRecord]==0)
							{
								if (pdElev[nCurrentRecord]>cent_z)
								{
									piDrec[nCurrentRecord]=cent_rec+1;
								}
							}
						}

					}
					
					nCurrentRecord = cent_rec;

				}
			}
		}
	}
}

void CWeppProcess::split_segs(int *piSortedIndex)
{
	int start_rec = 0;
	int len_in_m = 0;
	int new_seg_len = 0;
	int num_segs = 0;
	int seg_len = 0;
	int curr_rec = 0;
	
	nCurrentRecord = 0;

	int nLoop = 0;
	

	m_Message = "Running split_segs...";
	////UpdateData(false);
	for(nLoop=0;nLoop<m_Row*m_Col;nLoop++)
	{
		if(piSeedType[nLoop]==4)
		{
			piSeedType[nLoop]=0;
		}
	}

	for(nLoop=0;nLoop<m_Row*m_Col;nLoop++)
	{
		nCurrentRecord = piSortedIndex[nLoop];
		if(piSeedType[nCurrentRecord]==1 || piSeedType[nCurrentRecord]==2||
			piSeedType[nCurrentRecord]==8)
		{
			seg_len = 0;
			if(piDrec[nCurrentRecord]-1!=nCurrentRecord)
			{
				nCurrentRecord = piDrec[nCurrentRecord]-1;
				seg_len = seg_len + 1;

				while(piSeedType[nCurrentRecord]==0)
				{
					nCurrentRecord = piDrec[nCurrentRecord]-1;
					seg_len = seg_len + 1;
				}
				len_in_m = seg_len * m_Size;

				if(len_in_m>m_MaxSegLength)
				{
					num_segs = (len_in_m/(int)m_MaxSegLength) + 1;
					new_seg_len = seg_len/num_segs;
					nCurrentRecord = piSortedIndex[nLoop];

					for(int nIndex=0;nIndex<num_segs -1;nIndex++)
					{
						for(int nIndex2=0;nIndex2<new_seg_len;nIndex2++)
						{
							nCurrentRecord = piDrec[nCurrentRecord]-1;
						}

						piSeedType[nCurrentRecord] = 4;
						if(piDrec[nCurrentRecord]-1!=nCurrentRecord)
						{
							curr_rec = nCurrentRecord;
							nCurrentRecord = piDrec[nCurrentRecord]-1;
							if(piSeedType[nCurrentRecord]==0)
							{
								piSeedType[nCurrentRecord] = 8;
							}
							nCurrentRecord = curr_rec;
						}


					}
				}


			}
			
		}
		
	}

}

void CWeppProcess::Flow2Chan(int *piSortedIndex)
{
	int nLoop = 0;
	
	nCurrentRecord = 0;
	int cent_rec = 0;
	int cent_y = 0;
	int cent_x = 0;
	double cent_z = 0;
	int zrec[8];
	int numneigh=0;



	for(nLoop=0;nLoop<m_Row*m_Col;nLoop++)
	{
		nCurrentRecord = piSortedIndex[nLoop];

		if(piChan_No[nCurrentRecord]>0 && piSeedType[nCurrentRecord]!=2)
		{


			cent_rec = nCurrentRecord;
			cent_y = nCurrentRecord / m_Col;
			cent_x = nCurrentRecord % m_Col;
			cent_z = pdElev[nCurrentRecord];
			numneigh = 0;
			if(cent_x >0 && cent_y >0)
			{
				
				zrec[numneigh] = cent_rec - (m_Col + 1);
				numneigh = numneigh + 1;
			}
			if(cent_y >0)
			{
				zrec[numneigh] = cent_rec - m_Col;
				numneigh = numneigh + 1;
			}
			if(cent_y >0 && cent_x <m_Col -1)
			{
				zrec[numneigh] = cent_rec - (m_Col -1);
				numneigh = numneigh + 1;
			}
			if(cent_x >0)
			{
				zrec[numneigh] = cent_rec -1;
				numneigh = numneigh + 1;
			}
			if(cent_x <m_Col-1)
			{
				zrec[numneigh] = cent_rec + 1;
				numneigh = numneigh + 1;				
			}

			if(cent_x >0 && cent_y <(m_Row-1))
			{
				zrec[numneigh] = cent_rec + (m_Col -1);
				numneigh = numneigh + 1;
			}
			if(cent_y <m_Row -1)
			{
				zrec[numneigh] = cent_rec + m_Col;
				numneigh = numneigh + 1;
			}
			if(cent_x <m_Col-1 && cent_y <(m_Row-1))
			{
				zrec[numneigh] = cent_rec  + m_Col +1;
				numneigh = numneigh + 1;
			}			
			
			for(int nIndex=0;nIndex<numneigh;nIndex++)
			{
				nCurrentRecord = zrec[nIndex];
				if(pdElev[nCurrentRecord]>0)
				{
					if(piChan_No[nCurrentRecord]==0 && pdElev[nCurrentRecord]>cent_z)
					{
						piDrec[nCurrentRecord] = cent_rec+1;
					}
				}
			}


		}
		
	}

}

void CWeppProcess::calc_segs(int *piSortedIndex)
{


	m_Message = "Running Calc_Segs...";
	//m_ProgressCtrl.StepIt();
	//UpdateData(false);
	//Allocate memory as needed
	piSegNo = new short int [m_Row*m_Col];
	piChan_Side = new short int [m_Row*m_Col];
	for(int j=0;j<m_Row*m_Col;j++)
	{
		piSegNo[j] = 0;
		piChan_Side[j] = 0;
	}
	
	pSegs = new Segs [1000];
	for(int j=0;j<1000;j++)
	{
		pSegs[j].Center_Seg = 0;
		pSegs[j].Chan_Shape = 0;
		pSegs[j].Down_Seg=0;
		pSegs[j].Drains_Rec = 0;
		pSegs[j].End_Cell=0;
		pSegs[j].End_Col=0;
		pSegs[j].End_Elev=0;
		pSegs[j].End_Row=0;
		pSegs[j].End_Type=0;
		pSegs[j].Final_Id=0;
		pSegs[j].Flow2Crow=0;
		pSegs[j].Impound=false;
		pSegs[j].Initial_Id=0;
		pSegs[j].Left_Hill=0;
		pSegs[j].Left_Imp=0;
		pSegs[j].Left_Seg=0;
		pSegs[j].Len_Cells=0;
		pSegs[j].Len_Meters=0;
		pSegs[j].Right_Hill=0;
		pSegs[j].Right_Imp=0;
		pSegs[j].Right_Seg=0;
		pSegs[j].Sort_Order=0;
		pSegs[j].Start_Cell=0;
		pSegs[j].Start_Col=0;
		pSegs[j].Start_Ddir=0;
		pSegs[j].Start_Elev=0;
		pSegs[j].Start_Row=0;
		pSegs[j].Start_Type=0;
		pSegs[j].Top_Hill=0;
		pSegs[j].Top_Imp=0;
		pSegs[j].Width=0;
		pSegs[j].Left_Rec = 0;
		pSegs[j].Right_Rec = 0;
		pSegs[j].Center_Rec = 0;
	}
	
	int seg_no = 0;
	nSegSize = 0;
	int nLoop=0;
	nCurrentRecord = 0;

	bool double_2 = false;
	int cent_y=0;
	int cent_x = 0;
	double cent_z = 0;
	int cent_rec = 0;
	int cent_chan = 0;
	int numneigh = 0;
	int seg_len = 0;

	int mstart_cell = 0;
	int mstart_row = 0;
	int mstart_col = 0;
	double mstart_elev = 0;
	int mstart_type = 0;
	bool mimpound = false;
	int msegment_no = 0;
	int mend_cell = 0;
	int mend_row = 0;
	int mend_col = 0;
	double mend_elev=0;
	int mend_type = 0;




	for(nLoop=0;nLoop<m_Row*m_Col;nLoop++)
	{
		nCurrentRecord = piSortedIndex[nLoop];
		if(piSeedType[nCurrentRecord] ==4 || piSeedType[nCurrentRecord]==5)
		{
			if(piDrec[nCurrentRecord]-1!=nCurrentRecord)
			{
				if(piChan_No[nCurrentRecord]>0)
				{

					nCurrentRecord = piDrec[nCurrentRecord]-1;
					if(piSeedType[nCurrentRecord]==0)
					{
						piSeedType[nCurrentRecord]=8;
					}
				}
			}
		}

	}
	

	for(nLoop=0;nLoop<m_Row*m_Col;nLoop++)
	{
		nCurrentRecord = piSortedIndex[nLoop];
		if(piSeedType[nCurrentRecord]==1 ||
			piSeedType[nCurrentRecord]==2||
			piSeedType[nCurrentRecord]==8||
			piSeedType[nCurrentRecord]==5)
		{
			//start_rec  = nCurrentRecord;
			double_2 = false;
			cent_rec = nCurrentRecord;
			cent_y = nCurrentRecord / m_Col;
			cent_x = nCurrentRecord % m_Col;
			cent_z = pdElev[nCurrentRecord];
			cent_chan = piChan_No[nCurrentRecord];

			numneigh = 0;

			mstart_cell = nCurrentRecord;
			mstart_row = cent_y;
			mstart_col = cent_x;
			mstart_elev = pdElev[nCurrentRecord];
			mstart_type = piSeedType[nCurrentRecord];
			seg_len = 1;
			mimpound = false;
			
			if(piSeedType[nCurrentRecord]==5)
			{
				msegment_no = piSegNo[nCurrentRecord];
				seg_no = seg_no +1;
				FindSide(&cent_y,&cent_x,&cent_z, &cent_rec,piSortedIndex);
				mend_cell = nCurrentRecord;
				mend_row = nCurrentRecord/m_Col;
				mend_col = nCurrentRecord%m_Col;
				mend_elev = pdElev[nCurrentRecord];
				mend_type = piSeedType[nCurrentRecord];
				
				mimpound = false;
				RecordSeg(mstart_cell,mstart_row, mstart_col, mstart_elev,
					mstart_type, mimpound,seg_no, mend_cell, mend_row, mend_col, mend_elev,
					mend_type,seg_len);
				seg_no = seg_no + 1;

				mimpound = true;

				RecordSeg(mstart_cell,mstart_row, mstart_col, mstart_elev,
					mstart_type, mimpound,seg_no, mend_cell, mend_row, mend_col, mend_elev,
					mend_type,seg_len);


			}
			else
			{
				FindSide(&cent_y,&cent_x,&cent_z, &cent_rec,piSortedIndex);
				nCurrentRecord = piDrec[nCurrentRecord]-1;
				seg_len = seg_len + 1;
				if(piSeedType[nCurrentRecord]==2 && mstart_type ==2)
				{
					cent_rec = nCurrentRecord;
					cent_y = nCurrentRecord / m_Col;
					cent_x = nCurrentRecord % m_Col;
					cent_z = pdElev[nCurrentRecord];
					cent_chan = piChan_No[nCurrentRecord];
					numneigh = 0;
					seg_len = seg_len + 1;
					seg_no= seg_no + 1;

					
					mend_cell = nCurrentRecord;
					mend_row = cent_y;
					mend_col = cent_x;
					mend_elev = cent_z;
					mend_type = piSeedType[nCurrentRecord];

					RecordSeg(mstart_cell,mstart_row, mstart_col, mstart_elev,
					mstart_type, mimpound,seg_no, mend_cell, mend_row, mend_col, mend_elev,
					mend_type,seg_len);


				}
				else
				{
					if(piSeedType[nCurrentRecord]==0 || piSeedType[nCurrentRecord]==2 ||
						piSeedType[nCurrentRecord]==8)
					{
						cent_rec = nCurrentRecord;
						cent_y = nCurrentRecord / m_Col;
						cent_x = nCurrentRecord % m_Col;
						cent_z = pdElev[nCurrentRecord];
						cent_chan = piChan_No[nCurrentRecord];
						numneigh = 0;
						seg_no = seg_no + 1;
						FindSide(&cent_y,&cent_x,&cent_z, &cent_rec,piSortedIndex);
						while (piSeedType[nCurrentRecord]==0)
						{
							nCurrentRecord = piDrec[nCurrentRecord]-1;
							seg_len = seg_len +1;
							cent_rec = nCurrentRecord;
							cent_y = nCurrentRecord / m_Col;
							cent_x = nCurrentRecord % m_Col;
							cent_z = pdElev[nCurrentRecord];
							cent_chan = piChan_No[nCurrentRecord];
							numneigh = 0;
							FindSide(&cent_y,&cent_x,&cent_z, &cent_rec,piSortedIndex);
							
						}
						cent_rec = nCurrentRecord;
						cent_y = nCurrentRecord / m_Col;
						cent_x = nCurrentRecord % m_Col;
						cent_z = pdElev[nCurrentRecord];
						cent_chan = piChan_No[nCurrentRecord];
						numneigh  = 0;

						mend_cell = nCurrentRecord;
						mend_row = cent_y;
						mend_col = cent_x;
						mend_elev = cent_z;
						mend_type = piSeedType[nCurrentRecord];

						FindSide(&cent_y,&cent_x,&cent_z, &cent_rec,piSortedIndex);

						RecordSeg(mstart_cell,mstart_row, mstart_col, mstart_elev,
						mstart_type, mimpound,seg_no, mend_cell, mend_row, mend_col, mend_elev,
						mend_type,seg_len);


					}
					else
					{
						mend_cell = nCurrentRecord;
						mend_row = nCurrentRecord /m_Col;
						mend_col = nCurrentRecord % m_Col;
						mend_elev = pdElev[nCurrentRecord];
						mend_type = piSeedType[nCurrentRecord];

						FindSide(&cent_y,&cent_x,&cent_z, &cent_rec,piSortedIndex);
						seg_no = seg_no + 1;

						RecordSeg(mstart_cell,mstart_row, mstart_col, mstart_elev,
						mstart_type, mimpound,seg_no, mend_cell, mend_row, mend_col, mend_elev,
						mend_type,seg_len);
					}
				}
			}
		}
	}
}


void CWeppProcess::RecordSeg(int mstart_cell, int mstart_row, int mstart_col, 
							 double mstart_elev, int mstart_type, bool mimpound, 
							 int seg_no,  int mend_cell, int mend_row, int mend_col, 
							 double mend_elev, int mend_type, int seg_len)
{
	
	if(nSegSize%1000==0 && nSegSize!=0)
	{
		Segs* pTemp = new Segs[nSegSize + 1000];





		for(int nLoop=0;nLoop<nSegSize;nLoop++)
		{
			pTemp[nLoop] = pSegs[nLoop];
		}
		delete pSegs;
		pSegs = pTemp;


		for(int j=nSegSize;j<nSegSize + 1000;j++)
		{
			pSegs[j].Center_Seg = 0;
			pSegs[j].Chan_Shape = 0;
			pSegs[j].Down_Seg=0;
			pSegs[j].Drains_Rec = 0;
			pSegs[j].End_Cell=0;
			pSegs[j].End_Col=0;
			pSegs[j].End_Elev=0;
			pSegs[j].End_Row=0;
			pSegs[j].End_Type=0;
			pSegs[j].Final_Id=0;
			pSegs[j].Flow2Crow=0;
			pSegs[j].Impound=false;
			pSegs[j].Initial_Id=0;
			pSegs[j].Left_Hill=0;
			pSegs[j].Left_Imp=0;
			pSegs[j].Left_Seg=0;
			pSegs[j].Len_Cells=0;
			pSegs[j].Len_Meters=0;
			pSegs[j].Right_Hill=0;
			pSegs[j].Right_Imp=0;
			pSegs[j].Right_Seg=0;
			pSegs[j].Sort_Order=0;
			pSegs[j].Start_Cell=0;
			pSegs[j].Start_Col=0;
			pSegs[j].Start_Ddir=0;
			pSegs[j].Start_Elev=0;
			pSegs[j].Start_Row=0;
			pSegs[j].Start_Type=0;
			pSegs[j].Top_Hill=0;
			pSegs[j].Top_Imp=0;
			pSegs[j].Width=0;
			pSegs[j].Left_Rec = 0;
			pSegs[j].Right_Rec = 0;
			pSegs[j].Center_Rec = 0;

		}


	}




	pSegs[nSegSize].Start_Cell = mstart_cell+1;
	pSegs[nSegSize].Start_Row = mstart_row+1;
	pSegs[nSegSize].Start_Col = mstart_col+1;
	pSegs[nSegSize].Start_Elev = mstart_elev;
	pSegs[nSegSize].Start_Type = mstart_type;
	pSegs[nSegSize].Impound = mimpound;
	pSegs[nSegSize].Initial_Id = seg_no;
	pSegs[nSegSize].End_Cell = mend_cell+1;
	pSegs[nSegSize].End_Row = mend_row+1;
	pSegs[nSegSize].End_Col = mend_col+1;
	pSegs[nSegSize].End_Elev = mend_elev;
	pSegs[nSegSize].End_Type = mend_type;
	pSegs[nSegSize].Len_Cells = seg_len;
	pSegs[nSegSize].Len_Meters = seg_len * m_Size;
	nSegSize++;
}



void CWeppProcess::FindSide(int *cent_y, int *cent_x, double *cent_z, int* cent_rec,
							int *piSortedIndex)
{
	short int row_diff =0;
	short int col_diff = 0;
	short int curr_cell = 0;
	int max_upslope = 0;

	*cent_rec = nCurrentRecord;
	*cent_y = nCurrentRecord / m_Col;
	*cent_x = nCurrentRecord % m_Col;
	*cent_z = pdElev[nCurrentRecord];
	
	int nLoop=0;
	int zrec[9];
	int down_cell = piDrec[nCurrentRecord]-1;
	int down_loc = 0;
	int up_loc = 0;

	double cent_elev = pdElev[nCurrentRecord];
	int cent_ddir = piDEMDdir[nCurrentRecord];
	int cent_drec = piDrec[nCurrentRecord]-1;
	int cent_shed = piDEMShedNow[nCurrentRecord];
	int side_val = 0;
	int curr_seed = piSeedType[nCurrentRecord];

	int ex_upslope = 0;
	int ex_up_cell = 0;
	int ex_up_loc = 0;
	int numneigh = 0;



	*cent_rec = nCurrentRecord;
	*cent_y = nCurrentRecord / m_Col;
	*cent_x = nCurrentRecord % m_Col;
	*cent_z = pdElev[nCurrentRecord];
	//cent_rec = nCurrentRecord;
	numneigh = 0;
	if(*cent_x >0 &&* cent_y >0)
	{
		
		zrec[numneigh] = *cent_rec - (m_Col + 1);
		numneigh = numneigh + 1;
	}
	if(*cent_y >0)
	{
		zrec[numneigh] = *cent_rec - m_Col;
		numneigh = numneigh + 1;
	}
	if(*cent_y >0 && *cent_x <m_Col -1)
	{
		zrec[numneigh] = *cent_rec - (m_Col -1);
		numneigh = numneigh + 1;
	}
	if(*cent_x >0)
	{
		zrec[numneigh] = *cent_rec -1;
		numneigh = numneigh + 1;
	}
	if(*cent_x <m_Col-1)
	{
		zrec[numneigh] = *cent_rec + 1;
		numneigh = numneigh + 1;				
	}

	if(*cent_x >0 && *cent_y <(m_Row-1))
	{
		zrec[numneigh] = *cent_rec + (m_Col -1);
		numneigh = numneigh + 1;
	}
	if(*cent_y <m_Row -1)
	{
		zrec[numneigh] = *cent_rec + m_Col;
		numneigh = numneigh + 1;
	}
	if(*cent_x <m_Col-1 && *cent_y <(m_Row-1))
	{
		zrec[numneigh] = *cent_rec  + m_Col +1;
		numneigh = numneigh + 1;
	}


	int Drains_in[9];
	int Cell_no[9];
	for(nLoop=0;nLoop<9;nLoop++)
	{
		Drains_in[nLoop] = 0;
		Cell_no[nLoop] = 0;
	}

	
	for(nLoop=0;nLoop<numneigh;nLoop++)
	{
		nCurrentRecord = zrec[nLoop];
		if(curr_seed ==1)
		{
			if(piDrec[nCurrentRecord]-1==*cent_rec &&
				piChan_No[nCurrentRecord]==0)
			{
				piChan_Side[nCurrentRecord] = 1;
			}
		}
		else
		{
			row_diff = *cent_y - (nCurrentRecord/m_Col);
			col_diff = *cent_x - (nCurrentRecord%m_Col);
			curr_cell = -1;
			if(row_diff ==1)
			{
				if(col_diff==1)
				{
					curr_cell = 1;
				}
				else
				{
					if(col_diff ==0)
					{
						curr_cell = 2;
					}
					else
					{
						if(col_diff ==-1)
						{
							curr_cell = 3;
						}
					}
				}
			}
			else
			{
				if(row_diff==0)
				{
					if(col_diff==1)
					{
						curr_cell = 4;
					}
					else
					{
						if(col_diff==0)
						{
							curr_cell = 5;
						}
						else
						{
							if(col_diff==-1)
							{
								curr_cell = 6;
							}
						}
					}
				}
				else
				{
					if(row_diff == -1)
					{
						if(col_diff ==1)
						{
							curr_cell = 7;
						}
						else
						{
							if(col_diff==0)
							{
								curr_cell = 8;
							}
							else
							{
								if(col_diff ==-1)
								{
									curr_cell = 9;
								}
							}
						}
					}
				}

			}


			if(piDrec[nCurrentRecord]-1==*cent_rec && piChan_No[nCurrentRecord]>0)
			{
				if(piUpSlope[nCurrentRecord]>max_upslope)
				{
					max_upslope = piUpSlope[nCurrentRecord];
					//up_cell = nCurrentRecord;
					up_loc = curr_cell;

				}
			}
			else
			{
				if(piDrec[nCurrentRecord]-1 == *cent_rec &&
					piChan_No[nCurrentRecord]==0)
				{
					Drains_in[curr_cell -1] = nCurrentRecord;
					Cell_no[curr_cell-1] = curr_cell;

					if(piUpSlope[nCurrentRecord]>ex_upslope && up_loc ==0)
					{
						ex_upslope = piUpSlope[nCurrentRecord];
						ex_up_cell = nCurrentRecord;
						ex_up_loc = curr_cell;
					}

				}
				else
				{
					if(nCurrentRecord == cent_drec)
					{
						down_cell = nCurrentRecord;
						down_loc = curr_cell;

					}
				}
			}

		}
	}
	
	if(up_loc ==0 && ex_up_loc>0)
	{
		up_loc = ex_up_loc;
	}

	nCurrentRecord = *cent_rec;
	curr_seed = piSeedType[nCurrentRecord];
	if(curr_seed !=1)
	{
		for(nLoop=0;nLoop<9;nLoop++)
		{
			if(Drains_in[nLoop]>0)
			{
				if(curr_seed==1)
				{
					side_val = 1;
				}
				else
				{
					Calc_Side(side_val,up_loc,Cell_no, down_loc, nLoop);
				}
				nCurrentRecord = Drains_in[nLoop];

				piChan_Side[nCurrentRecord] = side_val;
			}
		}
		nCurrentRecord = *cent_rec;
	}


}

void CWeppProcess::Calc_Side(int &side_val, int &up_loc, int *Cell_no, int& down_loc, int nLoop)
{
	side_val = 0;
	
	switch(down_loc)
	{
	case 1:
		switch(up_loc)
		{
		case 2:
			side_val = 3;
			break;
		case 3:
			if(Cell_no[nLoop]>3)
			{
				side_val = 3;
			}
			else
			{
				side_val = 2;
			}
			break;
		case 4:
			side_val = 2;
			break;
		case 6:
			if(Cell_no[nLoop]>3)
			{
				side_val = 3;
			}
			else
			{
				side_val = 2;
			}
			break;
		case 7:
			if(Cell_no[nLoop]==4)
			{
				side_val = 3;
			}
			else
			{
				side_val = 2;
			}
			break;
		case 8:
			if(Cell_no[nLoop]==4 || Cell_no[nLoop]==7)
			{
				side_val = 3;
			}
			else
			{
				side_val = 2;
			}
			break;
		case 9:
			if(Cell_no[nLoop]==4 || Cell_no[nLoop]>6)
			{
				side_val = 3;
			}
			else
			{
				side_val = 2;
			}
			break;
		}

		break;


	case 2:
		switch (up_loc)
		{
		case 1:
			side_val = 2;
			break;
		case 3:
			side_val =3;
			break;
		case 4:
			if(Cell_no[nLoop]==1)
			{
				side_val = 3;
			}
			else
			{
				side_val = 2;
			}
			break;
		case 6:
			if(Cell_no[nLoop]==3)
			{
				side_val = 2;
			}
			else
			{
				side_val = 3;
			}
			break;
		case 7:
			if(Cell_no[nLoop]==4 || Cell_no[nLoop]==1)
			{
				side_val = 3;
			}
			else
			{
				side_val = 2;
			}
			break;
		case 8:
			if(Cell_no[nLoop]==1 || Cell_no[nLoop]==4 || Cell_no[nLoop]==7)
			{
				side_val = 3;
			}
			else
			{
				side_val = 2;
			}
			break;

		case 9:
			if(Cell_no[nLoop]==3 || Cell_no[nLoop]==6)
			{
				side_val =2;
			}
			else
			{
				side_val = 3;
			}
			break;		
		}
		break;

	case 3:
		switch (up_loc)
		{
		case 1:
			if(Cell_no[nLoop]==2)
			{
				side_val = 3;
			}
			else
			{
				side_val = 2;
			}
			break;
		case 2:
			side_val = 2;
			break;
		case 4:
			if(Cell_no[nLoop]<3)
			{
				side_val = 3;
			}
			else
			{
				side_val = 2;
			}
			break;
		case 6:
			side_val = 3;
			break;
		case 7:
			if(Cell_no[nLoop]<5)
			{
				side_val = 3;
			}
			else
			{
				side_val = 2;
			}
			break;
		case 8:
			if(Cell_no[nLoop]==6 || Cell_no[nLoop]==9)
			{
				side_val = 2;
			}
			else
			{
				side_val = 3;
			}
			break;
		case 9:
			if(Cell_no[nLoop]==6)
			{
				side_val = 2;
			}
			else
			{
				side_val = 3;
			}
			break;
		}
		break;
	case 4:
		switch (up_loc)
		{
		case 1:
			side_val = 3;
			break;
		case 2:
			if(Cell_no[nLoop]==1)
			{
				side_val = 2;
			}
			else
			{
				side_val = 3;
			}
			break;
		case 3:
			if(Cell_no[nLoop]<3)
			{
				side_val = 2;
			}
			else
			{
				side_val = 3;
			}
			break;
		case 6:
			if(Cell_no[nLoop]<4)
			{
				side_val = 2;
			}
			else
			{
				side_val = 3;
			}
			break;
		case 7:
			side_val = 2;
			break;
		case 8:
			if(Cell_no[nLoop]==7)
			{
				side_val = 3;
			}
			else
			{
				side_val = 2;
			}
			break;
		case 9:
			if(Cell_no[nLoop]==7|| Cell_no[nLoop]==8)
			{
				side_val = 3;
			}
			else
			{
				side_val = 2;
			}
			break;

		}
		break;



		case 6:
			switch (up_loc)
			{
			case 1:
				if(Cell_no[nLoop]<4)
				{
					side_val = 3;
				}
				else
				{
					side_val = 2;
				}
				break;
				
			case 2:
				if(Cell_no[nLoop]==3)
				{
					side_val = 3;
				}
				else
				{
					side_val = 2;
				}
				break;
			case 3:
				side_val = 2;
				break;
			case 4:
				if(Cell_no[nLoop]<4)
				{
					side_val = 3;
				}
				else
				{
					side_val = 2;
				}
				break;
			case 7:
				if(Cell_no[nLoop]>7)
				{
					side_val = 2;
				}
				else
				{
					side_val = 3;
				}
				break;
			case 8:
				if(Cell_no[nLoop]==9)
				{
					side_val = 2;
				}
				else
				{
					side_val = 3;
				}
				break;
			case 9:
				side_val = 3;
				break;
			}
			break;

		case 7:
			switch(up_loc)
			{
			case 1:
				if(Cell_no[nLoop]==4)
				{
					side_val = 2;
				}
				else
				{
					side_val = 3;
				}
				break;
			case 2:
				if(Cell_no[nLoop]==1 || Cell_no[nLoop]==4)
				{
					side_val = 2;
				}
				else
				{
					side_val = 3;
				}
				break;
			case 3:
				if(Cell_no[nLoop]<5)
				{
					side_val = 2;
				}
				else
				{
					side_val = 3;
				}
				break;
			case 4:
				side_val = 3;
				break;
			case 6:
				if(Cell_no[nLoop]>7)
				{
					side_val = 3;
				}
				else
				{
					side_val = 2;
				}
				break;
			case 8:
				side_val = 2;
				break;
			case 9:
				if(Cell_no[nLoop]==8)
				{
					side_val = 3;
				}
				else
				{
					side_val = 2;
				}
				break;

			}
			break;
		case 8:
			switch(up_loc)
			{
			case 1:
				if(Cell_no[nLoop]==4 || Cell_no[nLoop]==7)
				{
					side_val = 2;
				}
				else
				{
					side_val = 3;
				}
				break;
			case 2:
				if(Cell_no[nLoop]==1 || Cell_no[nLoop] ==4 || Cell_no[nLoop]==7)
				{
					side_val = 2;
				}
				else
				{
					side_val = 3;
				}
				break;
			case 3:
				if(Cell_no[nLoop]==6 || Cell_no[nLoop]==9)
				{
					side_val = 3;
				}
				else
				{
					side_val = 2;
				}
				break;
			case 4:
				if(Cell_no[nLoop]==7)
				{
					side_val = 2;
				}
				else
				{
					side_val = 3;
				}
				break;
			
			case 6:
				if(Cell_no[nLoop]==9)
				{
					side_val = 3;
				}
				else
				{
					side_val = 2;
				}
				break;

			case 7:
				side_val = 3;
				break;

			case 9:
				side_val = 2;
				break;
			}
			break;

		case 9:
			switch(up_loc)
			{
			case 1:
				if(Cell_no[nLoop]==2 || Cell_no[nLoop]==3 || Cell_no[nLoop] ==6)
				{
					side_val = 3;
				}
				else
				{
					side_val = 2;
				}
				break;
			case 2:
				if(Cell_no[nLoop]==3 || Cell_no[nLoop]==6)
				{
					side_val = 3;
				}
				else
				{
					side_val = 2;
				}
				break;
			case 3:
				if(Cell_no[nLoop]==6)
				{
					side_val = 3;
				}
				else
				{
					side_val = 2;
				}
				break;
			case 4:
				if(Cell_no[nLoop]<7)
				{
					side_val = 3;
				}
				else
				{
					side_val = 2;
				}
				break;
			case 6:
				side_val = 2;
				break;
			case 7:
				if(Cell_no[nLoop]==8)
				{
					side_val = 2;
				}
				else
				{
					side_val = 3;
				}
				break;
			case 8:
				side_val = 3;
				break;
			}
			break;





	}

}

void CWeppProcess::Order_Segs(int *piSortedIndex)
{
	m_Message = "Running Order_Segs...";
	//m_ProgressCtrl.StepIt();
	//UpdateData(false);
	int* piSortedSeg = new int [nSegSize];
	double* pdValues = new double[nSegSize];

	int nLoop;
	
	bool bChanged = true;


	for(nLoop=0;nLoop<nSegSize;nLoop++)
	{
		pdValues[nLoop] = (double)(1000000-pSegs[nLoop].End_Elev*pow(10.0,10.0)) + pSegs[nLoop].Initial_Id;
		piSortedSeg[nLoop] = nLoop;
	}
	
	int nTemp=0;
	
	QuickSort(pdValues,nSegSize,piSortedSeg,true);

	for(nLoop=0;nLoop<nSegSize;nLoop++)
	{
		pSegs[piSortedSeg[nLoop]].Sort_Order = nLoop+1;
	}


	bool m_impound = false;
	int m_end_cell =0;
	int m_start_cell=0;
	int curr_seg = 0;
	int nCurrentWeppRecord = 0;
	int nCurrentSegRecord = 0;

	for(nLoop=0;nLoop<nSegSize;nLoop++)
	{
		nCurrentSegRecord = piSortedSeg[nLoop];
		m_impound = pSegs[nCurrentSegRecord].Impound;
		curr_seg = pSegs[nCurrentSegRecord].Sort_Order;
		m_end_cell = pSegs[nCurrentSegRecord].End_Cell -1;
		m_start_cell = pSegs[nCurrentSegRecord].Start_Cell -1;
		
		if (m_impound ==false)
		{
			nCurrentWeppRecord = m_start_cell;
			while(nCurrentWeppRecord!=m_end_cell)
			{
				piSegNo[nCurrentWeppRecord] = curr_seg;
				nCurrentWeppRecord = piDrec[nCurrentWeppRecord]-1;
			}
			if(piSegNo[nCurrentWeppRecord]<1)
			{
				piSegNo[nCurrentWeppRecord] = curr_seg;
			}


		}
		else
		{
			nCurrentWeppRecord = m_end_cell;
			if(piChan_No[nCurrentWeppRecord]==0 && piSegNo[nCurrentWeppRecord]<=0)
			{
				piSegNo[nCurrentWeppRecord] = curr_seg;
			}
			else
			{
				if(piSegNo[nCurrentWeppRecord] <=0)
				{
					piSegNo[nCurrentWeppRecord] = curr_seg;
				}
			}
		}
	}

	int nIndex = 0;
	int m_drain_rec =  0;
	int m_down_seg = 0;
	for(nLoop=0;nLoop<nSegSize;nLoop++)
	{
		m_end_cell = pSegs[nLoop].End_Cell-1 ;
		if(pSegs[nLoop].Start_Type !=5 && pSegs[nLoop].End_Type ==5)
		{
			m_drain_rec = m_end_cell;
			nCurrentSegRecord = 0;
			for(nIndex = 0;nIndex<nSegSize;nIndex++)
			{
				if(pSegs[nIndex].End_Cell-1 ==m_end_cell && 
					pSegs[nIndex].Start_Type ==5)
				{
					nCurrentSegRecord = nIndex;
					nIndex = nSegSize;
				}
			}
			m_down_seg = pSegs[nCurrentSegRecord].Sort_Order;
			pSegs[nLoop].Drains_Rec = m_drain_rec+1;
			pSegs[nLoop].Down_Seg = m_down_seg;

		}
		else
		{
			if(pSegs[nLoop].Start_Type ==5 && pSegs[nLoop].End_Type ==5)
			{
				m_down_seg = pSegs[nLoop].Sort_Order;
				m_drain_rec = pSegs[nLoop].End_Cell-1;
				if(pSegs[nLoop].Impound==false)
				{
					nCurrentSegRecord = -1;
					for(nIndex = 0;nIndex<nSegSize;nIndex++)
					{
						if(pSegs[nIndex].End_Cell-1 == m_drain_rec &&
							pSegs[nIndex].Impound == true)
						{
							nCurrentSegRecord = nIndex;
							nIndex = nSegSize;

						}
					}
					if(nCurrentSegRecord>=0)
					{
						m_down_seg = pSegs[nCurrentSegRecord].Sort_Order;
					}
				}
				else
				{
					nCurrentWeppRecord = m_end_cell;
					if(piDrec[nCurrentWeppRecord]-1 != nCurrentWeppRecord)
					{
						nCurrentWeppRecord = piDrec[nCurrentWeppRecord]-1;
						m_drain_rec = nCurrentWeppRecord;
						m_down_seg = piSegNo[nCurrentWeppRecord];
					}
				}
			}
			else
			{
				nCurrentWeppRecord = piDrec[m_end_cell]-1;
				m_drain_rec = nCurrentWeppRecord;
				m_down_seg = piSegNo[nCurrentWeppRecord];
			}
			
			pSegs[nLoop].Drains_Rec = m_drain_rec+1;
			pSegs[nLoop].Down_Seg = m_down_seg;

		}
	}

	delete piSortedSeg;
	delete pdValues;

}

void CWeppProcess::Redo_Ddir()
{
	m_Message = "Running Redo_Ddir...";
	//m_ProgressCtrl.StepIt();
	//UpdateData(false);
	int nCurrentRecord = 0;
	bool bProblem = false;
	int nLoop=0;
	int down_row = 0;
	int down_col =0;
	int row_diff = 0;
	int col_diff = 0;
	int cent_row = 0;
	int cent_col = 0;
	int nr = 0;
	int nc = 0;
	
	for(nLoop=0;nLoop<m_Row*m_Col;nLoop++)
	{
		piDdir[nLoop] = piDEMDdir[nLoop];
	}

	for(nLoop=0;nLoop<m_Row*m_Col;nLoop++)
	{
		if(piDrec[nLoop]!=piDEMDrec[nLoop])
		{
			nCurrentRecord = nLoop;
			bProblem = false;
			cent_row = nCurrentRecord/m_Col;
			cent_col = nCurrentRecord%m_Col;
			nCurrentRecord = piDrec[nLoop]-1;

			down_row = nCurrentRecord/m_Col;
			down_col = nCurrentRecord%m_Col;
			nCurrentRecord = nLoop;

			row_diff = down_row - cent_row;
			col_diff = down_col - cent_col;

			if(row_diff ==-1)
			{
				nr = 1;
			}
			else

			{
				if(row_diff ==0)
				{
					nr = 2;
				}
				else
				{
					if(row_diff ==1)

					{
						nr = 3;
					}
					else
					{
						bProblem = true;
					}
				}
			}

			switch(col_diff)
			{
			case (-1):
				nc = 1;
				break;
			case(0):
				nc = 2;
				break;
			case(1):
				nc = 3;
				break;
			default:
				bProblem = true;

			}
			
			if (bProblem ==false)
			{
				piDdir[nLoop] = (((3-nr)*3)+nc);
			}


		}
	}

}

void CWeppProcess::Add_Ddir()
{
	m_Message = "Running Add_Ddir...";
	//m_ProgressCtrl.StepIt();
	//UpdateData(false);
	
	for(int nLoop=0;nLoop<nSegSize;nLoop++)
	{
		pSegs[nLoop].Start_Ddir = piDdir[pSegs[nLoop].Start_Cell-1];

	}

}

void CWeppProcess::Find_UpSegs()
{

	int curr_row = 0;
	int curr_col = 0;
	int curr_seg = 0;
	int nLoop=0;

	int curr_rec = 0;
	int last_rec = 0;
	int curr_down = 0;
	int num_down = 0;
	int all_done = false;
	int upcell[10];
	int search_win[3][3];
	int cycle_win[9];

	int m_Down_Seg = 0;
	int m_UpNum = 0;
	int m_UpRec = 0 ;
	int UpSide = 0;
	int m_Start_Ddir = 0;
	int m_Start_Row =0;
	int m_Start_Col = 0;
	int m_Start_Rec = 0;
	int right_up = 0;
	int left_up = 0;
	int cent_up = 0;
	int double_in = 0;
	bool m_Impound = false;
	int m_End_Type = 0;





	m_Message = "Running Find_UpSegs...";
	//m_ProgressCtrl.StepIt();
	//UpdateData(false);



	int* piSortedIndex = new int[nSegSize];
	int* piValue = new int[nSegSize];

	//int nLoop=0;
	for(nLoop=0;nLoop<nSegSize;nLoop++)
	{
		piValue[nLoop] = pSegs[nLoop].Down_Seg;
		piSortedIndex[nLoop] = nLoop;
	}
	QuickSort(piValue,nSegSize,piSortedIndex,true);
	
	bool bChanged = true;
	int nTemp = 0;
	while(bChanged)
	{
		bChanged = false;
		for(int nLoop=0;nLoop<nSegSize-1;nLoop++)
		{
			if(pSegs[piSortedIndex[nLoop]].Down_Seg == 
				pSegs[piSortedIndex[nLoop+1]].Down_Seg && piSortedIndex[nLoop]>piSortedIndex[nLoop+1])
			{
				nTemp = piSortedIndex[nLoop];
				piSortedIndex[nLoop] = piSortedIndex[nLoop+1];
				piSortedIndex[nLoop+1] = nTemp;
				bChanged = true;

			}
		}
	}

	int N = 0;
	int nCurrentRecord = 0;
	int nCurrentSortedPointer = 0;
	int upside = 0;
	bool bEOF = false;
	
	while(all_done ==false)
	{

		for(nLoop=0;nLoop<3;nLoop++)
		{
			for(int j=0;j<3;j++)
			{
				search_win[nLoop][j]=0;
				cycle_win[nLoop*(3) + j]=0;
			}

		}
		nCurrentRecord = piSortedIndex[nCurrentSortedPointer];

		curr_down = pSegs[nCurrentRecord].Down_Seg;

		num_down = 0;
		right_up = 0;
		left_up = 0;
		cent_up = 0;
		double_in = 0;
		while(curr_down ==pSegs[nCurrentRecord].Down_Seg && bEOF ==false)
		{
			if(pSegs[nCurrentRecord].Down_Seg !=pSegs[nCurrentRecord].Sort_Order)

			{
				num_down = num_down + 1;
				upcell[num_down-1] = nCurrentRecord;
				upside = pSegs[nCurrentRecord].Sort_Order;
			}
			if(nCurrentSortedPointer<nSegSize-1)
			{
				nCurrentRecord = piSortedIndex[++nCurrentSortedPointer]; 
				last_rec = nCurrentSortedPointer;
			}
			else
			{
				bEOF = true;
			}
		}
		if(bEOF ==true) 
		{
			all_done = true;
		}
		else
		{
			last_rec = nCurrentSortedPointer;
		}

		if(num_down ==1)
		{

			m_Impound = false;
			nCurrentRecord = upcell[0];
			m_Down_Seg = pSegs[nCurrentRecord].Down_Seg;
			m_UpNum = pSegs[nCurrentRecord].Sort_Order;
			m_UpRec = pSegs[nCurrentRecord].End_Cell-1;
			m_End_Type = pSegs[nCurrentRecord].End_Type;
			if(pSegs[nCurrentRecord].Impound ==true)
			{
				m_Impound = true;
			}

			for(int j=0;j<nSegSize;j++)
			{
				if(pSegs[j].Sort_Order ==m_Down_Seg)
				{
					nCurrentRecord = j;
					j = nSegSize;
				}
			}
			if(m_Impound ==true)
			{
				pSegs[nCurrentRecord].Top_Imp = m_UpNum;
			}
			pSegs[nCurrentRecord].Center_Seg=m_UpNum;
			pSegs[nCurrentRecord].Center_Rec = m_UpRec+1;

		}

		if(num_down >1)
		{


			nCurrentRecord = upcell[0];
			m_Down_Seg = pSegs[nCurrentRecord].Down_Seg;

			for(int j=0;j<nSegSize;j++)
			{
				if(pSegs[j].Sort_Order ==m_Down_Seg)
				{
					nCurrentRecord = j;
					j = nSegSize;
				}
			}

			m_Start_Ddir = pSegs[nCurrentRecord].Start_Ddir;
			m_Start_Row = pSegs[nCurrentRecord].Start_Row-1;
			m_Start_Col = pSegs[nCurrentRecord].Start_Col-1;
		
			search_win [1][1] = pSegs[nCurrentRecord].Sort_Order;

			switch(m_Start_Ddir)
			{
			case (1):
				search_win[2][0] = pSegs[nCurrentRecord].Sort_Order;
				break;
			case (2):
				search_win[2][1] = pSegs[nCurrentRecord].Sort_Order;
				break;
			case (3):
				search_win[2][2] = pSegs[nCurrentRecord].Sort_Order;
				break;
			case (4):
				search_win[1][0] = pSegs[nCurrentRecord].Sort_Order;
				break;
			case (5):
				search_win[1][1] = pSegs[nCurrentRecord].Sort_Order;
				break;
			case (6):
				search_win[1][2] = pSegs[nCurrentRecord].Sort_Order;
				break;

			case (7):
				search_win[0][0] = pSegs[nCurrentRecord].Sort_Order;
				break;
			case (8):
				search_win[0][1] = pSegs[nCurrentRecord].Sort_Order;
				break;
			case (9):
				search_win[0][2] = pSegs[nCurrentRecord].Sort_Order;
				break;
			}

			for(N=0;N<num_down;N++)
			{
				nCurrentRecord = upcell[N];
				if(pSegs[nCurrentRecord].End_Type !=5)
				{
					curr_row = (pSegs[nCurrentRecord].End_Row -2 - m_Start_Row) + 2;
					curr_col = (pSegs[nCurrentRecord].End_Col -2 - m_Start_Col) + 2;
					curr_seg = pSegs[nCurrentRecord].Sort_Order;

					if(search_win[curr_row][curr_col]==0)
					{
						search_win[curr_row][curr_col] = pSegs[nCurrentRecord].Sort_Order ;
					}
					else
					{
						double_in = pSegs[nCurrentRecord].Sort_Order;
					}


				}
				else
				{
					if(N==0)
					{
						search_win[0][0] = pSegs[nCurrentRecord].Sort_Order ;
					}
					if(N==1)
					{
						search_win[0][2] = pSegs[nCurrentRecord].Sort_Order ;

					}
					if(N==2)
					{
						search_win[2][2] = pSegs[nCurrentRecord].Sort_Order ;

					}
				}
			}


			switch(m_Start_Ddir)
			{
			case (1):
				cycle_win[0]= search_win[1][0];
				cycle_win[1]= search_win[0][0];
				cycle_win[2]= search_win[0][1];
				cycle_win[3]= search_win[0][2];			
				cycle_win[4]= search_win[1][2];
				cycle_win[5]= search_win[2][2];			
				cycle_win[6]= search_win[2][1];
				cycle_win[7]= search_win[2][0];
				break;
			case (2):
				cycle_win[0]= search_win[2][0];
				cycle_win[1]= search_win[1][0];
				cycle_win[2]= search_win[0][0];
				cycle_win[3]= search_win[0][1];			
				cycle_win[4]= search_win[0][2];
				cycle_win[5]= search_win[1][2];			
				cycle_win[6]= search_win[2][2];
				cycle_win[7]= search_win[2][1];
				break;

			case (3):
				cycle_win[0]= search_win[2][1];
				cycle_win[1]= search_win[2][0];
				cycle_win[2]= search_win[1][0];
				cycle_win[3]= search_win[0][0];			
				cycle_win[4]= search_win[0][1];
				cycle_win[5]= search_win[0][2];			
				cycle_win[6]= search_win[1][2];
				cycle_win[7]= search_win[2][2];			
				break;			
			
			case(4):
				cycle_win[0]= search_win[0][0];
				cycle_win[1]= search_win[0][1];
				cycle_win[2]= search_win[0][2];
				cycle_win[3]= search_win[1][2];			
				cycle_win[4]= search_win[2][2];
				cycle_win[5]= search_win[2][1];			
				cycle_win[6]= search_win[2][0];
				cycle_win[7]= search_win[1][0];			
				break;
			
			
			case (5):
				cycle_win[0]= search_win[2][0];
				cycle_win[1]= search_win[1][0];
				cycle_win[2]= search_win[0][0];
				cycle_win[3]= search_win[0][1];			
				cycle_win[4]= search_win[0][2];
				cycle_win[5]= search_win[1][2];			
				cycle_win[6]= search_win[2][2];
				cycle_win[7]= search_win[2][1];
				break;
			case (6):
				cycle_win[0]= search_win[2][2];
				cycle_win[1]= search_win[2][1];
				cycle_win[2]= search_win[2][0];
				cycle_win[3]= search_win[1][0];			
				cycle_win[4]= search_win[0][0];
				cycle_win[5]= search_win[0][1];			
				cycle_win[6]= search_win[0][2];
				cycle_win[7]= search_win[1][2];
				break;

			case (7):
				cycle_win[0]= search_win[0][1];
				cycle_win[1]= search_win[0][2];
				cycle_win[2]= search_win[1][2];
				cycle_win[3]= search_win[2][2];			
				cycle_win[4]= search_win[2][1];
				cycle_win[5]= search_win[2][0];			
				cycle_win[6]= search_win[1][0];
				cycle_win[7]= search_win[0][0];			
				break;			
			
			case(8):
				cycle_win[0]= search_win[0][2];
				cycle_win[1]= search_win[1][2];
				cycle_win[2]= search_win[2][2];
				cycle_win[3]= search_win[2][1];			
				cycle_win[4]= search_win[2][0];
				cycle_win[5]= search_win[1][0];			
				cycle_win[6]= search_win[0][0];
				cycle_win[7]= search_win[0][1];			
				break;			
			
			case(9):
				cycle_win[0]= search_win[1][2];
				cycle_win[1]= search_win[2][2];
				cycle_win[2]= search_win[2][1];
				cycle_win[3]= search_win[2][0];			
				cycle_win[4]= search_win[1][0];
				cycle_win[5]= search_win[0][0];			
				cycle_win[6]= search_win[0][1];
				cycle_win[7]= search_win[0][2];			
				break;				
			}

			if(m_Start_Ddir!=-5)
			{
				for(N=0;N<7;N++)
				{
					if(cycle_win[N]>0)
					{
						if(right_up ==0)
						{
							right_up = cycle_win[N];
						}
						else
						{
							if(cent_up==0)
							{
								cent_up = cycle_win[N];
							}
							else
							{
								if(left_up==0 && num_down>2)
								{
									left_up = cycle_win[N];
								}
								else
								{
									left_up = double_in;
								}
							}
						}

					}


				}
				


			}
			else
			{
				for(N=0;N<8;N++)
				{
					if(cycle_win[N]>0)
					{
						if(right_up==0)
						{
							right_up = cycle_win[N];
						}
						else
						{
							if(cent_up==0)
							{
								cent_up = cycle_win[N];
							}
							else
							{
								if(left_up==0 && num_down>2)
								{
									if(double_in==0)
									{
										left_up = cycle_win[N];
									}
									else
									{
										left_up = double_in;
									}
								}
							}
						}
					}
				}
			}

			if(left_up ==0 && num_down ==2)
			{
				left_up = cent_up;
				cent_up = 0;
			}

			if(double_in>0)
			{
				left_up =double_in;
			}

			if(right_up>0)

			{
				m_Impound = false;
			
				for(int nLoop=0;nLoop<nSegSize;nLoop++)
				{
					if(pSegs[nLoop].Sort_Order == right_up)
					{
						nCurrentRecord = nLoop;
						nLoop = nSegSize;
					}

				}

				m_Down_Seg = pSegs[nCurrentRecord].Down_Seg;
				m_UpNum = pSegs[nCurrentRecord].Sort_Order;
				m_UpRec = pSegs[nCurrentRecord].End_Cell-1;
				m_End_Type = pSegs[nCurrentRecord].End_Type;

				if(pSegs[nCurrentRecord].Impound ==true)
				{
					m_Impound = true;
				}

				for(nLoop=0;nLoop<nSegSize;nLoop++)
				{
					if(pSegs[nLoop].Sort_Order ==m_Down_Seg)
					{
						nCurrentRecord = nLoop;
						nLoop = nSegSize;
					}
				}
				
				pSegs[nCurrentRecord].Right_Seg = m_UpNum;
				pSegs[nCurrentRecord].Right_Rec = m_UpRec + 1;
				if(m_Impound ==true)
				{
					pSegs[nCurrentRecord].Right_Imp = m_UpNum;
				}
			}

			if(left_up>0)
			{
				m_Impound = false;
				for(nLoop=0;nLoop<nSegSize;nLoop++)
				{
					if(pSegs[nLoop].Sort_Order == left_up)
					{
						nCurrentRecord = nLoop;
						nLoop = nSegSize;

					}
				}
				m_Down_Seg = pSegs[nCurrentRecord].Down_Seg;
				m_UpNum = pSegs[nCurrentRecord].Sort_Order;
				m_UpRec = pSegs[nCurrentRecord].End_Cell-1;
				m_End_Type = pSegs[nCurrentRecord].End_Type;

				if(pSegs[nCurrentRecord].Impound ==true)
				{
					m_Impound = true;
				}
				for(nLoop=0;nLoop<nSegSize;nLoop++)
				{
					if(pSegs[nLoop].Sort_Order ==m_Down_Seg)
					{
						nCurrentRecord = nLoop;
						nLoop = nSegSize;
					}
				}

				pSegs[nCurrentRecord].Left_Seg = m_UpNum;
				pSegs[nCurrentRecord].Left_Rec = m_UpRec+1;

				if(m_Impound==true)
				{
					pSegs[nCurrentRecord].Left_Imp = m_UpNum;
				}

			}


			if(cent_up>0 && num_down >2)
			{
				m_Impound = false;
				for(nLoop=0;nLoop<nSegSize;nLoop++)
				{
					if(pSegs[nLoop].Sort_Order ==cent_up)
					{
						nCurrentRecord = nLoop;
						nLoop = nSegSize;
					}
				}		
				
			

				m_Down_Seg = pSegs[nCurrentRecord].Down_Seg;
				m_UpNum = pSegs[nCurrentRecord].Sort_Order;
				m_UpRec = pSegs[nCurrentRecord].End_Cell-1;
				m_End_Type = pSegs[nCurrentRecord].End_Type;

				if(pSegs[nCurrentRecord].Impound==true)
				{
					m_Impound = true;
				}
				
				for(nLoop=0;nLoop<nSegSize;nLoop++)
				{
					if(pSegs[nLoop].Sort_Order == m_Down_Seg)
					{
						nCurrentRecord = nLoop;
						nLoop = nSegSize;
					}
				}
				
				pSegs[nCurrentRecord].Center_Seg = m_UpNum;
				pSegs[nCurrentRecord].Center_Rec = m_UpRec+1;
				if(m_Impound==true)
				{
					pSegs[nCurrentRecord].Top_Imp = m_UpNum;
				}
			}

		}


	if(num_down ==0)
	{
		last_rec = nCurrentSortedPointer;
	}
	nCurrentSortedPointer  = last_rec;
	}
	
	delete piSortedIndex;
	delete piValue;
	//exit(0);
}

void CWeppProcess::Hill_Sheds(int *piSortedIndex, int* piShedNo, short int* piShedSide, int* piHill_No)
{
	//piShedNo = new int[m_Row*m_Col];
	

	m_Message = "Running Hill_Sheds....";
	//m_ProgressCtrl.StepIt();
	//UpdateData(false);

	int j =0;
	int numdn = 0;
	int curr_shed =0;

	int nLoop=0;

	for(nLoop=0;nLoop<m_Row*m_Col;nLoop++)
	{
		if(piSegNo[nLoop]<1)
		{
			piSegNo[nLoop]=0;
		}
		piShedNo[nLoop]=0;
		piShedSide[nLoop]=0;
	}
	
	int start_rec = 0;
	int num_dn = 0;
	bool go_dn = true;
	bool bMissing = false;
	
	int nCurrentRecord = 0;
	int nSortedPointer = 0;
	int curr_side = 0;





	
	for(nLoop=0;nLoop<m_Row*m_Col;nLoop++)
	{
		nCurrentRecord = piSortedIndex[nLoop];
		start_rec = nCurrentRecord;
		num_dn = 0;
		go_dn = true;
		if(pdElev[nCurrentRecord]!=m_MissingData)
		{
			while(go_dn)
			{
				if(piChan_Side[nCurrentRecord]>0 && piSegNo[nCurrentRecord]==0)
				{
					curr_side = piChan_Side[nCurrentRecord];
				}
				if(piSegNo[nCurrentRecord]>0)
				{
					curr_shed = piSegNo[nCurrentRecord];
					go_dn = false;
				}
				else
				{
					if(piShedNo[nCurrentRecord]>0 && piSegNo[nCurrentRecord] ==0)
					{
						curr_shed = piShedNo[nCurrentRecord];
						curr_side = piShedSide[nCurrentRecord];
						go_dn = false;
					}
					else
					{
						if(piShedNo[nCurrentRecord]==0)
						{
							if(piSegNo[nCurrentRecord]==0)
							{
								//Added this line below because if we hit cell with ddir 5, we
								//will never exit.
								if(nCurrentRecord!=piDrec[nCurrentRecord]-1)
								{

									nCurrentRecord = piDrec[nCurrentRecord]-1;
									num_dn = num_dn + 1;
								}
								else
								{
									go_dn = false;
								}

							}
							else
							{
								go_dn = false;
							}
						}
						else
						{
							if(piDdir[nCurrentRecord]==5)
							{
								go_dn = false;
							}
						}
					}
				}
			}

			if(num_dn>0)
			{
				nCurrentRecord = start_rec;
				if(piSegNo[nCurrentRecord]==0)
				{
					piShedNo[nCurrentRecord]=curr_shed;
					piShedSide[nCurrentRecord]=curr_side;
				}

				for(j=0;j<num_dn;j++)
				{
					if(piSegNo[nCurrentRecord]==0)
					{
						nCurrentRecord = piDrec[nCurrentRecord]-1;
						piShedNo[nCurrentRecord] = curr_shed;
						piShedSide[nCurrentRecord] = curr_side;
					}
					else
					{
						nCurrentRecord = piDrec[nCurrentRecord]-1;
					}
				}

			}
		}
		else
		{
			piShedNo[nCurrentRecord]=0;
			piShedSide[nCurrentRecord]=0;
		}

	}


	int* anValues = new int [m_Row*m_Col];
	int* nSorted = new int [m_Row*m_Col];

	for(nLoop=0;nLoop<m_Row*m_Col;nLoop++)
	{
		if(piSegNo[nLoop]>0)
		{
			piShedNo[nLoop] = 0;
			piShedSide[nLoop]=0;
		}
		piHill_No[nLoop] = 0;
	}

	int nIndex = 0;
	int nMissingValue = 0;
	for(nLoop=0;nLoop<m_Row*m_Col;nLoop++)
	{
		if(pdElev[nLoop]!=m_MissingData)
		{
			anValues[nIndex] = piShedNo[nLoop]*10 + piShedSide[nLoop];
			nSorted[nIndex] = nLoop;
			nIndex++;
		}
		else
		{
			nMissingValue = nLoop;

		}
	}
	QuickSort(anValues,nIndex,nSorted,true);

	for(nLoop=nIndex;nLoop<m_Row*m_Col;nLoop++)
	{
		anValues[nLoop]=0;
		nSorted[nLoop]=nMissingValue;
	}
	
	
	nCurrentRecord = nSorted[0];
	nSortedPointer = 0;
	int curr_hillno = 0;
	curr_shed = piShedNo[nCurrentRecord];
	curr_side = piShedSide[nCurrentRecord];
	bool bEOF = false;
	for(nLoop=0;nLoop<m_Row*m_Col;)
	{
		

		while(piShedNo[nCurrentRecord]==0 && bEOF ==false)
		{
			
			nLoop++;
			
			
			if(nLoop>=m_Row*m_Col)
			{
				bEOF = true;
			}
			else
			{
				nCurrentRecord = nSorted[nLoop];
			}
		}
		if(bEOF ==false)
		{
			curr_hillno = curr_hillno + 1;
			curr_shed = piShedNo[nCurrentRecord];
			curr_side = piShedSide[nCurrentRecord];
			while(piShedNo[nCurrentRecord]==curr_shed && piShedSide[nCurrentRecord]==curr_side)
			{
				piHill_No[nCurrentRecord] = curr_hillno;
				nLoop++;
				if(nLoop>=m_Row*m_Col)
				{
					curr_side = -1;
				}
				else
				{
					nCurrentRecord = nSorted[nLoop];
				}
				
			}
		}
	}

	nMax_Hills = curr_hillno;



	delete anValues;
	delete nSorted;
}

void CWeppProcess::Renum_Segs(int* piShedNo)
{
	m_Message = "Running Renum_Segs...";
	//m_ProgressCtrl.StepIt();
	//UpdateData(false);
	int nLoop=0;

	for(nLoop=0;nLoop<m_Row*m_Col;nLoop++)
	{
		if(piSegNo[nLoop]>0)
		{
			piSegNo[nLoop]= piSegNo[nLoop] + nMax_Hills;
		}
		if(piShedNo[nLoop]>0)
		{
			piShedNo[nLoop]= piShedNo[nLoop] + nMax_Hills;
		}
	}
	
	Segs* pSeg;
	for(nLoop=0;nLoop<nSegSize;nLoop++)
	{
		pSeg = &pSegs[nLoop];
		pSeg->Final_Id = pSeg->Sort_Order + nMax_Hills;
		if(pSeg->Down_Seg>0)
		{
			pSeg->Down_Seg = pSeg->Down_Seg + nMax_Hills;
		}
		if(pSeg->Left_Seg >0)
		{
			pSeg->Left_Seg = pSeg->Left_Seg + nMax_Hills;
		}
		if(pSeg->Right_Seg >0)
		{
			pSeg->Right_Seg = pSeg->Right_Seg + nMax_Hills;
		}
		if(pSeg->Center_Seg >0)
		{
			pSeg->Center_Seg = pSeg->Center_Seg + nMax_Hills;
		}
		if(pSeg->Left_Imp > 0)
		{
			pSeg->Left_Imp = pSeg->Left_Imp + nMax_Hills;
		}
		if(pSeg->Right_Imp>0)
		{
			pSeg->Right_Imp = pSeg->Right_Imp + nMax_Hills;
		}
		if(pSeg->Top_Imp>0)
		{
			pSeg->Top_Imp = pSeg->Top_Imp + nMax_Hills;
		}
	}
}

void CWeppProcess::Build_Stru(int* piShedNo, int* piHillNo, short int* piShedSide)
{


	m_Message  = "Running Build_Stru...";
	//m_ProgressCtrl.StepIt();
	//UpdateData(false);
	int* anValues = new int[m_Row*m_Col];
	int* nSorted = new int[m_Row*m_Col];
	int nLoop;
	int nIndex = 0;
	int nMissingValue = 0;
	for(nLoop=0;nLoop<m_Row*m_Col;nLoop++)
	{
		if(pdElev[nLoop]!=m_MissingData)
		{
			anValues[nIndex] = piShedNo[nLoop]*10 + piShedSide[nLoop];
			nSorted[nIndex] = nLoop;
			nIndex++;
		}
		else
		{
			nMissingValue = nLoop;

		}
	}
	QuickSort(anValues,nIndex,nSorted,true);

	for(nLoop=nIndex;nLoop<m_Row*m_Col;nLoop++)
	{
		anValues[nLoop]=0;
		nSorted[nLoop]=nMissingValue;
	}
	
	int nCurrentRecord=0;
	int nSortedPointer = 0;
	int curr_hillno = 0;
	int curr_shed = 0;
	int curr_side = 0;
	bool all_done = false;
	int j = 0;

	for(nLoop=0;nLoop<m_Row*m_Col;)
	{
		nCurrentRecord = nSorted[nLoop];
		curr_hillno = piHillNo[nCurrentRecord];
		curr_side = piShedSide[nCurrentRecord];
		curr_shed = piShedNo[nCurrentRecord];

		for(j=0;j<nSegSize;j++)
		{
			if(pSegs[j].Final_Id ==curr_shed)
			{
				if(curr_side ==1)
				{
					pSegs[j].Top_Hill = curr_hillno;
				}
				if(curr_side ==2)
				{
					pSegs[j].Right_Hill = curr_hillno;
				}
				if(curr_side ==3)
				{
					pSegs[j].Left_Hill = curr_hillno;
				}
				j = nSegSize;
			}
		}

		while(piHillNo[nCurrentRecord]==curr_hillno)
		{
			nLoop++;
			if(nLoop>=m_Row*m_Col)
			{
				curr_hillno = -1;
				all_done = true;
			}
			else
			{
				nCurrentRecord = nSorted[nLoop];
			}
		}
	}




	delete nSorted;
	int* pFinalOrd = new int[nSegSize];
	for(nLoop=0;nLoop<nSegSize;nLoop++)
	{

		anValues[nLoop] = pSegs[nLoop].Final_Id ;
		pFinalOrd[nLoop] = nLoop;

		
	}
	QuickSort(anValues,nLoop,pFinalOrd,true);
	delete anValues;
	int m_element_no = 0;
	int m_left_hill = 0;
	int m_right_hill = 0;
	int m_top_hill = 0;
	int m_left_chan = 0;
	int m_right_chan = 0;
	int m_top_chan = 0;
	int m_left_imp =0;
	int m_right_imp = 0;
	int m_top_imp = 0;
	int m_ele_type = 0;
	char* m_comment=new char [30];



	pStructure =new Structure[1000];
	Segs* pSeg;
	char strTemp[30];
	for(nLoop=0;nLoop<nSegSize;nLoop++)
	{
		pSeg = & pSegs[nLoop];
		m_element_no = pSeg->Final_Id;
		m_left_hill = pSeg->Left_Hill;
		m_right_hill = pSeg->Right_Hill;
		m_top_hill = pSeg->Top_Hill;
		m_left_chan = 0;
		m_right_chan = 0;
		m_top_chan = 0;
		m_left_imp =0;
		m_right_imp = 0;
		m_top_imp = 0;
		m_ele_type = 0;

		if(pSeg->Impound)
		{
			m_ele_type = 3;
			_itoa( m_element_no, strTemp ,10);
			strcpy(m_comment , "Element No: ");
			strcat(m_comment,strTemp);
			strcat(m_comment," impoundment");

			if(pSeg->Left_Seg !=pSeg->Left_Imp)
			{
				m_left_chan = pSeg->Left_Seg;
			}
			else
			{
				m_left_chan = 0;
				m_left_imp = pSeg->Left_Imp;
			}

			if(pSeg->Right_Seg!=pSeg->Right_Imp)
			{
				m_right_chan = pSeg->Right_Seg;
			}
			else
			{
				m_right_chan = 0;
				m_right_imp = pSeg->Right_Imp;
			}
			if(pSeg->Center_Seg!=pSeg->Top_Imp)
			{
				m_top_chan = pSeg->Center_Seg;
			}
			else
			{
				m_top_chan = 0;
				m_top_imp = pSeg->Top_Imp;
			}


		}
		else
		{
			m_ele_type = 2;
			_itoa( m_element_no, strTemp ,10);
			strcpy(m_comment , "Element No: ");
			strcat(m_comment,strTemp);
			strcat(m_comment," channel");

			if(pSeg->Left_Seg !=pSeg->Left_Imp)
			{
				m_left_chan = pSeg->Left_Seg;
			}
			else
			{
				m_left_chan = 0;
				m_left_imp = pSeg->Left_Imp;
			}
			if(pSeg->Right_Seg!=pSeg->Right_Imp)
			{
				m_right_chan = pSeg->Right_Seg;
			}
			else
			{
				m_right_chan = 0;
				m_right_imp = pSeg->Right_Imp;
			}
			if(pSeg->Center_Seg!=pSeg->Top_Imp)
			{
				m_top_chan = pSeg->Center_Seg;
			}
			else
			{
				m_top_chan = 0;
				m_top_imp = pSeg->Top_Imp;
			}

		}
	
		if(nStructureSize>0 && (nStructureSize%1000==0))
		{
			Structure* pTemp = new Structure [nStructureSize + 1000];
			for(int j=0;j<nStructureSize;j++)
			{
				pTemp[j] = pStructure[j];
			}
			delete pStructure;
			pStructure = pTemp;
		}

		pStructure[nStructureSize].Element_No = m_element_no;
		pStructure[nStructureSize].Ele_Type = m_ele_type;
		pStructure[nStructureSize].Left_Hill = m_left_hill;
		pStructure[nStructureSize].Right_Hill = m_right_hill;
		pStructure[nStructureSize].Top_Hill = m_top_hill;
		pStructure[nStructureSize].Left_Chan = m_left_chan;
		pStructure[nStructureSize].Right_Chan = m_right_chan;
		pStructure[nStructureSize].Top_Chan = m_top_chan;
		pStructure[nStructureSize].Left_Imp = m_left_imp;
		pStructure[nStructureSize].Right_Imp = m_right_imp;
		pStructure[nStructureSize].Top_Imp = m_top_imp;
		strcpy(pStructure[nStructureSize].Comment, m_comment);
		nStructureSize++;

	}
	delete m_comment;
	delete pFinalOrd;
}

void CWeppProcess::WriteDbf()
{

	CFile myFile(m_FileName + "DEMOUT2.dbf",CFile::modeCreate | CFile::modeWrite );
	char* szBuffer = new char[2555];

	//Type of file
	//3 represents fox pro.
	szBuffer[0]  = 3;
	myFile.Seek(0,CFile::begin);
	myFile.Write(szBuffer,1);
	//char* p ;
	
	//Last Updated(YYMMDD)
	//This can be changed to be dinamic. Very easy just get todays date.
	myFile.Seek(1,CFile::begin);
	szBuffer[0] = 01;
	szBuffer[1] = 7;
	szBuffer[2] = 11;
	myFile.Write(szBuffer,3);
	
	//Number of records in the file.
	myFile.Seek(4,CFile::begin);
	int nNumberOfRecords = 0;
	
	int nNum = 0;
	
	szBuffer[0] = 0;
	szBuffer[1]=0;
	szBuffer[2]=0;
	szBuffer[3] = 0;
	int n = 500000*15;
	//Using memcpy function we can convert integer value to the char(byte)
	memcpy(szBuffer,&n,4);
	int* pTemp = (int*)szBuffer;
	myFile.Write(szBuffer,4);

	szBuffer[0] = 65;
	szBuffer[1]=0;

	//location of the first record.
	//this will be 32 bytes plus (number of fields * 32 bytes, because field subrecord is 32 bytes each) 
	//plus 1.
	myFile.Seek(8,CFile::begin);
	//_itoa(nNum,szBuffer,10);
	myFile.Write(szBuffer,2);
	
	szBuffer[0]=11;
	szBuffer[1]=0;

	//lenght of the record. This is lenght sum of all the fields.
	myFile.Seek(10,CFile::begin);
	myFile.Write(szBuffer,2);
	
	//Code page mark. Not sure what it is used for, but if not set, fox pro will ask for it
	myFile.Seek(29,CFile::begin);
	szBuffer[0] = 3;
	myFile.Write(szBuffer,1);
	
	//field subrecord
	//Description of each field in the table(type, lenght, decimals, etc.)
	myFile.Seek(32,CFile::begin);
	for(int nLoop=0;nLoop<1;nLoop++)
	{
		strcpy(szBuffer,"Field_One");
		myFile.Write(szBuffer,10);

		myFile.Seek(43,CFile::begin);
		strcpy(szBuffer,"N");
		myFile.Write(szBuffer,1);

		myFile.Seek(44,CFile::begin);
		//_itoa(0,szBuffer,10);
		szBuffer[0] = 10;
		szBuffer[1]=0;
		szBuffer[2]=0;
		szBuffer[3]=0;
		myFile.Write(szBuffer,4);
		
		myFile.Seek(48,CFile::begin);
		//_itoa(10,szBuffer,10);
		myFile.Write(szBuffer,1);

		myFile.Seek(49,CFile::begin);
		//_itoa(0,szBuffer,10);
		szBuffer[0] = 0;
		myFile.Write(szBuffer,1);

	}

	myFile.Seek(64,CFile::begin);
	szBuffer[0] = 13;
	myFile.Write(szBuffer,1);

	szBuffer[0] = 0;
	myFile.Seek(65,CFile::begin);
	myFile.Write(szBuffer,1);
	



	int nIndex = 0;
	myFile.Seek(65,CFile::begin);
	char* pszTemp = new char [255];
	int nLenght = 0;
	for(int j=0;j<500000;j++)
	{
		szBuffer[0]='\0';

		myFile.Seek(1,CFile::current);
		for(int nLoop2=0;nLoop2<15;nLoop2++)
		{
			_itoa(nIndex,pszTemp,10);
			nLenght = strlen(pszTemp);
			for(int nLoop=0;nLoop<11-nLenght;nLoop++)
			{
				strcat(pszTemp," ");
			}
			strcat(szBuffer,pszTemp);
			nIndex++;
			
		}
		myFile.Write(szBuffer,164);


	}
	szBuffer[0]=26;
	myFile.Write(szBuffer,1);
	myFile.Close();
	exit(1);
}

void CWeppProcess::Wepp_Form(int *piSortedIndex)
{

	m_Message = "Running Wepp_Form...";
	//m_ProgressCtrl.StepIt();
	//UpdateData(false);

	int rowno = 1;
	int colno = 1;
	int start2rec = 1;
	int start3rec = 1;
	int start4rec = 1;
	int no_rows = 1;
    int no_cols = 1;

	int d = m_Size;
	int twod = 2*d;
	int dsquared = d*d;

	no_rows = m_Row-1;
	no_cols = m_Col-1;
	int lastrec = m_Row*m_Col-1;

	int this_row = 0;
	int this_col = 0;
	
	rowno = 1;

	int nCurrentRecord = 0;

	int nLoop=0;

	bool bOne_Missing=false;
	pfSlope = new float [ m_Row*m_Col];
	piAspect = new short int[m_Row*m_Col];
	

	double z11=0;
	double z12 = 0;
	double z13 = 0;
	double z21 = 0;
	double z22 = 0;
	double z23 = 0;
	double z31 = 0;
	double z32 = 0;
	double z33 = 0;
	
	double slope_in_x = 0;
	double slope_in_y = 0;
	double slope_rad=0;
	double dlope_deg=0;
	double slope_pct=0;
	double slope_deg = 0;
	double this_slope=0;
	double local_angle = 0;
	double azimuth = 0;
	bool bFound = false;
	double prof_azimuth = 0;
	//double this_slope =0;

	for(nLoop=0;nLoop<m_Row*m_Col;nLoop++)
	{
		nCurrentRecord = nLoop;


		this_row = nCurrentRecord/m_Col;
		this_col = nCurrentRecord%m_Col;

		if(pdElev[nCurrentRecord]<0)
		{
			pfSlope[nCurrentRecord]=0;
			piAspect[nCurrentRecord]=0;
		}
		else
		{
			if(this_col ==0 ||this_col ==no_cols)
			{
				pfSlope[nCurrentRecord]=0;
				piAspect[nCurrentRecord]=0;
			}
			else
			{
				if(this_row==0 ||this_row ==no_rows)
				{
					pfSlope[nCurrentRecord]=0;
					piAspect[nCurrentRecord]=0;
				}
				else
				{


//Eyton Slope
					bOne_Missing = false;
					if(pdElev[nCurrentRecord-1]!=m_MissingData)
					{
						z21 = pdElev[nCurrentRecord-1]*100;

					}
					else
					{
						bOne_Missing = true;
					}

					if(pdElev[nCurrentRecord]!=m_MissingData)
					{
						z22 = pdElev[nCurrentRecord]*100;
					}
					else
					{
						bOne_Missing = true;
					}
					
					if(pdElev[nCurrentRecord +1]!=m_MissingData)
					{
						z23 = pdElev[nCurrentRecord + 1]*100;
					}
					else
					{
						bOne_Missing = true;
					}

					if(pdElev[nCurrentRecord-m_Col-1]!=m_MissingData)
					{
						z11= pdElev[nCurrentRecord-m_Col-1]*100;
					}
					else
					{
						bOne_Missing = true;
					}

					if(pdElev[nCurrentRecord-m_Col]!=m_MissingData)
					{
						z12 = pdElev[nCurrentRecord-m_Col]*100;
					}
					else
					{
						bOne_Missing = true;
					}

					if(pdElev[nCurrentRecord-m_Col + 1]!=m_MissingData)
					{
						z13 = pdElev[nCurrentRecord-m_Col+1]*100;
					}
					else
					{
						bOne_Missing = true;
					}

					if(pdElev[nCurrentRecord + m_Col -1]!=m_MissingData)
					{
						z31 = pdElev[nCurrentRecord +m_Col -1]*100;
					}
					else
					{
						bOne_Missing = true;
					}

					if(pdElev[nCurrentRecord + m_Col] !=m_MissingData)
					{
						z32 = pdElev[nCurrentRecord +m_Col]*100;
					}
					else
					{
						bOne_Missing = true;
					}

					if(pdElev[nCurrentRecord + m_Col+1]!=m_MissingData)
					{
						z33 = pdElev[nCurrentRecord + m_Col + 1]*100;
					}
					else
					{
						bOne_Missing = true;
					}

					if(bOne_Missing ==false)
					{
						bFound = false;
						slope_in_x = (z23-z21)/twod;
						slope_in_y = (z32 - z12)/twod;
						this_slope = sqrt(pow(slope_in_x,2) + pow(slope_in_y,2));
						slope_pct = this_slope;
						slope_rad = atan(this_slope/100);
						slope_deg = (slope_rad/(2*3.141592))*360;
						if(this_slope>0)
						{
							local_angle = fabs(slope_in_x);
							
							local_angle =acos(local_angle/this_slope);
							local_angle = (local_angle/(2*3.141592))*360;

							if(slope_in_x >0 && slope_in_y >0)
							{
								azimuth = 270 + local_angle;
								bFound = true;
							}

							if(slope_in_x >0 && slope_in_y <0)
							{
								azimuth = 270-local_angle;
								bFound = true;
							}

							if(slope_in_x<0 && slope_in_y >0)
							{
								azimuth = 90-local_angle;
								bFound = true;
							}

							if(slope_in_x <0 && slope_in_y <0)
							{
								azimuth = 90+ local_angle;
								bFound = true;
							}

							if(slope_in_y ==0 && slope_in_x <0)
							{
								azimuth = 90;
								bFound = true;
							}

							if(slope_in_y ==0 && slope_in_x >0)
							{
								azimuth =  270;
								bFound = true;
							}
							
							if(slope_in_x ==0 && slope_in_y<0)
							{
								azimuth = 180;
								bFound = true;
							}

							if(slope_in_x ==0 && slope_in_y>0)
							{
								azimuth = 360;
								bFound = true;
							}
							if(bFound ==false)
							{
								azimuth = 361;
							}

							prof_azimuth = azimuth;

						}
						else
						{
							this_slope = 0;
							slope_pct = 0;
							slope_deg = 0;
							local_angle = 360;
							azimuth = 360;
						}



					}
					else
					{
						slope_pct =  0;
						slope_deg = 0;
						azimuth = 360;
					}
					pfSlope[nCurrentRecord]= (float)slope_pct;
					piAspect[nCurrentRecord]=(short int)fabs(azimuth +0.5);
				}
			}

		}
	}


	int nRow=1;
	int nCol = 0;


	for(nRow=1;nRow<m_Row-1;nRow++)
	{
		pfSlope[nRow*m_Col + nCol] = pfSlope[nRow*m_Col + nCol +1] ;
		piAspect[nRow*m_Col + nCol] = piAspect[nRow*m_Col + nCol +1];

	}

	nCol = m_Col-1;
	for(nRow=1;nRow<m_Row-1;nRow++)
	{
		pfSlope[nRow*m_Col + nCol] = pfSlope[nRow*m_Col + nCol -1] ;
		piAspect[nRow*m_Col + nCol] = piAspect[nRow*m_Col + nCol -1];

	}
	
	nRow = 0;
	for(nCol=0;nCol<m_Col;nCol++)
	{
		pfSlope[nRow*m_Col + nCol] = pfSlope[(nRow + 1) *m_Col + nCol];
		piAspect[nRow*m_Col + nCol] = piAspect[(nRow+1)*m_Col + nCol];
	}


	nRow = m_Row-1;
	for(nCol=0;nCol<m_Col;nCol++)
	{
		pfSlope[nRow*m_Col + nCol] = pfSlope[(nRow - 1) *m_Col + nCol];
		piAspect[nRow*m_Col + nCol] = piAspect[(nRow-1)*m_Col + nCol];
	}
}

void CWeppProcess::Wepp_Len()
{
	m_Message = "Running Wepp_Len...";
	//m_ProgressCtrl.StepIt();
	//UpdateData(false);

	int d = m_Size;

	pfL2St = new float[m_Row*m_Col];
	pfZ2St = new float[m_Row*m_Col];
	piN2St = new int[m_Row*m_Col];

	int nLoop=0;

	for(nLoop=0;nLoop<m_Row*m_Col;nLoop++)
	{
		pfL2St[nLoop]=0;
		pfZ2St[nLoop]=0;
		piN2St[nLoop]=0;
	}

	int* piSortedIndex = new int[m_Row*m_Col];
	double* pdValues = new double[m_Row*m_Col];
	int nIndex = 0;
	int* piMissingValues = new int[m_Row*m_Col];

	
	nIndex = 0;
	int nIndex2 = 0;
	for(nLoop=0;nLoop<m_Row*m_Col;nLoop++)
	{
		if(pdElev[nLoop]!=m_MissingData)
		{
			pdValues[nIndex] = ((1000000-pdElev[nLoop])*pow(10.0,15))+piUpSlope[nLoop];
			piSortedIndex[nIndex]= nLoop;
			nIndex++;
		}
		else
		{
			piMissingValues[nIndex2]=nLoop;
			nIndex2++;
		}
	}
	QuickSort(pdValues,nIndex,piSortedIndex,true);
	
	nIndex2 = 0;
	for(nLoop=nIndex;nLoop<m_Row*m_Col;nLoop++)
	{
		piSortedIndex[nLoop] = piMissingValues[nIndex2];
		nIndex2 ++;
	}

	
	


	int start_rec = 0;

	int num_dn = 0;
	bool go_dn = false;
	int chan_row = 0;
	int chan_col = 0;
	double chan_elev = 0;
    int m_n2st = 0;
	
	int nCurrentRecord = 0;
	int max_12st = 0;
	//int m_n2st = 0;
	int last_rec = 0;
	double LEN2ST = 0;
	double m_z2st = 0;
	for(nLoop=0;nLoop<m_Row*m_Col;nLoop++)
	{
		nCurrentRecord = piSortedIndex[nLoop];
		start_rec = nCurrentRecord;

		num_dn = 0;
		if(piSegNo[nCurrentRecord]>0 || 
			pdElev[nCurrentRecord]<0 || 
			piN2St[nCurrentRecord]>0)
		{
			go_dn = false;
		}
		else
		{
			go_dn=true;
		}
		while(go_dn ==true)
		{
			if(pdElev[nCurrentRecord]<0)
			{
				go_dn = false;
			}
			else
			{
				if(piSegNo[nCurrentRecord]>0)
				{
					chan_row = nCurrentRecord/m_Col;
					chan_col = nCurrentRecord%m_Col;
					chan_elev = pdElev[nCurrentRecord];
					m_n2st = num_dn;
					go_dn = false;
				}
				else
				{
					if(piSegNo[nCurrentRecord]==0)
					{
						num_dn = num_dn + 1;
						last_rec = nCurrentRecord;
						if(piDdir[nCurrentRecord]!=5)
						{
							nCurrentRecord = piDrec[nCurrentRecord]-1;
						}
						else
						{
							go_dn = false;
							num_dn = 0;
						}
					}
					else
					{
						go_dn = false;
					}
				}
			}
		}

		if(num_dn >0)
		{
			go_dn = true;
			nCurrentRecord = start_rec;
			while(go_dn ==true)
			{
				if(piSegNo[nCurrentRecord]>0)
				{
					piN2St[nCurrentRecord]=0;
					pfL2St[nCurrentRecord] = 0;
					go_dn = false;
				}
				else
				{
					if(piN2St[nCurrentRecord]>0)
					{
						go_dn = false;
					}
					else
					{
						if(piN2St[nCurrentRecord]==0)
						{
							piN2St[nCurrentRecord]= m_n2st;
							LEN2ST = sqrt(pow((double)(chan_col+1) - (nCurrentRecord%m_Col+1),2) +
								pow((double)(chan_row+1) - (nCurrentRecord/m_Col+1),2)) * m_Size;

							m_z2st = pdElev[nCurrentRecord] - chan_elev;

							LEN2ST = sqrt(pow(LEN2ST,2) + pow(m_z2st*m_Size,2));
							pfL2St[nCurrentRecord] = (float)LEN2ST;
							pfZ2St[nCurrentRecord] =(float) m_z2st;
							m_n2st = m_n2st - 1;
							nCurrentRecord = piDrec[nCurrentRecord]-1;
							if(piDrec[nCurrentRecord]-1==nCurrentRecord)
							{
								piN2St[nCurrentRecord] = 0;
								pfL2St[nCurrentRecord] = 0;
								pfZ2St[nCurrentRecord] = 0;
								go_dn = false;

							}

						}
						else
						{
							go_dn = false;
						}
					}
				}
			}
		}

		nCurrentRecord = start_rec;
	}
	delete piSortedIndex;
	delete piMissingValues;
	delete pdValues;
}

void CWeppProcess::WriteDbf2()
{
	CFile myFile("BigFile.dbf",CFile::modeCreate|CFile::modeWrite );
	char* szBuffer = new char[2555];

	//Type of file
	//3 represents fox pro.
	szBuffer[0]  = 3;
	myFile.Seek(0,CFile::begin);
	myFile.Write(szBuffer,1);
	
	//Last Updated(YYMMDD)
	//This can be changed to be dinamic very easy, just get todays date.
	myFile.Seek(1,CFile::begin);
	szBuffer[0] = 01;
	szBuffer[1] = 7;
	szBuffer[2] = 11;
	myFile.Write(szBuffer,3);

	CStringArray strArray;

	strArray.Add("SeqNo");
	strArray.Add("Row");
	strArray.Add("Col");
	strArray.Add("Elev");
	strArray.Add("Ddir");
	strArray.Add("Drec");
	strArray.Add("UpSlope");
	strArray.Add("SeedType");
	strArray.Add("Wepp_Shed");
	strArray.Add("Shed_No");
	strArray.Add("Shed_Side");

	strArray.Add("Hill_No");
	strArray.Add("Chan_No");
	strArray.Add("Chan_Side");
	strArray.Add("Segment_No");
	strArray.Add("Slope");
	strArray.Add("Aspect");
	strArray.Add("L2St");
	strArray.Add("N2St");
	strArray.Add("Z2St");


	int nColNum = 1;
	int nFldSize [20];
	nFldSize[0]=13;
	nFldSize[1]=4;
	nFldSize[2]=4;
	nFldSize[3]=11;
	nFldSize[4]=2;
	nFldSize[5]=13;
	nFldSize[6]=13;
	nFldSize[7]=2;
	nFldSize[8]=13;
	nFldSize[9]=13;
	nFldSize[10]=2;
	nFldSize[11]=13;

	nFldSize[12]=13;
	nFldSize[13]=2;
	nFldSize[14]=13;
	nFldSize[15]=9;
	nFldSize[16]=4;
	nFldSize[17]=7;
	nFldSize[18]=6;
	nFldSize[19]=9;





	CStringArray strFldType ;
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	strFldType.Add("N");
	

	//Number of records in the file.
	myFile.Seek(4,CFile::begin);
	int nNumberOfRecords = m_Row*m_Col*3;
	memcpy(szBuffer,&nNumberOfRecords,4);
	int nNum = 0;
	
	myFile.Write(szBuffer,4);


	//location of the first record.
	//this will be 32 bytes plus (number of fields * 32 bytes, because field subrecord is 32 bytes each) 
	//plus 1.

	nNum = 32+(32*1)+1;
	memcpy(szBuffer,&nNum,2);
	myFile.Seek(8,CFile::begin);
	myFile.Write(szBuffer,2);
	


	//lenght of the record. This is lenght sum of all the fields.
	int nRecordLen = 13;

	memcpy(szBuffer,&nRecordLen,2);
	myFile.Seek(10,CFile::begin);
	myFile.Write(szBuffer,2);
	
	//Code page mark. Not sure what it is used for, but if not set, fox pro will ask for it
	myFile.Seek(29,CFile::begin);
	szBuffer[0] = 3;
	myFile.Write(szBuffer,1);
	
	//field subrecord
	//Description of each field in the table(type, lenght, decimals, etc.)
	myFile.Seek(32,CFile::begin);



	int j = 0;
	int nDisplacement = 0;
	int* nDec = new int [nColNum];
	int nLoop;
	for(nLoop=0;nLoop<nColNum;nLoop++)
	{
		nDec[nLoop]=0;
	}
	nDec[0] = 4;
	nDec[15] = 3;
	nDec[17] = 1;
	nDec[19] = 3;


	//write column headers
	for(nLoop=0;nLoop<nColNum;nLoop++)
	{
		//Column Name
		myFile.Seek(32+32*nLoop,CFile::begin);
		strcpy(szBuffer,"Elev");
		myFile.Write(szBuffer,10);
		//Data type
		myFile.Seek(32 + 32*nLoop + 11,CFile::begin);
		strcpy(szBuffer,"N");
		myFile.Write(szBuffer,1);
		
		nDisplacement = 0;
		for(j=0;j<nLoop;j++)
		{
			nDisplacement = nDisplacement + nFldSize[j];
		}
		//Field displacement in the record.
		memcpy(szBuffer,&nDisplacement,4);
		myFile.Seek(32 + 32*nLoop + 12 ,CFile::begin);

		myFile.Write(szBuffer,4);
		//Lenght of the field(don't forget the first delete byte flag.
		myFile.Seek(32 + 32*nLoop + 16,CFile::begin);
		memcpy(szBuffer,&nFldSize[nLoop],1);
		myFile.Write(szBuffer,1);
	
		myFile.Seek(32 + 32*nLoop + 17,CFile::begin);
		//Number of decimal places.
		memcpy(szBuffer,&nDec[nLoop],1);
		myFile.Write(szBuffer,1);

	}
	//Header Record Terminator value 13(0x0D)
	myFile.Seek(32+32*nLoop,CFile::begin);

	szBuffer[0] = 13;
	myFile.Write(szBuffer,1);

	szBuffer[0] = 0;

	myFile.Write(szBuffer,1);
	
	int nIndex;

	nIndex = 0;

	char* pszTemp = new char [255];
	int nLenght = 0;
	int nLoop3 = 0;
	int nStart = 0;
	int nFinish = 0;
	int nCounter = 0;
	



	int nCount = 0;

	for(j=0;j<m_Row*m_Col;j++)
	{
		

		
		szBuffer[0]='\0';

		
		nCounter = 0;


		
		//Elev
		_gcvt(pdElev[j],12,pszTemp);
		for(nLoop=0;nLoop<255;nLoop++)
		{
			if(pszTemp[nLoop]==0)
			{
				nLenght = nLoop;
				nLoop = 255;
			}
		}
		for(nLoop=nLenght;nLoop<nFldSize[0];nLoop++)
		{
			pszTemp[nLoop] = ' ';
		}
		nLoop3=0;
		nFinish = 13;
		//nFinish = nStart + nFldSize[0];
		//nCounter ++;
		for(nLoop=nStart;nLoop<nFinish;nLoop++)
		{
			szBuffer[nLoop] = pszTemp[nLoop3];
			nLoop3++;
		}

		

		szBuffer[nLoop] = 0;
		myFile.Write(szBuffer,nRecordLen);
		if (j+1==m_Row*m_Col)
		{
			if(nCount ==2)
			{
				nCount = 3;
				j = m_Row*m_Col-4;

			}
			else
			{
				if(nCount<2)
				{
					nCount++;
					j = 0;
				}
			}
		}
	}
	szBuffer[0]=26;
	myFile.Write(szBuffer,1);
	myFile.Close();
	exit(0);

}


void CWeppProcess::Hill_Stats(int* piHillNo, int* piDEMSort, int* piShedNo, short int* piShedSide)
{
	m_Message = "Running Hill_Stats...";
	//UpdateData(false);


	bool bExit2 = false;
	double delta_n = 0;
	double pi = 3.1415926535;
	int aspect_cls = 0;
	int nCurrentPointer = 0;
	bool bEOF = false;
	int nLoop=0;
	bool bFound = false;
	int curr_hill =0;
	double dTempValue = 0;
	
	double curr_width = 0;
	int nCurrentRecord = 0;
	int nSegRecordNum = 0;
	int m_hill_area = 0;
	double m_circ_aspect =0;
	double x_vector = 0;
	double y_vector = 0;
	double hill_circ = 0;
	double xovery = 0;
	float m_slope = 0;
	double m_aspect=0;
	int m_n2st = 0;
	double temp_aspect=0;
	pSTemp = new STemp[m_Row*m_Col/2];
	double* pdValue = new double[m_Row*m_Col/2];
	int* pnSortedTemp = new int[m_Row*m_Col/2];
	double max_n =0;
	bool bExit = false;
	int n = 0;
	double total_slope = 0;
	double m_num_points = 0;
	double curr_reln = 0;
	double curr_rel=0;
	double rel_dist = 0;
	double mean_slope = 0;
	CString sString;
	
	CString sProfString;

	double curr_n2st=0;

	double curr_len = 0;
	double curr_area = 0;
	//int curr_width = 0;
	
	int nCurrentPointerTemp =0;
	int nCount = 0;

	while (bEOF ==false)
	{


		


		bEOF = true;
		for(nLoop=nCurrentPointer;nLoop<m_Row*m_Col;nLoop++)
		{
			if(piHillNo[piDEMSort[nLoop]]>0)
			{
				nCurrentPointer= nLoop;
				bEOF = false;
				nLoop = m_Row*m_Col;
				nCurrentRecord = piDEMSort[nCurrentPointer];
			}
		}
		if(bEOF==false)
		{
			curr_hill = piHillNo[nCurrentRecord];
			if(piShedSide[nCurrentRecord]==1)
			{
				curr_width = -1;
			}
			else
			{
				curr_width = 0;
			}
			for(nLoop=0;nLoop<nSegSize;nLoop++)
			{
				if(pSegs[nLoop].Left_Hill == curr_hill || 
					pSegs[nLoop].Right_Hill== curr_hill||
					pSegs[nLoop].Top_Hill ==curr_hill)
				{
					nSegRecordNum = nLoop;
					nLoop = nSegSize;
				}
			}
			if(curr_width!=-1)
			{
				curr_width = pSegs[nSegRecordNum].Len_Cells;
			}
			aspect_cls = 0;
			m_hill_area = 0;
			x_vector = 0;
			y_vector =  0;
			m_circ_aspect =0;
			hill_circ = 0;
			xovery = 0;
			nTempFileSize = 0;
			bExit = false;

			while(bExit ==false)
			{
				m_slope = pfSlope[piDEMSort[nCurrentPointer]]/100;
				m_aspect= piAspect[piDEMSort[nCurrentPointer]];
				m_n2st = piN2St[piDEMSort[nCurrentPointer]];
				m_hill_area = m_hill_area + 1;
				
				x_vector = x_vector + sin((m_aspect/360)*2*pi);
				y_vector = y_vector + cos((m_aspect/360)*2*pi);
	
				pSTemp[nTempFileSize].Slope = m_slope;
				pSTemp[nTempFileSize].Aspect = (int)m_aspect;
				pSTemp[nTempFileSize].N2St = m_n2st;
				nTempFileSize++;
				nCurrentPointer++;
				if(nCurrentPointer>=m_Row*m_Col)
				{
					bExit = true;
				}
				else
				{
					if(piHillNo[piDEMSort[nCurrentPointer]]!=curr_hill)
					{
						bExit = true;
					}
				}

			}

			xovery =x_vector/y_vector;
			temp_aspect = ((atan(xovery)/(2*pi))*360);
			if(x_vector>=0 && y_vector>=0)
			{
				m_circ_aspect = temp_aspect;
			}
			else
			{
				if(x_vector>=0&&y_vector<=0)
				{
					m_circ_aspect = 180 + temp_aspect;
				}
				else
				{
					if(x_vector<=0 && y_vector<=0)
					{
						m_circ_aspect = 180 + temp_aspect;
					}
					else
					{
						if(x_vector<=0 && y_vector >=0)
						{
							m_circ_aspect = 360 + temp_aspect;
						}
						else
						{
							m_circ_aspect = 361;
						}
					}
				}
			}
			hill_circ = m_circ_aspect;
			


				for (int i = 0; i <nTempFileSize; i++)
				{
					pdValue[i] = pSTemp[i].N2St*pow(10.0,10)+pSTemp[i].Slope;
					pnSortedTemp[i] = i;
				}

				QuickSort(pdValue, nTempFileSize, pnSortedTemp,true); //sort only non missing values
			
			max_n = pSTemp[pnSortedTemp[nTempFileSize-1]].N2St;	
			
			for(nLoop=0;nLoop<nTempFileSize;nLoop++)
			{

				if(max_n>1)
				{
					if(pSTemp[pnSortedTemp[nLoop]].N2St ==1)
					{
						pSTemp[pnSortedTemp[nLoop]].Rel_n2St = 1.0;
					}
					else
					{
						pSTemp[pnSortedTemp[nLoop]].Rel_n2St = 
							1-((pSTemp[pnSortedTemp[nLoop]].N2St-1)/(max_n-1));

					}
				}
				else
				{
					pSTemp[pnSortedTemp[nLoop]].Rel_n2St = 1.0;
				}
			}
	
			if(max_n <21)
			{
				delta_n = 1;
			}
			else
			{
				delta_n = (max_n)/16;
			}

			curr_n2st = delta_n;
			n = 0;
			total_slope =0;
			m_num_points = 0;
			curr_reln = 0;
			nCurrentRecord = 0;
			
			nCurrentPointerTemp=0;
			if (max_n<21)
			{
				bExit = false;
				while(bExit==false)
				{
					if(nCurrentPointerTemp<nTempFileSize)
					{

						total_slope = 0;
						n=0;
						x_vector = 0;
						y_vector = 0;
						m_circ_aspect = 0;
						xovery = 0;
						curr_rel = pSTemp[pnSortedTemp[nCurrentPointerTemp]].Rel_n2St;
						bExit2 = false;
						while(bExit2==false)
						{
							if(nCurrentPointerTemp<nTempFileSize)
							{
								if(pSTemp[pnSortedTemp[nCurrentPointerTemp]].Rel_n2St>=curr_rel)
								{
									total_slope= total_slope + pSTemp[pnSortedTemp[nCurrentPointerTemp]].Slope;
									n = n+1;
									x_vector = x_vector + sin(((double)pSTemp[pnSortedTemp[nCurrentPointerTemp]].Aspect/360)*2*pi);
									y_vector = y_vector + cos(((double)pSTemp[pnSortedTemp[nCurrentPointerTemp]].Aspect/360)*2*pi);
									curr_reln = pSTemp[pnSortedTemp[nCurrentPointerTemp]].Rel_n2St;
									nCurrentPointerTemp++;
								}
								else
								{
									bExit2 = true;
								}
							}
							else
							{
								bExit2 = true;
							}

						}

						rel_dist = curr_reln;
						mean_slope = total_slope/n;
						xovery = x_vector/y_vector;
						temp_aspect = atan(xovery)/(2*pi)*360;
						
						if(x_vector>=0 && y_vector>=0)
						{
							m_circ_aspect = temp_aspect;
						}
						else
						{
							if(x_vector>=0 && y_vector<=0)
							{
								m_circ_aspect = 180+temp_aspect;
							}
							else
							{
								if(x_vector<=0 && y_vector<=0)
								{
									m_circ_aspect = 180+temp_aspect;
								}
								else
								{
									if(x_vector<=0 && y_vector>=0)
									{
										m_circ_aspect = 360+temp_aspect;
									}
									else
									{
										m_circ_aspect = 361;
									}
								}
							}
						}

						if(nProfSize%1000==0)
						{
							Prof* pTemp = new Prof[nProfSize+1000];
							for(nLoop=0;nLoop<nProfSize;nLoop++)
							{
								pTemp[nLoop] = pProf[nLoop];
							}
							delete pProf;
							pProf = pTemp;
						}
						
						
						pProf[nProfSize].Hill_No = curr_hill;
						pProf[nProfSize].Distance = rel_dist;
						pProf[nProfSize].Rel_n2St = curr_reln;
						pProf[nProfSize].Aspect = m_circ_aspect;
						pProf[nProfSize].Slope = mean_slope;
						nProfSize++;



						sString.Format("%4.3f,%6.3f",rel_dist,mean_slope);
						if(rel_dist==1.0)
						{
							sProfString = sString;
						}
						else
						{
							sProfString = sString + " " + sProfString;
						}
						m_num_points ++;						
					}
					else
					{
						bExit = true;
					}
				}
				if(nCount ==42)
				{
					m_Message = "After second while Loop";
					//UpdateData(false);
				}
			}
			else
			{
				curr_rel = 1.0;
				bExit = false;

				while(bExit==false)
				{
					if(nCurrentPointerTemp>=nTempFileSize)
					{
						bExit = true;
					}
					else
					{
						total_slope = 0;
						n = 0;
						x_vector = 0;
						y_vector = 0;
						m_circ_aspect = 0;
						xovery =0;
						
						bExit2 = false;

						while(bExit2==false)
						{
 							if(nCurrentPointerTemp<nTempFileSize)
							{
								if(pSTemp[pnSortedTemp[nCurrentPointerTemp]].Rel_n2St>=curr_rel)
								{

									total_slope = total_slope + pSTemp[pnSortedTemp[nCurrentPointerTemp]].Slope;
									n++;
									x_vector = x_vector + sin(((double)pSTemp[pnSortedTemp[nCurrentPointerTemp]].Aspect/360)*(2*pi));
									y_vector = y_vector + cos(((double)pSTemp[pnSortedTemp[nCurrentPointerTemp]].Aspect/360)*(2*pi));
									curr_reln = pSTemp[pnSortedTemp[nCurrentPointerTemp]].Rel_n2St;
									nCurrentPointerTemp++;
								}
								else
								{
									bExit2 = true;
								}

							}
							else
							{
								bExit2 = true;
							}
						}

						curr_rel = curr_rel - 0.06;
						rel_dist = curr_reln;
						mean_slope = total_slope /n;
						xovery = x_vector/y_vector;
						temp_aspect = atan(xovery)/(2*pi)*360;
						if(x_vector>=0 && y_vector>=0)
						{
							m_circ_aspect = temp_aspect;
						}
						else
						{
							if(x_vector>=0 && y_vector<=0)
							{
								m_circ_aspect = 180+temp_aspect;
							}
							else
							{
								if(x_vector<=0 && y_vector<=0)
								{
									m_circ_aspect = 180 + temp_aspect;
								}
								else
								{
									if(x_vector<=0 && y_vector>=0)
									{
										m_circ_aspect = 360 + temp_aspect;
									}
									else
									{
										m_circ_aspect = 361;
									}
								}
							}
						}

						if(nProfSize%1000==0)
						{
							Prof* pTemp = new Prof[nProfSize+1000];
							for(nLoop=0;nLoop<nProfSize;nLoop++)
							{
								pTemp[nLoop] = pProf[nLoop];
							}
							delete pProf;
							pProf = pTemp;
						}
						pProf[nProfSize].Hill_No = curr_hill;
						pProf[nProfSize].Distance = rel_dist;
						pProf[nProfSize].Rel_n2St = curr_reln;
						pProf[nProfSize].Aspect = m_circ_aspect;
						pProf[nProfSize].Slope = mean_slope;
						nProfSize++;
						sString.Format("%4.3f,%6.3f", rel_dist, mean_slope);
						if(rel_dist ==1.0)
						{
							sProfString = sString;
						}
						else
						{
							sProfString = sString +  " " + sProfString;
						}
						m_num_points++;
					}
				}
			}
			
			if (curr_width ==-1)
			{
				curr_len = max_n * m_Size;
				curr_area = m_hill_area *m_Size*m_Size;
				curr_width = curr_area /curr_len;
			}
			else
			{
				curr_width = curr_width*m_Size;
				curr_area = m_hill_area *m_Size*m_Size;
				curr_len = max_n*m_Size;
			}



			if(nHillFileSize%1000==0)
			{
				Hill* pTemp = new Hill[nHillFileSize+1000];
				for(nLoop=0;nLoop<nHillFileSize;nLoop++)
				{
					pTemp[nLoop] = pHill[nLoop];
				}
				delete pHill;
				pHill = pTemp;

			}
	
			pHill[nHillFileSize].Hill_No = curr_hill;
			pHill[nHillFileSize].Hill_Width = curr_width;
			pHill[nHillFileSize].Hill_Area = (int)curr_area;
			pHill[nHillFileSize].Max_Len = curr_len;
			pHill[nHillFileSize].Wepp_Len = curr_area/curr_width;

			if(m_num_points==1)
			{
				sProfString.Format("0.0, %6.3f 1.0,0.00",mean_slope);
				m_num_points = 2;
				
			}
			pHill[nHillFileSize].Num_Points = (int)m_num_points;
			pHill[nHillFileSize].Aspect = (int)hill_circ;
			char* p = sProfString.GetBuffer(2);
			strncpy(pHill[nHillFileSize].Profile,p,250 );
			nHillFileSize++;
			
			
		}
	}
	delete pdValue;
	delete pnSortedTemp;


}

void CWeppProcess::Chan_Stats()
{
	m_Message = "Running Chan_Stats...";
	//UpdateData(false);

	double temp_aspect=0;
	CString new_string;
	double rel_dist = 0;
	double pi = 3.1415926535;
	int zrec[9];
	int cent_x=0;
	int cent_y = 0;
	int cent_rec = 0;
	int numneigh =0 ;
	int start_rec = 0;
	double start_elev = 0;
	bool next2channel = false;
	double max_upslope =0;

	//int aspect_cls[8];
	double m_start2endl=0;

	CString prof_string;
	double up_slope = 0;
	int up_aspect = 0;
	int up_ddir = 0;

	bool End_Chan = false;
	int* pSortedSeg = new int[nSegSize];
	double* pValue = new double [nSegSize];
	int m_seg_no = 0;
	int	m_start_cell = 0;
	int	m_start_row = 0;
	int	m_start_col = 0;
	double	m_start_elev=0;
	int	m_start_ddir =0;
	int	m_end_cell = 0;
	int	m_end_row = 0;
	int	m_end_col = 0;
	double	m_end_elev = 0;
	int	m_len_cells=0;
	double	m_mean_slope = 0;
	double	m_mean_aspect = 0;
	double curr_slope  = 0;
	int m_num_points = 0;
	int nWeppRecordNum = 0;


	int nLoop=0;

	for(nLoop=0;nLoop<nSegSize;nLoop++)
	{
		pValue[nLoop] = pSegs[nLoop].Final_Id;
		pSortedSeg[nLoop] = nLoop;
	}
	QuickSort(pValue,nSegSize,pSortedSeg);

	
	int nCurrentPointer = 0;
	int nRecordNum = 0;
	int x_diff = 0;
	int y_diff = 0;
	double total_slope = 0;
	double total_aspect = 0;
	double delta_n;
	double len_cells = 0;
	int n = 0;
	double sample_no = 0;
	double m_circ_aspect = 0;
	bool bExit = false;
	double m_gen_slope =0;
	double xovery = 0;
	double x_vector = 0;
	double y_vector = 0;
	bool bExit2 = false;

	while(bExit ==false)
	{
		if(nCurrentPointer<nSegSize)
		{
			nRecordNum = pSortedSeg[nCurrentPointer];

			m_seg_no = pSegs[nRecordNum].Final_Id;
			m_start_cell = pSegs[nRecordNum].Start_Cell-1;
			m_start_row = pSegs[nRecordNum].Start_Row-1;
			m_start_col = pSegs[nRecordNum].Start_Col-1;
			m_start_elev = pSegs[nRecordNum].Start_Elev;
			m_start_ddir = pSegs[nRecordNum].Start_Ddir;
			m_end_cell = pSegs[nRecordNum].End_Cell-1;
			m_end_row = pSegs[nRecordNum].End_Row-1;
			m_end_col = pSegs[nRecordNum].End_Col-1;
			m_end_elev = pSegs[nRecordNum].End_Elev;
			m_len_cells = pSegs[nRecordNum].Len_Cells;
			m_mean_slope = 0;
			m_mean_aspect = 0;
			curr_slope = 0;
			m_num_points =0;
			
			if(m_end_col<m_start_col)
			{
				x_diff = (m_start_col - m_end_col)*m_Size;
			}
			else
			{
				x_diff = (m_end_col-m_start_col)*m_Size;
			}

			if(m_end_row <m_start_row)
			{
				y_diff = (m_start_row-m_end_row)*m_Size;
			}
			else
			{
				y_diff = (m_end_row - m_start_row)*m_Size;
			}

			m_start2endl = sqrt(pow((double)x_diff,2)+pow((double)y_diff,2));

			if(m_len_cells>1)
			{
				delta_n = 1;
			}
			else
			{
				delta_n = len_cells/18;
			}

			total_slope = 0;
			total_aspect = 0;
			n  = 0;
			sample_no = 0;
			nWeppRecordNum = m_start_cell;
			if(m_len_cells==1)
			{
				up_slope = 0;
				up_aspect = 0;
				up_ddir = 0;
				cent_y = nWeppRecordNum / m_Col;
				cent_x = nWeppRecordNum % m_Col;
				//cent_z = pdElev[nWeppRecordNum];
				cent_rec = nWeppRecordNum;
				numneigh = 0;
				if(cent_x >0 && cent_y >0)
				{
					
					zrec[numneigh] = cent_rec - (m_Col + 1);
					numneigh = numneigh + 1;
				}
				if(cent_y >0)
				{
					zrec[numneigh] = cent_rec - m_Col;
					numneigh = numneigh + 1;
				}
				if(cent_y >0 && cent_x <m_Col -1)
				{
					zrec[numneigh] = cent_rec - (m_Col -1);
					numneigh = numneigh + 1;
				}
				if(cent_x >0)
				{
					zrec[numneigh] = cent_rec -1;
					numneigh = numneigh + 1;
				}
				if(cent_x <m_Col-1)
				{
					zrec[numneigh] = cent_rec + 1;
					numneigh = numneigh + 1;				
				}

				if(cent_x >0 && cent_y <(m_Row-1))
				{
					zrec[numneigh] = cent_rec + (m_Col -1);
					numneigh = numneigh + 1;
				}
				if(cent_y <m_Row -1)
				{
					zrec[numneigh] = cent_rec + m_Col;
					numneigh = numneigh + 1;
				}
				if(cent_x <m_Col-1 && cent_y <(m_Row-1))
				{
					zrec[numneigh] = cent_rec  + m_Col +1;
					numneigh = numneigh + 1;
				}
				start_rec = nWeppRecordNum;
				start_elev = pdElev[start_rec];
				next2channel = false;
				max_upslope = 0;

				for(nLoop=0;nLoop<numneigh;nLoop++)
				{
					nWeppRecordNum = zrec[nLoop];
					if(piDrec[nWeppRecordNum]-1==start_rec && pdElev[nWeppRecordNum]>start_elev)
					{
						if(piUpSlope[nWeppRecordNum]>max_upslope)
						{
							max_upslope = piUpSlope[nWeppRecordNum];
							up_slope = pfSlope[nWeppRecordNum];
							up_aspect = piAspect[nWeppRecordNum];
							up_ddir = piDdir[nWeppRecordNum];
						}
					}
				}
				nWeppRecordNum = start_rec;

				total_slope = up_slope + pfSlope[nWeppRecordNum];
				m_mean_slope = total_slope/2;
				m_mean_slope = m_mean_slope /100;
				curr_slope = pfSlope[nWeppRecordNum]/100;
				up_slope = up_slope /100;
				m_circ_aspect = piAspect[nWeppRecordNum];
				prof_string.Format("0.0,%6.3f 1.0,%6.3f",up_slope,curr_slope);
				m_num_points = 2;
				n=1;
				m_gen_slope = m_mean_slope;

				

			}
			else
			{
				m_gen_slope = (m_start_elev - m_end_elev)/(m_len_cells*m_Size);
				End_Chan = false;
				m_mean_slope = 0;
				total_slope = 0;
				x_vector = 0;
				y_vector = 0;
				m_circ_aspect = 0;
				xovery =0;
				bExit2 = false;
				while (bExit2 ==false)
				{
					if(End_Chan ==false && nWeppRecordNum<m_Row*m_Col)
					{
						if(nWeppRecordNum == m_end_cell)
						{
							End_Chan = true;
						}
						total_slope = total_slope + pfSlope[nWeppRecordNum];
						curr_slope = pfSlope[nWeppRecordNum]/100;
						x_vector = x_vector + sin(((double)piAspect[nWeppRecordNum]/360)*2*pi);
						y_vector = y_vector + cos(((double)piAspect[nWeppRecordNum]/360)*2*pi);
						n = n+1;
						if(n>=sample_no)
						{
							sample_no = sample_no + delta_n;
							m_num_points = m_num_points +1;
							if(sample_no<=delta_n)
							{
								rel_dist = 0.0;
							}
							else
							{
								if(n ==m_len_cells || End_Chan == true)
								{
									rel_dist = 1.0;
								}
								else
								{
									rel_dist = (((double)n-1)/((double)m_len_cells-1));
								}
							}
							new_string.Format("%4.3f,%6.3f",rel_dist, curr_slope);
							if(rel_dist ==0.0)
							{
								prof_string = new_string;
							}
							else
							{
								prof_string = prof_string +" "+new_string;
							}
						}
						nWeppRecordNum = piDrec[nWeppRecordNum]-1;


					}
					else
					{
						bExit2 = true;
					}
				}

				m_mean_slope = total_slope/n;
				m_mean_slope = m_mean_slope /100;
				xovery = x_vector/y_vector;
				temp_aspect = (atan(xovery)/(2*pi))*360;
				if(x_vector>=0 && y_vector>=0)
				{
					m_circ_aspect= temp_aspect;
				}
				else
				{
					if(x_vector>=0 && y_vector<=0)
					{
						m_circ_aspect = 180 + temp_aspect;
					}
					else
					{
						if(x_vector<=0 && y_vector<=0)
						{
							m_circ_aspect = 180+temp_aspect;
						}
						else
						{
							if(x_vector<=0 && y_vector>=0)
							{
								m_circ_aspect = 360 +temp_aspect;
							}
							else
							{
								m_circ_aspect = 361;
							}
						}
					}
				}
			}
			if(nChanSize%5==0)
			{
				ChanStat* pTemp = new ChanStat[nChanSize + 5];
				for(nLoop=0;nLoop<nChanSize;nLoop++)
				{
					pTemp[nLoop] = pChanStat[nLoop];
				}
				delete pChanStat;
				pChanStat = pTemp;
			}
			pChanStat[nChanSize].Chan_No = m_seg_no;
			pChanStat[nChanSize].Chan_Len = n*m_Size;
			pChanStat[nChanSize].Num_Points  = m_num_points;
			pChanStat[nChanSize].Mean_Slope = m_mean_slope;
			pChanStat[nChanSize].Gen_Slope = m_gen_slope;
			pChanStat[nChanSize].Aspect = (int)m_circ_aspect;
			strncpy(pChanStat[nChanSize].Profile, prof_string.GetBuffer(1),250);
			nChanSize++;
			nCurrentPointer++;


		}
		else
		{
			bExit = true;
		}
	}


	delete pSortedSeg;
	delete pValue;




}























