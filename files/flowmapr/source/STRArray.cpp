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
// StrArray.cpp: implementation of the CStrArray class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "landmap.h"
#include "StrArray.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStrArray::CStrArray()
{
	pArrayPointer = new  char* [10];
	nRecordsNumber = 0;

}

CStrArray::~CStrArray()
{
	for(int nLoop=0;nLoop<nRecordsNumber;nLoop++)
	{
		delete pArrayPointer[nLoop];
	}
	delete pArrayPointer;

}

void CStrArray::Add(char *pString)
{
	if(nRecordsNumber%10==0 && nRecordsNumber!=0)
	{
		char** pTemp = new char*[nRecordsNumber + 10];
		//char* pTemp[255];
		for(int nLoop=0;nLoop<nRecordsNumber;nLoop++)
		{
			pTemp [nLoop] = pArrayPointer[nLoop];
			
		}
		delete pArrayPointer;
		pArrayPointer = pTemp;
	}

	pArrayPointer[nRecordsNumber] = pString;
	nRecordsNumber++;


}


char* CStrArray::GetAt(int nIndex)
{

	return pArrayPointer[nIndex];
}

int CStrArray::GetSize()
{
	return nRecordsNumber;

}


void CStrArray::RemoveAll()
{
	for(int nLoop=0;nLoop<nRecordsNumber;nLoop++)
	{
		delete pArrayPointer[nLoop];
	}
	delete pArrayPointer;
	pArrayPointer = new char* [10];
	nRecordsNumber=0;

}
