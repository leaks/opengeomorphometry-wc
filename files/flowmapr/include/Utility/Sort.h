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
// Sort.h: interface for the CSort class.
//
//////////////////////////////////////////////////////////////////////

#ifndef SORT_H
#define SORT_H

//typedef	float	DATA_TYPE;
//////////////////////////////////////////////////////////////////////////
// QuickSort functions
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// QuickSortRecursive - core of algorithm, do not call it, use QuickSort,
// see below

template <class T> void QuickSortRecursive(T *pArr, int *piOrder, int d, int h, bool bAscending)
{

	int i,j;
	T str;

	i = h;
	j = d;

	str = pArr[((int) ((d+h) / 2))];

	do {

		if (bAscending) {
			while (pArr[j] < str) j++;
			while (pArr[i] > str) i--;
		} else {
			while (pArr[j] > str) j++;
			while (pArr[i] < str) i--;
		}

		if ( i >= j ) {

			if ( i != j ) {
				T zal;
				int iTmp;

				zal = pArr[i];
				pArr[i] = pArr[j];
				pArr[j] = zal;

				iTmp = piOrder[i];
				piOrder[i] = piOrder[j];
				piOrder[j] = iTmp;
			}

			i--;
			j++;
		}
	} while (j <= i);

	if (d < i) QuickSortRecursive(pArr,piOrder,d,i,bAscending);
	if (j < h) QuickSortRecursive(pArr,piOrder,j,h,bAscending);


}

//////////////////////////////////////////////////////////////////////////
// QuickSort - entry to algorithm
//
// T *pArr			... pointer of array to sort
// int iSize		... size of array T *pArr
// BOOL bAscending	... if bAscending == TRUE, then sort ascending,
//						otherwise descending
//
// return TRUE if no error, error can be bad parameter, call ::GetLastError()
// if QuickSort returned FALSE
template <class T> bool QuickSort(T *pArr, int iSize, int *piOrder, bool bAscending=true)
{
	/*
	int nTotalTime ;
	double dTotalTime=0.0; 
	clock_t start, finish;
	char buffer[14];
	char buffer2[14];
	ifstream fin2("fout2.txt");
	fin2.getline(buffer2,13);
	fin2.close();
	ifstream fin1("fout1.txt");
	fin1.getline(buffer,13);
	dTotalTime = atof(buffer);
	nTotalTime = (int) atof(buffer2);
	nTotalTime++;
	ofstream fout2("fout2.txt");
	fout2<<nTotalTime<<endl;
	fout2.close();
	fin1.close();
	start = clock();
	*/
	bool rc = true;
	int		i, j, k, l;
	T		tTmp;
	int		iTmp;

	if (iSize > 1) {

		try {

			int	low = 0,
				high = iSize - 1;

			QuickSortRecursive(pArr,piOrder,low,high,bAscending);

			j = 1;
			tTmp = pArr[0];
			for (i = 1; i < iSize; i++)
			{
				if (pArr[i] != tTmp)
				{
					for (k = i - j; k < i - 1; k++)
					{
						for (l = k + 1; l < i; l++)
						{
							if (piOrder[l] > piOrder[k])
							{
								iTmp = piOrder[l];
								piOrder[l] = piOrder[k];
								piOrder[k] = iTmp;
							}
						}
					}
					j = 1;
					tTmp = pArr[i];
				}
				else
					j++;
				
			}
			if (j > 1)
			{
				for (k = i - j; k < i - 1; k++)
				{
					for (l = k + 1; l < i; l++)
					{
						if (piOrder[l] > piOrder[k])
						{
							iTmp = piOrder[l];
							piOrder[l] = piOrder[k];
							piOrder[k] = iTmp;
						}
					}
				}
			}

		} catch (...) {
			::SetLastError(ERROR_INVALID_PARAMETER);
			rc = FALSE;
		}

	} else {
		::SetLastError(ERROR_INVALID_PARAMETER);
		rc = FALSE;
	}
	/*
	finish = clock();
	ofstream fout1("fout1.txt");
	if (((double)(finish - start) / CLOCKS_PER_SEC))
	{
		
		dTotalTime  = dTotalTime +  (double)(finish - start) / CLOCKS_PER_SEC;
	}
	else
	{
		dTotalTime  = dTotalTime +  0.0005;
	}

	
	fout1<<dTotalTime<<endl;
	fout1.close();
	*/
	return rc;
}

class CSort  
{
public:
    CSort () {}
    virtual ~CSort()  {}
    //void build_list (DATA_TYPE input[], int *iDEMSort);
    //void debug_print (int iteration, int debug, 
    //                  char *hdr, int *iDEMSort);
    //void qsort (int First, int Last, int *iDEMSort);
    //void Quick_Sort (DATA_TYPE input[], int *iDEMSort);

private:
    //typedef    int        DATA_TYPE;
    //typedef    DATA_TYPE  ARY_LIST[];
    //DATA_TYPE  *A;  // Array list A[]
    //int        n;   // size of A
    //int        iter;
};

#endif // SORT_H

