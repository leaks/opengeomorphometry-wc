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
// DbfStructure.h: interface for the CDbfStructure class.
//
//////////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBFSTRUCTURE_H__567898B3_DB97_11D6_9B60_0080C8EC880F__INCLUDED_)
#define AFX_DBFSTRUCTURE_H__567898B3_DB97_11D6_9B60_0080C8EC880F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDbfStructure  
{
public:

	CDbfStructure(int nNumOfColumns, int nInitialSize = 20);
	virtual ~CDbfStructure();

protected:
	int nArraySize;
	int nRowNumber;
	int nColumnNumber;
	long** pValue;
	CString* pColumnNames;
};

#endif // !defined(AFX_DBFSTRUCTURE_H__567898B3_DB97_11D6_9B60_0080C8EC880F__INCLUDED_)
