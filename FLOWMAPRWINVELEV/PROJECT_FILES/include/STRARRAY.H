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
// StrArray.h: interface for the CStrArray class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STRARRAY_H__419C4780_496E_11D5_9AE2_0080C8EC880F__INCLUDED_)
#define AFX_STRARRAY_H__419C4780_496E_11D5_9AE2_0080C8EC880F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CStrArray  
{
public:
	void RemoveAll();
	int GetSize();
	char* GetAt(int nIndex);
	void Add(char* pString);
	CStrArray();
	virtual ~CStrArray();

protected:
	int nRecordsNumber;
	char** pArrayPointer;
};

#endif // !defined(AFX_STRARRAY_H__419C4780_496E_11D5_9AE2_0080C8EC880F__INCLUDED_)
