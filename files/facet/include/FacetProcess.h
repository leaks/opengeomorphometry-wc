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
// FacetProcess.h: interface for the CFacetProcess class.
//
//////////////////////////////////////////////////////////////////////
#include "facetDlg.h"
#if !defined(AFX_FACETPROCESS_H__EB3E515E_3499_4874_9C21_5F54D6154E12__INCLUDED_)
#define AFX_FACETPROCESS_H__EB3E515E_3499_4874_9C21_5F54D6154E12__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "facetDlg.h"
#include "DbfFile.h"


#include <assert.h>

class CFacetProcess  
{
public:
	bool* pbKillThread;
	__forceinline void CALC_CLASS3(CDbfFile *FormFile, int no_models, int nIndex, 
							   CString* FieldType, long** infield, int* model,
							   double* B, double* b_low, double* b_hi, double* b1,
							   double* b2, double *D, CString* outattr, double* Max_Code,
							   CString* Max_Class, double* Max_Value, double* attr_wt, 
							   CString* fuzz_attr,int* no_facets,CString* Facet_Name, 
							   int* Facet_Code, int* fuzz_attrIndex, int* no_attrib);
	__forceinline void GET_CRULE3(CDbfFile *CRuleFile, CString *facet_name, 
							   CString* fuzz_attr, int *facet_code, double *attr_wt, 
							   int *no_attrib, int *total_wt, int* no_facets);
	__forceinline void GET_ARULE3(CDbfFile* ARuleFile, int *no_models, long **infield,
							   CString *outattr, int *model, double *b, double *b_low, 
							   double *b_hi, double *b1, double *b2, double *D, CString* infieldType);
	CDbfFile LSMFile;
	void CALC_LSM(CPtrArray* pArray);
	__forceinline void CALC_CLASS(CDbfFile* FormFile, int no_models, int nIndex, CString* FieldType, 
		long** infield, int* model, double* B, double* b_low, double* b_hi, double* b1,
		double* b2, double *D, CString* outattr,int nSW_Aspect,
							   int nNE_Aspect, int nSteep,int nSlopeLt20,double* Max_Code, CString* Max_Class,
							   double* Max_Value, double* attr_wt, CString* fuzz_attr,
							   int* no_facets,CString* Facet_Name, int* Facet_Code,
							   int* fuzz_attrIndex,int* no_attrib);

	__forceinline  void GET_CRULE2(CDbfFile* CRuleFile,CString* facet_name, CString* fuzz_attr, 
		int* facet_code,double* attr_wt, int* no_attrib,int* total_wt, int* no_facets);
	CDbfFile GeoFile;
	__forceinline  void GET_ARULE2(CDbfFile* ARuleFile, int *no_models, long **infield, 
		CString *outattr, int *model, double *b, double *b_low, double *b_hi,
		double *b1, double *b2, double *D, CString* infieldType);
	CDbfFile DSSFile;
	void CALC_DSS(CPtrArray* pArray);
	CDbfFile CRuleFile;
	void LSM_MAXC(CPtrArray* pArray);
	CDbfFile FuzcFile;
	//CDbfFile CRuleFile;
	//int nOutField;
	//int nInField;
	bool same_size;
	int c_seqno;
	int b_seqno;
	int a_seqno;
	int nARuleFileSize;
	void Make_AFile();
	bool ReadARuleFile(CPtrArray* pArray);
	void LSM_FUZA(CPtrArray* pArray);
	void LSM_FUZC(CPtrArray* pArray);
	CFacetProcess();
	virtual ~CFacetProcess();
	struct ARule
	{
		int Sortorder;
		char File_in [25];
		char Attr_in [25];
		char Class_out [25];
		short Model_no;
		float B;
		float B_low;
		float B_hi;
		float B1;
		float B2;
		float D;
	};
	CDbfFile FuzaFile;
	CDbfFile relzfile;
	CDbfFile FormFile;
	CDbfFile DEMFile;
	ARule* pARule;
	CWnd* pWndNotifyProgress;
	
	//FuzaFile.

};

#endif // !defined(AFX_FACETPROCESS_H__EB3E515E_3499_4874_9C21_5F54D6154E12__INCLUDED_)
