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
// SortD.h: interface for the CSortD class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SORTD_H__C8C683B1_06A8_11D5_8C24_0050568B6A3C__INCLUDED_)
#define AFX_SORTD_H__C8C683B1_06A8_11D5_8C24_0050568B6A3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef	double DATA_TYPE_D;

class CSortD  
{
public:

    CSortD (int size) {iter =0; A = new DATA_TYPE_D[n=size];}
    ~CSortD()  { delete []A; }
    void build_list (DATA_TYPE_D input[], int *iDEMSort);
    void debug_print (int iteration, int debug, 
                      char *hdr, int *iDEMSort);
    void qsort (int First, int Last, int *iDEMSort);
    void Quick_Sort (DATA_TYPE_D input[], int *iDEMSort);

private:
    //typedef    int        DATA_TYPE;
    typedef    DATA_TYPE_D  ARY_LIST[];
    DATA_TYPE_D  *A;  // Array list A[]
    int        n;   // size of A
    int        iter;
};

#endif // !defined(AFX_SORTD_H__C8C683B1_06A8_11D5_8C24_0050568B6A3C__INCLUDED_)
