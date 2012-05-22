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
// Sort.cpp: implementation of the CSort class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
//#include "landmap.h"
#include "Sort.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


/*void CSort::build_list (ARY_LIST input, int *piDEMSort)
{
   for (int i = 0; i < n; i++)
   {
	   piDEMSort[i] = i;
       A[i] = input[i];
   }
}

//
//  debug_print ():
//    Print the array list A[n] for each iteration
//
void CSort::debug_print (int  iteration, int debug,
                        char *hdr, int *piDEMSort)
{
	FILE *hDebug;

	hDebug = fopen("debug.txt", "w");

	if (debug == 0)
		fprintf(hDebug, "\n %s \n", hdr);
	else
		fprintf(hDebug, " Pass #%d:", iteration + 1);
	for (int i = 0; i < n; i++)
		fprintf(hDebug, " %f", A[i]);
	fprintf(hDebug, "\n");
	for (i = 0; i < n; i++)
		fprintf(hDebug, " %d", piDEMSort[i]);
	fprintf(hDebug, "\n");

	fclose(hDebug);
}

//
//  qsort(): Perform quick sort 
//
void CSort::qsort (int First, int Last, int *piDEMSort)
{
	int			i;
	int			j;
	int			iTmp;
	DATA_TYPE	zTmp;

	for (i = First; i < Last; i++)
	{
		for (j = i + 1; j <= Last; j++)
		{
			if (A[i] >= A[j])
			{
				zTmp = A[i];
				A[i] = A[j];
				A[j] = zTmp;
				iTmp = piDEMSort[i];
				piDEMSort[i] = piDEMSort[j];
				piDEMSort[j] = iTmp;
			}
		}
	}*/
    /*  (First < Last) 
	{
		DATA_TYPE  Pivot = A[First];
		int        i = First;
		int        j = Last;
		int		 iTmp;	

		while (i < j) 
		{
			while (A[i] <= Pivot && i < Last)
            // continue increasing 'i'
				i += 1; 
			while (A[j] >= Pivot && j > First)
            // continue decreasing 'i'
				j -= 1;
			if (i < j) {  // Swap the Pivots
				DATA_TYPE swap_area = A[j];
				A[j] = A[i];
				A[i] = swap_area;
				iTmp = piDEMSort[j];
				piDEMSort[j] = piDEMSort[i];
				piDEMSort[i] = iTmp;
			} 
		}
      // Interchange A[j] & A[First]
		DATA_TYPE  swap_area = A[j];
		A[j]      = A[First];
		A[First]  = swap_area;
		iTmp = piDEMSort[j];
		piDEMSort[j] = piDEMSort[First];
		piDEMSort[First] = iTmp;

		debug_print (iter++, 1, "", piDEMSort);
		qsort (First, j - 1, piDEMSort);
		qsort (j + 1, Last, piDEMSort);
	}*/
/*}

void CSort::Quick_Sort (ARY_LIST input, int *piDEMSort)
{
    build_list(input, piDEMSort);  // Build array A
    //debug_print (0, 0,
    //  "List to be sorted in ascending order:", piDEMSort);
    if (n > 0) 
       qsort (0, n - 1, piDEMSort);
}

*/