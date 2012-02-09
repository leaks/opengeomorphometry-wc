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
// FormProcess.cpp: implementation of the FormProcess class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "form.h"
#include "FormProcess.h"
#include <math.h>

#include "sort.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFormProcess::CFormProcess()
{
	missing_value = -9999.0;
	max_vol = 10.0;
	max_area = 10.0;
	max_depth = 0.15;
	max_runoff = 300.0;

}

CFormProcess::~CFormProcess()
{

}

bool CFormProcess::PROCESS1(CPtrArray *pArray)
{
	
	




	pProgressBar2->SetRange(0,11);
	bool bReturnValue = false;
	
	FormFile.AddField("SEQNO",'I',9,0);
	FormFile.AddField("SLOPE",'D',8,3);
	FormFile.AddField("ASPECT",'S',3,0);
	FormFile.AddField("PROF",'D',8,3);
	FormFile.AddField("PLAN",'D',8,3);
	FormFile.AddField("QAREA",'D',17,2);
	FormFile.AddField("QWETI",'D',7,3);
	//FormFile.AddField("Seq",'I',9);


	CString* sMessage = new CString;
	*sMessage = "Opening file " + sDEMFileName;
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
	if(DEMFile.OpenFile(sDEMFileName,pProgressBar1,false) == false) 
	{
		AfxMessageBox("Could not open file " + sDEMFileName);
		exit(1);
	}
	if(*bKillThread == true)
	{
		AfxEndThread(1);
	}
	pProgressBar2->StepIt();
	int nColumn = DEMFile.FindField("SEQNO");
	ASSERT(nColumn!=-1);
	int* pDEMSeqNo = (int*)DEMFile.ReturnPointer(nColumn);




	int nSize = DEMFile.GetNumRecords();
	FormFile.SetInitialFileSize(nSize);

	nColumn = DEMFile.FindField("SEQNO");
	ASSERT(nColumn!=-1);
	int* pFormSeqNo = (int*)FormFile.ReturnPointer(nColumn);


	nColumn = FormFile.FindField("Slope");
	ASSERT(nColumn!=-1);
	double* pSlope = (double*)FormFile.ReturnPointer(nColumn);

	nColumn = FormFile.FindField("Aspect");
	ASSERT(nColumn!=-1);
	short int* pAspect = (short int*)FormFile.ReturnPointer(nColumn);

	nColumn = FormFile.FindField("Prof");
	ASSERT(nColumn!=-1);
	double* pProf = (double*)FormFile.ReturnPointer(nColumn);

	nColumn = FormFile.FindField("Plan");
	ASSERT(nColumn!=-1);
	double* pPlan= (double*)FormFile.ReturnPointer(nColumn);

	nColumn = FormFile.FindField("QArea");
	ASSERT(nColumn!=-1);
	double* pQArea= (double*)FormFile.ReturnPointer(nColumn);

	nColumn = FormFile.FindField("QWETI");
	ASSERT(nColumn!=-1);
	double* pQweti= (double*)FormFile.ReturnPointer(nColumn);















	for(int nLoop=0;nLoop<nSize;nLoop++)
	{
		FormFile.AddRecord();
		pFormSeqNo[nLoop] = pDEMSeqNo[nLoop];
		pSlope[nLoop] = 0;
		pAspect[nLoop] = 0;
		pProf[nLoop] =0;
		pPlan[nLoop] = 0;
		pQArea[nLoop] = 0;
		pQweti[nLoop] = 0;
	}

	if(*bKillThread == true)
	{
		AfxEndThread(1);
	}
	*sMessage = "Running CALC_FORM function..";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
	CALC_FORM();
	pProgressBar2->StepIt();
	if(*bKillThread == true)
	{
		AfxEndThread(1);
	}
	*sMessage = "Running CALC_WETI function..";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
	CALC_WETI(1);
	pProgressBar2->StepIt();
	if(*bKillThread == true)
	{
		AfxEndThread(1);
	}


	RelzFile.AddField("Seqno",'I',9,0);

	RelzFile.AddField("Z2ST",'D',8,3);
	RelzFile.AddField("Z2CR",'D',8,3);
	RelzFile.AddField("Z2PIT",'D',8,3);
	RelzFile.AddField("Z2PEAK",'D',8,3);
	RelzFile.AddField("Z2TOP",'D',8,3);
	RelzFile.AddField("ZCR2ST",'D',8,3);
	RelzFile.AddField("ZPIT2PEAK",'D',8,3);
	RelzFile.AddField("ZTOP2PIT",'D',8,3);


	RelzFile.AddField("PCTZ2ST",'S',3,0);
	RelzFile.AddField("PCTZ2PIT",'S',3,0);
	RelzFile.AddField("PCTZ2TOP",'S',3,0);
	RelzFile.AddField("PMIN2MAX",'S',3,0);

	


	RelzFile.SetInitialFileSize(nSize);
	//initializing variables. 
	for(int nLoop=0;nLoop<nSize;nLoop++)
	{
		RelzFile.AddRecord();
		RelzFile.SetValue(0,nLoop,(int)0);

		RelzFile.SetValue(1,nLoop,(double)0);
		RelzFile.SetValue(2,nLoop,(double)0.0);
		RelzFile.SetValue(3,nLoop,(double)0);
		RelzFile.SetValue(4,nLoop,(double)0);
		RelzFile.SetValue(5,nLoop,(double)0);
		RelzFile.SetValue(6,nLoop,(double)0.0);
		RelzFile.SetValue(7,nLoop,(double)0);
		RelzFile.SetValue(8,nLoop,(double)0);


		RelzFile.SetValue(9,nLoop,(short int)0);
		RelzFile.SetValue(10,nLoop,(short int)0);		
		RelzFile.SetValue(11,nLoop,(short int)0);
		RelzFile.SetValue(12,nLoop,(short int)0);
	
	}





	if(*bKillThread == true)
	{
		AfxEndThread(1);
	}	
	*sMessage = "Saving file " + strDirectory + str3CharName + "Form.dbf";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
	FormFile.SaveToFile(strDirectory + str3CharName + "Form.dbf",pProgressBar1, bKillThread);
	FormFile.Empty();
	pProgressBar2->StepIt();
	if(*bKillThread == true)
	{
		AfxEndThread(1);
	}
	CALC_RELZ1();





	





	*sMessage = "Saving file " + strDirectory + str3CharName + "Relz.dbf";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
	RelzFile.SaveToFile(strDirectory + str3CharName + "Relz.dbf",pProgressBar1, bKillThread);
	RelzFile.Empty();
	pProgressBar2->StepIt();
	//AfxMessageBox("Done");

	*sMessage = "Process complite";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,1,(long) sMessage);



	return bReturnValue;

}

void CFormProcess::SetVariables(CPtrArray * pPointerArray)
{

					
	//Index 1
	sDEMFileName = *(CString*) pPointerArray->GetAt(1);

	//Index 2
	dGridSize = *(int*)pPointerArray->GetAt(2);

	//Index 3
	dUpslopeArea = *(double*) pPointerArray->GetAt(3);
	
	//Index 4
	dDSArea = *(double*) pPointerArray->GetAt(4);

	//Index 5
	pProgressBar1 = (CProgressCtrl*) pPointerArray->GetAt(5);

	//index 6
	pProgressBar2 = (CProgressCtrl*) pPointerArray->GetAt(6);


	//Index 7
	//pPointerArray->Add(&m_CURRENT_STATUS);

	//Index 8
	//pPointerArray->Add(&m_OVERALL_STATUS);

	//Index 9 - window handle.
	phWnd = (HWND)pPointerArray->GetAt(9);

	//Index 10
	str3CharName = *(CString*)pPointerArray->GetAt(10);

	//Index 11
	strDirectory = *(CString*) pPointerArray->GetAt(11);


	//Index 12

	bKillThread = (bool*) pPointerArray->GetAt(12);
	pWndNotifyProgress = CWnd::FromHandle((HWND)pPointerArray->GetAt(9));
	

	//Index 13
	missing_value = *(double*)pPointerArray->GetAt(13);
}

void CFormProcess::CALC_FORM()
{
	int no_rows = 0;
	int no_cols = 0;

	int rowno = 1;


	int nColumn = DEMFile.FindField("ROW");
	ASSERT(nColumn!=-1);
	int* pRow = (int*)DEMFile.ReturnPointer(nColumn);
	
	nColumn = DEMFile.FindField("COL");
	ASSERT(nColumn!=-1);
	int* pCol = (int*)DEMFile.ReturnPointer(nColumn);


	nColumn = DEMFile.FindField("ELEV");
	ASSERT(nColumn!=-1);
	double* pElev= (double*) DEMFile.ReturnPointer(nColumn);

	nColumn = DEMFile.FindField("MISSING");
	ASSERT(nColumn!=-1);
	bool* pMissing= (bool*) DEMFile.ReturnPointer(nColumn);
	
	int nNumRec = DEMFile.GetNumRecords();

	no_rows = pRow[nNumRec-1];
	no_cols = pCol[nNumRec-1];


	double* row1z = new double[no_cols];
	double* row2z = new double[no_cols];
	double* row3z = new double[no_cols];
	
	int start1rec = 0;
	int start2rec = 0;
	int start3rec = 0;
	int start4rec = 0;

	int nLoop = 0 ;
	int nLoop2 = 0;
	int nLoop3 = 0;

	for(nLoop=0;nLoop<no_cols;nLoop++)
	{
		row1z[nLoop] = 0;
	}

	for(nLoop=0;nLoop<no_cols;nLoop++)
	{
		row2z[nLoop] = pElev[nLoop];
	}

	start3rec = nLoop;
	nLoop2 = nLoop + no_cols;

	for(nLoop;nLoop<nLoop2;nLoop++)
	{
		row3z[nLoop3] = pElev[nLoop];
		nLoop3++;
	}
	start4rec = nLoop;

	bool bExit = false;
	int this_row = 0;
	int this_col = 0;

	




	nColumn = FormFile.FindField("Slope");
	ASSERT(nColumn!=-1);
	double* pSlope = (double*)FormFile.ReturnPointer(nColumn);

	nColumn = FormFile.FindField("Aspect");
	ASSERT(nColumn!=-1);
	short int* pAspect = (short int*)FormFile.ReturnPointer(nColumn);

	nColumn = FormFile.FindField("Prof");
	ASSERT(nColumn!=-1);
	double* pProf = (double*)FormFile.ReturnPointer(nColumn);

	nColumn = FormFile.FindField("Plan");
	ASSERT(nColumn!=-1);
	double* pPlan= (double*)FormFile.ReturnPointer(nColumn);

	nColumn = FormFile.FindField("QArea");
	ASSERT(nColumn!=-1);
	double* pQArea= (double*)FormFile.ReturnPointer(nColumn);

	nColumn = FormFile.FindField("QWETI");
	ASSERT(nColumn!=-1);
	double* pQweti= (double*)FormFile.ReturnPointer(nColumn);

	
	



	for(nLoop=0;nLoop<nNumRec;nLoop++)
	{
		if(*bKillThread == true)
		{
			AfxEndThread(1);
		}
		this_row = pRow[nLoop];
		this_col = pCol[nLoop];
		if(pMissing[nLoop] == true)
		{
			pSlope[nLoop] = 0;
			pAspect[nLoop] = -1;
			pProf[nLoop] = 0 ;
			pPlan[nLoop] = 0;
			pQArea[nLoop] = 0;
			pQweti[nLoop] = 0;

		}
		else
		{
			if((this_col ==1) || (this_col == no_cols))
			{
				pSlope[nLoop] = 0;
				pAspect[nLoop] = 0;
				pProf[nLoop] = 0 ;
				pPlan[nLoop] = 0;				
			}
			else
			{
				if(this_row ==1 || this_row ==no_rows)
				{
					pSlope[nLoop] = 0;
					pAspect[nLoop] = 0;
					pProf[nLoop] = 0 ;
					pPlan[nLoop] = 0;	
				}
				else
				{
					eyton_form(row1z,row2z,row3z,this_col, pSlope, pAspect, pProf, pPlan, nLoop);
				}
				
			}
		}
		if(this_col == no_cols)
		{
			if(this_row == no_rows)
			{

			}
			else
			{
				rowno = rowno + 1;
				

				for(nLoop2=0;nLoop2<no_cols;nLoop2++)
				{
					row1z[nLoop2] = row2z[nLoop2];
					row2z[nLoop2] = row3z[nLoop2];
				}

				if(rowno < no_rows)
				{
					nLoop = start4rec;
					for(nLoop2 = 0;nLoop2<no_cols;nLoop2++)
					{
						row3z[nLoop2] = pElev[nLoop + nLoop2];
					}
				}
				start2rec = start3rec;
				start3rec = start4rec;
				start4rec = nLoop + nLoop2;


				nLoop = start2rec -1;
			}
		}
		else
		{
		}

		
		
	}
	
	delete row1z;
	delete row2z;
	delete row3z;
	pad_edges(pSlope, pAspect, pProf, pPlan, no_cols, no_rows);
	//FormFile.SaveToFile("c:\\formfile.dbf",pProgressBar1);
	//AfxMessageBox("Done");

}

void CFormProcess::eyton_form(double* row1z, double* row2z, double* row3z, int this_col,
							double* pSlope, short int* pAspect, double* pProf, double* pPlan, int nIndex)
{




	double	slope_pct = 0;
	double 	slope_deg = 0;
	double 	azimuth = 0;
	double 	this_prof = 0;
	double 	this_plan = 0;
	double local_angle = 0;







	double twod = dGridSize*2;
	double z11 = 0;
	double z12 = 0;
	double z13 = 0;

	bool one_missing = false;

	if(row1z[this_col - 2] !=missing_value)
	{
		z11 = row1z[this_col - 2]*100;
	}
	else
	{
		one_missing = true;
	}

	if(row1z[this_col-1] !=missing_value)
	{
		z12 = row1z[this_col-1]*100;
	}
	else
	{
		one_missing = true;
	}

	if(row1z[this_col] !=missing_value)
	{
		z13 = row1z[this_col]*100;
	}
	else
	{
		one_missing = true;
	}

	double z21 = 0;
	double z22 = 0;
	double z23 = 0;
	
	if(row2z[this_col - 2] !=missing_value)
	{
		z21 = row2z[this_col - 2]*100;
	}
	else
	{
		one_missing = true;
	}

	if(row2z[this_col-1] !=missing_value)
	{
		z22 = row2z[this_col-1]*100;
	}
	else
	{
		one_missing = true;
	}

	if(row2z[this_col] !=missing_value)
	{
		z23 = row2z[this_col]*100;
	}
	else
	{
		one_missing = true;
	}

	double z31 = 0;
	double z32 = 0;
	double z33 = 0;
	
	if(row3z[this_col - 2] !=missing_value)
	{
		z31 = row3z[this_col - 2]*100;
	}
	else
	{
		one_missing = true;
	}

	if(row3z[this_col-1] !=missing_value)
	{
		z32 = row3z[this_col-1]*100;
	}
	else
	{
		one_missing = true;
	}

	if(row3z[this_col] !=missing_value)
	{
		z33 = row3z[this_col]*100;
	}
	else
	{
		one_missing = true;
	}

	
	if(one_missing == false)
	{
		double slope_in_x = (z23 - z21)/twod;
		double slope_in_y = (z32 - z12)/twod;
		double this_slope = sqrt(slope_in_x * slope_in_x + slope_in_y*slope_in_y);
		slope_pct = this_slope;
		double slope_rad = atan(this_slope/100);
		double slope_deg = (slope_rad *180)/3.141592;
		if(this_slope>0) 
		{
			azimuth = 0;
			local_angle = (acos(fabs(slope_in_x)/this_slope) *180)/3.141592;
			if(slope_in_x > 0 && slope_in_y >0)
			{
				azimuth = 270 + local_angle;
			}
			else
			{
				if(slope_in_x >0 && slope_in_y <0) 
				{
					azimuth = 270 - local_angle;
				}
				else
				{
					if(slope_in_x <0 && slope_in_y >0)
					{
						azimuth = 90 - local_angle;
					}
					else
					{
						if(slope_in_x <0 && slope_in_y <0)
						{
							azimuth = 90 + local_angle;
						}
						else
						{
							if(slope_in_y == 0 && slope_in_x <0)
							{
								azimuth = 90;
							}
							else
							{
								if(slope_in_y == 0 && slope_in_x >0)
								{
									azimuth = 270;
								}
								else
								{
									if(slope_in_x == 0 && slope_in_y <0) 
									{
										azimuth = 180;
									}
									else
									{
										if(slope_in_x == 0 && slope_in_y >0) 
										{
											azimuth = 360;
										}
										else
										{
											azimuth = 361;
										}
									}
								}
							}
						}
					}
				}
			}
			
			double prof_azimuth = azimuth;
			if(prof_azimuth >180)
			{
				prof_azimuth = prof_azimuth - 180;
			}

			double plan_azimuth = prof_azimuth + 90;
			if(plan_azimuth  > 180) 
			{
				plan_azimuth = plan_azimuth - 180;
			}

			double prof_xa = 2.0 + sin((prof_azimuth *3.141592)/180);
			double prof_ya = 2.0 - cos((prof_azimuth *3.141592)/180);
			double prof_xb = 2.0 - sin((prof_azimuth *3.141592)/180);
			double prof_yb = 2.0 + cos((prof_azimuth *3.141592)/180);

			double plan_xa = 2.0 + sin((plan_azimuth *3.141592)/180);
			double plan_ya = 2.0 - cos((plan_azimuth *3.141592)/180);
			double plan_xb = 2.0 - sin((plan_azimuth *3.141592)/180);
			double plan_yb = 2.0 + cos((plan_azimuth *3.141592)/180);

			double prof_za = 0;
			double prof_zb = 0;
			double plan_za = 0;
			double plan_zb = 0;
			if(prof_azimuth <=90)
			{
				prof_za = (2.0 - prof_ya)*((z13*(prof_xa-2.0))+(z12*(3.0 - prof_xa)));
				prof_za = prof_za + ((prof_ya - 1.0)*((z23*(prof_xa - 2.0))+(z22*(3.0 - prof_xa))));

				prof_zb = (3.0 - prof_yb)*((z22*(prof_xb-1.0))+(z21*(2.0 - prof_xb)));
				prof_zb = prof_zb + ((prof_yb - 2.0)*((z32*(prof_xb - 1.0))+(z31*(2.0 - prof_xb))));
			}
			else
			{
				prof_za = (3.0 - prof_ya)*((z23*(prof_xa-2.0))+(z22*(3.0 - prof_xa)));
				prof_za = prof_za + ((prof_ya - 2.0)*((z33*(prof_xa - 2.0))+(z32*(3.0 - prof_xa))));

				prof_zb = (2.0 - prof_yb)*((z12*(prof_xb-1.0))+(z11*(2.0 - prof_xb)));
				prof_zb = prof_zb + ((prof_yb - 1.0)*((z22*(prof_xb - 1.0))+(z21*(2.0 - prof_xb))));
			}

			
			if(plan_azimuth <=90)
			{
				plan_za = (2.0 - plan_ya)*((z13*(plan_xa-2.0))+(z12*(3.0 - plan_xa)));
			    plan_za = plan_za + ((plan_ya - 1.0)*((z23*(plan_xa - 2.0))+(z22*(3.0 - plan_xa))));

				plan_zb = (3.0 - plan_yb)*((z22*(plan_xb-1.0))+(z21*(2.0 - plan_xb)));
				plan_zb = plan_zb + ((plan_yb - 2.0)*((z32*(plan_xb - 1.0))+(z31*(2.0 - plan_xb))));			
			}

			else
			{
				plan_za = (3.0 - plan_ya)*((z23*(plan_xa-2.0))+(z22*(3.0 - plan_xa)));
				plan_za = plan_za + ((plan_ya - 2.0)*((z33*(plan_xa - 2.0))+(z32*(3.0 - plan_xa))));

				plan_zb = (2.0 - plan_yb)*((z12*(plan_xb-1.0))+(z11*(2.0 - plan_xb)));
				plan_zb = plan_zb + ((plan_yb - 1.0)*((z22*(plan_xb - 1.0))+(z21*(2.0 - plan_xb))));
			}
			
			this_prof = ((atan((((2*z22) - prof_za - prof_zb)/(dGridSize*dGridSize))))*180)/3.141592;
			this_plan = ((atan((((2*z22) - plan_za - plan_zb)/(dGridSize*dGridSize))))*180)/3.141592;			
			

	

		}
		else
		{
			this_slope = 0;
			slope_pct = 0;
			slope_deg = 0;
			local_angle = 360;
			azimuth = 360;
			this_plan = 0;
			this_prof = 0;
		}
	}
	else
	{
		slope_pct = 0;
		slope_deg = 0;
		azimuth = 360;
		this_prof = 0;
		this_plan = 0;
	}

	pSlope[nIndex] = slope_pct;
	pAspect[nIndex] = (short int) (azimuth + 0.5);
	pProf[nIndex] = this_prof;
	pPlan[nIndex] = this_plan;

}

void CFormProcess::pad_edges(double *pSlope, short *pAspect, double *pProf, double *pPlan, int no_cols, int no_rows)
{
	int cells = 0;
	int first_col = no_cols;
	int next_col = first_col + 1;
	int nLoop = 0;
	
	double next_slope = 0;
	short int next_aspect = 0;
	double next_prof = 0;
	double next_plan = 0;


	for(nLoop = 0;nLoop<no_rows -2;nLoop++)
	{
		if(*bKillThread == true)
		{
			AfxEndThread(1);
		}
		next_slope = pSlope[next_col];
		next_aspect = pAspect[next_col];
		next_prof = pProf[next_col];
		next_plan = pPlan[next_col];

		pSlope[first_col] = next_slope;
		pAspect[first_col] = next_aspect;
		pProf[first_col] = next_prof;
		pPlan[first_col] = next_plan;
		
		first_col = first_col + no_cols;
		next_col = first_col + 1;
	}




	int last_col = no_cols * 2 -1;
	int prev_col = last_col - 1;

	for(nLoop = 0;nLoop<no_rows -2;nLoop++)
	{
		if(*bKillThread == true)
		{
			AfxEndThread(1);
		}
		next_slope = pSlope[prev_col];
		next_aspect = pAspect[prev_col];
		next_prof = pProf[prev_col];
		next_plan = pPlan[prev_col];

		pSlope[last_col] = next_slope;
		pAspect[last_col] = next_aspect;
		pProf[last_col] = next_prof;
		pPlan[last_col] = next_plan;
		last_col = last_col + no_cols;
		prev_col = last_col - 1	;	
	}

	int top_col = -1;
	int col_below = top_col + no_cols;



	for(nLoop = 0;nLoop<no_cols;nLoop++)
	{
		if(*bKillThread == true)
		{
			AfxEndThread(1);
		}
		top_col = top_col + 1;
		col_below = top_col + no_cols;
		next_slope = pSlope[col_below];
		next_aspect = pAspect[col_below];
		next_prof = pProf[col_below];
		next_plan = pPlan[col_below];

		pSlope[top_col] = next_slope;
		pAspect[top_col] = next_aspect;
		pProf[top_col] = next_prof;
		pPlan[top_col] = next_plan;
	}


	int col_above = (no_rows * no_cols) - (2*no_cols) -1;
	int bot_col = col_above + no_cols;

	for(nLoop = 0;nLoop<no_cols;nLoop++)
	{
		if(*bKillThread == true)
		{
			AfxEndThread(1);
		}
		col_above = col_above + 1;
		bot_col = col_above + no_cols;

		next_slope = pSlope[col_above];
		next_aspect = pAspect[col_above];
		next_prof = pProf[col_above];
		next_plan = pPlan[col_above];

		pSlope[bot_col] = next_slope;
		pAspect[bot_col] = next_aspect;
		pProf[bot_col] = next_prof;
		pPlan[bot_col] = next_plan;
	}
}

void CFormProcess::CALC_WETI(int nNum)
{
	double l1 = dGridSize * 0.5;
	double l2 = dGridSize * 0.354;
	double l_sqr = dGridSize* dGridSize;

	double orthogonal = dGridSize;
	double diagonal = dGridSize * sqrt(2.0);

	int nColumn = 0;

	nColumn = DEMFile.FindField("ROW");
	ASSERT(nColumn!=-1);
	int* pRow = (int*) DEMFile.ReturnPointer(nColumn);


	nColumn = DEMFile.FindField("Col");
	ASSERT(nColumn!=-1);
	int* pCol = (int*) DEMFile.ReturnPointer(nColumn);


	nColumn = DEMFile.FindField("seqno");
	ASSERT(nColumn!=-1);
	int* pSeqNo = (int*) DEMFile.ReturnPointer(nColumn);


	nColumn = FormFile.FindField("QArea");
	ASSERT(nColumn!=-1);
	double* pQArea= (double*)FormFile.ReturnPointer(nColumn);

	nColumn = FormFile.FindField("QWETI");
	ASSERT(nColumn!=-1);
	double* pQweti= (double*)FormFile.ReturnPointer(nColumn);
	

	int nNumRec = DEMFile.GetNumRecords();

	int maxrows = pRow[nNumRec - 1];
	int maxcols = pCol[nNumRec -1];

	

	int lastrec = nNumRec;
	int a_seqno = pSeqNo[nNumRec -1];
	int nLoop = 0;

	if(nNum == 1)
	{
		for(nLoop = 0;nLoop<nNumRec;nLoop++)
		{
			pQArea[nLoop] = 1;
		}
	}
	else
	{
		for(nLoop=0;nLoop<nNumRec;nLoop++)
		{
			pQArea[nLoop] = l_sqr;
		}
	}
	
	nColumn = DEMFile.FindField("Upslope");
	ASSERT(nColumn!=-1);
    int* pUpslope = (int*)DEMFile.ReturnPointer(nColumn);
	

	nColumn = DEMFile.FindField("Elev");
	ASSERT(nColumn!=-1);
	double* pElev = (double*)DEMFile.ReturnPointer(nColumn);

	double* pSortValue = new double[nNumRec];
	int* pSortedIndex = new int [nNumRec];


	//CDbfFile  dbf;
	//dbf.OpenFile("c:\\copy of m1b\\m1bform.dbf",pProgressBar1,false);

	//nColumn = dbf.FindField("QArea");
	//double* pQAreaA = (double*)dbf.ReturnPointer(nColumn);

	//nColumn = dbf.FindField("SeqNo");


	for(nLoop=0;nLoop<nNumRec;nLoop++)
	{
		pSortValue[nLoop]  = ((1000000 - pElev[nLoop]) * 10000000000) + pUpslope[nLoop];
		//pSortValue[nLoop] = pQAreaA[nLoop];
		pSortedIndex[nLoop] = nLoop;
	}
	

	QuickSort(pSortValue, nNumRec,pSortedIndex);
	
	/*
	nColumn = FormFile.FindField("Seq");

	int* pSeq = (int*)FormFile.ReturnPointer(nColumn);

	for(nLoop=0;nLoop<nNumRec;nLoop++)
	{
		pSeq[pSortedIndex[nLoop]] = nLoop + 1;

	}
	 
	*/



	


	delete pSortValue;


	int cent_x = 0;
	int cent_y = 0;
	double cent_z = 0;
	double cent_qa = 0;
	int centrec = 0;


	centrec = pSortedIndex[0];
	cent_z = pElev[pSortedIndex[0]];

	int numneigh = 0;
	int* zrec = new int[9];
	double* deltax = new double[9];
	double* l = new double [9];

	nColumn = DEMFile.FindField("ddir");
	ASSERT(nColumn!=-1);
	short int* pDDir = (short int*)DEMFile.ReturnPointer(nColumn);
	

	nColumn = DEMFile.FindField("drec");
	ASSERT(nColumn !=-1);
	int* pDrec = (int*) DEMFile.ReturnPointer(nColumn);

	
	int nIndex = 0;

	double qc = 0;
	for(nLoop=0;nLoop<nNumRec;nLoop++)
	{
		if(*bKillThread == true)
		{
			AfxEndThread(1);
		}
		nIndex = pSortedIndex[nLoop];
		
		cent_y = pRow[nIndex];
		cent_x = pCol[nIndex];
		cent_z = pElev[nIndex];
		centrec = nIndex;


		cent_qa = pQArea[nIndex];
		if(cent_z != missing_value)
		{
			qfindneig(cent_x, cent_y,  cent_z, cent_qa, numneigh, zrec, l,deltax,  orthogonal, diagonal,
				maxcols, maxrows, l1, l2);
			calc_quinn(numneigh, cent_z, cent_qa, zrec, deltax, pElev,pDDir, pDrec, centrec, l, pQArea, pQweti);
		}
		else
		{
			qc = cent_qa/0.0001;
			pQweti[nIndex] = log(1 + qc);
		}

	}


	if(nNum>1)
	{
		nColumn = FormFile.FindField("Aspect");
		ASSERT(nColumn!=-1);
		short int* pAspect = (short int*)FormFile.ReturnPointer(nColumn);
		nColumn = FormFile.FindField("LNQArea");
		ASSERT(nColumn!=-1);
		double* pLnqArea = (double*)FormFile.ReturnPointer(nColumn);

		nColumn = FormFile.FindField("New_Asp");
		ASSERT(nColumn!=-1);
		short int* pNewAsp = (short int*)FormFile.ReturnPointer(nColumn);


		for(nLoop=0;nLoop<nNumRec;nLoop++)
		{
			if(pAspect[nLoop] >-1)
			{
				pLnqArea[nLoop] = log(pQArea[nLoop]);
				pNewAsp[nLoop] = pAspect[nLoop] + 45;

				if(pNewAsp[nLoop] >360)
				{
					pNewAsp[nLoop] = pNewAsp[nLoop] - 360;
				}
			}

			else
			{
				pAspect[nLoop] = 0;
				pLnqArea[nLoop] = 0;
				pNewAsp[nLoop] = 0;
			}

		}
	}
	
	delete pSortedIndex;
	delete l;
	delete deltax;
	delete zrec;

}

void CFormProcess::qfindneig(int cent_x, int cent_y, double cent_z, double cent_qa, int &numneigh, int *zrec,
							 double *l, double *deltax, double orthogonal, double diagonal, int maxcols, int maxrows,
							 double l1	, double l2)
{
	numneigh = 0;
	if(cent_x > 1 && cent_y > 1)
	{
		numneigh = numneigh + 1;
		zrec[numneigh-1]  = (((cent_y - 2) * maxcols) + (cent_x - 1));

		l[numneigh -1] = l2;
		deltax[numneigh-1] = diagonal;
	}

	if(cent_y >1)
	{
		numneigh = numneigh+1;
		zrec[numneigh-1] = (((cent_y - 2) * maxcols) + cent_x);
		l[numneigh-1] = l1;
		deltax[numneigh-1] = orthogonal;
	}

	if (cent_y > 1 && cent_x < maxcols)
	{
		numneigh = numneigh+1;
		zrec[numneigh-1] = (((cent_y - 2) * maxcols) + (cent_x + 1));
		l[numneigh-1] = l2;
		deltax[numneigh-1] = diagonal;
	}
	if(cent_x > 1)
	{
		numneigh = numneigh+1;
		zrec[numneigh-1] = (((cent_y - 1) * maxcols) + (cent_x - 1));
		l[numneigh-1] = l1;
		deltax[numneigh-1] = orthogonal;
	}

	if(cent_x < maxcols)
	{
		numneigh = numneigh+1;
		zrec[numneigh-1] = (((cent_y - 1) * maxcols) + (cent_x + 1));
		l[numneigh-1] = l1;
		deltax[numneigh-1] = orthogonal;
	}


	if(cent_x > 1 && cent_y < maxrows)
	{
		numneigh = numneigh+1;
		zrec[numneigh-1] = ( (cent_y * maxcols) + (cent_x - 1) );
		l[numneigh-1] = l2;
		deltax[numneigh-1] = diagonal;
	}


	if(cent_y < maxrows)
	{
		numneigh = numneigh+1;
		zrec[numneigh-1] = ( (cent_y * maxcols) + cent_x );
		l[numneigh-1] = l1;
		deltax[numneigh-1] = orthogonal;
	}

	if(cent_x < maxcols && cent_y < maxrows)
	{
		numneigh = numneigh+1;
		zrec[numneigh-1] =  ( (cent_y * maxcols) + (cent_x + 1) );
		l[numneigh-1] = l2;
		deltax[numneigh-1] = diagonal;
	}



}



void CFormProcess::calc_quinn(int numneigh, double cent_z, double cent_qa, int *zrec, double *deltax, 
							  double *pElev, short int *pDDir, int *pDrec, int centrec, double* l , 
							  double* pQArea, double* pQweti)
{
	int j = 0;
	int numlower = 0;
	int nLoop = 0;
	int nIndex = 0;
	double ql[9];
	double sumtanbl = 0;
	
	double qc = 0;

	int down[9];

	double tanb[9];
	for(nLoop=0;nLoop<numneigh;nLoop++)
	{
		nIndex = zrec[nLoop] ;

		if(pElev[nIndex-1] <cent_z && pElev[nIndex-1] !=missing_value)
		{
			j= j + 1;
			tanb[j-1] = (cent_z - pElev[nIndex-1])/deltax[nLoop];
			down[j-1] = zrec[nLoop];
			ql[j-1] = l[nLoop];
			sumtanbl = sumtanbl+ (tanb[j-1]*ql[j-1]);
			numlower = numlower + 1;
		}

	}


	double new_qa[9];
	if(numlower> 0)
	{
		qc = cent_qa /sumtanbl;
		for(j=0;j<numlower;j++)
		{
			new_qa[j] = qc * (tanb[j]*ql[j]);
			nIndex = down[j];
			pQArea[nIndex-1] = pQArea[nIndex-1] +new_qa[j];


		}
	}
	else
	{
		nIndex = centrec;
		qc = cent_qa / 0.0001;
		if(pDDir[nIndex] !=5)
		{
			nIndex = pDrec[nIndex];
			pQArea[nIndex-1] = pQArea[nIndex-1] +cent_qa;
		}
	}


	nIndex = centrec;
	if(qc>1)
	{
		pQweti[nIndex] = log(qc);
	}
	else
	{
		pQweti[nIndex] = log(1 + qc);
	}

}

void CFormProcess::CALC_RELZ1()
{
	int tempz2cr  = 0;
	double d = dGridSize;
	int numup = 0;
	int numdn = 0;
	int j = 0;


	CString* sMessage = new CString;
	*sMessage = "Running go_stream1 function..";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
	go_stream1(numup, numdn );
	pProgressBar2->StepIt();


	*sMessage = "Running go_pit1 function..";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
	go_pit1();
	pProgressBar2->StepIt();


	*sMessage = "Running go_ridge1 function..";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
	go_ridge1();
	pProgressBar2->StepIt();


	*sMessage = "Running up2peak1 function..";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
	up2peak1();
	pProgressBar2->StepIt();


	*sMessage = "Running calcrelief1 function..";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,1,(long) sMessage);
	calcrelief1();
	pProgressBar2->StepIt();
}

void CFormProcess::go_stream1(int &numup, int &numdn)
{
	double str_elev = 0;
	double extra_z = 0;
	double current_n = 0;

	int nNumRec = DEMFile.GetNumRecords();

	int nLoop = 0;
	
	int nColumn = 0;
	nColumn = DEMFile.FindField("SeqNo");
	ASSERT(nColumn !=-1);
	int* pDEMSeqNo = (int*)DEMFile.ReturnPointer(nColumn);

	
	nColumn = DEMFile.FindField("ShedNow");
	ASSERT(nColumn !=-1);
	int* pDEMShednow = (int*)DEMFile.ReturnPointer(nColumn);

	
	nColumn = DEMFile.FindField("Elev");
	ASSERT(nColumn !=-1);
	double* pElev = (double*) DEMFile.ReturnPointer(nColumn);


	nColumn = DEMFile.FindField("Upslope");
	ASSERT(nColumn !=-1);
	int* pUpslope = (int*) DEMFile.ReturnPointer(nColumn);




	nColumn = DEMFile.FindField("DDir");
	ASSERT(nColumn !=-1);
	short int* pDDir = (short int*) DEMFile.ReturnPointer(nColumn);
	

	nColumn = DEMFile.FindField("Drec");
	ASSERT(nColumn !=-1);
	int* pDrec = (int*) DEMFile.ReturnPointer(nColumn);







	
	nColumn = RelzFile.FindField("SeqNo");
	ASSERT(nColumn !=-1);
	int* pRelzSeqNo = (int*) RelzFile.ReturnPointer(nColumn);



	nColumn = RelzFile.FindField("zpit2peak");
	ASSERT(nColumn !=-1);
	double* pzpit2peak = (double*) RelzFile.ReturnPointer(nColumn);

	nColumn = RelzFile.FindField("z2pit");
	ASSERT(nColumn !=-1);
	double* pz2pit = (double*) RelzFile.ReturnPointer(nColumn);


	nColumn = RelzFile.FindField("z2peak");
	ASSERT(nColumn !=-1);
	double* pz2peak = (double*) RelzFile.ReturnPointer(nColumn);


	nColumn = RelzFile.FindField("z2st");
	ASSERT(nColumn !=-1);
	double* pz2st = (double*) RelzFile.ReturnPointer(nColumn);

	nColumn = RelzFile.FindField("z2cr");
	ASSERT(nColumn !=-1);
	double* pz2cr = (double*) RelzFile.ReturnPointer(nColumn);


	nColumn = RelzFile.FindField("z2top");
	ASSERT(nColumn !=-1);
	double* pz2top = (double*) RelzFile.ReturnPointer(nColumn);


	nColumn = RelzFile.FindField("zcr2st");
	ASSERT(nColumn !=-1);
	double* pzcr2st = (double*) RelzFile.ReturnPointer(nColumn);


	nColumn = RelzFile.FindField("ztop2pit");
	ASSERT(nColumn !=-1);
	double* pztop2pit = (double*) RelzFile.ReturnPointer(nColumn);






	//Initialize file object.


	for(nLoop=0;nLoop<nNumRec;nLoop++)
	{
		//RelzFile.AddRecord();
		pRelzSeqNo[nLoop] = pDEMSeqNo[nLoop];
		pz2st[nLoop] = 0;
		pz2cr[nLoop] = 0;
		pz2peak[nLoop] = 0;
		pztop2pit[nLoop] = 0;
		pzpit2peak[nLoop] = 0;
		pzcr2st[nLoop] = 0;
		pz2top[nLoop]  = 0;

	}

	double *pSortValue = new double[nNumRec];
	int *pSortedIndex = new int [nNumRec];

	int nCounter = 0;
	for(nLoop=0;nLoop<nNumRec;nLoop++)
	{
		if(pElev[nLoop] !=missing_value) 
		{
			pSortValue[nCounter] = pDEMShednow[nLoop] * 10000000000000000 + ((100000 - pElev[nLoop]) * 100000000000) + pUpslope[nLoop];
			pSortedIndex[nCounter] = nLoop;
			nCounter++;
		}
	}
	QuickSort(pSortValue,nCounter,pSortedIndex);

	delete pSortValue;

	int nIndex = 0;
	int start_rec = 0;
	
	bool bgo_dn =0;
	double extraz = 0;
	for(nLoop=0;nLoop<nCounter;nLoop++)
	{
		if(*bKillThread == true)
		{
			AfxEndThread(1);
		}
		nIndex = pSortedIndex[nLoop];

		if(pElev[nIndex] != missing_value)
		{
			start_rec = nIndex;
			numdn = 0;
			if(pz2st[nIndex]>0 || pUpslope[nIndex] >= dDSArea)
			{
				if(pUpslope[nIndex] >= dDSArea)
				{
					pz2st[nIndex] = 0;

				}
				bgo_dn = false;
			}
			else
			{
				bgo_dn = true;
			}
			extraz = 0;
			while(bgo_dn == true)
			{
				if(*bKillThread == true)
				{
					AfxEndThread(1);
				}
				if(pz2st[nIndex] == 0 && pUpslope[nIndex] < dDSArea)
				{
					if(pDDir[nIndex] !=5)
					{
						numdn = numdn + 1;
						nIndex = pDrec[nIndex] -1;
					}
					else
					{
						bgo_dn = false;
						extraz = 0;
						str_elev = pElev[nIndex];
						pz2st[nIndex] = 0;

					}

				}
				else
				{
					if(pUpslope[nIndex] >=dDSArea)
					{
						extraz = 0;
						str_elev = pElev[nIndex];
						pz2st[nIndex] = 0;
					}
					else
					{
						extraz = pz2st[nIndex];
						str_elev = pElev[nIndex] - extraz;
						if(str_elev > pElev[nIndex])
						{
							str_elev = pElev[nIndex];
						}
					}
					bgo_dn = false;
				}
			}

			if(numdn > 0) 
			{
				nIndex = start_rec;
				for(int j=0;j<numdn;j++)
				{
					if(pElev[nIndex] > str_elev)
					{
						pz2st[nIndex] = pElev[nIndex] - str_elev;

					}
					else
					{
						pz2st[nIndex] = 0;
					}
					nIndex = pDrec[nIndex] -1;
				}
			}
		}

	}
	delete pSortedIndex;
}

void CFormProcess::go_pit1()
{
	int nColumn=0;

	CDbfFile PondFile;
	if(PondFile.OpenFile(strDirectory +   str3CharName + "pond.dbf", pProgressBar1,bKillThread) ==false)
	{
		AfxMessageBox("Could not open file : " + strDirectory  + str3CharName + "pond.dbf");
		AfxEndThread(0);
	}
	




	nColumn = PondFile.FindField("pitrow");
	ASSERT(nColumn!=-1);
	short int* pPitRow = (short int*) PondFile.ReturnPointer(nColumn);



	nColumn = PondFile.FindField("pitcol");
	ASSERT(nColumn!=-1);
	short int* pPitCol = (short int*) PondFile.ReturnPointer(nColumn);


	nColumn = PondFile.FindField("pitelev");
	ASSERT(nColumn!=-1);
	double* pPitElev = (double*) PondFile.ReturnPointer(nColumn);


	nColumn = PondFile.FindField("Shedno");
	ASSERT(nColumn!=-1);
	int* pPondShedNo = (int*) PondFile.ReturnPointer(nColumn);

	


	nColumn = DEMFile.FindField("elev");
	ASSERT(nColumn!=-1);
	double* pElev = (double*) DEMFile.ReturnPointer(nColumn);


	nColumn = RelzFile.FindField("z2pit");
	ASSERT(nColumn!=-1);
	double* pz2pit = (double*) RelzFile.ReturnPointer(nColumn);

	nColumn = RelzFile.FindField("z2st");
	ASSERT(nColumn !=-1);
	double* pz2st = (double*) RelzFile.ReturnPointer(nColumn);



	nColumn = RelzFile.FindField("z2cr");
	ASSERT(nColumn !=-1);
	double* pz2cr = (double*) RelzFile.ReturnPointer(nColumn);



	nColumn = DEMFile.FindField("Shedno");
	ASSERT(nColumn!=-1);
	int* pShedNo = (int*) DEMFile.ReturnPointer(nColumn);

	
	int nNumRec  = DEMFile.GetNumRecords();

	int nLoop = 0;
	
	//Had to do my own sorting, because the QSort is very slow if a lot of keys are the same.
	int* pSortValue = new int[nNumRec];
	int* pSortTemp = new int[nNumRec];
	int* pSortedIndex = new int[nNumRec];

	int nSmallest = 99999;

	for(nLoop=0;nLoop<nNumRec;nLoop++)
	{
		pSortValue[nLoop] = pShedNo[nLoop];
		//pSortedIndex[nLoop] = nLoop;
		if(nSmallest > pShedNo[nLoop])
		{
			nSmallest = pShedNo[nLoop];
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
	int nIndex=0;
	
	int nStart = 0;
	for(nLoop=0;nLoop<nNumRec;nLoop++)
	{
		nIndex = pSortedIndex[nLoop];
		if(pShedNo[nIndex] >0)
		{
			nStart = nLoop;
			nLoop = nNumRec;
		}
	}
	
	int this_shed = 0;
	int nLoop2 = 0;

	int nPitRow = 0;
	int nPitCol = 0;
	double dPitElev = 0;
	bExit = false;

	int nPondFileSize = PondFile.GetNumRecords();
	for(nLoop=nStart;nLoop<nNumRec;nLoop++)
	{
		if(*bKillThread == true)
		{
			AfxEndThread(1);
		}
		nIndex = pSortedIndex[nLoop];
		this_shed = pShedNo[nIndex];
		
		for(nLoop2=0;nLoop2<nPondFileSize;nLoop2++)
		{
			if(pPondShedNo[nLoop2] == this_shed)
			{
				nPitRow = pPitRow[nLoop2];
				nPitCol = pPitCol[nLoop2];
				dPitElev = pPitElev[nLoop2];
			}
		}
		bExit = false;
		while(bExit == false)
		{
			if(*bKillThread == true)
			{
				AfxEndThread(1);
			}
			if(pElev[nIndex] !=missing_value)
			{
				if(pElev[nIndex] > dPitElev)
				{
					pz2pit[nIndex] = pElev[nIndex] - dPitElev;
				}
				else
				{
					pz2pit[nIndex] = 0;
				}
			}
			nLoop = nLoop + 1;
			if(nLoop == nNumRec)
			{
				bExit = true;
			}
			else
			{
				nIndex = pSortedIndex[nLoop];
				if(pShedNo[nIndex] != this_shed)
				{
					bExit = true;
					nLoop --;

				}
			}

		}


	}
	
	PondFile.Empty();
	delete pSortedIndex;

}

void CFormProcess::go_ridge1()
{
	int nColumn;

	CString* sMessage = new CString;
	*sMessage = "Opening file " + strDirectory + str3CharName + "idem.dbf";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);

	if(IDemFile.OpenFile(strDirectory + str3CharName + "idem.dbf",pProgressBar1,bKillThread) ==false)
	{
		AfxMessageBox("Could not open file " + strDirectory + str3CharName + "idem.dbf");
		AfxEndThread(0);
	}
	pProgressBar2->StepIt();


	*sMessage = "Running go_ridge1 function..";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,1,(long) sMessage);
	nColumn = IDemFile.FindField("DDir");
	ASSERT(nColumn !=-1);
	short int* pDDir = (short int*)IDemFile.ReturnPointer(nColumn);


	nColumn = IDemFile.FindField("Drec");
	ASSERT(nColumn!=-1);
	int* pDrec = (int*) IDemFile.ReturnPointer(nColumn);

	


	nColumn = IDemFile.FindField("ShedNow");
	ASSERT(nColumn !=-1);
	int* pShedNow = (int*) IDemFile.ReturnPointer(nColumn);
	


	nColumn = IDemFile.FindField("Upslope");
	ASSERT(nColumn !=-1);
	int* pUpslope = (int*) IDemFile.ReturnPointer(nColumn);

	nColumn = IDemFile.FindField("Elev");
	ASSERT(nColumn !=-1);
	double* pElev = (double*) IDemFile.ReturnPointer(nColumn);

	
	nColumn = RelzFile.FindField("z2cr");
	ASSERT(nColumn !=-1);
	double* pz2cr = (double*)RelzFile.ReturnPointer(nColumn);














	
	int nNumRec = IDemFile.GetNumRecords();


	//unsigned _int64 * pSortValue = new unsigned _int64[nNumRec];
	//int* pSortedIndex = new int[nNumRec];
	int nLoop=0;



	//for(nLoop=0;nLoop<nNumRec;nLoop++)
	//{
	//	pSortValue[nLoop] = pShedNow[nLoop] * 1000000000000000 + ((100000 - pElev[nLoop]) * 100000000000) + pUpslope[nLoop];
	//	pSortedIndex[nLoop] = nLoop;
	//}

	//QuickSort(pSortValue,nNumRec,pSortedIndex);

	//delete pSortValue;
	double cr_elev = 0;

	bool go_dn = false;
	
	int num_dn = 0;

	int nIndex = 0;
	int start_rec = 0;
	double extra_z =0;
	for(nLoop=0;nLoop<nNumRec;nLoop++)
	{
		if(*bKillThread == true)
		{
			AfxEndThread(1);
		}
		//nIndex = pSortedIndex[nLoop];	
		nIndex = nLoop;
		if(pElev[nIndex] != missing_value)
		{
			start_rec = nIndex;
			num_dn = 0;
			if(pz2cr[nIndex]>0 || pUpslope[nIndex] >= dDSArea)
			{
				if(pUpslope[nIndex] >=dDSArea && pz2cr[nIndex] ==0)
				{
					cr_elev = pElev[nIndex];
					pz2cr[nIndex] = 0;
				}
				go_dn = false;
			}
			else
			{
				go_dn = true;
			}

			extra_z = 0;
			while(go_dn == true)
			{
				if(pz2cr[nIndex] ==0 && pUpslope[nIndex] < dDSArea)
				{
					if(pDDir[nIndex] !=5)
					{
						num_dn = num_dn + 1;
						nIndex = pDrec[nIndex] -1;	 
					}
					else
					{
						go_dn = false;
						extra_z = 0;
						cr_elev = pElev[nIndex];
						pz2cr[nIndex] = 0;
					}
				}
				else
				{
					if(pUpslope[nIndex] >= dDSArea)
					{
						extra_z =  0;
						cr_elev = pElev[nIndex];
						pz2cr[nIndex] = 0;
					}
					else
					{
						extra_z = pz2cr[nIndex];
						cr_elev = pElev[nIndex] - extra_z;
						if(cr_elev > pElev[nIndex])
						{
							cr_elev = pElev[nIndex];
						}
					}
					go_dn = false;
				}

			}


			if(num_dn > 0)
			{
				nIndex = start_rec;
				for(int j=0;j<num_dn;j++)
				{
					if(pElev[nIndex] > cr_elev)
					{
						pz2cr[nIndex] = pElev[nIndex]  - cr_elev;
					}
					else
					{
						pz2cr[nIndex] = 0;
					}
					nIndex = pDrec[nIndex] -1;
				}
			}


			nIndex  = start_rec;



		}


	}
	//delete pSortedIndex;
	
}

void CFormProcess::up2peak1()
{
	int nColumn = 0;

	nColumn = RelzFile.FindField("z2peak");
	ASSERT(nColumn!=-1);
	double* pz2peak = (double*) RelzFile.ReturnPointer(nColumn);

	nColumn = IDemFile.FindField("DDir");
	ASSERT(nColumn !=-1);
	short int* pDDir = (short int*)IDemFile.ReturnPointer(nColumn);


	nColumn = IDemFile.FindField("Drec");
	ASSERT(nColumn!=-1);
	int* pDrec = (int*) IDemFile.ReturnPointer(nColumn);

	


	nColumn = IDemFile.FindField("ShedNow");
	ASSERT(nColumn !=-1);
	int* pShedNow = (int*) IDemFile.ReturnPointer(nColumn);
	


	nColumn = IDemFile.FindField("Upslope");
	ASSERT(nColumn !=-1);
	int* pUpslope = (int*) IDemFile.ReturnPointer(nColumn);

	nColumn = IDemFile.FindField("Elev");
	ASSERT(nColumn !=-1);
	double* pElev = (double*) IDemFile.ReturnPointer(nColumn);

	int nNumRec = IDemFile.GetNumRecords();


	long double * pSortValue = new long double[nNumRec];
	int* pSortedIndex = new int[nNumRec];
	int nLoop=0;




	int nCounter = 0;
	for(nLoop=0;nLoop<nNumRec;nLoop++)
	{
		if(pElev[nLoop] !=missing_value) 
		{
			pSortValue[nCounter] = pShedNow[nLoop] * 10000000000000000 + ((100000 - pElev[nLoop]) * 100000000000) + pUpslope[nLoop];
			pSortedIndex[nCounter] = nLoop;
			nCounter++;
		}
	}
	QuickSort(pSortValue,nCounter,pSortedIndex);

	delete pSortValue;
	
	int start_rec = 0;
	int num_dn = 0;
	bool go_dn = false;
	double extra_z = 0;
	
	double pk_elev = 0;
	int nIndex=0;
	for(nLoop=0;nLoop<nCounter;nLoop++)
	{
		if(*bKillThread == true)
		{
			AfxEndThread(1);
		}

		nIndex = pSortedIndex[nLoop];
		if(pElev[nIndex]!=missing_value)
		{
			start_rec = nIndex;
			num_dn = 0;
			if(pz2peak[nIndex] > 0 || pDDir[nIndex] == 5)
			{
				if(pDDir[nIndex]==5)
				{
					pk_elev = pElev[nIndex];
					pz2peak[nIndex] = 0;
				}
				go_dn = false;
			}
			else
			{
				go_dn = true;
			}
			extra_z = 0;


			while(go_dn)
			{
				if(pz2peak[nIndex] == 0)
				{
					if(pDDir[nIndex] !=5)
					{
						num_dn++;
						nIndex = pDrec[nIndex] -1;
					}
					else
					{
						go_dn = false;
						extra_z = 0;
						pk_elev = pElev[nIndex];
						pz2peak[nIndex] = 0;
					}
				}
				else
				{
					extra_z = pz2peak[nIndex];
					pk_elev = pElev[nIndex] - extra_z;
					go_dn = false;
				}
			}
			
			if(num_dn >0)
			{
				nIndex = start_rec;
				for(int j=0;j<num_dn;j++)
				{
					if(pElev[nIndex] > pk_elev)
					{
						pz2peak[nIndex] = pElev[nIndex] - pk_elev;
					}
					else
					{
						pz2peak[nIndex] = 0;
					}
					nIndex = pDrec[nIndex] -1;
				}
			}

		}
	}
	delete pSortedIndex;

}

void CFormProcess::calcrelief1()
{
	int nColumn = 0;

	double min_elev = 99999999;
	double max_elev =-1;

	nColumn = DEMFile.FindField("Elev");
	ASSERT(nColumn!=-1);
	double* pElev = (double*) DEMFile.ReturnPointer(nColumn);
	
	int nNumRec = DEMFile.GetNumRecords();


	int nLoop = 0;
	for(nLoop=0;nLoop<nNumRec;nLoop++)
	{
		if (pElev[nLoop] > max_elev)
		{
			max_elev = pElev[nLoop];
		}

		if(pElev[nLoop] < min_elev)
		{
			if(pElev[nLoop] != missing_value)
			{
				min_elev = pElev[nLoop];
			}
		}
	}


	double elev_range = max_elev - min_elev;

	nColumn = DEMFile.FindField("Shednow");
	ASSERT(nColumn!=-1);
	int* pShedNow = (int*) DEMFile.ReturnPointer(nColumn);


	nColumn = DEMFile.FindField("Upslope");
	ASSERT(nColumn!=-1);
	int* pUpslope = (int*) DEMFile.ReturnPointer(nColumn);


	long double * pSortValue = new long double[nNumRec];
	int* pSortedIndex = new int[nNumRec];





	int nCounter = 0;
	for(nLoop=0;nLoop<nNumRec;nLoop++)
	{
		if(pElev[nLoop] !=missing_value) 
		{
			pSortValue[nCounter] = pShedNow[nLoop] * 10000000000000000 + ((100000 - pElev[nLoop]) * 100000000000) + pUpslope[nLoop];
			pSortedIndex[nCounter] = nLoop;
			nCounter++;
		}
	}
	QuickSort(pSortValue,nCounter,pSortedIndex);



	delete pSortValue;


	int nIndex = 0;


	int nStart = 0;
	for(nLoop=0;nLoop<nCounter;nLoop++)
	{
		if(pShedNow[pSortedIndex[nLoop]]>0)
		{
			nStart = nLoop;
			nLoop = nNumRec;
		}		
	}
	

	nColumn = RelzFile.FindField("zpit2peak");
	ASSERT(nColumn !=-1);
	double* pzpit2peak = (double*) RelzFile.ReturnPointer(nColumn);

	nColumn = RelzFile.FindField("z2pit");
	ASSERT(nColumn !=-1);
	double* pz2pit = (double*) RelzFile.ReturnPointer(nColumn);


	nColumn = RelzFile.FindField("z2peak");
	ASSERT(nColumn !=-1);
	double* pz2peak = (double*) RelzFile.ReturnPointer(nColumn);


	nColumn = RelzFile.FindField("z2st");
	ASSERT(nColumn !=-1);
	double* pz2st = (double*) RelzFile.ReturnPointer(nColumn);

	nColumn = RelzFile.FindField("z2cr");
	ASSERT(nColumn !=-1);
	double* pz2cr = (double*) RelzFile.ReturnPointer(nColumn);


	nColumn = RelzFile.FindField("z2top");
	ASSERT(nColumn !=-1);
	double* pz2top = (double*) RelzFile.ReturnPointer(nColumn);


	nColumn = RelzFile.FindField("zcr2st");
	ASSERT(nColumn !=-1);
	double* pzcr2st = (double*) RelzFile.ReturnPointer(nColumn);


	nColumn = RelzFile.FindField("ztop2pit");
	ASSERT(nColumn !=-1);
	double* pztop2pit = (double*) RelzFile.ReturnPointer(nColumn);



	nColumn = RelzFile.FindField("pctz2top");
	ASSERT(nColumn !=-1);
	short int* ppctz2top = (short int*) RelzFile.ReturnPointer(nColumn);

	nColumn = RelzFile.FindField("pctz2st");
	ASSERT(nColumn !=-1);
	short int* ppctz2st = (short int*) RelzFile.ReturnPointer(nColumn);

	nColumn = RelzFile.FindField("pctz2pit");
	ASSERT(nColumn !=-1);
	short int* ppctz2pit = (short int*) RelzFile.ReturnPointer(nColumn);


	nColumn = RelzFile.FindField("pmin2max");
	ASSERT(nColumn !=-1);
	short int* ppmin2max = (short int*) RelzFile.ReturnPointer(nColumn);









	int this_shed = 0;
	double top_elev = 0;
	bool go_dn = false;
	for(nLoop=nStart;nLoop<nCounter;nLoop++)
	{
		if(*bKillThread == true)
		{
			AfxEndThread(1);
		}


		nIndex = pSortedIndex[nLoop];

		this_shed = pShedNow[nIndex];
		top_elev = pElev[nIndex];
		

		if(pElev[nIndex] == missing_value)
		{
			//nLoop = nNumRec;
		}
		else
		{

			go_dn = true;
			while(go_dn == true)
			{
				if(*bKillThread == true)
				{
					AfxEndThread(1);
				}
				pzpit2peak[nIndex] = pz2pit[nIndex] + pz2peak[nIndex];
				pzcr2st[nIndex] = pz2cr[nIndex] + pz2st[nIndex];
				pz2top[nIndex] = top_elev - pElev[nIndex];
				pztop2pit[nIndex] = pz2top[nIndex] + pz2pit[nIndex];
				
				if(pztop2pit[nIndex] > 0)
				{
					ppctz2top[nIndex] = (short int) ( 100 -  (pz2top[nIndex] /pztop2pit[nIndex])*100);
				}
				else
				{
					ppctz2top[nIndex] = 0;
				}


				if(pzcr2st[nIndex] >0)
				{
					ppctz2st[nIndex] =  (short int) ((pz2st[nIndex]/pzcr2st[nIndex])*100);
				}
				else
				{
					ppctz2st[nIndex] = 0;
				}

				

				if(pzpit2peak[nIndex]>0)
				{
					ppctz2pit[nIndex] =  (short int) ( ((pz2pit[nIndex] / pzpit2peak[nIndex])*100));
				}
				else
				{
					ppctz2pit[nIndex] = 0;
				}
				

				ppmin2max[nIndex] =  (short int) ( ((pElev[nIndex] - min_elev) / elev_range)*100);
				///REPLACE b.pmin2max WITH INT(((elev - min_elev)/ elev_range)*100)











				nLoop++;

				if(nLoop<nNumRec)
				{
					nIndex = pSortedIndex[nLoop];

					if(pShedNow[nIndex] != this_shed)
					{
						go_dn = false;
						nLoop--;
					}
				}
				else
				{
					go_dn = false;
				}
			}
			

		}


	}


	delete pSortedIndex;

}

void CFormProcess::CALC_RELZ2()
{	
	CString* sMessage = new CString;
	*sMessage = "Running go_stream2 function.. ";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,1,(long) sMessage);
	go_stream2();
	pProgressBar2->StepIt();
}

void CFormProcess::go_stream2()
{
	int numdn = 0;

	double str_elev = 0;
	double extra_z = 0;
	int current_n = 0;

	int nNumRec = DEMFile.GetNumRecords();

	int nLoop = 0;
	
	int nColumn = 0;
	nColumn = DEMFile.FindField("SeqNo");
	ASSERT(nColumn !=-1);
	int* pDEMSeqNo = (int*)DEMFile.ReturnPointer(nColumn);

	
	nColumn = DEMFile.FindField("ShedNow");
	ASSERT(nColumn !=-1);
	int* pDEMShednow = (int*)DEMFile.ReturnPointer(nColumn);

	
	nColumn = DEMFile.FindField("Elev");
	ASSERT(nColumn !=-1);
	double* pElev = (double*) DEMFile.ReturnPointer(nColumn);


	nColumn = DEMFile.FindField("Upslope");
	ASSERT(nColumn !=-1);
	int* pUpslope = (int*) DEMFile.ReturnPointer(nColumn);




	nColumn = DEMFile.FindField("DDir");
	ASSERT(nColumn !=-1);
	short int* pDDir = (short int*) DEMFile.ReturnPointer(nColumn);
	

	nColumn = DEMFile.FindField("Drec");
	ASSERT(nColumn !=-1);
	int* pDrec = (int*) DEMFile.ReturnPointer(nColumn);







	
	nColumn = RelzFile.FindField("SeqNo");
	ASSERT(nColumn !=-1);
	int* pRelzSeqNo = (int*) RelzFile.ReturnPointer(nColumn);



	nColumn = RelzFile.FindField("z2st");
	ASSERT(nColumn !=-1);
	double* pz2st = (double*) RelzFile.ReturnPointer(nColumn);



	nColumn = RelzFile.FindField("n2st");
	ASSERT(nColumn !=-1);
	short int* pn2st = (short int*) RelzFile.ReturnPointer(nColumn);






	




	for(nLoop=0;nLoop<nNumRec;nLoop++)
	{
		//RelzFile.AddRecord();
		pRelzSeqNo[nLoop] = pDEMSeqNo[nLoop];
		pz2st[nLoop] = 0;
		pn2st[nLoop] = 0;
	}

	double *pSortValue = new double[nNumRec];
	int *pSortedIndex = new int [nNumRec];


	int nCounter = 0;
	for(nLoop=0;nLoop<nNumRec;nLoop++)
	{
		if(pElev[nLoop] !=missing_value) 
		{
			pSortValue[nCounter] = pDEMShednow[nLoop] * 10000000000000000 + ((100000 - pElev[nLoop]) * 100000000000) + pUpslope[nLoop];
			pSortedIndex[nCounter] = nLoop;
			nCounter++;
		}
	}
	QuickSort(pSortValue,nCounter,pSortedIndex);

	delete pSortValue;

	int nIndex = 0;
	int start_rec = 0;
	//int current_n = 0;
	bool bgo_dn =0;
	double extraz = 0;
	int extran = 0;
	for(nLoop=0;nLoop<nCounter;nLoop++)
	{
		if(*bKillThread == true)
		{
			AfxEndThread(1);
		}
		nIndex = pSortedIndex[nLoop];

		if(pElev[nIndex] != missing_value)
		{
			start_rec = nIndex;
			numdn = 0;
			if(pz2st[nIndex]>0 || pUpslope[nIndex] >= dDSArea)
			{
				if(pUpslope[nIndex] >= dDSArea)
				{
					pz2st[nIndex] = 0;
					pn2st[nIndex] = 0;

				}
				bgo_dn = false;
			}
			else
			{
				bgo_dn = true;
			}
			extraz = 0;
			extran = 0;
			while(bgo_dn == true)
			{
				if(*bKillThread == true)
				{
					AfxEndThread(1);
				}
				if(pz2st[nIndex] == 0 && pUpslope[nIndex] < dDSArea)
				{
					if(pDDir[nIndex] !=5)
					{
						numdn = numdn + 1;
						nIndex = pDrec[nIndex] -1;
					}
					else
					{
						bgo_dn = false;
						extraz = 0;
						extran = 0;
						str_elev = pElev[nIndex];
						pz2st[nIndex] = 0;
						pn2st[nIndex] = 0;

					}

				}
				else
				{
					if(pUpslope[nIndex] >=dDSArea)
					{
						extraz = 0;
						extran = 0;
						str_elev = pElev[nIndex];
						pz2st[nIndex] = 0;
						pn2st[nIndex] = 0;
					}
					else
					{
						extraz = pz2st[nIndex];
						extran = pn2st[nIndex];
						str_elev = pElev[nIndex] - extraz;
						if(str_elev > pElev[nIndex])
						{
							str_elev = pElev[nIndex];
						}
					}
					bgo_dn = false;
				}
			}

			if(numdn > 0) 
			{
				current_n = numdn + extran;

				nIndex = start_rec;
				for(int j=0;j<numdn;j++)
				{
					if(pElev[nIndex] > str_elev)
					{
						pz2st[nIndex] = pElev[nIndex] - str_elev;
					}
					else
					{
						pz2st[nIndex] = 0;
					}
					pn2st[nIndex] = current_n;
					nIndex = pDrec[nIndex] -1;
					current_n = current_n - 1;
				}
			}
		}

	}
	delete pSortedIndex;
}

void CFormProcess::PROCESS2(CPtrArray *pArray)
{
	pProgressBar2->SetRange(0,6);

	bool bReturnValue = false;
	
	FormFile.AddField("SEQNO",'I',9,0);
	FormFile.AddField("SLOPE",'D',8,3);
	FormFile.AddField("ASPECT",'S',3,0);
	FormFile.AddField("PROF",'D',8,3);
	FormFile.AddField("PLAN",'D',8,3);
	FormFile.AddField("QAREA",'D',17,2);
	FormFile.AddField("QWETI",'D',7,3);
	FormFile.AddField("LNQAREA",'D',7,3);
	FormFile.AddField("NEW_ASP",'S',3,0);


	//FormFile.AddField("Seq",'I',9);

	CString* sMessage = new CString;
	*sMessage = "Opening file " + sDEMFileName;
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);

	if(DEMFile.OpenFile(sDEMFileName,pProgressBar1,false) == false) 
	{
		AfxMessageBox("Could not open file : " + sDEMFileName);

		exit(1);
	}
	pProgressBar2->StepIt();

	int nColumn = DEMFile.FindField("SEQNO");
	ASSERT(nColumn!=-1);
	int* pDEMSeqNo = (int*)DEMFile.ReturnPointer(nColumn);




	int nSize = DEMFile.GetNumRecords();
	FormFile.SetInitialFileSize(nSize);

	nColumn = DEMFile.FindField("SEQNO");
	ASSERT(nColumn!=-1);
	int* pFormSeqNo = (int*)FormFile.ReturnPointer(nColumn);


	nColumn = FormFile.FindField("Slope");
	ASSERT(nColumn!=-1);
	double* pSlope = (double*)FormFile.ReturnPointer(nColumn);

	nColumn = FormFile.FindField("Aspect");
	ASSERT(nColumn!=-1);
	short int* pAspect = (short int*)FormFile.ReturnPointer(nColumn);

	nColumn = FormFile.FindField("Prof");
	ASSERT(nColumn!=-1);
	double* pProf = (double*)FormFile.ReturnPointer(nColumn);

	nColumn = FormFile.FindField("Plan");
	ASSERT(nColumn!=-1);
	double* pPlan= (double*)FormFile.ReturnPointer(nColumn);

	nColumn = FormFile.FindField("QArea");
	ASSERT(nColumn!=-1);
	double* pQArea= (double*)FormFile.ReturnPointer(nColumn);

	nColumn = FormFile.FindField("QWETI");
	ASSERT(nColumn!=-1);
	double* pQweti= (double*)FormFile.ReturnPointer(nColumn);















	for(int nLoop=0;nLoop<nSize;nLoop++)
	{
		FormFile.AddRecord();
		pFormSeqNo[nLoop] = pDEMSeqNo[nLoop];
		pSlope[nLoop] = 0;
		pAspect[nLoop] = 0;
		pProf[nLoop] =0;
		pPlan[nLoop] = 0;
		pQArea[nLoop] = 0;
		pQweti[nLoop] = 0;
	}


	*sMessage = "Running CALC_FORM function.. ";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
	CALC_FORM();
	pProgressBar2->StepIt();
	if(*bKillThread == true)
	{
		AfxEndThread(1);
	}	

	*sMessage = "Running CALC_WETI function.. " ;
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
	CALC_WETI(2);
	pProgressBar2->StepIt();
	if(*bKillThread == true)
	{
		AfxEndThread(1);
	}

	RelzFile.AddField("Seqno",'I',9,0);
	RelzFile.AddField("Z2ST",'D',8,3);
	RelzFile.AddField("N2ST",'S',4,0);

	if(*bKillThread == true)
	{
		AfxEndThread(1);
	}	


	RelzFile.SetInitialFileSize(nSize);

	
	//initializing variables. 
	for(int nLoop=0;nLoop<nSize;nLoop++)
	{
		RelzFile.AddRecord();
		RelzFile.SetValue(0,nLoop,(int)0);
		RelzFile.SetValue(1,nLoop,(double)0);
		RelzFile.SetValue(2,nLoop,(short int)0);
		RelzFile.SetValue(3,nLoop,(double)0);
		RelzFile.SetValue(4,nLoop,(double)0);
		RelzFile.SetValue(5,nLoop,(double)0.0);
		RelzFile.SetValue(6,nLoop,(double)0);
		RelzFile.SetValue(7,nLoop,(double)0.0);
		RelzFile.SetValue(8,nLoop,(int)0);


	}

	
	*sMessage = "Saving file " + strDirectory + str3CharName + "Form.dbf";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
	FormFile.SaveToFile(strDirectory + str3CharName + "Form.dbf",pProgressBar1, bKillThread);
	FormFile.Empty();
	pProgressBar2->StepIt();
	if(*bKillThread == true)
	{
		AfxEndThread(1);
	}	

	CALC_RELZ2();

	if(*bKillThread == true)
	{
		AfxEndThread(1);
	}
	*sMessage = "Saving file " + strDirectory + str3CharName + "Relz.dbf";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);

	RelzFile.SaveToFile(strDirectory + str3CharName + "Relz.dbf",pProgressBar1, bKillThread);
	RelzFile.Empty();
	pProgressBar2->StepIt();
	*sMessage = "Process complite";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,1,(long) sMessage);
	//AfxMessageBox("Done");
}



void CFormProcess::PROCESS3(CPtrArray *pArray)
{

	pProgressBar2->SetRange(0,12);



	bool bReturnValue = false;
	
	FormFile.AddField("SEQNO",'I',9,0);
	FormFile.AddField("SLOPE",'D',8,3);
	FormFile.AddField("ASPECT",'S',3,0);
	FormFile.AddField("PROF",'D',8,3);
	FormFile.AddField("PLAN",'D',8,3);
	FormFile.AddField("QAREA",'D',17,2);
	FormFile.AddField("QWETI",'D',7,3);
	FormFile.AddField("LNQAREA",'D',7,3);
	FormFile.AddField("NEW_ASP",'S',3,0);
	//FormFile.AddField("Seq",'I',9);
	
	CString* sMessage = new CString;
	*sMessage = "Opening file " + sDEMFileName;
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);

	if(DEMFile.OpenFile(sDEMFileName,pProgressBar1,bKillThread) == false) 
	{
		AfxMessageBox("Could not open file " + sDEMFileName);
		exit(1);
	}

	pProgressBar2->StepIt();
	
	int nColumn = DEMFile.FindField("SEQNO");
	ASSERT(nColumn!=-1);
	int* pDEMSeqNo = (int*)DEMFile.ReturnPointer(nColumn);




	int nSize = DEMFile.GetNumRecords();
	FormFile.SetInitialFileSize(nSize);

	nColumn = DEMFile.FindField("SEQNO");
	ASSERT(nColumn!=-1);
	int* pFormSeqNo = (int*)FormFile.ReturnPointer(nColumn);


	nColumn = FormFile.FindField("Slope");
	ASSERT(nColumn!=-1);
	double* pSlope = (double*)FormFile.ReturnPointer(nColumn);

	nColumn = FormFile.FindField("Aspect");
	ASSERT(nColumn!=-1);
	short int* pAspect = (short int*)FormFile.ReturnPointer(nColumn);

	nColumn = FormFile.FindField("Prof");
	ASSERT(nColumn!=-1);
	double* pProf = (double*)FormFile.ReturnPointer(nColumn);

	nColumn = FormFile.FindField("Plan");
	ASSERT(nColumn!=-1);
	double* pPlan= (double*)FormFile.ReturnPointer(nColumn);

	nColumn = FormFile.FindField("QArea");
	ASSERT(nColumn!=-1);
	double* pQArea= (double*)FormFile.ReturnPointer(nColumn);

	nColumn = FormFile.FindField("QWETI");
	ASSERT(nColumn!=-1);
	double* pQweti= (double*)FormFile.ReturnPointer(nColumn);















	for(int nLoop=0;nLoop<nSize;nLoop++)
	{
		FormFile.AddRecord();
		pFormSeqNo[nLoop] = pDEMSeqNo[nLoop];
		pSlope[nLoop] = 0;
		pAspect[nLoop] = 0;
		pProf[nLoop] =0;
		pPlan[nLoop] = 0;
		pQArea[nLoop] = 0;
		pQweti[nLoop] = 0;
	}



	CALC_FORM();
	*sMessage = "Running Calc_Form function.. ";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
	pProgressBar2->StepIt();
	*sMessage = "Running Calc_Weti function.. ";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
	CALC_WETI(3);
	pProgressBar2->StepIt();
	if(*bKillThread == true)
	{
		AfxEndThread(1);
	}

	RelzFile.AddField("Seqno",'I',9,0);
	RelzFile.AddField("ST_ROW",'S',4,0);
	RelzFile.AddField("ST_COL",'S',4,0);
	RelzFile.AddField("ST_ELEV",'D',10,5);
	RelzFile.AddField("Z2ST",'D',8,3);
	RelzFile.AddField("N2ST",'S',4,0);
	RelzFile.AddField("CR_ROW",'S',4,0);
	RelzFile.AddField("CR_COL",'S',4,0);
	RelzFile.AddField("CR_ELEV",'D',8,3);
	RelzFile.AddField("Z2CR",'D',8,3);
	RelzFile.AddField("N2CR",'S',4,0);
	RelzFile.AddField("PIT_ROW",'S',4,0);
	RelzFile.AddField("PIT_COL",'S',4,0);


	RelzFile.AddField("PIT_ELEV",'D',8,3);
	RelzFile.AddField("Z2PIT",'D',8,3);
	RelzFile.AddField("PK_ROW",'S',4,0);
	RelzFile.AddField("PK_COL",'S',4,0);
	RelzFile.AddField("PK_ELEV",'D',8,3);
	RelzFile.AddField("Z2PEAK",'D',8,3);
	RelzFile.AddField("N2PEAK",'S',4,0);
	RelzFile.AddField("Z2TOP",'D',8,3);



	RelzFile.AddField("ZPIT2PEAK",'D',8,3);
	RelzFile.AddField("ZCR2ST",'D',8,3);
	RelzFile.AddField("ZCR2PIT",'D',8,3);
	RelzFile.AddField("ZTOP2PIT",'D',8,3);
	RelzFile.AddField("NCR2ST",'D',8,3);
	RelzFile.AddField("PMIN2MAX",'S',3,0);
	RelzFile.AddField("PCTZ2ST",'S',3,0);
	RelzFile.AddField("PCTZ2PIT",'S',3,0);
	RelzFile.AddField("PCTZ2TOP",'S',3,0);
	RelzFile.AddField("PCTN2ST",'S',3,0);










	



	

	*sMessage = "Saving file : " + strDirectory + str3CharName + "Form.dbf";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
	FormFile.SaveToFile(strDirectory + str3CharName + "Form.dbf",pProgressBar1, bKillThread);
	FormFile.Empty();
	if(*bKillThread == true)
	{
		AfxEndThread(1);
	}
	RelzFile.SetInitialFileSize(nSize);
	
	


	//initializing variables. 
	for(int nLoop=0;nLoop<nSize;nLoop++)
	{
		RelzFile.AddRecord();
		RelzFile.SetValue(0,nLoop,(int)0);
		RelzFile.SetValue(1,nLoop,(short int)0);
		RelzFile.SetValue(2,nLoop,(short int)0);
		RelzFile.SetValue(3,nLoop,(double)0);
		RelzFile.SetValue(4,nLoop,(double)0.0);
		RelzFile.SetValue(5,nLoop,(short)0);
		RelzFile.SetValue(6,nLoop,(short) 0);
		RelzFile.SetValue(7,nLoop,(short)0);
		RelzFile.SetValue(8,nLoop,(double)0);
		RelzFile.SetValue(9,nLoop,(double)0);
		RelzFile.SetValue(10,nLoop,(short)0.0);
		RelzFile.SetValue(11,nLoop,(short)0);		
		RelzFile.SetValue(12,nLoop,(short)0);



		RelzFile.SetValue(13,nLoop,(double)0);
		RelzFile.SetValue(14,nLoop,(double)0);
		RelzFile.SetValue(15,nLoop,(short)0);
		RelzFile.SetValue(16,nLoop,(short)0.0);
		RelzFile.SetValue(17,nLoop,(double)0);
		RelzFile.SetValue(18,nLoop,(double)0);
		RelzFile.SetValue(19,nLoop,(short)0);
		RelzFile.SetValue(20,nLoop,(double)0);



		RelzFile.SetValue(21,nLoop,(double)0);
		RelzFile.SetValue(22,nLoop,(double)0.0);
		RelzFile.SetValue(23,nLoop,(double)0);
		RelzFile.SetValue(24,nLoop,(double)0);
		RelzFile.SetValue(25,nLoop,(double)0);
		RelzFile.SetValue(26,nLoop,(short)0);
		RelzFile.SetValue(27,nLoop,(short)0);
		RelzFile.SetValue(28,nLoop,(short)0);
		RelzFile.SetValue(29,nLoop,(short)0.0);
		RelzFile.SetValue(30,nLoop,(short)0);	
	}



















	*sMessage = "Running Calc_Relz2 function.. " ;
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
	CALC_RELZ3();
	pProgressBar2->StepIt();
	DEMFile.Empty();

	
	*sMessage = "Saving file : " + strDirectory + str3CharName + "Relz.dbf";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);

	
	/*
	CFile myFile;

	myFile.Open(strDirectory + str3CharName + "Relz.dbf",CFile::modeCreate|CFile::modeWrite );
	
	CArchive arc (&myFile, CArchive::store, 512 );

	int nNumRec = RelzFile.GetNumRecords();
	int nNumCol = RelzFile.GetNumFields();

	int nIndex2 = 0;
	int nIndex3 = 0;

	

	for(nIndex2=0;nIndex2<nNumRec;nIndex2++)
	{
		for(nIndex3=0;nIndex3<nNumCol;nIndex3++)
		{
			if(RelzFile.CType[nIndex3] == 'I')
			{
				arc<<(((int*) RelzFile.pColumnValue[nIndex3])[nIndex2]);
			}
			if(RelzFile.CType[nIndex3] == 'D')
			{
				arc<<(((double*) RelzFile.pColumnValue[nIndex3])[nIndex2]);
			}
			if(RelzFile.CType[nIndex3] == 'F')
			{
				arc<<(((float*) RelzFile.pColumnValue[nIndex3])[nIndex2]);
			}
			if(RelzFile.CType[nIndex3] == 'S')
			{
				arc<<(((short int*) RelzFile.pColumnValue[nIndex3])[nIndex2]);
			}
			if(RelzFile.CType[nIndex3] == 'L')
			{
				arc<<(((bool*) RelzFile.pColumnValue[nIndex3])[nIndex2]);
			}
			if(RelzFile.CType[nIndex3] == 'C')
			{
				arc<<(((CString*) RelzFile.pColumnValue[nIndex3])[nIndex2]);
			}
		}
	}


	*/




	RelzFile.SaveToFile(strDirectory + str3CharName + "Relz.dbf",pProgressBar1, bKillThread);
	RelzFile.Empty();
	pProgressBar2->StepIt();
	//AfxMessageBox("Done");

	*sMessage = "Process complite";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,1,(long) sMessage);
}

void CFormProcess::CALC_RELZ3()
{
	int* pSortedIndex = 0;
	int* pSortedIndex2 = 0;
	CString* sMessage = new CString;
	*sMessage = "Running go_stream3..";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
	pSortedIndex = go_stream3();

	if(*bKillThread == true)
	{
		AfxEndThread(1);
	}

	*sMessage = "Running go_pit3..";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
	go_pit3();

	if(*bKillThread == true)
	{
		AfxEndThread(1);
	}

	*sMessage = "Running go_ridge3..";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
	go_ridge3(pSortedIndex2);

	if(*bKillThread == true)
	{
		AfxEndThread(1);
	}

	*sMessage = "Running go_peak3..";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
	go_peak3(pSortedIndex2);

	if(*bKillThread == true)
	{
		AfxEndThread(1);
	}


	*sMessage = "Running calcrelief3..";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
	calcrelief3(pSortedIndex);

	if(*bKillThread == true)
	{
		AfxEndThread(1);
	}


	IDemFile.Empty();
	*sMessage = "Running calc_len3";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,1,(long) sMessage);
	calc_len3();
	delete pSortedIndex;
	delete pSortedIndex2;

}

int* CFormProcess::go_stream3()
{
	int numdn = 0;

	double extra_z = 0;
	int current_n = 0;

	int nNumRec = DEMFile.GetNumRecords();

	int nLoop = 0;
	
	int nColumn = 0;
	nColumn = DEMFile.FindField("SeqNo");
	ASSERT(nColumn !=-1);
	int* pDEMSeqNo = (int*)DEMFile.ReturnPointer(nColumn);

	
	nColumn = DEMFile.FindField("ShedNow");
	ASSERT(nColumn !=-1);
	int* pDEMShednow = (int*)DEMFile.ReturnPointer(nColumn);

	
	nColumn = DEMFile.FindField("Elev");
	ASSERT(nColumn !=-1);
	double* pElev = (double*) DEMFile.ReturnPointer(nColumn);


	nColumn = DEMFile.FindField("Upslope");
	ASSERT(nColumn !=-1);
	int* pUpslope = (int*) DEMFile.ReturnPointer(nColumn);

	
	nColumn = DEMFile.FindField("ROW");
	ASSERT(nColumn !=-1);
	int* pRow = (int*)  DEMFile.ReturnPointer(nColumn);


	nColumn = DEMFile.FindField("COL");
	ASSERT(nColumn !=-1);
	int* pCol = (int*) DEMFile.ReturnPointer(nColumn);

	nColumn = DEMFile.FindField("DDir");
	ASSERT(nColumn !=-1);
	short int* pDDir = (short int*) DEMFile.ReturnPointer(nColumn);
	

	nColumn = DEMFile.FindField("Drec");
	ASSERT(nColumn !=-1);
	int* pDrec = (int*) DEMFile.ReturnPointer(nColumn);







	
	nColumn = RelzFile.FindField("SeqNo");
	ASSERT(nColumn !=-1);
	int* pRelzSeqNo = (int*) RelzFile.ReturnPointer(nColumn);



	nColumn = RelzFile.FindField("zpit2peak");
	ASSERT(nColumn !=-1);
	double* pzpit2peak = (double*) RelzFile.ReturnPointer(nColumn);

	nColumn = RelzFile.FindField("z2pit");
	ASSERT(nColumn !=-1);
	double* pz2pit = (double*) RelzFile.ReturnPointer(nColumn);


	nColumn = RelzFile.FindField("z2peak");
	ASSERT(nColumn !=-1);
	double* pz2peak = (double*) RelzFile.ReturnPointer(nColumn);


	nColumn = RelzFile.FindField("z2st");
	ASSERT(nColumn !=-1);
	double* pz2st = (double*) RelzFile.ReturnPointer(nColumn);

	nColumn = RelzFile.FindField("z2cr");
	ASSERT(nColumn !=-1);
	double* pz2cr = (double*) RelzFile.ReturnPointer(nColumn);


	nColumn = RelzFile.FindField("z2top");
	ASSERT(nColumn !=-1);
	double* pz2top = (double*) RelzFile.ReturnPointer(nColumn);


	nColumn = RelzFile.FindField("zcr2st");
	ASSERT(nColumn !=-1);
	double* pzcr2st = (double*) RelzFile.ReturnPointer(nColumn);


	nColumn = RelzFile.FindField("ztop2pit");
	ASSERT(nColumn !=-1);
	double* pztop2pit = (double*) RelzFile.ReturnPointer(nColumn);



	nColumn = RelzFile.FindField("n2CR");
	ASSERT(nColumn!=-1);
	short int* pn2cr = (short int*) RelzFile.ReturnPointer(nColumn);


	nColumn = RelzFile.FindField("n2st");
	ASSERT(nColumn!=-1);
	short int* pn2st = (short int*) RelzFile.ReturnPointer(nColumn);



	nColumn = RelzFile.FindField("ST_ROW");
	ASSERT(nColumn !=-1);
	short int* pSt_Row = (short int*) RelzFile.ReturnPointer(nColumn);



	nColumn = RelzFile.FindField("ST_COL");
	ASSERT(nColumn !=-1);
	short int* pSt_Col = (short int*) RelzFile.ReturnPointer(nColumn);



	nColumn = RelzFile.FindField("ST_Elev");
	ASSERT(nColumn !=-1);
	double* pSt_Elev = (double*) RelzFile.ReturnPointer(nColumn);




	//Initialize file object.


	for(nLoop=0;nLoop<nNumRec;nLoop++)
	{
		//RelzFile.AddRecord();
		pRelzSeqNo[nLoop] = pDEMSeqNo[nLoop];
		pz2st[nLoop] = 0;
		pz2cr[nLoop] = 0;
		pz2peak[nLoop] = 0;
		pztop2pit[nLoop] = 0;
		pzpit2peak[nLoop] = 0;
		pzcr2st[nLoop] = 0;
		pz2top[nLoop]  = 0;
		pn2cr[nLoop] = 0;
		pn2st[nLoop] = 0;
		pz2pit[nLoop] = 0;

	}

	double *pSortValue = new double[nNumRec];
	int *pSortedIndex = new int [nNumRec];


	int nCounter = 0;
	for(nLoop=0;nLoop<nNumRec;nLoop++)
	{
		if(pElev[nLoop] !=missing_value) 
		{
			pSortValue[nCounter] = pDEMShednow[nLoop] * 10000000000000000 + ((100000 - pElev[nLoop]) * 100000000000) + pUpslope[nLoop];
			pSortedIndex[nCounter] = nLoop;
			nCounter++;
		}
	}
	QuickSort(pSortValue,nCounter,pSortedIndex);

	delete pSortValue;

	int nIndex = 0;
	int start_rec = 0;
	
	bool bgo_dn =0;
	double extraz = 0;
	int str_row =0 ;
	int str_col = 0;
	double str_elev = 0;
	int extran = 0;
	for(nLoop=0;nLoop<nCounter;nLoop++)
	{
		if(*bKillThread == true)
		{
			AfxEndThread(1);
		}
		nIndex = pSortedIndex[nLoop];

		if(pElev[nIndex] != missing_value)
		{
			start_rec = nIndex;
			numdn = 0;
			if(pz2st[nIndex]>0 || pUpslope[nIndex] >= dDSArea)
			{
				if(pUpslope[nIndex] >= dDSArea && pz2st[nIndex] == 0 )
				{
					str_row = pRow[nIndex];
					str_col = pCol[nIndex];
					str_elev = pElev[nIndex];

					pSt_Row[nIndex] = str_row;
					pSt_Col[nIndex] = str_col;
					pSt_Elev[nIndex] = str_elev;

					pz2st[nIndex] = 0;
					pn2st[nIndex]= 0;


				}
				bgo_dn = false;
			}
			else
			{
				bgo_dn = true;
			}
			//extraz = 0;
			extran = 0;
			while(bgo_dn == true)
			{
				if(*bKillThread == true)
				{
					AfxEndThread(1);
				}
				if(pz2st[nIndex] == 0 && pUpslope[nIndex] < dDSArea)
				{
					if(pDDir[nIndex] !=5)
					{
						numdn = numdn + 1;
						nIndex = pDrec[nIndex] -1;
					}
					else
					{
						bgo_dn = false;
						extran = 0;

						str_row = pRow[nIndex];
						str_col = pCol[nIndex];
						str_elev = pElev[nIndex];

						pSt_Row[nIndex] = str_row;
						pSt_Col[nIndex] = str_col;
						pSt_Elev[nIndex] = str_elev;

						pz2st[nIndex] = 0;
						pn2st[nIndex]= 0;



					}

				}
				else
				{
					if(pUpslope[nIndex] >=dDSArea)
					{
						extran = 0;
						str_row = pRow[nIndex];
						str_col = pCol[nIndex];
						str_elev = pElev[nIndex];

						pSt_Row[nIndex] = str_row;
						pSt_Col[nIndex] = str_col;
						pSt_Elev[nIndex] = str_elev;

						pz2st[nIndex] = 0;
						pn2st[nIndex]= 0;
					}
					else
					{
						extran = pn2st[nIndex];
						str_row = pSt_Row[nIndex];
						str_col = pSt_Col[nIndex];
						str_elev = pSt_Elev[nIndex];
					}
					bgo_dn = false;
				}
			}

			if(numdn > 0) 
			{
				current_n = numdn + extran;
				nIndex = start_rec;
				for(int j=0;j<numdn;j++)
				{
					if(pElev[nIndex] > str_elev)
					{
						pz2st[nIndex] = pElev[nIndex] - str_elev;
						pSt_Row[nIndex] = str_row;
						pSt_Col[nIndex] = str_col;
						pSt_Elev[nIndex] = str_elev;

					}
					else
					{
						pz2st[nIndex] = 0;
						pSt_Row[nIndex] = str_row;
						pSt_Col[nIndex] = str_col;
						pSt_Elev[nIndex] = str_elev;


					}
					pn2st[nIndex] = current_n;
					current_n--;
					nIndex = pDrec[nIndex] -1;
					if(nIndex == 0)
					{
						nIndex = 0;
					}
				}
			}
		}

	}
	pProgressBar2->StepIt();
	return pSortedIndex;
}

void CFormProcess::go_pit3()
{
	int nColumn=0;

	CDbfFile PondFile;
	if(PondFile.OpenFile(strDirectory +   str3CharName + "pond.dbf", pProgressBar1,bKillThread) ==false)
	{
		AfxMessageBox("Could not open file : " + strDirectory  + str3CharName + "pond.dbf");
		AfxEndThread(0);
	}
	




	nColumn = PondFile.FindField("pitrow");
	ASSERT(nColumn!=-1);
	int* pPitRow = (int*) PondFile.ReturnPointer(nColumn);



	nColumn = PondFile.FindField("pitcol");
	ASSERT(nColumn!=-1);
	int* pPitCol = (int*) PondFile.ReturnPointer(nColumn);


	nColumn = PondFile.FindField("pitelev");
	ASSERT(nColumn!=-1);
	double* pPitElev = (double*) PondFile.ReturnPointer(nColumn);


	nColumn = PondFile.FindField("Shedno");
	ASSERT(nColumn!=-1);
	int* pPondShedNo = (int*) PondFile.ReturnPointer(nColumn);

	


	nColumn = DEMFile.FindField("elev");
	ASSERT(nColumn!=-1);
	double* pElev = (double*) DEMFile.ReturnPointer(nColumn);


	nColumn = RelzFile.FindField("z2pit");
	ASSERT(nColumn!=-1);
	double* pz2pit = (double*) RelzFile.ReturnPointer(nColumn);

	nColumn = RelzFile.FindField("z2st");
	ASSERT(nColumn !=-1);
	double* pz2st = (double*) RelzFile.ReturnPointer(nColumn);



	nColumn = RelzFile.FindField("pit_row");
	ASSERT(nColumn!=-1);
	short int* pRelzFilepit_row = (short int*) RelzFile.ReturnPointer(nColumn);

	nColumn = RelzFile.FindField("pit_col");
	ASSERT(nColumn!=-1);
	short int* pRelzFilepit_col = (short int*) RelzFile.ReturnPointer(nColumn);

	nColumn = RelzFile.FindField("pit_elev");
	ASSERT(nColumn!=-1);
	double* pRelzFilepit_elev = (double*) RelzFile.ReturnPointer(nColumn);


	nColumn = RelzFile.FindField("z2cr");
	ASSERT(nColumn !=-1);
	double* pz2cr = (double*) RelzFile.ReturnPointer(nColumn);



	nColumn = DEMFile.FindField("Shedno");
	ASSERT(nColumn!=-1);
	int* pShedNo = (int*) DEMFile.ReturnPointer(nColumn);

	
	int nNumRec  = DEMFile.GetNumRecords();

	int nLoop = 0;
	
	
	int* pSortValue = new int[nNumRec];
	int* pSortTemp = new int[nNumRec];
	int* pSortedIndex = new int[nNumRec];

	int nSmallest = 99999;

	for(nLoop=0;nLoop<nNumRec;nLoop++)
	{
		pSortValue[nLoop] = pShedNo[nLoop];
		//pSortedIndex[nLoop] = nLoop;
		if(nSmallest > pShedNo[nLoop])
		{
			nSmallest = pShedNo[nLoop];
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
	int nIndex=0;
	
	int nStart = 0;
	for(nLoop=0;nLoop<nNumRec;nLoop++)
	{
		nIndex = pSortedIndex[nLoop];
		if(pShedNo[nIndex] >0)
		{
			nStart = nLoop;
			nLoop = nNumRec;
		}
	}
	
	int this_shed = 0;
	int nLoop2 = 0;

	int nPitRow = 0;
	int nPitCol = 0;
	double dPitElev = 0;
	bExit = false;

	int nPondFileSize = PondFile.GetNumRecords();
	for(nLoop=nStart;nLoop<nNumRec;nLoop++)
	{
		if(*bKillThread == true)
		{
			AfxEndThread(1);
		}
		nIndex = pSortedIndex[nLoop];
		this_shed = pShedNo[nIndex];
		
		for(nLoop2=0;nLoop2<nPondFileSize;nLoop2++)
		{
			if(pPondShedNo[nLoop2] == this_shed)
			{
				nPitRow = pPitRow[nLoop2];
				nPitCol = pPitCol[nLoop2];
				dPitElev = pPitElev[nLoop2];
				nLoop2= nPondFileSize;
			}
		}
		bExit = false;
		while(bExit == false)
		{
			if(*bKillThread == true)
			{
				AfxEndThread(1);
			}
			if(pElev[nIndex] !=missing_value)
			{
				pRelzFilepit_row[nIndex] = nPitRow;
				pRelzFilepit_col[nIndex] = nPitCol;
				pRelzFilepit_elev[nIndex] = dPitElev;
				if(pElev[nIndex] > dPitElev)
				{
					pz2pit[nIndex] = pElev[nIndex] - dPitElev;
				}
			}
			nLoop = nLoop + 1;
			if(nLoop == nNumRec)
			{
				bExit = true;
			}
			else
			{
				nIndex = pSortedIndex[nLoop];
				if(pShedNo[nIndex] != this_shed)
				{
					bExit = true;
					nLoop --;

				}
			}

		}


	}	
	pProgressBar2->StepIt();
	PondFile.Empty();
	delete pSortedIndex;
}

void CFormProcess::go_ridge3(int* pSortedIndex)
{
	int nColumn;

	CString* sMessage = new CString;
	*sMessage = "Opening file: " + strDirectory + str3CharName + "idem.dbf";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);	

	if(IDemFile.OpenFile(strDirectory + str3CharName + "idem.dbf",pProgressBar1,bKillThread) ==false)
	{
		AfxMessageBox("Could not open file " + strDirectory + str3CharName + "idem.dbf");
		AfxEndThread(0);
	}
	pProgressBar2->StepIt();
	*sMessage = "Running go_ridge3 function..";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,1,(long) sMessage);		
	nColumn = IDemFile.FindField("Row");
	ASSERT(nColumn!=-1);
	int* pRow = (int*) IDemFile.ReturnPointer(nColumn);


	nColumn = IDemFile.FindField("Col");
	ASSERT(nColumn!=-1);
	int* pCol = (int*) IDemFile.ReturnPointer(nColumn);


	nColumn = IDemFile.FindField("DDir");
	ASSERT(nColumn !=-1);
	short int* pDDir = (short int*)IDemFile.ReturnPointer(nColumn);


	nColumn = IDemFile.FindField("Drec");
	ASSERT(nColumn!=-1);
	int* pDrec = (int*) IDemFile.ReturnPointer(nColumn);

	


	nColumn = IDemFile.FindField("ShedNow");
	ASSERT(nColumn !=-1);
	int* pShedNow = (int*) IDemFile.ReturnPointer(nColumn);
	


	nColumn = IDemFile.FindField("Upslope");
	ASSERT(nColumn !=-1);
	int* pUpslope = (int*) IDemFile.ReturnPointer(nColumn);

	nColumn = IDemFile.FindField("Elev");
	ASSERT(nColumn !=-1);
	double* pElev = (double*) IDemFile.ReturnPointer(nColumn);

	
	nColumn = RelzFile.FindField("z2cr");
	ASSERT(nColumn !=-1);
	double* pz2cr = (double*)RelzFile.ReturnPointer(nColumn);


	

	nColumn = RelzFile.FindField("cr_row");
	ASSERT(nColumn!=-1);
	short int* pcr_row = (short int*) RelzFile.ReturnPointer(nColumn);

	nColumn = RelzFile.FindField("cr_col");
	ASSERT(nColumn!=-1);
	short int* pcr_col = (short int*)RelzFile.ReturnPointer(nColumn);

	nColumn = RelzFile.FindField("cr_elev");
	ASSERT(nColumn!=-1);
	double* pcr_elev = (double*)RelzFile.ReturnPointer(nColumn);



	nColumn = RelzFile.FindField("n2cr");
	ASSERT(nColumn!=-1);
	short int* pn2cr = (short int*) RelzFile.ReturnPointer(nColumn);




	
	int nNumRec = IDemFile.GetNumRecords();

	
	//unsigned _int64 * pSortValue = new unsigned _int64[nNumRec];
	//int* pSortedIndex = new int[nNumRec];
	int nLoop=0;

	
	/*
	for(nLoop=0;nLoop<nNumRec;nLoop++)
	{
		pSortValue[nLoop] = pShedNow[nLoop] * 1000000000000000 + ((100000 - pElev[nLoop]) * 100000000000) + pUpslope[nLoop];
		pSortedIndex[nLoop] = nLoop;
	}

	QuickSort(pSortValue,nNumRec,pSortedIndex);
	*/
	//delete pSortValue;
	double cr_elev = 0;

	bool go_dn = false;
	
	int num_dn = 0;

	int nIndex = 0;
	int start_rec = 0;
	double extra_z =0;
	int cr_row = 0;
	int cr_col = 0;
	int chan_cell = 0;
	int extran = 0;
	int current_n = 0;
	//double cr_elev = 0;
	for(nLoop=0;nLoop<nNumRec;nLoop++)
	{

		if(*bKillThread == true)
		{
			AfxEndThread(1);
		}

		//nIndex = pSortedIndex[nLoop];	
		nIndex = nLoop;
		if(pElev[nIndex] != missing_value)
		{
			start_rec = nIndex;
			num_dn = 0;
			if(pz2cr[nIndex]>0 || pUpslope[nIndex] >= dDSArea)
			{
				if(pUpslope[nIndex] >=dDSArea && pz2cr[nIndex] ==0)
				{
					cr_row = pRow[nIndex];
					cr_col = pCol[nIndex];
					cr_elev = pElev[nIndex];
					pz2cr[nIndex] = 0;
					

					pcr_elev[nIndex] = cr_elev;
					pcr_row[nIndex] = cr_row;
					pcr_col[nIndex] = cr_col;
					pn2cr[nIndex] = 0;


				}
				go_dn = false;
			}
			else
			{
				go_dn = true;
			}
			
			extran = 0;
			chan_cell = 0;
			while(go_dn == true)
			{
				if(*bKillThread == true)
				{
					AfxEndThread(1);
				}


				if(pz2cr[nIndex] ==0 && pUpslope[nIndex] < dDSArea)
				{
					if(pDDir[nIndex] !=5)
					{
						num_dn = num_dn + 1;
						nIndex = pDrec[nIndex] -1;	 
					}
					else
					{
						go_dn = false;
						extran = 0;

						cr_row = pRow[nIndex];
						cr_col = pCol[nIndex];
						cr_elev = pElev[nIndex];
						pz2cr[nIndex] = 0;
						pn2cr[nIndex] = 0;

						pcr_row[nIndex] = cr_row;
						pcr_col[nIndex] = cr_col;
						pcr_elev[nIndex] = cr_elev;
					}
				}
				else
				{
					if(pUpslope[nIndex] >= dDSArea)
					{
						extran =  0;
						cr_elev = pElev[nIndex];
						cr_row = pRow[nIndex];
						cr_col = pCol[nIndex];
						pz2cr[nIndex] = 0;
						pn2cr[nIndex] = 0;
						pcr_row[nIndex] = cr_row;
						pcr_col[nIndex] = cr_col;
						pcr_elev[nIndex] = cr_elev;
					}
					else
					{
						extran = pn2cr[nIndex];
						cr_elev = pcr_elev[nIndex];
						cr_row = pcr_row[nIndex];
						cr_col = pcr_col[nIndex];

					}
					go_dn = false;
				}

			}


			if(num_dn > 0)
			{
				current_n = num_dn + extran;
				nIndex = start_rec;
				for(int j=0;j<num_dn;j++)
				{
					if(pElev[nIndex] > cr_elev)
					{
						pz2cr[nIndex] = pElev[nIndex]  - cr_elev;
						pcr_elev[nIndex] = cr_elev;
						pcr_row[nIndex] = cr_row;
						pcr_col[nIndex] = cr_col;

					}
					else
					{
						pz2cr[nIndex] = 0;
						pcr_elev[nIndex] = cr_elev;
						pcr_row[nIndex] = cr_row;
						pcr_col[nIndex] = cr_col;
					}
					pn2cr[nIndex] = current_n;
					nIndex = pDrec[nIndex] -1;
					current_n = current_n -1;
				}
			}


			nIndex  = start_rec;



		}


	}

}

void CFormProcess::go_peak3(int* pSortedIndex)
{
	int nColumn = 0;

	nColumn = RelzFile.FindField("z2peak");
	ASSERT(nColumn!=-1);
	double* pz2peak = (double*) RelzFile.ReturnPointer(nColumn);

	nColumn = RelzFile.FindField("n2peak");
	ASSERT(nColumn!=-1);
	short int* pn2peak = (short int*) RelzFile.ReturnPointer(nColumn);
	

	nColumn = RelzFile.FindField("pk_row");
	ASSERT(nColumn!=-1);
	short int* ppk_row = (short int*) RelzFile.ReturnPointer(nColumn);

	nColumn = RelzFile.FindField("pk_col");
	ASSERT(nColumn!=-1);
	short int* ppk_col = (short int*) RelzFile.ReturnPointer(nColumn);


	nColumn = RelzFile.FindField("pk_elev");
	ASSERT(nColumn !=-1);
	double * ppk_elev = (double*) RelzFile.ReturnPointer(nColumn);

	nColumn = IDemFile.FindField("DDir");
	ASSERT(nColumn !=-1);
	short int* pDDir = (short int*)IDemFile.ReturnPointer(nColumn);


	nColumn = IDemFile.FindField("Drec");
	ASSERT(nColumn!=-1);
	int* pDrec = (int*) IDemFile.ReturnPointer(nColumn);


	nColumn = IDemFile.FindField("Row");
	ASSERT(nColumn !=-1);
	int* pRow = (int*) IDemFile.ReturnPointer(nColumn);


	nColumn = IDemFile.FindField("Col");
	ASSERT(nColumn !=-1);
	int* pCol = (int*) IDemFile.ReturnPointer(nColumn);




	


	nColumn = IDemFile.FindField("ShedNow");
	ASSERT(nColumn !=-1);
	int* pShedNow = (int*) IDemFile.ReturnPointer(nColumn);
	


	nColumn = IDemFile.FindField("Upslope");
	ASSERT(nColumn !=-1);
	int* pUpslope = (int*) IDemFile.ReturnPointer(nColumn);

	nColumn = IDemFile.FindField("Elev");
	ASSERT(nColumn !=-1);
	double* pElev = (double*) IDemFile.ReturnPointer(nColumn);

	int nNumRec = IDemFile.GetNumRecords();

	/*
	long double * pSortValue = new long double[nNumRec];
	int* pSortedIndex = new int[nNumRec];
	int nLoop=0;



	for(nLoop=0;nLoop<nNumRec;nLoop++)
	{
		pSortValue[nLoop] = pShedNow[nLoop] * 1000000000000000 + ((100000 - pElev[nLoop]) * 1000000000000) + pUpslope[nLoop];
		pSortedIndex[nLoop] = nLoop;
	}

	QuickSort(pSortValue,nNumRec,pSortedIndex);

	delete pSortValue;
	*/
	int start_rec = 0;
	int num_dn = 0;
	bool go_dn = false;
	double extra_z = 0;

	int extran = 0;
	int current_n = 0;
	
	double pk_elev = 0;
	int nIndex=0;

	int pk_row = 0;
	int pk_col = 0;

	int nLoop =0;


	for(nLoop=0;nLoop<nNumRec;nLoop++)
	{
		if(*bKillThread == true)
		{
			AfxEndThread(1);
		}
		//nIndex = pSortedIndex[nLoop];
		nIndex = nLoop;
		if(pElev[nIndex]!=missing_value)
		{
			start_rec = nIndex;
			num_dn = 0;
			if(pz2peak[nIndex] > 0 || pDDir[nIndex] == 5)
			{
				if(pDDir[nIndex]==5)
				{
					pk_row = pRow[nIndex];
					pk_col = pCol[nIndex];
					pk_elev = pElev[nIndex];



					pz2peak[nIndex] = 0;
					pn2peak[nIndex] = 0;
				}
				go_dn = false;
			}
			else
			{
				go_dn = true;
			}
			extran = 0;


			while(go_dn)
			{
				if(pz2peak[nIndex] == 0)
				{
					if(pDDir[nIndex] !=5)
					{
						num_dn++;
						nIndex = pDrec[nIndex] -1;
					}
					else
					{
						go_dn = false;
						extran = 0;


						pk_elev = pElev[nIndex];
						pk_row = pRow[nIndex];
						pk_col = pCol[nIndex];

						ppk_elev[nIndex] = pk_elev;
						ppk_row[nIndex] = pk_row;
						ppk_col[nIndex] = pk_col;


						pz2peak[nIndex] = 0;
						pn2peak[nIndex] = 0;
					}
				}
				else
				{
					extran = pn2peak[nIndex];
					pk_row = ppk_row[nIndex];
					pk_col = ppk_col[nIndex];
					pk_elev = ppk_elev[nIndex];



					//pk_elev = pElev[nIndex] - extra_z;
					go_dn = false;
				}
			}
			
			if(num_dn >0)
			{
				current_n = num_dn + extran;
				nIndex = start_rec;
				for(int j=0;j<num_dn;j++)
				{
					if(pElev[nIndex] > pk_elev)
					{
						ppk_row[nIndex] = pk_row;
						ppk_col[nIndex] = pk_col;
						ppk_elev[nIndex] = pk_elev;

						pz2peak[nIndex] = pElev[nIndex] - pk_elev;
					}
					else
					{
						ppk_row[nIndex] = pk_row;
						ppk_col[nIndex] = pk_col;
						ppk_elev[nIndex] = pk_elev;
						pz2peak[nIndex] = 0;
					}
					pn2peak[nIndex] = current_n;

					nIndex = pDrec[nIndex] -1;
					current_n --;
				}
			}

		}
	}
	pProgressBar2->StepIt();
}

void CFormProcess::calcrelief3(int* pSortedIndex)
{
	int nColumn = 0;

	double min_elev = 99999999;
	double max_elev =-1;

	nColumn = DEMFile.FindField("Elev");
	ASSERT(nColumn!=-1);
	double* pElev = (double*) DEMFile.ReturnPointer(nColumn);
	
	int nNumRec = DEMFile.GetNumRecords();


	int nLoop = 0;
	for(nLoop=0;nLoop<nNumRec;nLoop++)
	{
		if (pElev[nLoop] > max_elev)
		{
			max_elev = pElev[nLoop];
		}

		if(pElev[nLoop] < min_elev)
		{
			if(pElev[nLoop] != missing_value)
			{
				min_elev = pElev[nLoop];
			}
		}
	}


	double elev_range = max_elev - min_elev;

	nColumn = DEMFile.FindField("Shednow");
	ASSERT(nColumn!=-1);
	int* pShedNow = (int*) DEMFile.ReturnPointer(nColumn);


	nColumn = DEMFile.FindField("Upslope");
	ASSERT(nColumn!=-1);
	int* pUpslope = (int*) DEMFile.ReturnPointer(nColumn);

	/*
	long double * pSortValue = new long double[nNumRec];
	int* pSortedIndex = new int[nNumRec];




	for(nLoop=0;nLoop<nNumRec;nLoop++)
	{
		pSortValue[nLoop] =   (pShedNow[nLoop] * 1000000000000000 + ((100000 - pElev[nLoop]) * 1000000000000) + pUpslope[nLoop]);
		pSortedIndex[nLoop] = nLoop;
	}



	QuickSort(pSortValue,nNumRec,pSortedIndex);



	delete pSortValue;
	*/

	int nIndex = 0;


	int nStart = 0;
	for(nLoop=0;nLoop<nNumRec;nLoop++)
	{
		if(pShedNow[pSortedIndex[nLoop]]>0)
		{
			nStart = nLoop;
			nLoop = nNumRec;
		}		
	}
	

	nColumn = RelzFile.FindField("zpit2peak");
	ASSERT(nColumn !=-1);
	double* pzpit2peak = (double*) RelzFile.ReturnPointer(nColumn);

	nColumn = RelzFile.FindField("z2pit");
	ASSERT(nColumn !=-1);
	double* pz2pit = (double*) RelzFile.ReturnPointer(nColumn);


	nColumn = RelzFile.FindField("z2peak");
	ASSERT(nColumn !=-1);
	double* pz2peak = (double*) RelzFile.ReturnPointer(nColumn);


	nColumn = RelzFile.FindField("z2st");
	ASSERT(nColumn !=-1);
	double* pz2st = (double*) RelzFile.ReturnPointer(nColumn);

	nColumn = RelzFile.FindField("z2cr");
	ASSERT(nColumn !=-1);
	double* pz2cr = (double*) RelzFile.ReturnPointer(nColumn);


	nColumn = RelzFile.FindField("z2top");
	ASSERT(nColumn !=-1);
	double* pz2top = (double*) RelzFile.ReturnPointer(nColumn);


	nColumn = RelzFile.FindField("zcr2st");
	ASSERT(nColumn !=-1);
	double* pzcr2st = (double*) RelzFile.ReturnPointer(nColumn);


	nColumn = RelzFile.FindField("ztop2pit");
	ASSERT(nColumn !=-1);
	double* pztop2pit = (double*) RelzFile.ReturnPointer(nColumn);



	nColumn = RelzFile.FindField("pctz2top");
	ASSERT(nColumn !=-1);
	short int* ppctz2top = (short int*) RelzFile.ReturnPointer(nColumn);

	nColumn = RelzFile.FindField("pctz2st");
	ASSERT(nColumn !=-1);
	short int* ppctz2st = (short int*) RelzFile.ReturnPointer(nColumn);

	nColumn = RelzFile.FindField("pctz2pit");
	ASSERT(nColumn !=-1);
	short int* ppctz2pit = (short int*) RelzFile.ReturnPointer(nColumn);


	nColumn = RelzFile.FindField("pmin2max");
	ASSERT(nColumn !=-1);
	short int* ppmin2max = (short int*) RelzFile.ReturnPointer(nColumn);


	
	nColumn = RelzFile.FindField("cr_elev");
	ASSERT(nColumn!=-1);
	double * pcr_elev = (double*) RelzFile.ReturnPointer(nColumn);


	nColumn = RelzFile.FindField("pk_elev");
	ASSERT(nColumn!=-1);
	double * ppk_elev = (double*) RelzFile.ReturnPointer(nColumn);


	
	nColumn = RelzFile.FindField("ncr2st");
	ASSERT(nColumn !=-1);
	double* pncr2st = (double*) RelzFile.ReturnPointer(nColumn);


	nColumn = RelzFile.FindField("zcr2pit");
	ASSERT(nColumn !=-1);
	double* pzcr2pit = (double*) RelzFile.ReturnPointer(nColumn);


	

	nColumn = RelzFile.FindField("n2st");
	ASSERT(nColumn !=-1);
	short int* pn2st = (short int*) RelzFile.ReturnPointer(nColumn);

	nColumn = RelzFile.FindField("n2cr");
	ASSERT(nColumn !=-1);
	short int* pn2cr = (short int*) RelzFile.ReturnPointer(nColumn);


	nColumn = RelzFile.FindField("pctn2st");
	ASSERT(nColumn !=-1);
	short int* ppctn2st = (short int*) RelzFile.ReturnPointer(nColumn);


	nColumn = RelzFile.FindField("pmin2max");
	ASSERT(nColumn !=-1);
	//short int* ppmin2max = (short int*) RelzFile.ReturnPointer(nColumn);

	
	
	
	for(nLoop=0;nLoop<nNumRec;nLoop++)
	{
		if(pElev[nLoop] !=missing_value)
		{
			pcr_elev[nLoop] = max_elev - pcr_elev[nLoop];
			ppk_elev[nLoop] = max_elev - ppk_elev[nLoop];
		}
		else
		{
			pcr_elev[nLoop] = 0;
			ppk_elev[nLoop] = 0;
		}
	}
	

	

	int this_shed = 0;
	double top_elev = 0;
	bool go_dn = false;
	for(nLoop=nStart;nLoop<nNumRec;nLoop++)
	{


		if(*bKillThread == true)
		{
			AfxEndThread(1);
		}
		nIndex = pSortedIndex[nLoop];
		//nIndex = nLoop;

		this_shed = pShedNow[nIndex];
		top_elev = pElev[nIndex];
		

		if(pElev[nIndex] == missing_value)
		{
			//nLoop = nNumRec;
		}
		else
		{

			go_dn = true;
			while(go_dn == true)
			{

				if(*bKillThread == true)
				{
					AfxEndThread(1);
				}

				pzpit2peak[nIndex] = pz2pit[nIndex] + pz2peak[nIndex];
				pzcr2st[nIndex] = pz2cr[nIndex] + pz2st[nIndex];
				pzcr2pit[nIndex] = pz2cr[nIndex] + pz2pit[nIndex];
				pz2top[nIndex] = top_elev - pElev[nIndex];
				pztop2pit[nIndex] = pz2top[nIndex] + pz2pit[nIndex];
				pncr2st[nIndex] = pn2cr[nIndex] + pn2st[nIndex];


				
				if(pztop2pit[nIndex] > 0)
				{
					ppctz2top[nIndex] = (short int) ( 100 -  (pz2top[nIndex] /pztop2pit[nIndex])*100);
				}
				else
				{
					ppctz2top[nIndex] = 0;
				}


				if(pzcr2st[nIndex] >0)
				{
					ppctz2st[nIndex] =  (short int) ((pz2st[nIndex]/pzcr2st[nIndex])*100);
				}
				else
				{
					ppctz2st[nIndex] = 0;
				}

				

				if(pzpit2peak[nIndex]>0)
				{
					ppctz2pit[nIndex] =  (short int) ( ((pz2pit[nIndex] / pzpit2peak[nIndex])*100));
				}
				else
				{
					ppctz2pit[nIndex] = 0;
				}
				
				if(pncr2st[nIndex] > 0)
				{
					ppctn2st[nIndex] = (short int) ((pn2st[nIndex] / pncr2st[nIndex]) *100);
				}
				else
				{
					ppctn2st[nIndex] = 0;
				}

				ppmin2max[nIndex] =  (short int) ( ((pElev[nIndex] - min_elev) / elev_range)*100);
				///REPLACE b.pmin2max WITH INT(((elev - min_elev)/ elev_range)*100)











				nLoop++;

				if(nLoop<nNumRec)
				{
					nIndex = pSortedIndex[nLoop];

					if(pShedNow[nIndex] != this_shed)
					{
						go_dn = false;
						nLoop--;
					}
				}
				else
				{
					go_dn = false;
				}
			}
		}
	}
		pProgressBar2->StepIt();
}

void CFormProcess::calc_len3()
{

	CDbfFile LenFile;
	LenFile.AddField("SeqNo",'I',9,0);
	LenFile.AddField("L2PIT",'D',8,1);
	LenFile.AddField("L2PEAK",'D',8,1);
	LenFile.AddField("LPIT2PEAK",'D',8,1);
	LenFile.AddField("PPIT2PEAKL",'S',3,0);
	LenFile.AddField("L2STR",'D',8,1);
	LenFile.AddField("L2DIV",'D',8,1);
	LenFile.AddField("LSTR2DIV",'D',8,1);
	LenFile.AddField("PSTR2DIVL",'S',3,0);
	int nNumRec = DEMFile.GetNumRecords();
	LenFile.SetInitialFileSize(nNumRec);


	int nColumn = 0;
	nColumn = LenFile.FindField("SeqNo");
	ASSERT(nColumn !=-1);
	int* pLenFileSeqNo = (int*) LenFile.ReturnPointer(nColumn);
	
	nColumn = LenFile.FindField("l2pit");
	ASSERT(nColumn!=-1);
	double* pl2pit = (double*) LenFile.ReturnPointer(nColumn);

	nColumn = LenFile.FindField("l2peak");
	ASSERT(nColumn!=-1);
	double* pl2peak = (double*) LenFile.ReturnPointer(nColumn);		

	nColumn = LenFile.FindField("LPIT2PEAK");
	ASSERT(nColumn!=-1);
	double* plpit2peak = (double*) LenFile.ReturnPointer(nColumn);

	nColumn = LenFile.FindField("PPIT2PEAKL");
	ASSERT(nColumn !=-1);
	short int* pppit2peakl = (short int*) LenFile.ReturnPointer(nColumn);

	nColumn = LenFile.FindField("L2STR");
	ASSERT(nColumn !=-1);
	double* pl2str = (double*) LenFile.ReturnPointer(nColumn);

	nColumn = LenFile.FindField("L2DIV");
	ASSERT(nColumn !=-1);
	double* pl2div = (double*) LenFile.ReturnPointer(nColumn);

	nColumn = LenFile.FindField("lstr2div");
	ASSERT(nColumn!=-1);
	double* plstr2div = (double*) LenFile.ReturnPointer(nColumn);


	nColumn = LenFile.FindField("pstr2divl");
	ASSERT(nColumn !=-1);
	short int* ppstr2divl = (short int*) LenFile.ReturnPointer(nColumn);





	//This is allocates memory for each field, for this size.


	int nLoop=0;
	

	nColumn = RelzFile.FindField("cr_row");
	ASSERT(nColumn!=-1);
	short int* pcr_row = (short int*) RelzFile.ReturnPointer(nColumn);

	nColumn = RelzFile.FindField("cr_col");
	ASSERT(nColumn!=-1);
	short int* pcr_col = (short int*) RelzFile.ReturnPointer(nColumn);


	nColumn = RelzFile.FindField("SeqNo");
	ASSERT(nColumn!=-1);
	int* pRelzFileSeqNo = (int*)RelzFile.ReturnPointer(nColumn);

	nColumn = RelzFile.FindField("st_row");
	ASSERT(nColumn!=-1);
	short int* pst_row = (short int*) RelzFile.ReturnPointer(nColumn);

	nColumn = RelzFile.FindField("st_col");
	ASSERT(nColumn!=-1);
	short int* pst_col = (short int*) RelzFile.ReturnPointer(nColumn);


	
	nColumn = RelzFile.FindField("pk_col");
	ASSERT(nColumn !=-1);
	short int* ppk_col = (short int*) RelzFile.ReturnPointer(nColumn);

	nColumn = RelzFile.FindField("pk_row");
	ASSERT(nColumn !=-1);
	short int* ppk_row = (short int*) RelzFile.ReturnPointer(nColumn);


	nColumn = RelzFile.FindField("z2pit");
	ASSERT(nColumn !=-1);
	double* pz2pit = (double*) RelzFile.ReturnPointer(nColumn);

	nColumn = RelzFile.FindField("z2peak");
	ASSERT(nColumn !=-1);
	double* pz2peak = (double*) RelzFile.ReturnPointer(nColumn);


	nColumn = RelzFile.FindField("pit_row");
	ASSERT(nColumn!=-1);
	short int* ppit_row = (short int*) RelzFile.ReturnPointer(nColumn);


	nColumn = RelzFile.FindField("pit_col");
	ASSERT(nColumn!=-1);
	short int* ppit_col = (short int*) RelzFile.ReturnPointer(nColumn);


	nColumn = RelzFile.FindField("z2st");
	ASSERT(nColumn!=-1);
	double* pz2st = (double*) RelzFile.ReturnPointer(nColumn);



	int lastrec = nNumRec ;
	int a_seqno = pRelzFileSeqNo[nNumRec-1];
	//here we actually add records to the table objects, so we can use it in the future.
	for(nLoop=0;nLoop<nNumRec;nLoop++)
	{
		LenFile.AddRecord();
		pLenFileSeqNo[nLoop] = pRelzFileSeqNo[nLoop];

	}
	

	nColumn = DEMFile.FindField("Elev");
	ASSERT(nColumn!=-1);
	double* pDEMFileElev = (double*) DEMFile.ReturnPointer(nColumn);

	nColumn = DEMFile.FindField("Row");
	ASSERT(nColumn !=-1);
	int* pRow = (int*) DEMFile.ReturnPointer(nColumn);

	nColumn = DEMFile.FindField("Col");
	ASSERT(nColumn !=-1);
	int* pCol = (int*) DEMFile.ReturnPointer(nColumn);
	
	


	int thisrow = 0;
	int thiscol = 0;
	
	double len2pit=0;
	double len2peak = 0;
	double pit2peakl = 0;
	double len2str = 0;
	double len2div = 0;
	double str2divl = 0;
	for(nLoop = 0;nLoop<nNumRec;nLoop++)
	{
		if(*bKillThread == true)
		{
			AfxEndThread(1);
		}
		if(pDEMFileElev[nLoop] != missing_value)
		{	
			thisrow = pRow[nLoop];
			thiscol = pCol[nLoop];
			
			if(thisrow == ppit_row[nLoop] && thiscol ==ppit_col[nLoop])
			{
				len2pit =0;
			}
			else
			{
				len2pit = (sqrt((double)(ppit_col[nLoop] - thiscol)*(ppit_col[nLoop] - thiscol)+(ppit_row[nLoop]-thisrow)*(ppit_row[nLoop]-thisrow))) * dGridSize;
				len2pit = sqrt(len2pit * len2pit+ (pz2pit[nLoop]*dGridSize)*(pz2pit[nLoop]*dGridSize));
			}

			if(thisrow== ppk_row[nLoop] && thiscol == ppk_col[nLoop])
			{
				len2peak = 0;
			}
			else
			{
				len2peak  = sqrt((double)(ppk_col[nLoop] - thiscol)* (ppk_col[nLoop] - thiscol) + (ppk_row[nLoop] - thisrow) * (ppk_row[nLoop] - thisrow)) * dGridSize;
				len2peak = sqrt(len2peak * len2peak + (pz2peak[nLoop]*dGridSize)*(pz2peak[nLoop]*dGridSize));
			}

			pit2peakl = len2peak + len2pit;
			pl2pit[nLoop] = len2pit;
			pl2peak[nLoop] = len2peak;
			plpit2peak[nLoop] = pit2peakl;

			if(pit2peakl>0)
			{
				pppit2peakl[nLoop] = (short int)((len2pit/pit2peakl)*100);
			}
			else
			{
				pppit2peakl[nLoop] = 0;
			}

			if(thisrow == pst_row[nLoop] && thiscol == pst_col[nLoop])
			{
				len2str = 0;
			}
			else
			{
				len2str = (sqrt((double)(pst_col[nLoop] - thiscol) * (pst_col[nLoop] - thiscol) + (pst_row[nLoop] - thisrow)* (pst_row[nLoop] - thisrow))) * dGridSize;
				len2str = sqrt((len2str * len2str) + (pz2st[nLoop] * dGridSize) *(pz2st[nLoop] * dGridSize));
			}


			if(thisrow == pcr_row[nLoop] && thiscol == pcr_col[nLoop])
			{
				len2div = 0;
			}
			else
			{
				len2div = sqrt((double)(pcr_col[nLoop] - thiscol) * (pcr_col[nLoop] - thiscol) + (pcr_row[nLoop] - thisrow) * (pcr_row[nLoop] - thisrow)) * dGridSize;
				len2div = sqrt(len2div * len2div + (pz2peak[nLoop] * dGridSize) * (pz2peak[nLoop] * dGridSize));
			}
			
			str2divl = len2str + len2div;

			pl2str[nLoop] = len2str;
			pl2div[nLoop] = len2div;
			plstr2div[nLoop] = str2divl;
			if(str2divl>0)
			{
				ppstr2divl[nLoop] = (short int)((len2str/str2divl) * 100);
			}
			else
			{
				ppstr2divl[nLoop] = 0;
			}

		}
		else
		{
			pl2pit[nLoop] = 0;
			pl2peak[nLoop] = 0;
			plpit2peak[nLoop] =0;
			pppit2peakl[nLoop] =0 ;
			pl2str[nLoop] = 0;
			pl2div[nLoop] = 0;
			plstr2div[nLoop] = 0;
			ppstr2divl[nLoop] = 0;

		}

	}
	pProgressBar2->StepIt();
	CString* sMessage = new CString;
	*sMessage = "Saving file " + strDirectory + str3CharName + "Len.dbf";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,1,(long) sMessage);
	LenFile.SaveToFile(strDirectory + str3CharName + "Len.dbf",pProgressBar1, bKillThread);
	LenFile.Empty();
	pProgressBar2->StepIt();
}
