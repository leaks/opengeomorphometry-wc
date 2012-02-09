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
// DbfFile.h: interface for the CDbfFile class.
//
//////////////////////////////////////////////////////////////////////
#include <afxdao.h>
#if !defined(AFX_DBFFILE_H__04117EC1_6207_11D6_A2C4_005056C00001__INCLUDED_)
#define AFX_DBFFILE_H__04117EC1_6207_11D6_A2C4_005056C00001__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CDbfFile  
{
public:
	bool AddField(CString cszFieldName, char szFieldType, int nSize, int nNumDec);
	bool* pbKillThread;
	int GetNumFields();
	void SetInitialFileSize(int nFileSize);
	void Empty();
	int nNumOfDec[255];
	int nFieldSize[255];
	bool SaveToFile(CString strFileName, CProgressCtrl* pProgressBar, bool* pbKillThread);
	long* ReturnPointer(int nColumn);
	CString GetFieldName(int nFieldIndex);
	bool SetValue(int nColumnNum, int nRowNum, bool bValue);

	void AddRecord();
	int GetNumRecords();
	bool SetValue(int nColumnNum, int nRowNum, short int nValue);
	bool SetValue(int nColumnNum, int nRowNum, float fValue);
	bool SetValue(int nColumnNum, int nRowNum, double dValue);
	bool SetValue(int nColumnNum, int nRowNum, CString sValue);
	bool SetValue(int nColumnNum, int nRowNum, int nValue);
bool AddField(CString cszFieldName, char szFieldType, int nSize);
	char ReturnType(int nColumn);
	long* ReturnValue(int nColumn, int nRow);
	int FindField(CString sFieldName);
	bool OpenFile(CString sDBFFileName, CProgressCtrl* p, bool* pKillThread);
	CDbfFile();
	virtual ~CDbfFile();
protected:
	short int nNumOfColumns;
	long nNumOfRows;
	CString* pColumnName;

	int nSizeOfFile;
	
protected:

public:
	void InitializeVariables();
	char CType[255];
    long** pColumnValue;

};

#endif // !defined(AFX_DBFFILE_H__04117EC1_6207_11D6_A2C4_005056C00001__INCLUDED_)
