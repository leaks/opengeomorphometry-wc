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
// FacetProcess.cpp: implementation of the CFacetProcess class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "facet.h"
#include "FacetProcess.h"
#include <afx.h>
#include <math.h>
#include "Sort.h"
//#include "DbfFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFacetProcess::CFacetProcess()
{
	//Initilizing the variables used by the process
	
	/*Variable for holding ARule file information."*/
	pARule = new ARule[35];
	nARuleFileSize = 0;
	//pARule = 0;

	same_size = true;
}

CFacetProcess::~CFacetProcess()
{
	delete []pARule;

}
void CFacetProcess::LSM_FUZC(CPtrArray* pArray)
{
	CString* pDirectory = (CString*) pArray->GetAt(12);
	CString* p3CharName = (CString*) pArray->GetAt(13);
	CString* sMessage = new CString;

	//AfxMessageBox("Start");
	double fuzzfal = 0.0;
	bool facet_tag = false;
	int numdone = 0;
	int no_facets = 0;
	int facet = 0;

	int a_seqno = FuzaFile.GetNumRecords();
	
	CString* pString = (CString*) pArray->GetAt(4);
	CProgressCtrl* pProgressBar = (CProgressCtrl*) pArray->GetAt(0);
	CProgressCtrl* pProgressBar2 =(CProgressCtrl*) pArray->GetAt(1);
	//pProgressBar->SetPos(0);


	*sMessage = "Reading "  + * pString;
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);	

	if(CRuleFile.OpenFile(*pString,pProgressBar, pbKillThread )==false)
	{
		AfxMessageBox("Could not open file " + *pString);
		exit(1);
	}
	*sMessage = "Running calculations..";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
	pProgressBar2->StepIt();
	int nCRuleFileNumRecords = CRuleFile.GetNumRecords();

	int* pSortedCRuleIndex = new int[nCRuleFileNumRecords];

	double* pSortValue = new double[nCRuleFileNumRecords];
	
	int nLoop = 0;
	short int nAttrwtColumnNum = 0;
	nAttrwtColumnNum = CRuleFile.FindField("attrwt");
	ASSERT(nAttrwtColumnNum!=-1);

	short int nFacet_NoColumnNum = 0;
	nFacet_NoColumnNum = CRuleFile.FindField("Facet_no");
	ASSERT(nFacet_NoColumnNum !=-1);


	for(nLoop=0;nLoop<nCRuleFileNumRecords;nLoop++)
	{
		pSortValue[nLoop] = (*( (int*)CRuleFile.ReturnValue(nFacet_NoColumnNum,nLoop)) *100) + (99- ( *((int*) CRuleFile.ReturnValue(nAttrwtColumnNum,nLoop))));
		pSortedCRuleIndex[nLoop] = nLoop;
		if(*pbKillThread == true)
		{
			AfxEndThread(0);
		}
	}
	
	QuickSort(pSortValue,nCRuleFileNumRecords,pSortedCRuleIndex);
	if(*pbKillThread == true)
	{
		AfxEndThread(0);
	}
	/*
	int nTemp = 0;
	for(nLoop=0;nLoop<nCRuleFileNumRecords-1;nLoop++)
	{

		if(((int*) CRuleFile.ReturnValue(nFacet_NoColumnNum,pSortedCRuleIndex[nLoop]) ==
			(int*) CRuleFile.ReturnValue(nFacet_NoColumnNum,pSortedCRuleIndex[nLoop +1])) &&
			((int*) CRuleFile.ReturnValue(nAttrwtColumnNum,pSortedCRuleIndex[nLoop]) ==
			
			(int*) CRuleFile.ReturnValue(nAttrwtColumnNum,pSortedCRuleIndex[nLoop]+1)))
		{
			nTemp = pSortedCRuleIndex[nLoop];
			pSortedCRuleIndex[nLoop] = pSortedCRuleIndex[nLoop+1];
			pSortedCRuleIndex[nLoop+1]= nTemp;

		}

	}
	*/
	delete pSortValue;
	int no_attributes[30];
	int total_wt[30];

	short int fuzattrColumn = CRuleFile.FindField("fuzattr");
	ASSERT(fuzattrColumn !=-1);
	short int f_nameColumn = CRuleFile.FindField("f_name");
	ASSERT(f_nameColumn!=-1);
	no_facets = 0;
	int nFacet = 0;
	CString facet_name[30];
	int nLoop2 = 0;
	CString sString;
	CString fuzz_attr[30][30];
	int attr_no = 0;
	double attr_wt[30][30];
	bool bEnd = false;

	//pProgressBar->SetPos(0);
	///pProgressBar->SetRange32(0,nCRuleFileNumRecords);
	//pProgressBar->SetStep(1);
	for(nLoop=0;nLoop<nCRuleFileNumRecords;nLoop++)
	{

		if(*pbKillThread == true)
		{
			AfxEndThread(0);
		}
		//pProgressBar->StepIt();
		
		no_attributes[nFacet] = 0;
		facet_name[nFacet] = ((CString*) CRuleFile.ReturnValue(f_nameColumn,pSortedCRuleIndex[nLoop]))->GetBuffer(0);
		facet_name[nFacet].Remove(' ');
		total_wt[nFacet] = 0;
		for(nLoop2=nLoop;nLoop2<nCRuleFileNumRecords;nLoop2++)
		{


			if(*pbKillThread == true)
			{
				AfxEndThread(0);
			}
			bEnd = true;
			sString = ((CString*) CRuleFile.ReturnValue(f_nameColumn,pSortedCRuleIndex[nLoop2]))->GetBuffer(0);
			sString.Remove(' ');

			if(facet_name[nFacet].CompareNoCase(sString) ==0)
			{

				sString = ((CString*)CRuleFile.ReturnValue(fuzattrColumn,pSortedCRuleIndex[nLoop2]))->GetBuffer(0);
				sString.Remove(' ');
				fuzz_attr[nFacet][attr_no]= sString;
				attr_wt[nFacet][attr_no] = *(int*) CRuleFile.ReturnValue(nAttrwtColumnNum,pSortedCRuleIndex[nLoop2]);
				total_wt[nFacet] = total_wt[nFacet] + *(int*) CRuleFile.ReturnValue(nAttrwtColumnNum,pSortedCRuleIndex[nLoop2]);
				no_attributes[nFacet] ++;
				attr_no = no_attributes[nFacet];
			}
			else
			{

				bEnd = false;
				nLoop = nLoop2 -1;
				nLoop2 = nCRuleFileNumRecords;
				
			}
		}
		
		if(bEnd ==true)
		{
			nLoop = nCRuleFileNumRecords;
		}

		no_facets = no_facets++;
		nFacet = no_facets;
		attr_no = 0;
	}
	
	//pProgressBar->SetPos(0);
	//pProgressBar2->StepIt();

	for(nLoop=0;nLoop<no_facets;nLoop++)
	{
		for(nLoop2=0;nLoop2<no_attributes[nLoop];nLoop2++)
		{
			attr_wt[nLoop][nLoop2] = attr_wt[nLoop][nLoop2] /total_wt[nLoop];
		}
	}







	//Create fuzzy classification file(make_fuzc)

	FuzcFile.AddField("SEQNO",'I',9);
	FuzcFile.AddField("MISSING",'L',1);

	for(nLoop=0;nLoop<no_facets;nLoop++)
	{
		//pColumnName = CRuleFile.GetFieldName(nLoop
		FuzcFile.AddField(facet_name[nLoop],'S',6);		
	}

	FuzcFile.AddField("max_facet",'I',6);
	FuzcFile.AddField("next_facet",'I',6);
	FuzcFile.AddField("fac4",'I',6);
	int nFuzaFileNumRec = FuzaFile.GetNumRecords();
	if(FuzcFile.GetNumRecords() != nFuzaFileNumRec)
	{
		for(nLoop=0;nLoop<nFuzaFileNumRec;nLoop++)
		{
			FuzcFile.AddRecord();
			FuzcFile.SetValue(0,nLoop,(int) nLoop+1);
			if(*pbKillThread == true)
			{
				AfxEndThread(0);
			}
		}
		if(*pbKillThread == true)
		{
			AfxEndThread(0);
		}
	}

	numdone = 0;
	int AMissingColumn = FuzaFile.FindField("Missing");
	ASSERT(AMissingColumn!=-1);
	int CMissingColumn = FuzcFile.FindField("Missing");
	ASSERT(CMissingColumn!=-1);
	//double attrib_val;
	int nColumn = 0;

	double fuzzval = 0;
	int nLoop3 = 0;
	//pProgressBar->SetRange32(0,nFuzaFileNumRec);
	//pProgressBar->SetStep(1);
	bool* pAMissingPointer = (bool*)FuzaFile.ReturnPointer(AMissingColumn);
	bool* pCMissingPointer =(bool*) FuzcFile.ReturnPointer(CMissingColumn);


	//Variable to hold column location for each column. THis way we don't have to query the object every time.
	//This will increase performance of the loop about 800%.

	long* fuzz_attr2[30][30];

	for(nLoop=0;nLoop<no_facets;nLoop++)
	{
		for(nLoop2=0;nLoop2<no_attributes[nLoop];nLoop2++)
		{
			sString = fuzz_attr[nLoop][nLoop2];
			nColumn = FuzaFile.FindField(sString);
			ASSERT(nColumn!=-1);
			fuzz_attr2[nLoop][nLoop2] = (long*)FuzaFile.ReturnPointer(nColumn);
			if(*pbKillThread == true)
			{
				AfxEndThread(0);
			}
		}
		if(*pbKillThread == true)
		{
			AfxEndThread(0);
		}
	}




	long facet_name2[30];
	for(nLoop=0;nLoop<no_facets;nLoop++)
	{
		nColumn = FuzcFile.FindField(facet_name[nLoop]);
		ASSERT(nColumn!=-1);
		facet_name2[nLoop] = (long) FuzcFile.ReturnPointer(nColumn);
	}



	short int* pShortPointer = 0;

	
	for(nLoop=0;nLoop<nFuzaFileNumRec;nLoop++)
	{
		if(*pbKillThread == true)
		{
			AfxEndThread(0);
		}
		for(nLoop2=0;nLoop2<no_facets;nLoop2++)
		{
			if(*pbKillThread == true)
			{
				AfxEndThread(0);
			}
			fuzzval = 0.0;
			if( pAMissingPointer[nLoop] ==false)
			{
				pCMissingPointer[nLoop] = false;
				for(nLoop3=0;nLoop3<no_attributes[nLoop2];nLoop3++)
				{

					//sString = fuzz_attr[nLoop2][nLoop3];
					
					//nColumn = FuzaFile.FindField(sString);

					//ASSERT(nColumn!=-1);
					pShortPointer = (short int*) fuzz_attr2[nLoop2][nLoop3];
					//fuzzval = fuzzval + *(short int*) FuzaFile.ReturnValue(nColumn,nLoop2)*attr_wt[nLoop2][nLoop3];
					fuzzval = fuzzval + pShortPointer[nLoop] *attr_wt[nLoop2][nLoop3];
				}
			}
			else
			{
				pCMissingPointer[nLoop] = true;
				fuzzval= 0;
			}
			pShortPointer = (short int*) facet_name2[nLoop2];
			pShortPointer[nLoop] = (short int)(fuzzval + 0.5) ;

		}
		//pProgressBar->StepIt();
	}
	//pProgressBar->SetPos(0);
	pProgressBar2->StepIt();
	//AfxMessageBox("Done");
	//FuzcFile.SaveToFile("c:\\FuzcFile2.dbf");
	//exit(0);
	delete pSortedCRuleIndex;
	*sMessage = "Saving " + *pDirectory + *p3CharName + "fuza.dbf";




	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,1,(long) sMessage);
	FuzaFile.SaveToFile(*pDirectory + *p3CharName + "fuza.dbf",pProgressBar);
	pProgressBar2->StepIt();
	FuzaFile.Empty();
	//FuzcFile.Empty();
	
}

void CFacetProcess::LSM_FUZA(CPtrArray* pArray)
{
	pWndNotifyProgress = CWnd::FromHandle((HWND)pArray->GetAt(9));
	CString* sMessage = new CString;


	pbKillThread = (bool*) pArray->GetAt(14);	


	double fuzzval = 0.0;
	int nNumDone = 0;
	CString curr_file = " ";
	CString curr_text = " ";

	CProgressCtrl* pProgressBar = (CProgressCtrl*) pArray->GetAt(0);	
	CProgressCtrl* pProgressBar2 =(CProgressCtrl*) pArray->GetAt(1);

	int nLoop = 0;
	///CString* pFileName = (CString* ) pArray->GetAt(3);
	if(ReadARuleFile(pArray))
	{
		pProgressBar2->StepIt();
		Make_AFile();
		CString * pString =(CString*) pArray->GetAt(6);

		//pProgressBar->SetPos(0);


		*sMessage = "Reading "  + * pString;
		pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
		relzfile.OpenFile(pString->GetBuffer(0),pProgressBar, pbKillThread);
		pProgressBar2->StepIt();		

		//int pos1 = pProgressBar2->GetPos();
		a_seqno = relzfile.GetNumRecords();
		
		pString = (CString*) pArray->GetAt(2);

		*sMessage = "Reading "  + * pString;
		pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);	
		DEMFile.OpenFile(pString->GetBuffer(0),pProgressBar, pbKillThread );
		pProgressBar2->StepIt();


		pString = (CString*) pArray->GetAt(7);
		//AfxMessageBox(pString->GetBuffer(0));
		*sMessage = "Reading "  + * pString;
		pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
		FormFile.OpenFile(pString->GetBuffer(0),pProgressBar, pbKillThread );
		pProgressBar2->StepIt();
		b_seqno = FormFile.GetNumRecords();
		if(b_seqno != a_seqno)
		{
			same_size = false;
		}
		
		c_seqno = FuzaFile.GetNumRecords();
		if(c_seqno != a_seqno)
		{

			
			int nColumn1 = FuzaFile.FindField("SeqNo");
			int nColumn2 = FuzaFile.FindField("Missing");

			int nColumn11 = DEMFile.FindField("SeqNo");
			int nColumn22 = DEMFile.FindField("Missing");
			
			int* pIntPointer = 0;
			bool* pBoolPointer = 0;
			CString stype = DEMFile.ReturnType(nColumn11);
			
			int nDEMNumRec = DEMFile.GetNumRecords();
			//pProgressBar->SetPos(0);
			//pProgressBar->SetRange32(0,nDEMNumRec);
			for (nLoop=0;nLoop<nDEMNumRec;nLoop++)
			{
				//pProgressBar->StepIt();
				pIntPointer = (int*)DEMFile.ReturnValue(nColumn11,nLoop);
				pBoolPointer = (bool*)DEMFile.ReturnValue(nColumn22,nLoop);


				FuzaFile.AddRecord();
				FuzaFile.SetValue(nColumn1,FuzaFile.GetNumRecords()-1, *pIntPointer);
				FuzaFile.SetValue(nColumn2,FuzaFile.GetNumRecords()-1, *pBoolPointer);
				//FuzaFile
				if(*pbKillThread == true)
				{
					AfxEndThread(0);
				}
			}
			//pProgressBar->SetPos(0);
			pProgressBar2->StepIt();
		}
		*sMessage = "Running calculations..";
		pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,1,(long) sMessage);
		int nSizeOfFile = nARuleFileSize;
		CString szString;
		CDbfFile* pFilePointer = 0;
		int nLoop2 = 0;
		int nSecondFileSize = 0;
		int nMissingIndex = FuzaFile.FindField("Missing");
		CString strOutField;
		CString strInField;
		int nInFileColumnLoc = 0;
		double dX = 0.0;
		bool* pBoolPointer = 0;

		//pProgressBar->SetPos(0);
		//pProgressBar->SetRange32(0,nSizeOfFile);
		//pProgressBar->SetStep(1);

		//pDlgPointer->CUpdateData(FALSE);
		
		int formfileColumns[30];
		int relzfileColumns[30];

		for(nLoop=0;nLoop<nSizeOfFile;nLoop++)
		{
			szString = pARule[nLoop].File_in;
			szString.Remove(' ');
			if(szString.CompareNoCase("formfile") ==0)
			{
				strInField = pARule[nLoop].Attr_in;
				strInField.Remove(' ');
				nInFileColumnLoc = FormFile.FindField(strInField);
				ASSERT(nInFileColumnLoc!=-1);
				formfileColumns[nLoop]=nInFileColumnLoc;
		
			}
			if(szString.CompareNoCase("relzfile") ==0)
			{
				strInField = pARule[nLoop].Attr_in;
				strInField.Remove(' ');
				nInFileColumnLoc = relzfile.FindField(strInField);
				ASSERT(nInFileColumnLoc!=-1);
				relzfileColumns[nLoop]=nInFileColumnLoc;
			}
			if(*pbKillThread == true)
			{
				AfxEndThread(0);
			}
		}




		for(nLoop=0;nLoop<nSizeOfFile;nLoop++)
		{
			if(*pbKillThread == true)
			{
				AfxEndThread(0);
			}
			//pProgressBar->StepIt();

			szString = pARule[nLoop].File_in;
			szString.Remove(' ');
			if(szString.CompareNoCase("formfile") ==0)
			{
				pFilePointer = &FormFile;
				nInFileColumnLoc = formfileColumns[nLoop];
				
			}
			if(szString.CompareNoCase("relzfile") ==0)
			{
				pFilePointer = &relzfile;
				nInFileColumnLoc = relzfileColumns[nLoop];
			}
			nSecondFileSize = pFilePointer->GetNumRecords();
			for(nLoop2 = 0;nLoop2<nSecondFileSize;nLoop2++)
			{
				if(*pbKillThread == true)
				{
					AfxEndThread(0);
				}
				pBoolPointer = (bool*)FuzaFile.ReturnValue(nMissingIndex,nLoop2);

				if( *pBoolPointer== false)
				{
					//strOutField = pARule[nLoop].Class_out;
					//strInField = pARule[nLoop].Attr_in ;
					//strOutField.Remove(' ');
					//strInField.Remove(' ');

					//nInFileColumnLoc = pFilePointer->FindField(strInField);
					if(pFilePointer->ReturnType(nInFileColumnLoc) == 'D')
					{
						dX = *(double*) pFilePointer->ReturnValue(nInFileColumnLoc,nLoop2);
					}
					
					if(pFilePointer->ReturnType(nInFileColumnLoc) == 'S')
					{
						dX = *(short int*) pFilePointer->ReturnValue(nInFileColumnLoc,nLoop2);
					}
					if(pFilePointer->ReturnType(nInFileColumnLoc) == 'I')
					{
						dX = *(int*) pFilePointer->ReturnValue(nInFileColumnLoc,nLoop2);
					}					
					switch (pARule[nLoop].Model_no )
					{
						case(1):
							fuzzval = 1/(1 + pow((((dX - pARule[nLoop].B))/pARule[nLoop].D) ,2));
							break;
						case(2):
							if((dX> pARule[nLoop].B_low) && (dX< pARule[nLoop].B_hi ))
							{
								fuzzval = 1;
							}
							if(dX<=pARule[nLoop].B_low)
							{
								fuzzval = 1/(1 + pow((((dX - pARule[nLoop].B_low))/pARule[nLoop].D) ,2));
							}
							if(dX>=pARule[nLoop].B_hi)
							{
								fuzzval = 1/(1 + pow((((dX - pARule[nLoop].B_hi))/pARule[nLoop].D) ,2));
							}
							break;

						case(3):
							if((dX> pARule[nLoop].B_low) && (dX< pARule[nLoop].B_hi ))
							{
								fuzzval = 1;
							}
							if(dX<=pARule[nLoop].B_low)
							{
								fuzzval = 1/(1 + pow((((dX - pARule[nLoop].B_low))/pARule[nLoop].D) ,2));
							}
							if(dX>=pARule[nLoop].B_hi)
							{
								fuzzval = 1/(1 + pow((((dX - pARule[nLoop].B_hi))/pARule[nLoop].D) ,2));
							}
							break;

						case(4):
							if(dX>pARule[nLoop].B)
							{
								fuzzval = 1;
							}
							else
							{
								fuzzval = 1/(1 + pow((((dX - pARule[nLoop].B))/pARule[nLoop].D) ,2));
							}
							break;
						case(5):
							if(dX<pARule[nLoop].B)
							{
								fuzzval = 1;
							}
							else
							{
								fuzzval = 1/(1 + pow((((dX - pARule[nLoop].B))/pARule[nLoop].D) ,2));
							}

							break;
					}
					//fuzzval = fuzzval *100 + 0.5;
					
					FuzaFile.SetValue(nLoop +2,nLoop2,(short int) (fuzzval*100 + 0.5));



				}
				else
				{
					FuzaFile.SetValue(nLoop +2,nLoop2,(short int)0);
				}
			}
			//pProgressBar->StepIt();

		}
		//pProgressBar->SetPos(0);
		

	}
	//pProgressBar->SetPos(0);
	pProgressBar2->StepIt();
	//int pos = pProgressBar2->GetPos();
	int nPlaner_2X = FuzaFile.FindField("planar_2x");
	int nPlanar_d = FuzaFile.FindField("planar_d");
	int nPlanar_a = FuzaFile.FindField("planar_a");
	ASSERT(nPlaner_2X !=-1);
	ASSERT(nPlanar_d !=-1);
	ASSERT(nPlanar_a !=-1);
	short int nValue;
	for (nLoop=0;nLoop<FuzaFile.GetNumRecords();nLoop++)
	{
		nValue = ((*(short int*)FuzaFile.ReturnValue(nPlanar_d,nLoop))  + (*(short int*)FuzaFile.ReturnValue(nPlanar_a,nLoop)))/2;
		FuzaFile.SetValue(nPlaner_2X,nLoop,nValue);
		if(*pbKillThread == true)
		{
			AfxEndThread(0);
		}
	}


	//FuzaFile.SaveToFile("c:\\fuzaFile1.dbf");
	//exit(0);

	//We delete the parameter array here, because it will be used by this thread (the dialog 
	//thread will not wait for it.
	//delete []pArray;
}

bool CFacetProcess::ReadARuleFile(CPtrArray* pArray)
{
	/*
	int pTemp4[30];
	float pTemp5[30];
	float pTemp6[30];
	float pTemp7[30];
	float pTemp8[30];
	float pTemp9[30];
	float pTemp10[30];
	*/

	CString* sMessage = new CString;




	bool bReturnValue = true;


	
	char szBuffer [255];	
	CString* pARuleFileName = (CString*) pArray->GetAt(3);
	*sMessage = "Reading "  + * pARuleFileName;
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,1,(long) sMessage);


	CFile myFile;
	CString sError;
	sError = "Error opening file " ;
	sError = sError + *pARuleFileName ;
	if(myFile.Open(pARuleFileName->GetBuffer(0), CFile::modeRead ) ==0) 
	{
		AfxMessageBox(sError);
		bReturnValue = false;
	}

	else
	{

		myFile.Seek(4,CFile::begin);
		myFile.Read(szBuffer,4);

		int* pShortInt = new int;
		*pShortInt = *(int*) &szBuffer;
		
		CProgressCtrl *pProgressBar = (CProgressCtrl*) pArray->GetAt(0);
		//pProgressBar->SetRange32(0,*pShortInt);
		//pProgressBar->SetStep(1);
		myFile.Seek(32,CFile::begin);
		int* nSortOrderLocation = new int;
		*nSortOrderLocation = -1;
		int* nFile_inLocation = new int;
		*nFile_inLocation = -1;
		int* nAttr_inLocation = new int;
		* nAttr_inLocation = 0;
		int* nClass_outLocation = new int;
		*nClass_outLocation = -1;
		int* nModel_noLocation = new int;
		*nModel_noLocation = -1;
		int* nBLocation = new int;
		*nBLocation = -1;
		int* nB_lowLocation = new int;
		*nB_lowLocation = -1;
		int* nB_hiLocation = new int;
		*nB_hiLocation = -1;
		int* nB1Location = new int;
		*nB1Location = -1;	

		int* nB2Location = new int;
		*nB2Location = -1;	
		int* nDLocation = new int;
		*nDLocation = -1;	


		
		int nFieldLength[35];

		for(int nLoop = 0;nLoop<100;nLoop++)
		{
			myFile.Read(szBuffer,32);
			if(szBuffer[0] !=13)
			{
				if(_stricmp(szBuffer,"Sortorder") == 0)
				{
					*nSortOrderLocation = nLoop;
				}
				if(_stricmp(szBuffer,"File_in")==0)
				{
					*nFile_inLocation = nLoop;
				}
				if(_stricmp(szBuffer,"Attr_in")==0)
				{
					//nInField = nLoop;
					*nAttr_inLocation = nLoop;
				}
				if(_stricmp(szBuffer,"Class_out")==0)
				{
					//nOutField  = nLoop;
					*nClass_outLocation = nLoop;
				}
				if(_stricmp(szBuffer,"Model_no")==0)
				{
					*nModel_noLocation = nLoop;
				}
				if(_stricmp(szBuffer,"B")==0)
				{
					*nBLocation = nLoop;
				}
				if(_stricmp(szBuffer,"B_low")==0)
				{
					*nB_lowLocation = nLoop;
				}
				if(_stricmp(szBuffer,"B_hi")==0)
				{
					*nB_hiLocation = nLoop;
				}

				if(_stricmp(szBuffer,"B1")==0)
				{
					*nB1Location = nLoop;
				}
				if(_stricmp(szBuffer,"B2")==0)
				{
					*nB2Location = nLoop;
				}
				if(_stricmp(szBuffer,"D")==0)
				{
					*nDLocation = nLoop;
				}
				nFieldLength[nLoop] = szBuffer[16];
			}
			else
			{
				nLoop = 100;
			}
		}
		
		
		if(*nSortOrderLocation == -1 || *nDLocation ==-1 || *nB2Location==-1||
			*nB1Location ==-1 ||*nB_hiLocation ==-1 ||*nB_lowLocation ==-1 ||
			*nBLocation == -1 || *nModel_noLocation ==-1 || *nClass_outLocation ==-1||
			*nAttr_inLocation ==-1|| *nFile_inLocation ==-1 || *nSortOrderLocation == -1)
		{
			AfxMessageBox("One of the fields is missing. Process will stop.");
			exit(0);
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







		//Read fields Elev, Drec, DDir, UpSLp from dbf table
		//***********************************************************	
		int nSortOrderBitesOfSet = 0;
		int nFileInBitesOfSet =0;
		int nAttr_InBitesOfSet = 0;
		int nClass_outBitesOfSet = 0;
		int nModel_noBitesOfSet = 0;
		int nBBitesOfSet = 0;
		int nB_lowBitesOfSet = 0;
		int nB_hiBitesOfSet = 0;
		int nB1BitesOfSet = 0;
		int nDBitesOfSet = 0;
		int nB2BitesOfSet = 0;

		for(int nLoop=0;nLoop<*nSortOrderLocation;nLoop++)
		{
			nSortOrderBitesOfSet = nSortOrderBitesOfSet + nFieldLength[nLoop];
		}


		for(int nLoop=0;nLoop<*nFile_inLocation;nLoop++)
		{
			nFileInBitesOfSet = nFileInBitesOfSet + nFieldLength[nLoop];
		}
		for(int nLoop=0;nLoop<*nAttr_inLocation;nLoop++)
		{
			nAttr_InBitesOfSet = nAttr_InBitesOfSet + nFieldLength[nLoop];
		}
		for(int nLoop=0;nLoop<*nClass_outLocation;nLoop++)
		{
			nClass_outBitesOfSet = nClass_outBitesOfSet + nFieldLength[nLoop];
		}	
		for(int nLoop=0;nLoop<*nModel_noLocation;nLoop++)
		{
			nModel_noBitesOfSet = nModel_noBitesOfSet + nFieldLength[nLoop];
		}	
		for(int nLoop=0;nLoop<*nBLocation;nLoop++)
		{
			nBBitesOfSet = nBBitesOfSet + nFieldLength[nLoop];
		}		
		for(int nLoop=0;nLoop<*nB_lowLocation;nLoop++)
		{
			nB_lowBitesOfSet = nB_lowBitesOfSet + nFieldLength[nLoop];
		}

		
		for(int nLoop=0;nLoop<*nB_hiLocation;nLoop++)
		{
			nB_hiBitesOfSet = nB_hiBitesOfSet + nFieldLength[nLoop];
		}	
		for(int nLoop=0;nLoop<*nB1Location;nLoop++)
		{
			nB1BitesOfSet = nB1BitesOfSet + nFieldLength[nLoop];
		}		
		for(int nLoop=0;nLoop<*nB2Location;nLoop++)
		{
			nB2BitesOfSet = nB2BitesOfSet + nFieldLength[nLoop];
		}


		for(int nLoop=0;nLoop<*nDLocation;nLoop++)
		{
			nDBitesOfSet = nDBitesOfSet + nFieldLength[nLoop];
		}








		myFile.Seek(*piFirstRecordLoc,CFile::begin);
		
		char* szSortOrderBuffer = new char [nFieldLength[*nSortOrderLocation]+1];
		char* szFileInBuffer = new char [nFieldLength[*nFile_inLocation]+1];
		char* szAttrInBuffer = new char [nFieldLength[*nAttr_inLocation]+1];
		char* szClassOutBuffer = new char [nFieldLength[*nClass_outLocation]+1];
		char* szModelNoBuffer = new char[nFieldLength[*nModel_noLocation]+1];
		char* szBBuffer = new char[nFieldLength[*nBLocation]+1];
		char* szBLowBuffer = new char[nFieldLength[*nB_lowLocation]+1];
		char* szBHiBuffer = new char[nFieldLength[*nB_hiLocation]+1];
		char* szB1Buffer = new char[nFieldLength[*nB1Location]+1];
		char* szB2Buffer = new char[nFieldLength[*nB2Location]+1];
		char* szDBuffer = new char[nFieldLength[*nDLocation]+1];
		//int pTemp[30];
		CString pTemp1[30];
		CString pTemp2[30];
		CString pTemp3[30];

		int nNumRecords = 0;
		int nIndex = 0;
		int nLoop = -1;
		while((int)myFile.Read(szBuffer,*piRecordLength) == (int)*piRecordLength)
		{
			//pProgressBar->StepIt();
			nNumRecords ++;

			nLoop ++;

			//myFile.Read(szBuffer,*piRecordLength);

			for(nIndex=0;nIndex<nFieldLength[*nSortOrderLocation];nIndex++)
			{
				szSortOrderBuffer[nIndex] = szBuffer[nSortOrderBitesOfSet +1 +nIndex];

			}
			//pTemp[nLoop] = atoi(szSortOrderBuffer);
			pARule[nLoop].Sortorder = atoi(szSortOrderBuffer);

			for(nIndex=0;nIndex<nFieldLength[*nFile_inLocation];nIndex++)
			{
				szFileInBuffer[nIndex] = szBuffer[nFileInBitesOfSet +1 +nIndex];

			}
			szFileInBuffer[nIndex] = 0;
			//pTemp1[nLoop] = szFileInBuffer;
			strcpy(pARule[nLoop].File_in,szFileInBuffer);

			for(nIndex=0;nIndex<nFieldLength[*nAttr_inLocation];nIndex++)
			{
				szAttrInBuffer[nIndex] = szBuffer[nAttr_InBitesOfSet +1 +nIndex];

			}
			szAttrInBuffer[nIndex] = 0;
			//pTemp2[nLoop] = szAttrInBuffer;
			strcpy(pARule[nLoop].Attr_in,szAttrInBuffer);

			for(nIndex=0;nIndex<nFieldLength[*nClass_outLocation];nIndex++)
			{
				szClassOutBuffer[nIndex] = szBuffer[nClass_outBitesOfSet +1 +nIndex];

			}
			szClassOutBuffer[nIndex]= 0;
			//pTemp3[nLoop] = szClassOutBuffer;
			strcpy(pARule[nLoop].Class_out,szClassOutBuffer);

			for(nIndex=0;nIndex<nFieldLength[*nModel_noLocation];nIndex++)
			{
				szModelNoBuffer[nIndex] = szBuffer[nModel_noBitesOfSet +1 +nIndex];

			}
			szModelNoBuffer[nIndex] = 0;
			//pTemp4[nLoop] = atoi(szModelNoBuffer);
			pARule[nLoop].Model_no = atoi(szModelNoBuffer);


			for(nIndex=0;nIndex<nFieldLength[*nBLocation];nIndex++)
			{
				szBBuffer[nIndex] = szBuffer[nBBitesOfSet +1 +nIndex];

			}
			szBBuffer[nIndex] = 0;
			//pTemp5[nLoop] = (float)atof(szBBuffer);
			pARule[nLoop].B = (float) atof(szBBuffer);



			for(nIndex=0;nIndex<nFieldLength[*nB_lowLocation];nIndex++)
			{
				szBLowBuffer[nIndex] = szBuffer[nB_lowBitesOfSet +1 +nIndex];

			}
			szBLowBuffer[nIndex] = 0;
			//pTemp6[nLoop] = (float)atof(szBLowBuffer);
			pARule[nLoop].B_low = (float) atof(szBLowBuffer);

			for(nIndex=0;nIndex<nFieldLength[*nB_hiLocation];nIndex++)
			{
				szBHiBuffer[nIndex] = szBuffer[nB_hiBitesOfSet +1 +nIndex];

			}
			szBHiBuffer[nIndex] = 0;
			//pTemp7[nLoop] = (float)atof(szBHiBuffer);
			pARule[nLoop].B_hi  = (float) atof(szBHiBuffer);


			for(nIndex=0;nIndex<nFieldLength[*nB1Location];nIndex++)
			{
				szB1Buffer[nIndex] = szBuffer[nB1BitesOfSet +1 +nIndex];

			}
			szB1Buffer[nIndex] = 0;
			//pTemp8[nLoop] = (float)atof(szB1Buffer);
			pARule[nLoop].B1 = (float) atof(szB1Buffer);

			for(nIndex=0;nIndex<nFieldLength[*nB2Location];nIndex++)
			{
				szB2Buffer[nIndex] = szBuffer[nB2BitesOfSet +1 +nIndex];

			}
			szB2Buffer[nIndex] = 0;
			//pTemp9[nLoop] = (float)atof(szB2Buffer);
			pARule[nLoop].B2 = (float) atof(szB2Buffer);

			for(nIndex=0;nIndex<nFieldLength[*nDLocation];nIndex++)
			{
				szDBuffer[nIndex] = szBuffer[nDBitesOfSet +1 +nIndex];

			}
			szDBuffer[nIndex] = 0;
			//pTemp10[nLoop] = (float)atof(szDBuffer);
			pARule[nLoop].D = (float) atof(szDBuffer);
		}
		
		nARuleFileSize = nNumRecords;

		delete szSortOrderBuffer;
		delete szFileInBuffer;
		delete szAttrInBuffer ;
		delete szClassOutBuffer ;
		delete szModelNoBuffer;
		delete szBBuffer ;
		delete szBLowBuffer;
		delete szBHiBuffer ;
		delete szB1Buffer;
		delete szB2Buffer ;
		delete szDBuffer;


		delete piRecordLength;
		delete piFirstRecordLoc;
		delete nDLocation;
		delete nB2Location;
		delete nB1Location;
		delete nB_hiLocation;
		delete nBLocation;
		delete nB_lowLocation;
		delete nModel_noLocation;
		delete nClass_outLocation;
		delete nAttr_inLocation;
		delete nFile_inLocation;
		delete nSortOrderLocation;




		delete pShortInt;
		myFile.Close();

		
		

	}
	return true;

}

void CFacetProcess::Make_AFile()
{
	bool bSuccess = false;
	bSuccess = FuzaFile.AddField("SEQNO",'I',9);
	bSuccess = FuzaFile.AddField("MISSING",'L',1);
	

	for (int nLoop = 0;nLoop<nARuleFileSize;nLoop++)
	{
		bSuccess = FuzaFile.AddField(pARule[nLoop].Class_out,'S',6);

	}
	
	bSuccess = FuzaFile.AddField("PLANAR_2X",'S',6);
	

}

void CFacetProcess::LSM_MAXC(CPtrArray *pArray)
{
	CString* sMessage = new CString;
	CString* pDirectory = (CString*) pArray->GetAt(12);
	CString* p3CharName = (CString*) pArray->GetAt(13);
	*sMessage = "Processing..";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);	
	

	int maxval= 0;
	int nextval = 0;
	int maxclass = 0;
	int nextclass = 0;
	int no_facets = 0;
	int facet = 0;
	int i = 0;
	int j = 0;

	CString tagname;
	bool facet_tag = false;
	CString facet_name[30];
	int num1=0;
	int num2=0;
	int num3=0;
	int num4=0;

	CProgressCtrl* pProgressBar = (CProgressCtrl*) pArray->GetAt(0);
	CProgressCtrl* pProgressBar2 =(CProgressCtrl*) pArray->GetAt(1);
	//pProgressBar->SetPos(0);
	int nCRuleFileNumRecords = CRuleFile.GetNumRecords();

	int* pSortedCRuleIndex = new int[nCRuleFileNumRecords];

	double* pSortValue = new double[nCRuleFileNumRecords];
	
	int nLoop = 0;
	short int nAttrwtColumnNum = 0;
	nAttrwtColumnNum = CRuleFile.FindField("attrwt");
	ASSERT(nAttrwtColumnNum!=-1);

	short int nFacet_NoColumnNum = 0;
	nFacet_NoColumnNum = CRuleFile.FindField("Facet_no");
	ASSERT(nFacet_NoColumnNum !=-1);
	

	for(nLoop=0;nLoop<nCRuleFileNumRecords;nLoop++)
	{
		pSortValue[nLoop] = (*( (int*)CRuleFile.ReturnValue(nFacet_NoColumnNum,nLoop))
			*100) + (99- ( *((int*) CRuleFile.ReturnValue(nAttrwtColumnNum,nLoop))));
		pSortedCRuleIndex[nLoop] = nLoop;	
	}
	
	QuickSort(pSortValue,nCRuleFileNumRecords,pSortedCRuleIndex);
	if(*pbKillThread == true)
	{
		AfxEndThread(0);
	}	
	delete pSortValue;

	int nf_nameColumn = CRuleFile.FindField("f_name");
	ASSERT(nf_nameColumn !=-1);
	CString sString;
	CString sString2;
	int nLoop2=0;
	//pProgressBar->SetPos(0);
	//pProgressBar->SetRange32(0,nCRuleFileNumRecords);
	//pProgressBar->SetStep(1);
	for(nLoop=0;nLoop<nCRuleFileNumRecords;nLoop++)
	{


		if(*pbKillThread == true)
		{
			AfxEndThread(0);
		}
		//pProgressBar->StepIt();
		sString = *(CString*)CRuleFile.ReturnValue(nf_nameColumn,pSortedCRuleIndex[nLoop]);
		sString.Remove(' ');
		facet_name[facet]=sString;
		facet++;
		for(nLoop2 = nLoop+1;nLoop2<nCRuleFileNumRecords;nLoop2++)
		{

			if(*pbKillThread == true)
			{
				AfxEndThread(0);
			}

			sString2 = *(CString*)CRuleFile.ReturnValue(nf_nameColumn,pSortedCRuleIndex[nLoop2]);
			sString2.Remove(' ');
			if(sString2.CompareNoCase(sString) ==0) 
			{
				nLoop = nLoop2;
			}
			else
			{
				nLoop2 = nCRuleFileNumRecords;
			}
		}
	}
	//pProgressBar->SetPos(0);
	pProgressBar2->StepIt();
	no_facets =facet;
	int nMaxRec = FuzcFile.GetNumRecords();
	int nNumDone = 0;
	
	int nFuzcFileSize = nMaxRec;
	int nMissingColumn = FuzcFile.FindField("Missing");
	ASSERT(nMissingColumn!=-1);

	bool* pMissing = (bool*) FuzcFile.ReturnPointer(nMissingColumn);
	int nColumnLoc;
	short int* pColumnPointer [30];

	
	for (nLoop = 0;nLoop<no_facets;nLoop++)
	{
		nColumnLoc = FuzcFile.FindField(facet_name[nLoop]);
		ASSERT(nColumnLoc !=-1);
		pColumnPointer[nLoop] = (short int*)FuzcFile.ReturnPointer(nColumnLoc);
		if(*pbKillThread == true)
		{
			AfxEndThread(0);
		}
	}
	
	nColumnLoc = FuzcFile.FindField("Max_facet");
	ASSERT(nColumnLoc !=-1);
	int* pMaxFacet = (int*) FuzcFile.ReturnPointer(nColumnLoc);

	nColumnLoc = FuzcFile.FindField("next_facet");
	ASSERT(nColumnLoc !=-1);
	int* pNextFacet = (int*) FuzcFile.ReturnPointer(nColumnLoc);


	nColumnLoc = FuzcFile.FindField("Fac4");
	ASSERT(nColumnLoc !=-1);
	int* pFac4 = (int*) FuzcFile.ReturnPointer(nColumnLoc);


	//pProgressBar->SetPos(0);
	//pProgressBar->SetRange32(0,nFuzcFileSize);
	//pProgressBar->SetStep(1);


	for(nLoop=0;nLoop<nFuzcFileSize;nLoop++)
	{

		if(*pbKillThread == true)
		{
			AfxEndThread(0);
		}

		//pProgressBar->StepIt();
		if(pMissing[nLoop] ==false)
		{
			maxval =0;
			nextval = 0;
			maxclass = 0;
			nextclass = 0;
			for(nLoop2=0;nLoop2<no_facets;nLoop2++)
			{

				if(*pbKillThread == true)
				{
					AfxEndThread(0);
				}

				if(pColumnPointer[nLoop2][nLoop] >=maxval)
				{
					nextval = maxval;
					maxval = pColumnPointer[nLoop2][nLoop];
					nextclass = maxclass;
					maxclass = nLoop2+1;
				}
				else
				{
					if(pColumnPointer[nLoop2][nLoop]>= nextval)
					{
						nextval = pColumnPointer[nLoop2][nLoop];
						nextclass = nLoop2+1;
					}
				}

			}

		}
		else
		{
			maxclass = 0;
			nextclass = 0;
		}

		pMaxFacet[nLoop] = maxclass;
		pNextFacet[nLoop] = nextclass;

		if(pMissing[nLoop] == true) 
		{
			pFac4[nLoop] = 0;
		}
		else
		{
			if(maxclass<4)
			{
				pFac4[nLoop]=1;
				num1 = num1+1;
			}
			if(maxclass > 3 && maxclass < 10)
			{
				pFac4[nLoop]=2;
				num2 = num2 + 1;
			}
			if(maxclass> 9 && maxclass <14)
			{
				pFac4[nLoop]=3;
				num3 = num3 + 1;
			}
			if(maxclass>13)
			{
				pFac4[nLoop] = 4;
				num4 = num4 + 1;
			}
		}


	}
	
	//pProgressBar->SetPos(0);
	//pProgressBar2->StepIt();

	*sMessage = "Saving " + *pDirectory + *p3CharName + "fuzc.dbf";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);	

	FuzcFile.SaveToFile(*pDirectory + *p3CharName + "fuzc.dbf",pProgressBar);

	FuzcFile.Empty();
	*sMessage = "Complete";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,1,(long) sMessage);	
	pProgressBar2->StepIt();
	delete pSortedCRuleIndex;
	//delete pArray;
	DEMFile.Empty();
	CRuleFile.Empty();
	FuzaFile.Empty();
	GeoFile.Empty();
	LSMFile.Empty();
	FormFile.Empty();
	//ZoneFile.Empty();
	relzfile.Empty();
	//AfxMessageBox("Done");
}




void CFacetProcess::CALC_DSS(CPtrArray *pArray)
{
	HWND phWnd = (HWND) pArray->GetAt(9);
	pWndNotifyProgress = CWnd::FromHandle((HWND)pArray->GetAt(9));
	CString* sMessage = new CString;


	pbKillThread = (bool*) pArray->GetAt(14);


	CProgressCtrl* pProgressBar = (CProgressCtrl*) pArray->GetAt(0);
	CProgressCtrl* pProgressBar2 =(CProgressCtrl*) pArray->GetAt(1);
	pProgressBar2->SetRange(0,6);
	pProgressBar2->SetPos(0);


	CString* pZoneFileName = (CString*) pArray->GetAt(10);
	CString* pFormFile = (CString*) pArray->GetAt(7);
	CString* pGeoFile = (CString*) pArray->GetAt(11);
	CString * pRelzFile =(CString*) pArray->GetAt(6);

	CString* pDirectory = (CString*) pArray->GetAt(12);
	CString* p3CharName = (CString*) pArray->GetAt(13);


	*sMessage = "Reading "  + *pFormFile;
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
	if(!FormFile.OpenFile(pFormFile->GetBuffer(1),pProgressBar, pbKillThread ))
	{
		AfxMessageBox("Could not open Form file " + *pFormFile);
		exit(1);
	}
	pProgressBar2->StepIt();
	*sMessage = "Reading "  + *pGeoFile;
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
	if(!GeoFile.OpenFile(pGeoFile->GetBuffer(1),pProgressBar, pbKillThread ))
	{
		AfxMessageBox("Could not open Geo file " + *pGeoFile);
		exit(1);
	}
	pProgressBar2->StepIt();
	*sMessage = "Reading "  + *pRelzFile;
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
	if(!relzfile.OpenFile(pRelzFile->GetBuffer(1),pProgressBar, pbKillThread ))
	{
		AfxMessageBox("Could not open Relz file " + *pRelzFile);
		exit(1);
	}
	pProgressBar2->StepIt();

	if(*pbKillThread == true)
	{
		AfxEndThread(0);
	}

	
	int no_facets = 0;
	int facet = 0;



	CString max_class;
	double max_code = 0;
	double max_value = 0;
	
	//Create the DSS table
	DSSFile.AddField("SEQNO",'I',9);
	DSSFile.AddField("MISSING",'L',1);
	DSSFile.AddField("BEC_ZONE",'I',5);
	DSSFile.AddField("MAX_CLASS",'C',8);
	DSSFile.AddField("MAX_CODE",'I',6);
	DSSFile.AddField("MAX_VALUE",'S',3);

	CDbfFile ZoneFile;
	*sMessage = "Reading "  + *pZoneFileName;
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
	
	if(!ZoneFile.OpenFile(pZoneFileName->GetBuffer(1),pProgressBar, pbKillThread ))
	{
		AfxMessageBox("Could not open Zone file " + *pZoneFileName);
		exit(1);
	}
	pProgressBar2->StepIt();
	*sMessage = "Running Calculations...";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);	
	int nColNum = 0;
	
	nColNum = ZoneFile.FindField("SEQNO");
	ASSERT(nColNum !=-1);
	int* pZoneSeqNo = (int*) ZoneFile.ReturnPointer(nColNum);

	nColNum = ZoneFile.FindField("MISSING");
	ASSERT(nColNum !=-1);
	bool* pZoneMissing = (bool*) ZoneFile.ReturnPointer(nColNum);

	nColNum = ZoneFile.FindField("bec_zone");
	ASSERT(nColNum !=-1);
	int * pZoneBecZone = (int*) ZoneFile.ReturnPointer(nColNum);

	int nNumOfRecords = ZoneFile.GetNumRecords();
	DSSFile.SetInitialFileSize(nNumOfRecords);

	nColNum = (int) DSSFile.FindField("SeqNo");
	ASSERT(nColNum!=-1);
	int* pDSSSeqNo = (int*) DSSFile.ReturnPointer(nColNum);

	nColNum = (int) DSSFile.FindField("MISSING");
	ASSERT(nColNum!=-1);
	bool* pDSSMissing = (bool*) DSSFile.ReturnPointer(nColNum);

	nColNum = (int) DSSFile.FindField("bec_zone");
	ASSERT(nColNum!=-1);
	int* pDSSBecZone = ( int*) DSSFile.ReturnPointer(nColNum);

	int nLoop = 0;
	


	nColNum = (int) DSSFile.FindField("MAX_CLASS");
	ASSERT(nColNum!=-1);
	CString* pMaxClass = (CString*) DSSFile.ReturnPointer(nColNum);

	nColNum = (int) DSSFile.FindField("MAX_CODE");
	ASSERT(nColNum!=-1);
	int* pMaxCode = (int*) DSSFile.ReturnPointer(nColNum);

	nColNum = (int) DSSFile.FindField("MAX_VALUE");
	ASSERT(nColNum!=-1);
	short int* pMaxValue = (short int*) DSSFile.ReturnPointer(nColNum);


	for(nLoop=0;nLoop<nNumOfRecords;nLoop++)
	{
		DSSFile.AddRecord();
		pDSSSeqNo[nLoop] = pZoneSeqNo[nLoop];
		pDSSMissing[nLoop] = pZoneMissing[nLoop];
		pDSSBecZone[nLoop] = pZoneBecZone[nLoop];
		if(*pbKillThread == true)
		{
			AfxEndThread(0);
		}
	}
	
	ZoneFile.Empty();


	//QuickSort does not work very good. I have to use my own sorting to improve performance.

	
	//pDSSBecZone[nLoop]

	int nNumRec = nNumOfRecords;
	int* pSortValue = new int[nNumRec];
	int* pSortTemp = new int[nNumRec];
	int* pSortedIndex = new int[nNumRec];

	int nSmallest = 99999;

	for(nLoop=0;nLoop<nNumRec;nLoop++)
	{
		pSortValue[nLoop] = pDSSBecZone[nLoop];
		//pSortedIndex[nLoop] = nLoop;
		if(nSmallest > pDSSBecZone[nLoop])
		{
			nSmallest = pDSSBecZone[nLoop];
		}
	}
	int nPointer = 0;
	int nNextHighest = 999999;
	bool bExit = false;
	while(bExit == false)
	{
		for(nLoop=0;nLoop<nNumRec;nLoop++)
		{
			if(pSortValue[nLoop] == nSmallest)
			{
				pSortTemp[nPointer] = nLoop;
				nPointer ++;
			}
			if(pSortValue[nLoop] < nNextHighest && pSortValue[nLoop]> nSmallest)
			{
				nNextHighest = pSortValue[nLoop];
			}
		}
		if(nNextHighest == nSmallest)
		{
			bExit = true;
		}
		nSmallest = nNextHighest;
		nNextHighest = 999999;
	}
	//QuickSort(pSortValue,nNumRec,pSortedIndex);

	delete pSortValue;

	for(nLoop=0;nLoop<nNumRec;nLoop++)
	{
		pSortedIndex[nLoop] = pSortTemp[nLoop];
	}
	delete pSortTemp;


	if(*pbKillThread == true)
	{
		AfxEndThread(0);
	}
	bool bChanged = true;
	int nTemp = 0;

	//delete pSortVal;

	short int currzone = 0;
	bool bMissing = false;
	CString strCRule;
	CString strARule;
	char szString[255];
	CDbfFile ARuleFile;
	CDbfFile CRuleFile;

	//Variables used by get_arule2 function
	int no_models = 0;
	long** infield = new long*[40];
	CString outattr[100];
	int model[40];
	double B[40];
	double b_low[40];
	double b_hi[40];
	double b1[40];
	double b2[40];
	double D[40];
	
	
	CString facet_name[30];
	CString fuzz_attr[900];
	int fuzz_attrIndex[900];
	int facet_code[30];
	double attr_wt[900];
	int no_attrib[30];
	int total_wt[30];

	CString FieldType[40];
	bExit = false;
	
	int nLoop2=0;
	int nLoop3=0;
	CString sVariableName;


	int nIndex = 0;

	//pProgressBar->SetRange32(0,nNumOfRecords);
	//pProgressBar->SetPos(0);
	int nSW_Aspect = 0;
	int nNE_Aspect = 0;
	int nSteep = 0;
	int nSlopeLt20 =0;


	int nLoop5;
	int nLoop6;
	CString sAttrName;
	CString sClass_Name;

	int nFuzAttrIndex=0;
	int nClass_OutIndex=0;
				
	bool bFound = false;
	for(nLoop=0;nLoop<nNumOfRecords;nLoop++)
	{
		if(*pbKillThread == true)
		{
			AfxEndThread(0);
		}
		nIndex = pSortedIndex[nLoop];
		//pProgressBar->SetPos(nLoop);
		currzone = pDSSBecZone[pSortedIndex[nLoop]];
		bMissing = pDSSMissing[pSortedIndex[nLoop]];
		if(currzone>0 && bMissing == false)
		{
			strARule = pZoneFileName->Left(pZoneFileName->GetLength() - 11) + "Arule";
			_itoa(currzone,szString,10);
			strARule = strARule + szString + ".dbf";
			if(!ARuleFile.OpenFile(strARule,pProgressBar, pbKillThread ))
			{
				AfxMessageBox("Could not open rule file " + strARule);
				exit(1);
			}

			strCRule = pZoneFileName->Left(pZoneFileName->GetLength() - 11) + "Crule";
			_itoa(currzone,szString,10);
			strCRule = strCRule + szString + ".dbf";
			if(!CRuleFile.OpenFile(strCRule,pProgressBar, pbKillThread ))
			{
				AfxMessageBox("Could not open rule file " + strCRule);
				exit(1);
			}
			

			


			
			
			GET_ARULE2(&ARuleFile,&no_models, infield, outattr, model, B,b_low, b_hi,b1, b2,D, FieldType);
			GET_CRULE2(&CRuleFile,facet_name, fuzz_attr,facet_code,attr_wt,no_attrib,total_wt, &no_facets);
			//pProgressBar->SetRange32(0,nNumOfRecords);			
			//pProgressBar->SetPos(nLoop);			
			for(nLoop2=0;nLoop2<no_models;nLoop2++)
			{
				sVariableName = outattr[nLoop2];

				for(nLoop3=0;nLoop3<900;nLoop3++)
				{
					if(sVariableName.CompareNoCase(fuzz_attr[nLoop3]) ==0)
					{
						fuzz_attrIndex[nLoop3] = nLoop2;

					}
					if(*pbKillThread == true)
					{
						AfxEndThread(0);
					}
				}

			}

			nFuzAttrIndex= CRuleFile.FindField("FuzAttr");
			if(nFuzAttrIndex ==-1)
			{
				AfxMessageBox("Could not find field FuzAttr");
				AfxEndThread(0);
			}
			nClass_OutIndex= ARuleFile.FindField("Class_Out");
			if(nClass_OutIndex ==-1)
			{
				AfxMessageBox("Could not find field Class_Out");
				AfxEndThread(0);
			}
			for(nLoop5=0;nLoop5<CRuleFile.GetNumRecords();nLoop5++)
			{
				bFound = false;
				sAttrName = *(CString*)CRuleFile.ReturnValue(nFuzAttrIndex,nLoop5);
				sAttrName.TrimRight();

				for(nLoop6=0;nLoop6<ARuleFile.GetNumRecords();nLoop6++)
				{
					sClass_Name = *(CString*)ARuleFile.ReturnValue(nClass_OutIndex,nLoop6);
					sClass_Name.TrimRight();
					if(sClass_Name == sAttrName)
					{
						bFound = true;
						nLoop6 = ARuleFile.GetNumRecords();

					}
				}
				if(bFound == false)
				{
					AfxMessageBox("Cound not find value " + sAttrName + " in ARule file.");
					AfxEndThread(0);
				}

			}
			

			










			for(nLoop2=0;nLoop2<no_models;nLoop2++)
			{
				if(outattr[nLoop2].CompareNoCase("SW_Aspect") ==0)
				{
					nSW_Aspect = nLoop2;
				}
				if(outattr[nLoop2].CompareNoCase("NE_Aspect") ==0)
				{
					nNE_Aspect = nLoop2;
				}
				if(outattr[nLoop2].CompareNoCase("Steep") ==0)
				{
					nSteep = nLoop2;
				}
				if(outattr[nLoop2].CompareNoCase("SlopeLt20") ==0)
				{
					nSlopeLt20 = nLoop2;
				}
			}
			
					 
			
			//Set the location of each value
			for(nLoop2=0;nLoop2<900;nLoop2++)
			{
				if(fuzz_attr[nLoop2].CompareNoCase("steep_sw")==0)
				{
					fuzz_attrIndex[nLoop2] = no_models;
				}
				if(fuzz_attr[nLoop2].CompareNoCase("steep_ne")==0)
				{
					fuzz_attrIndex[nLoop2] = no_models +1;
				}
				if(fuzz_attr[nLoop2].CompareNoCase("gentle_sw")==0)
				{
					fuzz_attrIndex[nLoop2] = no_models +2;
				}
				if(fuzz_attr[nLoop2].CompareNoCase("gentle_ne")==0)
				{
					fuzz_attrIndex[nLoop2] = no_models +3;
				}
			}
			
			bExit = false;
			while(bExit ==false && currzone == pDSSBecZone[pSortedIndex[nLoop]])
			{
				if(pDSSMissing[pSortedIndex[nLoop]] ==false)
				{
					CALC_CLASS(&FormFile,no_models,nIndex,FieldType,infield, model, B,
						b_low,b_hi, b1,b2,D,outattr,nSW_Aspect,nNE_Aspect,nSteep,nSlopeLt20,
						&max_code,&max_class,&max_value, 
						attr_wt, fuzz_attr, &no_facets,facet_name,facet_code,
						fuzz_attrIndex, no_attrib);
					pMaxClass[pSortedIndex[nLoop]] = max_class;
					pMaxCode[pSortedIndex[nLoop]] = (int)max_code;
					pMaxValue[pSortedIndex[nLoop]] = (short int)(max_value + 0.5);


				}
				else
				{
					pMaxClass[pSortedIndex[nLoop]] = "NA";
					pMaxCode[pSortedIndex[nLoop]] = 0;
					pMaxValue[pSortedIndex[nLoop]] = 0;
				}
				if(nLoop < nNumOfRecords -1)
				{
					nLoop++;
					nIndex = pSortedIndex[nLoop];

					
					//pProgressBar->StepIt();
				}
				else
				{
					bExit = true;
				}
				if(*pbKillThread == true)
				{
					AfxEndThread(0);
				}

			}

			ARuleFile.Empty();
			CRuleFile.Empty();
			if(currzone != pDSSBecZone[pSortedIndex[nLoop]])
			{
				nLoop--;
			}
		}
		else
		{
			bExit = true;
			if(pDSSBecZone[pSortedIndex[nLoop]] <=0 || 
				pDSSMissing[pSortedIndex[nLoop]] ==true )
			{
				bExit = false;
			}
			while(bExit ==false)
			{
				pMaxClass[pSortedIndex[nLoop]] = "NA";
				pMaxCode[pSortedIndex[nLoop]] = 0;
				pMaxValue[pSortedIndex[nLoop]] = 0;
				if(nLoop < nNumOfRecords -1)
				{	
					nLoop++;
					if(	!(pDSSBecZone[pSortedIndex[nLoop]] <=0 || 
						pDSSMissing[pSortedIndex[nLoop]] ==true) )
					{	
						bExit = true;
					}
				}
				else
				{
					bExit = true;
				}
				if(*pbKillThread == true)
				{
					AfxEndThread(0);
				}

			}
			
		}

	}
	pProgressBar2->StepIt();	
	*sMessage = "Saving " + *pDirectory + *p3CharName + "dss.dbf";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
	if(DSSFile.SaveToFile(*pDirectory + *p3CharName + "dss.dbf" ,pProgressBar) == false)
	{
		AfxMessageBox("Could not save file " + *pDirectory + *p3CharName + "dss.dbf");
		exit(1);
	}
	DSSFile.Empty();
	*sMessage = "Complete.";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,1,(long) sMessage);
	//AfxMessageBox("Done");
	pProgressBar2->StepIt();
	delete pSortedIndex;
	delete infield;
	//delete pArray;
	DEMFile.Empty();
	CRuleFile.Empty();
	FuzaFile.Empty();
	GeoFile.Empty();
	LSMFile.Empty();
	FormFile.Empty();
	ZoneFile.Empty();
	relzfile.Empty();
}

void CFacetProcess::GET_ARULE2(CDbfFile* ARuleFile, int *no_models, long **infield,
							   CString *outattr, int *model, double *b, double *b_low, 
							   double *b_hi, double *b1, double *b2, double *D, CString* infieldType)
{
	int nNumOfRec = ARuleFile->GetNumRecords();
	int nLoop = 0;

	*no_models = 0;

	
	int nColLoc = 0;
	nColLoc = ARuleFile->FindField("file_in");
	ASSERT(nColLoc!=-1);
	CString* pfilein = (CString*) ARuleFile->ReturnPointer(nColLoc);


	nColLoc = ARuleFile->FindField("attr_in");
	ASSERT(nColLoc!=-1);
	CString* pattrin = (CString*) ARuleFile->ReturnPointer(nColLoc);


	nColLoc = ARuleFile->FindField("class_out");
	ASSERT(nColLoc!=-1);
	CString* pclassout = (CString*) ARuleFile->ReturnPointer(nColLoc);

	nColLoc = ARuleFile->FindField("model_no");
	ASSERT(nColLoc!=-1);
	int* pmodelno = (int*) ARuleFile->ReturnPointer(nColLoc);


	nColLoc = ARuleFile->FindField("B");
	ASSERT(nColLoc!=-1);
	double* pB = (double*) ARuleFile->ReturnPointer(nColLoc);
	
	nColLoc = ARuleFile->FindField("B_low");
	ASSERT(nColLoc!=-1);
	double* pBlow = (double*) ARuleFile->ReturnPointer(nColLoc);

	nColLoc = ARuleFile->FindField("B_hi");
	ASSERT(nColLoc!=-1);
	double* pBhi = (double*) ARuleFile->ReturnPointer(nColLoc);

	nColLoc = ARuleFile->FindField("B1");
	ASSERT(nColLoc!=-1);
	double* pB1 = (double*) ARuleFile->ReturnPointer(nColLoc);

	nColLoc = ARuleFile->FindField("B2");
	ASSERT(nColLoc!=-1);
	double* pB2 = (double*) ARuleFile->ReturnPointer(nColLoc);

	nColLoc = ARuleFile->FindField("D");
	ASSERT(nColLoc!=-1);
	double* pD = (double*) ARuleFile->ReturnPointer(nColLoc);






	CString curtext;
	CDbfFile* pcurfile;
	CString curalias;
	CString sString;
	
	CString sDataType;
	CString sString1;
	int nNumOfFiles= 0;
	for(nLoop=0;nLoop<nNumOfRec;nLoop++)
	{
		sString = pfilein[nLoop];
		sString.Remove(' ');
		sString.MakeUpper();
		if(sString != curtext)
		{
			curtext = sString;
			if(curtext =="FORMFILE")
			{
				pcurfile = &FormFile;
				//FileName[nLoop] = "FORMFILE";
			}
			if(curtext =="GEOFILE")
			{
				//FileName[nLoop] = "GEOFILE";
				pcurfile = &GeoFile;
			}
			if(curtext == "RELZFILE")
			{
				pcurfile = &relzfile;
			}
			nNumOfFiles++;
			

		}
		curalias = "";
		*no_models = *no_models + 1;
		sString1 = pattrin[nLoop];
		sString1.Remove(' ');
		sString1.MakeUpper();
		nColLoc = pcurfile->FindField(sString1);

		ASSERT(nColLoc!=-1);
		sDataType = pcurfile->ReturnType(nColLoc);


		infieldType[nLoop] = sDataType;
		infield[nLoop] = pcurfile->ReturnPointer(nColLoc);
	
		sString1 = pclassout[nLoop];
		sString1.Remove(' ');
		sString1.MakeUpper();
		outattr[nLoop] = sString1;

		model[nLoop]  = pmodelno[nLoop];
		b[nLoop] = pB[nLoop];
		b_low[nLoop] = pBlow[nLoop];
		b_hi[nLoop] = pBhi[nLoop];
		b1[nLoop] = pB1[nLoop];
		b2[nLoop] = pB2[nLoop];
		D[nLoop] = pD[nLoop];

		

		
	}


}

void CFacetProcess::GET_CRULE2(CDbfFile *CRuleFile, CString *facet_name, 
							   CString* fuzz_attr, int *facet_code, double *attr_wt, 
							   int *no_attrib, int *total_wt, int* no_facets)
{
	int nNumOfRec = CRuleFile->GetNumRecords();
	int nLoop = 0;
	
	int nColLoc = 0;
	nColLoc = CRuleFile->FindField("facet_no");
	ASSERT(nColLoc!=-1);
	int * pfacetno = (int*) CRuleFile->ReturnPointer(nColLoc);

	nColLoc = CRuleFile->FindField("attrwt");
	ASSERT(nColLoc!=-1);
	int * pattrwt = (int*) CRuleFile->ReturnPointer(nColLoc);

	double* pSortValue  = new double[nNumOfRec];
	int* pSortedIndex = new int [nNumOfRec];

	for(nLoop=0;nLoop<nNumOfRec;nLoop++)
	{
		pSortValue[nLoop] = pfacetno[nLoop] *100 + (99-pattrwt[nLoop]);
		pSortedIndex[nLoop] = nLoop;
	}
	QuickSort(pSortValue,nNumOfRec,pSortedIndex);
	delete pSortValue;

	*no_facets = 0;
	int facet = 0;

	nColLoc = CRuleFile->FindField("f_name");
	ASSERT(nColLoc!=-1);
	CString * pfname = (CString*) CRuleFile->ReturnPointer(nColLoc);

	nColLoc = CRuleFile->FindField("f_code");
	ASSERT(nColLoc!=-1);
	int * pcode = (int*) CRuleFile->ReturnPointer(nColLoc);

	nColLoc = CRuleFile->FindField("fuzattr");
	ASSERT(nColLoc!=-1);
	CString * pfuzzatr = (CString*) CRuleFile->ReturnPointer(nColLoc);


	
	int attribno = 0;
	CString sString;
	CString sFacetName;
	bool bExit = false;
	for(nLoop=0;nLoop<nNumOfRec;nLoop++)
	{
		*no_facets = *no_facets + 1;
		//facet = no_facets;
		no_attrib[facet] = 0;
		sString = pfname[pSortedIndex[nLoop]];
		sString.Remove(' ');
		sString.MakeUpper();
		facet_name[facet] = sString;
		facet_code[facet] = pcode[pSortedIndex[nLoop]];

		total_wt[facet] = 0;
		attribno = 0;
		sFacetName = sString;
		bExit = false;
		while(sFacetName.CompareNoCase(facet_name[facet]) == 0 && bExit ==false)
		{
			
			no_attrib[facet]++;
			sString = pfuzzatr[pSortedIndex[nLoop]];
			sString.Remove(' ');
			sString.MakeUpper();

			fuzz_attr[facet * 30 +attribno] = sString;
			attr_wt[facet*30 + attribno] = pattrwt[pSortedIndex[nLoop]];
			
			total_wt[facet] = total_wt[facet] + pattrwt[pSortedIndex[nLoop]];

			if(nLoop<nNumOfRec -1)
			{
				nLoop ++;
				sFacetName = pfname[pSortedIndex[nLoop]];
				sFacetName.Remove(' ');
			}
			else
			{
				bExit = true;
			}
			attribno++;
		}
		if(bExit == false)
		{
			nLoop--;
		}
		//*no_facets = *no_facets + 1;
		facet++;

		

	}
	int nLoop2=0;
	for(nLoop=0;nLoop<*no_facets;nLoop++)
	{
		for(nLoop2=0;nLoop2<no_attrib[nLoop];nLoop2++)
		{
			attr_wt[nLoop*30 + nLoop2] = attr_wt[nLoop*30 + nLoop2] /total_wt[nLoop];
		}
	}
	delete pSortedIndex;
}

void CFacetProcess::CALC_CLASS(CDbfFile *FormFile, int no_models, int nIndex, 
							   CString* FieldType, long** infield, int* model,
							   double* B, double* b_low, double* b_hi, double* b1,
							   double* b2, double *D, CString* outattr,int nSW_Aspect,
							   int nNE_Aspect, int nSteep,int nSlopeLt20, double* Max_Code,
							   CString* Max_Class, double* Max_Value, double* attr_wt, 
							   CString* fuzz_attr,int* no_facets,CString* Facet_Name, 
							   int* Facet_Code, int* fuzz_attrIndex, int* no_attrib)
{
	double fazzval = 0;
	int nLoop = 0;
	CString sString;
	double* pDouble;
	float* pFloat;
	int* pInt;
	short int* pShortInt;
	double x = 0.0;

	double outattrval[70];
	for(nLoop=0;nLoop<no_models;nLoop++)
	{
		if(FieldType[nLoop].CompareNoCase("D") ==0)
		{
			pDouble = (double*) infield[nLoop];
			x = pDouble[nIndex];
		}

		if(FieldType[nLoop].CompareNoCase("I") ==0)
		{
			pInt = (int*) infield[nLoop];
			x = pInt[nIndex];
		}

		if(FieldType[nLoop].CompareNoCase("F") ==0)
		{
			pFloat = (float*) infield[nLoop];
			x = pFloat[nIndex];
		}

		if(FieldType[nLoop].CompareNoCase("S") ==0)
		{
			pShortInt = (short int*) infield[nLoop];
			x = pShortInt[nIndex];
		}

		switch(model[nLoop])
		{
		case(1):
			fazzval = 1/(1+pow((x-B[nLoop])/D[nLoop],2));
			break;
		case(2):
			if(x>b_low[nLoop] && x<b_hi[nLoop])
			{
				fazzval = 1;
			}
			if(x<=b_low[nLoop])
			{
				fazzval = 1/(1+pow((x-b_low[nLoop])/D[nLoop],2));
			}
			if(x>=b_hi[nLoop])
			{
				fazzval = 1/(1+pow((x-b_hi[nLoop])/D[nLoop],2));
			}
			break;
		case(3):
			if(x>b1[nLoop] && x<b2[nLoop])
			{
				fazzval = 1;
			}
			if(x<=b1[nLoop])
			{
				fazzval = 1/(1+pow((x-b1[nLoop])/D[nLoop],2));
			}
			if(x>=b2[nLoop])
			{
				fazzval = 1/(1+pow((x-b2[nLoop])/D[nLoop],2));
			}
			break;
		case(4):
			if(x>B[nLoop])
			{
				fazzval = 1;
			}
			else
			{
				fazzval = 1/(1+pow((x-B[nLoop])/D[nLoop],2));
			}
			break;
		case(5):
			if(x<B[nLoop])
			{
				fazzval = 1;
			}
			else
			{
				fazzval = 1/(1+pow((x-B[nLoop])/D[nLoop],2));
			}
			break;
		}
		fazzval = fazzval*100;
		outattrval[nLoop] = fazzval;

	}

	int nColLoc = FormFile->FindField("New_Asp");
	ASSERT(nColLoc!=-1);
	short int* pNewAsp = (short int*)FormFile->ReturnPointer(nColLoc);


	/*
	for(nLoop=0;nLoop<no_models;nLoop++)
	{
		if(outattr[nLoop].CompareNoCase("SW_Aspect") ==0)
		{
			nSW_Aspect = nLoop;
		}
		if(outattr[nLoop].CompareNoCase("NE_Aspect") ==0)
		{
			nNE_Aspect = nLoop;
		}
		if(outattr[nLoop].CompareNoCase("Steep") ==0)
		{
			nSteep = nLoop;
		}
		if(outattr[nLoop].CompareNoCase("SlopeLt20") ==0)
		{
			nSlopeLt20 = nLoop;
		}
	}
	
	  */
	
		

	if(pNewAsp[nIndex] >180)
	{
		outattrval[nNE_Aspect] = 0;
	}
	if(pNewAsp[nIndex] <180)
	{
		outattrval[nSW_Aspect] = 0;
	}
	
	double steep_sw = 0;
	double steep_ne = 0;
	double gentle_sw = 0;
	double gentle_ne = 0;

	steep_sw = (outattrval[nSteep] * outattrval[nSW_Aspect])/100;
	steep_ne = (outattrval[nSteep] * outattrval[nNE_Aspect])/100;

	gentle_sw = (outattrval[nSlopeLt20] * outattrval[nSW_Aspect])/100;
	gentle_ne = (outattrval[nSlopeLt20] * outattrval[nNE_Aspect])/100;
	
	//Copy the values to the array	
	outattrval[no_models] = steep_sw;
	outattrval[no_models+1] = steep_ne;
	outattrval[no_models+2] = gentle_sw;
	outattrval[no_models+3] = gentle_ne;
	


	/*
	//Set the location of each value
	for(nLoop=0;nLoop<900;nLoop++)
	{
		if(fuzz_attr[nLoop].CompareNoCase("steep_sw")==0)
		{
			fuzz_attrIndex[nLoop] = no_models;
		}
		if(fuzz_attr[nLoop].CompareNoCase("steep_ne")==0)
		{
			fuzz_attrIndex[nLoop] = no_models +1;
		}
		if(fuzz_attr[nLoop].CompareNoCase("gentle_sw")==0)
		{
			fuzz_attrIndex[nLoop] = no_models +2;
		}
		if(fuzz_attr[nLoop].CompareNoCase("gentle_ne")==0)
		{
			fuzz_attrIndex[nLoop] = no_models +3;
		}
	}
	*/


	*Max_Code=0;
	*Max_Value = 0;
	*Max_Class = "";
	double classval = 0;
	double attrib_val = 0;
	int i=0;
	for(nLoop=0;nLoop<*no_facets;nLoop++)
	{
		classval = 0;
		for(i=0;i<no_attrib[nLoop];i++)
		{
			attrib_val = outattrval[fuzz_attrIndex[nLoop*30 + i]];
			classval = classval  + attrib_val * attr_wt[nLoop*30 + i];
		}
		if(classval >= *Max_Value)
		{
			*Max_Value = classval;
			*Max_Class = Facet_Name[nLoop];
			*Max_Code = Facet_Code[nLoop];
		}
		
	}


}


void CFacetProcess::CALC_LSM(CPtrArray *pArray)
{

	CProgressCtrl* pProgressBar = (CProgressCtrl*) pArray->GetAt(0);
	CProgressCtrl* pProgressBar2 =(CProgressCtrl*) pArray->GetAt(1);
	pbKillThread = (bool*) pArray->GetAt(14);

	


	pProgressBar2->SetRange(0,7);
	pProgressBar2->SetPos(0);



	CString* pDEMFileName = (CString*) pArray->GetAt(2);

	HWND phWnd = (HWND) pArray->GetAt(9);
	pWndNotifyProgress = CWnd::FromHandle((HWND)pArray->GetAt(9));
	CString* sMessage = new CString;


	CString* pDirectory = (CString*) pArray->GetAt(12);
	CString* p3CharName = (CString*) pArray->GetAt(13);
	*sMessage = "Reading "  + *pDEMFileName;
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
	if(!DEMFile.OpenFile(pDEMFileName->GetBuffer(1),pProgressBar,pbKillThread))
	{
		AfxMessageBox("Could not open Dem file " + *pDEMFileName);
		exit(1);
	}
	
	if(*pbKillThread == true)
	{
		AfxEndThread(0);
	}
	pProgressBar2->StepIt();


	//Have to remove
	CString* pZoneFileName = (CString*) pArray->GetAt(10);
	CString* pFormFile = (CString*) pArray->GetAt(7);
	CString* pGeoFile = (CString*) pArray->GetAt(11);
	CString * pRelzFile =(CString*) pArray->GetAt(6);

	CString* pFuzzyAttributeFile = (CString*) pArray->GetAt(3);
	CString* pFuzzyFacetFile = (CString*) pArray->GetAt(4);
	
	*sMessage = "Reading "  + *pFormFile;
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
	if(!FormFile.OpenFile(pFormFile->GetBuffer(1),pProgressBar, pbKillThread ))
	{
		AfxMessageBox("Could not open Form file " + *pFormFile);
		exit(1);
	}
	pProgressBar2->StepIt();
	//GeoFile.OpenFile(pGeoFile->GetBuffer(1),pProgressBar);
	*sMessage = "Reading "  + *pRelzFile;
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
	if(!relzfile.OpenFile(pRelzFile->GetBuffer(1),pProgressBar, pbKillThread ))
	{
		AfxMessageBox("Could not open Relz file " + *pRelzFile);
		exit(1);
	}
	pProgressBar2->StepIt();
	
	int no_facets = 0;
	int facet = 0;



	CString max_class;
	double max_code = 0;
	double max_value = 0;
	
	//Create the LSM table
	LSMFile.AddField("SEQNO",'I',9);
	LSMFile.AddField("MISSING",'L',1);
	LSMFile.AddField("MAX_CLASS",'C',8);
	LSMFile.AddField("MAX_CODE",'I',6);
	LSMFile.AddField("MAX_VALUE",'S',3);

	//CDbfFile ZoneFile;
	
	//ZoneFile.OpenFile(pZoneFileName->GetBuffer(1),pProgressBar);
	
	int nColNum = 0;
	
	nColNum = DEMFile.FindField("SEQNO");
	ASSERT(nColNum !=-1);
	int* pDEMSeqNo = (int*) DEMFile.ReturnPointer(nColNum);

	nColNum = DEMFile.FindField("MISSING");
	ASSERT(nColNum !=-1);
	bool* pDEMMissing = (bool*) DEMFile.ReturnPointer(nColNum);



	int nNumOfRecords = DEMFile.GetNumRecords();
	LSMFile.SetInitialFileSize(nNumOfRecords);

	nColNum = (int) LSMFile.FindField("SeqNo");
	ASSERT(nColNum!=-1);
	int* pLSMSeqNo = (int*) LSMFile.ReturnPointer(nColNum);

	nColNum = (int) LSMFile.FindField("MISSING");
	ASSERT(nColNum!=-1);
	bool* pLSMMissing = (bool*) LSMFile.ReturnPointer(nColNum);


	int nLoop = 0;
	


	nColNum = (int) LSMFile.FindField("MAX_CLASS");
	ASSERT(nColNum!=-1);
	CString* pMaxClass = (CString*) LSMFile.ReturnPointer(nColNum);

	nColNum = (int) LSMFile.FindField("MAX_CODE");
	ASSERT(nColNum!=-1);
	int* pMaxCode = (int*) LSMFile.ReturnPointer(nColNum);

	nColNum = (int) LSMFile.FindField("MAX_VALUE");
	ASSERT(nColNum!=-1);
	short int* pMaxValue = (short int*) LSMFile.ReturnPointer(nColNum);


	for(nLoop=0;nLoop<nNumOfRecords;nLoop++)
	{
		LSMFile.AddRecord();
		pLSMSeqNo[nLoop] = pDEMSeqNo[nLoop];
		pLSMMissing[nLoop] = pDEMMissing[nLoop];
		if(*pbKillThread == true)
		{
			AfxEndThread(0);
		}
	}
	

	



	bool bChanged = true;
	int nTemp = 0;


	short int currzone = 0;
	bool bMissing = false;
	CString strCRule;
	CString strARule;
	//char szString[255];
	CDbfFile ARuleFile;
	CDbfFile CRuleFile;

	//Variables used by get_arule2 function
	int no_models = 0;
	long** infield = new long*[40];
	CString outattr[100];
	int model[40];
	double B[40];
	double b_low[40];
	double b_hi[40];
	double b1[40];
	double b2[40];
	double D[40];
	
	
	CString facet_name[30];
	CString fuzz_attr[900];
	int fuzz_attrIndex[900];
	int facet_code[30];
	double attr_wt[900];
	int no_attrib[30];
	int total_wt[30];

	CString FieldType[40];
	bool bExit = false;
	
	int nLoop2=0;
	int nLoop3=0;
	CString sVariableName;


	int nIndex = 0;

	*sMessage = "Reading "  + *pFuzzyAttributeFile;
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
	
	if(!ARuleFile.OpenFile(pFuzzyAttributeFile->GetBuffer(1),pProgressBar, pbKillThread ))
	{
		AfxMessageBox("Could not open Rule file " + *pFuzzyAttributeFile);
		exit(1);
	}
	pProgressBar2->StepIt();	
	*sMessage = "Reading "  + *pFuzzyFacetFile;
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
	if(!CRuleFile.OpenFile(pFuzzyFacetFile->GetBuffer(1),pProgressBar, pbKillThread ))
	{
		AfxMessageBox("Could not open Rule file " + *pFuzzyFacetFile);
		exit(1);
	}
	pProgressBar2->StepIt();	


	*sMessage = "Running Calculations...";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
	

	//pProgressBar->SetRange32(0,nNumOfRecords);
	//pProgressBar->SetPos(0);

	for(nLoop=0;nLoop<nNumOfRecords;nLoop++)
	{
		if(*pbKillThread == true)
		{
			AfxEndThread(0);
		}

		//pProgressBar->StepIt();

		bMissing = pLSMMissing[nLoop];

		if(bMissing == false)
		{

		
			GET_ARULE3(&ARuleFile,&no_models, infield, outattr, model, B,b_low, b_hi,b1, b2,D, FieldType);
				
			GET_CRULE3(&CRuleFile,facet_name, fuzz_attr,facet_code,attr_wt,no_attrib,total_wt, &no_facets);
			
			for(nLoop2=0;nLoop2<no_models;nLoop2++)
			{
				sVariableName = outattr[nLoop2];
				for(nLoop3=0;nLoop3<900;nLoop3++)
				{
					if(sVariableName.CompareNoCase(fuzz_attr[nLoop3]) ==0)
					{
						fuzz_attrIndex[nLoop3] = nLoop2;

					}
					if(*pbKillThread == true)
					{
						AfxEndThread(0);
					}
				}

			}



			bExit = false;
			while(bExit ==false)
			{
				if(pLSMMissing[nLoop] ==false)
				{
					nIndex = nLoop;


					CALC_CLASS3(&FormFile,no_models,nIndex,FieldType,infield, model, B,
						b_low,b_hi, b1,b2,D,outattr,&max_code,&max_class,&max_value, 
						attr_wt, fuzz_attr, &no_facets,facet_name,facet_code,
						fuzz_attrIndex, no_attrib);
					pMaxClass[nLoop] = max_class;
					pMaxCode[nLoop] = (int)max_code;
					pMaxValue[nLoop] = (short int)(max_value + 0.5);


				}
				else
				{
					pMaxClass[nLoop] = "NA";
					pMaxCode[nLoop]= 0;
					pMaxValue[nLoop] = 0;
				}
				if(nLoop < nNumOfRecords -1)
				{
					nLoop++;
					//pProgressBar->StepIt();
				}
				else
				{
					bExit = true;
				}
				if(*pbKillThread == true)
				{
					AfxEndThread(0);
				}

			}

		}
		else
		{
			bExit = true;
			if(	pLSMMissing[nLoop] ==true )
			{
				bExit = false;
			}
			while(bExit ==false)
			{
				pMaxClass[nLoop] = "NA";
				pMaxCode[nLoop] = 0;
				pMaxValue[nLoop] = 0;
				if(nLoop < nNumOfRecords -1)
				{	
					nLoop++;
					if(	!(pLSMMissing[nLoop] ==true) )
					{	
						bExit = true;
					}
				}
				else
				{
					bExit = true;
				}
				if(*pbKillThread == true)
				{
					AfxEndThread(0);
				}

			}
			
		}

	}
	
	delete infield;
	//delete pArray;
	pProgressBar2->StepIt();
	*sMessage = "Saving " + *pDirectory + *p3CharName + "lsm.dbf";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);

	if(LSMFile.SaveToFile(*pDirectory + *p3CharName + "lsm.dbf" ,pProgressBar) ==false)
	{
		AfxMessageBox("Could not save file " + *pDirectory + *p3CharName + "lsm.dbf");
		exit(1);
	}
	LSMFile.Empty();
	pProgressBar2->StepIt();
	*sMessage = "Complete";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,1,(long) sMessage);
	DEMFile.Empty();
	CRuleFile.Empty();
	FuzaFile.Empty();
	GeoFile.Empty();
	LSMFile.Empty();
	FormFile.Empty();
	//ZoneFile.Empty();
	relzfile.Empty();	
}

void CFacetProcess::GET_ARULE3(CDbfFile* ARuleFile, int *no_models, long **infield,
							   CString *outattr, int *model, double *b, double *b_low, 
							   double *b_hi, double *b1, double *b2, double *D, CString* infieldType)
{
	int nNumOfRec = ARuleFile->GetNumRecords();
	int nLoop = 0;

	*no_models = 0;

	
	int nColLoc = 0;
	nColLoc = ARuleFile->FindField("file_in");
	ASSERT(nColLoc!=-1);
	CString* pfilein = (CString*) ARuleFile->ReturnPointer(nColLoc);


	nColLoc = ARuleFile->FindField("attr_in");
	ASSERT(nColLoc!=-1);
	CString* pattrin = (CString*) ARuleFile->ReturnPointer(nColLoc);


	nColLoc = ARuleFile->FindField("class_out");
	ASSERT(nColLoc!=-1);
	CString* pclassout = (CString*) ARuleFile->ReturnPointer(nColLoc);

	nColLoc = ARuleFile->FindField("model_no");
	ASSERT(nColLoc!=-1);
	int* pmodelno = (int*) ARuleFile->ReturnPointer(nColLoc);


	nColLoc = ARuleFile->FindField("B");
	ASSERT(nColLoc!=-1);
	float* pB = (float*) ARuleFile->ReturnPointer(nColLoc);
	
	nColLoc = ARuleFile->FindField("B_low");
	ASSERT(nColLoc!=-1);
	float* pBlow = (float*) ARuleFile->ReturnPointer(nColLoc);

	nColLoc = ARuleFile->FindField("B_hi");
	ASSERT(nColLoc!=-1);
	float* pBhi = (float*) ARuleFile->ReturnPointer(nColLoc);

	nColLoc = ARuleFile->FindField("B1");
	ASSERT(nColLoc!=-1);
	float* pB1 = (float*) ARuleFile->ReturnPointer(nColLoc);

	nColLoc = ARuleFile->FindField("B2");
	ASSERT(nColLoc!=-1);
	float* pB2 = (float*) ARuleFile->ReturnPointer(nColLoc);

	nColLoc = ARuleFile->FindField("D");
	ASSERT(nColLoc!=-1);
	float* pD = (float*) ARuleFile->ReturnPointer(nColLoc);






	CString curtext;
	CDbfFile* pcurfile;
	CString curalias;
	CString sString;
	
	CString sDataType;
	CString sString1;
	int nNumOfFiles= 0;
	for(nLoop=0;nLoop<nNumOfRec;nLoop++)
	{
		sString = pfilein[nLoop];
		sString.Remove(' ');
		sString.MakeUpper();
		if(sString != curtext)
		{
			curtext = sString;
			if(curtext =="FORMFILE")
			{
				pcurfile = &FormFile;
				//FileName[nLoop] = "FORMFILE";
			}
			if(curtext =="GEOFILE")
			{
				//FileName[nLoop] = "GEOFILE";
				pcurfile = &GeoFile;
			}
			if(curtext == "RELZFILE")
			{
				pcurfile = &relzfile;
			}
			nNumOfFiles++;
			

		}
		curalias = "";
		*no_models = *no_models + 1;
		sString1 = pattrin[nLoop];
		sString1.Remove(' ');
		sString1.MakeUpper();
		nColLoc = pcurfile->FindField(sString1);

		ASSERT(nColLoc!=-1);
		sDataType = pcurfile->ReturnType(nColLoc);


		infieldType[nLoop] = sDataType;
		infield[nLoop] = pcurfile->ReturnPointer(nColLoc);
	
		sString1 = pclassout[nLoop];
		sString1.Remove(' ');
		sString1.MakeUpper();
		outattr[nLoop] = sString1;

		model[nLoop]  = pmodelno[nLoop];
		b[nLoop] = pB[nLoop];
		b_low[nLoop] = pBlow[nLoop];
		b_hi[nLoop] = pBhi[nLoop];
		b1[nLoop] = pB1[nLoop];
		b2[nLoop] = pB2[nLoop];
		D[nLoop] = pD[nLoop];

		

		
	}

}

void CFacetProcess::GET_CRULE3(CDbfFile *CRuleFile, CString *facet_name, 
							   CString* fuzz_attr, int *facet_code, double *attr_wt, 
							   int *no_attrib, int *total_wt, int* no_facets)
{
	int nNumOfRec = CRuleFile->GetNumRecords();
	int nLoop = 0;
	
	int nColLoc = 0;
	nColLoc = CRuleFile->FindField("facet_no");
	ASSERT(nColLoc!=-1);
	int * pfacetno = (int*) CRuleFile->ReturnPointer(nColLoc);

	nColLoc = CRuleFile->FindField("attrwt");
	ASSERT(nColLoc!=-1);
	int * pattrwt = (int*) CRuleFile->ReturnPointer(nColLoc);

	double* pSortValue  = new double[nNumOfRec];
	int* pSortedIndex = new int [nNumOfRec];

	for(nLoop=0;nLoop<nNumOfRec;nLoop++)
	{
		pSortValue[nLoop] = pfacetno[nLoop] *100 + (99-pattrwt[nLoop]);
		pSortedIndex[nLoop] = nLoop;
	}
	QuickSort(pSortValue,nNumOfRec,pSortedIndex);
	delete pSortValue;

	*no_facets = 0;
	int facet = 0;

	nColLoc = CRuleFile->FindField("f_name");
	ASSERT(nColLoc!=-1);
	CString * pfname = (CString*) CRuleFile->ReturnPointer(nColLoc);

	nColLoc = CRuleFile->FindField("f_code");
	ASSERT(nColLoc!=-1);
	int * pcode = (int*) CRuleFile->ReturnPointer(nColLoc);

	nColLoc = CRuleFile->FindField("fuzattr");
	ASSERT(nColLoc!=-1);
	CString * pfuzzatr = (CString*) CRuleFile->ReturnPointer(nColLoc);


	
	int attribno = 0;
	CString sString;
	CString sFacetName;
	bool bExit = false;
	for(nLoop=0;nLoop<nNumOfRec;nLoop++)
	{
		*no_facets = *no_facets + 1;
		//facet = no_facets;
		no_attrib[facet] = 0;
		sString = pfname[pSortedIndex[nLoop]];
		sString.Remove(' ');
		sString.MakeUpper();
		facet_name[facet] = sString;
		facet_code[facet] = pcode[pSortedIndex[nLoop]];

		total_wt[facet] = 0;
		attribno = 0;
		sFacetName = sString;
		bExit = false;
		while(sFacetName.CompareNoCase(facet_name[facet]) == 0 && bExit ==false)
		{
			
			no_attrib[facet]++;
			sString = pfuzzatr[pSortedIndex[nLoop]];
			sString.Remove(' ');
			sString.MakeUpper();

			fuzz_attr[facet * 30 +attribno] = sString;
			attr_wt[facet*30 + attribno] = pattrwt[pSortedIndex[nLoop]];
			
			total_wt[facet] = total_wt[facet] + pattrwt[pSortedIndex[nLoop]];

			if(nLoop<nNumOfRec -1)
			{
				nLoop ++;
				sFacetName = pfname[pSortedIndex[nLoop]];
				sFacetName.Remove(' ');
			}
			else
			{
				bExit = true;
			}
			attribno++;
		}
		if(bExit == false)
		{
			nLoop--;
		}
		//*no_facets = *no_facets + 1;
		facet++;

		

	}
	int nLoop2=0;
	for(nLoop=0;nLoop<*no_facets;nLoop++)
	{
		for(nLoop2=0;nLoop2<no_attrib[nLoop];nLoop2++)
		{
			attr_wt[nLoop*30 + nLoop2] = attr_wt[nLoop*30 + nLoop2] /total_wt[nLoop];
		}
	}
	delete pSortedIndex;

}

void CFacetProcess::CALC_CLASS3(CDbfFile *FormFile, int no_models, int nIndex, 
							   CString* FieldType, long** infield, int* model,
							   double* B, double* b_low, double* b_hi, double* b1,
							   double* b2, double *D, CString* outattr, double* Max_Code,
							   CString* Max_Class, double* Max_Value, double* attr_wt, 
							   CString* fuzz_attr,int* no_facets,CString* Facet_Name, 
							   int* Facet_Code, int* fuzz_attrIndex, int* no_attrib)
{
	double fazzval = 0;
	int nLoop = 0;
	CString sString;
	double* pDouble;
	float* pFloat;
	int* pInt;
	short int* pShortInt;
	double x = 0.0;

	double outattrval[70];
	for(nLoop=0;nLoop<no_models;nLoop++)
	{
		if(FieldType[nLoop].CompareNoCase("D") ==0)
		{
			pDouble = (double*) infield[nLoop];
			x = pDouble[nIndex];
		}

		if(FieldType[nLoop].CompareNoCase("I") ==0)
		{
			pInt = (int*) infield[nLoop];
			x = pInt[nIndex];
		}

		if(FieldType[nLoop].CompareNoCase("F") ==0)
		{
			pFloat = (float*) infield[nLoop];
			x = pFloat[nIndex];
		}

		if(FieldType[nLoop].CompareNoCase("S") ==0)
		{
			pShortInt = (short int*) infield[nLoop];
			x = pShortInt[nIndex];
		}

		switch(model[nLoop])
		{
		case(1):
			fazzval = 1/(1+pow((x-B[nLoop])/D[nLoop],2));
			break;
		case(2):
			if(x>b_low[nLoop] && x<b_hi[nLoop])
			{
				fazzval = 1;
			}
			if(x<=b_low[nLoop])
			{
				fazzval = 1/(1+pow((x-b_low[nLoop])/D[nLoop],2));
			}
			if(x>=b_hi[nLoop])
			{
				fazzval = 1/(1+pow((x-b_hi[nLoop])/D[nLoop],2));
			}
			break;
		case(3):
			if(x>b1[nLoop] && x<b2[nLoop])
			{
				fazzval = 1;
			}
			if(x<=b1[nLoop])
			{
				fazzval = 1/(1+pow((x-b1[nLoop])/D[nLoop],2));
			}
			if(x>=b2[nLoop])
			{
				fazzval = 1/(1+pow((x-b2[nLoop])/D[nLoop],2));
			}
			break;
		case(4):
			if(x>B[nLoop])
			{
				fazzval = 1;
			}
			else
			{
				fazzval = 1/(1+pow((x-B[nLoop])/D[nLoop],2));
			}
			break;
		case(5):
			if(x<B[nLoop])
			{
				fazzval = 1;
			}
			else
			{
				fazzval = 1/(1+pow((x-B[nLoop])/D[nLoop],2));
			}
			break;
		}
		fazzval = fazzval*100;
		outattrval[nLoop] = fazzval;

	}

	//int nColLoc = FormFile->FindField("New_Asp");
	//ASSERT(nColLoc!=-1);
	//short int* pNewAsp = (short int*)FormFile->ReturnPointer(nColLoc);
	int nNE_Aspect = -1;
	int nSW_Aspect = -1;
	int nSteep = -1;
	int nSlopeLt20 = -1;

	
	int nPlanarD = -1;
	int nPlanarA = -1;


	for(nLoop=0;nLoop<no_models;nLoop++)
	{
		if(outattr[nLoop].CompareNoCase("planar_d") ==0)
		{
			nPlanarD = nLoop;
		}
		if(outattr[nLoop].CompareNoCase("planar_a") ==0)
		{
			nPlanarA = nLoop;
		}

	}
	ASSERT(nPlanarD !=-1);
	ASSERT(nPlanarA !=-1);

	
		


	/*
	//Set the location of each value
	for(nLoop=0;nLoop<900;nLoop++)
	{
		if(fuzz_attr[nLoop].CompareNoCase("steep_sw")==0)
		{
			fuzz_attrIndex[nLoop] = no_models;
		}
		if(fuzz_attr[nLoop].CompareNoCase("steep_ne")==0)
		{
			fuzz_attrIndex[nLoop] = no_models +1;
		}
		if(fuzz_attr[nLoop].CompareNoCase("gentle_sw")==0)
		{
			fuzz_attrIndex[nLoop] = no_models +2;
		}
		if(fuzz_attr[nLoop].CompareNoCase("gentle_ne")==0)
		{
			fuzz_attrIndex[nLoop] = no_models +3;
		}
	}

	*/

	*Max_Code=0;
	*Max_Value = 0;
	*Max_Class = "";
	double classval = 0;
	double attrib_val = 0;
	int i=0;
	for(nLoop=0;nLoop<*no_facets;nLoop++)
	{
		classval = 0;
		for(i=0;i<no_attrib[nLoop];i++)
		{
			attrib_val = outattrval[fuzz_attrIndex[nLoop*30 + i]];
			classval = classval  + attrib_val * attr_wt[nLoop*30 + i];
		}
		if(classval >= *Max_Value)
		{
			*Max_Value = classval;
			*Max_Class = Facet_Name[nLoop];
			*Max_Code = Facet_Code[nLoop];
		}
		
	}

}
