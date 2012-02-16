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
// DbfStructure.cpp: implementation of the CDbfStructure class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "facet.h"
#include "DbfStructure.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDbfStructure::CDbfStructure(int nNumOfColumns, int nInitialSize)
{
	nArraySize = 0;
	nColumnNumber = 0;
	nRowNumber = 0;
	pColumnNames = 0;
	pValue = 0;

}

CDbfStructure::~CDbfStructure()
{

	

}
