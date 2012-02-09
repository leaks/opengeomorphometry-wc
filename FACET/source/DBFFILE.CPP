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
// DbfFile.cpp: implementation of the CDbfFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "facet.h"
#include "DbfFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDbfFile::CDbfFile()
{
	pColumnName = new CString[255];
	pColumnValue = new long* [255];
	nNumOfColumns = 0;
	nNumOfRows = 0;
	nSizeOfFile = 0;
	int nLoop =0;
	for(nLoop=0;nLoop<255;nLoop++)
	{
		nFieldSize[nLoop]  = 0;
		nNumOfDec[nLoop] = 0;
	}
}

CDbfFile::~CDbfFile()
{
	for (int nLoop=0;nLoop<nNumOfColumns;nLoop++)
	{
		if(CType[nLoop] == 'C')
		{
			CString* pPointer =(CString*) pColumnValue[nLoop];
			delete []pPointer;
			pPointer = 0;
		}
		if(CType[nLoop] == 'I')
		{
			int * pPointer =(int *) pColumnValue[nLoop];
			delete []pPointer;
			pPointer = 0;
		}
		if(CType[nLoop] == 'F')
		{
			float * pPointer =(float *) pColumnValue[nLoop];
			delete []pPointer;
			pPointer =0;
		}		
		if(CType[nLoop] == 'D')
		{
			double * pPointer =(double *) pColumnValue[nLoop];
			delete []pPointer;
			pPointer = 0;
		}
		if(CType[nLoop] == 'S')
		{
			short int * pPointer =(short int *) pColumnValue[nLoop];
			delete []pPointer;
			pPointer = 0;
		}
		if(CType[nLoop] == 'L')
		{
			bool * pPointer =(bool *) pColumnValue[nLoop];
			delete []pPointer;
			pPointer = 0;
		}
	}
	delete pColumnValue;
	pColumnValue = 0;
	delete []pColumnName;
	pColumnName = 0;
	nNumOfColumns = 0;
	nSizeOfFile = 0;
	nNumOfRows = 0;

}


int CDbfFile::FindField(CString sFieldName)
{
	CString sColumnName;
	int nReturnValue = -1;
	
	for (int nLoop=0;nLoop<nNumOfColumns;nLoop++)
	{
			sColumnName = pColumnName[nLoop];
			sColumnName.Remove(' ');
		if	(sColumnName.CompareNoCase( sFieldName) ==0)
		{
			nReturnValue = nLoop;
			nLoop = nNumOfColumns;
		}
	}



	return nReturnValue;
}





bool CDbfFile::OpenFile(CString sDBFFileName, CProgressCtrl* pProgressBar, bool* pKillThread = false)
{
	pbKillThread = pKillThread;
	char szBuffer[255];
	bool bReturnValue = false;
	CFile myFile;

	short int nEachFieldLength[255];
	short int nEachFieldDecimal[255];

	char szFieldType[255];

	int nResult = 0;
	nResult = myFile.Open(sDBFFileName.GetBuffer(0),CFile::modeRead);
	if(nResult!=0)
	{

		int** pColumnLocation = new int* [255];

		myFile.Seek(4,CFile::begin);
		myFile.Read(szBuffer,4);

		int* pNumRecInFile = new int;
		*pNumRecInFile = *(int*) &szBuffer;
		
		nSizeOfFile = *pNumRecInFile;
		nNumOfRows = nSizeOfFile;
		if(pProgressBar !=0)
		{
			pProgressBar->SetRange32(0,*pNumRecInFile);
			pProgressBar->SetStep(1);
			pProgressBar->SetPos(0);
		}
		
		int* pShortInt = pNumRecInFile;
		myFile.Seek(32,CFile::begin);
		int nLoop = 0;

		for (nLoop = 0;nLoop<255;nLoop++)
		{
			pColumnLocation[nLoop] = new int;
			*pColumnLocation[nLoop] = -1;
		}


		
		int nFieldLength[35];

		for(nLoop = 0;nLoop<255;nLoop++)
		{
			myFile.Read(szBuffer,32);
			if(szBuffer[0] !=13)
			{

				nFieldLength[nLoop] = szBuffer[16];
				pColumnName[nNumOfColumns] = szBuffer;
				nEachFieldLength[nNumOfColumns] = szBuffer[16];
				nEachFieldDecimal[nNumOfColumns] = szBuffer[17];
				szFieldType[nNumOfColumns] = szBuffer[11];

				*pColumnLocation[nNumOfColumns] = nLoop;
				nNumOfColumns++;

			}
			else
			{
				nLoop = 255;
			}
		}
		
		//char** pTr;
		//char CType[255];

		for (nLoop=0;nLoop<nNumOfColumns;nLoop++)
		{
			nFieldSize[nLoop]  = nEachFieldLength[nLoop];
			nNumOfDec[nLoop] = nEachFieldDecimal[nLoop];
			//The type is character. Make size of the string = FieldLength
			if(szFieldType[nLoop] == 'C')
			{
				pColumnValue[nLoop] =(long*) new CString[*pNumRecInFile];
				CType[nLoop]='C';
			}

			if(szFieldType[nLoop] =='N' || szFieldType[nLoop] =='B'|| 
				szFieldType[nLoop] =='F'|| szFieldType[nLoop] =='I')
			{
				//The type is numeric, have to find out the type(int, shor int, double, float).
				//if we have decimal placed, we have to find out if it will be a float or double.
				if(nEachFieldDecimal[nLoop] >0)
				{
					//if the length of the field is more than 5, then we need double
					if(nEachFieldLength[nLoop]>5) 
					{
						pColumnValue[nLoop]=(long*) new double[*pNumRecInFile];
						CType[nLoop]='D';
					}
					//we need float
					else
					{
						pColumnValue[nLoop] = (long*) new float[*pNumRecInFile];
						CType[nLoop]='F';
					}

				}
				// find out if we need int or short
				else
				{
					//we need int
					if(nEachFieldLength[nLoop]>4)
					{
						pColumnValue[nLoop] = (long*) new int[*pNumRecInFile];
						CType[nLoop]='I';
					}
					//we need short int
					else
					{
						pColumnValue[nLoop] = (long*) new short int[*pNumRecInFile];
						CType[nLoop]='S';
					}

				}
			}
			if(szFieldType[nLoop] =='L')
			{
				pColumnValue[nLoop] = (long*) new bool[*pNumRecInFile];
				CType[nLoop] = 'L';
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


		int nBitsOfSet[255];


	

		//Read fields values
		//***********************************************************	
		for (nLoop=0;nLoop<255;nLoop++)
		{
			nBitsOfSet[nLoop]=0;
		}
		
		for (int nLoop2=0;nLoop2<nNumOfColumns;nLoop2++)
		{
			for (nLoop=0;nLoop<*pColumnLocation[nLoop2];nLoop++)
			{
				nBitsOfSet[nLoop2] = nBitsOfSet[nLoop2] + nFieldLength[nLoop];
			}

		}





		



		myFile.Seek(*piFirstRecordLoc,CFile::begin);
		
		char szValueBuffer  [255];
		
		//int pTemp[30];
		CString pTemp1[30];
		CString pTemp2[30];
		CString pTemp3[30];

		int nNumRecords = 0;
		int nIndex = 0;
		nLoop = -1;
		CString* CStrPointer;
		int* intPointer;
		double* dPointer;
		float* fPointer;
		short int* sPointer;
		bool* bPointer;
		while((int)myFile.Read(szBuffer,*piRecordLength) == (int) *piRecordLength)
		{
			if(pbKillThread!=0)
			{
				if(*pbKillThread ==true)
				{
					myFile.Close();
					AfxEndThread(0);
				}
			}
			if(pProgressBar!=0)
			{
				//pProgressBar->StepIt();
			}
			nNumRecords ++;

			nLoop ++;

			//myFile.Read(szBuffer,*piRecordLength);
			

			for(nLoop=0;nLoop<nNumOfColumns;nLoop++)
			{
				for(nIndex = 0;nIndex<nFieldLength[nLoop];nIndex++)
				{
					szValueBuffer[nIndex] = szBuffer[nBitsOfSet[nLoop]+1+nIndex];
				}
				szValueBuffer[nIndex]=0;
				if(CType[nLoop] == 'C')
				{

					CStrPointer =(CString*) pColumnValue[nLoop];
					CStrPointer[nNumRecords-1]= szValueBuffer;
				}

				if(CType[nLoop] == 'L')
				{

					bPointer =(bool*) pColumnValue[nLoop];
					if(*szValueBuffer =='F')
					{
						bPointer[nNumRecords-1]= false;
					}
					else
					{
						bPointer[nNumRecords-1]= true;
					}
					
				}

				if(CType[nLoop] =='D')
				{
					dPointer = (double*) pColumnValue[nLoop];
					dPointer[nNumRecords-1]=atof(szValueBuffer);
				}

				if(CType[nLoop]=='I')
				{
					intPointer = (int*) pColumnValue[nLoop];
					intPointer[nNumRecords-1] = atoi(szValueBuffer);
				}
				if(CType[nLoop]=='F')
				{
					fPointer = (float*) pColumnValue[nLoop];
					fPointer[nNumRecords-1] = (float) atof(szValueBuffer);
				}
				if(CType[nLoop]=='S')
				{
					sPointer = (short int *) pColumnValue[nLoop];
					sPointer[nNumRecords-1] = atoi(szValueBuffer);
				}


				
				for(int nLoop2=0;nLoop2<255;nLoop2++)
				{
					szValueBuffer[nLoop2]=0;
				}
			}

			
			
		

		}
		delete pNumRecInFile;
		delete piRecordLength;
		delete piFirstRecordLoc;
		dPointer = (double*) pColumnValue[1];
		for (nLoop = 0;nLoop<255;nLoop++)
		{

			delete pColumnLocation[nLoop];
		}
		delete pColumnLocation;
		myFile.Close();


		bReturnValue = true;


	}








	return bReturnValue;
}

long* CDbfFile::ReturnValue(int nColumn, int nRow)
{
	long* p = NULL;
	long* pReturnValue = 0;
	if(nColumn>=0 && nRow>=0 && nColumn<nNumOfColumns && nRow<nNumOfRows )
	{
		if(CType[nColumn]=='C')
		{
			CString* pReturnValue = (CString*) pColumnValue[nColumn];
			p = (long*) &pReturnValue[nRow];
		}
		if(CType[nColumn]=='D')
		{
			double* pReturnValue = (double*) pColumnValue[nColumn];
			p = (long *) &pReturnValue[nRow];
		}
		if(CType[nColumn]=='F')
		{
			float* pReturnValue = (float*) pColumnValue[nColumn];
			p = (long *) &pReturnValue[nRow];
		}
		if(CType[nColumn]=='S')
		{
			short int* pReturnValue = (short int*) pColumnValue[nColumn];
			p = (long *) &pReturnValue[nRow];

		}
		if(CType[nColumn]=='I')
		{
			int* pReturnValue = (int*) pColumnValue[nColumn];
			p = (long *) &pReturnValue[nRow];
		}
		if(CType[nColumn]=='L')
		{
			bool* pReturnValue = (bool*) pColumnValue[nColumn];
			p = (long *) &pReturnValue[nRow];
		}

	}
	return (long*) p;
}

char CDbfFile::ReturnType(int nColumn)
{
	char szReturnValue = '0';
	if(nColumn>=0 && nColumn<nNumOfColumns)
	{
		szReturnValue = CType[nColumn];

	}
	return szReturnValue;
}
bool CDbfFile::AddField(CString cszFieldName, char szFieldType, int nSize)
{
	bool bReturnValue =false;
	if((szFieldType =='I'||szFieldType=='C'||szFieldType == 'S' ||szFieldType =='D'
||
		szFieldType =='F' ||szFieldType =='L' ) && (cszFieldName.GetLength()>=0))
	{
		pColumnName[nNumOfColumns] = cszFieldName;
		if(szFieldType =='I')
		{
			int* pPointer = new int[nNumOfRows];
			pColumnValue[nNumOfColumns]= (long*)pPointer;
		}
		if(szFieldType =='C')
		{
			CString* pPointer = new CString[nNumOfRows];
			pColumnValue[nNumOfColumns]= (long*)pPointer;
		}
		if(szFieldType =='S')
		{
			short int* pPointer = new short int[nNumOfRows];
			pColumnValue[nNumOfColumns]= (long*)pPointer;
		}
		if(szFieldType =='D')
		{
			double* pPointer = new double[nNumOfRows];
			pColumnValue[nNumOfColumns]= (long*)pPointer;
		}

		if(szFieldType =='F')
		{
			float* pPointer = new float[nNumOfRows];
			pColumnValue[nNumOfColumns]= (long*)pPointer;
		}
		if(szFieldType =='L')
		{
			bool* pPointer = new bool[nNumOfRows];
			pColumnValue[nNumOfColumns]= (long*)pPointer;
		}
		CType[nNumOfColumns]=szFieldType;
		nFieldSize[nNumOfColumns] = nSize ;
		nNumOfColumns++;
		bReturnValue = true;
		
	}
	return bReturnValue;
}





bool CDbfFile::SetValue(int nColumnNum, int nRowNum, int nValue)
{
	bool bReturnValue = false;
	if(nColumnNum < nNumOfColumns && nRowNum<nNumOfRows)
	{
		int* pPointer =(int*) pColumnValue[nColumnNum];
		pPointer[nRowNum] = nValue;
		bReturnValue = true;
	}
	return bReturnValue;
}

bool CDbfFile::SetValue(int nColumnNum, int nRowNum, CString sValue)
{
	bool bReturnValue = false;
	if(nColumnNum < nNumOfColumns && nRowNum<nNumOfRows)
	{
		CString* pPointer =(CString*) pColumnValue[nColumnNum];
		pPointer[nRowNum] = sValue;
		bReturnValue = true;
	}
	return bReturnValue;
}

bool CDbfFile::SetValue(int nColumnNum, int nRowNum, double dValue)
{
	bool bReturnValue = false;
	if(nColumnNum < nNumOfColumns && nRowNum<nNumOfRows)
	{
		double* pPointer =(double*) pColumnValue[nColumnNum];
		pPointer[nRowNum] = dValue;
		bReturnValue = true;
	}
	return bReturnValue;
}

bool CDbfFile::SetValue(int nColumnNum, int nRowNum, float fValue)
{
	bool bReturnValue = false;
	if(nColumnNum < nNumOfColumns && nRowNum<nNumOfRows)
	{
		float* pPointer =(float*) pColumnValue[nColumnNum];
		pPointer[nRowNum] = fValue;
		bReturnValue = true;
	}
	return bReturnValue;
}

bool CDbfFile::SetValue(int nColumnNum, int nRowNum, short nValue)
{
	bool bReturnValue = false;
	if(nColumnNum < nNumOfColumns && nRowNum<nNumOfRows)
	{
		short int* pPointer =(short int*) pColumnValue[nColumnNum];
		pPointer[nRowNum] = nValue;
		bReturnValue = true;
	}
	return bReturnValue;
}

int CDbfFile::GetNumRecords()
{
	return nNumOfRows;

}

void CDbfFile::AddRecord()
{
	if(nNumOfRows<nSizeOfFile)
	{
	}
	else
	{
		for (int nLoop=0;nLoop<nNumOfColumns;nLoop++)
		{
			if(CType[nLoop] =='C')
			{
				CString* pPointer = (CString*) pColumnValue[nLoop];
				CString* pNewPointer = new CString [nNumOfRows + 10000];
				for (int nLoop2=0;nLoop2<nNumOfRows;nLoop2++)
				{
					pNewPointer[nLoop2] = pPointer[nLoop2];
				}

				delete []pPointer;
				pColumnValue[nLoop] = (long*) pNewPointer;
			}
			if(CType[nLoop] =='I')
			{
				int* pPointer = (int*) pColumnValue[nLoop];
				int* pNewPointer = new int [nNumOfRows + 10000];
				for (int nLoop2=0;nLoop2<nNumOfRows;nLoop2++)
				{
					pNewPointer[nLoop2] = pPointer[nLoop2];
				}

				delete pPointer;
				pColumnValue[nLoop] = (long*)pNewPointer;
			}
			if(CType[nLoop] =='F')
			{
				float* pPointer = (float*) pColumnValue[nLoop];
				float* pNewPointer = new float [nNumOfRows + 10000];
				for (int nLoop2=0;nLoop2<nNumOfRows;nLoop2++)
				{
					pNewPointer[nLoop2] = pPointer[nLoop2];
				}

				delete []pPointer;
				pColumnValue[nLoop] = (long*) pNewPointer;
			}
			if(CType[nLoop] =='D')
			{
				double* pPointer = (double*) pColumnValue[nLoop];
				double* pNewPointer = new double [nNumOfRows + 10000];
				for (int nLoop2=0;nLoop2<nNumOfRows;nLoop2++)
				{
					pNewPointer[nLoop2] = pPointer[nLoop2];
				}

				delete []pPointer;
				pColumnValue[nLoop] = (long*) pNewPointer;
			}
			if(CType[nLoop] =='S')
			{
				short int* pPointer = (short int*) pColumnValue[nLoop];
				short int* pNewPointer = new short int [nNumOfRows + 10000];
				for (int nLoop2=0;nLoop2<nNumOfRows;nLoop2++)
				{
					pNewPointer[nLoop2] = pPointer[nLoop2];
				}

				delete []pPointer;
				pColumnValue[nLoop] = (long*)pNewPointer;
			}
			if(CType[nLoop] =='L')
			{
				bool* pPointer = (bool*) pColumnValue[nLoop];
				bool* pNewPointer = new bool [nNumOfRows + 10000];
				for (int nLoop2=0;nLoop2<nNumOfRows;nLoop2++)
				{
					pNewPointer[nLoop2] = pPointer[nLoop2];
				}

				delete []pPointer;
				pColumnValue[nLoop] = (long*) pNewPointer;
			}
		}
		nSizeOfFile = nSizeOfFile + 10000;
	}

	nNumOfRows++;
}

bool CDbfFile::SetValue(int nColumnNum, int nRowNum, bool bValue)
{
	bool bReturnValue = false;
	if(nColumnNum < nNumOfColumns && nRowNum<nNumOfRows)
	{
		bool* pPointer =(bool*) pColumnValue[nColumnNum];
		pPointer[nRowNum] = bValue;
		bReturnValue = true;
	}
	return bReturnValue;
}

CString CDbfFile::GetFieldName(int nFieldIndex)
{
	CString sReturnValue;
	if(nFieldIndex<255)
	{
		sReturnValue = pColumnName[nFieldIndex];
	}
	return sReturnValue;
}

long* CDbfFile::ReturnPointer(int nColumn)
{
	long* pPointer = 0;
	if(nColumn<nNumOfColumns)
	{
		pPointer = (long*) pColumnValue[nColumn];
	}
	return pPointer;
}

bool CDbfFile::SaveToFile(CString strFileName, CProgressCtrl* pProgressBar)
{
	CFile myFile;
	bool bReturnValue = false;

	if(!(myFile.Open(strFileName,CFile::modeCreate|CFile::modeWrite )==0))
	{




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
		int nLoop = 0;
		for(nLoop=0;nLoop<nNumOfColumns;nLoop++)
		{
			strArray.Add(pColumnName[nLoop]);
		}

		

		int nColNum = nNumOfColumns;
		int nFldSize [255];
		CStringArray strFldType ;
		for(nLoop=0;nLoop<nNumOfColumns;nLoop++)
		{	if(nFieldSize[nLoop] >0) 
			{
				nFldSize[nLoop] = nFieldSize[nLoop];
				if(CType[nLoop] == 'C')
				{
					strFldType.Add("C");
				}
				if(CType[nLoop]=='N')
				{
					strFldType.Add("N");
				}
				if(CType[nLoop]== 'L')
				{
					strFldType.Add("L");			
				}
				if(CType[nLoop] =='S')
				{
					strFldType.Add("N");
				}
				if(CType[nLoop] =='I')
				{
					strFldType.Add("N");
				}
			}
			else
			{
				if(CType[nLoop] == 'I') 
				{
					nFldSize[nLoop]= 9;
					strFldType.Add("N");
				}
				if(CType[nLoop] == 'S')
				{
					nFldSize[nLoop] = 5;
					strFldType.Add("N");
				}
				if(CType[nLoop] == 'D')
				{
					nFldSize[nLoop] = 13;
					strFldType.Add("N");
				}
				if(CType[nLoop] == 'F')
				{
					nFldSize[nLoop] = 7;
					strFldType.Add("N");
				}
				if(CType[nLoop] == 'L')
				{
					nFldSize[nLoop] = 1;
					strFldType.Add( "L");
				}
				if(CType[nLoop] == 'C')
				{
					nFldSize[nLoop] = 1;
					strFldType.Add( "C");
				}

			}
		}


		

		//Number of records in the file.
		myFile.Seek(4,CFile::begin);
		int nNumberOfRecords = nNumOfRows;
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
		


		//lenght of the record. This is lenght sum of all the fields.
		int nRecordLen = 0;
		for(nLoop=0;nLoop<nColNum;nLoop++)
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



		int j = 0;
		int nDisplacement = 0;
		int* nDec = new int [nColNum];
		
		for(nLoop=0;nLoop<nColNum;nLoop++)
		{
			nDec[nLoop]=nNumOfDec[nLoop];
		}








		int nLenght = 0;
		int k = 0;
		//write column headers
		for(nLoop=0;nLoop<nColNum;nLoop++)
		{

			//Column Name
			myFile.Seek(32+32*nLoop,CFile::begin);
			strcpy(szBuffer,((CString) strArray.GetAt(nLoop)).GetBuffer(8));
			nLenght = strlen(szBuffer);
			for (k= nLenght;k<10;k++)
			{
				szBuffer[k] = 0;
			}
			myFile.Write(szBuffer,10);
			//Data type
			myFile.Seek(32 + 32*nLoop + 11,CFile::begin);
			strcpy(szBuffer,((CString) strFldType.GetAt(nLoop)).GetBuffer(1));
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
		
		int nIndex = 0;


		char* pszTemp = new char [255];
		nLenght = 0;
		int nLoop3 = 0;
		int nStart = 0;
		int nFinish = 0;
		int nCounter = 0;
		




		int nLoop2 = 0;
		//char pszTemp[1000];


		pProgressBar->SetPos(0);
		pProgressBar->SetRange32(0,nNumberOfRecords);
		pProgressBar->SetStep(1);










		for(j=0;j<nNumberOfRecords;j++)
		{
			//pProgressBar->StepIt();
			nCounter = 0;
			szBuffer[0]='\0';
			nLoop = 0;
			nFinish = 0;
			nStart = 0;
			for(nLoop2=0;nLoop2<nNumOfColumns;nLoop2++)
			{

				nLoop3=0;
				nStart = nFinish;
				nFinish = nStart + nFldSize[nCounter ];
				nCounter ++;			

				
				//
				if(CType[nLoop2] =='D')
				{
					sprintf(pszTemp, "%.3f",((double*)pColumnValue[nLoop2])[j]);
				}			
				if(CType[nLoop2] =='F')
				{
					sprintf(pszTemp, "%.3f",((float*)pColumnValue[nLoop2])[j]);
				}
				if(CType[nLoop2] =='S')
				{
					sprintf(pszTemp, "%d",((short int*)pColumnValue[nLoop2])[j]);
				}
				if(CType[nLoop2] =='I')
				{
					sprintf(pszTemp, "%d",((int*)pColumnValue[nLoop2])[j]);
				}
				if(CType[nLoop2] =='L')
				{
					if(((bool*) pColumnValue[nLoop2])[j] == true)
					{
						strcpy(pszTemp,"T");
					}
					else
					{
						strcpy(pszTemp,"F");
					}	
				}
				if(CType[nLoop2] =='C')
				{
					sprintf(pszTemp, "%s",((CString*)pColumnValue[nLoop2])[j]);
				}
				
				nLenght = strlen(pszTemp);

				/*
				for(nLoop=nLenght;nLoop<nFldSize[nLoop2];nLoop++)
				{
					pszTemp[nLoop] = ' ';
				}
				for(nLoop=nStart;nLoop<nFinish;nLoop++)
				{
					szBuffer[nLoop] = pszTemp[nLoop3];
					nLoop3++;
				}

				*/

				for(nLoop= nStart;nLoop<nFinish - nLenght +1;nLoop++)
				{
					szBuffer[nLoop] = ' ';
				}
				for(nLoop=nFinish - nLenght;nLoop<nFinish;nLoop++)
				{
					szBuffer[nLoop] = pszTemp[nLoop3];
					nLoop3++;
				}
			}
			
			szBuffer[nLoop] = 0;
			myFile.Write(szBuffer,nRecordLen);
		}
		szBuffer[0]=26;
		myFile.Write(szBuffer,1);
		myFile.Close();

		delete pszTemp;
		delete nDec;
		delete szBuffer;
		bReturnValue = true;
	}
	return bReturnValue;
}

void CDbfFile::Empty()
{
	for (int nLoop=0;nLoop<nNumOfColumns;nLoop++)
	{
		if(CType[nLoop] == 'C')
		{
			CString* pPointer =(CString*) pColumnValue[nLoop];
			delete []pPointer;
		}
		if(CType[nLoop] == 'I')
		{
			int * pPointer =(int *) pColumnValue[nLoop];
			delete []pPointer;
		}
		if(CType[nLoop] == 'F')
		{
			float * pPointer =(float *) pColumnValue[nLoop];
			delete []pPointer;
		}		
		if(CType[nLoop] == 'D')
		{
			double * pPointer =(double *) pColumnValue[nLoop];
			delete []pPointer;
		}
		if(CType[nLoop] == 'S')
		{
			short int * pPointer =(short int *) pColumnValue[nLoop];
			delete []pPointer;
		}
		if(CType[nLoop] == 'L')
		{
			bool * pPointer =(bool *) pColumnValue[nLoop];
			delete []pPointer;
		}
	}


	nNumOfColumns = 0;
	nSizeOfFile = 0;
	nNumOfRows = 0;
}

void CDbfFile::SetInitialFileSize(int nFileSize)
{
	if(nFileSize<nSizeOfFile)
	{
	}
	else
	{
		for (int nLoop=0;nLoop<nNumOfColumns;nLoop++)
		{
			if(CType[nLoop] =='C')
			{
				CString* pPointer = (CString*) pColumnValue[nLoop];
				CString* pNewPointer = new CString [nNumOfRows + nFileSize];
				for (int nLoop2=0;nLoop2<nNumOfRows;nLoop2++)
				{
					pNewPointer[nLoop2] = pPointer[nLoop2];
				}

				delete []pPointer;
				pColumnValue[nLoop] = (long*) pNewPointer;
			}
			if(CType[nLoop] =='I')
			{
				int* pPointer = (int*) pColumnValue[nLoop];
				int* pNewPointer = new int [nNumOfRows + nFileSize];
				for (int nLoop2=0;nLoop2<nNumOfRows;nLoop2++)
				{
					pNewPointer[nLoop2] = pPointer[nLoop2];
				}

				delete pPointer;
				pColumnValue[nLoop] = (long*)pNewPointer;
			}
			if(CType[nLoop] =='F')
			{
				float* pPointer = (float*) pColumnValue[nLoop];
				float* pNewPointer = new float [nNumOfRows + nFileSize];
				for (int nLoop2=0;nLoop2<nNumOfRows;nLoop2++)
				{
					pNewPointer[nLoop2] = pPointer[nLoop2];
				}

				delete []pPointer;
				pColumnValue[nLoop] = (long*) pNewPointer;
			}
			if(CType[nLoop] =='D')
			{
				double* pPointer = (double*) pColumnValue[nLoop];
				double* pNewPointer = new double [nNumOfRows + nFileSize];
				for (int nLoop2=0;nLoop2<nNumOfRows;nLoop2++)
				{
					pNewPointer[nLoop2] = pPointer[nLoop2];
				}

				delete []pPointer;
				pColumnValue[nLoop] = (long*) pNewPointer;
			}
			if(CType[nLoop] =='S')
			{
				short int* pPointer = (short int*) pColumnValue[nLoop];
				short int* pNewPointer = new short int [nNumOfRows + nFileSize];
				for (int nLoop2=0;nLoop2<nNumOfRows;nLoop2++)
				{
					pNewPointer[nLoop2] = pPointer[nLoop2];
				}

				delete []pPointer;
				pColumnValue[nLoop] = (long*)pNewPointer;
			}
			if(CType[nLoop] =='L')
			{
				bool* pPointer = (bool*) pColumnValue[nLoop];
				bool* pNewPointer = new bool [nNumOfRows + nFileSize];
				for (int nLoop2=0;nLoop2<nNumOfRows;nLoop2++)
				{
					pNewPointer[nLoop2] = pPointer[nLoop2];
				}

				delete []pPointer;
				pColumnValue[nLoop] = (long*) pNewPointer;
			}
		}
		nSizeOfFile = nSizeOfFile + nFileSize;
	}

	//nNumOfRows++;
}

int CDbfFile::GetNumFields()
{
	return nNumOfColumns;
}
